<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";


## UPLOAD DE ARQUIVO
$campos	= array("pdeid"		=>103,
				"arpdata"	=>"'".date('Y-m-d')."'");
	
$file = new FilesSimec("arquivospde", $campos);
if($_FILES["Arquivo"]){	
	$arquivoSalvo = $file->setUpload("Teste PDE");
	if($arquivoSalvo){
		echo '<script type="text/javascript"> alert(" Sucesso.");</script>';
	}
}

## DOWNLOAD DE ARQUIVO
$file = new FilesSimec();
$file->getDownloadArquivo('14140');


?>
<form action="" method="post" enctype="multipart/form-data" name="formulario">
<table>
	<tr>
		<td>Arquivo: <input type="file" name="Arquivo" />     </td>
	</tr>
	<tr>
		<td><a onclick="javascript:validaform();">incluir</a> </td>
	</tr>
</table>
</form>

<script>
var query;
var objForm = document.forms["formulario"];

function validaform(){
	var saida	= true;
	var alerta	= "Ocorreram os seguintes erros:\r\n\r\n";
	if(objForm.Arquivo.value==''){
		alerta	= alerta+" - Voce deve escolher um arquivo\r\n";
		saida	= false;
	}

	if(saida==false){
		alert(alerta);
	}
	else{
		objForm.submit();
	}
}
</script>