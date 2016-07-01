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

$sql = "select rulid from pse.rulresposta where pulid = 1 AND espid = ". $espid;
$rulid = $obPergunta->pegaUm($sql);
$rulid = $rulid ? $rulid : '';

$sql = "SELECT espparticipapse2009 from pse.escolapse
		WHERE entid=$entid";
$pse = $db->pegaUm($sql);			


if($_REQUEST["cadgestao01"])
{
	$atores 	= explode(",",$_REQUEST["atores"]);
	
	//$resposta =  new RulResposta($rulid ,1, $espid);
	
	$sql = "SELECT rulid from pse.rulresposta
			WHERE pulid=1 AND espid=$espid";
	$id = $db->pegaUm($sql);
	
	if($rulid<>'') {
		$sql = "DELETE FROM pse.isuitemselecionado WHERE rulid=$id";
		$db->executar($sql);
		$sql = "DELETE FROM pse.rulresposta WHERE rulid=$id";
		$db->executar($sql);
		$db->commit();
	} 	
	$sql = "INSERT INTO pse.rulresposta (pulid, espid, paqid, rulflagresposta) VALUES (1, $espid, 1, 's')";
	$db->executar($sql);
	$db->commit();
	foreach($atores as $ator)
	{
		if($ator)
		{
			$sql = "SELECT rulid from pse.rulresposta WHERE pulid=1 AND espid=$espid";
			$id = $db->pegaUm($sql);
			$sql = "INSERT INTO pse.isuitemselecionado (iulid, rulid) VALUES ($ator, $id)";
			$db->executar($sql);
			$db->commit();
		} 
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
<?php Cabecalho($entid,1); ?>
<table style="border-bottom: 0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?php
	//*****Carrega a pergunta 1
	$obPergunta->carregarPerguntas("'1.   '", $rulid, $espid);	
?>
<?php if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){?>
<tr bgcolor="#cccccc">
	<td style="text-align: center" colspan='2'>
		<input type="button" class="botao" name="btsalvar" value="Salvar" onclick="gravaGestao01()">
	</td>
</tr>	
<?PHP }?>
<?=navegacao('cadastroEstadoMunicipioArvore','gestao02');?> 
</table>
</form>

<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript"></script>