<?
$goto_ant = 'sispacto.php?modulo=principal/mec/mec&acao=A&aba=aprovarnomes';

$consulta = verificaPermissao();
?>
<script>
function trocarUniversidade() {

	var conf = confirm('Isso irá transferir todoas possíveis perfis ligados a este usuário para a universidade destino. Deseja realmente trocar a universidade?');
	if(conf) {
		var input = document.createElement("input");
		input.setAttribute("type", "hidden");
		input.setAttribute("name", "requisicao");
		input.setAttribute("value", "trocarUniversidade");
		document.getElementById("formtrocar").appendChild(input);
		document.getElementById("formtrocar").submit();
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

	$identificacaousuario = $db->pegaLinha("SELECT i2.iusnome || ' ( '||tu.turdesc||' )' as turma_atual, i.iusd, i.iuscpf, i.iusnome, p.pfldsc, uu.unisigla, uu.uninome, t2.pflcod as pflcod_sup FROM sispacto.identificacaousuario i 
					 						 INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
											 INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
											 INNER JOIN sispacto.universidadecadastro un ON un.uncid = i.uncid 
											 INNER JOIN sispacto.universidade uu ON uu.uniid = un.uniid 
											 LEFT JOIN sispacto.orientadorturma ot ON ot.iusd = i.iusd 
											 LEFT JOIN sispacto.turmas tu ON tu.turid = ot.turid 
											 LEFT JOIN sispacto.identificacaousuario i2 ON i2.iusd = tu.iusd 
											 LEFT JOIN sispacto.tipoperfil t2 ON t2.iusd = i2.iusd 
			 		 						 WHERE i.iuscpf='".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf'])."'");


?>
	<script>
	function selecionarUniversidade(uncid) {
		ajaxatualizar('requisicao=carregarTurmasUniversidade&pflcod=<?=$identificacaousuario['pflcod_sup'] ?>&uncid='+uncid,'td_turmadestino');
		
	}
	</script>
	<form method="post" name="formtrocar" id="formtrocar">
	<input type="hidden" name="iusd" id="iusd" value="<?=$identificacaousuario['iusd'] ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
    	<td class="SubTituloDireita">CPF</td>
    	<td><?=mascaraglobal($identificacaousuario['iuscpf'],"###.###.###-##") ?></td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">Nome</td>
    	<td><?=$identificacaousuario['iusnome'] ?></td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">Perfil</td>
    	<td><?=$identificacaousuario['pfldsc'] ?></td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">Universidade atual</td>
    	<td><?=$identificacaousuario['unisigla']." - ".$identificacaousuario['uninome'] ?></td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">Turma atual</td>
    	<td><?=$identificacaousuario['turma_atual'] ?></td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">Universidade destino</td>
    	<td><?
    	
    	$sql = "SELECT un.uncid as codigo, u.unisigla||' - '||u.uninome as descricao
				FROM sispacto.universidade u
				INNER JOIN sispacto.universidadecadastro un ON un.uniid = u.uniid
				ORDER BY uninome";
    	
    	$db->monta_combo('uncid', $sql, 'S', 'Selecione', 'selecionarUniversidade', '', '', '', 'S', 'uniid', '', $_REQUEST['uncid']);
    	 
    	
    	?></td>
    </tr>
    <tr>
    	<td class="SubTituloDireita">Turma destino</td>
    	<td id="td_turmadestino">Selecione a Universidade Destino</td>
    </tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="trocar" value="Trocar de Universidade" onclick="trocarUniversidade();">
		</td>
	</tr>
	</table>
	</form>
<? 

} else {
	echo "<p align=center><b>Por favor inserir CPF</b></p>";
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
