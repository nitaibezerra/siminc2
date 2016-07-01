<?php

if( $_REQUEST['exclui'] ){
	if( $_REQUEST['facid'] ){
		//verifica se ele tem telefones
		$sql = "SELECT telid FROM siscap.telefone WHERE facid = ".$_REQUEST['facid'];
		$arrTel = $db->carregarColuna( $sql );
		if( is_array($arrTel) && $arrTel[0] ){
			$sql = "DELETE FROM siscap.telefone WHERE telid IN ( ".implode(", ", $arrTel).")";
			$db->executar( $sql );
		}
		//verifica se esta vinculado a uma turma
		$sql = "SELECT tfid FROM siscap.turma_facilitador WHERE facid = ".$_REQUEST['facid'];
		$arrTF = $db->carregarColuna( $sql );
		if( is_array($arrTF) && $arrTF[0] ){
			$sql = "DELETE FROM siscap.turma_facilitador WHERE tfid IN (".implode(", ", $arrTF).")";
			$db->executar( $sql );
		}
		//deleta o facilitador
		$sql = "DELETE FROM siscap.facilitador WHERE facid = ".$_REQUEST['facid'];
		$db->executar( $sql );
		$db->commit();
	}
	montaListaFacilitador();
	exit;
}

unset($_SESSION['siscap']['facid']);
unset($_SESSION['siscap']['visualizar']);

include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Listar Facilitadores', 'Filtros de Pesquisa' );

?>
<form method="post" name="formulario2" id="formulario2">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td width="20%" class="subtitulodireita">Nome do Facilitador:</td>
			<td>
				<?php
					$facnome = $_POST['facnome'];
					echo campo_texto( 'facnome', 'N', 'S', '', 80, 50, '','');
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
		monta_titulo( '', 'Lista de Facilitador(es) Cadastrado(s)' );
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="50%">
		<tr>
			<td>
				<div  id='listaFacilitador' style="display: ''">
				<?php
					//$w = $_POST['curnome'] ? $_POST['curnome'] : "";
					montaListaFacilitador( $_POST['facnome'] );
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

		facnome	= document.getElementsByName("facnome")[0];

		if(facnome.value.length > 0 && facnome.value.length < 3) {
			alert("Para realizar a busca por nome do curso é necessário informar pelo menos 3 caracteres.");
			facnome.focus();
			btPesquisar.disabled = false;
			btCadastro.disabled  = false;
			return;
		}
		form.submit();
	}
	function novoCurso(){
		btPesquisar.disabled = true;
		btCadastro.disabled  = true;
		window.location.href='/siscap/siscap.php?modulo=principal/dadosPessoais&acao=A';
	}
	function alterarFacilitador( facid ){
		window.location.href='/siscap/siscap.php?modulo=principal/dadosPessoais&acao=A&facid='+facid;
	}
	function visualizarFacilitador( facid ){
		window.location.href='/siscap/siscap.php?modulo=principal/dadosPessoais&acao=A&visualizar=1&facid='+facid;
	}
	function excluirFacilitador( facid ){
		if( confirm( "Deseja realmente excluir o facilitador?" )){
			var myAjax = new Ajax.Request('siscap.php?modulo=principal/listaFacilitador&acao=A', {
				method 	   	 : 'post',
				parameters 	 : 'exclui=true&facid='+facid,
				asynchronous : false,
				onComplete   : function(res){

					if(res.responseText)
					{
						alert("Registro excluído com sucesso!");
						$('listaFacilitador').innerHTML = res.responseText;
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