<?php
	$schema = "par"; //informa o schema
	$table = "beneficiario"; //informa a tabela
	
	if( $_POST['inp_hdn_acao'] == 'excluir' && $_POST['inp_hdn_campo']){
		$sql = "SELECT benid FROM par.subacaobeneficiario b INNER JOIN par.subacao s ON s.sbaid = b.sbaid WHERE b.benid = ".$_POST['inp_hdn_campo_valor']." AND s.sbastatus = 'A'";
		$benid = $db->pegaUm( $sql );
		if( $benid ){
			echo "<script>
					alert('Esse registro já está sendo utilizado em uma subação!');
					history.back(-1);
				</script>";
			exit;
		}
	}
	
	$arrPermission = array(
								"inserir", //Permite inserir registros na tabela
								"alterar", //Permite alterar registros da tabela
								"excluir", //Permite excluir registros da tabela
								);

	include APPRAIZ .'/seguranca/modulos/sistema/tabelasApoio.inc';
?>