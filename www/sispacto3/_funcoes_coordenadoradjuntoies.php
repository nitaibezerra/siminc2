<?
function sqlEquipeCoordenadorAdjunto($dados) {
	global $db;
	
	$sql = "SELECT  i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal,
					i.iusformacaoinicialorientador, 
					p.pflcod,
					p.pfldsc, 
					(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
					(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod = t.pflcod) as perfil,
					(SELECT usucpf FROM sispacto3.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					CASE WHEN pic.picid IS NOT NULL THEN 
														CASE WHEN pic.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN pic.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
					
			FROM sispacto3.identificacaousuario i
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sispacto3.pactoidadecerta pic ON pic.picid = i.picid 
			LEFT JOIN workflow.documento d ON d.docid = pic.docid 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = pic.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE (t.pflcod IN('".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORLOCAL."') OR (t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.iusformacaoinicialorientador=true)) AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND CASE WHEN pic.picid IS NOT NULL THEN d.esdid=".ESD_VALIDADO_COORDENADOR_LOCAL." ELSE true END ORDER BY p.pflcod, i.iusnome";
	
	
	return $sql;
}

function carregarCoordenadorAdjuntoIES($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT u.uncid, re.reiid, su.uniuf, u.curid, u.docid, su.unisigla||' - '||su.uninome as descricao FROM sispacto3.universidadecadastro u 
					 	   INNER JOIN sispacto3.universidade su ON su.uniid = u.uniid
						   INNER JOIN sispacto3.reitor re on re.uniid = su.uniid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sispacto3.identificacaousuario i 
							   INNER JOIN sispacto3.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_COORDENADORADJUNTOIES."'");
	
	
	$_SESSION['sispacto3']['coordenadoradjuntoies'] = array("descricao" => $arr['descricao']."( ".$infprof['iusnome']." )","curid" => $arr['curid'], "uncid" => $arr['uncid'], "reiid" => $arr['reiid'], "estuf" => $arr['uniuf'], "docid" => $arr['docid'], "iusd" => $infprof['iusd'], "iuscpf" => $infprof['iuscpf']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sispacto3.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=principal");
		alertlocation($al);
	}
	
}


?>