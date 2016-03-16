<?php

$unidadeProcedenciaId = $_REQUEST["idExclusao"];
$requisicao = $_REQUEST["requisicao"];

if($requisicao=="deletar"){
	deletar($unidadeProcedenciaId); 		
	echo("<script>alert('O registro foi excluído com sucesso.');</script>");
}

function deletar($unidadeProcedenciaId) {
	global $db;
	$sql = "UPDATE conjur.unidadeprocedencia SET 
				undpstatus = 'I' 
			WHERE
	   			unpid =	".$unidadeProcedenciaId;
	$db->executar($sql);
	$db->commit();
}

function obterSqlUnidadesProcedencia() {
	$sql = "SELECT
			'<img 
			align=\"absmiddle\" 
			src=\"/imagens/alterar.gif\" 
			style=\"cursor: pointer\" 
		 	onclick=\"javascript: alterarRegistro(' || unp.unpid || ');\" 
			title=\"Editar Unidade de Procedência\" />
			
			<img
			align=\"absmiddle\"
			src=\"/imagens/excluir.gif\"
			style=\"cursor: pointer\"
		 	onclick=\"javascript: excluirRegistro(' || unp.unpid || ');\"
			title=\"Excluir Unidade de Procedência\" /> 
			',
			unp.unpdsc
		FROM
			conjur.unidadeprocedencia unp
		WHERE
			unp.undpstatus = 'A'
		ORDER BY
			unp.unpdsc ASC";
	
	return $sql;
} 


include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";

monta_titulo( 'CONJUR', 'Lista de Unidades de Procedência' ); 
 
?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script>
	function conPesquisaProcedencia(){
		document.getElementById('formulario').submit();
	}
</script>

<!-- Formulário de Pesquisa (soliticado por Luiz Fernando) -->
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="pesquisar"/>
	<input type="hidden" value="" id="idExclusao" name="idExclusao" />
</form>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<span style="cursor: pointer" onclick="cadastrarNovoRegistro(null);" title="Incluir Procedência">
					<img align="absmiddle" src="/imagens/gif_inclui.gif" /> <a href="#">Incluir Unidade de Procedência</a>
				</span>
			</td>
		</tr>
	</tbody>
</table>

<? 
	$sql = obterSqlUnidadesProcedencia();
	$cabecalho = array("Ações", "Descrição");
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', ''); 
?>

<script>

function cadastrarNovoRegistro() {
	return windowOpen( '?modulo=sistema/geral/cadDadosUnidadeProcedencia&acao=A','blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function alterarRegistro(id) {
	return windowOpen( '?modulo=sistema/geral/cadDadosUnidadeProcedencia&acao=A&unpid=' + id ,'blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );	
}

function excluirRegistro(id) {
	if( confirm("Deseja excluir a Unidade de Procedência ?") ) {
		if( atualizarFormularioParaExclusao(id) ) {
			var form = document.getElementById("formulario");
			form.submit();
		}
		else {
			alert('Erro ao tentar excluir registro');
		}
	}
}

function atualizarFormularioParaExclusao(id) {
	try {
		var idExclusao = document.getElementById("idExclusao");
		idExclusao.value = id;

		var requisicao = document.getElementById("requisicao");
		requisicao.value = "deletar";
	} catch(err) {
		return false;
	}
	return true;
}

</script>