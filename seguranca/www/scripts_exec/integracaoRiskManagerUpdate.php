<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

$db = new cls_banco();

$sql = "SELECT DISTINCT
			  icl.iclid,
			  ati._atinumero ||' - '|| ati.atidescricao as atividades,
			  icl.iclid || ' - ' ||icl.icldsc as itemdescricao,
			  to_char(icl.iclprazoinicio, 'DD/MM/YYYY') as iclprazoinicio,
			  case when icl.iclcritico = 't' then 'Sim' else 'Não' end as critico,
			  CASE WHEN (SELECT etcopcaoevidencia FROM pde.etapascontrole WHERE iclid = icl.iclid and tpvid = 1) is not null then 'Sim' else 'Não' end as etapaexecucao,
              CASE WHEN (SELECT etcopcaoevidencia FROM pde.etapascontrole WHERE iclid = icl.iclid and tpvid = 2) is not null then 'Sim' else 'Não' end  as etapavalidacao,
              CASE WHEN (SELECT etcopcaoevidencia FROM pde.etapascontrole WHERE iclid = icl.iclid and tpvid = 3) is not null then 'Sim' else 'Não' end  as etapacertificacao,
              CASE WHEN (SELECT etcopcaoevidencia FROM pde.etapascontrole WHERE iclid = icl.iclid and tpvid = 1) = 't' then 'Sim' else 'Não' end as evidenciaexecucacao,
              CASE WHEN (SELECT etcopcaoevidencia FROM pde.etapascontrole WHERE iclid = icl.iclid and tpvid = 2) = 't' then 'Sim' else 'Não' end  as evidenciavalidacao,
              to_char(icl.iclprazoexec, 'DD/MM/YYYY') as prazoexe,
              to_char(icl.iclprazoval, 'DD/MM/YYYY') as prazoval,
			  icl.iclprazo,
			  (SELECT max(hcldt) FROM pde.historicochecklist WHERE iclid = icl.iclid ) as dataatualizacao,
			  idm.irmdataatualizacao as dataatualizacaorisk,
			  idm.irmcodevento,
			  CASE WHEN val1.vldid IS NULL THEN 'Não Executado' ELSE 'Executado' END as executado,
			  CASE WHEN en1.entnome IS NULL THEN 'Sem executor(es)' ELSE en1.entnome || ' ' || case when trim('('||coalesce(trim(en1.entnumdddcomercial),'') ||') '|| coalesce(trim(en1.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en1.entnumdddcomercial),'') ||') '|| coalesce(trim(en1.entnumcomercial),'')) END 
			  END as executores,
			  
			  CASE WHEN val2.vldid IS NULL THEN 'Não Validado' ELSE 'Validado' END as validado,              
			  CASE WHEN en2.entnome IS NULL THEN 'Sem validador(es)' ELSE en2.entnome || ' ' || case when trim('('||coalesce(trim(en2.entnumdddcomercial),'') ||') '|| coalesce(trim(en2.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en2.entnumdddcomercial),'') ||') '|| coalesce(trim(en2.entnumcomercial),'')) END 
			  END as validadores,
			  
			  CASE WHEN val3.vldid IS NULL THEN 'Não Certificado' ELSE 'Certificado' END as certificado,
			  CASE WHEN en3.entnome IS NULL
			      THEN 'Sem certificador(es)'
			      ELSE coalesce(en3.entnome,' ') || ' ' || case when trim('('||coalesce(trim(en3.entnumdddcomercial),'') ||') '|| coalesce(trim(en3.entnumcomercial),'')) = '()' then '' else trim('('||coalesce(trim(en3.entnumdddcomercial),'') ||') '|| coalesce(trim(en3.entnumcomercial),'')) END 
			  END as certificadores,
			  idm.irmregexcluido
			FROM 
			    pde.itemchecklist icl 
			INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid AND ati.atistatus = 'A'
			LEFT JOIN pde.validacao val1 ON val1.iclid = icl.iclid AND val1.tpvid = 1 
			LEFT JOIN pde.checklistentidade ch1 ON ch1.iclid = icl.iclid AND ch1.tpvid = 1
			LEFT JOIN entidade.entidade en1 ON en1.entid = ch1.entid AND en1.entstatus = 'A'
			LEFT JOIN pde.validacao val2 ON val2.iclid = icl.iclid AND val2.tpvid = 2 
			LEFT JOIN pde.checklistentidade ch2 ON ch2.iclid = icl.iclid AND ch2.tpvid = 2
			LEFT JOIN entidade.entidade en2 ON en2.entid = ch2.entid AND en2.entstatus = 'A'
			LEFT JOIN pde.validacao val3 ON val3.iclid = icl.iclid AND val3.tpvid = 3 
			LEFT JOIN pde.checklistentidade ch3 ON ch3.iclid = icl.iclid AND ch3.tpvid = 3
			LEFT JOIN entidade.entidade en3 ON en3.entid = ch3.entid AND en3.entstatus = 'A' 
			INNER JOIN pde.integracaoriskmanager idm on idm.iclid = icl.iclid
			INNER JOIN workflow.documento doc on icl.docid = doc.docid
			INNER JOIN workflow.estadodocumento esd on esd.esdid = doc.esdid and esd.esdid <> 284 
		WHERE  
			(icl.iclcritico is true or 
		    (icl.iclprazo < now() AND ( ( en3.entid IS NOT NULL AND (val3.vldid IS NULL OR val3.vldsituacao IS NOT TRUE) )  OR 
		    							( en3.entid IS NULL AND en2.entid IS NOT NULL AND (val2.vldid IS NULL OR val2.vldsituacao IS NOT TRUE) ) OR 
		                                ( en3.entid IS NULL AND en2.entid IS NULL AND en1.entid IS NOT NULL  AND (val1.vldid IS NULL OR val1.vldsituacao IS NOT TRUE) ) 
		      						  )
		    ))
            and (SELECT max(hcldt) FROM pde.historicochecklist WHERE iclid = icl.iclid) <> idm.irmdataatualizacao";

$arDados = $db->carregar( $sql );
$arDados = $arDados ? $arDados : array();
//ver( sizeof($arDados), $arDados,d );
include_once APPRAIZ.'seguranca/www/include/integracaoRiskManager/oauth2_config.php';

//construct POST object for access token fetch request
$post = array('client_id' => CLIENT_ID,
              'client_secret' => CLIENT_SECRET,
              'grant_type' => 'client_credentials');

//get JSON access token object (with refresh_token parameter)
$token = json_decode(runCurl(ACCESS_TOKEN_ENDPOINT, 'POST', $post));

foreach ($arDados as $key => $v) {	
	$progress = ($v['etapaexecucao'] == 'Sim' ? 33 : 0);
	$progress += ($v['etapavalidacao'] == 'Sim' ? 33 : 0);
	$progress += ($v['etapacertificacao'] == 'Sim' ? 33 : 0);
	
	$progress = $progress == 99 ? '100' : $progress;
	
	$objeto = array("Comment" => 'Atividade: '.utf8_encode($v['atividades'].',
								 Descrição Item: '.$v['itemdescricao'].',
								 Data de Início: '.$v['iclprazoinicio'].',
								 Item Crítico: '.$v['critico'].',
								 Etapas de Controle: (Execução: '.$v['etapaexecucao'].', Validação: '.$v['etapavalidacao'].', Certificação: '.$v['etapacertificacao'].' ),
								 Coletar evidência na Execução: '.$v['evidenciaexecucacao'].',
								 Executor: '.$v['executores'].', 
								 Prazo de Execução: '.$v['prazoexe'].', 
								 Coletar evidência na Validação: '.$v['evidenciavalidacao'].',
								 Validador: '.$v['validadores'].', 
								 Prazo de Validação: '.$v['prazoval'].',
								 Requisição: '.($v['irmregexcluido'] == 'S' ? 'Delete' : 'Update') )
					, "Progress" => $progress);
	
	//fetch profile of current user
	$retorno = runCurlJson(WF_CREATE_EVENT."/".$v['irmcodevento'], 'PUT', simec_json_encode($objeto), $token->access_token);
	$retorno = json_decode($retorno);
	if( $retorno ){
		$dataAtualizacao = !empty($v['dataatualizacao']) ? $v['dataatualizacao'] : 'now()';
		
		$sql = "UPDATE pde.integracaoriskmanager SET irmdataatualizacao = '".$dataAtualizacao."' WHERE iclid = ".$v['iclid'];		
		$db->executar( $sql );
		$db->commit();
	}
}
?>