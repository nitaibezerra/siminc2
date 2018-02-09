<?php

//$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

//include_once "/var/www/simec/global/config.inc";
require_once BASE_PATH_SIMEC . '/global/config.inc';
//include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
// Pull in the NuSOAP code
require_once APPRAIZ . "/www/webservice/painel/nusoap.php";

/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


// Create the client instance
//Homologação
//$client = new soapcliente('http://sisuab.homolog.capes.gov.br/sisuab/services/WebServiceSimec?wsdl', true);
//$client = new soapcliente('http://sisuab.hom.capes.gov.br/sisuab/webservice?wsdl', true);

//Produção
//$client = new soapcliente('http://uab.capes.gov.br/sisuab/services/WebServiceSimec?wsdl', true);
$client = new soapcliente('http://sisuab.capes.gov.br/sisuab/webservice?wsdl', true);

// Check for an error
$err = $client->getError();

ob_start();

if ($err) {
    // Display the error
    echo '<h2>Constructor error</h2><pre>' . $err . '</pre>';
    // At this point, you know the call that follows will fail
}

// Call the SOAP method
$resultado = $client->call('consultaSisuabSimec', array());
//ver($resultado);

// Check for a fault
if ($client->fault) {
    echo '<h2>Fault</h2><pre>';
    print_r($result);
    echo '</pre>';
} else {
    // Check for errors
    //$err = $client->getError();
    if ($err) {
        // Display the error
        echo '<h2>Error</h2><pre>' . $err . '</pre>';
    } else {
        // Display the result
        echo '<h2>Result</h2><pre>';
        
        //$linhas = explode("\n",$resultado['consultaSisuabSimecReturn']);
		$linhas = explode("\n",$resultado);
		
        if($linhas) {
        	unset($linhas[0]);
        	foreach($linhas as $linha) {
        		
        		$dadosl = explode(";",$linha);
        		$idmantenedor 			= trim($dadosl[0]);
        		$mantenedor 			= trim($dadosl[1]);
        		$idpolo	 				= trim($dadosl[2]);
        		$tipocurso				= trim($dadosl[3]);
        		$idcurso				= trim($dadosl[4]);
        		$nomecurso				= trim($dadosl[5]);
        		$idies					= trim($dadosl[6]);
        		$muncod					= trim($dadosl[7]);
        		$nummatriculasativas	= trim($dadosl[8]);
        		$turmasativas			= trim($dadosl[9]);
        		$numsomatorioidade		= trim($dadosl[11]);
        		
				if($idpolo!=''){
					$sql[] = "INSERT INTO painel.consultaofertas(idmantenedor, mantenedor, idpolo, tipocurso, idcurso, nomecurso, idies, muncod, nummatriculasativas, turmasativas, numsomatorioidade)
		    			  VALUES ('{$idmantenedor}', '{$mantenedor}', '{$idpolo}', '{$tipocurso}', '{$idcurso}', '{$nomecurso}', '{$idies}', '{$muncod}', '{$nummatriculasativas}', '{$turmasativas}', '{$numsomatorioidade}');";
				}
        	}
        }
        
        if($sql) {
        	echo 'Carga efetuada com sucesso';
        	$db->executar("DELETE FROM painel.consultaofertas;");
        	$db->executar(implode("",$sql));
        	$db->commit();
			
			//CARGA MAPA DA UAB
			$db->executar("DELETE FROM mapa.valorindicador WHERE dtiid in (52, 53, 54, 81);");
			$sql = "INSERT INTO mapa.valorindicador (muncod, vliqtd, dtiid)
					(SELECT muncod, count(distinct idpolo) AS vliqtd, 52 AS dtiid
					FROM painel.consultaofertas
					GROUP BY muncod)";
			$db->executar($sql);
			
			$sql = "INSERT INTO mapa.valorindicador (muncod, vliqtd, dtiid)
					(SELECT muncod, sum(nummatriculasativas::integer) AS vliqtd, 53 AS dtiid
					FROM painel.consultaofertas
					GROUP BY muncod)";
			$db->executar($sql);
			
			$sql = "INSERT INTO mapa.valorindicador (muncod, vliqtd, dtiid)
					(SELECT muncod, sum(turmasativas::integer) AS vliqtd, 54 AS dtiid
					FROM painel.consultaofertas
					GROUP BY muncod)";
			$db->executar($sql);
			
			$sql = "INSERT INTO mapa.valorindicador (muncod, vliqtd, dtiid)
					(SELECT muncod, count(distinct idies) AS vliqtd, 81 AS dtiid
					FROM painel.consultaofertas
					GROUP BY muncod)";
			$db->executar($sql);
	        $db->commit();
			
        } else {
        	echo 'Não existem registros para carga';
        }
	    echo '</pre>';

    }
}

$corpoemail = ob_get_contents();
ob_end_clean();


/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "WS Atualizar UAB";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress($_SESSION['email_sistema']);
$mensagem->Subject = "WS Atualizar Consulta Oferta (UAB)";

$mensagem->Body = $corpoemail;
$mensagem->IsHTML( true );
$mensagem->Send();
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */

?>