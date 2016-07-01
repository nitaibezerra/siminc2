<?php
if($_POST['req'] == 'verificaRespFndeUG'){
	
	$usucpf = str_replace(array('.','-'), '', $_POST['usucpf']);
	
	$sql = "select * from 
				elabrev.usuarioresponsabilidade  
			where usucpf = '{$usucpf}' 
			and rpustatus = 'A' 
			and ungcod = '".UG_FNDE."'";
	
	$rs = $db->pegaLinha($sql);
	
	if($rs){
		echo 'true';
		die;
	}
	echo 'false';
	die;
}
?>

<?php

	$configuracao = array();
	array_push( $configuracao, $_SESSION['sisid'] == 5 ? 'elaboracao' : 'proposta'  );
	include APPRAIZ .'includes/cadastro_usuario_geral.php';

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	jQuery.noConflict();
	jQuery('[name=btnAbrirResp852]').each(function(i, v){
		if(i == 0){
			jQuery(v).hide();
		}	
	});
	jQuery('[name=btnAbrirResp864]').each(function(i, v){
		if(i == 0){
			jQuery(v).hide();
		}	
	});
	jQuery('[name=btnAbrirResp54]').each(function(i, v){
		if(i == 0){
			jQuery(v).hide();
		}	
	});
	jQuery('[name=btnAbrirResp53]').each(function(i, v){
		if(i == 0){
			jQuery(v).hide();
		}	
	});
	jQuery("[name^='pflcod'] option").each(function(i, v){
		// Verifica se o perfil é gabinete
		if(jQuery(v).val() == '859'){
			// Verifica se tem responsabilidade FNDE na UG
			jQuery.ajax({
				url		: 'elabrev.php?modulo=sistema/usuario/cadusuario&acao=A',
				type	: 'post',
				data	: 'req=verificaRespFndeUG&usucpf='+jQuery('[name=cpf]').val(),
				success	: function(e){
					if(e == 'false'){
						// Remove o botão atribuir da diretoria
						jQuery('[name=btnAbrirResp859]').each(function(i, v){
							if(i == 0){
								jQuery(v).hide();
								//jQuery('font:contains("Diretorias")').css('color', 'white');
							}	
						});
					}
				}
			});
		}
	});
</script>