<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( "Prova SIMEC", 'Questão 02 - Crie uma tela de cadastro de novas turmas' );

$mensagem = 
$turnome  = 
$turmid   = 
$turdtabr = 
$turdtfec = null;

if(isset($_GET['action']) && $_GET['action'] == 'edit') {
	$turmid = $_GET['turmid'];
	
	$query = "
		SELECT *
		FROM prova.turma
		WHERE turid = '$turmid'
	";
	$select = $db->carregar($query);
	
	$turnome  = $select[0]['turnome'];
	$turdtabr = date_format(new DateTime($select[0]['turdtabr']), 'd/m/Y');
	$turdtfec = date_format(new DateTime($select[0]['turdtfec']), 'd/m/Y');
}

if(isset($_GET['action']) && $_GET['action'] == 'delete') {
	$turmid = $_GET['turmid'];
	
	$query = "
		DELETE FROM prova.turma
		WHERE turid = '$turmid'
	";
	
	$db->executar($query);
	$db->commit();
}

if(isset($_POST) && $_POST) {
	try {
		$turnome  = $_POST['turnome'];
		$turdtabr = $_POST['turdtabr'];
		$turdtfec = $_POST['turdtfec'];
		
		if(
			!$turnome
			&& !$turdtabr
			&& !$turdtfec
		) {
			throw new Exception('Todos os campos deverão ser preenchidos corretamente!');
		}
		
		$turdtabrinsert = date_format(new DateTime(str_replace('/', '-', $turdtabr)), 'Y-m-d');
		$turdtfecinsert = date_format(new DateTime(str_replace('/', '-', $turdtfec)), 'Y-m-d');
		
		$query = "
			INSERT INTO prova.turma(turnome, turdtabr, turdtfec)
			VALUES('$turnome', '$turdtabrinsert', '$turdtfecinsert')
		";
		
		if($turmid) {
			$query = "
				UPDATE prova.turma SET
					turnome  = '$turnome', 
					turdtabr = '$turdtabrinsert', 
					turdtfec = '$turdtfecinsert'
				WHERE turid = '$turmid'
			";
		}
		
		$db->executar($query);
		$db->commit();
		
		$mensagem = '<div style="background:green;color:white;">Operação realizada com sucesso</div>';
		$turnome  = 
		$turdtabr = 
		$turdtfec = null;
	} catch(Exception $e) {
		$mensagem = '<div style="background:red;color:white;">' . $e->getMessage() . '</div>';
	}
}

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

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}
</script>

<form name="formulario" id="formulario" method="post" action="" >
	<?php 
		echo $mensagem;
	?>

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col width="25%" />
			<col width="75%" />
		</colgroup>
		<tr>
			<td class="SubTituloDireita">Nome da Turma:</td>
			<td><input type="text" name="turnome" id="turdsc" value="<?php echo $turnome; ?>" size="50"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de abertura da Turma:</td>
			<td><input type="text" name="turdtabr" id="turabr" value="<?php echo $turdtabr; ?>" size="50"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de fechamento da Turma:</td>
			<td><input type="text" name="turdtfec" id="turfec" value="<?php echo $turdtfec; ?>" size="50"></td>
		</tr>
		<tr bgcolor="#cccccc">
			<td class="SubTituloDireita">&nbsp;</td>
			<td><input value="Salvar" type="submit" id="salvar"></td>
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
			'<img title=\"Alterar\" src=\"../imagens/alterar.gif\" class=\"alterar\" data-id=\"'||turid||'\" />&nbsp;
			<img title=\"Excluir\" src=\"../imagens/excluir.gif\" class=\"excluir\" data-id=\"'||turid||'\" />' as acao, 
			turnome, 
			TO_CHAR(turdtabr,'dd/mm/YYYY') as tur_dt_abr, 
			TO_CHAR(turdtfec,'dd/mm/YYYY') as tur_dt_fec 
		FROM 
			prova.turma
		ORDER BY
			turnome";

$cabecalho = array("Ações","Nome da Turma","Data de abertura da Turma","Data de fechamento da Turma");
$db->monta_lista($sql,$cabecalho,50,5,'N','100%');
?>

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
<script type="text/javascript">
	$('.alterar').click(function() {
		window.location = 'http://simec-local/prova/prova.php?modulo=principal/prova2&acao=A&action=edit&turmid=' + $(this).attr('data-id');
	});
	
	$('.excluir').click(function() {
		var btconfirm = confirm('Tem certeza que deseja excluir a turma?');
		
		if(btconfirm === true) {
			window.location = 'http://simec-local/prova/prova.php?modulo=principal/prova2&acao=A&action=delete&turmid=' + $(this).attr('data-id');
		}
	});
</script>