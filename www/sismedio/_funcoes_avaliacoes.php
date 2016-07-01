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
					(SELECT m.docid FROM sismedio.mensario m WHERE iusd=faa.iusd AND fpbid='".$dados['fpbid']."') as docid,
					faa.mais FROM (
			(
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sismedio.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoSupervisor&uncid=".$dados['uncid']."&iusd='||i.iusd||'' as mais 
			FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_SUPERVISORIES."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
			
			) UNION ALL (
			
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sismedio.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoCoordenadorAdjuntoIES&uncid=".$dados['uncid']."&iusd='||i.iusd||'' as mais 
			FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_COORDENADORADJUNTOIES."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
			
			
			) 
			
			) faa 
			LEFT JOIN sismedio.mensario m ON m.iusd = faa.iusd 
			LEFT JOIN sismedio.mensarioavaliacoes ma ON ma.menid = m.menid AND (ma.mavtotal IS NULL OR ma.iusdavaliador='".$dados['iusd']."')
			WHERE  (m.fpbid='".$dados['fpbid']."' OR m.fpbid IS NULL)
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
   				   (SELECT m.docid FROM sismedio.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sismedio.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod IN(".PFL_PROFESSORALFABETIZADOR.",".PFL_COORDENADORPEDAGOGICO.") AND tt.iusd='".$dados['iusd']."' AND i.iusstatus='A'";
	
	return $sql;
}

function sqlAvaliacaoSupervisor($dados) {
	global $db;
	
	if($dados['fpbid']) {
	
		$sql = "
			(
			SELECT i.iusd
			FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_FORMADORIES."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
			
			) UNION ALL (

			SELECT i.iusd 
			FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_FORMADORREGIONAL."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome

			)";
	
		$iusds = $db->carregarColuna($sql);
	
		if($iusds) {
				
			foreach($iusds as $iusd) {
				criarMensario(array("iusd"=>$iusd,"fpbid"=>$dados['fpbid']));
			}
				
		}
	
	}
	
	$sql = "(
			SELECT  faa.iusd, 
					faa.iuscpf, 
					faa.iusnome, 
					faa.iusemailprincipal, 
					faa.pflcod,
					faa.pfldsc, 
					faa.mon,
					faa.mais,
					(SELECT m.docid FROM sismedio.mensario m WHERE iusd=faa.iusd AND fpbid='".$dados['fpbid']."') as docid
					 
			FROM (
			(
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sismedio.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoFormadorRegional&iusd='||i.iusd||'&uncid=".$dados['uncid']."' as mais 
			FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_FORMADORIES."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
			
			) UNION ALL (

			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sismedio.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoFormadorRegional&iusd='||i.iusd||'&uncid=".$dados['uncid']."' as mais 
			FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_FORMADORREGIONAL."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
					

			)
			
			
			) faa 
			LEFT JOIN sismedio.mensario m ON m.iusd = faa.iusd 
			LEFT JOIN sismedio.mensarioavaliacoes ma ON ma.menid = m.menid
			WHERE m.fpbid='".$dados['fpbid']."' AND (ma.mavid IS NULL OR ma.iusdavaliador='".$dados['iusd']."') 
			)
			";
	
	return $sql;
}

function sqlAvaliacaoFormadorRegional($dados) {
	global $db;
	
	$sql = "(
			
			SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc,
				   CASE WHEN (SELECT esdid FROM sismedio.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
				   (SELECT m.docid FROM sismedio.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid,
				   '&functionavaliacao=sqlAvaliacaoOrientador&uncid=".$dados['uncid']."&iusd='||i.iusd||'' as mais
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sismedio.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND tt.iusd='".$dados['iusd']."' 
			
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
					(SELECT m.docid FROM sismedio.mensario m WHERE iusd=faa.iusd AND fpbid='".$dados['fpbid']."') as docid 
			FROM (
			
			SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc,
				   CASE WHEN (SELECT esdid FROM sismedio.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
				   (SELECT m.docid FROM sismedio.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid,
				   '&functionavaliacao=sqlAvaliacaoOrientador&uncid=".$dados['uncid']."&iusd='||i.iusd||'' as mais
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND i.iusstatus='A' AND p.picid='".$dados['picid']."' AND iusformacaoinicialorientador=true 
			
			) faa 
			LEFT JOIN sismedio.mensario m ON m.iusd = faa.iusd  AND m.fpbid='".$dados['fpbid']."'
			LEFT JOIN sismedio.mensarioavaliacoes ma 
				INNER JOIN sismedio.tipoperfil tp ON tp.iusd = ma.iusdavaliador AND tp.pflcod=".PFL_COORDENADORLOCAL."
			ON ma.menid = m.menid
			WHERE (ma.mavid IS NULL OR ma.iusdavaliador='".$dados['iusd']."')
			
			)";
	
	return $sql;
}

function sqlAvaliacaoCoordenadorAdjuntoIES($dados) {
	global $db;
	
	if($dados['fpbid']) {
	
		$sql = "
			(
			SELECT i.iusd
			FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
			WHERE t.pflcod='".PFL_SUPERVISORIES."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
		
			)";
	
		$iusds = $db->carregarColuna($sql);
	
		if($iusds) {
	
			foreach($iusds as $iusd) {
				criarMensario(array("iusd"=>$iusd,"fpbid"=>$dados['fpbid']));
			}
	
		}
	
	}
	
	$sql = "(
			SELECT  faa.iusd, 
					faa.iuscpf, 
					faa.iusnome, 
					faa.iusemailprincipal, 
					faa.pflcod,
					faa.pfldsc, 
					faa.mon,
					(SELECT m.docid FROM sismedio.mensario m WHERE iusd=faa.iusd AND fpbid='".$dados['fpbid']."') as docid,
					faa.mais FROM (
			(
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sismedio.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoSupervisor&uncid='||i.uncid||'&iusd='||i.iusd||'' as mais 
			FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_SUPERVISORIES."' AND i.iusstatus='A' AND i.uncid='".$dados['uncid']."' ORDER BY p.pflcod, i.iusnome
			
			)
			
			
			) faa 
			LEFT JOIN sismedio.mensario m ON m.iusd = faa.iusd 
			LEFT JOIN sismedio.mensarioavaliacoes ma ON ma.menid = m.menid
			WHERE m.fpbid='".$dados['fpbid']."' AND (ma.mavtotal IS NULL OR ma.iusdavaliador='".$dados['iusd']."')
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
				   CASE WHEN (SELECT esdid FROM sismedio.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
				   (SELECT m.docid FROM sismedio.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid,
				   '&functionavaliacao=sqlAvaliacaoCoordenadorIES&uncid='||i.uncid||'&iusd='||i.iusd||'' as mais
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_COORDENADORIES." AND i.iusstatus='A' AND i.uncid='".$dados['uncid']."'
			
			)";
	
	return $sql;
}

?>