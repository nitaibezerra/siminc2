<?
	include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
	
	// monta cabeçalho
	include APPRAIZ . 'includes/cabecalho.inc';

	$entid = $_SESSION['pse']["entid"];
	$espid = $_SESSION['pse']["espid"];
	if( $entid == '' || empty($entid)){
		print "<script>"
			. "    alert('Faltam dados para esta tela!');"
			. "    history.back(-1);"
			. "</script>";
		
		die;
	}
	$perguntaPage = "27";
	$ano = 1;
	
	$titulo = "PSE - Programa Saúde na Escola";
	$titulo2 = "Unidade Local Integrada - 5 - Reconhecimento do Território";
	echo "<br>";
	monta_titulo( $titulo, '' );
	monta_titulo( $titulo2, '' );

	if($_POST){	
		gravaPerguntas($post,$espid,$ano);
	}
	
?>

<form method="POST" name="formulario" id="formulario">
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
	<?=navegacao('reconhecimento02','reconhecimento04');?>
	</table>
</form>	

<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	

</script>