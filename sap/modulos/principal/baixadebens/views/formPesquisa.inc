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
	 * @param acao - Ação da pesquisa
	 * @return void
	 */	
	function pesquisar(requisicao){
		$('requisicao').setValue(requisicao);
		$('formularioPesquisa').submit();
	}


	/**
	 * Redireciona para a edição
	 * @name detalharBensBaixa
	 * @param bebid - Id da baixa
	 * @return void
	 */
	function detalharBensBaixa(bebid){
		window.location = "?modulo=principal/baixadebens/editar&acao=A&bebid="+bebid;
	}

	function detalharBensBaixaEmAberto(bebid){
		window.location = "?modulo=principal/baixadebens/editarBaixaAberto&acao=A&bebid="+bebid;
	}


	/**
	 * Redireciona para a exclusão
	 * @name removerBensBaixa
	 * @param bebid - Id da baixa
	 * @return void
	 */
	function removerBensBaixa(bebid){
		window.location = "?modulo=principal/baixadebens/excluir&acao=A&bebid="+bebid;
	}

	/**
	 * Limpa pesquisa
	 * @name limpar
	 * @return void
	 */
	function limpar(tipo){
		if(tipo == 'I'){
			window.location = "?modulo=principal/baixadebens/processosAbertos&acao=A";
		}
		else if(tipo == 'A'){
			window.location = "?modulo=principal/baixadebens/pesquisar&acao=A";
		}		
	}

</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Motivo da Baixa: </td>
			<td class="campo">
				<?php $oTipoEntradaSaida->montaComboTipoEntradaSaida($tesid,'S','N'); ?>				
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data da Baixa: </td>
			<td class="campo">
				<?php echo campo_data2('bebdata','N','S','Informe a Data da Baixa','N'); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Processo: </td>
			<td class="campo">
				<?php echo campo_texto('bebnumprocesso','N','S','Informe o Número do Processo',20,20,'#####.######.####-##','','left','',0,'id="bebnumprocesso"'); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data do Processo: </td>
			<td class="campo">
				<?php echo campo_data2('bebdataprocesso','N','S','Informe a Data do Processo','N'); ?>
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<input type='hidden' name='requisicao' id='requisicao' />
            	<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Filtrar' onclick="pesquisar('filtrar');">
                <input type='reset' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="limpar('<?php echo $filtroBenStatus; ?>');">
            </td>
        </tr>
	</table>
</form>
<?php 
if(!empty($_GET['bebid'])){
?>	
	<script>
	var altura = screen.availHeight;
	var largura = screen.availWidth;
	var params = 'scrollbars=yes, width='+largura+', height='+altura+', top=0, left=0';
	
	window.location='?modulo=principal/baixadebens/pesquisar&acao=A';
	AbrirPopUp('?modulo=principal/baixadebens/visualizarTermo&acao=A&bebid=<?php echo $_GET["bebid"]; ?>','visualizaBaixa',params)
	</script>
<?php 
}
?>
<?php include_once APPSAP . 'modulos/principal/baixadebens/views/partialsViews/lista.inc'; ?>
