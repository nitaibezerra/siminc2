<?php

		//unset($_SESSION['naiid']);
		//unset($_SESSION['op2']);

function inserirNivel(){
	global $db;
	$sql = " INSERT INTO demandas.nivelatendimento
			(niadescricao, niastatus) VALUES
			('".$_POST['niadescricao']."', '".$_POST['niastatus']."')
	";
	$db->executar($sql);
	$db->commit();
	header( "Location: demandas.php?modulo=sistema/apoio/nivelAtendimento&acao=A" );
	exit();
}


function selecionarNivel(){
	global $db;
	$sql = "SELECT niaid, niadescricao, 
	CASE niastatus
	WHEN 'A' THEN 'Ativo'   
	ELSE 'Inativo' 
	END AS niastatus
	FROM  demandas.nivelatendimento  
				WHERE niaid = '".$_SESSION['niaid']."'
	";
	return $db->carregar($sql);
}


function alterarNivel(){
	global $db;
	$sql = "UPDATE demandas.nivelatendimento SET 
			niadescricao = '".$_POST['niadescricao']."',
			niastatus = '".$_POST['niastatus']."' 
				WHERE niaid = '".$_POST['niaid']."'
	";
	$db->executar($sql);
	$db->commit();
	
}

function deletarNivel(){
	global $db;
	$sql = " DELETE FROM demandas.nivelatendimento 
				WHERE niaid = '".$_SESSION['niaid']."'
	";
	$db->executar($sql);
	$db->commit();
}

if($_POST['niadescricao'] && $_POST['niastatus'] && !$_POST['niaid']){
	inserirNivel();
	unset($_SESSION['niaid']);
	unset($_SESSION['op2']);
	print "<script>
				alert('Operação realizada com sucesso!');
			 </script>";
	header( "Location: demandas.php?modulo=sistema/apoio/nivelAtendimento&acao=A" );
	exit();
	
}

if($_POST['niadescricao'] && $_POST['niastatus'] && $_POST['niaid']){
	alterarNivel();
	unset($_SESSION['niaid']);
	print "<script>
				alert('Operação realizada com sucesso!');
			 </script>";
	unset($_SESSION['niaid']);
	unset($_SESSION['op2']);
	header( "Location: demandas.php?modulo=sistema/apoio/nivelAtendimento&acao=A" );
	exit();
}

if($_GET['niaid'] && $_GET['op2']){
	session_start();
	$_SESSION['niaid'] = $_GET['niaid'];
	$_SESSION['op2'] = $_GET['op2'];
	header( "Location: demandas.php?modulo=sistema/apoio/nivelAtendimento&acao=A" );
	exit();
}

if($_SESSION['niaid'] && $_SESSION['op2']){
	if($_SESSION['op2'] == 'delete'){
		deletarNivel();	
		unset($_SESSION['niaid']);
		unset($_SESSION['op2']);
	}
	if($_SESSION['op2'] == 'update'){
		$dados = selecionarNivel();	
		unset($_SESSION['niaid']);
		unset($_SESSION['op2']);
	}

}


include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Grau de Severidade', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formAtendimento" name="formAtendimento" action="" method="post" onsubmit="return validaForm();" >
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<input type="hidden" name="niaid" value="<? echo $dados[0]['niaid']; ?>">
	<tr>
		<td align='right' class="SubTituloDireita" >Grau de Severidade</td>
		<td>
			<? $niadescricao = $dados[0]['niadescricao'];  ?>
			<?= campo_texto( 'niadescricao', 'N', 'S', '', 80, 250, '', '','','','','',''); echo obrigatorio();?>
		</td>
		<td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">
			Situação:
		</td>
		<td>
			<select id="niastatus" name="niastatus" class='CampoEstilo' >
			<option value=''>-- Informe a Situação --</option>
			<option <?php ($dados[0]['niastatus'] == 'Ativo')? $s = "selected=\"selected\"" : $s= ""; echo $s; ?> value="A">Ativo</option>
			<option <?php ($dados[0]['niastatus'] == 'Inativo')? $s2 = "selected=\"selected\"" : $s2 =""; echo $s2; ?> value="I">Inativo</option>
			</select>
			<? echo obrigatorio(); ?>
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
				<td></td>
				<td>
				<input type="submit" class="botao" name="btalterar" value="Salvar" /> 
				</td>
			</tr>
	<tr>
	</table>
	</form>
	<?php
				$sql = "SELECT
				'<a href=\"demandas.php?modulo=sistema/apoio/nivelAtendimento&acao=A&niaid=' || niaid || '&op2=update\"><img border=0 src=\"../imagens/alterar.gif\" /></a> <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/nivelAtendimento&acao=A&niaid=' || niaid || '&op2=delete\');\" ><img border=0 src=\"../imagens/excluir.gif\" /></a>', 
				niadescricao,
				 CASE
				 WHEN niastatus = 'A' THEN 'Ativo'
				 ELSE 'Inativo'
				 END
				 FROM demandas.nivelatendimento 
				ORDER BY 
				 niaid";
				$cabecalho = array( "Opções","Nível de Atendimento", "Situação");
				$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
				?>
<script type="text/javascript">
function validaForm(){
	
	if(document.formAtendimento.niadescricao.value == ''){
		alert ('O campo Nível de Atendimento deve ser preenchido.');
		document.formAtendimento.niadescricao.focus();
		return false;
	}
	if(document.formAtendimento.niastatus.value == ''){
		alert ('Informe a Situação.');
		document.formAtendimento.niastatus.focus();
		return false;
	}
}

function exclusao(url) {
	var questao = confirm("Tem Certeza?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>