<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();

// QUADRA
$sql = "SELECT distinct uf ,construcao FROM carga.novasobraspacestaduais  "; //  
$qtdObrasdeConstrucaoPorUF = $db->carregar($sql);

foreach($qtdObrasdeConstrucaoPorUF AS $dados){
	$sql = "SELECT  (COUNT(pre.preid) + 1) as total
					-- CASE WHEN MAX(pre.preprioridade::numeric) IS NULL 
					-- THEN COUNT(pre.preid) + 1 ELSE MAX(pre.preprioridade::numeric) END as total 
					
				FROM obras.preobra pre
				INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
				LEFT JOIN workflow.documento doc ON doc.docid = pre.docid
				LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
				WHERE 
					pre.estufpar = '{$dados['uf']}'
					AND pto.ptoesfera in ('E','T') AND pre.preesfera = 'E' AND pre.muncodpar IS NULL
					AND pto.ptoclassificacaoobra = 'Q' 
					AND pre.presistema = '23'
					AND pto.ptoclassificacaoobra = 'Q'
					AND pre.prestatus = 'A'
					AND pre.tooid =1
					AND pre.preidpai IS NULL";
	$numeracao = $db->pegaUm($sql);
	if($numeracao == false){
		$numeracao = 1;
	}
	
	$x = 1;
	if($dados['construcao'] > 0){
		while ( $x <= $dados['construcao']){
			$sql = "INSERT INTO workflow.documento  (tpdid, esdid, docdsc, docdatainclusao) VALUES ( 37, 193, 'Fluxo pró infância', now() ) returning docid;";
			$docid = $db->pegaUm($sql);
			
			$sql = "INSERT INTO obras.preobra(
	            				presistema, preidsistema, ptoid, estuf, predtinclusao, preano, 
	            				predescricao, prestatus, preprioridade, tooid, estufpar,
	            				premcmv, preanoselecao, preesfera, docid )
				 	VALUES (23, 23, 27, '{$dados['uf']}', now(), 2013, 
				            'PAC 2 - Construção de Quadra Escolar Coberta 0' || {$numeracao} , 'A', {$numeracao}, 1, '{$dados['uf']}',
				            false, 2013, 'E', {$docid});";
			
			
			//dbg($x); dbg($dados['uf']);
			$db->executar($sql);
			$x++;
			$numeracao++;
		}
	}
	echo("Inserido ".($x - 1)." QUADRAS para o Estado do ".$dados['uf'] );
	echo "<br>";
}
$db->commit();


// COBERTURA

$sql = "SELECT distinct uf ,cobertura FROM carga.novasobraspacestaduais "; //    WHERE uf = 'AC'
$qtdObrasdeCoberturaPorUF = $db->carregar($sql);

foreach($qtdObrasdeCoberturaPorUF AS $dados){
	$sql = "SELECT (COUNT(pre.preid) + 1) as total
				--	CASE WHEN MAX(pre.preprioridade::numeric) IS NULL 
				-- THEN COUNT(pre.preid) + 1 ELSE MAX(pre.preprioridade::numeric) END as total 
				FROM obras.preobra pre
				INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
				LEFT JOIN workflow.documento doc ON doc.docid = pre.docid
				LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
				WHERE 
					pre.estufpar = '{$dados['uf']}'
					AND pto.ptoesfera in ('E','T') AND pre.muncodpar IS NULL AND pre.preesfera = 'E'
					AND pre.presistema = '23'
					AND pto.ptoclassificacaoobra = 'C'
					AND pre.prestatus = 'A'
					AND pre.tooid =1
					AND pre.preidpai IS NULL	
			 ";
	$numeracaoCobertura = $db->pegaUm($sql);
	if($numeracaoCobertura == false){
		$numeracaoCobertura = 1;
	}

	$xc = 1;
	if($dados['cobertura'] > 0){
		while ( $xc <= $dados['cobertura']){
			$sql = "INSERT INTO workflow.documento  (tpdid, esdid, docdsc, docdatainclusao) VALUES ( 37, 193, 'Fluxo pró infância', now() ) returning docid;";
			$docidC = $db->pegaUm($sql);
			
			$sql = "INSERT INTO obras.preobra(
						presistema, preidsistema, ptoid, estuf, predtinclusao, preano, 
						predescricao, prestatus, preprioridade, tooid, estufpar,
						premcmv, preanoselecao, preesfera, docid )
					VALUES (23, 23, 21, '{$dados['uf']}', now(), 2013, 
					    'PAC 2 - Cobertura de Quadra Escolar 0' || {$numeracaoCobertura} , 'A', {$numeracaoCobertura}, 1, '{$dados['uf']}',
					    false, 2013, 'E', {$docidC});";
			
			//dbg($x); dbg($dados['uf']);
			$db->executar($sql);
			$xc++;
			$numeracaoCobertura++;
		}
	}
	echo("Inserido ".($xc - 1)." COBERTURAS para o Estado do ".$dados['uf'] );
	echo "<br>";
}
$db->commit();

?>