<?
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

$sql = "SELECT espparticipapse2009 from pse.escolapse
		WHERE entid=$entid";
$pse = $db->pegaUm($sql);			


if($_REQUEST["cadgestao04"])
{
	$resposta =  new RulResposta($rulid ,4, $espid);
	
	$radio1 = $_REQUEST['radio1'];
	
	if($radio1=='')
	{
		echo "1";
		exit;
	}
	
	$sql = "SELECT rulid from pse.rulresposta
			WHERE pulid=4 AND espid=$espid";
	$id = $db->pegaUm($sql);
	
	if($id=='') {
		$sql = "INSERT INTO pse.rulresposta (pulid, espid, paqid, rulflagresposta) VALUES (4, $espid, 1, 's')";
		$db->executar($sql);
		$db->commit();
	}
	$sql = "SELECT rulid from pse.rulresposta
			WHERE pulid=4 AND espid=$espid";
	$id = $db->pegaUm($sql);
	
	$sql = "SELECT isuid FROM pse.isuitemselecionado WHERE rulid = $id order by isuid";
	$isuid = $db->carregar($sql);
	if($isuid<>''){
		$sql = "DELETE FROM pse.isuitemselecionado WHERE rulid = $id";
		$db->executar($sql);
		$db->commit();
	}
	foreach($radio1 as $radio1)
	{
		$sql = "INSERT INTO pse.isuitemselecionado (iulid, rulid) VALUES ($radio1, $id)";
		$db->executar($sql);
		$db->commit();
	}
	//pergunta 05
//	$resposta =  new RulResposta($rulid ,5, $espid);
//	
//	$radio2 = $_REQUEST['radio2'];
//	
//	$sql = "SELECT rulid from pse.rulresposta
//			WHERE pulid=5 AND espid=$espid";
//	$id = $db->pegaUm($sql);
//	
//	if($id=='') {
//		$sql = "INSERT INTO pse.rulresposta (pulid, espid, paqid, rulflagresposta) VALUES (5, $espid, 1, 's')";
//		$db->executar($sql);
//		$db->commit();
//	}
//	
//	$sql = "SELECT rulid from pse.rulresposta
//			WHERE pulid=5 AND espid=$espid";
//	$id = $db->pegaUm($sql);
//	
//	$sql = "SELECT isuid FROM pse.isuitemselecionado WHERE rulid = $id order by isuid";
//	$isuid = $db->carregar($sql);
//	if($isuid<>''){
//		$sql = "DELETE FROM pse.isuitemselecionado WHERE rulid = $id";
//		$db->executar($sql);
//		$db->commit();
//	}
//	foreach($radio2 as $radio2)
//	{
//		$sql = "INSERT INTO pse.isuitemselecionado (iulid, rulid) VALUES ($radio2, $id)";
//		$db->executar($sql);
//		$db->commit();
//	}
	
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
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formunicipio" id="formunicipio">
<input type="hidden" id="rulid" name="rulid" value="<?=$rulid?>">
<?php Cabecalho($entid,1); ?>
<table style="border-bottom: 0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?php
	//*****Carrega a pergunta 4
	$sql = "select rulid from pse.rulresposta where pulid = 4 AND espid = ". $espid;
	$rulid = $obPergunta->pegaUm($sql);
	$rulid = $rulid ? $rulid : '';
	$obPergunta->carregarPerguntas("'4.   '", $rulid, $espid);	
	//*****Carrega a pergunta 5
//	$sql = "select rulid from pse.rulresposta where pulid = 5 AND espid = ". $espid;
//	$rulid = $obPergunta->pegaUm($sql);
//	$rulid = $rulid ? $rulid : '';
//	$obPergunta->carregarPerguntas("'5.   '", $rulid, $espid);	
?>
<?php if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){?>
<tr bgcolor="#cccccc">
	<td style="text-align: center" colspan='2'>
		<input type="button" class="botao" name="btsalvar" value="Salvar" onclick="gravaGestao04()">
<?=navegacao('gestao03','gestao05');?> 
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
</script>