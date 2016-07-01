<?php
// incluindo as classes necessárias
$oFornecedor = new Fornecedor();

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Presta&ccedil;&atilde;o de Contas - Valores Pagos','');
?>

<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript'>
	
	/**
	 * Abre a tela do relatório
	 * @name imprimir
	 * @return void
	 */	
	function imprimir( isXls ){
		$('btnImprimir').disable();
		$('btnImprimirXls').disable();

		if(validaFormularioPesquisa()){

			$('btnImprimir').enable();
			$('btnImprimirXls').enable();

			var datainicial = $F('datainicial');
			var datafinal = $F('datafinal');

			var agencias = '';
			for(var i=0; i<document.formularioPesquisa.agencias.length; i++){
				if(document.formularioPesquisa.agencias[i].value != ''){
					agencias += document.formularioPesquisa.agencias[i].value + ',';
				} 
			} 
			agencias = agencias.substr(0,agencias.lastIndexOf(','));
	        if( isXls ) 
			{			
				AbrirPopUp('?modulo=relatorio/prestacaodecontas/visualizarPrestacaoContas&acao=A&datainicial='+datainicial+'&datafinal='+datafinal+'&agencias='+agencias+'&isXls=1','visualizaPrestacaoContas','scrollbars=yes,width=800,height=570');
			}
			else
			{
				AbrirPopUp('?modulo=relatorio/prestacaodecontas/visualizarPrestacaoContas&acao=A&datainicial='+datainicial+'&datafinal='+datafinal+'&agencias='+agencias+'&isXls=0','visualizaPrestacaoContas','scrollbars=yes,width=800,height=570');
			}
		}
	}
	
	/**
	 * Valida os campos obrigatórios
	 * @name validaFormularioPesquisa
	 * @return bool
	 */	
	function validaFormularioPesquisa(){
	
		if($F('datainicial') == '' || $F('datafinal') == '') {
			if($F('datainicial') == ''){
				var msg = decodeURIComponent(escape('O campo "Data Inicial" é obrigatório!'));
				alert(msg);
				$('datainicial').focus();	
			}
			else if($F('datafinal') == ''){
				var msg = decodeURIComponent(escape('O campo "Data Final" é obrigatório!'));
				alert(msg);
				$('datafinal').focus();	
			}
			
			$('btnImprimir').enable();
			$('btnImprimirXls').enable();
	        return false;
		}
		else{
			var data = new Data(); 
			if(!data.comparaData($F('datainicial'),'01/01/2000','>=')){
				var msg = decodeURIComponent(escape('A data informada é inválida'));
				alert(msg);
				$('datainicial').focus();
				$('btnImprimir').enable();
				$('btnImprimirXls').enable();
		        return false;
			}
			else if(!data.comparaData($F('datafinal'),'01/01/2000','>=')){
				var msg = decodeURIComponent(escape('A data informada é inválida'));
				alert(msg);
				$('datafinal').focus();
				$('btnImprimir').enable();
				$('btnImprimirXls').enable();
		        return false;
			}
			
		}

		var agencias = '';
		for(var i=0; i<document.formularioPesquisa.agencias.length; i++){
			if(document.formularioPesquisa.agencias[i].value != ''){
				agencias += document.formularioPesquisa.agencias[i].value + ',';
			} 
		} 
		if(agencias == ''){
			var msg = decodeURIComponent(escape('O campo "Agências" é obrigatório!'));
			alert(msg);
			$('btnImprimir').enable();
			$('btnImprimirXls').enable();
	        return false;
		}
	
		return true;
	}

	/**
	 * Seta foco nos campos de data caso ocorra erro na validação das mesmas
	 * @name setaFocoData
	 * @param erroData - Erro ocorrido
	 * @return void
	 */	
	function setaFocoData(erroData){
		if(erroData == 'anoinicialinvalido' || erroData == 'datamenor'){
			$('datainicial').focus();
		}
		else if(erroData == 'anofinalinvalido'){
			$('datafinal').focus();
		}
		else if(erroData == 'nenhumregistro'){
			$('datainicial').focus();
		}
	}

    function validando_data(objeto) {
        // validando a data
        var data_regex = /^((0[1-9]|[12]\d)\/(0[1-9]|1[0-2])|30\/(0[13-9]|1[0-2])|31\/(0[13578]|1[02]))\/\d{4}$/;
        if( objeto.value && ( !data_regex.test(objeto.value) || !validaData(objeto) ) ){
            alert('Data inv�lida!');
            objeto.focus();
            objeto.value = '';
            return false;
        }else{
            return true;
        }
    }

</script>
<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Per&iacute;odo: </td>
			<td class="campo">
				<?php echo campo_data2('datainicial','N','S','Informe a Data Inicial','N','','',$datainicial);?> a 
				<?php echo campo_data2('datafinal','N','S','Informe a Data Final','N','','',$datafinal);?>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> 
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Ag&ecirc;ncias: </td>
			<td class="campo">
				<?php $oFornecedor->monta_combo_fornecedor_multiplo();?>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> 
			</td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='button' class="botao" name='btnImprimir' id='btnImprimir' value='Imprimir' onclick="javascript:imprimir();">
				<input type='button' class="botao" name='btnImprimirXls' id='btnImprimirXls' value='Exportar Excel' onclick="javascript:imprimir( true );">
            </td>
        </tr>
	</table>
</form>