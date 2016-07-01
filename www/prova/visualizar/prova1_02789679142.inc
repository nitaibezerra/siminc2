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
<table style="background-color: #F5F5F5;" align="center" border="1" width="95%" class="tabela" cellpadding="3" cellspacing="2">
	<trhead bgcolor="gray">
		<tr>
			<th><b>Nome da Turma</b></th>
			<th>Dia de abertura da turma</th>
			<th>Dia do fechamento da turma</th>
			<th>Quantidade de Alunos</th>
			<th>Professor responsável</th>
		</tr>
	</tr>
	<tbody>
	<?php 
		$sql = 'SELECT * FROM prova.turma t, prova.professor p WHERE t.proid = p.proid';
		$arrDados = $db->carregar($sql);
		foreach($arrDados as $row){
			echo '<tr>';
			echo '<td>'. $row['turnome'].'</td>';
			echo '<td>'. $row['turdtabr'].'</td>';
			echo '<td>'. $row['turdtfec'].'</td>';
			echo '<td>'. $row['qtdalu'].'</td>';			
			echo '<td>'. $row['pronome'].'</td>';
			echo '</tr>';
		}
	
	?>
	</tbody>
	
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