<?
$numeropolos = $db->pegaUm("SELECT count(*) FROM projovemurbano.polo pol 
							INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
							WHERE plm.pmustatus='A' AND pol.polstatus='A' AND pjuid='".$_SESSION['projovemurbano']['pjuid']."'");

if($numeropolos) {
	$numeronucleos = $db->pegaUm("SELECT count(*) FROM projovemurbano.nucleo nuc
								  INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
								  INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid    
								  INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
								  INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
								  WHERE nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A' AND pol.polstatus='A' AND pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
} else {
	$numeronucleos = $db->pegaUm("SELECT count(*) FROM projovemurbano.nucleo nuc 
								  INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
								  INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
								  WHERE nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A' AND pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
}

if($numeropolos) {
	$numeroestudantes = $db->pegaUm("SELECT COALESCE(SUM(nuc.nucqtdestudantes),0) FROM projovemurbano.nucleo nuc
								  INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
								  INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid    
								  INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
								  INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
								  WHERE nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A' AND pol.polstatus='A' AND pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
} else {
	$numeroestudantes = $db->pegaUm("SELECT COALESCE(SUM(nuc.nucqtdestudantes),0) FROM projovemurbano.nucleo nuc 
								  INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
								  INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
								  WHERE nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A' AND pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
}
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Orientações:</td>
		<td>
		<font color=blue>
		<p>Quantidade de polos (quando houver), núcleos e turmas, conforme as informações prestadas nas abas anteriores.</p>
		</font>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Número de Polos</td>
		<td><b><?=$numeropolos ?></b></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Número de Nucleos</td>
		<td><b><?=$numeronucleos ?></b></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Número de Turmas</td>
		<td><b><?=($numeronucleos*5) ?></b></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Número de Estudantes</td>
		<td><b><?=$numeroestudantes ?></b></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" name="anterior" value="Anterior" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo&aba2=poloNucleoCadastro';"> <input type="button" name="proximo" value="Próximo" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais';"></td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>
