<?php

switch ( $_REQUEST["requisicao"] ){
	case 'salvaranalise':
		if( salvaAnaliseDados( $_REQUEST ) ){
			echo "<script>alert('Operação realizada com sucesso!');window.opener.location.reload();window.close();</script>";
			exit();
		} else {
			echo "<script>alert('Falha na operação!');</script>";
		}
	break;
}

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );


if ( $_REQUEST["anaid"] ){
	
	$sql = "SELECT 
				ana.anaid,			
				ana.anasituacaoparecer, 
				ana.anaparecer,
				anx.anxdsc,
				arq.arqid,
				arq.arqnome || '.' || arq.arqextensao as arquivo
			FROM 
				emenda.analise ana
			LEFT JOIN
				emenda.anexo anx ON anx.anaid = ana.anaid
			LEFT JOIN
				public.arquivo arq on anx.arqid = arq.arqid
			WHERE 
				ana.anaid = {$_REQUEST["anaid"]}";
	$dadosAnalise = $db->pegaLinha( $sql );

	$sql = "SELECT uniid FROM emenda.analise WHERE anaidpai = {$_REQUEST["anaid"]}";

	$dadosUnidadeAnalise = $db->carregarColuna( $sql );

}

$ptrid = $_SESSION["emenda"]["ptridAnalise"];
$anaid = $_REQUEST["anaid"];
?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<script language="javascript" type="text/javascript" src="../includes/TextEditor.js"></script>
<script language="javascript" type="text/javascript">
     //TextEditor.init();
</script>

<script type="text/javascript">
	// Salva a análise
	// Salva a análise
	function salvarAnaliseDados(){
		var form = document.getElementById('formulario');
		var input = "";
		var marcadoInput = false;
	
		for(i = 0; i < form.length; i++){
			if( form.elements[i].type == "checkbox" ){
				if( form.elements[i].checked == true ){
					input = form.elements[i].value;
					marcadoInput = true;
				}
			}
		}
		
		var msg = '';
		if( !marcadoInput ){
			msg += 'Selecione uma unidade.\n';
		} 

		if(msg){
			alert(msg);
		} else{
			form.submit();
		}
		
	}
	
	tinyMCE.init({
		// General options
		mode : "textareas",
		theme : "advanced",
		language: "pt",
		plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

		// Theme options
		theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
		theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
		theme_advanced_buttons3 : "tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
		theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		theme_advanced_statusbar_location : "",
		theme_advanced_resizing : true,

		// Example content CSS (should be your site CSS)
		content_css : "css/content.css",

		// Drop lists for link/image/media/template dialogs
		template_external_list_url : "lists/template_list.js",
		external_link_list_url : "lists/link_list.js",
		external_image_list_url : "lists/image_list.js",
		media_external_list_url : "lists/media_list.js",

		// Replace values for the template plugin
		template_replace_values : {
			username : "Some User",
			staffid : "991234"
		}
	});
</script>
<form action="" method="post" name="formulario" id="formulario" enctype="multipart/form-data">
	<input type="hidden" name="requisicao" id="requisicao" value="salvaranalise">
	<input type="hidden" name="ptrid" id="ptrid" value="<?=$ptrid ?>">
	<input type="hidden" name="anaid" id="anaid" value="<?=$anaid ?>">
	<input type="hidden" name="concluir" id="concluir" value="false">
		<table id="tblform" class="tabela" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
			<tr>
				<td colspan="2" class="subtitulocentro">Vinculação das Unidades Gestoras</td>
			</tr>
			<!--
			<tr>
				<td class="subtitulodireita" width="25%"><b>Análise</b></td>
				<td>
					<label for="anasituacao1"><input type="radio" name="anasituacao" id="anasituacao1" value="F" <?php if($dadosAnalise["anasituacaoparecer"] == "F"){ echo "checked"; } ?>>Favorável</label>
					<label for="anasituacao2"><input type="radio" name="anasituacao" id="anasituacao2" value="E" <?php if($dadosAnalise["anasituacaoparecer"] == "E"){ echo "checked"; } ?>>Em diligência</label>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Parecer</b></td>
			 <td>
			 	<?php $anaparecer = $dadosAnalise["anaparecer"]; ?>
			 	<?=campo_textarea('anaparecer', 'S', 'S', 'Observação', 75, 20, 4000, '', '', '', '', 'Observação');?>
			 </td>
		</tr>
		-->
		
		
		<tr>
			<td class="subtitulodireita"><b>Unidades que realizarão análises de mérito:</b></td>
			 <td>
			 	<?php
				 	if( !$boReformulacao )
						$filtro = 'and uniid != 11';
						
			 		$sql = "SELECT 
							  uniid as codigo,
							  unisigla as descricao
							FROM 
							  emenda.unidades
							WHERE
							  unistatus = 'A' $filtro";
				 	
				 	$arUnidade = $db->carregar($sql);
				 	
				 	echo '<table>';
				 	
				 	$i = 0;
				 	$y = 0;			 	
				 	
				 	foreach ($arUnidade as $chave => $valor) {
				 		
				 		foreach( $dadosUnidadeAnalise as $k=>$v ){
					 		if($valor["codigo"] == $v){
					 			$checked = 'checked';
					 			break;
					 		}else{
					 			$checked = '';
					 		}
				 		}

				 		if($i == 0){
				 			echo '<tr>';
				 		}
				 		
				 		if($i < 3){
				 			$i++;
				 			echo '<td><input type="checkbox" name="uniid[]" id="uniid[]" value="'.$valor['codigo'].'" '.$checked.'>'.$valor['descricao'].'<td>';
				 		}
				 					 		
				 		if($i == 3){
				 			if($y == 0){
				 				//echo '<td rowspan="3"><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/></td>';
				 				$y = 1;
				 			}			 			
				 			echo '</tr>';
				 			$i = 0;
				 		}
				 	}
				 	
				 	echo '</table>';
			 	?>
			 </td>
		</tr>
		
		
		<!--
		<tr>
			<td class="subtitulodireita"><b>Anexo</b></td>
			<td>
				<?php if($dadosAnalise["arqid"]){?>
					<div id="div_anexo" >
						<a href="javascript:window.location='?modulo=principal/manterAnexos&acao=A&download=S&arqid=<?=$dadosAnalise["arqid"]?>'" ><?=$dadosAnalise["arquivo"]?></a>
						<img style="cursor: pointer;vertical-align: middle" onclick="excluirAnexoAnaid('<?=$dadosAnalise["anaid"]?>','<?=$dadosAnalise["arqid"]?>');" title="Excluir Anexo" src="/imagens/excluir.gif"/>
					</div>
				<?php }else{?>
					<input type="file" name="arquivo" id="arquivo" />
				<?php }?> 
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Descrição do Anexo</b></td>
			<td><?php $anxdsc = $dadosAnalise["anxdsc"] ?>
				<?=campo_texto( 'anxdsc', 'N', $habilitado, 'Descrição Anexo', 40, 40, '', '','','','','id="anxdsc"'); ?>
			</td>
		</tr>
		-->
		
		
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td>
				<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" style="cursor: pointer;" <?php if ($habilitaAnalise){ echo 'disabled="disabled"'; }else { echo 'onclick="salvarAnaliseDados();"'; } ?> >
				<input type="button" value="Fechar" name="btnFechar" id="btnFechar" style="cursor: pointer;" onclick="window.close();">
			</td>
		</tr>
	</table>
</form>