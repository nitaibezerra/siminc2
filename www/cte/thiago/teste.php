<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

/*************************  Utilização da classe de Data **************************/
//include_once APPRAIZ . "includes/classes/dateTime.inc";

//$data = new Data('03-22-2009 12:47:51.153357', 'mm/dd/yyyy');
//$retorno = $data->formataData( NULL, "DD/MM/YYYY as HH:MMI:SS");
//dbg($retorno);

//$data = new Data();
//$retorno = $data->formataData('2008-01-29 12:47:51.153357', "Brasília DD de mesTextual de YYYY as HH:MMI:SS");
//$retorno = $data->formataData('2008-12-29 12:47:51.153357', "MM-DD-YYYY as HH:MMI:SS");
//$retorno = $data->formataData('2008-01-29 12:47:51.153357', "DD/MM/YYYY as HH:MMI:SS");
//$retorno = $data->formataData('03-22-2009 12:47:51.153357', "DD/MM/YYYY as HH:MMI:SS", 'mm/dd/yyyy');
//$retorno = $data->dataAtual('d-m-Y');
//$retorno = $data->dataAtual();
//$retorno = $data->timeStampDeUmaData(  '05/01/2008');
//$retorno1 = $data->timeStampDeUmaData(  '06/01/2008');
//$retorno2 = $data->timeStampDeUmaData(  '01-25-2002 13:29:00','mm/dd/yyyy');
//$retorno = $data->diferencaEntreDatas(  '05/01/2008', '08/02/2009', 'tempoEntreDadas', 'string','dd/mm/yyyy');	
//$retorno = $data->diferencaEntreDatas(  '05/01/2008', '08/02/2009', 'tempoEntreDadas', 'array');
//$retorno = $data->diferencaEntreDatas(  '2009-01-26 11:25:00', '2002-01-25 13:29:00', 'maiorDataBolean');
//dbg($retorno);

//$segundos_diferenca = $retorno - $retorno1;
//$dias_diferenca = $segundos_diferenca / (60 * 60 * 24);
//$dias_diferenca = abs($dias_diferenca);
//$dias_diferenca = floor($dias_diferenca);

//dbg($dias_diferenca,1);

/*************************  Utilização da classe de Arquivo e classe de Arquivo SIMEC **************************/

// Upload de arquivo. //
/*
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
$campos	= array("cnvid"		=>763
				);	
$file = new FilesSimec("convenioxml", $campos ,"cte");			
if($_FILES["Arquivo"]){
	$arquivoSalvo = $file->setUpload("gravação de arquivo do XML de convênio entre SAPE e ". SIGLA_SISTEMA);
	if($arquivoSalvo){
		echo '<script type="text/javascript"> alert(" Sucesso.");</script>';
	}
}
*/
// criando e movendo arquivo
/*
		include_once APPRAIZ."includes/classes/fileSimec.class.inc";
		
		$campos		= array("cnvid"	=>$convenio);	
		$file 		= new FilesSimec("convenioxml", $campos ,"cte");
		$arquivo 	= $file->criaArquivo(APPRAIZ."arquivos/teste.xml",$xml);
		if($arquivo){
			$file->deletaArquivo(APPRAIZ."arquivos/teste.xml");
			$file->setMover(APPRAIZ."teste/teste.xml", "text/xml");
		}
*/
//FIM - criando e movendo arquivo


// Dowload de arquivo. //
//include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
//$file = new FilesSimec("convenioxml", "teste" ,"cte");	
//$arquivo = $file->getDownloadArquivo('14095');
//echo($arquivo);


/// Fazer agora o  Deletar documento
/*
	public function DeletarDocumento($documento){
		
		$sql = "UPDATE obras.arquivosobra SET aqostatus = 'I' where aqoid=".$documento["aqoid"];
		$this->simec->executar($sql);
	
		$sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$documento["arqid"];
		$this->simec->executar($sql);
	
		$this->simec->commit();
		$_REQUEST["acao"] = "A";
		$this->simec->sucesso("principal/documentos");
	
	}
*/

/////////////////////////////////////////////////////////////////////////

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