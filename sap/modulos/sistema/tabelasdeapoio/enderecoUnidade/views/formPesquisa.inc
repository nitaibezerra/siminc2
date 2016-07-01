<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript'>

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
	 * Recarrega os endereços na página após selecionar uma unidade
	 * @name recarregaEnderecos
	 * @param uorco_uorg_lotacao_servidor - Chave da unidade organizacional
	 * @param uorsg - Sigla da unidade organizacional
	 * @param uorno - Nome da unidade organizacional
	 * @return void
	 */
	function recarregaEnderecos(uorco_uorg_lotacao_servidor,uorsg,uorno){
		$('uorco_uorg_lotacao_servidor').setValue(uorco_uorg_lotacao_servidor);
		$('uorsg').setValue(uorsg);
		$('uorno').setValue(uorno);
		filtrar('naofiltrar');
	}


	/**
	 * Limpa os campos do form
	 * @name limpaFiltros
	 * @return void
	 */
	function limpaFiltros(){
		 var topo = $('topo').getValue();
		 var viaEntradaBens = $('viaEntradaBens').getValue();
		 window.location='?modulo=sistema/tabelasdeapoio/enderecoUnidade/pesquisar&acao=A&topo='+topo+'&viaEntradaBens='+viaEntradaBens;
	}


	 /**
	  * Direciona para a tela de adição de endereço à unidade
	  * @name novoEndereco
	  * @return void
	  */
	function novoEndereco(){
		var uorsg = $('uorsg').getValue();
		var uorno = $('uorno').getValue();
		var uorco_uorg_lotacao_servidor = $('uorco_uorg_lotacao_servidor').getValue();
		var viaEntradaBens = $('viaEntradaBens').getValue();
		var topo = $('topo').getValue();

		if(uorsg == '' || uorno == '' || uorco_uorg_lotacao_servidor == ''){
			alert('Selecione uma unidade para cadastrar um endereço');
			return false;
		}
		else{
			window.location='?modulo=sistema/tabelasdeapoio/enderecoUnidade/adicionar&acao=A&uorco_uorg_lotacao_servidor='+uorco_uorg_lotacao_servidor+'&uorsg='+uorsg+'&uorno='+uorno+'&viaEntradaBens='+viaEntradaBens+'&topo='+topo;
		}
	}

</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Sigla: </td>
			<td class="campo">
				<?=campo_texto('uorsg', 'N', 'S', 'Informe a Sigla da Unidade', 50, 50, '', '', '', '', '', 'id="uorsg"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Unidade: </td>
			<td class="campo">
				<?=campo_texto('uorno', 'N', 'S', 'Informe a Descrição da Unidade', 100, 150, '', '', '', '', '', 'id="uorno"');?>
            </td>
		</tr>
		<tr class="buttons">
			<td colspan='2'>
				<input type='hidden' name='requisicao' id='requisicao' />
				<input type='hidden' name='uorco_uorg_lotacao_servidor' id='uorco_uorg_lotacao_servidor' value='<?php echo $uorco_uorg_lotacao_servidor; ?>' />
				<input type='hidden' name='viaEntradaBens' id='viaEntradaBens' value='<?php echo $_GET['viaEntradaBens']; ?>' />
				<input type='hidden' name='topo' id='topo' value='<?php echo $_GET['topo']; ?>' />
				<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Pesquisar' onclick="javascript:filtrar('filtrar');" />
				<input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="javascript:limpaFiltros();" />
		    </td>
		</tr>
	</table>
</form>
<?php 
if(!empty($_GET['uorsg']) && !empty($_GET['uorno']) && !empty($_GET['uorco_uorg_lotacao_servidor']) && $_POST['requisicao'] != 'naofiltrar'){
	echo "<script>recarregaEnderecos('".$_GET['uorco_uorg_lotacao_servidor']."','".$_GET['uorsg']."','".$_GET['uorno']."')</script>";
}
?>
<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/enderecoUnidade/views/partialsViews/listaPesquisaUnidade.inc'; ?>
