<?

function carregarProfessorAlfabetizador($dados) {
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
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_PROFESSORALFABETIZADOR."'");
	$_SESSION['sisindigena2']['professoralfabetizador'] = array("descricao" => $arr['descricao']." ( ".$infprof['iusnome']." )",
															"curid" 	=> $arr['curid'], 
															"uncid" 	=> $arr['uncid'], 
															"reiid" 	=> $arr['reiid'], 
															"estuf" 	=> $arr['uniuf'], 
															"picid" 	=> $arr['picid'],
															"docid" 	=> $arr['docid'], 
															"iusd" 	   	=> $infprof['iusd'],
															"iuscpf"    => $infprof['iuscpf']);	
	
	if($dados['direcionar']) {
		$al = array("location"=>"sisindigena2.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

?>