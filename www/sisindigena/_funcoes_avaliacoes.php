<?

function sqlAvaliacaoCoordenadorIES($dados) {
	global $db;
	
	$sql = "SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					i.picid,
					(SELECT m.docid FROM sisindigena.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid,
					CASE WHEN (SELECT esdid FROM sisindigena.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoCoordenadorAdjuntoIES&uncid=".$dados['uncid']."&picid='||i.picid||'&iusd='||i.iusd||'' as mais 
			FROM sisindigena.identificacaousuario i
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sisindigena.nucleouniversidade n ON n.picid = i.picid 
			INNER JOIN sisindigena.folhapagamentouniversidade fu ON fu.picid = n.picid AND fu.fpbid='".$dados['fpbid']."' AND fu.uncid='".$dados['uncid']."'
			WHERE t.pflcod='".PFL_COORDENADORADJUNTOIES."' AND n.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
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
   				   (SELECT m.docid FROM sisindigena.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid
			FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sisindigena.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sisindigena.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_PROFESSORALFABETIZADOR." AND tt.iusd='".$dados['iusd']."' AND i.iusstatus='A'";
	
	return $sql;
}

function sqlAvaliacaoSupervisor($dados) {
	global $db;
	
	$sql = "(
			
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc,
					(SELECT m.docid FROM sisindigena.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid, 
					CASE WHEN (SELECT esdid FROM sisindigena.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					''::text as mais 
			FROM sisindigena.identificacaousuario i
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod IN('".PFL_ORIENTADORESTUDO."','".PFL_PROFESSORALFABETIZADOR."') AND i.picid='".$dados['picid']."' AND 
				  (i.iusd IN(SELECT ot.iusd FROM sisindigena.orientadorturma ot INNER JOIN sisindigena.turmas t ON t.turid = ot.turid WHERE t.iusd='".$dados['iusd']."') OR
				   i.iusd IN(SELECT ot.iusd FROM sisindigena.orientadorturma ot INNER JOIN sisindigena.turmas t ON t.turid = ot.turid WHERE t.iusd IN(SELECT ot.iusd FROM sisindigena.orientadorturma ot INNER JOIN sisindigena.turmas t ON t.turid = ot.turid WHERE t.iusd='".$dados['iusd']."'))
				   ) AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
			
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
				   CASE WHEN (SELECT esdid FROM sisindigena.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
				   (SELECT m.docid FROM sisindigena.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid,
				   '&functionavaliacao=sqlAvaliacaoOrientador&iusd='||i.iusd||'' as mais
			FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sisindigena.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND i.picid='".$dados['picid']."' AND i.iusstatus='A' AND tt.iusd='".$dados['iusd']."' AND iusformacaoinicialorientador=true 
			
			)
			";
	
	return $sql;
}

function sqlAvaliacaoCoordenadorLocal($dados) {
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
					(SELECT m.docid FROM sisindigena.mensario m WHERE iusd=faa.iusd AND fpbid='".$dados['fpbid']."') as docid 
			FROM (
			
			SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc,
				   CASE WHEN (SELECT esdid FROM sisindigena.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
				   (SELECT m.docid FROM sisindigena.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid,
				   '&functionavaliacao=sqlAvaliacaoOrientador&iusd='||i.iusd||'' as mais
			FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND i.iusstatus='A' AND p.picid='".$dados['picid']."' AND iusformacaoinicialorientador=true 
			
			) faa 
			LEFT JOIN sisindigena.mensario m ON m.iusd = faa.iusd  AND m.fpbid='".$dados['fpbid']."'
			LEFT JOIN sisindigena.mensarioavaliacoes ma 
				INNER JOIN sisindigena.tipoperfil tp ON tp.iusd = ma.iusdavaliador AND tp.pflcod=".PFL_COORDENADORLOCAL."
			ON ma.menid = m.menid
			WHERE (ma.mavid IS NULL OR ma.iusdavaliador='".$dados['iusd']."')
			
			)";
	
	return $sql;
}

function sqlAvaliacaoCoordenadorAdjuntoIES($dados) {
	global $db;
	
	$sql = "SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sisindigena.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					(SELECT m.docid FROM sisindigena.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid,
					CASE WHEN t.pflcod=".PFL_SUPERVISORIES." THEN '&functionavaliacao=sqlAvaliacaoSupervisor&uncid='||i.uncid||'&picid='||i.picid||'&iusd='||i.iusd||'' 
						 ELSE '' END as mais 
			FROM sisindigena.identificacaousuario i
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sisindigena.folhapagamentouniversidade fu ON fu.picid = i.picid AND fu.fpbid='".$dados['fpbid']."' AND fu.picid='".$dados['picid']."'
			WHERE t.pflcod IN('".PFL_CONTEUDISTA."','".PFL_PESQUISADOR."','".PFL_FORMADORIES."','".PFL_COORDENADORLOCAL."','".PFL_SUPERVISORIES."') AND i.iusstatus='A' AND i.picid='".$dados['picid']."' ORDER BY p.pflcod, i.iusnome
			
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
				   CASE WHEN (SELECT esdid FROM sisindigena.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
				   (SELECT m.docid FROM sisindigena.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid,
				   '&functionavaliacao=sqlAvaliacaoCoordenadorIES&iusd='||i.iusd||'' as mais
			FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_COORDENADORIES." AND i.iusstatus='A' AND i.uncid='".$dados['uncid']."'
			
			)";
	
	return $sql;
}

?>