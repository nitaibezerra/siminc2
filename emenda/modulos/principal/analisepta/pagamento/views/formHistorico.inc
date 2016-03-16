<html>
<head>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
</head>
<body>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
 	<tr>
 		<td colspan="4" class="subtitulocentro">Dados do Pagamento</td>
 	</tr>
	<tr>
		<td style="text-align: right; width:7.5%;" class="SubTituloEsquerda">Nº Empenho (NE):</td>
		<td style="width:30%; background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita"><?php echo $arDados['exfnumempenhooriginal']; ?></td>
	</tr>
	<tr>
 		<td colspan="6" class="subtitulocentro">Dados da Ordem Bancária</td>
 	</tr>
 	<tr>
 		<td colspan="2">
 			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="150" name="tabela_execfinanceira" id="tabela_execfinanceira">
				<tr>
					<td class="subtitulocentro">Ação</td>
					<td class="subtitulocentro">Mês Parcela</td>
					<td class="subtitulocentro">Ano Parcela</td>
					<td class="subtitulocentro">Valor Parcela (R$)</td>
					<td class="subtitulocentro">Nº Solicitação</td>
					<td class="subtitulocentro">Situação</td>
					<td class="subtitulocentro">Nº Ordem Bancária</td>
					<td class="subtitulocentro">Valor Pagamento (R$)</td>
					<td class="subtitulocentro">Data Emissao</td>
					<td class="subtitulocentro">Data Pagamento</td>
					<td class="subtitulocentro">Usuário</td>
					<td class="subtitulocentro">Data Inclusão</td>
					<td class="subtitulocentro">Data Alteração</td>
				</tr>
				<?php				
					$cor = ($i % 2) ? "#f4f4f4" : "#e0e0e0";
					echo  "<tr bgColor='{$cor}'>";
						//if($obExecucaoFinanceira->temFilho($dados["exfid"])){
					echo  "	   <td align='center' id='exec_acao{$arDados["orbid"]}'>"
						. "        <img src='/imagens/mais.gif' style='cursor:pointer;' onclick='abreHistoricoOrbHist({$arDados["orbid"]});'/>"
						. "	   </td>";
						//} else {
				//	echo  "	   <td align='center'> - </td>";
						//}
					echo  "	   <td >"
						.          $arDados["orbmesparcela"]
						. "	   </td>"
						. "	   <td >"
						.          $arDados["orbanoparcela"]
						. "	   </td>"
						. "	   <td >"
						.          number_format($arDados["orbvalorparcela"], 2, ",", "." )
						. "	   </td>"
						. "	   <td >"
						.          $arDados["orbnumsolicitacao"]
						. "	   </td>"
						. "	   <td >"
						.          $arDados["spgdescricao"]
						. "	   </td>"
						. "	   <td >"
						.          $arDados["orbnumordembancaria"]
						. "	   </td>"
						. "	   <td >"
						.          number_format($arDados["orbvalorpagamento"], 2, ",", "." )
						. "	   </td>"
						. "	   <td >"
						.          $arDados["orbdataemissao"]
						. "	   </td>"
						. "	   <td align='center'>"
						.          $arDados["orbdatapagamento"]
						. "	   </td>"
						. "	   <td align='center'>"
						.          $arDados["usucpf"]
						. "	   </td>"
						. "	   <td >"
						.          $arDados["orbdatainclusao"]
						. "	   </td>"
						. "	   <td >"
						.          $arDados["orbdataalteracao"]
						. "	   </td>"
						. "</tr>"
						. "<tr id='exec_{$arDados["orbid"]}' style='display:none;'>"
						. "    <td align='center' valign='center'><img src='/imagens/seta_filho.gif'/></td>"
						. "    <td colspan='15' id='exec_historico_{$arDados["orbid"]}'></td>"
						. "</tr>";
				
				?>
			</table> 		
 		</td>
 	</tr>
</table>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	function abreHistoricoOrbHist( id ){
		var acao  = 'exec_acao';
		var td    = 'exec_';
		var historico = 'exec_historico_';
		var carregando = "<img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\">";
		
		var url = "/emenda/emenda.php?modulo=principal/analisepta/pagamento/historicoSolicitacao&acao=A";
		var pars = 'execFinan=1&orbid='+id;
		
		var cor = "#fafafa";
		var myAjax = new Ajax.Request(
				url, 
				{
					method: 'post', 
					parameters: pars, 
					asynchronous: false,
					onLoading: $(historico + id).update(carregando+' Aguarde enquanto sua solicitação é finalizada. '),
					onComplete: function(r)
					{
						$(historico + id).update(r.responseText)
						//$('debug').update(r.responseText);
						//return false;
					}
				});
	
		$(td + id).style.display = 'table-row';
		
		document.getElementById( acao + id ).innerHTML = "<img src='/imagens/menos.gif' style='cursor:pointer;' onclick='fechaHistoricoExec(" + id + ");'/>";
	}

	function fechaHistoricoExec( id ){
		var acao  = 'exec_acao';
		var td    = 'exec_';
		var historico = 'exec_historico_';
		
		document.getElementById( td + id ).style.display = 'none';
		document.getElementById( historico + id ).innerHTML = '';
		document.getElementById( acao + id ).innerHTML = "<img src='/imagens/mais.gif' style='cursor:pointer;' onclick='abreHistoricoOrbHist(" + id + ");'/>";
		
	}
</script>
</body>
</html>