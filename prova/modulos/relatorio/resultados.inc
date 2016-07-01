<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( "Prova SIMEC", 'Resultados' );

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
	function visualizar(cpf, questao){
		location.href = "/prova/prova.php?modulo=relatorio/visualizar&acao=A&cpf="+cpf+"&questao="+questao;
	}
</script>
<?
$sql = "SELECT 
			'<img title=\"Visualizar prova 01\" src=\"../imagens/alterar.gif\" onclick=\"visualizar(\'' || cpf || '\', 1)\" />' as acao1, 
			'<img title=\"Visualizar prova 02\" src=\"../imagens/alterar.gif\" onclick=\"visualizar(\'' || cpf || '\', 2)\" />' as acao2, 
			cpf, 
			nome 
		FROM 
			prova.identificacao
		ORDER BY
			nome";

$cabecalho = array("Prova 01","Prova 02","CPF","Nome");
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