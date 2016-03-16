<?php

//header("Cache-Control: no-store, no-cache, must-revalidate");
//header("Cache-Control: post-check=0, pre-check=0", false);
//header("Pragma: no-cache");



// selecionando o perfil de maior nivel
$sql = "SELECT p.pflcod FROM seguranca.perfil p 
		LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod 
		WHERE pu.usucpf = '". $_SESSION['usucpf'] ."' and p.pflstatus = 'A' and p.sisid = '". $_SESSION['sisid'] ."' 
		ORDER BY pflnivel ASC LIMIT 1";

$perfilcod = $db->pegaUm($sql);

$_SESSION['sisid'] = 183;
$_SESSION['sisdiretorio'] = 'SASE';
$_SESSION['sisarquivo'] = 'SASE';
$_SESSION['sisdsc'] = 'SASE';

// Redireciona para a página inicial, dependendo do perfil do usuário
redirecionaTelaInicial();

//header('Location: sase.php?modulo=principal/assessoramento&acao=A');

?>
<!--<script type="text/javascript">-->
<!--	window.location.href="sase.php?modulo=principal/assessoramento&acao=A";-->
<!--</script>-->