<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( "Prova SIMEC", 'Questão 01 - Alimente a tabela com as informações solicitadas' );

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
$sql = " 
	SELECT * 
	FROM prova.turma
	LEFT JOIN prova.professor ON prova.professor.proid = prova.turma.proid  
";
$dados = $db->carregar($sql);
//echo "<pre>";print_r($dados);echo "</pre>";exit;

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}
</script>
<table style="background-color: #F5F5F5;" align="center" border="1" width="95%" class="tabela" cellpadding="3" cellspacing="2">
	<tr bgcolor="gray">
		<td>
			<b>Nome da Turma</b>
		</td>
		<td>
			<b>Dia de abertura da turma</b>
		</td>
		<td>
			<b>Dia do fechamento da turma</b>
		</td>
		<td>
			<b>Quantidade de Alunos</b>
		</td>
		<td>
			<b>Professor responsável</b>
		</td>
	</tr>
	<?php foreach( $dados as $chave => $linha ){ ?>
	<tr>
		<td>
			<?php echo $linha['turnome'] ?>
		</td>
		<td>
			<?php echo $linha['turdtabr'] ?>
		</td>
		<td>
			<?php echo $linha['turdtfec'] ?>
		</td>
		<td>
			<?php echo $linha['qtdalu'] ?>
		</td>
		<td>
			<?php echo $linha['pronome'] ?>
		</td>
	</tr>
	<?php } ?>
</table>


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