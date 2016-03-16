<?php
//ob_clean();
ver($_POST,d);
$texto = $_POST['texto'];
ver(simec_htmlentities($texto),d);
/*if( $texto ){
	if( strpos($texto, '<p style=\"page-break-before: always;\"><!-- pagebreak --></p>') ) {
		$texto = str_replace('<p style=\"page-break-before: always;\"><!-- pagebreak --></p>', '<p style="page-break-before:always"><!-- pagebreak --></p>', $texto );
	} else {
		$texto = str_replace("<!-- pagebreak -->", '<p style="page-break-before:always"><!-- pagebreak --></p>', $texto );
	}
}*/
//$texto = $texto ? "'".simec_htmlspecialchars($texto, ENT_QUOTES)."'" : 'null';
?>
<style type="">
	@media print {.notprint { display: none } .div_rolagem{display: none} }	
	@media screen {.notscreen { display: none; }
	
	.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
	
</style>
<table style="width: 90%" align="center" cellSpacing="1" cellPadding=3 >
	<tr>
		<td>
			<p style="text-align: justify;"><span style="font-size: small;">
				<?=$texto; ?>
			</span></p>
		</td>
	</tr>
	<tr>
		<td align="center" class="div_rolagem">
			<input type="button" value="Imprimir" id="btnImprimir1" onclick="javascript: window.print();" />
		</td>
	</tr>
</table>