<?php
//ini_set('max_execution_time', 0);
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/curl_parallel/sender.class.php";
include_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
include_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

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

		$sql = "select distinct
					pag.pagid
				from
					par.processoobra pro
					inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
					inner join par.pagamento pag on pag.empid = emp.empid and pag.pagstatus = 'A'
				where
					pro.prostatus = 'A'
					and emp.empstatus = 'A'
					and cast(to_char(coalesce(pag.pagdataatualizacao, '1900-01-01'), 'YYYY-MM-DD') as date) <> cast(to_char(now(), 'YYYY-MM-DD') as date)
				order by pag.pagid";
		
		$arrEmp = $db->carregarColuna($sql);
		$arrEmp = $arrEmp ? $arrEmp :array();
		
		$qtdEmp = 4000;
		$contador = $qtdEmp;
		$strEmpenho = '';
		$limitEmp = array();

		foreach ($arrEmp as $key => $pagid) {
			if( empty($strEmpenho) ){
				$strEmpenho = $pagid;
			} else {
				$strEmpenho = $strEmpenho . ', ' . $pagid;
			}
			
			if( ((int)$contador - 1) == $key ){
				array_push($limitEmp, $strEmpenho);
				$strEmpenho = '';
				$contador = $contador + $qtdEmp;
			}
			if( $key == sizeof($arrEmp)-1 && !empty($strEmpenho) ){
				array_push($limitEmp, $strEmpenho);
				$strEmpenho = '';
				$contador = 0;
			}
		}

		$totalUrl = sizeof($limitEmp);

		$db->executar("delete from par.pagamento_temp where sistema = 'PAC'");

		foreach ($limitEmp as $key => $pagid) {
			$sql = "INSERT INTO par.pagamento_temp(codigo, pagamento, quantidade, sistema)
					VALUES($key, '$pagid', ".sizeof(explode(', ', $pagid)).", 'PAC')";
			$db->executar($sql);
		}
		$db->commit();

		$arUrls = array();
		$strUrl = '';
		for ($i=0; $i<$totalUrl; $i++){
			$urls = $_SERVER['SERVER_NAME'].':'.$_SERVER['SERVER_PORT'].'/seguranca/scripts_exec/par_atualiza_pagamento_sigef.php?sistema=PAC&cont='.$i;
			array_push($arUrls, $urls);
		}
		$this->url_list = $arUrls;
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

$strMensagem 	= implode('<br>', $tc->retorno);
$strAssunto  	= SIGLA_SISTEMA. " - Atualização de Pagamento OBRA PAC SIGEF";
$remetente 		= array("nome"=>SIGLA_SISTEMA. " - SIGEF", "email"=>"noreply@mec.gov.br");
$strEmailTo 	= array($_SESSION['email_sistema']);

enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem, '', '', array() );
?>