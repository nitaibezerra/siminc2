<?php

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "3072M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// Pull in the NuSOAP code
require_once('nusoap.php');

$_REQUEST['baselogin'] = "simec_espelho_producao";

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

	if($senha == md5_decrypt_senha( $usr['ususenha'], '' )) {
		session_start();
		$_SESSION['usucpf'] = $usr['usucpf'];
		$_SESSION['sisid']  = SISID_PAR;
		return session_id();
	} else {
		return false;
	}
}

$server->wsdl->addComplexType(
        'csvs',
        'complexType',
        'array',
        '',
        'SOAP-ENC:Array',
        array(),
        array('csvs' => array('ref'=>'SOAP-ENC:arrayType','wsdl:arrayType'=>'xsd:string'))
);

// Register the method to expose
$server->register('pegarRelatorioPlanejamento',                // method name
    array('PHPSESSID' => 'xsd:string'),        // input parameters
    array('return' => 'tns:csvs'),      // output parameters
    'urn:pegarRelatorioPlanejamentowsdl',                      // namespace
    'urn:pegarRelatorioPlanejamentowsdl#pegarRelatorioPlanejamento',                // soapaction
    'rpc',                                // style
    'encoded',                            // use
    'Inserir dados em serie historica'            // documentation
);

function pegarRelatorioPlanejamento($PHPSESSID) {
	
	if (isset($PHPSESSID)) {
		session_id($PHPSESSID);
	}
	session_start();
	
	global $db;
	
	$sql = "SELECT 	distinct CASE WHEN ptoclassificacaoobra='P' THEN 'Proinfancia' WHEN ptoclassificacaoobra='Q' THEN 'Quadra' WHEN ptoclassificacaoobra='C' THEN 'Cobertura' END as tipo_obra,
			pre.preid as n_protocolo_simec,
			pre.estuf as uf,
			'Município' as proponente,
			mun.mundescricao as municipio_beneficiado,
			mun.muncod as cod_ibge,
			tmn.tpmdsc as grupo_pac,
			pto.ptodescricao as tipo_projeto,
			pre.prevalorobra as valor_obra,
			esd.esddsc as situacao_analise_fnde,
			CASE WHEN pre.preesfera = 'E' THEN 'Estadual' WHEN pre.preesfera ='M' THEN 'Municipal' END as preesfera,
			pre.predescricao as nome_obra,
			pre.prelogradouro || ', ' || pre.precomplemento || ', ' || pre.precep as endereco_obra,
			pre.prelatitude || ' / ' || pre.prelongitude as coordenada_geografica,
			ter.data_termo,
			SUM(emo.eobvalorempenho) as valor_empenho_solicitado,
			(select count(*) from par.pagamento pp2 
			inner join par.empenho ep3 on ep3.empid = pp2.empid 
			inner join par.empenhoobra emo2 on emo2.empid = ep3.empid and empstatus = 'A' and eobstatus = 'A' 
			where pp2.pagstatus='A' and emo2.preid=pre.preid) as numero_parcelas,
			(select to_char(MIN(pp2.pagdatapagamentosiafi),'dd/mm/YYYY') from par.pagamento pp2 
			inner join par.empenho ep3 on ep3.empid = pp2.empid and empstatus = 'A'
			where pp2.pagstatus='A' AND ep3.empnumeroprocesso = emp.empnumeroprocesso AND pp2.pagparcela=1) as data_pagamento_siafi_1_parcela,
			(select sum(ppo.pobvalorpagamento) from par.pagamentoobra ppo
			where ppo.preid = pre.preid)  as valor_pagamento_solicitado ,
			res.resdescricao,
			CASE WHEN pre.preesfera='E' THEN 'Estadual' WHEN pre.preesfera='M' THEN 'Municipal' END as Esfera,
			s.stodesc as situacao_obra,
			coalesce(o.obrpercexec,0) as percentual_execucao,
			pre.preanometa as ano_meta,
			to_char(o.obrdtinicio,'dd/mm/YYYY') as data_de_inicio_da_obra,
			to_char(o.obrdttermino,'dd/mm/YYYY') as data_de_termino_da_obra 
		FROM obras.preobra pre
		INNER JOIN workflow.documento doc ON doc.docid = pre.docid 
		INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid 
		LEFT JOIN obras.obrainfraestrutura o ON o.preid = pre.preid AND o.obsstatus = 'A'  
		LEFT JOIN obras.situacaoobra s ON o.stoid = s.stoid
		LEFT JOIN territorios.municipio mun ON mun.muncod = pre.muncod 
		LEFT JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod AND mtm.estuf = pre.estuf 
		LEFT JOIN territorios.tipomunicipio tmn ON tmn.tpmid = mtm.tpmid 
		LEFT JOIN workflow.estadodocumento esd 	ON doc.esdid = esd.esdid   
		LEFT JOIN (SELECT preid,empid,SUM(eobpercentualemp) eobpercentualemp,SUM(eobvalorempenho) eobvalorempenho FROM par.empenhoobra where eobstatus = 'A' group by preid,empid) emo ON emo.preid = pre.preid 
		LEFT JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' 
		LEFT JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A' 
		LEFT JOIN par.resolucao res ON res.resid = pro.resid
		
		Left Join(
			Select to_char(terdatainclusao,'dd/mm/YYYY') data_termo, teo.preid
			From par.termocompromissopac ter 
			Join par.termoobra teo on teo.terid = ter.terid 
			Where ter.terassinado = true and ter.terstatus='A' 
			--Order by ter.terid desc limit 1
		) as ter on ter.preid = pre.preid
		
		WHERE tmn.tpmid IN(163,164,165) AND doc.esdid IN('228','360','365','366','367')  AND pre.docid IS NOT NULL AND pre.prestatus = 'A' AND pre.tooid = 1 
		
		GROUP BY  
			pto.ptoclassificacaoobra,
			pre.preid,
			uf,
			proponente,
			mun.mundescricao, mun.muncod,
			tmn.tpmdsc,
			pto.ptodescricao,
			pre.prevalorobra,
			esd.esddsc,
			pre.predescricao,
			endereco_obra,
			coordenada_geografica,
			data_termo,
			numero_parcelas,
			data_pagamento_siafi_1_parcela,
			valor_pagamento_solicitado ,
			res.resdescricao,
			esfera,
			situacao_obra,
			percentual_execucao,
			pre.preanometa,
			o.obrdtinicio,
			o.obrdttermino
		ORDER BY 
			pre.preid";
	
	
	
	
	$res = $db->carregar($sql);
				 
	return $res;
	
}

$server->register('gravaTramitacaoObra',                				// method name
		array(	'PHPSESSID' 		=> 'xsd:string', 
				'id_obra' 			=> 'xsd:integer', 
				'nu_solicitacao'	=> 'xsd:integer', 
				'co_situacao' 		=> 'xsd:integer', 
				'ds_situacao' 		=> 'xsd:string',
				'dt_situacao'		=> 'xsd:date'),     // input parameters
		array('return' => 'xsd:string'),      						// output parameters
		'urn:gravaTramitacaoObrawsdl',                      			// namespace
		'urn:gravaTramitacaoObrawsdl#gravaTramitacaoObra',        	// soapaction
		'rpc',                                						// style
		'encoded',                            						// use
		'Inserir dados em ...'            							// documentation
);

function gravaTramitacaoObra($PHPSESSID, $id_obra, $nu_solicitacao, $co_situacao, $ds_situacao, $dt_situacao) {

	if (isset($PHPSESSID)) {
		session_id($PHPSESSID);
	}
	
	session_start();

	global $db;
		
	$co_documento = 9;
	$no_documento = 'nine'; 
	$dt_vencimento_documento = '2013/09/09';
	
	$_SESSION['usucpforigem'] = $_SESSION['usucpf'];
	
	$sql = "INSERT INTO par.historicosituacaoobrami
				(preid, hsonusolicitacao, hsocodsituacao, hsodescsituacao, hsodatasituacao)
		    VALUES 
		    	($id_obra, $nu_solicitacao, $co_situacao, '$no_situacao',  '$dt_situacao');";

	if( validaSituacaoSigarp($id_obra, $co_situacao) ){
		$db->executar($sql);
		$db->commit();
// 		foreach( $_SESSION as $k => $valor ){
// 			$texto .= "<br> $k = ".$valor;
// 		}
		return 1;
	}else{
		return 0;
	}

}

$server->register('gravaContratoObra',                				// method name
		array(	'PHPSESSID' 		=> 'xsd:string', 
				'id_obra'			=> 'xsd:integer', 
				'no_arquivo'		=> 'xsd:string',   
				'nu_contrato'		=> 'xsd:integer', 
				'dt_assinatura'		=> 'xsd:date',
				'dt_vencimento'		=> 'xsd:date', 
				'arquivo' 			=> 'xsd:string' 
				),     // input parameters
		array('return' => 'xsd:string'),      						// output parameters
		'urn:gravaContratoObrawsdl',                      			// namespace
		'urn:gravaContratoObrawsdl#gravaContratoObra',        	// soapaction
		'rpc',                                						// style
		'encoded',                            						// use
		'Inserir dados em ...'            							// documentation
);

function gravaContratoObra($PHPSESSID, $id_obra, $no_arquivo, $nu_contrato, $dt_assinatura, $arquivo, $dt_assinatura, $dt_vencimento) {

	if (isset($PHPSESSID)) {
		session_id($PHPSESSID);
	}
	
	session_start();
	
	$_SESSION['usucpforigem'] = $_SESSION['usucpf'];

	global $db;
	
	$pastaTemp = APPRAIZ.'arquivos/par/';

	file_put_contents($pastaTemp.'temp.pdf',base64_decode($arquivo));
	
	$_REQUEST['baselogin'] = "simec_espelho_producao";
	
	if (isset($autenticacao)) {
		session_id($autenticacao);
	}
	session_start();
	
	$fp = fopen( $pastaTemp.'temp.pdf' , "w" );
	if ($fp) {
		stream_set_write_buffer($fp, 0);
		fwrite($fp, $response);
		fclose($fp);
	}
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$campos	= array();
	$file = new FilesSimec("provisorio_arq_ws_situacao_obra", $arrCampos, "par");
	$file->setMover( $pastaTemp.'temp.pdf', 'pdf', false);
	$arqid = $file->getIdArquivo();
	
	
	$sql = "INSERT INTO par.historicocontratoobrami
				(preid, arqid, hcodescarquivo, hconucontrato, hcodataassinatura, hcodatacontratovencimento)
			VALUES 
				($id_obra, $arqid, '$no_arquivo', $nu_contrato, '$dt_assinatura', '$dt_vencimento')";
	
	if($arqid){
		$db->executar($sql);
		$db->commit();
		return 1;
	} else {
		$db->rollback();
		return 0;
	}
}

$server->register('verificaSecretario',                			// method name
		array(	'PHPSESSID' 		=> 'xsd:string', 
				'cpf' 				=> 'xsd:string', 
				'uf' 				=> 'xsd:string', 
				'ibge' 				=> 'xsd:string'), 
			//	'id_item' 			=> 'tns:itemlist'),     	// input parameters
		array('return' => 'xsd:string'),      					// output parameters
		'urn:verificaSecretariowsdl',                      		// namespace
		'urn:verificaSecretariowsdl#verificaSecretario',   		// soapaction
		'rpc',                                					// style
		'encoded',                            					// use
		'Retornar secretário'									// documentation
);

function verificaSecretario($PHPSESSID, $cpf, $uf, $ibge) {
	
	$data_created = date("c");
	
	if ($PHPSESSID) {
		session_id($PHPSESSID);
	} else {
		
$arqXml = <<<XML
<?xml version="1.0" encoding="UTF-8"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>0</result>
        <message>
            <code>0001</code>           
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
	
	if( ($cpf && $uf && $ibge) || ($cpf && $uf) || ($cpf && $ibge) || ($uf && $ibge) ){
		
$arqXml = <<<XML
<?xml version="1.0" encoding="UTF-8"?>
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
			<text>É permitido apenas um parâmetro.</text> 
        </message>
    </status>
</response>
XML;
		

		return $arqXml;
		
	}
	
	if( $cpf ){
	
	$sql = "SELECT 
				entnumcpfcnpj, 
				entnome, 
				entemail, 
				'(' || entnumdddcomercial || ')' || entnumcomercial as telefone,
				muncod as dibge,
				estuf as duf,
				entdatanasc as datanasc
			FROM 
				par.entidade e
			WHERE 
				dutid = 10 AND 
				entstatus = 'A' AND 
				entnumcpfcnpj = '".$cpf."'
				
			UNION ALL
			
			SELECT 
				entnumcpfcnpj, 
				entnome, 
				entemail, 
				'(' || entnumdddcomercial || ')' || entnumcomercial as telefone,
				CASE WHEN iu.itrid = 2 THEN iu.muncod ELSE '' END as dibge,
				CASE WHEN iu.itrid = 2 THEN iu.mun_estuf ELSE iu.estuf END as duf,
				e.entdatanasc as datanasc
			FROM 
				par.entidade e
			INNER JOIN par.instrumentounidade iu ON iu.inuid = e.inuid
			WHERE 
				dutid = 2 AND 
				entstatus = 'A' AND 
				entnumcpfcnpj = '".$cpf."'";
	} elseif($ibge) {
		$sql = "SELECT 
				entnumcpfcnpj, 
				entnome, 
				entemail, 
				'(' || entnumdddcomercial || ')' || entnumcomercial as telefone,
				iu.muncod as dibge,
				iu.mun_estuf as duf,
				e.entdatanasc as datanasc
			FROM 
				par.entidade e
			INNER JOIN par.instrumentounidade iu ON iu.inuid = e.inuid
			WHERE 
				dutid = 2 AND 
				entstatus = 'A' AND 
				iu.muncod = '".$ibge."'";
	} elseif($uf) {
		$sql = "SELECT 
				entnumcpfcnpj, 
				entnome, 
				entemail, 
				'(' || entnumdddcomercial || ')' || entnumcomercial as telefone,
				muncod as dibge,
				estuf as duf,
				entdatanasc as datanasc
			FROM 
				par.entidade
			WHERE 
				dutid = 10 AND 
				entstatus = 'A' AND 
				estuf = '".$uf."'";
	}
	
	if( $sql ){
		
		$dadosSecretario = $db->pegaLinha($sql);
		
		if( $dadosSecretario['entnumcpfcnpj'] ){
			
			extract($dadosSecretario);
		
$arqXml = <<<XML
<?xml version="1.0" encoding="UTF-8"?>
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
		<secretario>SIM</secretario>
		<cpf>$entnumcpfcnpj</cpf>
		<nome>$entnome</nome>
		<email>$entemail</email>
		<telefone>$telefone</telefone>
		<ibge>$dibge</ibge>
		<uf>$duf</uf>
		<datanasc>$datanasc</datanasc>
    </body>
</response>
XML;

		} else {
	
$arqXml = <<<XML
<?xml version="1.0" encoding="UTF-8"?>
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
			<text>Usuário não cadastrado como secretário.</text> 
        </message>
    </status>
</response>
XML;
			
		}
	} else {
		
$arqXml = <<<XML
<?xml version="1.0" encoding="UTF-8"?>
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
			<text>Dados não encontrados.</text> 
        </message>
    </status>
</response>
XML;

	}
	return $arqXml;
}

$server->register('retornaSecretarios',                			// method name
		array(	'PHPSESSID' 		=> 'xsd:string', 
				'esfera' 			=> 'xsd:string'), 
			//	'id_item' 			=> 'tns:itemlist'),     	// input parameters
		array('return' => 'xsd:string'),      					// output parameters
		'urn:retornaSecretarioswsdl',                      		// namespace
		'urn:retornaSecretarioswsdl#retornaSecretarios',   		// soapaction
		'rpc',                                					// style
		'encoded',                            					// use
		'Retornar secretários'									// documentation
);

function retornaSecretarios($PHPSESSID, $esfera) {
	
	$data_created = date("c");
	
	if ($PHPSESSID) {
		session_id($PHPSESSID);
	} else {
		
$arqXml = <<<XML
<?xml version="1.0" encoding="UTF-8"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <status>
        <result>0</result>
        <message>
            <code>0001</code>           
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
	
	if( $esfera == 'M' ){
		$sql = "SELECT 
					entnumcpfcnpj, 
					entnome, 
					entemail, 
					'(' || entnumdddcomercial || ')' || entnumcomercial as telefone,
					CASE WHEN iu.itrid = 2 THEN iu.muncod ELSE '' END as dibge,
					CASE WHEN iu.itrid = 2 THEN iu.mun_estuf ELSE iu.estuf END as duf,
					e.entdatanasc as datanasc,
					'Municipal' as tipo
				FROM 
					par.entidade e
				INNER JOIN par.instrumentounidade iu ON iu.inuid = e.inuid
				WHERE 
					dutid = 2 AND 
					entstatus = 'A'";
	} elseif( $esfera == 'E' ){
		$sql = "SELECT 
					entnumcpfcnpj, 
					entnome, 
					entemail, 
					'(' || entnumdddcomercial || ')' || entnumcomercial as telefone,
					muncod as dibge,
					estuf as duf,
					entdatanasc as datanasc,
					'Estadual' as tipo
				FROM 
					par.entidade e
				WHERE 
					dutid = 10 AND 
					entstatus = 'A' AND
					(muncod <> '' AND estuf <> '')";
	} else {
	
		$sql = "SELECT 
					entnumcpfcnpj, 
					entnome, 
					entemail, 
					'(' || entnumdddcomercial || ')' || entnumcomercial as telefone,
					muncod as dibge,
					estuf as duf,
					entdatanasc as datanasc,
					'Estadual' as tipo
				FROM 
					par.entidade e
				WHERE 
					dutid = 10 AND 
					entstatus = 'A' AND
					(muncod <> '' AND estuf <> '')
					
				UNION ALL
				
				SELECT 
					entnumcpfcnpj, 
					entnome, 
					entemail, 
					'(' || entnumdddcomercial || ')' || entnumcomercial as telefone,
					CASE WHEN iu.itrid = 2 THEN iu.muncod ELSE '' END as dibge,
					CASE WHEN iu.itrid = 2 THEN iu.mun_estuf ELSE iu.estuf END as duf,
					e.entdatanasc as datanasc,
					'Municipal' as tipo
				FROM 
					par.entidade e
				INNER JOIN par.instrumentounidade iu ON iu.inuid = e.inuid
				WHERE 
					dutid = 2 AND 
					entstatus = 'A'";
	}
	
	if( $sql ){
		
		$dadosSecretario = $db->carregar($sql);
		
		
$arqXmlSt = 
'<?xml version="1.0" encoding="UTF-8"?>
<response>
    <header>
        <app>SIMEC</app>
        <version>1.0</version>
        <created>'.$data_created.'</created>
    </header>
    <status>
        <result>1</result>
        <message>
            <code>0001</code>           
			<text>SUCESSO</text> 
        </message>
    </status>
    <body>';
	foreach( $dadosSecretario as $dado ){
    	$arqXmlSt .= '<parm>
						<secretario>SIM</secretario>
						<cpf>'.$dado['entnumcpfcnpj'].'</cpf>
						<nome>'.$dado['entnome'].'</nome>
						<email>'.$dado['entemail'].'</email>
						<telefone>'.$dado['telefone'].'</telefone>
						<ibge>'.$dado['dibge'].'</ibge>
						<uf>'.$dado['duf'].'</uf>
						<datanasc>'.$dado['datanasc'].'</datanasc>
						<tipo>'.$dado['tipo'].'</tipo>
			    	</parm>';
	}
$arqXmlSt .= '</body>
</response>';

$arqXml = <<<XML
{$arqXmlSt}
XML;

	
	} else {
		
$arqXml = <<<XML
<?xml version="1.0" encoding="UTF-8"?>
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
			<text>Dados não encontrados.</text> 
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