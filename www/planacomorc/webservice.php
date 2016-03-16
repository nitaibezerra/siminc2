<?php
//    $urlWsdl = 'https://homologacao.siop.planejamento.gov.br/services/WSQualitativo?wsdl';
//    $this->certificado = APPRAIZ . "planacomorc/modulos/sistema/comunica/si_mec.pem";
//    $this->senha_certificado = "siMEC2013";
//    

//    $codigomomento = $arrParam['post']['codigomomento'];
    
//    $wsusuario = 'wsmec';
//    $wssenha = 'Ch0c014t3';
//

    
    $client = new SoapClient("https://homologacao.siop.planejamento.gov.br/services/WSQuantitativo?wsdl", array(
        'proxy_host' => "proxy3.mec.gov.br",
        'proxy_port' => 8080,
        'local_cert' => "D:\\simec\\planacomorc/modulos/sistema/comunica/simec.pem",
        'passphrase ' => 'siMEC2013',
        'exceptions' => true,
        'trace' => true,
        'encoding' => 'ISO-8859-1'));
    
    $cl = new StdClass();
    $cl->credencial = new stdClass();
    $cl->credencial->perfil = 32;
    $cl->credencial->senha = '46e13e5b3e289bb88b9bb24d29c5706d';
    $cl->credencial->usuario = 'wsmec';
    
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
            'senha' => '46e13e5b3e289bb88b9bb24d29c5706d',
            'usuario' => 'wsmec'),
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
        'uri' => 'http://servicoweb.siop.sof.planejamento.gov.br/',
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
