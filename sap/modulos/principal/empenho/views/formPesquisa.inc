<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/calendario.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Função que dispara a pesquisa no banco
	 * @name pesquisar
	 * @param requisicao - Requisição que será executada
	 * @return void
	 */
	function pesquisar(requisicao){
		$('requisicao').setValue(requisicao);
		$('formularioPesquisa').submit();
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
	 * Limpa pesquisa
	 * @name limpar
	 * @return void
	 */
	function limpar(){
		window.location = '?modulo=principal/empenho/pesquisar&acao=A';
	}

</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Empenho: </td>
			<td class="campo">
				<?=campo_texto('empnumero','N','S','Informe o Número do Empenho',12,12,'############','','left','',0,'id="empnumero"','',null,"verificaSomenteNumeros(this,'O Número do Empenho deve ser numérico!');");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data do Empenho: </td>
			<td class="campo">
				<?=campo_data2('empdata','N','S','Informe a Data do Empenho','N');?>
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' />
            	<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Filtrar' onclick="javascript:pesquisar('filtrar');">
                <input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick='javascript:limpar();'>
            </td>
        </tr>
	</table>
</form>

<?php include_once APPSAP . 'modulos/principal/empenho/views/partialsViews/lista.inc'; ?>
