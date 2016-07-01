<?
verificarCoordenadorIESTermoCompromisso(array("uncid"=>$_SESSION['sisindigena2']['universidade']['uncid']));

$estado = wf_pegarEstadoAtual( $_SESSION['sisindigena2']['universidade']['docid'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_IES) {
	$consulta = true;
}


?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<form method="post" id="formulario">
<input type="hidden" name="goto" id="goto" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Orçamento</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Itens Financiáveis</td>
		<td>
		
		<? 
		
		$sql = "SELECT uu.unisigla||' - '||uu.uninome as universidade, g.gdedesc, 'Verba', o.orcvlrunitario, replace(o.orcdescricao,'\n','<br>')
			FROM sisindigena2.orcamento o
			INNER JOIN sisindigena2.grupodespesa g ON g.gdeid = o.gdeid 
			INNER JOIN sisindigena2.nucleouniversidade u ON u.picid = o.picid 
			INNER JOIN sisindigena2.universidade uu ON uu.uniid = u.uniid 
			WHERE u.uncid='".$_SESSION['sisindigena2']['universidade']['uncid']."' AND o.orcstatus='A'
			ORDER BY g.gdedesc";
		
		$cabecalho = array("Núcleo","Grupo de Despesa","Unidade de Medida","Valor total (R$)","Detalhamento");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'S','100%','S');
		
		
		?>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sisindigena2.php?modulo=principal/universidade/universidade&acao=A&aba=dados';">
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sisindigena2.php?modulo=principal/universidade/universidade&acao=A&aba=nucleos';">
		</td>
	</tr>
</table>
</form>