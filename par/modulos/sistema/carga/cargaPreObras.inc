<?php

function realizarCarga(){
	
	global $db;
	
	$sql = "SELECT 
				muncod, 
				estuf, 
				pcodsc, 
				pcoid
			FROM 
				obras.precargaobra
			WHERE
				pcostatus = 'A'";
	$carga = $db->carregar($sql);
	
	if(is_array($carga)){
		
		$sqlUp = '';
		
		foreach( $carga as $obra ){
			
			$sql = "INSERT INTO obras.preobra
						( presistema, ptoid, predtinclusao, estuf, muncod,
						preano, tooid, prestatus, predescricao )
					VALUES 
						( 23, 2, now(), '".$obra['estuf']."', '".$obra['muncod']."',  
						'2011', 1, 'A', '".$obra['pcodsc']."' )
					RETURNING
						preid";
			
			$preid = $db->pegaUm($sql);
			preCriarDocumento( $preid );
		}
		
		$sql = "UPDATE obras.precargaobra SET
					pcostatus = 'I'
				WHERE
					pcostatus = 'A'";
		$db->executar($sql);
		$db->sucesso('sistema/carga/cargaPreObras','');
	}
}

if($_REQUEST['req']){
	$_REQUEST['req']($_REQUEST);
}

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';

monta_titulo( $titulo_modulo, '' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>

jQuery.noConflict();

jQuery(document).ready(function() {

	jQuery('.carga').click(function(){
		if(confirm('Deseja realizar carge de dados da tabela \'precargaobra\' em \'preobra\'?')){
			jQuery('#req').val('realizarCarga');
			jQuery('#formCarga').submit();
		}
	});
});
</script>
<form method="post" name="formCarga" id="formCarga"> 
	<input type="hidden" id="req" name="req" value=""/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td>
				<center>
					<input type="button" class="carga" value="Realizar Carga">
				</center>
			</td>
		</tr>
	</table>
</form>
