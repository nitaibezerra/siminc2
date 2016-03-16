<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
/*
function percorreArray(&$array, $linhas=NULL) {
	foreach($array as $chave=>$valor ){
    	if(!is_array($valor)){
            $retorno .= '<ol>'.$valor.'</ol>';
        }else{
            $colspan = 'colspan="'.count($valor).'"';
            $retorno .= '<li>'.$chave."-".$colspan.'</li>';
            $retorno .=  percorreArray($valor);
        }
    }
    return $retorno;
}
*/


function percorreArray($array, $linha, $nivel){
	$trInicio 	= "<tr>";
	$trFim 		= "</tr>";
	//if($cont > $linha ){
		$linhas.= $trInicio;
	//}
	foreach( $array as $chave=>$valor){
		if(!is_array($valor)){
			$linhas .='<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$valor.'</td>';
		}else{
			$nivel++;
			$colspan = 'colspan="'.count($valor).'"';
			$linhas .='<td align="center" valign="top" '.$colspan.' class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$chave.'</td>';
			
			$linhas .= percorreArray($valor, $linha, $nivel);
			
		}
	}
	//if($cont > $linha){
		$linhas.= $trFim;
	//}
	return $linhas;
}

function montaCabecalho($array, $linha){
	$trInicio 	= "<tr>";
	$trFim 		= "</tr>";

	for($cont = 0; $cont < $linha; $cont++){
		//dbg(key($array));
		
		
		//$tabela .= $trInicio."<td>te</td>".$trFim;
		$tabela .= $trInicio;
		//$tabela .= percorreArray($array);
		$tabela .= $trFim;
	}
	return $tabela;
}

$cabecalho = array("Titulo"=>array( "Referência"  =>array("Mês de Monitoramento","status"),
									"Quantidades" =>array("empenhada","liquidada","paga"),
									"Valores"));


//dbg($cabecalho,1);
//print '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
//print percorreArray($cabecalho, 3);
//print '</table>';

/*

echo '<pre>';

$array = array();
$array['node1']['nivel1']['nivel2']['nivel3'] = array('key1'=>'valor','key2'=>'valor','key3'=>'valor');
$array['node2']['nivel1']['nivel1'] = array('key1'=>'valor','key2'=>'valor','key1'=>'valor');

function add ($index = array(), &$arrayInput = array(), $arrayAdd = array(), $level = 0)
{ 
    if ($level == count($index))
    {
        $arrayInput[$arrayAdd['key']] = $arrayAdd['value'];
    }
    else
    {     
        add($index, $arrayInput[$index[$level]], $arrayAdd, ($level+1));
    }
    return $arrayInput;
}


$input = array(0=> 'node1',1 => 'nivel1' , 2 => 'nivel2' , 3 => 'nivel3');

$add=array('key'=>'key4','value'=>'valor do 4');

print_r(add($input, $array, $add));

*/


?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../../cte/monitoraFinanceiro/includes/monitora_financeiro.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
</head>
<body>
<?php 
print '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
print percorreArray($cabecalho,3, 1);
print '</table>';
?>

<table align="center" border="1" class="tabela" cellpadding="3" cellspacing="1" width="100%"; />
	<tr>
		<td colspan="6">Titulo</td>
	</tr>
	<tr>
		<td colspan="2" >Referência</td>
		<td colspan="3" >Quantidades</td>
		<td rowspan="2" >valores</td>
		
	</tr>
	<tr>
		<td>Mês de Monitoramento</td>
		<td>status</td>
		<td>empenhada</td>
		<td>liquidada</td>
		<td>paga</td>
	</tr>
</table>
</body>
</html>