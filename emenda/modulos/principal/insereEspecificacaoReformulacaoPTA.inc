<?php

//include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');
$obPTA = new PTA();
$obRefor = new reformulacaoPTA( $db );

if( $_POST['submeter'] == 'salvar' ){
	$obPTA->manterEspecificacao( $_POST, true );
}

if($_REQUEST['pesquisa']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obRefor->listaPlanoEspecificacao($_POST['ptiid'], $_POST['pteid'], $_SESSION["emenda"]["ptridReformulacao"]);
	exit;
}
if($_REQUEST['montaComboUnidade']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->montaComboUnidade($_REQUEST['iceid']);
	exit;
}
if($_REQUEST['insereEspecificacaoPendente']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->insereEspecificacaoPendente($_POST);
	exit;
}
if($_REQUEST['excluir']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->excluirPlanoEspecificacao($_POST['pteid']);
	exit;
}
if($_REQUEST['alterar']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->alterarPlanoEspecificacao($_POST['pteid']);
	exit;
}
if($_REQUEST['alterarQuadroEmenda']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obRefor->iniciativaEspecificacaoRecursoAjax($_REQUEST['ptiid'], $_REQUEST['pteid'], $_SESSION["emenda"]["ptridReformulacao"]);
	//$obPTA->iniciativaEspecificacaoRecursoAjax($_REQUEST['ptiid'], $_REQUEST['pteid'], $_SESSION["emenda"]["ptridReformulacao"]);
	exit;
}
if($_REQUEST['alteraPTARecurso']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->carregaPTARecurso($_POST['pteid']);
	exit;
}
if($_REQUEST['excluirKit']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->excluirKitEspecif($_POST['pteid']);
	exit;
}
if($_REQUEST['cancelar']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->verificaExistenciaKit($_POST['pteid']);
	exit;
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if( !empty($_REQUEST['ptrid']) ){
	$_SESSION["emenda"]["ptridReformulacao"] = $_REQUEST['ptrid'];
	$_SESSION["emenda"]["ptiidReformulacao"] = $_REQUEST['ptiid'];
	$_SESSION['emenda']["iniidReformulacao"] = $_REQUEST['iniid'];
}

$ptrid = $_SESSION["emenda"]["ptridReformulacao"];
$ptiid = $_SESSION["emenda"]["ptiidReformulacao"];

#carrega os dados da inciativa cadastrado
$acao = $obPTA->carregaCabecalhoIniciativaEspecificacao( $ptiid );

$iniid = $acao['iniid'];
$ininome = $acao['ininome'];
$ptiid = ($ptiid ? $ptiid : $acao['ptiid'] );

$ptequantidade = 0;

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Detalhamento da Iniciativa', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');
echo "<br />";
?>
<html>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="js/especificacaoIniciativaReformulacao.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<style type="text/css">	
	div.dcontexto{
		 position:relative; 
		 padding:0;
		 color:#039;
		 text-decoration: none;
		 cursor: pointer; 
		 margin-right: -1px;
		 z-index:24;
	}
	div.dcontexto:hover{
		background:transparent;
		text-decoration: none;
		z-index:25; 
	}
	div.dcontexto span{
		display: none;
		text-decoration: none;
	}
	div.dcontexto:hover span{
		display: block;
		position:absolute;
		width: auto;
		font-size: 10px;
		top:2em;
		text-align:center;
		/*left:50px;*/
		padding:4px 0px;
		border:1px solid #000;
		background:#eee; 
		color:#000;
		text-decoration: none;
	}

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    _position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
.borda{
	border: 1px solid;
	border-color: red;
	color: red;
}
.borda1{
	border: 1px solid;
	border-color: red;
	color: black;
}
</style>
<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >

<input type="hidden" name="hidConcedente" id="hidConcedente" value="0">
<input type="hidden" name="totalEmendaInformada" id="totalEmendaInformada" value="0.00">
<input type="hidden" name="hidValorTotal" id="hidValorTotal" value="">
<input type="hidden" name="hidDataIni" id="hidDataIni" value="">
<input type="hidden" name="hidDataFim" id="hidDataFim" value="">
<input type="hidden" name="ptiid" id="ptiid" value="<?=($ptiid ? $ptiid : ''); ?>">
<input type="hidden" name="pteid" id="pteid" value="<?=($pteid ? $pteid : ''); ?>">
<input type="hidden" name="aceid1" id="aceid1" value="">
<input type="hidden" name="iniid" id="iniid" value="<?=($iniid ? $iniid : '');?>">
<input type="hidden" name="perid" id="perid" value="<?=($perid ? $perid : '');?>">
<input type="hidden" name="ptrid" id="ptrid" value="<?=$ptrid;?>">
<input type="hidden" name="kitmodificado" id="kitmodificado" value="false">
<input type="hidden" name="valorRecurso" id="valorRecurso" value="">
<input type="hidden" name="submeter" id="submeter" value="">
<input type="hidden" name="espkit1" id="espkit1" value="">

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
	<tr>
		<td align="center" bgcolor="#dedfde"><b>Iniciativa</b></td>
	</tr>
	<tr>
		<td align="center"><?=$ininome; ?></td>
	</tr>
	<tr>
		<td align="center" bgcolor="#dedfde"><b>Especificação da Iniciativa</b></td>
	</tr>
	<tr>
		<td align="center">
			<br />
			<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios
			<div id="lista"><?=$obRefor->listaPlanoEspecificacao($ptiid, $pteid, $ptrid ); ?></div>
			<br />
		</td>
	</tr>
</table>

</form>
<div id="erro"></div>
<div id="lista"></div>
</body>
</html>