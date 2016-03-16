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
				   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_MEDIO.") as status,
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod IN(".PFL_PROFESSORALFABETIZADOR.",".PFL_COORDENADORPEDAGOGICO.")) as perfil,
				   (SELECT usucpf FROM sismedio.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					CASE WHEN m.muncod IS NOT NULL THEN m.estuf||' / '||m.mundescricao 
					ELSE 'Equipe IES' END as rede
				   		
				   
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.orientadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sismedio.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod 
			LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao 
			WHERE t.pflcod IN(".PFL_PROFESSORALFABETIZADOR.",".PFL_COORDENADORPEDAGOGICO.") AND tt.iusd='".$dados['iusd']."' AND i.iusstatus='A'";
	
	return $sql;
}

function carregarOrientadorEstudo($dados) {
	global $db;
	
	$arr = $db->pegaLinha("SELECT u.uncid, re.reiid, su.uniuf, u.curid, u.docid, su.unisigla||' - '||su.uninome as descricao FROM sismedio.universidadecadastro u 
					 	   INNER JOIN sismedio.universidade su ON su.uniid = u.uniid
						   INNER JOIN sismedio.reitor re on re.uniid = su.uniid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sismedio.identificacaousuario i 
							   INNER JOIN sismedio.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_ORIENTADORESTUDO."'");
	
	
	$_SESSION['sismedio']['orientadorestudo'] = array("descricao" => $arr['descricao']." ( ".$infprof['iusnome']." )",
													  "curid" 	  => $arr['curid'], 
													  "uncid" 	  => $arr['uncid'], 
													  "reiid" 	  => $arr['reiid'], 
													  "estuf" 	  => $arr['uniuf'], 
													  "docid" 	  => $arr['docid'], 
													  "iusd" 	  => $infprof['iusd'],
													  "iuscpf"    => $infprof['iuscpf']);
	
	if($dados['direcionar']) {
		$al = array("location"=>"sismedio.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function mostrarAbaAvaliacaoComplementar($dados) {
	global $db;
	
	if($_SESSION['sismedio'][$dados['abapai']]['iusd']) {
		
		$sql = "SELECT COUNT(*) as tot FROM sismedio.grupoitensavaliacaocomplementar g 
				INNER JOIN sismedio.grupoitensavaliacaocomplementarperfil p ON  p.gicid = g.gicid 
				WHERE pflcod IN(SELECT pflcod FROM sismedio.tipoperfil WHERe iusd='".$_SESSION['sismedio'][$dados['abapai']]['iusd']."')";
		
		$tot = $db->pegaUm($sql);
		
		if($tot) return true;
		else return false;
	}
	
}

?>