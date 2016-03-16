<?

function carregarCoordenadorInstitucional($dados) {
	global $db;
	
	$sql = "SELECT 
				i.iusid,
				i.iusnome,
				i.iuscpf,
				ic.inssigla || ' / ' || ic.insnome as instituicao,
				p.pfldsc,
				i.insid
			FROM smfc.identificacaousuario i 
			INNER JOIN smfc.tipoperfil t ON t.iusid = i.iusid 
			INNER JOIN smfc.instituicoes ic ON ic.insid = i.insid 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE i.iusid='".$dados['iusid']."' AND 
				  t.pflcod='".PFL_COORDENADOR_INSTITUCIONAL."'";
	
	$identificacaousuario = $db->pegaLinha($sql);
	
	$_SESSION['smfc']['coordenadorinstitucional'] = $identificacaousuario;
	
	$al = array("location"=>"smfc.php?modulo=principal/coordenadorinstitucional/inscadastro&acao=A");
	alertlocation($al);
	
}

?>