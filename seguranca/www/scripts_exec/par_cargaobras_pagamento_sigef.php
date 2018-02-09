<?php

/**** INCLUDES ****/

ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";

include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes/curl_parallel/sender.class.php";

/**** DECLARAÇÃO DE VARIAVEIS ****/
session_start();

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db = new cls_banco();

class TestCurl implements iSenderConsumer {
	private $url_list = array();
	public 	$retorno = array();
  
	public function __construct(Sender $sender) {
		// read urls from a file, one by one
		$this->readUrls();
		$this->sender = $sender;
		
		foreach ($this->url_list as $url) {
			if($url == '') continue;
			$curlo = $this->sender->addRecipient($url, $this);
		}
	}
  
 	public function readUrls() {
  		global $db;
    
	  	$sql = "select
					count(empid) as total,
				    ano
				from(
				    select distinct
				        emp.empid,
				        substring(emp.empnumeroprocesso, 12, 4) as ano
				    from
				        par.processoobraspar pro
				        inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
                        inner join par.pagamento pag on pag.empid = emp.empid and pag.pagstatus = 'A'
				    where
				        pro.prostatus = 'A'
				        and emp.empstatus = 'A'
	  					and cast(to_char(coalesce(prodatapagamentosigef, '1900-01-01'), 'YYYY-MM-DD') as date) <> cast(to_char(now(), 'YYYY-MM-DD') as date)
				    order by emp.empid
				) as foo
				group by ano 
				order by ano";
		$arrEmpenho = $db->carregar($sql);
		$arrEmpenho = $arrEmpenho ? $arrEmpenho : array();
		
		$arUrls = array();
		$strUrl = '';
		foreach ($arrEmpenho as $v) {	
			$urls = $_SERVER['SERVER_NAME'].':'.$_SERVER['SERVER_PORT'].'/seguranca/scripts_exec/carga_ws_pagamento_simec_sigef.php?sistema=OBRAS&ano='.$v['ano'].'&qtdregistro='.$v['total'];
			array_push($arUrls, $urls);
		}
  		$this->url_list = $arUrls; //explode("\n", $c);
  	}
   
	public function consumeCurlResponse(HttpResponse $object,Curl $curlo = NULL) {
		// I just want to know if all goes right
		$strMensagem = $dataInicio = date("d/m/Y h:i:s") . " - " .$object->header_first_row. ' - ' .$object->getResponseCode() . " with a content of length: " . strlen($object->content)." requested url: ". $curlo->getUrl() ."<br>";
		if($object->getResponseCode() != 200) {
			$strMensagem = $object->content;
			$strMensagem = $object->raw_headers;
		}
		
		array_push($this->retorno, $strMensagem);
	}
}

$sender = new Sender();
$tc = new TestCurl($sender);
$sender->execute();

$strMensagem = '';
if( is_array($tc->retorno) ){
	$strMensagem 	= implode('<br>', $tc->retorno);
}
$strAssunto  = SIGLA_SISTEMA. " - Carga de Processo de Empenho OBRA PAR SIGEF";
$remetente 		= array("nome"=>SIGLA_SISTEMA. " - SIGEF", "email"=>"noreply@mec.gov.br");
$strEmailTo 	= array($_SESSION['email_sistema']);
//enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem, '', '', array() );

$db->close();
?>