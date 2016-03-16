<?php

if( $_POST['requisicao'] == 'salvar' ){
	$sql = "UPDATE emenda.planotrabalho SET
				ptridentificacaofederal = '{$_POST['ptridentificacaofederal']}',
  				ptrobjetivofederal = '{$_POST['ptrobjetivofederal']}',
  				ptrjustificativafederal = '{$_POST['ptrjustificativafederal']}' 
			WHERE 
			  	ptrid = {$_SESSION["emenda"]["ptrid"]}";
	
	$db->executar($sql);
	$db->commit();
	$db->sucesso('principal/dadosOrcamentarios');
	exit();
}

$obPTA = new PTA();

$obPTA->validaSessionPTA( $_SESSION["emenda"]["ptrid"] );

$ptrid = $_SESSION["emenda"]["ptrid"];

include  APPRAIZ."includes/cabecalho.inc";

montaAbasPTA( $abacod_tela, $url, '', '', '' );
monta_titulo( 'Dados Gerais do PTA', '');
echo $obPTA->cabecalhoPlanoTrabalho($ptrid);

$sql = "SELECT ptrpropostasiconvano, ptridentificacaofederal, ptrobjetivofederal, ptrjustificativafederal FROM emenda.planotrabalho WHERE ptrid = $ptrid";
$arDados = $db->pegaLinha($sql);
extract($arDados);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" name="requisicao" id="requisicao" value=""> 
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita" style="text-align: center;" colspan="2"><b>PLANO DE TRABALHO SIMPLIFICADO (para indicação de viabilidade de execução no Mod de Emendas do SIMEC)</b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Identificação (Título / Objeto da despesa):</td>
			<td><?=campo_texto( 'ptridentificacaofederal', 'S', 'S', 'Identificação (Título / Objeto da despesa)', 80, 70, '', '','','','','id="ptridentificacaofederal"', '', '', ""); ?>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Objetivo:</td>
			<td><?=campo_textarea('ptrobjetivofederal', 'S', 'S', 'Objetivo', 98, 5, 500, '', '', '', '', 'Objetivo');?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Justificativa (Motivação / Clientela / Cronograma físico):</td>
			<td><?=campo_textarea('ptrjustificativafederal', 'S', 'S', 'Justificativa (Motivação / Clientela / Cronograma físico)', 98, 5, 350, '', '', '', '', 'Justificativa (Motivação / Clientela / Cronograma físico)');?></td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="4" align="center">
				<input type="button" value="Salvar" id="btnSalvar" name="btnSalvar" onclick="salvarDadosGerais();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
<script>
	function salvarDadosGerais(){
		if( $('[name="ptridentificacaofederal"]').val() == '' ){
			alert('O campo "Identificação (Título / Objeto da despesa)" é de preenchimento obrigatório.');
			$('[name="ptridentificacaofederal"]').focus();
			return false;
		}
		if( $('[name="ptrobjetivofederal"]').val() == '' ){
			alert('O campo "Objetivo" é de preenchimento obrigatório.');
			$('[name="ptrobjetivofederal"]').focus();
			return false;
		}
		if( $('[name="ptrjustificativafederal"]').val() == '' ){
			alert('O campo "Justificativa (Motivação / Clientela / Cronograma físico)" é de preenchimento obrigatório.');
			$('[name="ptrjustificativafederal"]').focus();
			return false;
		}
		$('[name="requisicao"]').val('salvar');
		$('[name="formulario"]').submit();
	}
</script>