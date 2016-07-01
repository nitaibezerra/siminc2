<?
function sqlEquipeSupervisor($dados) {
	global $db;
	
	$sql = "(
			
			SELECT i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal, 
					p.pflcod,
					p.pfldsc, 
					to_char(t.tpeatuacaoinicio,'mm/YYYY')||' a '||to_char(t.tpeatuacaofim,'mm/YYYY') as periodo, 
					(FLOOR((t.tpeatuacaofim - t.tpeatuacaoinicio)/30)+1) as nmeses, 
					(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
					(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_FORMADORIES.") as perfil,
					'Equipe IES' as rede 
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod IN('".PFL_FORMADORIES."') AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome
			
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
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_ORIENTADORESTUDO.") as perfil,
					CASE WHEN p.picid IS NOT NULL THEN 
														CASE WHEN p.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN p.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
			FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			INNER JOIN sispacto.abrangencia a ON p.muncod=a.muncod 
			INNER JOIN sispacto.estruturacurso e ON e.ecuid = a.ecuid 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = p.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND e.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND a.esfera='M' AND i.iusformacaoinicialorientador=true
			
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
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_ORIENTADORESTUDO.") as perfil,
					CASE WHEN p.picid IS NOT NULL THEN 
														CASE WHEN p.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN p.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
				   
			FROM sispacto.identificacaousuario i 
			INNER JOIN territorios.municipio mm ON mm.muncod = i.muncodatuacao
			INNER JOIN sispacto.pactoidadecerta p ON p.estuf = mm.estuf AND p.picid = i.picid 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			INNER JOIN sispacto.abrangencia a ON mm.muncod=a.muncod 
			INNER JOIN sispacto.estruturacurso e ON e.ecuid = a.ecuid 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = p.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND e.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND a.esfera='E' AND i.iusformacaoinicialorientador=true
			
			)";
	
	return $sql;
}

function carregarSupervisorIES($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT u.uncid, re.reiid, su.uniuf, u.curid, u.docid, su.unisigla||' - '||su.uninome as descricao FROM sispacto.universidadecadastro u 
					 	   INNER JOIN sispacto.universidade su ON su.uniid = u.uniid
						   INNER JOIN sispacto.reitor re on re.uniid = su.uniid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf, i.iusdesativado 
							   FROM sispacto.identificacaousuario i 
							   INNER JOIN sispacto.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_SUPERVISORIES."'");
	
	
	$_SESSION['sispacto']['supervisories'] = array("descricao" => $arr['descricao']."( ".$infprof['iusnome']." )",
												   "curid" => $arr['curid'], 
												   "uncid" => $arr['uncid'], 
												   "reiid" => $arr['reiid'], 
												   "estuf" => $arr['uniuf'], 
												   "docid" => $arr['docid'], 
												   "iusd" => $infprof['iusd'],
												   "iusdesativado"  => $infprof['iusdesativado'],
												   "iuscpf" => $infprof['iuscpf']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sispacto.php?modulo=principal/supervisories/supervisories&acao=A&aba=principal");
		alertlocation($al);
	}
	
}



?>