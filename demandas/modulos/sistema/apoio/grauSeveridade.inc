<?php
 
		//unset($_SESSION['grsid']);
		//unset($_SESSION['op']);

function inserirGrau(){
	global $db;
	$sql = " INSERT INTO demandas.grauseveridade
			(grsdescricao, grsstatus) VALUES
			('".$_POST['grsdescricao']."', '".$_POST['grsstatus']."')
	";
	$db->executar($sql);
	$db->commit();
	header( "Location: demandas.php?modulo=sistema/apoio/grauSeveridade&acao=A" );
	exit();
}


function selecionarGrau(){
	global $db;
	$sql = "SELECT grsid, grsdescricao, 
	CASE grsstatus
	WHEN 'A' THEN 'Ativo'   
	ELSE 'Inativo' 
	END AS grsstatus
	FROM  demandas.grauseveridade  
				WHERE grsid = '".$_SESSION['grsid']."'
	";
	return $db->carregar($sql);
}


function alterarGrau(){
	global $db;
	$sql = "UPDATE demandas.grauseveridade SET 
			grsdescricao = '".$_POST['grsdescricao']."',
			grsstatus = '".$_POST['grsstatus']."' 
				WHERE grsid = '".$_POST['grsid']."'
	";
	$db->executar($sql);
	$db->commit();
	
}

function deletarGrau(){
	global $db;
	$sql = "UPDATE demandas.grauseveridade SET 
			grsstatus = 'I' 
				WHERE grsid = '".$_SESSION['grsid']."'
	";
	$db->executar($sql);
	$db->commit();
}

if($_POST['grsdescricao'] && $_POST['grsstatus'] && !$_POST['grsid']){
	inserirGrau();
	unset($_SESSION['grsid']);
	unset($_SESSION['op']);
	print "<script>
				alert('Operação realizada com sucesso!');
			 </script>";
	header( "Location: demandas.php?modulo=sistema/apoio/grauSeveridade&acao=A" );
	exit();
	
}

if($_POST['grsdescricao'] && $_POST['grsstatus'] && $_POST['grsid']){
	alterarGrau();
	unset($_SESSION['grsid']);
	print "<script>
				alert('Operação realizada com sucesso!');
			 </script>";
	unset($_SESSION['grsid']);
	unset($_SESSION['op']);
	header( "Location: demandas.php?modulo=sistema/apoio/grauSeveridade&acao=A" );
	exit();
}

if($_GET['grsid'] && $_GET['op']){
	session_start();
	$_SESSION['grsid'] = $_GET['grsid'];
	$_SESSION['op'] = $_GET['op'];
	header( "Location: demandas.php?modulo=sistema/apoio/grauSeveridade&acao=A" );
	exit();
}

if($_SESSION['grsid'] && $_SESSION['op']){
	if($_SESSION['op'] == 'delete'){
		deletarGrau();	
		unset($_SESSION['grsid']);
		unset($_SESSION['op']);
	}
	if($_SESSION['op'] == 'update'){
		$dados = selecionarGrau();	
		unset($_SESSION['grsid']);
		unset($_SESSION['op']);
	}

}


include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Grau de Severidade', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formSeveridade" name="formSeveridade" action="" method="post" onsubmit="return validaForm();" >
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<input type="hidden" name="grsid" value="<? echo $dados[0]['grsid']; ?>">
	<tr>
		<td width="25%" align="right" class="SubTituloDireita" >Grau de Severidade:</td>
		<td>
			<? $grsdescricao = $dados[0]['grsdescricao'];  ?>
			<?= campo_texto( 'grsdescricao', 'N', 'S', '', 80, 250, '', '','','','','',''); echo obrigatorio();?>
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" >
		Situação:
		</td>
		<td>
			<select id="grsstatus" name="grsstatus" class='CampoEstilo' >
		<option value=''>-- Informe a Situação --</option>
			<option <?php ($dados[0]['grsstatus'] == 'Ativo')? $s = "selected=\"selected\"" : $s= ""; echo $s; ?> value="A">Ativo</option>
			<option <?php ($dados[0]['grsstatus'] == 'Inativo')? $s2 = "selected=\"selected\"" : $s2 =""; echo $s2; ?> value="I">Inativo</option>
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
				'<a href=\"demandas.php?modulo=sistema/apoio/grauSeveridade&acao=A&grsid=' || grsid || '&op=update\"><img border=0 src=\"../imagens/alterar.gif\" /></a> <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/grauSeveridade&acao=A&grsid=' || grsid || '&op=delete\');\" ><img border=0 src=\"../imagens/excluir.gif\" /></a>', 
				grsdescricao,
				 CASE
				 WHEN grsstatus = 'A' THEN 'Ativo'
				 ELSE 'Inativo'
				 END
				 FROM demandas.grauseveridade 
				ORDER BY 
				 grsid";
				$cabecalho = array( "Opções","Grau de Severidade", "Situação");
				$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
				?>
<script type="text/javascript">
function validaForm(){
	
	if(document.formSeveridade.grsdescricao.value == ''){
		alert ('O campo Grau de Severidade deve ser preenchido.');
		document.formSeveridade.grsdescricao.focus();
		return false;
	}
	if(document.formSeveridade.grsstatus.value == ''){
		alert ('Informe a situação.');
		document.formSeveridade.grsstatus.focus();
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