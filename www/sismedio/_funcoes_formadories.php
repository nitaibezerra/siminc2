<?
function sqlEquipeFormador($dados) {
	global $db;
	
	$sql = "(
			
			SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc, 
				   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_MEDIO.") as status,
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_ORIENTADORESTUDO.") as perfil,
				   (SELECT usucpf FROM sismedio.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					CASE WHEN p.picid IS NOT NULL THEN 
														CASE WHEN p.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN p.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sismedio.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sismedio.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			INNER JOIN sismedio.abrangencia a ON p.muncod=a.muncod 
			INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = p.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND e.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND a.esfera='M' AND tt.iusd='".$dados['iusd']."'
			
			)
			UNION ALL (

			SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc, 
				   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_MEDIO.") as status,
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_ORIENTADORESTUDO.") as perfil,
				   (SELECT usucpf FROM sismedio.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					CASE WHEN p.picid IS NOT NULL THEN 
														CASE WHEN p.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN p.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sismedio.turmas tt ON tt.turid = ot.turid 
			INNER JOIN territorios.municipio mm ON mm.muncod = i.muncodatuacao
			INNER JOIN sismedio.pactoidadecerta p ON p.estuf = mm.estuf AND p.picid = i.picid 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			INNER JOIN sismedio.abrangencia a ON mm.muncod=a.muncod 
			INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = p.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND e.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND a.esfera='E' AND tt.iusd='".$dados['iusd']."'
			
			)";
	
	return $sql;
}

function carregarFormadorIES($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT u.uncid, re.reiid, su.uniuf, u.curid, u.docid, su.unisigla||' - '||su.uninome as descricao FROM sismedio.universidadecadastro u 
					 	   INNER JOIN sismedio.universidade su ON su.uniid = u.uniid
						   INNER JOIN sismedio.reitor re on re.uniid = su.uniid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sismedio.identificacaousuario i 
							   INNER JOIN sismedio.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_FORMADORIES."'");
	
	
	$_SESSION['sismedio']['formadories'] = array("descricao" => $arr['descricao']." ( ".$infprof['iusnome']." )",
												 "curid" => $arr['curid'], 
												 "uncid" => $arr['uncid'], 
												 "reiid" => $arr['reiid'], 
												 "estuf" => $arr['uniuf'], 
												 "docid" => $arr['docid'], 
												 "iusd"  => $infprof['iusd'],
												 "iuscpf" => $infprof['iuscpf']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sismedio.php?modulo=principal/formadories/formadories&acao=A&aba=principal");
		alertlocation($al);
	}
	
}





?>