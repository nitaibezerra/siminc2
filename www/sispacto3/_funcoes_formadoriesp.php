<?

function carregarFormadorIES($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT u.uncid, re.reiid, su.uniuf, u.curid, u.docid, su.unisigla||' - '||su.uninome as descricao FROM sispacto2.universidadecadastro u 
					 	   INNER JOIN sispacto2.universidade su ON su.uniid = u.uniid
						   INNER JOIN sispacto2.reitor re on re.uniid = su.uniid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sispacto2.identificacaousuario i 
							   INNER JOIN sispacto2.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_FORMADORIESP."'");
	
	
	$_SESSION['sispacto2']['formadoriesp'] = array("descricao" => $arr['descricao']." ( ".$infprof['iusnome']." )",
												 "curid" => $arr['curid'], 
												 "uncid" => $arr['uncid'], 
												 "reiid" => $arr['reiid'], 
												 "estuf" => $arr['uniuf'], 
												 "docid" => $arr['docid'], 
												 "iusd"  => $infprof['iusd'],
												 "iuscpf" => $infprof['iuscpf']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sispacto2.php?modulo=principal/formadoriesp/formadoriesp&acao=A&aba=principal");
		alertlocation($al);
	}
	
}





?>