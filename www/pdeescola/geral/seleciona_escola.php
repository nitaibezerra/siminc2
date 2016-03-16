<?php

	if ( empty($_REQUEST['muncod']) ){
		
		echo '<script>'
			.'	alert("Favor selecionar um municipio!");'
			.'	self.close();'
			.'</script>';
		
		die;
		
	}

	include "config.inc";	  
	include APPRAIZ ."includes/classes_simec.inc";
	include APPRAIZ ."includes/funcoes.inc";
	
	$db = new cls_banco();
	
?>
<html>
	<head>
		<META http-equiv="Pragma" content="no-cache">
		<title>Escolas</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script language="JavaScript">
			var campoSelect = window.opener.document.getElementById("<?=$_REQUEST['campo']?>");
		</script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
	</head>
	<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
			<form name="formulario">
				<thead>
					<tr>
						<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Clique na escola desejada</strong></td>
					</tr>
				</thead>
				<tr>
					<td>
						<table align="center" width="100%" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
							<?php
			
								$sql = "SELECT DISTINCT
											et.entid as codigo,
										 	et.entnome as descricao
										FROM
											pdeescola.memaiseducacao me
										INNER JOIN 
											entidade.entidade et ON et.entid = me.entid
										INNER JOIN 
											entidade.endereco ed ON et.entid = ed.entid
										INNER JOIN 
											territorios.municipio mun ON mun.muncod = ed.muncod
										WHERE
											". ( !empty($_REQUEST['muncod']) ? "ed.muncod = '{$_REQUEST['muncod']}' AND" : "" ) . " 
											me.memanoreferencia = '2009'
										ORDER BY
										 et.entnome";
								
								$RS = $db->carregar( $sql );
								$nlinhas = count($RS)-1;
								
								for ( $i=0; $i <= $nlinhas; $i++ ){
									
									$cor = ($i % 2) ? 'f4f4f4' : 'e0e0e0';
									
									echo '<tr bgColor="'.$cor.'">'
										.'	<td>'
										.		'<input type="checkbox" name="codigo" id="codigo_'.$RS[$i]['codigo'].'" value="'.$RS[$i]['codigo'].'" onclick="retorna(this.value, \''.$RS[$i]['descricao'].'\');"/>' 
										.'	</td>'
										.'	<td>'
										.		$RS[$i]['descricao']
										.'	</td>'
										.'</tr>';					
								}
								
							?>
					</table>
				</td>
			</tr>
			<tr bgcolor="#c0c0c0">
				<td>
					<input type="Button" name="ok" value="OK" onclick="self.close();">
				</td>
			</tr>
		</form>
	</table>
<script language="JavaScript">

function retorna( objeto, descricao ){
	
	var campos = document.getElementsByName( 'codigo' );
	var campo = document.getElementById( 'codigo_' + objeto );
	
	for ( i = 0; campos.length > i; i++ ){
		if ( campos[i].checked && campos[i] != campo ){
			alert('Favor selecionar apenas uma escola!');
			campo.checked = false;
			return false;
		}
	}
	
	
	if ( campoSelect.options[0].value == objeto ){
		campoSelect.options[0].value = '';
		campoSelect.options[0].text  = 'Clique Aqui Para Selecionar';
	}else{
		campoSelect.options[0].value = objeto;
		campoSelect.options[0].text  = descricao;
	}
		
}
</script>
