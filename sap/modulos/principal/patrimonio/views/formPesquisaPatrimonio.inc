




<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/calendario.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>

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
	
	/**
	 * Buscar Material
	 * @name consultarMaterial
	 * @return void
	 */	
	function consultarMaterial(){
		AbrirPopUp('?modulo=sistema/tabelasdeapoio/material/pesquisarMaterial&acao=A','buscarMaterial','scrollbars=yes,width=900,height=770');
	}


	/**
	 * Limpa os campos de responsável
	 * @name limparBuscaResponsavel
	 * @return void
	 */
	function limparBuscaResponsavel(){
		$('no_servidor').clear();
		$('nu_matricula_siape').clear();
	}



	/**
	 * Abre a tela de pesquisa de endereço
	 * @name pesquisarEndereco
	 * @return void
	 */
	function pesquisarEndereco(){

		var uorsg = $('uorco_uorg_lotacao_servidor').options[$('uorco_uorg_lotacao_servidor').selectedIndex].text;
		var uorco_uorg_lotacao_servidor = $('uorco_uorg_lotacao_servidor').getValue();
		var uorno = $('unidade').getValue();

		AbrirPopUp('?modulo=sistema/tabelasdeapoio/enderecoUnidade/pesquisar&acao=A&topo=N&viaEntradaBens=S&uorsg='+uorsg+'&uorno='+uorno+'&uorco_uorg_lotacao_servidor='+uorco_uorg_lotacao_servidor,'buscarEndereco','scrollbars=yes,width=800,height=770');
	}


	/**
	 * Função responsável por setar os dados de endereço e unidade
	 * @name retornaEndereco
	 * @return void
	 */	
	function retornaEndereco(uendid){
		carregaDadosEndereco(uendid);
	}


</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Tipo de Relatório: </td>
			<td class="campo">
			<input type="radio" name="relatorio" value="analitico" /> Analítico
			<input type="radio" name="relatorio" value="sintetico" /> Sintético
			<input type="radio" name="relatorio" value="mensal" /> Mensal
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Bens com entrada até: </td>
			<td class="campo">
				<?=campo_data2('bendtentrada','N','S','Informe a Data da Entrada','S','','',$bendtentrada);?>
            </td>
		</tr>		

		<tr class="buttons">
			<td colspan='2'>
				<input type='hidden' name='requisicao' id='requisicao' />
				<input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Pesquisar' onclick="filtrar('filtrar')" />
				<input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="javascript: window.location.href = window.location" />
		    </td>
		</tr>
	</table>
</form>
<?php 
include_once APPSAP . 'modulos/principal/patrimonio/views/partialsViews/lista.inc';
?>