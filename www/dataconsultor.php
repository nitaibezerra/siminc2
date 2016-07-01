<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";

if($_REQUEST['dias'] !== NULL && $_REQUEST['data'] !== NULL && $_REQUEST['parcelas'] !== NULL){
	$data = new Data();
	$dataatual = $_REQUEST['data'];
	$qtd = $_REQUEST['parcelas'];
	$letras = array(1=>'A',2=>'B',3=>'C',4=>'D',5=>'E',6=>'F',7=>'G',8=>'H',9=>'I',10=>'J',11=>'L',12=>'M',13=>'N',14=>'O',15=>'P');
	echo 'Previsão para receber cada parcela do documento técnico:';
	echo '<br></br>';
	
	for($x = 1; $x <= $qtd; $x++){
		$retorno = $data->acrescentaDiasNaData($dataatual,$_REQUEST['dias']);
		$ano = substr ( $retorno, 0, 4 );
		$mes = substr ( $retorno, 4, 2 );
		$dias = substr ( $retorno, 6, 2 );
		$dataatual = $dias."/".$mes."/".$ano;
		echo '<div style="border-bottom: 3px solid #1070A6;  width: 150px; float: left;" title="Documento Técnico '.$letras[$x].'"  > Doc Técnico '.$letras[$x].' <br> Parcela: '.$x.' <br> <span style="border-left:2px solid #1070A6; "> '.$dias.'/'.$mes.'/'.$ano.' </span>  </div>';
	}
	echo '<br></br><br></br><p></p>';
	
}
?>



<html>
<head>
	<link rel="stylesheet" type="text/css" href="includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="includes/listagem.css"/>
	<script type="text/javascript" src="includes/funcoes.js"></script>
</head>
<body>
<form action="" method="post" name="formulario">
	<table class="tabela" >
		<tr>
			<td class="SubTituloDireita" >Insira a data início contrato:</td>
			<td><input type="text" name="data" onkeyup="this.value=mascaraglobal('##/##/####', this.value);" /></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Quantidade de parcelas:</td>
			<td> <input type="text" name="parcelas" /></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tempo entre uma parcela e outra em dias:</td>
			<td><input type="text" name="dias" /></td>
		</tr>
		<tr>
			<td class="SubTituloCentro"  colspan="2"><a href="#" onclick="teste();">Ver data</a> </td>
		</tr>
	</table>
</form>
<script>
function teste(){

	if(document.forms['formulario'].dias.value == ''){
	  alert('falta dias');
	  return false;
	}
	
	if(document.forms['formulario'].parcelas.value == ''){
	  alert('falta parcelas');
	  return false;
	}
	
	if(document.forms['formulario'].data.value == ''){
	  alert('falta a data');
	  return false;
	}
	document.forms['formulario'].submit();
}
</script>
</body>
<html>