<?
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if($_REQUEST['download'] == 'S'){
	$arqid = $_REQUEST['arqid'];
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
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

//Correção para atender listagem do Painel Operacional
if( empty($_SESSION['fabrica_var']['ansid'])  && !empty($_SESSION['fabrica_var']['scsid']) )
{
    $sql = "SELECT ansid
            FROM fabrica.analisesolicitacao 
            WHERE scsid='".$_SESSION['fabrica_var']['scsid']."'";

    $dadosSS = $db->pegaLinha($sql);
    
    $_SESSION['fabrica_var']['ansid'] = $dadosSS['ansid'];
}

$sql = "SELECT a.ansdtrecebimento, a.ctrid, s.docid FROM fabrica.analisesolicitacao a
		LEFT JOIN fabrica.solicitacaoservico s ON a.scsid=s.scsid
		WHERE ansid='".$_SESSION['fabrica_var']['ansid']."'";
$dadosan = $db->pegaLinha($sql);

$ansdtrecebimento = $dadosan['ansdtrecebimento'];

$estado_wf = wf_pegarEstadoAtual($dadosan['docid']);
$ctrid = $dadosan['ctrid'];

//controle sessao
if(!$_SESSION['esdid_old']) $_SESSION['esdid_old'] = $estado_wf['esdid'];

if(($estado_wf['esdid'] != WF_ESTADO_AVALIACAO &&
   $estado_wf['esdid'] != WF_ESTADO_APROVACAO) ||
   $estado_wf['esdid'] != $_SESSION['esdid_old']) {
   	$_SESSION['esdid_old'] = '';
//	ob_clean();
//	header("location:fabrica.php?modulo=principal/listarSolicitacoes&acao=A");
//	exit;
}


//permissao para edição da tela
 $habil = 'S';
if(!verificaPermissaoEdicao()) $habil = 'N';

/**** somente super usuário, fiscal, gestor podem inserir/alterar uma analise ****/

$pfls = arrayPerfil();

switch($estado_wf['esdid']) {
	case WF_ESTADO_AVALIACAO:
		if(in_array(PERFIL_SUPER_USUARIO, $pfls) ||
		   in_array(PERFIL_FISCAL_CONTRATO,  $pfls) ||
		   in_array(PERFIL_GESTOR_CONTRATO,  $pfls) ||
			in_array(PERFIL_ADMINISTRADOR, $pfls)) {

		   	$habil = 'S';

		} else {

			$habil = 'N';
		}
                $ator = "Fiscal Técnico";
		break;
	case WF_ESTADO_APROVACAO:
		if(in_array(PERFIL_SUPER_USUARIO, $pfls) ||
		   in_array(PERFIL_GESTOR_CONTRATO,  $pfls) ||
			in_array(PERFIL_ADMINISTRADOR, $pfls)) {

		   	$habil = 'S';

		} else {

			$habil = 'N';
		}
                $ator = "Gestor do Contrato / Requisitante";
		break;

}


	function isDivergente($valor1, $valor2){
		$divergente = 1;

		if ($valor2 >= ($valor1 * 0.95) and ($valor2 <= ($valor1 * 1.05)) or $valor2 == 0 ){
			$divergente = 0;
		}
		return $divergente;
	}



$menu = carregarMenuAvaliacaoSolicitacao();

if($habil=='N') {
	unset($menu[2],$menu[3]);
}

echo montarAbasArray($menu, "/fabrica/fabrica.php?modulo=principal/abrirSolicitacao&acao=A&ansid=".$_SESSION['fabrica_var']['ansid']."&scsid=".$_SESSION['fabrica_var']['scsid']);

$titulo_modulo = "Avaliar / Aprovar Solicitação";
monta_titulo($titulo_modulo, $ator);

telaCabecalhoSolicitacaoServico(array("scsid" => $_SESSION['fabrica_var']['scsid']));
//listaDisciplinaArtefato($_SESSION['fabrica_var']['ansid'], null, true, 1);
editaDisciplinaArtefato($_SESSION['fabrica_var']['ansid']);


$sql = "SELECT
			odsid,
			odsdetalhamento,
			odsqtdpfestimada,
			odscontratada,
			odssubtotalpf,
			odssubtotalpfdetalhada,
			odsqtdpfdetalhada,
			to_char(odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio,
			to_char(odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino
		FROM
			fabrica.ordemservico
		WHERE
			scsid='".$_SESSION['fabrica_var']['scsid']."'
		AND
			tosid = ".TIPO_OS_GERAL."
		ORDER
			BY odscontratada,odsid";
$oss = $db->carregar($sql);


//variável que irá armazenar o total de pontos de função
$total_pf = 0;


//recupera sigla do item da metrica
$sigla = recuperaMetrica( $_SESSION['fabrica_var']['ansid'] );

?>
<style>
	.SubtituloTabela{background-color:#cccccc;text-align:center;font-weight:bold}
</style>

<script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>

<script>
	function estimarContagemPF(odsid)
	{
		if(odsid){
			window.open('fabrica.php?modulo=principal/popup/contagemPontoFuncao&acao=A&odsidpai=' + odsid + '&tosid=<?php echo TIPO_OS_CONTAGEM_ESTIMADA ?>&ctrid=<?php echo $ctrid ?>','Contagem_PF','scrollbars=yes,height=400,width=840,status=no,toolbar=no,menubar=no,location=no');
		}else{
			alert('Não é possível realizar a contagem desta OS!');
		}
	}

	function addOSMEC(odsid)
	{
		if(odsid){
			window.open('fabrica.php?modulo=principal/popup/OSContratante&acao=A&odsid=' + odsid,'OS Contratante','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
		}else{
			window.open('fabrica.php?modulo=principal/popup/OSContratante&acao=A','OS Contratante','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
		}
		void(0);
	}

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
</script>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">

<tr>
	<td width="95%" valign="top">
	<?
	if($oss[0]) {
		
		if($sigla == 'PF') {
			
		
			foreach($oss as $os) {
				// somando a quantidade de pontos de função
				$total_pf += $os['odsqtdpfestimada'];
			
				$odsfilho = $db->carregar("select *,
																		 to_char(odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio,
																		 to_char(odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino
																  from fabrica.ordemservico
																  where odsidpai = ".$os['odsid']." order by 1"); 
	        
	            $arrEstado  = wf_pegarEstadoAtual( $db->pegaUm( "select docid from fabrica.ordemservico where odsid = ". $os['odsid'] ) );
	            $estado     = $arrEstado['esddsc'];
			?>
																  
			<table class="listagem" width="100%">
			<tr>
				<td class="SubTituloEsquerda" colspan="4">
					Nº OS: <span title="Editar O.S." style="cursor:pointer; color: #0066CC;" onclick="window.open('fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=<? echo $os['odsid']; ?>','Observações','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');"><?  echo $os['odsid']; ?></span>
	                - <a style="cursor: pointer" onclick="window.open('fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=<? echo $os['odsid']; ?>','Observações','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');"><?php echo $estado ?></a>
				</td>
			</tr>
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
				<td width="35%" >
				<?php
	               if(isDivergente($os['odssubtotalpf'], $odsfilho[0]['odssubtotalpf']))
	                 	echo "<span style='font-weight:bold;color:red;'>";
	               	else
	               		echo "<span>";
	               		echo  $os['odssubtotalpf'], "</span>";
	              ?>
				
				</td>
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
					
					<? if($odsfilho){
	
					 foreach($odsfilho as $osfilho){ ?>
					<?php if($os['odsid']){?>
						<div style="float:left" id="divAnexosMais_<?=$os['odsid']?>"><a href="javascript:mostraAnexos('<?=$os['odsid']?>',1);">[+] Anexos nº os <?=$os['odsid']?></a></div>
						<div style="float:left;display:none" id="divAnexosMenos_<?=$os['odsid']?>"><a href="javascript:mostraAnexos('<?=$os['odsid']?>',2);">[-] Anexos nº os <?=$os['odsid']?></a></div>
					<?php } ?>
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
			<table class="listagem" width="100%">
			<tr>
				<td class="SubTituloEsquerda" colspan="4">
				
				
					   			<? if($osfilho): ?>
									<?php $estado = verificarEnvioContagemPF($os['odsid']) ?>
									<?php $estado2 = wf_pegarEstadoAtual($osfilho['docidpf']) ; ?>
									<?php if($estado): ?>
										<br/>
										<img src="../imagens/seta_filho.gif" align="absmiddle"> Nº OS <?php echo "<span style='font-size:14px; cursor:pointer; color: #0066CC;' onclick=window.location.href='fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid=".$osfilho['odsid']."&scsid=".$_SESSION['fabrica_var']['scsid']."'>".$osfilho['odsid']."</span>";?>
										- <a href="fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid=<?=$osfilho['odsid']; ?>&scsid=<?=$_SESSION['fabrica_var']['scsid']; ?>"><?php echo $estado2['esddsc'] ?></a>
									<?php endif; ?>
									<? endif; ?>
									<? if($osfilho['tosid'] == 2) echo 'Estimada'; else echo 'Detalhada'; ?>
									<tr>
										<td width="15%" class="SubTituloDireita">Subtotal de PF Detalhado:</td>
										<td width="35%" >
											<?php
											    if(isDivergente($os['odssubtotalpfdetalhada'], $osfilho['odssubtotalpf']))
											    	if ($osfilho['odssubtotalpf']!=''){
											        	echo "<span style='font-weight:bold;color:red;'>";
											    	}
											    else
											        echo "<span>";
											
											    echo $osfilho['odssubtotalpfdetalhada'], "</span>";
											?>
										</td>
										<td width="15%" class="SubTituloDireita">Previsão de Início:</td>
										<td width="35%" ><?  echo $osfilho['odsdtprevinicio']; ?></td>
									</tr>
									<tr>
										<td width="15%" class="SubTituloDireita">PF a Pagar:</td>
										<td width="35%" ><? echo $osfilho['odsqtdpfdetalhada']; ?></td>
										<td class="SubTituloDireita">Previsão de Término:</td>
										<td><?  echo $osfilho['odsdtprevtermino']; ?></td>
									</tr>
									<tr>
										<td width="15%" class="SubTituloDireita">Subtotal de PF :</td>
										<td width="35%" >
										<?php
											if(isDivergente($os['odssubtotalpf'], $osfilho['odssubtotalpf']))
												echo "<span style='font-weight:bold;color:red;'>";
											else
												echo "<span>";
												echo $osfilho['odssubtotalpf'], "</span>";
										?>
										</td>
									</tr>
									<tr>
										<td width="15%" class="SubTituloDireita">Qtd. PF:</td>
										<td width="35%" ><? echo $osfilho['odsqtdpfestimada']; ?></td>
									</tr>
									<tr>
										<td class="SubTituloDireita">Detalhamento:</td>
										<td><?  echo $osfilho['odsdetalhamento']; ?></td>
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
											<? if($osfilho): ?>
												<div style="margin-left: 10px">
													<br>
													<div style="float:left" id="divAnexosMais_<?=$osfilho['odsid']?>"><a href="javascript:mostraAnexos('<?=$osfilho['odsid']?>',1);"><img border=0 src=../imagens/seta_filho.gif align=absmiddle> [+] Anexos nº os <?=$osfilho['odsid']?></a></div>
													<div style="float:left;display:none" id="divAnexosMenos_<?=$osfilho['odsid']?>"><a href="javascript:mostraAnexos('<?=$osfilho['odsid']?>',2);"><img border=0 src=../imagens/seta_filho.gif align=absmiddle> [-] Anexos nº os <?=$osfilho['odsid']?></a></div>
												</div>
											<? endif; ?>
										</td>
									</tr>
					   		<?}
						
					}?>
				</td>
			</tr>
			<?php if($os['odsid']){?>
			<tr style="display: none" id="tr_anexos_<?=$os['odsid']?>">
				<td id="td_anexos_<?=$os['odsid']?>" class="SubTituloEsquerda" colspan="4">
				</td>
			</tr>
			<?php } ?>
			<?php if($osfilho['odsid']){?>
			<tr style="display: none" id="tr_anexos_<?=$osfilho['odsid']?>">
				<td id="td_anexos_<?=$osfilho['odsid']?>" class="SubTituloEsquerda" colspan="4">
				</td>
			</tr>
			<?php } ?>
			</table>
			<br />
			<?
			}//fecha foreach($oss as $os) {
			
			
			
		}else{ // UST
			
			
			foreach($oss as $os) {
				// somando a quantidade de pontos de função
				$total_pf += $os['odsqtdpfestimada'];
			
				$odsfilho = $db->carregar("select *,
																		 to_char(odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio,
																		 to_char(odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino
																  from fabrica.ordemservico
																  where odsidpai = ".$os['odsid']." order by 1"); 
	        
	            $arrEstado  = wf_pegarEstadoAtual( $db->pegaUm( "select docid from fabrica.ordemservico where odsid = ". $os['odsid'] ) );
	            $estado     = $arrEstado['esddsc'];
			?>
																  
			<table class="listagem" width="100%">
			<tr>
				<td class="SubTituloEsquerda" colspan="4">
					Nº OS: <span title="Editar O.S." style="cursor:pointer; color: #0066CC;" onclick="window.open('fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=<? echo $os['odsid']; ?>','Observações','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');"><?  echo $os['odsid']; ?></span>
	                - <a style="cursor: pointer" onclick="window.open('fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=<? echo $os['odsid']; ?>','Observações','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');"><?php echo $estado ?></a>
				</td>
			</tr>
			<tr>
				<td width="15%" class="SubTituloDireita">UST a Pagar:</td>
				<td width="35%" ><? echo $os['odsqtdpfdetalhada']; ?></td>
				<td width="15%" class="SubTituloDireita">Previsão de Início:</td>
				<td width="35%" ><?  echo $os['odsdtprevinicio']; ?></td>
			</tr>
			<tr>
				<td width="15%" class="SubTituloDireita">Qtd. UST:</td>
				<td width="35%" ><? echo $os['odsqtdpfestimada']; ?></td>
				<td class="SubTituloDireita">Previsão de Término:</td>
				<td><?  echo $os['odsdtprevtermino']; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Detalhamento:</td>
				<td><?  echo $os['odsdetalhamento']; ?></td>
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
						 <!-- 
						 <input type="button" name="btn_enviar_contagem" id="btn_enviar_contagem" value="Estimar Contagem de P.F." onclick="estimarContagemPF('<?  echo $os['odsid']; ?>')">
						  -->
						 <? endif; ?>
					<?php endif; ?>
					&nbsp;&nbsp;
					</div>
					
					<? if($odsfilho){
	
					 foreach($odsfilho as $osfilho){ ?>
					<?php if($os['odsid']){?>
						<div style="float:left" id="divAnexosMais_<?=$os['odsid']?>"><a href="javascript:mostraAnexos('<?=$os['odsid']?>',1);">[+] Anexos nº os <?=$os['odsid']?></a></div>
						<div style="float:left;display:none" id="divAnexosMenos_<?=$os['odsid']?>"><a href="javascript:mostraAnexos('<?=$os['odsid']?>',2);">[-] Anexos nº os <?=$os['odsid']?></a></div>
					<?php } ?>
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
			<table class="listagem" width="100%">
			<tr>
				<td class="SubTituloEsquerda" colspan="4">
				
				
					   			<? if($osfilho): ?>
									<?php $estado = verificarEnvioContagemPF($os['odsid']) ?>
									<?php $estado2 = wf_pegarEstadoAtual($osfilho['docidpf']) ; ?>
									<?php if($estado): ?>
										<br/>
										<img src="../imagens/seta_filho.gif" align="absmiddle"> Nº OS <?php echo "<span style='font-size:14px; cursor:pointer; color: #0066CC;' onclick=window.location.href='fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid=".$osfilho['odsid']."&scsid=".$_SESSION['fabrica_var']['scsid']."'>".$osfilho['odsid']."</span>";?>
										- <a href="fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid=<?=$osfilho['odsid']; ?>&scsid=<?=$_SESSION['fabrica_var']['scsid']; ?>"><?php echo $estado2['esddsc'] ?></a>
									<?php endif; ?>
									<? endif; ?>
									<? if($osfilho['tosid'] == 2) echo 'Estimada'; else echo 'Detalhada'; ?>
									<tr>
										<td width="15%" class="SubTituloDireita">UST a Pagar:</td>
										<td width="35%" ><? echo $osfilho['odsqtdpfdetalhada']; ?></td>										
										<td width="15%" class="SubTituloDireita">Previsão de Início:</td>
										<td width="35%" ><?  echo $osfilho['odsdtprevinicio']; ?></td>
									</tr>
									<tr>
										<td width="15%" class="SubTituloDireita">Qtd. UST:</td>
										<td width="35%" ><? echo $osfilho['odsqtdpfestimada']; ?></td>
										<td class="SubTituloDireita">Previsão de Término:</td>
										<td><?  echo $osfilho['odsdtprevtermino']; ?></td>
									</tr>
									<tr>
										<td class="SubTituloDireita">Detalhamento:</td>
										<td><?  echo $osfilho['odsdetalhamento']; ?></td>
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
											<? if($osfilho): ?>
												<div style="margin-left: 10px">
													<br>
													<div style="float:left" id="divAnexosMais_<?=$osfilho['odsid']?>"><a href="javascript:mostraAnexos('<?=$osfilho['odsid']?>',1);"><img border=0 src=../imagens/seta_filho.gif align=absmiddle> [+] Anexos nº os <?=$osfilho['odsid']?></a></div>
													<div style="float:left;display:none" id="divAnexosMenos_<?=$osfilho['odsid']?>"><a href="javascript:mostraAnexos('<?=$osfilho['odsid']?>',2);"><img border=0 src=../imagens/seta_filho.gif align=absmiddle> [-] Anexos nº os <?=$osfilho['odsid']?></a></div>
												</div>
											<? endif; ?>
										</td>
									</tr>
					   		<?}
						
					}?>
				</td>
			</tr>
			<?php if($os['odsid']){?>
			<tr style="display: none" id="tr_anexos_<?=$os['odsid']?>">
				<td id="td_anexos_<?=$os['odsid']?>" class="SubTituloEsquerda" colspan="4">
				</td>
			</tr>
			<?php } ?>
			<?php if($osfilho['odsid']){?>
			<tr style="display: none" id="tr_anexos_<?=$osfilho['odsid']?>">
				<td id="td_anexos_<?=$osfilho['odsid']?>" class="SubTituloEsquerda" colspan="4">
				</td>
			</tr>
			<?php } ?>
			</table>
			<br />
			<?
			}//fecha foreach($oss as $os) {
			
		}

	}//fecha if($oss[0]) {
	?>
	</td>
	<td valign="top" width="5%">
		<?
		$dados_wf = array('scsid' => $_SESSION['fabrica_var']['scsid']);
		wf_desenhaBarraNavegacao(pegarDocidSolicitacaoServico($dados_wf), $dados_wf);
		?>
		<?php if($habil=='S'): ?>
		<table cellspacing="0" cellpadding="3" border="0" style="background-color: rgb(245, 245, 245); border: 2px solid rgb(201, 201, 201); width: 80px;">
		<tbody>
		<tr style="text-align: center;">
			<td style="font-size: 7pt; text-align: center;cursor:pointer;">
			<a style="cursor:pointer;" id="btn_add_os_mec" onclick="addOSMEC()">Detalhar OS da Contratante</a>
			</td>
		</tr>
		</tbody></table>
		<? endif; ?>
	</td>
</tr>
<tr>
	<td colspan="4" class="SubTituloEsquerda">Total de PF: <span style="font-size: 14px;"><?php echo $total_pf; ?></span></td>
</tr>
</table>
</form>