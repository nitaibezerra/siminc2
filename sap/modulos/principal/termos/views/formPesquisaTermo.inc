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
	* variavel adicionada para aproveitar a mesma funcao carregarEmpenho(empid)
	* no arquivo www/sap/js/ajax.js
	* Quando definida como true, a funcao ira ignorar os campos (empdata,empvalorper)
	*/
	var TelaConsultaTermos = true;

	/**
	 * Aciona o filtro
	 * @name filtrar
	 * @param requisicao - Requisicao que sera executada
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
	 * Visualizar Detalhe do Termo
	 * @name visualizarTermo
	 * @return void
	 */	
	function visualizarTermo( url ){
		AbrirPopUp(url,'visualizaTermo','scrollbars=yes,width=900,height=770');
	}


	/**
	 * Limpa os campos de responssavel
	 * @name limparBuscaResponsavel
	 * @return void
	 */
	function limparBuscaResponsavel(){
		$('no_servidor').clear();
		$('nu_matricula_siape').clear();
	}

	/**
	 * Abre a tela de pesquisa de empenho
	 * @name consultarEmpenho
	 * @return void
	 */
	function consultarEmpenho(){
		AbrirPopUp('?modulo=principal/empenho/pesquisarEmpenho&acao=A&popup=S','pesquisarEmpenho','scrollbars=yes,width=800,height=500');
	}

	/**
	 * Função responsável por setar os dados do empenho que foi cadastrado
	 * @name retornaDadosEmpenho
	 * @param empnumero   - Número do empenho
	 * @param empdata     - Data do empenho
	 * @param empvalorper - Valor do empenho
	 * @param empid       - Id do empenho
	 * @return void
	 */
	function retornaDadosEmpenho(empnumero,empdata,empvalorper,empid){
		$('empnumero').setValue(empnumero);
		$('empdata').setValue(empdata);
		$('empvalorper').setValue(empvalorper);
		$('empid').setValue(empid);
	}

	/**
	 * Função responsável por limpar os dados dos campos de empenho
	 * @name limparBuscaEmpenho
	 * @return void
	 */
	function limparBuscaEmpenho(){
		$('empnumero').clear();
		$('empdata').clear();
		$('empvalorper').clear();
		$('empid').clear();
	}


	/**
	 * Função para ocultar ou exibir campos de acordo com o tipo da pesquisa
	 * @name ocultaCamposPorTipo
	 * @param tipo       - Tipo de Termo
	 * @return void
	 */
	function ocultaCamposPorTipo( tipo ){
		if(tipo=='EN'){
			//$('#empnumero').hide();
			document.getElementById('linhanumeroprocesso').style.display = '';
			document.getElementById('linhanumeroempenho').style.display = '';  
		}
		if(tipo=='MV'){
			document.getElementById('linhanumeroprocesso').style.display = 'none';
			document.getElementById('linhanumeroempenho').style.display = 'none';
			$('rgpnum').clear();
			$('empnumero').clear();
		}
		if(tipo=='BA'){
			document.getElementById('linhanumeroprocesso').style.display = '';
			document.getElementById('linhanumeroempenho').style.display = '';
			document.getElementById('linhaunidade').style.display = 'none';
			$('unidade').clear();
		}
	}
	
</script>


<form name="formularioPesquisa" id="formularioPesquisa" method="post">

	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita">Número do Termo</td>
			<td class="campo">
				<?=campo_texto('ternum','N','S','',30,20,'','','left','',0,'id="ternum"');?>
            </td>
		</tr>

		<tr>
			<td class="SubtituloDireita">Tipo do Termo: </td>
			<td class="campo">
				<?php $obTermo->montaComboTipoTermo(); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Data do Termo: </td>
			<td class="campo">
				De <?=campo_data2('teremissaodt_ini','N','S','Informe a Data Inicial','S','','',$teremissaodt_ini);?> 
				Até <?=campo_data2('teremissaodt_fim','N','S','Informe a Data Final','S','','',$teremissaodt_fim);?>
            </td>
		</tr>
		
 		<tr>
			<td class="SubtituloDireita">Responsável pelo Termo (SIAPE): </td>
			<td class="campo">
				<?=campo_texto('nu_matricula_siape','N','S','Informe a Matrícula SIAPE do Responsável',7,7,'','','left','',0,'id="nu_matricula_siape" onkeypress="return somenteNumeros(event);" onchange="javascript:carregaResponsavelUnidade(this.value);"');?>&nbsp;&nbsp;
				<?=campo_texto('no_servidor','N','N','',50,50,'','','left','',0,'id="no_servidor"');?>
			</td>
		</tr>

		<tr id='linhanumeroprocesso'>
			<td class="SubtituloDireita">Número do Processo: </td>
			<td class="campo">
				<?=campo_texto('rgpnum','N','S','Informe o Número do RGP',15,6,'','','left','',0,'id="rgpnum" onkeypress="return somenteNumeros(event);"','',null,"verificaSomenteNumeros(this,'O RGP deve ser numÃ©rico!');");?>
            </td>
		</tr>
		
		<tr id='linhanumeroempenho'>
			<td class="SubtituloDireita">Número do Empenho: </td>
			<td class="campo">
				<?php
				if(!empty($_GET['benid']) && $benstatus == 'A'){
					echo campo_texto('empnumero','S','N','Informe o Número do Empenho',12,12,'','','left','',0,'id="empnumero" onkeypress="return somenteNumeros(event);"');
				}
				else{
					echo campo_texto('empnumero','S','S','Informe o Número do Empenho',12,12,'','','left','',0,'id="empnumero" onkeypress="return somenteNumeros(event);"','','','buscarEmpenho(this.value);');
				}
				?>
				<input type='hidden' name='empid' id='empid' value='<?php echo $empid; ?>'>
				<?php
				if(empty($_GET['benid']) || $benstatus == 'I'){
				?>
				<input type='button' name='btnEmpenho' id='btnEmpenho' value='Buscar Empenho' onclick="javascript:consultarEmpenho();">
				<?php
				}
				?>
				
				<input type="hidden" name='benvlrdoc' id='benvlrdoc'>
				
            </td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> Material: </td>
			<td class="campo">
				<?php echo campo_texto('matdsc', 'N', 'N', '', 100, 100, '', '', 'left', '', 0, 'id="matdsc"'); ?>
				<input type="hidden" name="matid" id="matid" value="" />
				<input type='button' class="botao" name='btnConsultarMaterial' id='btnConsultarMaterial' value='Consultar Material' onclick="consultarMaterial()" />
            </td>
		</tr>
		
		<tr id='linhaunidade'>
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
include_once APPSAP . 'modulos/principal/termos/views/partialsViews/lista.inc';
?>
