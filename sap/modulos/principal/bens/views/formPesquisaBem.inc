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
	 * @param requisicao - Requisição que sera executada
	 * @return void
	 */
	function pesquisar(requisicao){
		$('requisicao').setValue(requisicao);
		$('formularioPesquisa').submit();
	}


	/**
	 * Redireciona para a edição
	 * @name detalharBem
	 * @param benid - Id do bem
	 * @return void
	 */
	function detalharBem(benid){
		window.location = "?modulo=principal/bens/editarBem&acao=A&benid="+benid;
	}

	 function detalharBemEmAberto(benid){
	     window.location = "?modulo=principal/bens/editarBemAberto&acao=A&benid="+benid;
	 }


	/**
	 * Redireciona para a exclusão
	 * @name removerBem
	 * @param benid - Id do bem
	 * @return void
	 */
	function removerBem(benid){
		window.location = "?modulo=principal/bens/excluirBem&acao=A&benid="+benid;
	}

	/**
	 * Limpa pesquisa
	 * @name limpar
	 * @return void
	 */
	function limpar(){
		window.location = '?modulo=principal/bens/pesquisarBem&acao=A';
	}

</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Tipo de Entrada: </td>
			<td class="campo">
				<?php $oTipoEntradaSaida->montaComboTipoEntradaSaida('','E','N');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data da Entrada: </td>
			<td class="campo">
				<?=campo_data2('bendtentrada','N','S','Informe a Data de Entrada','N');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Processo: </td>
			<td class="campo">
				<?=campo_texto('bennumproc','N','S','Informe o Número do Processo',20,20,'#####.######.####-##','','left','',0,'id="bennumproc"','',null,"verificaSomenteNumerosCpfCnpj(this,'O Número do Processo deve ser numérico!');");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data do Processo: </td>
			<td class="campo">
				<?=campo_data2('bendtproc','N','S','Informe a Data do Processo','N');?>
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<input type='hidden' name='requisicao' id='requisicao' />
            	<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Filtrar' onclick="pesquisar('filtrar');" />
                <input type='reset' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="limpar()" />
            </td>
        </tr>
	</table>
</form>
<?php 
if(!empty($_GET['benid']) && !empty($_GET['bebid'])){
?>	

<script>
var largura = screen.availWidth;
var altura = screen.availHeight;
var params = 'scrollbars=yes, width='+largura+', height='+altura+', left=0, top=0';

window.location='?modulo=principal/bens/pesquisarBem&acao=A';
AbrirPopUp('?modulo=principal/bens/visualizarTermo&acao=A&benid=<?php echo $_GET["benid"]; ?>&bebid=<?php echo $_GET["bebid"]; ?>','visualizaEntrada',params)
AbrirPopUp('?modulo=principal/baixadebens/visualizarTermo&acao=A&bebid=<?php echo $_GET["bebid"]; ?>','visualizaBaixa',params)
</script>

<?php 
}else if(!empty($_GET['benid'])){
?>
<script>
var largura = screen.availWidth;
var altura = screen.availHeight;
var params = 'scrollbars=yes, width='+largura+', height='+altura+', left=0, top=0';

window.location='?modulo=principal/bens/pesquisarBem&acao=A';
AbrirPopUp('?modulo=principal/bens/visualizarTermo&acao=A&benid=<?php echo $_GET["benid"]; ?>','visualizaEntrada',params)
</script>
<?php 
}
?>
<?php include_once APPSAP . 'modulos/principal/bens/views/partialsViews/listaBem.inc'; ?>
