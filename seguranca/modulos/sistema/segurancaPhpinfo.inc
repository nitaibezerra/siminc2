<?php
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = !$titulo ? str_replace("...","",$_SESSION['mnudsc']) : $titulo;
monta_titulo( $titulo, '&nbsp' );
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<? phpinfo(); ?>
		</td>
	</tr>
</table>
