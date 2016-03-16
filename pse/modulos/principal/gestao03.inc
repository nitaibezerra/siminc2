<?
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include  APPRAIZ. '/includes/Agrupador.php';
include_once( APPRAIZ. "pse/classes/PulPergunta.class.inc" );
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

$obPergunta = new PulPergunta();

$sql = "select rulid from pse.rulresposta where pulid = 3 AND espid = ". $espid;
$rulid = $obPergunta->pegaUm($sql);
$rulid = $rulid ? $rulid : '';

$sql = "SELECT espparticipapse2009 from pse.escolapse
		WHERE entid=$entid";
$pse = $db->pegaUm($sql);			

if($_REQUEST["cadgestao03"])
{
	$resposta =  new RulResposta($rulid ,3, $espid);
	
	$check1 = $_REQUEST['check1'];
	$texto = $_REQUEST['texto'];
	
	if($check1=='')
	{
		echo "1";
		exit;
	}
	
	$sql = "SELECT rulid from pse.rulresposta
			WHERE pulid=3 AND espid=$espid";
	$id = $db->pegaUm($sql);
	
	if($id=='') {
		$sql = "INSERT INTO pse.rulresposta (pulid, espid, paqid, rulflagresposta) VALUES (3, $espid, 1, 's') RETURNING rulid";
		$id = $db->pegaUm($sql);
	}
	
	$sql = "SELECT isuid FROM pse.isuitemselecionado WHERE rulid = $id order by isuid";
	$isuid = $db->carregar($sql);
	if($isuid<>''){
		foreach($isuid as $isuid)
		{
			$sql = "DELETE from pse.tultextoitemselecionado WHERE isuid = $isuid[isuid]";
			$db->executar($sql);
			$db->commit();
		}
		$sql = "DELETE FROM pse.isuitemselecionado WHERE rulid = $id";
		$db->executar($sql);
		$db->commit();
	}
	foreach($check1 as $check1)
	{
		$sql = "INSERT INTO pse.isuitemselecionado (iulid, rulid) VALUES ($check1, $id)";
		$db->executar($sql);
		$db->commit();
	}
	if($texto<>''){
		$sql = "SELECT isuid from pse.isuitemselecionado WHERE iulid=$check1 AND rulid=$id";
		$isuid = $db->pegaUm($sql);
		$sql = "INSERT INTO pse.tultextoitemselecionado (isuid, tultexto) VALUES ($isuid, '$texto')";
		$db->executar($sql);
		$db->commit();
	}
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
	//*****Carrega a pergunta 3
	$obPergunta->carregarPerguntas("'3.   '", $rulid, $espid);	
?>
<?php if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){?>
<tr bgcolor="#cccccc">
	<td style="text-align: center" colspan='2'>
		<input type="button" class="botao" name="btsalvar" value="Salvar" onclick="gravaGestao03()">
<?=navegacao('gestao02','gestao04');?> 
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

