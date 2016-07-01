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
 * '<img title=\"Alterar\" src=\"../imagens/alterar.gif\" />&nbsp;<img title=\"Excluir\" src=\"../imagens/excluir.gif\" />'
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
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col width="25%" />
			<col width="75%" />
		</colgroup>
		<tr>
			<td class="SubTituloDireita">Nome da Turma:</td>
			<td><input type="text" name="turnome" id="turnome" size="50"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de abertura da Turma:</td>
			<td><input type="text" name="turdtabr" id="turdtabr" size="50"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de fechamento da Turma:</td>
			<td><input type="text" name="turdtfec" id="turdtfec" size="50"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"></td>
			<td><input value="Salvar" type="submit" id="salvar" name="salvar"></td>
				
		</tr>
	</table>
	<?php 
		
	if(isset($_POST['salvar'])){
		
		$nomeTurma = $_POST['turnome'];
		$dtAbertura = $_POST['turdtabr'];
		$dtFechamento = $_POST['turdtfec'];
		
		$array1 = explode('/', $dtAbertura);
		$dtAbertura1 = $array1[2]."-".$array1[1]."-".$array1[0];
		
		$array2 = explode('/', $dtFechamento);
		$dtFechamento1 = $array2[2]."-".$array2[1]."-".$array2[0];
			
		$sql = "INSERT INTO prova.turma (turnome, turdtabr, turdtfec) VALUES ('".$nomeTurma."','".$dtAbertura1."','".$dtFechamento1."')";
 		$db->executar($sql); 
 		$db->commit(); 
 	
		
		}	
	
	if(isset($_POST['alterar'])){
		$sql2 = 'SELECT * FROM prova.turma t WHERE t.Turid =' . $_POST['id'];
		$arrDados = $db->carregar($sql2);
		
		$array = explode('-', $arrDados['turdtabr']);
		$dtAbertura = $array[2].'/'.$array[1].'/'.$array[0];
				
		$array2 = explode('-', $arrDados['turdtfec']);
		$dtFechamento = $array2[2].'/'.$array2[1].'/'.$array2[0];		
		
		$_POST['turnome'] = $arrDados['turnome'];
		$_POST['turdtabr'] = $dtAbertura;
		$_POST['turdtfec'] = $dtFechamento;				
	}
	
	if(isset($_POST['excluir'])){
		$sql2 = 'SELECT * FROM prova.turma t, prova.professor p WHERE t.proid = p.proid';
		$arrDados = $db->carregar($sql2);
 	}
	
	
	?>
</form>
<table align ="center">
<tr>
		<td align="center" colspan="2"><strong>Lista de Turmas</strong></td>
</tr>
</table>
<?
$sql = "SELECT 
			'<a href=\"prova.php?modulo=principal/prova2&acao=A&id=74\"><img title=\"Alterar\" src=\"../imagens/alterar.gif\" /></a>&nbsp;<img title=\"Excluir\" src=\"../imagens/excluir.gif\" />' as acao, 
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
				
				<div style="float:right; width:30%; text-align: right;">
					<input disabled="disabled" value="Próxima Questão" type="button" id="proximo">
				</div>
			</td>
		</tr>
	</tbody>
</table>