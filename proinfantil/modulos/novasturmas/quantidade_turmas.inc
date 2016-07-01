<?php
	global $db;
	
	$sql = "SELECT 			DISTINCT(t.muncod), m.estuf, m.mundescricao, 
							COALESCE(cp.creche_publica,0) as creche_publica, 
							COALESCE(cc.creche_conveniada,0) as creche_conveniada, 
							COALESCE(pp.preescola_publica,0) as preescola_publica, 
							COALESCE(pc.preescola_conveniada,0) as preescola_conveniada, 
							COALESCE(up.unificada_publica,0) as unificada_publica, 
							COALESCE(uc.unificada_conveniada,0) as unificada_conveniada
			FROM 			proinfantil.turma t
			INNER JOIN 		(SELECT count(turid) as creche_publica, muncod from proinfantil.turma 
							where turstatus = 'A'  AND turtipoestabelecimento = 1 AND ttuid = 1
							group by muncod) as cp on cp.muncod = t.muncod
			LEFT JOIN 		(SELECT count(turid) as creche_conveniada, muncod from proinfantil.turma 
							where turstatus = 'A'  AND turtipoestabelecimento = 1 AND ttuid = 2
							group by muncod) as cc on cc.muncod = t.muncod
			LEFT JOIN 		(SELECT count(turid) as preescola_publica, muncod from proinfantil.turma 
							where turstatus = 'A'  AND turtipoestabelecimento = 2 AND ttuid = 1
							group by muncod) as pp on pp.muncod = t.muncod
			LEFT JOIN 		(SELECT count(turid) as preescola_conveniada, muncod from proinfantil.turma 
							where turstatus = 'A'  AND turtipoestabelecimento = 2 AND ttuid = 2
							group by muncod) as pc on pc.muncod = t.muncod
			LEFT JOIN 		(SELECT count(turid) as unificada_publica, muncod from proinfantil.turma 
							where turstatus = 'A'  AND turtipoestabelecimento = 3 AND ttuid = 1
							group by muncod) as up on up.muncod = t.muncod
			LEFT JOIN 		(SELECT count(turid) as unificada_conveniada, muncod from proinfantil.turma 
							where turstatus = 'A'  AND turtipoestabelecimento = 3 AND ttuid = 2
							group by muncod) as uc on uc.muncod = t.muncod
			INNER JOIN 		territorios.municipio m ON t.muncod = m.muncod 
			WHERE 			t.turstatus = 'A'
			GROUP BY 		t.muncod, m.mundescricao, m.estuf, cp.creche_publica, cc.creche_conveniada, 
							pp.preescola_publica, pc.preescola_conveniada, up.unificada_publica, uc.unificada_conveniada
			ORDER BY  		m.estuf, m.mundescricao";
	
	$turmas = $db->carregar($sql); ?>
	<table>
		<tr>
			<td>Código IBGE</td>
			<td>Município</td>
			<td>Creche Pública (Permitida/Cadastrada)</td>
			<td>Creche Conveniada (Permitida/Cadastrada)</td>
			<td>Pré-Escola Pública (Permitida/Cadastrada)</td>
			<td>Pré-Escola Conveniada (Permitida/Cadastrada)</td>
			<td>Unificada Pública (Permitida/Cadastrada)</td>
			<td>Unificada Conveniada (Permitida/Cadastrada)</td>
		</tr>
<?php 
	foreach($turmas as $t){
	$sql = "SELECT
				ntmid,
				-- Turmas
				COALESCE(SUM(ntcqtdturmacrechepublica),0) as ntcqtdturmacrechepublica,
				COALESCE(SUM(ntcqtdturmapreescolapublica),0) as ntcqtdturmapreescolapublica,
				COALESCE(SUM(ntcqtdturmaunificadapublica),0) as ntcqtdturmaunificadapublica,
				COALESCE(SUM(ntcqtdturmacrecheconveniada),0) as ntcqtdturmacrecheconveniada,
				COALESCE(SUM(ntcqtdturmapreescolaconveniada),0) as ntcqtdturmapreescolaconveniada,
				COALESCE(SUM(ntcqtdturmaunificadaconveniada),0) as ntcqtdturmaunificadaconveniada,
				COALESCE(ntmqtdturmacrechepublica,0) as ntmqtdturmacrechepublica,
				COALESCE(ntmqtdturmapreescolapublica,0) as ntmqtdturmapreescolapublica,
				COALESCE(ntmqtdturmaunificadapublica,0) as ntmqtdturmaunificadapublica,
				COALESCE(ntmqtdturmacrecheconveniada,0) as ntmqtdturmacrecheconveniada,
				COALESCE(ntmqtdturmapreescolaconveniada,0) as ntmqtdturmapreescolaconveniada,
				COALESCE(ntmqtdturmaunificadaconveniada,0) as ntmqtdturmaunificadaconveniada
					
			FROM proinfantil.novasturmasdadoscenso dc
			LEFT JOIN proinfantil.novasturmasdadosmunicipios dm on dm.muncod = dc.muncod and ntmano = '".(date('Y')-1)."'
			WHERE dc.muncod = '{$t['muncod']}'
			AND dc.ntcanocenso = '".(date('Y')-2)."'
			and dc.ntcstatus = 'A'
			GROUP BY
				ntmid,
					
				-- Matriculas
				ntmqtdmatriculacrecheparcialpublica,
				ntmqtdmatriculacrecheintegralpublica,
				ntmqtdmatriculapreescolaparcialpublica,
				ntmqtdmatriculapreescolaintegralpublica,
				ntmqtdmatriculacrecheparcialconveniada,
				ntmqtdmatriculacrecheintegralconveniada,
				ntmqtdmatriculapreescolaparcialconveniada,
				ntmqtdmatriculapreescolaintegralconveniada,
					
				-- Turmas
				ntmqtdturmacrechepublica,
				ntmqtdturmapreescolapublica,
				ntmqtdturmaunificadapublica,
				ntmqtdturmacrecheconveniada,
				ntmqtdturmapreescolaconveniada,
				ntmqtdturmaunificadaconveniada";
		
		$rsMatriculasTurmas  = $db->pegaLinha($sql);
		
		if($rsMatriculasTurmas){ 
			$qtdCreche    = $rsMatriculasTurmas['ntmqtdturmacrechepublica']-$rsMatriculasTurmas['ntcqtdturmacrechepublica'];
			$qtdCrecheC   = $rsMatriculasTurmas['ntmqtdturmacrecheconveniada']-$rsMatriculasTurmas['ntcqtdturmacrecheconveniada'];
			$totalCreche  = $qtdCreche + $qtdCrecheC;
		
			if($qtdCreche > 0){
				if($qtdCreche > $totalCreche){
					$qtdCrechePub = $qtdCreche + $qtdCrecheC;
				} else {
					$qtdCrechePub = $qtdCreche;
				}		
			} else {
				$qtdCrechePub = 0;
			}
		
			if($qtdCrecheC > 0){
				if($qtdCrecheC > $totalCreche){
					$qtdCrecheCon = $qtdCrecheC + $qtdCreche;
				} else {
					$qtdCrecheCon = $qtdCrecheC;
				}		
			} else {
				$qtdCrecheCon = 0;
			}
			
			$qtdPreEscola  = $rsMatriculasTurmas['ntmqtdturmapreescolapublica']-$rsMatriculasTurmas['ntcqtdturmapreescolapublica'];
			$qtdPreEscolaC = $rsMatriculasTurmas['ntmqtdturmapreescolaconveniada']-$rsMatriculasTurmas['ntcqtdturmapreescolaconveniada'];
			$totalPreEscola  = $qtdPreEscola + $qtdPreEscolaC;
		
			if($qtdPreEscola > 0){
				if($qtdPreEscola > $totalPreEscola){
					$qtdPreEscolaPub = $qtdPreEscola + $qtdPreEscolaC;
				} else {
					$qtdPreEscolaPub = $qtdPreEscola;
				}
			} else {
				$qtdPreEscolaPub = 0;
			}
		
			if($qtdPreEscolaC > 0){
				if($qtdPreEscolaC > $totalPreEscola){
					$qtdPreEscolaCon = $qtdPreEscolaC + $qtdPreEscola;
				} else {
					$qtdPreEscolaCon = $qtdPreEscolaC;
				}
			} else {
				$qtdPreEscolaCon = 0;
			}
		
			$qtdUnificada  = $rsMatriculasTurmas['ntmqtdturmaunificadapublica']-$rsMatriculasTurmas['ntcqtdturmaunificadapublica'];
			$qtdUnificadaC = $rsMatriculasTurmas['ntmqtdturmaunificadaconveniada']-$rsMatriculasTurmas['ntcqtdturmaunificadaconveniada'];
			$totalUnificada  = $qtdUnificada + $qtdUnificadaC;
			
			if($qtdUnificada > 0){
				if($qtdUnificada > $totalUnificada){
					$qtdUnificadaPub = $qtdUnificada + $qtdUnificadaC;
				} else {
					$qtdUnificadaPub = $qtdUnificada;
				}
			} else {
				$qtdUnificadaPub = 0;
			}
		
			if($qtdUnificadaC > 0){
				if($qtdUnificadaC > $totalUnificada){
					$qtdUnificadaCon = $qtdUnificadaC + $qtdUnificada;
				} else {
					$qtdUnificadaCon = $qtdUnificadaC;
				}
			} else {
				$qtdUnificadaCon = 0;
			} ?>
			<?php if($t['creche_publica'] > $qtdCrechePub || $t['creche_conveniada'] > $qtdCrecheCon || $t['preescola_publica'] > $qtdPreEscolaPub
				|| $t['preescola_conveniada'] > $qtdPreEscolaCon || $t['unificada_publica'] > $qtdUnificadaPub || $t['unificada_conveniada'] > $qtdUnificadaCon ){?>
			<tr>
				<td><?php echo $t['muncod'];?></td>
				<td><?php echo $t['estuf']."-".$t['mundescricao'];?></td>
				<td><?php echo $qtdCrechePub; ?>/<?php echo $t['creche_publica'];?></td>
				<td><?php echo $qtdCrecheCon; ?>/<?php echo $t['creche_conveniada'];?></td>
				<td><?php echo $qtdPreEscolaPub; ?>/<?php echo $t['preescola_publica'];?></td>
				<td><?php echo $qtdPreEscolaCon; ?>/<?php echo $t['preescola_conveniada'];?></td>
				<td><?php echo $qtdUnificadaPub; ?>/<?php echo $t['unificada_publica'];?></td>
				<td><?php echo $qtdUnificadaCon; ?>/<?php echo $t['unificada_conveniada'];?></td>
			</tr>	
			<?php } ?>
<?php } } ?>
</table>