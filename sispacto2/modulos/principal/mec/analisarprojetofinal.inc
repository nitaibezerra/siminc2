<?
$goto_ant = 'sispacto2.php?modulo=principal/mec/mec&acao=A&aba=trocaruniversidade';

$consulta = verificaPermissao();
?>
<script>
function analisarRelatorioFinal(iusd,uncid) {
	window.open('sispacto2.php?modulo=principal/mec/mec&acao=A&requisicao=analisarRelatorioFinal&iusd='+iusd+'&uncid='+uncid,'imagem','scrollbars=yes,width=1280,height=1024,resizable=yes'); 
}


</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloDireita">Orientações</td>
	<td colspan="2">
	[ORIENTACOES]
	</td>
</tr>
<tr>
	<td colspan="3">

<?
$sql = "SELECT '<img src=../imagens/consultar.gif style=cursor:pointer; onclick=\"analisarRelatorioFinal('||i.iusd||','||i.uncid||')\">' as acao, 
			   i.iusnome, 
			   u.unisigla||' - '||u.uninome as universidade, 
			   e.esddsc
		FROM sispacto2.avaliacaofinalcg a 
		INNER JOIN sispacto2.identificacaousuario i ON i.iusd = a.iusd 
		INNER JOIN sispacto2.universidadecadastro uu ON uu.uncid = i.uncid
		INNER JOIN workflow.documento d ON d.docid = a.docid 
		INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
		INNER JOIN sispacto2.universidade u ON u.uniid = uu.uniid";

$cabecalho = array("&nbsp;","Coordenador IES","IES","Situação");
$db->monta_lista($sql,$cabecalho,10000,5,'N','center','N');

?>
	</td>
</tr>


<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_ant ?>';">
	</td>
</tr>


</table>
