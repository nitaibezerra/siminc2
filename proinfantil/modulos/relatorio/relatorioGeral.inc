<?php
function agrupador(){ 
		return array(
		array('codigo' 		=> 'estado',
			  'descricao' 	=> 'Estado'),				
		array('codigo' 		=> 'municipio',
			  'descricao' 	=> 'Município'),
		array('codigo' 		=> 'obra',
			  'descricao' 	=> 'Nome da Obra'),
		array('codigo' 		=> 'tipologia_obra',
			  'descricao' 	=> 'Tipologia da Obra')
	);
}

function agrupadord(){

}

function colunas(){
	return array(
		array('codigo' 		=> 'estado', 
			  'descricao' 	=> '02. Estado'),				
		array('codigo' 		=> 'municipio',
			  'descricao' 	=> '03. Município'),
		array('codigo'    	=> 'cnpj_prefeitura',
			  'descricao' 	=> '04. CNPJ Prefeitura'),				
 		array('codigo'    	=> 'codigo_municipio',
			  'descricao' 	=> '05. Código do Município'),
 		array('codigo'    	=> 'prefeito',
			  'descricao' 	=> '06. Nome Prefeito'),			
 		array('codigo'    	=> 'email_prefeito',
			  'descricao' 	=> '07. E-mail Prefeito'),				
 		array('codigo'    	=> 'telefone_prefeito',
			  'descricao' 	=> '08. Telefone Prefeito'),	
		array('codigo' 		=> 'mncintcre',
			  'descricao' 	=> '09. Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Integral / Creche'),
		array('codigo' 		=> 'mncintpre',
			  'descricao' 	=> '10. Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Integral / Pré-Escola'),
		array('codigo' 		=> 'mncparcre',
			  'descricao' 	=> '11. Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Parcial / Creche'),
		array('codigo' 		=> 'mncparpre',
			  'descricao' 	=> '12. Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Parcial / Pré-Escola'),
 		array('codigo' 		=> 'totalintcre',
			  'descricao' 	=> '13. Total de matrículas na escola – Tempo Integral / Creche'),
		array('codigo' 		=> 'totalintpre',
			  'descricao' 	=> '14. Total de matrículas na escola – Tempo Integral / Pré-Escola'),
		array('codigo' 		=> 'totalparcre',
			  'descricao' 	=> '15. Total de matrículas na escola – Tempo Parcial / Creche'),
		array('codigo' 		=> 'totalparpre',
			  'descricao' 	=> '16. Total de matrículas na escola – Tempo Parcial / Pré-Escola'),			
 		array('codigo' 		=> 'valorintcre',
			  'descricao' 	=> '17. Valor de repasse – Tempo Integral / Creche - Análise 1'),
 		array('codigo' 		=> 'valorintcre2',
			  'descricao' 	=> '18. Valor de repasse – Tempo Integral / Creche - Análise 2'),
		array('codigo' 		=> 'valorintpre',
			  'descricao' 	=> '19. Valor de repasse  – Tempo Integral / Pré-Escola - Análise 1'),
		array('codigo' 		=> 'valorintpre2',
			  'descricao' 	=> '20. Valor de repasse  – Tempo Integral / Pré-Escola - Análise 2'),
		array('codigo' 		=> 'valorparcre',
			  'descricao' 	=> '21. Valor de repasse  – Tempo Parcial / Creche - Análise 1'),
		array('codigo' 		=> 'valorparcre2',
			  'descricao' 	=> '22. Valor de repasse  – Tempo Parcial / Creche - Análise 2'),
		array('codigo' 		=> 'valorparpre',
			  'descricao' 	=> '23. Valor de repasse – Tempo Parcial / Pré-Escola - Análise 1'),				
		array('codigo' 		=> 'valorparpre2',
			  'descricao' 	=> '24. Valor de repasse – Tempo Parcial / Pré-Escola - Análise 2'),				
		array('codigo' 		=> 'obra',
			  'descricao' 	=> '25. Nome da Obra'),		
		array('codigo' 		=> 'data_inicio_atendimento',
			  'descricao' 	=> '26. Data de início do atendimento'),						
		array('codigo' 		=> 'tipologia_obra',
			  'descricao' 	=> '27. Tipologia'),	
		array('codigo' 		=> 'qtd_obra',
			  'descricao' 	=> '28. Qtd. de Obras'),
		array('codigo' 		=> 'percentual_obra',
			  'descricao' 	=> '29. Percentual Executado (%)'),
		array('codigo' 		=> 'estado_obra',
			  'descricao' 	=> '30. Situação da Obra'),
		array('codigo' 		=> 'estado_plano',
			  'descricao' 	=> '31. Situação do Plano'),
		
		array('codigo' 		=> 'valor_analise1',
			  'descricao' 	=> '32. Valor da Análise 1'),
		array('codigo' 		=> 'valor_analise2',
			  'descricao' 	=> '33. Valor da Análise 2'),
		
		array('codigo' 		=> 'total_repassado',
			  'descricao' 	=> '34. Valor Repassado'),
		array('codigo' 		=> 'total_repasse',
			  'descricao' 	=> '35. Valor Total de Repasse')
	);
}

function colunasd(){
	return array(
		array('codigo' => 'id_obra', 	'descricao' => '01. ID da Obra'),
		array('codigo' => 'qtd_obra', 	'descricao' => '31. Qtd. de Obras')
	);
}

if ($_POST['agrupador']){
	ini_set("memory_limit","2048M");
	$true = true;
	include("relatorioGeralResultado.inc");
	exit;
}

	
include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório Geral', '&nbsp;' );
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

	if (formulario.elements['agrupador'][0] == null){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}

	if (formulario.elements['coluna'][0] == null){
		alert('Selecione pelo menos uma coluna!');
		return false;
	}
	
	selectAllOptions(formulario.agrupador);
	selectAllOptions(formulario.coluna);
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
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr> 
			<td class="SubTituloDireita" valign="top">Agrupadores:</td>
			<td>
				<?php
				$matriz = agrupador();
				$matrizd = agrupadord();
				$campoAgrupador = new Agrupador('formulario');
				$campoAgrupador->setOrigem('agrupadorOrigem', null, $matriz );
				$campoAgrupador->setDestino('agrupador', null, $matrizd);
				$campoAgrupador->exibir();
				?>
			</td>
		</tr>
		<tr> 
			<td class="SubTituloDireita" valign="top">Colunas:</td>
			<td>
				<?php
				$colunas = colunas();
				$colunasd = colunasd();
				$campoAgrupador2 = new Agrupador('formulario');
				$campoAgrupador2->setOrigem('colunaOrigem', null, $colunas );
				$campoAgrupador2->setDestino('coluna', null, $colunasd);
				$campoAgrupador2->exibir();
				?>
			</td>
		</tr>	
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
		<td class="SubTituloDireita" valign="top">Tipologia:</td>
		<td>
			<?php
				$sql_combo = "SELECT 	DISTINCT tpoid as codigo, 
										tpodsc as descricao 
							  FROM		obras.tipologiaobra 
							  WHERE 	tpoid IN (".OBRA_TIPO_A.",".OBRA_TIPO_B.",".OBRA_TIPO_C.",".MI_OBRA_TIPO_B.",".MI_OBRA_TIPO_C.") 
							  ORDER BY  tpodsc";
				combo_popup('tpoid', $sql_combo, 'Selecione a(s) Tipologia(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Situação do Plano:</td>
		<td>
			<?php
				$sql_combo = "SELECT 	esdid as codigo, 
										esddsc as descricao 
							  FROM 		workflow.estadodocumento 
							  WHERE 	tpdid = ".WF_PROINFANTIL." AND esdstatus = 'A' 
							  ORDER BY 	esdordem";
				combo_popup('esdid', $sql_combo, 'Selecione a(s) Situação(ões) do(s) Plano(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Situação da Obra:</td>
		<td>
			<?php
				$sql_combo = "SELECT	esdid as codigo,
								esddsc as descricao
							FROM 
								workflow.estadodocumento 
							WHERE	
								esdstatus = 'A'
							AND
								tpdid = 105";
				combo_popup('stoid', $sql_combo, 'Selecione a(s) Tipologia(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true); ?>
		</td>
	</tr>
	<tr>
		<td class="subtituloDireita">Lote</td>
		<td>
			<?php $sql = "SELECT  	lotid as codigo,
									lotdsc as descricao
						  FROM		proinfantil.loteproinfancia
						  ORDER BY	lotdsc";
			$db->monta_combo('lotid', $sql, 'S', 'Selecione...', '', '', '');  ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Ano de Inicio do atendimento:</td>
		<td>
			<?php
			$arrayAnoIni = array(
				array( 'codigo' => '', 'descricao' => 'Selecione' ),
				array( 'codigo' => '2011', 'descricao' => '2011' ),
				array( 'codigo' => '2012', 'descricao' => '2012' ),
				array( 'codigo' => '2013', 'descricao' => '2013' )
			);
	
			$anoini = $_POST['anoini'];
			$db->monta_combo('anoini', $arrayAnoIni, 'S', '', '', '', 'Selecione o Ano!', '244','','',$anoini); ?>			
		</td>
	</tr>		
	<tr>
		<td class="SubTituloDireita" valign="top">Ano do Pagamento:</td>
		<td>
			<?php
			$arrayAnoPagamento = array(
				array( 'codigo' => '', 'descricao' => 'Selecione' ),
				array( 'codigo' => '2011', 'descricao' => '2011' ),
				array( 'codigo' => '2012', 'descricao' => '2012' ),
				array( 'codigo' => '2013', 'descricao' => '2013' )
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
