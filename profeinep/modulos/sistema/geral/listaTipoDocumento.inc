<?php

include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
monta_titulo( 'PROFE/INEP', 'Lista de Tipos de Documentos' ); 
 
if( $_REQUEST['requisicao']=='excluir' && $_REQUEST['tpdid'] ) {
	$tpdid = $_REQUEST['tpdid']; 
	$sql = "UPDATE profeinep.tipodocumento SET  
			   tpdstatus = 'I'
			WHERE tpdid=$tpdid";
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
				<span style="cursor: pointer" onclick="cadastrarNovoRegistro();" title="Incluir Tipo de Documento">
					<img align="absmiddle" src="/imagens/gif_inclui.gif" /> <a href="#">Incluir Tipo de Documento</a>
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
		 	onclick=\"javascript: editarRegistro (\'' || tipodoc.tpdid || '\');\"
			title=\"Editar Tipo de Documento\">  
			
			<img
			align=\"absmiddle\"
			src=\"/imagens/excluir.gif\"
			style=\"cursor: pointer\"
		 	onclick=\"javascript: excluirRegistro (\'' || tipodoc.tpdid || '\');\"
			title=\"Excluir Tipo de Documento\"> 
			',
			
			tipodoc.tpddsc as descricao , 

			case when tipodoc.tpdgeranumeracao = 'S' THEN 'Sim'
			     else 'Não' end as tpdgeranumeracao, 
			case when tipodoc.tpdstatus = 'A' THEN 'Ativo'
			     else '<font color=red>Inativo</font>' end as tpdstatus 
		from
			profeinep.tipodocumento as tipodoc

		ORDER BY tipodoc.tpddsc ASC";

$cabecalho = array("Ações", "Descrição", "Produz Numeração", "Status" ); 

$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', ''); 

?>

<script>

function validaForm(){
	document.formulario.submit();
}

function cadastrarNovoRegistro() {
	return windowOpen( '?modulo=sistema/geral/cadDadosTipoDocumento&acao=A','blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function editarRegistro(tpdid) {
	return windowOpen( '?modulo=sistema/geral/cadDadosTipoDocumento&acao=A&tpdid='+ tpdid,'blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );  
}

function excluirRegistro(tpdid) {
   var confirma = confirm("Deseja Excluir o Tipo de Documento?"); 
   if ( confirma ){
		var req = new Ajax.Request('profeinep.php?modulo=sistema/geral/listaTipoDocumento&acao=A', {
	        method:     'post',
	        onComplete: function (res) {
				window.location.href = '?modulo=sistema/geral/listaTipoDocumento&acao=A&requisicao=excluir&tpdid='+ tpdid;
			}
		});
	   return true;
   } else {
	   return false;
   }
}

</script>