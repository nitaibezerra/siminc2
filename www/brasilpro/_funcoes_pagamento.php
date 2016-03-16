<?php

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
				h.pagid = ".$dados['pagid'];
	$cabecalho = array("Usuário atualização","Data","Situação","Parcela","Valor parcela");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','80%',$par2);

}


function cabecalhoSolicitacaoEmpenho() {
	global $db;

	$arrDados = $db->pegaLinha("SELECT m.muncod,
									   m.estuf,
									   m.mundescricao,
									   p.pronumeroprocesso,
									   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
									   p.protipo
								FROM cte.processoobra p
							    INNER JOIN territorios.municipio m ON m.muncod = p.muncod
							    WHERE p.proid='".$_SESSION['par_var']['proid']."'");

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
	echo "<td>".$arrDados['pronumeroprocesso']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>Tipo obra:</td>";
	echo "<td>".$arrDados['tipoobra']."</td>";
	echo "</tr>";
	echo "</table>";
}

function obrasPagamento($dados) {
	global $db;
	
	echo "<h3>Lista de obras do pagamento</h3>";
	
	$sql = "SELECT pe.predescricao, pe.prevalorobra, po.pobpercentualpag, po.pobvalorpagamento FROM cte.pagamentoobra po 
			INNER JOIN obras.preobra pe ON pe.preid = po.preid 
			WHERE po.pagid='".$dados['pagid']."'";
	
	$cabecalho = array("Descrição da obra","Total da obra(R$)","% Pagamento","Pagamento da obra(R$)");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
	
	echo "<p align=center><input type=button value=Fechar onclick=closeMessage();></p>";
}

function listaPagamentoEmpenho($dados) {
	global $db;
	
	if(!$_SESSION['par_var']['proid']) {
		die("<p align=center><b>Número do processo não encontrado. Por favor feche a janela e reinicie o procedimento.</b></p>");	
	}
	
	$where[] = "p.proid='".$_SESSION['par_var']['proid']."'";
	$where[] = "funid = 1";

	$sql = "SELECT 
				'<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarPagamento(\''||e.empid||'\', this);\">' as mais,
				e.empcnpj, 
				en.entnome, 
				e.empprotocolo, 
				e.empvalorempenho, 
				u.usunome, 
				e.empsituacao 
			FROM cte.empenho e
			INNER JOIN cte.processoobra p ON trim(e.empnumeroprocesso) = trim(p.pronumeroprocesso)
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


function listaEmpenho($dados) {
	
	global $db;
	$sql = "SELECT DISTINCT
				pronumeroprocesso
			FROM 
				cte.processoobra
			WHERE
				proid = ".$_SESSION['par_var']['proid'];
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

function dadosPagamento($dados) {
	global $db;
	$sql = "SELECT 
				'<img align=absmiddle src=../imagens/mais.gif style=cursor:pointer; title=mais onclick=\"carregarHistoricoPagamento('||p.pagid||',this);\">' as mais,
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
	
	echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
	echo "<tr><td class=SubTituloCentro>Inserir nova parcela</td></tr>";
	echo "</table>";
	
	$sql = "SELECT '<input type=checkbox name=preid[] value='||po.preid||' onclick=marcarPreObra(this);>' as chk, 
				   po.predescricao, 
				   po.prevalorobra, 
				   (select SUM(pobvalorpagamento) from cte.pagamentoobra where preid=po.preid), 
				   ROUND((select SUM(pobvalorpagamento) from cte.pagamentoobra where preid=po.preid)/po.prevalorobra*100,2), 
				   '<input type=text class=disabled onblur=MouseBlur(this); onmouseout=MouseOut(this); onfocus=MouseClick(this);this.select(); onmouseover=MouseOver(this); onkeyup=\"this.value=mascaraglobal(\'##,##\',this.value);cacularValorPagamento(this);\" maxlength=5 size=7 name=porcent['||po.preid||'] style=text-align:; disabled>' as porcentpagamento, 
				   '<input type=text class=normal onblur=MouseBlur(this); onmouseout=MouseOut(this); onfocus=MouseClick(this);this.select(); onmouseover=MouseOver(this); onkeyup=this.value=mascaraglobal(\'[.###],##\',this.value); maxlength=20 size=21 name=valorpagamentoobra['||po.preid||'] style=text-align:; readonly=readonly>' as valorpagamento FROM cte.empenhoobra eo
			INNER JOIN obras.preobra po ON po.preid = eo.preid
			WHERE eo.empid='".$dados['empid']."'";

	$cabecalho = array("&nbsp;","Descrição da obra","Total obra(R$)","Valor pago(R$)","% Pago","% Pagamento","Valor pagamento(R$)");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
	
	$parcela = $db->pegaUm("SELECT COALESCE(MAX(p.pagparcela),0) as parcela FROM cte.pagamento p
				 			WHERE p.empid = ".$dados['empid']." AND p.pagstatus='A'");
	
	echo "<table align=center border=0 class=listagem style=width:100%; cellpadding=3 cellspacing=1>";
	echo "<thead>";
	echo "<tr>";
	echo "<td align=center style=width:10%>Parcela</td>";
	echo "<td align=center style=width:15%>Mês</td>";
	echo "<td align=center style=width:15%>Ano</td>";
	echo "<td align=center style=width:44%>Valor(R$)</td>";
	echo "<td align=center>&nbsp;</td>";
	echo "</tr>";
	echo "</thead>";
	echo "<tr>";
	echo "<td align=center style=width:10%><b>".($parcela+1)."</b><input type=hidden name=pagparcela value=".($parcela+1)." /></td>";
	
	$sql_mes = "SELECT mescod as codigo, mesdsc as descricao FROM public.meses";
	$sql_ano = "SELECT ano as codigo, ano as descricao FROM public.anos";
	
	echo "<td align=center>".$db->monta_combo('mes', $sql_mes, 'S', 'Selecione', '', '', '', '', 'S', 'mes', true, date("m"))."</td>";
	echo "<td align=center>".$db->monta_combo('ano', $sql_ano, 'S', 'Selecione', '', '', '', '', 'S', 'ano', true, date("Y"))."</td>";
	echo "<td align=center>".campo_texto('valorpagamento','S','S','','20','20','[.###],##','','','','','id="valorpagamento" readonly=readonly')."</td>";
	echo "<td align=center><input type=button id=solicitar name=solicitar  value=Solicitar pagamento disabled=disabled onclick=solPag(); /></td>";
	echo "</tr>";
	echo "</table>";

}

function executarPagamento($dados) {
	global $db;
	
	$valor = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
	$totalpagamento = $db->pegaUm("SELECT SUM(pagvalorparcela) FROM cte.pagamento WHERE empid='".$dados['empid']."' AND pagstatus='A'");
	$totalempenho   = $db->pegaUm("SELECT empvalorempenho FROM cte.empenho WHERE empid='".$dados['empid']."'");
	if(($totalpagamento+$valor) > $totalempenho) {
		die("SIMEC INFORMA : Total de pagamento esta maior que o valor do empenho");
	}
	
	if($dados['preid']) {
		foreach($dados['preid'] as $preid) {
			
			$sql = "SELECT DISTINCT                                       
		                    oi.obrid as id,
		                    oi.preid as idpreobra,
		                    oi.preid||' - '||oi.obrdesc as descricao,
		                    case when (va.vldstatushomologacao = 'N' or va.vldstatushomologacao is null) then 'nao' else 'sim' end as homologacao,
		                    case when (va.vldstatus25exec = 'N' or va.vldstatus25exec is null) then 'nao' else 'sim' end as execucao25,
		                    case when (va.vldstatus50exec = 'N' or va.vldstatus50exec is null) then 'nao' else 'sim' end as execucao50,
		                    case when oi.obrpercexec is null then '0.00 %' else oi.obrpercexec||' %' end as percexec
		                FROM
		                    obras.obrainfraestrutura oi
		                    left join obras.arquivosobra ao on ao.obrid = oi.obrid
		                        and ao.tpaid = 24
		                        and ao.aqostatus = 'A'
		                    left join public.arquivo ar on ar.arqid = ao.arqid
		                        and ar.arqtipo <> 'image/jpeg'
		                        and ar.arqtipo <> 'image/png'
		                        and ar.arqtipo <> 'image/gif'
		                    left join obras.validacao va on va.obrid = oi.obrid
		                WHERE               
		                    oi.orgid = 3
		                    and oi.obsstatus = 'A'
		                    and oi.preid = ".$preid;
			
			$dadospre = $db->pegaLinha($sql);
			
			switch($dados['pagparcela']) {
				case "2":
					if($dadospre['homologacao']=="nao") {
						die("SIMEC INFORMA : ".$dadospre['descricao']." não foi homologada");
					}
					break;
				case "3":
					if($dadospre['execucao25']=="nao") {
						die("SIMEC INFORMA : ".$dadospre['descricao']." não foi executada 25%");
					}
					break;
				case "4":
					if($dadospre['execucao50']=="nao") {
						die("SIMEC INFORMA : ".$dadospre['descricao']." não foi executada 50%");
					}
					break;
			}
			
		}
	}

	
	//$sql = "$dados['pagparcela']";
	
	$res_cc = consultarContaCorrente($dados);
	if($res_cc=="cc_criado_sucesso") {
		$res_cc = consultarContaCorrente($dados);
	}
	if($res_cc) $res_se = solicitarPagamento($dados);

}


function solicitarContaCorrente($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

        $dadoscc = $db->pegaLinha("SELECT pronumeroprocesso, probanco, proagencia, muncod, protipo FROM cte.processoobra WHERE proid='".$_SESSION['par_var']['proid']."'");

        if($dadoscc) {
	        // numero do processo (No desenvolvimento é fixo)
        	if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
        	   $_SESSION['baselogin'] == "simec_espelho_producao" ){
        	   	//$nu_processo='23034655466200900';
        	   	$nu_processo=$dadoscc['pronumeroprocesso'];//234000005642011
        	} else {
	        	$nu_processo=$dadoscc['pronumeroprocesso'];
        	}

	        // constante=001
	        $nu_banco=$dadoscc['probanco'];
	        // esperando envio
	        $nu_agencia=$dadoscc['proagencia'];
        }



		// CNPJ da prefeitura
		$nu_identificador=$db->pegaUm("SELECT ent.entnumcpfcnpj
					 				   FROM entidade.entidade ent
					 				   INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
					 				   INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 				   WHERE fen.funid=1 AND ende.muncod='".$dadoscc['muncod']."'");
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
        // constante=2
        $nu_sistema="2";
        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
        if($dadoscc['protipo'] == 'P') $co_programa_fnde="BW";
        else $co_programa_fnde="CN";



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

		if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
		   $_SESSION['baselogin'] == "simec_espelho_producao" ){
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

	    echo "------ SOLICITAÇÃO DE CONTA CORRENTE ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		$result = (integer) $xml->status->result;
		if(!$result) {
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}

			$sql = "INSERT INTO cte.historicowsprocessoobra(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."',
				    		'solicitarContaCorrente - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		    return false;
		} else {
			
		    $db->executar("UPDATE cte.processoobra SET proseqconta='".$xml->body->seq_solic_cr."', seq_conta_corrente='".$xml->body->nu_seq_conta."' WHERE proid='".$_SESSION['par_var']['proid']."'");

			$sql = "INSERT INTO cte.historicowsprocessoobra(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."',
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
			echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";

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

        $proseqconta = $db->pegaUm("SELECT proseqconta FROM cte.processoobra WHERE proid='".$_SESSION['par_var']['proid']."'");
        
        if(!$proseqconta) {
        	$r = solicitarContaCorrente($dados);
        	if($r) return "cc_criado_sucesso";
        	else return false;
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
					
			$xmlRetorno = $xml;

		    $xml = simplexml_load_string( stripslashes($xml));
		    
		    // verificando se a conta esta ativa e ok
		    if($xml->body->row->co_situacao_conta != "13") {
				echo "------ SIMEC INFORMA ------\n\n";
				echo date("d/m/Y h:i:s").": A conta corrente foi criada recentemente, o procedimento de ativação da conta não é efetuado no mesmo momento, isso impossibilita o pagamento. Tente novamente mais tarde.";
				
				$sql = "INSERT INTO cte.historicowsprocessoobra(
					    	proid, 
					    	hwpwebservice, 
					    	hwpxmlenvio, 
					    	hwpxmlretorno, 
					    	hwpdataenvio, 
					        usucpf)
					    VALUES ('".$_SESSION['par_var']['proid']."', 
					    		'consultarContaCorrente - Erro 13', 
					    		'".addslashes($arqXml)."', 
					    		'".addslashes($xmlRetorno)."', 
					    		NOW(), 
					            '".$_SESSION['usucpf']."');";
				
				$db->executar($sql);
				$db->commit();
				return false;
		    }
		    
		    if($xml->body->row->seq_conta) {
		    	$db->executar("UPDATE cte.processoobra SET nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', seq_conta_corrente='".$xml->body->row->seq_conta."' WHERE proseqconta='".$proseqconta."'");
		    }

			echo "------ CONSULTA DE CONTA CORRENTE ------\n\n";
			echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
			echo "*** Detalhes da consulta ***\n\n";
			echo "* Data movimento : ".$xml->body->row->dt_movimento."\n";
			echo "* Fase solicitação : ".iconv("UTF-8", "ISO-8859-1", $xml->body->row->fase_solicitacao)."\n";
			echo "* Entidade : ".iconv("UTF-8", "ISO-8859-1", $xml->body->row->ds_razao_social)."(".$xml->body->row->nu_identificador.")\n\n";
			
			$sql = "INSERT INTO cte.historicowsprocessoobra(
				    	proid, 
				    	hwpwebservice, 
				    	hwpxmlenvio, 
				    	hwpxmlretorno, 
				    	hwpdataenvio, 
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."', 
				    		'consultarContaCorrente', 
				    		'".addslashes($arqXml)."', 
				    		'".addslashes($xmlRetorno)."', 
				    		NOW(), 
				            '".$_SESSION['usucpf']."');";
			
			$db->executar($sql);
			$db->commit();

		    $result = (integer) $xml->status->result;

		    if(!$result) {
		    	return false;
		    } else {
		    	return true;
		    }

		} else {
			return true;
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




function consultarEmpenho($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

	    $dadosemp = $db->pegaLinha("SELECT * FROM cte.empenho WHERE empid='".$dados['empid']."'");

        if($dadosemp) {
        	$nu_seq_ne = $dadosemp['empprotocolo'];
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
        <nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
				->execute();

	    $xml = simplexml_load_string( stripslashes($xml));

	    echo "------ CONSULTA DE EMPENHO ------\n\n";
		echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
		echo "*** Detalhes da consulta ***\n\n";
		echo "* Nº processo: ".$xml->body->row->processo."\n";
		echo "* CNPJ: ".$xml->body->row->nu_cnpj."\n";
		echo "* Valor(R$): ".number_format($xml->body->row->valor_ne,2,",",".")."\n";
		echo "* Data: ".$xml->body->row->data_documento."\n";
		echo "* Nº documento: ".((strlen($xml->body->row->numero_documento))?$xml->body->row->numero_documento:"-")."\n";
		echo "* Valor empenhado(R$): ".((strlen($xml->body->row->valor_total_empenhado))?$xml->body->row->valor_total_empenhado:"-")."\n";
		echo "* Saldo pagamento(R$): ".((strlen($xml->body->row->valor_saldo_pagamento))?$xml->body->row->valor_saldo_pagamento:"-")."\n";
		echo "* Situação: ".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."\n\n";


		$db->executar("UPDATE cte.empenho SET empnumero='".$xml->body->row->numero_documento."',
											  ds_problema='".$xml->body->row->ds_problema."',
									  		  valor_total_empenhado=".((strlen($xml->body->row->valor_total_empenhado))?"'".$xml->body->row->valor_total_empenhado."'":"NULL").",
											  valor_saldo_pagamento=".((strlen($xml->body->row->valor_saldo_pagamento))?"'".$xml->body->row->valor_saldo_pagamento."'":"NULL").",
											  empsituacao='".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."'
					   WHERE empid='".$dados['empid']."'");

		$sql = "INSERT INTO cte.historicoempenho(
           		usucpf, empid, hepdata, empsituacao, ds_problema, valor_total_empenhado,
            	valor_saldo_pagamento)
    			VALUES ('".$_SESSION['usucpf']."',
    					'".$dados['empid']."',
    					NOW(),
    					'".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."',
    					'".$xml->body->row->ds_problema."',
    					".((strlen($xml->body->row->valor_total_empenhado))?"'".$xml->body->row->valor_total_empenhado."'":"NULL").",
    					".((strlen($xml->body->row->valor_saldo_pagamento))?"'".$xml->body->row->valor_saldo_pagamento."'":"NULL").");";

		$db->executar($sql);


		$db->commit();
		
		// simulando sem validacão do XML
		// return true;
		
		$result = (integer) $xml->status->result;

		if($result) {
			return false;
		} else {
		   	return true;
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


function solicitarPagamento($dados) {
	global $db;
	
	try {
		
	if(!$dados['empid']) {
		echo "Empenho não selecionado. Por favor, selecione um empenho";
		return false;
	}
		
	$data_created = date("c");
		
	$dadosse = $db->pegaLinha("SELECT emp.empcnpj, pro.proseqconta, pro.seq_conta_corrente,
									  emp.empnumeroprocesso, emp.empprogramafnde, 
									  emp.empnumerosistema, emp.empanooriginal,
									  emp.empnumero 
							   FROM cte.empenho emp 
							   INNER JOIN cte.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso 
							   WHERE empid='".$dados['empid']."'");
    if($dadosse) {
		
		// numero do processo (No desenvolvimento é fixo)
        if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
           $_SESSION['baselogin'] == "simec_espelho_producao" ){
           	
            $usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
			$nu_processo="23034655466200900";
			$nu_documento_siafi_ne = "340001";
//			$nu_cgc_favorecido = "12262713000102";
			$nu_cgc_favorecido = "15024029000180";
			$nu_seq_conta_corrente_favorec = "510793";
			
       	} else {
			$usuario = $dados['wsusuario'];
			$senha   = $dados['wssenha'];
			$nu_processo = $dadosse['empnumeroprocesso'];
			$nu_documento_siafi_ne = substr($dadosse['empnumero'],strpos($dadosse['empnumero'], 'NE')+2);
			$nu_cgc_favorecido = $dadosse['empcnpj'];
			$nu_seq_conta_corrente_favorec = $dadosse['seq_conta_corrente'];
        }

		$nu_cpf_favorecido = null;
		$nu_banco = null;
		$nu_agencia = null;
		$nu_conta_corrente = null;
		$an_convenio_original = null;
		$nu_convenio_original = null;
		$nu_convenio_siafi = null;
		$nu_proposta_siconv = null;
		$termo_aditivo_original = null;
		$apostilamento_original = null;
		$vl_custeio = "0";
		$vl_capital = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
		$an_referencia = date("Y");
		$sub_tipo_documento = "01";
		$nu_sistema = $dadosse['empnumerosistema'];
		$unidade_gestora = "153173";
		$gestao = "15253";
		$co_programa_fnde = $dadosse['empprogramafnde'];
		$parcela = $dados['pagparcela'];
		$darf = null;
		$tp_avaliador = null;
		$id_solicitante = null;
		$an_exercicio = $db->pegaUm("SELECT to_char(hepdata,'YYYY') as ano FROM cte.historicoempenho 
									 WHERE empid='".$dados['empid']."' ORDER BY hepdata ASC LIMIT 1");
 		
			
				
		$nu_mes = sprintf("%02d", $dados['mes']);				
		$valor = str_replace(array(".",","),array("","."),$dados['valorpagamento']);
					
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
				<an_parcela>$an_exercicio</an_parcela>
				<nu_mes>{$nu_mes}</nu_mes>
				<nu_documento_siafi_ne>{$nu_documento_siafi_ne}</nu_documento_siafi_ne>
			</item>
			</detalhamento_pagamento>
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
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}
			
			
			$sql = "INSERT INTO cte.historicowsprocessoobra(
				    	proid, 
				    	hwpwebservice, 
				    	hwpxmlenvio, 
				    	hwpxmlretorno, 
				    	hwpdataenvio, 
				        usucpf
				        )
				    VALUES ('".$_SESSION['par_var']['proid']."', 
				    		'solicitarPagamento - Erro', 
				    		'".addslashes($arqXml)."', 
				    		'".addslashes($xmlRetorno)."', 
				    		NOW(), 
				            '".$_SESSION['usucpf']."');";
			
			$db->executar($sql);
			$db->commit();
			
			
			echo "\n\n*** XML de solicitação ***\n\n";
			echo $arqXml;
	
		} else {
			
			$sql = "INSERT INTO cte.historicowsprocessoobra(
				    	proid, 
				    	hwpwebservice, 
				    	hwpxmlenvio, 
				    	hwpxmlretorno, 
				    	hwpdataenvio, 
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."', 
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
			
			if($dados['preid']) {
				foreach($dados['preid'] as $preid) {
					$sql = "INSERT INTO cte.pagamentoobra(preid, pagid, pobpercentualpag, pobvalorpagamento)
    						VALUES ('".$preid."', '".$pagid."', '".$dados['porcent'][$preid]."', '".str_replace(array(".",","),array("","."),$dados['valorpagamentoobra'][$preid])."');";
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
		
		
	}
}


function consultarPagamento($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

	    $dadospag = $db->pegaLinha("SELECT * FROM cte.pagamento WHERE pagid='".$dados['pagid']."'");
	
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

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/ob';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
		}

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
			
			$sql = "INSERT INTO cte.historicowsprocessoobra(
				    	proid, 
				    	hwpwebservice, 
				    	hwpxmlenvio, 
				    	hwpxmlretorno, 
				    	hwpdataenvio, 
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."', 
				    		'consultarPagamento - Erro', 
				    		'".addslashes($arqXml)."', 
				    		'".addslashes($xmlRetorno)."', 
				    		NOW(), 
				            '".$_SESSION['usucpf']."');";
			
			$db->executar($sql);
			$db->commit();
			
			return false;
			
	    
		} else {
			
			$sql = "INSERT INTO cte.historicowsprocessoobra(
				    	proid, 
				    	hwpwebservice, 
				    	hwpxmlenvio, 
				    	hwpxmlretorno, 
				    	hwpdataenvio, 
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."', 
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
			echo "* Valor(R$) : ".number_format($xml->body->row->valor_ob,2,",",".")."\n";
			echo "* Processo : ".$xml->body->row->processo."\n";
			echo "* Nº documento : ".((strlen($xml->body->row->numero_documento))?$xml->body->row->numero_documento:"-")."\n";
			echo "* CNPJ : ".((strlen($xml->body->row->nu_favorecido))?$xml->body->row->nu_favorecido:"-")."\n";
			echo "* Status : ".((strlen($xml->body->row->status))?$xml->body->row->status:"-")."\n";
			

			$db->executar("UPDATE cte.pagamento SET 
						   pagsituacaopagamento='".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."',
						   pagdatapagamentosiafi='".formata_data_sql(iconv("UTF-8", "ISO-8859-1", $xml->body->row->data_documento))."'
						   WHERE pagid='".$dadospag['pagid']."'");
	
			$db->executar("INSERT INTO cte.historicopagamento(
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

function cancelarPagamento($dados) {
	global $db;

	try {
		
		$data_created = date("c");
		
		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
			
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
			
			$sql = "INSERT INTO cte.historicowsprocessoobra(
				    	proid, 
				    	hwpwebservice, 
				    	hwpxmlenvio, 
				    	hwpxmlretorno, 
				    	hwpdataenvio, 
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."', 
				    		'cancelarPagamento - Sucesso', 
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
			
			$sql = "INSERT INTO cte.historicowsprocessoobra(
				    	proid, 
				    	hwpwebservice, 
				    	hwpxmlenvio, 
				    	hwpxmlretorno, 
				    	hwpdataenvio, 
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."', 
				    		'cancelarPagamento - Erro', 
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

?>