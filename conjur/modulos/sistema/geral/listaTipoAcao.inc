<?php

include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
monta_titulo( 'CONJUR', 'Lista de Tipos de Ação' ); 
 
if( $_REQUEST['requisicao']=='excluir' && $_REQUEST['tacid'] ) {
	$tacid = $_REQUEST['tacid']; 
	/*$sql = "UPDATE conjur.tipoacao SET  
			   tacstatus = 'I'
			WHERE tacid=$tacid";*/
	$sql = "DELETE FROM conjur.tipoacao WHERE tacid= {$tacid}";
	$db->executar($sql);
	$db->commit();		
}

?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<form action="" method="POST" name="formulario">
<input type='hidden' name="submetido" value="1">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<span style="cursor: pointer" onclick="cadastrarNovoRegistro();" title="Incluir Tipo de Ação">
					<img align="absmiddle" src="/imagens/gif_inclui.gif" /> <a href="#">Incluir Tipo de Ação</a>
				</span>
			</td>
		</tr>
	</tbody>
</table>
</form>

<? 
$sql = "SELECT
			'<img
			align=\"absmiddle\"
			src=\"/imagens/alterar.gif\"
			style=\"cursor: pointer\"
		 	onclick=\"javascript: editarRegistro (\'' || tac.tacid || '\');\"
			title=\"Editar Tipo de Ação\">  
			
			<img
			align=\"absmiddle\"
			src=\"/imagens/excluir.gif\"
			style=\"cursor: pointer\"
		 	onclick=\"javascript: excluirRegistro (\'' || tac.tacid || '\');\"
			title=\"Excluir Tipo de Ação\"> 
			',
			
			tac.tacdsc as descricao , 

			case when tac.tacstatus = 'A' THEN 'Ativo'
			     else '<font color=red>Inativo</font>' end as tacstatus 
		from
			conjur.tipoacao as tac

		ORDER BY tac.tacdsc ASC";

$cabecalho = array("Ações", "Descrição", "Status" ); 

$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', ''); 

?>

<script>

function cadastrarNovoRegistro() {
	return windowOpen( '?modulo=sistema/geral/cadDadosTipoAcao&acao=A','blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function editarRegistro(tacid) {
	return windowOpen( '?modulo=sistema/geral/cadDadosTipoAcao&acao=A&tacid='+ tacid,'blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );  
}

function excluirRegistro(tacid) {
   var confirma = confirm("Deseja Excluir o Tipo de Ação?"); 
   if ( confirma ){
		var req = new Ajax.Request('conjur.php?modulo=sistema/geral/listaTipoAcao&acao=A', {
	        method:     'post',
	        onComplete: function (res) {
				window.location.href = '?modulo=sistema/geral/listaTipoAcao&acao=A&requisicao=excluir&tacid='+ tacid;
			}
		});
	   return true;
   } else {
	   return false;
   }
}

</script>