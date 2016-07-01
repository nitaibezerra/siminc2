<?php
// carrega as funções gerais
header('content-type: text/html; charset=ISO-8859-1');

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	print '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">'.'Lista de Arquivos Habilita'.'</label></td></tr><tr>';
	print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';
?>
<html>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script>
	function abrepopupListaArquivo(arquivo){
		if( $('diretorio').value != ""){
			window.open('emenda.php?modulo=principal/popuplistaArquivosHabilita&acao=A&arquivo='+arquivo+'&diretorio='+$('diretorio').value, 'arquivo', 'scrollbars=no,height=580,width=800,status=no,toolbar=no,menubar=yes,location=no');
		} else {
			alert('Informe o diretório de pesquisa!');
		}
	}
	
	function mostraArquivos(){
		var url = "ajax_arquivo.php";
		var pars = 'abrirArquivoHabilita=lista&diretorio='+$('diretorio').value;
		if( $('diretorio').value != ""){
			var myAjax = new Ajax.Request(
					url, 
					{
						method: 'post', 
						parameters: pars, 
						onComplete: mostraRespostaHabilita
					});	
		} else {
			alert('Informe o diretório de pesquisa!');
		}
	}
	function mostraRespostaHabilita(resp){
		$('lista').innerHTML = resp.responseText;
	}
</script>

<body>
<table id="tb_tabela" class="tabela" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:19.5%;">Diretorio:</td>
		<td><input class="normal" type="text" title="" style="text-align: left;" onblur="MouseBlur(this);" 
			onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="../../arquivos/emenda/habilita" 
			size="80" name="diretorio" id="diretorio"/></td>
	</tr>
	<tr>
		<th colspan="2"><input type="button" value="Buscar" id="botao" onclick="mostraArquivos();"></th>
	</tr>
</table>
<br>
<div id="lista"></div>
</body>
</html>