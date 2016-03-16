<?php
include_once 'config.inc';
include_once APPRAIZ . 'includes/classes_simec.inc';

if(isset($_REQUEST['abrirArquivoHabilita'])) {
	switch($_REQUEST['abrirArquivoHabilita']) {
		case 'abrir': 
			abrirArquivoHabilita($_REQUEST['arquivo'], $_REQUEST['pasta'], $_REQUEST['diretorio']);
			break;
		case 'lista': 
			$db = new cls_banco();
			$arArquivos = array();
			$arArquivos = retornaArquivosDiretorio($_REQUEST['diretorio']);//../../arquivos/emenda/habilita
			
			$cabecalho = array("Codigo", "Arquivo");
			$db->monta_lista_array($arArquivos, $cabecalho, 20, 4, 'N','Center');
			break;
	}
}

function abrirArquivoHabilita($arquivo, $pasta, $diretorio){
	if( $fh = fopen($diretorio.'/'.$pasta.'/'.$arquivo, 'r') ){
		
		while($linha = fgets($fh)){
			echo "<pre>";
			echo( trim( utf8_encode($linha) ) );				
			echo "</pre>";
		}						
	}
}

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
								 "descricao" => $lista);
				}else{
					//pasta
					$arq = array("codigo" => 'pasta',
								 "descricao" => '<span onclick="abrepopupListaArquivo(\''.$lista.'\');" style="cursor: pointer; color: rgb(0, 102, 204);">'.$lista.'</span>');
				}
				$arquivo[] = $arq;
			}
		}
		return $arquivo;
	}else {
		return false;
	}
}
?>