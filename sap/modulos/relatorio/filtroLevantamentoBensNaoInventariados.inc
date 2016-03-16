<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/calendario.js'></script>

<?php 
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

//Monta o Título da página abaixo da aba
monta_titulo("Levantamento de Bens Não Inventariados", obrigatorio()." Indica campos obrigatórios.");

//$sql = "SELECT ccbid, ccbdsc, ccbstatus, ccbdata, ccbvidautil, ccbpercresidual FROM sap.contacontabil;"

?>

<script type="text/javascript">
/**
 * Aciona o filtro
 * @name filtrar
 * @param requisicao - Requisição que será executada
 * @return void
 */
function filtrar(requisicao){

	$('requisicao').setValue(requisicao);
	$('formularioPesquisa').submit();

}
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td colspan='2' bgcolor="#e9e9e9" align="center" style=""><b>Responsável</b></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Matricula SIAPE: </td>
			<td class="campo">
				<?=campo_texto('ternum','N','N','',15,10,'','','left','',0,'id="ternum"', '', $ternum);?>				
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Nome: </td>
			<td class="campo">
            </td>
		</tr>
		<tr>
			<td colspan='2' bgcolor="#e9e9e9" align="center" style=""><b>Localização</b></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Unidade: </td>
			<td class="campo"></td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> Endere&ccedil;o: </td>
			<td class="campo">
				<?php echo campo_texto('endlogatual','N','N','',100,200,'','','left','',0,'id="endlogatual"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Andar: </td>
			<td class="campo">
				<?php echo campo_texto('endcidatual','N','N','',100,100,'','','left','',0,'id="endcidatual"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Sala: </td>
			<td class="campo">
				<?php echo campo_texto('endcepatual','N','N','',15,10,'','','left','',0,'id="endcepatual"');?>
			</td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> Endere&ccedil;o: </td>
			<td class="campo">
				<?php echo campo_texto('endlogatual','N','N','',100,200,'','','left','',0,'id="endlogatual"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Andar: </td>
			<td class="campo">
				<?php echo campo_texto('enadescricaoatual','N','N','',15,15,'','','left','',0,'id="enadescricaoatual"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Sala: </td>
			<td class="campo">
				<?php echo campo_texto('easdescricaoatual','N','N','',50,50,'','','left','',0,'id="easdescricaoatual"'); ?>
            </td>
		</tr>

		<tr>
			<td class="SubtituloDireita"> Nº RGP: </td>
			<td class="campo"></td>
		</tr>
		<tr class="buttons">
			<td colspan='2' class="SubTituloCentro" align="center">
				<input type='hidden' name='requisicao' id='requisicao' />
				<input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Gerar Relatório' onclick="filtrar('filtrar')" />
				<input type='reset' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar Campos' />
		    </td>
		</tr>
	</table>
</form>


<?php 
	//$cabecalho		= array('Conta','Descrição','Status','Data','Vida Útil','Percentual');
	//$tamanho		= array("10%","60%","10%","10%","10%","10%");
	//$alinhamento	= array("center","center","center","center","center","center");
	//$db->monta_lista( $sql, $cabecalho, '', '', 'N', 'center', $par2, "", $tamanho, $alinhamento );
?>

<script type="text/javascript">

$("#formularioPesquisa").validate({
	//Define as regras dos campos
	rules:{
			ano:{
				required: true
			},
			co_mes:{
				required: true
			}
	},
	//Define as mensagens de alerta
	messages:{
		ano 	:"Campo Obrigatório",
		co_mes	:"Campo Obrigatório"
	}
});

</script>