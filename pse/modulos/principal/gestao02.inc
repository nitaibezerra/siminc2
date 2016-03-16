<?php
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include  APPRAIZ. '/includes/Agrupador.php';
include_once( APPRAIZ. "pse/classes/PulPergunta.class.inc" );
include_once( APPRAIZ. "pse/classes/TulTextoItemSelecionado.class.inc" );
VerSessao();
$entid = $_SESSION['pse']['entid'];
$espid = $_SESSION['pse']['espid'];
if( $entid == '' || empty($entid)){
		print "<script>"
			. "    alert('Faltam dados para esta tela!');"
			. "    history.back(-1);"
			. "</script>";
		
		die;
	}

$objTulTextoItemSelecionado	= new TulTextoItemSelecionado('',$_SESSION['pse']["muncod"]);

$obPergunta = new PulPergunta();

$sql = "select rulid from pse.rulresposta where pulid = 2 AND espid = ". $espid;
$rulid = $obPergunta->pegaUm($sql);
$rulid = $rulid ? $rulid : '';

$sql = "SELECT espparticipapse2009 from pse.escolapse
		WHERE entid=$entid";
$pse = $db->pegaUm($sql);			

if($_REQUEST["carregaParcerias"])
{
	$objTulTextoItemSelecionado->carregaListaParcerias($_REQUEST['parcerias'], $rulid);	
	exit;
}

if($_REQUEST["idItem"] && $_REQUEST["excluiFormularioAjax"])
{
	$sql = "SELECT rulid FROM pse.isuitemselecionado WHERE isuid = {$_REQUEST['isuid']}";
	$rul = $db->pegaUm($sql);
	$sql = "SELECT COUNT(isuid) FROM pse.tultextoitemselecionado WHERE isuid = {$_REQUEST['isuid']}";
	$qnt = $db->pegaUm($sql);
	$sql = "DELETE from pse.tultextoitemselecionado WHERE tulid = {$_REQUEST['idItem']}";
	$db->executar($sql);
	$db->commit();
	if($qnt > 1){
		exit;
	} else {
		$sql = "DELETE from pse.isuitemselecionado WHERE isuid = {$_REQUEST['isuid']}";
		$db->executar($sql);
		$db->commit();
	}

/*
	if($objTulTextoItemSelecionado->excluir($_REQUEST["idItem"]))
		$objTulTextoItemSelecionado->commit();
	else
		$objTulTextoItemSelecionado->rollback();	
	exit;*/
}

if($_REQUEST["excluiTudo"])
{
	if($rulid == ''){
		$sql = "INSERT INTO pse.rulresposta (pulid, espid, paqid, rulflagresposta) VALUES (2, $espid, 1, 's') RETURNING rulid";
		$rulid = $db->pegaUm($sql);
		$db->commit();
	}
	$sql = "SELECT isuid FROM pse.isuitemselecionado WHERE rulid = $rulid order by isuid";
	$isuid = $db->carregar($sql);
	if($isuid){
		foreach($isuid as $isuid)
		{
			$sql = "DELETE from pse.tultextoitemselecionado WHERE isuid = $isuid[isuid]";
			$db->executar($sql);
			$db->commit();
		}
		$sql = "DELETE from pse.isuitemselecionado WHERE rulid = $rulid";
		$db->executar($sql);
		$db->commit();	
	}
	$sql = "UPDATE pse.rulresposta set	rulflagresposta = 'n' WHERE rulid = $rulid";
	$db->executar($sql);
	$db->commit();
}

if($_REQUEST["cadgestao02"])
{
	$resposta =  new RulResposta($rulid ,2, $espid);
	
	$parcerias = $_REQUEST['parcerias'];
	
	$texto = iconv( 'UTF-8', 'ISO-8859-1', $_REQUEST['texto'] );
	
	$sql = "SELECT rulid from pse.rulresposta
			WHERE pulid=2 AND espid=$espid";
	$id = $db->pegaUm($sql);
	
	if($id=='') {
		$sql = "INSERT INTO pse.rulresposta (pulid, espid, paqid, rulflagresposta) VALUES (2, $espid, 1, 's')";
		$db->executar($sql);
		$db->commit();
	} else {
		$sql = "UPDATE pse.rulresposta set	rulflagresposta = 's' WHERE rulid = $id";
		$db->executar($sql);
		$db->commit();
	}
	
	$sql = "SELECT rulid from pse.rulresposta WHERE pulid=2 AND espid=$espid";
	$id = $db->pegaUm($sql);
	$sql = "SELECT isuid from pse.isuitemselecionado WHERE iulid=$parcerias AND rulid=$id";
	$isuid = $db->pegaUm($sql);
	if ($isuid == ''){
		$sql = "INSERT INTO pse.isuitemselecionado (iulid, rulid) VALUES ($parcerias, $id) RETURNING isuid";
		$isuid = $db->pegaUm($sql);
	}
	$sql = "INSERT INTO pse.tultextoitemselecionado (isuid, tultexto) VALUES ($isuid, '$texto')";
	$db->executar($sql);
	$db->commit();

	exit;
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
$titulo = "PSE - Programa Saúde na Escola";
monta_titulo( $titulo , '');
$titulo = "Unidade Local Integrada - 1 - Gestão";
monta_titulo( $titulo, '' );

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formunicipio" id="formunicipio">
<input type="hidden" id="rulid" name="rulid" value="<?=$rulid?>">
<?php Cabecalho($entid,1); ?>
<table style="border-bottom: 0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?php
	//*****Carrega a pergunta 2
	$obPergunta->carregarPerguntas("'2.   '", $rulid, $espid);	
?>
<?php if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){?>
<tr bgcolor="#cccccc">
	<td style="text-align: center" colspan='2'>
		<input type="button" class="botao" name="btsalvar" value="Incluir" onclick="gravaGestao02()">
<?=navegacao('gestao01','gestao03');?>
	</td>
</tr>	
<?PHP }?>
</table>
</form>
<tr>
	<td colspan="2">
		<div id="lista" style="display: ''"></div>
	</td>
</tr>
<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

function verifica() {
	if (document.formunicipio.checkparcerias.checked){
		document.formunicipio.parcerias.value='';
		document.formunicipio.texto.value='';
		document.formunicipio.parcerias.disabled = true;
		document.formunicipio.texto.disabled = true;
		document.formunicipio.btsalvar.disabled = true;
	}
}
verifica();
</script>

