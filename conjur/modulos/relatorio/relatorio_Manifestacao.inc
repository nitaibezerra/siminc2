<?php  
if($_REQUEST['requisicao']=='gerarRelatorio'){
	ini_set("memory_limit","256M");
	include "relatorio_Manifestacao_resultado.inc";
	exit();
}

include_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

monta_titulo( 'Relatório de Manifestações', '&nbsp;' ); 
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript">
	jQuery.noConflict();

	function pesquisarManifestacao(){
		var formulario = document.formulario;
        
        selectAllOptions(document.getElementById('advid'));
        selectAllOptions(document.getElementById('coonid'));

		if(jQuery('[name=htddatainicial]').val() == '' || jQuery('[name=htddatfinal]').val() == ''){
			alert('O campo "Período" é obrigatório!');
			jQuery('[name=htddatainicial]').focus();
			return false;
		} else {
			if(!validaDataMaior(formulario.htddatainicial,formulario.htddatfinal)){
				alert("Data Inicial não pode ser maior Data Final!");
				jQuery('[name=htddatainicial]').focus();
				return false;
		    }
		} 
		
        if( jQuery("[name='relA']").is(':checked') ){
	        if ( jQuery('#advid').val() == '' || jQuery('#advid').val() == null ) {
	            alert('Selecione pelo menos um Advogado!');
	            jQuery('#advid').focus();
	            return false;
	        }
	    }

	    if( jQuery("[name='relC']").is(':checked') ){
	    	if( jQuery('#coonid').val() == '' || jQuery('#coonid').val() == null ){
	    		alert('Selecione pelo menos uma Cordenação!');1111122225665
	            jQuery('#coonid').focus();
	            return false;
	    	}
	    }

    	if(jQuery('#relatorio:checked').length <= 0){
    		alert('Selecione um "Tipo de Relatório"!');
    		jQuery('#relatorio').focus();
    		return false;
    	}	

		var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.target = 'relatorio';
		formulario.submit();
		janela.focus();
	} 

	function abrepopupAdvogados(){
	    window.open('http://<?=$_SERVER['SERVER_NAME']?>/conjur/combo_advogados_bandalarga.php','Advogados','width=500,height=500,scrollbars=1');
	}

	function abrepopupCoordenacoes(){
	    window.open('/conjur/combo_coordenacoes_bandalarga.php','Advogados','width=500,height=500,scrollbars=1');
	}
</script>

<form name="formulario" id="formulario" method="post">
	<input type="hidden" id="requisicao" name="requisicao" value="gerarRelatorio"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td class="subtitulodireita">Período:</td>
			<td>
				<?php echo campo_data2('htddatainicial', 'S', 'S', 'Período Inicial', 'S', '', '', $htddatainicial); ;?>&nbsp;&nbsp;&nbsp;&nbsp;a
				&nbsp;&nbsp;&nbsp;&nbsp;<?php echo campo_data2('htddatfinal', 'S', 'S', 'Período Final', 'S', '', '', $htddatfinal);  ?>		
			</td>
		</tr>
        <tr>
	        <td class="subtitulodireita">Advogado(s):</td>
            <td>
    	        <select multiple="multiple" name="advid[]" onDblClick="abrepopupAdvogados();" id="advid" style="width:400px;" >
        	        <option value="">Duplo clique para selecionar da lista</option>
                </select>
            </td>
        </tr>
        <tr>
	        <td class="subtitulodireita">Coordenação(ões):</td>
            <td>
    	        <select multiple="multiple" name="coonid[]" onDblClick="abrepopupCoordenacoes();" id="coonid" style="width:400px;" >
        	        <option value="">Duplo clique para selecionar da lista</option>
                </select>
            </td>
        </tr>
        <tr>
	        <td class="subtitulodireita">Tipo de Relatório:</td>
            <td>
				<input type="checkbox" class="normal" value="A" name="relA" id="relatorio" style="margin-top: -1px;">&nbsp;Advogado
				<input type="checkbox" class="normal" value="C" name="relC" id="relatorio" style="margin-top: -1px;">&nbsp;Coordenação
				<input type="checkbox" class="normal" value="M" name="relM" id="relatorio" style="margin-top: -1px;">&nbsp;MEC
				<input type="checkbox" class="normal" value="T" name="relT" id="relatorio" style="margin-top: -1px;">&nbsp;Todos
            </td>
        </tr>        
		<tr>
			<td align="center" bgcolor="#DCDCDC" colspan="2">
				<input type="button" value="Pesquisar" id="btnPesquisar" onclick="pesquisarManifestacao();"/>
				<input type="reset" value="Limpar" id="btnLimpar"/>
			</td>
		</tr>				
	</table>
</form>