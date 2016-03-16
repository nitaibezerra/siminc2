<?php
// carrega as funções gerais
/*include_once 'config.inc';
include_once APPRAIZ . 'includes/classes_simec.inc';*/
header('content-type: text/html; charset=ISO-8859-1');
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="/includes/prototype.js"></script>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: left;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 700px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    height : 470px;
    margin-left: 10px;
}
#loader1 {
    background-color: #fff;
    color: #000033;
    width: 750px;
    /*border: 2px solid #cccccc;*/
    font-size: 12px;
    font-weight: bold;
    height : 25px;
    margin-left: 10px;
}
</style>
<script type="text/javascript">
	function abreArquivo(arquivo, pasta, diretorio){
		$('loader-container').show();
		
		var url = "ajax_arquivo.php";
		var pars = 'abrirArquivoHabilita=abrir&arquivo='+arquivo+'&pasta='+pasta+'&diretorio='+diretorio;
			
		var myAjax = new Ajax.Request(
				url, 
				{
					method: 'post', 
					parameters: pars, 
					onComplete: mostraRespostaHabilitaPopUp
				});	
		
	}
	function mostraRespostaHabilitaPopUp(resp){
		$('loader').innerHTML = resp.responseText + '<br/><br/><center><input type="button" value="Fechar" onclick="fechar();"><center>';
		$('loader1').innerHTML = '<br/><center><input type="button" value="Fechar" onclick="fechar();"><center>';
	}
	
	function fechar(){
		$('loader-container').hide();
	}
</script>
<div id="loader-container" style="display: none">
   	<div id="loader" style="overflow: scroll;"> </div>
   	<div id="loader1"></div>
</div>
<?

function retornaArquivosDiretorio($dir){
	$itens = array();
	if( file_exists($dir) ){
		$diretorio = opendir($dir);
		// monta os vetores com os itens encontrados na pasta
		while($arquivo = readdir($diretorio)){
			$itens[] = $arquivo;
		}
		// ordena o vetor de itens
		sort($itens);
		
		// percorre o vetor para fazer a separacao entre arquivos e pastas 
		foreach ($itens as $key => $lista) {
			if($lista != '.' && $lista != '..'){				
				// checa se o tipo de arquivo encontrado é uma pasta
				if(is_file($dir.'/'.$lista)){
					//arquivo
					$arq = array("codigo" => 'arquivo',
								 "descricao" => '<span onclick="abreArquivo(\''.$lista.'\', \''.$_REQUEST['arquivo'].'\', \''.$_REQUEST['diretorio'].'\');" style="cursor: pointer; color: rgb(0, 102, 204);">'.$lista.'</span>');
				}else{
					//pasta
					$arq = array("codigo" => 'pasta',
								 "descricao" => $lista);
				}
				$arquivo[] = $arq;
			}
		}
		return $arquivo;
	}else {
		return false;
	}
}

$arArquivos = array();
$arArquivos = retornaArquivosDiretorio($_REQUEST['diretorio'].'/'.$_REQUEST['arquivo']);//../../arquivos/emenda/habilita

/*echo "<pre>";
print_r( $arArquivos );
echo "</pre>";*/

$cabecalho = array("Codigo", "Arquivo");

print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	print '<tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">'.'Lista de Arquivos Habilita'.'</label></td></tr><tr>';
	print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';

$db->monta_lista_array($arArquivos, $cabecalho, 20, 4, 'N','Center');

''

?>
<br/>
<center><input type="button" value="Fechar" onclick="window.close();"><center>