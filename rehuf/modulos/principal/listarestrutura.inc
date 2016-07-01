<?php

if($_REQUEST['consultaajax']) {
	$_REQUEST['consultaajax']($_REQUEST);
}

if($_REQUEST['alterabd']) {
	$_REQUEST['alterabd']($_REQUEST);
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

$titulo_modulo = "REHUF";
monta_titulo( $titulo_modulo, 'Programa Nacional de Reestruturação dos Hospitais Universitários Federais' );

$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/gerenciarestrutura&acao=A&tabtid='|| tabtid ||'\'\"> <img src=\"/imagens/excluir.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"Excluir(\'?modulo=principal/listarestrutura&acao=A&alterabd=removertabela&tabtid='|| tabtid ||'\',\'Deseja realmente excluir a tabela?\');\"></center>' as opcoes, 
			   tabtdsc as tabelas,
			   to_char(tabtins,'dd/mm/YYYY HH24:MI') as tabtins,
			   CASE WHEN u.usunome IS NULL THEN '-' ELSE u.usunome END as usuario  
		FROM rehuf.tabela t 
		LEFT JOIN seguranca.usuario u ON t.tabtusucpf = u.usucpf 
		ORDER BY tabelas";

$cabecalho = array("Ações", "Tabelas","Data de modificação","Usuário que modificou");

$db->monta_lista($sql,$cabecalho,50,5,'N','center',$par2);

?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0">
	<td colspan="2">
		<div style="float: left;">
		<input type='button' onclick="window.location='?modulo=principal/gerenciarestrutura&acao=A'" value='Inserir'>
		</div>
	</td>
</tr>
</table>

