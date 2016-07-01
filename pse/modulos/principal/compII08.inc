<?
	include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
		
	include APPRAIZ . 'includes/cabecalho.inc';
	 
	$entid = $_SESSION['pse']["entid"];
	$espid = $_SESSION['pse']["espid"];
	if( $entid == '' || empty($entid)){
		print "<script>"
			. "    alert('Escolha uma entidade!');"
			. "    history.back(-1);"
			. "</script>";
		
		die;
	}
	if( $espid == '' || empty($espid)){
			print "<script>"
				. "    alert('Você não pode acessar essa área até fazer o cadastro em Identificação!');"
				. "    history.back(-1);"
				. "</script>";
			
			die;
    }	
	$perguntaPage = "22,23";
	$ano = 1;
	
	$titulo = "PSE - Programa Saúde na Escola";
	$titulo2 = "Unidade Local Integrada - 3 - Ações de Prevenção e Promoção da Saúde - Componente II";
	echo "<br>";
	monta_titulo( $titulo, '' );
	monta_titulo( $titulo2, '' );
	
	if($_POST){
		if(!empty($_REQUEST['rulflagresposta_22'])){
				$sql = "SELECT rulid FROM pse.rulresposta
						WHERE pulid = 22 and espid = $espid";
				$rulid = $db->pegaUm($sql);
	
				if($rulid==''){
					$sql = "INSERT INTO pse.rulresposta
							(pulid,espid,paqid,rulflagresposta)
							VALUES
							(22,$espid,$ano,'$_REQUEST[rulflagresposta_22]')
							RETURNING rulid";
					$rulid = $db->pegaUm($sql);
				}
				else {
						$sql = "UPDATE pse.rulresposta SET
								rulflagresposta = '$_REQUEST[rulflagresposta_22]'
								WHERE
								pulid = 22 AND
								espid = $espid AND
								paqid = $ano";
						$db->executar($sql);
				}
						
			$sql = "SELECT rulid FROM pse.rulresposta
					WHERE pulid = 23 and espid = $espid";
			$rulid = $db->pegaUm($sql);
	
			if($rulid==''){
				$sql = "INSERT INTO pse.rulresposta
						(pulid,espid,paqid,rulflagresposta)
						VALUES
						(23,$espid,$ano,'s')
						RETURNING rulid";
				$rulid = $db->pegaUm($sql);
			}
				
			$valores = $_REQUEST['perg_23'];
			
			$sql = "DELETE FROM pse.isuitemselecionado
					WHERE rulid = $rulid";
			$db->executar($sql);
			$db->commit();
			
			if(!empty($valores) && $_REQUEST['rulflagresposta_22']=='s'){
				foreach($valores as $valor){
					$sql = "INSERT INTO pse.isuitemselecionado
							(iulid,rulid)
							VALUES
							($valor,$rulid)";
					$db->executar($sql);
					$db->commit();
				}
			}

		}
		else
			echo "<script>alert('A pergunta 3,4 é obrigatória!');</script>";
	}

//Verifica se existe resposta já grava no BD	
$sql = "SELECT rulflagresposta from pse.rulresposta
		WHERE pulid = 22 and espid = $espid";
$radios = $db->pegaUm($sql);	
$radios = ($radios==''?'':$radios);
if($radios!="")
	$radios != "s" ? $checkNao = "checked" : $checkSim = "checked";
?>

<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<form method="POST"  name="formulario">
<?php Cabecalho($entid,1); ?> 
<table style="border-bottom: 0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><input type='hidden' name='rulid' id='rulid' value='<?=$perguntaPage;?>'></td>
	</tr>
	<?php 
		$idpergunta = explode(",",$perguntaPage);
		foreach($idpergunta as $pergunta){
			perguntasULI($pergunta,$espid); 
		}
	?>
	<?php if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){?>
		<tr style="background-color: #DCDCDC">
			<td colspan="2" align="center"><input type="submit" value="Salvar"></td>
		</tr>
	<?php }?>
	<?=navegacao('compII07','compIII01');?>
	</table>
</form>	

<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
function  check(opcao){
	if(opcao=='s'){
		$('itens23').show();
	}
	else
		$('itens23').hide();
}
check('<?= $radios; ?>');
</script>