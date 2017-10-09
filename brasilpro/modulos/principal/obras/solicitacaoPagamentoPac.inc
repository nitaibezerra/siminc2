<?php

if ($_REQUEST['titleFor']) {
	if( $_REQUEST['tp'] == 2 ){
		$sql = "SELECT 
					p.pagnumeroempenho as numero,
					p.pagvalorparcela as valor,
					p.pagsituacaopagamento as situacao
				FROM 
					cte.processobrasilpro pp 
				INNER JOIN cte.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso
				LEFT JOIN cte.pagamento p on p.empid = e.empid
				LEFT JOIN cte.empenhosubacao es ON es.empid = e.empid
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
					cte.processobrasilpro pp 
				INNER JOIN cte.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso
				LEFT JOIN cte.pagamento p on p.empid = e.empid
				LEFT JOIN cte.empenhosubacao es ON es.empid = e.empid
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
	
	$valor = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
	$totalpagamento = $db->pegaUm("SELECT SUM(pagvalorparcela) FROM cte.pagamento WHERE empid='".$dados['empid']."' AND pagstatus='A'");
	$totalempenho   = $db->pegaUm("SELECT empvalorempenho FROM cte.empenho WHERE empid='".$dados['empid']."'");
	$soma = ($totalpagamento+$valor);
	
	if( round($soma,2) > round($totalempenho,2) ) {
		die(SIGLA_SISTEMA. " INFORMA : Total de pagamento esta maior que o valor do empenho");
	}
	
	$res_cc = consultarContaCorrente($dados);

	if($res_cc != false){
		$res_se = solicitarPagamento($dados);
	}
	
	//echo "<script> window.location.href = window.location.href; </script>";
	//die();
	//if($res_cc=="cc_criado_sucesso") {
		//$res_cc = consultarContaCorrente($dados);	
	//}
	//$res_se = solicitarPagamento($dados);
	//if($res_cc) $res_se = solicitarPagamento($dados);
	
}

function consultarContaCorrente($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

        $proseqconta = $db->pegaUm("SELECT prpseqconta FROM cte.processobrasilpro WHERE prpid='".$_SESSION['par_var']['prpid']."'");
		
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
		    $sql = "INSERT INTO cte.historicowsprocessobrasilpro(
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
				    	$db->executar("UPDATE cte.processobrasilpro SET nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', seq_conta_corrente='".$xml->body->row->seq_conta."' WHERE prpseqconta='".$proseqconta."'");
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

	try {
		
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
									  prpbanco, prpagencia, nu_conta_corrente
							   FROM cte.empenho emp 
							   INNER JOIN cte.processobrasilpro pro ON pro.prpnumeroprocesso = emp.empnumeroprocesso 
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
		$vl_custeio = "0";
		$vl_capital = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
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
		$an_exercicio = $db->pegaUm("SELECT to_char(hepdata,'YYYY') as ano FROM cte.historicoempenho 
									 WHERE empid='".$dados['empid']."' ORDER BY hepdata ASC LIMIT 1");
 		
			
				
		$nu_mes = sprintf("%02d", $dados['mes']);				
		$valor = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
		$numeroconvenioSIAFI = $dadosse['prpnumeroconveniosiafi'];
		
		
    	if($dadosse['prptipoexecucao'] == 'C'){
	   		$co_programa_fnde="03";
	   		$an_referencia 	= date("Y"); //empanoconvenio //date("Y");
	   		$an_item 		= $dadosse['empanoconvenio'];
	   		
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
	        $co_programa_fnde="ED"; 
       
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
	        
	       //$nu_sistema="7";
	    }

	    if( $dados['tipo'] == 'visualiza' ){
			echo '<pre>';		
			echo simec_htmlentities($arqXml);
			exit;
	    }
//echo ver( $arqXml );
//die();
		   		
		//if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			//$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/ob';
		//} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
		//}
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();

		$xmlRetorno = $xml;
							
		$xml = simplexml_load_string( stripslashes($xml));
		
	    echo "------ SOLICITAÇÃO DE PAGAMENTO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
					
		$result = (integer) $xml->status->result;

		// simulando sem validacão do XML
		// $result = true;
		if(!$result) {
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message->text;	
			$erros = explode("[text]", $erros);
			$erros = explode("[status]", $erros[1]);
			if(is_array($erros)) {
				if($erros[0]){
			//if(count($erros)>0) {
				//foreach($erros as $err) {
					//dbg($err);
					echo "* ".iconv("UTF-8", "ISO-8859-1", $erros[0]);
				//}
			//}
				}
			}
			
			
			$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
				    	prpid, 
				    	hwpwebservice, 
				    	hwpxmlenvio, 
				    	hwpxmlretorno, 
				    	hwpdataenvio, 
				        usucpf
				        )
				    VALUES ('".$_SESSION['par_var']['prpid']."', 
				    		'solicitarPagamento - Erro', 
				    		'".addslashes($arqXml)."', 
				    		'".addslashes($xmlRetorno)."', 
				    		NOW(), 
				            '".$_SESSION['usucpf']."');";
			$db->executar($sql);
			$db->commit();
			
			
			
			
	
		} else {
			
			$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
				    	prpid, 
				    	hwpwebservice, 
				    	hwpxmlenvio, 
				    	hwpxmlretorno, 
				    	hwpdataenvio, 
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."', 
				    		'solicitarPagamento - Sucesso', 
				    		'".addslashes($arqXml)."', 
				    		'".addslashes($xmlRetorno)."', 
				    		NOW(), 
				            '".$_SESSION['usucpf']."');";
			$db->executar($sql);
			$db->commit();
						
						
			$sql = "INSERT INTO cte.pagamento(
								pagparcela,
								pagmes, 
								paganoparcela,
								pagvalorparcela,
								paganoexercicio,
								pagnumeroempenho, 
								empid, 
								usucpf, 
								pagdatapagamento,
								parnumseqob)
							VALUES (
								".$dados['pagparcela'].", 
								".$dados['mes'].", 
								".$dados['ano'].", 
								".$valor.", 
								".date('Y').", 
								'{$dadosse['empnumero']}', 
								".$dados['empid'].", 
								'".$_SESSION['usucpf']."', 
								NOW(),
								".(($xml->body->nu_registro_ob)?"'".$xml->body->nu_registro_ob."'":"NULL").")
							RETURNING
								pagid";
			$pagid = $db->pegaUm($sql);
			
			if($dados['sbaid']) {
				foreach($dados['sbaid'] as $chave=>$sbaid) {
					$sql = "INSERT INTO cte.pagamentosubacao(sbaid, pagid, pobpercentualpag, pobvalorpagamento, pobano)
    						VALUES ('".$sbaid."', '".$pagid."', '".str_replace(array(","),array("."),$dados['porcent'][$sbaid])."', '".str_replace(array(".",","),array("","."),$dados['valorpagamentoobra'][$sbaid])."','".$dados['anosubacao'][$chave]."');";
					$db->executar($sql);
				}
			}
		
			$sql = "INSERT INTO cte.historicopagamento(
								pagid, 
								hpgdata, 
								usucpf, 
								hpgparcela, 
								hpgvalorparcela, 
								hpgsituacaopagamento)
							VALUES (
								{$pagid}, 
								NOW(), 
								'".$_SESSION['usucpf']."', 
								".$dados['pagparcela'].", 
								".$valor.", 
								'Solicitado.')";
			$db->executar($sql);
			$db->commit();
		}
	}
		


	
	} catch (Exception $e){
		
				# Erro 404 página not found
				if($e->getCode() == 404){
					echo "Erro-Serviço Solicitar Pagamento encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
				}
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
		
				echo "Erro-WS Solicitar Pagamento no SIGEF: $erroMSG";
				echo "<pre>";
				print_r($e);
		
	}
}

function dadosPagamento($dados) {
	global $db;

	echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
	echo "<tr><td class=SubTituloCentro>Inserir nova parcela</td></tr>";
	echo "</table>";
	
	$sql = "
				SELECT 	
					dados.chk, 
					dados.sbaid,
					dados.sbadsc,
					dados.eobano, 
					dados.valorsubacao,
					dados.pagamentoempenho,
					'' as pagamentooutros,
					'<input type=text class=disabled onblur=\"MouseBlur(this);this.value=mascaraglobal(\'[###],##\',this.value);cacularValorPagamento(this);\" onmouseout=MouseOut(this); onfocus=MouseClick(this);this.select(); onmouseover=MouseOver(this); onkeyup=\"this.value=mascaraglobal(\'[###],##\',this.value);cacularValorPagamento(this);\" maxlength=6 size=7 id=porcent name=porcent['||dados.sbaid||'] style=text-align:; disabled>' as porcentpagamento, 
					'<input type=text class=disabled onblur=MouseBlur(this); onmouseout=MouseOut(this); onfocus=MouseClick(this);this.select(); onmouseover=MouseOver(this); onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value);cacularValorPagamento(this);\" maxlength=20 size=21 id=valorpagamentoobra name=valorpagamentoobra['||dados.sbaid||'] style=text-align:; disabled>' as valorpagamento,
					count( dados.pagid ) + 1 as parcelaatual 
				FROM (
					SELECT '<input type=checkbox name=sbaid[] value='||po.sbaid||' onclick=marcarSubacao(this);><input type=hidden name=anosubacao[] value='||eo.eobano||'>' as chk, 
						po.sbaid,
						po.sbadsc,
						p.pagid,
						p.pagvalorparcela as pagamentoempenho,	    			   
						CASE WHEN po.sbacronograma = 1 
						THEN ( SELECT sum(coalesce(sic.icoquantidade,0) * coalesce(sic.icovalor,0))::numeric as vlrsubacao
							FROM cte.subacaoitenscomposicao sic 
							WHERE sic.sbaid = eo.sbaid
							AND sic.icoano <= date_part('year', now())
							GROUP BY sic.sbaid )
						ELSE ( SELECT sum(coalesce(se.sesquantidade,0) * coalesce(sic.icovalor,0))::numeric as vlrsubacao
							FROM cte.subacaoitenscomposicao sic 
							INNER JOIN cte.subacaoescolas se ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
							WHERE sic.sbaid = po.sbaid AND sic.icoano <= date_part('year', now())
							GROUP BY sic.sbaid ) 
						END as valorsubacao,
						(select SUM(pobvalorpagamento) from cte.pagamentosubacao where sbaid= po.sbaid and pobano <= date_part('year', now())) as pagamento,
						eo.eobano
					FROM cte.empenhosubacao eo
					INNER JOIN cte.subacao po ON po.sbaid = eo.sbaid 
					LEFT JOIN cte.pagamento p ON p.empid = eo.empid AND p.pagstatus = 'A'
					WHERE eo.empid='".$dados['empid']."'
				) AS dados
				GROUP BY  chk, sbadsc, sbaid, eobano, pagamentoempenho, valorsubacao";
ver($sql);die;
	$cabecalho = array("&nbsp;","Descrição da Subação","Ano", "Valor(R$)", "Valor pago nesse empenho(R$)", "Valor pago em outros empenhos(R$)", "% Pagamento","Valor pagamento(R$)","Parcela");
	$arr = $db->carregar($sql);

	//trato o detalhamento do pagamento
	$xx = 0;
	$arRegistro = array();
	if( is_array($arr) ){
		foreach( $arr as $key => $value ){
	
			$sql = "SELECT 
						sum(p.pagvalorparcela) as valor
					FROM 
						cte.processobrasilpro pp 
					INNER JOIN cte.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso
					LEFT JOIN cte.pagamento p on p.empid = e.empid
					WHERE 
						p.pagstatus = 'A' AND
						e.empid != ".$dados['empid']." AND 
						pp.prpid = ".$_SESSION['par_var']['prpid'];
	
			$valorEmpenho = $db->pegaUm( $sql );
	
			foreach ($arr as $c) {
				foreach ($c as $key => $d) {
					if( $key == 'pagamentooutros' ){
						$arRegistro[$xx][$key] = $valorEmpenho;
					} else {
						$arRegistro[$xx][$key] = $value[$key];
					}
				}
				$xx++;
			}
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
	$total = 0;
	foreach( $arRegistro as $arr ){
		echo '<tr bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
		foreach( $arr as $k => $v ){
			if( $k != 'sbaid' ){
				if( $k == 'valorsubacao' ){
					echo '<td>'.number_format($v,2,',','.').'</td>';
					$total = $total + $v;
				} elseif( $k == 'pagamentoempenho' ) {
					echo "<td><a onmouseover=\"SuperTitleAjax('/brasilpro/brasilpro.php?modulo=principal/obras/solicitacaoPagamentoPac&acao=A&titleFor=".$dados['empid']."&sbaid=".$arr['sbaid']."&tp=1',this);\" onmouseout=\"SuperTitleOff(this);\" href=\"\" >".number_format($v,2,',','.')."</a></td>";
					//echo '<td>'.number_format($v,2,',','.').'</td>';
					$total2 = $total2 + $v;
				} elseif( $k == 'pagamentooutros' ) {
					echo "<td><a onmouseover=\"SuperTitleAjax('/brasilpro/brasilpro.php?modulo=principal/obras/solicitacaoPagamentoPac&acao=A&titleFor=".$dados['empid']."&sbaid=".$arr['sbaid']."&tp=2',this);\" onmouseout=\"SuperTitleOff(this);\" href=\"\" >".number_format($v,2,',','.')."</a></td>";
					$total3 = $total3 + $v;
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
	echo "<td>".number_format($total,2,',','.')."</td>";
	echo "<td>".number_format($total2,2,',','.')."</td>";
	echo "<td>".number_format($total3,2,',','.')."</td>";
	echo "<td></td>";
	echo "<td align=center>".campo_texto('valorpagamento','N','S','','20','20','[.###],##','','','','','id="valorpagamento" readonly=readonly')."</td>";
	echo "<td></td>";
	echo "</tr>";
	
	
	// Linha com restante a pagar
	
	$sql = "SELECT empvalorempenho FROM cte.empenho WHERE empid = ".$dados['empid'];
	$valor = $db->pegaUm( $sql );
	
	echo "<tr bgcolor=#DCDCDC>";
	echo "<td colspan=7 align='right'>";
	echo "<b>Restante a pagar(R$):</b>";
	echo "</td>";
	echo "<td>";
	echo number_format(($valor-$total2),2,',','.');
	echo "</td>";
	echo "<td>";
	echo "<input type='hidden' id='valempid' value='".($valor-$total2)."'>";
	echo "</td>";
	echo "</tr>";
	
	// parcelas
	
	$parcela = $db->pegaUm("SELECT COALESCE(MAX(p.pagparcela),0) as parcela FROM cte.pagamento p
				 			WHERE p.empid = ".$dados['empid']." AND p.pagstatus='A'");
	
	$sql_mes = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses";
	$sql_ano = "SELECT ano as codigo, ano as descricao FROM public.anos";
	
	echo "<tr bgcolor=#DCDCDC>";
	echo "<td align=center colspan=9><input type=hidden name=pagparcela value=".($parcela+1)." />";
			//<b>Parcela: ".($parcela+1)."</b><input type=hidden name=pagparcela value=".($parcela+1)." />&nbsp;&nbsp;&nbsp;
	echo "Mês: ".$db->monta_combo('mes', $sql_mes, 'S', 'Selecione', '', '', '', '', 'S', 'mes', true, date("m"))."&nbsp;&nbsp;&nbsp;
			Ano: ".$db->monta_combo('ano', $sql_ano, 'S', 'Selecione', '', '', '', '', 'S', 'ano', true, date("Y"))."&nbsp;&nbsp;&nbsp;
			<input type=button id=solicitar name=solicitar  value=Solicitar pagamento disabled=disabled onclick=solPag(); />";
	if( in_array( PAR_PERFIL_SUPER_USUARIO, pegaArrayPerfil($_SESSION['usucpf']) ) ){
		echo "<input type=button id=visualizar name=visualizar  value=Visualizar XML onclick=visPag(); />";
	}
	echo "</td></tr>";

	$sql = "SELECT 
				'<img align=absmiddle src=../imagens/mais.gif style=cursor:pointer; title=mais onclick=\"carregarHistoricoPagamentoPar('||p.pagid||',this);\">' as mais,
				'<center><img src=../imagens/refresh2.gif style=cursor:pointer; onclick=consultarPagamento('||p.pagid||','||e.empnumeroprocesso||'); > <img src=../imagens/excluir.gif style=cursor:pointer; onclick=cancelarPagamento('||p.pagid||','||e.empnumeroprocesso||');> <img src=../imagens/consultar.gif style=cursor:pointer; onclick=verObrasPagamento('||p.pagid||');></center>',
				p.pagparcela, 
				p.pagmes,
				p.paganoparcela, 
				p.pagvalorparcela,
				u.usunome,
				COALESCE(p.pagsituacaopagamento,'-'),
				p.paganoexercicio
			FROM 
				cte.pagamento p 
			INNER JOIN 
				cte.empenho e ON e.empid = p.empid 
			LEFT JOIN 
				seguranca.usuario u ON u.usucpf = p.usucpf
			WHERE
				p.empid = ".$dados['empid']." AND pagstatus='A'
			ORDER BY
				pagparcela";
	
	echo "<input type=hidden name=empid id=empid value=".$dados['empid'].">";
	
	echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
	echo "<tr><td class=SubTituloCentro>Dados de pagamentos</td></tr>";
	echo "</table>";
	$cabecalho = array("&nbsp;","&nbsp;","Parcela","Mês","Ano","Valor(R$)","Usuário criação","Situação pagamento","Exercício");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);

}

function listaEmpenho($dados) {
	global $db;
	$sql = "SELECT DISTINCT
				prpnumeroprocesso
			FROM 
				cte.processobrasilpro
			WHERE
				prpid = ".$_SESSION['par_var']['prpid'];
	$numprocesso = $db->pegaUm($sql);
	
	$empid = $dados['empid'] ? $dados['empid'] : 0; 
	
	$sql = "SELECT 
				'<input type=hidden id='|| empid ||' value=\"'|| empvalorempenho ||'\"/> <input type=radio class=teste name=empid value='|| empid ||' onclick=\"verDadosPagamento(this.value);\" />' as radio, 
				empnumero, 
				to_char(empvalorempenho,'999G999G999G999D99') as valor,
				empsituacao,
				e.empcnpj, 
				en.entnome, 
				e.empprotocolo 
			FROM 
				cte.empenho e
			LEFT JOIN 
				entidade.entidade en ON en.entnumcpfcnpj=e.empcnpj 
			LEFT JOIN 
				entidade.funcaoentidade fen ON fen.entid=en.entid
			WHERE
			 	fen.funid=1 AND
				e.empnumeroprocesso = '{$numprocesso}'";
	
	$cabecalho = array("&nbsp;","N° do Empenho","Valor empenho(R$)","Situação empenho","CNPJ","Entidade","Nº protocolo");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);

}

function cabecalhoSolicitacaoEmpenho() {
	global $db;

	$arrDados = $db->pegaLinha("SELECT m.muncod,
									   m.estuf,
									   m.mundescricao,
									   p.prpnumeroprocesso,
									   p.prptipo
								FROM cte.processobrasilpro p
							    INNER JOIN territorios.municipio m ON m.muncod = p.muncod
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
			$dadospag = $db->pegaLinha("SELECT * FROM cte.pagamento WHERE pagid='".$dados['pagid']."'");

	        if($dadospag) {
	        	$nu_seq_ob = $dadospag['parnumseqob'];
	        }

		} else {
			$usuario = $dados['wsusuario'];
			$senha   = $dados['wssenha'];
			
		    $dadospag = $db->pegaLinha("SELECT * FROM cte.pagamento WHERE pagid='".$dados['pagid']."'");

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
			 $sql = "INSERT INTO cte.historicowsprocessobrasilpro(
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


			$db->executar("UPDATE cte.pagamento SET pagsituacaopagamento='CANCELADO', pagstatus='I'
					   	  WHERE pagid='".$dadospag['pagid']."'");

			$db->executar("INSERT INTO cte.historicopagamento(
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

			 $sql = "INSERT INTO cte.historicowsprocessobrasilpro(
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

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

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
<script type="text/javascript" src="./js/brasilpro.js"></script>
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
			url: "brasilpro.php?modulo=principal/obras/solicitacaoPagamentoPac&acao=A",
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
		url: "brasilpro.php?modulo=principal/obras/solicitacaoPagamentoPac&acao=A",
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

function verObrasPagamento(pagid)
{
	var today = new Date();
	displayMessage('brasilpro.php?modulo=principal/obras/solicitacaoPagamentoPac&acao=A&requisicao=obrasPagamento&pagid='+pagid+'&hash='+today);
}

function marcarSubacao(obj) {

	if(obj.checked) {
		obj.parentNode.parentNode.cells[6].childNodes[0].className='normal';
		obj.parentNode.parentNode.cells[6].childNodes[0].disabled=false;
		obj.parentNode.parentNode.cells[7].childNodes[0].className='normal';
		obj.parentNode.parentNode.cells[7].childNodes[0].disabled=false;
		
	} else {
		obj.parentNode.parentNode.cells[6].childNodes[0].className='disabled';
		obj.parentNode.parentNode.cells[6].childNodes[0].disabled=true;
		obj.parentNode.parentNode.cells[6].childNodes[0].value='';
		obj.parentNode.parentNode.cells[7].childNodes[0].className='disabled';
		obj.parentNode.parentNode.cells[7].childNodes[0].disabled=true;
		obj.parentNode.parentNode.cells[7].childNodes[0].value='';
	}
}

function cacularValorPagamento(obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	if(obj.id == 'valorpagamentoobra'){
		var valorDigitado = replaceAll(replaceAll(obj.value, ".", ""),",", ".");
		var valorPago     = parseFloat(replaceAll(tabela.rows[(tabela.rows.length)-2].cells[0].childNodes[0].value,"<br>",""));
		var totalObra	  = replaceAll(replaceAll(replaceAll(obj.parentNode.parentNode.cells[3].innerHTML,".",""),",","."),"<br>","");

		if( isNaN(parseFloat(valorPago)) ){
			var total 		  = valorDigitado;
		} else {
			var total 		  = valorDigitado+valorPago;
		}

		if(Number(total) > Number(totalObra)) {
			alert('"Valor pagamento" + "Valor pago" não pode ser maior que o "Total"');
			obj.value='';
			return false;
		}
		
		var porcentPagamento = (valorDigitado*100)/totalObra;
		obj.parentNode.parentNode.cells[6].childNodes[0].value = mascaraglobal('[.###],##',porcentPagamento.toFixed(2));
		
	} else {
		var porcentDigitado = parseFloat(replaceAll(obj.value, ",", "."));
		var porcentPago     = parseFloat(replaceAll(tabela.rows[(tabela.rows.length)-2].cells[0].childNodes[0].value,"<br>",""));
		var totalObra		= parseFloat(replaceAll(replaceAll(replaceAll(obj.parentNode.parentNode.cells[3].innerHTML,".",""),",","."),"<br>",""));
	
		if(porcentDigitado > (100-porcentPago)) {
			alert('"Valor pagamento" + "Valor pago" não pode ser maior que o "Total"');
			obj.value='';
			return false;
		}
		
		var valorPagamento = (porcentDigitado/100)*totalObra;
		obj.parentNode.parentNode.cells[7].childNodes[0].value = mascaraglobal('[.###],##',valorPagamento.toFixed(2));
	
	}
	
	var totalPagamento=0;
	var falta;
	var valorempenho = tabela.rows[(tabela.rows.length)-2].cells[2].childNodes[0].value; //quanto falta pra pagar.
	
	for(var i=0;i<(tabela.rows.length)-3;i++) {
		if(tabela.rows[i].cells[7].childNodes[0].value) {
			totalPagamento += parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[7].childNodes[0].value,".",""),",","."));
		}
	}

	if(totalPagamento > 0) {
		document.getElementById('solicitar').disabled=false;
		document.getElementById('valorpagamento').value = mascaraglobal('[.###],##',totalPagamento.toFixed(2));
		falta = valorempenho - ( replaceAll(replaceAll(document.getElementById('valorpagamento').value,".",""),",",".") );
		if( mascaraglobal('[###].##',parseFloat(valorempenho).toFixed(2)) < ( replaceAll(replaceAll(document.getElementById('valorpagamento').value,".",""),",",".") ) ){
			tabela.rows[(tabela.rows.length)-2].cells[1].innerHTML = '<span style="color:red;">valor empenhado<br>ultrapassado</span>';
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
	document.getElementById('div_auth').style.display='block';
}

function visPag(){
	var dados = $('#formPagamento').serialize();

	var empid = dados.substring(dados.indexOf('='),dados.indexOf('&parcela'));
	empid = empid.replace('=','');

	$.ajax({
		type: "POST",
		url: "brasilpro.php?modulo=principal/obras/solicitacaoPagamentoPac&acao=A",
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

	$.ajax({
		type: "POST",
		url: "brasilpro.php?modulo=principal/obras/solicitacaoPagamentoPac&acao=A",
		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=executarPagamento&"+dados,
		async: false,
		success: function(msg){
		alert(msg);
	}
	});
	
	if(empid) {
		carregarListaEmpenhoPagamento(empid);
	}
	carregarListaPagamentoEmpenho();
	
	document.getElementById('div_auth').style.display='none';
	document.getElementById('solicitar').disabled = false;
	divCarregado();
	
	
}


</script>
<? cabecalhoSolicitacaoEmpenho(); ?>
<form id="formPagamento">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td id="listapagamento">Carregando...</td>
	</tr>
	<tr>
		<td id="dadospagamento"></td>
	</tr>
</table>
</form>
<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth";
?>
<style>
	.popup_alerta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
	.popup_alerta2{
			width:90%;height:90%;position:absolute;z-index:0;top:50%;left:30%;margin-top:-250;margin-left:-250;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php echo campo_texto("wsusuario","S","S","Usuário","22","","","","","","","id='wsusuario'") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="23" id="wssenha" name="wssenha">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#D5D5D5" colspan="2">
				<input type="button" 						  name="btn_enviar"	  onclick="solicitarPagamento()" value="ok" />
				<input type="button" id="btn_cancelar_auth" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
			</td>
		</tr>
		</table>
	</div>
</div>
<div id="visXML" class="popup_alerta2 <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('visXML').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="10%" class="SubtituloDireita">Dados do Pagamento:</td>
			<td id="visXMLDadosPagamento"></td>
		</tr>
		</table>
	</div>
</div>
<?php

$largura = "250px";
$altura = "120px";
$id = "div_auth_consulta";
?>
<style>
	.popup_alerta_consulta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta_consulta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td width="30%" class="SubtituloDireita">Usuário:</td>
				<td><?php echo campo_texto("ws_usuario_consulta","S","S","Usuário","22","","","","","","","id='ws_usuario_consulta'") ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Senha:</td>
				<td>
					<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="23" id="ws_senha_consulta" name="ws_senha_consulta">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td align="center" bgcolor="#D5D5D5" colspan="2">
					<input type="hidden" name="ws_pagid" id="ws_pagid" value="" />
					<input type="hidden" name="ws_processo" id="ws_processo" value="" />
					<input type="button" name="btn_enviar" onclick="consultarPagamentoWS()" value="ok" />
					<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
				</td>
			</tr>
		</table>
	</div>
</div>
<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth_cancela";
?>
<style>
	.popup_alerta_consulta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta_consulta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td width="30%" class="SubtituloDireita">Usuário:</td>
				<td><?php echo campo_texto("ws_usuario_cancela","S","S","Usuário","22","","","","","","","id='ws_usuario_cancela'") ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Senha:</td>
				<td>
					<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="23" id="ws_senha_cancela" name="ws_senha_cancela">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td align="center" bgcolor="#D5D5D5" colspan="2">
					<input type="hidden" name="ws_pagid" id="ws_pagid" value="" />
					<input type="hidden" name="ws_processo" id="ws_processo" value="" />
					<input type="button" name="btn_enviar" onclick="cancelarPagamentoWSPAR()" value="ok" />
					<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
				</td>
			</tr>
		</table>
	</div>
</div>

<?php 
function listaPagamentoEmpenhoPar($dados) {
	global $db;
	
	if($dados['empnumeroprocesso']) {
		$arrProcesso = $db->pegaLinha("select prpid, CASE WHEN iu.itrid = 2 THEN 'municipal' ELSE 'estadual' END as esfera from cte.processobrasilpro pp INNER JOIN cte.instrumentounidade iu ON pp.inuid=iu.inuid WHERE prpnumeroprocesso = '".$dados['empnumeroprocesso']."'");
		$_SESSION['par_var']['prpid'] = $arrProcesso['prpid'];
		$_SESSION['par_var']['esfera'] = $arrProcesso['esfera'];
	}

	if(!$_SESSION['par_var']['prpid']) {
		die("<p align=center><b>Número do processo não encontrado. Por favor feche a janela e reinicie o procedimento.</b></p>");
	}

	$where[] = "p.prpid='".$_SESSION['par_var']['prpid']."'";
	if($_SESSION['par_var']['esfera']=='estadual'){
		$where[] = "funid = 6";
	}else{
		$where[] = "funid = 1";
	}

	$sql = "SELECT
					'<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarPagamento(\''||e.empid||'\', this);\">' as mais,
					e.empcnpj,
					en.entnome,
					e.empprotocolo,
					e.empvalorempenho,
					u.usunome,
					e.empsituacao
				FROM cte.empenho e
				INNER JOIN cte.processobrasilpro p ON trim(e.empnumeroprocesso) = trim(p.prpnumeroprocesso)
				LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf
				LEFT JOIN entidade.entidade en ON en.entnumcpfcnpj=e.empcnpj
				LEFT JOIN entidade.funcaoentidade fun ON fun.entid=en.entid
				".(($where)?"WHERE ".implode(" AND ", $where):"");

	$cabecalho = array("&nbsp;","CNPJ","Entidade","Nº protocolo","Valor empenho(R$)","Usuário criação","Situação empenho");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
}

function listaPagamento($dados) {
	global $db;

	$where[] = "empnumeroprocesso='".$dados['empnumeroprocesso']."'";

	$sql = "SELECT 
				'<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarHistoricoPagamento(\''||p.pagid||'\', this);\">' as mais,
				'<img src=../imagens/refresh2.gif style=cursor:pointer; onclick=consultarPagamento('||p.pagid||','||e.empnumeroprocesso||');>' as atualizar,
				'<img src=../imagens/excluir.gif style=cursor:pointer; onclick=cancelarPagamento('||p.pagid||','||e.empnumeroprocesso||');>' as cancelar,
				pagparcela || '°' as parcela, 	
				pagmes,
				paganoparcela, 
				'R$ ' || to_char(pagvalorparcela,'999G999G999G999D99') as vlr,  
				u.usunome,
				paganoexercicio,
				COALESCE(pagsituacaopagamento,'-')
			FROM 
				cte.pagamento p
			LEFT JOIN seguranca.usuario u ON u.usucpf = p.usucpf 
			LEFT JOIN cte.empenho e ON e.empid = p.empid
			WHERE
				p.empid = ".$dados['empid'];


	$cabecalho = array("&nbsp;","&nbsp;","&nbsp;","Parcela","Mês da Parcela","Ano da Parcela","Valor da Parcela","Usuário criação","Exercício","Situação");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','90%',$par2);
}

function listaHistoricoPagamento($dados) {
	global $db;
	$sql = "SELECT
				u.usunome,
				to_char(hpgdata, 'dd/mm/YYYY HH24:MI') as data,
				hpgsituacaopagamento,
				hpgparcela,
				hpgvalorparcela
			FROM
				cte.historicopagamento h
			LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf
			WHERE
				h.pagid = ".$dados['pagid']."
			ORDER BY
				hpgdata DESC";
	$cabecalho = array("Usuário atualização","Data","Situação","Parcela","Valor parcela");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','80%',$par2);

}
?>