<?php
class WSContaCorrente {
	
	public $db;
	
	public function __construct(&$db = null){
		if($db){
			$this->db = $db;			
		} else {
			$this->db = new cls_banco(); 
		}
	}
	
	private function getDataAtual(){
		return date("c");
	}
	
	public function consultarContaCorrente(&$post, $boConsultaPeloMenu = false, $boAuto = false){
		$usuario = $post['usuario'];
		$senha 	 = $post['senha'];
		$ptrid 	 = $post['ptrid'];
		$exfid 	 = $post['exfid'];
		$xmlRetorno = "";
		
		if( !$boConsultaPeloMenu ){
			$filtro = " where exf.exfid = {$post['exfid']} and cc.ptrid = {$post['ptrid']} ";
		}
		
		$sql = "select 
					cc.cocid, 
					cc.cocnu_seq_mov_conta, 
					cc.cocsituacao_conta, 
					cc.cocnu_agencia, 
					cc.cocandamento,
					cc.cocseq_conta 
				from emenda.contacorrente cc
				inner join emenda.execucaofinanceira exf on cc.ptrid = exf.ptrid
				left join emenda.ordembancaria orb on exf.exfid = orb.exfid
					and orb.orbnumordembancaria is null 
				$filtro";
				
		$arDados = $this->db->carregar($sql);

		$arDados = ($arDados) ? $arDados : array();
		$arContaCorrenteAlterados = array();
		$count = 0;
		foreach($arDados as $dados){
			$cocnu_seq_mov_conta 	= $dados['cocnu_seq_mov_conta'];
			$seq_solic_cr 			= $dados['cocseq_conta'];
			$cocid 				 	= $dados['cocid'];
			$ptragenciabancaria  	= $dados['cocnu_agencia'];
			$cocandamento  			= $dados['cocandamento'];
			
			try {
		    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$this->getDataAtual()}</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <seq_solic_cr>$cocnu_seq_mov_conta</seq_solic_cr>
		</params>
	</body>
</request>
XML;

				/*if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
					$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';						
				} else {*/
					$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
				//}
				
				/*$xsd = APPRAIZ.'emenda/modulos/sistema/ws-sigef/xsd/xml.fnde.ws-sigef.conta_corrente.consultar.request.xsd';
				$arMessage = validaXML( $arqXml, $xsd );*/
				    
				$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
					->execute();

    			/*$xml = Fnde_Webservice_Client::CreateRequest()
						//->setURL('http://172.20.200.116/webservices/sigef/integracao/web/dev.php/contacorrente/consultar')
						->setURL($urlWS)
						->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
						->execute() ;*/

				$xmlRetorno = $xml;
				
				$arrParam = array(
								'ptrid' => $ptrid,
								'exfid' => $exfid,
								'cocid' => $cocid,
								'logTipo' => 'CC',
								'xmlEnvio' => $arqXml,
								'xmlRetorno' => $xmlRetorno,
								);
				if( !$boConsultaPeloMenu ) logWsSIGEF($arrParam);
				
				$xml = simplexml_load_string( stripslashes($xml));

				if ( (int) $xml->status->result ){
					$obContaCorrenteWS = '';
					$obContaCorrenteWS = $xml->body->row->children();
					$nu_seq_mov_conta 			   = (int)    $obContaCorrenteWS->seq_solic_cr;
					$nu_seq_conta_corrente_favorec = (int) 	  $obContaCorrenteWS->seq_conta;
					$ds_razao_social 			   = (string) $obContaCorrenteWS->ds_razao_social;
					$andamento 					   = (string) $obContaCorrenteWS->andamento;
					$nu_banco 					   = (int) 	  $obContaCorrenteWS->nu_banco;
					$nu_agencia 			   	   = (string) $obContaCorrenteWS->nu_agencia;
					$nu_conta_corrente 			   = (string) $obContaCorrenteWS->nu_conta_corrente;
					$cnpj 						   = (int)    $obContaCorrenteWS->nu_identificador;
					$co_situacao_conta 			   = (string) $obContaCorrenteWS->co_situacao_conta;
					$ds_problema     			   = (string) $obContaCorrenteWS->ds_problema;
					
					if($andamento != $cocandamento) {
						if($cocid) {
							$obContaCorrente = new ContaCorrente($cocid);
							$obContaCorrente->cocseq_conta	 				= ($nu_seq_conta_corrente_favorec) ? $nu_seq_conta_corrente_favorec : null;
							$obContaCorrente->cocds_razao_social 			= $ds_razao_social;
							$obContaCorrente->cocandamento 					= utf8_decode($andamento);
							$obContaCorrente->cocnu_banco 					= $nu_banco;
							$obContaCorrente->cocnu_agencia 				= $nu_agencia;
							$obContaCorrente->cocnu_conta_corrente 			= ($nu_conta_corrente) ? $nu_conta_corrente : null;
							$obContaCorrente->coccnpj 						= $cnpj;
							$obContaCorrente->cocsituacao_conta 			= $co_situacao_conta;
							$obContaCorrente->cocdsproblema			        = $ds_problema;
							
							$arCamposNulo = array('cocseq_conta','cocnu_conta_corrente');						
							$obContaCorrente->salvar(true, true, $arCamposNulo);
							
							$obContaCorrenteHistorico = new ContaCorrenteHistorico();
							$obContaCorrenteHistorico->cocid 			 = $cocid;
							$obContaCorrenteHistorico->usucpf 			 = $_SESSION['usucpf'];
							$obContaCorrenteHistorico->ccdatainclusao 	 = date('Y-m-d H:i:s');
							$obContaCorrenteHistorico->cocandamento 	 = $obContaCorrente->cocandamento;
							$obContaCorrenteHistorico->cocsituacao_conta = $obContaCorrente->cocsituacao_conta;
							$obContaCorrenteHistorico->cocdsproblema 	 = $obContaCorrente->cocdsproblema;
							$obContaCorrenteHistorico->salvar();
							
							$boSucesso = $obContaCorrente->commit();
							unset($obContaCorrente);
							unset($obContaCorrenteHistorico);
							if($boSucesso){
								$arContaCorrenteAlterados[] = $cocid;						
							}
							
						} else {
							throw new Exception("ID da conta corrente não existe");
						}
					}
				} else {
			    					
					//echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";
					throw new Exception(utf8_decode($xml->status->error->message->text));
				}
			} catch (Exception $e){
				/**
				 * @var LogErroWS
				 * Bloco que grava o erro em nossa base
				 */
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
				
				$arrParam = array(
								'ptrid' => $ptrid,
								'exfid' => $exfid,
								'cocid' => $cocid,
								'logTipo' => 'CC',
								'xmlEnvio' => $arqXml,
								'xmlRetorno' => $xmlRetorno.' - '.$erroMSG,
								);
				
				if( !$boConsultaPeloMenu ) logWsSIGEF($arrParam);
				
				# Erro 404 página not found
				if($e->getCode() == 404){
					echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
					die;
				}
						
				echo "<span style='color: red;'><b>Detalhes do erro - Consultar Conta Corrente no SIGEF:</b><br/><br/>".$erroMSG."</span><br><br>";
				//die;
			}
		$count++;
		
		} // fim foreach
		
//$boConsultaPeloMenu = 1;
//$arContaCorrenteAlterados = array(1);
		if($boConsultaPeloMenu){
			$sql = "";
			$arDados = "";
			if(count($arContaCorrenteAlterados)){
				$cocid = implode(',',$arContaCorrenteAlterados);
				$sql = "select
							pt.ptrid || '/' || pt.ptrexercicio as planotrabalho,
							cc.cocnu_seq_mov_conta,
							cc.cocnu_agencia,
							cc.cocnu_conta_corrente,
							cc.cocds_razao_social, 
							cc.cocandamento, 
							cc.cocsituacao_conta 
						from emenda.contacorrente cc
							inner join emenda.planotrabalho pt on cc.ptrid = pt.ptrid
						where cocid in ($cocid) ";
				$arDados = $this->db->carregar($sql);
			}
			$arDados = ($arDados) ? $arDados : array();
			$this->db->cria_aba( $abacod_tela, $url, '' );

			if($boAuto){
				ob_start();
			}
			
			monta_titulo( $titulo_modulo, 'Conta Corrente Atualizadas' );
			
			$cabecalho = array("Plano de Trabalho", "Número sequencial", "Agência", "Conta Corrente", "Razão Social", "Andamento","Situação");
			$this->db->monta_lista_array($arDados, $cabecalho, 50, 20, '', 'center','');
			
			if($boAuto){
				$conteudo = ob_get_contents();
				ob_end_clean();
				
				/*
				 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
				 */
				require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
				require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
				$mensagem = new PHPMailer();
				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
				$mensagem->FromName		= "Conta Corrente Atualizadas";
				$mensagem->From 		= "simec@mec.gov.br";
				$mensagem->AddAddress( "thiago.barbosa@mec.gov.br", "Wesley Silva" );
				$mensagem->Subject = "Conta Corrente Atualizadas";
				
				$mensagem->Body = $conteudo;
				$mensagem->IsHTML( true );
				$mensagem->Send();
			}
			
			# Feito isso Para não parar com a rotina automatica
			if(!$boAuto){
				die;
			}
		}
	}
	public function consultarContaCorrenteGeral(&$post, $cocid){
		$usuario = $post['usuario'];
		$senha 	 = $post['senha'];
		$ptrid 	 = $post['ptrid'];
		$exfid 	 = $post['exfid'];
		$xmlRetorno = "";
		
		$sql = "select 
					cc.cocid, 
					cc.cocnu_seq_mov_conta, 
					cc.cocsituacao_conta, 
					cc.cocnu_agencia, 
					cc.cocandamento,
					cc.cocseq_conta  
				from emenda.contacorrente cc
				inner join emenda.execucaofinanceira exf on cc.ptrid = exf.ptrid
				left join emenda.ordembancaria orb on exf.exfid = orb.exfid
					and orb.orbnumordembancaria is null 
				where 
					cocid = $cocid";
				
		$arDados = $this->db->carregar($sql);

		$arDados = ($arDados) ? $arDados : array();
		$arContaCorrenteAlterados = array();
		$count = 0;
		foreach($arDados as $dados){
			$cocnu_seq_mov_conta = $dados['cocnu_seq_mov_conta'];
			$seq_solic_cr 		 = $dados['cocseq_conta'];
			$cocid 				 = $dados['cocid'];
			$ptragenciabancaria  = $dados['cocnu_agencia'];
			$cocandamento  = $dados['cocandamento'];
			
			try {
$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$this->getDataAtual()}</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <seq_solic_cr>$seq_solic_cr</seq_solic_cr>
		</params>
	</body>
</request>
XML;

				if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
					$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';						
				} else {
					$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
				}
				
				/*$xsd = APPRAIZ.'emenda/modulos/sistema/ws-sigef/xsd/xml.fnde.ws-sigef.conta_corrente.consultar.request.xsd';
				$arMessage = validaXML( $arqXml, $xsd );*/
				    
				$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
					->execute();

				$xmlRetorno = $xml;
				
				$arrParam = array(
								'ptrid' => $ptrid,
								'exfid' => $exfid,
								'cocid' => $cocid,
								'logTipo' => 'CC',
								'xmlEnvio' => $arqXml,
								'xmlRetorno' => $xmlRetorno,
								);
				
				logWsSIGEF($arrParam);

				$xml = simplexml_load_string( stripslashes($xml));
				if ( (int) $xml->status->result ){
					$obContaCorrenteWS = '';
					$obContaCorrenteWS = $xml->body->row->children();
					
					$nu_seq_mov_conta 			   = (int)    $obContaCorrenteWS->seq_solic_cr;
					$nu_seq_conta_corrente_favorec = (int) 	  $obContaCorrenteWS->seq_conta;
					$ds_razao_social 			   = (string) $obContaCorrenteWS->ds_razao_social;
					$andamento 					   = (string) $obContaCorrenteWS->andamento;
					$nu_banco 					   = (int) 	  $obContaCorrenteWS->nu_banco;
					$nu_agencia 			   	   = (string) $obContaCorrenteWS->nu_agencia;
					$nu_conta_corrente 			   = (string) $obContaCorrenteWS->nu_conta_corrente;
					$cnpj 						   = (int)    $obContaCorrenteWS->nu_identificador;
					$co_situacao_conta 			   = (string) $obContaCorrenteWS->co_situacao_conta;
					$ds_problema     			   = (string) $obContaCorrenteWS->ds_problema;
					
					if($andamento != $cocandamento) {
						if($cocid) {
							$obContaCorrente = new ContaCorrente($cocid);
							$obContaCorrente->cocseq_conta	 				= ($nu_seq_conta_corrente_favorec) ? $nu_seq_conta_corrente_favorec : null;
							$obContaCorrente->cocds_razao_social 			= $ds_razao_social;
							$obContaCorrente->cocandamento 					= utf8_decode($andamento);
							$obContaCorrente->cocnu_banco 					= $nu_banco;
							$obContaCorrente->cocnu_agencia 				= $nu_agencia;
							$obContaCorrente->cocnu_conta_corrente 			= ($nu_conta_corrente) ? $nu_conta_corrente : null;
							$obContaCorrente->coccnpj 						= $cnpj;
							$obContaCorrente->cocsituacao_conta 			= $co_situacao_conta;
							$obContaCorrente->cocdsproblema			        = $ds_problema;
							
							$arCamposNulo = array('cocseq_conta','cocnu_conta_corrente');						
							$obContaCorrente->salvar(true, true, $arCamposNulo);
							
							$obContaCorrenteHistorico = new ContaCorrenteHistorico();
							$obContaCorrenteHistorico->cocid 			 = $cocid;
							$obContaCorrenteHistorico->usucpf 			 = $_SESSION['usucpf'];
							$obContaCorrenteHistorico->ccdatainclusao 	 = date('Y-m-d H:i:s');
							$obContaCorrenteHistorico->cocandamento 	 = $obContaCorrente->cocandamento;
							$obContaCorrenteHistorico->cocsituacao_conta = $obContaCorrente->cocsituacao_conta;
							$obContaCorrenteHistorico->cocdsproblema 	 = $obContaCorrente->cocdsproblema;
							$obContaCorrenteHistorico->salvar();
							
							$boSucesso = $obContaCorrente->commit();
							unset($obContaCorrente);
							unset($obContaCorrenteHistorico);
							if($boSucesso){
								$arContaCorrenteAlterados[] = $cocid;						
							}
							
						} else {
							$htmllog = "ID da conta corrente não existe";
						}
					}
				} else {
					$htmllog = utf8_decode($xml->status->error->message->text);
				}
			} catch (Exception $e){
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
				
				$arrParam = array(
								'ptrid' => $ptrid,
								'exfid' => $exfid,
								'cocid' => $cocid,
								'logTipo' => 'CC',
								'xmlEnvio' => $arqXml,
								'xmlRetorno' => $xmlRetorno.' - '.$erroMSG,
								);
				
				logWsSIGEF($arrParam);
				
				$htmllog = $erroMSG;
			}
		$count++;
		
		} // fim foreach
		return $htmllog;
	}
	
	public function solicitarContaCorrente(&$post){
		$usuario = $post['usuario'];
		$senha   = $post['senha'];
		$ptrid 	 = $post['ptrid'];
		$exfid 	 = $post['exfid'];
		
		$xmlRetorno = "";
		
		$sql = "select cc.cocid, cc.cocnu_seq_mov_conta, pt.ptrnumprocessoempenho, b.bcocod, pt.ptragenciabancaria, e.enbcnpj
					from emenda.planotrabalho pt 
	  					inner join emenda.banco b on pt.bcoid = b.bcoid
	  					left join emenda.contacorrente cc on cc.ptrid = pt.ptrid 
	  					inner join emenda.entidadebeneficiada e on pt.enbid = e.enbid
					where pt.ptrid = $ptrid ";
		
		$arContaCorrente = $this->db->pegaLinha($sql);
		$arContaCorrente = ($arContaCorrente) ? $arContaCorrente : array();
		
		if(!$arContaCorrente['cocnu_seq_mov_conta']){
			$cocid = $arContaCorrente['cocid'];	
			// somente os 4 digitos da agencia bancaria
			$arContaCorrente['ptragenciabancaria'] = substr(trim($arContaCorrente['ptragenciabancaria']),0,4);
			
			$nu_identificador 	= $arContaCorrente['enbcnpj'];
			$tp_identificador 	= '1';
			$nu_processo 		= $arContaCorrente['ptrnumprocessoempenho'];
			$nu_banco 			= $arContaCorrente['bcocod'];
			$nu_agencia 		= $arContaCorrente['ptragenciabancaria'];
			$nu_conta_corrente 	= null;
			$tp_solicitacao 	= '01';
			$motivo_solicitacao	= '0032';
			$convenio_bb 		= null;
			$tp_conta 			= 'N';
			$nu_sistema 		= '2';
			$co_programa_fnde 	= '03';

$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$this->getDataAtual()}</created>
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
	
			try {
				if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
					$urlWS = 'http://dev.fnde.gov.br/webservices/sigef/integracao/public/index.php/financeiro/cr';						
				} else {
					$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
				}
				
				$xml = Fnde_Webservice_Client::CreateRequest()
						->setURL($urlWS)
						->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
						->execute();
					
				$xmlRetorno = $xml;
				
				$arrParam = array(
								'ptrid' => $ptrid,
								'exfid' => $exfid,
								'logTipo' => 'CC',
								'xmlEnvio' => $arqXml,
								'xmlRetorno' => $xmlRetorno,
								);
				
				logWsSIGEF($arrParam);
				
				$xml = simplexml_load_string( stripslashes($xml));
				if ( (int) $xml->status->result ){
										
					unset($obContaCorrenteWS);
			        $obContaCorrenteWS = $xml->body->children();
			        $nu_seq_mov_conta = (string) $obContaCorrenteWS->seq_solic_cr;
					if($nu_seq_mov_conta){
						# Atualiza a tabela de PlanoTrabalho.
						$obContaCorrente = new ContaCorrente();
						$obContaCorrente->ptrid 			  = $ptrid; 
						$obContaCorrente->cocnu_seq_mov_conta = $nu_seq_mov_conta;
						$obContaCorrente->cocdatainclusao 	  = date('Y-m-d H:i:s');
						$obContaCorrente->salvar();
						$obContaCorrente->commit();
						unset($obContaCorrente);
						return true;
					} else {
						$erro = 'Erro-Operação da Conta Corrente falhou.';
						echo "<span style='color: red;'><b>Detalhes do erro - solicitar Conta Corrente:</b><br/><br/>".$erro."</span><br><br>";
						return false;
					}
			    } else {
			    	$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
			    	$erroMSG = str_replace( "'", '"', $erroMSG );
			    							
					/*$erro = "Erro-WS Solicitar Conta Corrente no SIGEF: $erroMSG";
					
					echo "<span style='color: red;'><b>Detalhes do erro - solicitar Conta Corrente:</b><br/><br/>".$erro."</span><br><br>";*/
					throw new Exception(utf8_decode($xml->status->error->message->text));
			    }
			} catch (Exception $e){
				/**
				 * @var LogErroWS
				 * Bloco que grava o erro em nossa base
				 */
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
				
				$arrParam = array(
								'ptrid' => $ptrid,
								'exfid' => $exfid,
								'logTipo' => 'CC',
								'xmlEnvio' => $arqXml,
								'xmlRetorno' => $xmlRetorno.' - '.$erroMSG,
								);
				
				logWsSIGEF($arrParam);
				
				# Erro 404 página not found
				if($e->getCode() == 404){
					$erro = "Erro-Serviço Solicitar Conta Corrente encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
					echo "<span style='color: red;'><b>Detalhes do erro - solicitar Conta Corrente SIGEF:</b><br/><br/>".$erro."</span>";
					return false;
				}						
				
				echo "<span style='color: red;'><b>Detalhes do erro - solicitar Conta Corrente SIGEF:</b><br/><br/>".$erroMSG."</span><br><br>";
			}
		}
	}

	public function atualizaDadosContaCorrente($post){
		
		$usuario = $post['usuario'];
		$senha 	 = $post['senha'];
		$ptrid 	 = $post['ptrid'];
		$xmlRetorno = "";
		
		$sql = "select 
					cc.cocid, 
					cc.cocnu_seq_mov_conta, 
					cc.cocsituacao_conta, 
					cc.cocnu_agencia, 
					cc.cocseq_conta, 
					cc.cocandamento,
					ptr.ptrnumprocessoempenho,
					enb.enbcnpj
				from emenda.planotrabalho ptr
				inner join emenda.execucaofinanceira exf on exf.ptrid = ptr.ptrid and exf.exfstatus = 'A'
				inner join emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid 
				left join emenda.contacorrente cc on cc.ptrid = ptr.ptrid
				where ptr.ptrid = $ptrid";
				
		$arDados = $this->db->carregar($sql);
		//ver( $arDados,d );
		$arDados = ($arDados) ? $arDados : array();
		$arContaCorrenteAlterados = array();
		$count = 0;
		foreach($arDados as $dados){
			$cocnu_seq_mov_conta 	= $dados['cocnu_seq_mov_conta'];
			/*$nu_identificador	    = '00003239349';
			$ptrnumprocessoempenho  = '23034026872200529';*/
			$nu_identificador	    = $dados['enbcnpj'];
			$ptrnumprocessoempenho  = $dados['ptrnumprocessoempenho'];
			$cocandamento  			= $dados['cocandamento'];
			$cocid					= $dados['cocid'];
			$somente_conta_ativa	= 'N';
			$numero_de_linhas		= '200';
			
			try {
    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$this->getDataAtual()}</created>
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

    
				/* if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
					//$urlWS = 'http://dev.fnde.gov.br/webservices/sigef/integracao/public/index.php/financeiro/cr';
					$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
				} else { */
					$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
				//}
				
				$xsd = APPRAIZ.'emenda/modulos/sistema/ws-sigef/xsd/xml.fnde.ws-sigef_integracao.financeiro.cr.consultarandamentocc.request.xsd';
				//$arMessage = validaXML( $arqXml, $xsd );
				
				if( empty( $arMessage ) ){
					$xml = Fnde_Webservice_Client::CreateRequest()
							->setURL($urlWS)
							->setParams( array('xml' => $arqXml, 'method' => 'consultarAndamentoCC') )
							->execute();
			    } else {
			    	echo 'Erro-WS: validação xml - consultar conta corrente: <br>';
			    	foreach ($arMessage as $msg) {
			    		print_r( $msg['text'] );
			    	}
			    	die;
			    }

				$xmlRetorno = $xml;

				$xml = simplexml_load_string( stripslashes($xml));
				//ver( $xml,d );
				
				if ( (int) $xml->status->result ){
					$obContaCorrenteWS = '';
					$obContaCorrenteWS = $xml->body->row->children();
					
					$seq_solic_cr 		= !empty($obContaCorrenteWS->seq_solic_cr) ? (int)$obContaCorrenteWS->seq_solic_cr : 'null';
					$seq_conta 			= !empty($obContaCorrenteWS->seq_conta) ? (int)$obContaCorrenteWS->seq_conta : 'null';
					$dt_movimento 		= !empty($obContaCorrenteWS->dt_movimento) ? (int)$obContaCorrenteWS->dt_movimento : 'null';
					$nu_banco 			= !empty($obContaCorrenteWS->nu_banco) ? "'".$obContaCorrenteWS->nu_banco."'" : 'null';
					$nu_agencia 		= !empty($obContaCorrenteWS->nu_agencia) ? "'".$obContaCorrenteWS->nu_agencia."'" : 'null';
					$nu_conta_corrente	= !empty($obContaCorrenteWS->nu_conta_corrente) ? "'".$obContaCorrenteWS->nu_conta_corrente."'" : 'null';
					$fase_solicitacao	= !empty($obContaCorrenteWS->fase_solicitacao) ? "'".$obContaCorrenteWS->fase_solicitacao."'" : 'null';
					$co_situacao_conta	= !empty($obContaCorrenteWS->co_situacao_conta) ? (int)$obContaCorrenteWS->co_situacao_conta : 'null';
					$situacao_conta 	= !empty($obContaCorrenteWS->situacao_conta) ? "'".$obContaCorrenteWS->situacao_conta."'" : 'null';
					$nu_processo 		= !empty($obContaCorrenteWS->nu_processo) ? "'".$obContaCorrenteWS->nu_processo."'" : 'null';
					$nu_identificador 	= !empty($obContaCorrenteWS->nu_identificador) ? "'".$obContaCorrenteWS->nu_identificador."'" : 'null';
					$ds_razao_social 	= !empty($obContaCorrenteWS->ds_razao_social) ? "'".$obContaCorrenteWS->ds_razao_social."'" : 'null';
					$ds_problema		= "'-'";
					$rnum 				= (int)		$obContaCorrenteWS->rnum;
					$status 			= (string)	$obContaCorrenteWS->status;
					$co_status			= substr( $status, 0, 1 );
					
					if( trim($co_status) != 0 ){
						if( empty($cocid) ){					
							$sql = "INSERT INTO emenda.contacorrente(ptrid, cocnu_seq_mov_conta, cocseq_conta, cocds_razao_social, cocandamento,
		  								cocnu_banco, cocnu_agencia, coccnpj, cocsituacao_conta, cocdatainclusao, cocdsproblema, cocnu_conta_corrente) 
									VALUES ($ptrid, $seq_solic_cr, $seq_conta, $ds_razao_social, $fase_solicitacao,
		  								$nu_banco, $nu_agencia, $nu_identificador, $co_situacao_conta, now(), $ds_problema, $nu_conta_corrente) RETURNING cocid";
		  					$cocid = $this->db->pegaUm($sql);
						} else {
							$sql = "UPDATE emenda.contacorrente SET 
									  ptrid = $ptrid,
									  cocnu_seq_mov_conta = $seq_solic_cr,
									  cocseq_conta = $seq_conta,
									  cocds_razao_social = $ds_razao_social,
									  cocandamento = $fase_solicitacao,
									  cocnu_banco = $nu_banco,
									  cocnu_agencia = $nu_agencia,
									  coccnpj = $nu_identificador,
									  cocsituacao_conta = $co_situacao_conta,
									  cocnu_conta_corrente = $nu_conta_corrente								 
									WHERE 
									  cocid = $cocid";
							$this->db->executar($sql);
						}
						$arBody = (array)$xml->body->children();
						
						foreach ($arBody[row] as $key => $v) {
							$v = (array)$v;
							$cocandamento		= $v[fase_solicitacao];
							$cocsituacao_conta	= $v[co_situacao_conta];
							$usucpf				= $_SESSION['usucpf'];
							$ccdatainclusao		= 'now()';
							//$ccdatainclusao		= $v[dt_movimento];
							$cocdsproblema		= '-';
							
							$sql = "SELECT cchid FROM emenda.contacorrentehistorico WHERE cocid = $cocid and cocandamento = '$cocandamento'";
							$cchid = $this->db->pegaUm($sql);
							
							if( empty($cchid) ){
								$sql = "INSERT INTO emenda.contacorrentehistorico(cocid, cocandamento, cocsituacao_conta,
	  										usucpf, ccdatainclusao, cocdsproblema) 
										VALUES ($cocid, '$cocandamento', '$cocsituacao_conta',
	  										'$usucpf', '$ccdatainclusao', '$cocdsproblema')";
							} else {
								$sql = "UPDATE emenda.contacorrentehistorico SET
										  cocandamento = '$cocandamento',
										  cocsituacao_conta = '$cocsituacao_conta',
										  usucpf = '$usucpf',
										  ccdatainclusao = '$ccdatainclusao',
										  cocdsproblema = '$cocdsproblema'										 
										WHERE 
										  cchid = $cchid";
							}
							$this->db->executar($sql);
						}
						if($this->db->commit()){
							$erro = "Consultar Conta Corrente no SIGEF: ".$xml->status->message->text;
							echo "<span style='color: red;'><b>Detalhes do erro:</b><br/><br/>".$erro."</span>";
							die;
						}
					} else {
						$erro = "Erro-WS Consultar Conta Corrente no SIGEF: $status";
						echo "<span style='color: red;'><b>Detalhes do erro:</b><br/><br/>".$erro."</span>";
						die;
					}
				} else {
					$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
			    	$erroMSG = str_replace( "'", '"', $erroMSG );
			    							
					$erro = "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";
					echo "<span style='color: red;'><b>Detalhes do erro:</b><br/><br/>".$erro."</span>";
					die;
				}
			} catch (Exception $e){
				/**
				 * @var LogErroWS
				 * Bloco que grava o erro em nossa base
				 */
				$obLogErroWS = new LogErroWS();
				$erro = "XML de Envio: <br />".$arqXml;
				$obLogErroWS->montaTextoErro($erro);
				$erro = "XML de Retorno: <br />".$xmlRetorno;
				$obLogErroWS->montaTextoErro($erro);
				$erro = "Objeto Exception: <br />".$e;
				$obLogErroWS->montaTextoErro($erro);
				$obLogErroWS->ptrid 		  = $_SESSION['emenda']['ptridAnalise'];
				$obLogErroWS->cocid 		  = $cocid;
				$obLogErroWS->logtipo 		  = 'CC';
				$obLogErroWS->usucpf 		  = $_SESSION['usucpf'];
				$obLogErroWS->logdatainclusao = date('Y-m-d H:i:s');
				$obLogErroWS->logerro 		  = str_replace( "'", '"', $obLogErroWS->logerro );
				$obLogErroWS->salvar();
				$obLogErroWS->commit();
				unset($obLogErroWS);
				
				# Erro 404 página not found
				if($e->getCode() == 404){
					$erro = "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
					echo "<span style='color: red;'><b>Detalhes do erro:</b><br/><br/>".$erro."</span>";
					die;
				} else {
					$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
					$erroMSG = str_replace( "'", '"', $erroMSG );
						
					$erro = "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";
					echo "<span style='color: red;'><b>Detalhes do erro:</b><br/><br/>".$erro."</span>";
					die;
				}
			}
		$count++;
		
		} // fim foreach
	}
}
