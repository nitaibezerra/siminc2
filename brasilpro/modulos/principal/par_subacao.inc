<?php
cte_verificaSessao();

$admTemp 				= cte_possuiPerfil(array(CTE_PERFIL_ADMINISTRATOR_TEMP)) ? 1 : 2; // **@@&& -- retirar apos reformulação

include_once( "par_subacao_funcoes.inc" );

//DEFINE SE UMA SUB-AÇÃO NOVA
define('CTE_NOVA_SUBACAO', trim($_REQUEST['sbaid']) == '');

//DEFINE QUAL O PERFIL QUE PODE EDITAR (ALTERAÇÃO DE DADOS) DE UMA SUB-AÇÃO
define('CTE_PRM_EDITAR_SUBACAO', cte_podeEditarParecer($_SESSION['inuid']) && $_REQUEST['bloqueado'] !=='1');

//DEFINE QUAL O PERFIL QUE PODE EXCLUIR A SUB-AÇÃO
define('CTE_PRM_EXCLUIR_SUBACAO', CTE_PRM_EDITAR_SUBACAO);

//DEFINE QUAL O PERFIL QUE PODE EMITIR PARECER
define('CTE_PRM_EMITIR_PARECER', CTE_PRM_EDITAR_SUBACAO && ($db->testa_superuser() ||
                                                            cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,
                                                                                   CTE_PERFIL_EQUIPE_TECNICA,
                                                                                   147,
                                                                                   CTE_PERFIL_CONSULTORES, CTE_PERFIL_ADMINISTRATOR_TEMP))));

//DEFINE QUAL O PERFIL QUE VAI SOMENTE VISUALIZAR O PARECER
define('CTE_PRM_VISUALIZAR_PARECER', CTE_PRM_EMITIR_PARECER || cte_possuiPerfil(array(CTE_PERFIL_ALTA_GESTAO, CTE_PERFIL_ADMINISTRATOR_TEMP)));

define('CTE_PRM_VISUALIZAR_PI', CTE_PRM_EDITAR_SUBACAO && cte_possuiPerfil(array(CTE_PERFIL_SUPER_USUARIO,
                                                                                      CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_ADMINISTRATOR_TEMP )));

header('content-type: text/html; charset=iso-8859-1;');


/****************************  	REGRA DE NEGÓCIO DO PAR:  ADITIVOS. ********************************/

$anoAtualTrava 	= 0;
$anoParecer2007 = 0;
$anoParecer2008 = 0;
$anoParecer2009 = 0;
$anoParecer2010 = 0;
$anoParecer2011 = 0;
$anoConvenio 	= 0000;
$valorTotalSub  = 0;

if($db->testa_superuser()){
	$super = 1; // É super usuário
}else{
	$super = 2; 
}

if(cte_possuiPerfil(array(CTE_PERFIL_ADMINISTRADOR))){
	$adm = 1; // É Administrador
}else{
	$adm = 2; 
}

if (!CTE_NOVA_SUBACAO) {
	/**************** Verifica se a subção foi conveniada com o FNDE Se foi bloqueia o ano que foi conveniada ***************/
	$subConveniada 	= subacaoConveniada();
	$anosDoConvenio = array();
	if(is_array($subConveniada)){
		foreach( $subConveniada as $anoSubacoes ){
			$anosDoConvenio[] = $anoSubacoes["sbcano"];
			$ultimoAnoConveniado = $anoSubacoes["sbcano"];
		}
	}
	$anoConvenio 	= ($ultimoAnoConveniado == NULL) ? 0000 : $ultimoAnoConveniado;
	/**************** FIM Verifica se a subção foi conveniada com o FNDE ***************/
	
	/*******************************
	* 	REGRA DE NEGÓCIO DO PAR:  TRAVA ABA ANO SE O ANO FOR MENOR QUE O ANO CORRENTE.
	* 	
	* 	CHAMADA JAVASCRIPT	: function travaAnosAnteriores(cosano); (no arquivo: www/includes/par_subacao.js)
	* 	DESCRIÇÃO:
	*	Se estiver em uma aba onde o ano e menor que o ano atual, e a forma de execução da subação
	* 	for assistencia financeira, transferencia voluntaria ou assistencia tecnica com complementação financeira,
	*   a aba dos anos anteriores são travadas.
	*******************************/
//	dbg($_REQUEST,1);
	//$sql = "SELECT sbaid FROM cte.subacaoindicador si  WHERE  si.sbaid = ".$_REQUEST['sbaid']." AND si.frmid in(16,17)";
	$sql = "SELECT sbaid FROM cte.subacaoindicador si  WHERE  si.sbaid = ".$_REQUEST['sbaid'];
	$frmFinanceira = $db->pegaUm($sql);
	if($frmFinanceira){
		$anoAtualTrava = getdate();
		$anoAtualTrava = $anoAtualTrava["year"]; 	
	}

	/*******************************
	* 	REGRA DE NEGÓCIO DO PAR:  TRAVA ABA ANO EM ELABORAÇÃO DO PAR E VALIDAÇÃO DO MUNICÍPIO.
	* 
	* 	DESCRIÇÃO:
	*	Se estiver em Elaboração do PAR ou em  Validação do Município e se o usuário estiver em alguma aba onde no ano tenha 
	*   o status da subação diferente de vazio Bloqueia a aba do ano da subação.
	*******************************/
	$sql =		"SELECT sptano 
				FROM cte.subacaoparecertecnico 
				WHERE sbaid = '".$_REQUEST['sbaid']."' 
				AND ssuid is not null";
	$anosParecerDados 	= $db->carregar($sql);
	$docid 				= cte_pegarDocid( $_SESSION['inuid'] );
	$documento 			= wf_pegarEstadoAtual( $docid );
	$total 				= count($anosParecerDados); // Anos que tem o parecer Técnico dado.
	
	for($i = 0; $i < $total; $i++){
		switch($anosParecerDados[$i]["sptano"]) {
			case '2007':
				$anoParecer2007 = $anosParecerDados[$i]["sptano"];
				break; 
			case '2008':
				$anoParecer2008 = $anosParecerDados[$i]["sptano"];
				break; 
			case '2009':
				$anoParecer2009 = $anosParecerDados[$i]["sptano"];
				break; 
			case '2010':
				$anoParecer2010 = $anosParecerDados[$i]["sptano"];
				break; 
			case '2011':
				$anoParecer2011 = $anosParecerDados[$i]["sptano"];
				break; 
		}
	}
	/****** FIM TRAVA ABA ANO EM ELABORAÇÃO DO PAR E VALIDAÇÃO DO MUNICÍPIO. ******/
}



/************************************************************	
* 	CARREGAR ADITIVOS/REFORMULAÇÔES
* 	(FUNÇÃO AJAX)
* 	REQUEST				: carregaAditivos
*   CHAMADA JAVASCRIPT	: function carregarAditivos(cosano)
* 	DESCRIÇÃO: 
*   Carrega lista de Aditivos da subação por aba.
*************************************************************/
if($_REQUEST['req'] == 'carregaAditivos'){//lista de Aditivos	
	header('Content-type: text/html;charset=ISO-8859-1');
	if(!$_REQUEST['sbaid']){
		$nrSbaid = 0;
	}else{
		$nrSbaid = $_REQUEST['sbaid'];
	}
	
	$cosano = $_REQUEST['cosano'];
	
	//SQL lista Aditivos
	$sql1= 'SELECT distinct
				\'<center><img onclick="return irAditivo(\' || si.sbaid || \',\' || si.sbaanoaditivo || \')" src="/imagens/consultar.gif" style="border:none" title="Editar aditivo." alt="Ir para o aditivo" /></center>\' as sbaid,
				\'Aditivo \' || si.sbaseqaditivo as texto,  
				to_char(si.sbadata, \'dd/mm/yyyy\') AS data, 
				prsnumeroprocesso,
				\'Aditivo\' as tipo
			FROM cte.subacaoindicador si
			LEFT JOIN cte.projetosapesubacao sc on sc.sbaid = si.sbaid
			LEFT JOIN cte.projetosape c on sc.prsid = c.prsid
			WHERE sbaidpai = '.$nrSbaid.'  
			AND sbaanoaditivo = ('.$cosano.')';
	
	//SQL lista reformulações
	$sql2=	'SELECT distinct
					\'<center><img onclick="return irReformulacao(\' || si.sbaid || \',\' || si.sbaanoreformulacao || \')" src="/imagens/consultar.gif" style="border:none" title="Editar reformulação." alt="Ir para o reformulação" /></center>\' as sbaid,
					CASE WHEN si.sbaseqreformulacao = 1 
						THEN  \'Original\'
						ELSE \'Reformulação \' || si.sbaseqreformulacao 
					END AS texto,  
					to_char(si.sbadata, \'dd/mm/yyyy\') AS data, 
					prsnumeroprocesso,
					\'Reformulação\' as tipo
		FROM cte.subacaoindicador si
		LEFT JOIN cte.projetosapesubacao sc on sc.sbaid = si.sbaid
		LEFT JOIN cte.projetosape c on sc.prsid = c.prsid
		WHERE sbaidpai = '.$nrSbaid.'  
		AND sbaanoreformulacao = \''.$cosano.'\'
		ORDER BY texto';
	
	//Uni as listas
	$sql = $sql1.' union all '.$sql2;
//	ver($sql);
//	die();
	// Existe lista de Aditivos/Reformulações.
	$existeListaAditivoReformulacao = $db->carregar($sql); 
	
	$anoAba = $_REQUEST['cosano'] ? $_REQUEST['cosano'] : 0000; // Ano da aba atual.
	if(is_array($anosDoConvenio)){
		foreach($anosDoConvenio as $dados){
			//if($dados == $anoAba && !$_REQUEST['ad'] ){ 
			if($dados >= $anoAba && !$_REQUEST['ad'] ){ ////**@@&& ALTERAR DEPOIS PARA ESTE QUANDO ACABAR A REFORMULAÇÃO
				if( ($anoConvenio != 0000) || ($existeListaAditivoReformulacao != false) ){//Se existe aditivo 0000 quer dizer que não tem aditivo.
					if($existeListaAditivoReformulacao != false){
						$listaAditivos = '<div style="border-bottom: 2px white solid;"><b><a href="" onclick="extratoSubAcacao();" title="Extrato completo da Subação + seus Aditivos." >Relatório completo</a></b></div>';
					}
					$listaAditivos .= '<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Lista de Aditivos/Reformulação</div>';
					echo $listaAditivos;
					$db->monta_lista_simples( $sql, array('Ações','Convênios', 'Data de criação', 'Número de Processo ','Tipo'), 1000, 1000, 'N', '100%','N' );
				}
			}			
		}			
	}
	die();
}

/************************************************************	
* 	VOLTA PARA SUAÇÃO PAI
* 	(FUNÇÃO AJAX)
* 	REQUEST				: voltarsubacaooriginal
*   CHAMADA JAVASCRIPT	: function voltarSubacaoPai(sbaidPai)
* 	DESCRIÇÃO: 
*   Volta para subação pai (Estando na subação aditivo)
*************************************************************/
if($_REQUEST['req'] == 'voltarsubacaooriginal'){
	$sbaidOrigianl = $_REQUEST['sbaidpai'];
	$_REQUEST['sbaidpai'] = NULL;
	$_REQUEST['anoconvenio'] = NULL;
	echo $sbaidOrigianl;
	die();
}

function excluirObra()
{
	$preid = $_POST['preid'];
	if(!$preid){
		echo "erro";
	}else{
		$oPre = new PreObra();
		$oPre->excluirPreObra($preid);
		$oPre->commit();
	}
}

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['requisicaoAjax']();
	die;
}
/************************************************************	
* 	CRIA O NOVO ADITIVO
* 	(FUNÇÃO AJAX)
* 	REQUEST				: novoAditivo/novaReformulacao
*   CHAMADA JAVASCRIPT	: function adicionarAditivo(ano)
* 	DESCRIÇÃO: 
*   Insere na tabela de subaçãoIndicador a subação e 
*   da update na mesma colocando o sbaidpai
*
*************************************************************/
if($_REQUEST['req'] == 'novoAditivo' || $_REQUEST['req'] == 'novaReformulacao'){ // Se for aditivo insere na tabela subacaoindicador a subação que e aditivo.
	//inserirNovoAditivo($_REQUEST['sbaid'], $cosano);
	//dbg($_REQUEST['ano'],1);
	if($_REQUEST['req'] == 'novaReformulacao'){
		$campoAno = "sbaanoreformulacao";
		$campoSeq = "sbaseqreformulacao";
		$ano      = "'".$_REQUEST['ano']."'";
	}else{
		$campoAno = "sbaanoaditivo";
		$campoSeq = "sbaseqaditivo";
		$ano      = $_REQUEST['ano'];
	}
	$sql = " INSERT INTO cte.subacaoindicador (aciid,
							  undid,
							  frmid,
							  sbadsc,
							  sbastgmpl,
							  sbaprm,
							  sbapcr,
							  sbadata,
							  usucpf,
							  sbaparecer,
							  psuid,
							  ssuid,
							  foaid,
							  prgid,
							  sbaporescola,
							  ppsid,
							  sbaordem,
							  sbaobjetivo,
							  sbatexto,
							  sbacategoria,
							  sbaidpai,
							  ".$campoAno."
							  ) 
			(
					SELECT  aciid,
						undid,
						frmid,
						sbadsc,
						sbastgmpl,
						sbaprm,
						sbapcr,
						sbadata,
						usucpf,
						sbaparecer,
						psuid,
						ssuid,
						foaid,
						prgid,
						sbaporescola,
						ppsid,
						sbaordem,
						sbaobjetivo,
						sbatexto,
						sbacategoria,
						sbaid,
						'".$_REQUEST['ano']."' as ".$campoAno."
					FROM cte.subacaoindicador WHERE sbaid = '".$_REQUEST['sbaid']."'
			) returning sbaid";
	
	
	$sbaidNovo = $db->pegaUm($sql);
	unset($sql);
	$sql="SELECT 
			CASE WHEN max(".$campoSeq.") IS NULL 
				THEN 1 
				ELSE max(".$campoSeq.")+1 
			END as sequencia 
		  FROM cte.subacaoindicador 
		  WHERE sbaidpai = ".$_REQUEST['sbaid']." 
		  	AND ".$campoAno." = ".$ano;
	$numSeq = $db->pegaUm($sql);
//	echo $sql;
//	echo $numSeq;
//	die();
	unset($sql);
	
	$sql = "update cte.subacaoindicador 
			set -- ".$campoAno." = date_part('year', current_date), 
				".$campoSeq." = ".$numSeq."
			where sbaid = ".$sbaidNovo;
	$insereAno = $db->executar($sql);
	unset($sql);
	$db->commit();
	$_REQUEST['sbaidpai'] = $sbaidNovo;
	$_REQUEST['anoconvenio'] = $anoConvenio;
	
//	$test = $_REQUEST['sbaid'];
	
	importarDadosSubAcao( $_REQUEST['sbaid'], $_REQUEST['ano'], $_REQUEST['ano'] , $sbaidNovo ); //**@@&&
//	echo "teste";
   	echo $sbaidNovo;
	die();
}

/******************************* FIM ADITIVO ******************************
***************************************************************************
**************************************************************************/


/************************************************************************
*								SALVAR DADOS							*
************************************************************************/

if ($_REQUEST['formularioSumit'] == '1' && CTE_PRM_EDITAR_SUBACAO) {
	
	// Exclusão
    if ($_REQUEST['vlrProximaSubacao'] == '-1' && !CTE_NOVA_SUBACAO &&  CTE_PRM_EXCLUIR_SUBACAO) { 
    	
	    // Exclusão efetuada com sucesso
    	if( excluirSubacao( $_REQUEST['sbaid'] ) ){
	        echo '<script type="text/javascript">'
	            .'window.opener.location.reload();'
	            .'alert("Sub-ação excluída com sucesso.");'
	            .'window.opener.focus();'
	            .'window.close();'
	            .'</script>';
    	}
    	// Erro na exclusão
    	else{
			echo '<script type="text/javascript">'
	        	.'alert("Operação não realizada!\nFoi dado o parecer técnico ou foi gerado convênio.");'
	            .'window.location.href = "?modulo=principal/par_subacao&acao=A&sbaid=' . $_REQUEST['sbaid'] . '";'
	            .'</script>';
    	}
        die();
    }

    // Inserção
    $anoExercicio = $_REQUEST['anoExercicio'];

    if (trim($_REQUEST['aciid']) == '' || $_REQUEST['aciid'] == '0') {
        echo 'alert("Não foi possível capturar o indicador da subação.");';
        die();
    }    
    
    // Inserindo nova subação
    if( CTE_NOVA_SUBACAO ){

    	$arDados = $_REQUEST;
    	$arDados['usucpf'] = $_SESSION['usucpf'];
    	
        $_REQUEST['sbaid'] = inserirNovaSubacao( $arDados );
        //echo 'window.location.href="' , $_SERVER['REQUEST_URI'] , '&sbaid=' , $_REQUEST['sbaid'] , '";';
    }
    else {

    	$msg = gravarSubAcao( $anoExercicio );

        if (!$_REQUEST['vlrProximaSubacao'])
            $_REQUEST['vlrProximaSubacao'] = $_REQUEST['sbaid'];
			
    		// Verifica se e Aditivo ou subação normal //
            if($_REQUEST['sbaidpai'] && $_REQUEST['ad'] && $_REQUEST['anoconvenio']){
            	echo '<script> alert("'.$msg.'");'
	            ,'window.location.href="/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['vlrProximaSubacao']
	            ,'&sbaidpai=' , $_REQUEST['sbaidpai']
	            ,'&ad=' , $_REQUEST['ad']
	            ,'&anoconvenio=' , $_REQUEST['anoconvenio'] , '"; </script>';
            }else{
            	echo '<script> alert("'.$msg.'");'
	            ,'window.location.href="/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['vlrProximaSubacao']
	            ,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '"; </script>';
            }
            
       // header('Location: /brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&sbaid=' . $_REQUEST['vlrProximaSubacao'] . '&parsubacaoanocrt=' . $_REQUEST['anoExercicio']);
    }
}

//!@------------------------------------------------------------- FUNCOES AJAX

/************************************************************************
*		   LISTAGEM, EXCULSÃO DE ITENS-INDICADOR E TOTALIZADORES		*
************************************************************************/

if( array_key_exists( 'req', $_REQUEST ) ){
	
    ob_clean();

    if (trim($_REQUEST['sbaid']) == '') {
        exit;
    }

	// Limpar SubAção
    if ($_REQUEST['req'] == 'limparSubacao') {

    	if( limparSubAcao() ){
			echo 'alert("Dados excluídos com sucesso.");'
                ,'window.location.href="/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['sbaid']
                ,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '";';
        } 
		else{
			echo 'alert("Já foi dados parecer para esta subação. Os dados NÃO foram apagados.");'
				,'window.location.href="/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['sbaid']
				,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '";';
		}	    	
    	
    }
    // Plano Interno    
    elseif ($_REQUEST['req'] == 'prgplanointerno') {
        echo $db->pegaUm('SELECT prgplanointerno
                          FROM cte.programa
                          WHERE prgid = ' . (integer) $_REQUEST['prgplanointerno']);
    }
    // Verificar Inserção Dados SubAção     
    elseif ($_REQUEST['req'] == 'verificarInsercaoDadosSubacao') {
    	
        if ($_REQUEST['sbaporescola'] == 't') {
            $sql = 'SELECT count(sba.sbaid)
            		FROM cte.subacaoindicador sba
            			INNER JOIN cte.subacaobeneficiario ben ON sba.sbaid = ben.sbaid AND (vlrurbano    > 0 OR vlrrural > 0)
            			INNER JOIN cte.composicaosubacao cos ON sba.sbaid = cos.sbaid   AND (cosqtd       > 0)
            			INNER JOIN cte.qtdfisicoano qfa ON sba.sbaid = qfa.sbaid        AND (qfaqtd       > 0)
            		WHERE sba.sbaid = ' . $_REQUEST['sbaid'];
        } else {
            $sql = 'SELECT count(sba.sbaid)
            		FROM cte.subacaoindicador sba
            			INNER JOIN cte.subacaobeneficiario ben ON sba.sbaid = ben.sbaid AND (vlrurbano    > 0 OR vlrrural > 0)
            			INNER JOIN cte.composicaosubacao cos ON sba.sbaid = cos.sbaid   AND (cosqtd       > 0)
            		WHERE sba.sbaid = ' . $_REQUEST['sbaid'];
        }

        echo '<script>_totalizar(' , $db->pegaUm($sql) , ');</script>';

    }
    // Carregar Itens de Composição    
    elseif ($_REQUEST['req'] == 'carregarItensComposicao') {
       
        header('Content-type: text/html;charset=ISO-8859-1');    	

		$sql = montarSQLItensComposicao();		
        $db->monta_lista_simples($sql, array('Ações', 'Identificação do Item', 'Un. de Medida', 'Qtd', 'Valor Unitário', 
        									 'Total'), 1000, 1000, 'N', '100%','N');
		$valorTotalSub = $db->pegaUm("SELECT
											trim(to_char(sum(cos.cosqtd * cos.cosvlruni), '99999999D99')) as vlrtotal
										FROM cte.composicaosubacao cos
										INNER JOIN cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
										INNER JOIN cte.subacaoindicador si ON cos.sbaid = si.sbaid
										WHERE si.sbaidpai  = " . $_REQUEST['sbaid']  . "
										AND si.sbaseqreformulacao = 1
										AND cos.cosano = " . $_REQUEST['cosano']);
		
	    $valorTotalSub = str_replace(',','.',$valorTotalSub); 
//	    dbg($valorTotalSub,1);
	     
	    $sql=  "SELECT  
					  prsnumeroprocesso
				FROM cte.subacaoindicador si
				LEFT JOIN cte.projetosapesubacao sc on sc.sbaid = si.sbaid
				LEFT JOIN cte.projetosape c on sc.prsid = c.prsid
				WHERE sbaidpai = ".$_REQUEST['sbaid']."  
				AND sbaanoreformulacao = '".$_REQUEST['cosano']."'";
	    $existeListaAditivo = $db->carregar($sql);
	    if($existeListaAditivo){ 
	  		echo '<table width=\'100%\'>
		  			  <tr>
		  				<td colspan = \'6\' align = \'right\'>
		  				<b>Total da Subação Original:</b>
		  				</td>
		  				<td align = \'left\' width=\'15%\'>
		  				<b>R$ '.number_format($valorTotalSub, 2, ',', '.').'</b>
		  				</td>
		  			  </tr>
		  		  </table>';
	    }
        
    }
    // Remover Itens de Composição     
    elseif ($_REQUEST['req'] == 'removerItemComposicao' && $_REQUEST['cosid'] != ''){

        $sql = 'DELETE FROM cte.monitoramentocomposicao
        		WHERE cosid = ' . (integer) $_REQUEST['cosid'] . ';
        		
				DELETE FROM cte.escolacomposicaosubacao
        		WHERE cosid = ' . (integer) $_REQUEST['cosid'] . ';

        		DELETE FROM cte.composicaosubacao
        		WHERE cosid = ' . (integer) $_REQUEST['cosid'];

        $db->executar($sql);
        $db->commit();

    }
	// Carregar Beneficiários     
    elseif ($_REQUEST['req'] == 'carregarBeneficiarios' && $_REQUEST['sbaid']  != '' && $_REQUEST['sabano'] != ''){

        header('Content-type: text/html;charset=ISO-8859-1');

        $sql = montarSQLBeneficiarios();
        $db->monta_lista_simples($sql, array('Ações', 'Beneficiário', 'Zona Urbana', 'Zona Rural',
                                             'Total'), 1000, 1000, 'S', '100%','N');

    }
    // Remove Beneficiários   
    elseif ($_REQUEST['req'] == 'removerBeneficiario' && $_REQUEST['benid']  != '' && $_REQUEST['sabano'] != ''){

        $sql = '
		        DELETE FROM cte.subacaobeneficiario
		        WHERE benid  	   = ' . (integer) $_REQUEST['benid' ] .'
				AND sabano = ' . (integer) $_REQUEST['sabano'] .'
				AND sbaid  = ' . (integer) $_REQUEST['sbaid'];

        $db->executar($sql);
        $db->commit();


    }
    // Carregar Escolas     
    elseif ($_REQUEST['req'] == 'carregarEscolas' && $_REQUEST['sbaid']  != '' && $_REQUEST['qfaano'] != ''){

		$arEscolas = montarSQLEscolas();
        $db->monta_lista_simples($arEscolas, array('Ações', 'Código INEP', 'Escola', 'Município', 'Oferta EPT', 'Quantidade'), 
        									 	   1000, 1000, 'N', '100%','N');

    }
    // Remover Escolas     
    elseif ($_REQUEST['req'] == 'removerEscola' && $_REQUEST['qfaid'] != ''){

        $sql = 'DELETE FROM cte.escolacomposicaosubacao
        		WHERE qfaid = ' . (integer) $_REQUEST['qfaid'] . ';

		        DELETE FROM cte.qtdfisicoano
        		WHERE qfaid = ' . (integer) $_REQUEST['qfaid'];

        $db->executar($sql);
        $db->commit();

    }
    // Carregar dados do Parecer     
    elseif ($_REQUEST['req'] == 'carregarDadosParecer') {

        $sql = '
		        SELECT
		            coalesce(spt.sptparecer,\'\')      as sptparecer,
		            coalesce(spt.sptunt,0)             as sptunt,
		            coalesce(spt.sptuntdsc,\'\')       as sptuntdsc,
		            coalesce(spt.sptano,0)             as sptano,
		            spt.sptinicio,
		            spt.sptfim,
		            coalesce(spt.sptplanointerno, \'\') as plinumplanointerno,
		            coalesce(spt.sptnumprocesso, \'\') as sptnumprocesso,
		            coalesce(spt.ssuid,0)              as ssuid
		        FROM cte.subacaoparecertecnico spt
		        	INNER JOIN cte.subacaoindicador sba on spt.sbaid = sba.sbaid
		        WHERE spt.sbaid  = ' . $_REQUEST['sbaid'] . '
				AND spt.sptano = ' . $_REQUEST['sbtano'];
		  
        $res = $db->pegaLinha($sql);

        header('content-type: text/xml; charset=iso-8859-1');
        echo '<?xml version="1.0" encoding="iso-8859-1"?>'                              , "\n"
            ,'<response>'                                                               , "\n"
            ,'<sptparecer><![CDATA[', $res['sptparecer']                                , ']]></sptparecer>'
            ,'<sptunt>'             , $res['sptunt']                                    , '</sptunt>'
            ,'<sptuntdsc><![CDATA[' , $res['sptuntdsc']                                 , ']]></sptuntdsc>'
            ,'<sptano>'             , $res['sptano']                                    , '</sptano>'
            ,'<sptinicio>'          , $res['sptinicio'] == '' ? '0' : $res['sptinicio'] , '</sptinicio>'
            ,'<sptfim>'             , $res['sptfim']    == '' ? '0' : $res['sptfim']    , '</sptfim>'
            ,'<sptanoterminocurso>' , $res['sptanoterminocurso']                        , '</sptanoterminocurso>'
            ,'<sptnumprocesso>'		, $res['sptnumprocesso']    						, '</sptnumprocesso>'
            ,'<plinumplanointerno>'	, $res['plinumplanointerno']    					, '</plinumplanointerno>'
            ,'<ssuid>'              , $res['ssuid']                                     , '</ssuid>'
            ,'</response>';
    } 
    // Importar Dados de um ano para outro     
    elseif ($_REQUEST['req'] == 'importarDados') {
		
		//$sql="select pssano as sbcano from cte.projetosapesubacao where sbaid = ".trim( $_REQUEST['sbaid'] );
	
		//$res = $db->carregar( $sql );

		//if( !$res ){
	    	if( importarDadosSubAcao( $_REQUEST['sbaid'], $_REQUEST['anoExercicio'], $_REQUEST['csAno'] ) ){
				echo 'alert("Dados importados com sucesso.");'
	                ,'window.location.href="/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['sbaid']
	                ,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '";';    			
	    	}
			else{
				echo 'alert("Não foi possível importar os dados.");'
					,'window.location.href="/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['sbaid']
					,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '";';
			}
		/*
		}
		
    	else{
			echo 'alert("Não foi possível importar os dados, pois a subação já foi conveniada!!!");'
				,'window.location.href="/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&sbaid=' , $_REQUEST['sbaid']
				,'&parsubacaoanocrt=' , $_REQUEST['anoExercicio'] , '";';
		}
		*/
	} 
    // Carregar Totalizadores    
    elseif ($_REQUEST['req'] == 'carregarTotalizadores') {
        if ($_REQUEST['sbaporescola'] == 't') {
            // TOTALIZACAO POR ESCOLA
            $sql = '
            SELECT DISTINCT
                ent.entnome,
                sum(qfa.qfaqtd) as qtdtotal,
                sum(tot.cosvlruni * ecs.ecsqtd) as vlrtotal
            FROM
                cte.qtdfisicoano qfa
            LEFT JOIN
                cte.escolacomposicaosubacao ecs on qfa.qfaid = ecs.qfaid
            LEFT JOIN
                cte.composicaosubacao cos on cos.cosid = ecs.cosid
            INNER JOIN
                entidade.entidade ent on qfa.entid = ent.entid
            LEFT JOIN
                (SELECT DISTINCT
                    cos.cosid,
                    cos.cosvlruni
                 FROM
                    cte.escolacomposicaosubacao ecs
                 INNER JOIN
                    cte.composicaosubacao cos on cos.cosid = ecs.cosid
                 GROUP BY
                    cos.cosid,
                    cos.cosvlruni) as tot on tot.cosid = ecs.cosid
            WHERE
                qfa.sbaid = ' . $_REQUEST['sbaid'] . '
                and  qfa.qfaano not in (0) 
            GROUP BY
                ent.entnome
            ORDER BY
                ent.entnome';

            echo '<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">'
                ,'<tr>'
                ,'<td style="font-weight:bold;text-align:center">Totalizador Por Escola</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td>';
            $db->monta_lista_simples($sql, array('Escola',
                                                 'Quantidade',
                                                 'Total (R$)'), 1000, 1000, 'S', '100%','N');

            echo '</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td style="font-weight:bold;text-align:center">Totalizador Por Itens de Composição</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td>';

        } else {
            echo '<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">'
                ,'<tr>'
                ,'<td style="font-weight:bold;text-align:center">Totalizador Por Itens de Composição</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td>';
        }


        $sql = '
        SELECT
            cos.cosdsc
            ,coalesce(cos.cosqtd, 0) as cosqtd
            ,coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0) as cosvlrtotal
        FROM
            cte.composicaosubacao cos
        WHERE
            cos.sbaid  = ' . $_REQUEST['sbaid'];

        $db->monta_lista_simples($sql, array('Identificação do Item',
                                             'Qtd',
//                                             'Valor Unitário',
                                             'Total'), 1000, 1000, 'S', '100%');

        echo '</td>'
            ,'</tr>'
            ,'<tr>'
            ,'<td style="font-weight:bold;text-align:center">Totalizador Por Beneficiários</td>'
            ,'</tr>'
            ,'<tr>'
            ,'<td>';

        $sql = '
        SELECT
            be.bendsc,
            sb.vlrurbano as vlrurbano,
            sb.vlrrural  as vlrrural,
            sb.vlrurbano + sb.vlrrural as sabvlrtotal
        FROM
            cte.subacaobeneficiario sb
        INNER JOIN
            cte.beneficiario be ON sb.benid = be.benid
        WHERE
            sb.sbaid  = ' . $_REQUEST['sbaid'];

        $db->monta_lista_simples($sql, array('Beneficiário',
                                             'Zona Urbana',
                                             'Zona Rural',
                                             'Total'), 1000, 1000, 'S', '100%');
        echo '</td>'
            ,'</tr>'
            ,'</table>';
    }

    die();
} // Fim de if( array_key_exists('req', $_REQUEST ) )



function sql2html($coluna) {
    if (strtolower($coluna) == 't')
        return true;

    if (strtolower($coluna) == 'f')
        return false;

    if (strtolower($coluna) == 'null' || $coluna == null)
        return '';

    return $coluna;
}



/*!@
if ($db->testa_superuser() || cte_possuiPerfil(CTE_PERFIL_EQUIPE_TECNICA_FNDE)) {
    $display2007 = "";
} else {
    $display2007 = "style=\"display:none\"";
}
//                                                                          */

include APPRAIZ . "includes/registraracesso.php";

$podeExcluirSubacao = cte_podeEditarSubacao($_SESSION["inuid"]);
$docid              = cte_pegarDocid($_SESSION['inuid']);
$estadoDocumento    = wf_pegarEstadoAtual($docid);
$capturaTipo        = cte_pegarItrid($_SESSION['inuid']) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";



//---------------------------------------------- SUBAÇÕES - PROXIMA E ANTERIOR
$sql = '
    select
        sai.sbaid,
        dim.dimcod,
        ard.ardcod,
        ind.indcod,
        ind.indid
    from cte.subacaoindicador sai
        inner join cte.acaoindicador aci on
            aci.aciid = sai.aciid
        inner join cte.pontuacao pto on
            pto.ptoid = aci.ptoid
        inner join cte.indicador ind on
            ind.indid = pto.indid
        inner join cte.areadimensao ard on
            ard.ardid = ind.ardid
        inner join cte.dimensao dim on
            dim.dimid = ard.dimid
    where
        pto.inuid = ' . $_SESSION['inuid'] . ' and
        pto.ptostatus = \'A\'
    order by
        dim.dimcod,
        ard.ardcod,
        ind.indcod,
        sai.sbaordem,
        sai.sbadsc';


$subAcoes      = (array) $db->carregar($sql);
$sbaidAnterior = false;
$sbaidProximo  = false;

while (list($k, $subacao) = each($subAcoes)) {
    if ($subacao["sbaid"] == (integer) $_REQUEST['sbaid']) {

        if (array_key_exists($k - 1, $subAcoes)) {
            $sbaidAnterior = $subAcoes[$k - 1]["sbaid"];
        }

        if (array_key_exists($k + 1, $subAcoes)) {
            $sbaidProximo  = $subAcoes[$k + 1]["sbaid"];
        }

        break;
    }
}




//-------------------------------------------------- SUBAÇÕES - CARGA DE DADOS
$sql = '
select
    s.sbaid,
    s.aciid,
    s.undid,
    s.frmid,
    s.sbadsc,
    s.sbastgmpl,
    s.sbaprm,
    s.sbapcr,
    s.sbadata,
    s.usucpf,
    s.sbaparecer,
    s.psuid,
    s.ssuid,
    s.foaid,
    s.prgid,
    s.sbaporescola,
    s.ppsid,
    s.sbaordem,
    s.sbaobjetivo,
    s.sbatexto,
    s.sbacategoria
from
    cte.subacaoindicador s
left join
    cte.programa p on p.prgid = s.prgid
where
    s.sbaid = ' . (integer) $_REQUEST['sbaid'];

$sai = array_map('sql2html', (array) $db->pegaLinha($sql));
extract($sai);
$_SESSION['brasilpro']['frmid'] = $frmid;

$sbaporescola            = $sbaporescola == 't';
$existemEscolasAtendidas = $db->pegaUm('SELECT count(*) FROM cte.qtdfisicoano WHERE sbaid = ' . (integer) $_REQUEST['sbaid']) > 0;

$sql = '
select
    dim.dimid,
    dim.dimcod,
    dim.dimdsc,
    are.ardid,
    are.ardcod,
    are.arddsc,
    ind.indid,
    ind.indcod,
    ind.inddsc,
    aci.acidsc,
    aci.acilocalizador,
    pto.ptoid
from
    cte.acaoindicador aci
inner join
    cte.pontuacao pto on pto.ptoid = aci.ptoid and pto.ptostatus = \'A\'
inner join
    cte.indicador ind on ind.indid = pto.indid
inner join
    cte.areadimensao are on are.ardid = ind.ardid
inner join
    cte.dimensao dim on dim.dimid = are.dimid
where
    aci.aciid = ' . (trim($aciid) != '' ? $aciid : (integer) $_REQUEST['aciid']);

//dbg($aciid,1);
$aci = (array) $db->pegaLinha($sql);
extract($aci);

$_SESSION['indid'] = $indid;
$aciid             = trim($aciid) != '' ? $aciid : (integer) $_REQUEST['aciid'];

//$_cosano = brp_recuperArArAno();
if($_REQUEST['sbaidpai']){ // Se o usuario estiver em um Aditivo. (Então tem SbaidPai)
	$anoAditivo =  $_REQUEST['anoconvenio'];
	$_cosano = brp_recuperArArAno($anoAditivo); // Mostra aba apenas do ano do aditivo.
}else{
	$_cosano = brp_recuperArArAno(); // mostra todos os anos.
}
?>

<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title>PAR - Brasil Profissionalizado - Subação<?php echo " - " , $titulo; ?></title>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link rel="stylesheet" type="text/css" href="../brasilpro/includes/par_subacao.css"/>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript">

    jQuery.noConflict();
    
    function getElementText(tag, parentElem, index)
    {
        if (!index) {
            index=0;
        }

        var result = parentElem.responseXML.getElementsByTagName(tag)[index];

        if (result) {
            if (result.childNodes.length == 0) {
                return '';
            } else if (result.childNodes.length > 1) {
                return result.childNodes[1].nodeValue;
            } else {
                return result.firstChild.nodeValue;
            }
        } else {
            return '';
        }
    }
    </script>

  </head>
  <body>
    <div id="loader-container">
      <div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
    </div>

    <div id="container">
      <?php cte_montaTitulo( $titulo_modulo, '' ); ?>

      <!-- LANÇAMENTO DE DADOS                                              -->

<?php

if ($estadoDocumento['esdid'] == CTE_ESTADO_DIAGNOSTICO) {

?>
      <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <colgroup>
          <col/>
        </colgroup>
        <tbody>
          <tr>
            <td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
              <img align="top" src="/imagens/atencao.png" /> <p>Para elaborar o Plano de Ações Articuladas é necessário finalizar o <a href="?modulo=principal/estrutura_avaliacao&acao=A" title="Exibir diagnóstico" onclick="return redirecionarFaseDiagnostico();">diagnóstico</a>.</p>
            </td>
          </tr>
        </tbody>
      </table>
    </div>
  </body>

  <script type="text/javascript">
  <!--
    function redirecionarFaseDiagnostico()
    {
        window.opener.location.href = '/brasilpro/brasilpro.php?modulo=principal/estrutura_avaliacao&acao=A';
        window.opener.focus();
        window.close();
    }

    document.getElementById('loader-container').style.display = 'none';
    -->

  </script>
<?php
    die();
} // if ($estadoDocumento['esdid'] == CTE_ESTADO_DIAGNOSTICO)

?>

      <form method="post" id="frmParSubacao" action="<?php echo $_SERVER['REQUEST_URI']; ?>">
        <input type="hidden" name="formularioSumit" value="1"/>
        <input type="hidden" name="prgidteste" id="prgidteste" value="<?php echo $prgid ?>"/>
        <input type="hidden" name="ppsid" id="ppsid" value="<?php echo $ppsid ?>"/>
        <input type="hidden" name="aciid" id="aciid" value="<?php echo $aciid ?>"/>
        <input type="hidden" name="sbaid" id="sbaid" value="<?php echo $sbaid ?>"/>
        <input type="hidden" name="anoExercicio" id="anoExercicio" value="0"/>
        <input type="hidden" name="sbaobjetivo" id="sbaobjetivo" value="<?php echo $sbaobjetivo ?>"/>

        <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
          <colgroup>
            <col width="25%" />
            <col width="75%" />
          </colgroup>

          <tbody>
	          <tr id="buttonsContainer">
	          	<td style="text-align : left;">
	             	<input onclick="return submeterFrmParSubacao(false, '<?php echo $sbaidAnterior;     ?>');" type="button" name="btnAnterior" value="Anterior" id="btnAnterior" <?php echo !$sbaidAnterior ? 'disabled="disabled"' : ''; ?>/>
	            </td>
	            <td style="text-align : right;">
	            	<input onclick="return submeterFrmParSubacao(false, '<?php echo $sbaidProximo; ?>');" type="button" name="btnProximo" value="Próxima" id="btnProximo"        <?php echo !$sbaidProximo ? 'disabled="disabled"' : ''; ?>/>
	            </td>
	          </tr>
            <tr>
              <td class="SubTituloDireita">Dimensão:</td>
              <td><?php echo $aci['dimcod'] , '. ' , $aci['dimdsc'] ?></td>
            </tr>

            <tr>
              <td class="SubTituloDireita">Área:</td>
              <td><?php echo $aci['ardcod'] , '. ' , $aci['arddsc']; ?></td>
            </tr>

            <tr>
              <td class="SubTituloDireita">Indicador:</td>
              <td>
                <?php echo $aci['indcod']   , '. ' , $aci['inddsc']; ?>
              </td>
            </tr>

            <tr>
              <td class="SubTituloDireita">Demanda:</td>
              <td><?php echo $aci['acilocalizador'] == "E" ? "Rede Estadual" : "Rede Municipal"; ?></td>
            </tr>

            <tr>
              <td class="SubTituloDireita">Ação:</td>
              <td>
                <?php echo $aci['acidsc']; ?>
              </td>
            </tr>

<?php
// CTE_PRM_EDITAR_SUBACAO
if (!CTE_NOVA_SUBACAO) {

    $select_formaAtendimento = $db->pegaUm('SELECT foadsc FROM cte.formaatendimento WHERE foaid = ' . (integer) $foaid);
    $select_programa         = $db->pegaUm('SELECT prgdsc FROM cte.programa WHERE prgid = '         . (integer) $prgid);
    $select_unidadeMedida    = $db->pegaUm('SELECT unddsc FROM cte.unidademedida WHERE undid = '    . (integer) $undid);

    $select_formaExecucao    = $db->pegaUm('SELECT frmdsc FROM cte.formaexecucao WHERE frmid = '    . (integer) $frmid);

    $prgplanointerno         = $db->pegaUm('SELECT
                                                prgplanointerno
                                            FROM
                                                cte.subacaoindicador sba
                                            INNER JOIN
                                                cte.programa prg ON prg.prgid = sba.prgid
                                            WHERE
                                                sbaid = '    . (integer) $sbaid);

?>
            <tr style="background-color: #cccccc;">
              <td align="left" colspan="<?php echo sizeof($_cosano) + 2; ?>">
                <strong>Dados da Subação</strong>
              </td>
            </tr>
			<!-- 
            <tr id="tr_categoriaDespesa">
              <td align='right' class="SubTituloDireita">Categoria de Despesa:</td>
              <td><?php echo $sbacategoria == 3 ? 'Custeio' : 'Capital'; ?></td>
            </tr>
			-->

            <tr>
              <td align='right' class="SubTituloDireita">Forma de atendimento:</td>
              <td><?php echo $select_formaAtendimento; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Descrição da Subação:</td>
              <td><?php echo $sbadsc; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Estratégia de Implementação:</td>
              <td><?php echo $sbastgmpl; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Programa:</td>
              <td><?php echo $select_programa; ?></td>
            </tr>
			<!-- 
            <tr>
              <td align='right' class="SubTituloDireita">Plano Interno:</td>
              <td><?php echo $prgplanointerno; ?></td>
            </tr>
 			-->
            <tr>
              <td align="right" class="SubTituloDireita">Unidade de Medida:</td>
              <td><?php echo $select_unidadeMedida; ?></td>
            </tr>

            <tr>
              <td align="right" class="SubTituloDireita">Forma de Execução:</td>
              <td><?php echo $select_formaExecucao ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Instituição Parceira (se houver):</td>
              <td><?php echo $sbapcr; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Cronograma:</td>
              <td><?php echo !$sbaporescola ? 'Global' : 'Por Escola'; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">&nbsp;</td>
              <td id="TdEditarSubacao" name="TdEditarSubacao" bgcolor="#dddddd"><input type="button" name="BtnEditarSubacao" id="BtnEditarSubacao" value="Editar" onclick="popupEditarSubacao();" /></td>
            </tr>
		</tbody>
	</table>
<?php

} else {


//------------------------------------------------------- CONSTRUÇÃO DE COMBOS
    $select_formaAtendimento = $db->monta_combo('foaid',
                                                'select foaid as codigo, foadsc as descricao from cte.formaatendimento order by foadsc',
                                                'S', 'Escolha...', '', '', '', '', 'N', 'foaid', true);

    $select_programa         = $db->monta_combo('prgid',
                                                'select prgid as codigo, substr(prgdsc, 1, 100) as descricao from cte.programa order by descricao',
                                                'S', 'Selecione', 'exibirPlanoInterno', '', '', '', 'S', 'prgid', true);

    $select_unidadeMedida    = $db->monta_combo('undid',
                                                'SELECT DISTINCT uni.undid as codigo, uni.unddsc as descricao FROM cte.unidademedida uni INNER JOIN cte.indicadorunidademedida iun ON uni.undid = iun.undid WHERE undtipo = \'' . $capturaTipo . '\' and indid = ' . $indid . ' ORDER BY descricao',
                                                'S', 'Escolha a unidade de medida', '', '','','','S', 'undid', true);

    $sql = '
    SELECT
        frmid as codigo,
        frmdsc as descricao
    FROM
        cte.formaexecucao
    WHERE
        frmtipo = \'' . $capturaTipo . '\'
        AND
        frmbrasilpro = true
    ORDER BY
        descricao';

    $select_formaExecucao = $db->monta_combo('frmid', $sql, 'S', 'Selecione', 'regraFormaExec', '','','','S', 'frmid', true);

?>
            <tr style="background-color: #cccccc;">
              <td align="left" colspan="<?php echo sizeof($_cosano) + 2; ?>">
                <strong>Dados da Subação</strong>
              </td>
            </tr>

            <tr id="tr_categoriaDespesa">
              <td align='right' class="SubTituloDireita">Categoria de Despesa:</td>
              <td>
                <select name="sbacategoria" class="CampoEstilo" id="sbacategoria">
                  <option value="">Escolha...</option>
                  <option value="3" <?php echo $sbacategoria == 3 ? 'selected="selected"' : '' ?>>Custeio</option>
                  <option value="4" <?php echo $sbacategoria == 4 ? 'selected="selected"' : '' ?>>Capital</option>
                </select>
              </td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Forma de atendimento:</td>
              <td><?php echo $select_formaAtendimento; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Descrição da Subação:</td>
              <td><?php echo campo_textarea('sbadsc', "S", 'S', '', 70, 3, 1995 ); ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Estratégia de Implementação:</td>
              <td><?php echo campo_textarea('sbastgmpl', 'S', 'S', '', 70, 3, 1995 ); ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Programa:</td>
              <td><?php echo $select_programa; ?></td>
            </tr>
			<!-- 
            <tr>
              <td align='right' class="SubTituloDireita">Plano Interno:</td>
              <td><?php echo campo_texto('prgplanointerno', "N", "N", "Plano Interno", 51, 100, "", "", 'left', '', 0, 'id="prgplanointerno"' );?></td>
            </tr>
			-->
            <tr>
              <td align="right" class="SubTituloDireita">Unidade de Medida:</td>
              <td><?php echo $select_unidadeMedida; ?></td>
            </tr>

            <tr>
              <td align="right" class="SubTituloDireita">Forma de Execução:</td>
              <td><?php echo $select_formaExecucao ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Instituição Parceira (se houver):</td>
              <td><?php echo campo_texto('sbapcr', '', 'S' , '', 50, 255, '', '', 'left', '', 0, 'id="sbapcr"');?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Cronograma:</td>
              <td>
                <input id="sbaporescola0" type="radio" name="sbaporescola" value="false" <?php echo !$sbaporescola ? 'checked="checked"' : '' ?> /><label for="sbaporescola0">Global</label>
                <input id="sbaporescola1" type="radio" name="sbaporescola" value="true"  <?php echo  $sbaporescola ? 'checked="checked"' : '' ?> /><label for="sbaporescola1">Por Escola</label>
              </td>
            </tr>

<?php

} // if (!CTE_NOVA_SUBACAO)

$colspan       = sizeof($_cosano) + 1;
$tamanhoCelula = round(100 / $colspan);
// Mostrar apenas 1 aba do Aditivo
if(count($_cosano) == 1){
	$colspan = 4;
}
?>

        <table align="center" border="0" class="tabela listagem" cellpadding="0" cellspacing="0" style="display:none" id="alertaPermitirCadastroItensSubacao">
          <tbody>
            <tr style="background-color: #cccccc;">
              <td align="center" colspan="<?php echo $colspan; ?>" style="padding: 15px; font-size: 110%">
                <strong>Finalize o cadastro da subação para que escolas, itens, beneficiários e parecer possam ser registrados.</strong>
              </td>
            </tr>
            <tr>
              <td align="center" colspan="<?php echo $colspan; ?>" style="padding: 15px; font-size: 110%">
                <input onclick="return submeterFrmParSubacao(true,  '<?php echo $_REQUEST['sbaid']; ?>');" type="button" name="btnSalvar" value="Gravar" id="btnSalvar" <?php echo !CTE_PRM_EDITAR_SUBACAO ? 'disabled="disabled"' : ''; ?>/>
                <input onclick="return submeterFrmParSubacao(false, '');" type="button" name="btnCancelar" value="Fechar" id="btnCancelar" />
              </td>
            </tr>
          </tbody>
        </table>

        <table align="center" border="0" class="tabela listagem" cellpadding="0" cellspacing="0" id="tabAbasSelecaoCosano">
          <tbody>
            <tr style="background-color: #cccccc;">
              <td align="left" colspan="<?php echo $colspan; ?>" style="padding: 5px; font-size: 110%">
                <strong>Detalhamento dos Itens que Compõem a Especificação - <span id="spanAno"></span></strong>
              </td>
            </tr>

            <tr id="abasSelecaoCosano">
<?php
    foreach ($_cosano as $ano) {
        $i = $ano;
        // Reformulação // 
     	if( $_REQUEST['ref'] == 1)
     	{ 
     		// se for reformulação escreve na aba de ano a var '$escreve'.
        	$tamanhoCelula = 45;
        	$sql="	select sbaseqreformulacao 
        			from cte.subacaoindicador 
        			where sbaid = ".$_REQUEST['sbaid']." and sbaanoreformulacao = '".$ano."'";
        	$numReformulacao = $db->pegaUm($sql);
        	$escreve = " Reformulação ".$numReformulacao." / ";
        } // fim reformulação //
     	if( $_REQUEST['ad'] == 1)
     	{ 
     		// se for aditivo escreve na aba de ano a var '$escreve'.
        	$tamanhoCelula = 45;
        	$sql="	select sbaseqaditivo 
        			from cte.subacaoindicador 
        			where sbaid = ".$_REQUEST['sbaid']." and sbaanoaditivo = ".$ano;
        	$numAditivo = $db->pegaUm($sql);
        	$escreve = " Aditivo ".$numAditivo." / ";
        } // fim aditivo //
     	global $db;
     	if($_REQUEST['sbaid']){
	  		$sql = "SELECT
			            ssudescricao
			        FROM cte.subacaoparecertecnico spt
			        	INNER JOIN cte.subacaoindicador sba ON spt.sbaid = sba.sbaid
			        	INNER JOIN cte.statussubacao    ss  ON spt.ssuid = ss.ssuid 
			        WHERE spt.sbaid  = ".$_REQUEST['sbaid'] ."
					AND spt.sptano = ".$i;
	  		$status = $db->pegaUm($sql);
     	}
  		if($status == ''){
  			$status = '---';
  		}
        echo '<td id="aba_' , $i , '" style="width: ' , $tamanhoCelula , '%;"><a onclick="return selecionarAba(' , $i , ', true)" href="' , $_SERVER['REQUEST_URI'] , '">' , $escreve.$i , '<br>',$status,'</a>
        	  </td>' , "\n";
    } // foreach ($_cosano as $ano)
    if(count($_cosano) != 1){ // Se for maior que 1 aba mostra os tatalizadores.
?>
              <td id="aba_0000" style="width: <?php echo $tamanhoCelula; ?>%;" valign="top"><a onclick="return selecionarAba('0000', true)" href="<?php echo $_SERVER['REQUEST_URI']; ?>" >Totalizadores<br> - </a>
              </td>
            <?php }else{ ?>
							<td width="90%" style="background: none; border:none;"></td>
							<td width="90%" style="background: none; border:none;"></td>
							<td width="90%" style="background: none; border:none;"></td>
			<?php } ?>
            </tr>

<?php
   
    foreach ($_cosano as $ano) {
        $i = $ano;
?>
<!--		  <script type="text/javascript"> $('total').innerHTML = mascaraglobal('###.###.###.###.###,##', '' );</script> -->
            <tr class="container" style="display: none; height: auto; vertical-align: top" id="abaCosano<?php echo $i; ?>">
              <td align="center" colspan="<?php echo $colspan; ?>">
                <table cellpadding="0" cellspacing="0" width="100%">
				  <tr id="areaListaAditivo<?=$i;?>" >
				    <!-- Lista de Aditivo -->
					<td id="divListaAditivo<?php echo $i; ?>" colspan="3" style="padding:5 5 5 5;"></td>
				  </tr>	
				  <tr id="divAditivo<?php echo $i; ?>" style="display:none;">
					<td   style="background-color: #ddd; height:28px; padding: 5px 5px 5px 5px; " >
						<span style="cursor:pointer;" onclick="return adicionarAditivo(<?php echo $i; ?>);"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Inserir Aditivo</span>
					<?php if( $super == 1 ){ ?>
						<span style="cursor:pointer;" onclick="return adicionarReformulacao(<?php echo $i; ?>);"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Inserir Reformulação</span>	
					<?php }?>	
					</td>
				  </tr>
				  <tr>
				  	<td>
				  		<!--<div id="divStatusSub" class="SubtituloEsquerda">Status da Subação: <?=' '.$status.' '?></div>-->
				  	</td>
				  </tr>
<?php
if( $frmid == 21 && $ano >= '2011' ){?>
					<tr>
						<td colspan="2" class="SubTituloTabela">
							<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Obras</div>
							<script>
								function inserirObras(ano,sbaid)
								{
									url = "brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&sbaid=" + sbaid + "&ano=" + ano;
									janela(url,700,550,"Escolas da Subação");
								}
								function consultarObra(preid,sbaid,sobano)
								{
									url = "brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&sbaid="+sbaid+"&ano="+sobano+"&preid="+preid;
									janela(url,700,550,"Escolas da Subação");
								}
								function excluirObra(obj,preid)
					    		{

					    			if(confirm("Deseja realmente excluir esta obra permanentemente?")){
					    				jQuery.ajax({
										   type: "POST",
										   url: window.location,
										   data: "requisicaoAjax=excluirObra&preid=" + preid,
										   success: function(msg){
												if(!msg){
													$(obj.parentNode.parentNode.parentNode).remove();
												}else{
													alert("Não foi possível excluir a obra!");
												}
										   }
									 });
					    			}
					    		}
							</script>
						</td>
					</tr>
					<tr>
						<td id="obras_<?php echo $ano; ?>" colspan="2" >
							<?php $oPre = new PreObra();?>
							<?php $oPre->montaLista($sbaid,$ano)?>
						</td>
					</tr>
					<tr>
						<td>
							<span id="adObras" name="adObras" style="cursor:pointer;display:block;float:left" 
								  onclick="inserirObras('<?php echo $ano ?>','<?php echo $sbaid ?>')">
								<img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Inserir Obras
							</span>
						</td>
					</tr>
				
<?php 
}
//else{ --Retirado a pedido do Marcelo
?>
                  <tr>
                    <td style="background-color: #ddd">
                      <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Quantidades e Cronograma Físico</div>
					    <input type="hidden" id="valorTotalSub<?=$i; ?>" name="valorTotalSub<?=$i; ?>" value="<?=$valorTotalSub; ?>"></input>
                      <table align="center" border="0" class="tabela" cellpadding="2" cellspacing="1" style="margin-bottom: 10px;">
<?php 
if (!$sbaporescola) {

?>
                        <tr id="cronogramaGlobal<?php echo $i; ?>">
                          <td class="SubTituloDireita">Quantidades:</td>
                          <td style="background-color: #fff"><?php echo campo_texto('sptunt_' . $i, "S", "S", 'Quantitativos', 10, 25 , '', '', 'left', '', 0, 'id="sptunt_' . $i . '"');?></td>
                        </tr>

<?php

}

/*
if($_REQUEST['sbaid'] != ''){
	$sqlParecer = " select sptinicio, sptfim, sptunt from cte.subacaoparecertecnico 
					where sbaid = '".$_REQUEST['sbaid']."'
					and sptano = $i ";

	$arParecer = $db->recuperar($sqlParecer);

	$sptinicio = $sptinicio ? $sptinicio : $arParecer["sptinicio"];
	$sptfim = $sptfim ? $sptfim : $arParecer["sptfim"];
	$sptunt = $sptunt ? $sptunt : $arParecer["sptunt"];
}
*/

?>
                        <tr>
                          <td class="SubTituloDireita">Cronograma Físico:</td>
                          <td style="background-color: #fff; padding-left: 2px">
                            <select name="sptinicio_<?php echo $i; ?>" id="sptinicio_<?php echo $i; ?>">
                              <option value="0">Selecione</option>
                              <option value="1" >Janeiro</option>
                              <option value="2" >Fevereiro</option>
                              <option value="3" >Março</option>
                              <option value="4" >Abril</option>
                              <option value="5" >Maio</option>
                              <option value="6" >Junho</option>
                              <option value="7" >Julho</option>
                              <option value="8" >Agosto</option>
                              <option value="9" >Setembro</option>
                              <option value="10">Outubro</option>
                              <option value="11">Novembro</option>
                              <option value="12">Dezembro</option>
                            </select>
                            a
                            <select name="sptfim_<?php echo $i; ?>" id="sptfim_<?php echo $i; ?>">
                              <option value="0">Selecione</option>
                              <option value="1" >Janeiro</option>
                              <option value="2" >Fevereiro</option>
                              <option value="3" >Março</option>
                              <option value="4" >Abril</option>
                              <option value="5" >Maio</option>
                              <option value="6" >Junho</option>
                              <option value="7" >Julho</option>
                              <option value="8" >Agosto</option>
                              <option value="9" >Setembro</option>
                              <option value="10">Outubro</option>
                              <option value="11">Novembro</option>
                              <option value="12">Dezembro</option>
                            </select>
                          </td>
                        </tr>
                      </table>
                      <br />
                    </td>
                  </tr>

<?php

if ($sbaporescola) {

?>
                  <tr id="cronogramaPorEscola<?php echo $i; ?>">
                    <td>
                      <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Escolas</div>
                      <div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="escolasSubAcao<?php echo $i; ?>"></div>
                      <div style="padding: 5px 0px 0px 5px">
                        <span id="adEscolas" name="adEscolas" style="cursor:pointer;display:block;float:left" onclick="return popupSelecionarEscolas(<?php echo $i; ?>);"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Editar / Inserir Escolas</span>
                        <!--<span id="totalizadorEscolas<?php echo $i; ?>">0</span>-->
                      </div><br />
                    </td>
                  </tr>
<?php
} // if ($sbaporescola)

 if($prgid && CTE_PRM_VISUALIZAR_PI){ ?>

				<tr>
											<td class="SubtituloEsquerda" style="padding: 5px; text-align: center" class="SubTituloDireita">
												Plano Interno
											</td>
										</tr>
										<tr>
											<td class="SubtituloEsquerda" style="padding: 5px; text-align: center" >
												<table align="center" border="0" class="tabela" cellpadding="2" cellspacing="1" style="margin-bottom: 10px;">
														<tr>
															<td class="SubTituloDireita"  style="padding-left: 42px;" >Plano Interno:</td>
															<td style="background-color: #fff">
																<?php
																// RECUPERA OS PLANOS INTERNOS
																$sqlPlanos = "	SELECT distinct plinumplanointerno as codigo, plinumplanointerno as descricao 
																				FROM  cte.programa p
																				INNER JOIN cte.planointerno pi ON pi.prgid = p.prgid
																				WHERE p.prgid = ".$prgid." AND pliano = ".$i." 
																				ORDER BY plinumplanointerno";
																$planos = $db->carregar($sqlPlanos);
																if($planos){
																	$totalPlanos = count($planos);
																	echo '<input type="hidden" name="semplanointerno'.$i.'" id="semplanointerno'.$i.'" value="true"/>';
																	if($totalPlanos > 1){
																		$db->monta_combo(	'plinumplanointerno_' . $i,
																							$sqlPlanos,
																							'S', 'Selecione', '', '', '', '', '', 'plinumplanointerno_' . $i );
																	}else{
																		$db->monta_combo(	'plinumplanointerno_' . $i,
																							$sqlPlanos,
																							'S', '', '', '', '', '', '', 'plinumplanointerno_' . $i );
																	}
																}else{
																	$db->monta_combo(	'plinumplanointerno_' . $i,
																							$sqlPlanos,
																							'N', 'Não existe plano Interno cadastrado para este programa neste ano.', '', '', '', '', '', 'plinumplanointerno_' . $i );	
																	echo '<input type="hidden" name="semplanointerno'.$i.'" id="semplanointerno'.$i.'" value="false"/>';
																}
																?>							
							
														</tr>
												</table>				
											</td>
										</tr>
										<tr>
											<td class="SubtituloEsquerda" style="padding: 5px; text-align: center" >
												<table align="center" border="0" class="tabela" cellpadding="2" cellspacing="1" style="margin-bottom: 10px;">
														<tr>
															<td class="SubTituloDireita" >Número de Processo:</td>
															<td style="background-color: #fff">
																<?php
																// RECUPERA OS NÚMEROS DE PROCESSO DO MUNICÍPIO / ESTADO
																$sqlNumProc = "	SELECT DISTINCT cvrnumprocesso as codigo, cvrnumprocesso as descricao FROM cte.convenioretorno 
																				WHERE inuid = ".$_SESSION['inuid']." ORDER BY cvrnumprocesso";
																
																$numProcessos = $db->carregar($sqlNumProc);
																
																if(is_array($numProcessos)){
																	array_unshift($numProcessos, array('codigo'=>"", 'descricao'=>"Novo Convênio"));
																}									
																if($numProcessos){
																	$totalNumProcessos = count($numProcessos);
																	echo '<input type="hidden" name="semnumprocesso'.$i.'" id="semnumprocesso'.$i.'" value="true"/>';
																	$db->monta_combo(	'cvrnumprocesso_' . $i,
																						$numProcessos,
																						'S', 'Selecione', '', '', '', '', '', 'cvrnumprocesso_' . $i );
																}else{
																	$db->monta_combo(	'cvrnumprocesso_' . $i,
																							$sqlNumProc,
																							'N', 'Não existe Número de processo para esta Entidade', '', '', '', '', '', 'cvrnumprocesso_' . $i );	
																	echo '<input type="hidden" name="semnumprocesso'.$i.'" id="semnumprocesso'.$i.'" value="false"/>';
																}
																?>							
							
														</tr>
												</table>				
											</td>
										</tr>
								<? } ?>
                  <tr>
                    <td>
                      <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Itens</div>
                      <div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="itensComposicaoSubAcao<?php echo $i; ?>"></div>
                      <div id="adItensComposicao" name="adItensComposicao" style="padding: 5px 0px 0px 5px">
                        <span  style="cursor:pointer;display:block;float:left" onclick="return adicionarItensComposicao();"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Editar / Inserir Itens de Composição</span>
                        <!--<span id="totalizadorItensComposicaoSubAcao<?php echo $i; ?>">0</span>-->
                      </div><br />
                    </td>
                  </tr>

                  <tr id="beneficiarioSubAcao<?php echo $i; ?>">
                    <td>
                      <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Beneficiários</div>
                      <div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="beneficiariosSubAcao<?php echo $i; ?>"></div>
                      <div id="adBeneficiarios" name="adBeneficiarios" style="padding: 5px 0px 0px 5px">
                        <span  style="cursor:pointer;display:block;float:left" onclick="return adicionarBeneficiarios();"><img src="/imagens/gif_inclui.gif" align="top" style="border: none" /> Editar / Inserir Beneficiários</span>
                        <!--<span id="totalizadorBeneficiarios<?php echo $i; ?>">0</span>-->
                      </div><br />
                    </td>
                  </tr>

                  <tr>
                    <td>
                      <table id="parecerEquipeTecnica<?php echo $i; ?>" border="0" align="center" cellpadding="0" cellspacing="0" width="100%">
                        <colgroup>
                          <col width="25%" />
                          <col width="75%" />
                        </colgroup>
                        <tr>
                          <td align="right" class="SubTituloDireita">Detalhamento/Comentário:</td>
                          <td><?php echo campo_textarea('sptuntdsc_'  . $i, 'N', 'S' , '', 100, 10, null ); ?></td>
                        </tr>

<?php

if (CTE_PRM_VISUALIZAR_PARECER) {

?>
                        <tr>
                          <td align="right" class="SubTituloDireita">Parecer:</td>
                          <td><?php echo campo_textarea('sptparecer_' . $i, 'N', 'S', '', 100, 3, 2000 ); ?></td>
                        </tr>
                        <tr>
                          <td align="right" class="SubTituloDireita">Status Subação:</td>
                          <td>
                              <?php
                              $db->monta_combo('ssuid_' . $i,
                                               "select ssuid as codigo, ssudescricao as descricao from cte.statussubacao WHERE ssutipostatus = 'E'",
                                               'S', 'Selecione', '', '', '', '', '', 'ssuid_' . $i);
                              ?>
                          </td>
                        </tr>

<?php

} // if (CTE_PRM_VISUALIZAR_PARECER)

?>
                      </table><br />
                    </td>
                  </tr>
<?php
    if (!CTE_PRM_EMITIR_PARECER) {
        echo '<script type="text/javascript">$("sptparecer_' , $i , '", "ssuid_'      , $i , '").each(function(e){if(e)e.disable()});</script>';
    }

//    	} --Retirado a pedido do Marcelo
?>
                </table>
              </td>
            </tr>
<?php
    } // foreach ($_cosano as $ano)
?>

            <tr class="container" style="display: none; height: auto" id="abaCosano0000">
              <td align="left" colspan="<?php echo $colspan; ?>">
                <div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="totalizadoresSubacao"></div>
              </td>
            </tr>
            <tr>
              <td align="center" colspan="<?php echo $colspan; ?>">
              <table width="100%">
                <tr>
                <td colspan="3" style="font-weight:bold;color:red;text-align : center; display:none" id="errorMessageContainer">
                    Documentos em análise financeira devem conter Escolas, Itens de Composição e Beneficiários cadastrados.
                  </td>
                </tr>
				<?php 
				/************************************************************	
				* 	BOTÕES DO RODAPÉ
				* 	DESCRIÇÃO: 
				*   Se existe sbaid pai então mostra BTNS do Aditivo.
				*************************************************************/
				if( $_REQUEST['sbaidpai'] ){ 
				?>
					<tr id="buttonsContainer">
						<td style="width: 25%;text-align : left;"><input type="hidden" name="vlrProximaSubacao" value="" id="vlrProximaSubacao" /></td>
						<td style="text-align : center;">
							<br /><?php 
							if(!$_REQUEST['ref']){
							?>
							<input onclick="return submeterFrmParSubacao(true,  '<?php echo $_REQUEST['sbaid']; ?>');" type="button" name="btnSalvar" value="Gravar" id="btnSalvar" <?php echo !CTE_PRM_EDITAR_SUBACAO ? 'disabled="disabled"' : ''; ?> />
							<?php 
							}
							?>
							<input onclick="return limparSubacao('<?php echo $_REQUEST['sbaid']; ?>');" type="button" name="btnLimparSubacao" value="Limpar Subação" id="btnLimparSubacao" />
							<input onclick="return submeterFrmParSubacao(true,  -1);" type="button" name="btnExcluir" value="Excluir" id="btnExcluir" <?php echo !CTE_PRM_EXCLUIR_SUBACAO ? 'disabled="disabled"' : ''; ?>/>
							<input onclick="return fecharJanela();" type="button" name="btnCancelar"       value="Fechar"            id="btnCancelar" />
						</td>
						<td style="width: 25%;text-align : right;">
							<input onclick="return voltarSubacaoPai('<?php echo $_REQUEST['sbaidpai']; ?>');" type="button" name="btnVoltar" value="Voltar para Subação" id="btnVoltar"/>
						</td>
					</tr>
				<?php }else{ // Está na subação padrão. ?>
              	<tr id="buttonsContainer">
              	  <td style="width: 25%;text-align : left;"><input type="hidden" name="vlrProximaSubacao" value="" id="vlrProximaSubacao" />
                  <input onclick="return submeterFrmParSubacao(false, '<?php echo $sbaidAnterior;     ?>');" type="button" name="btnAnterior" value="Anterior" id="btnAnterior" <?php echo !$sbaidAnterior ? 'disabled="disabled"' : ''; ?>/>
                </td>

                <td style="text-align : center;">
                  <input onclick="return submeterFrmParSubacao(true,  '<?php echo $sbaidAnterior;     ?>');" type="button" name="btnSalvarAnterior" value="Salvar e Anterior" id="btnSalvarAnterior" <?php echo !$sbaidAnterior || !CTE_PRM_EDITAR_SUBACAO ? 'disabled="disabled"' : ''; ?>/>
                  <input onclick="return submeterFrmParSubacao(true,  '<?php echo $_REQUEST['sbaid']; ?>');" type="button" name="btnSalvar"         value="Gravar"            id="btnSalvar"         <?php echo !CTE_PRM_EDITAR_SUBACAO ? 'disabled="disabled"' : ''; ?> />
                  <input onclick="return submeterFrmParSubacao(true,  '<?php echo $sbaidProximo;      ?>');" type="button" name="btnSalvarProximo"  value="Salvar e Próxima"  id="btnSalvarProximo"  <?php echo !$sbaidProximo  || !CTE_PRM_EDITAR_SUBACAO ? 'disabled="disabled"' : ''; ?>/>
                  <br />
                   <div id="divImportardados"></div>

                  <?php if( CTE_PRM_EDITAR_SUBACAO ){ ?>
	                   <input onclick="javscript: toggleDiv( 'divImportardados' );" type="button" name="btnImportarDados" value="Importar Dados" id="btnImportarDados" />
                  <?php } ?>
                   <input onclick="return limparSubacao('<?php echo $_REQUEST['sbaid']; ?>');" type="button" name="btnLimparSubacao" value="Limpar Subação" id="btnLimparSubacao" />                  
                  <input onclick="return submeterFrmParSubacao(true,  -1);"                                  type="button" name="btnExcluir"        value="Excluir"           id="btnExcluir"        <?php echo !CTE_PRM_EXCLUIR_SUBACAO                   ? 'disabled="disabled"' : ''; ?>/>
                  <input onclick="return submeterFrmParSubacao(false, '');"                                  type="button" name="btnCancelar"       value="Fechar"            id="btnCancelar" />
                </td>

                <td style="width: 25%;text-align : right;">
                  <input onclick="return submeterFrmParSubacao(false, '<?php echo $sbaidProximo; ?>');" type="button" name="btnProximo" value="Próxima" id="btnProximo"        <?php echo !$sbaidProximo ? 'disabled="disabled"' : ''; ?>/>
                </td>
              	</tr>
              	<?php } ?>
              </table>
              </td>
            </tr>

          </tbody>
        </table>
      </form>


<?php
if ($undid) {
    $sql = '
    SELECT
        il.labid
    FROM
        cte.laboratorio l
    INNER JOIN
        cte.itenslaboratorio il ON l.labid = il.labid
    WHERE
        l.undid = ' . $undid;

    if (($labid = $db->pegaUm($sql)) === false)
        $labid = 'null';
} else {
    $labid = 'null';
}


?>
<script type="text/javascript">
  
    /**
     * @public
     * @global
     */
    var IExplorer               = !!document.all;
    var safari                  = navigator.userAgent.indexOf( 'Safari' ) > 0;
    var gecko                   = navigator.product == 'Gecko';
    var _displayBlock           = IExplorer ? 'block' : 'table-row';

    var TAB_TOTALIZADORES       = '0000';
    var existemEscolasAtendidas = <?php echo $sbaporescola && $existemEscolasAtendidas ? 'true' : 'false' ?>;
    var anoExercicio            = '<?php echo ($_REQUEST['parsubacaoanocrt'])?$_REQUEST['parsubacaoanocrt']: max($_cosano);  ?>';
    var permitirCadastroSubacao = <?php echo trim($_REQUEST['sbaid']) != '' ? 'true' : 'false'; ?>;

    var itemModificado          = false;

    var totalizadorItensComposicao = 0;
    var totalizadorBeneficiarios   = 0;
    var totalizadorEscolas         = 0;
    var labid                      = <?php echo $labid; ?>;
    var total                      = 0;
    var PIVisivel	= <?php echo CTE_PRM_VISUALIZAR_PI ? CTE_PRM_VISUALIZAR_PI : 0; ?>;
    
    /***************** Aditivo ***************/
    var anoAtualTrava 	= <?php echo $anoAtualTrava;?>;
    var anoParecer2007 	= <?php echo $anoParecer2007;?>;
    var anoParecer2008 	= <?php echo $anoParecer2008;?>;
    var anoParecer2009 	= <?php echo $anoParecer2009;?>;
    var anoParecer2010 	= <?php echo $anoParecer2010;?>;
    var anoParecer2011 	= <?php echo $anoParecer2011;?>;

    var novaSub 	  = <?php echo $novaSub = CTE_NOVA_SUBACAO ? 'true' : 'false'; ?>; // Se for nova subação não trava os dados
    //var reqt 		  = '<?php echo $reqt; ?>';
    var porEscola     = '<?php echo $sbaporescola ? 't' : 'f'; ?>';
    var subacao		  = <?php echo $_REQUEST['sbaid'] ? $_REQUEST['sbaid'] : 0;  ?>;
    var eAditivo	  = <?php echo $_REQUEST['ad'] ? $_REQUEST['ad'] : 0; ?>;
    var eReformulacao = <?php echo $_REQUEST['ref'] ? $_REQUEST['ref'] : 0; ?>;
    var anoConvenio   = <?php echo $anoConvenio ? $anoConvenio : 0;?>;
    if( eAditivo || eReformulacao ){
    	 var anoConvenio = '<?php echo $anoConvenio ? $anoConvenio : $_REQUEST['anoconvenio'];?>';
    }
    
    /************** fim Aditivo *****************/

</script>

<script type="text/javascript" src="../brasilpro/includes/par_subacao.js"></script>

<script type="text/javascript" >
	function extratoSubAcacao()
    {
        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/relatorioParSubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>',
                          'extratoSubacao',
                          'height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }
	    
    function carregarAditivos(cosano)
    {	
        return new Ajax.Request(window.location.href,
                                {
                                    method: 'post',
                                    parameters: '&req=carregaAditivos&cosano=' + cosano,
                                    asynchronous: false,
                                    onComplete: function(res)
                                    {	
                                      // Controle do que deve ser travado de acordo com a situação da subação.
                                        <?php if (!CTE_NOVA_SUBACAO) { ?>  
                                        		if( (<?=$admTemp; ?> == 2) && (<?=$super;?> == 2)  ){ // **@@&& retirar depois o if apos reformulação - se não for administrator temporario trava. 
	           										if( ( <?= CTE_ESTADO_PAR?> == <?=$documento["esdid"]; ?> ) && <?php echo (integer) !cte_possuiPerfil( array( CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR ) ) ?>  ){
	           										
	           											travaAbaAnoJaAnalisada(cosano);
	            									}else if(<?=$anoConvenio; ?>){
	               										travaAbaAnoSeAditivada(cosano);
	               									}
	               									if( <?=$anoConvenio; ?> != cosano && !(<?=$super;?> == 1 || <?=$adm;?> == 1) ){
	               										travaAnosAnteriores(cosano);
	               									}
	               								}else{
	               									if(<?=$anoConvenio; ?> >= cosano ){ // **@@&& retirar depois o if apos reformulação se for 2007 - 2008 - 2009 mostra add aditivo 
	               												$('divAditivo' + cosano).style.display="table-row";//**@@&& Mostra botão de Adicionar Aditivo.  retirar apos a reformulação
	               									}
	               								}
            								<? } ?>
                                        $('divListaAditivo' + anoExercicio).innerHTML = res.responseText; // Mostra lista de Aditivos
                                    }
                                });
    }

<?php
//------------------------- FUNCOES RELATIVAS SOMENTE A CRONOGRAMAS POR ESCOLA
if ($sbaporescola) {
    if (CTE_PRM_EDITAR_SUBACAO) {
?>
    /**
     * 
     */
    function removerEscola(qfaid)
    {
        if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
            return false;
        }

        return new Ajax.Request(window.location.href,
                                   {
                                       method: 'post',
                                       parameters: '&req=removerEscola&qfaid=' + qfaid,
                                       onComplete: function(res)
                                       {
                                           carregarEscolas();
                                       }
                                   });

        return false;
    }

<?php
    } else {
        echo 'function removerEscola() {return false;}';
    }
?>

    /**
     * 
     */
    function extratoEscolas(qfaid)
    {
        /*@
        if (itemModificado)
            submeterFrmParSubacao(true,  '<?php echo $_REQUEST['sbaid']; ?>');
        */

        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/extratoescolassubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&qfaid=' + qfaid,
                          'extratoEscolas',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    /**
     * 
     */
    function carregarEscolas()
    {
        return new Ajax.Request(window.location.href,
                                {
                                    method: 'post',
                                    parameters: '&req=carregarEscolas&qfaano=' + anoExercicio,
                                    onComplete: function(res)
                                    {
                                        $('escolasSubAcao' + anoExercicio).innerHTML = res.responseText;
                                    }
                                });
    }


    /*!@
     * Funções relacionadas a visualização/manipulação de abas para itens
     * da composição da subação
     */
    function selecionarAba(cosano, carregarDados)
    {	
        if (itemModificado && !confirm(('Existem itens de composição modificados que não foram salvos.\n' +
                                        'Deseja continuar sem salvá-los?'))) {
            return false;
        } else {
            itemModificado = false;
        }

        var celula;
        var tabela;
        var totalizadores = cosano == '0000';
		
		// Se for aditivo
		if(eAditivo || eReformulacao){
        	anoExercicio = anoConvenio;
        	cosano		= anoConvenio;
        }
		
        // Última selecionada
		$('abaCosano' + anoExercicio).hide();
        $('aba_'      + anoExercicio).style.backgroundPosition            = '0% 0%';
        $('aba_'      + anoExercicio).firstChild.style.backgroundPosition = '0% 0%';

        $('abaCosano' + cosano)      .show();
        $('aba_'      + cosano)      .style.backgroundPosition            = '0% -150px';
        $('aba_'      + cosano)      .firstChild.style.backgroundPosition = '100% -150px';

        anoExercicio = cosano;

        if (totalizadores) {
            $('spanAno').innerHTML = 'Totalizadores';
            carregarTotalizadores();
        } else {
            if (carregarDados) {
                carregarItensComposicao();
                carregarBeneficiarios();
                carregarDadosParecer();
                carregarEscolas();
                if(!eAditivo && !eReformulacao){
                	carregarDivImportacao( cosano );
                }
                carregarAditivos(cosano);
            }

            $('spanAno').innerHTML = cosano;
        }
		
        return false;
    }
//------------------------ !FUNCOES RELATIVAS SOMENTE A CRONOGRAMAS POR ESCOLA
<?
} else {

?>
    /*!@
     * Funções relacionadas a visualização/manipulação de abas para itens
     * da composição da subação
     */
    function selecionarAba(cosano, carregarDados)
    {
        if (itemModificado && !confirm(('Existem itens de composição modificados que não foram salvos.\n' +
                                        'Deseja continuar sem salvá-los?'))) {

            return false;
        } else {
            itemModificado = false;
        }

        var celula;
        var tabela;
        var totalizadores = cosano == '0000';

        // Última selecionada
        $('abaCosano' + anoExercicio).hide();
        $('aba_'      + anoExercicio).style.backgroundPosition            = '0% 0%';
        $('aba_'      + anoExercicio).firstChild.style.backgroundPosition = '0% 0%';
        $('aba_'      + anoExercicio).firstChild.style.color              = '#333';

        $('abaCosano' + cosano)      .show();
        $('aba_'      + cosano)      .style.backgroundPosition            = '0% -150px';
        $('aba_'      + cosano)      .firstChild.style.backgroundPosition = '100% -150px';
        $('aba_'      + cosano)      .firstChild.style.color              = '#333';

        anoExercicio = cosano;

        if (totalizadores) {
            $('spanAno').innerHTML = 'Totalizadores';
            carregarTotalizadores();
        } else {
            if (carregarDados) {
                carregarItensComposicao();
                carregarBeneficiarios();
                carregarDadosParecer();
                if(!eAditivo && !eReformulacao){
                	carregarDivImportacao( cosano );
                }
                carregarAditivos(cosano);
            }

            $('spanAno').innerHTML = cosano;
        }

        return false;
    }
<?php

} // if ($sbaporescola) {

?>


    /**
     * 
     */
    function verificarModificaoItensComposicao(el)
    {
        if (!itemModificado)
            itemModificado = el.value != el.defaultValue;
    }


    /**
     * 
     */
    function regraFormaExec()
    {
        return true;
    }

    function exibirPlanoInterno(value)
    {
        new Ajax.Request(window.location.href,
                         {
                             parameters: 'req=prgplanointerno&prgplanointerno=' + value,
                             onComplete: function(e)
                             {
                                 $('prgplanointerno').value = e.responseText;
                             }
                         });
    }


    /**
     * 
     */
    function calcularValorTotal(cosid)
    {
    	var Hvlruni = string2float($('Hcosvlruni' + cosid).value);
    	var vlruni  = string2float($('cosvlruni' + cosid).value);

		if( Hvlruni.toFixed(2) != vlruni.toFixed(2) ){
	    	$('Hcosvlruni' + cosid).value = mascaraglobal('###.###.###.###.###,######', vlruni.toFixed(6) );
		}

		var uni   = string2float($('Hcosvlruni' + cosid).value).toFixed(6);
		var qtd   = string2float($('cosqtd'    + cosid).value).toFixed(6);

        var vlrtotal = parseFloat( uni ) * 
        			   parseFloat( qtd ) ;

        $('Hcosvlrtotal' + cosid).value = mascaraglobal('###.###.###.###.###,######', vlrtotal.toFixed(6) );
		$('cosvlrtotal' + cosid).value  = mascaraglobal('###.###.###.###.###,##'    , vlrtotal.toFixed(2) );

		// Totalizador Geral
		var totalGeral = 0;
        var arTotais = document.getElementsByName("Hcosvlrtotal");
        var qtdTotalArray = arTotais.length;
        
        for( i=0; i<qtdTotalArray; i++ ){

	        nr = string2float(arTotais[i].value);
	        nr = nr.toFixed(6);
//	        alert(nr);
        	totalGeral += parseFloat( nr );
        }
		
        $('valorTotalSubAtual'+ anoExercicio).value   = mascaraglobal('###.###.###.###.###,##', totalGeral.toFixed(2) );
        $('vlrTotalSubAtual'+ anoExercicio).innerHTML = 'R$ '+ mascaraglobal('###.###.###.###.###,##', totalGeral.toFixed(2) );

    }
    
    function calcularValorUnitario(cosid,ano)
    {        
		var Hvlrtotal = string2float($('Hcosvlrtotal' + cosid).value);
    	var vlrtotal  = string2float($('cosvlrtotal' + cosid).value);

		if( Hvlrtotal.toFixed(2) != vlrtotal.toFixed(2) ){
	    	$('Hcosvlrtotal' + cosid).value = vlrtotal.toFixed(6);
		}
		
		var total = string2float($('cosvlrtotal' + cosid).value).toFixed(6);
		var qtd   = string2float($('cosqtd'    + cosid).value).toFixed(6);

        var vlruni = parseFloat( total ) / 
        			 parseFloat( qtd ) ;
        
		$('Hcosvlruni' + cosid).value = mascaraglobal('###.###.###.###.###,######', vlruni.toFixed(6) );
		$('cosvlruni' + cosid).value  = mascaraglobal('###.###.###.###.###,##'    , vlruni.toFixed(2) );

		// Totalizador Geral
		var totalGeral = 0;
        var arTotais = document.getElementsByName("Hcosvlrtotal");
        var qtdTotalArray = arTotais.length;
        
        for( i=0; i<qtdTotalArray; i++ ){

	        nr = string2float(arTotais[i].value);
	        nr = nr.toFixed(6);
//	        alert(nr);
        	totalGeral += parseFloat( nr );
        }
		
        $('valorTotalSubAtual'+ anoExercicio).value   = mascaraglobal('###.###.###.###.###,##', totalGeral.toFixed(2) );
        $('vlrTotalSubAtual'+ anoExercicio).innerHTML = 'R$ '+ mascaraglobal('###.###.###.###.###,##', totalGeral.toFixed(2) );
        	

    }

    /**
     * 
     */
    function string2float(str)
    {
        if (str.indexOf(',') == -1)
            return parseFloat(str);
        else
            return parseFloat(str.replace(/\./g, '').replace(/,/g, '.'));
    }

    /**
     * 
     */
    function float2string(num)
    {
        return mascaraglobal('###.###.###.###.###,##', (parseFloat(num).toFixed(2)) + '');
    }


    /**
     * @private
     */
    //this._closeWindows = false;

<?php
if (!CTE_PRM_EDITAR_SUBACAO) {

?>
    function submeterFrmParSubacao(submit, vlrProximaSubacao)
    {    
        if (vlrProximaSubacao != '') {
            window.location.href = '/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&sbaid=' + vlrProximaSubacao + '&bloqueado=1';
        } else {
            window.opener.location.reload();
            window.opener.focus();
            window.close();
        }
    }

    Form.disable('frmParSubacao');

    $('btnCancelar').removeAttribute('disabled');
    $('btnProximo') .removeAttribute('disabled');
    $('btnAnterior').removeAttribute('disabled');

    //---------------------------------------------------------------- POP-UPS
    function popupSelecionarEscolas(qfaano)
    {
        return false;
    }

    function popupEditarSubacao()
    {
        return false;
    }

    function popupSelecionarEscolasItemComposicao(input, cosid)
    {
        return false;
    }

    function adicionarItensComposicao()
    {
        return false;
    }

    function adicionarItensComposicao()
    {
        return false;
    }

    function adicionarBeneficiarios()
    {
        return false;
    }

    function removerItemComposicao(cosid)
    {
        return false;
    }

    function removerBeneficiario(benid)
    {
        return false;
    }
<?php
} else {
?>

    var submeter = false;
    //---------------------------------------------------------------- POP-UPS
    /**
     * 
     */
    function adicionarItensComposicao()
    {
        //if (itemModificado && !confirm(('Existem itens de composição que não foram salvos. Ao adicionar novos itens, os dados modificados serão perdidos.\n' +
        //                                'Deseja continuar sem salvá-los?'))) {
        //    return false;
        //} else {
        if (itemModificado)
            submeterFrmParSubacao(true,  '<?php echo $_REQUEST['sbaid']; ?>');
		/*
        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/cadastraritenssubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&cosano=' + anoExercicio,
                          'adicionarItensComposicao',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
                */
        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/cadastraritenssubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&cosano=' + anoExercicio +'&sbaidpai='+ <?php echo $_REQUEST['sbaidpai'] ? $_REQUEST['sbaidpai'] : 0; ?> + '&anoconvenio='+<?php echo $_REQUEST['anoconvenio'] ? $_REQUEST['anoconvenio'] : 0; ?>+ '&ad='+<?php echo $_REQUEST['ad'] ? $_REQUEST['ad'] : 0; ?>,
                          'adicionarItensComposicao',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        //}
    }


    /**
     * 
     */
    function adicionarBeneficiarios()
    {
        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/cadastrarbeneficiariossubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&sabano=' + anoExercicio+'&sbaidpai='+ <?php echo $_REQUEST['sbaidpai'] ? $_REQUEST['sbaidpai'] : 0; ?> + '&anoconvenio='+<?php echo $_REQUEST['anoconvenio'] ? $_REQUEST['anoconvenio'] : 0; ?>+ '&ad='+<?php echo $_REQUEST['ad'] ? $_REQUEST['ad'] : 0; ?>,
                          'adicionarBeneficiarios',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    /**
     * 
     */
    function removerItemComposicao(cosid)
    {
        if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
            return false;
        }

        return new Ajax.Request(window.location.href,
                                   {
                                       method: 'post',
                                       parameters: '&req=removerItemComposicao&cosid=' + cosid,
                                       onComplete: function(res)
                                       {
                                           carregarItensComposicao();
                                       }
                                   });

        return false;
    }


    /**
     * 
     */
    function removerBeneficiario(benid)
    {
        if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
            return false;
        }

        var req = new Ajax.Request(window.location.href,
                                   {
                                       method: 'post',
                                       parameters: '&req=removerBeneficiario&benid=' + benid + '&sabano=' + anoExercicio,
                                       onComplete: function(res)
                                       {
                                           carregarBeneficiarios();
                                       }
                                   });

        return false;
    }

    /**
     * 
     */
    function popupSelecionarEscolas(qfaano)
    {
        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/cadastrarescolassubacao&acao=A&tipo=cadastrarEscolas&sbaid=<?php echo $_REQUEST['sbaid']; ?>&qfaano=' + qfaano,
                          'popupSelecionarEscolas',
                          'height=400,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        return false;
    }

    /**
     * 
     */
    function popupEditarSubacao()
    {
        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/popupeditarsubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>',
                          'editarSubacao',
                          'height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    /**
     * 
     */
    function popupSelecionarEscolasItemComposicao(input, cosid)
    {
        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/escolasescolhasubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&cosid='+cosid+'&qfaano='+anoExercicio,
                          'escolherEscolasItemComposicao',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no');
    }

    function submeterFrmParSubacao(submit, vlrProximaSubacao)
    {
    
        $('anoExercicio').value = anoExercicio;

		// Exclusão de sub-ação
        if (vlrProximaSubacao == '-1' && permitirCadastroSubacao) {
            if (confirm('Deseja realmente excluir a subação?')) {
                $('vlrProximaSubacao').value = vlrProximaSubacao;
                $('frmParSubacao').submit();
                return true;
            } else {
                return false;
            }
        }

		// Gravar sub-ação
        if (submit) {
        	 var numprocesso         = $('cvrnumprocesso_'	 + anoExercicio);
        
            if (Number($F('sptinicio_' + anoExercicio)) > Number($F('sptfim_' + anoExercicio))) {
                alert('O mês fim para o campo Cronograma Físico deve ser maior ou igual o mês inicial.');
                return false;
            }

            if (vlrProximaSubacao != '') {
                $('vlrProximaSubacao').value = vlrProximaSubacao;
            }
            
            var prgidteste         = $('prgidteste');
            if( (prgidteste.value && PIVisivel != 0 )  ){
				var plinumplanointerno = $('plinumplanointerno_' + anoExercicio);      
				if(!plinumplanointerno.value && $('semplanointerno'+ anoExercicio).value == 'true'){
					alert(  "Não foi possível gravar essa subação.\n"+
							"Selecionar um plano interno\n");
					return false;	
				}
			}

            var errors = new Array();

            if ($('sbacategoria') && $F('sbacategoria') == 0)
                errors.push('Categoria de Despesa');

            if ($('sbadsc')       && $F('sbadsc') == '')
                errors.push('Descrição');

            if ($('sbastgmpl')    && $F('sbastgmpl') == '')
                errors.push('Estratégia de Implementação');

            if ($('prgid')        && $F('prgid') == 0)
                errors.push('Programa');

            if ($('undid')        && $F('undid') == 0)
                errors.push('Unidade de Medida');

            if ($('frmid')        && $F('frmid') == 0)
                errors.push('Forma de Execução');

            //                                                              */
            if (errors.length > 0) {
                if (errors.length == 1)
                    alert('O campo ' + errors.join('') + ' é obrigatório!');
                else
                    alert('Os campos abaixo são obrigatórios:\n - '
                         +errors.join('\n - ')
                         +'\n\nPor favor corrija para continua!');

                return false;
            } else {
				<?php
				if (in_array($estadoDocumento['esdid'], array(30,31,26))) {
				?>
	                if (total <= 0) {
	                    alert('Em fase de análise financeira, a sub-ação deve conter Escolas, Itens de Composição e Beneficiários viculados.');
	                    return false;
	                }
				<?php
				} // if ($estadoDocumento['esdid'] == 59)@1823
				?>
				
                var submeter = true;
                var inputs   = $('frmParSubacao').getInputs('text');
                
				<?php if ($labid == 'null' && !CTE_NOVA_SUBACAO && CTE_PRM_EMITIR_PARECER ){ ?>
					if( $( "ssuid_" + anoExercicio ).value != 6 ){
		                for (var i = 0; i < inputs.length; i++) {
		                    if ( /cosqtd/   .test(inputs[i].getAttribute('name'))) {
		                        if (string2float(inputs[i].value) <= 0) {
		                            inputs[i].setStyle({backgroundColor: '#ffd'});
		                            submeter = false;
		                        } 
		                        else {
		                            inputs[i].setStyle({backgroundColor: '#fff'});
		                        }
		                    }
						}
	                }
				<?php } ?>
				
                if (submeter) {
                    $('frmParSubacao').submit();
                    return true;
                } 
                else {
                    alert('O dados para Quantidade dos Itens de Composição devem ser preenchidos.');
                    return false;
                }
            }
        } else {
            if (vlrProximaSubacao != '') {
                window.location.href = '/brasilpro/brasilpro.php?modulo=principal/par_subacao&acao=A&sbaid=' + vlrProximaSubacao;
            } else {
                window.opener.location.reload();
                window.opener.focus();
                window.close();
            }
        }
    }

<?php

} // if (!CTE_PRM_EDITAR_SUBACAO) {

if (trim($_REQUEST['sbaid']) != ''){
?>
    selecionarAba(anoExercicio, true);
<?php
	}else {
?>
    $("alertaPermitirCadastroItensSubacao").show();
    $("tabAbasSelecaoCosano").hide();
    $("btnExcluir").setAttribute("disabled", "disabled");
<?php
}

?>
    $('loader-container').hide();

<?php
if (in_array($estadoDocumento['esdid'], array(30,31,26))) {
?>
    new Ajax.PeriodicalUpdater('loader-container', window.location.href,
                               {
                                   frequency: 5,
                                   decay: 2,
                                   parameters: 'req=verificarInsercaoDadosSubacao&sbaporescola=<?php echo $sbaporescola ? 't' : 'f'; ?>',
                                   method: 'post',
                                   evalScripts:true
                               });
<?php
} // if ($estadoDocumento['esdid'] == 59)@1823



if (CTE_NOVA_SUBACAO) {
    echo '$(\'loader-container\').hide();';

} 
else {
?>

    function limparSubacao(sbaid)
    {
        if (confirm('Atenção!\nTodo o preenchimento do ano ' + anoExercicio + ' da subação será excluído.\n'
                   +'Os dados NÃO poderão ser recuperados posteriormente.\n\nDeseja continuar?'))
        {
            $('loader-container').show();
            $('frmParSubacao').request(
            {
                encoding: 'ISO-8859-1',
                parameters: {
                    anoExercicio: anoExercicio,
                    req: 'limparSubacao',
                    formularioSumit: 0
                },
                onComplete: function(response)
                {
                    eval(response.responseText);
                    $('loader-container').hide();
                }
            });
        }
    }


<?php } ?>

function carregarDivImportacao( anoCorrente ){
	
	var obDiv = document.getElementById( "divImportardados" );
	obDiv.innerHTML = '';
	
	var obLabel = document.createElement( "label" );
	obLabel.setAttribute( "for", "csAno" );
	obLabel.innerHTML = "Importar Dados de: ";
	
	var obSelect = document.createElement( "select" );
	obSelect.setAttribute( "id", "csAno" );
	obSelect.setAttribute( "name", "csAno" );
	
	var arAno = new Array();
	arAno[0] = 2008;
	arAno[1] = 2009;
	arAno[2] = 2010;
	arAno[3] = 2011;
		
	for( i=0; i<arAno.length; i++){
	
		if( arAno[i] == anoCorrente ) continue;
		
		var obOption = document.createElement( "option" );
		obOption.setAttribute( "value", arAno[i] );
		obOption.innerHTML = arAno[i];
		
		obSelect.appendChild( obOption );
	}
	
	var obButton = document.createElement( "input" );
	obButton.setAttribute( "type", "button" );
	obButton.setAttribute( "id", "btnImportar" );
	obButton.setAttribute( "name", "btnImportar" );
	obButton.setAttribute( "value", "Importar" );
	
	if(window.addEventListener){ // Mozilla, Netscape, Firefox
		obButton.setAttribute( "onclick", "javascript: importarDados();" );
	} 
	else { // IE
		obButton.attachEvent( "onclick", importarDados );
	}
	
	
	
	obDiv.appendChild( obLabel );
	obDiv.appendChild( obSelect );
	obDiv.appendChild( document.createElement( "br" ) );
	obDiv.appendChild( document.createElement( "br" ) );
	obDiv.appendChild( obButton );
	
		

}

function importarDados(){

	var csAno = document.getElementById( 'csAno' ).value;
	
	if( confirm( "Esta operação adicionará todos os dados de itens de composição e de escolas (se for o caso) do ano de " + csAno + " para o ano em questão!\n\nDeseja continuar?" ) ){
	
		$('loader-container').show();
		return new Ajax.Request(window.location.href,{
									method: 'post',
									parameters: '&req=importarDados&anoExercicio=' + anoExercicio + '&csAno=' + csAno,
									onComplete: function(res){
										eval( res.responseText );
                    					$('loader-container').hide();
									}
								});	
	}
}

function toggleDiv( idDiv ){

	var obDiv = document.getElementById( idDiv );

	if( obDiv.style.display == 'block' )
		obDiv.style.display = 'none';
	else
		obDiv.style.display = 'block';
	
}


<?php   // Aditivo
if($anoConvenio != 0000 or $_REQUEST['sbaidpai'] ){
?> 
	document.getElementById('BtnEditarSubacao').style.display="none";
	<?php if($_REQUEST['ad'] == 1 || $_REQUEST['ref'] == 1){ 
			if($_REQUEST['ad'] == 1){
				$escreve = " Aditivo ".$numAditivo." / ";
	?>
				document.getElementById('TdEditarSubacao').innerHTML="<b style='color:black;' >Criação & Edição do Ativio nº <?=$numAditivo;?> ano "+anoExercicio+"</b>";
	<?php 	}else{
				$escreve = " Reformulação ".$numReformulacao." / ";
	?>
				document.getElementById('TdEditarSubacao').innerHTML="<b style='color:black;' >Criação & Edição do Ativio nº <?=$numReformulacao;?> ano "+anoExercicio+"</b>";
	<?php 
			}
		  }else{ 		?>
		document.getElementById('TdEditarSubacao').innerHTML="<b style='color:red;' ></b>";
	<?php } ?>
	
	
<? } ?>

--></script>

<?php die(); ?>


