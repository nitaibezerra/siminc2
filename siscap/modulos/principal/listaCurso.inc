<?php

if( $_REQUEST['exclui'] ){
	if( $_REQUEST['curid'] ){
		$sql = "DELETE FROM siscap.curso WHERE curid = ".$_REQUEST['curid'];
		$db->executar( $sql );
		$db->commit();
	}
	montaListaCursos();
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Listar Cursos', 'Filtro de Pesquisa' );

?>
<form method="post" name="formulario2" id="formulario2">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td width="20%" class="subtitulodireita">Nome do Curso:</td>
			<td>
				<?php
				$curnome = $_POST['curnome'];
					echo campo_texto( 'curnome', 'N', 'S', '', 80, 50, '','');
				?>
			</td>
		</tr>
        <tr bgcolor="#D0D0D0">
            <td></td>
            <td style="text-align: left">
				<input type="button" id="btPesquisar" value="Pesquisar" onclick="pesquisar();" />
				<input type="button" id="btCadastro" value="Novo" onclick="novoCurso()" />
			</td>
		</tr>
	</table>
</form>
	<?php
		monta_titulo( '', 'Lista de Curso(s) Cadastrado(s)' );
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="50%">
		<tr>
			<td>
				<div  id='listaCurso' style="display: ''">
				<?php
					//$w = $_POST['curnome'] ? $_POST['curnome'] : "";
					montaListaCursos( $_POST['curnome'] );
				?>
				</div>
			</td>
		</tr>
	</table>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="JavaScript">
	form		=	$("formulario2");
	btPesquisar	=	$("btPesquisar");
	btCadastro	=	$("btCadastro");

	function pesquisar(){
		btPesquisar.disabled = true;
		btCadastro.disabled  = true;

		curnome	= document.getElementsByName("curnome")[0];

		if(curnome.value.length > 0 && curnome.value.length < 3) {
			alert("Para realizar a busca por nome do curso é necessário informar pelo menos 3 caracteres.");
			curnome.focus();
			btPesquisar.disabled = false;
			btCadastro.disabled  = false;
			return;
		}
		form.submit();
	}
	function novoCurso(){
		btPesquisar.disabled = true;
		btCadastro.disabled  = true;
		window.location.href='/siscap/siscap.php?modulo=principal/gerenciaCurso&acao=A';
	}
	function alterarCurso( curid ){
		window.location.href='/siscap/siscap.php?modulo=principal/gerenciaCurso&acao=A&curid='+curid;
	}
	function visualizarCurso( curid ){
		window.location.href='/siscap/siscap.php?modulo=principal/gerenciaCurso&acao=A&visualiza=1&curid='+curid;
	}
	function excluirCurso( curid ){
		if( confirm( "Deseja realmente excluir o Curso?" ) ){
			var myAjax = new Ajax.Request('siscap.php?modulo=principal/listaCurso&acao=A', {
				method 	   	 : 'post',
				parameters 	 : 'exclui=true&curid='+curid,
				asynchronous : false,
				onComplete   : function(res){

					if(res.responseText)
					{
						alert("Registro excluído com sucesso!");
						$('listaCurso').innerHTML = res.responseText;
					}
					else
					{
						alert("Erro ao excluir o registro!");
					}
				}
			});
		}
	}
</script>