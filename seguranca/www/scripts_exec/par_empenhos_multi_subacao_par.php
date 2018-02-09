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

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

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
					emp.empid
				from
					par.processopar pro
					inner join par.empenho emp on emp.empnumeroprocesso = pro.prpnumeroprocesso
				where
					pro.prpstatus = 'A'
					and emp.empstatus = 'A'
					and cast(to_char(coalesce(emp.empdataatualizacao, '1900-01-01'), 'YYYY-MM-DD') as date) <> cast(to_char(now(), 'YYYY-MM-DD') as date)
				order by emp.empid";
		
		$arrEmp = $db->carregarColuna($sql);
		$arrEmp = $arrEmp ? $arrEmp :array();
		
		$qtdEmp = 4000;
		$contador = $qtdEmp;
		$strEmpenho = '';
		$limitEmp = array();

		foreach ($arrEmp as $key => $empid) {
			if( empty($strEmpenho) ){
				$strEmpenho = $empid;
			} else {
				$strEmpenho = $strEmpenho . ', ' . $empid;
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

		//ver($limitEmp,d);
		$db->executar("delete from par.empenho_temp where sistema = 'PAR'");

		foreach ($limitEmp as $key => $empid) {
			$sql = "insert into par.empenho_temp(codigo, empenho, quantidade, sistema)
					values($key, '$empid', ".sizeof(explode(', ', $empid)).", 'PAR')";
			$db->executar($sql);
		}
		$db->commit();

		$arUrls = array();
		$strUrl = '';
		for ($i=0; $i<$totalUrl; $i++){
			$urls = $_SERVER['SERVER_NAME'].':'.$_SERVER['SERVER_PORT'].'/seguranca/scripts_exec/par_atualiza_empenho_sigef.php?sistema=PAR&cont='.$i;
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
$strAssunto  = SIGLA_SISTEMA. " - Atualização de Empenho SUBAÇÃO SIGEF";
$remetente 		= array("nome"=>SIGLA_SISTEMA. " - SIGEF", "email"=>"noreply@mec.gov.br");
$strEmailTo 	= array($_SESSION['email_sistema']);
enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem, '', '', array() );

$db->close();