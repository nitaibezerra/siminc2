<?
criaSessaoProfissionais();
$educadores_F = pegarEducadores($_SESSION['projovemurbano']['proid'], 'F');
$educadores_Q = pegarEducadores($_SESSION['projovemurbano']['proid'], 'Q');
$educadores_P = pegarEducadores($_SESSION['projovemurbano']['proid'], 'P');
$educadores_M = pegarEducadores($_SESSION['projovemurbano']['proid'], 'M');
$educadores_T = pegarEducadores($_SESSION['projovemurbano']['proid'], 'T');
$educadores_E = pegarEducadores($_SESSION['projovemurbano']['proid'], 'E');
?>
<form id="form" name="form" method="POST">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Orientações:</td>
		<td>
		<font color=blue>
		<p>Quantidade de profissionais, conforme as informações prestadas na aba de profissionais.</p>
		</font>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Resumo Geral de Educadores</td>
		<td>
		
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">&nbsp;</td>
			<td class="SubTituloCentro">Total</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Educador de Ensino Fundamental</td>
			<td align="right"><?=$educadores_F['eduqtd'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Educador de Qualificação Profissional</td>
			<td align="right"><?=$educadores_Q['eduqtd'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Educador de Participação Cidadã</td>
			<td align="right"><?=$educadores_P['eduqtd'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Educador de Monitoramento de acolhimento às crianças filhas do jovens atendidos pelo programa</td>
			<td align="right"><?=$educadores_M['eduqtd'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tradutor e Interpreté de libras</td>
			<td align="right"><?=$educadores_T['eduqtd'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Educador para atendimento educacional especializado</td>
			<td align="right"><?=$educadores_E['eduqtd'] ?></td>
		</tr>

		<tr>
			<td class="SubTituloDireita"><b>Total</b></td>
			<td align="right"><?=$educadores_E['eduqtd']+$educadores_T['eduqtd']+$educadores_M['eduqtd']+$educadores_P['eduqtd']+$educadores_Q['eduqtd']+$educadores_F['eduqtd'] ?></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<input type="button" name="anterior" value="Anterior" 
					onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?=$_REQUEST['aba'] ?>&aba2=profissionais&aba3=profissionaisResumo';"> 
			<input type="button" name="proximo" value="Próximo" 
					onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?=$_REQUEST['aba'] ?>&aba2=formacaoEducadores';">
			</td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>
