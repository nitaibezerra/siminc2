<?
function sqlEquipeOrientador($dados) {
	global $db;
	
	$sql = "SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc, 
				   '' as periodo,
				   0 as nmeses,
				   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_INDIGENA.") as status,
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_PROFESSORALFABETIZADOR.") as perfil
			FROM sisindigena2.identificacaousuario i 
			INNER JOIN sisindigena2.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sisindigena2.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sisindigena2.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod
			WHERE t.pflcod=".PFL_PROFESSORALFABETIZADOR." AND tt.iusd='".$dados['iusd']."' AND i.iusstatus='A'";
	
	return $sql;
}

function carregarOrientadorEstudo($dados) {
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
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_ORIENTADORESTUDO."'");
	
	
	$_SESSION['sisindigena2']['orientadorestudo'] = array("descricao" => $arr['descricao']." ( ".$infprof['iusnome']." )",
													  "curid" 	  => $arr['curid'], 
													  "uncid" 	  => $arr['uncid'], 
													  "picid" 	  => $arr['picid'],
													  "reiid" 	  => $arr['reiid'], 
													  "estuf" 	  => $arr['uniuf'], 
													  "docid" 	  => $arr['docid'], 
													  "iusd" 	  => $infprof['iusd'],
													  "iuscpf"    => $infprof['iuscpf']);
	
	if($dados['direcionar']) {
		$al = array("location"=>"sisindigena2.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function mostrarAbaAvaliacaoComplementar($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*) as tot FROM sisindigena2.grupoitensavaliacaocomplementar g 
			INNER JOIN sisindigena2.grupoitensavaliacaocomplementarperfil p ON  p.gicid = g.gicid 
			WHERE pflcod IN(SELECT pflcod FROM sisindigena2.tipoperfil WHERe iusd='".$_SESSION['sisindigena2'][$dados['abapai']]['iusd']."')";
	
	$tot = $db->pegaUm($sql);
	
	if($tot) return true;
	else return false;
	
}

?>