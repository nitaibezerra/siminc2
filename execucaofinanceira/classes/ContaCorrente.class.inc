<?php
class ContaCorrente extends Processo {
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "execucaofinanceira.contacorrente";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ccoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ccoid' 							=> null, 
									  	'ccobanco' 							=> null, 
									  	'ccoagencia' 						=> null,
    									'ccoconta' 							=> null,
    									'ccosequencialcontasigef' 			=> null,
    									'ccostatus' 						=> null,
    									'ccodata' 							=> null,
    									'ccoseqsoliccrsigef' 				=> null,
    									'ccoseqcontasigef' 					=> null,
    									'ccodatamovimentosigef' 			=> null,
    									'ccofasesolicitacaosigef' 			=> null,
    									'ccocodigosituacaocontasigef' 		=> null,
    									'ccodescricaosituacaocontasigef'	=> null,
    									'proid' 							=> null
									  );	

	function __construct() {
		//parent::__construct();
	}
	
	public function atualizaDadosContaCorrente( $dados, Processo $obProcesso ){
		
    	$an_processo 			= date("Y");
    	$nu_processo 			= $obProcesso->pronumeroprocesso;
		$nu_cnpj_favorecido 	= $obProcesso->procnpj;
	
	    $data_created 			= date("c");
		$usuario 				= $dados['wsusuario'];
		$senha   				= $dados['wssenha'];
		$somente_conta_ativa	= 'S';
		$numero_de_linhas		= '200';
	
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
			<nu_identificador>$nu_cnpj_favorecido</nu_identificador>
			<nu_processo>$nu_processo</nu_processo>
			<somente_conta_ativa>$somente_conta_ativa</somente_conta_ativa>
			<numero_de_linhas>$numero_de_linhas</numero_de_linhas>
		</params>
	</body>
</request>
XML;

 
		if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
		}
		
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'consultarAndamentoCC') )
				->execute();

		$xmlRetorno = $xml;

		$xml = simplexml_load_string( stripslashes($xml));
		$result = (integer) $xml->status->result;
		
		if(!$result) {
			$erros = $xml->status->error->message;
			
			if(count($erros)>0) {	
				foreach($erros as $err) {	
			 		$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}							
			$arrHistorico = array('tipo' => 'atualizaDadosContaCorrente - Erro',
								  'proid' => $obProcesso->proid,
								  'envio' => $arqXml,
								  'retorno' => $xmlRetorno
								);
			$obProcesso->insereHistoricoWS($arrHistorico);
				
			if($err->text == '0 - Movimento de conta Corrente não existe.'){
				echo $obProcesso->mensagem( 'ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF',  $err->text);
			} else {
				echo $obProcesso->mensagem( 'ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF',  $mensagem);
			}

		    return false;
		} else {
			$obContaCorrenteWS = $xml->body->row->children();
			
			$seq_solic_cr 		= !empty($obContaCorrenteWS->seq_solic_cr) ? "'".(int)$obContaCorrenteWS->seq_solic_cr."'" : 'null';
			$seq_conta 			= !empty($obContaCorrenteWS->seq_conta) ? "'".(int)$obContaCorrenteWS->seq_conta."'" : 'null';
			$dt_movimento 		= !empty($obContaCorrenteWS->dt_movimento) ? "'".(string)$obContaCorrenteWS->dt_movimento."'" : 'null';
			$nu_banco 			= !empty($obContaCorrenteWS->nu_banco) ? "'".(string)$obContaCorrenteWS->nu_banco."'" : 'null';
			$nu_agencia 		= !empty($obContaCorrenteWS->nu_agencia) ? "'".(string)$obContaCorrenteWS->nu_agencia."'" : 'null';
			$nu_conta_corrente	= !empty($obContaCorrenteWS->nu_conta_corrente) ? "'".(string)$obContaCorrenteWS->nu_conta_corrente."'" : 'null';
			$fase_solicitacao	= !empty($obContaCorrenteWS->fase_solicitacao) ? "'".(string)$obContaCorrenteWS->fase_solicitacao."'" : 'null';
			$co_situacao_conta	= !empty($obContaCorrenteWS->co_situacao_conta) ? "'".(string)$obContaCorrenteWS->co_situacao_conta."'" : 'null';
			$situacao_conta 	= !empty($obContaCorrenteWS->situacao_conta) ? "'".(string)$obContaCorrenteWS->situacao_conta."'" : 'null';
			$nu_processo 		= !empty($obContaCorrenteWS->nu_processo) ? "'".(string)$obContaCorrenteWS->nu_processo."'" : 'null';
			$nu_identificador 	= !empty($obContaCorrenteWS->nu_identificador) ? "'".(string)$obContaCorrenteWS->nu_identificador."'" : 'null';
			$ds_razao_social 	= !empty($obContaCorrenteWS->ds_razao_social) ? "'".(string)$obContaCorrenteWS->ds_razao_social."'" : 'null';
			$ds_problema		= "'-'";
			$rnum 				= (int)		$obContaCorrenteWS->rnum;
			$status 			= (string)	$obContaCorrenteWS->status;
			$co_status			= substr( $status, 0, 1 );
			
			if( trim($co_status) != 0 ){
				$sql = "UPDATE execucaofinanceira.contacorrente SET 
							ccobanco = $nu_banco, 
							ccoagencia = $nu_agencia,
							ccoconta = $nu_conta_corrente,
  							ccoseqsoliccrsigef = $seq_solic_cr,
  							ccoseqcontasigef = $seq_conta,
  							ccodatamovimentosigef = $dt_movimento,
  							ccofasesolicitacaosigef = $fase_solicitacao,
  							ccocodigosituacaocontasigef = $co_situacao_conta,
  							ccodescricaosituacaocontasigef = $situacao_conta
  						WHERE proid = {$obProcesso->proid}";
						  	
				$obProcesso->executar($sql);
				$obProcesso->commit();
				
				$arrHistorico = array('tipo' => 'atualizaDadosContaCorrente - Sucesso',
									  'proid' => $obProcesso->proid,
									  'envio' => $arqXml,
									  'retorno' => $xmlRetorno
									);
				$obProcesso->insereHistoricoWS($arrHistorico);
			} else {
				if( (int)trim($co_status) == 0 ){
					$mensagem = ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $status);
					echo $obProcesso->mensagem( 'ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF',  $mensagem);
				} else {
					$mensagem = ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $status);
					echo $obProcesso->mensagem( 'ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF',  $mensagem);
				}
				
				$arrHistorico = array('tipo' => 'atualizaDadosContaCorrente - Erro',
									  'proid' => $obProcesso->proid,
									  'envio' => $arqXml,
									  'retorno' => $xmlRetorno
									);
				$obProcesso->insereHistoricoWS($arrHistorico);	
			    return false;
			}
			return true;
		}
	}

	public function solicitarContaCorrente($dados, Processo $obProcesso) {
			
		try {			
			$data_created 				= date("c");
			$usuario 					= $dados['wsusuario'];
			$senha   					= $dados['wssenha'];			
			$arrConta 					= $this->carregarContaPorProcesso( $obProcesso );			
	       	$nu_processo 				= $obProcesso->pronumeroprocesso;
	        $nu_banco					= $arrConta['ccobanco'];
	        $nu_agencia					= $arrConta['ccoagencia'];
			$nu_identificador 			= $obProcesso->procnpj;	
	        $tp_identificador			= "1";
	        $nu_conta_corrente			= null;
	        $tp_solicitacao				= "01";
	        $motivo_solicitacao			= "0032";
	        $convenio_bb				= null;
	        $tp_conta					= "N";
	        $co_programa_fnde			= $obProcesso->tipprogramafnde;
		    $nu_sistema					= $obProcesso->tipnumerosistemasigef;
	       	
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

			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
			}
	
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
						$mensagem .= ' '.iconv("UTF-8", "ISO-8859-1", $err->text).' ';
					}
				}
				
				echo $obProcesso->mensagem( 'ERRO AO SOLICITAR A CONTA CORRENTE',  $mensagem);
				$arrHistorico = array('tipo' => 'solicitarContaCorrente - Erro',
									  'proid' => $obProcesso->proid,
									  'envio' => $arqXml,
									  'retorno' => $xmlRetorno
									);
				$obProcesso->insereHistoricoWS($arrHistorico);	
			    return false;
			} else {
				
				$sql = "UPDATE execucaofinanceira.contacorrente SET
  							ccoseqsoliccrsigef = '{$xml->body->seq_solic_cr}',
  							ccoseqcontasigef = '{$xml->body->nu_seq_conta}' 
						WHERE proid = {$obProcesso->proid}";
				
			   $arrHistorico = array('tipo' => 'solicitarContaCorrente - Sucesso',
									  'proid' => $obProcesso->proid,
									  'envio' => $arqXml,
									  'retorno' => $xmlRetorno
									);
				$obProcesso->insereHistoricoWS($arrHistorico);
	
				return true;
			}
	
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				$mensagem = "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.";
				echo $obProcesso->mensagem( 'ERRO AO SOLICITAR A CONTA CORRENTE',  $mensagem);
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			
			$arrHistorico = array('tipo' => 'solicitarContaCorrente - Erro',
								  'proid' => $obProcesso->proid,
								  'envio' => $arqXml,
								  'retorno' => $xmlRetorno
								);
			$obProcesso->insereHistoricoWS($arrHistorico);
	
			echo $obProcesso->mensagem( 'ERRO AO SOLICITAR A CONTA CORRENTE',  $erroMSG);	
		}
	}

	public function carregarContaPorProcesso(Processo $obProcesso){
		$sql = "SELECT
					ccoid,
				  	ccobanco,
				  	ccoagencia,
				  	ccoconta,
				  	ccosequencialcontasigef,
				  	ccostatus,
				  	ccodata,
				  	ccoseqsoliccrsigef,
				  	ccoseqcontasigef,
				  	ccodatamovimentosigef,
				  	ccofasesolicitacaosigef,
				  	ccocodigosituacaocontasigef,
				  	ccodescricaosituacaocontasigef
				FROM 
				  execucaofinanceira.contacorrente 
				WHERE proid = {$obProcesso->proid}";
				
		return $obProcesso->pegaLinha( $sql );
	}

    public function consultarSituacaoContaCorrentePorProcesso($usuario, $senha, $pronumeroprocesso) {

        global $db;

        try {

            $data_created = date("c");

            $proseq = $db->pegalinha("
                                        SELECT proseqconta, proid, tipo FROM (

                                            SELECT proid, proseqconta, pronumeroprocesso, 'par' as tipo FROM par.processoobraspar
                                            UNION
                                            SELECT proid, proseqconta, pronumeroprocesso, 'pac' as tipo FROM par.processoobra

                                        ) as p
                                        WHERE pronumeroprocesso = '".$pronumeroprocesso."' ");

            $proseqconta = $proseq['proseqconta'];
            $proid = $proseq['proid'];

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
        <seq_solic_cr>$proseqconta</seq_solic_cr>
		</params>
	</body>
</request>
XML;


//        if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
//            $urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
//        } else {
            $urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
//        }

            if($proseqconta) {

                $xml = Fnde_Webservice_Client::CreateRequest()
                    ->setURL($urlWS)
                    ->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
                    ->execute();

                $xmlRetorno = $xml;
                $xml = simplexml_load_string( stripslashes($xml));

                if($xml->body->row->seq_conta) {
                    $db->executar("UPDATE par.processoobraspar SET nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', seq_conta_corrente='".$xml->body->row->seq_conta."' WHERE proseqconta='".$proseqconta."'");
                    $db->commit();
                }

                $tabela = ($proseq['tipo'] == 'par') ? 'historicowsprocessoobrapar' : 'historicowsprocessoobra';

                $sql = "INSERT INTO par.$tabela(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$proid."',
				    		'consultarContaCorrente',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

                $db->executar($sql);
                $db->commit();

                $result = (integer) $xml->status->result;
                if($result)
                    return (integer) $xml->body->row->co_situacao_conta;
            }
            return false;

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
}
?>