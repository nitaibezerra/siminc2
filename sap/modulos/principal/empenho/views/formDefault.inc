<link type="text/css" rel="stylesheet" href="./css/default.css" />

<?php if($_GET['popup'] == 'S'){ ?>
<link type="text/css" rel="stylesheet" href="../includes/Estilo.css" />
<?php } ?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/calendario.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>


	/**
	 * Submete o formulário
	 * @name gravarEmpenho
	 * @return void
	 */
	function gravarEmpenho(){
		$('btnSalvar').disable();
		if(validaFormulario()){
			$('formularioCadastro').submit();
		}
	}


	/**
	 * Valida os campos obrigatórios
	 * @name validaFormulario
	 * @return bool
	 */
	function validaFormulario(){
		
		if($('empnumero').getValue() == '') {
			alert('O campo "Número do Empenho" é obrigatório!');
			$('empnumero').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('empdata').getValue() == '') {
			alert('O campo "Data do Empenho" é obrigatório!');
			$('empdata').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('empvalorper').getValue() == '0,00') {
			alert('O valor do empenho permanente não pode ser zero!');
			$('btnSalvar').enable();
	        return false;
		}

		return true;

	}


	/**
	 * Redireciona para a edição
	 * @name detalharEmpenho
	 * @param empid - Id do empenho
	 * @return void
	 */
	function detalharEmpenho(empid){
		window.location = "?modulo=principal/empenho/editar&acao=A&empid="+empid;
	}


	/**
	 * Redireciona para a exclusão
	 * @name removerEmpenho
	 * @param empid - Id do empenho
	 * @return void
	 */
	function removerEmpenho(empid){
		window.location = "?modulo=principal/empenho/excluir&acao=A&empid="+empid;
	}

	/**
	 * Cancela a operação
	 * @name cancelar
	 * @param popup - Indica se a tela foi chamada da entrada de bens(popup == 'S')
	 * @return void
	 */
	function cancelar(popup){
		if(popup == 'S'){
			executarScriptPai('limparBuscaEmpenho()');
			this.close();
		}
		else{
			window.location = '?modulo=principal/empenho/adicionar&acao=A';
		}
	}


	/**
	 * Função que faz a soma dos valores permanente e consumo
	 * @name calculaTotal
	 * @return void
	 */
	function calculaTotal(){

		//instancia o objeto
		var obCal = new Calculo();

		//pega os valores a serem somados
		var empvalorper = $('empvalorper').getValue();
		var empvalortotal = $('empvalortotal').getValue();

		//calcula a soma dos valores
		var total = obCal.operacao(empvalorper, empvalortotal, '+');

		//seta o total no campo correspondente
		$('total').setValue(total);
		
	}

</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">

		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Empenho: </td>
			<td class="campo">
				<?=campo_texto('empnumero','S','S','Informe o Número do Empenho',12,12,'############','','left','',0,'id="empnumero"','',null,"verificaSomenteNumeros(this,'O Número do Empenho deve ser numérico!');");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data do Empenho: </td>
			<td class="campo">
				<?=campo_data2('empdata','S','S','Informe a Data do Empenho','N','','',$empdata);?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Valor Para Material Permanente: </td>
			<td class="campo">
				<?=campo_texto('empvalorper','N',($emUso) ? 'N' : 'S','Informe o Valor Para Material Permanente',17,14,'[###.]###,##','','left','',0,'id="empvalorper" onkeypress="return somenteNumeros(event);"','',(!empty($empvalorper))?number_format($empvalorper, 2, ',', '.'):'',"verificaSomenteNumerosMonetario(this,'O Valor Total Para Material Permanente deve ser numérico!');verificaTamanho(this,'O Valor Total Para Material Permanente excedeu o tamanho');calculaTotal();");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Valor Para Bens de Consumo: </td>
			<td class="campo">
				<?=campo_texto('empvalortotal','N',($emUso) ? 'N' : 'S','Informe o Valor Para Bens de Consumo',17,14,'[###.]###,##','','left','',0,'id="empvalortotal" onkeypress="return somenteNumeros(event);"','',(!empty($empvalortotal))?number_format($empvalortotal, 2, ',', '.'):'',"verificaSomenteNumerosMonetario(this,'O Valor Total Para Consumo deve ser numérico!');verificaTamanho(this,'O Valor Total Para Consumo excedeu o tamanho');calculaTotal();");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Valor Total: </td>
			<td class="campo">
				<?=campo_texto('total','N','N','',17,17,'[###.]###,##','','left','',0,'id="total"','',number_format($total, 2, ',', '.'));?>
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='empid' id='empid' value='<?php echo $_GET['empid']; ?>'>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:gravarEmpenho();">
                <input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick="javascript:cancelar('<?php echo $_GET['popup']; ?>');">
            </td>
        </tr>
	</table>
</form>

<?php 
if($_GET['popup'] != 'S'){
	include_once APPSAP . 'modulos/principal/empenho/views/partialsViews/lista.inc';
} 
?>