<?
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


$menu = carregarMenuExecucaoSolicitacao();

echo montarAbasArray($menu, "/fabrica/fabrica.php?modulo=principal/abriSolicitacao&acao=A&ansid=".$_SESSION['fabrica_var']['ansid']."&scsid=".$_SESSION['fabrica_var']['scsid']);

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
	   		url: "fabrica.php?modulo=principal/abrirSolicitacao&acao=A",
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
function voltar2(url){
	window.location = url;
}

</script>
<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">

<tr>
	<td width="95%" valign="top">
	<?
	if($oss[0]) {
		foreach($oss as $os) {
			// somando a quantidade de pontos de função
			$total_pf += $os['odsqtdpfestimada'];
            
            $arrEstado = wf_pegarEstadoAtual( $db->pegaUm("select docid from fabrica.ordemservico where odsid = ".$os['odsid']));
            $estado = $arrEstado['esddsc'];
		?>
		<table class="tabela" width="100%">
		<tr>
			<td class="SubTituloEsquerda" colspan="4">
				Nº OS <span title="Editar O.S." style="font-size:14px; cursor:pointer; color: #0066CC;" onclick="window.open('fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=<? echo $os['odsid']; ?>','Observações','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');"><?  echo $os['odsid']; ?></span>
				<?php if(!in_array(PERFIL_PREPOSTO,$pfls) && !in_array(PERFIL_ESPECIALISTA_SQUADRA,$pfls)){?>
					&nbsp;
					<img src="../imagens/alterar.gif" title="Editar O.S." style="cursor:pointer;" onclick="window.open('fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=<? echo $os['odsid']; ?>','Observações','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');">
					&nbsp;
					<img src="../imagens/editar_nome.gif" title="Estimar Contagem de P.F." style="cursor:pointer;" onclick="window.open('fabrica.php?modulo=principal/popup/contagemPontoFuncao&acao=A&odsidpai=<?=$os['odsid']?>&tosid=<?=TIPO_OS_CONTAGEM_ESTIMADA?>','ContagemPFE','scrollbars=yes,height=400,width=840,status=no,toolbar=no,menubar=no,location=no');">
					&nbsp;
					<img src="../imagens/editar_nome_vermelho.gif" title="Detalhar Contagem de P.F." style="cursor:pointer;" onclick="window.open('fabrica.php?modulo=principal/popup/contagemPontoFuncao&acao=A&odsidpai=<?=$os['odsid']?>&tosid=<?=TIPO_OS_CONTAGEM_DETALHADA?>','ContagemPFD','scrollbars=yes,height=400,width=840,status=no,toolbar=no,menubar=no,location=no');">
				<?php }?>
                    - <a style="cursor: pointer" onclick="window.open('fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=<? echo $os['odsid']; ?>','Observações','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');"><?php echo $estado ?></a>
				<table class="tabela">
					<tr>
						<td width="15%" class="SubTituloDireita">Subtotal de PF Detalhado:</td>
						<td width="35%" ><? echo $os['odssubtotalpfdetalhada']; ?></td>
						<td width="15%" class="SubTituloDireita">Previsão de Início:</td>
						<td width="35%" ><?  echo $os['odsdtprevinicio']; ?></td>
					</tr>
					<tr>
						<td width="15%" class="SubTituloDireita">PF a Pagar:</td>
						<td width="35%" ><? echo $os['odsqtdpfdetalhada']; ?></td>
						<td class="SubTituloDireita">Previsão de Término:</td>
						<td><?  echo $os['odsdtprevtermino']; ?></td>
					</tr>
					<tr>
						<td width="15%" class="SubTituloDireita">Subtotal de PF:</td>
						<td width="35%" ><? echo $os['odssubtotalpf']; ?></td>
					</tr>
					<tr>
						<td width="15%" class="SubTituloDireita">Qtd. PF:</td>
						<td width="35%" ><? echo $os['odsqtdpfestimada']; ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Detalhamento:</td>
						<td><?  echo $os['odsdetalhamento']; ?></td>
					</tr>
					<tr>
						<td>
							<div style="float:left" id="divAnexosMais_<?=$os['odsid']?>"><a href="javascript:mostraAnexos('<?=$os['odsid']?>',1);">[+] Anexos nº os <?=$os['odsid']?></a></div>
							<div style="float:left;display:none" id="divAnexosMenos_<?=$os['odsid']?>"><a href="javascript:mostraAnexos('<?=$os['odsid']?>',2);">[-] Anexos nº os <?=$os['odsid']?></a></div>
						</td>
					</tr>
					<tr style="display: none" id="tr_anexos_<?=$os['odsid']?>">
						<td id="td_anexos_<?=$os['odsid']?>" class="SubTituloEsquerda" colspan="4">
						</td>
					</tr>
				</table>

				<? $odsfilho = $db->carregar("select *,
													 to_char(odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio,
													 to_char(odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino
											  from fabrica.ordemservico
											  where odsidpai = ".$os['odsid']." order by 1"); ?>
				<? if($odsfilho){

				   		foreach($odsfilho as $osfilho){

					 		$arrEstado = wf_pegarEstadoAtual( $db->pegaUm("select docidpf from fabrica.ordemservico where odsid = ".$osfilho['odsid']));
					 		$estado = $arrEstado['esddsc']; ?>

							<div style="margin-left: 10px">
							<br/>
							<img src=../imagens/seta_filho.gif align=absmiddle> Nº OS <?php echo "<span style='font-size:14px; cursor:pointer; color: #0066CC;' onclick=window.location.href='fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid=".$osfilho['odsid']."&scsid=".$_SESSION['fabrica_var']['scsid']."'>".$osfilho['odsid']."</span>";?>
							(<? if($osfilho['tosid'] == 2) echo 'Estimada'; else echo 'Detalhada'; ?>)
							- <a href="fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid=<?=$osfilho['odsid']; ?>&scsid=<?=$_SESSION['fabrica_var']['scsid']; ?>"><?php echo $estado ?></a>
							<br>
							<table class="tabela">



                                                                <?php if($osfilho['tosid'] == 2): ?>

                                                                <tr>
                                                                    <td width="15%" class="SubTituloDireita">Subtotal de PF:</td>
                                                                    <td width="35%" >

                                                                    <?php

                                                                            if($os['odssubtotalpf'] > $osfilho['odssubtotalpf']){

                                                                                $perct = $os['odssubtotalpf'] * 5 / 100;
                                                                                $limite = $os['odssubtotalpf'] - $perct;
                                                                                $valorComp = $osfilho['odssubtotalpf'];


                                                                            }else{

                                                                                $perct = $osfilho['odssubtotalpf'] * 5 / 100;
                                                                                $limite = $osfilho['odssubtotalpf'] - $perct;
                                                                                $valorComp = $os['odssubtotalpf'];

                                                                            }

                                                                            if($valorComp < $limite)
                                                                                echo "<span style='font-weight:bold;color:red;'>";
                                                                            else
                                                                                echo "<span>";

                                                                            echo $osfilho['odssubtotalpf'], "</span>";

                                                                    ?>

                                                                    </td>
                                                                    </tr>
                                                                    <tr>
                                                                            <td width="15%" class="SubTituloDireita">Qtd. PF:</td>
                                                                            <td width="35%" >
                                                                                <?php echo $osfilho['odsqtdpfestimada']; ?>
                                                                            </td>
                                                                    </tr>

                                                                <?php else: ?>


                                                                    <tr>
                                                                        <td width="15%" class="SubTituloDireita">Subtotal de PF Detalhado:</td>
                                                                        <td width="35%" >
                                                                        <?php


                                                                            if($os['odssubtotalpfdetalhada'] > $osfilho['odssubtotalpf']){

                                                                                $perct = $os['odssubtotalpfdetalhada'] * 5 / 100;
                                                                                $limite = $os['odssubtotalpfdetalhada'] - $perct;
                                                                                $valorComp = $osfilho['odssubtotalpf'];


                                                                            }else{

                                                                                $perct = $osfilho['odssubtotalpf'] * 5 / 100;
                                                                                $limite = $osfilho['odssubtotalpf'] - $perct;
                                                                                $valorComp = $os['odssubtotalpfdetalhada'];

                                                                            }

                                                                            if($valorComp < $limite)
                                                                                echo "<span style='font-weight:bold;color:red;'>";
                                                                            else
                                                                                echo "<span>";

                                                                            echo $osfilho['odssubtotalpf'], "</span>";


                                                                        ?>

                                                                        </td>
                                                                    </tr>
                                                                    <tr>
                                                                        <td width="15%" class="SubTituloDireita">PF a Pagar:</td>
                                                                        <td width="35%" ><?  echo $osfilho['odsqtdpfdetalhada']; ?></td>
                                                                    </tr>

                                                                <?php endif; ?>

                                                                <tr>
									<td width="15%" class="SubTituloDireita">Previsão de Início:</td>
									<td width="35%" ><?  echo $osfilho['odsdtprevinicio']; ?></td>
									<td class="SubTituloDireita">Previsão de Término:</td>
									<td><?  echo $osfilho['odsdtprevtermino']; ?></td>
								</tr>
								<tr>
									<td class="SubTituloDireita">Detalhamento:</td>
									<td><?  echo $osfilho['odsdetalhamento']; ?></td>
								</tr>
								<tr>
									<td>
										<div style="float:left" id="divAnexosMais_<?=$osfilho['odsid']?>"><a href="javascript:mostraAnexos('<?=$osfilho['odsid']?>',1);">[+] Anexos nº os <?=$osfilho['odsid']?></a></div>
										<div style="float:left;display:none" id="divAnexosMenos_<?=$osfilho['odsid']?>"><a href="javascript:mostraAnexos('<?=$osfilho['odsid']?>',2);">[-] Anexos nº os <?=$osfilho['odsid']?></a></div>
									</td>
								</tr>
								<tr style="display: none" id="tr_anexos_<?=$osfilho['odsid']?>">
									<td id="td_anexos_<?=$osfilho['odsid']?>" class="SubTituloEsquerda" colspan="4">
									</td>
								</tr>
							</table>
							</div>
				   		<?}
					}?>
			</td>
		</tr>

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
		<input type="button" name="voltar" value="Voltar" onclick="voltar2('?modulo=principal/listarSolicitacoes&acao=A&manterPesquisa=true');">
	</td>
</tr>
</table>

</form>



