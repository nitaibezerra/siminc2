<?php



function inserirFeriado(){
	global $db;
	
	$sql = " INSERT INTO demandas.feriado
			 (
			 	frdtitulo, 
			 	frddata,
			 	frdstatus
			 ) VALUES (
			 	'".$_POST['frdtitulo']."',
			 	'".formata_data_sql($_POST['frddata'])."',  
			 	'A'
			 );";
	$db->executar($sql);
	$db->commit();
}

function selecionarFeriado(){
	global $db;
	$sql = "SELECT 
				o.frdid, 
				o.frdtitulo,
				to_char(o.frddata::timestamp,'DD/MM/YYYY') as frddata,
				CASE frdstatus
					WHEN 'A' THEN 'ATIVO'   
					ELSE 'INATIVO' 
				END AS frdstatus
			FROM  
				demandas.feriado o 
			WHERE 
				o.frdid = '".$_SESSION['frdid']."'
	";
	return $db->carregar($sql);
}


function alterarFeriado(){
	global $db;
	
	$sql = "UPDATE 
				demandas.feriado 
			SET 
				frdtitulo = '".$_POST['frdtitulo']."',
				frddata = '".formata_data_sql($_POST['frddata'])."'
			WHERE 
				frdid = '".$_POST['frdid']."';";

	$db->executar($sql);					
	$db->commit();
}


function deletarFeriado(){
	global $db;
	
	$sql = " UPDATE demandas.feriado SET frdstatus='I' WHERE frdid=".$_SESSION['frdid'];
	$db->executar($sql);
	$db->commit();
}

if($_POST['frdtitulo'] && $_POST['frddata'] && !$_POST['frdid']){
	inserirFeriado();
	unset($_SESSION['frdid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='demandas.php?modulo=sistema/apoio/feriado&acao=A'</script>";
	//header( "Location: demandas.php?modulo=sistema/apoio/feriado&acao=A" );
	exit();
	
}

if($_POST['frdtitulo'] && $_POST['frddata'] && $_POST['frdid']){
	alterarFeriado();
	unset($_SESSION['frdid']);
	unset($_SESSION['op3']);
	print "<script>alert('Operação realizada com sucesso!');</script>";
	print "<script>location.href='demandas.php?modulo=sistema/apoio/feriado&acao=A'</script>";
	//header( "Location: demandas.php?modulo=sistema/apoio/feriado&acao=A" );
	exit();
}

if($_GET['frdid'] && $_GET['op3']){
	session_start();
	$_SESSION['frdid'] = $_GET['frdid'];
	$_SESSION['op3'] = $_GET['op3'];
	header( "Location: demandas.php?modulo=sistema/apoio/feriado&acao=A" );
	exit();
}

if($_SESSION['frdid'] && $_SESSION['op3']){
	if($_SESSION['op3'] == 'delete'){
		deletarFeriado();	
		unset($_SESSION['frdid']);
		unset($_SESSION['op3']);
		print "<script>alert('Operação realizada com sucesso!');</script>";
		print "<script>location.href='demandas.php?modulo=sistema/apoio/feriado&acao=A'</script>";
	}
	if($_SESSION['op3'] == 'update'){
		$dados = selecionarFeriado();	
		unset($_SESSION['frdid']);
		unset($_SESSION['op3']);
	}

}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Feriados', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
<script src="../includes/calendario.js"></script>

<form id="formulario" name="formulario" action="" method="post" onsubmit="return validaForm();" >

<input type="hidden" name="frdid" value="<? echo $dados[0]['frdid']; ?>" />

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
	<td align='right' class="SubTituloDireita">Título do Feriado:</td>
	<td>
		<? $frdtitulo = $dados[0]['frdtitulo'];  ?>
		<?= campo_texto( 'frdtitulo', 'N', 'S', '', 94, 250, '', '','','','','',''); echo obrigatorio();?>
	</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita">Data do Feriado:</td>
	<td>
		<? $frddata = $dados[0]['frddata'];  ?>
		<?= campo_data( 'frddata', 'S', 'S', '', '' ); ?>
	</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td width="25%">&nbsp;</td>
		<td>
		<input type="submit" class="botao" name="btalterar" value="Salvar">
		<?php
		if (isset($dados)){
			echo '<input type="button" class="botao" name="del" value="Novo" onclick="javascript:location.href = window.location;">';	
		}
		?>
		</td>
	</tr>
	</table>
	<?php
	$sql = "SELECT
				'<a href=\"demandas.php?modulo=sistema/apoio/feriado&acao=A&frdid=' || o.frdid || '&op3=update\">
				   <img border=0 src=\"../imagens/alterar.gif\" />
				 </a> 
				 <a style=\"cursor:pointer\"  onclick=\"return exclusao(\'demandas.php?modulo=sistema/apoio/feriado&acao=A&frdid=' || o.frdid || '&op3=delete\');\" >
				   <img border=0 src=\"../imagens/excluir.gif\" />
				 </a>', 
				o.frdtitulo,
				to_char(o.frddata::timestamp,'DD/MM/YYYY'),		
				CASE
				 WHEN frdstatus = 'A' THEN 'ATIVO'
				 ELSE 'INATIVO'
				END AS status
			FROM 
				demandas.feriado o
			WHERE
				frdstatus = 'A'
		  	ORDER BY 
				 o.frddata";
	
	$cabecalho = array( "Opções","Título do Feriado","Data do Feriado","Situação");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
	?>
	</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function validaForm(){
	
	if(document.formulario.frdtitulo.value == ''){
		alert ('O campo Título do Feriado deve ser preenchido.');
		document.formulario.frdtitulo.focus();
		return false;
	}
	if(document.formulario.frddata.value == ''){
		alert ('O campo Data do Feriado deve ser preenchido.');
		document.formulario.frddata.focus();
		return false;
	}
}


function exclusao(url) {
	var questao = confirm("Deseja realmente excluir este registro?")
	if (questao){
		window.location = url;
	}
}
</script>
</body>