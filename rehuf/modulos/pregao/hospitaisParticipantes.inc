<?php

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Hospitais Participantes', '' );

$preid = $_SESSION['rehuf']['preid'];

echo rehuf_cabecalho_pregao( $preid );
?>
<script language="JavaScript" src="./js/rehuf.js"></script>
<form method="post">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Filtros de busca</td>
</tr>
<tr>
	<td class="SubTituloDireita">Por preço :</td>
	<td><? echo campo_texto('filtro[preco]', "N", "S", "Preço Unitário", 20, 19, "", "", 'right', '', 0, 'id="precounit"', 'this.value=formataPrecoPregao(this.value);', '',  'this.value=formataPrecoPregao(this.value);'); ?></td>
</tr>
	<tr>
		<td colspan="2" align="center"><input type="submit" value="Buscar"/> <input type="button" value="Ver todos" onclick="window.location='?modulo=pregao/hospitaisParticipantes&acao=A';"/></td>
	</tr>
</table>
</form>
<table id="tblHospitaisParticipantes" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<?php rehuf_lista_hospitais_participantes_pregao($preid, $_REQUEST['filtro'] ); ?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td>
			<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor:pointer;"/>
		</td>
	</tr>
</table>