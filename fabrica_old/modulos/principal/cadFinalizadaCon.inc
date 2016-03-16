<?
include APPRAIZ ."includes/workflow.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

echo '<br>';

if($_REQUEST['scsid']) {
	$_SESSION['fabrica_var']['scsid'] = $_REQUEST['scsid'];
}

if($_REQUEST['ansid']) {
	$_SESSION['fabrica_var']['ansid'] = $_REQUEST['ansid'];
}


$sql = "SELECT a.ansdtrecebimento, s.docid FROM fabrica.analisesolicitacao a
		LEFT JOIN fabrica.solicitacaoservico s ON a.scsid=s.scsid
		WHERE ansid='".$_SESSION['fabrica_var']['ansid']."'";
$dadosan = $db->pegaLinha($sql);

$ansdtrecebimento = $dadosan['ansdtrecebimento'];

$estado_wf = wf_pegarEstadoAtual($dadosan['docid']);

if($estado_wf['esdid'] == WF_ESTADO_AVALIACAO || $estado_wf['esdid'] == WF_ESTADO_APROVACAO || $estado_wf['esdid'] == WF_ESTADO_EXECUCAO) {
	$formhabil = "S";
} else {
	$formhabil = "N";
}


$menu = array(0 => array("id" => 1, "descricao" => "Executar Solicitação",      		 "link" => "/fabrica/fabrica.php?modulo=principal/cadExecucaoCon&acao=A&ansid=".$_SESSION['fabrica_var']['ansid']."&scsid=".$_SESSION['fabrica_var']['scsid']),
				  1 => array("id" => 2, "descricao" => "Observações", 						 "link" => "/fabrica/fabrica.php?modulo=principal/cadSSObservacaoCon&acao=A&tipoobs=cadExecucao"),
				  2 => array("id" => 3, "descricao" => "Anexos da ordem de serviço",         "link" => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoAnexosCon&acao=D"),
				  3 => array("id" => 4, "descricao" => "Monitoramento / Controle de riscos", "link" => "/fabrica/fabrica.php?modulo=principal/monitoramentoRiscosCon&acao=C"),
				  4 => array("id" => 5, "descricao" => "Providências", 		 		 	 	 "link" => "/fabrica/fabrica.php?modulo=principal/providenciasCon&acao=A&tipoaba=cadExecucao")
			  	  );

echo montarAbasArray($menu, "/fabrica/fabrica.php?modulo=principal/cadExecucaoCon&acao=A&ansid=".$_SESSION['fabrica_var']['ansid']."&scsid=".$_SESSION['fabrica_var']['scsid']);

$titulo_modulo = "Executando solicitação";
monta_titulo($titulo_modulo, 'Fiscal Técnico');

telaCabecalhoSolicitacaoServico(array("scsid" => $_SESSION['fabrica_var']['scsid']));
listaDisciplinaArtefato($_SESSION['fabrica_var']['ansid'], null, true, 1);


//recupera perfis de usuario
$pfls = arrayPerfil();


$sql = "SELECT
            os.odsid,
            os.odsdetalhamento,
            os.odsqtdpfestimada,
            to_char(os.odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio,
            to_char(os.odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino,
            esd.esddsc,
            os.odssubtotalpfdetalhada,
            os.odsqtdpfdetalhada,
            os.odssubtotalpf

		FROM fabrica.ordemservico os
		LEFT JOIN workflow.documento doc ON doc.docid=os.docid
		LEFT JOIN workflow.estadodocumento esd ON esd.esdid=doc.esdid
		WHERE os.scsid='".$_SESSION['fabrica_var']['scsid']."' and tosid = ".TIPO_OS_GERAL." ORDER BY os.odsid";
$oss = $db->carregar($sql);
?>

<script>

function mostraAnexos(odsid, tipo){

	var tr = document.getElementById('tr_anexos_'+odsid);
	var td = document.getElementById('td_anexos_'+odsid);
	var mais = document.getElementById('divAnexosMais_'+odsid);
	var menos = document.getElementById('divAnexosMenos_'+odsid);

	if(tipo == 1){
		mais.style.display = 'none';
		menos.style.display = '';
		tr.style.display = '';
		td.style.display = '';

		$.ajax({
	   		type: "POST",
	   		url: "fabrica.php?modulo=principal/cadFinalizadaCon&acao=A",
	   		data: "requisicao=mostrarAnexos&odsid="+odsid,
	   		success: function(msg){
				td.innerHTML = msg;
	   		}
	 		});

	}
	else{
		td.style.display = 'none';
		tr.style.display = 'none';
		mais.style.display = '';
		menos.style.display = 'none';
	}

}

</script>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">

<tr>
	<td width="95%" valign="top">
	<?
	if($oss[0]) {
		foreach($oss as $os) {
			// somando a quantidade de pontos de função
			$total_pf += $os['odsqtdpfestimada'];
		?>
		<table class="listagem" width="100%">
		<tr>
			<td class="SubTituloEsquerda" colspan="4">
				Nº OS <span style=font-size:14px;><?  echo $os['odsid']; ?></span>
				<?php if(!in_array(PERFIL_PREPOSTO,$pfls) && !in_array(PERFIL_ESPECIALISTA_SQUADRA,$pfls)){?>
					&nbsp;
					<img src="../imagens/alterar.gif" title="Editar O.S." style="cursor:pointer;" onclick="window.open('fabrica.php?modulo=principal/cadOSExecucaoCon&acao=A&odsid=<? echo $os['odsid']; ?>','Observações','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');">
					&nbsp;
					
					<?
					$odsfilho1 = $db->pegaUm("select * from fabrica.ordemservico where odsidpai = ".$os['odsid']." and tosid = 2");
					if($odsfilho1){?>
						<img src="../imagens/editar_nome.gif" title="Estimar Contagem de P.F." style="cursor:pointer;" onclick="window.open('fabrica.php?modulo=principal/popup/contagemPontoFuncaoCon&acao=A&odsidpai=<?=$os['odsid']?>&tosid=<?=TIPO_OS_CONTAGEM_ESTIMADA?>','ContagemPFE','scrollbars=yes,height=400,width=840,status=no,toolbar=no,menubar=no,location=no');">
						&nbsp;
					<?}?>
					<?
					$odsfilho2 = $db->pegaUm("select * from fabrica.ordemservico where odsidpai = ".$os['odsid']." and tosid = 3");
					if($odsfilho2){?>
						<img src="../imagens/editar_nome_vermelho.gif" title="Detalhar Contagem de P.F." style="cursor:pointer;" onclick="window.open('fabrica.php?modulo=principal/popup/contagemPontoFuncaoCon&acao=A&odsidpai=<?=$os['odsid']?>&tosid=<?=TIPO_OS_CONTAGEM_DETALHADA?>','ContagemPFD','scrollbars=yes,height=400,width=840,status=no,toolbar=no,menubar=no,location=no');">
					<?}?>
					
				<?php }?>
				<? $odsfilho = $db->pegaUm("select odsid from fabrica.ordemservico where odsidpai = ".$os['odsid']); ?>
				<? if($odsfilho): ?>
				<?php $estado = verificarEnvioContagemPF($os['odsid']) ?>
				<?php if($estado): ?>
					<br/>
					<img src=../imagens/seta_filho.gif align=absmiddle> Nº OS <span style=font-size:14px;><? echo $odsfilho; ?></span>
					- <a href="fabrica.php?modulo=principal/cadContagemOSCon&acao=A&odsid=<?=$odsfilho; ?>&scsid=<?=$_SESSION['fabrica_var']['scsid']; ?>"><?php echo $estado ?></a>
				<?php endif; ?>
				<? endif; ?>
			</td>
		</tr>


    <?if($estado_wf['esdid'] == WF_ESTADO_FINALIZADA):?>

        <tr>
            <td width="15%" class="SubTituloDireita">Subtotal de PF Detalhado:</td>
            <td width="35%" ><? echo $os['odssubtotalpfdetalhada']; ?></td>
        </tr>
        <tr>
            <td width="15%" class="SubTituloDireita">PF a Pagar:</td>
            <td width="35%" ><?  echo $os['odsqtdpfdetalhada']; ?></td>
            <td width="15%" class="SubTituloDireita">Subtotal de PF:</td>
            <td width="35%" ><?  echo $os['odssubtotalpf']; ?></td>
        </tr>
    <? endif;?>

		<tr>
			<td width="15%" class="SubTituloDireita">Qtd. PF:</td>
			<td width="35%" ><? echo $os['odsqtdpfestimada']; ?></td>
			<td width="15%" class="SubTituloDireita">Previsão de Início:</td>
			<td width="35%" ><?  echo $os['odsdtprevinicio']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Detalhamento:</td>
			<td><?  echo $os['odsdetalhamento']; ?></td>
			<td class="SubTituloDireita">Previsão de Término:</td>
			<td><?  echo $os['odsdtprevtermino']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" colspan="4">
				<div style="float:left">
				<?php if($os['odscontratada'] == "f"): ?>
					<? //if($editarTela): ?>
					<!-- <img src="/imagens/alterar.gif" border=0 title="Editar OS" style="cursor:pointer;vertical-align:middle;" onclick="addOSMEC('<? // echo $os['odsid']; ?>');" > -->
					<? //endif; ?>
				<?php endif; ?>
				<?php $estado = verificarEnvioContagemPF($os['odsid']) ?>
				<?php if(!$estado): ?>
					 <? if($habil=='S'): ?>
					 <input type="button" name="btn_enviar_contagem" id="btn_enviar_contagem" value="Estimar Contagem de P.F." onclick="estimarContagemPF('<?  echo $os['odsid']; ?>')">
					 <? endif; ?>
				<?php endif; ?>
				&nbsp;&nbsp;
				</div>

				<?php if($os['odsid']){?>
					<div style="float:left" id="divAnexosMais_<?=$os['odsid']?>"><a href="javascript:mostraAnexos('<?=$os['odsid']?>',1);">[+] Anexos nº os <?=$os['odsid']?></a></div>
					<div style="float:left;display:none" id="divAnexosMenos_<?=$os['odsid']?>"><a href="javascript:mostraAnexos('<?=$os['odsid']?>',2);">[-] Anexos nº os <?=$os['odsid']?></a></div>
				<?php } ?>

				<? if($odsfilho): ?>
					<div style="margin-left: 10px">
						<br>
						<div style="float:left" id="divAnexosMais_<?=$odsfilho?>"><a href="javascript:mostraAnexos('<?=$odsfilho?>',1);"><img border=0 src=../imagens/seta_filho.gif align=absmiddle> [+] Anexos nº os <?=$odsfilho?></a></div>
						<div style="float:left;display:none" id="divAnexosMenos_<?=$odsfilho?>"><a href="javascript:mostraAnexos('<?=$odsfilho?>',2);"><img border=0 src=../imagens/seta_filho.gif align=absmiddle> [-] Anexos nº os <?=$odsfilho?></a></div>
					</div>
				<? endif; ?>
			</td>
		</tr>
		<?php if($os['odsid']){?>
		<tr style="display: none" id="tr_anexos_<?=$os['odsid']?>">
			<td id="td_anexos_<?=$os['odsid']?>" class="SubTituloEsquerda" colspan="4">
			</td>
		</tr>
		<?php } ?>
		<?php if($odsfilho){?>
		<tr style="display: none" id="tr_anexos_<?=$odsfilho?>">
			<td id="td_anexos_<?=$odsfilho?>" class="SubTituloEsquerda" colspan="4">
			</td>
		</tr>
		<?php } ?>
		</table>
		<br />
		<?
		}

	}
	?>
	</td>
	<td valign="top" width="5%">
	<?
	$dados_wf = array('scsid' => $_SESSION['fabrica_var']['scsid']);
	wf_desenhaBarraNavegacao(pegarDocidSolicitacaoServico($dados_wf), $dados_wf);
	?>
	</td>
</tr>
<tr>
	<td colspan="4" class="SubTituloEsquerda">Total de PF: <span style="font-size: 14px;"><?php echo $total_pf; ?></span></td>
</tr>
<tr>
	<td class="SubTituloDireita" colspan="4" align="center">
	<input type="button" name="voltar" value="Voltar" onclick="history.back(-1);">
	</td>
</tr>
</table>

</form>



