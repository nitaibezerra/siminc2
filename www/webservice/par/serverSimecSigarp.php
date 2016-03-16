<?php

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "3072M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// Pull in the NuSOAP code
require_once('nusoap.php');

$_REQUEST['baselogin'] = "simec_desenvolvimento";
//$_REQUEST['baselogin'] = "simec_espelho_producao";

// Connects to basedata in simec
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";


define("SISID_PAR", 23);

// 	abre conexão com o servidor de banco de dados
$db = new cls_banco();

// Create the server instance
$server = new soapservidor();

// Initialize WSDL support
$server->configureWSDL('parwsdl', 'urn:parwsdl');


// Register the method to expose
$server->register('autenticarUsuario',                		// method name
    array('cpf' => 'xsd:string', 'senha' => 'xsd:string'),  // input parameters
    array('return' => 'xsd:string'),      					// output parameters
    'urn:autenticarUsuariowsdl',                      		// namespace
    'urn:autenticarUsuariowsdl#autenticarUsuario',          // soapaction
    'rpc',                                					// style
    'encoded',                            					// use
    'Autentica usuário no simec'            				// documentation
);

// Autenticar o usuário
function autenticarUsuario($login, $senha) {
	global $db;

	$sql = "SELECT * FROM seguranca.usuario WHERE usucpf='".$login."'";
	$usr = $db->pegaLinha($sql);
	
	if($usr && $senha == md5_decrypt_senha( $usr['ususenha'], '' )) {
		session_start();
		$_SESSION['usucpf'] = $usr['usucpf'];
		$_SESSION['usucpforigem'] = $_SESSION['usucpf'];
		$_SESSION['sisid']  = SISID_PAR;
		return session_id();
	} else {
		return false;
	}
}
/*
$server->wsdl->addComplexType(
        'csvs',
        'complexType',
        'array',
        '',
        'SOAP-ENC:Array',
        array(),
        array('csvs' => array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'xsd:string'))
);
*/

// consultar Situação da Obra
/*
$server->wsdl->addComplexType(
        'arraysituacao',
        'complexType',
        'array',
        '',
        'SOAP-ENC:Array',
        array(),
        array('arraysituacao' => array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'xsd:string'))
);
*/

$server->register('consultarSituacaoObra',                			// method name
		array(	'PHPSESSID' 		=> 'xsd:string', 
				'preid'				=> 'xsd:integer',
				'CNPJ_FORNECEDOR'	=> 'xsd:string', 
				'NOME_FORNECEDOR'	=> 'xsd:string', 
				'COD_SITUACAO_FASE'	=> 'xsd:integer', 
				'DT_ALTERACAO_FASE'	=> 'xsd:string', 
				'SITUACAO_ADESAO'	=> 'xsd:string'),     			// input parameters
		array('return' => 'xsd:string'),      						// output parameters
		'urn:consultarSituacaoObrawsdl',                      		// namespace
		'urn:consultarSituacaoObrawsdl#consultarSituacaoObra',      // soapaction
		'rpc',                                						// style
		'encoded',                            						// use
		'Consultar situação da Obra'								// documentation
);

/*
$server->register('consultarSituacaoObra',                			// method name
		array(	'PHPSESSID' 		=> 'xsd:string', 
				'preid'				=> 'xsd:integer', 
				'situacao' 			=> 'tns:arraysituacao'),     	// input parameters
		array('return' => 'xsd:string'),      						// output parameters
		'urn:consultarSituacaoObrawsdl',                      		// namespace
		'urn:consultarSituacaoObrawsdl#consultarSituacaoObra',      // soapaction
		'rpc',                                						// style
		'encoded',                            						// use
		'Consultar situação da Obra'								// documentation
);
*/
function consultarSituacaoObra($PHPSESSID, $preid = null, $CNPJ_FORNECEDOR = null, $NOME_FORNECEDOR = null, $COD_SITUACAO_FASE = null, $DT_ALTERACAO_FASE = null, $SITUACAO_ADESAO = null) {
//function consultarSituacaoObra($PHPSESSID, $preid, $situacao) {
	
	$data_created = date("c");
	
	if ($PHPSESSID) {
		session_id($PHPSESSID);
	} else {
		
$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>0</result>
        <message>
            <code>0003</code>           
			<text>Problema na autenticação.</text> 
        </message>
    </status>
</response>
XML;
		

	return $arqXml;
		
	}
	
	session_start();

	global $db;
	
	$sql = "INSERT INTO par.historicowssituacaoobra (phpsessid, preid, cnpjfornecedor, nomefornecedor, codsituacaofase, dtalteracaofase, situacaoadesao) VALUES 
			('".utf8_decode((string)$PHPSESSID)."', ".(integer)$preid.", '".utf8_decode((string)$CNPJ_FORNECEDOR)."', '".utf8_decode((string)$NOME_FORNECEDOR)."', ".(integer)$COD_SITUACAO_FASE.", '".$DT_ALTERACAO_FASE."', '".utf8_decode((string)$SITUACAO_ADESAO)."')";
	$db->executar($sql);
	$db->commit();
	
	
	// Verifico os campos
	if( !$preid || !$CNPJ_FORNECEDOR || !$NOME_FORNECEDOR || !$COD_SITUACAO_FASE || !$DT_ALTERACAO_FASE || !$SITUACAO_ADESAO ){
		
$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>0</result>
        <message>
            <code>0005</code>           
			<text>Todos os campos são obrigatórios. Verifique o preenchimento do XML.</text> 
        </message>
    </status>
</response>
XML;
		
	return $arqXml;	
		
	}
	
	
//	$sql = "SELECT aosid FROM par.adesaoobraspacsituacao WHERE preid = ".$preid." AND aoscodsituacao = ".(integer)$COD_SITUACAO_FASE." AND aosdtalteracao = '".$DT_ALTERACAO_FASE."' AND aosdescsituacao = '".utf8_decode((string)$SITUACAO_ADESAO)."'";
//	$testaSituacao = $db->pegaUm( $sql );
//	if( !$testaSituacao ){ // Ainda não temos essa situação no banco
		
		$sql = "SELECT 
					pre.obrid, pre.preid, sts.esdid, pre.preterraplanagem, o.docid, doc.esdid as esdidatual, esd.esddsc
				FROM obras.preobra pre
				INNER JOIN obras2.sigarpsituacao sts ON sts.stsid = ".(integer)$COD_SITUACAO_FASE."
				INNER JOIN obras2.obras o ON o.obrid = pre.obrid AND o.obrstatus = 'A'
				LEFT JOIN workflow.documento doc ON doc.docid = o.docid
				LEFT JOIN workflow.estadodocumento esd on esd.esdid = doc.esdid
				WHERE 
					pre.preid = ".$preid."
				ORDER BY 
					pre.preid, sts.stsordem";
//		ver($sql, d);
		$dado = $db->pegaLinha($sql);
		if($dado){
			
			$arresdid2 = array( 771, 871 );
			$sql = "SELECT DISTINCT esdid FROM obras2.sigarpsituacao";
			$arresdid = $db->carregarColuna($sql);
//			ver('foi', $dado['esdidatual'], $arresdid, $arresdid2);

			if( in_array( $dado['esdidatual'], $arresdid ) || in_array( $dado['esdidatual'], $arresdid2 ) ){
//				ver('entrou aqui', d);
				$obrid = $dado['obrid'];
				$preid = $dado['preid'];
				$esdid = $dado['esdid'];
				$aosdtalteracao = $DT_ALTERACAO_FASE;
				$preterraplanagem = $dado['preterraplanagem'];
				$docid = $dado['docid'];
				$esdidatual = $dado['esdidatual'];
				$estadoatual = $dado['esddsc'];
				$hstid = '';
				
				if(!$docid){
					$esdidatual=870;
					$docdsc = "Fluxo de obra do módulo Obras II - obrid ".$obrid;
					$sql = "INSERT INTO workflow.documento(tpdid, esdid, docdsc) VALUES (105, ". $esdidatual. ", '". $docdsc ."') RETURNING docid";
					$docid = $db->pegaUm($sql);
					
					$sql = "UPDATE obras2.obras SET docid = ".$docid." WHERE obrid = " .$obrid;
					$db->executar($sql);
				}
				
				$esdidAnterior = $esdidatual;
				
				if($preterraplanagem=='t' && $esdid == 864){
					$esdid = 872;
				}
		
				if($esdidAnterior <> $esdid){
					$sql = "SELECT aedid
						FROM workflow.acaoestadodoc
						WHERE esdidorigem = " . $esdidAnterior . " AND esdiddestino = " . $esdid;
					$aedid = $db->pegaUm($sql);
					if($aedid){
						$sql = "INSERT INTO workflow.historicodocumento(aedid, docid, usucpf, pflcod, htddata)
								VALUES (".$aedid.", ".$docid.", '', 932, '".$aosdtalteracao."') RETURNING hstid";
						$hstid = $db->pegaUm($sql);
					}else{
						$dadoEsdid = $db->pegaUm("SELECT esddsc FROM workflow.estadodocumento WHERE esdid = ".$esdid);
						
						$sql = "INSERT INTO workflow.acaoestadodoc(esdidorigem, esdiddestino, aeddscrealizar, aeddscrealizada, aedvisivel)
								VALUES (".$esdidAnterior.", ".$esdid.", 'Enviar para ".$dadoEsdid."', 'Enviado para ".$dadoEsdid."', 'f') RETURNING aedid";
						$aedid = $db->pegaUm($sql);
						
						$sql = "INSERT INTO workflow.historicodocumento(aedid, docid, usucpf, pflcod, htddata)
								VALUES (".$aedid.", ".$docid.", '', 932, '".$aosdtalteracao."') RETURNING hstid";
						$hstid = $db->pegaUm($sql);
					}
		
					if($hstid){
						$sql = "UPDATE workflow.documento SET esdid=".$esdid.", hstid=".$hstid." WHERE docid = " . $docid;
						$db->executar($sql);
					}else{
						$sql = "UPDATE workflow.documento SET esdid=".$esdid." WHERE docid = ". $docid;
						$db->executar($sql);
					}
				}
		
				$sqlInsert = "INSERT INTO par.adesaoobraspacsituacao ( preid, aoscodsituacao, aosdtalteracao, aosdescsituacao, aoscnpjfornecedor, aosnomefornecedor, aosdata, aosprocessado ) VALUES ( ".$preid.", '".(integer)$COD_SITUACAO_FASE."', '".$DT_ALTERACAO_FASE."', '".utf8_decode((string)$SITUACAO_ADESAO)."', '".utf8_decode((string)$CNPJ_FORNECEDOR)."', '".utf8_decode((string)$NOME_FORNECEDOR)."', 'NOW()', 't' ) RETURNING aosid";
			
			} else { // Depois da OS
				
$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>0</result>
        <message>
            <code>0006</code>           
			<text>Tramitação negada. Obra em execução, reformulação ou pregão vencido.</text> 
        </message>
    </status>
</response>
XML;
		
	return $arqXml;				
			
			}
		}
		
//	} else {
//$arqXml = '//<<<XML
/*<?xml version="1.0" encoding="ISO-8859-1"?>
//<response>
//    <header>
//        <app>SIMEC</app>
//        <version>1.0</version>
//        <created>$data_created</created>
//    </header>
//    <status>
//        <result>0</result>
//        <message>
//            <code>0004</code>           
//			<text>As informações da situação já se encontram na base de dados.</text> 
//        </message>
//    </status>
//</response>
//XML';
	return $arqXml;
*/
	
//	}
		
	if( $sqlInsert ){
		$serial = $db->pegaUm($sqlInsert);
		$db->commit();
	}
	

$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>1</result>
        <message>
            <code>0001</code>           
			<text>SUCESSO</text> 
        </message>
    </status>
    <body>
		<serial>$serial</serial>    
    </body>
</response>
XML;
		

	return $arqXml;

}



$server->register('gravaCategoriaSigarp',                			// method name
		array(	'PHPSESSID' 		=> 'xsd:string', 
				'id_categoria' 		=> 'xsd:integer', 
				'ds_categoria' 		=> 'xsd:string'),     			// input parameters
		array('return' => 'xsd:string'),      						// output parameters
		'urn:gravaCategoriaSigarpwsdl',                      		// namespace
		'urn:gravaCategoriaSigarpwsdl#gravaCategoriaSigarp',      	// soapaction
		'rpc',                                						// style
		'encoded',                            						// use
		'Alimentar tabela de categorias vindas do SIGARP'			// documentation
);

function gravaCategoriaSigarp($PHPSESSID, $id_categoria, $ds_categoria) {

	$data_created = date("c");
	
	if ($PHPSESSID) {
		session_id($PHPSESSID);
	} else {
		
$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>0</result>
        <message>
            <code>0003</code>           
			<text>Problema na autenticação.</text> 
        </message>
    </status>
</response>
XML;
		

	return $arqXml;
		
	}
	
	session_start();

	global $db;
		
	$_SESSION['usucpforigem'] = $_SESSION['usucpf'];
	
	$sql = "SELECT sctcodigo FROM par.sigarpcategoria WHERE sctcodigo = ".$id_categoria;
	$sctcodigo = $db->pegaUm($sql);
	
	if( $sctcodigo ){
		$sql = "UPDATE par.sigarpcategoria SET sctdsc = '$ds_categoria', sctdtinclusao = 'NOW()' WHERE sctcodigo = ".$id_categoria." RETURNING sctid;";
	} else {
		$sql = "INSERT INTO par.sigarpcategoria
					(sctcodigo, sctdsc, sctdtinclusao)
			    VALUES 
			    	($id_categoria, '$ds_categoria', 'NOW()') RETURNING sctid;";
	}
	$sctid = $db->pegaUm($sql);
	$db->commit();


$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>1</result>
        <message>
            <code>0001</code>           
			<text>SUCESSO</text> 
        </message>
    </status>
    <body>
		<categoria>$sctid</categoria>    
    </body>
</response>
XML;
		

	return $arqXml;

}

$server->register('gravaItemSigarp',                			// method name
		array(	'PHPSESSID' 		=> 'xsd:string', 
				'id_item' 			=> 'xsd:integer', 
				'ds_item' 			=> 'xsd:string',     			
				'ds_especificacao' 	=> 'xsd:string',     			
				'id_categoria' 		=> 'xsd:integer'),     		// input parameters
		array('return' => 'xsd:string'),      					// output parameters
		'urn:gravaItemSigarpwsdl',                      		// namespace
		'urn:gravaItemSigarpwsdl#gravaItemSigarp',      		// soapaction
		'rpc',                                					// style
		'encoded',                            					// use
		'Alimentar tabela de itens vindos do SIGARP'			// documentation
);

function gravaItemSigarp($PHPSESSID, $id_item, $ds_item, $ds_especificacao, $id_categoria) {
	
	$data_created = date("c");

	if ($PHPSESSID) {
		session_id($PHPSESSID);
	} else {
		
$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>0</result>
        <message>
            <code>0003</code>           
			<text>Problema na autenticação.</text> 
        </message>
    </status>
</response>
XML;
		

	return $arqXml;
		
	}
	
	session_start();

	global $db;
		
	$_SESSION['usucpforigem'] = $_SESSION['usucpf'];
	
	$sql = "SELECT sitcodigo FROM par.sigarpitem WHERE sitcodigo = ".$id_item;
	$sitcodigo = $db->pegaUm($sql);

	$sql = "SELECT sctid FROM par.sigarpcategoria WHERE sctcodigo = ".$id_categoria;
	$sctid = $db->pegaUm($sql);
	
	if( !$sctid ){
		
$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>0</result>
        <message>
            <code>0002</code>           
			<text>Não existe esta categoria cadastrada!</text> 
        </message>
    </status>
</response>
XML;
		

		return $arqXml;
	}
	
	if( $sitcodigo ){
		$sql = "UPDATE par.sigarpitem SET sitdsc = '$ds_item', sitdtinclusao = 'NOW()', sctid = ".$sctid." WHERE sitcodigo = ".$sitcodigo." RETURNING sitid;";
	} else {
		$sql = "INSERT INTO par.sigarpitem
					(sitcodigo, sitdsc, sitdtinclusao, sctid)
			    VALUES 
			    	($id_item, '$ds_item', 'NOW()', $sctid) RETURNING sitid;";
	}
	$sitid = $db->pegaUm($sql);
	$db->commit();

$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>1</result>
        <message>
            <code>0001</code>           
			<text>SUCESSO</text> 
        </message>
    </status>
    <body>
		<item>$sitid</item>    
    </body>
</response>
XML;
		

	return $arqXml;
	
}


$server->wsdl->addComplexType(
        'uflist',
        'complexType',
        'array',
        '',
        'SOAP-ENC:Array',
        array(),
        array('uflist' => array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'xsd:string'))
);

$server->wsdl->addComplexType(
        'itemlist',
        'complexType',
        'array',
        '',
        'SOAP-ENC:Array',
        array(),
        array('itemlist' => array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'xsd:integer'))
);


$server->register('gravaPregaoSigarp',                			// method name
		array(	'PHPSESSID' 		=> 'xsd:string', 
				'nu_pregao' 		=> 'xsd:string', 
				'nu_seq_pregao' 	=> 'xsd:integer', 
				'regiao' 			=> 'tns:uflist',     			
				'dt_inicio' 		=> 'xsd:date',     			
				'dt_fim'	 		=> 'xsd:date'),     			
			//	'id_item' 			=> 'tns:itemlist'),     	// input parameters
		array('return' => 'xsd:string'),      					// output parameters
		'urn:gravaPregaoSigarpwsdl',                      		// namespace
		'urn:gravaPregaoSigarpwsdl#gravaPregaoSigarp',     		// soapaction
		'rpc',                                					// style
		'encoded',                            					// use
		'Alimentar tabela de Pregões vindos do SIGARP'			// documentation
);

//function gravaPregaoSigarp($PHPSESSID, $id_pregao, $uf, $dt_inicio, $dt_fim, $id_item) {
function gravaPregaoSigarp($PHPSESSID, $nu_pregao, $nu_seq_pregao, $regiao, $dt_inicio, $dt_fim) {

	$data_created = date("c");
	
	if ($PHPSESSID) {
		session_id($PHPSESSID);
	} else {
		
$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>0</result>
        <message>
            <code>0003</code>           
			<text>Problema na autenticação.</text> 
        </message>
    </status>
</response>
XML;
		

		return $arqXml;
		
	}

	session_start();

	global $db;
	
	$_SESSION['usucpforigem'] = $_SESSION['usucpf'];
	
	$sql = "SELECT sprid FROM par.sigarppregao WHERE sprnuseqpregao = ".$nu_seq_pregao;
	$sprid = $db->pegaUm($sql);

	if( $sprid ){
		$sql = "UPDATE par.sigarppregao SET sprdtinicio = '".$dt_inicio."', sprdtfim = '".$dt_fim."', sprdtinclusao = 'NOW()', sprnupregao = '".$nu_pregao."' WHERE sprid = ".$sprid;
		$db->executar($sql);
	} else {
		$sql = "INSERT INTO par.sigarppregao
					(sprnuseqpregao, sprnupregao, sprdtinicio, sprdtfim, sprdtinclusao)
			    VALUES 
			    	(".$nu_seq_pregao.", '".$nu_pregao."', '".$dt_inicio."', '".$dt_fim."', 'NOW()')
			    RETURNING sprid";
		$sprid = $db->pegaUm($sql);
	}
	$db->commit();
	
	$sucesso = 0;
	$arrItemPerdido = array();
	$sql = "";
	
	if( $sprid ){	
		if( is_array($regiao) ){
			foreach( $regiao as $dadoregiao ){
				if( is_array($dadoregiao['uf']) ){
					foreach( $dadoregiao['uf'] as $uf ){
						if(is_array($dadoregiao['item'])){
							foreach( $dadoregiao['item'] as $item ){
								$sqlItem = "SELECT sitid FROM par.sigarpitem WHERE sitcodigo = ".$item['id_item'];
								$sitid = $db->pegaUm( $sqlItem );
				
								if( !$sitid ){
									$arrItemPerdido[] = $item;
								} else {
									$sucesso = 1;
									$sql .= "INSERT INTO par.sigarppregaodados( sprid, sitid, spduf, spdvalor ) VALUES ( ".$sprid.", ".$sitid.", '".$uf."', ".$item['vlr_item']." );";
								}
								unset($sitid);
							}
						}
					}
				}
			}
			if( $sucesso == 1 ){
				$sqlDelete = "DELETE FROM par.sigarppregaodados WHERE sprid = ".$sprid;
				$db->executar($sqlDelete);
			}
			$db->executar($sql);
			$db->commit();
		}

		/*
		if( is_array($id_item) ){
			$sql = "DELETE FROM par.sigarppregaoitem WHERE sprid = ".$sprid."; ";
			foreach( $id_item as $item ){
				$sqlItem = "SELECT sitid FROM par.sigarpitem WHERE sitcodigo = ".$item;
				$sitid = $db->pegaUm( $sqlItem );
				
				if( !$sitid ){
					$arrItemPerdido[] = $item;
				} else {
					$sucesso = 1;
					$sql .= "INSERT INTO par.sigarppregaoitem ( sprid, sitid ) VALUES ( ".$sprid.", ".$sitid." );";
				}
				
				unset($sitid);
			}
			$db->executar($sql);
			$db->commit();
		}
		if( $sucesso == 1 ){
			if( is_array($uf) ){
				$sql = "DELETE FROM par.sigarppregaouf WHERE sprid = ".$sprid."; ";
				foreach( $uf as $ufatual ){
					$sql .= "INSERT INTO par.sigarppregaouf ( sprid, uf ) VALUES ( ".$sprid.", '".$ufatual."' );";
				}
				$db->executar($sql);
				$db->commit();
			}
		}
		*/
	}

	
if( $sucesso == 1 ){
	
$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>1</result>
        <message>
            <code>0001</code>           
			<text>SUCESSO</text> 
        </message>
    </status>
    <body>
		<pregao>$sprid</pregao>    
    </body>
</response>
XML;

} elseif( $arrItemPerdido[0] ) {

	$sql = "DELETE FROM par.sigarppregaodados WHERE sprid = ".$sprid."; ";
	$sql .= "DELETE FROM par.sigarppregao WHERE sprid = ".$sprid."; ";
	$db->executar($sql);
	$db->commit();
	
$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>0</result>
        <message>
            <code>0004</code>           
			<text>Itens não cadastrados.</text> 
        </message>
    </status>
</response>
XML;

}

	return $arqXml;

}

// Use the request to (try to) invoke the service
$HTTP_RAW_POST_DATA = isset($HTTP_RAW_POST_DATA) ? $HTTP_RAW_POST_DATA : '';
$server->service($HTTP_RAW_POST_DATA);
?>