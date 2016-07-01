<?php
if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( "Autorização de Pagamento", "Selecione o ano e o mês.");

$habilitado = "S";
?>

<style>
.link {
	cursor: pointer
}

.bold {
	font-weight: bold
}

.center {
	text-align: center
}
</style>
<script>
function autorizaPagamento()
{
	var erro = 0;
	if(jQuery("#mes").val() && jQuery("#ano").val() && jQuery("#tipo").val() ){
		jQuery("#btn_autorizar").attr("value","Carregando...");
		jQuery("#btn_autorizar").attr("disabled",true);
		jQuery("#formulario").submit();
	}else{
		alert('Favor selecionar o Ano, o Mês e o Tipo.');
	}	
}

</script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<form name="formulario" id="formulario" method="post" action="">
	<input type="hidden" id="requisicao" name="requisicao" value="autorizaPagamento" />
	<input type="hidden" id="classe" name="classe" value="Ws_Tutor" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
		<tr>
			<td width="25%" class="subtituloDireita" >Ano</td>
			<td>
				<?php 
				$ano = date("Y");
				$arrAnos[0]['codigo'] = $ano-1;
				$arrAnos[0]['descricao'] = $ano-1;
				for($i=1;$i<5;$i++)
				{
					$arrAnos[$i]['codigo'] = $ano+$i-1;
					$arrAnos[$i]['descricao'] = $ano+$i-1;
				}
				$db->monta_combo("ano", $arrAnos, $habilitado, "Selecione...", '', '', '', '', 'S','ano');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" >Mês</td>
			<td>
			<?php 
				$mesInicial = "01";
				$mes = date("m");
				for($i=0;$i<12;$i++)
				{
					$arrMes[$i]['codigo'] = $mesInicial+$i;
					$arrMes[$i]['descricao'] = retornaMesMaisMedicos($mesInicial+$i);
				}
				$db->monta_combo("mes", $arrMes, $habilitado, "Selecione...", '', '', '', '', 'S','mes');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" >Tipo</td>
			<td>
			<?php 
				$arrTipo[] = array("codigo"=>"S","descricao"=>"Supervisor");
				$arrTipo[] = array("codigo"=>"T","descricao"=>"Tutor");
				$db->monta_combo("tipo", $arrTipo, $habilitado, "Selecione...", '', '', '', '', 'S','tipo');
			?>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="center"  >
				<input type="button" value="Autorizar" id="btn_autorizar" onclick="autorizaPagamento()" />
			</td>
		</tr>
	</table>
</form>
<script>
jQuery(function() {
	<?php if($_SESSION['maismedicos']['ws_tutor']['alert']): ?>
		alert('<?php echo $_SESSION['maismedicos']['ws_tutor']['alert'] ?>');
		<?php unset($_SESSION['maismedicos']['ws_tutor']['alert']) ?>
	<?php endif; ?>
});
</script>	