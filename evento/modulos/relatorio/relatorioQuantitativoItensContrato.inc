<?php

if ($_REQUEST['exibir'] == 1){
	ini_set("memory_limit","256M");
	include("popUpRelQuantItensContratos.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório Quantitativo Itens por Contrato', 'Selecione os filtros' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">
function gerarRelatorio(consulta){
	var formulario = document.formulario;
//        alert(consulta);
        console.log(formulario);
	formulario.consulta.value = consulta;
        
	selectAllOptions(formulario.tipoContrato);
	selectAllOptions(formulario.porteEvento);
	
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
        document.getElementById('exibir').value = 1;
	formulario.target = 'relatorio';
//	ajaxRelatorio();
	formulario.submit();
	
	janela.focus();

}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo ){
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' ){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function obras_exibeRelatorioGeralXLS(){
	
	var formulario = document.formulario;
	var agrupador  = document.getElementById( 'agrupador' );
		
	prepara_formulario();
//	selectAllOptions(formulario.agrupador);
	
//	if ( !agrupador.options.length ){
//		alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
//		return false;
//	}
	
//	selectAllOptions( agrupador );
	selectAllOptions(document.getElementById('tipoContrato'));
	selectAllOptions(document.getElementById('porteEvento'));

	formulario.submit();
}
</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
<input type="hidden" name='exibir' id='exibir' value=''>
<input type="hidden" name="limpaSession" id="limpaSession" value="true">
<input type="hidden" name="consulta" id="consulta" value="html">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
    <tr>
    	<td align="left">
            <?php

            /////////////////////////////////////////////////////////////FILTROS DO RELATÓRIO////////////////////////////////////////////////////////////////////////
            ////////////////Tipo do Contrato				
                $sqlContrato = " SELECT
                                cp.coeid AS codigo, 
                                cp.coerazaosocial AS descricao
                           FROM 
                                evento.contratopregao AS cp
                           WHERE cp.coestatus = 'A'
                          -- ORDER BY cp.coeid ASC ";
                $stSqlCarregados = "";
                $arrOrdem = array("cod");
                mostrarComboPopup( 'Número do Contrato', 'tipoContrato',  $sqlContrato, $stSqlCarregados , 'Selecione o número do Contrato',null,'','','','',$arrOrdem ); 
            ?>
        </td>
    </tr>
<!--     <tr>
    	<td align="left">
            <?
//                $sqlPorte = "SELECT porid as codigo, 
//                                    porminpart ||' até '|| pormaxpart as descricao
//                            FROM evento.porteevento";
//                $stSqlCarregados = "";
//                mostrarComboPopup( 'Público', 'porteEvento',  $sqlPorte, $stSqlCarregados, 'Selecione um público',null,'','','','',array("cod") );
//                $db->monta_combo('Público',$sql,'S','Selecione uma opção','','','','','','idprocesso');
            ?>
        </td>
   </tr>-->
    
    <tr>
        <td align="center" colspan="2">
                <input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio('html');" style="cursor: pointer;"/>
                <input type="button" name="Gerar Relatório XLS" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio('xls');" style="cursor: pointer;"/>
        </td>
    </tr>
</table>
</form>
</body>
</html>
