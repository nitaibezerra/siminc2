<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Questionário de Atualização do PAR', 'Respondido <img src="/imagens/check_p.gif" style="border:0;">
							 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						     Não Respondido <img src="/imagens/atencao.png" style="border:0;">' );
?>
<table style="background-color: #F5F5F5;" align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
<tr>
	<td>
		<?php
			echo '<BR><CENTER><h1>FALTA O QUESTIONÁRIO</h1></CENTER>';
		?>
	</td>
</tr>
</table>