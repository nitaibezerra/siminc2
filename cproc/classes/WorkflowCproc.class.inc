<?php

/**
 * WorkflowCproc
 * Classe que objetiva organizar o funcionamento do workflow
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 */
class WorkflowCproc
{

	/**
	 * 
	 */
	public static function aplicaWorkflowNaPagina( Processo $Processo ){

		if( !$Processo->docid && $Processo->prcid ){

			$docid = wf_cadastrarDocumento( CPROC_GESTAODOCUMENTOSDISUP_TPDID, 'Documento Assistência Técnica' );
			$Processo->docid = $docid;
			
			$Processo->alterar();
			$Processo->commit();
		}

		if($Processo->prcid && $Processo->docid){ 
			wf_desenhaBarraNavegacao( $Processo->docid, array('docid'=>$Processo->docid) ); 
		}
	}

	/**
	 * Estado Documento: Em Cadastramento - CPROC
	 * Pré Ação de "Enviar para Análise do Procedimento pelo Coordenador"
	 *
	 * Atualiza status e fase do processo em suas 
	 * respectivas tabelas através de uma seleção
	 * (cproc.faseprocesso) (cproc.statusprocesso)
	 */
	public static function preAcaoEnviarParaAnaliseDoProcedimentoPeloCoordenador(){
		global $db;

		extract( $_POST );

		if( !$fasid )
			exit( simec_json_encode(array('boo' => false, 	'msg' => 'Fase não pode ficar em branco.')) );
		if( !$stsid )
			exit( simec_json_encode(array('boo' => false, 	'msg' => 'Status não pode ficar em branco.')) );
		if( !$docid )
			exit( simec_json_encode(array('boo' => false, 	'msg' => 'Houve um erro ao identificar processo, tente novamente.')) );
		if( $documento[0]['esdid'] == CPROC_EMCADASTRAMENTO_ESDID && !$coordenador )
			exit( simec_json_encode(array('boo' => false, 	'msg' => 'É necessário selecionar um Coordenador.')) );

		// associa fase
		$sql = " INSERT INTO cproc.faseprocesso ( fasid, prcid, fprdthratribuicao )
				 VALUES ( " . $fasid . ", (SELECT prcid FROM cproc.processo WHERE docid = '" . $docid . "'), NOW() ) ";
		$db->executar( $sql );

		// associa status
		$sql = " INSERT INTO cproc.statusprocesso ( stsid, prcid, stpdthratribuicao )
				 VALUES ( " . $stsid . ", (SELECT prcid FROM cproc.processo WHERE docid = '" . $docid . "'), NOW() ) ";
		$db->executar( $sql );

		// busca documento
		$sql = " SELECT *
				 FROM workflow.documento
				 WHERE docid = " . $docid;
		$documento = $db->carregar( $sql );

		// executa funcoes extras de acordo com estado do documento
		switch ( $documento[0]['esdid'] ) {
			case CPROC_EMCADASTRAMENTO_ESDID:

				WorkflowCproc::selecionarCoordenador();
				break;
			//case CPROC_ATUALIZACAODOPROCEDIMENTO_ESDID:

				//WorkflowCproc::selecionarTecnico();
				//break;

			case CPROC_IMPRESSAO_ASSINATURA_E_NUMERACAO_DO_DOCUMENTO_ESDID: 
				
				WorkflowCproc::registraPrazoManifestacao();
				break;
		}

		if( $db->commit() )
			exit( simec_json_encode(array('boo' => true, 	'msg' => '')) );
		else
			exit( simec_json_encode(array('boo' => false, 	'msg' => 'Erro com solicitação.')) );
	}

	/**
	 * Estado Documento: Em Cadastramento - CPROC
	 * Pós Ação de "Enviar para Análise do Procedimento pelo Coordenador"
	 *
	 * Seleciona o Coordenador (cproc.coordenadoresponsavel)
	 *
	 * @internal função sendo usada na condicao da pre ação 
	 *			 "preAcaoEnviarParaAnaliseDoProcedimentoPeloCoordenador"
	 *			 para o estado documento CPROC_EMCADASTRAMENTO_ESDID
	 */
	public static function selecionarCoordenador(){
		global $db;

		extract($_POST);

		$sql = " INSERT INTO cproc.coordenadoresponsavel ( prcid, usucpf, crpdthrdistribuicao )
				 VALUES ( (SELECT prcid FROM cproc.processo WHERE docid = '" . $docid . "'), '".$coordenador."', NOW() ) ";
		$db->executar( $sql );
	}

	/**
	 * Estado Documento: Em Atualização do Procedimento
	 * Pós Ação de "Enviar para Elaboração da minuta pelo técnico"
	 *
	 * Seleciona o Coordenador (cproc.coordenadoresponsavel)
	 *
	 * @internal função sendo usada na condicao da pre ação 
	 *			 "preAcaoEnviarParaAnaliseDoProcedimentoPeloCoordenador"
	 *			 para o estado documento CPROC_EMCADASTRAMENTO_ESDID
	 */
	public static function selecionarTecnico(){
		global $db;

		extract($_POST);
		
		$sql = "select count(*) as qtd from cproc.tecnicoresponsavel where trpstatus = 'A' and prcid = (SELECT prcid FROM cproc.processo WHERE docid = '" . $docid . "')";
		$res = $db->carregar($sql);
		
		if($res[0]['qtd'] > 0){
			$sql = "update cproc.tecnicoresponsavel set trpstatus = 'I' where prcid = (SELECT prcid FROM cproc.processo WHERE docid = '" . $docid . "')";
			$db->executar($sql);
		}

		$sql = " INSERT INTO cproc.tecnicoresponsavel ( prcid, usucpf, trpdthrdesignacao )
				 VALUES ( (SELECT prcid FROM cproc.processo WHERE docid = '" . $docid . "'), '".$tecnico."', NOW() ) ";
		$db->executar( $sql );
	}

	/**
	 * Estado Documento: Impressão, assinatura e numeração do documento - CPROC
	 * Pré Ação de "Enviar para Aguardar Manifestação de Interessado"
	 *
	 * Registra prazo na tabela 'cproc.prazomanifestacao'
	 */
	public static function registraPrazoManifestacao(){
		global $db;

		$sql = " INSERT INTO cproc.prazomanifestacao
					( pzmprazomanifestacao, pzmdthrmanifestacao, pzmdtfinalprazo, prcid )
				VALUES
					( ".$_POST['pzmprazomanifestacao'].", 
					  NOW(), 
					  NOW() + interval '1 day' * ".$_POST['pzmprazomanifestacao'].",
					  (SELECT prcid 
					   FROM cproc.processo 
					   WHERE docid = '".$_POST['docid']."') ) ";
		$db->executar( $sql );
	}

	/**
	 * Estado Documento: Em Análise de Procedimento pelo Coordenador
	 * Pós-Ação de "Designar Técnico Responsável"
	 *
	 * Determinar Prazo (nº de dias) (cproc.coordenadoresponsavel)
	 */
	public static function preAcaoDesignarTecnicoResponsavel(){
		global $db;
            
		extract( $_POST );

		if( !$crpprazodefinido )
			exit( simec_json_encode(array('boo' => false, 	'msg' => 'Prazo não pode ficar em branco.')) );
		if( !$docid )
			exit( simec_json_encode(array('boo' => false, 	'msg' => 'Houve um erro ao identificar processo, tente novamente.')) );

		// associa fase
		$sql = " UPDATE cproc.coordenadoresponsavel 
				 SET crpprazodefinido = '".$crpprazodefinido."'
				 WHERE crpid = (
				 	SELECT crpid
					FROM cproc.coordenadoresponsavel 
					WHERE prcid = (
						SELECT prcid 
						FROM cproc.processo 
						WHERE docid = '".$docid."'
					)
					ORDER BY crpdthrdistribuicao DESC LIMIT 1
				 ) ";
		$db->executar( $sql );
		
		// Victor martins Machado
		// Está linha foi adicionada para incluir a gravação do técnico na ação.
		WorkflowCproc::selecionarTecnico();

		if( $db->commit() )
			exit( simec_json_encode(array('boo' => true, 	'msg' => '')) );
		else
			exit( simec_json_encode(array('boo' => false, 	'msg' => 'Erro com solicitação.')) );
	}

	/**
	 * Estado Documento: Em Análise de Procedimento pelo Coordenador
	 * Pós-Ação de "Elaborar Minuta"
	 *
	 * Faz upload de pós-ação do documento(arquivo) da minuta
	 */
	public static function posAcaoElaborarMinuta(){
		global $db;

	    $arquivo = $_FILES["arquivo"];
	    if ($_FILES["arquivo"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"]) {
	        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

	        $file = new FilesSimec(null, null, "cproc");
	        $file->setPasta('cproc');
	        $file->setUpload(null, 'arquivo', false);
	        $arqid = $file->getIdArquivo();
	    }

	    // registra minuta - pq não tem informação de que ela já exista!
	        $sql = " INSERT INTO cproc.minuta ( prcid, tdpid, usucpf ) 
	                 VALUES ( 
	                    ".$_POST['prcid']." ,
	                    ".$_POST['tdpid']." ,
	                   	'".$_SESSION['usucpf']."'
	                 )
	                 RETURNING mntid ";
	        $mntid = $db->pegaUm( $sql );

	    //Insere o registro na tabela cproc.minutaarquivo
	        $sql = " INSERT INTO cproc.minutaarquivo( arqid, mntid )
	                 VALUES (
	                 ".$arqid.", 
	                 ".$mntid." ) ";
	        $db->executar($sql);

	    return $db->commit();
	}
	
	public static function posAcaoAnexarDocumento(){
		global $db;
		
		$arquivo = $_FILES["arquivo"];
	    if ($_FILES["arquivo"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"]) {
	        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

	        $file = new FilesSimec(null, null, "cproc");
	        $file->setPasta('cproc');
	        $file->setUpload(null, 'arquivo', false);
	        $arqid = $file->getIdArquivo();
	    }

	    // registra documento - pq não tem informação de que ela já exista!
	        $sql = " INSERT INTO cproc.documento ( prcid, tdpid, usucpf ) 
	                 VALUES ( 
	                    ".$_POST['prcid']." ,
	                    ".$_POST['tdpid']." ,
	                    '".$_SESSION['usucpf']."'
	                 )
	                 RETURNING docid ";
	        $docid = $db->pegaUm( $sql );

	    //Insere o registro na tabela cproc.documentoarquivo
	        $sql = " INSERT INTO cproc.documentoarquivo( arqid, docid )
	                 VALUES (
	                 ".$arqid.", 
	                 ".$docid." ) ";
	        $db->executar($sql);

	    return $db->commit();
	}

	/**
	 * Estado Documento: Em Atualização do Procedimento
	 * Pós-Ação de "Enviar para Elaboração da minuta pelo técnico"
	 *
	 * Converter número de dias do prazo apontado em "posAcaoDesignarTecnicoResponsavel" 
	 * (cproc.coordenadoresponsavel) a partir da data atual e gravar em (cproc.tecnicoresponsavel)
	 */
	public static function posAcaoEnviarParaElaboracaoDaMinutaPeloTecnico(){
		global $db;

		$sql = " UPDATE cproc.tecnicoresponsavel 
				 SET trpdtfinalprazo = NOW() + interval '1 day' * (
				 	SELECT crpprazodefinido
				 	FROM cproc.coordenadoresponsavel 
				 	WHERE prcid = ( SELECT prcid 
									FROM cproc.processo 
									WHERE docid = '".$_POST['docid']."' )
				 	ORDER BY crpdthrdistribuicao DESC
				 	LIMIT 1
				 )
				 WHERE prcid = ( SELECT prcid 
								 FROM cproc.processo 
								 WHERE docid = '".$_POST['docid']."' ) ";
		
		$db->executar( $sql );
		
		return $db->commit();
	}

	/**
	 * Estado Documento: Impressão, assinatura e numeração do documento
	 * Pré-Ação de "Registrar Publicação"
	 *
	 * registra prazo na tabela cproc.prazoexecucao
	 */
	public static function preAcaoRegistrarPublicacao(){
		global $db;

		// ver($_POST,d);
		$sql = " INSERT INTO cproc.prazoexecucao 
					( pzeprazoexecucao, pzedthrexecucao, pzedtfinalprazo, prcid )
				VALUES
					( ".$_POST['pzeprazoexecucao'].", 
					  NOW(), 
					  NOW() + interval '1 day' * ".$_POST['pzeprazoexecucao'].",
					  (SELECT prcid 
					   FROM cproc.processo 
					   WHERE docid = '".$_POST['docid']."') ) ";
		$db->executar( $sql );

		if( $db->commit() )
			exit( simec_json_encode(array('boo' => true, 'msg' => '')) );
		else
			exit( simec_json_encode(array('boo' => false, 'msg' => 'Erro com solicitação.')) );
	}

	/**
	 * Estado Documento: Impressão, assinatura e numeração do documento
	 * Pós-Ação de "Registrar Publicação"
	 *
	 * Enviar email para técnico e coordenador
	 * @todo questionar o siqueira sobre o cotneudo e assunto
	 */
	public static function posAcaoRegistrarPublicacao(){
		global $db;

		// busca email de coordenador e do tecnico com os respectivos nomes
		$sql = " select 
					(select utr.usunome from cproc.tecnicoresponsavel tr
					left join seguranca.usuario utr ON utr.usucpf = tr.usucpf
					where tr.prcid = p.prcid 
					order by trpdthrdesignacao desc limit 1 ) as ut_usunome,

					(select utr.usuemail from cproc.tecnicoresponsavel tr
					left join seguranca.usuario utr ON utr.usucpf = tr.usucpf
					where tr.prcid = p.prcid 
					order by trpdthrdesignacao desc limit 1 ) as ut_usuemail,

					(select ucr.usunome from cproc.coordenadoresponsavel cr
					left join seguranca.usuario ucr ON ucr.usucpf = cr.usucpf
					where cr.prcid = p.prcid 
					order by crpdthrdistribuicao desc limit 1 ) as uc_usunome,

					(select ucr.usuemail from cproc.coordenadoresponsavel cr
					left join seguranca.usuario ucr ON ucr.usucpf = cr.usucpf
					where cr.prcid = p.prcid 
					order by crpdthrdistribuicao desc limit 1 ) as uc_usuemail
					
				 FROM cproc.processo p
				 WHERE p.prcid = ( SELECT prcid 
								   FROM cproc.processo 
								   WHERE docid = '".$_POST['docid']."' ) ";
		$info_para_email = $db->carregar( $sql );
		$info_para_email = $info_para_email[0];

		$remetente = "";
		$destinatario = array(
			array( // tecnico
				'usunome' => $info_para_email['ut_usunome'],
				'usuemail' => $info_para_email['ut_usuemail']
			),
			array(
				'usunome' => $info_para_email['uc_usunome'],
				'usuemail' => $info_para_email['uc_usuemail']
			)
		);

		// para teste de envio de email


		$assunto = "Impressão, assinatura e numeração do documento";
		$conteudo = $_POST['cmddsc'] . "<br/><br/>Atenciosamente,<br/>Equipe SIMEC. ";

		return enviar_email( $remetente, $destinatario, $assunto, $conteudo);
	}
	
}