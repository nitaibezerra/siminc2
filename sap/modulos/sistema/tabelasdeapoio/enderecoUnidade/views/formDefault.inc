<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Submete o formulário
	 * @name gravarEndereco
	 * @return void
	 */
	function gravarEndereco(){
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

		if($('endid').getValue() == '') {
			alert('O campo "Endereço" é obrigatório!');
			$('endid').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('enaid').getValue() == '') {
			alert('O campo "Andar" é obrigatório!');
			$('enaid').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('easid').getValue() == '') {
			alert('O campo "Sala" é obrigatório!');
			$('easid').focus();
			$('btnSalvar').enable();
	        return false;
		}
	
		return true;

	}


	/**
	 * Buscar Unidade
	 * @name buscarUnidade
	 * @return void
	 */	
	function buscarUnidade(){
		var topo = $('topo').getValue();
		var viaEntradaBens = $('viaEntradaBens').getValue();
		window.location = '?modulo=sistema/tabelasdeapoio/enderecoUnidade/pesquisar&acao=A&topo='+topo+'&viaEntradaBens='+viaEntradaBens;
	}

	/**
	 * Cancela a operação e retorna à listagem
	 * @name cancela
	 * @return void
	 */
	function cancela(){
		var topo = $('topo').getValue();
		var viaEntradaBens = $('viaEntradaBens').getValue();
		var uorco_uorg_lotacao_servidor = $('uorco_uorg_lotacao_servidor').getValue();
		window.location = '?modulo=sistema/tabelasdeapoio/enderecoUnidade/pesquisar&acao=A&topo='+topo+'&viaEntradaBens='+viaEntradaBens+'&uorco_uorg_lotacao_servidor='+uorco_uorg_lotacao_servidor;
	}

</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Sigla: </td>
			<td class="campo">
				<?php echo campo_texto('uorsg', 'N', 'N', '', 50, 50, '', '', '', '', '', 'id="uorsg"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Unidade: </td>
			<td class="campo">
				<?php echo campo_texto('uorno', 'N', 'N', '', 100, 150, '', '', '', '', '', 'id="uorno"');?>
            </td>
		</tr>
		<tr class="buttons">
			<td colspan='2'>
				<input type='button' class="botao" name='btnBuscarUnidade' id='btnBuscarUnidade' value='Selecionar Outra Unidade' onclick="javascript:buscarUnidade();" />
		    </td>
		</tr>
	</table>
	
	<div class="tituloLista"> Dados do Endere&ccedil;o </div>
	
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		
		<tr>
			<td class="SubtituloDireita"> Edif&iacute;cio: </td>
			<td class="campo">
				<?php $oEnderecoUnidade->montaComboEndereco();?> 
			</td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> Cep: </td>
			<td class="campo">
				<?php echo campo_texto('endcep', 'N', 'N', '', 15, 10, '##.###-###', '', '', '', '', 'id="endcep"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Logradouro: </td>
			<td class="campo">
				<?php echo campo_texto('endlog', 'N', 'N', '', 100, 100, '', '', '', '', '', 'id="endlog"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Complemento: </td>
			<td class="campo">
				<?php echo campo_texto('endcom', 'N', 'N', '', 100, 100, '', '', '', '', '', 'id="endcom"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero: </td>
			<td class="campo">
				<?php echo campo_texto('endnum', 'N', 'N', '', 4, 4, '', '', '', '', '', 'id="endnum"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Bairro: </td>
			<td class="campo">
				<?php echo campo_texto('endbairro', 'N', 'N', '', 100, 100, '', '', '', '', '', 'id="endbairro"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Cidade: </td>
			<td class="campo">
				<?php echo campo_texto('endcid', 'N', 'N', '', 100, 100, '', '', '', '', '', 'id="endcid"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> UF: </td>
			<td class="campo">
				<?php echo campo_texto('enduf', 'N', 'N', '', 4, 4, '', '', '', '', '', 'id="enduf"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Andar: </td>
			<td class="campo">
				<div id='andar'>
				<?php $oEnderecoUnidade->montaComboAndar($endid);?> 
				</div>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Sala: </td>
			<td class="campo">
				<div id='sala'>
				<?php $oEnderecoUnidade->montaComboSala($enaid);?> 
				</div>
			</td>
		</tr>
		
		
		<tr class="buttons">
			<td colspan='2'>
				<input type='hidden' name='requisicao' id='requisicao' value='salvar' />
				<input type='hidden' name='topo' id='topo' value='<?php echo $_GET['topo']; ?>' />
				<input type='hidden' name='uorco_uorg_lotacao_servidor' id='uorco_uorg_lotacao_servidor' value='<?php echo $uorco_uorg_lotacao_servidor; ?>' />
				<input type='hidden' name='viaEntradaBens' id='viaEntradaBens' value='<?php echo $_GET['viaEntradaBens']; ?>' />
				<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:gravarEndereco();" />
				<input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick="javascript:cancela();" />
		    </td>
		</tr>
	</table>
</form>

<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/enderecoUnidade/views/partialsViews/lista.inc'; 


//caso o cadastro de endereço tenha sido feito a partir de uma chamada
//da tela de entrada de bens, qdo chegar neste ponto retorna os dados
//da unidade e do endereço para a tela
if(!empty($_GET['uendid'])){
	echo "<script>executarScriptPai('retornaEndereco(".$_GET['uendid'].")');self.close();</script>";
}
?>