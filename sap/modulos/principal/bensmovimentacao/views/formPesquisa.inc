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
	
		AbrirPopUp('?modulo=sistema/tabelasdeapoio/enderecoUnidade/pesquisar&acao=A&topo=N&viaEntradaBens=S&uorsg='+uorsg+'&uorno='+uorno+'&uorco_uorg_lotacao_servidor='+uorco_uorg_lotacao_servidor,'buscarEndereco','scrollbars=yes, width=800, height=770');
	}
	
	
	/**
	 * Função responsável por setar os dados de endereço e unidade
	 * @name retornaEndereco
	 * @return void
	 */	
	function retornaEndereco(uendid){
		carregaDadosEndereco(uendid);
	}

	/**
	 * Função responsável por limpar os campos
	 * @name limpar
	 * @return void
	 */
	function limpar(){
		window.location='?modulo=principal/bensmovimentacao/pesquisar&acao=A';
	}


	/**
	 * Redireciona para a página de edição
	 * @param id - Chave para edição
	 * @param tipo - Tipo da movimentação
	 * @return void
	 */
	function detalharMovimentacao(id,tipo){
		if(tipo == 'G'){
			location.href = '?modulo=principal/bensmovimentacao/editarMovimentacaoGeral&acao=A&mvbid='+id;
		}
		else if(tipo == 'S'){
			location.href = '?modulo=principal/bensmovimentacao/editarMovimentacaoSiape&acao=A&mvbid='+id;
		}
		else if(tipo == 'L'){
			location.href = '?modulo=principal/bensmovimentacao/editarMovimentacaoLocalidade&acao=A&mvbid='+id+'&edicao=S'+'&mvbtipo='+tipo;
		}
	}


	/**
	 * Redireciona para a página de edição
	 * @param id - Chave para edição
	 * @param tipo - Tipo da movimentação
	 * @return void
	 */
	function detalharMovimentacaoEmAberto(id,tipo){
		if(tipo == 'G'){
			location.href = '?modulo=principal/bensmovimentacao/adicionarMovimentacaoGeral&acao=A&mvbid='+id;
		}
		else if(tipo == 'S'){
			location.href = '?modulo=principal/bensmovimentacao/adicionarMovimentacaoSiape&acao=A&mvbid='+id;
		}
		else if(tipo == 'L'){
			location.href = '?modulo=principal/bensmovimentacao/adicionarMovimentacaoLocalidade&acao=A&mvbid='+id+'&edicao=S'+'&mvbtipo='+tipo;
		}
	}


	/**
     * Redireciona para a exclusão
     * @name removerMovimentacao
     * @param bebid - Id da baixa
     * @return void
     */
    function removerMovimentacao(mvbid){
        window.location = "?modulo=principal/bensmovimentacao/excluir&acao=A&mvbid="+mvbid+"&origem=mov";
    }

	
</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
		
		<tr>
			<td class="SubtituloDireita"> Tipo de Movimenta&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php
				$oTipoMovimentacao->montaComboTipoMovimentacao($tmvid,'N');
				?>
            </td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> Número do Termo de Responsabilidade/Devolução: </td>
			<td class="campo">
				<?=campo_texto('ternum','N','S','',15,10,'','','left','',0,'id="ternum"', '', $ternum);?>				
            </td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> SIAPE do Respons&aacute;vel: </td>
			<td class="campo">
				<?=campo_texto('nu_matricula_siape','N','S','Informe a Matr&iacute;cula SIAPE do Respons&aacute;vel',7,7,'','','left','',0,'id="nu_matricula_siape" onkeypress="return somenteNumeros(event);" onchange="javascript:carregaResponsavelUnidade(this.value);"');?>&nbsp;&nbsp;
				<?=campo_texto('no_servidor','N','N','',50,50,'','','left','',0,'id="no_servidor"');?>
			</td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> Unidade Organizacional: </td>
			<td class="campo">
				<div id='unidades' style='display:inline'>
				<?php echo $oEnderecoUnidade->montaComboUnidade(); ?>
				</div>
				<div style='display:inline'>
				<?=campo_texto( 'unidade','N','N','',57,200,'','','left','',0,'id="unidade"' );?>
				<?php 
				if(!empty($uendid)){
				?>
				<input type='hidden' name='uendid' id='uendid' value='<?php echo $uendid; ?>'>
				<?php 
				}
				else{
				?>
				<input type='hidden' name='uendid' id='uendid'>
				<?php 
				}
				?>
				</div>
            </td>
		</tr>
		
		
		<tr>
			<td class="SubtituloDireita"> UF: </td>
			<td class="campo">
				<?php echo campo_texto('enduf','N','N','',2,2,'','','left','',0,'id="enduf"');?>
				<input type='button' name='btnBuscaEndereco' id='btnBuscaEndereco' value='Buscar Unidade e Endereço' onclick="javascript:pesquisarEndereco();">
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Cidade: </td>
			<td class="campo">
				<?php echo campo_texto('endcid','N','N','',100,100,'','','left','',0,'id="endcid"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> CEP: </td>
			<td class="campo">
				<?php echo campo_texto('endcep','N','N','',15,10,'','','left','',0,'id="endcep"');?>
			</td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> Endere&ccedil;o: </td>
			<td class="campo">
				<?php echo campo_texto('endlog','N','N','',100,200,'','','left','',0,'id="endlog"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Andar: </td>
			<td class="campo">
				<?php echo campo_texto('enadescricao','N','N','',15,15,'','','left','',0,'id="enadescricao"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Sala: </td>
			<td class="campo">
				<?php echo campo_texto('easdescricao','N','N','',50,50,'','','left','',0,'id="easdescricao"'); ?>
            </td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> Período: </td>
			<td class="campo">
				de <?=campo_data2('data_de','N','S','Informe a Data Inicial','N');?>
				até <?=campo_data2('data_ate','N','S','Informe a Data Final','N');?>
            </td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> RGP: </td>
			<td class="campo">
				<?=campo_texto('rgpnum','N','S','Informe o Número do RGP',15,6,'','','left','',0,'id="rgpnum" onkeypress="return somenteNumeros(event);"','',null,"verificaSomenteNumeros(this,'O RGP deve ser numérico!');");?>
            </td>
		</tr>
		
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<input type='hidden' name='requisicao' id='requisicao' />
            	<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Pesquisar' onclick="javascript:pesquisar('filtrar');">
                <input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="javascript:limpar();">
            </td>
        </tr>
	</table>
</form>
<?php 
if(!empty($_GET['mvbid'])){
?>	
	<script>
	var altura = screen.availHeight;
	var largura = screen.availWidth;
	var params = 'scrollbars=yes, width='+largura+', height='+altura+', top=0, left=0';
	
	window.location='?modulo=principal/bensmovimentacao/pesquisar&acao=A';
	AbrirPopUp('?modulo=principal/bensmovimentacao/visualizarTermo&acao=A&mvbid=<?php echo $_GET["mvbid"]; ?>','visualizaMovimentacao',params)
	</script>
<?php 
}
?>
<?php include_once APPSAP . 'modulos/principal/bensmovimentacao/views/partialsViews/listaPesquisa.inc'; ?>