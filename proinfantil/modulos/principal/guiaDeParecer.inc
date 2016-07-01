<?php

function inserirValor(){
	global $db;
	
	$sql = "SELECT count(parid) as total FROM proinfantil.parecer  
			WHERE parstatus = 'A' 
			and tipid = ".$_POST['tipid']." 
			and modid = '".$_POST['modid']."'
			and pardescricao = '".$_POST['pardescricao']."'";
	$total = $db->pegaUm($sql);
	
	if($total == 0){
		$sql = " INSERT INTO proinfantil.parecer
				 (
				 	tipid, 
				 	modid,
				 	pardescricao,
				 	parstatus
				 ) VALUES (
				 	'".$_POST['tipid']."',
				 	'".$_POST['modid']."',
				 	'".$_POST['pardescricao']."',
				 	'A'
				 );";
		$db->executar($sql);
		$db->commit();
	}
	else{
		print '<script>
					alert("Já existe este Parecer. \nFavor verificar.");
					history.back();
			  </script>';
		exit;
	}
	
}

function selecionarValor(){
	global $db;
	$sql = "SELECT 
				parid, 
				tipid,
				modid,
				pardescricao
			FROM  
				proinfantil.parecer  
			WHERE 
				parid = '".$_SESSION['parid']."'";
	
	return $db->carregar($sql);
}


function alterarValor(){
	global $db;
	
	$sql = "SELECT count(parid) as total FROM proinfantil.parecer  
			WHERE parstatus = 'A' 
			and tipid = ".$_POST['tipid']."
			and modid = '".$_POST['modid']."'
			and pardescricao = '".$_POST['pardescricao']."'
			AND parid not in (".$_POST['parid'].")";
	$total = $db->pegaUm($sql);
	
	if($total == 0){
		$sql = "UPDATE 
					proinfantil.parecer 
				SET 
					tipid = '".$_POST['tipid']."',
					modid = '".$_POST['modid']."',
					pardescricao = '".$_POST['pardescricao']."'
				WHERE 
					parid = '".$_POST['parid']."';";
	
		$db->executar($sql);					
		$db->commit();
	}
	else{
		print '<script>
					alert("Já existe este Parecer. \nFavor verificar.");
					history.back();
			  </script>';
		exit;
	}
		
}


function deletarValor(){
	global $db;
	
	$sql = " UPDATE proinfantil.parecer SET parstatus='I' WHERE parid=".$_SESSION['parid'];
	$db->executar($sql);
	$db->commit();
}

if($_POST['tipid'] && $_POST['pardescricao'] && !$_POST['parid']){
	inserirValor();
	unset($_SESSION['parid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='proinfantil.php?modulo=principal/guiaDeParecer&acao=A&modid=".$_POST['modid']."'</script>";
	//header( "Location: proinfantil.php?modulo=principal/guiaDeParecer&acao=A" );
	exit();
	
}

if($_POST['tipid'] && $_POST['pardescricao'] && $_POST['parid']){
	alterarValor();
	unset($_SESSION['parid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='proinfantil.php?modulo=principal/guiaDeParecer&acao=A&modid=".$_POST['modid']."'</script>";
	//header( "Location: proinfantil.php?modulo=principal/guiaDeParecer&acao=A" );
	exit();
}

if($_GET['parid'] && $_GET['op3']){
	session_start();
	$_SESSION['parid'] = $_GET['parid'];
	$_SESSION['op3'] = $_GET['op3'];
	header( "Location: proinfantil.php?modulo=principal/guiaDeParecer&acao=A" );
	exit();
}

if($_SESSION['parid'] && $_SESSION['op3']){
	if($_SESSION['op3'] == 'delete'){
		deletarValor();	
		$modid = $db->pegaUm("select modid from proinfantil.parecer where parid = ".$_SESSION['parid']);
		unset($_SESSION['parid']);
		unset($_SESSION['op3']);
		print "<script>alert('Operação realizada com sucesso!');</script>";
		print "<script>location.href='proinfantil.php?modulo=principal/guiaDeParecer&acao=A&modid=".$modid."'</script>";
	}
	if($_SESSION['op3'] == 'update'){
		$dados = selecionarValor();	
		unset($_SESSION['parid']);
		unset($_SESSION['op3']);
	}

}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Guia de Parecer', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<form id="formulario" name="formulario" action="" method="post"  >

<input type="hidden" name="parid" value="<? echo $dados[0]['parid']; ?>" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr >
		<td align='right' class="subtitulodireita">Selecione o Módulo:</td>
		<td>
			<?php
				$sql = "SELECT
						 modid AS codigo,
						 moddsc AS descricao
						FROM
						 proinfantil.modulo 
						 WHERE modstatus = 'A' 
						ORDER BY
						 2;";
				$modid = $_REQUEST['modid'] ? $_REQUEST['modid'] : $dados[0]['modid'];
				$db->monta_combo("modid",$sql,'S',"-- Selecione --",'filtraModulo','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>
	<tr >
		<td align='right' colspan="2" class="subtitulodireita">&nbsp;</td>
	</tr>
	<tr >
		<td align='right' class="subtitulodireita">Tipo:</td>
		<td>
			<?php
				$sql = "SELECT
						 tipid AS codigo,
						 tipdescricao AS descricao
						FROM
						 proinfantil.tipoparecer 
						ORDER BY
						 2;";
				$tipid = $dados[0]['tipid'] ? $dados[0]['tipid'] : $_REQUEST['tipid'];
				$db->monta_combo("tipid",$sql,'S',"-- Selecione --",'','');
			?>
			<?= obrigatorio(); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" width="45%" valign="top">Parecer:</td>
		<td>
			<? 
				$pardescricao = $dados[0]['pardescricao'] ? $dados[0]['pardescricao'] : $_REQUEST['pardescricao']; 
			?>
			<?= campo_textarea("pardescricao",'S','S','Pardescricao',80,10,4000); ?>
		</td>
	</tr>
	
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="button" class="botao" name="btalterar" value="Salvar" onclick="validaForm();">
		<?php
		if (isset($dados)){
			echo '<input type="button" class="botao" name="del" value="Novo" onclick="javascript:location.href = window.location;">';	
		}
		?>
		</td>
	</tr>
	</table>
</form>
	
	<?
	
	if($_REQUEST['modid']){
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr >
						<td align="right" colspan="2" class="subtitulocentro"><br>Lista de Pareceres</td>
					</tr>
				</table>';
		
		$sql = "SELECT
					'<center><a href=\"proinfantil.php?modulo=principal/guiaDeParecer&acao=A&parid=' || o.parid || '&op3=update\">
					   <img border=0 src=\"../imagens/alterar.gif\" />
					 </a> 
					 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'proinfantil.php?modulo=principal/guiaDeParecer&acao=A&parid=' || o.parid || '&op3=delete\');\" >
					   <img border=0 src=\"../imagens/excluir.gif\" />
					 </a></center>' as opcao, 
					o.pardescricao,
					t.tipdescricao as tipo,
					m.moddsc as modulo
				FROM 
					proinfantil.parecer o
				INNER JOIN
					proinfantil.tipoparecer t on t.tipid = o.tipid
				INNER JOIN
					proinfantil.modulo m on m.modid = o.modid
				WHERE
					o.parstatus = 'A'
				and
					o.modid = ".$_REQUEST['modid']."
			  	ORDER BY 
					 2";
		$cabecalho = array( "<center>Ação</center>","Parecer","Tipo","Módulo");
		$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
	}
	?>
	

<script type="text/javascript">

function filtraModulo(){
	document.formulario.submit();
}

function validaForm(){
	
	if(document.formulario.modid.value == ''){
		alert ('O campo Módulo deve ser preenchido.');
		document.formulario.modid.focus();
		return false;
	}
	if(document.formulario.tipid.value == ''){
		alert ('O campo Tipo deve ser preenchido.');
		document.formulario.tipid.focus();
		return false;
	}
	if(document.formulario.pardescricao.value == ''){
		alert ('O campo Parecer deve ser preenchido.');
		document.formulario.pardescricao.focus();
		return false;
	}

	document.formulario.submit();

}


function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este Parecer?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>