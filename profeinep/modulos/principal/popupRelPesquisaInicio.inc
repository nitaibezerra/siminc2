<?php
/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

$filtroprocesso[] = " AND prcstatus = 'A' ";

$filtro_estado 	  = " ";
		
if(is_array($_REQUEST['esdid'])) {
	$filtroprocesso[] = "esd.esdid in (".implode(',',$_REQUEST['esdid']).")";
}elseif( $_REQUEST['todos'] || $_REQUEST['evento'] == '' ){
	$filtroprocesso[] = "esd.esdid in (374,382,381,378,380)";
}
if($_REQUEST['tprid']) {
	$filtroprocesso[] = "tprid='".$_REQUEST['tprid']."'";
}
if($_REQUEST['prcnumsidoc']) {
	$filtroprocesso[] = "prcnumsidoc='".eregi_replace('([^0-9])','',$_REQUEST['prcnumsidoc'])."'";
}
if($_REQUEST['prcnomeinteressado']) {
	$filtroprocesso[] = "removeacento(prcnomeinteressado) ilike(removeacento('%".$_REQUEST['prcnomeinteressado']."%'))";
}

if($_REQUEST['prcdtentradaini']) {
	$dataformatadainicio = formata_data_sql($_REQUEST['prcdtentradaini']);

	$filtroprocesso[] = "prcdtentrada >= '".$dataformatadainicio."'";
}
if($_REQUEST['prcdtentradafim']) {
	$dataformatadafim = formata_data_sql($_REQUEST['prcdtentradafim']);

	$filtroprocesso[] = "prcdtentrada <= '".$dataformatadafim."'";
}
if($_REQUEST['prcdtultimaini']) {
	$dataultimainicio = formata_data_sql($_REQUEST['prcdtultimaini']);

	$filtroprocesso[] = "TO_CHAR(data, 'YYYY-MM-DD') >= '".$dataultimainicio."'";
}
if($_REQUEST['prcdtultimafim']) {
	$dataultimafim = formata_data_sql($_REQUEST['prcdtultimafim']);

	$filtroprocesso[] = "TO_CHAR(data, 'YYYY-MM-DD') <= '".$dataultimafim."'";
}
if($_REQUEST['advid']) {
	$filtroprocesso[] = "advid = '".$_REQUEST['advid']."'";
}

if($_REQUEST['anxdesc']) {
	$filtroprocesso[] = "anx.anxdesc ilike '%".$_REQUEST['anxdesc']."%'";
}

if(strlen($_REQUEST['proid'])!=0) {
	$code = explode("&", $_REQUEST['proid']);
	$filtroprocesso[] = "pro.proid='".$code[0]."' ";
}
if($_REQUEST['tasid']) {
	$filtroprocesso[] = "tasid='".$_REQUEST['tasid']."'";
}
if($_REQUEST['cooid']) {
	$filtroprocesso[] = "cooid in (".implode(',',$_REQUEST['cooid']).")";
}
if($_REQUEST['esdid']) {
	$filtroprocesso[] = "esd.esdid in (".implode(',',$_REQUEST['esdid']).")";
}
if($_REQUEST['expressaochave']) {
	$filtroprocesso[] = "removeacento(excdsc) ilike (removeacento('%".$_REQUEST['expressaochave']."%'))";
}
if($_REQUEST['tacid']) {
	$filtroprocesso[] = "tac.tacid = ".$_REQUEST['tacid'];
}
if($_REQUEST['prcnumeroprocjudicial']) {
	$filtroprocesso[] = "prc.prcnumeroprocjudicial='".$_REQUEST['prcnumeroprocjudicial']."'";
}

if($_POST['com_sem_adv'] == "1"){
	$filtroprocesso[] = "prc.advid is not null";
}
if($_POST['com_sem_adv'] == "0"){
	$filtroprocesso[] = "prc.advid is null";
}

$cabecalho = array('Ações','Nº do Processo SIDOC','Interessado','Data de Entrada','Situação PROFE/INEP');
	
$condicoes = ( (count($filtroprocesso) > 1) ? implode(" AND ", $filtroprocesso)  : " AND prcstatus = 'A' " );

$qualquer_situacao = $_REQUEST['prcnumsidoc']==false && $_REQUEST['prcnumeroprocjudicial']==false && $_REQUEST['prcnumeroprocjudantigo']==false; 

if($_REQUEST['esdid']==false && $qualquer_situacao ) {
	$condicoes .= " AND esd.esdid not in (30,99,".EST_ANEXADO.") ";
}

if($_REQUEST['encerrados'] != 'sim' && $_REQUEST['esdid']!=PROFEINEP_PROCESSO_ENCERRADO && $qualquer_situacao ) {
	$condicoes .= " AND esd.esdid <> ". PROFEINEP_PROCESSO_ENCERRADO;
}

if($_REQUEST['prioritarios'] == 'sim' ) {
	$condicoes .= " AND prc.prcprioritario = true ";
}

if($_REQUEST['prcnumeroprocjudantigo'] && $qualquer_situacao==false) {
	$condicoes .= " AND prc.prcnumeroprocjudantigo='".$_REQUEST['prcnumeroprocjudantigo']."' ";
}
if($_REQUEST['tpdid']) {
	$condicoes .= " AND nud.tpdid = ".$_REQUEST['tpdid']." ";
}
if($_REQUEST['nudnumero']) {
	$condicoes .= " AND to_char(nud.nudnumero,'9999999999') LIKE '%".$_REQUEST['nudnumero']."%' AND nud.nudano = ".$_REQUEST['nudano'];
}elseif($_REQUEST['nudano']){
	$condicoes .= " AND nud.nudano = ".$_REQUEST['nudano'];
}

$sql = "SELECT 
			 
			CASE WHEN (esd.esdid <> 80) 
				THEN
				CASE WHEN (current_date - INTERVAL '15 days' > prcdtentrada AND tpr.tipid = 2 ) 
					THEN '<font color=red onclick=\"window.opener.location=\'?modulo=principal/editarprocesso&acao=A&prcid='||prc.prcid||'\'\" title=\"Passaram 15 dias desde a entrada.\">' || prcnumsidoc || '</font>' 
                    ELSE 
					CASE WHEN (current_date - INTERVAL '5 days' > prcdtentrada AND tpr.tipid = 1 ) 
						THEN '<font color=red onclick=\"window.opener.location=\'?modulo=principal/editarprocesso&acao=A&prcid='||prc.prcid||'\'\" title=\"Passaram 5 dias desde a entrada.\">' || prcnumsidoc || '</font>' 
                        ELSE '<font onclick=\"window.opener.location=\'?modulo=principal/editarprocesso&acao=A&prcid='||prc.prcid||'\'\" title=\"Passaram 5 dias desde a entrada.\">' || prcnumsidoc || '</font>' 
                    END 
                END 
            	ELSE '<font onclick=\"window.opener.location=\'?modulo=principal/editarprocesso&acao=A&prcid='||prc.prcid||'\'\" title=\"Passaram 5 dias desde a entrada.\">' || prcnumsidoc || '</font>'  
            END as prcnumsidoc,
			prcnomeinteressado,
			CASE WHEN wultimadata.dataultimocadastro is not null 
				THEN 
	            CASE WHEN (current_date - INTERVAL '15 days' > wultimadata.dataultimocadastro AND tpr.tipid = 2 ) 
	            	THEN '<font color=red title=\"Passaram 15 dias desde o ultimo cadastro.\">' || to_char(wultimadata.dataultimocadastro, 'DD/MM/YYYY') || '</font>' 
	                ELSE 
	                CASE WHEN (current_date - INTERVAL '5 days' > wultimadata.dataultimocadastro AND tpr.tipid = 1 ) 
	                	THEN '<font color=red title=\"Passaram 5 dias desde o ultimo cadastro.\">' || to_char(wultimadata.dataultimocadastro, 'DD/MM/YYYY') || '</font>' 
	                    ELSE to_char(wultimadata.dataultimocadastro, 'DD/MM/YYYY') 
	                END 
	            END				
		        ELSE 
	            CASE WHEN (current_date - INTERVAL '15 days' > prcdtentrada AND tpr.tipid = 2 ) 
	            	THEN '<font color=red title=\"Passaram 15 dias desde a entrada.\">' || to_char(prcdtentrada, 'DD/MM/YYYY') || '</font>' 
	                ELSE 
	                CASE WHEN (current_date - INTERVAL '5 days' > prcdtentrada AND tpr.tipid = 1 ) 
	                	THEN '<font color=red title=\"Passaram 5 dias desde a entrada.\">' || to_char(prcdtentrada, 'DD/MM/YYYY') || '</font>' 
	                    ELSE to_char(prcdtentrada, 'DD/MM/YYYY') 
	                END 
	            END
			END as dataultimocadastro,
			CASE WHEN data is not null 
				THEN to_char(data	, 'DD/MM/YYYY')
				ELSE '-' 
			END as dttramite,
			tpr.tipdsc as prioridade,
			CASE WHEN prc.prcprioritario IS TRUE
				THEN 'SIM'
				ELSE ''
			END as prioritario,           	
		    CASE WHEN ( esp.espnumdiasrespexterna IS NOT NULL ) 
				THEN 
				CASE WHEN ( ( current_date - esp.espnumdiasrespexterna )  > prcdtentrada ) 
					THEN '<font color=red title=\"Situação em atraso.\">' || esd.esddsc || '<br> (' || to_char(prcdtentrada::date + esp.espnumdiasrespexterna, 'DD/MM/YYYY' ) || ') </font>' 
					ELSE esd.esddsc || '<br> (' || to_char(prcdtentrada::date + esp.espnumdiasrespexterna, 'DD/MM/YYYY' ) || ')'
				END 
				ELSE esd.esddsc 
			END as esddsc,
			prc.prcid,
			coo.coodsc, 
			prc.usucpf 
	FROM 
		profeinep.processoprofeinep prc 
    LEFT JOIN profeinep.estruturaprocesso esp ON prc.prcid = esp.prcid 
    LEFT JOIN profeinep.coordenacao coo ON coo.coonid = prc.cooid
    LEFT JOIN workflow.documento doc ON doc.docid = esp.docid 
    LEFT JOIN (SELECT max(htddata) as data, docid FROM workflow.historicodocumento GROUP BY docid ) wd ON wd.docid = doc.docid
    LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
    LEFT JOIN profeinep.expressaochave exp ON exp.prcid = prc.prcid  
    LEFT JOIN profeinep.procedencia pro ON pro.proid = prc.proid
	LEFT JOIN profeinep.tipoprioridade tpr ON tpr.tipid = prc.tipid 
    LEFT JOIN profeinep.anexos anx ON anx.prcid = prc.prcid 
    LEFT JOIN profeinep.numeracaodocumento nud ON nud.nudid = anx.nudid
    LEFT JOIN profeinep.tipoacao tac ON tac.tacid = prc.tacid
	LEFT JOIN (
		SELECT
			hd.docid as docid, 
			ed.esddsc,
			ac.aeddscrealizada,
			us.usunome,
			hd.htddata as dataultimocadastro,
			cd.cmddsc
		FROM 
			workflow.historicodocumento hd
		INNER JOIN workflow.acaoestadodoc       ac ON ac.aedid = hd.aedid
		INNER JOIN workflow.estadodocumento     ed ON ed.esdid = ac.esdidorigem
		INNER JOIN seguranca.usuario 	        us ON us.usucpf = hd.usucpf
		LEFT  JOIN workflow.comentariodocumento cd ON cd.hstid = hd.hstid
		WHERE
			ac.esdidorigem = 77
			AND hd.htddata = (
					SELECT  	
						max(hd2.htddata) as htddata
					FROM 
						workflow.historicodocumento hd2
					INNER JOIN workflow.acaoestadodoc 	   ac ON ac.aedid = hd2.aedid
					INNER JOIN workflow.estadodocumento    ed ON ed.esdid = ac.esdidorigem
					INNER JOIN seguranca.usuario 		   us ON us.usucpf = hd2.usucpf
					LEFT JOIN workflow.comentariodocumento cd ON cd.hstid = hd2.hstid
					WHERE
						hd2.docid = hd.docid
						AND ac.esdidorigem = 77
					)
		) AS wultimadata ON wultimadata.docid = doc.docid
	WHERE 
	    0=0 
	{$condicoes} 
	GROUP BY prc.prcid, prcnumsidoc, prcnomeinteressado, prcdtentrada,tpr.tipdsc, prc.prcprioritario, esd.esddsc, prc.usucpf,coo.coodsc , esd.esdid, tpr.tipid, dataultimocadastro, esp.espnumdiasrespexterna, data
	ORDER BY tpr.tipid ";


if($_REQUEST['esdid'] || $_POST['requisicao'] == 'pesquisarprojeto'  || $_GET['todos']){
	$dados = $db->carregar($sql);	
	
	if( $dados ){		
		foreach($dados as $chave => $val){	
				
				if( $val['prioritario']=='t' ) {
					$prioritario = "<img src=\"/imagens/check_p.gif\" style=\"cursor:pointer\"  border=\"0\" align=\"absmiddle\" title=\"Prioritário.\" />"; 
				} else {
					$prioritario = " ";
				} 
				
				$dados_array[$chave] = array("num_processo" 	=> $val['prcnumsidoc']." ", 
											 "interessado" 		=> $val['prcnomeinteressado']." ",
											 "prioridade" 		=> $val['prioridade'],
											 "prioritario" 		=> $prioritario,
											 "data" 			=> $val['dataultimocadastro'],
											 "dttramite" 		    => $val['dttramite'],
											 "coord" 			=> $val['coodsc'],
											 "situacao" 		=> $val['esddsc']
				);
		}
	} 
	
	$coluna = Array(Array('campo'	 => 'prcnomeinteressado',
	  					  'label'	 => 'Interessado',
						  'blockAgp' => '',
						  'type'     => 'string'),
					Array('campo'	 => 'prioridade',
	  					  'label'	 => 'Prioridade',
						  'blockAgp' => '',
						  'type'     => 'string'),
					Array('campo'	 => 'prioritario',
	  					  'label'	 => 'Prioritário',
						  'blockAgp' => '',
						  'type'     => 'string'),
					Array('campo'	 => 'dataultimocadastro',
	  					  'label'	 => 'Data de Entrada',
						  'blockAgp' => '',
						  'type'     => 'string'),
					Array('campo'	 => 'dttramite',
	  					  'label'	 => 'Data da Última Tramitação',
						  'blockAgp' => '',
						  'type'     => 'string'),
					Array('campo'	 => 'coodsc',
	  					  'label'	 => 'Coordenação',
						  'blockAgp' => '',
						  'type'     => 'string'),
					Array('campo'	 => 'esddsc',
	  					  'label'	 => 'Situação PROFE/INEP',
						  'blockAgp' => '',
						  'type'     => 'string'));
					
	$agrupador = Array('agrupador' 		=> Array(Array('campo'=>'prcnumsidoc','label'=>'Nº do Processo SIDOC')),
					   'agrupadoColuna' => Array('prcnomeinteressado',
					   							 'prioridade',
					   							 'prioritario',
					   							 'dataultimocadastro',
					   							 'dttramite',
					   							 'coodsc',
					   							 'esddsc'));
	
	$rel->setAgrupador($agrupador, $dados); 
	$rel->setColuna($coluna);
	$rel->setTolizadorLinha(false);
	$rel->setEspandir(true);
	$rel->setMonstrarTolizadorNivel(true);
?>
<html>
	<head>
		<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		
		<!--  Monta o Relatório -->
		<? echo $rel->getRelatorio(); ?>
		
	</body>
</html>
<?php 
}else{
	?>
<script>
	alert('A lista não possui registro.');
	window.close();
</script>
<?php 	
}?>
