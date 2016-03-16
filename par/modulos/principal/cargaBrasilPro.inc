<?php
set_time_limit(0);
ini_set("memory_limit", "12000M");

global $db;

$sql = "SELECT DISTINCT ppsid::numeric FROM carga.cargabrasilpro cbp -- where ppsid = '1225' ";
$arrPpsid = $db->carregarColuna($sql);

foreach( $arrPpsid as $ppsidAntigo ){
	
	// PPSID
	$sql = "SELECT DISTINCT 
					ppsdsc, (ppsordem + 100) as ppsordem, frmid, prgid, indid, ppsestrategiaimplementacao, 
			       undid, ppspeso, foaid, ppsobra, ppsptres, ppsnaturezadespesa, 
			       1 as ppscronograma, ppsmonitora, ptsid, ppsobjetivo, ppstexto, ppscobertura, 
			       ppscarga
			FROM
				par.propostasubacao pps
			WHERE
				pps.ppsid = ".$ppsidAntigo;
	$dadosOriginal = $db->pegaLinha($sql);

	$sql = "INSERT INTO par.propostasubacao(
            ppsdsc, ppsordem, frmid, prgid, indid, ppsestrategiaimplementacao, 
            undid, ppspeso, foaid, ppsobra, ppsptres, ppsnaturezadespesa, 
            ppscronograma, ppsmonitora, ptsid, ppsobjetivo, ppstexto, ppscobertura, 
            ppscarga) VALUES (
					'".$dadosOriginal['ppsdsc']."', ".$dadosOriginal['ppsordem'].", ".$dadosOriginal['frmid'].", ".$dadosOriginal['prgid'].", ".$dadosOriginal['indid'].", '".$dadosOriginal['ppsestrategiaimplementacao']."', 
			       ".$dadosOriginal['undid'].", ".$dadosOriginal['ppspeso'].", ".($dadosOriginal['foaid']?$dadosOriginal['foaid']:'null').", ".$dadosOriginal['ppsobra'].", ".$dadosOriginal['ppsptres'].", ".$dadosOriginal['ppsnaturezadespesa'].", 
			       ".$dadosOriginal['ppscronograma'].", '".$dadosOriginal['ppsmonitora']."', ".$dadosOriginal['ptsid'].", '".$dadosOriginal['ppsobjetivo']."', '".$dadosOriginal['ppstexto']."', '".$dadosOriginal['ppscobertura']."', 
			       '".$dadosOriginal['ppscarga']."' )
			RETURNING ppsid";
	$ppsidNovo = $db->pegaUm($sql);
	
	// ATUALIZO A MINHA TABELA DE CONTROLE
	$sql = "UPDATE carga.cargabrasilpro SET ppsidnovo = ".$ppsidNovo." WHERE ppsid = '".$ppsidAntigo."'";
	$db->executar($sql);
	
	// ANOS
	$sql = "INSERT INTO par.propostasubacaoanos ( ppsid, praid )
			SELECT ".$ppsidNovo.", praid FROM par.propostasubacaoanos WHERE ppsid = ".$ppsidAntigo;
	$db->executar($sql);
	
	// PONTUAวรO
	$sql = "INSERT INTO par.criteriopropostasubacao ( ppsid, crtid )
			SELECT ".$ppsidNovo.", crtid FROM par.criteriopropostasubacao WHERE ppsid = ".$ppsidAntigo;
	$db->executar($sql);
	
	// ITENS
	$sql = "INSERT INTO par.propostasubacaoitem ( ppsid, picid )
			SELECT ".$ppsidNovo.", picid FROM par.propostasubacaoitem WHERE ppsid = ".$ppsidAntigo;
	$db->executar($sql);
	
	// GRUPOS
	$sql = "INSERT INTO par.propostagrupoitem ( ppsid, gicid )
			SELECT ".$ppsidNovo.", gicid FROM par.propostagrupoitem WHERE ppsid = ".$ppsidAntigo;
	$db->executar($sql);
	
}
?>