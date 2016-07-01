<?php

if ($_POST['listagem']){
	ini_set("memory_limit","2048M");
	$true = true;
	include("relatorioproinfanciaResultado.inc");
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
// Monta o título da página
monta_titulo( 'Relatório Município Proinfância', '&nbsp;' );
?>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready(function(){
	$('#colunaOrigem').css('width','510px');
	$('#coluna').css('width','510px');
});

function gerarRelatorio(req){
	var formulario = document.formulario;

	selectAllOptions(formulario.estuf);
	selectAllOptions(formulario.muncod);
	selectAllOptions(formulario.tpoid);
	selectAllOptions(formulario.esdid);
	selectAllOptions(formulario.stoid);
			
	document.getElementById('req').value = req;
			
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
	formulario.submit();
	janela.focus();
}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */

function onOffCampo( campo ){
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' ) {
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	} else {
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}
</script>

<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="req" id="req" value="" />	
        <input type="hidden" name="listagem" id="listagem" value="listagem" />	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		
		<?php 
		$arrVisivel = array("descricao");
		$arrOrdem = array("descricao");
				
		$stSql = "SELECT 	estuf as codigo, 
							estdescricao as descricao 
				  FROM		territorios.estado												
				  ORDER BY	estuf";

		$stSqlCarregados = "";
		mostrarComboPopup('UF:', 'estuf',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);
		mostrarComboPopupMunicipios('muncod');	?>
<!-- 	<tr>  -->
<!-- 		<td class="SubTituloDireita" valign="top">Município com<br>Usuário Responsável:</td> -->
<!-- 		<td> -->
			<?php	$sql = "(SELECT	'S' as codigo, 'SIM' as descricao)
// 								UNION ALL
// 							(SELECT	'N' as codigo, 'NÃO' as descricao)";	
// 			$db->monta_radio('muncodresp', $sql, '',1); ?>
<!-- 		</td> -->
<!-- 	</tr> -->
	<tr>
		<td class="SubTituloDireita" valign="top">Situação:</td>
		<td>
			<?php
			$arraySituacao = array(
				array( 'codigo' => '372', 'descricao' => 'Aguardando Pagamento' ),
				array( 'codigo' => '373', 'descricao' => 'Pagamento Efetuado' )
			);

			$db->monta_combo('situacao', $arraySituacao, 'S', '', '', '', 'Selecione a Situação!', '244','','',$situacao); ?>			
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" valign="top">Ano do Pagamento:</td>
		<td>
			<?php
			$arrayAnoPagamento = array(
                            	array( 'codigo' => '', 'descricao' => 'Todos os anos' ),
				array( 'codigo' => '2011', 'descricao' => '2011' ),
				array( 'codigo' => '2012', 'descricao' => '2012' ),
				array( 'codigo' => '2013', 'descricao' => '2013' ),
                                array( 'codigo' => '2014', 'descricao' => '2014' ),
				array( 'codigo' => '2015', 'descricao' => '2015' )
			);
	
			$anopagamento = $_POST['anopagamento'];
			$db->monta_combo('anopagamento', $arrayAnoPagamento, 'S', '', '', '', 'Selecione o Ano!', '244','','',$anopagamento); ?>			
		</td>
	</tr>		
	<!-- <tr> 
		<td class="SubTituloDireita" valign="top">Expandir Relatório</td>
		<td>
			<?php
			$expandir = 0;
			$sql = "(SELECT 1 as codigo, 'Expandir' as descricao)
					UNION ALL
					(SELECT	0 as codigo, 'Não Expandir' as descricao)";	
			//$db->monta_radio('expandir', $sql, '',1); ?>
		</td>
	</tr> -->
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio(1);"/>
			<input type="button" name="Gerar Relatório XLS" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio(2);"/>
		</td>
	</tr>
</table>
</form>
