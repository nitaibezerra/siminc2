<?php
// monta cabeçalho

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

$titulo = "Título do Parecer";




$parecerGeralSesu 		= "parecer da sesu";
$parecerGeralAdhoc  	= "parecer Adhoc";
$parecerGeralComissao 	= "parecer da Comissao";



$podeVerParecerSesu 		= true;
$podeEditarParecerSesu  	= true;


$podeVerParecerAdhoc 		= true;
$podeEditarParecerAdhoc 	= true;


$podeVerParecerComissao		= true;
$podeEditarParecerComissao	= true;


?>
<!-- biblioteca javascript local -->
<script type="text/javascript">
</script>
<script language="JavaScript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">
//Editor de textos
tinyMCE.init({
	mode : "textareas",
	theme : "advanced",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
	theme_advanced_buttons1 :
	"undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright,justifyfull",
	theme_advanced_buttons2 : "",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	extended_valid_elements :
	"a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
	language : "pt_br",
	entity_encoding : "raw"
});
</script>

<!-- corpo da página -->
<form action="" method="post" name="formulario"
	enctype="multipart/form-data">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td colspan="2" align="center" bgcolor="#cococo">
			<b><?= $titulo ?></b>
		</td>
	</tr>
	
	
<?if($podeVerParecerSesu){?>	
	
	<tr>
		<td class="SubTituloDireita" align="right"><b>Parecer geral Sesu:</b></td>
		<td>
			<?php 
			if($podeEditarParecerSesu)
			echo campo_textarea( 'parecerGeralSesu', 'N', '', '',130, 20,'');
			else
			echo $parecerGeralSesu;
			?>
		</td>
	</tr>
	<?if($podeEditarParecerSesu){?>
	<tr>
		<td class="SubTituloDireita" align="right">&nbsp;</td>
		<td>
			<input class="botao" type="button" value="Gravar" onclick="salvarParecerGeral();">
		</td>
	</tr>
		
	<?}?>
<?}?>	

<?if($podeVerParecerAdhoc){?>	
	
	<tr>
		<td class="SubTituloDireita" align="right"><b>Parecer geral Adhoc:</b></td>
		<td>
			<?php 
			if($podeEditarParecerAdhoc)
			echo campo_textarea( 'parecerGeralAdhoc', 'N', '', '',130, 20,'');
			else
			echo $parecerGeralAdhoc;
			?>
		</td>
	</tr>
	<?if($podeEditarParecerAdhoc){?>
	<tr>
		<td class="SubTituloDireita" align="right">&nbsp;</td>
		<td>
			<input class="botao" type="button" value="Gravar" onclick="salvarParecerGeral();">
		</td>
	</tr>
		
	<?}?>
<?}?>

<?if($podeVerParecerComissao){?>	
	
	<tr>
		<td class="SubTituloDireita" align="right"><b>Parecer geral Comissão:</b></td>
		<td>
			<?php 
			if($podeEditarParecerComissao)
			echo campo_textarea( 'parecerGeralComissao', 'N', '', '',130, 20,'');
			else
			echo $parecerGeralComissao;
			?>
		</td>
	</tr>
	<?if($podeEditarParecerComissao){?>
	<tr>
		<td class="SubTituloDireita" align="right">&nbsp;</td>
		<td>
			<input class="botao" type="button" value="Gravar" onclick="salvarParecerGeral()">
		</td>
	</tr>
		
	<?}?>
<?}?>


	<tr bgcolor="#cococo">
		<td align="right">&nbsp;</td>
		<td align="left">
			<input class="botao" type="button" value="Voltar" onclick="history.back();">
		</td>
	</tr>

</table>

<script>

function salvarParecerGeral(tipo){
	
	switch(tipo)
	{

		case 1:
		//objRadio    = document.formulario.situacao_Sesu;
		objCampoTexto =  tinyMCE.getContent('parecerGeralSesu');
		break

		case 2:
		//objRadio 	  = document.formulario.situacao_Adhoc;
		objCampoTexto =  tinyMCE.getContent('parecerGeralAdhoc');
		break

		case 3:
		//objRadio 	  = document.formulario.situacao_Comissao;
		objCampoTexto =  tinyMCE.getContent('parecerGeralComissao');
		break

		default:
		alert('Problemas na excução do script!');
		return false;
	}
	
	
	/*
	request.open( 'GET', '../reuni/ajax/salvaParecer.php?parecer=' + escape( objCampoTexto ) + "&codPergunta=" + codResposta + "&tipoParecer=" + tipo + "&situacao=" + valorRadio );
	request.onreadystatechange=function()
	{
		if (request.readyState==4)
		{
			var x = request.responseText;
			alert(x);
		}
	}
	request.send( null );
	*/
	
}	


</script>