<?php

if ($_REQUEST['exibirRelatorio']=='SIM'){
	ini_set("memory_limit","256M");
	include("resultComponentesGrafico.inc");
	exit; 
}

include APPRAIZ . 'includes/cabecalho.inc';
include "resultComponentesGraficoBack.php";

print '<br/>';

monta_titulo( 'Relatório Componentes PSE - Gráfico', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">

function gerarRelatorio(){
	inicializarCamposFormulario();
	var janela = window.open( '', 'relatorio', 'width='+screen.width+',height='+screen.height+',status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	document.formulario.target = 'relatorio';	
	document.formulario.submit();
	janela.focus();
}

function inicializarCamposFormulario() {
	selectAllOptions( document.formulario.estado );
	selectAllOptions( document.formulario.municipio );
	selectAllOptions( document.formulario.comp1 );
	selectAllOptions( document.formulario.comp2 );
	selectAllOptions( document.formulario.comp3 );
	selectAllOptions( document.formulario.comp4 );	
}


var div_on;
var div_off;
var input;

/**
 * Uso chamado pela função 'mostrarComboPopup'. 
 */
function onOffCampo(campo) {
	recuperarCamposEspecificos(campo); 
	settarCamposDivComValorOpostoAoAtual();
}

function recuperarCamposEspecificos(campo) {
	div_on  = document.getElementById( campo + '_campo_on' );
	div_off = document.getElementById( campo + '_campo_off' );
	input   = document.getElementById( campo + '_campo_flag' );	
}

function settarCamposDivComValorOpostoAoAtual() {
	var divOnEstaDesativo = div_on.style.display == 'none'; 
	if ( divOnEstaDesativo ) {
		div_on.style.display  = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else {
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
<input type="hidden" name="exibirRelatorio" value="SIM" />	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Agrupadores</td>
		<td>
			Brasil
			<div style="padding-left:0px;">
				<img src="../imagens/seta_filho.gif" align="absmiddle"/> Estado 
			</div>
			<div style="padding-left:20px;">
				<img src="../imagens/seta_filho.gif" align="absmiddle"/> Município 
			</div>
			<div style="padding-left:40px;">
				<img src="../imagens/seta_filho.gif" align="absmiddle"/> Componente/Subcomponente 
			</div>
			<div style="padding-left:60px;">
				<img src="../imagens/seta_filho.gif" align="absmiddle"/> Ação
			</div>
			<div style="padding-left:80px;">
				<img src="../imagens/seta_filho.gif" align="absmiddle"/> Quantidade de ações
			</div>															
		</td>
	</tr>
		<?php
				//----- Estados -----------------------------------------------------------------------------------------------------------
				mostrarComboPopup( 'Estado', 'estado', obterSqlComboBoxEstados(), "", 'Selecione o(s) Estado(s)' ); 
				
				//----- Municípios --------------------------------------------------------------------------------------------------------
				$elementosArrayWhereEstadoUfParaMunicipios = array(	array(	"codigo" 	=> "estuf",
																			"descricao" => "Estado <b style='size: 8px;'>(sigla)</b>",
																			"numeric"	=> false
																	   	  ) );
				mostrarComboPopup( 'Município', 'municipio' ,  obterSqlComboBoxMunicipios()      , "", 'Selecione o(s) Município(s)', $elementosArrayWhereEstadoUfParaMunicipios );    
			
				//----- Componente I ------------------------------------------------------------------------------------------------------
				mostrarComboPopup( 'Componente I'  , 'comp1',  obterSqlComboBoxComponenteUm()    , "", 'Selecione a avaliação da condição de saúde dos estudantes referente ao Componente I' ); 
				
				//----- Componente II -----------------------------------------------------------------------------------------------------
				mostrarComboPopup( 'Componente II' , 'comp2',  obterSqlComboBoxComponenteDois()  , "", 'Selecione a promoção da saúde e prevenção referente ao Componente II' ); 
				
				//----- Componente III ----------------------------------------------------------------------------------------------------
				mostrarComboPopup( 'Componente III', 'comp3',  obterSqlComboBoxComponenteTres()  , "", 'Selecione a educação permanente e capacitação dos profissionais da educação e da saúde de jovens para o pse referente ao Componente III' ); 
			
				//----- Componente IV -----------------------------------------------------------------------------------------------------
				mostrarComboPopup( 'Componente IV' , 'comp4',  obterSqlComboBoxComponenteQuatro(), "", 'Selecione o monitoramento e avaliação da saúde dos estudantes referente ao Componente IV' );
		?>
	<tr>
		<td class="SubTituloDireita" align="right">
			Cada gráfico será representado por</td>
		<td>
		<?php		
				$sqlRepresentacaoMaiorCadaGrafico = 
					"
						select 1 as CODIGO, 'Brasil' as DESCRICAO 
						union all
						select 2 as CODIGO, 'Estado' as DESCRICAO 
						union all
						select 3 as CODIGO, 'Município' as DESCRICAO 
						union all
						select 4 as CODIGO, 'Componente/Subcomponente' as DESCRICAO
						union all
						select 5, 'Ação' as DESCRICAO 					
					";
				$db->monta_combo( "nivelRepresentacaoMaior", $sqlRepresentacaoMaiorCadaGrafico, 'S', '', '', '', '', '', '210' );
		?>
		</td>
	</tr>		
	<tr>
		<td class="SubTituloDireita" align="right" width="30%">
			Cada porção de cada gráfico conterá somatórios de valores por</td>
		<td>
		<?php		
				$sqlPorcaoSomatorioCadaGrafico = 
					"
						select 2 as CODIGO, 'Estado' as DESCRICAO 
						union all
						select 3 as CODIGO, 'Município' as DESCRICAO 
						union all
						select 4 as CODIGO, 'Componente/Subcomponente' as DESCRICAO
						union all
						select 5, 'Ação' as DESCRICAO
						union all
						select 6 as CODIGO, 'Quantidade de ações' as DESCRICAO 
					";
				$db->monta_combo( "nivelPorcaoSomatorio", $sqlPorcaoSomatorioCadaGrafico, 'S', '', 'filtro', '', '', '210' );
		?>
		</td>
	</tr>			
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio();"/>
		</td>
	</tr>
</table>
</form>
</body>
</html>
