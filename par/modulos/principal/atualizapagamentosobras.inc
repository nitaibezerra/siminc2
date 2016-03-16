<?php
// GERO EX
set_time_limit(0);
ini_set("memory_limit", "500000M");

global $db;

function historicoPagamento( $processo, $wsusuario, $wssenha, $processoOriginal ){
	global $db;
	$data_created = date("c");
	
	
$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$wsusuario</usuario>
			<senha>$wssenha</senha>
		</auth>
		<params>
			<nu_processo>$processo</nu_processo>
		</params>
	</body>
</request>
XML;

//echo '<pre>';		
//	echo simec_htmlentities($arqXml);
//	die();

	//$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
	$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/pf';
	$xml = Fnde_Webservice_Client::CreateRequest()
		->setURL($urlWS)
		->setParams( array('xml' => $arqXml, 'method' => 'historicopagamento') )
		->execute();
	
	$xmlRetorno = $xml;
	$xml = simplexml_load_string( stripslashes($xml));
//	ver($xml,d);
	if( (int)$xml->status->result == 1 && (int)$xml->status->message->code == 1 ){
		$executa = true;
		$arrXML = $xml->body->children();
		$arrXML2 = $xml->body->row->children();
		//ver($arrXML,d);
		$arrRegistro = array();
		foreach($arrXML as $chaves => $dados ){
			$nu_parcela					= trim((string)$dados->nu_parcela);
			$an_exercicio 				= trim((string)$dados->an_exercicio);
			$vl_parcela 				= trim((string)$dados->vl_parcela);
			$nu_mes 					= trim((string)$dados->nu_mes);
			$nu_documento_siafi_ne 		= trim((string)$dados->nu_documento_siafi_ne);
			$nu_seq_mov_ne 				= trim((string)$dados->nu_seq_mov_ne);
			$ds_username_movimento 		= trim((string)$dados->ds_username_movimento);
			$ds_situacao_doc_siafi 		= trim((string)$dados->ds_situacao_doc_siafi);			
			$dt_movimento 				= trim((string)$dados->dt_movimento);			
			$nu_seq_mov_pag 			= trim((string)$dados->nu_seq_mov_pag);			
			$dt_emissao 				= trim((string)$dados->dt_emissao);
			$nu_documento_siafi 		= trim((string)$dados->nu_documento_siafi);
			$numero_de_vinculacao 		= trim((string)$dados->numero_de_vinculacao);
			
			$sql = "INSERT INTO carga.dadospagamentoobra ( processo, processooriginal, nu_parcela, an_exercicio, vl_parcela, nu_mes, nu_documento_siafi_ne, nu_seq_mov_ne, ds_username_movimento, ds_situacao_doc_siafi, dt_movimento, nu_seq_mov_pag, dt_emissao, nu_documento_siafi, numero_de_vinculacao ) 
					VALUES ('".trim($processo)."', '".trim($processoOriginal)."', '{$nu_parcela}', '{$an_exercicio}', '{$vl_parcela}', '{$nu_mes}', '{$nu_documento_siafi_ne}', '{$nu_seq_mov_ne}', '{$ds_username_movimento}', '{$ds_situacao_doc_siafi}', '{$dt_movimento}', '{$nu_seq_mov_pag}', '{$dt_emissao}', '{$nu_documento_siafi}', '{$numero_de_vinculacao}'  )";
			
			$db->executar($sql);
			$db->commit();
		}
	} else {
//	ver((string)$xml->status->message->text, d);
//echo '<pre>';		
//	echo simec_htmlentities($arqXml);
//	die();
		$sql = "INSERT INTO carga.dadospagamentoobra ( processo, processooriginal, mensagem ) 
				VALUES ('{$processo}', '{$processoOriginal}', '".(string)$xml->status->message->text."' )";
		
		$db->executar($sql);
		$db->commit();
	}
}


$sql = "SELECT 
			distinct o.obrnumprocessoconv  as processo
		FROM 
			carga.dadosobras dao
		INNER JOIN obras2.obras o ON o.obrid = dao.obrid::numeric
		WHERE
			obrnumprocessoconv IS NOT NULL";
$dados = $db->carregar($sql);

foreach( $dados as $dado ){
	$processoOriginal = trim($dado['processo']);

	if( strlen(trim($dado['processo'])) == 20 ){
		ver('entrou para formatar');
		$dado['processo'] = str_replace(".","", $dado['processo']);
		$dado['processo'] = str_replace("/","", $dado['processo']);
		$dado['processo'] = str_replace("-","", $dado['processo']);
		$dado['processo'] = str_replace(" ","", $dado['processo']);
	} elseif( strlen(trim($dado['processo'])) == 18 ){
		ver('entrou para remover');
		$dado['processo'] = substr( $dado['processo'], 1 );	
	}
	$processo = trim($dado['processo']);
ver('teste4', $processoOriginal, strlen($processoOriginal), $processo, $dado['processo']);
	historicoPagamento($processo, 'USAP_WS_SIGARP', '28241625', $processoOriginal);
}

echo "FIM:".date("d/m/Y h:i:s");
//echo "<script>window.location=window.location;</script>";
?>