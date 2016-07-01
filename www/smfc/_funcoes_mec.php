<?
function sqlEquipeMEC($dados) {
	
	$sql = "SELECT i.iusid, 
				   t.pflcod, 
				   i.iusnome, 
				   i.iuscpf,
				   i.iusemailprincipal,
				   p.pfldsc,
				   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SMFC.") as status,
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=t.pflcod) as perfil,
				   ins.inssigla ||' - '|| ins.insnome as instituicao 
			FROM smfc.identificacaousuario i 
			INNER JOIN smfc.tipoperfil t ON t.iusid = i.iusid 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN smfc.instituicoes ins ON ins.insid = i.insid";
	
	return $sql;
}






function salvarPlanejamentoInstituicaoExercicio($dados) {
	global $db;
	
	$docid = wf_cadastrarDocumento(TPD_PLANEJAMENTO_LOA,"Planejamento LOA");
	
	$sql = "INSERT INTO smfc.planejamentoinstituicaoexercicio(
            pieexercicio, insid, docid)
    		VALUES ('".$dados['pieexercicio']."', '".$dados['insid']."', {$docid});";
	
	$db->executar($sql);
	$db->commit();

	$al = array("alert"=>"Planejamento inserido com sucesso",
				"location"=>"smfc.php?modulo=principal/mec/meccadastro&acao=A&aba=planejamento");
	alertlocation($al);
	
	
}
?>