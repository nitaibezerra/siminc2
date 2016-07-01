<?php
if ( $_POST['orgid'] && $_POST['ajax'] == 'comboMes' ){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
	die(comboMes($_POST['orgid']));
}

if ( $_POST ){
	header( 'Content-Type: text/html; charset=iso-8859-1' ); 
	include("resultImpactoFinanc.inc");
	exit;
}

include APPRAIZ . '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Impacto Financeiro', '&nbsp;' );

?>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<script type="text/javascript" src="../includes/prototype.js"></script>
<form name="formulario" id="formulario" action="" method="post">	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td class="SubTituloDireita" valign="top" width="40%">Tipo de Ensino</td>	
			<td>
			<?php
				$sql = " SELECT
								orgid AS codigo,
								orgdesc AS descricao
							FROM
								academico.orgao 
							Where
								orgstatus = 'A'
							ORDER BY 2 ASC;";
				$db->monta_combo("tipoensino", $sql, 'S', '-- Selecione --', 'ajaxComboMes', '', '', '', 'N', 'tipoensino');
		    ?>		
			</td>
		</tr>						
		<tr>
			<td class="SubTituloDireita" valign="top">Ano Base</td>	
			<td>
			<?php
				$exercicio = $_POST['exercicio'] ? $_POST['exercicio'] : date('Y');
				
//				// Exercício
//				$sql = " SELECT DISTINCT
//								prtano AS codigo,
//								prtano AS descricao
//							FROM 
//								academico.portarias
//							ORDER BY
//								prtano ";
				$sql =  array(
								array(
										"codigo" 	=> 2008,
										"descricao" => 2008
									  ),
								array(
										"codigo" 	=> 2009,
										"descricao" => 2009
									  ),
								array(
										"codigo" 	=> 2010,
										"descricao" => 2010
									  ),
								array(
										"codigo" 	=> 2011,
										"descricao" => 2011
									  ),
								array(
										"codigo" 	=> 2012,
										"descricao" => 2012
									  )									  
							 );
				
				$db->monta_combo("exercicio", $sql, 'S', null, '', '', '', '', 'N', 'exercicio');
		    ?>		
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Mês de Impacto</td>	
			<td>
			<?php
				$exercicio = $_POST['exercicio'] ? $_POST['exercicio'] : date('Y');
				
				// Mês de impacto
			$mesArr = array(
						array(
								"codigo"    => '',
								"descricao" => "-- Selecione --"
							 ),
//						array(
//								"codigo"    => 2,
//								"descricao" => "Fevereiro"
//							 ),
//						array(
//								"codigo"    => 3,
//								"descricao" => "Março"
//							 ),
//						array(
//								"codigo"    => 4,
//								"descricao" => "Abril"
//							 ),
//						array(
//								"codigo"    => 5,
//								"descricao" => "Maio"
//							 ),
//						array(
//								"codigo"    => 6,
//								"descricao" => "Junho"
//							 ),
//						array(
//								"codigo"    => 7,
//								"descricao" => "Julho"
//							 ),
//						array(
//								"codigo"    => 8,
//								"descricao" => "Agosto"
//							 ),
//						array(
//								"codigo"    => 9,
//								"descricao" => "Setembro"
//							 ),
//						array(
//								"codigo"    => 10,
//								"descricao" => "Outubro"
//							 ),
//						array(
//								"codigo"    => 11,
//								"descricao" => "Novembro"
//							 ),
//						array(
//								"codigo"    => 12,
//								"descricao" => "Dezembro"
//							 )							 
					 );
					 
//				$mes = $_POST['mes'] ? $_POST['mes'] : date('n');
				$db->monta_combo("impid", $mesArr, 'N', null, '', '', '', '', 'N', 'impid');
		    ?>		
			</td>
		</tr>
		<tr>
			<td align="center" colspan="2">
				<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio();"/>
			</td>
		</tr>
	</table>
</form>
<div id="resultformulario">
<?php
//	if (!$_POST){
//		include "resultImpactoFinanc.inc";
//	}
?>
<div>
<script type="text/javascript">
		/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
	
	
function gerarRelatorio(){
	if ( $('tipoensino').value == '' ){
		alert('O campo tipo de ensino deve ser preenchido!');
		return false;
	}
	ajaxRelatorio();
}	
	
	
	
function ajaxRelatorio(){
	var formulario = document.formulario;
	var divRel 	   = document.getElementById('resultformulario');

	divRel.style.textAlign='center';
	divRel.innerHTML = 'carregando...';
				
	var param = '&tipoensino=' + $('tipoensino').value +  
				'&exercicio=' + $('exercicio').value + 
				'&impid=' + $('impid').value;
				 
   var req = new Ajax.Request('academico.php?modulo=relatorio/formImpactoFinanc&acao=C', {
					        method:       'post',
					        parameters:   param,
					        asynchronous: false,
					        onComplete:   function (res) {
													    	divRel.innerHTML = res.responseText;
												         }
						});
}	





function ajaxComboMes(orgid){
	var formulario = document.formulario;
	var selectMes  = document.getElementById('impid');
	
	selectMes.disabled = true;	
	selectMes.innerHTML = '';
	
	if ( orgid == ''){
		selectMes.options[0] = new Option(' -- Selecione --', '');	
		return false;
	}	
	
	selectMes.options[0] = new Option('carregando...', '');	
				
	var param =  '&orgid=' + $('tipoensino').value + 
				 '&ajax=comboMes';
				 
   var req = new Ajax.Request('academico.php?modulo=relatorio/formImpactoFinanc&acao=C', {
					        method:       'post',
					        parameters:   param,
					        asynchronous: false,
					        onComplete:   function (res) {
					        								eval(res.responseText);
					        								selectMes.innerHTML = '';
					        								
					        								for (i=0; i < dado.length; i++){
					        									selectMes.options[i] = new Option(dado[i]['descricao'], dado[i]['codigo'])
					        									if (dado[i]['mes'] == mes){
						        									selectMes.options[i].selected=true;
					        									}
					        								}
					        								
													    	selectMes.disabled  = false;
												         }
						});
}	


	
	
	
	
	function serialize( mixed_value ) {
    // http://kevin.vanzonneveld.net
    // +   original by: Arpad Ray (mailto:arpad@php.net)
    // +   improved by: Dino
    // +   bugfixed by: Andrej Pavlovic
    // +   bugfixed by: Garagoth
    // %          note: We feel the main purpose of this function should be to ease the transport of data between php & js
    // %          note: Aiming for PHP-compatibility, we have to translate objects to arrays
    // *     example 1: serialize(['Kevin', 'van', 'Zonneveld']);
    // *     returns 1: 'a:3:{i:0;s:5:"Kevin";i:1;s:3:"van";i:2;s:9:"Zonneveld";}'
    // *     example 2: serialize({firstName: 'Kevin', midName: 'van', surName: 'Zonneveld'});
    // *     returns 2: 'a:3:{s:9:"firstName";s:5:"Kevin";s:7:"midName";s:3:"van";s:7:"surName";s:9:"Zonneveld";}'
 
    var _getType = function( inp ) {
        var type = typeof inp, match;
        var key;
        if (type == 'object' && !inp) {
            return 'null';
        }
        if (type == "object") {
            if (!inp.constructor) {
                return 'object';
            }
            var cons = inp.constructor.toString();
            match = cons.match(/(\w+)\(/);
            if (match) {
                cons = match[1].toLowerCase();
            }
            var types = ["boolean", "number", "string", "array"];
            for (key in types) {
                if (cons == types[key]) {
                    type = types[key];
                    break;
                }
            }
        }
        return type;
    };
    var type = _getType(mixed_value);
    var val, ktype = '';
    
    switch (type) {
        case "function": 
            val = ""; 
            break;
        case "undefined":
            val = "N";
            break;
        case "boolean":
            val = "b:" + (mixed_value ? "1" : "0");
            break;
        case "number":
            val = (Math.round(mixed_value) == mixed_value ? "i" : "d") + ":" + mixed_value;
            break;
        case "string":
            val = "s:" + mixed_value.length + ":\"" + mixed_value + "\"";
            break;
        case "array":
        case "object":
            val = "a";
            /*
            if (type == "object") {
                var objname = mixed_value.constructor.toString().match(/(\w+)\(\)/);
                if (objname == undefined) {
                    return;
                }
                objname[1] = serialize(objname[1]);
                val = "O" + objname[1].substring(1, objname[1].length - 1);
            }
            */
            var count = 0;
            var vals = "";
            var okey;
            var key;
            for (key in mixed_value) {
                ktype = _getType(mixed_value[key]);
                if (ktype == "function") { 
                    continue; 
                }
                
                okey = (key.match(/^[0-9]+$/) ? parseInt(key, 10) : key);
                vals += serialize(okey) +
                        serialize(mixed_value[key]);
                count++;
            }
            val += ":" + count + ":{" + vals + "}";
            break;
    }
    if (type != "object" && type != "array") {
      val += ";";
  }
    return val;
}

</script>
<?
function comboMes( $orgid ){
	global $db;
	
	$sql = "SELECT
				impid AS codigo,
				impmes || ' - ' || trim(to_char(impvalor,'99999999.999')) AS descricao,
				impsequencia AS mes				
			FROM
				academico.indiceimpacto
			WHERE
				orgid = " . trim($orgid) . "
			ORDER BY
				impsequencia;";

	$dados = $db->carregar($sql);	
	$mes = date('n');
	
	$js = "var mes = $mes;";
	$js .= "var dado = new Array();";
	
	$z = 0;
	foreach($dados as $item){
		$js .= "dado[$z] = new Array();";
		$js .= "dado[$z]['codigo']={$item['codigo']};";
		$js .= "dado[$z]['descricao']='{$item['descricao']}';";
		$js .= "dado[$z]['mes']={$item['mes']};";
		
//		$option .= '<option ' . ($item['mes'] == $mes ? 'selected="selected"' : '') . ' value="' . $item['codigo'] . '">' . $item['descricao'] . '</option>';
		$z++;
	}

	return $js;
//	return $option;
}
?>