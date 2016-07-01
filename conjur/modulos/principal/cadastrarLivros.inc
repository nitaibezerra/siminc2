<?php

function inserirLivro(){
	global $db;
	
	//verifica se ja existe
	$sql = "SELECT count(lvaid) as total
			FROM conjur.livroacervo
			WHERE lvastatus = 'A'
			and (upper(lvapatrimonio) = '".str_to_upper($_POST['lvapatrimonio'])."' 
			OR upper(lvatitulo) = '".str_to_upper($_POST['lvatitulo'])."')";
	$total = $db->pegaUm($sql);
	
	if($total>0){
		print "<script>
				alert('Já existe um livro com este Patrimônio ou Título!');
				history.back();
			   </script>";
		exit();
	}
	
	$sql = " INSERT INTO conjur.livroacervo
			 (
			 	lvapatrimonio, 
			 	lvatitulo,
			 	lvaautor,
			 	lvaeditora,
			 	lvaedicao,
			 	lvadtinclusao,
			 	lvastatus
			 ) VALUES (
			 	'".$_POST['lvapatrimonio']."',
			 	'".$_POST['lvatitulo']."',
			 	'".$_POST['lvaautor']."',
			 	'".$_POST['lvaeditora']."',
			 	'".$_POST['lvaedicao']."',  
			 	now(),
			 	'A'
			 ) returning lvaid; ";
	$lvaid = $db->pegaUm($sql);
	$db->commit();
	
	$docid = criarDocumentoB( $lvaid );

}



function selecionarLivro(){
	global $db;
	$sql = "SELECT 
				lvaid,
				lvapatrimonio, 
			 	lvatitulo,
			 	lvaautor,
			 	lvaeditora,
			 	lvaedicao,
			 	lvadtinclusao,
			 	lvastatus
			FROM  
				conjur.livroacervo
			WHERE 
				lvaid = '".$_SESSION['lvaid']."'";
	return $db->carregar($sql);
}



function alterarLivro(){
	global $db;
	
	//verifica se ja existe
	$sql = "SELECT count(lvaid) as total
			FROM conjur.livroacervo
			WHERE lvastatus = 'A'
			and lvaid not in (".$_POST['lvaid'].")
			AND (upper(lvapatrimonio) = '".str_to_upper($_POST['lvapatrimonio'])."' 
			OR upper(lvatitulo) = '".str_to_upper($_POST['lvatitulo'])."')";
	$total = $db->pegaUm($sql);
	
	if($total>0){
		print "<script>
				alert('Já existe um livro com este Patrimônio ou Título!');
				history.back();
			   </script>";
		exit();
	}
	
	
	$sql = "UPDATE 
				conjur.livroacervo 
			SET 
				lvapatrimonio = '".$_POST['lvapatrimonio']."',
				lvatitulo = '".$_POST['lvatitulo']."',
				lvaautor = '".$_POST['lvaautor']."',
				lvaeditora = '".$_POST['lvaeditora']."',
				lvaedicao = '".$_POST['lvaedicao']."'
			WHERE 
				lvaid = '".$_POST['lvaid']."';";

	$db->executar($sql);					
	$db->commit();
}



function deletarLivro(){
	global $db;
	
	$sql = "UPDATE conjur.livroacervo SET lvastatus='I' WHERE lvaid=".$_SESSION['lvaid'];
	$db->executar($sql);
	$db->commit();
}

if($_POST['lvapatrimonio'] && !$_POST['lvaid']){
	inserirLivro();
	unset($_SESSION['lvaid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='conjur.php?modulo=principal/listaBiblioteca&acao=A'</script>";
	exit();
	
}

if($_POST['lvapatrimonio'] && $_POST['lvaid']){
	alterarLivro();
	unset($_SESSION['lvaid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='conjur.php?modulo=principal/listaBiblioteca&acao=A'</script>";
	exit();
}

if($_GET['lvaid'] && $_GET['op3']){
	session_start();
	$_SESSION['lvaid'] = $_GET['lvaid'];
	$_SESSION['op3'] = $_GET['op3'];
	header( "Location: conjur.php?modulo=principal/cadastrarLivros&acao=A" );
	exit();
}

if($_SESSION['lvaid'] && $_SESSION['op3']){
	if($_SESSION['op3'] == 'delete'){
		deletarLivro();	
		unset($_SESSION['lvaid']);
		unset($_SESSION['op3']);
		print "<script>alert('Operação realizada com sucesso!');</script>";
		print "<script>location.href='conjur.php?modulo=principal/listaBiblioteca&acao=A'</script>";
	}
	if($_SESSION['op3'] == 'update'){
		$dados = selecionarLivro();	
		unset($_SESSION['lvaid']);
		unset($_SESSION['op3']);
	}

}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';


	$menu[0] = array("descricao" => "Biblioteca", "link"=> "conjur.php?modulo=principal/listaBiblioteca&acao=A");
	$menu[1] = array("descricao" => "Livros", "link"=> "conjur.php?modulo=principal/cadastrarLivros&acao=A");
	echo montarAbasArray($menu, "conjur.php?modulo=principal/cadastrarLivros&acao=A");


monta_titulo( 'Cadastrar/Editar Livros', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>

<form id="formulario" name="formulario" action="" method="post" onsubmit="return validaForm();" >

<input type="hidden" name="lvaid" value="<? echo $dados[0]['lvaid']; ?>" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="40%" align='right' class="SubTituloDireita">Patrimônio:</td>
		<td>
			<? $lvapatrimonio = $dados[0]['lvapatrimonio'];  ?>
			<?= campo_texto( 'lvapatrimonio', 'S', 'S', '', 7, 6, '', '','','','','','');?>
		</td>
	</tr>
	<tr>
		<td width="40%" align='right' class="SubTituloDireita">Título:</td>
		<td>
			<? $lvatitulo = $dados[0]['lvatitulo'];  ?>
			<?= campo_texto( 'lvatitulo', 'S', 'S', '', 50, 100, '', '','','','','','');?>
		</td>
	</tr>
	<tr>
		<td width="40%" align='right' class="SubTituloDireita">Autor:</td>
		<td>
			<? $lvaautor = $dados[0]['lvaautor'];  ?>
			<?= campo_texto( 'lvaautor', 'S', 'S', '', 50, 50, '', '','','','','','');?>
		</td>
	</tr>
	<tr>
		<td width="40%" align='right' class="SubTituloDireita">Editora:</td>
		<td>
			<? $lvaeditora = $dados[0]['lvaeditora'];  ?>
			<?= campo_texto( 'lvaeditora', 'S', 'S', '', 50, 50, '', '','','','','','');?>
		</td>
	</tr>
	<tr>
		<td width="40%" align='right' class="SubTituloDireita">Edição:</td>
		<td>
			<? $lvaedicao = $dados[0]['lvaedicao'];  ?>
			<?= campo_texto( 'lvaedicao', 'S', 'S', '', 20, 20, '', '','','','','','');?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="submit" class="botao" name="btalterar" value="Salvar">
		<?php
		if (isset($dados)){
			echo '&nbsp;<input type="button" class="botao" name="del" value="Novo" onclick="javascript:location.href = window.location;">';	
		}
		?>
		&nbsp;<input type="button" class="botao" name="voltar" value="Voltar" onclick="javascript:location.href = 'conjur.php?modulo=principal/listaBiblioteca&acao=A';">
		</td>
	</tr>
</table>
</form>

<?php
	/*
	$sql = "SELECT
				'<a href=\"conjur.php?modulo=principal/cadastrarLivros&acao=A&lvaid=' || lvaid || '&op3=update\">
				   <img border=0 src=\"../imagens/alterar.gif\" />
				 </a> 
				 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'conjur.php?modulo=principal/cadastrarLivros&acao=A&lvaid=' || lvaid || '&op3=delete\');\" >
				   <img border=0 src=\"../imagens/excluir.gif\" />
				 </a>' as acao, 
				lvapatrimonio,
				lvatitulo,
				lvaautor,
				lvaeditora,
				lvaedicao
			FROM 
				conjur.livroacervo
			WHERE
				lvastatus = 'A'
		  	ORDER BY 
				 lvatitulo";
	
	$cabecalho = array( "Ação","Patrimônio","Título","Autor","Editora","Edição");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
	*/
?>


<script type="text/javascript">
function validaForm(){
	
	if(document.formulario.lvapatrimonio.value == ''){
		alert ('O campo Patrimônio deve ser preenchido.');
		document.formulario.lvapatrimonio.focus();
		return false;
	}
	if(document.formulario.lvatitulo.value == ''){
		alert ('O campo Título deve ser preenchido.');
		document.formulario.lvatitulo.focus();
		return false;
	}
	if(document.formulario.lvaautor.value == ''){
		alert ('O campo Autor deve ser preenchido.');
		document.formulario.lvaautor.focus();
		return false;
	}
	if(document.formulario.lvaeditora.value == ''){
		alert ('O campo Editora deve ser preenchido.');
		document.formulario.lvaeditora.focus();
		return false;
	}
	if(document.formulario.lvaedicao.value == ''){
		alert ('O campo Edição deve ser preenchido.');
		document.formulario.lvaedicao.focus();
		return false;
	}
	
}

/*
function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este registro?")
	if (questao){
		window.location = url;
	}
}
*/
</script>
</body>