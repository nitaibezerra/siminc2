<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Principal</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>
<tr>
	<td colspan="2">
	<? if($execucao) : ?>
[PRINCIPAL EXECUCAO]
	<? else : ?>
	
	<?
	$goto_pro 		= "escolaterra.php?modulo=principal/coordenadorestadual/coordenadorestadual&acao=A&aba=dados";
	
	$preenchimento  = $db->pegaUm("SELECT iustermocompromisso FROM escolaterra.identificacaousuario WHERE iusid='".$_SESSION['escolaterra']['tutor']['iusid']."'");
	
	$esdid = $db->pegaUm("SELECT d.esdid FROM escolaterra.turmas t 
								  INNER JOIN workflow.documento d ON d.docid = t.docid  
								  WHERE iusid='".$_SESSION['escolaterra']['tutor']['iusid']."'");
	if($esdid != ESD_EM_CADASTRAMENTO && $esdid) {
		$cadastramento_tut = true;
	}
	
	?>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloEsquerda" width="80%">1. Preenchimento dos dados cadastrais</td>
		<td><?=(($preenchimento=='t')?"<span style=\"color:blue;font-size:15px;\"><b>CONCLUÍDO":"<span style=\"color:red;font-size:15px;\"><b>PENDENTE") ?></b></span></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" width="80%">2. Cadastramento de tutores</td>
		<td><?=(($cadastramento_tut)?"<span style=\"color:blue;font-size:15px;\"><b>CONCLUÍDO":"<span style=\"color:red;font-size:15px;\"><b>PENDENTE") ?></b></span></td>
	</tr>
	</table>
	<? endif; ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td><input type="button" value="Próximo" onclick="divCarregando();window.location='<?=$goto_pro ?>';"></td>
</tr>
</table>