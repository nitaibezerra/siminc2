<?

function sqlAvaliacaoCoordenadorIES($dados) {
	global $db;
	
	$sql = "(
			SELECT  faa.iusd, 
					faa.iuscpf, 
					faa.iusnome, 
					faa.iusemailprincipal, 
					faa.pflcod,
					faa.pfldsc, 
					faa.mon,
					(SELECT m.docid FROM sispacto.mensario m WHERE iusd=faa.iusd AND fpbid='".$dados['fpbid']."') as docid,
					faa.mais FROM (
			(
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sispacto.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoSupervisor&uncid=".$dados['uncid']."&iusd='||i.iusd||'' as mais 
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_SUPERVISORIES."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
			
			) UNION ALL (
			
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sispacto.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoCoordenadorAdjuntoIES&uncid=".$dados['uncid']."&iusd='||i.iusd||'' as mais 
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_COORDENADORADJUNTOIES."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
			
			
			) UNION ALL (
			
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sispacto.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoCoordenadorLocal&picid='||i.picid||'&iusd='||i.iusd as mais
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sispacto.pactoidadecerta pa ON pa.picid = i.picid 
			WHERE t.pflcod='".PFL_COORDENADORLOCAL."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND pa.muncod IS NOT NULL ORDER BY p.pflcod, i.iusnome
			
			) UNION ALL (

			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sispacto.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='1') IN('601','657') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoCoordenadorLocal&iusd='||i.iusd||'&picid='||pa.picid as mais
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sispacto.pactoidadecerta pa ON pa.picid = i.picid 
			WHERE t.pflcod='".PFL_COORDENADORLOCAL."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND pa.estuf IS NOT NULL ORDER BY p.pflcod, i.iusnome
			
			)
			
			) faa 
			LEFT JOIN sispacto.mensario m ON m.iusd = faa.iusd 
			LEFT JOIN sispacto.mensarioavaliacoes ma ON ma.menid = m.menid
			WHERE (ma.mavtotal IS NULL OR ma.iusdavaliador='".$dados['iusd']."') AND m.fpbid='".$dados['fpbid']."'
			)
			";
	
	return $sql;
}

function sqlAvaliacaoOrientador($dados) {
	global $db;
	
	$sql = "SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc,
   				   'TRUE'::text as mon,
   				   ''::text as mais,
   				   (SELECT m.docid FROM sispacto.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid
			FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sispacto.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_PROFESSORALFABETIZADOR." AND tt.iusd='".$dados['iusd']."' AND i.iusstatus='A'";
	
	return $sql;
}

function sqlAvaliacaoSupervisor($dados) {
	global $db;
	
	$sql = "(
			SELECT  faa.iusd, 
					faa.iuscpf, 
					faa.iusnome, 
					faa.iusemailprincipal, 
					faa.pflcod,
					faa.pfldsc, 
					faa.mon,
					faa.mais,
					(SELECT m.docid FROM sispacto.mensario m WHERE iusd=faa.iusd AND fpbid='".$dados['fpbid']."') as docid
					 
			FROM (
			(
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sispacto.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoFormador&iusd='||i.iusd||'&uncid=".$dados['uncid']."' as mais 
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_FORMADORIES."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
			
			) UNION ALL (
			
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sispacto.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoCoordenadorLocal&iusd='||i.iusd||'&picid='||pa.picid as mais
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sispacto.pactoidadecerta pa ON pa.picid = i.picid 
			WHERE t.pflcod='".PFL_COORDENADORLOCAL."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND pa.muncod IS NOT NULL ORDER BY p.pflcod, i.iusnome
			
			) UNION ALL (
			
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sispacto.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='1') IN('601','657') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoCoordenadorLocal&iusd='||i.iusd||'&picid='||pa.picid as mais
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sispacto.pactoidadecerta pa ON pa.picid = i.picid 
			WHERE t.pflcod='".PFL_COORDENADORLOCAL."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND pa.estuf IS NOT NULL ORDER BY p.pflcod, i.iusnome
			
			
			)
			
			
			) faa 
			LEFT JOIN sispacto.mensario m ON m.iusd = faa.iusd 
			LEFT JOIN sispacto.mensarioavaliacoes ma ON ma.menid = m.menid
			WHERE (ma.mavid IS NULL OR ma.iusdavaliador='".$dados['iusd']."') AND m.fpbid='".$dados['fpbid']."' 
			)
			";
	return $sql;
}

function sqlAvaliacaoFormador($dados) {
	global $db;
	
	$sql = "(
			
			SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc,
				   CASE WHEN (SELECT esdid FROM sispacto.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
				   (SELECT m.docid FROM sispacto.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid,
				   '&functionavaliacao=sqlAvaliacaoOrientador&iusd='||i.iusd||'' as mais
			FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sispacto.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND tt.iusd='".$dados['iusd']."' AND iusformacaoinicialorientador=true 
			
			)
			";
	
	return $sql;
}

function sqlAvaliacaoCoordenadorLocal($dados) {
	global $db;
	
	$sql = "(
	
			SELECT  			 
					faa.* 
			FROM (
			
			SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc,
				   CASE WHEN (SELECT esdid FROM sispacto.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
				   (SELECT m.docid FROM sispacto.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid,
				   '&functionavaliacao=sqlAvaliacaoOrientador&iusd='||i.iusd||'' as mais
			FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND i.iusstatus='A' AND p.picid='".$dados['picid']."' AND iusformacaoinicialorientador=true 
			
			) faa 
			LEFT JOIN sispacto.mensario m ON m.iusd = faa.iusd  AND m.fpbid='".$dados['fpbid']."'
			LEFT JOIN sispacto.mensarioavaliacoes ma 
				INNER JOIN sispacto.tipoperfil tp ON tp.iusd = ma.iusdavaliador AND tp.pflcod=".PFL_COORDENADORLOCAL."
			ON ma.menid = m.menid
			WHERE (ma.mavid IS NULL OR ma.iusdavaliador='".$dados['iusd']."')
			
			)";
	
	return $sql;
}

function sqlAvaliacaoCoordenadorAdjuntoIES($dados) {
	global $db;
	
	$sql = "(
			SELECT  faa.iusd, 
					faa.iuscpf, 
					faa.iusnome, 
					faa.iusemailprincipal, 
					faa.pflcod,
					faa.pfldsc, 
					faa.mon,
					(SELECT m.docid FROM sispacto.mensario m WHERE iusd=faa.iusd AND fpbid='".$dados['fpbid']."') as docid,
					faa.mais FROM (
			(
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sispacto.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoSupervisor&uncid='||i.uncid||'&iusd='||i.iusd||'' as mais 
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_SUPERVISORIES."' AND i.iusstatus='A' AND i.uncid='".$dados['uncid']."' ORDER BY p.pflcod, i.iusnome
			
			) UNION ALL (
			
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sispacto.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoCoordenadorLocal&picid='||i.picid||'&iusd='||i.iusd||'' as mais 
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sispacto.pactoidadecerta pa ON pa.picid = i.picid 
			WHERE t.pflcod='".PFL_COORDENADORLOCAL."' AND i.iusstatus='A' AND i.uncid='".$dados['uncid']."' AND pa.muncod IS NOT NULL ORDER BY p.pflcod, i.iusnome
			
			) UNION ALL (
			
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sispacto.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='1') IN('601','657') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoCoordenadorLocal&iusd='||i.iusd||'&picid='||pa.picid as mais
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sispacto.pactoidadecerta pa ON pa.picid = i.picid 
			WHERE t.pflcod='".PFL_COORDENADORLOCAL."' AND i.iusstatus='A' AND i.uncid='".$dados['uncid']."' AND pa.estuf IS NOT NULL ORDER BY p.pflcod, i.iusnome
			
			
			)
			
			
			) faa 
			LEFT JOIN sispacto.mensario m ON m.iusd = faa.iusd 
			LEFT JOIN sispacto.mensarioavaliacoes ma ON ma.menid = m.menid
			WHERE (ma.mavtotal IS NULL OR ma.iusdavaliador='".$dados['iusd']."') AND m.fpbid='".$dados['fpbid']."'
			)
			";
	return $sql;
}

function sqlAvaliacaoMEC($dados) {
	global $db;
	
	$sql = "(
			
			SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc,
				   CASE WHEN (SELECT esdid FROM sispacto.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
				   (SELECT m.docid FROM sispacto.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid,
				   '&functionavaliacao=sqlAvaliacaoCoordenadorIES&iusd='||i.iusd||'' as mais
			FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_COORDENADORIES." AND i.iusstatus='A' AND i.uncid='".$dados['uncid']."'
			
			)";
	
	return $sql;
}

?>