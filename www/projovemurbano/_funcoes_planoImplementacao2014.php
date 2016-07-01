<?php 
function inserirCoordenadorResponsavelPorAno($dados) {
	
	global $db;
	
	$perfil = ( $_SESSION['projovemurbano']['muncod'] != '' ? PFL_COORDENADOR_MUNICIPAL : PFL_COORDENADOR_ESTADUAL);
	
	$campo = ( $_SESSION['projovemurbano']['muncod'] != '' ? 'muncod' : 'estuf');
	
	$sql = "SELECT
				corcpf
			FROM 
				projovemurbano.coordenadorresponsavel
			WHERE
				pjuid = '".$_SESSION['projovemurbano']['pjuid']."'
				AND ppuid = '".$_SESSION['projovemurbano']['ppuid']."'";
	
	$cpfs = $db->carregarColuna($sql);
	
	$sql = "UPDATE projovemurbano.coordenadorresponsavel SET
				corstatus = 'I'
			WHERE
				corcpf in ('".(implode("','",$cpfs))."')
				AND pjuid = '".$_SESSION['projovemurbano']['pjuid']."'
				AND ppuid = '".$_SESSION['projovemurbano']['ppuid']."';";
	
	$sql .= "DELETE FROM seguranca.perfilusuario WHERE usucpf in ('".(implode("','",$cpfs))."') AND pflcod = $perfil;";
	
	$sql .= "UPDATE projovemurbano.usuarioresponsabilidade SET
				 rpustatus = 'I'
			WHERE 
				usucpf in ('".(implode("','",$cpfs))."') 
				AND pflcod = $perfil
				AND $campo = '{$_SESSION['projovemurbano'][$campo]}';";

	$db->executar($sql);
	
	$db->commit();
	
	$sql = "INSERT INTO projovemurbano.coordenadorresponsavel(
				pjuid, ppuid, corcpf, cornome, corsecretario, corstatus)
			VALUES (
				'".$_SESSION['projovemurbano']['pjuid']."',
				'".$_SESSION['projovemurbano']['ppuid']."',
				'".str_replace(array(".","-"),array("",""),$dados['corcpf'])."',
				'".$dados['cornome']."',
				".(($dados['corsecretario']=="sim")?"TRUE":"FALSE").",
				'A');";

	$db->executar($sql);

	$db->commit();
	
	$perfis = pegaPerfilGeral( str_replace(Array('.','-'),'',$dados['corcpf']) );
	$perfis = $perfis ? $perfis : Array();
	
	if( !in_array( $perfil, $perfis) ){
		$sql = "INSERT INTO seguranca.perfilusuario( usucpf, pflcod)
				VALUES( '".str_replace(Array('.','-'),'',$dados['corcpf'])."', $perfil );";
	}
	
	$resp = "SELECT true FROM projovemurbano.usuarioresponsabilidade 
			 WHERE usucpf = '".str_replace(Array('.','-'),'',$dados['corcpf'])."' AND $campo = '{$_SESSION['projovemurbano'][$campo]}' AND rpustatus = 'A'";
	
	$teste = $db->pegaUm($resp);
	
	if( $teste != 't' ){
		$sql .= "INSERT INTO projovemurbano.usuarioresponsabilidade ( usucpf, pflcod, $campo)
				VALUES( '".str_replace(Array('.','-'),'',$dados['corcpf'])."', $perfil, '{$_SESSION['projovemurbano'][$campo]}' );";
	}
// 	ver($sql,d);
	$db->executar($sql);
	
	$db->commit();

	echo "<script>
			alert('Dados salvos com sucesso');
			window.location='projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}';
		</script>";

}

function atualizarCoordenadorResponsavelPorAno($dados) {
	
	global $db;
	
	$sql = "SELECT
				true
			FROM
			--seguranca.perfilusuario
			seguranca.usuario
			WHERE
				usucpf = '".str_replace(Array('.','-'),'',$dados['corcpf'])."'";
	
	$teste = $db->pegaUm($sql);
//	dbg($teste);
	if(!$teste){
		echo "
			<script>
				alert('Este cpf ainda não está cadastrado no SIMEC.');
				window.location.href = window.location.href;
			</script>";
		die();
	}

	$perfil = ( $_SESSION['projovemurbano']['muncod'] != '' ? PFL_COORDENADOR_MUNICIPAL : PFL_COORDENADOR_ESTADUAL);
	
	$campo = ( $_SESSION['projovemurbano']['muncod'] != '' ? 'muncod' : 'estuf');
	
	$sql = "SELECT
				corcpf
			FROM
				projovemurbano.coordenadorresponsavel
			WHERE
				pjuid = '".$_SESSION['projovemurbano']['pjuid']."'
				AND ppuid = '".$_SESSION['projovemurbano']['ppuid']."'";
	
	$cpfs = $db->carregarColuna($sql);
	
	$sql = "UPDATE projovemurbano.coordenadorresponsavel SET
				corstatus = 'I'
			WHERE
				corcpf in ('".(implode("','",$cpfs))."')
				AND pjuid = '".$_SESSION['projovemurbano']['pjuid']."'
				AND ppuid = '".$_SESSION['projovemurbano']['ppuid']."';";
	
	$sql .= "DELETE FROM seguranca.perfilusuario WHERE usucpf in ('".(implode("','",$cpfs))."') AND pflcod = $perfil;";
	
	$sql .= "UPDATE projovemurbano.usuarioresponsabilidade SET
				rpustatus = 'I'
			WHERE
				usucpf in ('".(implode("','",$cpfs))."')
				AND pflcod = $perfil
				AND $campo = '{$_SESSION['projovemurbano'][$campo]}';";
	
	$db->executar($sql);
	
	$db->commit();
	
	$sql = "UPDATE projovemurbano.coordenadorresponsavel SET 
				corcpf='".str_replace(array(".","-"),array("",""),$dados['corcpf'])."',
				cornome='".$dados['cornome']."',
				corsecretario=".(($dados['corsecretario']=="sim")?"TRUE":"FALSE").",
				corstatus = 'A' 
			WHERE 
				pjuid='".$_SESSION['projovemurbano']['pjuid']."';";

	$db->executar($sql);

	$db->commit();
	
	$perfis = pegaPerfilGeral( str_replace(Array('.','-'),'',$dados['corcpf']) );
	$perfis = $perfis ? $perfis : Array();
	if( !in_array( $perfil, $perfis) ){
		$sql = "INSERT INTO seguranca.perfilusuario( usucpf, pflcod)
				VALUES( '".str_replace(Array('.','-'),'',$dados['corcpf'])."', $perfil );";
	}
	
	$resp = "SELECT true FROM projovemurbano.usuarioresponsabilidade
			WHERE usucpf = '".str_replace(Array('.','-'),'',$dados['corcpf'])."' AND $campo = '{$_SESSION['projovemurbano'][$campo]}' AND rpustatus = 'A'";
	
	$teste = $db->pegaUm($resp);
	
	if( $teste != 't' ){
		$sql .= "INSERT INTO projovemurbano.usuarioresponsabilidade ( usucpf, pflcod, $campo)
				VALUES( '".str_replace(Array('.','-'),'',$dados['corcpf'])."', $perfil, '{$_SESSION['projovemurbano'][$campo]}' );";
	}
	$db->executar($sql);
	
	$db->commit();
	echo "<script>
			alert('Coordenador gravado com sucesso');
			window.location='projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}';
		</script>";
}

?>