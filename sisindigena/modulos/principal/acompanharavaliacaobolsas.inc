<?
if(!$_SESSION['sisindigena'][$sis]['uncid']) {
	$al = array("alert"=>"Usuário não vinculado com nenhuma universidade","location"=>"sisindigena.php?modulo=inicio&acao=C");
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
	<td><? echo carregarOrientacao("/sisindigena/sisindigena.php?modulo=principal/{$sis}/".(($sis=='universidade')?'universidadeexecucao':(($sis=='coordenadorlocal')?'coordenadorlocalexecucao':$sis))."&acao=A&aba=acompanhamentoavaliacaobolsas"); ?></td>
</tr>
<tr>
	<td colspan="2">
	<?
		
	$sql = "SELECT DISTINCT f.fpbid as codigo, 
				   'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia as descricao,
				   COALESCE((
				   	SELECT e.esddsc || ' ( ' || to_char(h.htddata,'dd/mm/YYYY HH24:MI') || ' )' as s FROM sisindigena.pagamentobolsista p 
					INNER JOIN workflow.documento d ON d.docid = p.docid 
					INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
					LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
					WHERE p.iusd='".$_SESSION['sisindigena'][$sis]['iusd']."' AND p.fpbid=f.fpbid
					),'') as statuspagamento,
					(fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-01')::date as peri  
			FROM sisindigena.folhapagamento f 
			INNER JOIN sisindigena.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE rf.uncid='".$_SESSION['sisindigena'][$sis]['uncid']."' ".(($_SESSION['sisindigena'][$sis]['picid'])?" AND rf.picid='".$_SESSION['sisindigena'][$sis]['picid']."'":"")." AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'01')::date,'YYYYmm') ORDER BY peri";
	
	$folhapagamento = $db->carregar($sql);
	
	if($folhapagamento[0]) {
		
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="8" align="center">';
		echo '<tr><td class="SubTituloEsquerda" colspan="2">Extrato de pagamento/avaliações</td></tr>';
		echo '<tr><td class="SubTituloCentro" width="50%">Parcela</td><td class="SubTituloCentro" width="50%">Situação pagamento(Data de atualização)</td></tr>';
		
		foreach($folhapagamento as $fl) {
			
			echo '<tr><td class="SubTituloEsquerda">'.$fl['descricao'].'</td>
					  <td><font size=3><b>'.$fl['statuspagamento'].'</b></font></td></tr>';
			echo '<tr><td colspan="2"><img src="../imagens/mais.gif" style="cursor:pointer;" title="mais" id="img_ava_'.$fl['codigo'].'" onclick="abrirDetalhes(\'ava_'.$fl['codigo'].'\');"> Detalhes da avaliação</td></td></tr>';
			echo '<tr style="display:none;" id="tr_ava_'.$fl['codigo'].'"><td colspan="2">';
			echo '<p align="center"><b>INFORMAÇÕES SOBRE AVALIAÇÕES</b></p>';
			
			$sql = "SELECT * FROM sisindigena.mensario WHERE fpbid='".$fl['codigo']."' AND iusd='".$_SESSION['sisindigena'][$sis]['iusd']."'";
			$mensario = $db->pegaLinha($sql);
			
			if($mensario['menid']) consultarDetalhesAvaliacoes(array('menid'=>$mensario['menid']));
			else echo '<p align=center style=color:red;>Não existem avaliações nesse período de referência</p>';
			
			echo '</td></tr>';
			echo '<tr><td colspan="2"><img src="../imagens/mais.gif" style="cursor:pointer;" title="mais" id="img_pag_'.$fl['codigo'].'" onclick="abrirDetalhes(\'pag_'.$fl['codigo'].'\');"> Detalhes do pagamento</td></td></tr>';
			echo '<tr style="display:none;" id="tr_pag_'.$fl['codigo'].'"><td colspan="2">';
			echo '<p align="center"><b>INFORMAÇÕES SOBRE PAGAMENTO</b></p>';
			
			$sql = "SELECT pboid FROM sisindigena.pagamentobolsista WHERE fpbid='".$fl['codigo']."' AND iusd='".$_SESSION['sisindigena'][$sis]['iusd']."'";
			$pboid = $db->pegaUm($sql);
			
			if($pboid) {
				consultarDetalhesPagamento(array('pboid'=>$pboid));
			} else {
				echo "<p align=center style=color:red;>Não existem pagamentos nesse período de referência</p>";	
				
				$restricao = pegarRestricaoPagamento(array('iusd' => $_SESSION['sisindigena'][$sis]['iusd'], 'fpbid' => $fl['codigo']));
				
				echo "<table class=\"listagem\" bgcolor=\"#f5f5f5\" cellSpacing=\"5\" cellPadding=\"10\" align=\"center\">";
				echo "<tr>";
				echo "<td class=\"SubTituloDireita\"><b>Possível restrição:</b></td>";
				echo "<td><b>".$restricao."</b></td>";
				echo "</tr>";
				echo "</table>";
			} 
			
			
			echo '</td></tr>';
			
		}
		
		//echo '<tr><td class="SubTituloCentro" colspan="2"><input type=button value=Voltar onclick="window.location=\'sisindigena_consulta_pagamento.php\';"></td></tr>';
		
		echo '</table>';
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="8" align="center">';
		echo '<tr><td colspan="2"><p>Prezado bolsista, após ser APROVADO no fluxo de avaliação, o pagamento das bolsas no âmbito do Pacto Nacional pela Alfabetização na Idade Certa obedece ao seguinte fluxo:</p></td></tr>';
		echo '<tr><td class="SubTituloCentro">Status de pagamento</td><td class="SubTituloCentro">Descrição</td></tr>';
		echo '<tr><td>Aguardando autorização IES</td><td>O bolsista foi avaliado e considerado apto a receber a bolsa. A liberação do pagamento está aguardando autorização final pela Universidade responsável pela formação.</td></tr>';
		echo '<tr><td>Autorizado IES</td><td>O pagamento da bolsa foi autorizado pela Universidade e está sendo processado pelos sistemas do MEC.</td></tr>';
		echo '<tr><td>Aguardando autorização SGB</td><td>O pagamento da bolsa está no Sistema de Gestão de Bolsas, aguardando autorização do MEC.</td></tr>';
		echo '<tr><td>Aguardando pagamento</td><td>O pagamento da bolsa foi autorizado pelo SGB e está em processamento.</td></tr>';
		echo '<tr><td>Enviado ao Banco</td><td>A ordem bancária referente ao pagamento da bolsa foi emitida. O pagamento estará disponível para saque em até 02 dias úteis, em função do processamento da O.B. pelo banco</td></tr>';
		echo '<tr><td>Pagamento efetivado</td><td>O pagamento foi creditado em conta e confirmado pelo banco.</td></tr>';
		echo '<tr><td>Pagamento não autorizado FNDE</td><td>O pagamento da bolsa não foi autorizado pelo FNDE, pois o bolsista recebe bolsa de outro programa do MEC.</td></tr>';
		echo '<tr><td>Pagamento recusado</td><td>Pagamento recusado em função de algum erro de registro. Será reencaminhado a IES responsável pela formação.</td></tr>';
		echo '<tr><td colspan="2"><p>Observação: Caso o seu status no fluxo de pagamento esteja em BRANCO, significa que o mês de referencia ainda não teve o seu fluxo de avaliação concluído. Você deve procurar a coordenação local do PACTO ou a IES responsável pela formação do seu município.</p></td></tr>';
		echo '</table>';
		
	} else {
		$al = array("alert"=>"A universidade do Usuário não possui período de referência atribuído","location"=>"sisindigena.php?modulo=inicio&acao=C");
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