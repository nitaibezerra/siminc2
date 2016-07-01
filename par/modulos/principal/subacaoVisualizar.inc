<?php
include_once APPRAIZ . "includes/workflow.php";

//if( !$_SESSION['par'] ){
//	echo "<script>alert('Erro de sessão!');window.close();</script>";
//	exit;
//}

// varifica se a subação é de ônibus acessível
function isSubacaoOnibusAcessivel($dados)
{ 
	if($dados['sbaid'])
	{
		global $db;
		
		if( is_array($dados['sbaid']) ){
			$anoatual = $dados['anoatual'];
			$sbaIdPesquisa = $dados['sbaid'][$anoatual];
		} else {
			$sbaIdPesquisa = $dados['sbaid'];
		}
		$perfil = pegaArrayPerfil($_SESSION['usucpf']);
		
		$sql = "select *  from par.subacao  where sbaid = $sbaIdPesquisa AND prgid = 153" ;
		$result = $db->pegaLinha($sql);
		if(is_array($result) && count($result))
		{
			$isTransporteAcessivel = true;
			
			$muncod = $_SESSION['par']['muncod'];
			$inuId  = $_SESSION['par']['inuid'];
			if( ($muncod) && ($inuId) )
			{
				
				$sqlCota = "
					SELECT ma.mprqtd as cota_inicial ,
						CASE WHEN /*CASO*/
						(
							       (
									      select sum( icoquantidade  )::integer
									      from  par.subacao s 
									      inner join par.subacaodetalhe sd on s.sbaid = sd.sbaid 
									      inner join par.subacaoitenscomposicao si on si.sbaid = s.sbaid  and si.icoano = sd.sbdano
									      inner join par.acao                     a on a.aciid = s.aciid
									      inner join par.pontuacao                p on p.ptoid = a.ptoid
									      WHERE  prgid in ( 153) and p.inuid = {$inuId}
									      and s.sbastatus = 'A'
							       ) /*SOLICITADO*/
							       
							       > /*MAIOR QUE */
							       
							       ma.mprqtd::integer /* COTA INICIAL */
						)
						THEN /*ENTÃO*/
						
						(
							       ma.mprqtd::integer  /*COTA INICIAL*/
							       
							       - /* MENOS */
							      (
								   select  sum( icoquantidadetecnico  )::integer
									from par.subacao s  
									inner join par.subacaodetalhe sd on s.sbaid = sd.sbaid 
									inner join par.subacaoitenscomposicao si on si.sbaid = s.sbaid  and si.icoano = sd.sbdano
									
									inner join par.acao                     a on a.aciid = s.aciid
									inner join par.pontuacao                p on p.ptoid = a.ptoid
									WHERE p.inuid =  {$inuId}
									and prgid in ( 153)
									and sd.sbdid in ( select sd.sbdid 
										       from par.empenhosubacao es 
										       inner join  par.subacaodetalhe sd on es.sbaid = sd.sbaid and sd.sbdano = es.eobano 
										       inner join par.empenho e on e.empid = es.empid and empsituacao = '2 - EFETIVADO'  and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
										       inner join par.processopar pp on pp.prpnumeroprocesso = e.empnumeroprocesso 
										       where inuid = {$inuId} and es.eobstatus = 'A'
										       )
									and s.sbastatus = 'A'
								       
								) /*ENPENHADO*/
						)
						
						ELSE  /*SENÃO */
						
						(
							       (
								      select sum( icoquantidade  )::integer
							       from  par.subacao s 
							       inner join par.subacaodetalhe sd on s.sbaid = sd.sbaid 
							       inner join par.subacaoitenscomposicao si on si.sbaid = s.sbaid  and si.icoano = sd.sbdano
							       inner join par.acao                     a on a.aciid = s.aciid
							       inner join par.pontuacao                p on p.ptoid = a.ptoid
							       WHERE  prgid in ( 153) and p.inuid =  {$inuId}
							       and s.sbastatus = 'A'
						
							       ) /* SOLICITADO */
						
							       - /*MENOS*/
						
							      (
								   select  sum( icoquantidadetecnico  )::integer
									from par.subacao s  
									inner join par.subacaodetalhe sd on s.sbaid = sd.sbaid 
									inner join par.subacaoitenscomposicao si on si.sbaid = s.sbaid  and si.icoano = sd.sbdano
									
									inner join par.acao                     a on a.aciid = s.aciid
									inner join par.pontuacao                p on p.ptoid = a.ptoid
									WHERE p.inuid =   {$inuId}
									and prgid in ( 153)
									and sd.sbdid in ( select sd.sbdid 
										       from par.empenhosubacao es  
										       inner join  par.subacaodetalhe sd on es.sbaid = sd.sbaid and sd.sbdano = es.eobano 
										       inner join par.empenho e on e.empid = es.empid and empsituacao = '2 - EFETIVADO' and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' 
										       inner join par.processopar pp on pp.prpnumeroprocesso = e.empnumeroprocesso 
										       where inuid = {$inuId} and es.eobstatus = 'A'
										       )
									and s.sbastatus = 'A'
								       
								)  /*EMPENHADO*/
						) 
						END
						
						as cota_atual
						
						
						from par.municipioadesaoprograma ma 
						
						where muncod = '{$muncod}' and pfaid = 11
				
				
				";
				
				$resultCota = $db->pegaLinha($sqlCota);
				
				$cotaInicial = $resultCota['cota_inicial'];
				$cotaAtual   = $resultCota['cota_atual'];
				if ( is_array($resultCota) && count($resultCota) )
				{
					return array(
						'temCota' => true,
						'cotaInicial'=> $cotaInicial,  
						'cotaAtual'  => $cotaAtual
					);
				}
				else
				{
					return array(
							'temCota' => false
					);
					
				}
			}
			else
			{
				return array(
						'temCota' => false
				);
				
			}
			
		}
		else
		{
			return array(
					'temCota' => false
			);
			
		}
		
	}
	else
	{
		return array(
				'temCota' => false
		);
		
	}
}

function verificaComplementoDB(){
	
	global $db;
	
	if( $_POST['ssuid'] == 20 || $_POST['ssuid'] == 23 ){
		$campos = "	coalesce(sbdrepassevlrcomplementar,0)::numeric(20,2) as sbdrepassevlrcomplementar,
					coalesce(sbdrepassevlrempenho,0)::numeric(20,2) as sbdrepassevlrempenho,
					coalesce(sbdrepassevlrraf,0)::numeric(20,2) as sbdrepassevlrraf";
	}
	if( $_POST['ssuid'] == 21 ){
		$campos = "	coalesce(sbdrepassevlrcomplementaraprovado,0)::numeric(20,2) as sbdrepassevlrcomplementaraprovado,
					coalesce(sbdrepassevlrempenhoaprovado,0)::numeric(20,2) as sbdrepassevlrempenhoaprovado,
					coalesce(sbdrepassevlrrafaprovado,0)::numeric(20,2) as sbdrepassevlrrafaprovado";
	}
	
	
	$sql = "SELECT 
				$campos
			FROM par.subacaodetalhe 
			WHERE 
				sbaid = {$_POST['sbaid']} 
				AND sbdano = '{$_POST['ano']}'";
	
	$dados = $db->pegaLinha($sql);
	
	echo implode('}{', $dados);
}

function carregaEmendaPTA(){
	global $db;

	$sql = "select vede.emdid as codigo, vede.emecod as descricao from emenda.ptemendadetalheentidade pte
				inner join emenda.v_emendadetalheentidade vede on vede.edeid = pte.edeid
			where
				pte.ptrid = {$_POST['ptrid']}
				and vede.edestatus = 'A'";
	$emeid = $db->carregar($sql);

	if( sizeof($emeid) == '1' ){
		$html = '<select maximo="0" tipo="combo_popup" multiple="multiple" size="5" name="emenda['.$_POST['ano'].'][]" id="emenda['.$_POST['ano'].']" onclick="javascript:combo_popup_alterar_campo_busca( this );"
						ondblclick="javascript:combo_popup_abre_janela( \'emenda['.$_POST['ano'].']\', 400, 400, false );" onkeydown="javascript:combo_popup_remove_selecionados( event, \'emenda['.$_POST['ano'].']\' );"
						class="CampoEstilo" style="width:400px;">
					<option value="'.$emeid[0]['codigo'].'">'.$emeid[0]['descricao'].'</option>
				</select>';
		echo $html;
	} else {
		combo_popup( 'emenda['.$_POST['ano'].']', $sql, '', '400x400', 0, array(), '', true, false, false, 5, 400 );
	}
	die();
}

function preencherParecer(){
	global $db;

	$sbaid = $_POST['sbaid'];

	$sql = "SELECT
				pp.pspdescricao
			FROM
				par.propostasubacaoparecer pp
			INNER JOIN par.subacao s ON s.ppsid = pp.ppsid
			WHERE
				s.sbaid = ".$sbaid;

	$parecer = $db->pegaUm( $sql );

	if( $parecer ){
		echo $parecer;
	} else {
		echo 1;
	}
	die;
}

function filtraPTRES(){
	global $db;

	$sql = "select
				pliptres as codigo,
				pliptres as descricao
			from par.planointerno
			where plinumplanointerno = '{$_POST['sbdplanointerno']}'";

	echo $db->monta_combo( "sbdptres[".$_POST['ano']."]", $sql, 'S', 'Selecione...', '',"","","","N","","");
	exit;
}

function recarregarEscola(){
	global $db;

	$oSes = new SubacaoEscola();

	echo $oSes->montaListaAlunos($_REQUEST['sbaid'],$_REQUEST['ano'],$_REQUEST['frmid'],$_REQUEST['ptsid']);
	die;
}

if( $_REQUEST['sbaid'] ){
	if( is_array($_REQUEST['sbaid']) ){
		if( $_REQUEST['anoatual'] ){
			$anoT = $_REQUEST['anoatual'];
		} else {
			$anoT = date('Y');
		}
		$sbaid = $_REQUEST["sbaid"][$anoT];
	} else {
		$sbaid = $_REQUEST["sbaid"];
	}
	$inuid = $db->pegaUm( "SELECT iu.inuid FROM par.subacao s
							INNER JOIN par.acao a ON a.aciid = s.aciid
							INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
							INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
							WHERE s.sbaid IN (".$sbaid.")");
}

if( !$inuid ){
	$inuid = $_REQUEST['inuid'] ? $_REQUEST['inuid'] : $_SESSION['par']['inuid'];
	$_SESSION['par']['inuid'] = $inuid;
	if( !$inuid ){
		echo "<script>alert('Falta o INUID!');window.close();</script>";
		exit;
	}
}

if( $_SESSION['par']['itrid'] == 2 ){
	//$_SESSION['par']['muncod'] = $db->pegaUm( "SELECT muncod FROM par.instrumentounidade WHERE inuid = ".$inuid );
	if( !$_SESSION['par']['muncod'] ){
		if( is_array($_REQUEST['sbaid']) ){
			$sbaid = implode(', ', $_REQUEST['sbaid']);
		} else {
			$sbaid = $_REQUEST['sbaid'];
		}
		$_SESSION['par']['muncod'] = $db->pegaUm( "SELECT iu.muncod FROM par.subacao s
													INNER JOIN par.acao a ON a.aciid = s.aciid
													INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
													INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
													WHERE s.sbaid IN (".$sbaid.")");
	}
} elseif( $_SESSION['par']['itrid'] == 1 ){
	if( is_array($_REQUEST['sbaid']) ){
		$sbaid = implode(', ', $_REQUEST['sbaid']);
	} else {
		$sbaid = $_REQUEST['sbaid'];
	}
	if( !$_SESSION['par']['estuf'] ){
		$_SESSION['par']['estuf'] = $db->pegaUm( "SELECT iu.estuf FROM par.subacao s
													INNER JOIN par.acao a ON a.aciid = s.aciid
													INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
													INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
													WHERE s.sbaid IN (".$sbaid.")");
	}
}

$docid = parPegarDocidParaEntidade($inuid);
$estadoAtual = wf_pegarEstadoAtual( $docid );

//pega os perfis do usuario
$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

$sbaid = $_GET['sbaid'];
if( empty($sbaid) ){
	echo "<script>alert('Subação não encontrada!');window.close();</script>";
	exit;
}

/** Declaração de Variaveis **/
global $db;

/** Carrega subações próxima e anterior **/
if($inuid){
	$sql = "SELECT  s.sbaid
			FROM par.subacao 	 	 s
			INNER JOIN par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus = 'A'
			INNER JOIN par.pontuacao p  ON p.ptoid  = a.ptoid AND p.ptostatus = 'A'
			INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus = 'A'
			INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
			INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
			INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
			WHERE s.sbastatus = 'A' AND
				p.inuid = {$inuid}
			ORDER BY d.dimcod,
					 ar.arecod,
					 i.indcod,
					 s.sbaordem,
					 s.sbadsc";

	$subacoes = (array) $db->carregar($sql);

	foreach($subacoes as $key => $subacao){
		if( $subacao['sbaid'] == (integer) $_GET['sbaid'] ){
			$sbaidAnterior = $subacoes[$key - 1 ]['sbaid'];
			$sbaidProximo = $subacoes[$key + 1 ]['sbaid'];
			break;
		}
	}
}

/** Carrega informações de localização e dados da subação  **/
$sql = "SELECT  s.sbaid,
				s.sbadsc,
				s.foaid,
				d.dimid,
				d.dimcod,
				d.dimdsc,
				ar.areid,
				ar.arecod,
				ar.aredsc,
				i.indcod,
				i.indid,
				i.inddsc,
				a.acidsc,
				p.ptoid,
				s.sbaordem,
				frm.frmid,
				s.sbacronograma,
				s.ppsid,
				s.ptsid,
				s.docid,
				s.sbaextraordinaria,
				s.sbareformulacao,
				pps.ppscarga,
				COALESCE(s.sbaestrategiaimplementacao,'N/A') as sbaestrategiaimplementacao,
				COALESCE(u.unddsc,'N/A') as unddsc,
				COALESCE(f.foadescricao,'N/A') as foadescricao,
				s.prgid,
				COALESCE(prg.prgdsc,'N/A') as prgdsc,
				prg.prglink,
				COALESCE(frm.frmdsc,'N/A') as frmdsc,
				COALESCE(tps.tpsdescricao, COALESCE(pts.ptsdescricao,'N/A')) as ptsdescricao,
				s.sbaextraordinariatramita
		FROM par.subacao 	 	 s
		INNER JOIN par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus IN ('A','E')
		INNER JOIN par.pontuacao p  ON p.ptoid  = a.ptoid AND p.ptostatus IN ('A','E')
		INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus IN ('A','E')
		INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
		INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
		INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
		LEFT JOIN par.formaatendimento f ON f.foaid = s.foaid
		LEFT JOIN par.propostatiposubacao pts ON pts.ptsid = s.ptsid
		LEFT JOIN par.tiposubacao tps on tps.tpsid = pts.tpsid
		LEFT JOIN par.propostasubacao pps ON pps.ppsid = s.ppsid
		LEFT JOIN par.unidademedida u ON u.undid = s.undid
		LEFT JOIN par.programa prg ON prg.prgid = s.prgid
		LEFT JOIN par.formaexecucao frm ON frm.frmid = s.frmid
		WHERE s.sbastatus IN ('A','R') AND s.sbaid = $sbaid";

$subacao = $db->pegaLinha($sql);
$subacaoPrograma = $subacao['prgid'] ? $subacao['prgid'] : '0';
$estadoAtualSubacao = wf_pegarEstadoAtual( $subacao['docid'] );

if( $estadoAtualSubacao['esdid'] == WF_SUBACAO_DILIGENCIA ){

	$sql = "SELECT
				true
			FROM
				workflow.historicodocumento hst
			INNER JOIN workflow.documento doc ON doc.docid = hst.docid AND doc.tpdid = ".WF_TPDID_SUBACOES_PAR."
			INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdidorigem AND esd.esdid = ".WF_SUBACAO_DILIGENCIA_CONDICIONAL."
			WHERE doc.docid = {$subacao['docid']}";

	$testaDiligenciaCondicional = $db->pegaUm($sql);
}

$arrPerfil = pegaPerfilGeral();

$valida = "true";
$habil  = "N";
$merito = "N";
$ssuid = array();

$docid = parPegarDocidParaEntidade($inuid);
$estadoAtual = wf_pegarEstadoAtual( $docid );

$sbaextraordinariatramita = $subacao['sbaextraordinariatramita'];

if( $estadoAtual['esdid'] == WF_ELABORACAO && !$subacao['docid'] ){ // Caso a subação não tenha docid e o par esteja em elaboração entao a subação estará em elaboração
	$estadoAtualSubacao["esdid"] = WF_SUBACAO_ELABORACAO;
}

if( $subacao['ppsid'] ){
	$sql = "SELECT DISTINCT
				pra.praanos
			FROM par.propostasubacaoanos psa
			INNER JOIN par.propostaanos pra ON pra.praid = psa.praid
			WHERE
				pra.prastatus = 'A' AND
				psa.ppsid = ".$subacao['ppsid']."
			ORDER BY
				pra.praanos";

	$anos = $db->carregarColuna( $sql );
}

if( empty($anos) ) $anos = array( 2011, 2012, 2013, 2014);

// Solicitado pelo Julio no dia 06/12 ( código 23 adicionado no dia 11/12 a pedido do Julio)
if( $subacao['sbacronograma'] == 2 ){
	$tipos = array(14, 5, 7, 19, 20);
} else {
	$tipos = array(14, 5, 7, 19, 20, 8, 9, 52, 55, 56, 23);
}

if( in_array( $subacao['ptsid'], $tipos ) ){
	$detalha = true;
} else {
	$detalha = false;
}

$habilParecer 	= "N";
$validaSalvar 	= "false";
$habilSalvar 	= "N";
$habilStatus 	= "N";
$habil 			= "N";
$valida 		= "false";
$permitido		= "N";
$merito 		= "N";
$vinculaObra 	= "N";
$validaBP		= false;
$habilita_combo_Plano_PTR 	= 'N';
$habilita_combo_PTA 		= 'N';

if( $_REQUEST['anoatual'] ){
	$sbaid = isset($_REQUEST['sbaid'][$_REQUEST['anoatual']]) ? $_REQUEST['sbaid'][$_REQUEST['anoatual']] : $_REQUEST['sbaid'];
}else{
	$sbaid = $_REQUEST['sbaid'];
}

?>
<html>
	<head>
	    <meta http-equiv="Cache-Control" content="no-cache">
	    <meta http-equiv="Pragma" content="no-cache">
	    <meta http-equiv="Connection" content="Keep-Alive">
	    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
	    <title>PAR 2010 - Plano de Metas - Subação</title>

	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <link rel="stylesheet" type="text/css" href="../par/css/subacao.css"/>

	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

	    <style>
			.disabled_textarea{
				border:none;
				border-left:#888888 3px solid;
				color:#404040;
				width:100ex;
				text-align:;
			}
		</style>
		
		<script>
			$(function(){

				$('input').attr('disabled','disabled');
				$('[name*="btn_salvar"],[name*="btn_importar"],[name*="btn_limpar"]').remove();
				$('select').attr('disabled','disabled');
				$('[src*="excluir"]').remove();

				$('.mostrar_ocultar_historico').click(function(){
					$('.img_mais_menos, #div_historico_subacao').toggle();
				});

			});
		</script>
	</head>
	<body>
		<form name="form_subacao" id="form_subacao" method="post" action="" >
			<input type="hidden" name="requisicao" value="salvarSubacao"  />
			<input type="hidden" name="anoatual" id="anoatual" value="<?php echo $_GET['anoatual']; ?>"  />
			<input type="hidden" name="tipoReform" id="tipoReform" value=""  />
			<input type="hidden" name="quant" id="quant" value="<?php echo $subacao['sbacronograma']; ?>"  />
			<input type="hidden" name="frmid" id="frmid" value="<?php echo $subacao['frmid']; ?>"  />
			<input type="hidden" name="ptsid" id="ptsid" value="<?php echo $subacao['ptsid']; ?>"  />
			<input type="hidden" name="docid" id="docid" value="<?php echo $subacao['docid']; ?>"  />
			<input type="hidden" name="esdid" id="esdid" value="<?php echo $estadoAtualSubacao["esdid"] ?>"  />
			<input type="hidden" name="reformulacao" id="reformulacao" value="<?php echo $subacao['sbareformulacao']; ?>"  />
			<input type="hidden" name="detalha" value="<?php echo $detalha; ?>"  />
			<input type="hidden" name="carregarSbaid" value=""  />
			<!--
			/*
			 * REGRA QUE LIBERA EDIÇÃO PARA OS USUÁRIOS
			 * RENILDA PERES DE LIMA
			 * JULIO CEZAR DA CAMARA RIBEIRO VIANA
			 * CRIADA EM 07/08/2012 POR WESCLEY
			 * DEMANDANTE: THIAGO TASCA
			 */
			 -->
			<input type="hidden" name="subacaoAnalise" value="<?php echo in_array($_SESSION['usucpf'], array('', '')) ? 'analise' : ''; ?>"  />
			<?php
				if( $_REQUEST['inuid'] ){
					$verificaInstrumento = $db->pegaLinha("SELECT itrid, estuf, muncod FROM par.instrumentounidade WHERE inuid = ".$_REQUEST['inuid']);
					if( $verificaInstrumento['itrid'] == 1 ){
						$_SESSION['par']['estuf'] = $verificaInstrumento['estuf'];
					}else{
						$_SESSION['par']['muncod'] = $verificaInstrumento['muncod'];
					}
				}
			?>
			<style>
			.carimbo{
				width:80px;
			}
			.div_carimbo_ref{
				position:		absolute;
				float:			right;
				margin-left:	50%;
			}
			</style>
			<?php

			$sql = "SELECT
						sbdano
					FROM 
						par.subacaodetalhe 
					WHERE
						sbaid = $sbaid
						AND ( ssuid in (20,21,23) OR sbdreformulacao IS TRUE)";
			
			$sbdano = $db->carregarColuna($sql);
			
			if( $sbdano[0] != '' ){
			?>
			<div class="div_carimbo_ref">
			<?php foreach( $sbdano as $anoR ){?>
				<img border="0" class="carimbo" align="absmiddle" src="../imagens/par/carimbo-reformulacao-<?=$anoR ?>.png">
			<?php }?>
			</div>
			<?php 
			}
			?>
			<table width="100%">
				<tr>
					<td valign="top">
						<table align="center" border="0" cellpadding="3" cellspacing="1" class="tabela tabelasubacao">
							<colgroup>
									<col width="25%" />
									<col width="75%" />
							</colgroup>
							<tbody>
								<tr>
									<td colspan="2" align="center" style="background-color: #cccccc;" >
										<b><font size="3px">Subação</font></b>
									</td>
								</tr>
								<tr>
									<td colspan="2" style="background-color:#F5F5F5;color: blue; font-size: 22px">
										<a href="javascript:exibeArvore()">
											<?php //$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod']; ?>
											<?php $entidadePar = $db->pegaUm("SELECT CASE WHEN itrid = 1 THEN estuf ELSE muncod END as entidade FROM par.instrumentounidade WHERE inuid = ".$inuid); ?>
											<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']); ?>
										</a>
									</td>
								</tr>
								<tr id="buttonsContainer">
									<td class="tdbtncontainerleft">
										<input onclick="controleproxAntSubacao('<?php echo $sbaidAnterior;?>');" type="button" name="btnAnterior" value="Anterior" id="btnAnterior" <?php echo !$sbaidAnterior ? 'disabled="disabled"' : ''; ?>/>
									</td>
									<td align="right" class="tdbtncontainerRight" >
										<input onclick="controleproxAntSubacao('<?php echo $sbaidProximo;?>');" type="button" name="btnProximo" value="Próxima" id="btnProximo" <?php echo !$sbaidProximo ? 'disabled="disabled"' : ''; ?>/>
									</td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Dimensão:</td>
									<td><?php echo $subacao['dimcod'] , '. ' , $subacao['dimdsc'] ?></td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Área:</td>
									<td><?php echo $subacao['dimcod'] , '.' , $subacao['arecod'] , '. ' , $subacao['aredsc']; ?></td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Indicador:</td>
									<td>
										<?php echo $subacao['dimcod'] , '.' , $subacao['arecod'] , '.' , $subacao['indcod']   , '. ' , $subacao['inddsc']; ?>
									</td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Ação:</td>
									<td><?php echo $subacao['acidsc']; ?></td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Tipo da subação:</td>
									<td><?php echo $subacao['ptsdescricao']; ?></td>
								</tr>
								<tr style="background-color: #cccccc;">
									<td align="left" colspan="2">
										<strong>Dados da Subação</strong>
									</td>
								</tr>
								<!--
								<tr>
									<td align='right' class="SubTituloDireita">Forma de atendimento:</td>
									<td><? // echo $subacao['foadescricao']; ?></td>
								</tr>
								 -->
								<tr>
									<td align='right' class="SubTituloDireita">Descrição da Subação:</td>
									<td><?php echo $subacao['dimcod'].".".$subacao['arecod'].".".$subacao['indcod'].".".$subacao['sbaordem']." - ".$subacao['sbadsc']; ?></td>
								</tr>
								<tr>
									<td align='right' class="SubTituloDireita">Estratégia de Implementação:</td>
									<td><?php echo $subacao['sbaestrategiaimplementacao'] ?></td>
								</tr>
								<tr>
									<td align='right' class="SubTituloDireita">Programa:</td>
									<td><?php echo $subacao['prgdsc'] ?></td>
								</tr>
								<?php if( $subacao['prglink'] ){
										if( strpos( $subacao['prglink'], 'http://' ) === false ){
											$link = 'http://'.$subacao['prglink'];
										} else {
											$link = $subacao['prglink'];
										}
									?>
									<tr>
										<td align='right' class="SubTituloDireita">Mais informações sobre o Programa:</td>
										<td><a target="_blank" href="<?php echo $link ?>"><?php echo $link ?></a></td>
									</tr>
								<?php } ?>
								<?php if($subacao['frmid'] != FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA && $subacao['frmid'] != ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA){ ?>
								<tr>
									<td align="right" class="SubTituloDireita">Unidade de Medida:</td>
									<td><?php echo $subacao['unddsc'] ?></td>
								</tr>
								<?php } ?>
								<tr>
									<td align="right" class="SubTituloDireita">Forma de Execução:</td>
									<td><?php echo $subacao['frmdsc'] ?></td>
								</tr>
								<tr>
									<td align='right' class="SubTituloDireita">Cronograma:</td>
									<td><?php echo $subacao['sbacronograma'] == 1 ? "Global" : ( $subacao['sbacronograma'] == 2 ? "Escola" : "N/A" ) ?></td>
								</tr>
								
								<?php 
									$onibusAcessivel = isSubacaoOnibusAcessivel($_REQUEST);
									
									if($onibusAcessivel["temCota"])
									{
										echo "
										<tr style='background-color: #cccccc;'>
											<td align='left' colspan='2'>
											<strong>Dados transporte escolar acessível</strong>
											</td> 
										</tr>
										<tr>
											<td align='right' class='SubTituloDireita'>Cota Inicial:</td>
											<td>{$onibusAcessivel["cotaInicial"]}</td>
										</tr>
										<tr>
											<td align='right' class='SubTituloDireita'>Cota Atualizada( Cota Inicial - Qtd Empenhada):</td>
											<td>{$onibusAcessivel["cotaAtual"]}</td>
										</tr>";
									}

									$sqlVerEmp = "select
														count(emp.empid)
													from
														par.empenhosubacao ems
													inner join par.empenho emp on emp.empid = ems.empid and empcodigoespecie not in ('03', '13', '02', '04') and eobstatus = 'A' and empstatus = 'A' 
													inner join par.processopar prp ON prp.prpnumeroprocesso = emp.empnumeroprocesso
													left join par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid
													left join par.subacaodetalhe sbd  on ems.sbaid = sbd.sbaid -- and sbdano = {$ano}
													where
														ems.sbaid = ".$sbaid;
									$verifica = $db->pegaUm($sqlVerEmp);

									if( $verifica == 0 && $testaCopia != 't' ){
										if(
											in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) ||
											in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil) ||
											(
												(
													in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrayPerfil) ||
													in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrayPerfil)
												) && $estadoAtualSubacao["esdid"] == WF_SUBACAO_ELABORACAO )
											){
										?>
										<tr>
											<td align="right" class="SubTituloDireita">&nbsp;</td>
											<td id="TdEditarSubacao" bgcolor="#dddddd"><input type="button" name="BtnEditarSubacao" id="BtnEditarSubacao" value="Editar" onclick="popupEditarSubacao('<?php echo $sbaid ?>');" /></td>
										</tr>
								<?php  }} ?>
							</tbody>
						</table>
						<table align="center" border="0" class="tabela listagem" cellpadding="0" cellspacing="0" id="tabAbasSelecaoCosano">
							<tr id="abasSelecaoCosano">
								<?php
								$colspan 		= 5;
								$tamanhoCelula 	= round(100 / $colspan);
								foreach( $anos as $ano ){
									echo "<td id=\"aba_$ano\" style=\"width: 50px;\" ><a href=\"javascript:selecionarAba('$ano')\">$ano</a></td>";
								}
								?>
								<td id="aba_totalizadores" style="width: 80px;"><a href="javascript:selecionarAba('totalizadores')" >Totalizadores<br></a></td>
							</tr>
							<?php
								$arrMeses = array(
										0 =>  array("codigo" => 1,  "descricao" => "Janeiro"),
										1 =>  array("codigo" => 2,  "descricao" => "Fevereiro"),
										2 =>  array("codigo" => 3,  "descricao" => "Março"),
										3 =>  array("codigo" => 4,  "descricao" => "Abril"),
										4 =>  array("codigo" => 5,  "descricao" => "Maio"),
										5 =>  array("codigo" => 6,  "descricao" => "Junho"),
										6 =>  array("codigo" => 7,  "descricao" => "Julho"),
										7 =>  array("codigo" => 8,  "descricao" => "Agosto"),
										8 =>  array("codigo" => 9,  "descricao" => "Setembro"),
										9 =>  array("codigo" => 10, "descricao" => "Outubro"),
										10 => array("codigo" => 11, "descricao" => "Novembro"),
										11 => array("codigo" => 12, "descricao" => "Dezembro")
								);
							?>
							<?php foreach( $anos as $ano ){ ?>
								<tr id="abaAnos<?php echo $ano; ?>" class="abaAnos">
									<td colspan="<?php echo $colspan; ?>" >
										<?php
											$valida 		= "false";
											$habil 			= "N";
											$validaSalvar   = "false";
											$habilSalvar 	= "N";
											$habilStatus 	= "N";
											$vinculaObra 	= "N";
											$habilParecer   = "N";
											$reformulacao	= "false";
											$permitido		= "N";
											$validaBP		= true;
											$reprogr		= false;
											$travReformulacao = true;

										?>
										<input type="hidden" name="sbdid[<?php echo $ano ?>]" value="<?php echo $sbdid ?>" />
										<input type="hidden" name="sbaid[<?php echo $ano ?>]" value="<?php echo $sbaid ?>" />
										<input type="hidden" name="sbdano[<?php echo $ano ?>]" value="<?php echo $ano ?>" />
										<input type="hidden" name="ssuids[<?php echo $ano ?>]" value="<?php echo $oSbd->ssuid ?>" />
										<input type="hidden" name="merito[<?php echo $ano ?>]" value="<?php echo $merito ?>" />
										<input type="hidden" name="reforco[<?php echo $ano ?>]" value="<?php echo $reforco ?>" />
										<style>
										.botao_tr:hover{
											background-color: #F8F8F8;
										}
										.botao_tr{
											width: 100%;
											background-color: #E0E0E0;
											margin-top: 1px;
											padding-top : 6px;
											padding-bottom : 6px;
											border-radius: 5px; /*5px;*/
											cursor:pointer;
										}
										.cancelar:hover{
											background-color: #F8F8F8;
										}
										.cancelar{
											background-color: #E0E0E0;
										}
										</style>
										<table class="tabela tabelaExecucaoAnos" border="0" cellpadding="3" cellspacing="0" >
											<tr class="hidden" >
												<td colspan="2" class="SubTituloTabela">Execução Financeira</td>
											</tr>
											<tr class="hidden" id="planoInterno_<?php echo $ano; ?>">
												<td class="SubTituloDireita" width="40%">Plano Interno:</td>
												<td></td>
											</tr>
											<tr class="hidden" id="numeroDoProcesso_<?php echo $ano; ?>">
												<td class="SubTituloDireita">Número do Processo:</td>
												<td></td>
											</tr>
											<?php
											#Para esse objeto "Parecer da equipe técnica", não é adcionado nos perfis, clausulas de forma de execução, as que foram definidas pelas regras,
											#pois nesse momento oucutaria o objeto, não sendo necessário porque esse objeto é disabilitado. Dessa forma esse objeto obdece as regras definidas para os perfis.
											#Perfis esses:  PAR_PERFIL_EQUIPE_TECNICA, PAR_PERFIL_EQUIPE_FINANCEIRA
												if(	(($estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_ANALISE_COMISSAO) &&
													(	in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) ||
													 	in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) ||
													 	in_array(PAR_PERFIL_EQUIPE_TECNICA, $arrayPerfil) ||
													 	in_array(PAR_PERFIL_EQUIPE_FINANCEIRA ,$arrayPerfil) ||
													 	in_array(PAR_PERFIL_COORDENADOR_GERAL ,$arrayPerfil)
													 ) ) ||
													 ( ($estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA_CONDICIONAL || $estadoAtualSubacao["esdid"] == WF_SUBACAO_APROVACAO_CONDICIONAL) &&
																(	in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) ||
																	in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) ) )
												){
											?>
											<tr>
												<td colspan="2" class="SubTituloTabela">Dados Orçamentários</td>
											</tr>
											<tr>
												<td class="SubTituloDireita" width="40%">Plano Interno:</td>
												<td>
													<?php
													$sqlPlanos = "
														SELECT 	DISTINCT
																plinumplanointerno as codigo,
																plinumplanointerno as descricao
														FROM
															par.planointerno
														WHERE pliano = ".$ano."  AND prgid = ".$subacaoPrograma;
													$db->monta_combo( "sbdplanointerno[$ano]", $sqlPlanos , $habilita_combo_Plano_PTR, 'Selecione', 'filtraPTRES', '', '','','N', 'sbdplanointerno', false, $oSbd->sbdplanointerno, 'Plano Interno' ); ?>
												</td>
											</tr>
											<tr>
										        <td class="SubTituloDireita">PTRES:</td>
										        <td id="td_ptres_<?php echo $ano; ?>">
										        	<?php $sql = "select
																	pliptres as codigo,
																	pliptres as descricao
																from par.planointerno
																where plinumplanointerno = '{$oSbd->sbdplanointerno}'";

										        	$db->monta_combo("sbdptres[$ano]",$sql, $habilita_combo_Plano_PTR,"Selecione...",'',"","","","N","","",$oSbd->sbdptres,"PTRES"); ?>
										        </td>
											</tr>
											<?php } ?>
											<?if( $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA_OBRAS ){ ?>
											<tr>
										        <td class="SubTituloDireita">PTA:</td>
										        <td id="td_pta_<?php echo $ano; ?>">
										        	<?php

										        	$pta = '';
										        	if( $oSbd->sbdid ){
											        	$sql = "SELECT ptrid FROM par.subacaoemendapta WHERE sbdid = ".$oSbd->sbdid." and sepstatus = 'A'";
											        	$pta = $db->pegaUm($sql);
										        	}

										        	if( !$_SESSION['par']['inuid'] ){
										        		$inuid = $db->pegaUm("SELECT iu.inuid FROM par.instrumentounidade iu
										        								INNER JOIN par.pontuacao p on p.inuid = iu.inuid
										        								INNER JOIN par.acao a ON a.ptoid = p.ptoid
										        								INNER JOIN par.subacao s on s.aciid = a.aciid
										        								WHERE
										        									s.sbaid = ".$sbaid);
										        	} else {
										        		$inuid = $_SESSION['par']['inuid'];
										        	}
	
										        	$iuecnpj = pegaCnpjInuid($inuid);
//										        	$iuecnpj = pegaCnpjInuid($_SESSION['par']['inuid']);

										        	if($pta){
										        		$sql = "SELECT
										        					ptr.ptrid as codigo,
										        					ptr.ptrcod as descricao
																FROM emenda.planotrabalho ptr
																	inner join emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid
																WHERE
																	ptr.ptrexercicio = '$ano'
																    and ptr.ptrstatus = 'A'
																    and enb.enbcnpj = '$iuecnpj'
																    AND ptr.ptrid = $pta
																ORDER BY ptr.ptrcod";
										        		$p = $db->pegaLinha($sql);
										        		echo $p['descricao'];
										        	}
// 										        	$db->monta_combo("pta[$ano]",$sql, 'S',"Selecione...",'carregaEmendaPTA',"","","","N","","",$pta,"PTRES"); ?>
										        </td>
											</tr>
											<tr>
										        <td class="SubTituloDireita">Código da Emenda:</td>
										        <td id="td_emenda_<?php echo $ano; ?>">
										        	<?php
										        	$arrEmenda = '';
										        	if( $oSbd->sbdid ){
											        	$sql = "SELECT distinct
																	ed.emdid as codigo,
																	eme.emecod as descricao
																FROM par.subacaoemendapta sep
																	inner join emenda.emendadetalhe ed on ed.emdid = sep.emdid
																    inner join emenda.emenda eme on eme.emeid = ed.emeid
																WHERE sep.sbdid = ".$oSbd->sbdid." and sep.sepstatus = 'A'";

											        	$arrEmenda = $db->carregar($sql);
										        	}
										        	if( $arrEmenda ){
										        		foreach ($arrEmenda as $v) {
															echo $v['descricao'].'<br>';
										        		}
										        	}
													?>
										        </td>
											</tr>
											<tr>
										        <td class="SubTituloDireita">Redução da Emenda com base em 1,2% da RCL de 2013 (25,363%):</td>
										        <td id="td_reducao_<?php echo $ano; ?>">
										        	<?php
										        	$edevalorreducao = 0;
										        	if( $oSbd->sbdid ){
											        	$sql = "SELECT distinct
																	sum(ede.edevalorreducao)
																FROM par.subacaoemendapta sep
																	inner join emenda.emendadetalhe ed on ed.emdid = sep.emdid
																    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
																WHERE sep.sbdid = ".$oSbd->sbdid." and sep.sepstatus = 'A'";

											        	$edevalorreducao = $db->pegaUm($sql);
										        	}
										        	echo 'R$ '.number_format($edevalorreducao, 2, ',', '.');
													?>
										        </td>
											</tr>
											<tr>
										        <td class="SubTituloDireita">Valor indicado pela Entidade:</td>
										        <td id="td_indicado_entidade_<?php echo $ano; ?>">
										        	<?php
										        	$edevalor = 0;
										        	if( $oSbd->sbdid ){
											        	$sql = "SELECT distinct
																	sum(ede.edevalor)
																FROM par.subacaoemendapta sep
																	inner join emenda.emendadetalhe ed on ed.emdid = sep.emdid
																    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
																    inner join emenda.ptemendadetalheentidade pte on pte.edeid = ede.edeid
																    inner join emenda.planotrabalho ptr on ptr.ptrid = pte.ptrid and ptr.ptrid = sep.ptrid
																WHERE sep.sbdid = ".$oSbd->sbdid." and sep.sepstatus = 'A'";

											        	$edevalor = $db->pegaUm($sql);
										        	}
										        	echo 'R$ '.number_format($edevalor, 2, ',', '.');
													?>
										        </td>
											</tr>
											<tr>
										        <td class="SubTituloDireita">Valor Reduzido pelo Parlamentar:</td>
										        <td id="td_reducao_parlamentar_<?php echo $ano; ?>">
										        	<?php
										        	$edevalor = 0;
										        	if( $oSbd->sbdid ){
											        	$sql = "SELECT distinct
																	sum(ede.edevalorreducao)
																FROM par.subacaoemendapta sep
																	inner join emenda.emendadetalhe ed on ed.emdid = sep.emdid
																    inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
																    inner join emenda.ptemendadetalheentidade pte on pte.edeid = ede.edeid
																    inner join emenda.planotrabalho ptr on ptr.ptrid = pte.ptrid and ptr.ptrid = sep.ptrid
																WHERE sep.sbdid = ".$oSbd->sbdid." and sep.sepstatus = 'A'";

											        	$edevalor = $db->pegaUm($sql);
										        	}
										        	echo 'R$ '.number_format($edevalor, 2, ',', '.');
													?>
										        </td>
											</tr>
											<tr>
										        <td class="SubTituloDireita">Valor aceito pela Entidade:</td>
										        <td id="td_aceito_entidade_<?php echo $ano; ?>">
										        	<?php
										        	$sepvalor = 0;
										        	if( $oSbd->sbdid ){
											        	$sql = "SELECT sepvalor FROM par.subacaoemendapta sep WHERE sep.sbdid = ".$oSbd->sbdid." and sepstatus = 'A'";
											        	$sepvalor = $db->pegaUm($sql);
										        	}
										        	echo 'R$ '.number_format($sepvalor, 2, ',', '.');
													?>
										        </td>
											</tr>
											<? }
												
											$sql = "SELECT 
														sbdrepassecomplementar as sbdrepassecomplementar,
														sbdrepasseempenho as sbdrepasseempenho,
														sbdrepasseraf as sbdrepasseraf,
														coalesce(sbdrepassevlrcomplementar::numeric			,0::numeric) as sbdrepassevlrcomplementar,
														coalesce(sbdrepassevlrempenho::numeric				,0::numeric) as sbdrepassevlrempenho,
														coalesce(sbdrepassevlrraf::numeric					,0::numeric) as sbdrepassevlrraf,
														coalesce(sbdrepassevlrcomplementaraprovado::numeric	,0::numeric) as sbdrepassevlrcomplementaraprovado,
														coalesce(sbdrepassevlrempenhoaprovado::numeric		,0::numeric) as sbdrepassevlrempenhoaprovado,
														coalesce(sbdrepassevlrrafaprovado::numeric			,0::numeric) as sbdrepassevlrrafaprovado,
														sbdreformulacao
													FROM
														par.subacaodetalhe WHERE sbaid = {$sbaid} AND sbdano = ".$ano;
											
											$dadosRepassePr = $db->pegaLinha($sql);
											$sbdrepassevlrcomplementar 			= number_format($dadosRepassePr['sbdrepassevlrcomplementar'],2,',','.');
											$sbdrepassevlrempenho 				= number_format($dadosRepassePr['sbdrepassevlrempenho'],2,',','.');
											$sbdrepassevlrraf					= number_format($dadosRepassePr['sbdrepassevlrraf'],2,',','.');
											$sbdrepassevlrcomplementaraprovado 	= number_format($dadosRepassePr['sbdrepassevlrcomplementaraprovado'],2,',','.');
											$sbdrepassevlrempenhoaprovado 		= number_format($dadosRepassePr['sbdrepassevlrempenhoaprovado'],2,',','.');
											$sbdrepassevlrrafaprovado			= number_format($dadosRepassePr['sbdrepassevlrrafaprovado'],2,',','.');

											// Se for reformulação
											if( $ssuid[$ano] == 20 || $ssuid[$ano] == 23 ){
												if( $dadosRepassePr['sbdreformulacao'] == 't' ){
													$sql = "SELECT
																(
																	par.recuperavalorplanejadossubacaoporano($sbaid, $ano)
																)
																-
																(
																	CASE WHEN
																		par.recuperavalorvalidadossubacaoporano( ( SELECT MAX(sba.sbaid) FROM par.subacao sba inner join par.subacaodetalhe sd on sd.sbaid = sba.sbaid AND sd.sbdano = $ano WHERE sba.sbastatus <> 'I' AND sba.sbaidpai = $sbaid ) , $ano) IS NOT NULL
																	THEN
																		par.recuperavalorvalidadossubacaoporano( ( SELECT MAX(sba.sbaid) FROM par.subacao sba inner join par.subacaodetalhe sd on sd.sbaid = sba.sbaid AND sd.sbdano = $ano WHERE sba.sbastatus <> 'I' AND sba.sbaidpai = $sbaid ) , $ano)
																	ELSE
																		par.recuperavalorplanejadossubacaoporano( ( SELECT MAX(sba.sbaid) FROM par.subacao sba inner join par.subacaodetalhe sd on sd.sbaid = sba.sbaid AND sd.sbdano = $ano WHERE sba.sbastatus <> 'I' AND sba.sbaidpai = $sbaid ) , $ano)
																	END
																)";
												
													$testaValores = $db->pegaUm($sql);
												}else{
													$sql = "SELECT
																par.recuperavalorplanejadossubacaoporano($sbaid, '$ano') - coalesce(sep.sepvalor,0)  as dif
															FROM
																par.subacaoemendapta sep
															INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = sep.sbdid AND sbd.sbdano = '$ano' AND sbd.sbaid = $sbaid and sep.sepstatus = 'A'";
												
													$testaValores = $db->pegaUm($sql);
											
													$sql = "SELECT
																coalesce(sep.sepvalor,0)  as vlrempenho
															FROM
																par.subacaoemendapta sep
															INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = sep.sbdid AND sbd.sbdano = '$ano' AND sbd.sbaid = $sbaid and sep.sepstatus = 'A'";
													
													$vlrEmpenho = $db->pegaUm($sql);
												}
// 											} elseif( $ssuid[$ano] == 21 ) { // se for a analise da reformulação
											} else{ // se for a analise da reformulação e outros estados pega o valor validado se existir. Senão pega valor planejado.
												if( $dadosRepassePr['sbdreformulacao'] == 't' ){
													$sql = "SELECT
															(
																par.recuperavalorvalidadossubacaoporano($sbaid, $ano)
															)
															-
															(
																--par.recuperavalorvalidadossubacaoporano( ( SELECT MAX(sba.sbaid) FROM par.subacao sba inner join par.subacaodetalhe sd on sd.sbaid = sba.sbaid AND sd.sbdano = $ano WHERE sba.sbastatus <> 'I' AND sba.sbaidpai = $sbaid ) , $ano)
																CASE WHEN
																	par.recuperavalorvalidadossubacaoporano( ( SELECT MAX(sba.sbaid) FROM par.subacao sba inner join par.subacaodetalhe sd on sd.sbaid = sba.sbaid AND sd.sbdano = $ano WHERE sba.sbastatus <> 'I' AND sba.sbaidpai = $sbaid ) , $ano) IS NOT NULL
																THEN
																	par.recuperavalorvalidadossubacaoporano( ( SELECT MAX(sba.sbaid) FROM par.subacao sba inner join par.subacaodetalhe sd on sd.sbaid = sba.sbaid AND sd.sbdano = $ano WHERE sba.sbastatus <> 'I' AND sba.sbaidpai = $sbaid ) , $ano)
																ELSE
																	par.recuperavalorplanejadossubacaoporano( ( SELECT MAX(sba.sbaid) FROM par.subacao sba inner join par.subacaodetalhe sd on sd.sbaid = sba.sbaid AND sd.sbdano = $ano WHERE sba.sbastatus <> 'I' AND sba.sbaidpai = $sbaid ) , $ano)
																END
															)";
	
													$testaValores = $db->pegaUm($sql);
												}else{
													$sql = "SELECT
																par.recuperavalorvalidadossubacaoporano($sbaid, '$ano') - coalesce(sep.sepvalor,0)  as dif
															FROM
																par.subacaoemendapta sep
															INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = sep.sbdid AND sbd.sbdano = '$ano' AND sbd.sbaid = $sbaid and sep.sepstatus = 'A'";
	
													$testaValores = $db->pegaUm($sql);
	
													$sql = "SELECT
																coalesce(sep.sepvalor,0)  as vlrempenho
															FROM
																par.subacaoemendapta sep
															INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = sep.sbdid AND sbd.sbdano = '$ano' AND sbd.sbaid = $sbaid and sep.sepstatus = 'A'";
	
													$vlrEmpenho = $db->pegaUm($sql);
												}
											}
											if($_SESSION['par']['itrid'] == 2 ){
												$local = $db->pegaUm( "SELECT mundescricao || '-' || estuf FROM territorios.municipio WHERE muncod = '{$_SESSION['par']['muncod']}'" );
											} else {
												$local = $_SESSION['par']['estuf'];
											}
											if( 
												$testaValores > 0 && 
												(
													(float)$sbdrepassevlrcomplementaraprovado 	> 0 ||
													(float)$sbdrepassevlrempenhoaprovado 		> 0 ||
													(float)$sbdrepassevlrrafaprovado			> 0 
												) 
											){
												if( $dadosRepassePr['sbdreformulacao'] == 't' ){
													$sql = "SELECT
																(
																par.recuperavalorplanejadossubacaoporano($sbaid, $ano)
																)
																-
																(
																--par.recuperavalorvalidadossubacaoporano( ( SELECT MAX(sba.sbaid) FROM par.subacao sba inner join par.subacaodetalhe sd on sd.sbaid = sba.sbaid AND sd.sbdano = $ano WHERE sba.sbastatus <> 'I' AND sba.sbaidpai = $sbaid ) , $ano)
																CASE WHEN
																	par.recuperavalorvalidadossubacaoporano( ( SELECT MAX(sba.sbaid) FROM par.subacao sba inner join par.subacaodetalhe sd on sd.sbaid = sba.sbaid AND sd.sbdano = $ano WHERE sba.sbastatus <> 'I' AND sba.sbaidpai = $sbaid ) , $ano) IS NOT NULL
																THEN
																	par.recuperavalorvalidadossubacaoporano( ( SELECT MAX(sba.sbaid) FROM par.subacao sba inner join par.subacaodetalhe sd on sd.sbaid = sba.sbaid AND sd.sbdano = $ano WHERE sba.sbastatus <> 'I' AND sba.sbaidpai = $sbaid ) , $ano)
																ELSE
																	par.recuperavalorplanejadossubacaoporano( ( SELECT MAX(sba.sbaid) FROM par.subacao sba inner join par.subacaodetalhe sd on sd.sbaid = sba.sbaid AND sd.sbdano = $ano WHERE sba.sbastatus <> 'I' AND sba.sbaidpai = $sbaid ) , $ano)
																END
																)";
												
													$difPlanejado = $db->pegaUm($sql);
												}else{
													$sql = "SELECT
																par.recuperavalorplanejadossubacaoporano($sbaid, '$ano') - coalesce(sep.sepvalor,0)  as dif
															FROM
																par.subacaoemendapta sep
															INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = sep.sbdid AND sbd.sbdano = '$ano' AND sbd.sbaid = $sbaid and sep.sepstatus = 'A'";
														
													$difPlanejado = $db->pegaUm($sql);
												}
											?>
											<tr>
												<td colspan="2" class="SubTituloTabela">
													<font color="red">Complementação</font>
												</td>
											</tr>
											<tr>
												<td colspan="2">
													<table class="tabela tabelaExecucaoAnos" border="0" cellpadding="3" cellspacing="0" >
														<tr>
															<td width="20%"><b>Complemento</b></td>
															<td><b>Valor Planejado</b></td>
															<td><b>Valor Valor Aprovado</b></td>
														</tr>
														<tr>
															<td class="SubTituloDireita">Valor Complementar Total:</td>
															<td>
																R$ <?=($difPlanejado < 0 ? '0,00' : number_format($difPlanejado,2,',','.') ); ?>
															</td>
															<td>
																R$ <?=number_format($testaValores,2,',','.') ?>
															</td>
														</tr>
														<tr>
															<td class="SubTituloDireita"><?=$local ?>:</td>
															<td>
																R$ <?=$sbdrepassevlrcomplementar; ?>
															</td>
															<td>
																R$ <?=$sbdrepassevlrcomplementaraprovado; ?>
															</td>
														</tr>
														<tr>
															<td class="SubTituloDireita">Empenho Complementar (MEC/FNDE):</td>
															<td>
																R$ <?=$sbdrepassevlrempenho; ?>
															</td>
															<td>
																R$ <?=$sbdrepassevlrempenhoaprovado; ?>
															</td>
														</tr>
														<tr>
															<td class="SubTituloDireita">RAF (MEC/FNDE):</td>
															<td>
																R$ <?=$sbdrepassevlrraf; ?>
															</td>
															<td>
																R$ <?=$sbdrepassevlrrafaprovado; ?>
															</td>
														</tr>
													</table>
												</td>
											</tr>
											<?php 
											}
											
											$sql = "SELECT true FROM par.subacao WHERE sbaidpai = $sbaid";

											$testaReformulacao = $db->pegaUm($sql);

											if( $testaReformulacao == 't' && $dadosRepassePr['sbdreformulacao'] == 't' ){
											?>
											<tr>
												<td colspan="2" class="SubTituloTabela">
													<font color="red">Histórico de Reformulações</font> 
													<img src="../imagens/mais.gif" style="cursor:pointer;" onclick="abrefecha('abre', '<?php echo $ano; ?>', 'div_reformular')">
														&nbsp;
													<img src="../imagens/menos.gif" style="cursor:pointer;" onclick="abrefecha('fecha', '<?php echo $ano; ?>', 'div_reformular')">
												</td>
											</tr>
											<tr>
												<td colspan="2">
													<div id="div_reformular_<?=$ano ?>" style="display: none;">
														<table class="tabela tabelaExecucaoAnos" border="0" cellpadding="3" cellspacing="0" width="100%">
															<tr>
																<td>
																<?
																$sql = "SELECT DISTINCT
																			'<center><img style=\"cursor:pointer\" src=\"/imagens/alterar.gif\" onclick=\"listarSubacao(\'' || sd.sbaid || '\');\" border=\"0\" />' as acao,
																			s.sbadsc,
																			sd.sbdano,
																			'<center>'||to_char(s.sbadatareformulacao, 'DD/MM/YYYY')||'</center>' as data
																		FROM
																			par.subacao s
																		INNER JOIN par.subacaodetalhe sd ON s.sbaid = sd.sbaid
																		WHERE
																			s.sbastatus <> 'I' AND
																			s.sbaidpai = ".$subacao['sbaid']." AND
																			sd.sbdano = $ano";

																$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Ano da Subação", "Data da Reprogramação" );
																$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');
																?>
																</td>
															</tr>
														</table>
													</div>
												</td>
											</tr>
											<? }

												$sql = "select
															emp.empid,
															emp.empdata as data,
															empnumero as notaempenho,
															emp.empnumeroprocesso as numeroprocesso,
															dop.dopnumerodocumento || '/' || date_part('year',dopdatainclusao) as numerotermo,
															sbdplanointerno,
															sbdptres,
															CASE WHEN emr.vrlreforco IS NOT NULL THEN
																sum(vve.vrlempenhocancelado+emr.vrlreforco)
															ELSE
																sum(vve.vrlempenhocancelado)
															END as valorempenho,
															ems.eobpercentualemp as percentualempenho
														from
															par.empenhosubacao ems
															inner join par.empenho emp on emp.empid = ems.empid and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
															inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
															inner join par.processopar prp ON prp.prpnumeroprocesso = emp.empnumeroprocesso
															left join par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid
															left join par.subacaodetalhe sbd  on ems.sbaid = sbd.sbaid and sbdano = {$ano}
															left join (
																select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlreforco, ep.empcodigoespecie, sd1.sbdid 
																from par.empenho ep
																	inner join par.empenhosubacao es on es.empid = ep.empid and empstatus = 'A'
																	inner join par.subacaodetalhe sd1 on sd1.sbaid = es.sbaid and sd1.sbdano = es.eobano
																where ep.empcodigoespecie in ('02')
																group by 
																	ep.empnumeroprocesso,
																	ep.empcodigoespecie,
																	ep.empidpai,
																	sd1.sbdid
															) as emr on emr.empidpai = emp.empid and emr.sbdid = sbd.sbdid
														where
															ems.sbaid = ".$_GET['sbaid']."  AND ems.eobano = ".$ano." AND ems.eobstatus = 'A'
														GROUP BY
															emp.empid,
															emp.empdata,
															empnumero,
															emp.empnumeroprocesso,
															dop.dopnumerodocumento,
															dopdatainclusao,
															sbdplanointerno,
															sbdptres,
															emr.vrlreforco,
															ems.eobpercentualemp";
												$numerosEmpenhos = $db->carregar( $sql );
												if( $numerosEmpenhos[0]['empid'] ){ ?>
													<tr>
															<td colspan="2" class="SubTituloTabela">Empenhos</td>
													</tr>
											<?php
													if( is_array($numerosEmpenhos) ){
														foreach( $numerosEmpenhos as $i => $numerosEmpenho ){

											?>
												<tr>
													<td colspan="2">Dados do Empenho <?=$i+1 ?> (<?=formata_data($numerosEmpenho['data']) ?>) &nbsp;
														<img src="../imagens/mais.gif" style="cursor:pointer;" onclick="abrefecha('abre', '<?php echo $ano; ?>', 'empenho_<?=$i ?>')">
															&nbsp;
														<img src="../imagens/menos.gif" style="cursor:pointer;" onclick="abrefecha('fecha', '<?php echo $ano; ?>', 'empenho_<?=$i ?>')"></td>
												</tr>
													<tr><td colspan="2">
													<div id="empenho_<?=$i ?>_<?=$ano ?>" style="display: none;">
														<table class="tabela tabelaExecucaoAnos" border="0" cellpadding="3" cellspacing="0" width="100%">
														<colgroup>
															<col width="25%" />
															<col width="35%" />
															<col width="20%" />
															<col width="20%" />
														</colgroup>
														<tr>
															<td class="SubTituloDireita">Nota de Empenho:</td>
															<td colspan="3">
																<?php echo $numerosEmpenho['notaempenho']; ?>
															</td>
														</tr>
														<tr>
															<td class="SubTituloDireita">Nº do Processo:</td>
															<td colspan="3">
																<?php echo $numerosEmpenho['numeroprocesso']; ?>
															</td>
														</tr>
														<tr>
															<td class="SubTituloDireita">Nº do Termo:</td>
															<td colspan="3">
																<?php echo $numerosEmpenho['numerotermo']; ?>
															</td>
														</tr>
														<tr>
															<td class="SubTituloDireita">Nº do PI:</td>
															<td colspan="3">
																<?php echo $numerosEmpenho['sbdplanointerno']; ?>
															</td>
														</tr>
														<tr>
															<td class="SubTituloDireita">Nº do PTRES:</td>
															<td colspan="3">
																<?php echo $numerosEmpenho['sbdptres']; ?>
															</td>
														</tr>
														<tr>
															<td class="SubTituloDireita">Valor do Empenho:</td>
															<td colspan="3">
																<?php echo number_format($numerosEmpenho['valorempenho'],2,',','.'). ' ('. $numerosEmpenho['percentualempenho'] .' %)'; ?>
															</td>
														</tr>
														<?php
															$sqlPagamento = "SELECT
																				pag.pagdatapagamento as data,
																				pob.pobvalorpagamento as valorpagamento,
																				COALESCE(pob.pobpercentualpag,0) as percentualpagamento,
																				coalesce(pagnumeroob,'-') as pagnumeroob
																			FROM
																				par.pagamentosubacao pob
																			INNER JOIN par.pagamento pag on pag.pagid = pob.pagid AND pag.pagstatus = 'A'
																			WHERE
																				pob.pobano = {$ano} AND pag.empid = ".$numerosEmpenho['empid'];

															$dadosPagamento = $db->carregar( $sqlPagamento );
															if( is_array( $dadosPagamento ) ){ ?>
																<tr>
																		<td colspan="4" class="SubTituloTabela">Dados do Pagamento</td>
																</tr>
														<?php
																foreach( $dadosPagamento as $p => $dadosPag ){
														?>
														<tr>
															<td class="SubTituloDireita">Valor do Pagamento <?=$p+1 ?> <br>(<?=formata_data( $dadosPag['data'] ) ?>):</td>
															<td>
																<?php echo number_format($dadosPag['valorpagamento'],2,',','.'). ' ('. $dadosPag['percentualpagamento'] .' %)'; ?>
															</td>
															<td class="SubTituloDireita">Número da OB :</td>
															<td>
																<?=$dadosPag['pagnumeroob'] ?>
															</td>
														</tr>
														<? 		} 
															} ?>
														</table>
													</div>
													</td></tr>
											<?
														} 
													}
												}
											?>
											<tr>
												<td colspan="2" class="SubTituloTabela">Quantidades e Cronograma de Execução</td>
											</tr>
											<?php if($subacao['sbacronograma'] != 2 && ($subacao['frmid'] != FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA && $subacao['frmid'] != ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA) && $subacao['frmid'] != FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS ): ?>
											<tr id="cronogramaGlobal_<?php echo $ano; ?>">
												<td class="SubTituloDireita">Quantidade:</td>
												<td>
													<?php echo campo_texto("sbdquantidade[$ano]","N",$habil,"Quantidade","20","20","[.###]","","","","","sbdquantidade_$ano","", $oSbd->sbdquantidade ? number_format($oSbd->sbdquantidade,0,2,'.') : "", "this.value=mascaraglobal('[.###]',this.value);" ) ?>
												</td>
											</tr>
											<? endif; ?>
											<tr>
												<td class="SubTituloDireita">Cronograma Físico:</td>
												<td>
													<?php
													$habil = 'N';
													$db->monta_combo("sbdinicio[$ano]",$arrMeses, $habil,"Selecione...","","","","","N","sbdinicio_$ano","",$oSbd->sbdinicio)
													?>
													a
													<?php $db->monta_combo("sbdfim[$ano]",$arrMeses, $habil,"Selecione...","","","","","N","sbdfim_$ano","",$oSbd->sbdfim) ?>
													 <span style="margin-left:50px;font-weight:bold">Ano de Término:</span>
													<?php echo campo_texto("sbdanotermino[$ano]","N",$habil,"Ano de Término","5","4","[#]","","","","","sbdanotermino_$ano","",$oSbd->sbdanotermino) ?>
												</td>
											</tr>
											<?php
												if($subacao['frmid'] != FORMA_EXECUCAO_FINANCIAMENTO_BNDES){
													if(	$subacao['sbacronograma'] == 2 && $subacao['frmid'] != FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS	){
											?>
												<tr>
													<td colspan="2" class="SubTituloTabela">Escolas
														<img src="../imagens/mais.gif" style="cursor:pointer;" onclick="abrefecha('abre', '<?php echo $ano; ?>', 'escolas')">
															&nbsp;
														<img src="../imagens/menos.gif" style="cursor:pointer;" onclick="abrefecha('fecha', '<?php echo $ano; ?>', 'escolas')">
													</td>
												</tr>
												<tr>
													<td id="escolas_<?php echo $ano; ?>" colspan="2" style="display: none;">
														<?php $oSes = new SubacaoEscola();?>
														<?php $oSes->montaLista($_GET['sbaid'], $ano, $subacao['frmid'], $subacao['ptsid']); ?>
														<input type="hidden" name="escolas[<?php echo $ano ?>]" value="<?php echo $oSes->conta($_GET['sbaid'],$ano); ?>" />
													</td>
												</tr>
											<?php 
													}
												if(	$subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA || $subacao['frmid'] ==  ASSISTENCIA_FINANCEIRA_BRASIL_PRO ||
													$subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EXTRAORDINARIA ||
													($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO && $subacao['ptsid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO_COM_ITENS) ||
													($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO && $subacao['ptsid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO_COM_ITENS)
												){
											?>
											<tr>
												<td colspan="2" class="SubTituloTabela">Itens de Composição</td>
											</tr>
											<tr>
												<td id="itens_<?php echo $ano; ?>" colspan="2" >
												<?php $oIC = new SubacaoItensComposicao();?>
												<?php $oIC->montaLista($_GET['sbaid'], $ano, $subacao['sbacronograma'], $valida, $reformulacao)?>
												<input type="hidden" name="itens[<?php echo $ano ?>]" value="<?php echo $oIC->conta($_GET['sbaid'],$ano); ?>" />
												</td>
											</tr>
											<?
												}
												if( $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EMENDA_OBRAS || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_BRASIL_PRO_OBRAS){?>
											<tr>
												<td colspan="2" class="SubTituloTabela">Obras
													<img src="../imagens/mais.gif" style="cursor:pointer;" onclick="abrefecha('abre', '<?php echo $ano; ?>', 'obras')">
													&nbsp;
													<img src="../imagens/menos.gif" style="cursor:pointer;" onclick="abrefecha('fecha', '<?php echo $ano; ?>', 'obras')">
												</td>
											</tr>
											<tr>
												<td id="obras_<?php echo $ano; ?>" colspan="2" style="display: none;">
													<?php $oPre = new PreObra();?>
													<?php $oPre->montaLista($_GET['sbaid'],$ano);?>
												</td>
											</tr>
											<?php 
												}
												$arrPpsid = Array(1171, 1256, 563, 924, 906, 913, 925, 914, 904);
												if(
													($subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA && $vinculaObra == 'S')  
													||	
													( 
														( in_array(PAR_PERFIL_ADM_OBRAS, $arrayPerfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) ) 
														&& ( in_array($subacao['ppsid'], $arrPpsid) )
													)
												){ 
												?>
											<tr>
												<td colspan="2" class="SubTituloTabela">Vincular Obras</td>
											</tr>
											<tr>
												<td id="itens_<?php echo $ano; ?>" colspan="2" >
												<?php $oPre = new PreObra();?>
												<?php $oPre->montaListaVinculadas($_GET['sbaid'],$ano);?>
												</td>
											</tr>
											<tr>
												<td class="SubTituloEsquerda" colspan="2" >
													<img class="middle link" onclick="vincularObras('<?php echo $ano ?>','<?php echo $_GET['sbaid'] ?>')"  src="../imagens/gif_inclui.gif" /> <a href="javascript:vincularObras('<?php echo $ano ?>','<?php echo $_GET['sbaid'] ?>')">Vincular / Desvincular Obras</a>
												</td>
											</tr>
											<?php 
												}
											}?>
										</table>
									</td>
								</tr>
							<? }// Fim do foreach anos ?>
							<? // Inicio do totalizadores ?>
							<tr id="abaAnostotalizadores" class="abaAnos">
								<td colspan="<?php echo $colspan; ?>" >
									<?php $oSbd = new SubacaoDetalhe(); ?>
									<?php $arrSbdid = $oSbd->retornaDetalheSubacaoTotal($sbaid);

									foreach( $anos as $ano ){
										$select .= "coalesce(sum(ano".$ano."),0) AS a".$ano.",";
										$case .= "CASE WHEN sbdano = ".$ano." THEN coalesce(sbdquantidade,0) END AS ano".$ano.",";
									}

									$sql = "SELECT	{$select}
								                	coalesce(sum(stotal) ,0) AS total
								            FROM (
								                SELECT
												{$case}
								            	sum(sbdquantidade) as stotal
								            FROM par.subacaodetalhe where sbaid = {$sbaid} AND sbdano IN (".implode(', ', $anos).") GROUP BY sbdano, sbdquantidade) AS valores";

									$anos[] = 'Total';

									?>
									<table class="tabela tabelaExecucaoAnos" border="0" cellpadding="3" cellspacing="0" >
										<?php if($subacao['sbacronograma'] == 1){ ?>
										<tr>
											<td colspan="2" class="SubTituloTabela">Quantidades e Cronograma de Execução</td>
										</tr>
										<tr>
											<td colspan="2"><?php $db->monta_lista_simples($sql, $anos, 1000, 1000, 'N', '100%','N'); ?></td>
										</tr>
										<?php } ?>
										<?php if($subacao['sbacronograma'] == 2): ?>
												<tr>
													<td colspan="2" class="SubTituloTabela">Escolas</td>
												</tr>
												<tr>
													<td id="escolas_<?php echo $ano; ?>" colspan="2" >
														<?php $oSes = new SubacaoEscola();?>
														<?php $oSes->montaLista($_GET['sbaid'])?>
													</td>
												</tr>
											<?php endif;
										if($subacao['frmid'] != FORMA_EXECUCAO_FINANCIAMENTO_BNDES){
											if( $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_MOB_EQUIPE_PROINFANCIA || $subacao['frmid'] == ASSISTENCIA_FINANCEIRA_EXTRAORDINARIA ||
												($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO && $subacao['ptsid'] == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO_COM_ITENS) ||
												($subacao['frmid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO && $subacao['ptsid'] == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO_COM_ITENS)
											){?>
											<tr>
												<td colspan="2" class="SubTituloTabela">Itens de Composição</td>
											</tr>
											<tr>
												<td id="itens_<?php echo $ano; ?>" colspan="2" >
												<?php $oIC = new SubacaoItensComposicao();?>
												<?php $oIC->montaLista($_GET['sbaid'], "totalizadores", $subacao['sbacronograma'], $valida)?>
												</td>
											</tr>
											<?} else if( $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA_OBRAS ){?>
											<tr>
												<td colspan="2" class="SubTituloTabela">Obras</td>
											</tr>
											<tr>
												<td id="obras_<?php echo $ano; ?>" colspan="2" >
													<?php $oPre = new PreObra();?>
													<?php $oPre->montaLista($_GET['sbaid'],$ano)?>
												</td>
											</tr>
												<?} ?>
										<?php } ?>
									</table>
								</td>
							</tr>
							<? //fim do totalizadores ?>
						</table>
					</td>
				</tr>
			</table>
		</form>
		<script type="text/javascript"><!--
		
			function orcamentoImpositivo(ano, sbaid){
				window.open('par.php?modulo=principal/orcamentoImpositivoPar&acao=A&aba=orcamento&ano='+ano+'&sbaid='+sbaid,'Orcamento','scrollbars=yes,location=no,toolbar=no,menubar=no,width=800,height=600'); 
				w.focus();
			}

			function number_format( number, decimals, dec_point, thousands_sep ) {
			    // %     nota 1: Para 1000.55 retorna com precisão 1 no FF/Opera é 1,000.5, mas no IE é 1,000.6
			    // *     exemplo 1: number_format(1234.56);
			    // *     retorno 1: '1,235'
			    // *     exemplo 2: number_format(1234.56, 2, ',', ' ');
			    // *     retorno 2: '1 234,56'
			    // *     exemplo 3: number_format(1234.5678, 2, '.', '');
			    // *     retorno 3: '1234.57'
			    // *     exemplo 4: number_format(67, 2, ',', '.');
			    // *     retorno 4: '67,00'
			    // *     exemplo 5: number_format(1000);
			    // *     retorno 5: '1,000'
			    // *     exemplo 6: number_format(67.311, 2);
			    // *     retorno 6: '67.31'

			    var n = number, prec = decimals;
			    n = !isFinite(+n) ? 0 : +n;
			    prec = !isFinite(+prec) ? 0 : Math.abs(prec);
			    var sep = (typeof thousands_sep == "undefined") ? ',' : thousands_sep;
			    var dec = (typeof dec_point == "undefined") ? '.' : dec_point;

			    var s = (prec > 0) ? n.toFixed(prec) : Math.round(n).toFixed(prec); //fix for IE parseFloat(0.55).toFixed(0) = 0;

			    var abs = Math.abs(n).toFixed(prec);
			    var _, i;

			    if (abs >= 1000) {
			        _ = abs.split(/\D/);
			        i = _[0].length % 3 || 3;

			        _[0] = s.slice(0,i + (n < 0)) +
			              _[0].slice(i).replace(/(\d{3})/g, sep+'$1');

			        s = _.join(dec);
			    } else {
			        s = s.replace('.', dec);
			    }

			    return s;
			}

			$(function() {
				if($('#anoatual').val()){
					selecionarAba($('#anoatual').val());
				} else {
					selecionarAba('<?php echo date("Y"); ?>');
					$('#anoatual').val('<?php echo date("Y"); ?>');
				}
			});

			$(document).ready(function(){
				$('input').keyup();
			});
			
			function verificaValorReformulacao(valorComp){
				var total = new Number();
				var valorComp = new Number(valorComp);
				$('[name^="sbdrepassevlr"]').each(function() {
					vlr = this.value.replace('.','');
					vlr = vlr.replace(',','.');
					valor = new Number(vlr);
					total = new Number(total + valor);
				});

				if( total > valorComp ){
					alert('O valor é maior que o permitido!');
					$('[name^="btn_salvar_"]').attr('disabled','disabled');
				} else {
					$('[name^="btn_salvar_"]').attr('disabled','');
				}
			}

			function abrefecha(funcao, ano, tipo){
				if( funcao == 'abre' ){
					$("#"+tipo+"_"+ano).show();
				} else {
					$("#"+tipo+"_"+ano).hide();
				}
			}

			function filtraPTRES(plicod) {
				ano = $('#anoatual').val();
				if(plicod!=''){
					$.ajax({
					   type: "POST",
					   url: window.location,
					   data: "requisicaoAjax=filtraPTRES&" + "sbdplanointerno=" + plicod + "&ano=" + ano,
					   success: function(resp){
							document.getElementById("td_ptres_"+ano).innerHTML = resp;
						}
					});
				}
			}

			function carregaEmendaPTA(ptrid) {
				ano = $('#anoatual').val();
				if(ptrid != '' ){
					$.ajax({
					   type: "POST",
					   url: window.location,
					   data: "requisicaoAjax=carregaEmendaPTA&" + "ptrid=" + ptrid + "&ano=" + ano,
					   success: function(resp){
							document.getElementById("td_emenda_"+ano).innerHTML = resp;
						}
					});
				}
			}

			function popupSelecionarEscolasItemComposicao(sbaid, ano, icoid, tipo){
				url = "par.php?modulo=principal/quantidadeEscola&acao=A&ano=" + ano + "&sbaid=" + sbaid + "&icoid=" + icoid + "&tipo=" + tipo;
	    		janela(url,900,500,"Quantidade por escolas "+tipo);
			}

			function recarregaEscolas(sbaid, ano, frmid, ptsid){
				divCarregando();
				$.ajax({
					   type: "POST",
					   url: window.location,
					   data: "requisicaoAjax=recarregarEscola&sbaid=" + sbaid + "&ano=" + ano + "&frmid=" + frmid + "&ptsid="+ptsid,
					   success: function(res){
							$( '#escolas_' + ano ).html( res );
							divCarregado();
						}
				 });
			}

			function selecionarAba(cosano)
			{
            	$('[id^="abaAnos"]').attr("style","display:none");
            	$('[id^="aba_"]').attr("style","width:50px;margin: 0;padding: 0 0 0 5px;background: url(\"../../imagens/left_both.gif\") no-repeat left top;border-bottom: 1px solid #765;background-position: 0% 0%");
            	$('[id^="aba_"] a').attr("style","display: block;background: url(\"../../imagens/right_both.gif\") no-repeat right top;padding: 5px 0 4px 0;text-decoration: none;font-weight: bold;color: #765;width: auto;font-size:12px");
            	$('#spanAno').innerHTML = cosano;
        		$('#abaAnos' + cosano).attr("style","display:table-row");
				$('#aba_' + cosano).attr("style","width:50px;background-position:0% -150px;color: #333;");
				$('#aba_' + cosano + ' a:first').attr("style","background-position:100% -150px;color: #333;");
				$('#anoatual').val(cosano);
				if( cosano == 'totalizadores' ){
	            	$('[id^="linhaAno_"]').attr("style","display:none");
	            //	$('[id^="linhaAno_totalizadores"]').attr("style","");
	    			$('input[name=btn_salvar_anterior]').attr( "disabled", "disabled" );
	    			$('input[name=btn_salvar]').attr( "disabled", "disabled" );
	    			$('input[name=btn_salvar_proximo]').attr( "disabled", "disabled" );
	    			$('input[name=btn_limpar]').attr( "disabled", "disabled" );
	    			$('input[name=btn_importar]').attr( "disabled", "disabled" );
				} else {
	            	$('[id^="linhaAno_"]').attr("style","display:none");
	            	$('[id^="linhaAno_'+cosano+'"]').attr("style","");
	    			$('input[name=btn_salvar_anterior]').attr( "disabled", "" );
	    			$('input[name=btn_salvar]').attr( "disabled", "" );
	    			$('input[name=btn_importar]').attr( "disabled", "" );
	    			$('input[name=btn_salvar_proximo]').attr( "disabled", "" );
				}
				$('[type="text"]').keyup();
    		}

    		function exibeArvore()
    		{
    			window.opener.location.href = "par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore";
    			window.close();
    		}

    		function fechar()
    		{
    			window.opener.document.location.reload();
    		}

    		function verificaAnoValido(obj,ano)
    		{
    			if(obj.value < ano){
    				alert('O ano de término deve ser maior que ' + ano + '!');
    				$('[name=' + obj.name + ']').val("");
    			}
    		}

    		function consultarObra(preid,sbaid,ano)
    		{
    			url = "par.php?modulo=principal/subacaoObras&acao=A&sbaid=" + sbaid + "&ano=" + ano+ "&preid=" + preid;
    			janela(url,800,600,"Pré-Obra da Subação")
    		}

    		function listarSubacao(sbaid){
				var local = "par.php?modulo=principal/subacaoVisualizar&acao=A&sbaid=" + sbaid;
				janela(local,800,600,"Subação2");
			}
		</script>
	</body>
</html>