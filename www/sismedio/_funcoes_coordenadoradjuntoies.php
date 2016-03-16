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
					(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_MEDIO.") as status,
					(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=t.pflcod) as perfil,
				    (SELECT usucpf FROM sismedio.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					'Equipe IES' as rede
			FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod IN('".PFL_FORMADORREGIONAL."','".PFL_FORMADORIES."','".PFL_SUPERVISORIES."') AND i.iusstatus='A' AND i.uncid='".$dados['uncid']."' ORDER BY p.pflcod, i.iusnome
			
			)
			";
	
	return $sql;
}

function carregarCoordenadorAdjuntoIES($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT u.uncid, su.uniuf, u.curid, u.docid, su.unisigla||' - '||su.uninome as descricao FROM sismedio.universidadecadastro u 
					 	   INNER JOIN sismedio.universidade su ON su.uniid = u.uniid
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sismedio.identificacaousuario i 
							   INNER JOIN sismedio.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_COORDENADORADJUNTOIES."'");
	
	
	$_SESSION['sismedio']['coordenadoradjuntoies'] = array("descricao" => $arr['descricao']."( ".$infprof['iusnome']." )","curid" => $arr['curid'], "uncid" => $arr['uncid'], "reiid" => $arr['reiid'], "estuf" => $arr['uniuf'], "docid" => $arr['docid'], "iusd" => $infprof['iusd'], "iuscpf" => $infprof['iuscpf']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sismedio.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=principal");
		alertlocation($al);
	}
	
}


?>