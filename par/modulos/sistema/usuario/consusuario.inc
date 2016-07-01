<?php
	$perfil = pegaPerfilGeral();
	
	if($_REQUEST['acao'] == 'A')
	{
		if( ! ( in_array(PAR_PERFIL_CONTROLE_USUARIO, $perfil)))
		{
			echo "<script>
						alert('Acesso negado!');
						window.location.href = '	par.php?modulo=inicio&acao=C';
					  </script>";
			die();		
		}
	}
	
	$configuracao = array();
	array_push( $configuracao, $_SESSION['sisid'] == 23 ? 'monitoramento' : 'projetos_especiais'  );
	include APPRAIZ .'seguranca/modulos/sistema/usuario/consusuario.inc';
?>