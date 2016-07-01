<?
$goto_ant = 'sispacto2.php?modulo=principal/mec/mec&acao=A&aba=aprovarnomes';

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

	$identificacaousuario = $db->pegaLinha("SELECT i2.iusnome || ' ( '||CASE WHEN tu.turdesc IS NOT NULL THEN tu.turdesc ELSE tu2.turdesc END||' )' as turma_atual, i.iusd, i.iuscpf, i.iusnome, p.pfldsc, uu.unisigla, uu.uninome, t.pflcod as pflcod_pes, t2.pflcod as pflcod_sup FROM sispacto2.identificacaousuario i 
					 						 INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
											 INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
											 INNER JOIN sispacto2.universidadecadastro un ON un.uncid = i.uncid 
											 INNER JOIN sispacto2.universidade uu ON uu.uniid = un.uniid 
											 LEFT JOIN sispacto2.orientadorturma ot ON ot.iusd = i.iusd 
											 LEFT JOIN sispacto2.orientadorturmaoutros ot2 ON ot2.iusd = i.iusd 
											 LEFT JOIN sispacto2.turmas tu ON tu.turid = ot.turid 
											 LEFT JOIN sispacto2.turmas tu2 ON tu2.turid = ot2.turid
											 LEFT JOIN sispacto2.identificacaousuario i2 ON i2.iusd = tu.iusd or i2.iusd = tu2.iusd
											 LEFT JOIN sispacto2.tipoperfil t2 ON t2.iusd = i2.iusd 
			 		 						 WHERE i.iuscpf='".str_replace(array(".","-"),array("",""),$_REQUEST['iuscpf'])."'");


	if($identificacaousuario['pflcod_sup']) {
		$pfl_turma = $identificacaousuario['pflcod_sup'];
	} elseif($identificacaousuario['pflcod_pes']) {
		if($identificacaousuario['pflcod_pes'] == PFL_ORIENTADORESTUDO) $pfl_turma = PFL_FORMADORIES;
		if($identificacaousuario['pflcod_pes'] == PFL_PROFESSORALFABETIZADOR) $pfl_turma = PFL_ORIENTADORESTUDO;
	}

?>
	<script>
	function selecionarUniversidade(uncid) {
		ajaxatualizar('requisicao=carregarTurmasUniversidade&pflcod=<?=$pfl_turma ?>&uncid='+uncid,'td_turmadestino');
		
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
				FROM sispacto2.universidade u
				INNER JOIN sispacto2.universidadecadastro un ON un.uniid = u.uniid
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
