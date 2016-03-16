<?php

include_once APPRAIZ . 'includes/workflow.php';

if( $_POST['requisicao'] == 'carregarListaObras' ){
	$sbaid = $_POST['sbaid'];
	$sql = "select 
				p.preid,
			    p.preano,
			    p.predescricao,
			    p.prevalorobra
			from
				obras.preobra p
			    inner join par.subacaoobra so on so.preid = p.preid
			where
				so.sbaid = {$sbaid}
			order by
				p.predescricao";
	
	$cabecalho = array("Código", "Ano","Descrição","Valor");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%','S');
	exit();
}

if( $_POST['requisicao'] == 'detalharRecursoEmenda' ){
	detalharDistribuicaoRecurso($_POST);
	exit();
}

if( $_POST['requisicao'] == 'salvarDetalharRecurso' ){
	echo salvarDetalharRecursoObra($_POST);
	exit();
}

if( $_POST['requisicao'] == 'salvarDistribuicaoObra' ){
	$sql = "update obras.preobra set prevalorsuplementar = {$_POST['prevalorsuplementar']}, prevalorcomplementarfnde = {$_POST['prevalorcomplementarfnde']} where preid = {$_POST['preid']}";
	$db->executar($sql);
	echo $db->commit();
	exit();
}

if( $_POST['recusarProrrogacao'] == true ){
	global $db;

	$sql = "UPDATE obras.preobraprorrogacao SET popstatus = 'F' WHERE popstatus = 'P' AND preid = ".$_POST['preid'];
	$db->executar($sql);

	$docid = $db->pegaUm("SELECT docid FROM obras.preobra WHERE preid='".$_POST['preid']."'");
	$result = wf_alterarEstado( $docid, WF_AEDID_PAR_AGUARDANDO_PRORROGACAO_ENVIAR_PARA_OBRA_APROVADA, 'Prorrogação de prazo cancelada.', $d = array('preid' => $_POST['preid']));
	
	if( $result ){
		if( $db->commit() ){
			ob_clean();
			echo 'sucesso';
		} else {
			ob_clean();
			echo 'erro';
		}
	}else{
		$db->rollback();
		ob_clean();
		echo 'erro';
	}
	
	die();
}

if( $_POST['aceitarProrrogacao'] == true ){
	global $db;

	return gerarDocumentoProrrogacao($_POST['preid']);
	die();

}

if($_REQUEST['requisicaoJson']) {
    $_REQUEST['requisicaoJson']();
}

?>

<link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
<br>
<?php

function aprovarReformulacao() {
	require_once APPRAIZ . 'includes/workflow.php';
	$teste = wf_alterarEstado( $_POST['docid'], 1590, $_POST ['comentario'], Array( 'preid' => $_SESSION['par']['preid'], 'esdid' => $_POST['esdid'] ) );
	echo "<script>window.location.href = 'par.php?modulo=principal/subacaoObras&acao=A&sbaid={$_REQUEST['sbaid']}&ano={$_REQUEST['ano']}&preid={$_SESSION['par']['preid']}&aba=Analise';</script>";
}
function popupAprovacaoReformulacao() {
	global $db;
	
	$sql = "SELECT 
				pre.docid,
				obr.obrid,
				max(hst.hstid) as hstid
			FROM 
				obras.preobra pre
			LEFT  JOIN obras2.obras obr ON obr.obrid = pre.obrid
			LEFT  JOIN workflow.documento doc ON doc.docid = obr.docid
			LEFT  join workflow.historicodocumento hst ON hst.docid = obr.docid
			WHERE 
				pre.preid =  {$_SESSION['par']['preid']}
			GROUP BY
				pre.docid, obr.obrid ";
	
	$obra = $db->pegaLinha ( $sql );
	
	if ($obra ['hstid'] != '') {
		$sql = "SELECT
					esd.esdid,
					esd.esddsc
				FROM 
					workflow.historicodocumento hst
				INNER JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
				INNER JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdidorigem
				WHERE 
					hstid = {$obra['hstid']}";
		
		$estadoObra = $db->pegaLinha ( $sql );
	}
	
	$arrTipoObrasMI = Array (
			42,
			43,
			44,
			45 
	);
	
	$sql = "SELECT ptoid FROM obras.preobra WHERE preid = {$_SESSION['par']['preid']}";
	
	$ptoid = $db->pegaUm ( $sql );
	
	?>
<style>
.texto {
	font-size: 12px;
}

.butaoHorizontalGrande {
	align: center;
	text-align: center;
	vertical-align: middle;
	width: 550px;
	height: 50px;
	background-color: #FFF8DC;
	-moz-border-radius: 5px;
	-webkit-border-radius: 5px;
	-khtml-border-radius: 5px;
	border-radius: 5px;
	display: table-cell;
	cursor: pointer;
}

.butaoHorizontalGrande:hover {
	font-weight: bold;
	background-color: #FFFFF0;
}
</style>
<script>
	function travaTela( trava ){
		if( trava ){
			$('#aguardando').show();
		}else{
			$('#aguardando').hide();
		}
	}
	$('.retorna').click(function(){
		travaTela( true );
		if( $('#comentario').val() == '' ){
			travaTela( false );
			alert('Preencha a justificativa.');
			$('#comentario').focus();
			return false;
		}
		$('#esdid').val($(this).attr('id'));
		$('#formAprova').submit();
	});
	$('.envia').click(function(){
		travaTela( true );
		if( $('#comentario').val() == '' ){
			travaTela( false );
			alert('Preencha a justificativa.');
			$('#comentario').focus();
			return false;
		}
		if( $('#esdid_envia').val() == '' ){
			travaTela( false );
			alert('Selecione uma situação.');
			$('#esdid_envia').focus();
			return false;
		}
		$('#esdid').val($('#esdid_envia').val());
		$('#formAprova').submit();
	});
	</script>
<center>
	<div id="aguardando"
		style="display: none; position: absolute; background-color: white; height: 98%; width: 95%; opacity: 0.4; filter: alpha(opacity =               40)">
		<div style="margin-top: 250px; align: center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>
<form method="POST" id="formAprova" name="formAprova">
	<input type="hidden" name="esdid" 	id="esdid" /> 
	<input type="hidden" name="docid" 	id="docid" 	value="<?=$obra['docid'] ?>" /> 
	<input type="hidden" name="req" 	id="req" 	value="aprovarReformulacao" />
	<div class="texto">
		<br>Justificativa:
	</div>
		<?=campo_textarea('comentario', 'S', 'S', '', 60, 5, 500)?>
		<br>
</form>
<?php
	if ($obra ['obrid'] == '') {
		?>
<div class="texto">
	Esta obra ainda não possui registro no Obras 2 e a aprovação da
	reformulação só terá efeito no sistema atual. <br>
</div>
<br>
<div class="butaoHorizontalGrande retorna">Enviar para obra aprovada.</div>
<br>
<?php
	} elseif (in_array ( $ptoid, $arrTipoObrasMI )) {
		?>
<div class="texto">
	Esta é uma obra do tipo MI.<br> Neste caso ele só pode ser encaminhada
	para 'Aguardando Adesão do Município.'
</div>
<br>
<div class="butaoHorizontalGrande retorna"
	id="<?=OBR_ESDID_AGUARDANDO_ADESAO_DO_MUNICIPIO ?>">Tramitar para
	'Aguardando Adesão do Município.'</div>
<br>
<?php
	} else {
		if ($estadoObra ['esdid'] != '') {
			?>
<div class="texto"> A obra se encontrava no módulo de Obras 2 na situação: '<?=$estadoObra['esddsc'] ?>'</div>
<br>
<div class="butaoHorizontalGrande retorna"
	id="<?=$estadoObra['esdid'] ?>">Retornar para '<?=$estadoObra['esddsc'] ?>'</div>
<br>
<div class="texto">ou</div>
<?php
		} else {
			?>
<div class="texto">
	A obra não possui histórico de tramitação no Obras 2. <br> Escolha o
	novo estado da Obra no sistema Obras 2.
</div>
<br>
<?php
		}
		?>
<div class="texto">Situação:</div>
<?php
		$sql = "SELECT
					esdid as codigo,
					esddsc as descricao
				FROM
					workflow.estadodocumento
				WHERE
					tpdid = " . TPDID_OBJETO;
		?>
	<?=$db->monta_combo( "esdid_envia", $sql, 'S', 'Selecione...', '', '', '', '', 'S', 'esdid_envia','','','Estado Documento', ''); ?><br>
<br>
<div class="butaoHorizontalGrande envia">Tramitar para situação
	escolhida.</div>
<br>
<?php
	}
	?>
<?php
}

if ($_REQUEST ['req']) {
	$_REQUEST ['req'] ();
	die ();
}

include_once APPRAIZ . 'includes/workflow.php';
$preid = ($_SESSION ['par'] ['preid']) ? $_SESSION ['par'] ['preid'] : $_REQUEST ['preid'];

if ($preid) {
	$qrpid = pegaQrpidAnalisePAC ( $preid, 49 );
}
$docid = prePegarDocid ( $preid );
$esdid = prePegarEstadoAtual ( $docid );
if ($_POST ['atualizaBarraNavegacao']) {
	die ( wf_desenhaBarraNavegacao ( $docid, array (
			'preid' => $preid,
			'qrpid' => $qrpid 
	) ) );
}

if ($_REQUEST ['preid']) {
	$sbaid = $db->pegaUm ( "SELECT sbaid FROM par.subacaoobra WHERE preid = " . $_REQUEST ['preid'] );
	if ($sbaid) {
		$_SESSION ['par'] ['obras'] ['sbaid'] = $sbaid;
	}
	
	$muncod = $db->pegaUm ( "SELECT muncod FROM obras.preobra WHERE preid = " . $_REQUEST ['preid'] );
	if ($sbaid) {
		$_SESSION ['par'] ['muncod'] = $muncod;
	}
	
	$_SESSION ['par'] ['esfera'] = retornaEsfera ( $_REQUEST ['preid'] );
	
	if ($_SESSION ['par'] ['esfera'] == 'M') {
		$sql = "SELECT inuid FROM par.instrumentounidade WHERE muncod = '" . $_SESSION ['par'] ['muncod'] . "'";
	} else {
		$sql = "SELECT inuid FROM par.instrumentounidade WHERE estuf in (SELECT estuf FROM territorios.municipio WHERE muncod = '" . $_SESSION ['par'] ['muncod'] . "')";
	}
	$inuid = $db->pegaUm ( $sql );
	$_SESSION ['par'] ['inuid'] = $inuid;
}

$sbaid = $_REQUEST ['sbaid'] ? $_REQUEST ['sbaid'] : $_SESSION ['par'] ['obras'] ['sbaid'];

if( $sbaid != '' && !is_numeric($sbaid) ){
	echo "
		<script>
			alert('Id de subação inválido');
			window.close();
		</script>";
	die();
}

$_SESSION ['par'] ['obras'] ['sbaid'] = $sbaid;

if (! $sbaid) {
	echo "<script>alert('Subação não encontrada!');window.close();</script>";
	die ();
}

$ano = $_REQUEST ['ano'] ? $_REQUEST ['ano'] : $_SESSION ['par'] ['obras'] ['ano'];
$ano = $_REQUEST ['preid'] == '' ? $ano : $db->pegaUm ( "SELECT sobano FROM par.subacaoobra WHERE preid = " . $_REQUEST ['preid'] );

$_SESSION ['par'] ['obras'] ['ano'] = $ano;

$_SESSION ['par'] ['preid'] = $preid = $_REQUEST ['preid'];

include_once (APPRAIZ . "www/par/_funcoes_subacao_obras.php");
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if ($_POST ['requisicao']) {
	$_POST ['requisicao'] ();
}
if ($_POST ['requisicaoAjax']) {
	$_POST ['requisicaoAjax'] ();
	die ();
}

// Carrega informações de localização e dados da subação
$sql = "
		SELECT  s.sbaid,
				s.sbadsc,
				s.ptsid,
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
				s.sbaestrategiaimplementacao,
				s.undid,
				s.prgid,
				s.frmid,
				s.docid,
				pps.ppscronograma
		FROM par.subacao 	 	 s
		INNER JOIN par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus = 'A'
		INNER JOIN par.pontuacao p  ON p.ptoid  = a.ptoid AND p.ptostatus = 'A'
		INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus = 'A' 
		INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
		INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
		INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
		LEFT JOIN par.propostasubacao pps ON pps.ppsid = s.ppsid
		WHERE s.sbastatus = 'A' AND s.sbaid = $sbaid
	";
$subacao = $db->pegaLinha ( $sql );

$perfil = pegaArrayPerfil ( $_SESSION ['usucpf'] );
$estado = prePegarEstadoAtual ( prePegarDocid ( $_REQUEST ['preid'] ) );
/*
 * //regras de acesso passada por Thiago em 24/05/2012 if(	in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil) || in_array(PAR_PERFIL_CONSULTA_ESTADUAL, $perfil) || in_array(PAR_PERFIL_CONSULTA, $perfil) || in_array(PAR_PERFIL_EQUIPE_FINANCEIRA, $perfil) || in_array(PAR_PERFIL_ANALISTA_QUESTOES_PONTUAIS, $perfil) || in_array(PAR_PERFIL_EQUIPE_TECNICA, $perfil) || in_array(PAR_PERFIL_EMPENHADOR, $perfil) || in_array(PAR_PERFIL_MANUTENCAO_TABELAS_APOIO, $perfil) || in_array(PAR_PERFIL_GERADOR_DOCUMENTO, $perfil) || in_array(PAR_PERFIL_PAGADOR, $perfil) || in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) || in_array(PAR_PERFIL_PROFUNC_PREANALISEPF, $perfil) || in_array(PAR_PERFIL_PROFUNC_ANALISEPF, $perfil) || in_array(PAR_PERFIL_ALTA_GESTAO_MEC, $perfil) ){ $boAtivo = 'N'; $habil = 'N'; $readonly = 'readonly="readonly"'; }else{ if($estado){ if(in_array($estado, array(WF_PAR_EM_CADASTRAMENTO, WF_PAR_EM_DILIGENCIA, WF_PAR_OBRA_VAL_INDEF_REFORMULACAO))){ $boAtivo = 'S'; $habil = 'S'; } } else { $boAtivo = 'S'; $habil = 'S'; } }
 */
$readonly = "readonly='readonly'";
$boAtivo = 'N';
$habil = 'N';
$habilitado = 'N';

// Caso o perfil seja super usuario o mesmo tem acesso à tudo independente de estado.
if (in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil )) {
	$boAtivo = 'S';
	$habil = 'S';
	$readonly = '';
	$habilitado = 'S';
	
}

// regras de acesso passada por Thiago em 24/05/2012 - atualizadas em 31/12/2013
if (
	in_array ( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) || 
	in_array ( PAR_PERFIL_ADM_OBRAS, $perfil ) || 
	in_array ( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil ) || 
	in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) || 
	in_array ( PAR_PERFIL_PREFEITO, $perfil )|| 
	in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil )) {
	if ($estado) {
		if (in_array ( $estado, array (
				WF_PAR_EM_CADASTRAMENTO,
				WF_PAR_EM_DILIGENCIA,
				WF_PAR_OBRA_EM_REFORMULACAO,
				WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR,
				WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL_PAR 
		) )) {
			$boAtivo = 'S';
			$habil = 'S';
			$readonly = '';
			$boHabilitado = 'disabled="disabled"';
			$habilitado = 'S';
		}
	}else{
		$boAtivo = 'S';
		$habil = 'S';
		$readonly = '';
		$boHabilitado = 'disabled="disabled"';
		$habilitado = 'S';
	}
}

// regras de acesso passada por Thiago em 31/12/2013
if (in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) || in_array ( PAR_PERFIL_ADM_OBRAS, $perfil )) {
	if ($estado) {
		if (in_array ( $estado, array (
				WF_PAR_VALIDACAO_INDEFERIMENTO,
				WF_PAR_OBRA_EM_VALIDACAO_INDEFERIMENTO_REFORMULACAO,
				WF_PAR_OBRA_EM_VALIDACAO_DEFERIMENTO_REFORMULACAO,
				WF_PAR_OBRA_EM_CADASTRAMENTO_CONDICIONAL,
				WF_PAR_OBRA_EM_APROVACAO_CONDICIONAL 
		) )) {
			
			$boAtivo = 'S';
			$habil = 'S';
			$readonly = '';
			$habilitado = 'S';
		
		}
	}
}

?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
<title>PAR 2010 - Plano de Metas - Subação</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
<script type="text/javascript" src="../includes/funcoes.js"></script>
<!-- 	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script> -->
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript"
	src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css"
	href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css" />
</head>
<body>
	<script type="text/javascript">
			function salvarObras()
			{
				var erro = 0;
				$("[class~=obrigatorio]").each(function() { 
					if(!this.value || this.value == "Selecione..."){
						erro = 1;
						alert('Favor preencher todos os campos obrigatórios!');
						this.focus();
						return false;
					}
				});
				if(erro == 0){
					$("#form_subacao").submit();
				}
			}

			function visualizaComentariosDiligencia(docid) {
				if(docid != '') {
					if(jQuery('#btnVisualizaComentariosDiligencias').hasClass('mais')) {
			    	    jQuery.ajax({
			        	    url : 'par.php?modulo=principal/subacaoObras&acao=A&requisicaoJson=visualizarDetalheDiligencia',
			        	    type: "POST",
			    	        data: { 
			        	        docid : docid
			    	        },
			    	        success : function(retorno) {
			    	            jQuery('#comentariosDiligencia').html(retorno);
			    	            jQuery('#btnVisualizaComentariosDiligencias').addClass('menos');
			    	            jQuery('#btnVisualizaComentariosDiligencias').removeClass('mais');
			    	            jQuery('#btnVisualizaComentariosDiligencias').html('<img src="../imagens/menos.gif" alt="Menos" title="Menos" />');
			    	        }
			    	    });
					} else {
						jQuery('#comentariosDiligencia').html('');
			            jQuery('#btnVisualizaComentariosDiligencias').addClass('mais');
			            jQuery('#btnVisualizaComentariosDiligencias').removeClass('menos');
			            jQuery('#btnVisualizaComentariosDiligencias').html('<img src="../imagens/mais.gif" alt="Mais" title="Mais" />');
					}
				}

			}
		</script>

    <?php
    // Monta tarja vermelha com aviso de pendência de obras
    montarAvisoCabecalho($_SESSION['par']['esfera'], $_SESSION['par']['muncod'], $_SESSION['par']['estuf']);
    ?>

	<table class="tabela" style="margin-bottom: 15px" bgcolor="#f5f5f5"
		cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td colspan="2" style="color: blue; font-size: 22px"><a href="#">
						<?php $entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod']; ?>
						<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']); ?>
					</a></td>
		</tr>
        <?php echo $avisoPendencia; ?>
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
	</table>
		<?php $abaParObras = $_GET['aba'] ? "subacaoObras".$_GET['aba'] : null;?>
		<?php $stPaginaAtual = "par.php?modulo=principal/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano".($preid ? "&preid=$preid" : "").($abaParObras ? "&aba=".$_GET['aba'] : "" )?>
		<?=carregaAbasSubacaoObras($stPaginaAtual, Array('preid' => $preid, 'sbaid' => $sbaid, 'ano' => $ano ) )?>
		<?php include_once(APPRAIZ."par/modulos/principal/".($abaParObras ? $abaParObras : "subacaoObrasDados").".inc" )?>
	</body>
</html>