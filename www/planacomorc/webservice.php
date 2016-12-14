<?php
//    $urlWsdl = WEB_SERVICE_SIOP_URL. 'WSQualitativo?wsdl';
//    $this->certificado = WEB_SERVICE_SIOP_CERTIFICADO;
//    $this->senha_certificado = WEB_SERVICE_SIOP_SENHA;
//    

//    $codigomomento = $arrParam['post']['codigomomento'];
    
//    $wsusuario = WEB_SERVICE_SIOP_USUARIO;
//    $wssenha = WEB_SERVICE_SIOP_SENHA;
//

    
    $client = new SoapClient(WEB_SERVICE_SIOP_URL. "WSQuantitativo?wsdl", array(
//        'proxy_host' => "proxy3.mec.gov.br",
//        'proxy_port' => 8080,
        'local_cert' => WEB_SERVICE_SIOP_CERTIFICADO,
        'passphrase ' => WEB_SERVICE_SIOP_SENHA,
        'exceptions' => true,
        'trace' => true,
        'encoding' => 'ISO-8859-1'));
    
    $cl = new StdClass();
    $cl->credencial = new stdClass();
    $cl->credencial->perfil = 32;
    $cl->credencial->senha = WEB_SERVICE_SIOP_SENHA;
    $cl->credencial->usuario = WEB_SERVICE_SIOP_USUARIO;
    
    $cl->filtro = new StdClass();
    $cl->filtro->anoReferencia = 2013;
    //$cl->filtro->acoes = array('20RM');
    //$cl->filtro->unidadesOrcamentarias = array('26101');

    $cl->selecaoRetorno = new StdClass();
    $cl->selecaoRetorno->anoReferencia = true;
    $cl->selecaoRetorno->acao = true;
    $cl->selecaoRetorno->programa = true;
    $cl->selecaoRetorno->unidadeOrcamentaria = true;
	$cl->selecaoRetorno->localizador = true;
        
    $cl->selecaoRetorno->dotacaoInicial = true;
    $cl->selecaoRetorno->dotAtual = true;
    $cl->selecaoRetorno->empenhadoALiquidar = true;
    $cl->selecaoRetorno->empLiquidado = true;
    $cl->selecaoRetorno->pago = true;

    $cl->selecaoRetorno->rapInscritoNaoProcessado = true;
    $cl->selecaoRetorno->rapNaoProcessadoLiquidadoAPagar = true;
    $cl->selecaoRetorno->rapPagoNaoProcessado = true;

    
    
    
    //print_r($cl);
    
    /*
    $ar = array(
        'credencial' => array(
            'perfil' => 32,
            'senha' => WEB_SERVICE_SIOP_SENHA,
            'usuario' => WEB_SERVICE_SIOP_USUARIO),
        'filtro' => array(
            'anoReferencia' => 2013,
            //'unidadesOrcamentarias' => array('26101'),
            'acoes'=> '20RM'),
        'selecaoRetorno' => array(
            'anoReferencia'=>true,
            'unidadeGestoraResponsavel' => true,
            'categoriaEconomica' => true,
            'acao'=>true));*/

    echo '<pre>';
    
    
    //$x = $client->__call("consultarExecucaoOrcamentaria", $ar);
    $x = $client->__soapCall("consultarExecucaoOrcamentaria", array($cl), array(
        'uri' => WEB_SERVICE_SIOP_URL,
        'soapaction' => ''));
    if($x->return->execucoesOrcamentarias->execucaoOrcamentaria) {
    	foreach($x->return->execucoesOrcamentarias->execucaoOrcamentaria as $r) {
    		$sql = "INSERT INTO planacomorc.dadosfinanceirossiafi(
            		dfsptres, dfsexercicio, dfsdotacaoinicial, dfsdotacaoatual, 
            		dfsempenhado, dfsliquidado, dfspago, dfsrapnaoprocessadoinscritoliquido, 
            		dfsrapnaoprocessadoliquidadoapagar, dfsrapnaoprocessado)
    				VALUES ('".$r->programa.".".$r->acao.".".$r->unidadeOrcamentaria.".".$r->localizador."', 
    						'".$r->anoReferencia."', 
    						'".$r->dotacaoInicial."', 
    						'".$r->dotAtual."', 
            				'".$r->empenhadoALiquidar."', 
            				'".$r->empLiquidado."', 
            				'".$r->pago."', 
            				'".$r->rapInscritoNaoProcessado."', 
            				'".$r->rapNaoProcessadoLiquidadoAPagar."', 
            				'".$r->rapPagoNaoProcessado."');<br>";
    		echo $sql;
    	}
    	
    }

// -- Desmembrar o ptres em programa - acao - unidade - localizador
//UPDATE planacomorc.dadosfinanceirossiafi
//  SET programa = substring(dfsptres FROM '^[0-9A-Z]+'),
//      acao = substring(substring(dfsptres FROM '^[0-9A-Z]+.[0-9A-Z]+') FROM '[0-9A-Z]+$'),
//      unidade = substring(substring(dfsptres FROM '[0-9A-Z]+.[0-9A-Z]+$') FROM '^[0-9A-Z]+'),
//      localizador = substring(dfsptres FROM '[0-9A-Z]+$')    
?>
