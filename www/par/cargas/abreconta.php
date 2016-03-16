<?php
set_time_limit(0);
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "/includes/classes/Fnde_Webservice_Client.class.inc";

$db = new cls_banco();

//echo "Carregando Processos...";

$sql = "select proid, protipo, muncod, pronumeroprocesso, procnpj, probanco, proagencia, empenho as tipo
		from (
			select distinct po.proid, po.muncod, po.pronumeroprocesso, po.proseqconta, po.seq_conta_corrente, po.probanco, po.proagencia, protipo, e.empprogramafnde as empenho, 
			replace(replace(replace(replace(replace(replace(replace( replace( replace( substr(hwpxmlenvio, 793, 8) , 'fnde>', '')  , '<', '')  , '>', '')  , '/co', '')   , '/co_p', '') , '_p', '') , 'nde', '')
			, '/', ''), '_', '') as abertura,
			 hwpwebservice, procnpj
			from par.processoobra po
			inner join par.empenho e on e.empnumeroprocesso = po.pronumeroprocesso and empstatus = 'A'
			inner join par.historicowsprocessoobra h on h.proid = po.proid
			where po.prostatus = 'A'  and ( hwpwebservice ilike '%GERAPROC - SolicitarContaCorrente - Sucesso%'   or hwpwebservice ilike '%solicitarContaCorrente - Sucesso%' ) 
			and po.proid not in ( 	select proid from par.pagamento p 
						inner join par.empenho e on e.empid = p.empid and empstatus = 'A'
						inner join par.processoobra pr on e.empnumeroprocesso = pr.pronumeroprocesso pr.prostatus = 'A' 
						where pagstatus = 'A' )
			order by po.pronumeroprocesso
		) as foo
		where abertura <> empenho
		-- and proid = 6158
		and proid not in ( 3808, 6160, 6158 )  -- and proid = 5037 ";

$dados =  $db->carregar($sql);

//echo "Processos carregados do Banco <br>";

$resposta =  array('municipio'=>null, 'cnpj'=>null, 'mensagem SIGEF'=>null, 'sucesso'=>null);


foreach($dados as $dado){
	$result[] = solicitarContaCorrente($dado, $resposta);
	$contador++;
}
$cabecalho = array('municipio', 'cnpj', 'mensagem SIGEF', 'sucesso');
$db->monta_lista_array($result, $cabecalho, 1000, 5, N,'center');

function solicitarContaCorrente($dados, $resposta) {
	$erro = array();
	global $db;
	try {
		$data_created 		= date("c");
		$usuario 			= 'juliov';
		$senha   			= '1segredo';
		
		//$usuario 			= 'asd';
		//$senha   			= 'asd';
		
		$nu_identificador 	= $dados["procnpj"];
		$tp_identificador	= "1";
		$nu_processo		= $dados["pronumeroprocesso"];
		$nu_banco			= $dados["probanco"];
		$nu_agencia			= $dados["proagencia"];
		$nu_conta_corrente	= null;
		$tp_solicitacao		= "01";
		$motivo_solicitacao = "0032";
		$convenio_bb		= null;
		$tp_conta			= "N";
		$nu_sistema			= "5";
		if($dados['protipo'] == 'P'){
			$co_programa_fnde="BW"; // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
		}else{
			$co_programa_fnde="CN";
		}

        /*
        $dadoscc = $db->pegaLinha("SELECT pronumeroprocesso, probanco, proagencia, muncod, protipo FROM par.processoobra WHERE prostatus = 'A'  and  proid='".$dados['proid']."'");
        if($dadoscc) {
	       
	        $nu_processo	=	$dadoscc['pronumeroprocesso'];
	        $nu_banco		=	$dadoscc['probanco'];
	        $nu_agencia		=	$dadoscc['proagencia'];
        }
		$nu_identificador=$db->pegaUm("	SELECT trim(procnpj) FROM par.processoobra WHERE prostatus = 'A'  and pronumeroprocesso =  '{$dadoscc['pronumeroprocesso']}'");
		*/
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
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_identificador>$nu_identificador</nu_identificador>
        <tp_identificador>$tp_identificador</tp_identificador>
        <nu_processo>$nu_processo</nu_processo>
        <nu_banco>$nu_banco</nu_banco>
        <nu_agencia>$nu_agencia</nu_agencia>
        <nu_conta_corrente>$nu_conta_corrente</nu_conta_corrente>
        <tp_solicitacao>$tp_solicitacao</tp_solicitacao>
        <motivo_solicitacao>$motivo_solicitacao</motivo_solicitacao>
        <convenio_bb>$convenio_bb</convenio_bb>
        <tp_conta>$tp_conta</tp_conta>
        <nu_sistema>$nu_sistema</nu_sistema>
        <co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;

//TESTE para visualizacao do xml
/*
ob_get_clean();
header( "Content-Type: text/xml" );
echo $arqXml;
exit();
*/  
		
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;
		if(!$result) {
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					$erro[] =  iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}

			$sql = "INSERT INTO par.historicowsprocessoobra(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$dados['proid']."',
				    		'solicitarContaCorrente - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();
			
			
			$resposta['municipio'] = $dados["muncod"];
			$resposta['cnpj'] = $nu_identificador;
			$resposta['mensagem SIGEF'] = $xml->status->message->code." - ".$xml->status->message->text."\n\n - ".implode(',' , $erro ) ." - ".$erros;
			$resposta['sucesso'] = 'Erro';
		    return $resposta;
		} else {

		    $db->executar("UPDATE par.processoobra SET proseqconta='".$xml->body->seq_solic_cr."', seq_conta_corrente='".$xml->body->nu_seq_conta."' WHERE proid='".$dados['proid']."'");

			$sql = "INSERT INTO par.historicowsprocessoobra(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$dados['proid']."',
				    		'solicitarContaCorrente - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();
			
			$resposta['municipio'] = $dados["muncod"];
			$resposta['cnpj'] = $nu_identificador;
			$resposta['mensagem SIGEF'] = $xml->status->message->code." - ".$xml->status->message->text."\n\n";
			$resposta['sucesso'] = 'Sucesso';


			return $resposta;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";


	}
}
?>