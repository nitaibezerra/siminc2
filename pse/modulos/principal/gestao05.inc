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

$sql = "SELECT espparticipapse2009 from pse.escolapse
		WHERE entid=$entid";
$pse = $db->pegaUm($sql);			

//if($_REQUEST["cadgestao05"])
if($_POST)
{
	$atores = $_REQUEST["atores"];
	$check2 = $_REQUEST['check2'];
	
	if($_REQUEST["per_6"] <> '' && $_REQUEST["per_8"] <> ''){
		if($_REQUEST["per_6"] == 's' && $atores <> '' || $_REQUEST["per_6"] == 'n'){
			if($_REQUEST["per_8"] == 's' && $check2 <> '' || $_REQUEST["per_8"] == 'n'){
				foreach ($_REQUEST as $chave => $values)
				{
					if(strstr($chave,"per_"))
					{
						$pergunta = explode("_",$chave);	
						
						$sql = "SELECT rulid from pse.rulresposta
								WHERE pulid=$pergunta[1] AND espid=$espid";
						$id = $db->pegaUm($sql);
						
						if ($_REQUEST["per_8"]=='s' && $check2==''){
							echo "1";
							exit;
						}
									
						if($id<>'') {
							$sql = "DELETE FROM pse.isuitemselecionado WHERE rulid=$id";
							$db->executar($sql);
							$sql = "DELETE FROM pse.rulresposta WHERE rulid=$id";
							$db->executar($sql);
							$db->commit();
						} 	
						$sql = "INSERT INTO pse.rulresposta (pulid, espid, paqid, rulflagresposta) VALUES ($pergunta[1], $espid, 1, '$values')";
						$db->executar($sql);
						if($pergunta[1]==6 && $_REQUEST["per_6"]=='s'){
							foreach($atores as $ator)
							{
								if($ator){
									$sql = "SELECT rulid from pse.rulresposta WHERE pulid=6 AND espid=$espid";
									$id = $db->pegaUm($sql);
									$sql = "INSERT INTO pse.isuitemselecionado (iulid, rulid) VALUES ($ator, $id)";
									$db->executar($sql);
								}
							}
						}
						if($pergunta[1]==8 && $_REQUEST["per_8"]=='s'){
							foreach($check2 as $check)
							{
								$sql = "SELECT rulid from pse.rulresposta WHERE pulid=8 AND espid=$espid";
								$id = $db->pegaUm($sql);
								$sql = "INSERT INTO pse.isuitemselecionado (iulid, rulid) VALUES ($check, $id)";
								$db->executar($sql);
													
							}
						}
					}
				}
				$db->commit();
				echo "<script>alert('Operação realizada com sucesso!');
				window.location.href='/pse/pse.php?modulo=principal/gestao05&acao=A';</script>";
				//exit;
			} else {
				echo "<script>alert('A Questão 7.1 é obrigatória!');
				window.location.href='/pse/pse.php?modulo=principal/gestao05&acao=A';</script>";
			}
		} else {
			echo "<script>alert('A Questão 6.1 é obrigatória!');
				window.location.href='/pse/pse.php?modulo=principal/gestao05&acao=A';</script>";
		}
	} else {
		echo "<script>alert('As Questões 6. e 7. são obrigatórias!');
				window.location.href='/pse/pse.php?modulo=principal/gestao05&acao=A';</script>";
	}
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
<form method="POST"  name="formunicipio" id="formunicipio" onSubmit="selectAllOptions( document.getElementById('atores'))">
<input type="hidden" id="rulid" name="rulid" value="<?=$rulid?>">
<?php Cabecalho($entid,1); ?>
<table style="border-bottom: 0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?php
	//*****Carrega a pergunta 6
	$sql = "select rulid from pse.rulresposta where pulid = 6 AND espid = ". $espid;
	$rulid = $obPergunta->pegaUm($sql);
	$rulid = $rulid ? $rulid : '';
	$obPergunta->metodoPergunta = "validaItem(0,'item6[]')";
	$obPergunta->carregarPerguntas("'6.   '", $rulid, $espid);	
	//*****Carrega a pergunta 7
	$sql = "select rulid from pse.rulresposta where pulid = 8 AND espid = ". $espid;
	$rulid = $obPergunta->pegaUm($sql);
	$rulid = $rulid ? $rulid : '';
	$obPergunta->metodoPergunta = "validaItem(0,'item7[]')";
	$obPergunta->carregarPerguntas("'7.   '", $rulid, $espid);	
?>
<?php if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){?>
<tr bgcolor="#cccccc">
	<td style="text-align: center" colspan='2'>
		<input type="submit" class="botao" name="btsalvar" value="Salvar">
<?=navegacao('gestao04','compInecessidadeCarencia01');?> 
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
<?php
//*****Valida a pergunta 06 assim que a página é carregada!
$sql = "SELECT rulflagresposta FROM pse.rulresposta WHERE pulid=6 AND espid=$espid";
$id = $db->pegaUm($sql);
if ($id == 's'){ $resp = 1; } else {$resp = 0;}?>
	validaItem('<?=$resp?>','item6[]');
<?php
//*****Valida a pergunta 08 assim que a página é carregada!
$sql = "SELECT rulflagresposta FROM pse.rulresposta WHERE pulid=8 AND espid=$espid";
$id = $db->pegaUm($sql);
if ($id == 's'){ $resp = 1; } else {$resp = 0;}?>
	validaItem('<?=$resp?>','item7[]');
</script>