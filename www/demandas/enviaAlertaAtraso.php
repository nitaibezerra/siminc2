<?php


	date_default_timezone_set ('America/Sao_Paulo');
	
	// controle o cache do navegador
	header( "Cache-Control: no-store, no-cache, must-revalidate" );
	header( "Cache-Control: post-check=0, pre-check=0", false );
	header( "Cache-control: private, no-cache" );   
	header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
	header( "Pragma: no-cache" );
	
	// carrega as funções gerais
	include_once "config.inc";
	include_once APPRAIZ . "includes/funcoes.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";
	
	// carrega as funções específicas do módulo
	include_once '_constantes.php';
	include_once '_funcoes.php';
	//include_once '_componentes.php';
	
	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();

	$ano_ini = date("Y");
	$mes_ini = date("m");
	$dia_ini = date("d");	
	$hor_ini = date("H");
	$min_ini = date("i");
	
	$dataini = mktime($hor_ini,$min_ini,0,$mes_ini,$dia_ini,$ano_ini);
	$datafim = mktime($hor_ini+1,$min_ini,0,$mes_ini,$dia_ini,$ano_ini);
	 
	$dataini = strftime("%Y-%m-%d %H:%M:%S", $dataini);
	$datafim = strftime("%Y-%m-%d %H:%M:%S", $datafim);			


	//EM ANALISE E EM ATENDIMENTO
	$sql = "		
			SELECT
				 --to_char(d.dmddatainclusao::date,'DD/MM/YYYY') ||' '|| to_char(d.dmddatainclusao, 'HH24:MI') AS dataabertura,
				 --to_char(d.dmddatafimprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatafimprevatendimento, 
				 --to_char(d.dmddatainiprevatendimento::timestamp,'YYYY-MM-DD HH24:MI:00') as dmddatainiprevatendimento
				 --d.dmddatainclusao, 
				 --d.dmddatainiprevatendimento,
				 --d.dmddatafimprevatendimento,
				 --ed.esdid as situacao,
				 d.dmdid
				FROM
				 demandas.demanda d
				 INNER JOIN demandas.tiposervico t ON t.tipid = d.tipid
				 INNER JOIN workflow.documento doc ON doc.docid = d.docid
				 INNER JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
				WHERE 
				  d.dmdstatus = 'A'	
				  AND doc.esdid in (91,92,107,108) 	
				  AND t.ordid = 1 -- (sistemas de informação)
				  AND d.dmddatafimprevatendimento BETWEEN '$dataini' AND '$datafim'
			";
	
	
	$dados = $db->carregar($sql);
	
	if($dados){
		
		foreach($dados as $d){
			
			enviaEmailAlertaDemandaAtraso($d['dmdid']);
		}
	}
	
	exit;
