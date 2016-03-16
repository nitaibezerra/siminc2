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
				   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_PROFESSORALFABETIZADOR.") as perfil,
					CASE WHEN p.picid IS NOT NULL THEN 
														CASE WHEN p.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN p.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
				   
			FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sispacto.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = p.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE t.pflcod=".PFL_PROFESSORALFABETIZADOR." AND tt.iusd='".$dados['iusd']."' AND i.iusstatus='A'";
	
	return $sql;
}

function carregarOrientadorEstudo($dados) {
	global $db;
	
	$arr = $db->pegaLinha("SELECT u.uncid, re.reiid, su.uniuf, u.curid, u.docid, su.unisigla||' - '||su.uninome as descricao FROM sispacto.universidadecadastro u 
					 	   INNER JOIN sispacto.universidade su ON su.uniid = u.uniid
						   INNER JOIN sispacto.reitor re on re.uniid = su.uniid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf, i.iusdesativado 
							   FROM sispacto.identificacaousuario i 
							   INNER JOIN sispacto.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_ORIENTADORESTUDO."'");
	
	
	$_SESSION['sispacto']['orientadorestudo'] = array("descricao" => $arr['descricao']." ( ".$infprof['iusnome']." )",
													  "curid" 	  => $arr['curid'], 
													  "uncid" 	  => $arr['uncid'], 
													  "reiid" 	  => $arr['reiid'], 
													  "estuf" 	  => $arr['uniuf'], 
													  "docid" 	  => $arr['docid'], 
													  "iusd" 	  => $infprof['iusd'],
													  "iuscpf"    => $infprof['iuscpf'],
													  "iusdesativado" => $infprof['iusdesativado']);
	
	if($dados['direcionar']) {
		$al = array("location"=>"sispacto.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function mostrarAbaAvaliacaoComplementar($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*) as tot FROM sispacto.grupoitensavaliacaocomplementar g 
			INNER JOIN sispacto.grupoitensavaliacaocomplementarperfil p ON  p.gicid = g.gicid 
			WHERE pflcod IN(SELECT pflcod FROM sispacto.tipoperfil WHERe iusd='".$_SESSION['sispacto'][$dados['abapai']]['iusd']."')";
	
	$tot = $db->pegaUm($sql);
	
	if($tot) return true;
	else return false;
	
}

?>