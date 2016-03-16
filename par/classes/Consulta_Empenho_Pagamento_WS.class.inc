<?php
include_once APPRAIZ . "/includes/classes/Fnde_Webservice_Client.class.inc";

class Consulta_Empenho_Pagamento_WS{
	private $wsusuario, $wssenha, $chave, $tabelahistorico, $sistema, $offset, $empenho, $pagamento;
	
	function __construct( $arrParam = array() ){
		
		$this->wsusuario 	= $arrParam['wsusuario'];
		$this->wssenha		= $arrParam['wssenha'];
		$this->sistema		= $arrParam['sistema'];
		$this->offset		= $arrParam['offset'];
		$this->empenho		= $arrParam['empenho'];
		$this->pagamento	= $arrParam['pagamento'];
	}
	
	public function consultaEmpenho(){
		
		if( $this->sistema == 'PAR' ){
			$this->chave			= 'prpid';
			$this->tabelahistorico 	= 'par.historicowsprocessopar';
				
			$this->atualizaContaCorrentePAR();
			$this->atualizaEmpenhoPAR();
		}
		if( $this->sistema == 'PAC' ){
			$this->chave			= 'proid';
			$this->tabelahistorico 	= 'par.historicowsprocessoobra';
				
			$this->atualizaContaCorrentePAC();
			$this->atualizaEmpenhoPAC();
		}
		if( $this->sistema == 'OBRAS' ){
			$this->chave			= 'proid';
			$this->tabelahistorico 	= 'par.historicowsprocessoobrapar';
				
			$this->atualizaContaCorrenteOBRAS();
			$this->atualizaEmpenhoOBRAS();
		}
	}
	
	public function consultaPagamento(){
		
		if( $this->sistema == 'PAR' ){
			$this->chave			= 'prpid';
			$this->tabelahistorico 	= 'par.historicowsprocessopar';
		
			$this->atualizaPagamentoPAR();
		}
		if( $this->sistema == 'PAC' ){
			$this->chave			= 'proid';
			$this->tabelahistorico 	= 'par.historicowsprocessoobra';
		
			$this->atualizaPagamentoPAC();
		}
		if( $this->sistema == 'OBRAS' ){
			$this->chave			= 'proid';
			$this->tabelahistorico 	= 'par.historicowsprocessoobrapar';
		
			$this->atualizaPagamentoOBRAS();
		}
	}
	
	public function atualizaContaCorrentePAR(){
		global $db;
		
		$sql = "SELECT prpseqconta, prpnumeroprocesso, prpid
				FROM par.processopar
				WHERE prpstatus = 'A'
					and nu_conta_corrente IS NULL
					AND seq_conta_corrente IS NOT NULL";
	
		$arrContas = $db->carregar($sql);
		$arrContas = $arrContas ? $arrContas : array();
	
		foreach($arrContas as $conta) {
			$parametros = array('wsusuario' 		=> $this->wsusuario,
								'wssenha'			=> $this->wssenha,
								'sequencial'		=> $conta['prpseqconta'],
								'processo'			=> $conta['prpnumeroprocesso'],
								'codigo'			=> $conta['prpid']
			);
			$this->consultarContaCorrente($parametros);
		}
		return true;
	}
	
	public function atualizaContaCorrenteOBRAS(){
		global $db;
	
		$sql = "SELECT proseqconta, pronumeroprocesso, proid
				FROM par.processoobraspar
				WHERE prostatus = 'A'
					and nu_conta_corrente IS NULL
					AND seq_conta_corrente IS NOT NULL";
	
		$arrContas = $db->carregar($sql);
		$arrContas = $arrContas ? $arrContas : array();
	
		foreach($arrContas as $conta) {
			$parametros = array('wsusuario' 		=> $this->wsusuario,
					'wssenha'			=> $this->wssenha,
					'sequencial'		=> $conta['proseqconta'],
					'processo'			=> $conta['pronumeroprocesso'],
					'codigo'			=> $conta['proid']
			);
			$this->consultarContaCorrente($parametros);
		}
		return true;
	}
	
	public function atualizaContaCorrentePAC(){
		global $db;
		
		$sql = "SELECT proseqconta, pronumeroprocesso, proid
				FROM par.processoobra
				WHERE prostatus = 'A'
					and  nu_conta_corrente IS NULL
				    and seq_conta_corrente IS NOT NULL";
		
		$arrContas = $db->carregar($sql);
		$arrContas = $arrContas ? $arrContas : array();
		
		foreach($arrContas as $conta) {	
			$parametros = array('wsusuario' 		=> $this->wsusuario,
								'wssenha'			=> $this->wssenha,
								'sequencial'		=> $conta['proseqconta'],
								'processo'			=> $conta['pronumeroprocesso'],
								'codigo'			=> $conta['proid']
							);
			$this->consultarContaCorrente($parametros);
		}
		return true;
	}
	
	public function atualizaEmpenhoPAR(){
		global $db;
				
		$sql = "select
					pro.prpid,
					pro.prpnumeroprocesso,
				    emp.empprotocolo,
				    emp.empid,
				    emp.empsituacao,
					emp.empstatus
				from
					par.processopar pro
				    inner join par.empenho emp on emp.empnumeroprocesso = pro.prpnumeroprocesso
				where
					pro.prpstatus = 'A'
				    and emp.empstatus = 'A'
					and emp.empid in (".$this->empenho.")";	
		
		$arrEmpenhos = $db->carregar($sql);
		$arrEmpenhos = $arrEmpenhos ? $arrEmpenhos : array();
		
	 	foreach($arrEmpenhos as $empenho) {
			
			$parametros = array('wsusuario' 	=> $this->wsusuario,
								'wssenha'		=> $this->wssenha,
								'codigo'	   	=> $empenho['prpid'],
								'empid'		   	=> $empenho['empid'],
								'processo'		=> $empenho['prpnumeroprocesso'],
								'empprotocolo'  => $empenho['empprotocolo'],
								'empsituacao'   => $empenho['empsituacao'],
								'empstatus'   	=> $empenho['empstatus']
							);	
			$this->consultarEmpenhoSIGEF($parametros);
		}
		return true;
	}
	
	public function atualizaEmpenhoOBRAS(){
		global $db;
				
		$sql = "select
					pro.proid,
					pro.pronumeroprocesso,
				    emp.empprotocolo,
				    emp.empid,
				    emp.empsituacao,
					emp.empstatus
				from
					par.processoobraspar pro
				    inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
				where
					pro.prostatus = 'A'
				    and emp.empstatus = 'A'
					and emp.empid in (".$this->empenho.")";	
		
		$arrEmpenhos = $db->carregar($sql);
		$arrEmpenhos = $arrEmpenhos ? $arrEmpenhos : array();
		
	 	foreach($arrEmpenhos as $empenho) {
			
			$parametros = array('wsusuario' 	=> $this->wsusuario,
								'wssenha'		=> $this->wssenha,
								'codigo'	   	=> $empenho['proid'],
								'empid'		   	=> $empenho['empid'],
								'processo'		=> $empenho['pronumeroprocesso'],
								'empprotocolo'  => $empenho['empprotocolo'],
								'empsituacao'   => $empenho['empsituacao'],
								'empstatus'   	=> $empenho['empstatus']
							);	
			$this->consultarEmpenhoSIGEF($parametros);
		}
		return true;
	}
	
	public function atualizaEmpenhoPAC(){
		global $db;
		
		$sqlAt = '';
		
		$sql = "select
					pro.proid,
					pro.pronumeroprocesso,
				    emp.empprotocolo,
				    emp.empid,
				    emp.empsituacao,
					emp.empstatus
				from
					par.processoobra pro
				    inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
				where
					pro.prostatus = 'A'
				    and emp.empstatus = 'A'
					and emp.empid in (".$this->empenho.")";	
		
		$arrEmpenhos = $db->carregar($sql);
		$arrEmpenhos = $arrEmpenhos ? $arrEmpenhos : array();
		
	 	foreach($arrEmpenhos as $empenho) {
			
			$parametros = array('wsusuario' 	=> $this->wsusuario,
								'wssenha'		=> $this->wssenha,
								'codigo'	   	=> $empenho['proid'],
								'empid'		   	=> $empenho['empid'],
								'processo'		=> $empenho['pronumeroprocesso'],
								'empprotocolo'  => $empenho['empprotocolo'],
								'empsituacao'   => $empenho['empsituacao'],
								'empstatus'   	=> $empenho['empstatus']
							);	
			$this->consultarEmpenhoSIGEF($parametros);
		}
		return true;
	}
	
	public function atualizaPagamentoPAR(){
		global $db;
		
		$sql = "select
					pro.prpid,
					pro.prpnumeroprocesso,
				    emp.empid,
		            pag.pagid,
		            pag.pagsituacaopagamento,
		            pag.parnumseqob,
					pag.pagparcela,
					pag.pagstatus
				from 
					par.processopar pro
				    inner join par.empenho emp on emp.empnumeroprocesso = pro.prpnumeroprocesso
		            inner join par.pagamento pag on pag.empid = emp.empid and pag.pagstatus = 'A'
				where
					pro.prpstatus = 'A'
				    and emp.empstatus = 'A'
		            and pag.pagid in (".$this->pagamento.")";	
		
		$arrPagamentos = $db->carregar($sql);
		$arrPagamentos = $arrPagamentos ? $arrPagamentos : array();
		
		foreach($arrPagamentos as $pagamento) {
	
			$parametros = array('wsusuario' 			=> $this->wsusuario,
								'wssenha'				=> $this->wssenha,
								'codigo'	   			=> $pagamento['prpid'],
								'empid'		   			=> $pagamento['empid'],
								'pagid'		   			=> $pagamento['pagid'],
								'processo'				=> $pagamento['prpnumeroprocesso'],
								'parnumseqob'  			=> $pagamento['parnumseqob'],
								'pagparcela'  			=> $pagamento['pagparcela'],
								'pagstatus'  			=> $pagamento['pagstatus'],
								'pagsituacaopagamento'  => $pagamento['pagsituacaopagamento']
								);
			$this->consultarPagamentoSIGEF($parametros);
		}
	}
	
	public function atualizaPagamentoOBRAS(){
		global $db;
		
		$sql = "select
					pro.proid,
					pro.pronumeroprocesso,
				    emp.empid,
		            pag.pagid,
		            pag.pagsituacaopagamento,
		            pag.parnumseqob,
					pag.pagparcela,
					pag.pagstatus
				from 
					par.processoobraspar pro
				    inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
		            inner join par.pagamento pag on pag.empid = emp.empid
				where
					pro.prostatus = 'A'
				    and emp.empstatus = 'A'
		            and pag.pagstatus = 'A'
		            and pag.pagid in (".$this->pagamento.")";
		
		$arrPagamentos = $db->carregar($sql);
		$arrPagamentos = $arrPagamentos ? $arrPagamentos : array();
		
		foreach($arrPagamentos as $pagamento) {
	
			$parametros = array('wsusuario' 			=> $this->wsusuario,
								'wssenha'				=> $this->wssenha,
								'codigo'	   			=> $pagamento['proid'],
								'empid'		   			=> $pagamento['empid'],
								'pagid'		   			=> $pagamento['pagid'],
								'processo'				=> $pagamento['pronumeroprocesso'],
								'parnumseqob'  			=> $pagamento['parnumseqob'],
								'pagparcela'  			=> $pagamento['pagparcela'],
								'pagstatus'  			=> $pagamento['pagstatus'],
								'pagsituacaopagamento'  => $pagamento['pagsituacaopagamento']
								);
			$this->consultarPagamentoSIGEF($parametros);
		}
	}
	
	public function atualizaPagamentoPAC(){
		global $db;
		
		$sql = "select
					pro.proid,
					pro.pronumeroprocesso,
				    emp.empid,
		            pag.pagid,
		            pag.pagsituacaopagamento,
		            pag.parnumseqob,
					pag.pagparcela,
					pag.pagstatus
				from 
					par.processoobra pro
				    inner join par.empenho emp on emp.empnumeroprocesso = pro.pronumeroprocesso
		            inner join par.pagamento pag on pag.empid = emp.empid
				where
					pro.prostatus = 'A'
				    and emp.empstatus = 'A'
		            and pag.pagstatus = 'A'
		            and pag.pagid in (".$this->pagamento.")";
		
		$arrPagamentos = $db->carregar($sql);
		$arrPagamentos = $arrPagamentos ? $arrPagamentos : array();
		
		foreach($arrPagamentos as $pagamento) {
	
			$parametros = array('wsusuario' 			=> $this->wsusuario,
								'wssenha'				=> $this->wssenha,
								'codigo'	   			=> $pagamento['proid'],
								'empid'		   			=> $pagamento['empid'],
								'pagid'		   			=> $pagamento['pagid'],
								'processo'				=> $pagamento['pronumeroprocesso'],
								'parnumseqob'  			=> $pagamento['parnumseqob'],
								'pagparcela'  			=> $pagamento['pagparcela'],
								'pagstatus'  			=> $pagamento['pagstatus'],
								'pagsituacaopagamento'  => $pagamento['pagsituacaopagamento']
								);
			$this->consultarPagamentoSIGEF($parametros);
		}
	}
	
	private function consultarContaCorrente($dados) {
		global $db;
		
		try {
	
			$data_created 	= date("c");
			$usuario 		= $dados['wsusuario'];
			$senha   		= $dados['wssenha'];
			$sequencial   	= $dados['sequencial'];
			$codigo		   	= $dados['codigo'];
	
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
        <seq_solic_cr>$sequencial</seq_solic_cr>
		</params>
	</body>
</request>
XML;
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
	
			if($sequencial) {
	
				$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
				->execute();
	
				$xmlRetorno = $xml;
	
				$xml = simplexml_load_string( stripslashes($xml));
				
				$result = (integer) $xml->status->result;
	
				if($result) {
					if($xml->body->row->seq_conta) {
						if( $this->sistema == 'PAR' ){
							$sql = "UPDATE par.processopar SET
									nu_conta_corrente	= '".$xml->body->row->nu_conta_corrente."',
									seq_conta_corrente	= '".$xml->body->row->seq_conta."'
								WHERE prpseqconta = '".$sequencial."'
													and prpid = $codigo";
					
						}elseif( $this->sistema == 'OBRAS' ){
							$sql = "UPDATE par.processoobraspar SET
		    						nu_conta_corrente	= '".$xml->body->row->nu_conta_corrente."',
		    						seq_conta_corrente	= '".$xml->body->row->seq_conta."'
		    					WHERE proseqconta = '".$sequencial."'
							    					and proid = $codigo";
					
						}elseif( $this->sistema == 'PAC' ){
							$sql = "UPDATE par.processoobra SET
			    					nu_conta_corrente	= '".$xml->body->row->nu_conta_corrente."',
			    					seq_conta_corrente	= '".$xml->body->row->seq_conta."'
			    				WHERE proseqconta = '".$sequencial."'
								    				and proid = $codigo";
						}
						$db->executar($sql);
						$db->commit();
					}
					return true;
				} else {
					return false;
				}
			} else {
				return true;
			}
	
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
	
			//echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";	
		}
	}

	private function consultarEmpenhoSIGEF($dados) {
		global $db;
		
		try {
			$data_created 	= date("c");
			$usuario 		= $dados['wsusuario'];
			$senha   		= $dados['wssenha'];
			$nu_seq_ne 		= $dados['empprotocolo'];
			$codigo 		= $dados['codigo'];
			$empstatus 		= $dados['empstatus'];
	
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
        	<nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;

			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
	
			$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
			->execute();
	
			$xmlRetorno = $xml;
	
			$xml 		= simplexml_load_string( stripslashes($xml));
			
			$result 	= (integer) $xml->status->result;	  
			$status 	= utf8_decode((string)$xml->body->row->status);
			$co_status	= substr( $status, 0, 1 );
			if( $result ){
				if( (int)$co_status == (int)1 ){
		  			
		  			if( $empstatus == 'I' ){
						$db->executar("update par.empenho set empstatus = 'A' where empid = {$dados['empid']}");
						$db->commit();
					}
					
					if( trim($dados['empsituacao']) !=  trim(utf8_decode($xml->body->row->situacao_documento)) ) {
						
						$set = array();
						if( $xml->body->row->nu_cnpj ) 						$set[] = "empcnpj = '".$xml->body->row->nu_cnpj."'";
						if( $xml->body->row->processo ) 					$set[] = "empnumeroprocesso = '".$xml->body->row->processo."'";
						if( $xml->body->row->nu_seq_ne ) 					$set[] = "empprotocolo = '".$xml->body->row->nu_seq_ne."'";
						if( $xml->body->row->co_especie_empenho ) 			$set[] = "empcodigoespecie = '".$xml->body->row->co_especie_empenho."'";
						if( $xml->body->row->situacao_documento ) 			$set[] = "empsituacao = '".trim(utf8_decode($xml->body->row->situacao_documento))."'";
							
						$numero_documento = trim((string)$xml->body->row->numero_documento);
							
						if( !empty($numero_documento) ){
							$empnumerooriginal 	= substr($numero_documento, 6);
							$empanooriginal 	= substr($numero_documento, 0, 4);
	
							$set[] = "empnumero = '".$numero_documento."'";
							$set[] = "empnumerooriginal = '".$empnumerooriginal."'";
							$set[] = "empanooriginal = '".$empanooriginal."'";
	
						}
						if( $xml->body->row->valor_ne ) 					$set[] = "empvalorempenho = '".trim($xml->body->row->valor_ne)."'";
						if( $xml->body->row->ds_problema ) 					$set[] = "ds_problema = '".trim($xml->body->row->ds_problema)."'";
						if( $xml->body->row->valor_total_empenhado )		$set[] = "valor_total_empenhado = '".trim($xml->body->row->valor_total_empenhado)."'";
						if( $xml->body->row->valor_saldo_pagamento )		$set[] = "valor_saldo_pagamento = '".trim($xml->body->row->valor_saldo_pagamento)."'";
						if( $xml->body->row->data_documento )				$set[] = "empdata = '".trim($xml->body->row->data_documento)."'";
						if( $xml->body->row->tp_especializacao )			$set[] = "tp_especializacao = '".trim($xml->body->row->tp_especializacao)."'";
						if( $xml->body->row->co_diretoria )					$set[] = "co_diretoria = '".trim($xml->body->row->co_diretoria)."'";
	
						if($set) {
							$sql = "UPDATE par.empenho SET ".implode(",",$set)."
									WHERE empid='".$dados['empid']."'";
							
							$db->executar($sql);
	
							$sql = "INSERT INTO par.historicoempenho(usucpf, empid, hepdata, empsituacao, co_especie_empenho, ds_problema, valor_total_empenhado, valor_saldo_pagamento)
					    			VALUES ('".$_SESSION['usucpf']."',
					    					'".$dados['empid']."',
					    					NOW(),
					    					'".trim(utf8_decode($xml->body->row->situacao_documento))."',
					    					'".$xml->body->row->co_especie_empenho."',
					    					'".trim($xml->body->row->ds_problema)."',
					    					".((strlen($xml->body->row->valor_total_empenhado))?"'".$xml->body->row->valor_total_empenhado."'":"NULL").",
					    					".((strlen($xml->body->row->valor_saldo_pagamento))?"'".$xml->body->row->valor_saldo_pagamento."'":"NULL").");";
							
							$db->executar($sql);
							$db->commit();
						}
						$sql = "INSERT INTO ".$this->tabelahistorico."(
							    	".($this->sistema == 'PAR' ? 'prpid' : 'proid').",
							    	hwpwebservice,
							    	hwpxmlenvio,
							    	hwpxmlretorno,
							    	hwpdataenvio,
							        usucpf)
							    VALUES ('".$codigo."',
							    		'consultarEmpenhoRotina - Sucesso',
							    		'".addslashes($arqXml)."',
							    		'".addslashes($xmlRetorno)."',
							    		NOW(),
							            '".$_SESSION['usucpf']."');";
						
						$db->executar($sql);
						$db->commit();
					}
				} else {
					$db->executar("update par.empenho set empstatus = 'I' where empid = {$dados['empid']}");
					$db->commit();
				}
			}
			$sql = "UPDATE par.empenho SET empdataatualizacao = now() WHERE empid='".$dados['empid']."'";				
			$db->executar($sql);
			$db->commit();
			return true;
	
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Erro-Serviço Consulta empenho encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
				
			//echo "Erro-WS Consultar empenho no SIGEF: $erroMSG";
			return false;
		}
	}

	private function consultarPagamentoSIGEF($dados) {
		global $db;
		
		try {
			$data_created 	= date("c");
			$usuario 		= $dados['wsusuario'];
			$senha   		= $dados['wssenha'];
			$nu_seq_ob 		= $dados['parnumseqob'];
			$codigo 		= $dados['codigo'];
			$pagstatus 		= $dados['pagstatus'];
	
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
        <nu_seq_ob>$nu_seq_ob</nu_seq_ob>
		</params>
	</body>
</request>
XML;

			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
	
			$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
			->execute();
	
			$xmlRetorno = $xml;
	
			$xml 	= simplexml_load_string( stripslashes($xml));
			
			$result = (integer) $xml->status->result;
			
			if($result) {
				//$db->executar("UPDATE par.pagamento SET pagstatus = 'A' WHERE pagid = {$dados['pagid']}");
				
				if( $pagstatus == 'I' ){
					$db->executar("UPDATE par.pagamento SET pagstatus = 'A' WHERE pagid = {$dados['pagid']}");
					$db->commit();
				}
	
				$data = (string)$xml->body->row->data_documento ? "'".formata_data_sql(iconv("UTF-8", "ISO-8859-1", $xml->body->row->data_documento))."'" : 'NULL';
					
				$situacao_documento = trim(utf8_decode($xml->body->row->situacao_documento));
					
				if( trim($dados['pagsituacaopagamento']) !=  $situacao_documento) {
											
					$pagnumeroob = ((strlen($xml->body->row->numero_documento))?$xml->body->row->numero_documento:"-");
						
					$sql = "UPDATE par.pagamento SET
								pagsituacaopagamento = '".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."',
								pagdatapagamentosiafi = ".$data.",
								pagnumeroob = '$pagnumeroob'
							WHERE
								pagid = '".$dados['pagid']."'";
					
					$db->executar($sql);
							
					$sql = "INSERT INTO par.historicopagamento(pagid, hpgdata, usucpf, hpgparcela, hpgvalorparcela, hpgsituacaopagamento)
			   				VALUES ('".$dados['pagid']."', NOW(), '".$_SESSION['usucpf']."',
			   					'".$dados['pagparcela']."', '".$xml->body->row->valor_ob."', '".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."');";
					
					$db->executar($sql);
					$db->commit();
	
					$sql = "INSERT INTO ".$this->tabelahistorico."(
						    	".($this->sistema == 'PAR' ? 'prpid' : 'proid').",
						    	hwpwebservice,
						    	hwpxmlenvio,
						    	hwpxmlretorno,
						    	hwpdataenvio,
						        usucpf)
						    VALUES ('".$codigo."',
						    		'consultarPagamento - Sucesso - Rotina Automática',
						    		'".addslashes($arqXml)."',
						    		'".addslashes($xmlRetorno)."',
						    		NOW(),
						            '".$_SESSION['usucpf']."');";
					
					$db->executar($sql);
					$db->commit();
				}	
			}			
			$sql = "UPDATE par.pagamento SET pagdataatualizacao = now() WHERE pagid='".$dados['pagid']."'";
			$db->executar($sql);
			$db->commit();
			return true;
	
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Erro-Serviço Consulta pagamento encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
	
			//echo "Erro-WS Consultar Pagamento no SIGEF: $erroMSG";	
		}
	}
}