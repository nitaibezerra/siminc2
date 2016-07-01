<?php

function listaHistoricoEmpenho($dados) {
	global $db;
	$sql = "SELECT u.usunome, to_char(hepdata, 'dd/mm/YYYY HH24:MI') as data, empsituacao, ds_problema, valor_total_empenhado, valor_saldo_pagamento FROM cte.historicoempenho h
			LEFT JOIN seguranca.usuario u ON u.usucpf=h.usucpf
			WHERE h.empid='".$dados['empid']."'";
	
	$cabecalho = array("Usuário atualização","Data","Situação","Problema encontrado","Valor empenhado(R$)","Valor pagamento(R$)");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);

}


function cabecalhoSolicitacaoEmpenho()  {
	global $db;
	
	if($_SESSION['par_var']['esfera']=='estadual') {
		
		$arrDados = $db->pegaLinha("SELECT p.estuf,
										   '-' as mundescricao,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM cte.processoobra p
								    WHERE p.proid='".$_SESSION['par_var']['proid']."'");
		
	} else {

		$arrDados = $db->pegaLinha("SELECT m.muncod,
										   m.estuf,
										   m.mundescricao,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM cte.processoobra p
								    INNER JOIN territorios.municipio m ON m.muncod = p.muncod
								    WHERE p.proid='".$_SESSION['par_var']['proid']."'");
		
		
	}

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

function listaEmpenhoProcesso($dados) {
	global $db;
	if($dados['empnumeroprocesso']) {
		$where[] = "empnumeroprocesso='".$dados['empnumeroprocesso']."'";
	} elseif($_SESSION['par_var']['proid']) {
		$where[] = "proid='".$_SESSION['par_var']['proid']."'";
	}
	$where[] = "funid = 1";

	$sql = "SELECT '<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarHistoricoEmpenho(\''||e.empid||'\', this);\">' as mais,
				   CASE WHEN e.empsituacao!='CANCELADO' THEN '<img src=../imagens/refresh2.gif style=cursor:pointer; onclick=consultarEmpenho('||e.empid||',\'' || trim(e.empnumeroprocesso) || '\');>' ELSE '&nbsp;' END as acao_consultar,
				   CASE WHEN e.empsituacao!='CANCELADO' THEN '<img src=../imagens/excluir.gif align=absmiddle style=cursor:pointer; onclick=cancelarEmpenho('||e.empid||',\'' || trim(e.empnumeroprocesso) || '\');>' ELSE '&nbsp;' END as acao_cancelar,
				   e.empcnpj, en.entnome, e.empprotocolo, e.empnumero, e.empvalorempenho, u.usunome, e.empsituacao FROM cte.empenho e
			LEFT JOIN cte.processoobra p ON trim(e.empnumeroprocesso) = trim(p.pronumeroprocesso)
			LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf
			LEFT JOIN entidade.entidade en ON en.entnumcpfcnpj=e.empcnpj
			LEFT JOIN entidade.funcaoentidade fun ON fun.entid=en.entid
			".(($where)?"WHERE ".implode(" AND ", $where):"");

	$cabecalho = array("&nbsp;","&nbsp;","&nbsp;","CNPJ","Entidade","Nº protocolo","Nº empenho","Valor empenho(R$)","Usuário criação","Situação empenho");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
}

function listaObrasEmpenhadas($empid){
	global $db;
	
	if($_SESSION['par_var']['esfera']=='estadual') {
		
		$where[] = "po.preesfera='E'";
		
		$arrDados = $db->pegaLinha("SELECT p.estuf,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM cte.processoobra p
								    WHERE p.proid='".$_SESSION['par_var']['proid']."'");
		
	} else {
		
		$where[] = "po.preesfera='M'";

		$arrDados = $db->pegaLinha("SELECT m.muncod,
										   m.estuf,
										   m.mundescricao,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM cte.processoobra p
								    INNER JOIN territorios.municipio m ON m.muncod = p.muncod
								    WHERE p.proid='".$_SESSION['par_var']['proid']."'");
		
		
	}
	
	// PARAMETROS FIXOS
	$where[] = "doc.esdid='228'";
	$where[] = "po.prestatus='A'";
	$where[] = "po.tooid='1'";
	// FIM PARAMETROS FIXOS

	if($arrDados['estuf']) {
		$where[] = "po.estuf='".$arrDados['estuf']."'";
	}

	if($arrDados['muncod']) {
		$where[] = "po.muncod='".$arrDados['muncod']."'";
	}

	if($arrDados['protipo']) {
		$where[] = "pp.ptoclassificacaoobra='".$arrDados['protipo']."'";		
	}

	$sql = "SELECT terid FROM cte.termocompromissopac WHERE proid='".$_SESSION['par_var']['proid']."'";
	$terid = $db->pegaUm($sql);

	/* Se o processo ja tiver gerado termo, apresentar apenas as obras do termo */
	if($terid) {

		$join = "INNER JOIN cte.termoobra teo ON teo.preid = po.preid AND teo.terid='".$terid."'";

	} else {

		/* Filtro padrão pegando municipio, UF, e tipo de obra */
		if($where) {
			$join = "WHERE ".implode(" AND ", $where)."  -- AND emo.eobid IS NULL";
		}

	}
	
	$sql = "SELECT
				'' as mais,
				po.preid || ' - ' || po.predescricao as nomedaobra,
				sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as vlr,
				'<center>'||emo.eobpercentualemp::text||'</center>' as porcempenho,
				sum(distinct emo.eobvalorempenho) as vlr_empenho
			FROM obras.preobra po 
			INNER JOIN obras.pretipoobra pp on po.ptoid = pp.ptoid 
			INNER JOIN workflow.documento 			 doc ON doc.docid = po.docid
			INNER JOIN obras.preitenscomposicao      itc ON po.ptoid   = itc.ptoid AND itcquantidade > 0
			LEFT JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid   = ppo.itcid AND ppo.preid = po.preid
			INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid   = po.ptoid
			INNER JOIN cte.empenhoobra emo ON emo.preid   = po.preid AND emo.empid IN (SELECT empid FROM cte.empenho emp
																					  INNER JOIN cte.processoobra pro ON emp.empnumeroprocesso = pro.pronumeroprocesso
																					  WHERE pro.proid='".$_SESSION['par_var']['proid']."')
																					  
						AND emo.preid in (	select po.preid
																	from obras.preobra po
																	INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
																	INNER JOIN cte.empenhoobra emo ON emo.preid   = po.preid
																	group by po.preid having sum(emo.eobpercentualemp) = 100)
			
			WHERE ".implode(" AND ", $where)."  AND empid = ".$empid["empid"]." 
			GROUP BY po.preid, po.predescricao, tpo.ptopercentualempenho, emo.eobid,emo.eobpercentualemp";

	$cabecalho = array("&nbsp;","Nome da obra","Valor da obra","% Empenho","Valor empenhado");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'S','95%','S');
}


function listaPreObras($dados) {
	global $db;

	if($_SESSION['par_var']['esfera']=='estadual') {
		
		$where[] = "po.preesfera='E'";
		
		$arrDados = $db->pegaLinha("SELECT p.estuf,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM cte.processoobra p
								    WHERE p.proid='".$_SESSION['par_var']['proid']."'");
		
	} else {
		
		$where[] = "po.preesfera='M'";

		$arrDados = $db->pegaLinha("SELECT m.muncod,
										   m.estuf,
										   m.mundescricao,
										   p.pronumeroprocesso,
										   CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										   p.protipo
									FROM cte.processoobra p
								    INNER JOIN territorios.municipio m ON m.muncod = p.muncod
								    WHERE p.proid='".$_SESSION['par_var']['proid']."'");
		
		
	}
	
	// PARAMETROS FIXOS
	$where[] = "doc.esdid='228'";
	$where[] = "po.tooid='1'";
	$where[] = "po.prestatus='A'";
	// FIM PARAMETROS FIXOS
	
	if($arrDados['estuf']) {
		$where[] = "po.estuf='".$arrDados['estuf']."'";
	}

	if($arrDados['muncod']) {
		$where[] = "po.muncod='".$arrDados['muncod']."'";
	}

	if($arrDados['protipo']) {
		$where[] = "pp.ptoclassificacaoobra='".$arrDados['protipo']."'";		
	}
	
	$sql = "SELECT terid FROM cte.termocompromissopac WHERE proid='".$_SESSION['par_var']['proid']."'";
	$terid = $db->pegaUm($sql);
	
	/* Se o processo ja tiver gerado termo, apresentar apenas as obras do termo */
	if($terid) {
		$join = "INNER JOIN cte.termoobra teo ON teo.preid = po.preid AND teo.terid= '".$terid."'";
	} else {
		/* Filtro padrão pegando municipio, UF, e tipo de obra */
		if($where) {
			$clwhere = "WHERE ".implode(" AND ", $where)." ";
		}
	}
	
	$sql = "
			SELECT  '<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarObrasEmpenhadas(\''||emp.empid||'\', this);\">' as mais,
					emp.empnumero,
					res.resdescricao
			FROM obras.preobra po 
			INNER JOIN obras.pretipoobra pp on po.ptoid = pp.ptoid
			INNER JOIN workflow.documento 		doc ON doc.docid = po.docid
			INNER JOIN obras.preitenscomposicao itc ON po.ptoid = itc.ptoid AND itcquantidade > 0
			LEFT JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid   = ppo.itcid AND ppo.preid = po.preid
			INNER JOIN cte.resolucao res ON res.resid = po.resid
			INNER JOIN cte.empenhoobra emo ON emo.preid   = po.preid AND emo.empid IN (	SELECT empid FROM cte.empenho emp
													INNER JOIN cte.processoobra pro ON emp.empnumeroprocesso = pro.pronumeroprocesso
													WHERE pro.proid='".$_SESSION['par_var']['proid']."')
													
										AND emo.preid in (	select po.preid
																	from obras.preobra po
																	INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
																	INNER JOIN cte.empenhoobra emo ON emo.preid   = po.preid
																	group by po.preid having sum(emo.eobpercentualemp) = 100)
			INNER JOIN cte.empenho emp ON emp.empid = emo.empid
			WHERE 	".implode(" AND ", $where)." 
			GROUP BY emp.empnumero,emp.empid, res.resdescricao";
	
	echo "<h3>Obras 100% empenhadas</h3>";
	$cabecalho = array("&nbsp;","Nº do Empenho","Resolução");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%','S');

			 
	$sql = "SELECT
                '<input type=checkbox name=chk[] onclick=marcarChk(this); id=chk_'||po.preid||' value='||po.preid||'>' as chk,
				po.preid || ' - ' || po.predescricao as nomedaobra,
				sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as vlr,
				'<center>'||emo.eobpercentualemp::text||'</center> <input type=hidden id=porcentagem_'||po.preid||' value='||emo.eobpercentualemp||' >' as percentual_Empenhado,
				emo.eobvalorempenho as vlr_empenhado, 
				-- sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade)*tpo.ptopercentualempenho/100 as vlr_empenhado,
				-- '<input type=text id=id_'||po.preid||' value=\'\' name=name_'||po.preid||' size=6 onKeyUp=\"this.value=mascaraglobal(\'##########\',this.value);calculaEmpenho(this);\" onBlur=\"verificaPreenchimentoPorcentagem(this) \" class=\"disabled\" readonly=readonly onfocus=\"this.select();\"><input type=hidden id=vlr_'||po.preid||' name=vlr_'||po.preid||' value='||(sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade)*tpo.ptopercentualempenho/100)||'>' as porcempenho,
				CASE WHEN emo.eobpercentualemp = NULL  
					THEN '<input type=text id=id_'||po.preid||' value='||tpo.ptopercentualempenho||' name=name_'||po.preid||' size=6 onKeyUp=\"calculaEmpenho(this);\" onBlur=\"verificaPreenchimentoPorcentagem(this) \" class=\"disabled\" readonly=readonly onfocus=\"this.select();\"><input type=hidden id=vlr_'||po.preid||' name=vlr_'||po.preid||' value='||(sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade)*tpo.ptopercentualempenho/100)||'>' 
					ELSE '<input type=text id=id_'||po.preid||' value=\'\' name=name_'||po.preid||' size=6 onKeyUp=\"calculaEmpenho(this);\" onBlur=\"verificaPreenchimentoPorcentagem(this) \" class=\"disabled\" readonly=readonly onfocus=\"this.select();\"><input type=hidden id=vlr_'||po.preid||' name=vlr_'||po.preid||' value=\'\'>'  
				END as porcempenho,
				
				CASE WHEN emo.eobpercentualemp = NULL  
					THEN sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) - (sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade)*tpo.ptopercentualempenho/100)
					ELSE NULL 
				END as vlr_empenho,
				-- sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) - (sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade)*tpo.ptopercentualempenho/100) as vlr_empenho,
				res.resdescricao
         FROM obras.preobra po
         INNER JOIN workflow.documento                                   doc ON doc.docid = po.docid
         INNER JOIN cte.resolucao                                             res ON res.resid = po.resid
         INNER JOIN obras.preitenscomposicao      itc ON po.ptoid   = itc.ptoid AND itcquantidade > 0
         LEFT JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid  = ppo.itcid AND ppo.preid = po.preid
         INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
         INNER JOIN cte.empenhoobra emo ON emo.preid   = po.preid  
         {$join}
		 where emo.empid IN (       SELECT empid FROM cte.empenho emp
                 					INNER JOIN cte.processoobra pro ON emp.empnumeroprocesso = pro.pronumeroprocesso
                 					WHERE pro.proid='".$_SESSION['par_var']['proid']."') 
                 		AND emo.preid in (  select po.preid
             								from obras.preobra po
								             INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
								             INNER JOIN cte.empenhoobra emo ON emo.preid   = po.preid
            								 group by po.preid having sum(emo.eobpercentualemp) <> 100)
        GROUP BY po.preid, po.predescricao, tpo.ptopercentualempenho, emo.eobid,emo.eobpercentualemp, res.resdescricao, emo.eobvalorempenho
        
        UNION ALL
        
        SELECT
				'<input type=checkbox name=chk[] onclick=marcarChk(this); id=chk_'||po.preid||' value='||po.preid||'>' as chk,
				po.preid || ' - ' || po.predescricao as nomedaobra,
				sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as vlr,
				'' as percentual_Empenhado,
				0 as vlr_empenhado, 
				'<input type=text id=id_'||po.preid||' value='||tpo.ptopercentualempenho||' name=name_'||po.preid||' size=6 onKeyUp=\"calculaEmpenhoSemNadaEmpenhado(this);\" class=\"disabled\" readonly=readonly onfocus=\"this.select();\"><input type=hidden id=vlr_'||po.preid||' name=vlr_'||po.preid||' value='||(sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade)*tpo.ptopercentualempenho/100)||'>'  as porcempenho, 
				sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade)  vlr_empenho,
				res.resdescricao
			FROM obras.preobra po 
			INNER JOIN obras.pretipoobra pp on po.ptoid = pp.ptoid
			INNER JOIN workflow.documento 			 doc ON doc.docid = po.docid
			INNER JOIN cte.resolucao 			 	 res ON res.resid = po.resid
			INNER JOIN obras.preitenscomposicao      itc ON po.ptoid   = itc.ptoid AND itcquantidade > 0
			LEFT JOIN obras.preplanilhaorcamentaria  ppo ON itc.itcid  = ppo.itcid AND ppo.preid = po.preid
			INNER JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
			left JOIN cte.empenhoobra emo ON emo.preid   = po.preid
			 {$clwhere}
			and po.preid not in (select preid from cte.empenhoobra  )
			GROUP BY po.preid, po.predescricao, tpo.ptopercentualempenho, res.resdescricao";
			 
	echo "<h3>Obras a serem empenhadas</h3>";
	echo "<form id=\"formpreobras\">";
	$cabecalho = array("&nbsp;","Nome da obra","Valor da obra","% empenhado","Valor empenhado","% Empenho","Valor a empenhar","Resolução");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%','S');
	echo "</form>";


}


function executarEmpenho($dados) {

	$res_sp = solicitarProcesso($dados);
	$res_cc = consultarContaCorrente($dados);
	if($res_cc) $res_sc = solicitarContaCorrente($dados);
	$res_se = solicitarEmpenho($dados);

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

		    if($xml->body->row->seq_conta) {
		    	$db->executar("UPDATE cte.processoobra SET nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', seq_conta_corrente='".$xml->body->row->seq_conta."' WHERE proseqconta='".$proseqconta."'");
		    	$db->commit();
		    }

			echo "------ CONSULTA DE CONTA CORRENTE ------\n\n";
			echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
			echo "*** Detalhes da consulta ***\n\n";
			echo "* Data movimento:".(($xml->body->row->dt_movimento)?$xml->body->row->dt_movimento:'-')."\n";
			echo "* Fase solicitação:".(($xml->body->row->fase_solicitacao)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->fase_solicitacao):'-')."\n";
			echo "* Entidade:".(($xml->body->row->ds_razao_social)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->ds_razao_social):'-')."(".(($xml->body->row->nu_identificador)?$xml->body->row->nu_identificador:'-').")\n\n";

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

		    if($result) {
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
        // constante=5
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

function consultarEmpenho($dados) {
	global $db;

	try {

		$data_created = date("c");

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
			$nu_seq_ne = "73289";
		} else {
			$usuario = $dados['wsusuario'];
			$senha   = $dados['wssenha'];

		    $dadosemp = $db->pegaLinha("SELECT * FROM cte.empenho WHERE empid='".$dados['empid']."'");

	        if($dadosemp) {
	        	$nu_seq_ne = $dadosemp['empprotocolo'];
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

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;
	    
		if($result) $hwpwebservice="consultarEmpenho - Sucesso";
		else $hwpwebservice="consultarEmpenho - Erro";
		
		$sql = "INSERT INTO cte.historicowsprocessoobra(
			    	proid,
			    	hwpwebservice,
			    	hwpxmlenvio,
			    	hwpxmlretorno,
			    	hwpdataenvio,
			        usucpf)
			    VALUES ('".$_SESSION['par_var']['proid']."',
			    		'".$hwpwebservice."',
			    		'".addslashes($arqXml)."',
			    		'".addslashes($xmlRetorno)."',
			    		NOW(),
			            '".$_SESSION['usucpf']."');";

		$db->executar($sql);
		$db->commit();

		
	    echo "------ CONSULTA DE EMPENHO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
		echo "*** Detalhes da consulta ***\n\n";
		echo "* Nº processo : ".(($xml->body->row->processo)?$xml->body->row->processo:"-")."\n";
		echo "* CNPJ : ".$xml->body->row->nu_cnpj."\n";
		echo "* Valor(R$) : ".number_format($xml->body->row->valor_ne,2,",",".")."\n";
		echo "* Data : ".$xml->body->row->data_documento."\n";
		echo "* Nº documento : ".((strlen($xml->body->row->numero_documento))?$xml->body->row->numero_documento:"-")."\n";
		echo "* Valor empenhado(R$) : ".((strlen($xml->body->row->valor_total_empenhado))?$xml->body->row->valor_total_empenhado:"-")."\n";
		echo "* Saldo pagamento(R$) : ".((strlen($xml->body->row->valor_saldo_pagamento))?$xml->body->row->valor_saldo_pagamento:"-")."\n";
		echo "* Situação : ".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."\n\n";
		
		if($xml->body->row->numero_documento) $set[] = "empnumero='".$xml->body->row->numero_documento."'";
		if($xml->body->row->ds_problema) $set[] = "ds_problema='".$xml->body->row->ds_problema."'";
		if($xml->body->row->valor_total_empenhado) $set[] = "valor_total_empenhado='".$xml->body->row->valor_total_empenhado."'";
		if($xml->body->row->valor_saldo_pagamento) $set[] = "valor_saldo_pagamento='".$xml->body->row->valor_saldo_pagamento."'";
		if($xml->body->row->situacao_documento) $set[] = "empsituacao='".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."'";
		
		if($set) {
			$db->executar("UPDATE cte.empenho SET ".(($set)?implode(",",$set):"")." 
						   WHERE empid='".$dados['empid']."'");
		}

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
			
		if($result) {
			return false;
		} else {
		   	return true;
		}


	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Consulta empenho encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar empenho no SIGEF: $erroMSG";


	}
}

function solicitarEmpenho($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

	    $dadosse = $db->pegaLinha("SELECT pronumeroprocesso, probanco, proagencia, muncod, protipo FROM cte.processoobra WHERE proid='".$_SESSION['par_var']['proid']."'");
        if($dadosse) {

	        // numero do processo (No desenvolvimento é fixo)
        	if($_SESSION['baselogin'] == "simec_desenvolvimento1" ||
        	   $_SESSION['baselogin'] == "simec_espelho_producao1" ){

        	   	$nu_processo='23034655466200900';
				$co_fonte_recurso_solic="0100000000";
				$co_plano_interno_solic="PFB02F52BWN";
				$co_ptres_solic="020990";
				$co_natureza_despesa_solic="44504200";



        	} else {

	        	$nu_processo=$dadosse['pronumeroprocesso'];
				// constante=4042
				$co_natureza_despesa_solic="44404200";

	        	if($dadosse['protipo'] == 'P') {
					// constante=MEC00001
					$co_plano_interno_solic="MEC00001";
					// constante=037825
					$co_ptres_solic="037825";
					// constante=12365144812KU0001
					$frpfuncionalprogramatica="12365144812KU0001";

	        	} else {
					// constante=MEC00002
					$co_plano_interno_solic="MEC00002";
					// constante=037826
					$co_ptres_solic="037826";
					// constante=12365144812KV0001
					$frpfuncionalprogramatica="12365144812KV0001";
	        	}

				$sql = "SELECT * FROM cte.fonterecursopac WHERE frpfuncionalprogramatica='".$frpfuncionalprogramatica."' ORDER BY frpid";
				$fonterecursopac = $db->carregar($sql);
				if($fonterecursopac[0]) {
					foreach($fonterecursopac as $frpsaldo) {
						$sql = "SELECT sum(e.empvalorempenho) as vlr FROM cte.empenho e 
								INNER JOIN cte.processoobra p ON p.pronumeroprocesso = e.empnumeroprocesso 
								WHERE p.protipo='".$dadosse['protipo']."' AND e.empfonterecurso='".$frpsaldo['frpcodigo']."'";
						$valor_recurso_pac = $db->pegaUm($sql);
						$total_parcial = ($valor_recurso_pac + str_replace(array(".",","),array("","."),$dados['name_total']));
						if($total_parcial <= $frpsaldo['frpsaldo']) {
							$co_fonte_recurso_solic=$frpsaldo['frpcodigo'];
							break;
						}
					}
					if(!$co_fonte_recurso_solic) {
					    echo "------ SOLICITAÇÃO DE EMPENHO ------\n\n";
					    echo "MSG SIMEC : Fonte de recurso ".$frpsaldo['frpcodigo']." - ".$frpsaldo['frptitulo']." finalizada \n";
					    echo "Saldo da fonte : ".$frpsaldo['frpsaldo']." < (Total de empenho : ".$valor_recurso_pac." + Valor empenho atual : ".str_replace(array(".",","),array("","."),$dados['name_total']).")";
					    return false;
					}
					
				}


        	}
        }

		// cnpj da prefeitura
		$nu_cnpj_favorecido=$db->pegaUm("SELECT ent.entnumcpfcnpj
					 				   FROM entidade.entidade ent
					 				   INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
					 				   INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 				   WHERE fen.funid=1 AND ende.muncod='".$dadosse['muncod']."'");;
		// nulo
		$nu_empenho_original=null;
		// nulo
		$an_exercicio_original=null;
		// total do empenho, calculado na tela
		$vl_empenho=str_replace(array(".",","),array("","."),$dados['name_total']);
		// constante=01
		$co_especie_empenho="01";
		// constante=1
		$co_esfera_orcamentaria_solic="1";
		// constante=69500000000
		$co_centro_gestao_solic="69500000000";
		// nulo
		$an_convenio=null;
		// nulo
		$nu_convenio=null;
		// constante=2
		$co_observacao="2";
		// constante=3
		$co_tipo_empenho="3";
		// constante=0010
		$co_descricao_empenho="0010";
		// constante=15253
		$co_gestao_emitente="15253";
        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
        if($dadosse['protipo'] == 'P') $co_programa_fnde="BW";
        else $co_programa_fnde="CN";
		// constante=153173
		$co_unidade_gestora_emitente="153173";
		// constante=26298
		$co_unidade_orcamentaria_solic="26298";
		// nulo
		$nu_proposta_siconv=null;
		// constante=2
		$nu_sistema="2";


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
		<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
		<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
		<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
		<vl_empenho>$vl_empenho</vl_empenho>
		<nu_processo>$nu_processo</nu_processo>
		<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
		<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
		<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
		<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
		<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
		<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
		<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
		<an_convenio>$an_convenio</an_convenio>
		<nu_convenio>$nu_convenio</nu_convenio>
		<co_observacao>$co_observacao</co_observacao>
		<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
		<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
		<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
		<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
		<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
		<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
		<nu_sistema>$nu_sistema</nu_sistema>
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
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

	    echo "------ SOLICITAÇÃO DE EMPENHO ------\n\n";
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
				    		'solicitarEmpenho - Erro',
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
				    		'solicitarEmpenho - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);


			$sql = "INSERT INTO cte.empenho(
            empcnpj,
            empnumerooriginal,
            empanooriginal,
            empvalorempenho,
            empnumeroprocesso,
            empcodigoespecie,
            empcodigopi,
            empcodigoesfera,
            empcodigoptres,
            empfonterecurso,
            empcodigonatdespesa,
            empcentrogestaosolic,
            empanoconvenio,
            empnumeroconvenio,
            empcodigoobs,
            empcodigotipo,
            empdescricao,
            empgestaoeminente,
            empunidgestoraeminente,
            empprogramafnde,
            empnumerosistema,
            usucpf,
            empprotocolo,
            empsituacao
            )
		    VALUES (".(($nu_cnpj_favorecido)?"'".$nu_cnpj_favorecido."'":"NULL").",
		    		".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
		    		".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
		    		".(($vl_empenho)?"'".$vl_empenho."'":"NULL").",
		            ".(($nu_processo)?"'".$nu_processo."'":"NULL").",
		            ".(($co_especie_empenho)?"'".$co_especie_empenho."'":"NULL").",
		            ".(($co_plano_interno_solic)?"'".$co_plano_interno_solic."'":"NULL").",
		            ".(($co_esfera_orcamentaria_solic)?"'".$co_esfera_orcamentaria_solic."'":"NULL").",
		            ".(($co_ptres_solic)?"'".$co_ptres_solic."'":"NULL").",
		            ".(($co_fonte_recurso_solic)?"'".$co_fonte_recurso_solic."'":"NULL").",
		            ".(($co_natureza_despesa_solic)?"'".$co_natureza_despesa_solic."'":"NULL").",
		            ".(($co_centro_gestao_solic)?"'".$co_centro_gestao_solic."'":"NULL").",
		            ".(($an_convenio)?"'".$an_convenio."'":"NULL").",
		            ".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
		            ".(($co_observacao)?"'".$co_observacao."'":"NULL").",
		            ".(($co_tipo_empenho)?"'".$co_tipo_empenho."'":"NULL").",
		            ".(($co_descricao_empenho)?"'".$co_descricao_empenho."'":"NULL").",
		            ".(($co_gestao_emitente)?"'".$co_gestao_emitente."'":"NULL").",
		            ".(($co_unidade_gestora_emitente)?"'".$co_unidade_gestora_emitente."'":"NULL").",
		            ".(($co_programa_fnde)?"'".$co_programa_fnde."'":"NULL").",
		            ".(($nu_sistema)?"'".$nu_sistema."'":"NULL").",
		            '".$_SESSION['usucpf']."',
		            '".$xml->body->nu_seq_ne."',
		            '8 - SOLICITAÇÃO APROVADA'
		            ) RETURNING empid;";

			$empid = $db->pegaUm($sql);

			$sql = "INSERT INTO cte.historicoempenho(
            		usucpf, empid, hepdata, empsituacao)
    				VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '8 - SOLICITAÇÃO APROVADA');";

			$db->executar($sql);

			if($dados['chk']) {
				foreach($dados['chk'] as $preid)
				{
					$sql = "INSERT INTO cte.empenhoobra(
            				preid, empid, eobpercentualemp, eobvalorempenho)
    						VALUES ('".$preid."', '".$empid."', '".$dados['name_'.$preid]."', '".$dados['vlr_'.$preid]."');";

					$db->executar($sql);
					
					/*** INICIO - Importação dos dados para o sistema de Obras - INICIO ***/
					
					/*** Só executa a importação caso a obra não exista ***/
					$sql = "SELECT count(1) FROM obras.preobra WHERE preid = ".$preid." AND obrid is not null";
					$existeObra = $db->pegaUm($sql);
					
					if( (integer)$existeObra < 1 )
					{
						/*** Recupera dados da Pre Obra ***/
						$sql = "SELECT 
									p.predescricao || ' - ' || mun.mundescricao || ' - ' || mun.estuf as nome_obra,
									ent.entid as unidade_implantadora
								FROM 
									obras.preobra p
								INNER JOIN
									territorios.municipio mun on p.muncod = mun.muncod
								INNER JOIN 
									entidade.endereco ende ON ende.muncod = p.muncod
								INNER JOIN
									entidade.entidade ent ON ent.entid = ende.entid AND ent.entstatus = 'A'
								INNER JOIN
									entidade.funcaoentidade fen ON ent.entid = fen.entid AND fen.funid IN (1)
								WHERE 
									p.preid = ".$preid;
						$dadosPreObra = $db->carregar($sql);
						
						/*** Insere a nova obra ***/
						$sql = "INSERT INTO 
								obras.obrainfraestrutura(obrdesc,entidunidade,orgid,tooid) 
								VALUES('".$dadosPreObra[0]['nome_obra']."',
										".$dadosPreObra[0]['unidade_implantadora'].",
										3,
										1) 
								RETURNING obrid";
						$obrid = $db->pegaUm($sql);
						
						/*** Recupera as fotos do terreno no Pré Obra ***/
						$sql = "SELECT DISTINCT
									arq.arqid
								FROM 
									public.arquivo arq
								INNER JOIN 
									obras.preobrafotos pof ON arq.arqid = pof.arqid
								INNER JOIN 
									obras.preobra pre ON pre.preid = pof.preid
								WHERE							
									pre.preid = ".$preid."
								AND							
									(substring(arqtipo,1,5) = 'image')";
						$fotosTerreno = $db->carregar($sql);
						
						if( $fotosTerreno )
						{
							/*** Insere as fotos para galeria de fotos da obra ***/
							foreach($fotosTerreno as $foto)
							{
								$sql = "INSERT INTO 
										obras.arquivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
										VALUES
										(".$obrid.", 21, ".$foto['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
								$db->executar($sql);
							}
						}
						
						/*** Recupera os documentos anexos no Pré Obra ***/
						$sql = "SELECT DISTINCT
									arq.arqid
								FROM 
									obras.preobraanexo p
								INNER JOIN 
									public.arquivo arq ON arq.arqid = p.arqid
								WHERE							
									p.preid = ".$preid;
						$anexos = $db->carregar($sql);
						
						if( $anexos )
						{
							/*** Insere os documentos nos arquivos da obra ***/
							foreach($anexos as $anexo)
							{
								$sql = "INSERT INTO 
										obras.arquivosobra(obrid,tpaid,arqid,usucpf,aqodtinclusao,aqostatus)
										VALUES
										(".$obrid.", 21, ".$anexo['arqid'].", '".$_SESSION['usucpf']."', '".date("Y-m-d H:i:s")."', 'A')";
								$db->executar($sql);
							}
						}
						
						/*** Inclue o ID da nova obra na tabela do pre obra ***/
						$sql = "UPDATE obras.preobra SET obrid = ".$obrid." WHERE preid = ".$preid;
						$db->executar($sql);
					}
					/*** FIM - Importação dos dados para o sistema de Obras - FIM ***/
				}
			}

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

function cancelarEmpenho($dados) {
	global $db;

	try {
		// validando se tem termo
		$sql = "SELECT te.terid FROM cte.empenhoobra o
				INNER JOIN cte.termoobra t ON t.preid = o.preid 
				INNER JOIN cte.termocompromissopac te ON te.terid = t.terid
				WHERE o.empid='".$dados['empid']."' AND te.terassinado=TRUE";

		$existe_termo = $db->pegaUm($sql);

		if($existe_termo) {
	    	echo "------ EMPENHO NÃO PODE SER CANCELADO ------\n\n";
	    	echo "Obras existentes neste empenho pertencem a um termo de compromisso";
	    	exit;
		}


		$data_created = date("c");
		
		$sql = "SELECT count(*) as num FROM cte.historicoempenho WHERE empid='".$dados['empid']."' AND empsituacao='2 - EFETIVADO'";
		$historicoefetivado = $db->pegaUm($sql);
		
		

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
			$nu_seq_ne = "73289";
		} else {
			$usuario = $dados['wsusuario'];
			$senha   = $dados['wssenha'];

		    $dadosemp = $db->pegaLinha("SELECT * FROM cte.empenho WHERE empid='".$dados['empid']."'");

	        if($dadosemp) {
	        	$nu_seq_ne = $dadosemp['empprotocolo'];
	        }

		}
		
		if($historicoefetivado>0) {
			
	      	$nu_processo=$dadosemp['empnumeroprocesso'];
			// constante=4042
			$co_natureza_despesa_solic="44404200";
			
			if($dadosemp['empprogramafnde'] == 'BW') {
				// constante=MEC00001
				$co_plano_interno_solic="MEC00001";
				// constante=037825
				$co_ptres_solic="037825";
				// constante=12365144812KU0001
				$frpfuncionalprogramatica="12365144812KU0001";
        	} else {
				// constante=MEC00002
				$co_plano_interno_solic="MEC00002";
				// constante=037826
				$co_ptres_solic="037826";
				// constante=12365144812KV0001
				$frpfuncionalprogramatica="12365144812KV0001";
        	}
			
			$nu_cnpj_favorecido=$dadosemp['empcnpj'];
			
			if($dadosemp['empnumero']) {
				$arrNumero = explode("NE",$dadosemp['empnumero']);
				// nulo
				$nu_empenho_original=$arrNumero[1];
				// nulo
				$an_exercicio_original=$arrNumero[0];
			} else {
				// nulo
				$nu_empenho_original=null;
				// nulo
				$an_exercicio_original=null;
			}
			
			// total do empenho, calculado na tela
			$vl_empenho=str_replace(array(".",","),array("","."),$dadosemp['empvalorempenho']);
			// constante=01
			$co_especie_empenho="03";
			// constante=1
			$co_esfera_orcamentaria_solic="1";
			// constante=69500000000
			$co_centro_gestao_solic="69500000000";
			// nulo
			$an_convenio=null;
			// nulo
			$nu_convenio=null;
			// constante=2
			$co_observacao="2";
			// constante=3
			$co_tipo_empenho="3";
			// constante=0010
			$co_descricao_empenho="0010";
			// constante=15253
			$co_gestao_emitente="15253";
	        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
	        $co_programa_fnde=$dadosemp['empprogramafnde'];
	        
			// constante=153173
			$co_unidade_gestora_emitente="153173";
			// constante=26298
			$co_unidade_orcamentaria_solic="26298";
			// nulo
			$nu_proposta_siconv=null;
			// constante=2
			$nu_sistema="2";
	
	
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
			<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
			<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
			<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
			<vl_empenho>$vl_empenho</vl_empenho>
			<nu_processo>$nu_processo</nu_processo>
			<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
			<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
			<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
			<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
			<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
			<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
			<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
			<an_convenio>$an_convenio</an_convenio>
			<nu_convenio>$nu_convenio</nu_convenio>
			<co_observacao>$co_observacao</co_observacao>
			<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
			<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
			<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
			<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
			<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
			<nu_sistema>$nu_sistema</nu_sistema>
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
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute();
	
			$xmlRetorno = $xml;
	
		    $xml = simplexml_load_string( stripslashes($xml));
	
		    echo "------ SOLICITAÇÃO DE EMPENHO (CANCELAMENTO) ------\n\n";
			echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
			
			$result = (integer) $xml->status->result;
	
			if($result) $hwpwebservice = "cancelarEmpenho2 - Sucesso";
			else $hwpwebservice = "cancelarEmpenho2 - Erro";
			
			$sql = "INSERT INTO cte.historicowsprocessoobra(
				    	proid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['proid']."',
				    		'".$hwpwebservice."',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
			
			$db->executar($sql);
			$db->commit();

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
				->setParams( array('xml' => $arqXml, 'method' => 'cancelar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

	    echo "------ CANCELAMENTO DE EMPENHO ------\n\n";
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
				    		'cancelarEmpenho - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			$db->executar("UPDATE cte.empenho SET empsituacao='CANCELADO'
					   	  WHERE empid='".$dados['empid']."'");

			$db->executar("INSERT INTO cte.historicoempenho(
            			   usucpf, empid, hepdata, empsituacao)
    					   VALUES ('".$_SESSION['usucpf']."', '".$dados['empid']."', NOW(), 'CANCELADO');");

			$db->executar("DELETE FROM cte.empenhoobra WHERE empid='".$dados['empid']."'");

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
				    		'cancelarEmpenho - Erro',
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
			echo "Erro-Serviço Cancelar Empenho encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Cancelar Empenho no SIGEF: $erroMSG";


	}
}


function solicitarProcesso($dados) {
	global $db;

    $dadosse = $db->pegaLinha("SELECT pronumeroprocesso, probanco, proagencia, muncod, protipo
    						   FROM cte.processoobra
    						   WHERE proid='".$_SESSION['par_var']['proid']."'");

    if($dadosse) {
    	$an_processo = date("Y");
    	$nu_processo=$dadosse['pronumeroprocesso'];
    	$tp_processo=1;
    	if($dadosse['protipo'] == 'P') $co_programa_fnde="BW";
        else $co_programa_fnde="CN";

		// cnpj da prefeitura
		$nu_cnpj_favorecido=$db->pegaUm("SELECT ent.entnumcpfcnpj
					 				   FROM entidade.entidade ent
					 				   INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
					 				   INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 				   WHERE fen.funid=1 AND ende.muncod='".$dadosse['muncod']."'");
    }

	$data_created = date("c");
	$usuario = $dados['wsusuario'];
	$senha   = $dados['wssenha'];

    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<params>
      	<nu_cnpj>$nu_cnpj_favorecido</nu_cnpj>
      	<nu_processo>$nu_processo</nu_processo>
      	<tp_processo>$tp_processo</tp_processo>
      	<an_processo>$an_processo</an_processo>
      	<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;


		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/corp/integracao/web/dev.php/processo/solicitar';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/corp/index.php/processo/solicitar';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

	    echo "------ SOLICITAÇÃO DE PROCESSO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		$result = (integer) $xml->status->result;
		if(!$result) {
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
					echo "\n";
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
				    		'solicitarProcesso - Erro',
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
				    		'solicitarProcesso - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return true;
		}

}

?>