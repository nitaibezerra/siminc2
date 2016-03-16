<?php
$dmdid = $_SESSION['dmdid'];


$habil = 'N';
$mostraWorkflow = 'N';

//verifica se o usuário pode editar a demanda
$verificaEditaDemanda = verificaEditaDemanda();
if($verificaEditaDemanda == true){
	$habil = 'S';
	$mostraWorkflow = 'S';
}
else{
	$habil = 'N';
	$mostraWorkflow = 'N';
}

// Pega o "estado do documento", vinculado à demanda
$esdid = recuperaEstadoDocumento();
//verifica se o estado do documento é finalizada ou cancelada (se sim, bloqueia todos os campos).
$andTipid = "AND tipstatus = 'A'";
if( in_array($esdid, array(DEMANDA_ESTADO_CANCELADO,DEMANDA_GENERICO_ESTADO_CANCELADO,DEMANDA_ESTADO_FINALIZADO,DEMANDA_GENERICO_ESTADO_FINALIZADO,DEMANDA_ESTADO_INVALIDADA,DEMANDA_GENERICO_ESTADO_INVALIDADA,DEMANDA_ESTADO_VALIDADA_FORA_PRAZO,DEMANDA_GENERICO_ESTADO_AUDITADO,DEMANDA_ESTADO_AUDITADO)) ){
	$habil = 'N';
	$andTipid = '';
}

//desabilita campos para o perfil analista fnde
if (in_array(DEMANDA_PERFIL_ANALISTA_FNDE, arrayPerfil())){
	$habil = 'N';
}

//habilita e desabilita campos para o perfil gerente fabrica de software
if ( in_array(DEMANDA_PERFIL_GERENTE_FSW, arrayPerfil()) || in_array(DEMANDA_PERFIL_ANALISTA_FSW, arrayPerfil()) ){
	$habil = 'N';
	$mostraWorkflow = 'S';
}

if ( count(arrayPerfil()) == 1 && ( in_array(DEMANDA_PERFIL_DEMANDANTE, arrayPerfil()) || in_array(DEMANDA_PERFIL_DEMANDANTE_AVANCADO, arrayPerfil()) ) ) {
	$habil = 'N';
}


//insere o campo SS
if ($_REQUEST['dmdidSS'] && $_REQUEST['scsid']) {
	header('content-type: text/html; charset=ISO-8859-1');
	
	$sql = "UPDATE demandas.demanda SET scsid = ".$_REQUEST['scsid']." WHERE dmdid = '".$_REQUEST['dmdidSS']."'";
	$db->executar($sql);
	
	
	//grava log de reclassificação
	$obsdsc = "";

	if($_REQUEST['scsid_old'] != $_REQUEST['scsid'])
		$obsdsc .= "<br>- Solicitação de Serviço (SS): [Nº ".$_REQUEST['scsid_old']."] para [Nº ".$_REQUEST['scsid']."]";
	
	//insere observação
	if($obsdsc){
		$obsdsc = "O usuário [{$_SESSION['usunome']}] alterou o seguinte campo da Aba Dados da Demanda:" . $obsdsc;
		$sql = "INSERT INTO demandas.observacoes (
					usucpf,
					dmdid,
					obsdsc,
					obsstatus,
					obslog,
					obsdata
				) VALUES (
					'".$_SESSION['usucpf']."', 
					'".$_REQUEST['dmdidSS']."',
					'".pg_escape_string($obsdsc)."',
					'A',
					'A',
					now()
				);";
		$db->executar($sql);
	}

	$db->commit();
	
	echo "OK";
	exit;
}



function updateDemanda(){
	global $db;
	
	($_POST['sisid'] == '' || ($_POST['ordid'] != 1 && $_POST['ordid'] != 11 && $_POST['ordid'] != 18 && $_POST['ordid'] != 19 && $_POST['ordid'] != 20 && $_POST['ordid'] != 21))? $sisid = "NULL" : $sisid = " '".$_POST['sisid']." '";
	($_POST['laaid'] == '') ? $laaid = "NULL" : $laaid = " '".$_POST['laaid']."'";
	
	($_POST['tipid'] == '') ? $tipid = "NULL" : $tipid = " '".$_POST['tipid']."'";
	($_POST['celid'] == '') ? $celid = "NULL" : $celid = " '".$_POST['celid']."'";
        
        
	($_POST['motid'] == '') ? $motid = "NULL" : $motid = " '".$_POST['motid']."'";
	($_POST['dmdnumdocrastreamento'] == '') ? $dmdnumdocrastreamento = "''" : $dmdnumdocrastreamento = " '".$_POST['dmdnumdocrastreamento']."'";
	($_POST['dmddatadocrastreamento'] == '') ? $dmddatadocrastreamento = "NULL" : $dmddatadocrastreamento = " '".$_POST['dmddatadocrastreamento']."'";
	($_POST['dmdunidadedocrastreamento'] == '') ? $dmdunidadedocrastreamento = "NULL" : $dmdunidadedocrastreamento = " '".$_POST['dmdunidadedocrastreamento']."'";
	($_POST['scsid'] == '') ? $scsid = "NULL" : $scsid = " '".$_POST['scsid']."'";
	
	
	($_POST['dmdqtde'] == '') ? $dmdqtde = 0 : $dmdqtde = $_POST['dmdqtde'];
	
	
	if($_POST['horarioA'] && $_POST['horarioN']){
		$horario = "T";	
	}elseif($_POST['horarioA'] && !$_POST['horarioN']){
		$horario = "A";
	}elseif(!$_POST['horarioA'] && $_POST['horarioN']){
		$horario = "N";
	}else{
		$horario = "C";
	}
	
	if(!$_POST['atendimentoRemoto']) $_POST['atendimentoRemoto'] = 'f';
	if(!$_POST['dmdatendurgente']) $_POST['dmdatendurgente'] = 'f';

	//$data_hora = date ("Y-m-d H:i:s");
	
	//1º classificação
	if(!$_POST['usucpfclassificador']){
		$campoClassificador = "usucpfclassificador = '".$_SESSION['usucpf']."', 
							   dmddataclassificacao = '".date('Y-m-d H:i:s')."',";
	}
	
	//2º classsificação para origem: serviço de impressão
	//ordid = 12 -> serviço de impressão
	//tipid = 532 -> 00 - Classificar Demanda (Encaminhado pelo Service Desk)
	if(!$_POST['usucpfclassificadorsi'] && 
		$_POST['ordid'] == 12 && 
		$_POST['tipid'] != 532 &&
		in_array(DEMANDA_PERFIL_EMPRESA_TYPE, arrayPerfil())){
		$campoClassificadorSi = "usucpfclassificadorsi = '".$_SESSION['usucpf']."', 
							   dmddataclassificacaosi = '".date('Y-m-d H:i:s')."',";
	}


	//verifica saldo material
	//$saldoMaterial = verificaSaldoMaterial();
	if($_POST['tpmid']){
		$sql = "select
					c.tpmid, 
					(coalesce(totalentrada,0) - coalesce(totalsaida,0))  + coalesce(totaldemanda,0) as saldo
				from demandas.controlematerial c
				left join (select sum(ctmqtd) as totalentrada, tpmid from demandas.controlematerial where tpcid = 1 and ctmstatus = 'A' group by tpmid) e ON e.tpmid = c.tpmid
				left join (select sum(ctmqtd) as totalsaida, tpmid from demandas.controlematerial where tpcid = 2 and ctmstatus = 'A' group by tpmid) s ON s.tpmid = c.tpmid
				left join (select sum(ctmqtd) as totaldemanda, tpmid from demandas.controlematerial where tpcid = 2 and ctmstatus = 'A' and tpmid=".$_POST['tpmid']." and dmdid=".$_POST['dmdid']." group by tpmid) d ON d.tpmid = c.tpmid
				where c.ctmstatus='A' 
				and c.tpmid=".$_POST['tpmid']."
				group by c.tpmid, totalentrada, totalsaida, totaldemanda";
		$saldo = $db->pegaLinha($sql);
		
		if($dmdqtde > $saldo['saldo']){
			die("<script>
				alert('Saldo Indisponível do Material Solicitado!');
				location.href='demandas.php?modulo=principal/dadosDemanda&acao=A';
				//history.go(-1);
			 </script>");	
		}
	}
	//fim verifica saldo material
	
	
	//controle de material
	$sql = " UPDATE demandas.controlematerial SET 
			 ctmstatus = 'I'
			 WHERE dmdid = ".$_POST['dmdid'] ;
	$db->executar($sql); 

  	$blocoMaterial = ", dmdmaterial = NULL";
	$blocoMaterial .= ", dmdmaterialentregue = NULL";
	$blocoMaterial .= ", dmdmaterialdevolvido = NULL";
	
	if($_POST['dmdmaterial'] == 't'){
		
		$blocoMaterial = ", dmdmaterial = '".$_POST['dmdmaterial']."'";
		if($_POST['dmdmaterialentregue']){
			$blocoMaterial .= ", dmdmaterialentregue = '".$_POST['dmdmaterialentregue']."'";
		}
		if($_POST['dmdmaterialdevolvido']){
			$blocoMaterial .= ", dmdmaterialdevolvido = '".$_POST['dmdmaterialdevolvido']."'";
		}
		
		
  		$sql = sprintf("INSERT INTO demandas.controlematerial
				(
					tpcid,
					tpmid,
					ctmqtd,
					ctmstatus,
					usucpf,
					ctmdatahora,
					dmdid		
				)VALUES(
					%s,
					%s,
					%s,
					'%s',
					'%s',
					'%s',
					%s
				)",
				2,
				$_POST['tpmid'],
				$dmdqtde,
				'A',
				$_SESSION['usucpf'],
				date('Y-m-d H:i:s'),
				$_POST['dmdid']
				);
 	 	$db->executar($sql); 	 

	}
	//fim controle de material
	
	
	//atualiza a demanda
	$sql = "UPDATE demandas.demanda SET
						
						dmdtitulo = '".simec_addslashes($_POST['dmdtitulo'])."',
						sidid = $sisid,
						tipid = $tipid,
						dmddsc = '".simec_addslashes($_POST['dmddsc'])."',
						dmdreproducao = '".simec_addslashes($_POST['dmdreproducao'])."',
						dmdsalaatendimento = '".$_POST['dmdsalaatendimento']."',
						--priid = '".$_POST['priid']."',
						laaid = $laaid,
						unaid = ".$_POST['unaid'].",
						motid = $motid,
						dmdqtde = $dmdqtde,
						dmdnumdocrastreamento = $dmdnumdocrastreamento,
						dmddatadocrastreamento = $dmddatadocrastreamento,
						dmdunidadedocrastreamento = $dmdunidadedocrastreamento,
						scsid = $scsid,
                        celid = $celid,
						dmdhorarioatendimento = '".$horario."',
						$campoClassificador
						$campoClassificadorSi
						dmdatendremoto = '".$_POST['atendimentoRemoto']."',
						dmdatendurgente = '".$_POST['dmdatendurgente']."',
						dmdjusturgente = '".$_POST['dmdjusturgente']."',
						dmdjudicial = '".$_POST['dmdjudicial']."'
						$blocoMaterial
						--,						dmddatainiatendefetivo = '".$data_hora."'
							
						WHERE dmdid = '".$_POST['dmdid']."'
						";
	//dbg($sql,1);
	$db->executar($sql);
	
	//controla a classificação da demanda
	$sql = " DELETE FROM demandas.controledemanda where usucpf = '".$_SESSION['usucpf']."'";
	$db->executar($sql);

 	 //Atualiza dados cadastrais do usuário
 	 if($_POST['usucpfdemandante']){
 	 	$usucpfdemandante = $_POST['usucpfdemandante'];
 	 	$sql = "select usdid from demandas.usuariodetalhes where usucpf = '$usucpfdemandante'";
 	 	$usdid = $db->pegaUm($sql);
 	 	
 	 	$usdcelular = str_replace("-","",$_POST['usdcelular']);
 	 	
 	 	if(!$usdid){
 	 		$sql = sprintf("INSERT INTO demandas.usuariodetalhes
					(
						usucpf,
						usdgabinete,
						usdramal,
						usdcelular,
						usdnomecomputador		
					)VALUES(
						'%s',
						%s,
						'%s',
						'%s',
						'%s'
					)",
					$usucpfdemandante,
					($_POST['usdgabinete'] ? $_POST['usdgabinete'] : 'FALSE'),
					$_POST['usdramal'],
					$usdcelular,
					$_POST['usdnomecomputador']
					);
 	 	}
 	 	else{
			$sql = " UPDATE demandas.usuariodetalhes SET 
						usdgabinete = '".$_POST['usdgabinete']."',
						usdramal = '".$_POST['usdramal']."',
						usdcelular = '".$usdcelular."',
						usdnomecomputador = '".$_POST['usdnomecomputador']."'
					 WHERE usdid = {$usdid}
					 and usucpf = '$usucpfdemandante'";
 	 		
 	 	}
 	 	$db->executar($sql); 	 		
			
 	 	
	}
	

	$db->commit();
	
	
	//controle de material
	if($_POST['tpmid']){
		//envia email para o GESTOR MEC
 	 	enviaEmailMaterial();
	}
	

}





function montaCelula($ordid){
	global $db;
 	
	if($ordid == 1 || $ordid == 18 || $ordid == 19 || $ordid == 20 || $ordid == 21){
		
		$ordidx1 = 1;
		
		$sql = "SELECT
				 c.celid AS codigo,
				 c.celnome || ' - Gerente: ' || 
				 CASE 
				 	 WHEN u.usunome != '' THEN u.usunome
					 ELSE ' - '
				 END as DESCRICAO
				FROM
				 demandas.celula c
	 			 LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = c.celid AND ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO." AND ur.rpustatus = 'A' 
	 			 LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf 
				WHERE
				 c.ordid = ".$ordidx1." AND
				 c.celstatus = 'A'
				ORDER BY
				 c.celnome;";
		$db->monta_combo( 'celid', $sql, 'S', '-- Informe a célula --','filtraSistema', '' );
		echo obrigatorio();
	}
	else if($ordid == 2 || $ordid == 13 || $ordid == 14 || $ordid == 23 || $ordid == 24){
		$sql = "SELECT
				 c.celid AS codigo,
				 c.celnome as DESCRICAO
				FROM
				 demandas.celula c
				WHERE
				 c.ordid = ".$ordid." AND
				 c.celstatus = 'A'
				ORDER BY
				 c.celnome;";
		$db->monta_combo( 'celid', $sql, 'S', '-- Informe a célula --','filtraTipoRede', '' );
		echo obrigatorio();
	}
	
}


function montaSistema($celid){
	global $db;
 
	$sql = sprintf("select  
						s.sidid  AS codigo, 
						upper(s.sidabrev) || ' - ' || s.siddescricao AS descricao 
					from 
						demandas.sistemadetalhe s
					left join demandas.sistemacelula c on s.sidid = c.sidid  
					where  s.sidstatus = 'A'
					AND celid = %d
					order by s.sidabrev", 
			$celid);
	$db->monta_combo( 'sisid', $sql, 'S', '-- Informe o Sistema --', '', '' );
	echo obrigatorio();
	//listar_municipios 
}


function montaTipoRede($celid){
	global $db;
 
	$sql = sprintf("SELECT 
					 tipid AS codigo,
					 tipnome AS descricao 
					FROM 
					 demandas.tiposervico
					WHERE
					 celid = %d AND
					 tipstatus = 'A' 
					ORDER BY 
					 tipnome", 
			$celid);
	$db->monta_combo( 'tipid', $sql, 'S', '-- Informe o tipo de demanda --', 'filtraQtd', '','','','','tipid2','');
	echo obrigatorio();
}



function montaTipoDemanda($ordid){
	global $db;
 
	$sql = sprintf("SELECT 
					 tipid AS codigo,
					 tipnome AS descricao
					FROM 
					 demandas.tiposervico
					WHERE
					 ordid = %d AND
					 tipstatus = 'A' 
					ORDER BY 
					 tipnome", 
			$ordid);
	$db->monta_combo( 'tipid', $sql, 'S', '-- Informe o tipo de demanda --', 'filtraQtd', '','','','','tipid2','');
	echo obrigatorio(); 
}


function montaQtdDemanda($tipid){
	global $db;
 
	$sql = sprintf("SELECT 
					 tipdescricao, tipquantidade 
					FROM 
					 demandas.tiposervico
					WHERE
					 tipid = %d AND
					 tipstatus = 'A' 
					", 
			$tipid);
	$tipos = $db->PegaLinha($sql);
	
	$dsc = str_replace(chr(13), "<br>", $tipos['tipdescricao']);
	
	if($tipos['tipquantidade'] == 't'){
		echo $dsc .'|'. campo_texto( 'dmdqtde', 'S', 'S', '', 10, 8, '########', '','','','','','','1');
	}
	else echo $dsc ."|N";
	 
}


function montaAndarDemanda($lcaid){
	global $db;
 
	$sql = sprintf("SELECT 
					 laa.laaid AS codigo,
					 aa.anddescricao AS descricao
					 --laa.laaid, laa.lcaid, aa.andid, aa.anddescricao 
					FROM 
					 	demandas.localandaratendimento laa
						inner join demandas.andaratendimento aa on laa.andid = aa.andid
					WHERE
					 laa.lcaid = %d 
					ORDER BY 
					 aa.anddescricao", 
			$lcaid);
	$dados = $db->carregar($sql);
	//echo 'ok='.count($dados).'<br>';
	if(count($dados)==1) $laaid = $dados[0]['codigo'];
	//echo $laaid;
	$db->monta_combo( 'laaid', $sql, 'S', '-- Informe o Andar --', '', '', '', '', '', '', '', $laaid );
	//echo obrigatorio();
}


function verificaClassificacao($dmdidx){
	global $db;
 
	$sql = sprintf("SELECT 
					 tipid
					FROM 
					 	demandas.demanda
					WHERE
					 dmdid = %d 
					", 
			$dmdidx);
	
	$tipidx = $db->PegaUm($sql);
	
	echo $tipidx;
	exit;
}



function iniciadatademanda(){
	global $db;
 
	$data_hora = date ("Y-m-d H:i:s");
	$sql = " UPDATE demandas.demanda SET dmddatainiatendefetivo='$data_hora' where dmdid=".$_SESSION['dmdid'];
	$db->executar($sql);
	$db->commit();

	echo "OK";
	exit;
}



if ($_REQUEST['ordidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaTipoDemanda($_POST['ordidAjax']);
	exit;
}

if ($_REQUEST['ordidAjax2']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaCelula($_POST['ordidAjax2']);
	exit;
}


if ($_REQUEST['celtiporedeAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaTipoRede($_POST['celtiporedeAjax']);
	exit;
}


//filtra tipo para mostrar qtd
if ($_REQUEST['tipidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaQtdDemanda($_POST['tipidAjax']);
	exit;
}

//filtra o andar
if ($_REQUEST['lcaidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaAndarDemanda($_POST['lcaidAjax']);
	exit;
}

//filtra o andar
if ($_REQUEST['celidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaSistema($_POST['celidAjax']);
	exit;
}

//verifica classificacao da demanda
if ($_REQUEST['dmdidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	verificaClassificacao($_POST['dmdidAjax']);
	exit;
}

//inicia data demanda
if ($_REQUEST['dtinicioAjax'] == '1') {
	header('content-type: text/html; charset=ISO-8859-1');
	iniciadatademanda();
	exit;
}



if($_POST['dmdid']){
	
	updateDemanda();
	
	//grava log de reclassificação
	$obsdsc = "";

		//Assunto
		if($_POST['dmdtitulo_old'] != $_POST['dmdtitulo'])
			$obsdsc .= "<br>- Assunto: [".$_POST['dmdtitulo_old']."] para [".$_POST['dmdtitulo']."]";
	
		//Origem da Demanda
		if($_POST['ordid']){
			$sql = "SELECT orddescricao FROM demandas.origemdemanda WHERE ordid = ".$_POST['ordid'];
			$ordnome = $db->PegaUm($sql);
		}	
		if($_POST['ordid_old']){
			$sql = "SELECT orddescricao FROM demandas.origemdemanda WHERE ordid = ".$_POST['ordid_old'];
			$ordnome_old = $db->PegaUm($sql);
		}
		
		if($ordnome_old != $ordnome)
			$obsdsc .= "<br>- Origem da Demanda: [".$ordnome_old."] para [".$ordnome."]";
	
		//Célula
		if($_POST['celid_old']){
			$sql = "SELECT celnome FROM demandas.celula WHERE celid = ".$_POST['celid_old'];
			$celnome_old = $db->PegaUm($sql);
		}
		if($_POST['celid']){
			$sql = "SELECT celnome FROM demandas.celula WHERE celid = ".$_POST['celid'];
			$celnome = $db->PegaUm($sql);
			
			if($celnome_old != $celnome)
				$obsdsc .= "<br>- Célula: [".$celnome_old."] para [".$celnome."]";
		}	

		//Sistema
		if($_POST['sisid_old']){
			$sql = "SELECT sidabrev || ' - ' || siddescricao FROM demandas.sistemadetalhe WHERE sidid = ".$_POST['sisid_old'];
			$sisnome_old = $db->PegaUm($sql);
		}
		if($_POST['sisid']){
			$sql = "SELECT sidabrev || ' - ' || siddescricao FROM demandas.sistemadetalhe WHERE sidid = ".$_POST['sisid'];
			$sisnome = $db->PegaUm($sql);
			
			if($sisnome_old != $sisnome_old)
				$obsdsc .= "<br>- Sistema: [".$sisnome_old."] para [".$sisnome."]";
		}	
		
		//Tipo de Demanda
		if($_POST['tipid']){
			$sql = "SELECT tipnome FROM demandas.tiposervico WHERE tipid = ".$_POST['tipid'];
			$tipnome = $db->PegaUm($sql);
		}	
		if($_POST['tipid_old']){
			$sql = "SELECT tipnome FROM demandas.tiposervico WHERE tipid = ".$_POST['tipid_old'];
			$tipnome_old = $db->PegaUm($sql);
		}
		
		if($tipnome_old != $tipnome)
			$obsdsc .= "<br>- Tipo de Demanda: [".$tipnome_old."] para [".$tipnome."]";
		
		//Quantidade do serviço
		if($_POST['dmdqtde']){
			
			if($_POST['dmdqtde_old'] != $_POST['dmdqtde'])
				$obsdsc .= "<br>- Quantidade do serviço: [".$_POST['dmdqtde_old']."] para [".$_POST['dmdqtde']."]";
		}	
		
		//Descrição detalhada
		if($_POST['dmddsc_old'] != $_POST['dmddsc'])
			$obsdsc .= "<br>- Descrição detalhada: [".$_POST['dmddsc_old']."] para [".$_POST['dmddsc']."]";

		//Reprodução do problema
		if($_POST['dmdreproducao_old'] != $_POST['dmdreproducao'])
			$obsdsc .= "<br>- Reprodução do problema: [".$_POST['dmdreproducao_old']."] para [".$_POST['dmdreproducao']."]";
			
	
	
	//envia email
	if( in_array($esdid, array(DEMANDA_ESTADO_EM_ANALISE,DEMANDA_GENERICO_ESTADO_EM_ANALISE,DEMANDA_ESTADO_EM_ATENDIMENTO,DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO)) ){
		enviaEmailAltera();
		
		//verifica mudança de tipo da demanda (tipid) e quantidade - para calculo de tempo data prevista inicio e termino
			if( ($tipnome_old != $tipnome) || ($_POST['dmdqtde'] && $_POST['dmdqtde_old'] != $_POST['dmdqtde']) ){
					
				//pega prioridade atual
				$sql = "select priid, dmdtempoadicional, 
							   to_char(dmddatainiprevatendimento, 'DD/MM/YYYY HH24:MI:SS') as dmddatainiprevatendimento,
							   to_char(dmddatafimprevatendimento, 'DD/MM/YYYY HH24:MI') as dmddatafimprevatendimento 
						from demandas.demanda where dmdid = {$_POST['dmdid']}";
				$dadosDemanda = $db->pegaLinha($sql);
				$prioridadeAtual = $dadosDemanda['priid'];
				$dmddatainiprevatendimento = $dadosDemanda['dmddatainiprevatendimento'];
				$dmddatafimprevatendimento = $dadosDemanda['dmddatafimprevatendimento'];
				$dmdtempoadicional = $dadosDemanda['dmdtempoadicional'];
				 
				if($_POST['ordid'] == '3'){ //suporte de atendimento
					$andPrioridade = "and t.tsppontuacao <> 0";	
				}
				else{
					//$andPrioridade = "and t.tsptempo is not null";	
					$andPrioridade = "and (t.tsphora is not null or t.tspminuto is not null)";
				}
				
				//verifica se existe prioridade no catalogo
				$sql = "select distinct p.priid 
								from demandas.demanda d
								inner join demandas.tiposervicoprioridade AS t on t.tipid = d.tipid
								inner join demandas.prioridade AS p ON t.priid = p.priid 
								where d.dmdid = {$_POST['dmdid']}
								and p.pristatus = 'A'
								$andPrioridade
								order by p.priid";
				$prioridade = $db->carregarColuna($sql);
				
				if($prioridadeAtual){
					if( !in_array($prioridadeAtual, $prioridade) ){
						$sql = "select distinct p.priid 
								from demandas.demanda d
								inner join demandas.tiposervicoprioridade AS t on t.tipid = d.tipid
								inner join demandas.prioridade AS p ON t.priid = p.priid 
								where d.dmdid = {$_POST['dmdid']}
								and p.pristatus = 'A'
								$andPrioridade
								order by p.priid limit 1";
						$prioridadeAtual = $db->pegaUm($sql);
					}
					
					
				}
				
				//calcula prioridade ou quantidade
				$retorno = verificaCalculoTempoPrioridade($prioridadeAtual,$dmddatainiprevatendimento,$dmdtempoadicional);
				
				if($retorno){
					$nretorno = explode("|", $retorno);
					$dtfim = formata_data_sql($nretorno[2]) .' '. $nretorno[3];
					$dtfimObs = $nretorno[2] .' '. $nretorno[3];
				}
				
				//atualiza a prioridade e data prevista termino
				if($prioridadeAtual && $dtfim){
						
					//atualiza data fim
					$sql = "UPDATE demandas.demanda SET 
									priid = {$prioridadeAtual},
									dmddatafimprevatendimento = '".$dtfim."' 
							WHERE dmdid = ".$_POST['dmdid'];
					$db->executar($sql);
					$db->commit();
					
					//insere observação para data fim
					if($dmddatafimprevatendimento != $dtfimObs){
						$obsdsc .= "<br><br>Devido a alteração da Origem ou Quantidade da Demanda, o sistema Demandas atualizou o seguinte campo da tela de atendimendo: <br>- Previsão de término do atendimento: [$dmddatafimprevatendimento] para [$dtfimObs]";
					}
					
				}
				
								
			}
			
		//fim verifica	
		
	}	
	

	//insere observação
	if($obsdsc){
		$obsdsc = str_replace("'", "", $obsdsc);
		$obsdsc = "O usuário [{$_SESSION['usunome']}] alterou os seguintes campos da Classificação da Demanda:" . $obsdsc;
		$sql = "INSERT INTO demandas.observacoes (
					usucpf,
					dmdid,
					obsdsc,
					obsstatus,
					obslog,
					obsdata
				) VALUES (
					'".$_SESSION['usucpf']."', 
					'".$dmdid."',
					'".pg_escape_string($obsdsc)."',
					'A',
					'A',
					now()
				);";
		$db->executar($sql);
		$db->commit();
	}	
	
	
	
	print "<script>
				alert('Operação realizada com sucesso!');
				location.href='demandas.php?modulo=principal/dadosDemanda&acao=A';
				//history.go(-1);
			 </script>";
	exit;
}


include_once APPRAIZ . 'includes/workflow.php';
/*
 * Recupera o "docid"
 * Caso não exista "docid" cadastrado na demanda 
 * Insere o documento e vincula na demanda
 */
$docid = criarDocumento( $dmdid );



//INICIO controla a classificação da demanda
//Caso o técnico esteja classificando a demanda, exibir mensagem para os demais técnicos e bloquear classificação
//Pega array com os perfis do usuário que podem acessar a demanda.
$liberaControleDem = true;
$perfilUsu = arrayPerfil();
if(in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfilUsu) || in_array(DEMANDA_PERFIL_ADMINISTRADOR,$perfilUsu) || in_array(DEMANDA_PERFIL_GESTOR_MEC,$perfilUsu) ){
	$liberaControleDem = false;
}

if($liberaControleDem){		

	if( in_array(DEMANDA_PERFIL_TECNICO1,$perfilUsu) ){		
		
		if(!$docid || in_array($esdid, array(DEMANDA_ESTADO_EM_ANALISE,DEMANDA_GENERICO_ESTADO_EM_ANALISE)) ){
			$sql = "select c.codid, u.usunome, c.usucpf 
					from demandas.controledemanda c
					inner join seguranca.usuario u on u.usucpf = c.usucpf
					where c.dmdid = $dmdid ";
		  	$dados = $db->pegaLinha($sql);
		  	
		  	if(!$dados['codid']){
		  		$sql = sprintf("INSERT INTO demandas.controledemanda
						(
							usucpf,
							dmdid		
						)VALUES(
							'%s',
							%s
						)",
						$_SESSION['usucpf'],
						$dmdid
						);
				$db->executar($sql);
				$db->commit();
		  	}
		  	else{
		  		if($dados['usucpf'] <> $_SESSION['usucpf']){
			  		?>
			  			<script>
			  				if(!confirm("O técnico: <?=$dados['usunome']?>,\njá está classificando esta demanda.\n\nDeseja ver os detalhes desta demanda?")){
			  					history.back();
			  				}
			  			</script>
			  		<? 
			  		$msgControle = "<center><font color=red><b>O técnico: ".$dados['usunome'].",<br>já está classificando esta demanda.</b></font></center>";
			  		$habil = 'N';
		  		}
		  	}
		  	 
		}
		
	}
	
}
//FIM controla a classificação da demanda



$sql = "SELECT d.dmdid,
	d.usucpfdemandante,
	d.dmdtitulo,
	o.ordid,
	o.orddescricao,
	t.tipid,
	s.sidid,
	s.siddescricao,
	t.tipnome,
	d.dmddsc,
	d.dmdreproducao,
	--p.priid,
	d.dmdsalaatendimento,
	d.laaid,
	--p.pridsc,
	l.lcaid,
	d.unaid,
	d.motid,
	d.dmdnumdocrastreamento,
	d.dmddatadocrastreamento,
	d.dmdunidadedocrastreamento,
	d.scsid,
	d.celid,
	d.dmdqtde,
	t.tipquantidade,
	t.tipnumserie,
	t.tipnumpatr,
	t.tipdescricao,
	d.dmdhorarioatendimento,
	d.dmdatendremoto,
	d.usucpfexecutor,
	d.dmddatainiatendefetivo,
	d.usucpfclassificador,
	d.usucpfclassificadorsi,
	d.dmdatendurgente,
	d.dmdjusturgente,
	u.usdcelular,
	u.usdramal,
	u.usdgabinete,
	u.usdnomecomputador,
	d.dmdmaterial,
	d.dmdmaterialentregue,
	d.dmdmaterialdevolvido,
	cm.tpmid,
	d.dmdjudicial
	FROM demandas.demanda AS d

--LEFT JOIN demandas.prioridade AS p ON p.priid = d.priid 
LEFT JOIN demandas.tiposervico AS t ON t.tipid = d.tipid 
LEFT JOIN demandas.origemdemanda AS o ON o.ordid = t.ordid 
LEFT JOIN demandas.sistemadetalhe AS s ON s.sidid = d.sidid 
--LEFT JOIN demandas.sistemacelula AS c ON c.sidid = d.sidid 
LEFT JOIN demandas.celula AS cel ON cel.celid = d.celid
LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
LEFT JOIN demandas.usuariodetalhes AS u ON u.usucpf = d.usucpfdemandante
LEFT JOIN demandas.controlematerial AS cm ON cm.dmdid = d.dmdid and cm.ctmstatus='A'
WHERE d.dmdid = ".$_SESSION['dmdid']."";

//dbg($sql,1);
$dados = $db->carregar($sql);

$usucpfexecutor = $dados[0]['usucpfexecutor'];
$dmddatainiatendefetivo = $dados[0]['dmddatainiatendefetivo'];


$display = 'none';
$displayCel = 'none';
if($dados[0]['ordid'] == 1 || $dados[0]['ordid'] == 18 || $dados[0]['ordid'] == 19 || $dados[0]['ordid'] == 20 || $dados[0]['ordid'] == 21){
	$display = '';
	$displayCel = '';
}else if($dados[0]['ordid'] == 2 || $dados[0]['ordid'] == 13 || $dados[0]['ordid'] == 14 || $dados[0]['ordid'] == 23 || $dados[0]['ordid'] == 24){
	$display = 'none';
	$displayCel = '';
}else if($dados[0]['ordid'] == 11){
	$display = '';
	$displayCel = 'none';
}
($dados[0]['tipquantidade'] == 't')? $displayQtd = '' : $displayQtd = 'none';

($dados[0]['tipnumserie'] == 't' || $dados[0]['tipnumpatr'] == 't')? $displayNserie = '' : $displayNserie = 'none';

($dados[0]['dmdmaterial'] == 't')? $displayMaterial = '' : $displayMaterial = 'none';

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

$db->cria_aba( $abacod_tela, $url, '');

monta_titulo( 'Dados da Demanda - Cód. # '.$dados[0]['dmdid'], '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<?=cabecalhoDemanda(); ?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/calendario.js"></script>
<script src="../fabrica/js/autocomplete/jquery-1.7.2.js"></script>
<script type="text/javascript">
jQuery.noConflict();
jQuery(document).ready(function(){
<?php 
    $estadoAtualDaDemanda =  wf_pegarEstadoAtual($docid);
    if( in_array($estadoAtualDaDemanda['esdid'], array(
    DEMANDA_ESTADO_EM_ANALISE
    ,DEMANDA_ESTADO_EM_ATENDIMENTO
    ,DEMANDA_GENERICO_ESTADO_EM_ANALISE	       				
    ,DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO			   				
    ) ) ) { ?>
        jQuery('a[title^="Avaliação da demanda"]').removeAttr("href").css('color', '#B5B5B5').css('text-decoration', 'none');
<?php } ?>
});
</script>
<form id="formulario" name="formulario" action="" method="post">
<input type="hidden" name="dmdid" value="<? echo $dados[0]['dmdid']; ?>"  />
<input type="hidden" name="usucpfdemandante" value="<? echo $dados[0]['usucpfdemandante']; ?>"  />
<input type="hidden" name="usucpfclassificador" value="<? echo $dados[0]['usucpfclassificador']; ?>"  />
<input type="hidden" name="usucpfclassificadorsi" value="<? echo $dados[0]['usucpfclassificadorsi']; ?>"  />


<?php if($displayQtd){?>
	<input type="hidden" name="verificaqtd" id="verificaqtd" value="">
<?php }else{?>
	<input type="hidden" name="verificaqtd" id="verificaqtd" value="1">
<?php }?>	

<?php if($msgControle){?>
	<table id="tblformulario" class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
		<tr>
			<td class="SubTituloDireita">
				<?=$msgControle?>
			</td>
		</tr>
	</table>
<?php }?>

<table id="tblformulario" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="20%"  align='right' class="SubTituloDireita">Assunto:</td>
		<td>
				<?= campo_texto( 'dmdtitulo', 'N', $habil, '', 80, 250, '', '','','','','','',$dados[0]['dmdtitulo']); echo obrigatorio();?>
				<input type="hidden" name="dmdtitulo_old" value="<?=$dados[0]['dmdtitulo']?>">
		</td>
		<td rowspan="11" style="background-color:#fafafa; color:#404040; width: 1px;" valign="top" align="center">
			<?php 
				if($mostraWorkflow == 'S'){
					// Barra de estado atual e ações e Historico
					wf_desenhaBarraNavegacao( $docid , array( 'unicod' => '' ) );	
					//Barra outras opções
					criarNovaDemanda(); //www/demandas/_funcoes.php
				}else{
					?>
						<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>estado atual</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<?php 
												if($esdid){
													$sqlesd = "SELECT esddsc
															FROM workflow.estadodocumento
															WHERE esdid = $esdid";
													
													echo $db->pegaUm($sqlesd);
												}
												else{
													echo "Em Processamento";
												}
												?>					
											</span>
										</td>
									</tr>
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>ações</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												você não possui permissão
											</span>
										</td>
									</tr>
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>histórico</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<img style="cursor: pointer;" src="http://<?=$_SERVER['SERVER_NAME']?>/imagens/fluxodoc.gif"	 onclick="exibeHistoricoDem( '<?=$docid?>' );">
										</td>
									</tr>
									
													
						</table>					
					<? 
				}
			?>
			
		</td>		
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Origem da Demanda:</td>
		<td>
		<?php
		$sql = "SELECT
		 ordid AS codigo,
		 orddescricao AS descricao
		FROM 
		 demandas.origemdemanda  
		WHERE ordstatus = 'A'
		ORDER BY 
		 orddescricao";
		$db->monta_combo( 'ordid', $sql, $habil, '-- Informe a origem da demanda --', 'tipo_demanda(this.value);exibe_sistema', '','','','','ordid','',$dados[0]['ordid']);
		 echo obrigatorio();?>
		 <input type="hidden" name="ordid_old" value="<?=$dados[0]['ordid']?>">
		 </td>
	</tr>
	<tr id="tr_celula" style="display:<? echo $displayCel;  ?>">
		<td align='right' class="SubTituloDireita" >
		Célula:
		</td>
		<td id="td_celula">
		<?php
			if($dados[0]['ordid'] == 1 || $dados[0]['ordid'] == 18 || $dados[0]['ordid'] == 19 || $dados[0]['ordid'] == 20 || $dados[0]['ordid'] == 21){
				$sql = "SELECT
						 c.celid AS codigo,
						 c.celnome || ' - Gerente: ' || 
						 CASE 
						 	 WHEN u.usunome != '' THEN u.usunome
							 ELSE ' - '
						 END as DESCRICAO
						FROM
						 demandas.celula c
			 			 LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = c.celid AND ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO." AND ur.rpustatus = 'A' 
			 			 LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf 
						WHERE
						 --c.ordid = ".$dados[0]['ordid']." AND
						 c.ordid = 1 AND
						 c.celstatus = 'A'
						ORDER BY
						 c.celnome;";
				//dbg($sql);
				$db->monta_combo( 'celid', $sql, $habil, '-- Informe a célula --', 'filtraSistema', '','','','','celid','',$dados[0]['celid']);
			}
			else if($dados[0]['ordid'] == 2 || $dados[0]['ordid'] == 13 || $dados[0]['ordid'] == 14 || $dados[0]['ordid'] == 23 || $dados[0]['ordid'] == 24){
				$sql = "SELECT
						 c.celid AS codigo,
						 c.celnome as DESCRICAO
						FROM
						 demandas.celula c
						WHERE
						 c.ordid = ".$dados[0]['ordid']." AND
						 c.celstatus = 'A'
						ORDER BY
						 c.celnome;";
				$db->monta_combo( 'celid', $sql, $habil, '-- Informe a célula --', 'filtraTipoRede', '','','','','celid','',$dados[0]['celid']);
			}
			
			echo obrigatorio(); ?>
			<input type="hidden" name="celid_old" value="<?=$dados[0]['celid']?>">
		 </td>
	</tr>
	<tr id="tr_sistema" style="display:<? echo $display;  ?>">
		<td  align='right' class="SubTituloDireita">
		Sistema:
		</td>
		<td id='listasistema'>
		<?php
		//$dados[0]['celid']
		/*
		$sql = "SELECT
		 sidid AS codigo,
		 upper(sidabrev) ||' - '|| siddescricao AS descricao
		FROM 
		 demandas.sistemadetalhe
		WHERE
		 sidstatus = 'A' 
		ORDER BY 
		 siddescricao";
		*/
		$sql = sprintf("select  
						s.sidid  AS codigo, 
						upper(s.sidabrev) || ' - ' || s.siddescricao AS descricao 
					from 
						demandas.sistemadetalhe s
					left join demandas.sistemacelula c on s.sidid = c.sidid  
					where  s.sidstatus = 'A'
					AND celid = %d
					order by s.sidabrev", 
			$dados[0]['celid']);		
		$db->monta_combo( 'sisid', $sql, $habil, '-- Informe o sistema --', '', '','','','','sisid','',$dados[0]['sidid']);
		 echo obrigatorio(); ?>
		 <input type="hidden" name="sisid_old" value="<?=$dados[0]['sidid']?>">
		 </td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Tipo da Demanda:</td>
		<td>
			<div id="novadmd" style="float:left">
			</div>
			<div id="antigadmd" style="float:left">
			<?php
				if($dados[0]['ordid'] == '2' || $dados[0]['ordid'] == '13' || $dados[0]['ordid'] == '14' || $dados[0]['ordid'] == '23' || $dados[0]['ordid'] == '24'){ // 2=REDES - 13=Gestão Documentos CGI
					$sql = sprintf("SELECT 
							 tipid AS codigo,
							 tipnome AS descricao
							FROM 
							 demandas.tiposervico
							WHERE
							 celid = %d 
							 $andTipid 
							ORDER BY 
							 tipnome", 
					$dados[0]['celid']);
				}
				else{
					$sql = sprintf("SELECT 
							 tipid AS codigo,
							 tipnome AS descricao
							FROM 
							 demandas.tiposervico
							WHERE
							 ordid = %d 
							 $andTipid 
							ORDER BY 
							 tipnome", 
					$dados[0]['ordid']);
				}
				
				$db->monta_combo( 'tipid', $sql, $habil, '-- Informe o tipo de demanda --', 'filtraQtd', '','','','','tipid1','',$dados[0]['tipid']);
				echo obrigatorio();?>
				
			   <input type="hidden" name="tipid_old" value="<?=$dados[0]['tipid']?>">
			   
			</div>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Descrição Tipo da Demanda:</td>
		<td id='dscTipoDemanda'>
			<?
			$tipdescricao = str_replace(chr(13), "<br>", $dados[0]['tipdescricao']);
			echo $tipdescricao;
			?>
		</td>
	</tr>	
	<tr id="tr_qtd" style="display:<? echo $displayQtd;  ?>;">
		<td align='right' class="SubTituloDireita">Informe a <b>quantidade do serviço</B>:</td>
		<td id='qtddemanda'>
			<?$dmdqtde = $dados[0]['dmdqtde'];?>
			<?=campo_texto( 'dmdqtde', 'S', $habil, '', 10, 8, '########', ''); ?>
			<input type="hidden" name="dmdqtde_old" value="<?=$dados[0]['dmdqtde']?>">
		</td>
	</tr>	
	<?php if(in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfilUsu) || in_array(DEMANDA_PERFIL_DEPOSITO_DTI,$perfilUsu) || in_array(DEMANDA_PERFIL_GESTOR_MEC,$perfilUsu) || in_array(DEMANDA_PERFIL_ADMINISTRADOR,$perfilUsu) || in_array(DEMANDA_PERFIL_TECNICO1,$perfilUsu) || in_array(DEMANDA_PERFIL_TECNICO,$perfilUsu) ){?>
	<tr>
		<td align='right' class="SubTituloDireita">Necessita de Peças e/ou Acessórios:</td>
		<td >
			<? //$dmdatendremoto = $dados[0]['dmdatendremoto']; ?>
			<input type="radio" name="dmdmaterial" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> value="t" <?if($dados[0]['dmdmaterial'] == 't' ) echo "checked";?> onclick="checaMaterial();"> Sim
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" name="dmdmaterial" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> value="f" <?if($dados[0]['dmdmaterial'] == 'f' || $dados[0]['dmdmaterial'] == '') echo "checked";?> onclick="checaMaterial();"> Não
		</td>
	</tr>	
	<tr id="tr_material" style="display:<? echo $displayMaterial;  ?>;">
		<td align='right' class="SubTituloDireita">Material:</td>
		<td >
			<select style="width: 200px;" class="CampoEstilo" name="tpmid" <?= $habil == 'N' ? 'disabled="disabled"' : ''?>>
				<option value=''>Selecione...</option>
					<?
					
					 if($dados[0]['ordid']) $andMat = " and m.ordid = ".$dados[0]['ordid'];
					 //$sql = "select mtrid as codigo, mtrdsc as descricao from demandas.material where mtrstatus='A' $andMat order by 2";
				 	$sql = "select
							m.mtrid as codigo, 
							m.mtrdsc as descricao
						from demandas.material m
						left join demandas.tipomaterial t ON t.mtrid = m.mtrid
						left join (select sum(ctmqtd) as totalentrada, tpmid from demandas.controlematerial where tpcid = 1 and ctmstatus = 'A' group by tpmid) e ON e.tpmid = t.tpmid
						left join (select sum(ctmqtd) as totalsaida, tpmid from demandas.controlematerial where tpcid = 2 and ctmstatus = 'A' group by tpmid) s ON s.tpmid = t.tpmid
						where m.mtrstatus ='A' 
						and t.tpmstatus='A' 
						$andMat
						group by m.mtrid, m.mtrdsc, totalentrada, totalsaida
						having ( coalesce(totalentrada,0) - coalesce(totalsaida,0) ) > 0";
				 	
					 if($dados[0]['tpmid']){
					 	$sql .= "union 

							select
								mm.mtrid as codigo, 
								mm.mtrdsc as descricao
							from demandas.material mm
							left join demandas.tipomaterial tt ON tt.mtrid = mm.mtrid
							where mm.mtrstatus ='A' 
							and tt.tpmstatus='A'
							and tt.tpmid = ".$dados[0]['tpmid'];
					 }
					 
					 $sql .= " order by 2 ";
					 
					 $tipo = $db->carregar( $sql );
					 if($tipo){
					 	
						 foreach( $tipo as $t ){
							
						 	echo "<optgroup label='".$t['descricao']."'>";
							
						 	//$sql = "select tpmid as codigo, tpmdsc as descricao from demandas.tipomaterial where tpmstatus='A' and mtrid=".$t['codigo']." order by 2";
						 	$sql = "select
										t.tpmid as codigo, 
										t.tpmdsc as descricao,
										totalentrada,
										totalsaida
									from demandas.tipomaterial t
									left join (select sum(ctmqtd) as totalentrada, tpmid from demandas.controlematerial where tpcid = 1 and ctmstatus = 'A' group by tpmid) e ON e.tpmid = t.tpmid
									left join (select sum(ctmqtd) as totalsaida, tpmid from demandas.controlematerial where tpcid = 2 and ctmstatus = 'A' group by tpmid) s ON s.tpmid = t.tpmid
									
									where t.tpmstatus='A' 
									and t.mtrid=".$t['codigo']."
									group by t.tpmid, t.tpmdsc, totalentrada, totalsaida
									having ( coalesce(totalentrada,0) - coalesce(totalsaida,0) ) > 0
									";
						 	
							 if($dados[0]['tpmid']){
							 	$sql .= "union 
		
									select
									tm.tpmid as codigo, 
									tm.tpmdsc as descricao,
									'0' as totalentrada,
									'0' as totalsaida
									from demandas.tipomaterial tm
									where tm.tpmstatus='A'
									and tm.mtrid=".$t['codigo']." 
									and tm.tpmid = ".$dados[0]['tpmid'];
							 }
							 
							 $sql .= " order by 2 ";
									
						 	$material = $db->carregar( $sql );
						 	if($material){
							 	foreach( $material as $m ){
							 		?>
							 		<option value="<?= $m['codigo'] ?>" <?php if($dados[0]['tpmid'] == $m['codigo']) echo "selected";?>><?= $m['descricao'] ?></option>
							 		<? 
							 	}
						 	}else{
						 		//echo "<option value=''>Não existe material.</option>";
						 	}					 	
							echo "</optgroup>";
							
						 }
					 }
					 ?>
			</select>		
			<?php echo obrigatorio();?>	
			
		</td>
	</tr>	
	<?php }?>
	<?php if(in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfilUsu) || in_array(DEMANDA_PERFIL_DEPOSITO_DTI,$perfilUsu) || in_array(DEMANDA_PERFIL_GESTOR_MEC,$perfilUsu) ){?>
	<tr id="tr_material_entregue" style="display:<? echo $displayMaterial;  ?>;">
		<td align='right' class="SubTituloDireita">Material Entregue:</td>
		<td >
			<? //$dmdatendremoto = $dados[0]['dmdatendremoto']; ?>
			<input type="radio" name="dmdmaterialentregue" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> value="t" <?if($dados[0]['dmdmaterialentregue'] == 't' ) echo "checked";?> > Sim
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" name="dmdmaterialentregue" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> value="f" <?if($dados[0]['dmdmaterialentregue'] == 'f' || $dados[0]['dmdmaterialentregue'] == '') echo "checked";?> > Não
		</td>
	</tr>	
	<tr id="tr_material_devolvido" style="display:<? echo $displayMaterial;  ?>;">
		<td align='right' class="SubTituloDireita">Material com Defeito Devolvido:</td>
		<td >
			<? //$dmdatendremoto = $dados[0]['dmdatendremoto']; ?>
			<input type="radio" name="dmdmaterialdevolvido" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> value="t" <?if($dados[0]['dmdmaterialdevolvido'] == 't' ) echo "checked";?> > Sim
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" name="dmdmaterialdevolvido" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> value="f" <?if($dados[0]['dmdmaterialdevolvido'] == 'f' || $dados[0]['dmdmaterialdevolvido'] == '') echo "checked";?> > Não
		</td>
	</tr>	
	<?php }?>
	<tr style="display:<? echo $displayNserie;  ?>;">
		<td align='right' class="SubTituloDireita">Nº Serie / Nº Patrimonio:</td>
		<td>
			<?php
			
				//pega nº serie / patrimonio
				$nseriepatr = "";

					if($dmdid){
						$sqln = "select ihdnumserie, ihdnumpatrimonio 
								  from demandas.itemhardwaredemanda 
								  where dmdid = ".$dmdid;
						$nsp = array();
						$nsp = $db->carregar( $sqln );
						$ctnsp = 1;
						if($nsp){
							$nseriepatr = '<div align=left>';
							foreach($nsp as $nspx){
								
								$nseriepatr .= '<b><u>Item '.$ctnsp .'</u> - </b>';
								
								if($nspx['ihdnumserie']) $nseriepatr .= 'Nº Série: '. $nspx['ihdnumserie'];
								if($nspx['ihdnumserie'] && $nspx['ihdnumpatrimonio']) $nseriepatr .= ' / ';
								if($nspx['ihdnumpatrimonio']) $nseriepatr .= 'Nº Patrimônio: '. $nspx['ihdnumpatrimonio'];
								
								$nseriepatr .= '<br>';
								
								$ctnsp++;	
							}
							$nseriepatr .= '</div>';
						}	
						
					}
				
				echo $nseriepatr;		

			?>
			<?if($habil == 'S'){ ?>
			<a href="javascript:popCadnSeriePatr();">Editar número(s)</a>
			<?}?>
			<?php // popCadnSeriePatr() esta no arquivo _funcoes.php ?>
		</td>
	</tr>	
	<tr>
		<td align='right' class="SubTituloDireita">Descrição detalhada:</td>
		<td>
		<? $dmddsc = $dados[0]['dmddsc']; ?>
		<?if($habil == 'S'){ ?>
			<?=campo_textarea('dmddsc', 'N ', $habil, '', 80, 5, 4000); ?>
		<?}else{ ?>
			<div style="height: 85px; width: 400px; background-color: #eee; border: none;" class="div_rolagem" >
			<table class="ex" border=0 cellpadding="3" cellspacing="0" >
				<tr>
					<td>
						<?if($dmddsc) echo str_replace ( chr(13), "<BR>", $dmddsc )?>
					</td>
				</tr>
			</table>
			</div>
		<?}?>
							
		<textarea name="dmddsc_old" style="display:none"><?=$dmddsc?></textarea>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Reprodução do problema:</td>
		<td>
		<? $dmdreproducao = $dados[0]['dmdreproducao']; ?>	
		<?= campo_textarea('dmdreproducao', 'N ', $habil, '', 80, 5, 4000); ?>
		<textarea name="dmdreproducao_old" style="display:none"><?=$dmdreproducao?></textarea>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Horário Alternativo do atendimento:</td>
		<td>
			<? $dmdhorarioatendimento = $dados[0]['dmdhorarioatendimento']; ?>
			<input type="checkbox" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> name="horarioA" value="A" <?if($dmdhorarioatendimento == 'A' || $dmdhorarioatendimento == 'T') echo "checked";?> > Horário de Almoço (12:00 às 14:00)
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="checkbox" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> name="horarioN" value="N" <?if($dmdhorarioatendimento == 'N' || $dmdhorarioatendimento == 'T') echo "checked";?> > Horário Noturno (18:00 às 22:00)
		</td>
	</tr>				
	<tr>
		<td align='right' class="SubTituloDireita">Deseja receber atendimento remoto:</td>
		<td>
			<? $dmdatendremoto = $dados[0]['dmdatendremoto']; ?>
			<input type="radio" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> name="atendimentoRemoto" value="t" onclick="atendRemoto();" <?if($dmdatendremoto == 't' ) echo "checked";?> > Sim
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> name="atendimentoRemoto" value="f" onclick="atendRemoto();" <?if($dmdatendremoto == 'f' || $dmdatendremoto == '') echo "checked";?> > Não
		</td>
	</tr>	
	<tr id="tr_atendremoto" style="display:<?if($dmdatendremoto == 'f' || $dmdatendremoto == '') echo 'none;';?>">
		<td  align='right' class="SubTituloDireita">Nome do Computador(CPU):<br>Clique com o botão direito do mouse no atalho do "Meu computador", depois clique em "Propriedades", depois clique na aba "Nome do computador". Ex.: mec-32-2014.mec.gov.br (Digite somente mec-32-2014).</td>
		<td><? $usdnomecomputador = $dados[0]['usdnomecomputador']; ?>
			<?= campo_texto( 'usdnomecomputador', 'S', $habil, '', 40, 100, '', ''); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Atendimento é urgente:</td>
		<td>
			<? $dmdatendurgente = $dados[0]['dmdatendurgente'];?>
			<input type="radio" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> name="dmdatendurgente" value="t" <?if($dmdatendurgente == 't' ) echo "checked";?> onclick="atendUrgente();"> Sim
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> name="dmdatendurgente" value="f" <?if($dmdatendurgente == 'f' || $dmdatendurgente == '') echo "checked";?> onclick="atendUrgente();"> Não
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Demanda Judicial:</td>
		<td>			
			<input type="radio" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> name="dmdjudicial" value="t" <?php echo $dados[0]['dmdjudicial'] == 't' ? 'checked' : '' ?>> Sim
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> name="dmdjudicial" value="f" <?php echo ($dados[0]['dmdjudicial'] == 'f' || empty($dados[0]['dmdjudicial'])) ? 'checked' : '' ?>> Não
		</td>
	</tr>	
	<tr id="tr_atendurgente" style="display:<?if($dmdatendurgente == 'f' || $dmdatendurgente == '') echo 'none;';?>">
		<td  align='right' class="SubTituloDireita">Informe a justificativa:</td>
		<td><? $dmdjusturgente = $dados[0]['dmdjusturgente']; ?>
			<?= campo_textarea('dmdjusturgente', 'S ', $habil, '', 80, 5, 4000); ?>
		</td>
	</tr>
	<tr>
		<td align='left' bgcolor="#DCDCDC" colspan="2" style="padding-left:36px"><b>Local de Atendimento</b>:</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Setor:</td>
		<td>
			<?php
				$unaid = $dados[0]['unaid'];
				$sql = " SELECT
						  unaid AS codigo, 
					  	  upper(unasigla)||' - '||unadescricao as descricao 
						 FROM
						  demandas.unidadeatendimento 
						 WHERE
						  unastatus = 'A'
						  order by unasigla, unadescricao";
				$db->monta_combo("unaid",$sql,$habil,"-- Selecione um setor --",'','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Edifício:</td>
		<td>
		<?php
		$sql = "SELECT 
				 lcaid AS codigo,
				 lcadescricao AS descricao
				FROM 
				 demandas.localatendimento
				ORDER BY 
				 lcadescricao";
		$lcaid = $dados[0]['lcaid'];
		$db->monta_combo( 'lcaid', $sql, $habil, '-- Informe o local de atendimento --', 'filtraAndar', '' );
		?>	
		<?= obrigatorio(); ?>
		</td>
	</tr>	
	<tr id="tr_andar" >
		<td align='right' class="SubTituloDireita" >Andar:</td>
		<td id='andardemanda'>
			<?
			$sql = sprintf("SELECT 
							 laa.laaid AS codigo,
							 aa.anddescricao AS descricao
							 --laa.laaid, laa.lcaid, aa.andid, aa.anddescricao 
							FROM 
							 	demandas.localandaratendimento laa
								inner join demandas.andaratendimento aa on laa.andid = aa.andid
							WHERE
							 laa.lcaid = %d 
							ORDER BY 
							 aa.anddescricao", 
					$lcaid);
			$dados2 = $db->carregar($sql);
			//echo 'ok='.count($dados).'<br>';
			$laaid = $dados[0]['laaid'];
			if(count($dados2)==1) $laaid = $dados2[0]['codigo'];
			//echo $laaid;
			$db->monta_combo( 'laaid', $sql, $habil, '-- Informe o Andar --', '', '', '', '', '', '', '', $laaid );
			
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>				
	<tr id="tr_sala" >
		<td align='right' class="SubTituloDireita">Sala:</td>
		<td><?$dmdsalaatendimento = $dados[0]['dmdsalaatendimento'];?><?= campo_texto( 'dmdsalaatendimento', 'S', $habil, '', 20, 50, '', ''); ?></td>
	</tr>	
	<tr>
		<td align='right' class="SubTituloDireita">Sala fica no Gabinete:</td>
		<td>
			<?$usdgabinete = $dados[0]['usdgabinete'];?>
			<input type="radio" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> name="usdgabinete" value="TRUE" <?if($usdgabinete == 't') echo "checked";?>> Sim
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" <?= $habil == 'N' ? 'disabled="disabled"' : ''?> name="usdgabinete" value="FALSE" <?if($usdgabinete == 'f' || $usdgabinete == '') echo "checked";?>> Não
		</td>
	</tr>				
	<tr>
		<td align='right' class="SubTituloDireita">Ramal:</td>
		<td>
			<?$usdramal = $dados[0]['usdramal'];?>
			<?= campo_texto( 'usdramal', 'N', $habil, '', 4, 4, '####', ''); ?>
		</td>
	</tr>		
	<tr>
		<td align='right' class="SubTituloDireita">Celular:<br><b>(Opcional)</b></td>
		<td>
			<?$usdcelular = formata_fone_fax($dados[0]['usdcelular']);?>
			<?= campo_texto( 'usdcelular', 'N', $habil, '', 10, 9, '####-####', ''); ?>
		</td>
	</tr>		
	<tr>
		<td align='left' bgcolor="#DCDCDC" colspan="2" style="padding-left:23px"><b>Dados de Rastreamento</b>:</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Motivador da demanda:</td>
		<td>
			<?php
				$motid = $dados[0]['motid'];
				$sql = " SELECT
						  motid AS codigo, 
					  	  motdesc as descricao 
						 FROM
						  demandas.motivador 
						 WHERE
						  motstatus = 'A'
						  order by motdesc";
				$db->monta_combo("motid",$sql, $habil,"-- Selecione o Motivador da demanda --",'filtraDoc','');
			?>
		</td>
	</tr>
	<tr id="tr_numdoc" style="display:none;">
		<td align='right' class="SubTituloDireita">Nº do documento:</td>
		<td>
			<?$dmdnumdocrastreamento = $dados[0]['dmdnumdocrastreamento']; ?>
			<?= campo_texto( 'dmdnumdocrastreamento', 'N', $habil, '', 20, 50, '', ''); ?>
		</td>
	</tr>
	<tr id="tr_dtdoc" style="display:none;">
		<td align='right' class="SubTituloDireita">Data:</td>
		<td>
			<?$dmddatadocrastreamento = $dados[0]['dmddatadocrastreamento']; ?>
			<?= campo_data( 'dmddatadocrastreamento', 'N', $habil, '', 'S' ); ?>
		</td>
	</tr>
	<tr id="tr_orgdoc" style="display:none;">
		<td align='right' class="SubTituloDireita">Órgão:</td>
		<td>
			<?php
				$dmdunidadedocrastreamento = $dados[0]['dmdunidadedocrastreamento'];
				$sql = " SELECT
						  unaid AS codigo, 
					  	  upper(unasigla)||' - '||unadescricao as descricao 
						 FROM
						  demandas.unidadeatendimento 
						 WHERE
						  unastatus = 'A'
						  order by unasigla, unadescricao";
				$db->monta_combo("dmdunidadedocrastreamento",$sql, $habil,"-- Selecione um Orgão --",'','');
			?>
		</td>
	</tr>
	<?if( ($dados[0]['ordid'] == 18 || $dados[0]['ordid'] == 21) && in_array($esdid, array(DEMANDA_GENERICO_ESTADO_FINALIZADO,DEMANDA_ESTADO_AUDITADO)) ){
		
		if( in_array($esdid, array(DEMANDA_GENERICO_ESTADO_FINALIZADO)) ){
			$existeSS = 'S';
		}else{
			$existeSS = 'N';
		}
		
		?>
		<tr>
			<td align='left' bgcolor="#DCDCDC" colspan="2" style="padding-left:23px"><b>Fábrica de Software</b>:</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Solicitação de Serviço (SS):</td>
			<td>
				<?php
					if($dados[0]['sidid']){
						$scsid = $dados[0]['scsid'];
						$sql = " SELECT
								  scsid AS codigo, 
							  	  'Nº ' || scsid || ' - ' || substring(scsnecessidade,0,60) || '...' as descricao 
								 FROM fabrica.solicitacaoservico ss
								 inner join workflow.documento doc on doc.docid = ss.docid and doc.esdid not in (251,300) --251=Cancelada Sem Custo / 300=Cancelada Com Custo
								 WHERE
								  ss.scsstatus = 'A' and ss.sidid = ".$dados[0]['sidid']." 
								 order by 1";
						
						$db->monta_combo("scsid",$sql, $existeSS,"-- Selecione a Solicitação de Serviço --",'','');
					}
					else{
						echo '<font color="red">informe o sistema!</font>';
					}
				?>
				<input type="hidden" name="scsid_old" value="<?=$scsid?>">
			</td>
		</tr>
	<?}?>
	<tr bgcolor="#C0C0C0">
		<td></td>
		<td colspan="2">
		<?if($existeSS=='S'){?>
			<input type="button" class="botao" name="btalterar" value="Salvar" onclick="validaFormSS();">			 
		<?}else{?>
			<input type="button" class="botao" name="btalterar" value="Salvar" onclick="validaForm();" <?= $habil == 'N' ? 'disabled="disabled"' : ''?>>
		<?}?>
		<input type="button" class="botao" name="btvoltar" value="Voltar" onclick="history.go(-1)">
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
    
d = document;

function atendRemoto(){

	tr  = d.getElementById('tr_atendremoto');
	
	if(d.getElementsByName('atendimentoRemoto')[0].checked == true){
		tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
	}
	if(d.getElementsByName('atendimentoRemoto')[1].checked == true){
		tr.style.display = 'none';
	}
}


function atendUrgente(){

	tr  = d.getElementById('tr_atendurgente');
	
	if(d.getElementsByName('dmdatendurgente')[0].checked == true){
		tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
	}
	if(d.getElementsByName('dmdatendurgente')[1].checked == true){
		tr.style.display = 'none';
	}
}

function checaMaterial(){

	
	if(d.getElementsByName('dmdmaterial')[0].checked == true){
		d.getElementById('tr_material').style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		<?php if(in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfilUsu) || in_array(DEMANDA_PERFIL_DEPOSITO_DTI,$perfilUsu) ){?>
		d.getElementById('tr_material_entregue').style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		d.getElementById('tr_material_devolvido').style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
		<?php }?>
	}
	if(d.getElementsByName('dmdmaterial')[1].checked == true){
		d.getElementById('tr_material').style.display = 'none';
		<?php if(in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfilUsu) || in_array(DEMANDA_PERFIL_DEPOSITO_DTI,$perfilUsu) ){?>
		d.getElementById('tr_material_entregue').style.display = 'none';
		d.getElementById('tr_material_devolvido').style.display = 'none';
		<?php }?>
	}
}




function exibe_sistema(){
	if(d.getElementById('ordid').value == 1 || d.getElementById('ordid').value == 18 || d.getElementById('ordid').value == 19 || d.getElementById('ordid').value == 20 || d.getElementById('ordid').value == 21){
		d.getElementById('tr_celula').style.display = '';
		d.getElementById('td_celula').style.display = '';
		d.getElementById('tr_sistema').style.display = '';
		d.getElementById('sisid').disabled = false ;
	}
	else if(d.getElementById('ordid').value == 2 || d.getElementById('ordid').value == 13 || d.getElementById('ordid').value == 14 || d.getElementById('ordid').value == 23 || d.getElementById('ordid').value == 24){
		d.getElementById('tr_celula').style.display = '';
		d.getElementById('td_celula').style.display = '';
		d.getElementById('tr_sistema').style.display = 'none';
		d.getElementById('sisid').disabled = true ;
	}
	else if(d.getElementById('ordid').value == 11){
		d.getElementById('tr_celula').style.display = 'none';
		d.getElementById('td_celula').style.display = 'none';
		d.getElementById('tr_sistema').style.display = '';
		d.getElementById('sisid').disabled = false ;
	}
	else{
		d.getElementById('tr_celula').style.display = 'none';
		d.getElementById('td_celula').style.display = 'none';
		d.getElementById('tr_sistema').style.display = 'none';
		d.getElementById('sisid').disabled = true ;
	}
}


//ja traz carregado os tipos de documento
filtraDoc(<?=$motid?>);


//Faz requisição via ajax
//Filtra o tipo de damanda, atravéz do parametro passado 'ordid'
function tipo_demanda(ordid) {
	//esconde a tr qtd
	d.getElementById('tr_qtd').style.display = 'none';
	
	valor = d.getElementById('ordid').value;
	
	if(valor){
	ndmd = d.getElementById('novadmd');
	admd = d.getElementById('antigadmd');
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('demandas.php?modulo=principal/dadosDemanda&acao=A', {
							        method:     'post',
							        parameters: '&ordidAjax=' + valor,
							        onComplete: function (res)
							        {		
										ndmd.innerHTML = res.responseText;
										admd.style.display = 'none';
										d.getElementById('tipid1').disabled = true;
										filtraCelula(ordid);
							        }
							  });
	}
}


function filtraTipoRede(celid) {
	
	//esconde a tr qtd
	d.getElementById('tr_qtd').style.display = 'none';
	
		
	if(celid){
	
	ndmd = d.getElementById('novadmd');
	admd = d.getElementById('antigadmd');
	
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('demandas.php?modulo=principal/dadosDemanda&acao=A', {
							        method:     'post',
							        parameters: '&celtiporedeAjax=' + celid,
							        onComplete: function (res)
							        {			
										ndmd.innerHTML = res.responseText;
										admd.style.display = 'none';
										d.getElementById('tipid1').disabled = true;
							        }
							  });
	}
	
}



function filtraCelula(ordid) {

	if(ordid != 11){//DIFERENTE de Projeto Web
		
		//alert(ordid);
		tdCel  = d.getElementById('td_celula');
		//select = td.getElementsByTagName('select')[0];
			
			
		
		// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
		//if (select)
			//select.disabled = true;
			
		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/dadosDemanda&acao=A', {
								        method:     'post',
								        parameters: '&ordidAjax2=' + ordid,
								        onComplete: function (res)
								        {	
											tdCel.innerHTML = res.responseText;
											tdCel.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
								        }
								  });
	}else{//Projeto Web
		celid = 9;
		filtraSistema(celid);
	}
	  
}


/*
* Faz requisição via ajax
* Filtra a qtd de damanda, atravéz do parametro passado 'ordid'
*/
function filtraQtd(tipid) {

	tr 	   = d.getElementById('tr_qtd');	
	td     = d.getElementById('qtddemanda');	
	verificaQtd = d.getElementById('verificaqtd');
	dscTipo     = d.getElementById('dscTipoDemanda');
	
	if(tipid){
		
		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/dadosDemanda&acao=A', {
								        method:     'post',
								        parameters: '&tipidAjax=' + tipid,
								        onComplete: function (res)
								        {			
	        								var result = res.responseText;

											if(result){
												result = result.split("|"); 

												if(result[0]){
													dscTipo.innerHTML = result[0];
												}
												else{
													dscTipo.innerHTML = "";	
												}
												
									        	if(result[1] != "N"){
													td.innerHTML = result[1];
													tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
													verificaQtd.value='1';
												}
												else{
													td.innerHTML = '';
													tr.style.display = 'none';
													verificaQtd.value='';
												}
											}
											
								        }
								  });
	}
	else{
		td.innerHTML = '';
		tr.style.display = 'none';
		verificaQtd.value='';
		dscTipo.innerHTML = "";
	}
	
}


/*
* Faz requisição via ajax
* Filtra o andar, atravéz do parametro passado 'lcaid'
*/
function filtraAndar(lcaid) {
	
	if(!lcaid) lcaid = '999999';
	
	trs	   = d.getElementById('tr_sala');
	tr 	   = d.getElementById('tr_andar');	
	td     = d.getElementById('andardemanda');	
	select = td.getElementsByTagName('select')[0];

	// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
	if (select)
		select.disabled = true;
		
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('demandas.php?modulo=principal/dadosDemanda&acao=A', {
							        method:     'post',
							        parameters: '&lcaidAjax=' + lcaid,
							        onComplete: function (res)
							        {			
										td.innerHTML = res.responseText;
										//tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
										//trs.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
							        }
							  });

}

/*
* Faz requisição via ajax
* Filtra o tipo de damanda, atravéz do parametro passado 'ordid'
*/
function filtraSistema(celid) {
	
	if(!celid) celid = 999999;
	
	tr 	   = d.getElementById('tr_sistema');	
	td     = d.getElementById('listasistema');	
	select = td.getElementsByTagName('select')[0];

	// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
	if (select)
		select.disabled = true;
		
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('demandas.php?modulo=principal/dadosDemanda&acao=A', {
							        method:     'post',
							        parameters: '&celidAjax=' + celid,
							        onComplete: function (res)
							        {			
										td.innerHTML = res.responseText;
										tr.style.display = navigator.appName == 'Netscape' ? 'table-row' : 'block';
							        }
							  });
}


function filtraDoc(motid) {
	
	d.getElementById('tr_numdoc').style.display = 'none';
	d.getElementById('tr_orgdoc').style.display = 'none';
	d.getElementById('tr_dtdoc').style.display = 'none';
	
	if(motid == 1){
		d.getElementById('tr_numdoc').style.display = '';
		d.getElementById('tr_orgdoc').style.display = '';
		d.getElementById('tr_dtdoc').style.display = '';
	}
	if(motid == 2){
		d.getElementById('tr_numdoc').style.display = '';
		d.getElementById('tr_orgdoc').style.display = '';
		d.getElementById('tr_dtdoc').style.display = '';
	}
	if(motid == 3){
		d.getElementById('tr_numdoc').style.display = 'none';
		d.getElementById('tr_orgdoc').style.display = 'none';
		d.getElementById('tr_dtdoc').style.display = '';
	}
	

}


function validaForm(){
	

	if(document.formulario.dmdtitulo.value == ''){
		alert ('O campo Assunto deve ser preenchido.');
		document.formulario.dmdtitulo.focus();
		return;
	}
	if(document.formulario.ordid.value == ''){
		alert ('Informe a Origem da Demanda.');
		return;
	}
	
	if(document.formulario.ordid.value == 1 || document.formulario.ordid.value == 18 || document.formulario.ordid.value == 19 || document.formulario.ordid.value == 20 || document.formulario.ordid.value == 21){
		if(document.formulario.celid.value == ''){
			alert ('Informe a Célula.');
			return;
		}
		if(document.formulario.sisid.value == ''){
			alert ('Informe o Sistema.');
			return;
		}
	}
	if(document.formulario.ordid.value == 2 || document.formulario.ordid.value == 13 || document.formulario.ordid.value == 14 || document.formulario.ordid.value == 23 || document.formulario.ordid.value == 24){
		if(document.formulario.celid.value == ''){
			alert ('Informe a Célula.');
			return;
		}
	}
	if(document.formulario.ordid.value == 11){
		if(document.formulario.sisid.value == ''){
			alert ('Informe o Sistema.');
			return;
		}
	}

	if((document.formulario.tipid1.value == '') && (document.getElementById('tipid1').disabled == false)){
		alert ('Informe o Tipo da Demanda.');
		return;
	}
	if(document.getElementById('tipid1').disabled == true){
		if(document.formulario.tipid2.value == ''){
			alert ('Informe o Tipo da Demanda.');
			return;
		}
	}
	/*
	if(document.formulario.priid.value == ''){
		alert ('Informe a Prioridade do Usuário.');
		return false;
	}
	*/

	
	if(d.getElementById('verificaqtd').value != ""){
		if (d.formulario.dmdqtde.value < 1){
			d.formulario.dmdqtde.focus();
			alert('O campo quantidade do serviço, deve ser maior que zero!');
			return false;
		}
	}

	<?if(in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfilUsu) || in_array(DEMANDA_PERFIL_DEPOSITO_DTI,$perfilUsu) || in_array(DEMANDA_PERFIL_GESTOR_MEC,$perfilUsu) ){?>
		if(d.getElementsByName('dmdmaterial')[0].checked == true){
			if(d.formulario.tpmid.value == ''){
				alert('Selecione o Material.');
				d.formulario.tpmid.focus();
				return false;
			}
		}
	<?}?>

	if(d.getElementsByName('atendimentoRemoto')[0].checked == true){
		if(d.getElementsByName('usdnomecomputador')[0].value == ''){
			alert('O campo Nome do Computador(CPU), deve ser preenchido!');
			d.getElementsByName('usdnomecomputador')[0].focus();
			return false;
		}		
	}

	if(d.getElementsByName('dmdatendurgente')[0].checked == true){
		if(d.getElementsByName('dmdjusturgente')[0].value == ''){
			alert('O campo Justificativa, deve ser preenchido!');
			d.getElementsByName('dmdjusturgente')[0].focus();
			return false;
		}		
	}
	
	
	if(d.getElementsByName('unaid')[0].value == ''){
		alert('O campo Setor, deve ser preenchido!');
		d.getElementsByName('unaid')[0].focus();
		return;
	}

	//if(document.formulario.ordid.value == '4' || document.formulario.ordid.value == '3' || document.formulario.ordid.value == '5'){
		if(d.getElementsByName('lcaid')[0].value == ''){
			alert('O campo Edifício, deve ser preenchido!');
			d.getElementsByName('lcaid')[0].focus();
			return;
		}
		if(d.getElementsByName('laaid')[0].value == ''){
			alert('O campo Andar, deve ser preenchido!');
			d.getElementsByName('laaid')[0].focus();
			return;
		}
		if(d.getElementsByName('dmdsalaatendimento')[0].value == ''){
			alert('O campo Sala, deve ser preenchido!');
			d.getElementsByName('dmdsalaatendimento')[0].focus();
			return;
		}
	//}
	
	
	
	var dmdid = "<?=$dmdid?>";
	//Faz uma requisição ajax, passando o parametro 'dmdid', via POST
	var req = new Ajax.Request('demandas.php?modulo=principal/dadosDemanda&acao=A', {
							        method:     'post',
							        parameters: '&dmdidAjax=' + dmdid,
							        onComplete: function (res)
							        {			
										if(res.responseText != ""){
											if(confirm('Esta demanda já foi classificada por outro técnico. \nDeseja reclassificá-la?')){
												document.formulario.submit();
											}
										}
										else{
											document.formulario.submit();
										}
							        }
							  });	
							  

}


function validaFormSS(){
	
	var scsid = document.formulario.scsid.value;
	var scsid_old = document.formulario.scsid_old.value;
	if(scsid == ''){
		alert ('O campo Solicitação de Serviço (SS) deve ser preenchido.');
		document.formulario.scsid.focus();
		return;
	}
	
	var dmdid = "<?=$dmdid?>";
	//Faz uma requisição ajax, passando o parametro 'dmdid', via POST
	var req = new Ajax.Request('demandas.php?modulo=principal/dadosDemanda&acao=A', {
							        method:     'post',
							        parameters: '&dmdidSS=' + dmdid + '&scsid=' + scsid + '&scsid_old=' + scsid_old,
							        onComplete: function (res)
							        {			
										if(res.responseText == "OK"){
											alert("Operação efetuada com sucesso!");
											location.href="demandas.php?modulo=principal/dadosDemanda&acao=A";
										}
							        }
							  });	
							  

}

function exibeHistoricoDem( docid )
{
	var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid=' + docid;
	window.open(
		url,
		'alterarEstado',
		'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
	);
}				

</script>


<!-- Pop up de Efetivação da Demanda -->
<script language= "javascript">
 
<?php  if ($dmddatainiatendefetivo == ''){
		if($usucpfexecutor == $_SESSION['usucpf'] && in_array($esdid, array(DEMANDA_ESTADO_EM_ATENDIMENTO,DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO)) ){
?>	
				
		 
			if (window.confirm ("Deseja iniciar a Demanda ? "))
				{
					//Faz uma requisição ajax, passando o parametro 'dmdid', via POST
					var req = new Ajax.Request('demandas.php?modulo=principal/dadosDemanda&acao=A', {
											        method:     'post',
											        parameters: '&dtinicioAjax=1',
											        onComplete: function (res)
											        {			
				        								//alert(res.responseText);
														if(res.responseText == "OK"){
															alert("Demanda Iniciada!");
														}
														
											        }
											  });					
				}
			else
				{
			location.href=('demandas.php?modulo=principal/lista&acao=A'); 
			}
<?php }
	}
?>
</script>
</body>