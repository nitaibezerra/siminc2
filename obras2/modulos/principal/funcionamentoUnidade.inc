<?php
//Regra para usar Obra ou Objeto no Funcionamento da Unidade
if($GET['acao'] == "A"){//Se for Obras, a ação é "A", e o campo é empid
	$empid = $_SESSION['obras2']['empid'];
	$obrid = NULL;
	unset($_SESSION['obras2']['obrid']);
}elseif($GET['acao'] == "O"){ // Se for Objeto, a ação é "O", e o campo é obrid
	$obrid = $_SESSION['obras2']['obrid'];
	$empid = NULL;
	unset($_SESSION['obras2']['empid']);
}

if($_POST['requisicao']){
	$tecnico = new FuncionamentoUnidade();
	if($tecnico->$_POST['requisicao']()){
		$_SESSION['obras2']['funcionamentounidade']['alert'] = "Operação realizada com sucesso!";
		$tecnico->commit();
	}else{
		$_SESSION['obras2']['funcionamentounidade']['alert'] = "Não foi possivel realizar a operação!";
	}
	header("Location: obras2.php?modulo=principal/funcionamentoUnidade&acao=A");
	exit;
}
if($_POST['funid'] && !$_POST['requisicao']){
	$tecnico = new FuncionamentoUnidade($_POST['funid']);
	extract($tecnico->getDados());
	$resultado = $tecnico->getDados();
	if($obrid){
		$obra = new Obras($obrid);
		$resultado['funpercexecutado'] = $obra->obrpercentultvistoria;
	}
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

if ( $_GET['acao'] == 'A' ){
	$db->cria_aba(ID_ABA_EMP_CADASTRADO,$url,$parametros);
	$msgTitle 		  = (!$eorid ? 'Salve o orçamento para a obra e então será liberado o cadastro de detalhamento do orçamento' : '');
	$displayEmpenhado = (!$eorid ? 'none' : '');
	$displayLiquidado = (!$eorid ? 'none' : '');
}else{
	if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
	}else{
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
	}
	$msgTitle 		  = (!$eorid ? 'Salve o orçamento para o objeto e então será liberado o cadastro de detalhamento do orçamento' : '');
	$displayEmpenhado = (!$eorid ? 'none' : '');
	$displayLiquidado = (!$eorid ? 'none' : '');
}
if($empid){ //Cabeçaho Obras
	$empreendimento = new Empreendimento( $empid );
	$empreendimento->montaCabecalho();
}
if($obrid){ //Cabeçalho Objeto
	echo cabecalhoObra($obrid);
}

monta_titulo("Funcionamento da Unidade", '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );

$somenteLeitura = "S";
$habilitado = true;
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<style>
	.link{cursor:pointer}
</style>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript">

jQuery(document).ready(function()
{
		<?php if($_SESSION['obras2']['funcionamentounidade']['alert']): ?>
			alert('<?php echo $_SESSION['obras2']['funcionamentounidade']['alert'] ?>');
			<?php unset($_SESSION['obras2']['funcionamentounidade']['alert']) ?>
		<?php endif; ?>
		
		jQuery('#salvar').click(function(){
			var erro = true;
			jQuery('[name="funtipofuncionamento"]').each(function(){
				if( jQuery(this).attr('checked') ){
					erro = false;
				}
			});
			if( erro ){
				alert('Favor escoler um tipo.');
				jQuery('[name="funtipofuncionamento"]').focus();
				return false;
			}
			var erro = 0;
			jQuery("[class~=obrigatorio]").each(function() { 
				if(!this.value || this.value == "Selecione..."){
					erro = 1;
					alert('Favor preencher todos os campos obrigatórios!');
					this.focus();
					return false;
				}
			});
			if(erro == 0){
				jQuery('#requisicao').val('salvar');
				jQuery('#formulario').submit();
			}
		});
});

function editarFuncionamentoUnidade(funid)
{
	jQuery('#requisicao').val('');
	jQuery('#funid').val(funid);
	jQuery('#formulario').submit();
}

function excluirFuncionamentoUnidade(funid)
{
	if(confirm('Deseja realmente excluir?')){
		jQuery('#funid').val(funid);
		jQuery('#requisicao').val('excluirFuncionamentoUnidade');
		jQuery('#formulario').submit();
	}
}

</script>
<center><h1>Esta tela ainda não está pronta! Faltam algumas regras e campos para salvar no banco de dados.</h1></center>
<form name="formulario" id="formulario" method="post" > 
	<input type="hidden" name="requisicao" id="requisicao" value=""/> 
	<input type="hidden" name="funid" id="funid" value="<?=$funid?>"/>
	<input type="hidden" name="obrid" id="obrid" value="<?=$obrid?>"/> 
	<input type="hidden" name="empid" id="empid" value="<?=$empid?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita" colspan="2"><center><b>Tipo:</b></center></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="25%"></td>
			<td>
				<? $funtipofuncionamento = $resultado['funtipofuncionamento']; ?>
				<input type="radio" name="funtipofuncionamento" value="A" <?=$funtipofuncionamento == 'A' ? 'checked' : '' ?>/> Declaração do município do módulo Proinfancia Manutenção<br>
				<input type="radio" name="funtipofuncionamento" value="B" <?=$funtipofuncionamento == 'B' ? 'checked' : '' ?>/> Informações cadastradas pelo FNDE<br>
				<input type="radio" name="funtipofuncionamento" value="C" <?=$funtipofuncionamento == 'C' ? 'checked' : '' ?>/> Informações alimentadas pelo município<br>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2"><center><b>Detalhamento:</b></center></td>
		</tr>
		<?php if( $_REQUEST['funid'] ){?>
		<tr>
			<td class="SubTituloDireita">% de Execução: </td>
			<td>
				<?=number_format($resultado['funpercexecutado'],2,',','.'); ?> %
			</td>
		</tr>
		<?php }?>
		<tr>
			<td class="SubTituloDireita">Data de conclusão de obra: </td>
			<td>
				<? $fundtconclusaoobra = $resultado['fundtconclusaoobra']; ?>
				<?= campo_data2( 'fundtconclusaoobra', 'S', $somenteLeitura, '', 'S', '', "" ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Inauguração c/ representante MEC: </td>
			<td>
				<? $funinaugrepmec = $resultado['funinaugrepmec']; ?>
				<input type="radio" name="funinaugrepmec" value="TRUE" <?=$funinaugrepmec == 't' ? 'checked' : '' ?>/> Sim
				<input type="radio" name="funinaugrepmec" value="FALSE" <?=$funinaugrepmec == 't' ? '' : 'checked' ?>/> Não
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Convênio de Imobiliário: </td>
			<td>
				<? $funconveniomob = $resultado['funconveniomob']; ?>
				<input type="radio" name="funconveniomob" value="TRUE" <?=$funconveniomob == 't' ? 'checked' : '' ?>/> Sim
				<input type="radio" name="funconveniomob" value="FALSE" <?=$funconveniomob == 't' ? '' : 'checked' ?>/> Não
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Mobiliário já Adquirido? </td>
			<td>
				<? $funmobadquirido = $resultado['funmobadquirido'];	?>
				<input type="radio" name="funmobadquirido" value="TRUE" <?=$funmobadquirido == 't' ? 'checked' : '' ?>/> Sim
				<input type="radio" name="funmobadquirido" value="FALSE" <?=$funmobadquirido == 't' ? '' : 'checked' ?>/> Não
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Está em funcionamento? </td>
			<td>
				<? $funundfuncionamento = $resultado['funundfuncionamento']; ?>
				<input type="radio" name="funundfuncionamento" value="TRUE" <?=$funundfuncionamento == 't' ? 'checked' : '' ?>/> Sim
				<input type="radio" name="funundfuncionamento" value="FALSE" <?=$funundfuncionamento == 't' ? '' : 'checked' ?>/> Não
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data Início de Funcionamento: </td>
			<td>
				<? $funundtfuncionamento = $resultado['funundtfuncionamento'];	?>
				<?= campo_data2( 'funundtfuncionamento', 'S', $somenteLeitura, '', 'S', '', "" ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Previsão de Inauguração: </td>
			<td>
				<? $fununddtprevinauguracao = $resultado['fununddtprevinauguracao'];	?>
				<?= campo_data2( 'fununddtprevinauguracao', 'S', $somenteLeitura, '', 'S', '', "" ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Previsão de Início de Funcionamento: </td>
			<td>
				<? $fundtprevfuncionamento = $resultado['fundtprevfuncionamento'];	?>
				<?= campo_data2( 'fundtprevfuncionamento', 'S', $somenteLeitura, '', 'S', '', "" ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Crianças atendidas:</td>
			<td> 
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
					<tr>
						<td colspan="4" style="border-bottom:1px solid #CCCCCC;"> <center> Efetivo </center> </td>
						<td colspan="4" style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Previsão </center> </td>
					</tr>
					<tr>
						<td colspan="2" style="border-bottom:1px solid #CCCCCC;"> <center> Creche </center> </td>
						<td colspan="2" style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Pre-Escola </center> </td>
						<td colspan="2" style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Creche </center> </td>
						<td colspan="2" style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Pre-Escola </center> </td>
					</tr>
					<tr>
						<td style="border-bottom:1px solid #CCCCCC;"> <center> Parcial </center> </td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Integral </center> </td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Parcial </center> </td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Integral </center> </td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Parcial </center> </td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Integral </center> </td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Parcial </center> </td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;"> <center> Integral </center> </td>
					</tr>
					<tr>
						<td style="border-bottom:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancacrhparcialefetivo = number_format($resultado['funqtdcriancacrhparcialefetivo'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancacrhparcialefetivo', 'N', $somenteLeitura, '', 17, 14, '[.###]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancacrhintegralefetivo = number_format($resultado['funqtdcriancacrhintegralefetivo'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancacrhintegralefetivo', 'N', $somenteLeitura, '', 17, 14, '[.###]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancapreescparefetivo = number_format($resultado['funqtdcriancapreescparefetivo'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancapreescparefetivo', 'N', $somenteLeitura, '', 17, 14, '[.###]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancapreescintegfetivo = number_format($resultado['funqtdcriancapreescintegfetivo'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancapreescintegfetivo', 'N', $somenteLeitura, '', 17, 14, '[.###]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancacrhparcialprevisao = number_format($resultado['funqtdcriancacrhparcialprevisao'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancacrhparcialprevisao', 'N', $somenteLeitura, '', 17, 14, '[.###]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancacrhintprevisao = number_format($resultado['funqtdcriancacrhintprevisao'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancacrhintprevisao', 'N', $somenteLeitura, '', 17, 14, '[.###]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancapreescparprevisao = number_format($resultado['funqtdcriancapreescparprevisao'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancapreescparprevisao', 'N', $somenteLeitura, '', 17, 14, '[.###]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancapreescintprevisao = number_format($resultado['funqtdcriancapreescintprevisao'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancapreescintprevisao', 'N', $somenteLeitura, '', 17, 14, '[.###]',  '', 'left', '', 0); ?>
							</center>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<div style="float: left;">
					<?php if ($habilitado){ ?>
						<?php if( $_REQUEST['funid'] ){?>
							<input type="button" value="Salvar Alterações" style="cursor: pointer" id="salvar">
							<input type="button" value="Novo" style="cursor: pointer" onclick="window.location=window.location">
						<?php }else{?>
							<input type="button" value="Salvar" style="cursor: pointer" id="salvar">
						<?php }?>
					<?php } ?> 
				</div>
			</td>
		</tr>
	</table>
</form>
<?php
if(!$tecnico){
	$tecnico = new FuncionamentoUnidade();
}
$tecnico->listarFuncionamentoUnidade();
?>	