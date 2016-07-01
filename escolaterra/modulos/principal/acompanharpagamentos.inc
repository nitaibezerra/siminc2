<script>
function wf_exibirHistorico( docid )
{
	var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid=' + docid;
	window.open(
		url,
		'alterarEstado',
		'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
	);
}

function abrirDetalhes(id) {
	if(document.getElementById('img_'+id).title=='mais') {
		document.getElementById('tr_'+id).style.display='';
		document.getElementById('img_'+id).title='menos';
		document.getElementById('img_'+id).src='../imagens/menos.gif'
	} else {
		document.getElementById('tr_'+id).style.display='none';
		document.getElementById('img_'+id).title='mais';
		document.getElementById('img_'+id).src='../imagens/mais.gif'

	}

}

</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="10" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Acompanhamento de bolsas</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td>[ORIENTAÇÕES]</td>
</tr>
<tr>
	<td colspan="2">
	<?
	
	$sql = "SELECT i.iusnome ||' ( '|| replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ||' )', p.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao, pe.pfldsc, pp.pbovlrpagamento, e.esddsc FROM escolaterra.periodoreferenciauf p 
			INNER JOIN escolaterra.periodoreferencia r ON r.fpbid = p.fpbid 
			INNER JOIN public.meses m ON m.mescod::integer = r.fpbmesreferencia
			INNER JOIN escolaterra.identificacaousuario i ON i.ufpid = p.ufpid 
			INNER JOIN escolaterra.ufparticipantes u ON u.ufpid = i.ufpid 
			LEFT JOIN escolaterra.pagamentobolsista pp ON i.iusid = pp.iusid AND p.fpbid = pp.fpbid  
			LEFT JOIN workflow.documento d ON d.docid = pp.docid 
			LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
			LEFT JOIN seguranca.perfil pe ON pe.pflcod = pp.pflcod
			WHERE i.iusid='".$_SESSION['escolaterra'][$sis]['iusid']."' 
			ORDER BY p.fpbid";
	
	$cabecalho = array("Bolsista","Período","Perfil","R$","Situação");
	$db->monta_lista_simples($sql, $cabecalho, 50, 5, 'S', '100%');

	?>
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="1" align="center">
	<tr>
		<td class="SubTituloCentro" style="font-size:xx-small;">Status de pagamento</td>
		<td class="SubTituloCentro" style="font-size:xx-small;">Tempo médio</td>
		<td class="SubTituloCentro" style="font-size:xx-small;">Descrição</td>
	</tr>
	<tr>
		<td style="font-size:xx-small;">Aguardando autorização</td>
		<td style="font-size:xx-small;">aprox. 2 dias</td>
		<td style="font-size:xx-small;">O bolsista foi avaliado e considerado apto a receber a bolsa. A liberação do pagamento está aguardando autorização final do MEC.</td>
	</tr>
	<tr>
		<td style="font-size:xx-small;">Autorizado</td><td style="font-size:xx-small;">aprox. 3 dias</td>
		<td style="font-size:xx-small;">O pagamento da bolsa foi autorizado, e esta trâmite com o FNDE.</td>
	</tr>
	<tr>
		<td style="font-size:xx-small;">Aguardando autorização SGB</td>
		<td style="font-size:xx-small;">aprox. 3 dias</td>
		<td style="font-size:xx-small;">O pagamento da bolsa encontra-se no Sistema de Gestão de Bolsas (SGB), aguardando homologação.</td>
	</tr>
	<tr>
		<td style="font-size:xx-small;">Aguardando pagamento</td>
		<td style="font-size:xx-small;">aprox. 9 dias</td>
		<td style="font-size:xx-small;">O pagamento da bolsa foi homologado no SGB e está em processamento.</td>
	</tr>
	<tr>
		<td style="font-size:xx-small;">Enviado ao Banco</td>
		<td style="font-size:xx-small;">aprox. 7 dias</td>
		<td style="font-size:xx-small;">A ordem bancária referente ao pagamento da bolsa foi emitida.</td>
	</tr>
	<tr>
		<td style="font-size:xx-small;">Pagamento efetivado</td>
		<td style="font-size:xx-small;">-</td>
		<td style="font-size:xx-small;">O pagamento foi creditado em conta e confirmado pelo banco.</td>
	</tr>
	<tr>
		<td style="font-size:xx-small;">Pagamento não autorizado FNDE</td>
		<td style="font-size:xx-small;">-</td>
		<td style="font-size:xx-small;">O pagamento da bolsa não foi autorizado pelo FNDE, pois o bolsista recebe bolsa de outro programa do MEC.</td>
	</tr>
	<tr>
		<td style="font-size:xx-small;">Pagamento recusado</td>
		<td style="font-size:xx-small;">aprox. 2 dias</td>
		<td style="font-size:xx-small;">Pagamento recusado em função de algum erro de registro. Será reencaminhado ao responsável pela autorização.</td>
	</tr>
	<tr><td colspan="3" style="font-size:xx-small;"><p><b>Observação: Caso o seu status no fluxo de pagamento esteja em BRANCO, significa que a avaliação ainda não foi concluída naquele mês de referência. Neste caso, você deve procurar a coordenação do curso.</b></p></td></tr>
	</table>
	
	</td>
</tr>

</table>