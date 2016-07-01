<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>


<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/calendario.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script type="text/javascript">

	/**
	 * Aciona o filtro
	 * @name filtrar
	 * @param requisicao - Requisição que será executada
	 * @return boolean
	 */	
	function filtrar(requisicao){
		$('requisicao').setValue(requisicao);
		$('formularioPesquisa').submit();
	}

	/**
	 * Abre o cadastro de empenho
	 * @name cadastrarEmpenho
	 * @return void
	 */	
	function cadastrarEmpenho(){
		window.location='?modulo=principal/empenho/adicionarPopup&acao=A&popup=S';
	}

	/**
	 * Limpa pesquisa
	 * @name limpar
	 * @return void
	 */	
	function limpar(){
		var popup = $('popup').getValue();
		window.location='?modulo=principal/empenho/pesquisarEmpenho&acao=A&popup='+popup;
	}

	/**
	 * Redireciona para a edição
	 * @name detalharEmpenho
	 * @param empid - Id do empenho
	 * @return void
	 */
	function detalharEmpenho(empid){
		window.location = "?modulo=principal/empenho/editarPopup&acao=A&empid="+empid+"&popup=S";
	}


	/**
	 * Redireciona para a exclusão
	 * @name removerEmpenho
	 * @param empid - Id do empenho
	 * @return void
	 */
	function removerEmpenho(empid){
		window.location = "?modulo=principal/empenho/excluir&acao=A&empid="+empid+"&popup=S";
	}
	
</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		
		<tr>
			<td class="SubtituloDireita"> Número do Empenho: </td>
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
			<td colspan='2'>
				<input type='hidden' name='requisicao' id='requisicao' />
				<input type='hidden' name='popup' id='popup' value='<?php echo $_GET['popup']; ?>' />
				<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Filtrar' onclick="javascript:filtrar('filtrar');" />
				<input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="javascript:limpar();" />
				<input type='button' class="botao" name='btnNovo' id='btnNovo' value='Novo' onclick="javascript:cadastrarEmpenho();" />
		    </td>
		</tr>
	</table>
</form>

<?php include_once APPSAP . 'modulos/principal/empenho/views/partialsViews/listaPesquisaEmpenho.inc'; ?>
