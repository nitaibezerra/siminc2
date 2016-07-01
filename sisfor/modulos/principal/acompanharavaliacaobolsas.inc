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
	
	$sql = "SELECT 
	
				foo.bolsista, 
				foo.pfldsc, 
				foo.uninome,
				foo.curdesc, 
				foo.pbovlrpagamento, 
				foo.periodo, 
				foo.situacao
			
			FROM (
		
			(
	
			SELECT replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||i.iusnome as bolsista, 
				   pp.pfldsc, 
				   u.uniabrev ||' - '|| unidsc as uninome,
				   case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
					     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
					     when s.ocuid is not null then oc.ocunome 
					     when s.oatid is not null then oatnome end as curdesc, 
					round(p.pbovlrpagamento,2) as pbovlrpagamento, 
					'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia as periodo, 
					esd.esddsc || '<img src=\"../imagens/fluxodoc.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"wf_exibirHistorico( '||p.docid||' )\">' as situacao,
					(f.fpbanoreferencia||'-'||lpad(f.fpbmesreferencia::text,4,'0')||'-01')::date as referencia
			FROM sisfor.pagamentobolsista p 
			INNER JOIN sisfor.identificacaousuario i ON i.iusd = p.iusd
			INNER JOIN sisfor.folhapagamento f ON f.fpbid = p.fpbid  
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			INNER JOIN sisfor.tipoperfil t ON t.tpeid=p.tpeid 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = p.pflcod  
			INNER JOIN sisfor.sisfor s ON s.sifid=t.sifid 
			INNER JOIN public.unidade u on u.unicod = s.unicod 
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
		    left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid 
            left join catalogocurso2014.curso cur on cur.curid = ieo.curid AND cur.curstatus = 'A'
            left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
            left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid 
            left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid AND cur2.curstatus = 'A' 
            left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid 
            left join seguranca.usuario usu on usu.usucpf = s.usucpf 
            left join sisfor.outrocurso oc on oc.ocuid = s.ocuid 
            left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
			left join sisfor.outraatividade oat on oat.oatid = s.oatid 
			left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
			WHERE p.iusd='{$iusd_aco}'
			
			) UNION ALL (
			
			SELECT replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||i.iusnome as bolsista, 
				   pp.pfldsc, 
				   u.uniabrev ||' - '|| unidsc as uninome,
				   '-' as curdesc, 
				   round(p.pbovlrpagamento,2) as pbovlrpagamento, 
				   'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia as periodo, 
				   esd.esddsc || '<img src=\"../imagens/fluxodoc.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"wf_exibirHistorico( '||p.docid||' )\">' as situacao,
				   (f.fpbanoreferencia||'-'||lpad(f.fpbmesreferencia::text,4,'0')||'-01')::date as referencia 
			FROM sisfor.pagamentobolsista p 
			INNER JOIN sisfor.identificacaousuario i ON i.iusd = p.iusd
			INNER JOIN sisfor.folhapagamento f ON f.fpbid = p.fpbid  
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			INNER JOIN sisfor.tipoperfil t ON t.tpeid=p.tpeid AND t.pflcod=".PFL_COORDENADOR_INST."
			INNER JOIN seguranca.perfil pp ON pp.pflcod = p.pflcod  
			INNER JOIN sisfor.sisfories s ON s.usucpf = i.iuscpf 
			INNER JOIN public.unidade u on u.unicod = s.unicod 
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
			WHERE p.iusd='{$iusd_aco}'
	
			)
	
			) foo 
	
			ORDER BY foo.referencia";
	
	$cabecalho = array("Bolsista","Perfil","IES","Curso","R$","Período","Situação");
	$db->monta_lista_simples($sql, $cabecalho, 50, 5, 'S', '100%');

		
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="1" align="center">';
	echo '<tr><td class="SubTituloCentro" style="font-size:xx-small;">Status de pagamento</td><td class="SubTituloCentro" style="font-size:xx-small;">Tempo médio</td><td class="SubTituloCentro" style="font-size:xx-small;">Descrição</td></tr>';
	echo '<tr><td style="font-size:xx-small;">Aguardando autorização</td><td style="font-size:xx-small;">aprox. 2 dias</td><td style="font-size:xx-small;">O bolsista foi avaliado e considerado apto a receber a bolsa. A liberação do pagamento está aguardando autorização final pelo COMFOR.</td></tr>';
	echo '<tr><td style="font-size:xx-small;">Autorizado</td><td style="font-size:xx-small;">aprox. 3 dias</td><td style="font-size:xx-small;">O pagamento da bolsa foi autorizado pelo COMFOR e está em análise no MEC.</td></tr>';
	echo '<tr><td style="font-size:xx-small;">Aguardando autorização SGB</td><td style="font-size:xx-small;">aprox. 3 dias</td><td style="font-size:xx-small;">O pagamento da bolsa foi aprovado pelo MEC e encontra-se no Sistema de Gestão de Bolsas (SGB), aguardando homologação.</td></tr>';
	echo '<tr><td style="font-size:xx-small;">Aguardando pagamento</td><td style="font-size:xx-small;">aprox. 9 dias</td><td style="font-size:xx-small;">O pagamento da bolsa foi homologado no SGB e está em processamento.</td></tr>';
	echo '<tr><td style="font-size:xx-small;">Enviado ao Banco</td><td style="font-size:xx-small;">aprox. 7 dias</td><td style="font-size:xx-small;">A ordem bancária referente ao pagamento da bolsa foi emitida.</td></tr>';
	echo '<tr><td style="font-size:xx-small;">Pagamento efetivado</td><td style="font-size:xx-small;">-</td><td style="font-size:xx-small;">O pagamento foi creditado em conta e confirmado pelo banco.</td></tr>';
	echo '<tr><td style="font-size:xx-small;">Pagamento não autorizado FNDE</td><td style="font-size:xx-small;">-</td><td style="font-size:xx-small;">O pagamento da bolsa não foi autorizado pelo FNDE, pois o bolsista recebe bolsa de outro programa do MEC.</td></tr>';
	echo '<tr><td style="font-size:xx-small;">Pagamento recusado</td><td style="font-size:xx-small;">aprox. 2 dias</td><td style="font-size:xx-small;">Pagamento recusado em função de algum erro de registro. Será reencaminhado a IES responsável pela formação.</td></tr>';
	echo '<tr><td colspan="3" style="font-size:xx-small;"><p><b>Observação: Caso o seu status no fluxo de pagamento esteja em BRANCO, significa que a avaliação ainda não foi concluída naquele mês de referência. Neste caso, você deve procurar a coordenação do curso.</b></p></td></tr>';
	echo '</table>';

	?>
	</td>
</tr>

</table>