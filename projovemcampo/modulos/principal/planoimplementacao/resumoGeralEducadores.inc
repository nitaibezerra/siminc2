<?
$efetivorecprog = 3;
$efeticocomp = 2;
$eftivo30 = 1;
$efetivorecprop = 4;

$educadores_EF_eft= pegarprofissionais('4',$eftivo30);
$educadores_EF_recprop= pegarprofissionais('4',$efetivorecprog);
$educadores_EF_rec = pegarprofissionais('4',$efetivorecprop);
$educadores_EF_ccm = pegarprofissionais('4',$efeticocomp);

$educadores_EQ_eft= pegarprofissionais('5',$eftivo30);
$educadores_EQ_recprop= pegarprofissionais('5',$efetivorecprog);
$educadores_EQ_rec = pegarprofissionais('5',$efetivorecprop);
$educadores_EQ_ccm = pegarprofissionais('5',$efeticocomp);

$educadores_EP_eft= pegarprofissionais('6',$eftivo30);
$educadores_EP_recprop= pegarprofissionais('6',$efetivorecprog);
$educadores_EP_rec = pegarprofissionais('6',$efetivorecprop);
$educadores_EP_ccm = pegarprofissionais('6',$efeticocomp);

$educadores_ET_eft= pegarprofissionais('7',$eftivo30);
$educadores_ET_recprop= pegarprofissionais('7',$efetivorecprog);
$educadores_ET_rec = pegarprofissionais('7',$efetivorecprop);

$educadores_EE_eft= pegarprofissionais('8',$eftivo30);
$educadores_EE_recprop= pegarprofissionais('8',$efetivorecprog);
$educadores_E = $educadores_EE_eft['qtdcontratado'] + $educadores_EE_recprop['qtdcontratado'];

$total = $educadores_EE_eft['qtdcontratado']+$educadores_EE_recprop['qtdcontratado']+ $educadores_ET_eft['qtdcontratado']+$educadores_ET_recprop['qtdcontratado']+$educadores_ET_rec['qtdcontratado']+
				$educadores_EP_eft['qtdcontratado']+$educadores_EP_recprop['qtdcontratado']+$educadores_EP_rec['qtdcontratado']+$educadores_EP_ccm['qtdcontratado'] +
				$educadores_EQ_eft['qtdcontratado']+$educadores_EQ_recprop['qtdcontratado']+$educadores_EQ_rec['qtdcontratado']+$educadores_EQ_ccm['qtdcontratado'] +
				$educadores_EF_eft['qtdcontratado']+$educadores_EF_recprop['qtdcontratado']+$educadores_EF_rec['qtdcontratado']+$educadores_EF_ccm['qtdcontratado'];
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
			<td class="SubTituloDireita">Educadores/Professores de educação básica</td>
			<td align="right"><?=$educadores_EF_eft['qtdcontratado']+$educadores_EF_recprop['qtdcontratado']+$educadores_EF_rec['qtdcontratado']+$educadores_EF_ccm['qtdcontratado'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Educador de Qualificação Profissional</td>
			<td align="right"><?=$educadores_EQ_eft['qtdcontratado']+$educadores_EQ_recprop['qtdcontratado']+$educadores_EQ_rec['qtdcontratado']+$educadores_EQ_ccm['qtdcontratado']  ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Educador de Monitoramento de acolhimento às crianças filhas do jovens atendidos pelo programa</td>
			<td align="right"><?=$educadores_EP_eft['qtdcontratado']+$educadores_EP_recprop['qtdcontratado']+$educadores_EP_rec['qtdcontratado']+$educadores_EP_ccm['qtdcontratado']  ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tradutor e Interpreté de libras</td>
			<td align="right"><?=$educadores_ET_eft['qtdcontratado']+$educadores_ET_recprop['qtdcontratado']+$educadores_ET_rec['qtdcontratado']  ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Educador para atendimento educacional especializado</td>
			<td align="right"><?=$educadores_EE_eft['qtdcontratado']+$educadores_EE_recprop['qtdcontratado'] ?></td>
		</tr> 
		<tr>
			<td class="SubTituloDireita"><b>Total</b></td>
			<td align="right"><?=$total?></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<input type="button" name="anterior" value="Anterior" 
					onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=profissionaisResumo';"> 
			<input type="button" name="proximo" value="Próximo" 
					onclick="window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores';">
			</td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>
