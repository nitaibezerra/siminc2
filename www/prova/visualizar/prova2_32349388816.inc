<?php 
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( "Prova ". SIGLA_SISTEMA, 'Questão 02 - Crie uma tela de cadastro de novas turmas' );

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
 
 if(!empty($_GET['Turid'])) {
	if(!empty($_GET['excluir'])) {
		$delete = "DELETE FROM prova.turma WHERE Turid = " . $_GET['Turid'];
		$db->executar($delete);
		if($db->commit()) {
			echo '<script>alert("Registro deletado com sucesso.")</script>';
		}	
	}      	
	$sql = "SELECT * FROM prova.turma WHERE Turid = " . $_GET['Turid'];	 		
	$resultArr = $db->carregar($sql);
 }
 
 if(!empty($_POST)) {
 	extract($_POST); 	
 	
	if(empty($_POST['Turid'])) {
		$insert = "INSERT INTO prova.turma(turnome, turdtabr, turdtfec) VALUES('" . $turnome . "', '" . $turdtabr . "', '" . $turdtfec . "')";
		$db->executar($insert);
		if($db->commit()) {
			echo '<script>alert("Registro inserido com sucesso.")</script>';
		}
	}
	else {		
		$sql = "UPDATE prova.turma
				SET turnome = '" . $turnome . "',
				turdtabr = '" . $turdtabr . "',
				turdtfec = '" . $turdtfec . "'
				WHERE Turid = " . $Turid . "
		";		
		$db->executar($sql); 
		if($db->commit()) {
			echo '<script>alert("Registro atualizado com sucesso.")</script>';
		}
	}
 }

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	$(document).ready(function() {		
		$('#salvar').click(function() {				
			if($('#turnome').val() == '') {
				alert('Favor informar o nome da turma.');
				return false;
			}
			
			if($('#turdtabr').val() === '') {
				alert('Favor informar a data de abertura da turma.');
				return false;
			}
			
			if($('#turdtfec').val() === '') {
				alert('Favor informar a data de fechamento da turma.');
				return false;
			}
			
			$('#formulario').submit();
		});				
		
	});
	
	function goToAlterar(Turid) {		
		window.location.href = "http://simec-local/prova/prova.php?modulo=principal/prova2&acao=A&Turid=" + Turid;
	}
	
	function goToExcluir(Turid) {		
		var confirm_e=confirm("Tem certeza que deseja escluir este registro?	")
		if (confirm_e == true) {
			window.location.href = "http://simec-local/prova/prova.php?modulo=principal/prova2&acao=A&Turid=" + Turid + "&excluir=1";
		}
		else {
			return false;
		}
	}	
	
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}
</script>

<form name="formulario" id="formulario" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col width="25%" />
			<col width="75%" />
		</colgroup>
		<tr>
			<td class="SubTituloDireita">Nome da Turma:</td>
			<td><input type="text" name="turnome" id="turnome" value="<?php echo (!empty($resultArr[0]['turnome']) ? $resultArr[0]['turnome'] : ''); ?>" size="50"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de abertura da Turma:</td>
			<td><input type="text" name="turdtabr" id="turdtabr" size="50" value="<?php echo (!empty($resultArr[0]['turdtabr']) ? $resultArr[0]['turdtabr'] : ''); ?>" size="50"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de fechamento da Turma:</td>
			<td><input type="text" name="turdtfec" id="turdtfec" size="50" value="<?php echo (!empty($resultArr[0]['turdtfec']) ? $resultArr[0]['turdtfec'] : ''); ?>" size="50"></td>
		</tr>
	</table>
	<input type="hidden" name="Turid" id="Turid" value="<?php echo (!empty($_GET['Turid']) ? $_GET['Turid'] : '' ); ?>">
</form>
<table align ="center">
<tr>
		<td align="center" colspan="2"><strong>Lista de Turmas</strong></td>
</tr>
</table>
<?
$sql = "SELECT 
			'<a href=\"#\" onclick=\"goToAlterar(' || Turid || ');\"><img title=\"Alterar\" id=\"alterarLink\"' || ' src=\"../imagens/alterar.gif\" />&nbsp;
			<a href=\"#\" onclick=\"goToExcluir(' || Turid || ');\"><img id=\"excluirLink\" title=\"Excluir\" src=\"../imagens/excluir.gif\" /></a>' as acao, 
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