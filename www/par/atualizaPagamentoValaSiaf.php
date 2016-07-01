<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include  APPRAIZ."par/classes/Habilita.class.inc";
include_once APPRAIZ . "/includes/classes/Fnde_Webservice_Client.class.inc";

session_start();

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

// CPF do administrador de sistemas
/*if( empty($_SESSION['usucpforigem']) ){
	$_SESSION['usucpforigem'] = '';
	$_SESSION['usucpf'] = '';
}*/

$db = new cls_banco();

$obVala = new PagamentoValaSiaf();
$obVala->carregaPagamentoValaSiaf();

class PagamentoValaSiaf{
	private $wsusuario;
	private $wssenha;
	private $arrRetorno;
	private $prpid;
	private $xmlEnvio;
	private $xmlRetorno;
	
	function __construct(){
		$this->wsusuario 	= 'jcarlo';
		$this->wssenha 		= 'paulino1';
		$this->arrRetorno	= array();
		$this->xmlEnvio		= '';
		$this->xmlRetorno	= '';
	}
	
	public function carregaPagamentoValaSiaf(){
		global $db;
		
		$sql = "SELECT 
				  	cs.no_razao_social, cs.nu_cgc_entidade, cs.nu_processo, cs.nu_seq_conta_corrente, cs.nu_banco, cs.nu_agencia, cs.nu_conta_corrente,
				  	p.prpid, p.prpnumeroprocesso, p.muncod, p.prpbanco, p.prpagencia, p.prpdatainclusao, p.usucpf, p.prpseqconta, p.prptipo, p.seq_conta_corrente, p.nu_conta_corrente, p.inuid,
				  	p.prptipoexecucao, p.prpnumeroconveniofnde, p.prpanoconveniofnde, p.prpnumeroconveniosiafi, p.prpdocumenta, p.prpcnpj, p.sisid, p.prpstatus, p.dt_movimento, p.fase_solicitacao,
				  	p.co_situacao_conta, p.situacao_conta, p.nu_identificador, p.ds_razao_social, p.prpgeraproc, p.arqidanexodoc, p.prpstatusmotivo
				FROM 
					par.processopar p
				  	left JOIN carga.contasigef cs on p.prpnumeroprocesso = cs.nu_processo
				WHERE 
					p.prpstatus = 'A'
					and p.prpnumeroprocesso = '23400009789201207'";
		//cs.nu_processo in ('23400011440201227', '23400011434201270') Julio Enviou
		//, '23400000217201254', '23400000182201253'
		$arrDados = $db->carregar($sql);
		$arrDados = $arrDados ? $arrDados : array();
		
		$this->consultarContaCorrente( array('prpseqconta' => '3578362') );
		
		ver($arrDados,d);
		foreach ($arrDados as $key => $v) {
			$this->prpid = $v['prpid'];
			
			$retCC = $this->consultarContaCorrente($v);
			
			//$retCC = $this->consultarAndamentoContaCorrente($v);
			if($retCC == '25'){
				$this->solicitarContaCorrente($v);
			} else {
				$this->arrRetorno[]= array(
									'funcao' => 'SOLICITAÇÃO DE CONTA CORRENTE',
									'xmlEnvio' => '',
									'xmlRetorno' => '',
									'dados' => $v
									);
			}
		}
		$this->montaTabelaRetorno();
	}
	
	private function consultarAndamentoContaCorrente($dados) {
		global $db;
	
		$data_created 			= date("c");
    	$usuario 				= $this->wsusuario; 
		$senha   				= $this->wssenha;
		$nu_identificador 		= $dados['nu_cgc_entidade'];
		$ptrnumprocessoempenho 	= $dados['nu_processo'];
		$somente_conta_ativa	= "N";
		$numero_de_linhas		= "200";
				
		try {
    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$data_created}</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<nu_identificador>$nu_identificador</nu_identificador>
			<nu_processo>$ptrnumprocessoempenho</nu_processo>
			<somente_conta_ativa>$somente_conta_ativa</somente_conta_ativa>
			<numero_de_linhas>$numero_de_linhas</numero_de_linhas>
		</params>
	</body>
</request>
XML;
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultarAndamentoCC') )
					->execute();
	
			$xmlRetorno = $xml;
	
			$xmlRetorno = $xml;
			
			$this->xmlEnvio 	= $arqXml;
			$this->xmlRetorno 	= $xmlRetorno;
			
		    $xml = simplexml_load_string( stripslashes($xml));
			ver($xml,d);
			$this->arrRetorno[]= array(
									'funcao' => 'CONSULTAR CONTA CORRENTE',
									'xmlEnvio' => $arqXml,
									'xmlRetorno' => $xmlRetorno,
									'dados' => $dados				
									);
	
			$result = (integer) $xml->status->result;
	
			if($result) {
				
				$status 	= (string)$xml->body->row->status;
				$co_status	= substr( $status, 0, 1 );
								
				$this->gravaLog('consultaContaCorrenteCarga - Sucesso');	
				$db->commit();
				return $co_status;
			} else {
				$this->gravaLog('consultaContaCorrenteCarga - Erro');	
			   	return false;
			}
	
	
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Erro-Serviço Cancelar Pagamento encontra-se temporariamente indisponível. Favor tente mais tarde.".'\n';
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
		}
	}
	
	private function consultarContaCorrente($dados) {
		global $db;
	
		try {
			
			$data_created 	= date("c");
			$usuario 		= $this->wsusuario; 
			$senha   		= $this->wssenha;	
	        $prpseqconta 	= $dados['prpseqconta'];
	        
			
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
			<seq_solic_cr>$prpseqconta</seq_solic_cr>
		</params>
	</body>
</request>
XML;

			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
					->execute();
	
			$xmlRetorno = $xml;
			$this->xmlEnvio 	= $arqXml;
			$this->xmlRetorno 	= $xmlRetorno;
						
			$this->arrRetorno[]= array(
									'funcao' => 'CONSULTAR CONTA CORRENTE',
									'xmlEnvio' => $arqXml,
									'xmlRetorno' => $xmlRetorno,
									'dados' => $dados				
									);
	
		    $xml = simplexml_load_string( stripslashes($xml));
		    ver($xml,d);
		    if( (int)$xml->status->result == 1 ){
		    	$co_situacao_conta = (string)$xml->body->row->co_situacao_conta;
		    	$this->gravaLog('consultaContaCorrenteCarga - Sucesso');
		    	return $co_situacao_conta;
		    } else {
		    	$this->gravaLog('consultaContaCorrenteCarga - Erro');
		    	return false;
		    }
	
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';				
			}
			$this->gravaLog('solicitarContaCorrenteCarga - Erro');
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );	
		}
	}
	
	private function solicitarContaCorrente($dados) {
		global $db;
		
		try {
	
			$data_created 			= date("c");
			$usuario 				= $this->wsusuario;
			$senha   				= $this->wssenha;			
		    $nu_processo			= $dados['nu_processo'];
		    $nu_banco				= $dados['nu_banco'];
		    $nu_agencia				= trim((int)$dados['nu_agencia']);
	        
			$obHabilita 			= new Habilita();
			$nu_identificador 		= $dados['nu_cgc_entidade'];
	        $tp_identificador		= "1";
	        $nu_conta_corrente		= null;//$dados['nu_conta_corrente'];
	        $tp_solicitacao			= "01";
	        $motivo_solicitacao		= "0032";
	        $convenio_bb			= null;
	        $tp_conta				= "N";
	        	        
			if($dados['prptipoexecucao'] == 'C'){
		   		$co_programa_fnde 	= "03";
		   		$nu_sistema			= "2";
		    }else if($dados['prptipoexecucao'] == 'T'){ 
		        $co_programa_fnde	= "CM";
		        $nu_sistema			= "7";
		    }

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

	    //	if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
		//	   $_SESSION['baselogin'] == "simec_espelho_producao" ){
		//		$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
		//	} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
				//$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
		//	}
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute();
	
			$xmlRetorno = $xml;
			$this->xmlEnvio 	= $arqXml;
			$this->xmlRetorno 	= $xmlRetorno;
						
			$this->arrRetorno[]= array(
									'funcao' => 'SOLICITAÇÃO DE CONTA CORRENTE',
									'xmlEnvio' => $arqXml,
									'xmlRetorno' => $xmlRetorno,
									'dados' => $dados				
									);
									
			$result = (int) $xml->status->result;
			
			$db->executar("UPDATE par.processopar SET 
						    	prpseqconta = '".(string)$xml->body->seq_solic_cr."', 
						    	seq_conta_corrente = '".(string)$xml->body->nu_seq_conta."' 
						    WHERE prpid = '".$dados['prpid']."'");
			$db->commit();
			$this->gravaLog('solicitarContaCorrenteCarga - Sucesso');
	
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
				
			}
			$this->gravaLog('solicitarContaCorrenteCarga - Erro');
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
	
		}
	}
	
	private function gravaLog($tipo){
		global $db;
		
		$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$this->prpid."',
				    		'{$tipo}',
				    		'".addslashes($this->xmlEnvio)."',
				    		'".addslashes($this->xmlRetorno)."',
				    		NOW(),
				            '');";

		$db->executar($sql);
		$db->commit();
	}
	
	private function montaTabelaRetorno(){
		monta_titulo('Pagamentos', '');
		
		$html = '<table align="center" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
				<tr>
					<th width="10%">Processo</th>
					<th width="10%">CNPJ</th>
					<th width="25%">Entidade</th>
					<th width="15%">funcao</th>
					<th width="40%">Mensagem Retorno</th>
				</tr>';
		$arrResponse = $this->arrRetorno;
		
		foreach ($arrResponse as $key => $retorno) {
			
			if( $retorno['xmlRetorno'] ){
				$retornoXML = simplexml_load_string( stripslashes($retorno['xmlRetorno']));
				
				if($retornoXML->status->result == '0'){
					if( is_array($retornoXML->status->error->message) ){
						$strMensagem = implode('<br>', $retornoXML->status->error->message);
					} else {
						if( $retorno['funcao'] == 'CONSULTAR CONTA CORRENTE' ){
							$strMensagem = utf8_decode((string)$retornoXML->body->row->situacao_conta);
						} else {
							$strMensagem = utf8_decode($retornoXML->status->error->message->text);
						}
					}
					$mensagem = $strMensagem;
					$corTD = 'red';
				} else {
					$corTD = 'blue';
					if( $retorno['funcao'] == 'CONSULTAR CONTA CORRENTE' ){
						$situacao_conta = utf8_decode((string)$retornoXML->body->row->situacao_conta);
						$fase_solicitacao = utf8_decode((string)$retornoXML->body->row->fase_solicitacao);
						
						$mensagem = ($situacao_conta ? $situacao_conta : $fase_solicitacao);
					} else {
						$mensagem = $retornoXML->status->message->text;
					}
				}
			} else {
				$mensagem = 'Conta corrente já existe e está ativa no SIGEF.';
				$corTD = 'blue';
			}
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
						<td>'.$retorno['dados']['nu_processo'].'</td>
						<td>'.formatar_cpf_cnpj($retorno['dados']['nu_cgc_entidade']).'</td>
						<td>'.$retorno['dados']['no_razao_social'].'</td>
						<td>'.$retorno['funcao'].'</td>
						<td style="color: '.$corTD.';">'.$mensagem.'</td>
					</tr>';
		}
		$html.= '</table>';
		
		echo $html;
	}
}
?>