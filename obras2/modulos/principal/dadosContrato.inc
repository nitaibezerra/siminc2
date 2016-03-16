<?php

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<form method="post" name="formulario" id="formulario" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tbody>
		<tr>
			<td colspan="2" class="SubTituloCentro" style="background-color: #DCDCDC">Contratação da Obra</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Empresa Contratada:</td>
			<td>
				<span id="entnomeempresa">TECNOTEL AMBIENTAL REFORMAS E SERVICOS GERAIS LTDA</span>
				<input type="hidden" name="entidempresa" id="entidempresa" value="680578">
				<input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;" onclick="inserirEmpresa(document.getElementById('entidempresa').value);">
				<?=obrigatorio(); ?>			  
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data de Assinatura do Contrato:</td>
			<td><?=campo_data2('crtdtassinatura', 'S', 'S','Data de Assinatura do Contrato', '', '', '', $crtdtassinatura, '', '', 'crtdtassinatura'); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Prazo de Vigência do Contrato (dias):</td>
			<td><?=campo_texto('crtprazovigencia', 'S', 'S', '', 5, 10, '[#]', '', '', '', 0, 'id="crtprazovigencia"','', $crtprazovigencia, '' ); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data de término do contrato:</td>
			<td><?=campo_data2('crtdttermino', 'N', 'N','Data de término do contrato', '', '', '', $crtdttermino, '', '', 'crtdttermino'); ?></td>
		</tr>
		<tr>
			<td colspan="2" class="SubTituloCentro" style="background-color: #DCDCDC">Sobre a Obra</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Cópia da Obra Original</td>
			<td><? $db->monta_lista_simples($sql, $cabecalho, 50000, 5, 'S'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Histórico de Paralisações</td>
			<td><? $db->monta_lista_simples($sql, $cabecalho, 50000, 5, 'S'); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data da Ordem de Serviço:</td>
			<td><?=campo_data2('crtdtordemservico', 'S', 'S','Data da Ordem de Serviço', '', '', '', $crtdtordemservico, '', '', 'crtdtordemservico'); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Início de Execução da Obra:</td>
			<td><?=campo_data2('crtdtinicioexecucao', 'S', 'S','Início de Execução da Obra', '', '', '', $crtdtinicioexecucao, '', '', 'crtdtinicioexecucao'); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Prazo de Execução (dias):</td>
			<td><?=campo_texto('crtprazoexecucao', 'S', 'S', '', 5, 10, '[#]', '', '', '', 0, 'id="crtprazoexecucao"','', $crtprazoexecucao, '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Término de Execução da Obra:</td>
			<td><?=campo_data2('crtdtterminoexecucao', 'N', 'N','Término de Execução da Obra', '', '', '', $crtdtterminoexecucao, '', '', 'crtdtterminoexecucao'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor Contratado da Obra (R$):</td>
			<td><?=campo_texto('crtvalorexecucao', 'S', 'S', '', 20, 18, '[###.]###,##', '', '', '', 0, 'id="crtvalorexecucao"','', $crtvalorexecucao, '' ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Área/Quantidade a ser Construída:</td>
			<td><?=campo_texto('crtcustounitario', 'S', 'S', '', 20, 18, '[###.]###,##', '', '', '', 0, 'id="crtcustounitario"','', $crtcustounitario, '' ); 
				echo 'Unidade de Medida:' . $db->monta_combo("umdid", array(), 'S', '', '', '', '', '100', 'N');?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Custo Unitário R$:</td>
			<td><?=campo_texto('crtcustounitario', 'N', 'N', '', 20, 18, '[###.]###,##', '', '', '', 0, 'id="crtcustounitario"','', $crtcustounitario, '' ); ?> (R$ / Unidade de Medida)</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Percentual BDI:</td>
			<td><?=campo_texto('crtpercentualdbi', 'S', 'S', '', 20, 18, '[###.]###,##', '', '', '', 0, 'id="crtpercentualdbi"','', $crtpercentualdbi, '' ); ?> (Administração, taxas, emolumentos, impostos e lucro.)</td>
		</tr>
		<tr>
			<td colspan="2" class="SubTituloCentro" style="background-color: #DCDCDC">Supervisões - Empresa</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Histórico de Supervisões</td>
			<td><? $db->monta_lista_simples($sql, $cabecalho, 50000, 5, 'S'); ?></td>
		</tr>
		<tr>
			<td colspan="2" class="SubTituloCentro" style="background-color: #DCDCDC">Aditivos</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Histórico de Aditivos</td>
			<td><? $db->monta_lista_simples($sql, $cabecalho, 50000, 5, 'S'); ?>
			<a href="#" style="text-align: left; color: #0F55A9;" onclick="">Incluir Aditivo</a></td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<input type="button" id="salvar" value="Salvar" onclick="validaRec(&#39;11&#39;)" style="cursor: pointer">
				<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
			</td>
		</tr>
	</tbody>
	</table>
</form>