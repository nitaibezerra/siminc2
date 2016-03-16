<?php
	include_once "config.inc";
	include_once APPRAIZ . "includes/funcoes.inc";
	
	$path = "D:\musicas_alter";
	$diretorio = scandir($path);
	
    //ver($diretorio,d);
	//$arquivo = scandir($diretorio);
    echo "Lista de Arquivos do diretório '<strong>".$path."</strong>':<br />";
    
    foreach ($diretorio as $dir) {
    	if( strstr($dir, '-') ){
	    	$arq = explode('-', $dir);
	    	
	      	//echo trim($arq[0]).' <> '.trim($arq[1]).' <> '.trim($arq[2]).'<br>';
	      	if( strstr($arq[1], '.m') ) $arqNom = explode('.m', $arq[1]);
	      	else $arqNom = explode('.M', $arq[1]);
	      	
			$nome  = trim($arqNom[0]);
			$exten = trim($arqNom[1]);
			
			$nomeArq = /*trim($arq[1]).' - '.*/$nome.' - '.trim($arq[0]).'.m'.strtolower($exten);
			//ver($arq, $arqNom, $arq[0], $arq[1], $nomeArq,d);
			
			rename( $path.'/'.$dir,  $path.'/'.$nomeArq);
		}
    }	
	$diretorio -> close();
?>