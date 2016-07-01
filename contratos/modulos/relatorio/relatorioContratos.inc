<?php

if ($_POST['agrupador']){
	ini_set("memory_limit","256M");
	include("popUpRelatorioContratos.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório Geral de Contratos', 'Selecione os filtros e as colunas desejadas' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">
function gerarRelatorio(consulta){
	var formulario = document.formulario;
	formulario.consulta.value = consulta;
	
	if (formulario.elements['agrupador'][0] == null){
		alert('Selecione pelo menos uma coluna!');
		return false;
	}	
	
	var data1 = document.getElementById("dtinicio").value;
	var data2 = document.getElementById("dtfim").value;

//valida data
	if(data1 != '' && data2 != ''){
		var nova_data1 = parseInt(data1.split("/")[2].toString() + data1.split("/")[1].toString() + data1.split("/")[0].toString());
		var nova_data2 = parseInt(data2.split("/")[2].toString() + data2.split("/")[1].toString() + data2.split("/")[0].toString());
	}
	if(nova_data2 < nova_data1){
		 alert('A data final não pode ser menor que a data inicial.');
	 return false;
	}	
	
	selectAllOptions(formulario.agrupador);
	selectAllOptions(formulario.contratada);
	selectAllOptions(formulario.cnpjContratada);
	selectAllOptions(formulario.fiscalTitular);
	selectAllOptions(formulario.cpfFiscalTitular);
	selectAllOptions(formulario.fiscalSubstituto);
	selectAllOptions(formulario.cpfFiscalSubstituto);
	selectAllOptions(formulario.responsavel );
	selectAllOptions(formulario.vencimento );
	selectAllOptions(formulario.dtInicioModalidadeGarantida);
	selectAllOptions(formulario.dtFimModalidadeGarantida);
	selectAllOptions(formulario.n_processoexefin);
	selectAllOptions(formulario.n_processoexectr);
	selectAllOptions(formulario.n_contrato);
	selectAllOptions(formulario.tipoContrato);
	selectAllOptions(formulario.anoContrato);
	selectAllOptions(formulario.modalidade);
	selectAllOptions(formulario.n_modalidade);
	selectAllOptions(formulario.vlr_inicial);
	selectAllOptions(formulario.vlr_mensal);
	selectAllOptions(formulario.vlr_total);
	selectAllOptions(formulario.vlr_global);
	selectAllOptions(formulario.modalidadeGarantida);
	selectAllOptions(formulario.objeto);
	selectAllOptions(formulario.situacao);
	selectAllOptions(formulario.obv_vigencia);
	selectAllOptions(formulario.num_cronograma);
	selectAllOptions(formulario.obv_contrato);
	selectAllOptions(formulario.vlr_modalidadeGarantida);
	selectAllOptions(formulario.obv_modalidadeGarantida);
	selectAllOptions(formulario.num_portaria_fiscal);
	selectAllOptions(formulario.dt_portaria_fiscal);
	selectAllOptions(formulario.processo_contrato);
	
	
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
//	ajaxRelatorio();
	formulario.submit();
	
	janela.focus();

	
		
	if(formulario.dtinicio.value != '' && formulario.dtfim.value != ''){
		
		if(!validaData(formulario.dtinicio)){
			alert("Data Início Inválida.");
			formulario.dtinicio.focus();
			return false;
		}		
		if(!validaData(formulario.dtfim)){
			alert("Data Fim Inválida.");
			formulario.dtfim.focus();
			return false;
		}		
	}
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
	if ( div_on.style.display == 'none' ){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function obras_exibeRelatorioGeralXLS(){
	
	var formulario = document.formulario;
	var agrupador  = document.getElementById( 'agrupador' );
		
	prepara_formulario();
	selectAllOptions(formulario.agrupador);
	
	if ( !agrupador.options.length ){
		alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
		return false;
	}
	
	selectAllOptions( agrupador );

	selectAllOptions(document.getElementById('contratada'));
	selectAllOptions(document.getElementById('cnpjContratada'));
	selectAllOptions(document.getElementById('fiscalTitular'));
	selectAllOptions(document.getElementById('cpfFiscalTitular'));
	selectAllOptions(document.getElementById('fiscalSubstituto'));
	selectAllOptions(document.getElementById('cpfFiscalSubstituto'));
	selectAllOptions(document.getElementById('responsavel'));
	selectAllOptions(document.getElementById('vencimento'));
	selectAllOptions(document.getElementById('dtInicioModalidadeGarantida'));
	selectAllOptions(document.getElementById('dtFimModalidadeGarantida'));
	selectAllOptions(document.getElementById('n_processoexefin'));
	selectAllOptions(document.getElementById('n_processoexectr'));
	selectAllOptions(document.getElementById('n_contrato'));
	selectAllOptions(document.getElementById('tipoContrato'));
	selectAllOptions(document.getElementById('anoContrato'));
	selectAllOptions(document.getElementById('modalidade'));
	selectAllOptions(document.getElementById('n_modalidade'));
	selectAllOptions(document.getElementById('vlr_inicial'));
	selectAllOptions(document.getElementById('vlr_mensal'));
	selectAllOptions(document.getElementById('vlr_total'));
	selectAllOptions(document.getElementById('vlr_global'));
	selectAllOptions(document.getElementById('modalidadeGarantida'));	
	selectAllOptions(document.getElementById('objeto'));
	selectAllOptions(document.getElementById('situacao'));
	selectAllOptions(document.getElementById('obv_vigencia'));
	selectAllOptions(document.getElementById('num_cronograma'));
	selectAllOptions(document.getElementById('obv_contrato'));
	selectAllOptions(document.getElementById('vlr_modalidadeGarantida'));
	selectAllOptions(document.getElementById('obv_modalidadeGarantida'));
	selectAllOptions(document.getElementById('num_portaria_fiscal'));
	selectAllOptions(document.getElementById('dt_portaria_fiscal'));
	selectAllOptions(document.getElementById('processo_contrato'));

	formulario.submit();
}
</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">	
<input type="hidden" name="limpaSession" id="limpaSession" value="true">
<input type="hidden" name="consulta" id="consulta" value="html">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita">Colunas
		</td>
		<td><?
			$matriz = agrupador();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupador', null);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
<?php
			
/////////////////////////////////////////////////////////////FILTROS DO RELATÓRIO////////////////////////////////////////////////////////////////////////
////////////////Tipo do Contrato				
				$stSql = " SELECT DISTINCT
								tipo_contrato.tpcdsc AS codigo, 
								tipo_contrato.tpcdsc AS descricao
						  FROM 
								evento.ctcontrato AS contrato
						  INNER JOIN
								evento.cttipocontrato AS tipo_contrato 
						  ON 
						  		contrato.tpcid = tipo_contrato.tpcid 
						  ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Tipo do Contrato', 'tipoContrato',  $stSql, $stSqlCarregados, 'Selecione o(s) Tipo(s) do(s) Contrato(s)' ); 
////////////////Ano				
				$stSql = "  SELECT DISTINCT  
									contrato.ctrano AS codigo,
									contrato.ctrano AS descricao
							FROM 
									evento.ctcontrato AS contrato
							ORDER BY
								descricao ASC
						   ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Ano', 'anoContrato',  $stSql, $stSqlCarregados, 'Selecione o(s) Ano(s)' ); 
////////////////Modalidade				
				$stSql = "SELECT 
						        modalidadeContrato.moddsc AS codigo,
						        modalidadeContrato.moddsc AS descricao 
						  FROM 
								evento.ctcontrato AS contrato
						  INNER JOIN
								evento.ctmodalidadecontrato AS modalidadeContrato 
						  ON 
						        contrato.modid = modalidadeContrato.modid
						  ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Modalidade', 'modalidade',  $stSql, $stSqlCarregados, 'Selecione a(s) Modalidades(s)' ); 
////////////////Situação				
				$stSql = " SELECT 
									situacao.sitdsc AS codigo ,
									situacao.sitdsc AS descricao
						   FROM 
									evento.ctcontrato AS contrato
						   INNER JOIN
									evento.ctsituacaocontrato AS situacao
						   ON
									contrato.sitid = situacao.sitid
						   ORDER BY
									contrato.sitid 
						   ASC ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Situação da Vigência', 'situacao',  $stSql, $stSqlCarregados, 'Selecione a(s) Situação(ões)' ); 				
//////////////// Área Responsável
				$stSql = " SELECT DISTINCT
									uniatendimento.unasigla AS codigo,
									uniatendimento.unasigla AS descricao
						  FROM 
									evento.ctcontrato AS evento
						  INNER JOIN
									demandas.unidadeatendimento AS uniatendimento 
						  ON 
									evento.unaid = uniatendimento.unaid
						  ORDER BY
									codigo 
						  ASC ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Área Responsável', 'responsavel',  $stSql, $stSqlCarregados, 'Selecione a(s) Área(s)' ); 
////////////////Contratada			
				$stSql = " SELECT DISTINCT
									entidade.entnome AS codigo, 
									entidade.entnome AS descricao									
						   FROM 
									evento.ctcontrato AS contrato
						   INNER JOIN	
						   			entidade.entidade AS entidade
			   			   ON
			   			   			contrato.entidcontratada = entidade.entid 
						   WHERE
									entidade.entnome IS NOT NULL
						   ORDER BY
						   			entidade.entnome ASC";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Contratada', 'contratada',  $stSql, $stSqlCarregados, 'Selecione a(s) Contratada(s)' ); 
////////////////Objeto			
				$stSql = " SELECT DISTINCT 
									ctrobj AS codigo,
									ctrobj AS descricao
						   FROM 
									evento.ctcontrato
						   WHERE 
									ctrobj IS NOT NULL";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Objeto ', 'objeto',  $stSql, $stSqlCarregados, 'Selecione o(s) Objeto (s)' ); 
///////////////Observação Vigência
				$stSql = "SELECT 
								observacaoVigencia.obvdsc AS codigo, 
								observacaoVigencia.obvdsc AS descricao
       					  FROM
       					  		evento.ctcontrato AS contrato
  						  INNER JOIN
								evento.ctobsvigencia AS observacaoVigencia 
						  ON
						  		contrato.obvid = observacaoVigencia.obvid";
				$stSqlCarregados = '';
				mostrarComboPopup('Observação Vigência','obv_vigencia',$stSql, $stSqlCarregados, 'Selecione a(s) Observação(ões)');	
?>
<!--Venmcimento-->
	<tr>
		<td class="SubTituloDireita">Vencimento:</td>
			<td>
				&nbsp;
				De:	<?= campo_data( 'dtinicio', 'N', 'S', '', 'S','Data Inicial','',''); ?>
				&nbsp;&nbsp;&nbsp;
				até
				&nbsp;&nbsp;&nbsp;
				<?= campo_data( 'dtfim', 'N', 'S', '', 'S','Data Final','','' ); ?>
			</td>
	</tr>
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio('html');" style="cursor: pointer;"/>
			<input type="button" name="Gerar Relatório XLS" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio('xls');" style="cursor: pointer;"/>
			<!--<input type="button" value="Visualizar XLS" onclick="obras_exibeRelatorioGeralXLS();" style="cursor: pointer;"/>-->
		</td>
	</tr>
</table>
</form>
</body>
</html>
<?
function agrupador(){
	return array(
				
				array('codigo'=>'contratada','descricao'=>'Contratada'),
				array('codigo'=>'cnpjContratada','descricao'=>'CNPJ Contratada'),
				array('codigo'=>'fiscalTitular','descricao'=>'Fiscal Titular'),
				array('codigo'=>'cpfFiscalTitular','descricao'=>'CPF Fiscal Titular'),
				array('codigo'=>'fiscalSubstituto','descricao'=>'Fiscal Substituto '),
				array('codigo'=>'cpfFiscalSubstituto','descricao'=>'CPF Fiscal Substituto '),
				array('codigo'=>'responsavel','descricao'=>'Área Responsável'),
				array('codigo'=>'vencimento','descricao'=>'Vencimento'),
				array('codigo'=> 'dtInicioModalidadeGarantida','descricao'=>'Data Inicio Modalidade Garantida'),
				array('codigo'=>'dtFimModalidadeGarantida','descricao'=>'Data Fim Modalidade Garantida '),	
				array('codigo'=>'n_processoexefin','descricao'=>'Processo Execução Financeira '),
				array('codigo'=>'n_processoexectr','descricao'=>'Processo Execução do Contrato '),	
				array('codigo'=>'n_contrato','descricao'=>'Número '),	
				array('codigo'=>'tipoContrato','descricao'=>'Tipo do Contrato'),
				array('codigo'=>'anoContrato','descricao'=>'Ano'),	
				array('codigo'=>'modalidade','descricao'=>'Modalidade'),
				array('codigo'=>'n_modalidade','descricao'=>'Número Modalidade'),	
				array('codigo'=>'vlr_inicial','descricao' => 'Valor Inicial'),
				array('codigo'=>'vlr_mensal','descricao'=>'Valor Atual Mensal'),	
				array('codigo'=>'vlr_total','descricao'=>'Valor Atual Anual'),
				array('codigo'=>'vlr_global','descricao'=>'Valor Atual Global'),
				array('codigo'=>'modalidadeGarantida','descricao'=>'Modalidade Garantida'),
				array('codigo'=>'objeto','descricao'=>'Objeto'),
				array('codigo'=>'situacao','descricao'=>'Situação da Vigência'),
				array('codigo'=>'obv_vigencia','descricao'=>'Observação da Vigência'),
				array('codigo'=>'num_cronograma','descricao'=>'Número do Cronograma'),
				array('codigo'=>'obv_contrato','descricao'=>'Observação do Contrato'),
				array('codigo'=>'vlr_modalidadeGarantida','descricao'=>'Valor Modalidade Garantida'),
				array('codigo'=>'obv_modalidadeGarantida','descricao'=>'Observação Modalidade Garantida'),
				array('codigo'=>'num_portaria_fiscal','descricao'=>'Número da Portaria do Fiscal'),
				array('codigo'=>'dt_portaria_fiscal','descricao'=>'Data da Portaria do Fiscal'),
				array('codigo'=>'processo_contrato','descricao'=>'Processo do Contrato')
				
	);
}
?>