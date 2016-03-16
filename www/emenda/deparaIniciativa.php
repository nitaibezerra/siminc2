<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

/*$_REQUEST['baselogin'] = "simec_espelho_producao";*/

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$arIniciativa[] = array("anterior" => "8", "atual" => "51", "resid" => 2);						
$arIniciativa[] = array("anterior" => "9", "atual" => "53", "resid" => 2);						
$arIniciativa[] = array("anterior" => "44", "atual" => "56", "resid" => 2);
$arIniciativa[] = array("anterior" => "43", "atual" => "58", "resid" => 2);
$arIniciativa[] = array("anterior" => "16", "atual" => "60", "resid" => 2);
$arIniciativa[] = array("anterior" => "17", "atual" => "63", "resid" => 2);
						
$arIniciativa[] = array("anterior" => "9", "atual" => "52", "resid" => 1);
$arIniciativa[] = array("anterior" => "45", "atual" => "54", "resid" => 1);
$arIniciativa[] = array("anterior" => "44", "atual" => "55", "resid" => 1);
$arIniciativa[] = array("anterior" => "14", "atual" => "50", "resid" => 1);
$arIniciativa[] = array("anterior" => "43", "atual" => "57", "resid" => 1);
$arIniciativa[] = array("anterior" => "16", "atual" => "59", "resid" => 1);
$arIniciativa[] = array("anterior" => "46", "atual" => "61", "resid" => 1);
$arIniciativa[] = array("anterior" => "17", "atual" => "62", "resid" => 1);

foreach ($arIniciativa as $v) {
	
	$sql = "UPDATE emenda.iniciativaemendadetalhe SET iniid = ".$v['atual']."
			WHERE 
			  iedid in (SELECT ied.iedid
			            FROM
			                emenda.emenda e
			                inner join emenda.emendadetalhe ed
			                    on e.emeid = ed.emeid
			                inner join emenda.iniciativaemendadetalhe ied
			                    on ied.emdid = ed.emdid
			                inner join emenda.iniciativa i
			                    on i.iniid = ied.iniid
			            WHERE
			                e.resid = ".$v['resid']."
			                and ied.iniid = ".$v['anterior'].")";
	$db->executar( $sql );
	
	$sql = "UPDATE emenda.iniciativadetalheentidade SET iniid = ".$v['atual']."
			WHERE ideid in (SELECT ide.ideid
							FROM
								emenda.emenda e
							    inner join emenda.emendadetalhe ed
							    	on e.emeid = ed.emeid
							    inner join emenda.emendadetalheentidade ede
							    	on ede.emdid = ed.emdid
							    inner join emenda.iniciativadetalheentidade ide
							    	on ide.edeid = ede.edeid
							    inner join emenda.iniciativa i
							    	on i.iniid = ide.iniid
							WHERE
								e.resid = ".$v['resid']."
							    and ide.iniid = ".$v['anterior'].")";
	$db->executar( $sql );
	
	$sql = "UPDATE emenda.ptiniciativa SET iniid = ".$v['atual']."
			WHERE ptiid in ( SELECT DISTINCT pti.ptiid
								FROM
								    emenda.planotrabalho ptr
								    inner join emenda.ptiniciativa pti
								    	on pti.ptrid = ptr.ptrid
								    inner join emenda.ptiniciativaespecificacao ptie
								    	on ptie.ptiid = pti.ptiid
								    inner join emenda.iniciativaespecificacao ie
								    	on ie.iceid = ptie.iceid
								WHERE
									ptr.resid = ".$v['resid']."
								    and ie.iniid = ".$v['anterior'].")";
	$db->executar( $sql );
	/*
	$sql = "UPDATE emenda.iniciativabeneficiario SET iniid = ".$v['atual']." 
			WHERE icbid in (SELECT DISTINCT ib.icbid
							FROM
							    emenda.planotrabalho ptr
							    inner join emenda.ptiniciativa pti
							        on pti.ptrid = ptr.ptrid
							    inner join emenda.ptiniciativabeneficiario ptib
							        on ptib.ptiid = pti.ptiid
							    inner join emenda.iniciativabeneficiario ib
							        on ib.icbid = ptib.icbid
							WHERE
							    ptr.resid = ".$v['resid']."
							    and ib.iniid = ".$v['anterior'].")";
	$db->executar( $sql );	
	*/
}
if($db->commit()){
	echo 'ok...';
} else {
	echo 'Falhou...';
}
?>