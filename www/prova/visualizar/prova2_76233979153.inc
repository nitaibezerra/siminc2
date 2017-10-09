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

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}
	
	/*$(document).ready(function(){
		$("#formulario").validate();
		
		$("#turnome").change(function(){
		
			alert($(this).val());
		
		});
		
	});*/
	
	
	function executar(){
		document.getElementById('valor').value = 'Salvar';
		document.getElementById('formulario').submit();
	}
	
</script>

<?
if( $_POST['valor'] == 'Salvar' ){

	$dtAber = substr($_POST['turdtabr'],6,4).'-'.substr($_POST['turdtabr'],3,2).'-'.substr($_POST['turdtabr'],0,2).' '.date('His');
	$dtFech = substr($_POST['turdtfec'],6,4).'-'.substr($_POST['turdtfec'],3,2).'-'.substr($_POST['turdtfec'],0,2).' '.date('His');

	if($_POST['Turid'] == ''){
					
		$sql = "INSERT INTO prova.turma(turnome,turdtabr,turdtfec,qtdalu,proid) VALUES ('".$_POST['turnome']."','".$dtAber."','".$dtFech."','".$_POST['qtdalu']."','".$_POST['proid']."')";
		$db->executar($sql);
		$db->commit();
		echo '<script>alert("Operação realizada com sucesso."); window.location.reload();</script>';
		exit;
	}
}
?>

<form name="formulario" id="formulario" method="post" action="" >

	<input type="hidden" id="Turid" name="Turid">
	<input type="hidden" id="valor" name="valor">
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col width="25%" />
			<col width="75%" />
		</colgroup>
		<tr>
			<td class="SubTituloDireita">Nome da Turma:</td>
			<td><input type="text" name="turnome" id="turdsc" size="50" required"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de abertura da Turma:</td>
			<td><input type="text" name="turdtabr" id="turabr" size="50"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de fechamento da Turma:</td>
			<td><input type="text" name="turdtfec" id="turfec" size="50"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Quantidade de Alunos:</td>
			<td><input type="text" name="qtdalu" id="qtdalu" size="10" ></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Responsável:</td>
			<td>
				<select name="proid" id="proid" >
					<option value='1'>teste</option>
					
				</select>
			</td>
		</tr>
		
		<tr>
			<td></td>
			<td><input value="Cadastrar" type="button" id="Cadastrar" onClick="executar();"></td>
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
			 
			turnome, 
			turdtabr as tur_dt_abr, 
			TO_CHAR(turdtfec,'dd/mm/YYYY') as tur_dt_fec, 
			qtdalu,pronome,
			'<img title=\"Alterar\" src=\"../imagens/alterar.gif\" />&nbsp;<img title=\"Excluir\" src=\"../imagens/excluir.gif\" />' as acao
		FROM 
			prova.turma, prova.professor
			
		WHERE prova.turma.proid = prova.professor.proid
					
		ORDER BY
			turnome";

$cabecalho = array("Nome da Turma","Data de abertura da Turma","Data de fechamento da Turma","Quantidade de Alunos","Professor Responsavel","Ação");
$db->monta_lista($sql,$cabecalho,50,5,'N','100%');


?>


<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" width="100%" class="tabela">
	<tbody>
		<tr bgcolor="#cccccc">
			<td>
				<div style="float:left; width:30%; text-align: left;">
					<input value="Questão anterior" type="button" onclick="navega(1)" id="anterior">
				</div>
				<div style="float:right; width:30%; text-align: right;">
					<input disabled="disabled" value="Próxima Questão" type="button" id="proximo">
				</div>
			</td>
		</tr>
	</tbody>
</table>