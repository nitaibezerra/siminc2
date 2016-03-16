<?
if(!$_SESSION['sispacto3'][$sis]['uncid']) {
	$al = array("alert"=>"Usuário não vinculado com nenhuma universidade","location"=>"sispacto3.php?modulo=inicio&acao=C");
	alertlocation($al);
}
?>
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
	<td class="SubTituloCentro" colspan="2">Acompanhamento de avaliações e bolsas</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao("/sispacto3/sispacto3.php?modulo=principal/{$sis}/".(($sis=='universidade')?'universidadeexecucao':(($sis=='coordenadorlocal')?'coordenadorlocalexecucao':$sis))."&acao=A&aba=acompanhamentoavaliacaobolsas"); ?></td>
</tr>
<tr>
	<td colspan="2">
	<?
	
	$sql = "SELECT p.plpmaximobolsas 
			FROM sispacto3.identificacaousuario i 
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sispacto3.pagamentoperfil p ON p.pflcod = t.pflcod 
			WHERE i.iusd='".$_SESSION['sispacto3'][$sis]['iusd']."'";
	
	$nmaximobolsas = $db->pegaUm($sql);
		
	$sql = "SELECT f.fpbid as codigo, 
				   rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao,
				   COALESCE((
				   	SELECT e.esddsc || ' ( ' || to_char(h.htddata,'dd/mm/YYYY HH24:MI') || ' )' as s FROM sispacto3.pagamentobolsista p 
					INNER JOIN workflow.documento d ON d.docid = p.docid 
					INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
					LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
					WHERE p.iusd='".$_SESSION['sispacto3'][$sis]['iusd']."' AND p.fpbid=f.fpbid
					),'') as statuspagamento  
			FROM sispacto3.folhapagamento f 
			INNER JOIN sispacto3.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid AND rf.pflcod=(SELECT pflcod FROM sispacto3.tipoperfil WHERE iusd=".$_SESSION['sispacto3'][$sis]['iusd'].") 
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE rf.uncid='".$_SESSION['sispacto3'][$sis]['uncid']."' AND to_char(NOW(),'YYYY-mm-dd')::date>=(fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-01')::date
			ORDER BY (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date 
			LIMIT ".$nmaximobolsas;
	
	$folhapagamento = $db->carregar($sql);
	
	if($folhapagamento[0]) {
		
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="8" align="center">';
		echo '<tr><td class="SubTituloEsquerda" colspan="2">Extrato de pagamento/avaliações</td></tr>';
		echo '<tr><td class="SubTituloCentro" width="30%">Parcela</td><td class="SubTituloCentro" width="70%">Situação pagamento(Data de atualização)</td></tr>';
		
		foreach($folhapagamento as $fl) {
			
			echo '<tr><td class="SubTituloEsquerda">'.$fl['descricao'].'<br>
				   		<img src="../imagens/mais.gif" style="cursor:pointer;" title="mais" id="img_ava_'.$fl['codigo'].'" onclick="abrirDetalhes(\'ava_'.$fl['codigo'].'\');"> <span style=font-size:xx-small;>Avaliação</span><br>
				   		<img src="../imagens/mais.gif" style="cursor:pointer;" title="mais" id="img_pag_'.$fl['codigo'].'" onclick="abrirDetalhes(\'pag_'.$fl['codigo'].'\');"> <span style=font-size:xx-small;>Pagamento</span>
				   	  </td>
					  <td><font size=3><b>'.(($fl['statuspagamento'])?$fl['statuspagamento']:'').'</b></font></td></tr>';
			echo '<tr style="display:none;" id="tr_ava_'.$fl['codigo'].'"><td colspan="2">';
			echo '<p align="center"><b>INFORMAÇÕES SOBRE AVALIAÇÕES</b></p>';
			
			$sql = "SELECT * FROM sispacto3.mensario WHERE fpbid='".$fl['codigo']."' AND iusd='".$_SESSION['sispacto3'][$sis]['iusd']."'";
			$mensario = $db->pegaLinha($sql);
			
			if($mensario['menid']) consultarDetalhesAvaliacoes(array('menid'=>$mensario['menid']));
			else echo '<p align=center style=color:red;>Não existem avaliações nesse período de referência</p>';
			
			echo '</td></tr>';
			echo '<tr style="display:none;" id="tr_pag_'.$fl['codigo'].'"><td colspan="2">';
			echo '<p align="center"><b>INFORMAÇÕES SOBRE PAGAMENTO</b></p>';
			
			$sql = "SELECT pboid FROM sispacto3.pagamentobolsista WHERE fpbid='".$fl['codigo']."' AND iusd='".$_SESSION['sispacto3'][$sis]['iusd']."'";
			$pboid = $db->pegaUm($sql);
			
			if($pboid) {
				consultarDetalhesPagamento(array('pboid'=>$pboid));
			} else {
				echo "<p align=center style=color:red;>Não existem pagamentos nesse período de referência</p>";	
				
				$restricao = pegarRestricaoPagamento(array('iusd' => $_SESSION['sispacto3'][$sis]['iusd'], 'fpbid' => $fl['codigo']));
				
				echo "<table class=\"listagem\" bgcolor=\"#f5f5f5\" cellSpacing=\"5\" cellPadding=\"10\" align=\"center\">";
				echo "<tr>";
				echo "<td class=\"SubTituloDireita\"><b>Possível restrição:</b></td>";
				echo "<td><b>".$restricao."</b></td>";
				echo "</tr>";
				echo "</table>";
			} 
			
			
			echo '</td></tr>';
			
		}
		echo '<tr><td class="SubTituloCentro" colspan="2">';
		criarBotoesNavegacao(array('url' => $_SERVER['REQUEST_URI']));
		echo '</td></tr>';
		echo '</table>';
		
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="1" align="center">';
		echo '<tr><td colspan="3" style="font-size:xx-small;"><p>Prezado bolsista, após ser APROVADO no fluxo de avaliação, o pagamento das bolsas no âmbito do Pacto Nacional pela Alfabetização na Idade Certa obedece ao seguinte fluxo:</p></td></tr>';
		echo '<tr><td class="SubTituloCentro" style="font-size:xx-small;">Status de pagamento</td><td class="SubTituloCentro" style="font-size:xx-small;">Tempo médio</td><td class="SubTituloCentro" style="font-size:xx-small;">Descrição</td></tr>';
		echo '<tr><td style="font-size:xx-small;">Aguardando autorização IES</td><td style="font-size:xx-small;">aprox. 2 dias</td><td style="font-size:xx-small;">O bolsista foi avaliado e considerado apto a receber a bolsa. A liberação do pagamento está aguardando autorização final pela Universidade responsável pela formação.</td></tr>';
		echo '<tr><td style="font-size:xx-small;">Autorizado IES</td><td style="font-size:xx-small;">aprox. 3 dias</td><td style="font-size:xx-small;">O pagamento da bolsa foi autorizado pela Universidade e está sendo processado pelos sistemas do MEC.</td></tr>';
		echo '<tr><td style="font-size:xx-small;">Aguardando autorização SGB</td><td style="font-size:xx-small;">aprox. 3 dias</td><td style="font-size:xx-small;">O pagamento da bolsa está no Sistema de Gestão de Bolsas, aguardando autorização do MEC.</td></tr>';
		echo '<tr><td style="font-size:xx-small;">Aguardando pagamento</td><td style="font-size:xx-small;">aprox. 9 dias</td><td style="font-size:xx-small;">O pagamento da bolsa foi autorizado pelo SGB e está em processamento.</td></tr>';
		echo '<tr><td style="font-size:xx-small;">Enviado ao Banco</td><td style="font-size:xx-small;">aprox. 7 dias</td><td style="font-size:xx-small;">A ordem bancária referente ao pagamento da bolsa foi emitida.</td></tr>';
		echo '<tr><td style="font-size:xx-small;">Pagamento efetivado</td><td style="font-size:xx-small;">-</td><td style="font-size:xx-small;">O pagamento foi creditado em conta e confirmado pelo banco.</td></tr>';
		echo '<tr><td style="font-size:xx-small;">Pagamento não autorizado FNDE</td><td style="font-size:xx-small;">-</td><td style="font-size:xx-small;">O pagamento da bolsa não foi autorizado pelo FNDE, pois o bolsista recebe bolsa de outro programa do MEC.</td></tr>';
		echo '<tr><td style="font-size:xx-small;">Pagamento recusado</td><td style="font-size:xx-small;">aprox. 2 dias</td><td style="font-size:xx-small;">Pagamento recusado em função de algum erro de registro. Será reencaminhado a IES responsável pela formação.</td></tr>';
		echo '<tr><td colspan="3" style="font-size:xx-small;"><p><b>Observação: Caso o seu status no fluxo de pagamento esteja em BRANCO, significa que o mês de referencia ainda não teve o seu fluxo de avaliação concluído. Você deve procurar a coordenação local do PACTO ou a IES responsável pela formação do seu município.</b></p></td></tr>';
		echo '</table>';
					
		
		
	} else {
		$al = array("alert"=>"A universidade do Usuário não possui período de referência atribuído","location"=>"sispacto3.php?modulo=inicio&acao=C");
		alertlocation($al);
	}

	?>
	</td>
</tr>

<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<? if($goto_ant) : ?>
	<input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_ant ?>';">
	<? endif; ?>
	<? if($goto_pro) : ?>
	<input type="button" value="Próximo" onclick="divCarregando();window.location='<?=$goto_pro ?>';">
	<? endif; ?>
	</td>
</tr>
</table>