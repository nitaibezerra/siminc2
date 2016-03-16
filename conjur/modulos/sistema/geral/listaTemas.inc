<?php

$temaId = $_REQUEST["idExclusao"];
$requisicao = $_REQUEST["requisicao"];

if($requisicao=="deletar"){
	deletar($temaId); 		
	echo("<script>alert('O registro foi excluído com sucesso.');</script>");
}

function deletar($temaId) {
	global $db;
	$sql = "UPDATE conjur.tipoassunto SET 
				tasstatus = 'I' 
			WHERE
	   			tasid =	".$temaId;
	$db->executar($sql);
	$db->commit();
}

function obterSqlTema() {
	$sql = "SELECT
			'<img 
			align=\"absmiddle\" 
			src=\"/imagens/alterar.gif\" 
			style=\"cursor: pointer\" 
		 	onclick=\"javascript: alterarRegistro(' || tas.tasid || ');\" 
			title=\"Editar Tema\" />
			
			<img
			align=\"absmiddle\"
			src=\"/imagens/excluir.gif\"
			style=\"cursor: pointer\"
		 	onclick=\"javascript: excluirRegistro(' || tas.tasid || ');\"
			title=\"Excluir Tema\" /> 
			',
			tas.tasdsc
		FROM
			conjur.tipoassunto tas
		WHERE
			tas.tasstatus = 'A'
		ORDER BY
			tas.tasdsc ASC";
	
	return $sql;
} 


include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";

monta_titulo( 'CONJUR', 'Lista de Temas' ); 
 
?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script>
	function conPesquisaTema(){
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
				<span style="cursor: pointer" onclick="cadastrarNovoRegistro(null);" title="Incluir Tema">
					<img align="absmiddle" src="/imagens/gif_inclui.gif" /> <a href="#">Incluir Tema</a>
				</span>
			</td>
		</tr>
	</tbody>
</table>

<? 
	$sql = obterSqlTema();
	$cabecalho = array("Ações", "Descrição");
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', ''); 
?>

<script>

function cadastrarNovoRegistro() {
	return windowOpen( '?modulo=sistema/geral/cadDadosTemas&acao=A','blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function alterarRegistro(id) {
	return windowOpen( '?modulo=sistema/geral/cadDadosTemas&acao=A&tasid=' + id ,'blank','height=250,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );	
}

function excluirRegistro(id) {
	if( confirm("Deseja excluir o Tema?") ) {
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