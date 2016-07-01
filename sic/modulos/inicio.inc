<?php

if(checkPerfil(SIC_PERFIL_AREA_RESPONSAVEL) && !checkPerfil(SIC_PERFIL_SUPER_USUARIO)){
	
	$sql = "select 
				secid 
			from 
				sic.usuarioresponsabilidade 
			where 
				usucpf = '{$_SESSION['usucpf']}'
			and
				rpustatus = 'A'";
	
	$rs = $db->carregar($sql);
	
	if($rs){
		$_SESSION['sic']['secretarias'] = array();
		foreach($rs as $dados){
			$_SESSION['sic']['secretarias'][] = $dados['secid'];		
		}
	}
	
}else{
	
	unset($_SESSION['sic']['secretarias']);
}

//header("Location: sic.php?modulo=principal/formPedidoInformacao&acao=A");
header("Location: sic.php?modulo=principal/listaPedidos&acao=A");

?>

