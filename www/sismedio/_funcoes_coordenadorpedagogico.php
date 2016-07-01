<?

function carregarCoordenadorPedagogico($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT u.uncid, 
								  re.reiid, 
								  su.uniuf, 
								  u.curid, 
								  u.docid, 
								  su.unisigla||' - '||su.uninome as descricao
						   FROM sismedio.universidadecadastro u 
					 	   INNER JOIN sismedio.universidade su ON su.uniid = u.uniid
						   INNER JOIN sismedio.reitor re on re.uniid = su.uniid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sismedio.identificacaousuario i 
							   INNER JOIN sismedio.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_COORDENADORPEDAGOGICO."'");
	
	$_SESSION['sismedio']['coordenadorpedagogico'] = array("descricao" => $arr['descricao']." ( ".$infprof['iusnome']." )",
															"curid" 	=> $arr['curid'], 
															"uncid" 	=> $arr['uncid'], 
															"reiid" 	=> $arr['reiid'], 
															"estuf" 	=> $arr['uniuf'], 
															"docid" 	=> $arr['docid'], 
															"iusd" 	   	=> $infprof['iusd'],
															"iuscpf"    => $infprof['iuscpf']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sismedio.php?modulo=principal/coordenadorpedagogico/coordenadorpedagogico&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

?>