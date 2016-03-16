<?php

$sql = "SELECT distinct
			true
		FROM
			obras.supervisao s
		INNER JOIN obras.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
		INNER JOIN obras.obrainfraestrutura oi ON oi.obrid = s.obrid		
		WHERE
			s.obrid = " . $_SESSION["obra"]["obrid"] . " AND
			s.supstatus = 'A'
			AND rs.rsuid = 1 
			AND obrpercexec >= 80";

$percVist = $db->pegaUm($sql);
$arMnuid = array();	
//esconde aba Diligencia
if( $_SESSION["obra"]["orgid"] != 3 ){ 
	echo "<script>window.history.back();</script>";
};
if( $percVist != true ){
	echo "<script>window.history.back(-1);</script>";
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
include APPRAIZ . 'www/cte/_funcoes.php';

$obras = new Obras();
$dobras = new DadosObra(null);
$funcionamento = new FuncionamentoUnidade();

if( $_REQUEST['req'] ){
	$funcionamento->$_REQUEST['req']($_REQUEST);
}

if($_REQUEST['funid']){
	$resultado = $funcionamento->buscaEditar($_REQUEST[funid]);	
	$dados = $funcionamento->dados($resultado);
}elseif($_SESSION['obra']['obrid'] && $funcionamento->testa($_SESSION['obra']['obrid'])=='t'){
	$resultado = $funcionamento->busca($_SESSION['obra']['obrid']);	
	$dados = $funcionamento->dados($resultado);
}

?>

<br/>

<?php

montaAbaObras($abacod_tela,$url,$parametros);

$titulo_modulo = "Funcionamento da Unidade";
monta_titulo( $titulo_modulo, "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");

echo $obras->CabecalhoObras();

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript">

jQuery(document).ready(function()
{
		jQuery('#salvar').click(function(){
			var erro = true;
			jQuery('[name="funtipofuncionamento"]').each(function(){
				if( jQuery(this).attr('checked') ){
					erro = false;
				}
			});
			if( erro ){
				alert('Escolha um tipo.');
				jQuery('[name="funtipofuncionamento"]').focus();
				return false;
			}
			jQuery('#req').val('salvar');
			jQuery('#formulario').submit();
		});
});

</script>
<form name="formulario" id="formulario" method="post" > 
	<input type="hidden" name="req" id="req" value="executar"/> 
	<input type="hidden" name="obrid" value="<?=$_SESSION["obra"]["obrid"] ?>"/> 
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
								<?= campo_texto( 'funqtdcriancacrhparcialefetivo', 'N', $somenteLeitura, '', 17, 14, '[#]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancacrhintegralefetivo = number_format($resultado['funqtdcriancacrhintegralefetivo'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancacrhintegralefetivo', 'N', $somenteLeitura, '', 17, 14, '[#]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancapreescparefetivo = number_format($resultado['funqtdcriancapreescparefetivo'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancapreescparefetivo', 'N', $somenteLeitura, '', 17, 14, '[#]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancapreescintegfetivo = number_format($resultado['funqtdcriancapreescintegfetivo'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancapreescintegfetivo', 'N', $somenteLeitura, '', 17, 14, '[#]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancacrhparcialprevisao = number_format($resultado['funqtdcriancacrhparcialprevisao'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancacrhparcialprevisao', 'N', $somenteLeitura, '', 17, 14, '[#]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancacrhintprevisao = number_format($resultado['funqtdcriancacrhintprevisao'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancacrhintprevisao', 'N', $somenteLeitura, '', 17, 14, '[#]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancapreescparprevisao = number_format($resultado['funqtdcriancapreescparprevisao'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancapreescparprevisao', 'N', $somenteLeitura, '', 17, 14, '[#]',  '', 'left', '', 0); ?>
							</center>
						</td>
						<td style="border-bottom:1px solid #CCCCCC;border-left:1px solid #CCCCCC;">
							<center>
								<? $funqtdcriancapreescintprevisao = number_format($resultado['funqtdcriancapreescintprevisao'],0,',','.');	?>
								<?= campo_texto( 'funqtdcriancapreescintprevisao', 'N', $somenteLeitura, '', 17, 14, '[#]',  '', 'left', '', 0); ?>
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
							<input type="button" value="Novo" style="cursor: pointer" onclick="window.location='obras.php?modulo=principal/funcionamento_unidade&acao=A'">
						<?php }else{?>
							<input type="button" value="Salvar" style="cursor: pointer" id="salvar">
						<?php }?>
					<?php } ?> 
					<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
				</div>
			</td>
		</tr>
	</table>
</form>	
<?=$funcionamento->lista($_SESSION['obra']['obrid']);	 ?>