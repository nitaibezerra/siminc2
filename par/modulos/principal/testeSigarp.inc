<?php

function consultaCategoria(){

	global $db;
	
	try {
		
		$data_created = date("c");
		$usuario = "GABRIELAS";
		$senha   = "76201626";


$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>$usuario</usuario>
            <senha>$senha</senha>
        </auth>
        <params>
        </params>
    </body>
</request>
XML;
	/*        
echo '<pre>';		
echo simec_htmlentities($arqXml);
die();
		*/   		
		$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/categoria';
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'listar') )
				->execute();

		$xmlRetorno = $xml;
							
	//	$xml = simplexml_load_string( stripslashes($xml));
		
	    echo "------ SIGARP ------\n\n";
	//	echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
		echo '<pre>';		
		//echo simec_htmlentities($xml);
		$xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;
		
		print_r ($xml);

		// simulando sem validacão do XML
		// $result = true;
		if(!$result) {
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message->text;	
			$erros = explode("[text]", $erros);
			$erros = explode("[status]", $erros[1]);
			if(is_array($erros)) {
				if($erros[0]){
			//if(count($erros)>0) {
				//foreach($erros as $err) {
					//dbg($err);
					echo "* ".iconv("UTF-8", "ISO-8859-1", $erros[0]);
				//}
			//}
				}
			}
		}
		
	} catch (Exception $e){
		
				# Erro 404 página not found
				if($e->getCode() == 404){
					echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				}
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
		
				echo "Erro-WS SIGARP: $erroMSG";
				echo "<pre>";
				print_r($e);
		
	}
}

function disponibilizaItensCategoria(){

	global $db;
	
	try {
		
		$data_created = date("c");
		$usuario = "GABRIELAS";
		$senha   = "76201626";


$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>$usuario</usuario>
            <senha>$senha</senha>
        </auth>
        <params>
        	<nu_seq_categoria>3</nu_seq_categoria>
        </params>
    </body>
</request>
XML;
	/*        
echo '<pre>';		
echo simec_htmlentities($arqXml);
die();
		*/   		
		$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/item';
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'listar') )
				->execute();

		$xmlRetorno = $xml;
							
		
	    echo "------ SIGARP ------\n\n";
		echo '<pre>';		
					
		$xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;
		
		print_r ($xml);

		// simulando sem validacão do XML
		// $result = true;
		if(!$result) {
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message->text;	
			$erros = explode("[text]", $erros);
			$erros = explode("[status]", $erros[1]);
			if(is_array($erros)) {
				if($erros[0]){
			//if(count($erros)>0) {
				//foreach($erros as $err) {
					//dbg($err);
					echo "* ".iconv("UTF-8", "ISO-8859-1", $erros[0]);
				//}
			//}
				}
			}
		}
		
	} catch (Exception $e){
		
		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS SIGARP: $erroMSG";
		echo "<pre>";
		print_r($e);
		
	}
}

function diponibilizaPregoesItens(){

	global $db;
	
	try {
		
		$data_created = date("c");
		$usuario = "GABRIELAS";
		$senha   = "76201626";


$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>$usuario</usuario>
            <senha>$senha</senha>
        </auth>
        <params>
            <nu_seq_item>1</nu_seq_item>
            <uf>DF</uf>
        </params>
    </body>
</request>
XML;
	/*        
echo '<pre>';		
echo simec_htmlentities($arqXml);
die();
		*/   		
		$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/pregao';
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'listar') )
				->execute();

		$xmlRetorno = $xml;
							
	 	echo "------ SIGARP ------\n\n";
		echo '<pre>';		
					
		$xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;
		
		print_r ($xml);

		// simulando sem validacão do XML
		// $result = true;
		if(!$result) {
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message->text;	
			$erros = explode("[text]", $erros);
			$erros = explode("[status]", $erros[1]);
			if(is_array($erros)) {
				if($erros[0]){
			//if(count($erros)>0) {
				//foreach($erros as $err) {
					//dbg($err);
					echo "* ".iconv("UTF-8", "ISO-8859-1", $erros[0]);
				//}
			//}
				}
			}
		}
		
	} catch (Exception $e){
		
				# Erro 404 página not found
				if($e->getCode() == 404){
					echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				}
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
		
				echo "Erro-WS SIGARP: $erroMSG";
				echo "<pre>";
				print_r($e);
		
	}
}

function diponibilizaItemEmpenhoAprovado(){

	global $db;
	
	try {
		
		$data_created = date("c");
		$usuario = "GABRIELAS";
		$senha   = "76201626";


$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>$usuario</usuario>
            <senha>$senha</senha>
        </auth>
        <params>
			<cnpj>05.054.937/0001-63</cnpj> 
			<nu_seq_pregao>41</nu_seq_pregao> 
			<nu_seq_item>2</nu_seq_item> 
			<qt_item>10</qt_item> 
			<nu_seq_tipo_pagamento>1</nu_seq_tipo_pagamento> 
			<vl_pagamento>1000</vl_pagamento> 
		</params>
    </body>
</request>
XML;
	/*        
echo '<pre>';		
echo simec_htmlentities($arqXml);
die();
		*/   		
		$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/item';
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();

		$xmlRetorno = $xml;
							
	 	echo "------ SIGARP ------\n\n";
		echo '<pre>';		
					
		$xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;
		
		print_r ($xml);

		// simulando sem validacão do XML
		// $result = true;
		if(!$result) {
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message->text;	
			$erros = explode("[text]", $erros);
			$erros = explode("[status]", $erros[1]);
			if(is_array($erros)) {
				if($erros[0]){
			//if(count($erros)>0) {
				//foreach($erros as $err) {
					//dbg($err);
					echo "* ".iconv("UTF-8", "ISO-8859-1", $erros[0]);
				//}
			//}
				}
			}
		}
		
	} catch (Exception $e){
		
				# Erro 404 página not found
				if($e->getCode() == 404){
					echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				}
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
		
				echo "Erro-WS SIGARP: $erroMSG";
				echo "<pre>";
				print_r($e);
		
	}
}


//echo consultaCategoria(); // Envia ao SIMEC dados cadastrais da Categoria.
//echo disponibilizaItensCategoria(); //Envia ao SIMEC dados cadastrais do Item por Categoria.
echo diponibilizaPregoesItens(); // SIMEC envia a categoria ao SIGARP e recebe o (s) item (ns) vinculado (s) a essa categoria.
//echo diponibilizaItemEmpenhoAprovado(); // O SIGARP recebe o código do item e a UF que teve empenho aprovado no SIMEC, verifica se tem pregão ativo, caso exista  retorna o valor do item.
?>