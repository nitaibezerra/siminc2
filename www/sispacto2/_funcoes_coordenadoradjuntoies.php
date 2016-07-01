<?
function sqlEquipeCoordenadorAdjunto($dados) {
	global $db;
	
	$sql = "
			(
			
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					to_char(t.tpeatuacaoinicio,'mm/YYYY')||' a '||to_char(t.tpeatuacaofim,'mm/YYYY') as periodo, 
					(FLOOR((t.tpeatuacaofim - t.tpeatuacaoinicio)/30)+1) as nmeses, 
					(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
					(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod IN('".PFL_FORMADORIES."','".PFL_SUPERVISORIES."')) as perfil,
					(SELECT usucpf FROM sispacto2.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					'Equipe IES' as rede
			FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod IN('".PFL_FORMADORIES."','".PFL_SUPERVISORIES."') AND i.iusstatus='A' AND i.uncid='".$dados['uncid']."' ORDER BY p.pflcod, i.iusnome
			
			)
			UNION ALL (
			
			SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc, 
				   '' as periodo,
				   0 as nmeses,
				   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod='".PFL_ORIENTADORESTUDO."') as perfil,
				   (SELECT usucpf FROM sispacto2.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					CASE WHEN p.picid IS NOT NULL THEN 
														CASE WHEN p.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN p.estuf  IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
				   
			FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			INNER JOIN sispacto2.abrangencia a ON p.muncod=a.muncod 
			INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = p.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND e.uncid='".$dados['uncid']."' AND a.esfera='M' AND i.iusstatus='A'
			
			) 
			UNION ALL (
			
			SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc, 
				   '' as periodo,
				   0 as nmeses,
				   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod='".PFL_ORIENTADORESTUDO."') as perfil,
				   (SELECT usucpf FROM sispacto2.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					CASE WHEN p.picid IS NOT NULL THEN 
														CASE WHEN p.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN p.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
				   
			FROM sispacto2.identificacaousuario i 
			INNER JOIN territorios.municipio mm ON mm.muncod = i.muncodatuacao
			INNER JOIN sispacto2.pactoidadecerta p ON p.estuf = mm.estuf AND p.picid = i.picid 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			INNER JOIN sispacto2.abrangencia a ON mm.muncod=a.muncod 
			INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = p.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND e.uncid='".$dados['uncid']."' AND a.esfera='E' AND i.iusstatus='A'
			
			)
			";
	
	return $sql;
}

function carregarCoordenadorAdjuntoIES($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT u.uncid, re.reiid, su.uniuf, u.curid, u.docid, su.unisigla||' - '||su.uninome as descricao FROM sispacto2.universidadecadastro u 
					 	   INNER JOIN sispacto2.universidade su ON su.uniid = u.uniid
						   INNER JOIN sispacto2.reitor re on re.uniid = su.uniid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf, i.iusdesativado 
							   FROM sispacto2.identificacaousuario i 
							   INNER JOIN sispacto2.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_COORDENADORADJUNTOIES."'");
	
	
	$_SESSION['sispacto2']['coordenadoradjuntoies'] = array("descricao" => $arr['descricao']."( ".$infprof['iusnome']." )",
														    "curid" => $arr['curid'], 
														    "uncid" => $arr['uncid'], 
														    "reiid" => $arr['reiid'], 
															"estuf" => $arr['uniuf'], 
															"docid" => $arr['docid'], 
															"iusd" => $infprof['iusd'], 
															"iuscpf" => $infprof['iuscpf'], 
															"iusdesativado" => $infprof['iusdesativado']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sispacto2.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=principal");
		alertlocation($al);
	}
	
}


?>