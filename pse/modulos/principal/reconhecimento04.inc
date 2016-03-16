
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


//if($_POST)
if($_REQUEST["reconhecimento04"])
{
	$texto = iconv( 'UTF-8', 'ISO-8859-1', $_REQUEST['tultexto'] );
	
	foreach ($_REQUEST as $chave => $values)
	{
		if(strstr($chave,"per_"))
		{
			$pergunta = explode("_",$chave);	
			
			$sql = "SELECT rulid from pse.rulresposta
					WHERE pulid=$pergunta[1] AND espid=$espid";
			$id = $db->pegaUm($sql);
			
			if($id<>'') {
				$sql = "SELECT isuid FROM pse.isuitemselecionado WHERE rulid = $id order by isuid";
				$isuid = $db->pegaUm($sql);
				if($isuid<>''){
					$sql = "DELETE from pse.tultextoitemselecionado WHERE isuid = $isuid";
					$db->executar($sql);
					$db->commit();
				}
				$sql = "DELETE FROM pse.isuitemselecionado WHERE rulid=$id";
				$db->executar($sql);
				$sql = "DELETE FROM pse.rulresposta WHERE rulid=$id";
				$db->executar($sql);
				$db->commit();
			} 	
			$sql = "INSERT INTO pse.rulresposta (pulid, espid, paqid, rulflagresposta) VALUES ($pergunta[1], $espid, 1, '$values')";
			$db->executar($sql);
			if($_REQUEST["per_28"]=='s'){
				$sql = "SELECT iulid from pse.iulitempergunta where pulid = 28";
				$iulid = $db->pegaUm($sql);
				$sql = "SELECT rulid from pse.rulresposta WHERE pulid=28 AND espid=$espid";
				$id = $db->pegaUm($sql);
				$sql = "INSERT INTO pse.isuitemselecionado (iulid, rulid) VALUES ($iulid, $id)";
				$db->executar($sql);
				$sql = "SELECT isuid from pse.isuitemselecionado WHERE iulid=$iulid AND rulid=$id";
				$isuid = $db->pegaUm($sql);
				$sql = "INSERT INTO pse.tultextoitemselecionado (isuid, tultexto) VALUES ($isuid, '$texto')";
				$db->executar($sql);
			} else {
				echo "1";
			}
		}
	}
	$db->commit();
	exit;
}


// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
$titulo = "PSE - Programa Saúde na Escola";
$titulo2 = "Unidade Local Integrada - 5 - Reconhecimento do Território";
echo "<br>";
monta_titulo( $titulo, '' );
monta_titulo( $titulo2, '' );

?>
<form method="POST" name="formunicipio" id="formunicipio">
<?php Cabecalho($entid,1); ?> 
<table style="border-bottom: 0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><input type='hidden' name='rulid' id='rulid' value='<?=$perguntaPage;?>'></td>
	</tr>
	<?php 
		$sql = "select rulid from pse.rulresposta where pulid = 28 AND espid = ". $espid;
		$rulid = $obPergunta->pegaUm($sql);
		$rulid = $rulid ? $rulid : '';
		$obPergunta->metodoPergunta = "validaItem(0,'item28[]')";
		$obPergunta->carregarPerguntas("'5.4. '", $rulid, $espid);	
	?>

	<?php if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){?>
	<tr style="background-color: #DCDCDC">
		<td colspan="2" align="center">
			<input type="button" class="botao" name="btsalvar" value="Salvar" onclick="gravaReconhecimento04()">
		</td>
	</tr>
	<?php }?>
	<?=navegacao('reconhecimento03','reconhecimento05');?>
	</table>
</form>	

<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	<?php $sql = "SELECT rulflagresposta FROM pse.rulresposta WHERE pulid=28 AND espid=$espid";
	$id = $db->pegaUm($sql);
	if ($id == 's'){
		$resp = 1;
	} else {
		$resp = 0;
	}?>
	validaItem('<?=$resp?>','item28[]');
</script>