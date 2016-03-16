<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( "Prova SIMEC", 'Questão 02 - Crie uma tela de cadastro de novas turmas' );

/*
 * DICAS:
 * 
 * Com a função "executar" você executa uma query, por exemplo para inseri-la no banco. Ex:
 * 
 * 		$sql = "INSERT INTO data (dia, mes, ano) VALUES ('24','03','1984')";
 * 		$db->executar($sql); 
 * 		$db->commit(); 
 * 
 */


if( isset($_POST['turnome']) )
{
	if( empty($_POST['turnome']) || empty($_POST['turdtabr']) || empty($_POST['turdtfec']) ){
		header('Location: prova.php?modulo=principal/prova2&acao=A&msg=Todos os campos devem ser preenchidos.');
	}else{
		if( isset($_POST['tipo']) && $_POST['tipo'] == 'edita' ){ // update
			$sql = " UPDATE prova.turma SET turnome = '{$_POST['turnome']}' , turdtabr = '{$_POST['turdtabr']}' , turdtfec = '{$_POST['turdtfec']}' WHERE turid = '{$_POST['turid']}' ";
			$db->executar($sql); 
			if( $db->commit() ){
				header('Location: prova.php?modulo=principal/prova2&acao=A&msg=Operação realizada com sucesso!');
			}else{
				header('Location: prova.php?modulo=principal/prova2&acao=A&msg=Erro ao realizar operação.');
			}
		}else{ // insert
			$sql = " INSERT INTO prova.turma(turnome , turdtabr , turdtfec) VALUES('{$_POST['turnome']}','{$_POST['turdtabr']}','{$_POST['turdtfec']}')";
			$db->executar($sql); 
			if( $db->commit() ){
				header('Location: prova.php?modulo=principal/prova2&acao=A&msg=Operação realizada com sucesso!');
			}else{
				header('Location: prova.php?modulo=principal/prova2&acao=A&msg=Erro ao realizar operação.');
			}
		}
	}
}

// apagar
if(isset($_GET['apaga'])){
	$sql = " DELETE FROM prova.turma WHERE turid = '{$_GET['apaga']}' ";
	$db->executar($sql); 
	if( $db->commit() ){
		header('Location: prova.php?modulo=principal/prova2&acao=A&msg=Operação realizada com sucesso!');
	}else{
		header('Location: prova.php?modulo=principal/prova2&acao=A&msg=Erro ao realizar operação.');
	}
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}
	
	function apagaEsseElemento(elemento){
		alert($(elemento).parent().parent().html());
	}
</script>
<style>
	.linha:hover{
		background-color:#ccc;
	}
</style>

<?php
if(isset($_GET['edita']))
{
	$sql = "SELECT 
			turid, 
			turnome, 
			turdtabr, 
			turdtfec 
		FROM 
			prova.turma
		WHERE turid = '{$_GET['edita']}'";
	$busca = $db->carregar($sql);
}
?>

<form name="formulario" id="formulario" method="post" action="" >
	<?php if(isset($_GET['edita'])){?><input type="hidden" name="tipo" value="edita"/><input type="hidden" name="turid" value="<?php echo $_GET['edita']; ?>"/><?php } ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col width="25%" />
			<col width="75%" />
		</colgroup>
		<tr>
			<td></td>
			<td><?php if(isset($_GET['msg'])){echo $_GET['msg'];} ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome da Turma:</td>
			<td><input type="text" name="turnome" id="turdsc" size="50" value="<?php if(isset($_GET['edita'])){echo $busca[0]['turnome'];} ?>"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de abertura da Turma:</td>
			<td><input type="text" name="turdtabr" id="turabr" size="50" value="<?php if(isset($_GET['edita'])){echo $busca[0]['turdtabr'];} ?>"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de fechamento da Turma:</td>
			<td><input type="text" name="turdtfec" id="turfec" size="50" value="<?php if(isset($_GET['edita'])){echo $busca[0]['turdtfec'];} ?>"></td>
		</tr>
		<tr>
			<td></td>
			<td>
				<input type="submit" value="Enviar"/>
			</td>
		</tr>
	</table>
</form>
<table align ="center">
<tr>
		<td align="center" colspan="2"><strong>Lista de Turmas</strong></td>
</tr>
</table>
<?
$sql = "SELECT 
			'<img title=\"Alterar\" src=\"../imagens/alterar.gif\" />&nbsp;<img id=\"excluir\" title=\"Excluir\" src=\"../imagens/excluir.gif\" />' as acao, 
			turnome, 
			TO_CHAR(turdtabr,'dd/mm/YYYY') as tur_dt_abr, 
			TO_CHAR(turdtfec,'dd/mm/YYYY') as tur_dt_fec 
		FROM 
			prova.turma
		ORDER BY
			turnome";
//$cabecalho = array("Ações","Nome da Turma","Data de abertura da Turma","Data de fechamento da Turma");
//$db->monta_lista($sql,$cabecalho,50,5,'N','100%');

?><table style="width:96%;margin:0 auto;"><thead>
	<th style="width:20%">Ações</th>
	<th style="width:30%">Nome da Turma</th>
	<th style="width:25%">Data de abertura da Turma</th>
	<th style="width:25%">Data de fechamento da Turma</th>
</thead><?php
$sql = "SELECT 
			turid, 
			turnome, 
			TO_CHAR(turdtabr,'dd/mm/YYYY') as tur_dt_abr, 
			TO_CHAR(turdtfec,'dd/mm/YYYY') as tur_dt_fec 
		FROM 
			prova.turma
		ORDER BY
			turnome";
$dados = $db->carregar($sql);
//echo "<pre>";print_r($dados);echo "</pre>";exit;
foreach( $dados as $chave => $linha){
	?><tr class="linha">
		<td>
			<a href="prova.php?modulo=principal/prova2&acao=A&edita=<?php echo $linha['turid']; ?>"><img title="Alterar" src="../imagens/alterar.gif" /></a>&nbsp;
			<a onclick="javascript:if(confirm('Tem certeza que deseja excluir a turma?')){window.location.href = 'prova.php?modulo=principal/prova2&acao=A&apaga=<?php echo $linha['turid']; ?>';}"><img id="excluir" title="Excluir" src="../imagens/excluir.gif" /></a>
		</td>
		<td><?php echo $linha['turnome']; ?></td>
		<td><?php echo $linha['tur_dt_abr']; ?></td>
		<td><?php echo $linha['tur_dt_fec']; ?></td>
	</tr><?php
}
?></table>

<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" width="100%" class="tabela">
	<tbody>
		<tr bgcolor="#cccccc">
			<td>
				<div style="float:left; width:30%; text-align: left;">
					<input value="Questão anterior" type="button" onclick="navega(1)" id="anterior">
				</div>
				<div style="float:left; width:30%; text-align: center">
					<input value="Salvar" type="button" id="salvar">
				</div>
				<div style="float:right; width:30%; text-align: right;">
					<input disabled="disabled" value="Próxima Questão" type="button" id="proximo">
				</div>
			</td>
		</tr>
	</tbody>
</table>
