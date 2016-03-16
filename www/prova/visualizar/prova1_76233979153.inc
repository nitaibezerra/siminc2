<?php 
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( "Prova SIMEC", 'Questão 01 - Liste as informações solicitadas' );

/*
 * DICAS:
 * 
 * Com a função "carregar" tem-se o resultado da query em um array. Ex:
 * 
 * 		$sql = "SELECT dia, mes, ano FROM data WHERE condicao = condicao";
 * 		$arrDados = $db->carregar($sql); 
 * 
 * RESULTADO DO $arrDados:
 * 
 * 		[0] => Array
	        (
	            [dia] => 24
	            [mes] => 03
	            [ano] => 1984
	        )
	
	    [1] => Array
	        (
	            [dia] => 22
	            [mes] => 10
	            [ano] => 1983
	        )
	
	    [2] => Array
	        (
	            [dia] => 27
	            [mes] => 12
	            [ano] => 2012
	        )
 * 
 */

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}
</script>

<?
$sql = "SELECT 
			 
			turnome, 
			TO_CHAR(turdtabr,'dd/mm/YYYY') as tur_dt_abr, 
			TO_CHAR(turdtfec,'dd/mm/YYYY') as tur_dt_fec, 
			qtdalu,pronome
		FROM 
			prova.turma, prova.professor
			
		WHERE prova.turma.proid = prova.professor.proid
					
		ORDER BY
			turnome";

$cabecalho = array("Nome da Turma","Data de abertura da Turma","Data de fechamento da Turma","Quantidade de Alunos","Professor Responsavel");
$db->monta_lista($sql,$cabecalho,50,5,'N','100%');
?>


<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" width="95%" class="tabela">
	<tbody>
		<tr bgcolor="#cccccc">
			<td>
				<div style="float:left; width:30%; text-align: left;">
					<input disabled="disabled" value="Questão anterior" type="button" id="anterior">
				</div>
				<div style="float:right; width:30%; text-align: right;">
					<input value="Próxima Questão" onclick="navega(2)" type="button" id="proximo">
				</div>
			</td>
		</tr>
	</tbody>
</table>