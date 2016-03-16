<?
	include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
	include APPRAIZ . 'includes/Agrupador.php';
	
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
	$perguntaPage = "20";
	$ano = 1;
	
	$titulo = "PSE - Programa Saúde na Escola";
	$titulo2 = "Unidade Local Integrada - 3 - Ações de Prevenção e Promoção da Saúde - Componente II";
	echo "<br>";
	monta_titulo( $titulo, '' );
	monta_titulo( $titulo2, '' );

	if($_POST){	
		gravaPerguntas($post,$espid,$ano);
	}
	
	
?>

<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/agrupador.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>

<form method="POST"  name="formulario" onSubmit="selectAllOptions( document.getElementById('destino') )">
<?php Cabecalho($entid,1); ?> 
<table style="border-bottom: 0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><input type='hidden' name='rulid' id='rulid' value='<?=$perguntaPage;?>'></td>
	</tr>
	<?php 
		$idpergunta = explode(",",$perguntaPage);
		foreach($idpergunta as $pergunta){
			perguntasULI($pergunta,$espid,''); 
		}
	?>

	<?php if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){?>
	<tr style="background-color: #DCDCDC">
		<td colspan="2" align="center"><input type="submit" value="Salvar"></td>
	</tr>
	<?php }?>
	<?=navegacao('compII01','compII07');?>
	</table>
</form>	

<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	

</script>