<?php
require_once APPRAIZ . "www/includes/webservice/pj.php";
?>
<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/calendario.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript' src='./js/mascaraCpfCnpj.js'></script>
<script language='javascript' type='text/javascript' src='/includes/webservice/pj.js'></script>
<script language='javascript' type='text/javascript'>


	/**
	 * Submete o formulário
	 * @name gravarBem
	 * @return void
	 */
	function gravarBem(){
		$('btnSalvar').disable();
		if(validaFormulario()){
			$('formularioCadastroBem').submit();
		}
	}


	/**
	 * Valida os campos obrigatórios
	 * @name validaFormulario
	 * @return bool
	 */
	function validaFormulario(){

		if($('tesid').getValue() == '') {
			alert('O campo "Tipo de Entrada" é obrigatório!');
			$('tesid').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('nu_matricula_siape').getValue() == '' || $('no_servidor').getValue() == '') {
			alert('O campo "Responsável" é obrigatório!');
			$('nu_matricula_siape').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('uendid').getValue() == '') {
			alert('O campo "Unidade Organizacional" é obrigatório!');
			$('uendid').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('bennumproc').getValue() == '') {
			alert('O campo "Número do Processo" é obrigatório!');
			$('bennumproc').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('tesslg').getValue() != 'SUBS' && ($('empnumero').getValue() == '' || $('empid').getValue() == '')) {
			alert('O campo "Número do Empenho" é obrigatório!');
			$('empnumero').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('bennumdoc').getValue() == '') {
			alert('O campo "Número do Documento" é obrigatório!');
			$('bennumdoc').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('bendtdoc').getValue() == '') {
			alert('O campo "Data do Documento" é obrigatório!');
			$('bendtdoc').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('benvlrdoc').getValue() == '') {
			alert('O campo "Valor do Documento" é obrigatório!');
			$('benvlrdoc').focus();
			$('btnSalvar').enable();
	        return false;
		}

		/*if($('benitdoc').getValue() == '') {
			alert('O campo "Qtd. de Itens no Documento" é obrigatório!');
			$('benitdoc').focus();
			$('btnSalvar').enable();
	        return false;
		}*/

		if($('tesslg').getValue() == 'DOAC' && $('bencnpjceddoa').getValue() == ''){
			alert('O "Cedente/Doador" é obrigatório!');
			$('btnSalvar').enable();
	        return false;
		}

		if($('tesslg').getValue() != 'DOAC' && $('forcpfcnpj').getValue() == ''){
			alert('O "Fornecedor" é obrigatório!');
			$('btnSalvar').enable();
	        return false;
		}

		/*if($('benvlrdoc').getValue() == '0,00' || $('benitdoc').getValue() < 1) {
			alert('O valor do documento e a quantidade de itens não pode ser zero!');
			$('btnSalvar').enable();
	        return false;
		}*/

		if($('benvlrdoc').getValue() == '0,00') {
			alert('O valor do documento não pode ser zero!');
			$('btnSalvar').enable();
	        return false;
		}

		if($('benjust') && $('benjust').getValue() == ''){
			alert('O campo "Justificativa" é obrigatório!');
			$('benjust').focus();
			$('btnSalvar').enable();
	        return false;
		}

		return true;

	}


	/**
	 * Limpa o campo de nome do responsável
	 * @name limparBuscaResponsavel
	 * @return void
	 */
	function limparBuscaResponsavel(){
		$('no_servidor').clear();
	}


	/**
	 * Abre a tela de pesquisa de endereço
	 * @name pesquisarEndereco
	 * @return void
	 */
	function pesquisarEndereco(){
		var uorco_uorg_lotacao_servidor = $('uorco_uorg_lotacao_servidor').getValue();
		AbrirPopUp('?modulo=sistema/tabelasdeapoio/enderecoUnidade/pesquisar&acao=A&topo=N&viaEntradaBens=S&uorco_uorg_lotacao_servidor='+uorco_uorg_lotacao_servidor,'buscarEndereco','scrollbars=yes,width=800,height=770');
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
	 * Abre a tela de pesquisa de fornecedor
	 * @name consultarFornecedor
	 * @return void
	 */
	function consultarFornecedor(){
		AbrirPopUp('?modulo=principal/fornecedor/pesquisarFornecedor&acao=A&popup=S','pesquisarFornecedor','scrollbars=yes,width=800,height=470');
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
	 * Função responsável por setar os dados do fornecedor que foi cadastrado
	 * @name retornaDadosFornecedor
	 * @param forcpfcnpj          - CPF/CNPJ do fornecedor
	 * @param fornomefantasia     - Nome do fornecedor
	 * @return void
	 */
	function retornaDadosFornecedor(forcpfcnpj,fornomefantasia){
		$('forcpfcnpj').setValue(forcpfcnpj);
		$('fornomefantasia').setValue(fornomefantasia);
	}



	/**
	 * Função responsável por verificar o tipo de entrada e saída selecionado e atualizar alguns campos
	 * @name verificaTipoEntradaSaida
	 * @param tipo - Tipo de entrada/saída
	 * @return void
	 */
	function verificaTipoEntradaSaida(tipo){

		//chama o ajax que recupera a sigla do tipo de entrada pelo id
		if(!isNaN(tipo)){
			recuperaSiglaTipoEntrada(tipo);
		}

		//pega a sigla que o ajax recuperou e setou no campo oculto
		var sigla = $('tesslg').getValue();

		if(sigla == 'COMP'){
			var numDocumento = $('bennumdoc').getValue();
			var fieldNumDocumento = '<input type="text" class="obrigatorio normal" title="Informe o Número do Documento" onkeypress="return somenteNumeros(event);" id="bennumdoc" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="'+numDocumento+'" maxlength="20" size="21" name="bennumdoc"><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';

			var documento = $('dscdoc').getValue();
			var fieldDocumento = '<input id="dscdoc" class="disabled" type="text" title="" style="width: 103ex; text-align: left;" readonly="readonly" value="'+documento+'" maxlength="100" size="101" name="dscdoc">';

			$('documento').innerHTML = fieldNumDocumento+fieldDocumento;
			$('dscdoc').setValue('Nota Fiscal');
		}
		else if(sigla == 'SUBS'){
			var numDocumento = $('bennumdoc').getValue();
			var fieldNumDocumento = '<input type="text" class="obrigatorio normal" title="Informe o Número do Documento" onkeypress="return somenteNumeros(event);" id="bennumdoc" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="'+numDocumento+'" maxlength="20" size="21" name="bennumdoc"><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';

			var documento = $('dscdoc').getValue();
			var fieldDocumento = '<input id="dscdoc" class="disabled" type="text" title="" style="width: 103ex; text-align: left;" readonly="readonly" value="'+documento+'" maxlength="100" size="101" name="dscdoc">';

			$('documento').innerHTML = fieldNumDocumento+fieldDocumento;
			$('dscdoc').setValue('Nota Fiscal');
		}
		else if(sigla != 'COMP' && sigla != 'SUBS' && sigla != ''){
			var numDocumento = $('bennumdoc').getValue();
			var fieldNumDocumento = '<input type="text" class="obrigatorio normal" title="Informe o Número do Documento" onkeypress="return somenteNumeros(event);" id="bennumdoc" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="'+numDocumento+'" maxlength="20" size="21" name="bennumdoc"><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';

			var documento = $('dscdoc').getValue();
			var fieldDocumento = '<input type="text" class="obrigatorio normal" title="Informe a Descrição do Documento" style="width: 103ex; text-align: left;" id="dscdoc" value="'+documento+'" maxlength="100" size="101" name="dscdoc" id="dscdoc">';

			$('documento').innerHTML = fieldNumDocumento+fieldDocumento;
		}

		if(sigla != 'DOAC' && sigla != ''){
			$('linhadocumentofornecedor').show();
			$('linhanomefornecedor').show();
			$('linhadocumentodoador').hide();
			$('linhanomedoador').hide();
			$('bencnpjceddoa').clear();
			$('benceddoa').clear();
		}
		else if(sigla == 'DOAC'){
			$('linhadocumentofornecedor').hide();
			$('linhanomefornecedor').hide();
			$('forcpfcnpj').clear();
			$('fornomefantasia').clear();
			$('linhadocumentodoador').show();
			$('linhanomedoador').show();
		}


		if(sigla == 'SUBS'){
			$('linhanumeroempenho').hide();
			$('linhadataempenho').hide();
			$('linhavalorempenho').hide();
		}
		else if(sigla != 'SUBS'){
			$('linhanumeroempenho').show();
			$('linhadataempenho').show();
			$('linhavalorempenho').show();
		}

	}


	/**
	 * Função responsável por limpar os dados dos campos de fornecedor
	 * @name limparBuscaFornecedor
	 * @return void
	 */
	function limparBuscaFornecedor(){
		$('forcpfcnpj').clear();
		$('fornomefantasia').clear();
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
	 * Função responsável para buscar os dados do cedente/doador no webservice da Receita
	 * @name buscaCedenteDoador
	 * @param obj - Campo com o valor a ser pesquisado
	 * @return void
	 */
	function buscaCedenteDoador(obj){

		 $('benceddoa').clear();

		if($('bencnpjceddoa').getValue() != ''){

			if( !validarCnpj( obj.value  ) ){
				alert( "O CNPJ Informado é Inválido." );
				obj.value = "";
				return false;
			} else {
				var comp     = new dCNPJ();
				comp.buscarDados( obj.value );
				/*console.log(comp);*/
				if(comp.dados.no_empresarial_rf == ''){
					alert('O CNPJ do Cedente/Doador Não Foi Encontrado.');
				}
				else{
					$('benceddoa').setValue( comp.dados.no_empresarial_rf );
				}
			}
		}
	}


	/**
	 * Função responsável por cancelar a operação. Disponível apenas na edição
	 * @name cancelarEntrada
	 * @return void
	 */
	function cancelarEntrada(){
		window.location='?modulo=principal/bens/pesquisarBem&acao=A';
	}

	/**
	 * Função responsável por avançar a segunda tela do cadastro. Disponível apenas na edição
	 * @name continuarEntrada
	 * @return void
	 */
	function continuarEntrada(){
		var benid = $('benid').getValue();
		window.location='?modulo=principal/bens/adicionarBemMaterialAtivo&acao=A&benid='+benid;
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
    * Abre a tela de relatório de visualização de entrada de bens
    * @name visualizarEntrada
    * @return void
    */
    function visualizarEntrada(){
   	    var benid = $('benid').getValue();

		var altura = screen.availHeight;
		var largura = screen.availWidth;
		var params = 'scrollbars=yes, width='+largura+', height='+altura+', top=0, left=0';
   	    
        AbrirPopUp('?modulo=principal/bens/visualizarEntrada&acao=A&benid='+benid,'visualizaEntrada',params);
    }

    function concluir(){
 	    $('requisicao').setValue('concluir');
 	  	if(validaFormulario()){
			$('formularioCadastroBem').submit();
		}		
    }

</script>

<form name="formularioCadastroBem" id="formularioCadastroBem" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">

		<tr>
			<td class="SubtituloDireita"> Tipo de Entrada: </td>
			<td class="campo">
				<?php
				if(!empty($_GET['benid']) && $benstatus == 'A'){
					$oTipoEntradaSaida->montaComboTipoEntradaSaida('','E','S','S','N');
				}else{
					$oTipoEntradaSaida->montaComboTipoEntradaSaida('','E','S','S');
				}
				?>
				<input type='hidden' name='tesslg' id='tesslg' value='<?php echo $tesslg; ?>'>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data de Entrada: </td>
			<td class="campo">
				<?php
				if(empty($bendtentrada)){
					$bendtentrada = date('d/m/Y');
				}
				?>
				<?php echo campo_texto('bendtentrada','N','N','',12,12,'','','left','',0,'id="bendtentrada"','',$bendtentrada);?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Respons&aacute;vel Pelos RGP's: </td>
			<td class="campo">
				<?php
				if(!empty($_GET['benid']) && $benstatus == 'A'){
					echo campo_texto('nu_matricula_siape','S','N','Informe a Matr&iacute;cula SIAPE do Respons&aacute;vel',20,20,'','','left','',0,'id="nu_matricula_siape" onkeypress="return somenteNumeros(event);" onchange="javascript:carregaResponsavel(this.value);"');
				}
				else{
					echo campo_texto('nu_matricula_siape','S','S','Informe a Matr&iacute;cula SIAPE do Respons&aacute;vel',20,20,'','','left','',0,'id="nu_matricula_siape" onkeypress="return somenteNumeros(event);" onchange="javascript:carregaResponsavel(this.value);"');
				}
				?>
				<?php echo campo_texto('no_servidor','N','N','',50,50,'','','left','',0,'id="no_servidor"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Unidade Organizacional: </td>
			<td class="campo">
				<?php echo campo_texto('unidade','S','N','',100,150,'','','left','',0,'id="unidade"');?>
				<input type='hidden' name='uendid' id='uendid' value='<?php echo $uendid; ?>'>
				<input type='hidden' name='uorco_uorg_lotacao_servidor' id='uorco_uorg_lotacao_servidor' value='<?php echo $uorco_uorg_lotacao_servidor; ?>'>
				<input type='hidden' name='uorsg' id='uorsg' value='<?php echo $uorsg; ?>'>
				<?php
				if(empty($_GET['benid']) || $benstatus == 'I'){
				?>
				<input type='button' name='btnBuscaEndereco' id='btnBuscaEndereco' value='Buscar Unidade e Endereço' onclick="javascript:pesquisarEndereco();">
				<?php
				}
				?>
            </td>
		</tr>

		<tr>
			<td class="SubtituloDireita"> UF: </td>
			<td class="campo">
				<?php echo campo_texto('enduf','S','N','',2,2,'','','left','',0,'id="enduf"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Cidade: </td>
			<td class="campo">
				<?php echo campo_texto('endcid','S','N','',100,100,'','','left','',0,'id="endcid"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> CEP: </td>
			<td class="campo">
				<?php echo campo_texto('endcep','S','N','',12,12,'','','left','',0,'id="endcep"');?>
			</td>
		</tr>

		<tr>
			<td class="SubtituloDireita"> Endereço: </td>
			<td class="campo">
				<?php echo campo_texto('endlog','S','N','',100,200,'','','left','',0,'id="endlog"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Andar: </td>
			<td class="campo">
				<?php echo campo_texto('enadescricao','S','N','',15,15,'','','left','',0,'id="enadescricao"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Sala: </td>
			<td class="campo">
				<?php echo campo_texto('easdescricao','S','N','',50,50,'','','left','',0,'id="easdescricao"'); ?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Processo: </td>
			<td class="campo">
				<?php
				if(!empty($bennumproc) && !empty($_GET['benid']) && $benstatus == 'A') {
					//$bennumproc = substr($bennumproc, 0,5).'.'.substr($bennumproc, 5,6).'.'.substr($bennumproc, 11,4).'-'.substr($bennumproc, 15);
					echo campo_texto('bennumproc','S','N','Informe o N&uacute;mero do Processo',25,20,'#####.######.####-##','','left','',0,'id="bennumproc"','',null,"carregarProcesso(this.value);");
				}
				else{
					if(strlen($bennumproc) == 17){
						$bennumproc = substr($bennumproc, 0,5).'.'.substr($bennumproc, 5,6).'.'.substr($bennumproc, 11,4).'-'.substr($bennumproc, 15);
					}
					echo campo_texto('bennumproc','S','S','Informe o N&uacute;mero do Processo',25,20,'#####.######.####-##','','left','',0,'id="bennumproc"','',null,"carregarProcesso(this.value);");
				}
				?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data do Processo: </td>
			<td class="campo">
				<?php echo campo_texto('bendtproc','N','N','',15,10,'','','left','',0,'id="bendtproc"','');?>
            </td>
		</tr>
		<tr id='linhanumeroempenho'>
			<td class="SubtituloDireita"> N&uacute;mero do Empenho: </td>
			<td class="campo">
				<?php
				if(!empty($_GET['benid']) && $benstatus == 'A'){
					echo campo_texto('empnumero','S','N','Informe o N&uacute;mero do Empenho',12,12,'','','left','',0,'id="empnumero" onkeypress="return somenteNumeros(event);"');
				}
				else{
					echo campo_texto('empnumero','S','S','Informe o N&uacute;mero do Empenho',12,12,'','','left','',0,'id="empnumero" onkeypress="return somenteNumeros(event);"','','','buscarEmpenho(this.value);');
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
            </td>
		</tr>
		<tr id='linhadataempenho'>
			<td class="SubtituloDireita"> Data do Empenho: </td>
			<td class="campo">
				<?php echo campo_texto('empdata','N','N','',12,10,'','','left','',0,'id="empdata"');?>
			</td>
		</tr>
		<tr id='linhavalorempenho'>
			<td class="SubtituloDireita"> Valor do Empenho Para Material Permanente: </td>
			<td class="campo">
				<?php
				if(empty($erro)){
					echo campo_texto('empvalorper','N','N','',12,50,'','','left','',0,'id="empvalorper"','',number_format($empvalorper, 2, ',', '.'));
				}
				else{
					echo campo_texto('empvalorper','N','N','',12,50,'','','left','',0,'id="empvalorper"','',$empvalorper);
				}

				?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero do Documento: </td>
			<td class="campo">
				<div id='documento'>
				<?php echo campo_texto('bennumdoc','S','S','Informe o N&uacute;mero do Documento',20,20,'','','left','',0,'id="bennumdoc" onkeypress="return somenteNumeros(event);"');?>
				<?php echo campo_texto('dscdoc','N','N','',100,100,'','','left','',0,'id="dscdoc"');?>
				</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data do Documento: </td>
			<td class="campo">
				<?php echo campo_data2('bendtdoc','S','S','Informe a Data do Documento','N','','',$bendtdoc);?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Valor do Documento: </td>
			<td class="campo">
				<?php
				if(empty($erro)){
					echo campo_texto('benvlrdoc','S','S','Informe o Valor do Documento', 14, 14,'[###.]###,##','','left','',0,'id="benvlrdoc" onkeypress="return somenteNumeros(event);"', '', (!empty($benvlrdoc))?formata_valor($benvlrdoc):'',"verificaSomenteNumerosMonetario(this,'O Valor do Documento deve ser numérico!');");
				}else{
					echo campo_texto('benvlrdoc','S','S','Informe o Valor do Documento', 14, 14,'[###.]###,##','','left','',0,'id="benvlrdoc" onkeypress="return somenteNumeros(event);"', '', $benvlrdoc,"verificaSomenteNumerosMonetario(this,'O Valor do Documento deve ser numérico!');");
				}
				?>

            </td>
		</tr>
		<!--<tr>
			<td class="SubtituloDireita"> Qtd. de Itens no Documento: </td>
			<td class="campo">
				<?php //echo campo_texto('benitdoc','S','S','Informe a Quantidade de Itens no Documento',3,3,'','','left','',0,'id="benitdoc" onkeypress="return somenteNumeros(event);"','',null,"verificaSomenteNumeros(this,'A Qtd. de Itens no Documento deve ser numérica!');");?>
            </td>
		</tr>-->
		<tr id='linhadocumentodoador'>
			<td class="SubtituloDireita"> CNPJ do Cedente/Doador: </td>
			<td class="campo">
				<div id='cedentedoador'>
				<?php
				if(!empty($bencnpjceddoa) && !empty($_GET['benid']) && strlen($bencnpjceddoa)==14){
					$bencnpjceddoa = substr($bencnpjceddoa, 0,2).'.'.substr($bencnpjceddoa, 2,3).'.'.substr($bencnpjceddoa, 5,3).'/'.substr($bencnpjceddoa, 8,4).'-'.substr($bencnpjceddoa, 12,2);
				}
				?>
				<?php echo campo_texto('bencnpjceddoa','S','S','Informe o CNPJ do Cedente/Doador',25,18,'##.###.###/####-##','','left','',0,'id="bencnpjceddoa"', '', '', 'buscaCedenteDoador(this);');?>
				</div>
            </td>
		</tr>
		<tr id='linhanomedoador'>
			<td class="SubtituloDireita"> Cedente/Doador: </td>
			<td class="campo">
				<?php echo campo_texto('benceddoa','S','N','',100,100,'','','left','',0,'id="benceddoa"');?>
            </td>
		</tr>
		<tr id='linhadocumentofornecedor'>
			<td class="SubtituloDireita"> Documento do Fornecedor (CNPJ/CPF/IE): </td>
			<td class="campo">
				<div id='cpfcnpjfornecedor' style='display:inline'>
				<?php echo campo_texto('forcpfcnpj','S','S','Informe o CPF/CNPJ do Fornecedor',25,18,'','','left','',0,'id="forcpfcnpj" onkeypress="mascaraMutuario(this,cpfCnpj);"','','','carregaFornecedor(this.value);');?>
				</div>
				<input type='button' name='btnFornecedor' id='btnFornecedor' value='Buscar Fornecedor' onclick="javascript:consultarFornecedor();">
            </td>
		</tr>
		<tr id='linhanomefornecedor'>
			<td class="SubtituloDireita"> Fornecedor: </td>
			<td class="campo">
				<?php echo campo_texto('fornomefantasia','S','N','',100,150,'','','left','',0,'id="fornomefantasia"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Observa&ccedil;&atilde;o:</td>
			<td class="campo">
				<?php echo campo_textarea('benobs','N', 'S', '', 54, 6, 5000,'','','','','',''); ?>
			</td>
		</tr>

		<?php
		if(!empty($_GET['benid']) && $benstatus == 'A'){
		?>
		<tr>
			<td class="SubtituloDireita">Justificativa:</td>
			<td class="campo">
				<?php echo campo_textarea('benjust','S', 'S', '', 54, 6, 5000,'','','','','',''); ?>
			</td>
		</tr>
		<?php
		}
		?>


		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='benstatus' id='benstatus' value='<?php echo $benstatus; ?>'>
            	<input type='hidden' name='benid' id='benid' value='<?php echo $_GET['benid']; ?>'>
            	<?php
            	if(!empty($_GET['benid']) && $benstatus == 'A'){
            	?>
            	<input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick="javascript:cancelarEntrada();">
            	<input type='button' class="botao" name='btnVisualizarEntrada' id='btnVisualizarEntrada' value='Visualizar Entrada' onclick="javascript:visualizarEntrada();" />
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Concluir Edição' onclick="javascript:concluir();">
            	<input type='button' class="botao" name='btnContinuar' id='btnContinuar' value='Próxima Etapa' onclick="javascript:continuarEntrada();">
            	<?php
            	}
            	else{
            	?>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Continuar' onclick="javascript:gravarBem();">
            	<?php
            	}
            	?>
            </td>
        </tr>
	</table>
</form>
<?php
//if(!empty($_GET['benid'])){
?>
<script>verificaTipoEntradaSaida();</script>
<?php
//}
?>