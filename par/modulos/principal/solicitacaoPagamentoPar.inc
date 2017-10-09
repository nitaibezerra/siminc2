<?php
include APPRAIZ."par/classes/Habilita.class.inc";

if($_REQUEST['inuid']) {
	$_SESSION['par_var']['inuid'] = $_REQUEST['inuid'];
	$esdid = $db->pegaLinha("SELECT esdid, CASE WHEN iu.itrid = 1 THEN 'Estado' ELSE 'Município' END as label FROM par.instrumentounidade iu INNER JOIN workflow.documento d ON d.docid = iu.docid WHERE inuid = ".$_REQUEST['inuid']);
}

/* function listaHistoricoPagamento($dados) {
	global $db;
	
	$sql = "SELECT
				u.usunome,
				to_char(hpgdata, 'dd/mm/YYYY HH24:MI') as data,
				hpgsituacaopagamento,
				hpgparcela,
				hpgvalorparcela
			FROM
				par.historicopagamento h
			LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf
			WHERE
				h.pagid = ".$dados['pagid']."
			ORDER BY
				hpgdata DESC";
	$cabecalho = array("Usuário atualização","Data","Situação","Parcela","Valor parcela");

	$arrHist = $db->carregar($sql);
	$aHistorico = array();
	$sUltimoHistorico = '';
	foreach ($arrHist as $historico) {
		if ($sUltimoHistorico == $historico['hpgsituacaopagamento']) {
			continue;
		}
		$aHistorico[] = $historico;
		$sUltimoHistorico = $historico['hpgsituacaopagamento'];
	}
	$db->monta_lista_simples($aHistorico,$cabecalho,500,5,'N','80%',$par2);

} */

if ($_REQUEST['titleFor']) {
	if( $_REQUEST['tp'] == 2 ){
		$sql = "SELECT 
					p.pagnumeroempenho as numero,
					p.pagvalorparcela as valor,
					p.pagsituacaopagamento as situacao
				FROM 
					par.processopar pp 
				INNER JOIN par.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso and empstatus = 'A'
				LEFT JOIN par.pagamento p on p.empid = e.empid
				LEFT JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
				WHERE 
					p.pagstatus = 'A' AND
					e.empid != ".$_REQUEST['titleFor']." AND 
					es.sbaid = ".$_REQUEST['sbaid']." AND
					pp.prpid = ".$_SESSION['par_var']['prpid'];
	
		$arEmpenho = $db->carregar( $sql );
		$arEmpenho = $arEmpenho ? $arEmpenho : array();								            
		$html = '<table>
					<tr>
						<th>Nº Empenho</th>
						<th> - Valor Pago</th>
						<th> - Situação</th>
					</tr>';
		if( $arEmpenho ){
			$excP = array();
			foreach ($arEmpenho as $empenho) {
				$html .= '<tr>
							<td>'.$empenho['numero'].'</td>
							<td align="right">'.number_format($empenho['valor'],2,',','.').'</td>
							<td align="right">&nbsp;&nbsp;'.$empenho['situacao'].'</td>
						</tr>';
			}
		} else {
			echo 'Sem pagamentos';
			die();
		}
		$html .= '</table>';
	} else {
		$sql = "SELECT 
					p.pagvalorparcela as valor,
					p.pagsituacaopagamento as situacao
				FROM 
					par.processopar pp 
				INNER JOIN par.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso and empstatus = 'A'
				LEFT JOIN par.pagamento p on p.empid = e.empid
				LEFT JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A'
				WHERE 
					p.pagstatus = 'A' AND
					e.empid = ".$_REQUEST['titleFor']." AND 
					es.sbaid = ".$_REQUEST['sbaid']." AND
					pp.prpid = ".$_SESSION['par_var']['prpid'];
		
		$arEmpenho = $db->carregar( $sql );
		$arEmpenho = $arEmpenho ? $arEmpenho : array();								            
		$html = '<table>
					<tr>
						<th> Valor Pago</th>
						<th> - Situação</th>
					</tr>';
		if( $arEmpenho ){
			$excP = array();
			foreach ($arEmpenho as $empenho) {
				$html .= '<tr>
							<td align="right">'.number_format($empenho['valor'],2,',','.').'</td>
							<td align="right">&nbsp;&nbsp;'.$empenho['situacao'].'</td>
						</tr>';
			}
		} else {
			echo 'Sem pagamentos';
			die();
		}
		$html .= '</table>';
	}
	echo $html;
	die();
}

function executarPagamento($dados) {
	
	global $db;
	
	$obHabilita = new Habilita();
	$cnpj 		= $obHabilita->pegaCnpj($_SESSION['par_var']['inuid']);
	$habilitado = $obHabilita->consultaHabilitaEntidade($cnpj);
	
	if($habilitado == 'Habilitado'){
		$valor = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
		$totalpagamento = $db->pegaUm("SELECT SUM(pagvalorparcela) FROM par.pagamento WHERE empid='".$dados['empid']."' AND pagstatus='A' AND pagsituacaopagamento IN ('EFETIVADO', '2 - EFETIVADO', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA', 'Enviado ao SIGEF')");
// 		$totalempenho   = $db->pegaUm("SELECT (empvalorempenho - coalesce(vrlcancelado, 0)) FROM par.empenho e
// 											left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie from par.empenho
// 					                    where empcodigoespecie in ('03', '13', '04') and empstatus = 'A' and eobstatus = 'A'
// 					                    group by 
// 					                        empnumeroprocesso,
// 					                        empcodigoespecie,
// 					                        empidpai) as ep on ep.empidpai = e.empid WHERE e.empcodigoespecie not in ('03', '13', '02', '04') and e.empid='".$dados['empid']."'");
		$sql = "SELECT saldo FROM par.v_dadosempenhos WHERE empid = {$dados['empid']}";
		$totalempenho   = $db->pegaUm( $sql );
		
		$soma = ($totalpagamento+$valor);
		if( round($soma,2) > round($totalempenho,2) ) {
			die(SIGLA_SISTEMA. " INFORMA : Total de pagamento esta maior que o valor do empenho");
		}
                
		$res_acc = atualizaDadosContaCorrenteParPag( $dados );
		if( $res_acc ){
			$res_cc = consultarContaCorrente($dados);
		
			if($res_cc != false){
				$res_se = solicitarPagamento($dados);
			} else {
				$res_sc = solicitarContaCorrente($dados);
				echo "Conta corrente solicitada neste momento.";
				return false;
			}
		} else {
			$res_sc = solicitarContaCorrente($dados);
			echo "Conta corrente solicitada neste momento.";
			return false;
		}
	}else{
		echo $habilitado;
	}
	//echo "<script> window.location.href = window.location.href; </script>";
	//die();
	//if($res_cc=="cc_criado_sucesso") {
		//$res_cc = consultarContaCorrente($dados);	
	//}
	//$res_se = solicitarPagamento($dados);
	//if($res_cc) $res_se = solicitarPagamento($dados);
	
}

function solicitarContaCorrente($dados) {
	global $db;
	
	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

        $dadoscc = $db->pegaLinha("SELECT prpnumeroprocesso,prptipoexecucao, prpbanco, prpagencia, muncod, prptipo, inuid FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."'");
		
        if($dadoscc) {
	        // numero do processo (No desenvolvimento é fixo)
        //	if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
        //	   $_SESSION['baselogin'] == "simec_espelho_producao" ){
        	   	//$nu_processo='23034655466200900';
        //	   	$nu_processo=$dadoscc['prpnumeroprocesso'];//234000005642011
        //	} else {
	        	$nu_processo=$dadoscc['prpnumeroprocesso'];
        //	}

	        // constante=001
	        $nu_banco=$dadoscc['prpbanco'];
	        // esperando envio
	        $nu_agencia=trim($dadoscc['prpagencia']);
        }
       
		// CNPJ da prefeitura
		$obHabilita 		= new Habilita();
		$nu_identificador 	= $obHabilita->pegaCnpj($_SESSION['par_var']['inuid']);
		/*
		$nu_identificador = $db->pegaUm("SELECT iue.iuecnpj 
	                                       FROM par.instrumentounidade iu
	                                               inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
	                                       WHERE
	                                               iu.inuid = {$_SESSION['par_var']['inuid']}
	                                           and iue.iuestatus = 'A'
	                                           and iue.iuedefault = true");*/
        //$nu_identificador = $db->pegaUm("SELECT inucnpj FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);

  		// constante=1
        $tp_identificador="1";

        // constante=nulo
        $nu_conta_corrente=null;
        // constante=01
        $tp_solicitacao="01";
        // constante=0032
        $motivo_solicitacao="0032";
        // constante=nulo
        $convenio_bb=null;
        // constante=N
        $tp_conta="N";
        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
        
		if($dadoscc['prptipoexecucao'] == 'C'){
	   		$co_programa_fnde="03";
	   		$nu_sistema="2";
	    }else if($dadoscc['prptipoexecucao'] == 'T'){ 
	        $co_programa_fnde="CM"; //"ED";
	        $nu_sistema="7";
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
	//	}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

	    /*
	    echo "------ SOLICITAÇÃO DE CONTA CORRENTE ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
		*/

		$result = (integer) $xml->status->result;
		if(!$result) {
			
			 $mensagem =  
			 	'<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					ERRO AO SOLICITAR ABERTURA DE CONTA CORRENTE:
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Descrição do erro: ';
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					//echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
					$mensagem .= ' '.iconv("UTF-8", "ISO-8859-1", $err->text).' ';
				}
			}
			 $mensagem .= '
					</div>
				</div>
			 	<br>
				';
			 echo $mensagem;
			
			
			//echo "*** Descrição do erro ***\n\n";
			

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'solicitarContaCorrente - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		    return false;
		} else {
			
		    $db->executar("UPDATE par.processopar SET prpseqconta='".$xml->body->seq_solic_cr."', seq_conta_corrente='".$xml->body->nu_seq_conta."' WHERE prpid='".$_SESSION['par_var']['prpid']."'");

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'solicitarContaCorrente - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return true;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
			
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					ERRO AO SOLICITAR A CONTA CORRENTE: 
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.
					</div>
				</div>
			 	<br>
				';
			
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		//echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";
		
		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
					ERRO AO SOLICITAR A CONTA CORRENTE: 
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						'.$erroMSG.'
					</div>
				</div>
			 	<br>
				';

	}
}

function atualizaDadosContaCorrenteParPag($dados) {
	global $db;
	
    $dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prptipoexecucao, muncod, prpbanco, prpagencia, prpdatainclusao, usucpf, prpseqconta, prptipo, seq_conta_corrente, nu_conta_corrente, inuid
    						   FROM par.processopar
    						   WHERE prpid='".$_SESSION['par_var']['prpid']."'");
	
    if($dadosse) {
    	$an_processo = date("Y");
    	$nu_processo=$dadosse['prpnumeroprocesso'];
    	$tp_processo=1; // O que vai ser no PAR

    	if($dadosse['prptipoexecucao'] == 'C'){
	   		$co_programa_fnde="03";
	    }else if($dadosse['prptipoexecucao'] == 'T'){ 
	        $co_programa_fnde= "CM";//"ED";
	    }
    	
    	if($_SESSION['par_var']['esfera']=='estadual') {
        	// CNPJ do Estado
			$nu_cnpj_favorecido = $db->pegaUm("select prpcnpj from par.processopar  where prpid = ".$_SESSION['par_var']['prpid']);
			/*
			$nu_cnpj_favorecido = $db->pegaUm("SELECT DISTINCT entnumcpfcnpj 
					FROM entidade.entidade ent
					INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid AND fen.fuestatus = 'A'
					INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					INNER JOIN par.instrumentounidade iu ON iu.estuf = ende.estuf
					WHERE funid in(6) and entstatus = 'A' and iu.inuid = ".$_SESSION['par_var']['inuid']);
			*/
        }else{
        	// CNPJ da prefeitura
        	$nu_cnpj_favorecido = $db->pegaUm("select prpcnpj from par.processopar  where prpid = ".$_SESSION['par_var']['prpid']);
        	/*
			$nu_cnpj_favorecido=$db->pegaUm("SELECT ent.entnumcpfcnpj
					 				   FROM entidade.entidade ent
					 				   INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
					 				   INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 				   WHERE fen.funid=1 AND ent.entstatus = 'A' AND ende.muncod='".$dadosse['muncod']."'");
			*/
        }
        
    	if($_SESSION['par_var']['inuid'] == 1 ){
        	// CNPJ do Estado
    		$nu_cnpj_favorecido = $db->pegaUm("select prpcnpj from par.processopar  where prpid = ".$_SESSION['par_var']['prpid']);
    		/*
    			$nu_cnpj_favorecido = $db->pegaUm("SELECT DISTINCT entnumcpfcnpj 
					FROM entidade.entidade ent
					INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid AND fen.fuestatus = 'A'
					INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					INNER JOIN par.instrumentounidade iu ON iu.estuf = ende.estuf
					WHERE funid in(6) and entstatus = 'A' and iu.inuid = ".$_SESSION['par_var']['inuid']);
    		*/
			
        }
	    
    }

    $data_created = date("c");
	$usuario = $dados['wsusuario'];
	$senha   = $dados['wssenha'];
	$somente_conta_ativa	= 'S';
	$numero_de_linhas		= '200';
	/*$usuario = 'MECTIAGOT';
	$senha   = 'M3135689';*/

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
					//$urlWS = 'http://dev.fnde.gov.br/webservices/sigef/integracao/public/index.php/financeiro/cr';
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
		
	    //echo "\n\n------ SOLICITAÇÃO DE PROCESSO ------\n\n";
		//echo "|".$xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
		

		$result = (integer) $xml->status->result;
		
		if(!$result) {
		
			$mensagem = '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF: 
		 					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >';
			
			$erros = $xml->status->error->message;
			if(count($erros)>0) {	
				foreach($erros as $err) {	
			 		$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}
				$mensagem .= '</div>
			 			</div>
			 			<br>';
				
				echo $mensagem;
				
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'atualizaDadosContaCorrentePar - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
	
			$db->executar($sql);
			$db->commit();

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
				$sql = "UPDATE par.processopar SET
							prpbanco = $nu_banco,
						  	prpagencia = $nu_agencia,
						  	prpseqconta = $seq_solic_cr,
						  	seq_conta_corrente = $seq_conta,
						  	nu_conta_corrente = $nu_conta_corrente,
						  	dt_movimento = $dt_movimento,
						  	fase_solicitacao = $fase_solicitacao,
						  	co_situacao_conta = $co_situacao_conta,
						  	situacao_conta = $situacao_conta,
						  	nu_identificador = $nu_identificador,
						  	ds_razao_social = $ds_razao_social						 
						WHERE 
						  	prpid = {$_SESSION['par_var']['prpid']}";
				$db->executar($sql);
			} else {
				$mensagem = '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF: 
		 					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >';
			
					
				$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $status);
				$mensagem .= '</div>
				 			</div>
				 			<br>';
					
				echo $mensagem;
				$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'atualizaDadosContaCorrentePar - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

				$db->executar($sql);
				$db->commit();
				return false;
			}
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'atualizaDadosContaCorrentePar - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return true;
		}

}

function consultarContaCorrente($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

        $proseqconta = $db->pegaUm("SELECT prpseqconta FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."'");
		
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

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
		}

		if($proseqconta) {

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
					->execute();
			/* XML de Teste */
					/*
			$xml = 
<<<XML
<?xml version="1.0" encoding="iso-8859-1"?>
<response>
<header>
	<app>SIGEF</app>
	<version>v0.0.2</version>
	<created>2012-03-29T18:37:17</created>
</header>
<status>
	<result>1</result>
	<message>
		<code>1</code>
		<text>Consulta realizada com sucesso.</text>
	</message>
</status>
<body>
	<row>
		<seq_solic_cr>3275966</seq_solic_cr>
		<seq_conta>1136703</seq_conta>
		<dt_movimento>22-FEB-12</dt_movimento>
		<nu_banco>001</nu_banco>
		<nu_agencia>2302</nu_agencia>
		<nu_conta_corrente>0000172952</nu_conta_corrente>
		<fase_solicitacao>28 SOLICITAÇÃO FINALIZADA</fase_solicitacao>
		<co_situacao_conta>13</co_situacao_conta>
		<situacao_conta>13 - Ativa</situacao_conta><nu_processo>23400003539201174</nu_processo><nu_identificador>64037872000107</nu_identificador><ds_razao_social>PREF MUN DE ILHA COMPRIDA</ds_razao_social><status>1 - Registro encontrado.</status></row></body></response>

XML;
 	*/	
					
			$xmlRetorno = $xml;
		    $xml = simplexml_load_string( stripslashes($xml));

		    $result = (integer) $xml->status->result;
		    $resultConsultaConta = (integer) $xml->body->row->co_situacao_conta;
			/*
		    $xml->status->message->code;
			$xml->status->message->text;
			$xml->status->error->message->text;
			*/
		    $sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$_SESSION['par_var']['prpid']."',
					    		'consultarContaCorrente',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
				$db->executar($sql);
				$db->commit();
				


				
		   
		    if($result != 1 ) { // 1 = sucesso
		    	echo "------ MENSAGEM SIGEF - CONSULTA DE CONTA CORRENTE ------\n\n";
				echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
				echo "*** Erro ao consultar conta corrente ***\n\n";
				echo "* Nº do erro:".$xml->status->message->code."\n";
				echo "* Descrição do Erro:".$xml->status->error->message->text."\n";
		    	return false;
		    } else {
		    	$statusContaSucessos = array('13','11','09');
				if(in_array($resultConsultaConta, $statusContaSucessos )){
				    if($xml->body->row->seq_conta) {
				    	$db->executar("UPDATE par.processopar SET nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', seq_conta_corrente='".$xml->body->row->seq_conta."' WHERE prpseqconta='".$proseqconta."'");
				    	$db->commit();
				    }
		
					echo "------ CONSULTA DE CONTA CORRENTE ------\n\n";
					echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
					echo "*** Detalhes da consulta ***\n\n";
					echo "* Data movimento:".(($xml->body->row->dt_movimento)?$xml->body->row->dt_movimento:'-')."\n";
					echo "* Fase solicitação:".(($xml->body->row->fase_solicitacao)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->fase_solicitacao):'-')."\n";
					echo "* Entidade:".(($xml->body->row->ds_razao_social)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->ds_razao_social):'-')."(".(($xml->body->row->nu_identificador)?$xml->body->row->nu_identificador:'-').")\n\n";
					//return $result;
				
					return true;
				}else{
					echo "------ ERRO AO CONSULTAR CONTA CORRENTE ------\n\n";
					echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
					echo "* A conta corrente não está ativa.\n";
					
					return false;
				}
		    }
		    
		} else {
			echo "------ CONSULTA DE CONTA CORRENTE ------\n\n";
			echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
			echo "*** Erro de integração entre SIMEC e SIGEF ***\n\n";
			echo "* Descrição do Erro:O sequencial da conta no SIMEC não foi encontrado.\n";
			
			return false;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";
		
		return false;

	}
}


function solicitarPagamento($dados) {
	global $db;
	
	if(!$dados['empid']) {
		echo "Empenho não selecionado. Por favor, selecione um empenho";
		return false;
	}
		
	$data_created = date("c");
		
	$dadosse = $db->pegaLinha("SELECT emp.empcnpj, pro.prpseqconta, pro.seq_conta_corrente,
									  emp.empnumeroprocesso, emp.empprogramafnde, 
									  emp.empnumerosistema, emp.empanooriginal,
									  emp.empnumero, pro.prptipoexecucao, emp.empnumeroconvenio, 
									  emp.empanoconvenio, pro.prpnumeroconveniosiafi,
									  prpbanco, prpagencia, nu_conta_corrente, empcodigonatdespesa
							   FROM par.empenho emp 
							   INNER JOIN par.processopar pro ON pro.prpnumeroprocesso = emp.empnumeroprocesso and empstatus = 'A' 
							   WHERE empid='".$dados['empid']."'");
	
    if($dadosse) {
		
		// numero do processo (No desenvolvimento é fixo)
		/*
        if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
           $_SESSION['baselogin'] == "simec_espelho_producao" ){
           	
            $usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
			$nu_processo="23034655466200900";
			$nu_documento_siafi_ne = "340001";
//			$nu_cgc_favorecido = "12262713000102";
			$nu_cgc_favorecido = "15024029000180";
			$nu_seq_conta_corrente_favorec = "510793";
			
       	} else {*/
			$usuario = $dados['wsusuario'];
			$senha   = $dados['wssenha'];
			$nu_processo = $dadosse['empnumeroprocesso'];
			$nu_documento_siafi_ne = substr($dadosse['empnumero'],strpos($dadosse['empnumero'], 'NE')+2);
			$nu_cgc_favorecido = $dadosse['empcnpj'];
			$nu_seq_conta_corrente_favorec = $dadosse['seq_conta_corrente'];
       // }

		$nu_cpf_favorecido = null;
		$nu_banco = $dadosse['prpbanco'];
		$nu_agencia = $dadosse['prpagencia'];
		$nu_conta_corrente = $dadosse['nu_conta_corrente'];
		$an_convenio_original = $dadosse['empanoconvenio'];
		$nu_convenio_original = $dadosse['empnumeroconvenio'];
		$nu_convenio_siafi = null;
		$nu_proposta_siconv = null;
		$termo_aditivo_original = null;
		$apostilamento_original = null;
		/*$vl_custeio = "0";
		$vl_capital = str_replace(array(".",","),array("","."),$dados['valorpagamento']);*/
		
    // Custeio
		if( $dadosse['empcodigonatdespesa'] == 33304100 || $dadosse['empcodigonatdespesa'] == 33404100 ){
			$vl_custeio = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
			$vl_capital = "0";
		} else { // Capital
			$vl_custeio = "0";
			$vl_capital = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
		}
		
		$an_referencia = date("Y");
		$an_exercicio = $dadosse['empanoconvenio']; //empanoconvenio //date("Y");
		$an_convenio = $dadosse['empanoconvenio'];
		$sub_tipo_documento = "01";
		$nu_sistema = $dadosse['empnumerosistema'];
		$unidade_gestora = "153173";
		$gestao = "15253";
		//$co_programa_fnde = $dadosse['empprogramafnde'];
		$parcela = $dados['pagparcela'];
		$darf = null;
		$tp_avaliador = null;
		$id_solicitante = null;
		
		/*
		$an_exercicio = $db->pegaUm("SELECT to_char(hepdata,'YYYY') as ano FROM par.historicoempenho 
									 WHERE empid='".$dados['empid']."' ORDER BY hepdata ASC LIMIT 1");
 		*/
			
    	$dadosNE = explode("NE", $dadosse['empnumero']);
		$an_exercicio = $dadosNE[0];

		if(!$an_exercicio){
			$an_exercicio = $db->pegaUm("SELECT to_char( empdata, 'YYYY' ) as ano FROM par.empenho
										 WHERE empid='".$dados['empid']."'");
		}
				
		$nu_mes = sprintf("%02d", $dados['mes']);				
		$valor = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
		$numeroconvenioSIAFI = $dadosse['prpnumeroconveniosiafi'];
		
		if( $dados['tipo'] != 'visualiza' ){
			$sql = "SELECT distinct l.lwsid FROM par.logws l
						inner join par.historicowsprocessopar h ON l.lwsid = h.lwsid
					WHERE
						h.prpid = {$_SESSION['par_var']['prpid']}
						and h.hwpxmlretorno is null
						and h.hwpdataenvio = (select max(hwpdataenvio) from par.historicowsprocessopar where prpid = {$_SESSION['par_var']['prpid']})
						and l.lwstiporequest = '05'";
        	$request_id = $db->pegaUm($sql);
        	
        	if( empty($request_id) ){
		        $arrParam = array(
		        		'lwstiporequest' => '05',
		        		'usucpf' 		 => $_SESSION['usucpf']
		        );
		        $request_id = logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'insert' );
        	}
        	$arrParam = array(
        			'mes' 				=> $dados['mes'],
        			'ano' 				=> $an_referencia,
        			'exercicio'			=> $an_exercicio,
        			'pagparcela'		=> $dados['pagparcela'],
        			'valor' 			=> $valor,
        			'empnumero' 		=> $dadosse['empnumero'],
        			'empid' 			=> $dados['empid'],
        			'request_id' 		=> $request_id,
        			'sistema' 			=> 'PAR',
        			'obra_sub'			=> $dados['sbaid'],
        			'percentual'		=> $dados['porcent'],
        			'anosubacao'		=> $dados['anosubacao'],
        			'vlrpagamentoItem' 	=> $dados['valorpagamentoobra'],
        	);
        	$id_pagamento = salvarDadosPagamento( $arrParam );
		}
		
    	if($dadosse['prptipoexecucao'] == 'C'){
	   		$co_programa_fnde="03";
	   		$an_referencia 	= date("Y"); //empanoconvenio //date("Y");
	   		$an_item 		= date("Y"); //$dadosse['empanoconvenio'];
	   		//<request_id>$request_id</request_id>
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
			<request_id>$id_pagamento</request_id>
			<nu_cgc_favorecido>$nu_cgc_favorecido</nu_cgc_favorecido>
			<nu_seq_conta_corrente_favorec>$nu_seq_conta_corrente_favorec</nu_seq_conta_corrente_favorec>
			<nu_processo>$nu_processo</nu_processo>
			<an_convenio_original>$an_convenio_original</an_convenio_original>
			<nu_convenio_original>$nu_convenio_original</nu_convenio_original>
			<nu_convenio_siafi>$numeroconvenioSIAFI</nu_convenio_siafi>
			<vl_custeio>$vl_custeio</vl_custeio>
			<vl_capital>$vl_capital</vl_capital>
			<an_referencia>$an_referencia</an_referencia>
			<sub_tipo_documento>$sub_tipo_documento</sub_tipo_documento>
			<nu_sistema>$nu_sistema</nu_sistema>
			<unidade_gestora>$unidade_gestora</unidade_gestora>
			<gestao>$gestao</gestao>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<detalhamento_pagamento>
			<item>
				<nu_parcela>$parcela</nu_parcela>
				<an_exercicio>$an_exercicio</an_exercicio>
				<vl_parcela>$valor</vl_parcela>
				<an_parcela>$an_item</an_parcela>
				<nu_mes>{$nu_mes}</nu_mes>
				<nu_documento_siafi_ne>{$nu_documento_siafi_ne}</nu_documento_siafi_ne>
			</item>
			</detalhamento_pagamento>
		</params>
	</body>
</request>
XML;
	   		//$nu_sistema="2";
	    }else if($dadosse['prptipoexecucao'] == 'T'){ 
	       // $co_programa_fnde="ED"; 
	        $co_programa_fnde= "CM";
       //<request_id>$request_id</request_id>
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
			<request_id>$id_pagamento</request_id>
			<nu_cgc_favorecido>$nu_cgc_favorecido</nu_cgc_favorecido>
			<nu_seq_conta_corrente_favorec>$nu_seq_conta_corrente_favorec</nu_seq_conta_corrente_favorec>
			<nu_processo>$nu_processo</nu_processo>
			<vl_custeio>$vl_custeio</vl_custeio>
			<vl_capital>$vl_capital</vl_capital>
			<an_referencia>$an_referencia</an_referencia>
			<sub_tipo_documento>$sub_tipo_documento</sub_tipo_documento>
			<nu_sistema>$nu_sistema</nu_sistema>
			<unidade_gestora>$unidade_gestora</unidade_gestora>
			<gestao>$gestao</gestao>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<detalhamento_pagamento>
			<item>
				<nu_parcela>$parcela</nu_parcela>
				<an_exercicio>$an_exercicio</an_exercicio>
				<vl_parcela>$valor</vl_parcela>
				<an_parcela>$an_referencia</an_parcela>
				<nu_mes>{$nu_mes}</nu_mes>
				<nu_documento_siafi_ne>{$nu_documento_siafi_ne}</nu_documento_siafi_ne>
			</item>
			</detalhamento_pagamento>
		</params>
	</body>
</request>
XML;
	    }

	    if( $dados['tipo'] == 'visualiza' ){
			echo '<pre>';		
			echo simec_htmlentities($arqXml);
			exit;
	    }
		   		
		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'https://hmg.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
		}
		$arrParam = array(
				'lwsrequestdata' => 'now()',
				'lwsurl' 		 => $urlWS,
				'lwsmetodo' 	 => 'solicitar',
				'lwsid' 		 => $request_id,
				'pagid' 		 => $id_pagamento
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
		
		$arrParam = array(
				'prpid' 		=> $_SESSION['par_var']['prpid'],
				'lwsid' 		=> $request_id,
				'hwpxmlenvio' 	=> str_replace( "'", '"', $arqXml),
				'hwpdataenvio' 	=> 'now()',
				'usucpf' 		=> $_SESSION['usucpf']
		);
		$hwpid = logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'insert' );
	
	try {
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();
		
		$arrParam = array(
				'lwsresponsedata' 	=> 'now()',
				'lwsid' 			=> $request_id
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );

		$xmlRetorno = $xml;
		
		$arrParam = array(
				'hwpid'			=> $hwpid,
				'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno)
		);
		logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );
							
		$xml = simplexml_load_string( stripslashes($xml));
		
	    echo "------ SOLICITAÇÃO DE PAGAMENTO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
					
		$result = (integer) $xml->status->result;

		// simulando sem validacão do XML
		// $result = true;
		if(!$result) {
			$sql = "UPDATE par.pagamento SET pagstatus = 'I' WHERE pagid = $id_pagamento";
			$db->executar($sql);
			$db->commit();
			
			$arrParam = array(
					'hwpid' 		=> $hwpid,
					'hwpwebservice' => 'solicitarPagamento - Erro'
			);
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );
			
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message->text;
			
			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err);
				}
			} else {
				echo "* ".iconv("UTF-8", "ISO-8859-1", $erros[0]);
			}
			
			$arrParam = array(
					'lwserro' => true,
					'lwsid' => $request_id,
					'lwsmsgretorno' => utf8_decode($xml->status->error->message->text)
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
			
		} else {
			$arrParam = array(
					'lwserro' => false,
					'lwsid' => $request_id
			);
			logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
			
			$arrParam = array(
					'hwpid' 		=> $hwpid,
					'hwpwebservice' => 'solicitarPagamento - Sucesso'
			);
			logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );
			
			$sql = "UPDATE par.pagamento SET
  						parnumseqob = ".(($xml->body->nu_registro_ob)?"'".$xml->body->nu_registro_ob."'":"NULL")."
			  		WHERE pagid = $id_pagamento";
			$db->executar($sql);
			$db->commit();
		}
	} catch (Exception $e){
		$arrParam = array(
				'lwserro' 			=> true,
				'lwsresponsedata' 	=> 'now()',
				'lwsid' 			=> $request_id
		);
		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
	
		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Solicitar Pagamento encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
		
		$arrParam = array(
				'hwpid' 		=> $hwpid,
				'hwpwebservice' => 'solicitarPagamento - Erro',
				'hwpxmlretorno' => str_replace( "'", '"', $xmlRetorno).' - Erro Exception: '.$erroMSG
		);
		logWsRequisicao($arrParam, 'hwpid', 'par.historicowsprocessopar', 'alter' );

		echo "Erro-WS Solicitar Pagamento no SIGEF: $erroMSG";
		echo "<pre>";
	}
	}
}

function dadosPagamento($dados) {
	
	global $db;
	
	$sql = "SELECT CASE WHEN itrid = 1 THEN 'estadual' ELSE 'municipal' END as esfera 
			FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid'];
	
	$_SESSION['par_var']['esfera'] = $db->pegaUm($sql);
	$tpcid = $_SESSION['par_var']['esfera'] == 'estadual' ? 1 : 3;
	if($_SESSION['par_var']['inuid'] == 1){
		$tpcid = 1;
	}

	echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
	echo "<tr><td class=SubTituloCentro>Inserir nova parcela</td></tr>";
	echo "</table>";
	
	$sql = "SELECT 	
				'<center>'||
				CASE 
					WHEN ROUND(dados.valorempenho) <= ROUND(dados.pagamentoempenho) THEN '<a title=\"Empenho 100% pago.\" style=\"cursor:pointer\"> EMP </a>' 
					WHEN ROUND(dados.valorsubacao) <= ROUND(dados.pagamentooutros)  THEN '<a title=\"Subação 100% paga.\" style=\"cursor:pointer\"> SUB </a>' 
					ELSE dados.chk
				END||'</center>' as chk,
				dados.sbaid,
				dados.sbaid||' - '||dados.sbadsc as sbadsc,
				dados.eobano, 
				dados.valorempenho,
				dados.valorsubacao,
				dados.pagamentoempenho,
				dados.pagamentooutros,
				case when dados.valorsubacao = '0.00' then 0.00 else (dados.pagamentooutros/dados.valorsubacao )*100 end as perc_pago,
				--(dados.pagamentooutros/dados.valorsubacao)*100 as perc_pago,
				'<input type=text class=disabled onblur=\"MouseBlur(this);this.value=mascaraglobal(\'[#]\',this.value);calcularValorPagamento(this);\" onmouseout=MouseOut(this); onfocus=MouseClick(this);this.select(); onmouseover=MouseOver(this); onkeyup=\"this.value=mascaraglobal(\'[#]\',this.value);calcularValorPagamento(this);\" maxlength=3 size=7 id=porcent name=porcent['||dados.sbaid||'] style=text-align:; disabled>' as porcentpagamento, 
				'<input type=text class=disabled onblur=\"MouseBlur(this);calcularValorPagamento(this);\" onmouseout=MouseOut(this); onfocus=MouseClick(this);this.select(); onmouseover=MouseOver(this); onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value);calcularValorPagamento(this);\" maxlength=20 size=21 id=valorpagamentoobra name=valorpagamentoobra['||dados.sbaid||'] style=text-align:; disabled>' as valorpagamento,
				dados.qtdParcela + 1 as parcelaatual 
			FROM (
				SELECT 
					'<input type=checkbox name=sbaid[] value='||s.sbaid||' onclick=\"marcarSubacao(this);\"><input type=hidden name=anosubacao[] value='||eo.eobano||'>' as chk, 
					s.sbaid,
					s.sbadsc,
					--p.pagid,
					--(((SELECT sum(eobvalorempenho) FROM par.empenhosubacao WHERE sbaid = s.sbaid and eobstatus = 'A' AND empid = eo.empid ) + ref.reevlr) - coalesce(ep.vrlcancelado, 0)) as valorempenho,
					(SELECT sum(saldo) FROM par.v_dadosempenhosubacao ves WHERE ves.sbdid = sd.sbdid) as valorempenho,
					(SELECT par.recuperaValorValidadosSubacaoPorAno(s.sbaid , sd.sbdano)::numeric(20,2)) AS valorsubacao,
					(SELECT SUM(pobvalorpagamento) FROM par.pagamentosubacao WHERE pagid IN (
	SELECT pagid FROM par.pagamento WHERE empid = ".$dados['empid']." AND pagstatus = 'A' and pagsituacaopagamento not ilike '%cancelado%'
)  and pobstatus = 'A' AND sbaid= s.sbaid AND pobano <= date_part('year', now()))::numeric(20,2) as pagamentoempenho,
					-- (SELECT SUM(pobvalorpagamento) FROM par.pagamentosubacao WHERE sbaid= s.sbaid AND pobano <= date_part('year', now()))::numeric(20,2) as pagamentooutros,
					( 	SELECT SUM(ps.pobvalorpagamento) 
						FROM par.pagamentosubacao ps 
						INNER JOIN par.pagamento pg ON pg.pagid = ps.pagid and pg.pagstatus = 'A' AND pagsituacaopagamento not ilike '%CANCELADO%'
						WHERE ps.sbaid = s.sbaid and pobstatus = 'A' AND ps.pobano = sd.sbdano  ) as pagamentooutros,
					eo.eobano,
                                        (SELECT COUNT(pobvalorpagamento) FROM par.pagamentosubacao WHERE pagid IN (
	SELECT pagid FROM par.pagamento WHERE empid = ".$dados['empid']." AND pagstatus = 'A'
)  and pobstatus = 'A' AND sbaid= s.sbaid AND pobano <= date_part('year', now())) as qtdParcela
				FROM par.empenhosubacao eo
					INNER JOIN par.subacao 			s  ON s.sbaid = eo.sbaid 
					INNER JOIN par.subacaodetalhe 	sd ON sd.sbaid = s.sbaid AND sd.sbdano = eo.eobano
					inner join par.empenho emp on emp.empid = eo.empid and emp.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
					left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie from par.empenho
			                    where empcodigoespecie in ('03', '13', '04') and empstatus = 'A'
			                    group by 
			                        empnumeroprocesso,
			                        empcodigoespecie,
			                        empidpai) as ep on ep.empidpai = emp.empid
					left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as reevlr, empcodigoespecie 
								from par.empenho
								where empcodigoespecie in ('02')
								group by 
									empnumeroprocesso,
									empcodigoespecie,
									empidpai) as ref on ref.empidpai = emp.empid 
				WHERE 
					eo.empid='".$dados['empid']."'
					and eo.eobstatus = 'A'
					AND s.sbaid in (SELECT distinct es.sbaid 
									FROM par.processopar pp
                                   	INNER JOIN par.vm_documentopar_ativos		dp  ON dp.prpid = pp.prpid
                          			INNER JOIN par.empenho 						e   ON e.empnumeroprocesso = pp.prpnumeroprocesso and e.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
                             		INNER JOIN par.empenhosubacao 				es  ON es.empid = e.empid and eobstatus = 'A'
                             		-- INNER JOIN par.documentoparvalidacao 	dpv ON dpv.dopid = dp.dopid and dpv.dpvstatus = 'A'
                                 	WHERE
                                        	-- dpv.dpvdatavalidacao IS NOT NULL AND
                                             e.empid = {$dados['empid']})

			) AS dados
			GROUP BY  chk, sbadsc, sbaid, eobano, pagamentoempenho, valorsubacao, pagamentooutros, valorempenho, dados.qtdparcela";
 	//ver(simec_htmlentities($sql),d);

	$cabecalho = Array("&nbsp;","Descrição da Subação","Ano", "Valor Empenhado (R$)", "Valor da Subação(R$)", "Valor Pago Nesse Empenho(R$)", "Valor Total Pago (R$)", "% Pago da Subação", "% Pagamento","Valor pagamento(R$)","Parcela");
	$arr = $db->carregar($sql);

	//trato o detalhamento do pagamento
	$xx = 0;
	$arRegistro = array();
	if( is_array($arr) ){
		foreach ($arr as $chave => $c) {
			foreach ($c as $key => $d) {
				$arRegistro[$xx][$key] = $c[$key];
			}
			$xx++;
		}
	}

	
	
	echo '<table align="center" cellspacing="0" cellpadding="2" border="0" width="95%" class="listagem">';
	echo '<thead>';
	echo '<tr>';
	foreach( $cabecalho as $cab ){
		echo '<td align="" bgcolor="" valign="top"><strong>'.$cab.'</strong></td>';
	}
	echo '</tr>';
	echo "</thead>";
	$total  = 0;
	$total2 = 0;
	$total3 = 0;
	$total4 = 0;
	foreach( $arRegistro as $arr ){
		echo '<tr bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
		foreach( $arr as $k => $v ){
			if( $k != 'sbaid' ){
				if( $k == 'valorsubacao' ){
					echo "<td><input type=hidden name=valorobra[".$arr['sbaid']."] id=valorobra value=".number_format($v,2,'.','').">".number_format($v,2,',','.')."</td>";
					$total = $total + $v;
				} elseif( $k == 'pagamentoempenho' ) {
					echo "<td>
							<a onmouseover=\"SuperTitleAjax('/par/par.php?modulo=principal/solicitacaoPagamentoPar&acao=A&titleFor=".$dados['empid']."&sbaid=".$arr['sbaid']."&tp=1',this);\" onmouseout=\"SuperTitleOff(this);\" href=\"\" >
								".number_format($v,2,',','.')."
							</a>
							<input type=hidden name=valorpagoempenho[".$arr['sbaid']."] id=valorpagoempenho value=".number_format($v,2,'.','').">
						  </td>";
					$total2 = $total2 + $v;
				} elseif( $k == 'pagamentooutros' ) {
					echo "<td>
							<a onmouseover=\"SuperTitleAjax('/par/par.php?modulo=principal/solicitacaoPagamentoPar&acao=A&titleFor=".$dados['empid']."&sbaid=".$arr['sbaid']."&tp=2',this);\" onmouseout=\"SuperTitleOff(this);\" href=\"\" >
								".number_format($v,2,',','.')."
							</a>
							<input type=hidden name=pagamentooutros[".$arr['sbaid']."] id=pagamentooutros value=".number_format($v,2,'.','').">
						  </td>";
					$total3 = $total3 + $v;
				} elseif( $k == 'valorempenho' ) {
					echo "<td><input type=hidden name=valorempenhado[".$arr['sbaid']."] id=valorempenhado value=".number_format($v,2,'.','').">".number_format($v,2,',','.')."</td>";
					$total4 = $total4 + $v;
				} elseif( $k == 'perc_pago' ) {
					echo "<td>
							<center>
								<input type=hidden name=valorempenhado[".$arr['preid']."] id=valorempenhado value=".number_format($v,2,'.','').">
								<a onmouseover=\"SuperTitleAjax('/par/par.php?modulo=principal/solicitacaoPagamentoPar&acao=A&titleFor=".$dados['empid']."&sbaid=".$arr['sbaid']."&tp=2',this);\" onmouseout=\"SuperTitleOff(this);\" href=\"\" >
									".number_format($v,0)." %
								</a>
							</center>
						</td>";
				} else {
					echo '<td>'.$v.'</td>';
				}
			}
		}
		echo '';
		echo '</tr>';
	}
	$geral = $total2 + $total3;
	
	//total
	
	echo "<tr bgcolor=#E9E9E9>";
	echo "<td><input type='hidden' name='hdvalor' id='hdvalor' value='".$geral."'><b>Total:</b></td>";
	echo "<td></td>";
	echo "<td></td>";
	echo "<td>".number_format($total4,2,',','.')."</td>";
	echo "<td>".number_format($total,2,',','.')."</td>";
	echo "<td>".number_format($total2,2,',','.')."</td>";
	echo "<td>".number_format($total3,2,',','.')."</td>";
	echo "<td></td>";
	echo "<td></td>";
	echo "<td align=center>".campo_texto('valorpagamento','N','S','','20','20','[.###],##','','','','','id="valorpagamento" readonly=readonly')."</td>";
	echo "<td></td>";
	echo "</tr>";
	
	
	// Linha com restante a pagar
	
	$sql = "SELECT (empvalorempenho - coalesce(vrlcancelado, 0)) FROM par.empenho e
						left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie from par.empenho
                    where empcodigoespecie in ('03', '13', '04') and empstatus = 'A'
                    group by 
                        empnumeroprocesso,
                        empcodigoespecie,
                        empidpai) as ep on ep.empidpai = e.empid WHERE e.empcodigoespecie not in ('03', '13', '02', '04') and e.empid='".$dados['empid']."'";
	$valor = $db->pegaUm( $sql );
	
	echo "<tr bgcolor=#DCDCDC>";
	echo "<td colspan=9 align='right'>";
	echo "<b>Restante a pagar(R$):</b>";
	echo "</td>";
	echo "<td>";
	echo number_format(($valor-$total2),2,',','.');
	echo "</td>";
	echo "<td>";
	echo "<input type='hidden' id='valempid' value='".number_format(($valor-$total2),2,'.','')."'>";
	echo "</td>";
	echo "</tr>";
	
	// parcelas
	
	$parcela = $db->pegaUm("SELECT COALESCE(MAX(p.pagparcela),0) as parcela FROM par.pagamento p
				 			WHERE p.empid = ".$dados['empid']." AND p.pagstatus='A'");
	
	$sql_mes = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses";
	$sql_ano = "SELECT ano as codigo, ano as descricao FROM public.anos";	
	
	echo "<tr bgcolor=#DCDCDC>";
	echo "<td align=center colspan=11><input type=hidden name=pagparcela value=".($parcela+1)." />";
			//<b>Parcela: ".($parcela+1)."</b><input type=hidden name=pagparcela value=".($parcela+1)." />&nbsp;&nbsp;&nbsp;
	echo "Mês: ".$db->monta_combo('mes', $sql_mes, 'S', 'Selecione', '', '', '', '', 'S', 'mes', true, date("m"))."&nbsp;&nbsp;&nbsp;";
	echo "Ano: ".$db->monta_combo('ano', $sql_ano, 'S', 'Selecione', '', '', '', '', 'S', 'ano', true, date("Y"))."&nbsp;&nbsp;&nbsp;";
	
	$perfil = pegaPerfilGeral();
	//regras de acesso passada por Thiago em 24/05/2012
	if( in_array(PAR_PERFIL_PAGADOR, $perfil) || 
		in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || 
		in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)
	){		
		echo "<input type=\"button\" name=\"solicitar\" id=\"solicitar\" value=\"Solicitar pagamento\" disabled=\"disabled\" onclick=\"solPag();\" />";
		
		/*$acaoConsultar = "'<img src=../imagens/refresh2.gif style=cursor:pointer; onclick=consultarPagamento('||p.pagid||','||e.empnumeroprocesso||'); >'";
		$acaoCancelar = "'<img src=../imagens/excluir.gif style=cursor:pointer; onclick=cancelarPagamento('||p.pagid||','||e.empnumeroprocesso||');>'";
		$acaoVerobras = "'<img src=../imagens/consultar.gif style=cursor:pointer; onclick=verObrasPagamento('||p.pagid||');>'";*/
	}else{
		echo "<input type=\"button\" name=\"but_sol\" id=\"but_sol\" value=\"Solicitar pagamento\" disabled=\"disabled\" onclick=\"solPag();\" />";
		/*$acaoConsultar = "''";
		$acaoCancelar = "''";
		$acaoVerobras = "''";	*/	
	}		
	
	
		
	if( in_array( PAR_PERFIL_SUPER_USUARIO, pegaArrayPerfil($_SESSION['usucpf']) ) ){
		echo "<input type=button id=visualizar name=visualizar  value=Visualizar XML onclick=visPag(); />";
	}
	//echo "</td></tr>";
	echo "</td></tr></table><br>";
	
	carregaDadosPagamento( $dados['empid'] );
	/* $sql = "SELECT 
				'<img align=absmiddle src=../imagens/mais.gif style=cursor:pointer; title=mais onclick=\"carregarHistoricoPagamentoPar('||p.pagid||',this);\">' as mais,
				$acaoConsultar as consultar,
				$acaoCancelar as cancelar,
				$acaoVerobras as verobras,
				p.pagparcela, 
				p.pagmes,
				p.paganoparcela, 
				p.pagvalorparcela,
				u.usunome,
				COALESCE(p.pagsituacaopagamento,'-') as situacao,
				p.paganoexercicio
			FROM 
				par.pagamento p 
				INNER JOIN par.empenho e ON e.empid = p.empid and empstatus = 'A' 
				LEFT JOIN seguranca.usuario u ON u.usucpf = p.usucpf
			WHERE
				p.empid = ".$dados['empid']." AND (pagstatus='A' or pagstatus='V')
			ORDER BY
				pagparcela";
	
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	$arrRegistro = array();
	foreach ($arrDados as $key => $v) {
	
		$boAprovada 	= stristr(trim($v['situacao']), 'SOLICITAÇÃO');
		$boAprovada 	= ( !empty($boAprovada) ? true : false );
	
		$boAutorizado 	= stristr(trim($v['situacao']), 'AUTORIZADO');
		$boAutorizado 	= ( !empty($boAutorizado) ? true : false );
	
		$boEnvioSiaf 	= stristr(trim($v['situacao']), 'ENVIADO AO SIAFI');
		$boEnvioSiaf 	= ( !empty($boEnvioSiaf) ? true : false );
	
		$boSemEnvio 	= stristr(trim($v['situacao']), 'SEM ENVIO');
		$boSemEnvio 	= ( !empty($boSemEnvio) ? true : false );
	
		$boRejeitado 	= stristr(trim($v['situacao']), 'REJEITADO');
		$boRejeitado 	= ( !empty($boRejeitado) ? true : false );
	
		$boCancelado 	= stristr(trim($v['situacao']), 'CANCELADO');
		$boCancelado 	= ( !empty($boCancelado) ? true : false );
	
		$boDevolvido 	= stristr(trim($v['situacao']), 'DEVOLVIDO');
		$boDevolvido 	= ( !empty($boDevolvido) ? true : false );
	
		$boCanobGerado 	= stristr(trim($v['situacao']), 'CANOB GERADO');
		$boCanobGerado 	= ( !empty($boCanobGerado) ? true : false );
	
		$boValaGestao 	= stristr(trim($v['situacao']), 'VALA CENTRO DE GESTÃO');
		$boValaGestao 	= ( !empty($boValaGestao) ? true : false );
	
		$boValaSiaf 	= stristr(trim($v['situacao']), 'VALA SIAFI');
		$boValaSiaf 	= ( !empty($boValaSiaf) ? true : false );
	
		$boCancelDevol 	= stristr(trim($v['situacao']), 'CANCELADO/DEVOLVIDO');
		$boCancelDevol 	= ( !empty($boCancelDevol) ? true : false );
	
		$boAgAjReem 	= stristr(trim($v['situacao']), 'AGUARDANDO AJUSTE PARA REEMISSÃO');
		$boAgAjReem 	= ( !empty($boAgAjReem) ? true : false );
	
		$boEfetivado 	= stristr(trim($v['situacao']), 'EFETIVADO');
		$boEfetivado 	= ( !empty($boEfetivado) ? true : false );
	
		$boHabilitaCancelamento = true;
	
		if( $boAprovada ){
			$boHabilitaCancelamento = true;
		}
		if( $boAutorizado || $boEnvioSiaf || $boSemEnvio || $boRejeitado || $boCancelado || $boDevolvido || $boCanobGerado || $boValaGestao || $boValaSiaf || $boCancelDevol || $boAgAjReem){
			$boHabilitaCancelamento = false;
		}
		if( $boEfetivado ){
			$boHabilitaCancelamento = false;
		}
		
		array_push($arrRegistro, array(
									'mais' => $v['mais'],
									'acao' => '<center>'.$v['verobras'].' '.$v['consultar'].' '.($boHabilitaCancelamento ? $v['cancelar'] : '').'</center>',
									'parcela' => $v['pagparcela'],
									'pagmes' => $v['pagmes'],
									'paganoparcela' => $v['paganoparcela'],
									'pagvalorparcela' => $v['pagvalorparcela'],
									'usunome' => $v['usunome'],
									'situacao' => $v['situacao'],
									'paganoexercicio' => $v['paganoexercicio']
								)
					);
	}
	
	echo "<input type=hidden name=empid id=empid value=".$dados['empid'].">";
	
	echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
	echo "<tr><td class=SubTituloCentro>Dados de pagamentos</td></tr>";
	echo "</table>";
	$cabecalho = array("&nbsp;","&nbsp;","Parcela","Mês","Ano","Valor(R$)","Usuário criação","Situação pagamento","Exercício");
	$db->monta_lista_simples($arrRegistro,$cabecalho,500,5,'N','100%',$par2); */

}

function listaEmpenho($dados) {
	global $db;
	
	$sql = "SELECT DISTINCT prpnumeroprocesso, sisid, prptipoexecucao FROM par.processopar WHERE prpid = ".$_SESSION['par_var']['prpid'];
// 	ver($sql);
	$arrProcesso = $db->pegaLinha($sql);
	
	$empid = $dados['empid'] ? $dados['empid'] : 0;
	
	$sql = "select distinct md.mdonome, md.mdoid,
				(select count(empid) from par.empenho where empnumeroprocesso = pp.prpnumeroprocesso and empstatus = 'A' ) as empenho,
                md.mdoqtdvalidacao, 
                (select count(dpvcpf) from par.documentoparvalidacao where dopid = dp.dopid and dpvstatus = 'A') as validado
			from par.vm_documentopar_ativos dp 
				left join par.modelosdocumentos md on md.mdoid = dp.mdoid and md.tpdcod in (21, 102) and md.mdostatus = 'A'
				inner join par.processopar pp on pp.prpid = dp.prpid
				left join par.documentoparvalidacao dv on dv.dopid = dp.dopid and dv.dpvstatus = 'A'
			where
				dp.prpid = {$_SESSION['par_var']['prpid']}";
// 	ver($sql);
	$arrDocumento = $db->pegaLinha($sql);
	
	$termoEX = $db->carregarColuna("select mdoid from par.modelosdocumentos where mdonome ilike '%_EX'");
	$termoEX = $termoEX ? $termoEX : array();
	
	$esfera = $db->pegaUm("select case when estuf is not null then 'estadual' else 'municipal' end from par.instrumentounidade where inuid = {$_SESSION['par_var']['inuid']}");
	
	$arrErros = array();
	if($esfera == 'estadual') {		
		$label = 'Estado';
		if( $arrProcesso['prptipoexecucao'] == 'T' ){ // Se for termo
			if( !strpos( strtolower($arrDocumento['mdonome']) , strtolower('estado') ) ){
				$arrErros[] = 'O termo do Estado ainda não foi gerado.';
			}
		}
	}else{
		$label = 'Município';
		if( $arrProcesso['prptipoexecucao'] == 'T' ){ // Se for termo
			if( !strpos( strtolower($arrDocumento['mdonome']) , strtolower('municípios') ) && !strpos( strtolower($arrDocumento['mdonome']) , strtolower('municipios') ) ){
				$arrErros[] = 'O termo do Município ainda não foi gerado.';
			}
		}
	}
	
	if( !in_array($arrDocumento['mdoid'], $termoEX) ){
		if( $arrProcesso['prptipoexecucao'] == 'T' ){ // Se for termo
			if( (int)$arrDocumento['mdoqtdvalidacao'] != (int)$arrDocumento['validado'] && !empty($arrDocumento['mdonome']) ){
				$arrErros[] = 'O termo do '.$label.' foi gerado mas não foi validado.';
			}
		}
	}
	
	if( (int)$arrDocumento['empenho'] == 0 ){
		$arrErros[] = 'Não possui empenho para este processo.';
	}
	 
	if( !empty($arrErros) ){
		
		$arrArray = array(
						array('<span style="color: red;"><b>Não é possível efetuar o pagamento, pois existem pendências.</b><br>* '.implode('<br>* ', $arrErros).'</span>')
						);
		
		$cabecalho = array("Termos");
		$db->monta_lista_simples($arrArray, $cabecalho, 500, 5, 'N', '100%', $par2);
	} else {
		$sql = "SELECT DISTINCT
					radio,
					empnumero,
					vpsvinculacao,
					empfonterecurso||'&nbsp;' as empfonterecurso,
					valor,
					valor_pago,
					empsituacao,
					iuecnpj, 
					iuenome, 
					empprotocolo
				FROM(
					SELECT distinct
						'<input type=hidden id='|| empid ||' value=\"'|| (empvalorempenho - coalesce(vrlcancelado,0)) ||'\"/> 
						<input type=radio class=teste name=empid value='|| empid ||' onclick=\"verDadosPagamento(this.value);\" />' as radio, 
						empnumero,
						vps.vpsvinculacao,
						e.empfonterecurso,
						(empvalorempenho - coalesce(vrlcancelado,0)) as vrlempcancelado, 
						cast(( (empvalorempenho + coalesce(er.vrlreforco,0)) - coalesce(vrlcancelado,0)) as numeric(20,2)) as valor,
	                    ( SELECT
	                      	sum(sub_p.pagvalorparcela) AS valor_pago
	                      FROM par.pagamento  sub_p  
	                      WHERE
	                      	sub_p.pagsituacaopagamento not ilike '%CANCELADO%' 
	                        AND sub_p.empid = e.empid
	                        AND sub_p.pagstatus = 'A'
	                    ) AS valor_pago,
						empsituacao,
						public.formata_cpf_cnpj(iue.iuecnpj) as iuecnpj,
						iue.iuenome, 
						e.empprotocolo||'&nbsp;' as empprotocolo
					FROM 
						par.empenho e
                    inner join par.instrumentounidadeentidade iue on iue.iuecnpj = e.empcnpj and empstatus = 'A'
                    inner join par.processopar p on p.prpcnpj = e.empcnpj
                    LEFT  JOIN par.vinculacaoptressigef	vps ON vps.vpsptres = e.empcodigoptres 
                    left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlcancelado, empcodigoespecie from par.empenho
		                    where empcodigoespecie in ('03', '13', '04') and empstatus = 'A'
		                    group by 
		                        empnumeroprocesso,
		                        empcodigoespecie,
		                        empidpai) as ep on ep.empidpai = e.empid
                    left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie 
                    		from par.empenho
		                    where empcodigoespecie in ('02') and empstatus = 'A'
		                    group by 
		                        empnumeroprocesso,
		                        empcodigoespecie,
		                        empidpai) as er on er.empidpai = e.empid
                WHERE
					e.empnumeroprocesso = '{$arrProcesso['prpnumeroprocesso']}'
                    and iue.iuestatus = 'A' 
                    and e.empcodigoespecie not in ('03', '13', '02', '04')
                    --and iue.iuedefault = true
			) as foo
			where vrlempcancelado > 0";
		
		$cabecalho = array("&nbsp;","N° do Empenho", "N° da Vinculação", "Fonte de Recurso", "Valor empenho(R$)", "Valor pago neste empenho(R$)", "Situação empenho","CNPJ","Entidade","Nº protocolo");
		$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%','S');
	}

}

function cabecalhoSolicitacaoEmpenho() {
	global $db;

	$arrDados = $db->pegaLinha("SELECT m.muncod,
									   CASE WHEN iu.itrid = 1 THEN e.estuf ELSE m.estuf END as estuf, 
									   m.mundescricao,
									   p.prpnumeroprocesso,
									   p.prptipo
								FROM par.processopar p
								INNER JOIN par.instrumentounidade iu on iu.inuid = p.inuid
							    LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
								LEFT JOIN territorios.estado e ON e.estuf = iu.estuf
							    WHERE p.prpid='".$_SESSION['par_var']['prpid']."'");

	echo "<table border=0 cellpadding=3 cellspacing=0 class=listagem width=95% align=center>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>UF:</td>";
	echo "<td>".$arrDados['estuf']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>Município:</td>";
	echo "<td>".$arrDados['mundescricao']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>Nº processo:</td>";
	echo "<td>".$arrDados['prpnumeroprocesso']."</td>";
	echo "</tr>";
	echo "</table>";
}

function cancelarPagamento($dados) {
	global $db;

	try {

		$data_created = date("c");

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
			$dadospag = $db->pegaLinha("SELECT pagid, parnumseqob, pagparcela, pagvalorparcela FROM par.pagamento WHERE pagid='".$dados['pagid']."'");

	        if($dadospag) {
	        	$nu_seq_ob = $dadospag['parnumseqob'];
	        }

		} else {
			$usuario = $dados['wsusuario'];
			$senha   = $dados['wssenha'];
			
		    $dadospag = $db->pegaLinha("SELECT pagid, parnumseqob, pagparcela, pagvalorparcela FROM par.pagamento WHERE pagid='".$dados['pagid']."'");

	        if($dadospag) {
	        	$nu_seq_ob = $dadospag['parnumseqob'];
	        }

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
        <nu_seq_ob>$nu_seq_ob</nu_seq_ob>
		</params>
	</body>
</request>
XML;

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/ob';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'cancelar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

	    echo "------ CANCELAMENTO DE PAGAMENTO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		$result = (integer) $xml->status->result;

		if($result) {	
			 $sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$_SESSION['par_var']['prpid']."',
					    		'consultarContaCorrente',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();


			$db->executar("UPDATE par.pagamento SET pagsituacaopagamento='CANCELADO', pagstatus='I'
					   	  WHERE pagid='".$dadospag['pagid']."'");

			$db->executar("INSERT INTO par.historicopagamento(
            			   pagid, hpgdata, usucpf, hpgparcela, hpgvalorparcela, hpgsituacaopagamento)
    					   VALUES ('".$dadospag['pagid']."', NOW(), '".$_SESSION['usucpf']."',
    					   		   '".$dadospag['pagparcela']."', '".$dadospag['pagvalorparcela']."', 'CANCELADA');");

			$db->commit();

		} else {

			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message;

			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}

			 $sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$_SESSION['par_var']['prpid']."',
					    		'consultarContaCorrente',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
			$db->executar($sql);
			$db->commit();


		   	return false;
		}


	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Cancelar Pagamento encontra-se temporariamente indisponível. Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Cancelar Pagamento no SIGEF: $erroMSG";


	}
}

function consultarPagamento($dados){
	global $db;
	
	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

	    $dadospag = $db->pegaLinha("SELECT pagid, parnumseqob, pagparcela, pagvalorparcela FROM par.pagamento WHERE pagid='".$dados['pagid']."'");

	    if($dadospag) {
	    	$nu_seq_ob = $dadospag['parnumseqob'];
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
        <nu_seq_ob>$nu_seq_ob</nu_seq_ob>
		</params>
	</body>
</request>
XML;

		/*if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/ob';
		} else {*/
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
		//}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;

	    echo "------ CONSULTA DE PAGAMENTO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		if(!$result) {

			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'consultarPagamento - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return false;


		} else {

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'consultarPagamento - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();
			
			echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
			echo "*** Detalhes da consulta ***\n\n";
			echo "* Situação : ".$xml->body->row->situacao_documento."\n";
			echo "* Data : ".$xml->body->row->data_documento."\n";
			echo "* Valor(R$) : ".number_format((int)$xml->body->row->valor_ob[0],2,",",".")."\n";
			echo "* Processo : ".$xml->body->row->processo."\n";
			echo "* Nº documento : ".((strlen($xml->body->row->numero_documento))?$xml->body->row->numero_documento:"-")."\n";
			echo "* CNPJ : ".((strlen($xml->body->row->nu_favorecido))?$xml->body->row->nu_favorecido:"-")."\n";
			echo "* Status : ".((strlen($xml->body->row->status))?$xml->body->row->status:"-")."\n";


			if( (string)$xml->body->row->data_documento ){
				$db->executar("UPDATE par.pagamento SET
							   pagsituacaopagamento='".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."'
							   ".(($xml->body->row->data_documento)?", pagdatapagamentosiafi='".formata_data_sql(iconv("UTF-8", "ISO-8859-1", $xml->body->row->data_documento))."'":"")."
							   WHERE pagid='".$dadospag['pagid']."'");
			} else {
				$db->executar("UPDATE par.pagamento SET
							   pagsituacaopagamento='".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."'
							   WHERE pagid='".$dadospag['pagid']."'");
			}

			$db->executar("INSERT INTO par.historicopagamento(
	           			   pagid, hpgdata, usucpf, hpgparcela, hpgvalorparcela, hpgsituacaopagamento)
	   					   VALUES ('".$dadospag['pagid']."', NOW(), '".$_SESSION['usucpf']."',
	   					   		   '".$dadospag['pagparcela']."', '".$dadospag['pagvalorparcela']."', '".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."');");

			$db->commit();

			return true;

		}


	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Consulta pagamento encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar Pagamento no SIGEF: $erroMSG";


	}
}

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['prpid']) {
	$_SESSION['par_var']['prpid'] = $_REQUEST['prpid']; 
}
if($_REQUEST['empnumeroprocesso']) {
	$_SESSION['par_var']['empnumeroprocesso'] = $_REQUEST['empnumeroprocesso']; 
}
$processo = $_REQUEST['processo'];

if(!$_SESSION['par_var']['prpid']) die("<script>
											alert('Processo não encontrado');
											window.close();
										</script>");

if(!$_SESSION['par_var']['inuid']) die("<script>
											alert('Processo não encontrado');
											window.close();
										</script>");

$retorno = verificaEmpenhoValorDivergente($processo, '', 'PAR');

if( !empty($retorno[0]['processo']) ){
	die("<script>
			alert('Para realizar o pagamento deste processo é necessário ajustar os dados de empenho.');
			window.close();
		</script>");
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$arrBloqueioObra = verificaBloqueioObras( $_SESSION['par_var']['inuid'] );

// Monta tarja vermelha com aviso de pendência de obras
montarAvisoCabecalho(null, null, null, $_REQUEST['inuid']);


?>
<html>
<head>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body>
<?php
monta_titulo( $titulo_modulo, '<p align=center><b>SELECIONE UM EMPENHO</b> para visualizar os dados do pagamento</p>' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<style type="">
	.ui-dialog-titlebar{
    	text-align: center;
    }
</style>
<script type="text/javascript" src="./js/par.js"></script>
    <link href="../library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet" media="screen">
    
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<link rel="stylesheet" href="../includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />

<script>

function carregarListaEmpenhoPagamento( empid ) {
	$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/solicitacaoPagamentoPar&acao=A",
			data: "requisicao=listaEmpenho&empid="+empid,
			async: false,
			success: function(msg){
				document.getElementById('listapagamento').innerHTML = msg;
			}
		}
	);
}

function verDadosPagamento(empid) {
	divCarregando();
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoPagamentoPar&acao=A",
		data: "requisicao=dadosPagamento&empid="+empid,
		async: false,
		success: function(msg){
		document.getElementById('dadospagamento').innerHTML = msg;
		divCarregado();
	}
	});
}

$(document).ready(function() {
	carregarListaEmpenhoPagamento();
});

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow
	
function displayMessage(url) {
	var today = new Date();
	messageObj.setSource(url+'&hash='+today);
	messageObj.setCssClassMessageBox(false);
	messageObj.setSize(690,400);
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
}

/*function verObrasPagamento(pagid)
{
	var today = new Date();
	displayMessage('par.php?modulo=principal/solicitacaoPagamento&acao=A&requisicao=obrasPagamento&pagid='+pagid+'&hash='+today);
}*/

function verObrasPagamento(pagid)
{		
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "requisicao=obrasPagamento&pagid="+pagid,
   		async: false,
   		success: function(msg){
   			jQuery('.ui-icon ui-icon-closethick').hide();
   			jQuery( '#dialog-aut' ).hide();
   			jQuery('#dialog-confirm').attr('title', 'Subações do Pagamento');
   			jQuery( '#dialog-confirm' ).hide();
   			jQuery( "#dialog-confirm" ).html(msg);
   			jQuery( "#dialog-confirm" ).dialog({
				resizable: true,
				height:400,
				width:800,
				modal: true,
				show: { effect: 'drop', direction: "up" },
				buttons: {
					"Fechar": function() {
						jQuery( this ).dialog( "close" );
					}					
				}
			});
   		}
	});
	jQuery('.ui-dialog-titlebar-close').hide();	
}

// Faz a requisição para popular o modal
function verMobiliarioObras(pagid)
{	
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "requisicao=mobiliarioEquipamentoObras&pagid="+pagid,
   		async: false,
   		success: function(msg){
   			jQuery('.ui-icon ui-icon-closethick').hide();
   			jQuery( '#dialog-aut' ).hide();
   			jQuery('#dialog-obras-mobiliario').attr('title', 'Vinculação de Mobiliário e equipamento nas obras');
   			jQuery( '#dialog-obras-mobiliario' ).hide();
   			jQuery( "#dialog-obras-mobiliario" ).html(msg);
   			jQuery( "#dialog-obras-mobiliario" ).dialog({
				resizable: true,
				height:600,
				width:800,
				modal: true,
				open: function(){
			            jQuery('.ui-widget-overlay').bind('click',function(){
			                jQuery('#dialog-obras-mobiliario').dialog('close');
			            })
			    },
				show: { effect: 'drop', direction: "up" },
				buttons: {
										
					"Fechar": function() {
						jQuery( this ).dialog( "close" );
					}					
				}
			});
   		}
	});
	jQuery('.ui-dialog-titlebar-close').hide();	
}

// Submete a vinculação do mobiliário a obra e trata seu envio
function vincularMobiliarioObra(pagid)
{	
	var selecionado = false;
	var arr_sovid = ''; 
	jQuery(".check_mob").each(function() {

		if(jQuery(this).is(':checked'))
		{
			selecionado = true;
			arr_sovid = arr_sovid + jQuery(this).val() + '|' ;
			
		}
	});
	// Caso tenha selecionado no mínimo uma obra, submete
	if(selecionado)
	{

		var pagid = jQuery("#pagid_atual_modal").val();
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=salvaMobiliarioEquipObras&arr_sovid="+arr_sovid+"&pagid="+pagid,
	   		async: false,
	   		success: function(msg){
	   			alert(msg);
	   		}
		});
	}
	else
	{
		alert('Selecione no mínimo 1 Obra');
	}
}


function marcarSubacao(obj) {
	
	if(obj.checked) {
		obj.parentNode.parentNode.parentNode.cells[9].childNodes[0].className='normal';
		obj.parentNode.parentNode.parentNode.cells[9].childNodes[0].disabled=false;
		obj.parentNode.parentNode.parentNode.cells[8].childNodes[0].className='normal';
		obj.parentNode.parentNode.parentNode.cells[8].childNodes[0].disabled=false;
	} else {
		obj.parentNode.parentNode.parentNode.cells[9].childNodes[0].className='disabled';
		obj.parentNode.parentNode.parentNode.cells[9].childNodes[0].disabled=true;
		obj.parentNode.parentNode.parentNode.cells[9].childNodes[0].value='';
		obj.parentNode.parentNode.parentNode.cells[8].childNodes[0].className='disabled';
		obj.parentNode.parentNode.parentNode.cells[8].childNodes[0].disabled=true;
		obj.parentNode.parentNode.parentNode.cells[8].childNodes[0].value='';
	}
}

function calcularValorPagamento(obj) {
	
	var tabela = obj.parentNode.parentNode.parentNode;

	var valorDigitado 			= replaceAll(replaceAll(obj.value, ".", ""),",", ".");
	var valorEmpenhoObra		= obj.parentNode.parentNode.cells[3].childNodes[0].value;
	var valorPago				= obj.parentNode.parentNode.cells[6].childNodes[3].value;
	var valorPagoNesteEmpenho	= obj.parentNode.parentNode.cells[5].childNodes[3].value;
	var totalObra				= obj.parentNode.parentNode.cells[4].childNodes[0].value;
	
	if(obj.id == 'valorpagamentoobra'){
		
		//Trata se valor pago maior que valor empenhado
		if( (parseFloat(valorDigitado)+parseFloat(valorPagoNesteEmpenho)) > parseFloat(valorEmpenhoObra) ){
			alert('"Valor pagamento (R$)" + "Valor pago nesse empenho(R$)" não pode ser maior que o "Valor Empenhado (R$)"');
			valorDigitado = parseFloat(valorEmpenhoObra)-parseFloat(valorPagoNesteEmpenho);
			if( valorDigitado < 0 ){
				valorDigitado = 0;
			}
			obj.value = mascaraglobal('[.###],##',valorDigitado.toFixed(2));
		}
		// FIM Trata se valor pago maior que valor empenhado
		
		// Trata se valor pago é maior que valor da Obra
		if( parseFloat(parseFloat(valorDigitado)+parseFloat(valorPago)) > parseFloat(totalObra) ) {
			alert('"Valor pagamento (R$)" + "Valor total pago (R$)" não pode ser maior que o "Valor da Subação (R$)"');
			if( (parseFloat(valorEmpenhoObra)-parseFloat(valorPagoNesteEmpenho)) > (parseFloat(totalObra)-parseFloat(valorPago)) ){
				valorDigitado = parseFloat(totalObra)-parseFloat(valorPago);
			}else{
				valorDigitado = parseFloat(valorEmpenhoObra)-parseFloat(valorPagoNesteEmpenho);
			}
			if( valorDigitado < 0 ){
				valorDigitado = 0;
			}
			obj.value = mascaraglobal('[.###],##',valorDigitado.toFixed(2));
		}
		// FIM Trata se valor pago é maior que valor da Obra
		
		var porcentPagamento = (parseFloat(valorDigitado) * 100) / parseFloat(totalObra);		
		obj.parentNode.parentNode.cells[8].childNodes[0].value = mascaraglobal('[#]',porcentPagamento.toFixed(0));
		
	} else {
		
		var porcentDigitado = parseFloat(replaceAll(obj.value, ",", "."));
		// Trata se Maior q 100%
		if( porcentDigitado > 100 ){
			porcentDigitado = 100;
			obj.value = 100;
		}
		// Fim Trata se Maior q 100%

		var valorPagamento = (porcentDigitado/100)*totalObra;

		//Trata se valor pago maoir que valor empenhado (%)
		if( parseFloat(parseFloat(valorPagamento)+parseFloat(valorPagoNesteEmpenho)) > parseFloat(valorEmpenhoObra) ){
			alert('"Valor pagamento (R$)" + "Valor pago nesse empenho(R$)" não pode ser maior que o "Valor Empenhado (R$)"');
			porcentDigitado = ((valorEmpenhoObra-valorPagoNesteEmpenho)/totalObra)*100;
			if( porcentDigitado < 0 ){
				porcentDigitado = 0;
			}
			obj.value = porcentDigitado.toFixed(0);
		}
		// FIM Trata se valor pago maoir que valor empenhado
		
		// Trata se valor pago é maior que valor da Obra (%)
		if( parseFloat(porcentDigitado) > parseFloat( 100 - (parseFloat(parseFloat(valorPago)/parseFloat(totalObra))*100) ) ) {
			alert('"Valor pagamento (R$)" + "Valor total pago (R$)" não pode ser maior que o "Valor da Subação (R$)"');
			if( (((valorEmpenhoObra-valorPagoNesteEmpenho)/totalObra)*100) > ((100-(parseFloat(parseFloat(valorPago)/parseFloat(totalObra))*100)) ) ){
				porcentDigitado = (100-(parseFloat(parseFloat(valorPago)/parseFloat(totalObra))*100) );
			}else{
				porcentDigitado = ((valorEmpenhoObra-valorPagoNesteEmpenho)/totalObra)*100;
			}
			if( porcentDigitado < 0 ){
				porcentDigitado = 0;
			}
			obj.value = porcentDigitado.toFixed(0);
		}
		// FIM Trata se valor pago é maior que valor da Obra (%)
		
		valorPagamento = (porcentDigitado/100)*totalObra;
		
		obj.parentNode.parentNode.cells[9].childNodes[0].value = mascaraglobal('[.###],##',valorPagamento.toFixed(2));
	
	}
	
	var totalPagamento=0;
	var falta;
	var valorempenho = document.getElementById('valempid').value; //quanto falta pra pagar.
	
	for(var i=0;i<(tabela.rows.length)-3;i++) {
		if(tabela.rows[i].cells[9].childNodes[0].value) {
			totalPagamento += parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[9].childNodes[0].value,".",""),",","."));
		}
	}

	if(totalPagamento > 0) {
		document.getElementById('solicitar').disabled=false;
		document.getElementById('valorpagamento').value = mascaraglobal('[.###],##',totalPagamento.toFixed(2));
		falta = valorempenho - totalPagamento;
		if( falta < 0 ){
			tabela.rows[(tabela.rows.length)-2].cells[1].innerHTML = '<span style="color:red;">Valor empenhado<br>ultrapassado em R$ '+mascaraglobal('[.###],##',falta.toFixed(2))+'</span>';
			document.getElementById('solicitar').disabled=true;
		} else {
			tabela.rows[(tabela.rows.length)-2].cells[1].innerHTML = mascaraglobal('[.###],##',falta.toFixed(2));
		}
	} else {
		document.getElementById('solicitar').disabled=true;
		document.getElementById('valorpagamento').value = '';
		tabela.rows[(tabela.rows.length)-2].cells[1].innerHTML = mascaraglobal('[.###],##',parseFloat(valorempenho).toFixed(2));
	}

}

function solPag() {

	if(document.getElementById('boTermoVenc').value == 'S'){
		alert('Não é possivel efetivar o pagamento. \nO termo desse processo encontra-se vencido.');
		return false;
	}else{
		var esdid = document.getElementById('esdid').value;
		var label = document.getElementById('label').value;
		var boSolicita = false;
		
		if( esdid == 313 ){
			if(confirm("Este "+label+" se encontra em Diagnóstico. Deseja continuar?")){
				//document.getElementById('div_auth').style.display='block';
				boSolicita = true;
			} else {
				boSolicita = false;
				return false;
			}
		} else if( esdid == 314 ){
			if(confirm("Este "+label+" se encontra em Elaboração. Deseja continuar?")){
				//document.getElementById('div_auth').style.display='block';
				boSolicita = true;
			} else {
				boSolicita = false;
				return false;
			}
		} else {
			boSolicita = true;
			//document.getElementById('div_auth').style.display='block';
		}
		if( jQuery('[name="empid"]:checked').length == 0 ){
			alert('Selecione o empenho que deseja pagar.');
			boSolicita = false;
			return false;
		}
		if( boSolicita == true ){
			jQuery('.ui-icon ui-icon-closethick').hide();
			jQuery( '#dialog-aut' ).hide();
			jQuery( '#dialog-confirm' ).hide();
			jQuery( '#dialog-aut' ).dialog({
					resizable: false,
					width: 450,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						'Ok': function() {
							jQuery( this ).dialog( 'close' );
							solicitarPagamento();
						},
						'Cancel': function() {
							jQuery( this ).dialog( 'close' );
						}
					}
				});
			jQuery('input[type="text"], input[type="button"], input[type="password"]').css('font-size', '14px');
		} 
	}
}

function visPag(){
	var dados = $('#formPagamento').serialize();

	var empid = dados.substring(dados.indexOf('='),dados.indexOf('&parcela'));
	empid = empid.replace('=','');

	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoPagamentoPar&acao=A",
		data: "requisicao=solicitarPagamento&tipo=visualiza&"+dados,
		async: false,
		success: function(msg){
			document.getElementById('visXML').style.display='block';
			document.getElementById('visXMLDadosPagamento').innerHTML=msg;
	}
	});
}

function solicitarPagamento() {
	var form = document.getElementById('formPagamento');
	
	var usuario = document.getElementById('wsusuario').value;
	var senha   = document.getElementById('wssenha').value;
	
	if(!usuario) {
		alert('Favor informar o usuário!');
		return false;
	}
	
	if(!senha) {
		alert('Favor informar a senha!');
		return false;
	}
	
	divCarregando();
	var dados = $('#formPagamento').serialize();

	var empid = dados.substring(dados.indexOf('='),dados.indexOf('&parcela'));
	empid = empid.replace('=','');

	document.getElementById('solicitar').disabled = true;

	jQuery.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoPagamentoPar&acao=A",
		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=executarPagamento&"+dados,
		async: false,
		success: function(msg){
			//document.getElementById('debug').innerHTML = msg;
			//alert(msg);
			jQuery('.ui-icon ui-icon-closethick').hide();
			jQuery( '#dialog-aut' ).hide();
			jQuery( '#dialog-confirm' ).hide();
			jQuery( "#dialog-confirm" ).html(msg);
			jQuery( "#dialog-confirm" ).dialog({
				resizable: false,
				height:300,
				width:500,
				modal: true,
				show: { effect: 'drop', direction: "up" },
				buttons: {
					"OK": function() {
						jQuery( this ).dialog( "close" );
						window.location.reload();								
					}
					
				}
			});
		}
	});
	
	if(empid) {
		carregarListaEmpenhoPagamento(empid);
	}
	carregarListaPagamentoEmpenho();
	
	//document.getElementById('div_auth').style.display='none';
	document.getElementById('solicitar').disabled = false;
	divCarregado();
	
}

function mostraPendencia(){
	$( "#dialog_pendencia" ).show();
	$( '#dialog_pendencia' ).dialog({
			resizable: false,
			width: 900,
			height: 600,
			modal: true,
			show: { effect: 'drop', direction: "up" },
			buttons: {
				'Fechar': function() {
					$( this ).dialog( 'close' );
				}
			}
	});
}


</script>
<?
cabecalhoSolicitacaoEmpenho();

$retornoHistorico = exibirHistoricoSigef($processo);
atualizaBasePagamentoSigef( $retornoHistorico, $processo, $_REQUEST['prpid'] );

include_once APPRAIZ."par/modulos/principal/pagamento/deparaCorrecaoPagamentoEmpenhoSubacao.inc";

$idInuIdRest = $_SESSION['par_var']['inuid'];
$today = date('Y-m-d');
if($idInuIdRest)
	{
	$sqlRestricoes = 
		"SELECT 
                    res.resdescricao,
			tpr.tprdescricao as tipo_restricao,
			CASE WHEN res.restemporestricao THEN
				to_char(resdatainicio, 'DD/MM/YYYY')
			ELSE
				'-'
			END
			as data_inicio,
			
			CASE WHEN res.restemporestricao THEN
				to_char(resdatafim, 'DD/MM/YYYY')
			ELSE
				'-'
			END
			as data_fim                        
                FROM par.restricaoentidade re
                INNER JOIN par.restricaofnde res ON re.resid = res.resid
                INNER JOIN par.tiporestricaofnde tpr ON res.tprid = tpr.tprid
                LEFT JOIN workflow.documento doc ON doc.docid = re.docid 
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE re.inuid = {$idInuIdRest} 
                AND res.resstatus = 'A'
                AND esd.esdid NOT IN (".ESDID_CONCLUIDO.")
                ";
	
	$arrRestricoes = $db->carregar($sqlRestricoes);
	
	$nProcesso 	= 	$_GET['processo'];
	$prpId 		=	$_GET['prpid'];
	
	$sqlTipoExecucao = "select prptipoexecucao from par.processopar prpid where prpnumeroprocesso = '{$nProcesso}' AND prpid = {$prpId}";
	$resultTipo = $db->carregar($sqlTipoExecucao);
	$tipo = $resultTipo[0]['prptipoexecucao'];
	//Caso o tipo da execução seja Termo:
	if($tipo == 'T')
	{
		// Verifica se existe algum contrato anexado
		$sqlContratosTermo = 
		"
			SELECT 
				tc.dopid
			FROM 
				par.processopar prp
			INNER JOIN par.vm_documentopar_ativos dp ON prp.prpid = dp.prpid
			INNER JOIN par.termocomposicao tc ON dp.dopid = tc.dopid
			INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = tc.sbdid
			INNER JOIN par.subacaoitenscomposicao ico ON sbd.sbaid = ico.sbaid AND sbd.sbdano = ico.icoano
			INNER JOIN par.subacaoitenscomposicaocontratos sic ON ico.icoid = sic.icoid
			WHERE 
				prpstatus = 'A'
				AND icostatus = 'A' 
				AND prpnumeroprocesso = '{$nProcesso}'
				AND prp.prpid = {$prpId}
		";
		
		$resultC = $db->carregar($sqlContratosTermo);
		
		if(( ! is_array($resultC) ) && ( count($resultC) > 0 ) )
		{
			$arrRestricoes[][resdescricao] = "Pendência impeditiva: Não existe nenhum contrato vinculado ao termo de compromisso";
		}
		
	}
	
	if( (count($arrRestricoes) && is_array($arrRestricoes)) || ($arrBloqueioObra['boBloqueia']) ) 
	{
?>
<table id="listaRestricoes" align="center" border="0" style="border-color: #CCCCCC; border-right: 1px solid #CCCCCC;border-style: solid;border-width: 1px;   font-size: xx-small;text-decoration: none; width: 95%;" cellpadding="3" cellspacing="1">
			<thead>
				<tr>
					<td  colspan="4"> Restrições do município </td>
				</tr>

			</thead>
			<tbody>
			
				<?php
				if( count($arrRestricoes) && is_array($arrRestricoes))
				{
					foreach($arrRestricoes as $kRes => $vRes )
					{
						echo "
							<tr style=\"color:red;\">
								<td style=\"width:40%;\" > {$vRes['resdescricao']}</td>
							</tr>
							";	
					}
				}	
					if( $arrBloqueioObra['boBloqueia'] ){
						
						echo "
							<tr style=\"color:red;\">
								<td style=\"width:40%;\" >";
						echo '<span style="color: red;">Existem pendências no Sistema '.$arrBloqueioObra['sistema'].'.</span><a href="#" onclick="mostraPendencia();";><span style="color: blue; cursor:pointer"> Clique aqui para verificar as pendências.</span></a>';
						echo " 	</td>
							 </tr>
								";	
						
					}
				?>
			</tbody>
			</table>
	<br>
<?php 
	}
}
	$boTermoVenc = 'N';
	if( pegaTermoprocesso( $_REQUEST['processo'] ) != '' ){
		$boTermoVenc = verificaTermoVencidoPAR( $_REQUEST['processo'] );
		$boTermoVenc = ($boTermoVenc ? 'S' : 'N');

		if( $boTermoVenc == 'S' ){
			$sql = "select
						d.dopdatafimvigencia,
						m.mdonome
					from par.documentopar d
						inner join par.modelosdocumentos m on m.mdoid = d.mdoid and m.mdostatus = 'A'
					where d.prpid = {$_REQUEST['prpid']} AND d.dopstatus = 'A'";
			$arrDocVenc =  $db->pegaLinha($sql);
?>
<table id="listaRestricoes" align="center" border="0" style="border-color: #CCCCCC; border-right: 1px solid #CCCCCC;border-style: solid;border-width: 1px;   font-size: xx-small;text-decoration: none; width: 95%;" cellpadding="3" cellspacing="1">
	<thead>
		<tr>
			<td style="color:red; font-size: 14px;" colspan="4">
				Caso necessite realizar o pagamento desse processo, não será possível, pois o termo encontra-se com data de vigência expirada.<br>
				Termo: <?php echo $arrDocVenc['mdonome']; ?><br>
				Data Fim de Vigência: <?php echo $arrDocVenc['dopdatafimvigencia']; ?>
			</td>
		</tr>
	</thead>
</table>
	<br>
<?php 
		}
	}else{
?>
<table id="listaRestricoes" align="center" border="0" style="border-color: #CCCCCC; border-right: 1px solid #CCCCCC;border-style: solid;border-width: 1px;   font-size: xx-small;text-decoration: none; width: 95%;" cellpadding="3" cellspacing="1">
	<thead>
		<tr>
			<td style="color:red;" colspan="4"><center>O processo não possui termo.</center></td>
		</tr>
	</thead>
</table>
	<br>
<?php 
	}
?>

<form id="formPagamento">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<input type="hidden" id="esdid" name="esdid" value="<?=$esdid['esdid'] ?>" />
	<input type="hidden" id="label" name="label" value="<?=$esdid['label'] ?>" />
	<input type="hidden" id="boTermoVenc" name="boTermoVenc" value="<?=$boTermoVenc ?>" />
	<input type="hidden" name="tiposistema" id="tiposistema" value="PAR" />

	<tr>
		<td id="listapagamento">Carregando...</td>
	</tr>
	<tr>
		<td id="dadospagamento"></td>
	</tr>
</table>

</form>

<div class="demo">
	<div id="dialog-confirm" title="Solicitação de Pagamento" style="overflow-x: auto; overflow-y: auto; width:100%; height:250px; font-size: 12px;"></div>
</div>

<div class="demo">
	<div id="dialog-obras-mobiliario" title="Solicitação de Pagamento" style="overflow-x: auto; overflow-y: auto; width:100%; height:250px; font-size: 12px;"></div>
</div>

<div id="dialog-aut" title="Autenticação" style="display:none;">
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td width="30%" style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Usuário:</td>
				<td>					
					<input type="text" title="Usuário" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
						onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$usuario ?>" size="23" id="wsusuario" name="wsusuario">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Senha:</td>
				<td>
					<input type="password" class="obrigatorio normal"  title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
						onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$senha ?>" size="23" id="wssenha" name="wssenha">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
					<input type="hidden" name="ws_pagid" id="ws_pagid" value="" />
					<input type="hidden" name="ws_processo" id="ws_processo" value="" />
				</td>
			</tr>
		</table>
	<div id="resposta" style="color: red; font-size: 0.9em"></div>
</div>
<div id="debug"></div>
<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth";
?>
<style>
	#listaRestricoes thead tr td
	{
		background-color: #F7F7F7;
		font-weight: bold;
		font-size: 12px;
		font-family: arial;
	}
	.popup_alerta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
	.popup_alerta2{
			width:90%;height:90%;position:absolute;z-index:0;top:50%;left:30%;margin-top:-250;margin-left:-250;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>

<div id="dialog_pendencia" title="Pendências <?=$arrBloqueioObra['sistema']; ?>" style="display: none" >
	<div style="padding:5px;text-align:justify; font-size: 8pt; color: red;">
	<?
	if(is_array($arrBloqueioObra) && count($arrBloqueioObra))
	{
		foreach ($arrBloqueioObra['pendencia'] as $obra => $pendencia) {
			echo ($obra + 1).' - '.$pendencia;
		}
	}	
	?>
	</div>
</div>
<?php 
function obrasPagamento($dados) {
	global $db;

	echo "<h3>Lista de Subação do pagamento</h3>";

	$sql = "SELECT s.sbadsc, par.recuperavalorvalidadossubacaoporano(s.sbaid, ps.pobano) as vrlsubacao, ps.pobpercentualpag, ps.pobvalorpagamento
			FROM par.pagamentosubacao ps
			INNER JOIN par.subacao s ON s.sbaid = ps.sbaid
			WHERE ps.pagid = ".$dados['pagid'];

	$cabecalho = array("Descrição da Subação","Total da Subação(R$)","% Pagamento","Pagamento da Subação(R$)");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2, true, false, false, true);

	// 	echo "<p align=center><input type=button value=Fechar onclick=closeMessage();></p>";
}

// Salva o relacionamento das subações de mobiliário com suas respectivas obras
function salvaMobiliarioEquipObras($dados)
{
	global $db;
	$arrOb = $dados['arr_sovid'];
	$arrSovid = explode("|", $arrOb);
	$key = count($arrSovid) - 1 ;
	unset($arrSovid[$key]);
	$pagid = $dados['pagid'];
	
	
	$arrSovid = (is_array($arrSovid)) ? $arrSovid : Array();
	
	// Busca todos possíveis registros para este pagamento
	$relacionamento1 = "
		SELECT 
			pov.povid, pov.pobid, pov.sovid
		FROM
			par.pagamentosubacaoobravinculada pov
		INNER JOIN par.pagamentosubacao pob ON pov.pobid = pob.pobid 
		INNER JOIN par.subacaoobravinculacao sov ON  pov.sovid = sov.sovid
		where
		pagid = {$pagid}
	";
	
	$arrRelacionamento1 = $db->carregar($relacionamento1);
	
	
	//Monta array para inserir os dados
	foreach($arrSovid as $k => $sovId)
	{
		//busca subação para pegar o pobid
		$sbaid = $db->PegaUm("select sbaid from par.subacaoobravinculacao where sovid = {$sovId}");
		
		if($sbaid)
		{
			$pobid = $db->PegaUm("select pobid from par.pagamentosubacao  where sbaid = {$sbaid} and pagid = {$pagid}");
			
			if( $pobid )
			{
				$arrSelecionados[] = array('pobid' => $pobid, 'sovid' => $sovId);
			}	 
		}
	}
	// Trata caso array seja vazio
	$arrRelacionamento1 = ($arrRelacionamento1) ? $arrRelacionamento1 : array();
	foreach($arrRelacionamento1 as $k => $v )
	{
		$deletePovid = true;
		// Caso tenha deletado
		foreach($arrSelecionados as $k1 => $v1 )
		{
			if(($v['pobid'] == $v1['pobid']) && ($v['sovid'] == $v1['sovid']) )
			{
				$deletePovid = false;
			}	
		}
		// Deleta caso tenha sido desmarcado o check anteriormente marcado
		if($deletePovid)
		{
			// Deleta registro
			$sqlDelete = "
				DELETE FROM par.pagamentosubacaoobravinculada WHERE povid = {$v['povid']} 
			";
			//Registra ação no histórico
			if($db->executar($sqlDelete))
			{
				$sqlInsertHst = "
				INSERT INTO par.historico_pagamentosubacaoobravinculada
					(pobid, sovid, usucpf, hpostatus, hpodata  )
				VALUES
					({$v['pobid']}, {$v['sovid']}, {$_SESSION['usucpf']}, 'I', 'now()' )";
				$db->executar($sqlInsertHst);
			}
		}
	}
	
	// Trata os novos dados
	$arrSelecionados = ($arrSelecionados) ? $arrSelecionados : array();
	// 
	foreach($arrSelecionados as $kI => $vI)
	{
		// Caso esteja tudo ok com as informações que serão inseridas
		if( $vI['sovid'] && $vI['pobid'] )
		{
			$pobid = $vI['pobid'];
			$sovId = $vI['sovid'];
			// Verifica se já existe o registro neste caso
			$povid = $db->PegaUm("select povid from  par.pagamentosubacaoobravinculada  where pobid = {$pobid} and sovid = {$sovId}");
			// Caso não exista ele insere
			if( ! $povid )
			{
				// Insere o registro
				$sqlInsert = "
					INSERT INTO par.pagamentosubacaoobravinculada
						(pobid, sovid )
					VALUES
						({$pobid}, {$sovId})";
				// Registra no histórico
				if($db->executar($sqlInsert))
				{
					$sqlInsertHst = "
					INSERT INTO par.historico_pagamentosubacaoobravinculada
						(pobid, sovid, usucpf, hpostatus, hpodata  )
					VALUES
						({$pobid}, {$sovId}, {$_SESSION['usucpf']}, 'A', 'now()' )";
					$db->executar($sqlInsertHst);	
				}
			}
		}
	}
	
	// Caso sucesso
	if($db->commit())
	{
		echo 'Vinculação salva com sucesso.';
	}
	else
	{
		echo 'Erro ao salvar vinculação.';
	}
	die();
		
	
	
	//
}

// Função que retorna conteúdo do modal
function mobiliarioEquipamentoObras($dados) 
{
	global $db;

	echo "<br>";
	
	$pagid = $dados['pagid'];
	// Caso tenha o pagid
	if( $pagid )
	{
		// guarda valor do pagid para passa na hora de salvar
		echo "<input type='hidden' value='{$pagid}' id='pagid_atual_modal'>";
		//Dados do pagamento
		$sql = "SELECT 
					pagnumeroob as ob, pagvalorparcela, pagsituacaopagamento, usu.usunome
				FROM 
					par.pagamento pg
				INNER JOIN seguranca.usuario usu ON usu.usucpf = pg.usucpf
				WHERE 
					pg.pagid = ".$pagid;
	
		// Carrega dados do pagamento
		$dadosPag = $db->pegaLinha($sql);
		
		// Formata o valor pago deste pagamento
		$valorPag = number_format($dadosPag['pagvalorparcela'],2,',','.');//number_format($dadosPag['pagvalorparcela'], 0, ',', '.');
		echo "<center><h4>Dados do Pagamento</h4></center>";
		// Monta tabela com os dados do pagamento
		echo "
	
				<table  class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
					<tr>	
						<td align='right' width=\"20%\" class=\"SubTituloDireita\">Ordem bancária:</td>
				        <td>
							{$dadosPag['ob']}
			        	</td>
					</tr>
					<tr>	
						<td align='right' class=\"SubTituloDireita\">Valor:</td>
				        <td>
							R$ {$valorPag}
			        	</td>
					</tr>
					<tr>	
						<td align='right' class=\"SubTituloDireita\">Situação:</td>
				        <td>
							{$dadosPag['pagsituacaopagamento']}
			        	</td>
					</tr>
					<tr>	
						<td align='right' class=\"SubTituloDireita\">Usuário:</td>
				        <td>
							{$dadosPag['usunome']}
			        	</td>
					</tr>
				</table>
			";
		// Busca as subações relacionadas a este pagamento
		$sqlSubacoes = "
			SELECT 
				distinct s.sbaid, par.retornacodigosubacao(s.sbaid) as localizacao, sbadsc 
			FROM 
				par.processopar p
			INNER JOIN par.processoparcomposicao ppc on ppc.prpid = p.prpid
			INNER JOIN par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid 
			INNER join  par.pagamentosubacao pg ON pg.sbaid = s.sbaid
			WHERE 
				s.ptsid in (69,51,8,39,74,56,9,40)
			and 
				pagid = {$pagid}
		";
		
		$subacoes = $db->carregar($sqlSubacoes);
		
		echo "<center><h4>Subações</h4></center>";
		// Carrega
		echo  "	<table  width='100%' class=\"listagem\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
						   		<thead>
									<tr>
										<td width=\"9%\">
											Localização:
										</td>
										<td>
											Subação:
										</td>
									</tr>
								</thead>
								<tbody>
							";
		// Cicla subações
		foreach($subacoes as $k => $v )
		{
			
			echo "
				<tr>
					<td>
						&nbsp;{$v['localizacao']}
					</td>
					<td>
						&nbsp; {$v['sbadsc']}
					</td>
				</tr>	
				<tr>
					<td>
						<img align=absmiddle src=\"../imagens/seta_filho.gif\">
					</td>
					<td>";
							// Insere as obras relacionadas aquela subação
							$sqlObrasSubacao = "
							SELECT 
								 '<center><input class=\"check_mob\" value=\"'|| sv.sovid ||'\"'
										||
										CASE WHEN EXISTS (
												SELECT povid FROM par.pagamentosubacaoobravinculada pov
												WHERE
													pov.pobid = ( select pobid from par.pagamentosubacao p where p.pagid = {$pagid} and p.sbaid = sv.sbaid)
												AND
													pov.sovid = sv.sovid
											)
										THEN
											'checked=checked'
										ELSE
											''
										END
										
										||' type=\"checkbox\"></center>' as acao,
															
								CASE WHEN p.preid IS NOT NULL THEN
									p.preid::text
								ELSE
									'<center>-</center>'
								END
									as preid, 
								CASE WHEN o.obrid IS NOT NULL THEN
									o.obrid::text
								ELSE
									'<center>-</center>'
								END as obrid, 
								CASE WHEN p.preid IS NOT NULL THEN
									p.predescricao
								ELSE
									o.obrnome 
								END as nome_obra,
							
								CASE WHEN p.preid IS NOT NULL THEN 
									pto.ptodescricao
								ELSE
									tobdesc
								END
								as tipo_obra
							FROM par.subacaoobravinculacao  sv
							INNER JOIN par.subacao s on s.sbaid = sv.sbaid
							LEFT JOIN obras.preobra p on p.preid = sv.preid
							left join obras.pretipoobra pto ON pto.ptoid = p.ptoid
							left JOIN obras2.obras o on o.obrid = sv.obrid 
							LEFT JOIN obras2.tipoobra  tob ON tob.tobid = o.tobid
							WHERE sv.sbaid =  {$v['sbaid']}
							";
							
						$cabecalho = array("Ação", "PREID", "OBRID", "Nome da Obra", "Tipo da Obra" );
						
						echo "<center>";
							echo "<b>Obras vinculadas</b><br>";
							$db->monta_lista($sqlObrasSubacao,$cabecalho,50000, 5,'N','90%','N');
						echo "</center>";
				
					echo "
					</td>
				</tr>
			";
			
		}
		echo "</table>";	
		// Botão para vinculação
		echo "<br><center><input type=button value=\"Vincular\" onclick=vincularMobiliarioObra();></center>";
		
	}	
	else
	{
		echo '<center>Erro ao carregar os dados</center>';
		die();
	}			
	die();

	// 	echo "<p align=center><input type=button value=Fechar onclick=closeMessage();></p>";
}
?>