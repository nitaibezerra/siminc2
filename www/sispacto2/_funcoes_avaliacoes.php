<?

function sqlAvaliacaoCoordenadorIES($dados) {
	global $db;
	
	if($dados['fpbid']) {
	
		$sql = "(
								
				SELECT i.iusd 
				FROM sispacto2.identificacaousuario i
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				WHERE t.pflcod='".PFL_SUPERVISORIES."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A'
				
				) UNION ALL (
				
				SELECT i.iusd 
				FROM sispacto2.identificacaousuario i
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				WHERE t.pflcod='".PFL_COORDENADORADJUNTOIES."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A'
				
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
					(SELECT m.docid FROM sispacto2.mensario m WHERE iusd=faa.iusd AND fpbid='".$dados['fpbid']."') as docid,
					faa.mais FROM (
			(
							
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sispacto2.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoSupervisor&uncid=".$dados['uncid']."&iusd='||i.iusd||'' as mais 
			FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_SUPERVISORIES."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
			
			) UNION ALL (
			
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sispacto2.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoCoordenadorAdjuntoIES&uncid=".$dados['uncid']."&iusd='||i.iusd||'' as mais 
			FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_COORDENADORADJUNTOIES."' AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
			
			)
			
			) faa 
			LEFT JOIN sispacto2.mensario m ON m.iusd = faa.iusd 
			LEFT JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = m.menid
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
   				   (SELECT m.docid FROM sispacto2.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid
			FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sispacto2.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sispacto2.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_PROFESSORALFABETIZADOR." AND tt.iusd='".$dados['iusd']."' AND i.iusstatus='A'";
	
	return $sql;
}

function sqlAvaliacaoSupervisor($dados) {
	global $db;
	
	$sql = "SELECT i.iusd,
				   i.iuscpf,
				   i.iusnome,
				   i.iusemailprincipal,
				   pp.pflcod,
				   pp.pfldsc,
   				   CASE WHEN (SELECT esdid FROM sispacto2.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
   				   CASE WHEN t.pflcod IN(".PFL_FORMADORIES.",".PFL_FORMADORIESP.") THEN '&functionavaliacao=sqlAvaliacaoFormador&iusd='||i.iusd||'&uncid=".$dados['uncid']."'
   				   		WHEN t.pflcod = '".PFL_COORDENADORLOCAL."' THEN '&functionavaliacao=sqlAvaliacaoCoordenadorLocal&iusd='||i.iusd||'&uncid=".$dados['uncid']."' END as mais,
   				   (SELECT m.docid FROM sispacto2.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid
			FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.orientadorturmaoutros ot ON ot.iusd = i.iusd
			INNER JOIN sispacto2.turmas tt ON tt.turid = ot.turid
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod IN(".PFL_FORMADORIES.",".PFL_FORMADORIESP.",".PFL_COORDENADORLOCAL.") AND tt.iusd='".$dados['iusd']."' AND i.iusstatus='A'";
	
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
				   CASE WHEN (SELECT esdid FROM sispacto2.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
				   (SELECT m.docid FROM sispacto2.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid,
				   '&functionavaliacao=sqlAvaliacaoOrientador&uncid='||i.uncid||'&iusd='||i.iusd||'' as mais
			FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sispacto2.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND tt.iusd='".$dados['iusd']."' AND iusformacaoinicialorientador=true 
			
			)
			";
	
	return $sql;
}

function sqlAvaliacaoCoordenadorLocal($dados) {
	global $db;
	
	
	$sql = "SELECT i.iusd,
				   i.iuscpf,
				   i.iusnome,
				   i.iusemailprincipal,
				   pp.pflcod,
				   pp.pfldsc,
   				   CASE WHEN (SELECT esdid FROM sispacto2.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
   				   '&functionavaliacao=sqlAvaliacaoOrientador&uncid='||i.uncid||'&iusd='||i.iusd||'' as mais,
   				   (SELECT m.docid FROM sispacto2.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid
			FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.orientadorturmaoutros ot ON ot.iusd = i.iusd
			INNER JOIN sispacto2.turmas tt ON tt.turid = ot.turid
			INNER JOIN sispacto2.pactoidadecerta p ON p.picid = i.picid
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND tt.iusd='".$dados['iusd']."' AND i.iusstatus='A' AND iusformacaoinicialorientador=true";
	
	return $sql;
	
}

function sqlAvaliacaoCoordenadorAdjuntoIES($dados) {
	global $db;
	
	if($dados['fpbid']) {
	
		$sql = "SELECT i.iusd 
			FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_SUPERVISORIES."' AND i.iusstatus='A' AND i.uncid='".$dados['uncid']."'";
	
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
					(SELECT m.docid FROM sispacto2.mensario m WHERE iusd=faa.iusd AND fpbid='".$dados['fpbid']."') as docid,
					faa.mais FROM (
			(
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					CASE WHEN (SELECT esdid FROM sispacto2.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
					'&functionavaliacao=sqlAvaliacaoSupervisor&uncid='||i.uncid||'&iusd='||i.iusd||'' as mais 
			FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod='".PFL_SUPERVISORIES."' AND i.iusstatus='A' AND i.uncid='".$dados['uncid']."' ORDER BY p.pflcod, i.iusnome
			
			)
			
			
			) faa 
			LEFT JOIN sispacto2.mensario m ON m.iusd = faa.iusd 
			LEFT JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = m.menid
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
				   CASE WHEN (SELECT esdid FROM sispacto2.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') IN('".ESD_ENVIADO_MENSARIO."','".ESD_APROVADO_MENSARIO."') THEN 'TRUE' ELSE 'FALSE' END as mon,
				   (SELECT m.docid FROM sispacto2.mensario m WHERE iusd=i.iusd AND fpbid='".$dados['fpbid']."') as docid,
				   '&functionavaliacao=sqlAvaliacaoCoordenadorIES&uncid='||i.uncid||'&iusd='||i.iusd||'' as mais
			FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_COORDENADORIES." AND i.iusstatus='A' AND i.uncid='".$dados['uncid']."'
			
			)";
	
	return $sql;
}

?>