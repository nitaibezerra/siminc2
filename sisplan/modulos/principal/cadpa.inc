<?php

// direcionando o user pra uma pagina com os planejamentos se o exercicio for 2013 ou mais
if( (int) $_SESSION['exercicio'] >= 2013 ){
    echo "<script>
                window.location='sisplan.php?modulo=principal/consultarPlanejamento&acao=B';
          </script>";
}

if ( get_magic_quotes_gpc() == 1 ){
    foreach ($_REQUEST as $k => $v){
        $_REQUEST[$k] = (!is_array($v) ? stripslashes($v) : $v);
        $_REQUEST[$k] = (!is_array($v) ? pg_escape_string($v) : $v);
    }
}

if ($_REQUEST['op']){
//	$op = $_REQUEST['op'] == 'AP' ? 'A' : 'R';
	if ($_REQUEST['op'] == 'AP'){
		$op = 'A';
		$assunto = 'Solicitação de PA aprovada';

		$sql = sprintf( "SELECT atinumeropi FROM pde.atividade WHERE atiid = %d", $_REQUEST['atiid'] );
		$atinumeropi = (int)$db->pegaUm( $sql );
		if ( !$atinumeropi )
			$atinumeropi = (1 + $db->pegaUm("SELECT MAX(atinumeropi) FROM pde.atividade WHERE atianopi = '".$_SESSION['exercicio'] . "' AND atinumeropi is not null"));
	}else{
		$op = 'R';
		$atijustificativarecusa = "atijustificativarecusa = '" . $_REQUEST['justreprovacao'] . "',";
		$assunto = 'Solicitação de PA reprovada';
		$atinumeropi = '';
	}

	$sql = sprintf("UPDATE
						pde.atividade
					SET
						atistatuspi 	= '%s',
						usucpfsituacao  = '%s',
						%s
						atidatasituacao = now(),
						atinumeropi = %d
					WHERE
						atiid = '%s'"
				, $op
				, $_SESSION['usucpf']
				, $atijustificativarecusa
				, $atinumeropi
				, $_REQUEST['atiid']
				);
	try
	{
		$db->executar($sql);
		$db->commit();

		/* alerta por email retirado por Adonias em 21/03/2012 a pedido do Luiz Borges
		// envia email para os Super Usuários
		$message = montaTabelaEmail($_REQUEST['atiid'], $assunto);
		$sql = "SELECT
					usuemail,
					usunome
				FROM
					seguranca.usuario u
					JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf
				WHERE
					pu.pflcod = " . PERFIL_SUPERUSER;

		$dados = $db->carregar($sql);
		$dados = $dados ? $dados : array();

		foreach($dados as $d):
			email($d['usunome'], $d['usuemail'], $assunto, $message, $cc='',$cco='');
		endforeach;
		*/
		//
		// envia email para os gestores de PI
//		$message = montaTabelaEmail($_REQUEST['atiid'], $assunto);
//		$sql = "SELECT
//					usuemail,
//					usunome
//				FROM
//					seguranca.usuario u
//					JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf
//				WHERE
//					pu.pflcod = " . PERFIL_GESTORPI;
//
//		$dados = $db->carregar($sql);
//		$dados = $dados ? $dados : array();
//
//		foreach($dados as $d):
//			email($d['usunome'], $d['usuemail'], $assunto, $message, $cc='',$cco='');
//		endforeach;

		//
//		// envia email para o usuario cadastrador do PI
//		$sql = "SELECT
//					usuemail,
//					usunome
//				FROM
//					seguranca.usuario u
//				JOIN pde.atividade a ON u.usucpf = a.usucpf
//				WHERE
//					a.atiid = '" . $_REQUEST['atiid'] . "'";
//
//		$dados = $db->carregar($sql);
//		$dados = $dados ? $dados : array();
//
// 		foreach($dados as $d):
//			email($d['usunome'], $d['usuemail'], $assunto, $message, $cc='',$cco='');
//		endforeach;

		/* alerta por email retirado por Adonias em 21/03/2012 a pedido do Luiz Borges
		//
		// envia email para os representante do PI
		$sql = "SELECT
					atirepresentante,
					atiemailrepresentante
				FROM
					pde.atividade
				WHERE
					atiid= '" . $_REQUEST['atiid'] . "' AND atisnenviaemailtramitacao = 't'";

		$dados = $db->carregar($sql);
		$dados = $dados ? $dados : array();

		foreach($dados as $d):
			email($d['usunome'], $d['usuemail'], $assunto, $message, $cc='',$cco='');
		endforeach;
		*/
		//
		// fim da operacao de cadastramento do PI
		$db->sucesso( $_REQUEST['modulo'], '&atiid='. $_REQUEST['atiid'] );
	}
	catch ( Exception $e )
	{
//		dbg($e,1);
		email('adonias malosso', 'malosso@gmail.com', 'erro iphan aprovacao de pa', $e->getMessage(), '', '' );
		$db->rollback();
		die('<script>alert(\'Não foi possível realizar a operação!\'); history.go(-1);</script>');
	}

}

if ($_REQUEST['ajax'] == 1){
		$acaid = $db->pegaUm( sprintf( "SELECT acaid FROM monitora.acao WHERE acasnrap = false AND prgano = '%s' AND prgcod = '%s' AND acacod = '%s' AND unicod = '%s' AND loccod = '%s'"
										, $_SESSION['exercicio']
										, $_REQUEST['prgcod']
										, $_REQUEST['acacod']
										, $_REQUEST['unicod']
										, $_REQUEST['loccod']
										)
									);
		die($acaid);
}



// captura dados do projeto da requisição
if($_GET['acao']=="A" || $_GET['acao']=="I" || $_GET['acao']=="C")
{
	$atiid = $_REQUEST['atiid'] ? (integer) $_REQUEST['atiid'] : null;

	if($atiid !== null)
		$_SESSION['projeto'] = $atiid;

	if($atiid == null && $_REQUEST['acao'] == 'A')
		$atiid = $_SESSION['projeto'];

	// captura as inforamções da atividade
	$atividade = atividade_pegar( $atiid );
	$paiid = 1; // brasil


	$codigoPais = 1;
	$ufEstado = 'NULL';
	$ufMunicipio = 'NULL';
	$codigoMunicipio = 'NULL';
	$codigoRegiaoAdministrativa = 'NULL';

	switch ( $_REQUEST['esfid'] )
	{
		case 1: // federal - nao faz nada
		break;
		case 2: // estadual
			$ufEstado = "'" . $_REQUEST['estuf'] . "'";
		break;
		case 3: // municipal
			$ufEstado 		 = "'" . $_REQUEST['estuf'] . "'";
			$ufMunicipio 	 = "'" . $_REQUEST['estuf'] . "'";
			if ( $_REQUEST['estuf'] == 'DF' )
			{
				$codigoMunicipio = "'5300108'"; // Brasilia
				$codigoRegiaoAdministrativa = "'" . $_REQUEST['muncod'] . "'";
			}
			else
			{
				$codigoMunicipio = "'" . $_REQUEST['muncod'] . "'";
				$codigoRegiaoAdministrativa = 'NULL';
			}
		break;
		case 4: // exterior
			$paiid = $_REQUEST['paiid'] ? $_REQUEST['paiid'] : $paiid;
		break;
	}

	if ($_REQUEST['prgcod'] && $_REQUEST['acacod'] && $_REQUEST['unicod'] && $_REQUEST['loccod']){
		$acaid = $db->pegaUm( sprintf( "SELECT acaid FROM monitora.acao WHERE acasnrap = false AND prgano = '%s' AND prgcod = '%s' AND acacod = '%s' AND unicod = '%s' AND loccod = '%s'"
									, $_SESSION['exercicio']
									, $_REQUEST['prgcod']
									, $_REQUEST['acacod']
									, $_REQUEST['unicod']
									, $_REQUEST['loccod']
									)
								);
	}

	if ( !$atividade )
	{
		$permissao_formulario='S';
		if($_REQUEST['gravar'] && $_REQUEST['acao']!='C' )
		{
			//uf municipio
			$ufmunicipio = $_REQUEST['muncod'] ? $_REQUEST['estuf'] : null;
			$atiid = $db->pegaUm( "select nextval( 'pde.atividade_atiid_seq' )" );

			$atinumeropi = '';//(1 + $db->pegaUm("SELECT MAX(atinumeropi) FROM pde.atividade WHERE atianopi = '".$_SESSION['exercicio'] . "'"));


			$naturezaDespesa = is_array($_REQUEST['natureza']) ? $_REQUEST['natureza'] : array();

			(int)$custeio 	  = 0;
			(int)$investimento = 0;
			foreach ( $naturezaDespesa as $k=>$natureza )
			{
				$valor = str_replace( ',','.',str_replace('.','',$_REQUEST['valor'][$k]) );

				if ( substr($natureza,0,1) == 4 )
				{
					$investimento += $valor;//(int)str_replace('.','',$_REQUEST['valor'][$k]);
				}
				else
				{
					$custeio += $valor;//(int)str_replace('.','',$_REQUEST['valor'][$k]);
				}
			}

			$sql = sprintf(
				"insert into pde.atividade (
				  atiid,
				 _atiprojeto,
				  atipronac,
				  atidescricao,
				  atidetalhamento,
				  atidatainicio,
				  atidatafim,
				  usucpf,
				  atiordem,
				  atistatuspi,
				  atiproponente,
				  atirepresentante,
				  estuf,
				  estufmun,
				  muncod,
				  esfid,
				  atiemailrepresentante,
				  atiorcamentocusteio,
				  atiorcamentocapital,
				  atiorcamento,
				  atianopi,
				  atisnenviaemailtramitacao,
				  atipropameta,
				  atiprosecundariometa,
				  pduid,
				  udaid,
				  paiid,
				  preid,
				  aesid,
				  uexid,
				  areid,
				  segid,
				  aueid,
				  tppid,
				  atinumeroinstrumento,
				  tpiid,
				  tauid,
				  ppoid,
				  acaid,
				  atiprotocolo,
				  tscid,
				  rgaid,
				  pddid,
				  depid,
				  dfeid,
				  iteid,
				  stpid,
				  itrid,
				  clsid,
				  sbcid,
				  medlatitude,
				  medlongitude,
				  atiemenda,
				  atipronacquest,
				  atirecursoquest,
				  pltid
				  ) values (
				  %d,
				  %d,
				  '%s', -- atipronac
				  '%s', -- descricao
				  '%s', -- detalhamento
				  %s,   -- dataini
				  %s,   -- datafim
				  '%s', -- usucpf
				  0,    -- atiordem
				  'P',  -- atistatuspi
				  '%s', -- atiproponente
				  '%s', -- atirepresentante
				  %s, -- estuf
				  %s, -- estufmun
				  %s, -- muncod
				  '%s', -- esfid
				  '%s', -- atiemailrepresente
				  '%s', -- atiorcamentocusteio
				  '%s', -- atiorcamentocapital
				  '%s', -- atiorcamento
				  '%s', -- atianopi
				  '%s', -- atisnenviaemailtramitacao
				   %d, -- atipropameta
				   %d, -- atiprosecundariometa
				  '%s', -- pduid
				  '%s', -- udaid
				  '%s', -- paiid
				  '%s', -- preid
				  '%s', -- aesid
				  '%s', -- uexid
				  '%s', -- areid
				  '%s', -- segid
				  %s, -- aueid
				  %s, -- tppid
				  '%s', -- atinumeroinstrumento
				  %s, -- tpiid
				  %s, -- tauid
				  %s, -- ppoid
				  '%s', -- acaid
				  '%s', -- atiprotocolo
				  %s, -- tscid
				  %s, -- rgaid
				  %s, -- pddid
				  %s, -- depid
				  %s, -- dfeid
				  %s, -- iteid
				  %s, -- stpid
				  %s, -- itrid
				  %s, -- clsid
				  %s, -- sbcid
				  '%s', --medlatitude
				  '%s', --medlongitude
				  %s, 	--atiemenda
				  %s, 	--atipronacquest
				  %s, --atirecursoquest
				  %s --pltid
				  )",
				$atiid,
				$atiid,
				$_REQUEST['atipronac'],
				$_REQUEST['atidescricao'],
//				str_replace("'", '"',str_replace( "\\", '',$_REQUEST['atidetalhamento'])),
				$_REQUEST['atidetalhamento'],
//				pg_escape_string($_REQUEST['atidetalhamento']),
				$_REQUEST['atidatainicio'] ? "'" . $_REQUEST['atidatainicio'] . "'" : 'NULL',
				$_REQUEST['atidatafim'] ? "'" . $_REQUEST['atidatafim'] . "'" : 'NULL',
				$_SESSION['usucpf'],
				$_REQUEST['atiproponente'],
				$_REQUEST['atirepresentante'],
				$ufEstado ? $ufEstado : 'null',
				$ufMunicipio ? $ufMunicipio : 'null',
				$codigoMunicipio ? $codigoMunicipio : 'null',
				$_REQUEST['esfid'],
				$_REQUEST['atiemailrepresentante'],
				str_replace( ',', '', number_format($custeio,2,'.','') ),//(int)str_replace('.','',$custeio),
				str_replace( ',', '', number_format($investimento,2,'.','') ),//(int)str_replace('.','',$investimento),
	            (int)str_replace( ',', '.', str_replace('.','',$_REQUEST['item_financeiro_valor_total']) ),
				$_SESSION['exercicio'],
				$_REQUEST['atisnenviaemailtramitacao']=='true' ? 't' : 'f',
                (int) str_replace( ".", "", $_REQUEST['atipropameta'] ),
                (int) str_replace( ".", "", $_REQUEST['atiprosecundariometa'] ),
				$_REQUEST['pduid'],
				$_REQUEST['udaid'],
				$paiid,
				$_REQUEST['preid'],
				$_REQUEST['aesid'],
				$_REQUEST['uexid'],
				$_REQUEST['areid'],
				$_REQUEST['segid'],
				$_REQUEST['aueid'] ? "'" . $_REQUEST['aueid'] . "'" : 'NULL',
				$_REQUEST['tppid'] ? "'" . $_REQUEST['tppid'] . "'" : 'NULL',
				$_REQUEST['atinumeroinstrumento'],
				$_REQUEST['tpiid'] ? "'" . $_REQUEST['tpiid'] . "'" : 'NULL',
				$_REQUEST['tauid'] ? "'" . $_REQUEST['tauid'] . "'" : 'NULL',
				$_REQUEST['ppoid'] ? "'" . $_REQUEST['ppoid'] . "'" : 'NULL',
				$acaid,
				$_REQUEST['atiprotocolo'],
				$_REQUEST['tscid'] ? $_REQUEST['tscid'] : 'NULL',
				$codigoRegiaoAdministrativa,
				$_REQUEST['pddid'] ? $_REQUEST['pddid'] : 'null',
				$_REQUEST['depid'] ? $_REQUEST['depid'] : 'null',
				$_REQUEST['dfeid'] ? $_REQUEST['dfeid'] : 'null',
				$_REQUEST['iteid'] ? $_REQUEST['iteid'] : 'null',
				$_REQUEST['stpid'] ? $_REQUEST['stpid'] : 'null',
				$_REQUEST['itrid'] ? $_REQUEST['itrid'] : 'null',
				$_REQUEST['clsid'] ? $_REQUEST['clsid'] : 'null',
				$_REQUEST['sbcid'] ? $_REQUEST['sbcid'] : 'null',
				$_REQUEST['graulatitude'].'.'.$_REQUEST['minlatitude'].'.'.$_REQUEST['seglatitude'].'.'.$_REQUEST['pololatitude'],
				$_REQUEST['graulongitude'].'.'.$_REQUEST['minlongitude'].'.'.$_REQUEST['seglongitude'],
				$_REQUEST['atiemenda'],
				$_REQUEST['atipronacquest'],
				$_REQUEST['atirecursoquest'],
				$_REQUEST['pltid'] ? $_REQUEST['pltid'] : 'null'
			);
//			dbg($_REQUEST['atipropameta']);
//			dbg($sql,1);
			try
			{
				if ( $db->executar( $sql ) ) {

					/**
					 * Sincroniza com o MINC para inserir quando for clicado no botão Enviar para o MINC
					 */
					if( $_REQUEST['gravar_minc'] == 1 )
					{
						if ( $_REQUEST['atiid'] )
							toXmlMinc($_REQUEST['atiid'], 'Atividade');
						else
							toXmlMinc($atiid, 'Atividade');
					}

					if( $_REQUEST['gravar_tramitar'] == 1 )
					{
						$docid = sisplan_pegarDocid($atiid);
						$descricaoWorkflow = "";
						if(verificaAtividadeSemPlanejamento($atiid))
						{
							$descricaoWorkflow = "Tramitação inicial";
							$aedid = WORKFLOW_ACAO_ANALISE_PRESIDENCIA;
						}
						else
						{
							$aedid = WORKFLOW_ACAO_ANALISE_COORDENACAO_PLANEJAMENTO;
						}
						wf_alterarEstado($docid, $aedid, $descricaoWorkflow, array('atiid'=>$atiid));
					}

					$naturezaDespesa = is_array($_REQUEST['natureza']) ? $_REQUEST['natureza'] : array();

					//INSERE O ORÇAMENTO
					foreach ( $naturezaDespesa as $k=>$naturezaOrcamento )
					{

						if ( $naturezaOrcamento != "" )
						{
							$sql = "SELECT ndpid FROM public.naturezadespesa WHERE ndpcod = '". str_pad($naturezaOrcamento,8, '0', STR_PAD_RIGHT) ."' ";
							$ndpid = $db->pegaUm($sql,0);

							$valor = str_replace( ',', '.', str_replace('.','',$_REQUEST['valor'][$k]) );

							$sql = sprintf(
								"INSERT INTO pde.orcamentopa
									( ndpid, atiid, opavalor )
								 VALUES
									( %d, %d, %s)",
								$ndpid,
								$atiid,
								$valor );

							$db->executar( $sql );
						}
					}

					$fontes = is_array($_REQUEST['fonid']) ? $_REQUEST['fonid'] : array();

					//INSERE O ORÇAMENTO POR FONTE
					foreach ( $fontes as $k=>$fonteOrcamento )
					{
						if ( $fonteOrcamento != "" )
						{

							$valorfonte = str_replace( ',', '.', str_replace('.','',$_REQUEST['valor_fonte'][$k]) );

							$sql = sprintf(
								"INSERT INTO pde.orcamentopafonte
									( fonid, atiid, opfvalor )
								 VALUES
									( %s, %d, %s)",
								$fonteOrcamento,
								$atiid,
								$valorfonte );

							$db->executar( $sql );
						}
					}

					//INSERE ABRANGENCIA
					$abrpais = is_array($_REQUEST['pais']) ? $_REQUEST['pais'] : array();
					foreach ( $abrpais as $k=>$pais )
					{
						if ( $pais != "" )
						{
							$sql   = "SELECT paiid FROM territorios.pais WHERE paidescricao = '". $pais ."'";
							$paiid = $db->pegaUm($sql,0);

							$arrayuf = explode(" - ", $_REQUEST['uf'][$k]);
							$estuf   = $arrayuf[0];


							if ( $estuf == 'DF' )
							{
								$sql      = "select rgaid from planointerno.regiaoadministrativa where rgadsc = '". $_REQUEST['mun'][$k] ."'";
								$rgaid    = $db->pegaUm($sql,0);
								$estufmun = '';
								$muncod   = '';
							}
							else
							{
								$arraymun = explode(" - ", $_REQUEST['mun'][$k]);
								$sql      = "select estuf as estufmun, muncod from territorios.municipio where estuf = '". $arraymun[0] ."' and mundescricao = '". $arraymun[1] ."'";
								$estufmun = $db->pegaUm($sql,0);
								$muncod   = $db->pegaUm($sql,1);
							}

							$sql = sprintf(
								"INSERT INTO sisplan.regionalizacao
									( atiid, paiid, estuf, estufmun, muncod, rgaid )
								 VALUES
									( %s, %s, %s, %s, %s, %s )",
								$atiid,
								$paiid,
								$estuf ? "'".$estuf."'" : 'NULL',
								$estufmun ? "'".$estufmun."'" : 'NULL',
								$muncod ? "'".$muncod."'" : 'NULL',
								$rgaid ? $rgaid : 'NULL' );

							$db->executar( $sql );
						}
					}

					atividade_atribuir_responsavel( $atiid, PERFIL_GESTOR, array( $_SESSION['usucpf'] ) );

					$db->commit();

					// INÍCIO ENVIA EMAIL
					$assunto = 'Nova solicitação de PA';
					$message = montaTabelaEmail($atiid, $assunto);

//					$sql = "SELECT
//									usuemail,
//									usunome
//								FROM
//									seguranca.usuario u
//									JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf
//								WHERE
//									pu.pflcod = " . PERFIL_GESTORPI;
//
//						$dados = $db->carregar($sql);
//						$dados = $dados ? $dados : array();
//
//						foreach($dados as $d):
//							email($d['usunome'], $d['usuemail'], $assunto, $message, $cc='',$cco='');
//						endforeach;

/*
 * Comentado a pedido dia 26-03-2012 (Função que envia email) - INÍCIO
 */
//					$sql = "SELECT
//									usuemail,
//									usunome
//								FROM
//									seguranca.usuario u
//									JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf
//								WHERE
//									pu.pflcod = " . PERFIL_SUPERUSER;
//
//						$dados = $db->carregar($sql);
//						$dados = $dados ? $dados : array();
//
//						foreach($dados as $d):
//							email($d['usunome'], $d['usuemail'], $assunto, $message, $cc='',$cco='');
//						endforeach;
//
//
//						$sql = "SELECT
//									usuemail,
//									usunome
//								FROM
//									seguranca.usuario u
//								WHERE
//									u.usucpf= '" . $_SESSION['usucpf'] . "'";
//
//						$dados = $db->carregar($sql);
//						$dados = $dados ? $dados : array();
//
//						foreach($dados as $d):
//							email($d['usunome'], $d['usuemail'], $assunto, $message, $cc='',$cco='');
//						endforeach;
//
//						$sql = "SELECT
//									atirepresentante,
//									atiemailrepresentante
//								FROM
//									pde.atividade
//								WHERE
//									atiid= '" . $atiid . "' AND atisnenviaemailtramitacao = 't'";
//
//						$dados = $db->carregar($sql);
//						$dados = $dados ? $dados : array();
//
//						foreach($dados as $d):
//							email($d['usunome'], $d['usuemail'], $assunto, $message, $cc='',$cco='');
//						endforeach;
/*
 * Comentado a pedido dia 26-03-2012 (Função que envia email) - FIM
 */

/////////////// FIM ENVIA EMAIL

					$db->sucesso( $_REQUEST['modulo'] );
				}
			}
			catch ( Exception $e )
			{
				dbg($e,1);
				$db->rollback();
				die('<script>alert(\'Não foi possível realizar a operação!\'); history.go(-1);</script>');
			}

		}
	}
	else
	{
		$acaidUP = $acaid;

		$permissao = unidadeexecutora_verificar_responsabilidade($atividade['uexid']);//projeto_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
		$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário
//		dbg($atividade,1);
		extract( $atividade ); # mantém o formulário preenchido

		if ($acaid){
			$dados_acaid = $db->pegaLinha( sprintf( "SELECT a.prgcod, a.acacod, a.unicod, a.loccod, u.unmdsc, p.prodsc FROM monitora.acao a LEFT JOIN public.produto
			p ON p.procod = a.procod LEFT JOIN public.unidademedida u ON u.unmcod = a.unmcod
			WHERE acaid = '%s'", $acaid ) );

			extract($dados_acaid);
		}
		// efetiva a alteração dos dados
		if ( $_REQUEST['gravar'] ) {

			$naturezaDespesa = is_array($_REQUEST['natureza']) ? $_REQUEST['natureza'] : array();

			$custeio 	  = 0;
			$investimento = 0;
			foreach ( $naturezaDespesa as $k=>$natureza )
			{
				$valor = str_replace( ',','.',str_replace('.','',$_REQUEST['valor'][$k]) );
				if ( substr($natureza,0,1) == 4 )
				{
					$investimento += $valor;
				}
				else
				{
					$custeio += $valor;
				}
			}

			$sql = sprintf(
				"update pde.atividade set
				  atipronac = '%s',
				  atidescricao = '%s' ,
				  atidetalhamento = '%s' ,
				  atidatainicio = %s ,
				  atidatafim = %s ,
				  atiproponente = '%s' ,
				  atirepresentante = '%s' ,
				  estuf = %s ,
				  estufmun = %s ,
				  muncod = %s ,
				  esfid = '%s' ,
				  atiemailrepresentante = '%s' ,
				  atiorcamentocusteio = '%s' ,
				  atiorcamentocapital = '%s' ,
				  atiorcamento = '%s' ,
				  atianopi = '%s' ,
				  atisnenviaemailtramitacao = '%s' ,
				  atipropameta = %d ,
				  atiprosecundariometa = %d ,
				  pduid = '%s' ,
				  udaid = '%s' ,
				  paiid = '%s' ,
				  preid = '%s' ,
				  aesid = '%s' ,
				  uexid = '%s' ,
				  areid = '%s' ,
				  segid = '%s' ,
				  aueid = %s ,
				  tppid = %s ,
				  atinumeroinstrumento = '%s' ,
				  tpiid	= %s,
				  tauid	= %s,
				  ppoid	= %s,
				  acaid = '%s',
				  atistatuspi = '%s',
				  atiprotocolo = '%s',
				  tscid = %s,
				  rgaid = %s,
				  pddid = %s,
				  depid = %s,
				  dfeid = %s,
				  iteid = %s,
				  stpid = %s,
				  itrid = %s,
				  clsid = %s,
				  sbcid = %s,
				  medlatitude = '%s',
				  medlongitude = '%s',
				  atiemenda = %s,
				  atipronacquest = %s,
				  atirecursoquest = %s
				where
					atiid = %d",
				$_REQUEST['atipronac'],
				$_REQUEST['atidescricao'],
				$_REQUEST['atidetalhamento'],
				$_REQUEST['atidatainicio'] ? "'" . $_REQUEST['atidatainicio'] . "'" : 'NULL',
				$_REQUEST['atidatafim'] ? "'" . $_REQUEST['atidatafim'] . "'" : 'NULL',
				$_REQUEST['atiproponente'],
				$_REQUEST['atirepresentante'],
				$ufEstado ? $ufEstado : 'null',
				$ufMunicipio ? $ufMunicipio : 'null',
				$codigoMunicipio ? $codigoMunicipio : 'null',
				$_REQUEST['esfid'],
				$_REQUEST['atiemailrepresentante'],
				str_replace( ',', '', number_format($custeio,2,'.','') ),//str_replace('.','',$custeio),
				str_replace( ',', '', number_format($investimento,2,'.','') ),//str_replace('.','',$investimento),
	            		str_replace( ',', '.', str_replace('.','',$_REQUEST['item_financeiro_valor_total']) ),
				$_SESSION['exercicio'],
				$_REQUEST['atisnenviaemailtramitacao']=='true' ? 't' : 'f',
				(int)preg_replace('/[^0-9]/','',$_REQUEST['atipropameta'] ),
				(int)preg_replace('/[^0-9]/','',$_REQUEST['atiprosecundariometa'] ),
				$_REQUEST['pduid'],
				$_REQUEST['udaid'],
				$paiid,
				$_REQUEST['preid'],
				$_REQUEST['aesid'],
				$_REQUEST['uexid'],
				$_REQUEST['areid'],
				$_REQUEST['segid'],
				$_REQUEST['aueid'] ? "'" . $_REQUEST['aueid'] . "'" : 'NULL',
				$_REQUEST['tppid'] ? "'" . $_REQUEST['tppid'] . "'" : 'NULL',
				$_REQUEST['atinumeroinstrumento'],
				$_REQUEST['tpiid'] ? "'" . $_REQUEST['tpiid'] . "'" : 'NULL',
				$_REQUEST['tauid'] ? "'" . $_REQUEST['tauid'] . "'" : 'NULL',
				$_REQUEST['ppoid'] ? "'" . $_REQUEST['ppoid'] . "'" : 'NULL',
				$acaidUP,
				$atistatuspi == 'R' ? 'P' : $atistatuspi,
				$_REQUEST['atiprotocolo'],
				$_REQUEST['tscid'] ? $_REQUEST['tscid'] : 'NULL',
				$codigoRegiaoAdministrativa,
				$_REQUEST['pddid'] ? $_REQUEST['pddid'] : 'null',
				$_REQUEST['depid'] ? $_REQUEST['depid'] : 'null',
				$_REQUEST['dfeid'] ? $_REQUEST['dfeid'] : 'null',
				$_REQUEST['iteid'] ? $_REQUEST['iteid'] : 'null',
				$_REQUEST['stpid'] ? $_REQUEST['stpid'] : 'null',
				$_REQUEST['itrid'] ? $_REQUEST['itrid'] : 'null',
				$_REQUEST['clsid'] ? $_REQUEST['clsid'] : 'null',
				$_REQUEST['sbcid'] ? $_REQUEST['sbcid'] : 'null',
				$_REQUEST['graulatitude'].'.'.$_REQUEST['minlatitude'].'.'.$_REQUEST['seglatitude'].'.'.$_REQUEST['pololatitude'],
				$_REQUEST['graulongitude'].'.'.$_REQUEST['minlongitude'].'.'.$_REQUEST['seglongitude'],
				$_REQUEST['atiemenda'],
				$_REQUEST['atipronacquest'],
				$_REQUEST['atirecursoquest'],
				$_REQUEST['atiid']
			);
//dbg($sql,1);
			try
			{
				if ( $db->executar( $sql ) ) {

					/**
					 * Sincroniza com o MINC para inserir quando for clicado no botão Enviar para o MINC
					 */
					if( $_REQUEST['gravar_minc'] == 1 )
					{
						if ( $_REQUEST['atiid'] )
							toXmlMinc($_REQUEST['atiid'], 'Atividade');
						else
							toXmlMinc($atiid, 'Atividade');
					}

					if( $_REQUEST['gravar_tramitar'] == 1 )
					{
						$docid = sisplan_pegarDocid($atiid);
						$descricaoWorkflow = "";
						if(verificaAtividadeSemPlanejamento($atiid))
						{
							$descricaoWorkflow = "Tramitação inicial";
							$aedid = WORKFLOW_ACAO_ANALISE_PRESIDENCIA;
						}
						else
						{
							$aedid = WORKFLOW_ACAO_ANALISE_COORDENACAO_PLANEJAMENTO;
						}
						wf_alterarEstado($docid, $aedid, $descricaoWorkflow, array('atiid'=>$atiid));
					}


					//INSERE O ORÇAMENTO
					$sql = "DELETE FROM pde.orcamentopa WHERE atiid = '". $atiid ."' ";
					$db->executar($sql);
					$naturezaDespesa = is_array($_REQUEST['natureza']) ? $_REQUEST['natureza'] : array();
					foreach ( $naturezaDespesa as $k=>$naturezaOrcamento )
					{
						if ( $naturezaOrcamento != "" )
						{
							$sql = "SELECT ndpid FROM public.naturezadespesa WHERE ndpcod = '". $naturezaOrcamento ."00' ";

							$ndpid = $db->pegaUm($sql);
							if ( $ndpid )
							{
								$sql = sprintf(
									"INSERT INTO pde.orcamentopa
										( ndpid, atiid, opavalor )
									 VALUES
										( %d, %d, %s)",
									$ndpid,
									$atiid,
									str_replace( ',', '.', str_replace( '.', '', $_REQUEST['valor'][$k]) ) );
								$db->executar( $sql );
							}
						}
					}


					//INSERE O ORÇAMENTO POR FONTE
					$sql = "DELETE FROM pde.orcamentopafonte WHERE atiid = '". $atiid ."' ";
					$db->executar($sql);
					$fontes = is_array($_REQUEST['fonid']) ? $_REQUEST['fonid'] : array();
					foreach ( $fontes as $k=>$fonteOrcamento )
					{
						if ( $fonteOrcamento != "" )
						{

							$valorfonte = str_replace( ',', '.', str_replace('.','',$_REQUEST['valor_fonte'][$k]) );

							$sql = sprintf(
								"INSERT INTO pde.orcamentopafonte
									( fonid, atiid, opfvalor )
								 VALUES
									( %s, %d, %s)",
								$fonteOrcamento,
								$atiid,
								$valorfonte );

							$db->executar( $sql );
						}
					}


					//INSERE ABRANGENCIA
					$sql = "DELETE FROM sisplan.regionalizacao WHERE atiid = '". $atiid ."'";
					$db->executar($sql);
					$abrpais = is_array($_REQUEST['pais']) ? $_REQUEST['pais'] : array();
					foreach ( $abrpais as $k=>$pais )
					{
						if ( $pais != "" )
						{
							$sql   = "SELECT paiid FROM territorios.pais WHERE paidescricao = '". $pais ."'";
							$paiid = $db->pegaUm($sql,0);

							$arrayuf = explode(" - ", $_REQUEST['uf'][$k]);
							$estuf   = $arrayuf[0];


							if ( $estuf == 'DF' )
							{
								$sql      = "select rgaid from planointerno.regiaoadministrativa where rgadsc = '". $_REQUEST['mun'][$k] ."'";
								$rgaid    = $db->pegaUm($sql,0);
								$estufmun = '';
								$muncod   = '';
							}
							else
							{
								$arraymun = explode(" - ", $_REQUEST['mun'][$k]);
								$sql      = "select estuf as estufmun, muncod from territorios.municipio where estuf = '". $arraymun[0] ."' and mundescricao = '". $arraymun[1] ."'";
								$estufmun = $db->pegaUm($sql,0);
								$muncod   = $db->pegaUm($sql,1);
							}

							$sql = sprintf(
								"INSERT INTO sisplan.regionalizacao
									( atiid, paiid, estuf, estufmun, muncod, rgaid )
								 VALUES
									( %s, %s, %s, %s, %s, %s )",
								$atiid,
								$paiid,
								$estuf ? "'".$estuf."'" : 'NULL',
								$estufmun ? "'".$estufmun."'" : 'NULL',
								$muncod ? "'".$muncod."'" : 'NULL',
								$rgaid ? $rgaid : 'NULL' );

							$db->executar( $sql );
						}
					}




					$_REQUEST['apoio'] = array_diff( $_REQUEST['apoio'], $_REQUEST['gerente'] ); # impede que os usuários redebam atribuição dupla de responsabilidade
					if ( !empty( $_REQUEST['gerente'] ) ) {
						atividade_atribuir_responsavel( $_REQUEST['atiid'], PERFIL_GESTOR, $_REQUEST['gerente'] );
					}
					atividade_atribuir_responsavel( $_REQUEST['atiid'], PERFIL_EQUIPE_APOIO_GESTOR, $_REQUEST['apoio'] );
					$db->commit();
					$db->sucesso( $_REQUEST['modulo'], '&atiid='. $_REQUEST['atiid'] );
				} else {
					$db->rollback();
					$db->insucesso( $_REQUEST['modulo'], '&atiid='. $_REQUEST['atiid'] );
				}
				exit();
			}
			catch ( Exception $e )
			{
						dbg($e,1);

				$db->rollback();
				die('<script>alert(\'Não foi possível realizar a operação!\'); history.go(-1);</script>');
			}

		}
	}
}

if ( $_GET['pltid'] || $pltid ){
    $planejamento = new PlanejamentoTatico( ($_GET['pltid'] ? $_GET['pltid'] : $pltid) );

    if ( $planejamento->pltstatus != 'A' ){
        die("<script>
                alert('O planejamento passado não é válido!');
                history.go(-1);
             </script>");
    }
    if ( !empty($_GET['pltid']) ){
        extract( $planejamento->getDados() );

        $atidescricao       = $pltnome;
        $atidetalhamento    = $pltobservacao;

        if ( !empty($muncod) ){
            $atividade['esfid'] = ESFERA_MUNICIPAL;
            $esfid              = ESFERA_MUNICIPAL;
        }elseif ( !empty($estuf) ){
            $atividade['esfid'] = ESFERA_ESTADUAL_DISTRITO_FEDERAL;
            $esfid              = ESFERA_ESTADUAL_DISTRITO_FEDERAL;
        }else{
            $atividade['esfid'] = ESFERA_FEDERAL;
            $esfid              = ESFERA_FEDERAL;
        }
    //    $atividade['prgcod'] = $prgcod;
    //    $atividade['acacod'] = $acacod;
        $atividade['estuf']  = $estuf;
        $atividade['muncod'] = $muncod;
    }else{
        $pltcusteio = $planejamento->pltcusteio;
        $pltinvestimento = $planejamento->pltinvestimento;
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );
?>
<script language="javascript" type="text/javascript" src="../includes/micoxAjax.js"></script>
<script type="text/javascript" src="../includes/prototype-1.6.0.3.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script>
jQuery.noConflict();

/**
 *
 */
function aplicarMascaraCpfCnpj(e)
{
    var el = e;

    if (el.value.length <= 14) {
        el.value = mascaraglobal("###.###.###-##", el.value);
    } else {
        el.value = mascaraglobal("##.###.###/####-##", el.value);
    }

    return el;
}


function formata_decimal2( valor )
{

		valor += '';
	// retira os caracteres que não são numericos
	for ( var i = 0 ; i < valor.length; )
	{
		if ( isNaN( valor.charAt( i ) ) == true )
		{
			valor = valor.replace( valor.charAt( i ), '' );
			continue;
		}
		i++;
	}
	// retira zeros a esquerda
	while ( valor.charAt( 0 ) == '0' )
	{
		valor = valor.substr( 1 );
	}
	// adiciona zeros a esquerda caso haja necessidade
/*	switch ( valor.length )
	{
		case 0:
			valor = '000' + valor;
			break;
		case 1:
			valor = '00' + valor;
			break;
		case 2:
			valor = '0' + valor;
			break;
	}
*/
	return mascaraglobal( '##.###.###.###,##', valor );

}
function formata_decimal3( valor )
{

	//	valor += '';
	// retira os caracteres que não são numericos
	return mascaraglobal( '###.###.###.###,##', valor );

}

function listar_ptres(id)
{
	if ( id != '' )
		ajaxGet('../ajax/lista_campos_ptres.php?ptres='+id,document.getElementById('span_ptres'),true);
}
</script>
<?
switch ( $atistatuspi )
{
	case 'P':
		$situacao = 'Pendente';
	break;
	case 'A':
		$situacao = 'Aprovado';
	break;
	case 'R':
		$situacao = 'Recusado';
	break;
}

$sql_financeiro_natureza = "SELECT SUBSTR(ndpcod,1,6) AS codigo, ndpdsc AS descricao FROM naturezadespesa WHERE edpcod != '00' AND ndpstatus = 'A' AND (sbecod = '0' OR sbecod = '00') ORDER BY ndpcod";

$sql_financeiro_validacao = "SELECT prgcod||acacod||unicod||loccod||natureza AS codigo, natureza AS descricao FROM sisplan.financeiro WHERE finstatus = 'A' AND exercicio = '".$_SESSION['exercicio']."' ORDER BY natureza";

$sql_financeiro_fonte = "SELECT fonid ||' # '|| fondsc AS codigo, fondsc AS descricao FROM sisplan.fonterecurso WHERE fonstatus = 'A' ORDER BY fonid";

?>
<form action="" method="post" name="formulario">
	<input type="hidden" name="atiid" value="<?= $atiid ?>"/>
	<input type="hidden" name="pltid" value="<?= $pltid ?>"/>
	<input type="hidden" name="pltcusteio" id="pltcusteio" value="<?= number_format($pltcusteio,2,',','.') ?>"/>
	<input type="hidden" name="pltinvestimento" id="pltinvestimento" value="<?= number_format($pltinvestimento,2,',','.') ?>"/>
	<input type="hidden" name="gravar" value="1"/>
	<input type="hidden" name="gravar_minc" value=""/>
	<input type="hidden" id="gravar_tramitar" name="gravar_tramitar" value=""/>
	<input type="hidden" name="ususenha" value="<?=$_SESSION['senha_minc']?>"/>
	<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">Projeto:</td>
		</tr>
		<!--<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Protocolo:</td>
			<td><input type="hidden" name="protocolo" id="protocolo" value="<?=$atiprotocolo;?>" /><?= campo_texto('atiprotocolo','N',$permissao_formulario,'',40,20,'','','','','','id="atiprotocolo"'); ?> <span id="resultprotocolo"></span></td>
		</tr>-->
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Número do Processo IPHAN:</td>
			<td><?= campo_texto('atiprotocolo','N',$permissao_formulario,'',25,20,'#####.######/####-##',''); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Situação:</td>
			<td><?=$situacao ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Título:</td>
			<td><?= campo_textarea( 'atidescricao', 'S', $permissao_formulario, '', 70, 3, 200 ); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Descrição:</td>
			<td><?= campo_textarea( 'atidetalhamento', 'S', $permissao_formulario, '', 70, 10, 500 ); ?></td>
		</tr>



		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">PPA:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">PTRES:</td>
			<td>
				<?=campo_texto('ptres','N',$permissao_formulario,'',12,5,'#####','','','Informe o número PTRES.','','id="ptres"');?>
				<span id="span_ptres"></span>
				<script type="text/javascript">
				$("ptres").observe("blur", function(event) {
				listar_ptres($("ptres").value);});
				</script>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Programa:</td>
			<td><?php
				$sql = sprintf( "SELECT distinct p.prgcod as codigo, p.prgcod || ' - ' || prgdsc AS descricao FROM monitora.programa p INNER JOIN monitora.acao a ON a.prgid = p.prgid WHERE a.unicod = '%s' and p.prgano = '%d' AND prgstatus = 'A' ORDER BY 2", UNIDADE_IPHAN, $_SESSION['exercicio'] );
				$db->monta_combo("prgcod",$sql,$permissao_formulario,'&nbsp','listar_acoes','','','580','S','prgcod');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ação:</td>
			<td><?php

				$sql = array();
				if ( usuario_possui_perfil( PERFIL_COORDENADOR_ACAO_SIGPLAN ) )
				{
					$sql = "
                    SELECT
                        ur.acacod
                    FROM sisplan.usuarioresponsabilidade ur
                    WHERE ur.usucpf = '". $_SESSION['usucpf'] . "'
                      AND ur.acacod is not null
                      AND ur.rpustatus = 'A'";

                    $acacodArr = (array) $db->carregarColuna($sql);
                    if ($acacodArr[0]){
                        $acacodWhere = " AND acacod IN ('" . ( implode("','", $acacodArr) ) . "')";
                        $sql = sprintf( "SELECT distinct acacod as codigo, acacod || ' - ' || acadsc AS descricao FROM monitora.acao WHERE acasnrap = false AND prgano = '%d' AND acacod IN ('" . ( implode("','", $acacodArr) ) . "') ORDER BY 2", $_SESSION['exercicio'] );
                    }
				}
				if ( $db->testa_superuser() ||
					 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_PLANEJAMENTO ) ||
					 usuario_possui_perfil( PERFIL_COORDENADOR_PLANEJAMENTO ) ||
					 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_ORCAMENTO ) ||
					 usuario_possui_perfil( PERFIL_COORDENADOR_ORCAMENTO ) ) {
					$sql = sprintf( "SELECT distinct acacod as codigo, acacod || ' - ' || acadsc AS descricao FROM monitora.acao a WHERE unicod = '%s' and acasnrap = false AND prgano = '%d' ORDER BY 2", UNIDADE_IPHAN, $_SESSION['exercicio'] );
				}
				$db->monta_combo("acacod",$sql,$permissao_formulario,'&nbsp','listar_unidades','','','580','S','acacod');
				//echo  obrigatorio();
			?><span id="span_acacod"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Unidade Orçamentária:</td>
			<td><?php

				if ( $acacod )
					$sql = sprintf( "select distinct u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from public.unidade u join monitora.acao a on a.unicod = u.unicod join monitora.programa p on a.prgano = p.prgano and p.prgano = '%d' and a.acacod = '%s' order by 2", $_SESSION['exercicio'], $acacod );
				else
					$sql = sprintf( "select distinct u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from public.unidade u join monitora.acao a on a.unicod = u.unicod join monitora.programa p on a.prgano = p.prgano and p.prgano = '%d' order by 2", $_SESSION['exercicio'] );
				$db->monta_combo("unicod",$sql,$permissao_formulario,'&nbsp','listar_localizadores','','','580','S','unicod');
				//echo  obrigatorio();
			?><span id="span_unicod"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Localizador:</td>
			<td><?php
					if ( $prgcod && $acacod && $unicod )
					$sql = sprintf( "SELECT DISTINCT a.loccod AS codigo, l.loccod || ' - ' || l.locdsc AS descricao FROM public.localizador l JOIN monitora.acao a ON a.loccod = l.loccod WHERE a.prgano = '%d' AND a.unicod = '%s' AND a.prgcod = '%s' AND a.acacod = '%s' ORDER BY 2", $_SESSION['exercicio'], $unicod, $prgcod, $acacod );
				else
					$sql = "SELECT DISTINCT l.loccod AS codigo, l.loccod || ' - ' || l.locdsc AS descricao FROM public.localizador l JOIN monitora.acao a ON a.loccod = l.loccod ORDER BY 2";
				$db->monta_combo("loccod",$sql,$permissao_formulario,'&nbsp','listar_produto','','','580','S','loccod');
				//echo  obrigatorio();
			?><span id="span_loccod"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Produto PPA:</td>
			<td>
				<table width="600" border="0" cellpadding="2" cellspacing="2">
					<tr>
						<td width="150" class="SubTituloCentro" style="background-color: #DCDCDC">Produto</td>
						<td width="150" class="SubTituloCentro" style="background-color: #DCDCDC">Unidade Medida</td>
						<td width="110" class="SubTituloCentro" style="background-color: #DCDCDC">Meta Prevista</td>
						<td width="110" class="SubTituloCentro" style="background-color: #DCDCDC">Meta a Realizar</td>
					</tr>
					<tr>
						<td id="produtoppa" style="text-align:center"><input type="hidden" name="codprg" id="codprg" value="<?=$procod;?>" /><?= campo_texto('prodsc','N','N','',30,20,'','','','','','id="prodsc"'); ?> <span id="span_prodsc"></span> </td>
						<td id="unidademedidappa" style="text-align:center"><input type="hidden" name="unmcod" id="unmcod" value="<?=$unmcod;?>" /><?= campo_texto('unmdsc','N','N','',30,20,'','','','','','id="unmdsc"'); ?> <span id="span_unmcod"></span></td>
						<td id="metaprevista" style="text-align:center"><input type="hidden" name="acaqtdprevisto" id="acaqtdprevisto" value="<?=$acaqtdprevisto;?>" /><?= campo_texto('acaqtdprevistoanocorrente','N','N','',20,20,'','','','','','id="acaqtdprevistoanocorrente"'); ?> <span id="span_acaqtdprevistoanocorrente"></span></td>
						<td style="text-align:center"><?= campo_texto('atipropameta','S',$permissao_formulario,'',12,10,'###.###.###','','','Informe a quantidade a ser executada.','','id="atipropameta"'); ?></td>
					</tr>
				</table>

				<span id="codprg"></div>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Produto Secund&aacute;rio:</td>
			<td>
				<table width="560" border="0" cellpadding="0" cellspacing="2">
					<tr>
						<td width="240" class="SubTituloCentro" style="background-color: #DCDCDC">Produto</td>
						<td width="220" class="SubTituloCentro" style="background-color: #DCDCDC">Unidade Medida</td>
						<td width="100" class="SubTituloCentro" style="background-color: #DCDCDC">Meta</td>
					</tr>
					<tr>
						<td style="text-align:center"><?php
							$sql = "SELECT pduid AS codigo, pdudsc AS descricao FROM planointerno.produto WHERE pdustatus = 'A'";
							$db->monta_combo( "pduid",$sql,$permissao_formulario,"&nbsp;",'','', '', '', 'S', "pduid" );
						?></td>
						<td style="text-align:center"><?php
							$sql = "SELECT udaid AS codigo, udadsc AS descricao FROM planointerno.unidademedida WHERE udastatus = 'A'";
							$db->monta_combo( "udaid",$sql,$permissao_formulario,"&nbsp;",'','', '', '', 'S', "udaid" );
						?></td>
						<td style="text-align:center"><?= campo_texto('atiprosecundariometa','S',$permissao_formulario,'',12,10,'###.###.###','','','Informe a quantidade a ser executada.','','id="atiprosecundariometa"'); ?></td>
					</tr>
				</table>
			</td>
		</tr>
		<!--<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Produto/Meta:</td>
			<td><?php
			/*	$sql = array();
				if ( $acacod )
					$sql = sprintf( "select distinct p.procod as codigo, p.prodsc as descricao from monitora.acao a inner join public.produto p ON p.procod = a.procod where a.prgano = '%s' and a.acacod = '%s' ORDER BY 2", $_SESSION['exercicio'], $acacod );
				else
					$sql = sprintf( "select distinct p.procod as codigo, p.prodsc as descricao from monitora.acao a inner join public.produto p ON p.procod = a.procod where a.prgano = '%s' ORDER BY 2", $_SESSION['exercicio'] );
				$db->monta_combo("procod",$sql,$permissao_formulario,'&nbsp','','','','580','N','procod');
				//echo  obrigatorio(); */
			?><span id="span_codprg"></span></td>
		</tr>-->


		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">Dados de Gestão:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Unidade Executora:</td>
			<td><?php
				$sql = '';
				if ( usuario_possui_perfil(PERFIL_GESTOR_UNIDADE_DESCENTRALIZADA) || usuario_possui_perfil(PERFIL_TECNICO_UNIDADE_DESCENTRALIZADA) )
				{
					$sql = "SELECT * FROM planointerno.unidadeexecutora_uo WHERE uexid in ( SELECT ur.uexid FROM sisplan.usuarioresponsabilidade ur WHERE ur.usucpf = '".$_SESSION['usucpf']."' AND ur.uexid is not null AND ur.rpustatus = 'A')";
				}

				if ( usuario_possui_perfil(PERFIL_APOIO_DIRETOR_DEPARTAMENTO) || usuario_possui_perfil(PERFIL_TECNICO_DEPARTAMENTO) || usuario_possui_perfil(PERFIL_DIRETOR_DEPARTAMENTO) )
				{
					$sql = "SELECT * FROM planointerno.unidadeexecutora_uo WHERE uexid in ( SELECT ur.uexid FROM sisplan.usuarioresponsabilidade ur WHERE ur.usucpf = '".$_SESSION['usucpf']."' AND ur.uexid is not null AND ur.rpustatus = 'A') or uexid = ( SELECT uexid FROM seguranca.usuario where usucpf = '" . $_SESSION['usucpf']."')";
				}

				if ( $db->testa_superuser() ||
					 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_PLANEJAMENTO ) ||
					 usuario_possui_perfil( PERFIL_COORDENADOR_PLANEJAMENTO ) ||
					 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_ORCAMENTO ) ||
					 usuario_possui_perfil( PERFIL_COORDENADOR_ORCAMENTO ) ) {
					$sql = "SELECT * FROM planointerno.unidadeexecutora_uo ";
				 }
				$db->monta_combo("uexid",$sql,$permissao_formulario,'&nbsp','','','','','S','uexid');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Departamento:</td>
			<td>
			<select name="depid" id="depid" class="CampoEstilo" style="width: 200px">
			</select>
				<?php

				//$sql = sprintf( "SELECT depid as codigo, depsigla || ' - ' || depdsc as descricao FROM sisplan.departamento where depstatus = 'A'" );

				//$db->monta_combo("depid",$sql,$permissao_formulario,'&nbsp','','','','','S','depid');
				echo  obrigatorio();
			?><span id="span_depid"></span></td>
		</tr>


		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">Prioridade:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Desafio Estratégico:</td>
			<td><?php
				$sql = "SELECT dfeid AS codigo, dfedsc AS descricao FROM sisplan.desafioestrategico WHERE dfestatus = 'A' ORDER BY 2";
				$db->monta_combo("dfeid",$sql,$permissao_formulario,'&nbsp','listar_iniciativas','','','','S','dfeid');
				//echo  obrigatorio();
			?><span id="span_dfeid"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Iniciativa estratégica:</td>
			<td>
<?php

				$sql = array();
				if ( $dfeid )
					$sql = sprintf( "SELECT iteid AS codigo, itedsc AS descricao FROM sisplan.iniciativaestrategica WHERE itestatus = 'A' and dfeid = '%d' ORDER BY 2", $_GET['acacod'] );
				else
					$sql = sprintf( "SELECT iteid AS codigo, itedsc AS descricao FROM sisplan.iniciativaestrategica WHERE itestatus = 'A' ORDER BY 2");

				$db->monta_combo("iteid",$sql,$permissao_formulario,'&nbsp','','','','','S','iteid');
				//echo  obrigatorio();
			?><span id="span_iteid"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Programas estratégicos:</td>
			<td><?php
				$sql = sprintf( "SELECT preid AS codigo, predsc AS descricao FROM planointerno.programaestrategico WHERE prestatus = 'A' ORDER BY 2" );
				$db->monta_combo("preid",$sql,$permissao_formulario,'&nbsp','listar_acoes_estrategicas','','','','S','preid');
				//echo  obrigatorio();
			?><span id="span_preid"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ações estratégicas:</td>
			<td><?php
				if ( $preid )
					$sql = sprintf( "SELECT aesid AS codigo, aesdsc AS descricao FROM planointerno.acaoestrategica WHERE aesstatus = 'A' AND preid =%d ORDER BY 2", $preid );
				else
					$sql = "SELECT aesid AS codigo, aesdsc AS descricao FROM planointerno.acaoestrategica WHERE aesstatus = 'A' ORDER BY 2";
				$db->monta_combo("aesid",$sql,$permissao_formulario,'&nbsp','','','','','S','aesid');
				//echo  obrigatorio();
			?><span id="span_aesid"></span></td>
		</tr>
		<!--
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Prioridade:</td>
			<td>
			<?php
				$sql = "SELECT pddid as codigo, pdddsc as descricao FROM sisplan.prioridade ORDER BY 1";
				$db->monta_combo("pddid",$sql,$permissao_formulario,'&nbsp','','','','','','pddid');
			?>
			<?=obrigatorio();?>
			</td>
		</tr>
		-->



		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">Caracterização do Projeto:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Classe:</td>
			<td><?php
				$sql = "SELECT clsid AS codigo, clsdsc AS descricao FROM sisplan.classe WHERE clsstatus = 'A' ORDER BY 2";
				$db->monta_combo("clsid",$sql,$permissao_formulario,'&nbsp','listar_subclasses','','','','','clsid');
				//echo  obrigatorio();
			?><span id="span_clsid"></span></td>
		</tr>

		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Sub-classe:</td>
		<td>
<?php
				$sql = array();
				if ( $clsid )
					$sql = sprintf( "SELECT sbcid AS codigo, sbcdsc AS descricao FROM sisplan.subclasse WHERE sbcstatus = 'A' and clsid = '%d' ORDER BY 2", $_GET['clsid'] );
				else
					$sql = sprintf( "SELECT sbcid AS codigo, sbcdsc AS descricao FROM sisplan.subclasse WHERE sbcstatus = 'A' ORDER BY 2");

				$db->monta_combo("sbcid",$sql,$permissao_formulario,'&nbsp','','','','','','sbcid');
				//echo  obrigatorio();
			?><div id="span_sbcid"></div></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Área:</td>
			<td><?php
				$sql = "SELECT areid AS codigo, aredsc AS descricao FROM planointerno.area WHERE arestatus = 'A' ORDER BY 2";
				$db->monta_combo("areid",$sql,$permissao_formulario,'&nbsp','','','','','S','areid');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Segmento:</td>
			<td><?php
				$sql = "SELECT segid AS codigo, segdsc AS descricao FROM planointerno.segmento WHERE segstatus = 'A' ORDER BY 2";
				$db->monta_combo("segid",$sql,$permissao_formulario,'&nbsp','','','','','S','segid');
				//echo  obrigatorio();
			?></td>
		</tr>




		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">Regionaliza&ccedil;&atilde;o:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Localização da ação:</td>
			<td>
				<?php
					$sql = "SELECT esfid AS codigo, esfdsc AS descricao FROM planointerno.esfera";
					$db->monta_combo( "esfid",$sql,$permissao_formulario,"&nbsp;",'action_esfera','', '', '', 'S', "esfid" );
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Pa&iacute;s:</td>
			<td>
				<?php
					$sql = "SELECT paiid AS codigo, paidescricao AS descricao FROM territorios.pais ORDER BY 2";
					$db->monta_combo("paiid",$sql,$permissao_formulario,'&nbsp','','','','','','paiid');
					//echo  obrigatorio();
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Unidade Federativa:</td>
			<td>
				<div id="estuf_on" style="<?php if($atividade['esfid']==1 || $atividade['esfid']==''){ ?>display:none;<?php }else{ ?>display:block;<?php } ?>">
				<?php
					$sql = "SELECT estuf AS codigo, estuf||' - '||estdescricao AS descricao FROM territorios.estado ORDER BY 2";
					$db->monta_combo("estuf",$sql,$permissao_formulario,'&nbsp','listar_municipios','','','','','estuf');
					//echo  obrigatorio();
				?>
				</div>
				<div id="estuf_off" style="color:#909090;">&nbsp;</div>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Município:</td>
			<td>
				<div id="muncod_on" style="<?php if($atividade['esfid']==1 || $atividade['esfid']==2 || $atividade['esfid']==''){ ?>display:none;<?php } else { echo "display:block;"; } ?>">
					<?
						if ( $atividade['estuf'] == 'DF' )
						{
							$sql = "SELECT rgaid, rgaid as codigo, rgadsc as descricao FROM planointerno.regiaoadministrativa ORDER BY 3 ";
						}
						else
						{
							$sql = "SELECT estuf, muncod as codigo, estuf || ' - ' || mundescricao as descricao FROM territorios.municipio WHERE estuf='".$atividade['estuf']."' ORDER BY 3 ";
						}
						$db->monta_combo("muncod",$sql,$permissao_formulario,"&nbsp;",'','','','','','muncod');
					?>
				</div>
				<div id="muncod_off" style="color:#909090;">&nbsp;</div>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Detalhar regionalização:</td>
			<td>
				<input type="checkbox" onclick="exibeAbrangencia();" id="atisndetalharregionalizacao" name="atisndetalharregionalizacao" <?php if($permissao_formulario=='N') { echo "disabled"; } ?> value="true" <?php if($atisndetalharregionalizacao=="t"){ echo " checked "; } ?>/> Marque esta opção para informar múltiplos valores de regionalização.
			</td>
		</tr>
		<tr id="trabrangencia" style="display:none">
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Abrangência:</td>
			<td>
				<table width="80%" align="rigth" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<thead>
						<tr>
							<td width="30%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>País</strong></td>
							<td width="30%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Unidade Federativa</strong></td>
							<td width="30%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Município</strong></td>
							<td width="10%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
						</tr>
					</thead>
						<tr bgcolor="#f7f7f7">
							<td>
								<div id="pais_on">
									<? $sql = "SELECT paidescricao AS codigo, paidescricao AS descricao FROM territorios.pais ORDER BY 2"; ?>
									<? $db->monta_combo("abrpaiid",$sql,$permissao_formulario,'&nbsp','','','',200,'','abrpaiid'); ?>
								</div>
								<div id="pais_off"></div>
							</td>
							<td>
								<div id="abrestuf_on" >
									<? $sql = "SELECT estuf||' - '||estdescricao AS codigo, estuf||' - '||estdescricao AS descricao FROM territorios.estado ORDER BY 2"; ?>
									<? $db->monta_combo("abrestuf",$sql,$permissao_formulario,'&nbsp','listar_municipios_abrangencia','','',200,'','abrestuf'); ?>
								</div>
								<div id="abrestuf_off"></div>
							</td>
							<td align="right">
								<div id="abrmuncod_on" >
									<?
									//dbg($atividade['estuf']);
										if ( $atividade['abrestuf'] == 'DF' )
										{
											$sql = "SELECT rgaid, rgadsc as codigo, rgadsc as descricao FROM planointerno.regiaoadministrativa ORDER BY 3 ";
										}
										else
										{
											$sql = "SELECT estuf, estuf || ' - ' || mundescricao as codigo, estuf || ' - ' || mundescricao as descricao FROM territorios.municipio WHERE estuf='".$atividade['estuf']."' ORDER BY 3 ";
										}
										$db->monta_combo("abrmuncod",$sql,$permissao_formulario,"&nbsp;",'','','','','','abrmuncod');
									?>
								</div>
								<div id="abrmuncod_off"></div>
							</td>
							<td align="left">
								<a href="javascript:item_abrangencia_adicionar_do_formulario();" title="Incluir" style="margin: 0; padding: 0 6px;">
									<img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
								</a>
							</td>
						</tr>
					<tr>
						<td colspan="4">
							<div class="div_rolagem" id="div_tabela_abrangencia" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
								<table id="tabela_abrangencia" width="100%" align="left" border="0" cellspacing="0" cellpadding="2">
									<tr valign="bottom">
										<td width="30%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="32%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="30%" style="height: 0; margin: 0; padding: 0;"></td>
										<td width="3%" style="height: 0; margin: 0; padding: 0;"></td>
									</tr>
								</table>
							</div>
						</td>
					</tr>
				</table>
			</td>
		</tr>

	</tr>
	<tr>
		<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">Localização Geográfica da Ação</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Latitude</td>
		<td><?php
		//$medlatitude = $endereco->medlatitude;
		$latitude = explode(".", $medlatitude);
		$graulatitude = $latitude[0];
		$minlatitude = $latitude[1];
		$seglatitude = $latitude[2];
		$pololatitude = $latitude[3];

		?> <?= campo_texto( 'graulatitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="graulatitude"'); ?>
		° <?= campo_texto( 'minlatitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="minlatitude" '); ?>
		' <?= campo_texto( 'seglatitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="seglatitude" '); ?>
		'' <select name="pololatitude" id="pololatitude" class="CampoEstilo"
			style="width: 40px;">
			<option value="S"
			<?php if ($pololatitude == "S") {echo 'selected="selected"';} ?>>S</option>
			<option value="N"
			<?php if ($pololatitude == "N") {echo 'selected="selected"';} ?>>N</option>
		</select>

		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Longitude</td>
		<td><?php
		//$medlongitude = $endereco->medlongitude;
		$longitude = explode(".", $medlongitude);
		$graulongitude = $longitude[0];
		$minlongitude = $longitude[1];
		$seglongitude = $longitude[2];
		?> <?= campo_texto( 'graulongitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="graulongitude"'); ?>
		° <?= campo_texto( 'minlongitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="minlongitude"'); ?>
		' <?= campo_texto( 'seglongitude', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="seglongitude"');  ?>
		'' &nbsp W

		</td>
	</tr>
	<?php
	#if($graulongitude != "" && $minlongitude != "" && $seglongitude != "" && $graulatitude != "" && $minlatitude != "" && $seglatitude != ""){ ?>
	<tr>
		<td class="SubTituloDireita"></td>
		<td><a href="#" onclick="abreMapa();">Visualizar / Buscar No Mapa</a> <input style="display:none;" type="text" name="endereco[endzoom]"id="endzoom" value=<? if ($entidade->enderecos[0]->endzoom ==null) echo "15"; else echo $entidade->enderecos[0]->endzoom;?>>
		</td>
	</tr>
	<?php #} ?>




		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">PRONAC<br><input type="radio" id="atipronacquest_sim" name="atipronacquest" <? if ($atipronacquest == 't' || !$atipronacquest ) echo 'checked'; ?> value="true" onclick="verificar_pronac(this.value)" />SIM <input type="radio" id="atipronacquest_nao" name="atipronacquest" <? if ($atipronacquest == 'f') echo 'checked'; ?> value="false" onclick="verificar_pronac(this.value)" />NÃO</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Nº do PRONAC:</td>
			<td><input type="hidden" name="pronac" id="pronac" value="<?=$atipronac;?>" /><?= campo_texto('atipronac','N',$permissao_formulario,'',40,20,'','','','','','onblur=\'verificarPronac()\' id="atipronac"'); ?> <span id="resultpronac"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Nome do Proponente:</td>
			<td><?= campo_texto('atiproponente','N',$permissao_formulario,'',70,70,'',''); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Tipo:</td>
			<td><?php
				$sql = "SELECT tppid AS codigo, tppdsc AS descricao FROM planointerno.tipoproponente WHERE tppstatus = 'A' ORDER BY 2";
				$db->monta_combo("tppid",$sql,$permissao_formulario,'&nbsp','','','','','','tppid');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Forma de Seleção:</td>
			<td><?php
				$sql = "SELECT tscid AS codigo, tscdsc AS descricao FROM planointerno.tiposetorcultural WHERE tscstatus = 'A' ORDER BY 2";
				$db->monta_combo("tscid",$sql,$permissao_formulario,'&nbsp','','','','','','tscid');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Enviar email de tramitação:</td>
			<td>
				<input type="checkbox" name="atisnenviaemailtramitacao" <?php if($permissao_formulario=='N') { echo "disabled"; } ?> value="true" <?php if($atisnenviaemailtramitacao=="t"){ echo " checked "; } ?>/> Marque esta opção para que o representante também seja informado por email quando da aprovação do PI. Se marcado deve ser informado o nome e o email do representante.

			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Nome do Representante:</td>
			<td><?= campo_texto('atirepresentante','N',$permissao_formulario,'',40,70,'',''); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">E-mail do Representante:</td>
			<td><?= campo_texto('atiemailrepresentante','N',$permissao_formulario,'',40,70,'',''); ?></td>
		</tr>



		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">Emenda Parlamentar<br><input type="radio" id="atiemenda_sim" name="atiemenda" <? if ($atiemenda == 't' || !$atiemenda ) echo 'checked'; ?> value="true" onclick="verificar_emenda(this.value)" />SIM <input type="radio" id="atiemenda_nao" name="atiemenda" <? if ($atiemenda == 'f') echo 'checked'; ?> value="false" onclick="verificar_emenda(this.value)" />NÃO</td>
		</tr>

		<div id="div_tabela_emenda_sim">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Tipo de Autor:</td>
			<td><?php
				$sql = "SELECT tauid AS codigo, taudsc AS descricao FROM planointerno.tipoautor WHERE taustatus = 'A' ORDER BY 2";
				$db->monta_combo("tauid",$sql,$permissao_formulario,'&nbsp','listar_partidos','','','','','tauid');
				//echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Partido:</td>
			<td><?php
				if ( $tauid )
					$sql = sprintf( "SELECT DISTINCT p.ppoid AS codigo, p.pposigla || ' - ' || p.ppodsc AS descricao FROM planointerno.partidopolitico p JOIN planointerno.autoremenda a ON p.ppoid = a.ppoid WHERE p.ppostatus = 'A' AND a.tauid = %d ORDER BY 2", $tauid );
				else
					$sql = "SELECT ppoid AS codigo, pposigla || ' - ' || ppodsc AS descricao FROM planointerno.partidopolitico WHERE ppostatus = 'A' ORDER BY 2";
				$db->monta_combo("ppoid",$sql,$permissao_formulario,'&nbsp','listar_autores','','','','','ppoid');
				//echo  obrigatorio();
			?><span id="span_ppoid"></span></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Autor:</td>
			<td><?php
				if ( $tauid && $ppoid )
					$sql = sprintf( "SELECT aueid AS codigo, auedsc AS descricao FROM planointerno.autoremenda WHERE auestatus = 'A' AND tauid = %d AND ppoid = %d ORDER BY 2", $tauid, $ppoid );
				else
					$sql = "SELECT aueid AS codigo, auedsc AS descricao FROM planointerno.autoremenda WHERE auestatus = 'A' ORDER BY 2";
				$db->monta_combo("aueid",$sql,$permissao_formulario,'&nbsp','','','','','','aueid');
				//echo  obrigatorio();
			?><span id="span_aueid"></span></td>
		</tr>
		</div>


		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">Orçamento	e Contratação:</td>
		</tr>
		<!--<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Tipo:</td>
			<td><?php
				$sql = "SELECT tpiid AS codigo, tpidsc AS descricao FROM planointerno.tipoinstrumento WHERE tpistatus = 'A' ORDER BY 2";
				$db->monta_combo("tpiid",$sql,$permissao_formulario,'&nbsp','','','','','','tpiid');
				//echo  obrigatorio();
			?></td>
		</tr>-->
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Situação do processo de contratação:</td>
			<td><?php
				$sql = "SELECT stpid AS codigo, stpdsc AS descricao FROM sisplan.situacaoprocesso WHERE stpstatus = 'A' ORDER BY 2";
				$db->monta_combo("stpid",$sql,$permissao_formulario,'&nbsp','','','','','','stpid');
				echo  obrigatorio();
			?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Forma de Contratação:</td>
			<td>
				<select style="width: 200px;" class="CampoEstilo" name="itrid">
					<option value=""></option>
					<optgroup label="Execução Direta">
						<?php
							$sql = sprintf( "select itrid, itrdsc from sisplan.instrumento where tpiid = 2 order by itrdsc asc" );
						?>
						<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
						<option value="<?= $tipo['itrid'] ?>" <? if ($itrid == $tipo['itrid'] ) echo 'selected'; ?> ><?= $tipo['itrdsc'] ?></option>
						<?php endforeach; ?>
					</optgroup>
					<optgroup label="Execução Indireta">
						<?php
							$sql = sprintf( "select itrid, itrdsc from sisplan.instrumento where tpiid = 1 order by itrdsc asc" );
						?>
						<?php foreach( $db->carregar( $sql ) as $tipo ): ?>
						<option value="<?= $tipo['itrid'] ?>" <? if ($itrid == $tipo['itrid'] ) echo 'selected'; ?> ><?= $tipo['itrdsc'] ?></option>
						<?php endforeach; ?>
					</optgroup>
				</select>
				<?=obrigatorio(); ?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Número:</td>
			<td><?= campo_texto('atinumeroinstrumento','N',$permissao_formulario,'',40,20,'','','','Informe o número do conv&ecirc;nio, contrato de repasse ou do identificador do instrumento em quest&atilde;o','','id="atinumeroinstrumento"'); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Data de Início:</td>
			<td><?= campo_data( 'atidatainicio', 'S', $permissao_formulario, '', 'S', 'Informe a data prevista de in&iacute;cio dos trabalhos ou de assinatura.' ); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Data de Término:</td>
			<td><?= campo_data( 'atidatafim', 'S', $permissao_formulario, '', 'S', 'Informe a data prevista de t&eacute;rmino dos trabalhos.' ); ?></td>
		</tr>
		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">Valor Estimado:</td>
		</tr>
		<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top">Orçamento IPHAN:</td>
		<td colspan="2">
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
			<tr>
			<td>
			<table width="95%" align="rigth" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>
						<td width="25%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Natureza</strong></td>
						<td width="35%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor (R$ 1,00)</strong></td>
						<td width="35%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
					</tr>
				</thead>
					<tr bgcolor="#f7f7f7">
						<td>
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'natureza\' );" '; ?>
							<? texto_popup( 'nova_natureza', $sql_financeiro_natureza, 'Natureza da Despesa', 6, 13, '######', '', $complemento ); ?>
						</td>
						<td align="right">
							<? $complemento = ' onkeypress="item_financeiro_alterar_campo_focado( event, \'valor\' );" '; ?>
							<?= campo_texto( 'novo_valor', 'N', 'S', '', 25, 14, '##.###.###.###,##', '', 'right', '', 0, ' id="novo_valor" onblur="MouseBlur(this);" ' . $complemento ); ?>
						</td>
						<td align="left">
							<a href="javascript:item_financeiro_adicionar_do_formulario();" title="Incluir" style="margin: 0; padding: 0 6px;">
								<img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
							</a>
						</td>
					</tr>
				<tr>
					<td colspan="3">
						<div class="div_rolagem" id="div_tabela_financeiro" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
							<table id="tabela_financeiro" width="100%" align="left" border="0" cellspacing="0" cellpadding="2">
								<tr valign="bottom">
									<td width="27%" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="38%" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="3%" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="100%" style="height: 0; margin: 0; padding: 0;"></td>
								</tr>
							</table>
						</div>
					</td>
				</tr>
				<tr valign="bottom">
					<td align="right" style="color: #ababab; border-top: 3px solid #dfdfdf;">Total</td>
					<td align="right" style="color: #0066cc; border-top: 3px solid #dfdfdf;" valign="bottom">
						<?= campo_texto( 'item_financeiro_valor_total', 'N', 'N', 'Valor Total', 25, 20, '##.###.###.###,##', '', 'right', '', 0, ' id="item_financeiro_valor_total" ' ) ?>
					</td>
					<td style="border-top: 3px solid #dfdfdf;">&nbsp;</td>
				</tr>
			</table>
			</td>
			<td>
			<?php echo obrigatorio(); ?>
			</td>
			</tr>
			</table>
		</td>
		</tr>



		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">
			Envolve recursos de outras fontes além do IPHAN<br>
			<input type="radio" id="atirecursoquest_sim" name="atirecursoquest" <? if ($atirecursoquest == 't') echo 'checked'; ?> value="true" onclick="verificar_recursos(this.value)" />SIM
			<input type="radio" id="atirecursoquest_nao" name="atirecursoquest" <? if ($atirecursoquest == 'f' || !$atirecursoquest) echo 'checked'; ?> value="false" onclick="verificar_recursos(this.value)" />NÃO</td>
		</tr>
		<tr style="display: none" id="orcamentoFontes">
			<td align='right' class="SubTituloDireita" style="vertical-align:top">Orçamento por Fontes:</td>
			<td colspan="2">
			<table width="80%" align="rigth" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>
						<td width="25%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Fonte de Recurso</strong></td>
						<td width="35%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; text-align: right;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor (R$ 1,00)</strong></td>
						<td width="35%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
					</tr>
				</thead>
					<tr bgcolor="#f7f7f7">
						<td>
							<? $complemento = ' onkeypress="item_financeiro_fonte_alterar_campo_focado( event, \'fonte\' );" '; ?>
							<? $db->monta_combo("nova_fonte",$sql_financeiro_fonte,$permissao_formulario,'&nbsp','','','',200,'','nova_fonte'); ?>
						</td>
						<td align="right">
							<? $complemento = ' onkeypress="item_financeiro_fonte_alterar_campo_focado( event, \'valor_fonte\' );" '; ?>
							<?= campo_texto( 'novo_valor_fonte', 'N', 'S', '', 25, 14, '##.###.###.###,##', '', 'right', '', 0, ' id="novo_valor_fonte" onblur="MouseBlur(this);" ' . $complemento_valor ); ?>
						</td>
						<td align="left">
							<a href="javascript:item_financeiro_fonte_adicionar_do_formulario();" title="Incluir" style="margin: 0; padding: 0 6px;">
								<img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
							</a>
						</td>
					</tr>
				<tr>
					<td colspan="3">
						<div class="div_rolagem" id="div_tabela_financeiro_fonte" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
							<table id="tabela_financeiro_fonte" width="100%" align="left" border="0" cellspacing="0" cellpadding="2">
								<tr valign="bottom">
									<td width="27%" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="38%" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="3%" style="height: 0; margin: 0; padding: 0;"></td>
									<td width="100%" style="height: 0; margin: 0; padding: 0;"></td>
								</tr>
							</table>
						</div>
					</td>
				</tr>
				<tr valign="bottom">
					<td width="25%" align="right" style="color: #ababab; border-top: 3px solid #dfdfdf;">Total</td>
					<td width="35%" align="right" style="color: #0066cc; border-top: 3px solid #dfdfdf;" valign="bottom">
						<?= campo_texto( 'item_financeiro_fonte_valor_total', 'N', 'N', 'Valor Total', 25, 20, '##.###.###.###,##', '', 'right', '', 0, ' id="item_financeiro_fonte_valor_total" ' ) ?>
					</td>
					<td width="35%" style="border-top: 3px solid #dfdfdf;">&nbsp;</td>
				</tr>
			</table>
			</td>
		</tr>

		<tr style="display: none" id="orcamentoFontes">
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">

					Orçamento Total: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;	<?= campo_texto( 'item_financeiro_total', 'N', 'N', 'Valor Total do Orçamento', 25, 20, '##.###.###.###,##', '', 'right', '', 0, ' id="item_financeiro_total" ' ) ?>
					&nbsp;&nbsp;&nbsp;
			</td>
		</tr>
		</td>
		</tr>

		<tr>
			<td colspan="3" class="SubTituloCentro" style="background-color: #DCDCDC">Equipe do projeto:</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Gestor:</td>
			<td>
				<?php
					if(!empty($atividade['atiid']))
					{
						$sql_gerente = "
							select
								u.usucpf as codigo,
								u.usucpf || ' - ' || u.usunome as descricao
							from seguranca.usuario u
								inner join pde.usuarioresponsabilidade ur on
									ur.usucpf = u.usucpf
								inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
							where
								ur.rpustatus = 'A' and
								ur.pflcod = '" . PERFIL_GESTOR . "' and
								atiid = " . $atividade['atiid'] . "
							order by u.usucpf
						";

						$gerente = $db->carregar( $sql_gerente );
						$gerente = $gerente ? $gerente : array();
					}
					$sql_combo = "
						select
							u.usucpf as codigo,
							u.usucpf || ' - ' || u.usunome as descricao
						from seguranca.usuario u
						inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
						where
							( u.suscod = 'A' or u.suscod = 'P' ) and
							us.suscod = 'A' and
							us.sisid = ". $_SESSION['sisid'] ."
						group by u.usucpf, u.usunome";
					combo_popup(
						'gerente',
						$sql_combo,
						'Selecione o(s) Identificador(es) de Uso',
						'400x400',
						1,
						array(),
						$permissao_formulario,
						$permissao_formulario,
						true,
						false,
						3
					);
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Equipe:</td>
			<td>
				<?php
					if( $atividade['atiid'] )
					{
						$sql_apoio = "
							select
								u.usucpf as codigo,
								u.usucpf || ' - ' || u.usunome as descricao
							from seguranca.usuario u
								inner join pde.usuarioresponsabilidade ur on
									ur.usucpf = u.usucpf
								inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
							where
								ur.rpustatus = 'A' and
					--			ur.pflcod = '" . PERFIL_EQUIPE_APOIO_GESTOR . "' and
								atiid = " . $atividade['atiid'] . "
							order by u.usucpf
						";

						$apoio = $db->carregar( $sql_apoio );
						$apoio = $apoio ? $apoio : array();
					}

					$sql_combo = "
						select
							u.usucpf as codigo,
							u.usucpf || ' - ' || u.usunome as descricao
						from seguranca.usuario u
						inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf
						where
							( u.suscod = 'A' or u.suscod = 'P' ) and
							us.suscod = 'A' and
							us.sisid = ". $_SESSION['sisid'] ."
						group by u.usucpf, u.usunome";
					combo_popup(
						'apoio',
						$sql_combo,
						'Selecione o(s) Identificador(es) de Uso',
						'400x400',
						0,
						array(),
						$permissao_formulario,
						$permissao_formulario,
						true
					);
				?>
			</td>
		</tr>
		<?
		if ( $atijustificativarecusa != '' && $atistatuspi == 'R' ):
			$justreprova = utf8_decode($atijustificativarecusa);
		?>
		<tr id="tr_just">
		<?
		else:
		?>
		<tr style="display:none;" id="tr_just">
		<?
		endif;
		?>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Motivo da Recusa:</td>
			<td><?= campo_textarea( 'justreprova', 'S', 'S', '', 70, 3, 500 ); ?></td>
		</tr>
		<?php if( $permissao_formulario ): ?>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
			<td colspan="2">
			<? if ( verifica_estado_pa($atiid) || $atiid == null ): ?>
				<input type="button" name="botao_gravar" value="Gravar" onclick="enviar(0,0);"/>
			<?php $estadoAtual = wf_pegarEstadoAtual(sisplan_pegarDocid($atiid));?>
			<?php if ($estadoAtual['esdid'] == WORKFLOW_EM_ELABORACAO ) :?>
				<input type="button" name="botao_gravar_tramitar" value="Gravar e Tramitar" onclick="enviar(0,1);"/>
			<? 	  endif; ?>
			<? endif; ?>
				<input type="button" name="botao_cancelar" value="Cancelar" onclick="cancelar();"/>
			</td>
		</tr>
		<?php endif; ?>
	</table>
</form>
<form action="" name="formreprova" method="post">
<input type="hidden" name="justreprovacao">
<input type="hidden" name="op">
</form>
<script type="text/javascript">


function exibeAbrangencia()
{
	document.getElementById('abrpaiid').value = 'Brasil';

	if (document.getElementById('trabrangencia').style.display == 'none' )
	{
		document.getElementById('trabrangencia').style.display = '';
	}
	else
	{
		document.getElementById('trabrangencia').style.display = 'none';
	}
}


function listar_municipios_abrangencia( regcod )
{
	var campo_select = document.getElementById( 'abrmuncod' );
	while( campo_select.options.length )
	{
		campo_select.options[0] = null;
	}
	campo_select.options[0] = new Option( '', '', false, true );

	ajaxGet('../ajax/lista_mun_abran.php?abrestuf='+regcod,document.getElementById('abrmuncod'),true);
}

//////////////////////////////////////////////////////////


function aprovar(param){
	if (param == 1){
		param = 'AP';
	}else{
		if (document.getElementById('tr_just').style.display == 'none' || document.getElementById('justreprova').value == '' ){
			document.getElementById('tr_just').style.display = '';
			alert('Preencha a justificativa pela reprovação.');
			return;
		}else{
			param = 'RP';
			document.formreprova.justreprovacao.value = document.getElementsByName('justreprova')[0].value;
			document.formreprova.op.value = param;
			document.formreprova.submit();
			return;
		}
	}
	location.href = '?modulo=principal/cadpa&acao=I&atiid=<?=$atiid ?>&op=' + param;
}

	function listar_acoes( prgcod )
	{
		var campo_select = document.getElementById( 'acacod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if( prgcod != '' )
		{
			ajaxGet('../ajax/lista_acoes.php?prgcod='+prgcod+'&acacod='+p_acacod,document.getElementById('span_acacod'),true);
		}
	}

	function listar_ppa( ppa )
	{

		var valorprgcod = ppa.substr( 0, 4 );
		var valoracacod = ppa.substr( 5, 4 );
		var valorunicod = ppa.substr( 10, 5 );
		var valorloccod = ppa.substr( 16, 4 );

		if( ppa != '' )
		{
			document.formulario.prgcod.value = valorprgcod;
			document.formulario.loccod.value = valorloccod;
			document.formulario.acacod.value = valoracacod;
			document.formulario.unicod.value = valorunicod;
			listagem_produto( valoracacod );
		}
	}

	function listar_unidades( acacod )
	{
		var campo_select = document.getElementById( 'unicod' );
		var campo_dep = document.getElementById( 'depid' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		while( campo_dep.options.length )
		{
			campo_dep.options[0] = null;
		}

		campo_select.options[0] = new Option( '', '', false, true );
		if( acacod != '' )
		{
			ajaxGet('../ajax/lista_unidades.php?acacod='+acacod+'&unicod='+p_unicod,document.getElementById('span_unicod'),true);
			listagem_produto( acacod );
			//listar_coordenador( acacod );
		}
	}

	function listar_iniciativas( dfeid )
	{
		var campo_select = document.getElementById( 'iteid' );
		var campo_dfeid = parseInt(dfeid, 2);

		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		if( dfeid )
		{
			ajaxGet('../ajax/lista_iniciativas.php?dfeid='+dfeid+'&iteid='+p_iteid,document.getElementById('span_iteid'),true);
		}
	}

	function listar_subclasses( clsid )
	{
		var campo_select = document.getElementById( 'sbcid' );
		var campo_clsid = parseInt(clsid, 2);

		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		if( clsid )
		{
			new Ajax.Request('../ajax/lista_subclasse.php?clsid='+clsid+'&sbcid='+p_sbcid, { method:'get'} );
		}
	}

	function listagem_produto( acacod )
	{
		var prgcod = document.formulario.prgcod.value && document.formulario.prgcod.value != 'null' ? document.formulario.prgcod.value : p_prgcod;
		var acacod = document.formulario.acacod.value && document.formulario.acacod.value != 'null' ? document.formulario.acacod.value : p_acacod;
		var unicod = document.formulario.unicod.value && document.formulario.unicod.value != 'null' ? document.formulario.unicod.value : p_unicod;

		var campo_select = document.getElementById( 'prodsc' );
		//while( campo_select.options.length )
		//{
		//	campo_select.options[0] = null;
		//}
		//campo_select.options[0] = new Option( '', '', false, true );
		if( acacod != '' )
		{
			ajaxGet('../ajax/lista_produto.php?prgcod='+prgcod+'&acacod='+acacod+'&unicod='+unicod+'&loccod=',true);
			listar_departamentos( acacod );
		}
	}
/*
	function listar_coordenador( acacod )
	{
		var campo_select = document.getElementById( 'usucpfcoordenacao' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		if( acacod != '' )
		{
			ajaxGet('../ajax/lista_coordenador.php?acacod='+acacod+'&usucpfcoordenacao='+p_usucpfcoordenacao,document.getElementById('span_usucpfcoordenacao'),true);
		}
	}
*/

	function listar_departamentos( acacod )
	{
		var campo_select = document.getElementById( 'depid' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		if( acacod != '' )
		{
			ajaxGet('../ajax/lista_departamentos.php?acacod='+acacod,document.getElementById('span_depid'),true);
		}
	}

	function listar_localizadores( unicod )
	{
		var prgcod = document.formulario.prgcod.value && document.formulario.prgcod.value != 'null' ? document.formulario.prgcod.value : p_prgcod;
		var acacod = document.formulario.acacod.value && document.formulario.acacod.value != 'null' ? document.formulario.acacod.value : p_acacod;
		var unicod = document.formulario.unicod.value && document.formulario.unicod.value != 'null' ? document.formulario.unicod.value : p_unicod;

		var campo_select = document.getElementById( 'loccod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if( unicod !='' && prgcod != '' && acacod != '')
		{
			ajaxGet('../ajax/lista_localizadores.php?prgcod='+prgcod+'&acacod='+acacod+'&unicod='+unicod+'&loccod='+p_loccod,document.getElementById('span_loccod'),true);
		}
	}

	function listar_produto( loccod )
	{
		var prgcod = document.formulario.prgcod.value && document.formulario.prgcod.value != 'null' ? document.formulario.prgcod.value : p_prgcod;
		var acacod = document.formulario.acacod.value && document.formulario.acacod.value != 'null' ? document.formulario.acacod.value : p_acacod;
		var unicod = document.formulario.unicod.value && document.formulario.unicod.value != 'null' ? document.formulario.unicod.value : p_unicod;

		if ( loccod != '' && prgcod != '' && acacod != '' && unicod != '' )
		{
			var url = '../ajax/lista_produto.php?prgcod='+prgcod+'&acacod='+acacod+'&unicod='+unicod+'&loccod='+loccod;
			ajaxGet(url, document.getElementById('codprg'),true);
		}
	}

	function listar_partidos( tauid )
	{
		var ppoid = '<?=$ppoid; ?>';
		var campo_select = document.getElementById( 'ppoid' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if ( tauid != '')
		{
			ajaxGet('../ajax/lista_partidos.php?tauid='+tauid+'&ppoid='+ppoid ,document.getElementById('span_ppoid'),true);
		}
	}

	function listar_autores( ppoid )
	{
		var aueid = '<?=$aueid ?>';
		var tauid = document.formulario.tauid.value;

		tauid = tauid ? tauid : '<?=$tauid ?>';

		var campo_select = document.getElementById( 'aueid' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if (tauid != '' && ppoid != '' )
		{
			ajaxGet('../ajax/lista_autores.php?ppoid='+ppoid+'&tauid='+tauid+'&aueid='+aueid,document.getElementById('span_aueid'),true);
		}
	}


	function listar_acoes_estrategicas(preid){

		var aesid = '<?=$aesid ?>';
		var campo_select = document.getElementById( 'aesid' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if (preid != '' )
		{
			ajaxGet('../ajax/lista_acoes_estrategica.php?preid='+preid+'&aesid='+aesid,document.getElementById('span_aesid'),true);
		}
	}

	function verificar_emenda(valor){

		if ( !valor ) return;

		if ( valor == 'false' )	{

			document.getElementById("tauid").className = "CampoEstiloLeitura";
			document.getElementById("ppoid").className = "CampoEstiloLeitura";
			document.getElementById("aueid").className = "CampoEstiloLeitura";
			document.getElementById('tauid').disabled  = true;
			document.getElementById('ppoid').disabled  = true;
			document.getElementById('aueid').disabled  = true;
		} else {
			document.getElementById('tauid').disabled  = false;
			document.getElementById('ppoid').disabled  = false;
			document.getElementById('aueid').disabled  = false;
			document.getElementById("tauid").className = "CampoEstilo";
			document.getElementById("ppoid").className = "CampoEstilo";
			document.getElementById("aueid").className = "CampoEstilo";
		}

	}

	function verificar_pronac(valor){

		if ( !valor ) return;

		if ( valor == 'false' )	{

			document.formulario.atiproponente.value = "IPHAN";
			document.formulario.atiproponente.className = "leitura";
			document.getElementById("tppid").className = "CampoEstiloLeitura";
			document.getElementById("tscid").className = "CampoEstiloLeitura";
			document.formulario.atisnenviaemailtramitacao.className = "leitura";
			document.formulario.atirepresentante.className = "leitura";
			document.formulario.atiemailrepresentante.className = "leitura";

			document.formulario.atiproponente.disabled  = true;
			document.getElementById('tppid').disabled  = true;
			document.getElementById('tscid').disabled  = true;
			document.formulario.atisnenviaemailtramitacao.disabled  = true;
			document.formulario.atirepresentante.disabled  = true;
			document.formulario.atiemailrepresentante.disabled  = true;

		} else {
			document.formulario.atiproponente.value = "";
			document.formulario.atiproponente.disabled  = false;
			document.getElementById('tppid').disabled  = false;
			document.getElementById('tscid').disabled  = false;
			document.formulario.atisnenviaemailtramitacao.disabled  = false;
			document.formulario.atirepresentante.disabled  = false;
			document.formulario.atiemailrepresentante.disabled  = false;

			document.formulario.atiproponente.className = "normal";
			document.getElementById("tppid").className = "CampoEstilo";
			document.getElementById("tscid").className = "CampoEstilo";
			document.formulario.atisnenviaemailtramitacao.className = "normal";
			document.formulario.atirepresentante.className = "normal";
			document.formulario.atiemailrepresentante.className = "normal";

		}

	}


	function verificar_recursos(valor)
	{
		if ( !valor ) return;

		if ( valor == 'false' )
			document.getElementById('orcamentoFontes').style.display  = 'none';
		else
			document.getElementById('orcamentoFontes').style.display  = '';
	}


	document.getElementById('atisndetalharregionalizacao').disabled  = true;
	function action_esfera(esfid)
	{
		if ( document.formulario.esfid.value == '' )
			document.getElementById('atisndetalharregionalizacao').disabled  = true;
		else
			document.getElementById('atisndetalharregionalizacao').disabled  = false;



		if (document.formulario.esfid.value == 4)
		{
			document.getElementById('abrestuf_on').style.display = 'none';
			document.getElementById('abrestuf_off').style.display = '';
			document.getElementById('abrmuncod_on').style.display = 'none';
			document.getElementById('abrmuncod_off').style.display = '';
		}
		else
		{
			document.getElementById('abrestuf_on').style.display = '';
			document.getElementById('abrestuf_off').style.display = 'none';
			document.getElementById('abrmuncod_on').style.display = '';
			document.getElementById('abrmuncod_off').style.display = 'none';
		}

		document.getElementById('atisndetalharregionalizacao').checked = false;
		document.getElementById('trabrangencia').style.display = 'none';

		esfid = esfid ? esfid : document.formulario.esfid.value;

		if(esfid=='')
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
			document.getElementById('estuf_on').style.display='none';
			document.getElementById('estuf_off').style.display='block';
			document.getElementById('muncod_on').style.display='none';
			document.getElementById('muncod_off').style.display='block';

			document.getElementById('estuf').value='';
			document.getElementById('muncod').value='';
		}
		if(esfid==1)
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
//			document.getElementById('estuf').value='';
//			document.getElementById('muncod').value='';
//
//			listar_municipios( document.getElementById('estuf').value );
//
//			document.getElementById('estuf_on').style.display='block';
//			document.getElementById('estuf_off').style.display='none';
//			document.getElementById('muncod_on').style.display='block';
//			document.getElementById('muncod_off').style.display='none';
		}
		else if(esfid==2)
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
			document.getElementById('estuf').value='<?=$estuf ?>';
			document.getElementById('muncod').value='';

			listar_municipios( document.getElementById('estuf').value );

			document.getElementById('estuf_on').style.display='block';
			document.getElementById('estuf_off').style.display='none';
			document.getElementById('muncod_on').style.display='block';
			document.getElementById('muncod_off').style.display='none';
		}
		else if(esfid==3)
		{
			document.formulario.paiid.disabled = true;
			document.formulario.paiid.value = 1;
			document.getElementById('estuf').value  = '<?=$estuf ?>';
			document.getElementById('muncod').value = '';

			<?
			if (!$muncod){
			?>
			listar_municipios( document.getElementById('estuf').value );
			<?
			}else{
				if ($estuf == 'DF'):
			?>
			document.getElementById('muncod').value = '<?=$rgaid ?>';
			<?
				else:
			?>
			document.getElementById('muncod').value = '<?=$muncod ?>';
			<?
				endif;
			}
			?>
			document.getElementById('estuf_on').style.display='block';
			document.getElementById('estuf_off').style.display='none';
		}
		else if(esfid==4)
		{
			document.formulario.paiid.disabled = false;
			document.getElementById('estuf_on').style.display='none';
			document.getElementById('estuf_off').style.display='block';
			document.getElementById('muncod_on').style.display='none';
			document.getElementById('muncod_off').style.display='block';

			document.getElementById('estuf').value='';
			document.getElementById('muncod').value='';
		}

	}

	function listar_municipios( regcod )
	{
		var campo_select = document.getElementById( 'muncod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );

		if(document.formulario.esfid.value!='')
		{
			document.getElementById('muncod_on').style.display='block';
			document.getElementById('muncod_off').style.display='none';
			new Ajax.Request('../ajax/lista_mun_pi.php?estuf='+regcod, { method:'get'} );
		}
	}


	function somar_orcamento( trocaExistente )
	{
		alert(document.getElementById[''].value);
		var vlrCusteio = document.formulario.atiorcamentocusteio.value;
		var vlrCapital = document.formulario.atiorcamentocapital.value;
		var vlrOrcamento = document.formulario.atiorcamento.value;
		var vlrTotal = 0;

		try
		{
			vlrCusteio = parseInt( vlrCusteio.replace( /[^0-9]/g, '' ) ) / 100;
			if ( isNaN( vlrCusteio ) )
				vlrCusteio = 0;
		}
		catch ( e )
		{
			vlrCusteio = 0;
		}

		try
		{
			vlrCapital = parseInt( vlrCapital.replace( /[^0-9]/g, '' ) ) / 100;
			//alert( vlrCapital );
			if ( isNaN( vlrCapital ) )
				vlrCapital = 0;

		}
		catch ( e )
		{
			vlrCapital = 0;
		}

		try
		{
			vlrTotal = ( vlrCusteio + vlrCapital ) * 100;
		}
		catch (e)
		{
			vlrTotal = document.formulario.atiorcamento.value;
		}

		if ( trocaExistente )
		{
			document.formulario.atiorcamento.value = formata_decimal3( vlrTotal );
		}
	}

	function verificarPronac() {
/*		if(document.getElementById('pronac').value == document.getElementById('atipronac').value)
		{
			//alert('diferente = '+document.getElementById('protocolo').value+' / '+ document.getElementById('atiprotocolo').value);
		}
		else
		{
			ajaxGet('../ajax/verificapronac.php?p='+document.getElementById('atipronac').value,document.getElementById('resultpronac'),true);
		}*/
		ajaxGet('../ajax/busca_pronac.php?p='+document.getElementById('atipronac').value,document.getElementById('resultpronac'),true);
	}



	function cancelar() {
		window.location.href = '?modulo=<?= $_SESSION['paginainicial'] ?>';
	}

	function enviar( envia_minc, envia_tramitacao ){

		var d = document;
		var f = d.formulario;
		var msg = '';

		if (f.atidescricao.value == ''){
			msg += 'Título\n';
		}
		if (f.atidetalhamento.value == ''){
			msg += 'Descrição\n';
		}
		if (f.prgcod.value == ''){
			msg += 'Programa\n';
		}
		if (f.acacod.value == ''){
			msg += 'Ação\n';
		}
		if (f.unicod.value == ''){
			msg += 'Unidade Orçamentária\n';
		}
		if (f.loccod.value == ''){
			msg += 'Localizador\n';
		}
		if (f.atipropameta.value == ''){
			msg += 'Produto PPA: Meta\n';
		}
		if (f.pduid.value == ''){
			msg += 'Produto Secundário: Produto\n';
		}
		if (f.udaid.value == ''){
			msg += 'Produto Secundário: Unidade Medida\n';
		}
		if (f.atiprosecundariometa.value == ''){
			msg += 'Produto Secundário: Meta\n';
		}
		if (f.uexid.value == ''){
			msg += 'Unidade Executora\n';
		}
//		if (f.depid.value == ''){
//			msg += 'Departamento. Caso o departamento não tenha sido preenchido, \nverifique com o administrador do sistema se existe atribuição \nentre a Ação PPA escolhida e os departamentos da unidade.\n\n';
//		}
		if (f.dfeid.value == ''){
			msg += 'Desafio Estratégico\n';
		}
		if (f.iteid.value == ''){
			msg += 'Iniciativa estratégica\n';
		}
		if (f.preid.value == ''){
			msg += 'Programas estratégicos\n';
		}
		if (f.aesid.value == ''){
			msg += 'Ações estratégicas\n';
		}
		/*
		if (f.pddid.value == ''){
			msg += 'Prioridade\n';
		}
		if (f.clsid.value == ''){
			msg += 'Classe\n';
		}
		if (f.sbcid.value == ''){
			msg += 'Sub-classe\n';
		}
		*/
		if (f.areid.value == ''){
			msg += 'Área\n';
		}
		if (f.segid.value == ''){
			msg += 'Segmento\n';
		}
		if (f.esfid.value == ''){
			msg += 'Esfera Administrativa\n';
		}
		if (f.paiid.value == ''){
			msg += 'Pais\n';
		}
		if ((f.esfid.value == 3 || f.esfid.value == 2) && f.estuf.value == ''){
			msg += 'Unidade Federação\n';
		}
		if (f.esfid.value == 3 && f.muncod.value == ''){
			msg += 'Município\n';
		}
		if ( f.atipronacquest[0].checked ){
			if (f.atipronac.value == ''){
				msg += 'Nº do PRONAC\n';
			}
			if (f.atiproponente.value == ''){
				msg += 'Nome do Proponente\n';
			}
			if ( f.tppid.value == '' ){
				msg += 'Tipo\n';
			}
			if ( f.tscid.value == '' ){
				msg += 'Forma de Seleção\n';
			}
			if ( f.atisnenviaemailtramitacao.checked )
			{
				if ( f.atirepresentante.value == '' ){
					msg += 'Nome do Representante\n';
				}
				if ( f.atiemailrepresentante.value == '' ){
					msg += 'E-mail do Representante\n';
				}
			}
		}

		if ( f.atiemenda[0].checked ){
			if (f.tauid.value == ''){
				msg += 'Tipo de Autor\n';
			}

			if (f.ppoid.value == ''){
				msg += 'Partido\n';
			}
			if (f.aueid.value == ''){
				msg += 'Autor\n';
			}
		}

//		var arNatureza = $$('input[name="natureza[]"]');
//		var qtdNatureza = arNatureza.size();
//		var validarNatureza = <?= VALIDAR_NATUREZA_DESPESA == 'S' ? 'true' : 'false' ; ?>;
//		var i = 0;
//		for( i = 0 ; i < qtdNatureza; i++ )
//		{
//			if ( validarNatureza && !financeiro_naturezas_validas[f.prgcod.value+f.acacod.value+f.unicod.value+f.loccod.value+arNatureza[i].value] )
//			{
//				alert( 'A Natureza de Despesa '+arNatureza[i].value+' não foi encontrada no orçamento de <?=$_SESSION['exercicio']?> para a Funcional Programática escolhida.\nSe você deseja cadastrar um PA com essa Natureza de Despesa contate o administrador.' );
//				return false;
//			}
//		}
//
//		if (qtdNatureza < 1 )
//		{
//			msg += 'Natureza de despesa\n';
//		}

		if (f.stpid.value == ''){
			msg += 'Situação do processo de contratação\n';
		}

		if (f.atidatainicio.value == ''){
			msg += 'Data de Início\n';
		}

		if (f.atidatafim.value == ''){
			msg += 'Data de Término\n';
		}

		if (f.itrid.value == ''){
			msg += 'Instrumento de Contratação\n';
		}

		if (msg != ''){
			alert('Os campos listados são obrigatórios e devem ser preenchidos:\n'+msg);
			return false;
		}

		if(validaAcao() == ''){
			alert('Ação PPA não encontrada!');
			return false;
		}

		selectAllOptions( formulario.gerente );
		selectAllOptions( formulario.apoio );

		if(envia_tramitacao == 1){
			document.getElementById('gravar_tramitar').value = 1;
		}

		document.formulario.submit();
	}

function validaAcao(){

	var d = document;
	var f = d.formulario;

	var prg = f.prgcod.value;
 	var ac  = f.acacod.value;
 	var uni = f.unicod.value;
 	var loc = f.loccod.value;

 	var url   = 'projetos.php?modulo=principal/planointerno/cadpa&acao=I&ajax=1';
 	var vars  = 'prgcod=' + prg + '&acacod=' + ac + '&unicod=' + uni + '&loccod=' + loc;
 	var acaid = '';

	var ajax = new Ajax.Request ( url , {
											method: 'post' ,
											parameters: vars ,
											asynchronous: false,
											onComplete: function(res) {
																		acaid = res.responseText;
																	  }
											});
   return acaid;
}



		/**
		 * Contém os itens financeiros adicionados. Utilizado para
		 * evitar repetições de natureza-iduso-fonte-idoc na tabela
		 * de item financeiro.
		 *
		 * @var boolean[]
		 */
		var item_financeiros_adicionados ={};
		var item_financeiros_fonte_adicionados = {};

		/**
		 * Soma de todos os itens da tabela financeiro.
		 *
		 * @var float
		 */
		var item_financeiro_valor_total = 0;
		var item_financeiro_fonte_valor_total = 0;

		/**
		 * Indica se usuário pode manipular itens financeiros onde
		 * as natureza possuem gnd = 1.
		 *
		 * @var boolean[]
		 */
		var item_financeiro_habilita_gnd_1 = <?= $habilita_gnd_1 ? 'true' : 'false' ?>;

		/**
		 * Sufixo do identificador de cada registro na tabela de
		 * financeiro. Variável incremental utilizada para evitar
		 * identificadores repetidos para cada tag 'tr' da tabela
		 * de financeiro.
		 *
		 * @var float
		 */
		var item_financeiro_id_atual = 0;
		var item_financeiro_fonte_id_atual = 0;

		/**
		 * Indica se existia itens financeiros ao iniciar a tela.
		 *
		 * @var float
		 */
		var possui_item_financeiro_inicial = <?= count( $itens_financeiro ) ? 'true' : 'false' ?>;
		var possui_item_financeiro_fonte_inicial = <?= count( $itens_financeiro ) ? 'true' : 'false' ?>;


		/**
		 * Indica de usuário possui privilégio para manipular os dados de despesa.
		 *
		 * @var boolean
		 */
		var permissao_momento = <?= $permissao_momento ? 'true' : 'false' ?>;



///////////////////////////////////////////////
//
// ITENS ABRNGENCIA
//
//////////////////////////////////////////////
		var item_abrangencia_id_atual = 0;

		var item_abrangencia_adicionados = {};

		function item_abrangencia_adicionar_do_formulario()
		{
			// captura os campos
			var pais = document.getElementById( 'abrpaiid' );
			var uf 	 = document.getElementById( 'abrestuf' );
			var mun  = document.getElementById( 'abrmuncod' );

			// realiza validação dos dados
			if ( !validaBranco( pais, 'País' ) ) return;
			if( document.formulario.esfid.value != 4 )
			{
				if ( !validaBranco( uf, 'Unidade Federativa' ) ) return;
				if ( !validaBranco( mun, 'Município' ) ) return;
			}


			// insere os dados na tabela
			if ( item_abrangencia_adicionar( pais.value, uf.value, mun.value ) == false )
			{
				return;
			}
			// limpa campos para nova inserção
			pais.value = '';
			uf.value = '';
			mun.value = '';
			//item_financeiro_preencher_valores_default();
			houve_modificacao = true;
			document.getElementById( 'abrpaiid' ).focus();
		}


//////////////////
		function item_abrangencia_adicionar( pais, uf, mun )
		{

			var id_novo_item = pais + '_' + uf + '_' + mun;
			// verifica se item já existe
			if ( item_abrangencia_adicionados[id_novo_item] )
			{
				alert( 'Já existe uma abrangência com essas informações. Você pode alterar as informações selecionando o item correspondente na tabela abaixo do formulário.' );
				// posiciona a linha a ser destacada
				document.getElementById( item_abrangencia_adicionados[id_novo_item] + '_ancora' ).focus();
				var funcao_hilight = new Function( ' item_abrangencia_hilight( \'' + item_abrangencia_adicionados[id_novo_item] + '\', 30 ); ' );
				setTimeout( funcao_hilight, 300 );
				return false;

			}

			// define dados gerais do item a ser adicionado
			item_abrangencia_id_atual++;
			var tabela = document.getElementById( 'tabela_abrangencia' );
			var id = 'item_abragencia_' + item_abrangencia_id_atual;
			item_abrangencia_adicionados[id_novo_item] = id;

			// FIM define dados gerais do item a ser adicionado
			// cria nova linha para a tabela de financeiro
			var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
			    nova_linha.id = id;
			    nova_linha.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );
			// FIM cria nova linha para a tabela de financeiro
			// cria as célular da nova linha
			var celula_pais = nova_linha.insertCell( 0 );
			var celula_uf = nova_linha.insertCell( 1 );
			    celula_uf.style.textAlign = 'right';
			    celula_uf.style.color = '#0066cc';
			var celula_mun = nova_linha.insertCell( 2 );
			    celula_mun.style.textAlign = 'right';
			    celula_mun.style.color = '#0066cc';
			var celula_acao_alterar = nova_linha.insertCell( 3 );
			    celula_acao_alterar.style.textAlign = 'center';
			var celula_acao_remover = nova_linha.insertCell( 4 );
			    celula_acao_remover.style.textAlign = 'left';
			// FIM cria as célular da nova linha
			// cria campos ocultos com os dados do novo item
			var input_hidden_pais = item_abrangencia_criar_input( pais, 'pais' );
			var input_hidden_uf = item_abrangencia_criar_input( uf, 'uf' );
			var input_hidden_mun = item_abrangencia_criar_input( mun, 'mun' );
				input_hidden_pais.id = id + '_valor';
			// FIM cria campos ocultos com os dados do novo item
			// define dado para as células de açãoe

				var link_acao_alterar = document.createElement( 'a' );
				    link_acao_alterar.href = 'javascript:item_abrangencia_alterar( \'' + id + '\', \'' + pais + '\', \'' + uf + '\', \'' + mun + '\' );';
				    link_acao_alterar.title = 'Editar';
				    link_acao_alterar.innerHTML = '<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
				var link_acao_remover = document.createElement( 'a' );
				    link_acao_remover.href = 'javascript:item_abrangencia_remover( \'' + id + '\', true );';
				    link_acao_remover.title = 'Excluir';
				    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif" border="0" align="absmiddle"/>';

			link_acao_alterar.id = id + '_ancora';
			// FIM define dado para as células de ação
			// insere os dados ( textos e inputs ) nas células
			celula_pais.appendChild( document.createTextNode( pais ) );
			celula_pais.appendChild( input_hidden_pais );
			celula_uf.appendChild( document.createTextNode( uf ) );
			celula_uf.appendChild( input_hidden_uf );
			celula_mun.appendChild( document.createTextNode( mun ) );
			celula_mun.appendChild( input_hidden_mun );

			if ( link_acao_alterar != null  )
			{
				celula_acao_alterar.appendChild( link_acao_alterar );
			}
			if ( link_acao_remover != null  )
			{
				celula_acao_remover.appendChild( link_acao_remover );
			}


			listar_municipios_abrangencia();

			return true;
		}

/////////////////
		function item_abrangencia_criar_input( valor, nome_campo )
		{
			var input = document.createElement( 'input' );
			input.type = 'hidden';
			input.name = nome_campo + '[]';
			input.value = valor;
			return input;
		}


///////////////
		function item_abrangencia_alterar( id_tr, pais, uf, mun )
		{
			houve_modificacao = true;
			// captura os campos do financeiro
			var campo_pais = document.getElementById( 'abrpaiid' );
			var campo_uf = document.getElementById( 'abrestuf' );
			var campo_mun = document.getElementById( 'abrmuncod' );
			listar_municipios_abrangencia(uf);
			campo_pais.value = pais;
			campo_uf.value = uf;
			campo_mun.value = mun;
			item_abrangencia_remover( id_tr, false );
		}


//////////////////
		function item_abrangencia_remover( id_tr, realizar_pergunta )
		{
			if ( realizar_pergunta )
			{
				if ( !confirm( 'Deseja realmente excluir o detalhamento?' ) )
				{
					return;
				}
			}
			houve_modificacao = true;
			var tr = document.getElementById( id_tr );
			var input = document.getElementById( id_tr + '_valor' );
			if ( tr && input )
			{
				tr.parentNode.removeChild( tr );
			}
		}


//////////////
		function item_abrangencia_hilight( id_tr, loop )
		{
			var tr = document.getElementById( id_tr );
			if ( !tr )
			{
				return;
			}
			if ( loop < 1 )
			{
				return;
			}
			var cor = '';
			switch ( loop-- % 9 )
			{
				case 0:	case 8:
					//cor = '#d0d0d0';
					cor = '#ffffff';
					break;
				case 1:	case 7:
					//cor = '#d8d8d8';
					cor = '#ffffdd';
					break;
				case 2:	case 6:
					//cor = '#e0e0e0';
					cor = '#ffffaa';
					break;
				case 3:	case 5:
					//cor = '#e8e8e8';
					cor = '#ffff80';
					break;
				case 4:
					//cor = '#f0f0f0';
					cor = '#ffff40';
					break;
			}
			tr.style.backgroundColor = cor;
			var funcao_hilight = new Function( ' item_abrangencia_hilight( \'' + id_tr + '\', ' + loop + ' ); ' );
			setTimeout( funcao_hilight, 120 );
		}




















///////////////////////////////////////////////
//
// ITENS FINANCEIROS
//
//////////////////////////////////////////////
		/**
		 * Detecta qual o campo focado atualmente e altera para o
		 * próxima caso a tecla apertada seja 'enter'.
		 *
		 * @param void
		 * @return void
		 */
		function item_financeiro_alterar_campo_focado( event, campo )
		{
			if ( event.keyCode != 13 )
			{
				return;
			}
			switch ( campo )
			{
				case 'natureza':
					var campo = document.getElementById( 'novo_valor' );
					break;
				case 'valor':
					item_financeiro_adicionar_do_formulario();
					return;
				default:
					return;
			}
			campo.focus();
			campo.select();
		}


		// VALORES DOS CAMPOS ITEM FINANCEIRO VÁLIDOS
		var financeiro_naturezas_validas = new Array();
		<?
			$dados_temp = $db->carregar( $sql_financeiro_validacao );
			if ( $dados_temp )
			{
				foreach ( $dados_temp as $dados_temp_valor )
				{
					?>financeiro_naturezas_validas['<?= $dados_temp_valor['codigo'] ?>'] = true; <?
				}
			}
		?>

		// VALORES DOS CAMPOS ITEM FINANCEIRO POR FONTE VÁLIDOS
		var financeiro_fontes_validas = new Array();
		<?
			$dados_temp = $db->carregar( $sql_financeiro_fonte );
			if ( $dados_temp )
			{
				foreach ( $dados_temp as $dados_temp_valor )
				{
					?>financeiro_fontes_validas['<?= $dados_temp_valor['codigo'] ?>'] = true; <?
				}
			}
		?>


				/**
		 * Organiza as cores da linhas, ora com valor vazio ora com 'efefef'.
		 *
		 * @return void
		 */
		function item_financeiro_organizar_cor_linha()
		{
			var tabela = document.getElementById( 'tabela_financeiro' );
			var quantidade_tr = tabela.rows.length;
			var cor = '';
			for ( var i = 0; i < quantidade_tr; i++ )
			{
				var tr = tabela.rows[i];
				cor = cor == '' ? '#e9e9e9' : '' ;
				tr.style.backgroundColor = cor;
				tr.onmouseout = new Function( ' this.style.backgroundColor = "' + cor + '"; ' );
			}
		}


				/**
		 * Organiza as cores da linhas, ora com valor vazio ora com 'efefef'.
		 *
		 * @return void
		 */
		function item_financeiro_fonte_organizar_cor_linha()
		{
			var tabela_fonte = document.getElementById( 'tabela_financeiro_fonte' );
			var quantidade_tr = tabela_fonte.rows.length;
			var cor = '';
			for ( var i = 0; i < quantidade_tr; i++ )
			{
				var tr = tabela_fonte.rows[i];
				cor = cor == '' ? '#e9e9e9' : '' ;
				tr.style.backgroundColor = cor;
				tr.onmouseout = new Function( ' this.style.backgroundColor = "' + cor + '"; ' );
			}
		}


		/**
		 * Transforma uma string com vírgulas e pontos em um número real.
		 *
		 * @param string
		 * @return float
		 */
		function desformata_decimal( valor )
		{
			if ( valor == null )
			{
				return 0.0;
			}
			valor = valor.replace( /\./g, '' );
			valor = valor.replace( ',', '.' );
			if ( valor[valor.length] == '.' )
			{
				valor += '0';
			}
			if ( valor.length == 0 )
			{
				return 0.0;
			}
			return parseFloat( valor );
		}

		/**
		 * Transforma uma string com vírgulas e pontos em um número com decimal.
		 *
		 * @param string
		 * @return float
		 */
		function desformata_numero( valor )
		{
			if ( valor == null )
			{
				return 0.0;
			}

			valor = valor.replace( /\./g, '' );
			valor = valor.replace( ',', '.' );
			if ( valor[valor.length] == '.' )
			{
				valor += '0';
			}
			if ( valor.length == 0 )
			{
				return 0.0;
			}

			return valor;
		}

		/**
		 * Atualiza o dado da tela de acordo com a variável
		 * item_financeiro_valor_total.
		 *
		 * @param void
		 * @return void
		 */
		function item_financeiro_atualizar_valor_total()
		{
			var d = document;
			var valor_total_string = item_financeiro_valor_total + '';

			document.getElementById( 'item_financeiro_valor_total' ).value = MascaraMonetario(valor_total_string);
			var valoradd = new Number(desformata_numero(d.getElementById( 'item_financeiro_fonte_valor_total' ).value));
			var valorbase = new Number(desformata_numero(d.getElementById( 'item_financeiro_valor_total' ).value));
			//var valortotal = desformata_numero(document.getElementById( 'item_financeiro_valor_total' ).value);
			document.getElementById( 'item_financeiro_total' ).value = MascaraMonetario(new String(valorbase + valoradd));
		}

		/**
		 * Atualiza o dado da tela de acordo com a variável
		 * item_financeiro_fonte_valor_total.
		 *
		 * @param void
		 * @return void
		 */
		function item_financeiro_fonte_atualizar_valor_total()
		{
			var d = document;
			var valor_total_string = item_financeiro_fonte_valor_total + '';

			//alert(desformata_decimal(valor_total_string2));
			d.getElementById( 'item_financeiro_fonte_valor_total' ).value = MascaraMonetario(valor_total_string);
			var valoradd = new Number(desformata_numero(d.getElementById( 'item_financeiro_fonte_valor_total' ).value));
			var valorbase = new Number(desformata_numero(d.getElementById( 'item_financeiro_valor_total' ).value));
			d.getElementById( 'item_financeiro_total' ).value = MascaraMonetario(new String(valorbase + valoradd));
		}

		/**
		 * Cria o conteúdo de uma célula da tabela de itens de despesa
		 *
		 * @param string
		 * @param string
		 * @return object
		 */
		function item_financeiro_criar_input( valor, nome_campo )
		{
			var input = document.createElement( 'input' );
			input.type = 'hidden';
			input.name = nome_campo + '[]';
			input.value = valor;
			return input;
		}

		/**
		 * Cria o conteúdo de uma célula da tabela de itens de despesa
		 *
		 * @param string
		 * @param string
		 * @return object
		 */
		function item_financeiro_fonte_criar_input( valor, nome_campo )
		{
			var input = document.createElement( 'input' );
			input.type = 'hidden';
			input.name = nome_campo + '[]';
			input.value = valor;
			return input;
		}

		/**
		 * Insere item financeiro na tabela de financeiro com
		 * os dados atuais do formulário. As verificações de
		 * preenchimento dos dados é realizada nesta função.
		 *
		 * @return void
		 */
		function item_financeiro_adicionar_do_formulario()
		{
			var validarNatureza = <?= VALIDAR_NATUREZA_DESPESA == 'S' ? 'true' : 'false' ; ?>;
			// captura os campos do financeiro
			var natureza = document.getElementById( 'nova_natureza' );
			var valor = document.getElementById( 'novo_valor' );
			var prgcod = document.getElementById( 'prgcod' );
			var acacod = document.getElementById( 'acacod' );
			var unicod = document.getElementById( 'unicod' );
			var loccod = document.getElementById( 'loccod' );

			// realiza validação dos dados
			if ( !validaBranco( natureza, 'Natureza de Despesa' ) ) return;
			if ( !validaBranco( valor, 'Valor' ) ) return;
			if ( validarNatureza )
			{
				if ( !validaBranco( prgcod, 'Programa' ) ) return;
				if ( !validaBranco( acacod, 'Ação' ) ) return;
				if ( !validaBranco( unicod, 'Unidade' ) ) return;
				if ( !validaBranco( loccod, 'Localizador' ) ) return;

				if ( !financeiro_naturezas_validas[prgcod.value+acacod.value+unicod.value+loccod.value+natureza.value] )
				{
					alert( 'A Natureza de Despesa informada não foi encontrada no orçamento de <?=$_SESSION['exercicio']?>. Se você deseja cadastrar um PA com essa Natureza de Despesa contate o administrador.' );
					natureza.focus();
					natureza.select();
					return;
				}
			}

			<?php
			if ($planejamento->pltid):
			?>
		    if ( verificaNaturezaByPlanejamento(natureza.value, valor.value) == false ){
		        return;
		    }
			<?php
			endif;
			?>


			// insere os dados na tabela de financeiro
			if ( item_financeiro_adicionar( natureza.value, valor.value ) == false )
			{
				return;
			}
			// limpa campos para nova inserção
			natureza.value = '';
			valor.value = '';
			//item_financeiro_preencher_valores_default();
			houve_modificacao = true;
			document.getElementById( 'nova_natureza' ).focus();
		}

		 function verificaNaturezaByPlanejamento(natureza, valor){
			 var obCalc           = new Calculo();

			 var custeio          = jQuery('#pltcusteio').val();
			 var investimento     = jQuery('#pltinvestimento').val();
			 var custeioSoma      = new String('0');
			 var investimentoSoma = new String('0');

			 custeioSoma      = ( natureza.substr(0,1) == '3' ? valor : '0' );
			 investimentoSoma = ( natureza.substr(0,1) == '4' ? valor : '0' );

			 jQuery('[name^=natureza]').each(function (i){
    			 if ( jQuery( this ).val().substr(0,1) == '3' ){
    				 custeioSoma = obCalc.operacao(custeioSoma, jQuery('[name^=valor]:eq(' + i + ')').val(), '+');
    				 custeioSoma = mascaraglobal("[###.]###,##", custeioSoma);
    			 }else if ( jQuery( this ).val().substr(0,1) == '4' ){
    				 investimentoSoma = obCalc.operacao(investimentoSoma, jQuery('[name^=valor]:eq(' + i + ')').val(), '+');
    				 investimentoSoma = mascaraglobal("[###.]###,##", investimentoSoma);
    			 }
			 });

			 if ( obCalc.comparar(custeio, custeioSoma, '<') ){
				    alert('A soma dos valores da Natureza de Despesa excedem o valor de custeio do planejamento!');
				    return false;
		     }

			 if ( obCalc.comparar(investimento, investimentoSoma, '<') ){
				    alert('A soma dos valores da Natureza de Despesa excedem o valor de custeio do planejamento!');
				    return false;
			 }

			 return true;
	     }

		/**
		 * Insere item financeiro FONTE na tabela de financeiro FONTE com
		 * os dados atuais do formulário. As verificações de
		 * preenchimento dos dados é realizada nesta função.
		 *
		 * @return void
		 */
		function item_financeiro_fonte_adicionar_do_formulario()
		{
			// captura os campos do financeiro
			var fonte = document.getElementById( 'nova_fonte' );
			var valor_fonte = document.getElementById( 'novo_valor_fonte' );

			// realiza validação dos dados
			if ( !validaBranco( fonte, 'Fonte de Recurso' ) ) return;
			if ( !validaBranco( valor_fonte, 'Valor' ) ) return;

			if ( !financeiro_fontes_validas[fonte.value] )
			{
				alert( 'Fonte de Recurso inválida.' );
				fonte.focus();
				fonte.select();
				return;
			}

			// insere os dados na tabela de financeiro
			if ( item_financeiro_fonte_adicionar( fonte.value, valor_fonte.value ) == false )
			{
				return;
			}
			// limpa campos para nova inserção
			fonte.value = '';
			valor_fonte.value = '';
			//item_financeiro_preencher_valores_default();
			houve_modificacao_fonte = true;
			document.getElementById( 'nova_fonte' ).focus();
		}


		/**
		 * Adiciona um item à tabela de itens de despesa. Também é adiciona
		 * um campo no formulário para as linhas.
		 *
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @return boolean
		 */

		function item_financeiro_adicionar( natureza, valor )
		{
			if ( valor <= 0 )
			{
				alert( 'Valor deve ser maior que zero.' );
				document.getElementById( 'novo_valor' ).focus();
				return false;
			}
			var id_novo_item = natureza ;
			// verifica se item já existe
			if ( item_financeiros_adicionados[id_novo_item] )
			{
				alert( 'Já existe item financeiro para esta natureza. Você pode alterar o valor desse item selecionando o item correspondente na tabela abaixo do formulário.' );
				// posiciona a linha a ser destacada
				document.getElementById( item_financeiros_adicionados[id_novo_item] + '_ancora' ).focus();
				var funcao_hilight = new Function( ' item_financeiro_hilight( \'' + item_financeiros_adicionados[id_novo_item] + '\', 30 ); ' );
				setTimeout( funcao_hilight, 300 );
				return false;

			}
			// define dados gerais do item a ser adicionado
			item_financeiro_id_atual++;
			var tabela = document.getElementById( 'tabela_financeiro' );
			var id = 'item_financeiro_' + item_financeiro_id_atual;
			item_financeiros_adicionados[id_novo_item] = id;
			// FIM define dados gerais do item a ser adicionado
			// cria nova linha para a tabela de financeiro
			var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
			    nova_linha.id = id;
			    nova_linha.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );
			// FIM cria nova linha para a tabela de financeiro
			// cria as célular da nova linha
			var celula_natureza = nova_linha.insertCell( 0 );
			var celula_valor = nova_linha.insertCell( 1 );
			    celula_valor.style.textAlign = 'right';
			    celula_valor.style.color = '#0066cc';
			var celula_acao_alterar = nova_linha.insertCell( 2 );
			    celula_acao_alterar.style.textAlign = 'center';
			var celula_acao_remover = nova_linha.insertCell( 3 );
			    celula_acao_remover.style.textAlign = 'left';
			// FIM cria as célular da nova linha
			// cria campos ocultos com os dados do novo item
			var input_hidden_natureza = item_financeiro_criar_input( natureza, 'natureza' );
			var input_hidden_valor = item_financeiro_criar_input( valor, 'valor' );
				input_hidden_valor.id = id + '_valor';
			// FIM cria campos ocultos com os dados do novo item
			// define dado para as células de açãoe

				var link_acao_alterar = document.createElement( 'a' );
				    link_acao_alterar.href = 'javascript:item_financeiro_alterar( \'' + id + '\', \'' + natureza + '\', \'' + valor + '\' );';
				    link_acao_alterar.title = 'Editar';
				    link_acao_alterar.innerHTML = '<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
				var link_acao_remover = document.createElement( 'a' );
				    link_acao_remover.href = 'javascript:item_financeiro_remover( \'' + id + '\', \'' + natureza + '\', true );';
				    link_acao_remover.title = 'Excluir';
				    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif" border="0" align="absmiddle"/>';

			link_acao_alterar.id = id + '_ancora';
			// FIM define dado para as células de ação
			// insere os dados ( textos e inputs ) nas células
			celula_natureza.appendChild( document.createTextNode( natureza ) );
			celula_natureza.appendChild( input_hidden_natureza );
			celula_valor.appendChild( document.createTextNode( valor ) );
			celula_valor.appendChild( input_hidden_valor );
			if ( link_acao_alterar != null  )
			{
				celula_acao_alterar.appendChild( link_acao_alterar );
			}
			if ( link_acao_remover != null  )
			{
				celula_acao_remover.appendChild( link_acao_remover );
			}
			// FIM insere os dados ( textos e inputs ) nas células
			// altera soma dos valores de acordo com o novo item
			item_financeiro_valor_total += desformata_decimal( valor );
			item_financeiro_atualizar_valor_total();
			// FIM altera soma dos valores de acordo com o novo item
			// atualiza as cores alternadas das linhas
			item_financeiro_organizar_cor_linha();
			return true;
		}



		/**
		 * Adiciona um item à tabela de itens de despesa. Também é adiciona
		 * um campo no formulário para as linhas.
		 *
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @return boolean
		 */

		function item_financeiro_fonte_adicionar( fonid, valor_fonte )
		{
			if ( valor_fonte <= 0 )
			{
				alert( 'Valor deve ser maior que zero.' );
				document.getElementById( 'novo_valor_fonte' ).focus();
				return false;
			}
			var id_novo_item_fonte = fonid ;
			// verifica se item já existe
			if ( item_financeiros_fonte_adicionados[id_novo_item_fonte] )
			{
				alert( 'Já existe item financeiro para esta fonte. Você pode alterar o valor desse item selecionando o item correspondente na tabela abaixo do formulário.' );
				// posiciona a linha a ser destacada
				document.getElementById( item_financeiros_fonte_adicionados[id_novo_item_fonte] + '_ancora_fonte' ).focus();
				var funcao_hilight = new Function( ' item_financeiro_fonte_hilight( \'' + item_financeiros_fonte_adicionados[id_novo_item_fonte] + '\', 30 ); ' );
				setTimeout( funcao_hilight, 300 );
				return false;

			}
			// define dados gerais do item a ser adicionado
			item_financeiro_fonte_id_atual++;
			var tabela_fonte = document.getElementById( 'tabela_financeiro_fonte' );
			var id = 'item_financeiro_fonte_' + item_financeiro_fonte_id_atual;
			item_financeiros_fonte_adicionados[id_novo_item_fonte] = id;
			// FIM define dados gerais do item a ser adicionado
			// cria nova linha para a tabela de financeiro
			var nova_linha_fonte = tabela_fonte.insertRow( tabela_fonte.rows.length - 1 );
			    nova_linha_fonte.id = id;
			    nova_linha_fonte.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );
			// FIM cria nova linha para a tabela de financeiro
			// cria as célular da nova linha
			var celula_fonte = nova_linha_fonte.insertCell( 0 );
			var celula_valor_fonte = nova_linha_fonte.insertCell( 1 );
			    celula_valor_fonte.style.textAlign = 'right';
			    celula_valor_fonte.style.color = '#0066cc';
			var celula_acao_alterar = nova_linha_fonte.insertCell( 2 );
			    celula_acao_alterar.style.textAlign = 'center';
			var celula_acao_remover = nova_linha_fonte.insertCell( 3 );
			    celula_acao_remover.style.textAlign = 'left';
			// FIM cria as célular da nova linha

			fonid = fonid.split(" # ");

			// cria campos ocultos com os dados do novo item
			var input_hidden_fonte = item_financeiro_fonte_criar_input( fonid[0], 'fonid' );
			var input_hidden_valor = item_financeiro_fonte_criar_input( valor_fonte, 'valor_fonte' );
				input_hidden_valor.id = id + '_valor_fonte';
			// FIM cria campos ocultos com os dados do novo item
			// define dado para as células de açãoe

				var link_acao_alterar = document.createElement( 'a' );
				    link_acao_alterar.href = 'javascript:item_financeiro_fonte_alterar( \'' + id + '\', \'' + fonid + '\', \'' + valor_fonte + '\' );';
				    link_acao_alterar.title = 'Editar';
				    link_acao_alterar.innerHTML = '<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
				var link_acao_remover = document.createElement( 'a' );
				    link_acao_remover.href = 'javascript:item_financeiro_fonte_remover( \'' + id + '\', \'' + fonid	 + '\', true );';
				    link_acao_remover.title = 'Excluir';
				    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif" border="0" align="absmiddle"/>';

			link_acao_alterar.id = id + '_ancora_fonte';
			// FIM define dado para as células de ação
			// insere os dados ( textos e inputs ) nas células
			celula_fonte.appendChild( document.createTextNode( fonid[1] ) );
			celula_fonte.appendChild( input_hidden_fonte );
			celula_valor_fonte.appendChild( document.createTextNode( valor_fonte ) );
			celula_valor_fonte.appendChild( input_hidden_valor );
			if ( link_acao_alterar != null  )
			{
				celula_acao_alterar.appendChild( link_acao_alterar );
			}
			if ( link_acao_remover != null  )
			{
				celula_acao_remover.appendChild( link_acao_remover );
			}
			// FIM insere os dados ( textos e inputs ) nas células
			// altera soma dos valores de acordo com o novo item
			item_financeiro_fonte_valor_total += desformata_decimal( valor_fonte );
			item_financeiro_fonte_atualizar_valor_total();
			// FIM altera soma dos valores de acordo com o novo item
			// atualiza as cores alternadas das linhas
			item_financeiro_fonte_organizar_cor_linha();
			return true;
		}

		/**
		 * Prepara formulário para edição de um item da tabela financeiro.
		 *
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @param string
		 * @return void
		 */
		function item_financeiro_alterar( id_tr, natureza, valor )
		{
			houve_modificacao = true;
			// captura os campos do financeiro
			var campo_natureza = document.getElementById( 'nova_natureza' );
			var campo_valor = document.getElementById( 'novo_valor' );
			campo_natureza.value = natureza;
			campo_valor.value = valor;
			item_financeiro_remover( id_tr, natureza, false );
			// retira item da lista dos já inseridos
			var id_novo_item = natureza;
			item_financeiros_adicionados[id_novo_item] = false;
		}


		/**
		 * Remove um item da tabela de financeiro.
		 *
		 * @param string
		 * @param boolean
		 * @return void
		 */
		function item_financeiro_remover( id_tr, natureza, realizar_pergunta )
		{
			if ( realizar_pergunta )
			{
				if ( !confirm( 'Deseja realmente excluir o detalhamento?' ) )
				{
					return;
				}
			}
			item_financeiros_adicionados[natureza] = '';
			houve_modificacao = true;
			var tr = document.getElementById( id_tr );
			var input = document.getElementById( id_tr + '_valor' );
			if ( tr && input )
			{

				item_financeiro_valor_total -= desformata_decimal( input.value );
				item_financeiro_atualizar_valor_total();
				tr.parentNode.removeChild( tr );
				item_financeiro_organizar_cor_linha();
			}
		}

		/**
		 * Remove um item da tabela de financeiro FONTE.
		 *
		 * @param string
		 * @param boolean
		 * @return void
		 */
		function item_financeiro_fonte_remover( id_tr, fonte, realizar_pergunta )
		{
			if ( realizar_pergunta )
			{
				if ( !confirm( 'Deseja realmente excluir o detalhamento?' ) )
				{
					return;
				}
			}
			item_financeiros_fonte_adicionados[fonte] = '';
			houve_modificacao = true;
			var tr = document.getElementById( id_tr );
			var input = document.getElementById( id_tr + '_valor' );
			if ( tr && input )
			{

				item_financeiro_fonte_valor_total -= desformata_decimal( input.value );
				item_financeiro_fonte_atualizar_valor_total();
				tr.parentNode.removeChild( tr );
				item_financeiro_fonte_organizar_cor_linha();
			}
		}


		function abreMapa(){
			var graulatitude = window.document.getElementById("graulatitude").value;
			var minlatitude  = window.document.getElementById("minlatitude").value;
			var seglatitude  = window.document.getElementById("seglatitude").value;
			var pololatitude = window.document.getElementById("pololatitude").value;

			var graulongitude = window.document.getElementById("graulongitude").value;
			var minlongitude  = window.document.getElementById("minlongitude").value;
			var seglongitude  = window.document.getElementById("seglongitude").value;

			var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
			var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
			//var obrid = document.getElementById("obrid").value;
			var janela=window.open('sisplan.php?modulo=principal/mapa&acao=A&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude+'&obrid', 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();

		}


		function item_financeiro_hilight( id_tr, loop )
		{
			var tr = document.getElementById( id_tr );
			if ( !tr )
			{
				return;
			}
			if ( loop < 1 )
			{
				item_financeiro_organizar_cor_linha();
				return;
			}
			var cor = '';
			switch ( loop-- % 9 )
			{
				case 0:	case 8:
					//cor = '#d0d0d0';
					cor = '#ffffff';
					break;
				case 1:	case 7:
					//cor = '#d8d8d8';
					cor = '#ffffdd';
					break;
				case 2:	case 6:
					//cor = '#e0e0e0';
					cor = '#ffffaa';
					break;
				case 3:	case 5:
					//cor = '#e8e8e8';
					cor = '#ffff80';
					break;
				case 4:
					//cor = '#f0f0f0';
					cor = '#ffff40';
					break;
			}
			tr.style.backgroundColor = cor;
			var funcao_hilight = new Function( ' item_financeiro_hilight( \'' + id_tr + '\', ' + loop + ' ); ' );
			setTimeout( funcao_hilight, 120 );
		}

		function item_financeiro_fonte_hilight( id_tr, loop )
		{
			var tr = document.getElementById( id_tr );
			if ( !tr )
			{
				return;
			}
			if ( loop < 1 )
			{
				item_financeiro_fonte_organizar_cor_linha();
				return;
			}
			var cor = '';
			switch ( loop-- % 9 )
			{
				case 0:	case 8:
					//cor = '#d0d0d0';
					cor = '#ffffff';
					break;
				case 1:	case 7:
					//cor = '#d8d8d8';
					cor = '#ffffdd';
					break;
				case 2:	case 6:
					//cor = '#e0e0e0';
					cor = '#ffffaa';
					break;
				case 3:	case 5:
					//cor = '#e8e8e8';
					cor = '#ffff80';
					break;
				case 4:
					//cor = '#f0f0f0';
					cor = '#ffff40';
					break;
			}
			tr.style.backgroundColor = cor;
			var funcao_hilight = new Function( ' item_financeiro_fonte_hilight( \'' + id_tr + '\', ' + loop + ' ); ' );
			setTimeout( funcao_hilight, 120 );
		}


<?php
if ( $atividade['atiid'] ):
	$sql = "
		SELECT SUBSTR(nd.ndpcod,1,6) AS ndpcod,
			   op.opavalor
		FROM pde.orcamentopa op
			 JOIN public.naturezadespesa nd
			 	ON  op.ndpid = nd.ndpid
		WHERE op.atiid = '". $_REQUEST['atiid'] ."' ";

	$dados = $db->carregar($sql);
	if ( !is_array( $dados ) )
		$dados = array();

	foreach ($dados as $dado):
?>
		item_financeiro_adicionar('<?=$dado['ndpcod']?>', '<?=number_format( $dado['opavalor'], 2, ",", "." )?>');
<?php
	endforeach;



	$sql = "
		SELECT fr.fonid||' # '||fr.fondsc AS fonid,
			   opf.opfvalor
		FROM pde.orcamentopafonte opf
			 JOIN sisplan.fonterecurso fr
			 	ON  opf.fonid = fr.fonid
		WHERE opf.atiid = '". $_REQUEST['atiid'] ."' ";

	$dados = $db->carregar( $sql );
	if ( !is_array( $dados ) )
		$dados = array();

	foreach ($dados as $dado):
?>
		item_financeiro_fonte_adicionar('<?=$dado['fonid']?>', '<?=number_format( $dado['opfvalor'], 2, ",", "." )?>');
<?php
	endforeach;


	$sql = "
		SELECT
			p.paidescricao AS pais,
			e.estuf||' - '||e.estdescricao AS uf,
			CASE r.estuf
				WHEN 'DF' THEN ra.rgadsc
				ELSE m.estuf||' - '||m.mundescricao
			END AS mun
		FROM
			sisplan.regionalizacao r
			JOIN territorios.pais p
				ON p.paiid = r.paiid
			LEFT JOIN territorios.municipio m
				ON m.muncod = r.muncod
			LEFT JOIN territorios.estado e
				ON e.estuf = r.estuf
			LEFT JOIN planointerno.regiaoadministrativa ra
				ON ra.rgaid = r.rgaid
		WHERE
			r.atiid = {$atiid}
	";

	$dados = is_array($db->carregar($sql)) ? $db->carregar($sql) : array();

	if ( !empty($dados) ):
?>
			document.getElementById('atisndetalharregionalizacao').checked = true;
			document.getElementById('trabrangencia').style.display = '';
<?php
	endif;
	foreach ($dados as $dado):
?>
			item_abrangencia_adicionar('<?=$dado['pais']?>', '<?=$dado['uf']?>', '<?=$dado['mun']?>');
<?php
	endforeach;
endif;

	if ( ($atipronacquest=='f') ):
?>
			document.formulario.atiproponente.value = "IPHAN";
			document.formulario.atiproponente.className = "leitura";
			document.getElementById("tppid").className = "CampoEstiloLeitura";
			document.getElementById("tscid").className = "CampoEstiloLeitura";
			document.formulario.atisnenviaemailtramitacao.className = "leitura";
			document.formulario.atirepresentante.className = "leitura";
			document.formulario.atiemailrepresentante.className = "leitura";

			document.formulario.atiproponente.disabled  = true;
			document.getElementById('tppid').disabled  = true;
			document.getElementById('tscid').disabled  = true;
			document.formulario.atisnenviaemailtramitacao.disabled  = true;
			document.formulario.atirepresentante.disabled  = true;
			document.formulario.atiemailrepresentante.disabled  = true;
<?php
	endif;

	if ( ($atiemenda=='f') ):
?>
			document.getElementById("tauid").className = "CampoEstiloLeitura";
			document.getElementById("ppoid").className = "CampoEstiloLeitura";
			document.getElementById("aueid").className = "CampoEstiloLeitura";
			document.getElementById('tauid').disabled  = true;
			document.getElementById('ppoid').disabled  = true;
			document.getElementById('aueid').disabled  = true;
<?php
	endif;
?>

<?


echo <<<EOS
var p_prgcod = '$prgcod';
var p_acacod = '$acacod';
var p_unicod = '$unicod';
var p_loccod = '$loccod';
var p_iteid = '$iteid';
var p_sbcid = '$sbcid';
EOS;

if ($prgcod){

	echo "listar_acoes('$prgcod');";
	if ( $acacod )
	{
		echo "listar_unidades('$acacod');";
		if ( $unicod )
		{
			echo "listar_localizadores('$unicod');";
			if ( $loccod )
			{
				echo "listar_produto('$loccod');";
				$ptres = $db->pegaUm("SELECT
											ptres
									  FROM
											sisplan.ptres
									  WHERE
											prgcod = '".$prgcod."'
										AND acacod = '".$acacod."'
										AND unicod = '".$unicod."'
										AND loccod = '".$loccod."'
										AND exercicio = '".$_SESSION['exercicio']."'");

				echo "document.formulario.ptres.value = '".$ptres."';";
			}
		}
	}
}

if ($esfid){
	echo "action_esfera($esfid);";
}
if ($dfeid){
	echo "listar_iniciativas($dfeid);";
}
if ($clsid){
	echo "listar_subclasses($clsid);";
}

?>
		</script>