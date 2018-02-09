<?php

if ($_POST['agrupador']){
	ini_set("memory_limit","256M");
	include("popUpRelatorioConvenio.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Convênio', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript"><!--
function gerarRelatorio(){
	var formulario = document.formulario;

	if (formulario.elements['agrupador'][0] == null){
		alert('Selecione pelo menos uma coluna!');
		return false;
	}	
	
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.n_pta );
	selectAllOptions( formulario.convenio );
	selectAllOptions( formulario.pago );
	selectAllOptions( formulario.empenho );
	selectAllOptions( formulario.siafi );
	selectAllOptions( formulario.fnde );
	selectAllOptions( formulario.processo );
	selectAllOptions( formulario.uniid );
				
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
//	ajaxRelatorio();
	formulario.submit();
	
	janela.focus();
}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

-->
</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">	
<input type="hidden" name="limpaSession" id="limpaSession" value="true">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Colunas</td>
		<td>
			<?
			$matriz = agrupador();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupador', null);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Pagamento efetivado:</td>
		<td>
			<input type="radio" value="S" id="pagamentoefetivado" name="pagamentoefetivado" <? if($_REQUEST["pagamentoefetivado"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="pagamentoefetivado" name="pagamentoefetivado" <? if($_REQUEST["pagamentoefetivado"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="pagamentoefetivado" name="pagamentoefetivado"  <? if($_REQUEST["pagamentoefetivado"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Foi Gerado Convênio:</td>
		<td>
			<input type="radio" value="S" id="geradoconvenio" name="geradoconvenio" <? if($_REQUEST["geradoconvenio"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="geradoconvenio" name="geradoconvenio" <? if($_REQUEST["geradoconvenio"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="geradoconvenio" name="geradoconvenio"  <? if($_REQUEST["geradoconvenio"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Foi Publicado:</td>
		<td>
			<input type="radio" value="S" id="foipublicado" name="foipublicado" <? if($_REQUEST["foipublicado"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="foipublicado" name="foipublicado" <? if($_REQUEST["foipublicado"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="foipublicado" name="foipublicado"  <? if($_REQUEST["foipublicado"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Gerado arquivo para conversão do SIAFI:</td>
		<td>
			<input type="radio" value="S" id="convertidosiaf" name="convertidosiaf" <? if($_REQUEST["convertidosiaf"] == "S") { echo "checked"; } ?> /> Sim
			<input type="radio" value="N" id="convertidosiaf" name="convertidosiaf" <? if($_REQUEST["convertidosiaf"] == "N") { echo "checked"; } ?> /> Não
			<input type="radio" value="" id="convertidosiaf" name="convertidosiaf"  <? if($_REQUEST["convertidosiaf"] == "") { echo "checked"; } ?> /> Todos
		</td>
	</tr>
		<?php
			
				// Filtros do relatório

//PTA				
		$stSqlPTA = "select distinct
						(pt.ptrid || '/' || pt.ptrexercicio) as codigo,
						(pt.ptrcod || '/' || pt.ptrexercicio) as descricao ";
		
		$stSql = "			
				from emenda.planotrabalho as pt
					inner join emenda.execucaofinanceira as ef on ef.ptrid=pt.ptrid 
					inner join emenda.ptminutaconvenio as pc on pc.ptrid=pt.ptrid
					left join emenda.ordembancaria as ob on ob.exfid=ef.exfid
					inner join emenda.analise as an on an.ptrid=pt.ptrid
					left join emenda.unidades on unidades.uniid=an.uniid
				where 
					pt.ptrstatus = 'A'
					and an.anastatus = 'A'
					and pc.pmcstatus = 'A'
					and ef.exfstatus = 'A'
					and pt.ptrexercicio = '".$_SESSION['exercicio']."'
				group by pt.ptrid, pt.ptrcod, pt.ptrexercicio, ef.exfnumempenhooriginal,pc.pmcnumconveniosiafi,
					 pt.ptrnumconvenio, pt.ptranoconvenio, pt.ptrnumprocessoempenho, unidades.uniid, unidades.unisigla		
				order by descricao";
				
			$stSqlCarregados = "";
			mostrarComboPopup( 'PTA', 'n_pta',  $stSqlPTA . $stSql, $stSqlCarregados, 'Selecione o(s) PTA(s)' ); 

// Convenio
				$stSqlConvenio = "select distinct
								sum(ef.exfvalor) as codigo,
								sum(ef.exfvalor) as descricao ";
			
			
				$stSqlCarregados = "";
				mostrarComboPopup( 'Valor do Convênio', 'convenio', $stSqlConvenio . $stSql, $stSqlCarregados, 'Selecione o(s) Valor (s) do Convênio', $where ); 


//Valor Pago				
				$stSqlValorPago = "select distinct
								(sum(ob.orbvalorpagamento)) as codigo,
								(sum(ob.orbvalorpagamento)) as descricao ";
			
			
				$stSqlCarregados = "";
				mostrarComboPopup( 'Valor Pago', 'pago',  $stSqlValorPago . $stSql, $stSqlCarregados, 'Selecione o(s) Valor (s) Pago (s)' ); 

//Nº da Nota de Empenho				
				$stSqlEmpenho = "select distinct
								ef.exfnumempenhooriginal as codigo,
								ef.exfnumempenhooriginal as descricao ";
							
			
				$stSqlCarregados = "";
				mostrarComboPopup( 'N° da Nota de Empenho', 'empenho',  $stSqlEmpenho . $stSql, $stSqlCarregados, 'Selecione o(s) Número (s) de Nota (s) de Empenho' ); 

//Nº do Convênio no Siafi			
				$stSqlConvSIAFI = "select distinct
									pc.pmcnumconveniosiafi as codigo,
									pc.pmcnumconveniosiafi as descricao ";
			
				$stSqlCarregados = "";
				mostrarComboPopup( 'N° do Convênio no SIAFI', 'siafi',  $stSqlConvSIAFI . $stSql, $stSqlCarregados, 'Selecione o(s) Número (s) de Convênio (s) no SIAF' ); 

//Nº do Convênio no FNDE			
				$stSqlConvFNDE = "select distinct
								(pt.ptrnumconvenio || '/' || pt.ptranoconvenio) as codigo,
								(pt.ptrnumconvenio || '/' || pt.ptranoconvenio) as descricao ";
			
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'N° do Convênio no FNDE', 'fnde',  $stSqlConvFNDE . $stSql, $stSqlCarregados, 'Selecione o(s) Número (s) de Convênio (s) no FNDE' ); 
				
//Nº do Processo			
				$stSqlProcesso = "select distinct
								pt.ptrnumprocessoempenho as codigo,
								pt.ptrnumprocessoempenho as descricao";
				
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'N° do Processo', 'processo',  $stSqlProcesso . $stSql, $stSqlCarregados, 'Selecione o(s) Número (s) de Processo (s)' ); 

//Unidade			
				$stSqlUnidade = "select distinct
								unidades.uniid as codigo,
								unidades.unisigla as descricao";
				
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'Unidade', 'uniid',  $stSqlUnidade . $stSql, $stSqlCarregados, 'Selecione o(s) Número (s) de Processo (s)' );				
				
?>
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio();"/>
		</td>
	</tr>
</table>
</form>
</body>
</html>
<?

function agrupador(){
	return array(
				array('codigo' 	  => 'n_pta', 'descricao' => 'PTA'),	
				array('codigo' 	  => 'valor_do_convenio', 'descricao' => 'Valor do Convênio'),
				array('codigo' 	  => 'valor_pago', 'descricao' => 'Valor Pago'),
				array('codigo' 	  => 'status_pagamento', 'descricao' => 'Status de Pagamento'),
				array('codigo' 	  => 'n_da_nota_de_empenho', 'descricao' => 'Nº da Nota de Empenho'),
				array('codigo' 	  => 'n_do_convenio_no_siafi', 'descricao' => 'Nº do Convênio no Siafi'),
				array('codigo' 	  => 'n_do_convenio_no_fnde', 'descricao' => 'Nº do Convênio no FNDE'),
				array('codigo' 	  => 'n_do_processo', 'descricao' => 'Nº do Processo'),
				array('codigo' 	  => 'siglas', 'descricao' => 'Unidades'),
				array('codigo' 	  => 'estuf', 'descricao' => 'UF'),
				array('codigo' 	  => 'enbnome', 'descricao' => 'Entidade'),
				array('codigo' 	  => 'prgdsc', 'descricao' => 'Programa'),
				array('codigo' 	  => 'ptivalortotal', 'descricao' => 'Valor Projeto'),
				array('codigo' 	  => 'ptrvalorconcedente', 'descricao' => 'Valor Concedente'),
				array('codigo' 	  => 'inivigencia', 'descricao' => 'Data de Celebração'),
				array('codigo' 	  => 'fimvigencia', 'descricao' => 'Final da vigência'),
				array('codigo' 	  => 'exfvalorempenho', 'descricao' => 'Valor Empenhado')
				);
}
?>