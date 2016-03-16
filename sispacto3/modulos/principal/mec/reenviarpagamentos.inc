<?
$goto_ant = 'sispacto2.php?modulo=principal/mec/mec&acao=A&aba=aprovarnomes';

$consulta = verificaPermissao();
?>
<script>
function reenviarPagamentos() {

	var conf = confirm('Deseja realmente reenviar pagamentos selecionados?');
	if(conf) {
		var input = document.createElement("input");
		input.setAttribute("type", "hidden");
		input.setAttribute("name", "requisicao");
		input.setAttribute("value", "reenviarPagamentos");
		document.getElementById("formreenviarpagamentos").appendChild(input);
		document.getElementById("formreenviarpagamentos").submit();
	}

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
	<? if(!$consulta) : ?>
	<form method="post" name="formbuscar" id="formbuscar">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
    	<td class="SubTituloDireita">CPF</td>
    	<td><?=campo_texto('iuscpf', "N", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iusnome"', '', $_REQUEST['iuscpf']); ?></td>
    </tr>
    <tr>
	    <td class="SubTituloDireita">Nome</td>
	    <td><?=campo_texto('iusnome', "N", "S", "Nome", 67, 60, "", "", '', '', 0, 'id="iusnome"', '', $_REQUEST['iusnome']); ?></td>
    </tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="buscar" value="Buscar" onclick="document.getElementById('formbuscar').submit();">
		<input type="button" name="limpar" value="Limpar" onclick="window.location=window.location;">
		</td>
	</tr>
	</table>
	</form>
	<? else : ?>
	Perfil sem permissão para acessar esta TELA
	<? endif; ?>
<?

if($_REQUEST['iuscpf']) {
	$wh[] = "i.iuscpf='".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf'])."'";
}

if($_REQUEST['iusnome']) {
	$wh[] = "i.iusnome ilike '".$_REQUEST['iusnome']."'";
}

if($wh) {

	$sql = "SELECT '<input type=\"checkbox\" name=\"doc[]\" value=\"'||p.docid||'\">' as chk, 
					trim(replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')) as iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pbovlrpagamento, u.unisigla||'/'||u.uninome as universidade, 
					'Ref. ' || m.mesdsc || ' / ' || f.fpbanoreferencia as periodo,
					e.esddsc 
			FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.pagamentobolsista p ON p.iusd = i.iusd 
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			INNER JOIN sispacto2.universidade u ON u.uniid = p.uniid 
			INNER JOIN sispacto2.folhapagamento f ON f.fpbid = p.fpbid
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE d.esdid NOT IN(".ESD_PAGAMENTO_EFETIVADO.",".ESD_PAGAMENTO_APTO.",".ESD_PAGAMENTO_AUTORIZADO.") ".(($wh)?"AND ".implode(" AND ",$wh):"")."
			ORDER BY iuscpf, f.fpbid";
	
	$cabecalho = array("&nbsp;","CPF","Nome","E-mail","R$ da bolsa","Universidade","Período","Situação");
	$db->monta_lista($sql,$cabecalho,10000,5,'N','center','N',"formreenviarpagamentos");
	
	echo "<p align=\"center\"><input type=\"button\" value=\"Reenviar pagamento\" name=\"reenviar\"  onclick=\"reenviarPagamentos();\"></p>";

} else {
	echo "<p align=center><b>Por favor inserir filtro de pesquisa (CPF/Nome)</b></p>";
}

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
