<?php
switch ( $_POST['op'] ){
	case 'carga':
		$rowAlterada = 0;
		$row 		 = 0;
		$handle = fopen($_FILES['arqcarga']['tmp_name'], "r");
		while (($data = fgetcsv($handle, 0, ";")) !== FALSE) {
		    $row++;
		    
		    if ( $row == 1 )
		    	continue;

		    $obrid 				  = $data[4];
		    $obrvalorprevistoNovo = str_replace(array(".", ","), array("", "."), $data[12] );
		    	
		    if (empty($obrid))
		     	continue;
		    
		    $sql 	  		  = "SELECT obrid, obrvalorprevisto FROM obras.obrainfraestrutura WHERE obrid = {$obrid}";	
		    $dadoObra 		  = $db->pegaLinha( $sql );
			$obrid			  = $dadoObra['obrid'];
		    $obrvalorprevisto = $dadoObra['obrvalorprevisto'];
			
		    if ( (empty($obrvalorprevisto) || $obrvalorprevisto == 0) && is_numeric( $obrvalorprevistoNovo ) && $obrid ){
		    	$rowAlterada++;
		    	$sql = "UPDATE obras.obrainfraestrutura SET 
		    				obrvalorprevisto = {$obrvalorprevistoNovo} 
		    			WHERE
		    				obrid = {$obrid}";
		    	
		    	$db->executar( $sql );
//			    echo $obrid . '<br>';
//			    echo $data[4] . ' | ' . $obrvalorprevisto . ' | ' . $data[12] . ' | ' . $obrvalorprevistoNovo . '<br>';
		    }    
		}
		fclose ($handle);
		$db->commit();	
		echo "Registros Atualizados: " . $rowAlterada;	
		die;
}
?>

<form method="post" action="?modulo=inicio&acao=A&carga=atualizaValorPrevisto" enctype="multipart/form-data">
<input type="hidden" name="op" value="carga">
<table>
	<tr>
		<td>Arquivo (carga)</td>
		<td>
			<input type="file" name="arqcarga" name="arqcarga">
		</td>
	</tr>
	<tr>
		<td></td>
		<td>
			<input type="submit" name="blt_enviar" value="Enviar">
		</td>
	</tr>
</table>
</form>