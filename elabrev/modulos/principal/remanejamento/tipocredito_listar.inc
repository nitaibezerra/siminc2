<?php

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, '' );

$sql =
	" select " .
		" '<a href=\"javascript:editar( \'' || tcrid || '\' );\"><img alt=\"Editar\" border=\"0\" src=\"/imagens/alterar.gif\"/></a>' " .
		" || '&nbsp;&nbsp;' || " .
		" '<a href=\"javascript:excluir( \'' || tcrid || '\' );\"><img alt=\"Excluir\" border=\"0\" src=\"/imagens/excluir.gif\"/></a>' " .
			" as link, " .
		" tcrcod, " .
		" case tcrtipoporcento when 'L' then 'Localizador' when 'U' then 'Unidade Orçamentária' end as tcrtipoporcento, " .
		" tcrdsc, " .
		" tcrporcentocancelamento || ' %' as can, " .
		" tcrporcentosuplementacao || ' %' as sup " .
	" from elabrev.tipocredito " .
	" where " .
		" tcrano = '" . $_SESSION['exercicio'] . "' and " .
		" tcrstatus = 'A' " .
	" order by " .
		" tcrcod";

$db->monta_lista( $sql, array( 'Ações', 'Código', 'Tipo', 'Descrição', 'Cancelamento Máximo', 'Suplementação Máxima' ), 10, 10, false, '', '' );

// alguma coisa aconteceu que a função monta_link está sumindo do arquivo funcoes.inc
if ( !function_exists( 'monta_link' ) )
{
	function monta_link( $modulo, $acao, $parametros = '' )
	{
		return '/' . $_SESSION['sisdiretorio'] . '/' . $_SESSION['sisdiretorio'] . '.php?modulo=' . $modulo . '&acao=' . urlencode( $acao ) . '&' . $parametros;
	}
}

?>
<script type="text/javascript">

	function adicionarTipoCredito()
	{
		window.location = '<?= monta_link( 'principal/remanejamento/tipocredito', 'E' ) ?>';
	}
	
	function editar( tcrid )
	{
		window.location = '<?= monta_link( 'principal/remanejamento/tipocredito', 'E' ) ?>&tcrid=' + tcrid;
	}
	
	function excluir( tcrid )
	{
		if ( confirm( 'Deseja realmente excluir este Tipo de Crédito?' ) )
		{
			window.location = '<?= monta_link( 'principal/remanejamento/tipocredito', 'R' ) ?>&tcrid=' + tcrid;
		}
	}

</script>
<p align="center">
	<input type="button" name="Adicionar" value="Adicionar Tipo de Crédito" onclick="adicionarTipoCredito();"/>
</p>
