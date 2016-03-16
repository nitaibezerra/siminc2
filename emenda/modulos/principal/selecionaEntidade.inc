<?php

if($_REQUEST['montaLista']){
	header('content-type: text/html; charset=ISO-8859-1');
	$entids = buscaEntidadePorCNPJ( $_REQUEST["enbcnpj"], 'popup', $_REQUEST["funid"]);
	
	$html = '
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td class="subtitulocentro">Ação</td>
		<td class="subtitulocentro">Nome da Unidade</td>
	</tr>';

		for( $i = 0; $i < count($entids); $i++ ){
			
			$cor = ($i % 2) ? "#f4f4f4": "#e0e0e0";
			
			$html.= "<tr bgcolor=\"{$cor}\">"
				. "		<td align=\"center\"><img src=\"/imagens/gif_inclui.gif\" style=\"cursor: pointer;\" onclick=\"selecionaEntidade(" . $entids[$i]["entid"] . ");\"></td>"
				. "		<td>" . $entids[$i]["entnome"] . "</td>"
				. "<tr>";
			
			
		}
		$html.= '</table>';
	echo $html;	
	exit;
}

print '<br/>';
monta_titulo( 'Unidades existentes com o CNPJ ' . $_REQUEST["enbcnpj"], 'Selecione abaixo a unidade desejada');

$enbcnpj = str_replace(".", "", $_REQUEST["enbcnpj"]);
$enbcnpj = str_replace("/", "", $enbcnpj);
$enbcnpj = str_replace("-", "", $enbcnpj);

?>

<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
<script type="text/javascript">
	function montaListaEntidade(funid){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/selecionaEntidade&acao=A', {
			        method:     'post',
			        parameters: '&montaLista=true&funid='+funid+'&enbcnpj='+$('enbcnpj').value,
			        asynchronous: false,
			        onComplete: function (res){
						$('listaentidade').innerHTML = res.responseText;
			        }
			  });
	}
</script>
<input type="hidden" value="<?=$enbcnpj ?>" id="enbcnpj" name="enbcnpj">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td class="subtitulocentro">Selecione o tipo de entidade:</td>
		<td><?
			$sql = "SELECT DISTINCT 
						f.funid as codigo, f.fundsc as descricao
					FROM
						entidade.funcaoentidade fe
						inner join entidade.entidade ent on ent.entid = fe.entid
						inner join entidade.funcao f on f.funid = fe.funid
					WHERE 
						ent.entnumcpfcnpj = '".$enbcnpj."'
					ORDER BY descricao";
					$db->monta_combo('funid', $sql, 'S', 'Selecione...', 'montaListaEntidade', '', '', '265','','funid');
		?></td>
	</tr>
	<!-- <tr><td colspan='2' style='background: rgb(252, 253, 219) none repeat scroll 0% 0%; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;'>
	<img src="../imagens/restricao.png"> <b>A entidade que procura não está na lista?</b> <a href="#" onclick="procurarNome($('enbcnpj').value, true); closeMessage();">Clique aqui</a> para consultar a base da Receita Federal.
	</td></tr> -->
	<!--  <tr style="display: none;">
		<td class="subtitulocentro">Ação</td>
		<td class="subtitulocentro">Nome da Unidade</td>
	</tr>-->
	<?php
		/*

		for( $i = 0; $i < count($entids); $i++ ){
			
			$cor = ($i % 2) ? "#f4f4f4": "#e0e0e0";
			
			print "<tr bgcolor=\"{$cor}\" style=\"display: none;\">"
				. "		<td align=\"center\"><img src=\"/imagens/gif_inclui.gif\" style=\"cursor: pointer;\" onclick=\"selecionaEntidade(" . $entids[$i]["entid"] . ");\"></td>"
				. "		<td>" . $entids[$i]["entnome"] . "</td>"
				. "<tr>";
			
			
		}*/
	
	?>
</table>
<div id="listaentidade"></div>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr bgcolor="#D0D0D0">
		<td colspan="2">
			<input type="button" value="Fechar" onclick="closeMessage();" style="cursor: pointer;"/>
		</td>
	</tr>
</table>