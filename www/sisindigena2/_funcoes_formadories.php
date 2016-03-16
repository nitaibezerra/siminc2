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
				   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_ORIENTADORESTUDO.") as perfil
			FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sispacto.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			INNER JOIN sispacto.abrangencia a ON p.muncod=a.muncod 
			INNER JOIN sispacto.estruturacurso e ON e.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND e.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND a.esfera='M' AND tt.iusd='".$dados['iusd']."'
			
			)
			UNION ALL (

			SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc, 
				   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_ORIENTADORESTUDO.") as perfil
			FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sispacto.turmas tt ON tt.turid = ot.turid 
			INNER JOIN territorios.municipio mm ON mm.muncod = i.muncodatuacao
			INNER JOIN sispacto.pactoidadecerta p ON p.estuf = mm.estuf AND p.picid = i.picid 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			INNER JOIN sispacto.abrangencia a ON mm.muncod=a.muncod 
			INNER JOIN sispacto.estruturacurso e ON e.ecuid = a.ecuid
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND e.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND a.esfera='E' AND tt.iusd='".$dados['iusd']."'
			
			)";
	
	return $sql;
}

function carregarFormadorIES($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT n.docid, n.picsede, n.picid, u.uncid, su.uniuf, su.unisigla||' - '||su.uninome||' >> '||su2.unisigla||' - '||su2.uninome as descricao 
						   FROM sisindigena2.nucleouniversidade n  
						   INNER JOIN sisindigena2.universidadecadastro u ON u.uncid = n.uncid  
					 	   INNER JOIN sisindigena2.universidade su 		 ON su.uniid = u.uniid 
					 	   INNER JOIN sisindigena2.universidade su2       ON su2.uniid = n.uniid 
					 	   INNER JOIN sisindigena2.identificacaousuario i ON i.picid = n.picid 
					 	   WHERE i.iusd = '".$dados['iusd']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sisindigena2.identificacaousuario i 
							   INNER JOIN sisindigena2.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_FORMADORIES."'");
	
	$_SESSION['sisindigena2']['formadories'] = array("descricao" => $arr['descricao']."( ".$infprof['iusnome']." )",
												   "iusnome" => $infprof['iusnome'],
												   "picid" => $arr['picid'],
												   "curid" => $arr['curid'], 
												   "uncid" => $arr['uncid'], 
												   "reiid" => $arr['reiid'], 
												   "estuf" => $arr['uniuf'], 
												   "iusd" => $infprof['iusd'],
												   "iuscpf" => $infprof['iuscpf']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sisindigena2.php?modulo=principal/formadories/formadories&acao=A&aba=principal");
		alertlocation($al);
	}
	
}





?>