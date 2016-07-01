<?php

if ($_POST['agrupador']){
	ini_set("memory_limit","256M");
	include_once("resultEscolaPseSecretaria.inc");
	exit;
}

include_once APPRAIZ. '/includes/Agrupador.php';
include_once APPRAIZ . '/includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório Escola PSE', '&nbsp;' );
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
		alert('Selecione pelo menos um agrupador!');
		return false;
	}	
	
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.anoportaria );
	selectAllOptions( formulario.estado );
	selectAllOptions( formulario.municipio );
	selectAllOptions( formulario.modalidade );
	selectAllOptions( formulario.nivel );
	selectAllOptions( formulario.ano );
	selectAllOptions( formulario.esfera );
	selectAllOptions( formulario.exer );
			
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
	formulario.submit();
	
	janela.focus();
}

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
--></script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Agrupadores</td>
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
		<?php
				//---------------------- esfera ---------------------------------------------------
				$stSql = "SELECT DISTINCT
								empflagestmun as codigo,
								(SELECT CASE WHEN empflagestmun = 'e' THEN 'Estadual'
								ELSE
								'Municipal'
								END) as descricao
						FROM
								pse.estadomunicipiopse";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Esfera', 'esfera',  $stSql, $stSqlCarregados, 'Selecione a Esfera' );
				//----------------------exercicio--------------------------------------------------
				$stSql = "SELECT DISTINCT
								prsano as codigo,
								prsano as descricao
						FROM
								pse.programacaoexercicio
						ORDER BY
								descricao";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Exercicio', 'exer',  $stSql, $stSqlCarregados, 'Selecione o ano de exercicio' );
				// --------------------- anoportaria ----------------------------------------------
				$stSql = "SELECT DISTINCT
							      porano as codigo,
							      (SELECT CASE porano
								       WHEN 2008 THEN 'Portaria 2.931 de 4 de dezembro de 2008'
								       WHEN 2009 THEN 'Portaria 1.537 de 15 de junho de 2010'
							       ELSE 'Portaria 1.910 de 9 de agosto de 2011'
							      END) as descricao
						 FROM
							      pse.portariapse";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Portaria', 'anoportaria',  $stSql, $stSqlCarregados, 'Selecione o Ano' );		
		
		
				// --------------------- estado ----------------------------------------------
				$stSql = "SELECT
							estuf AS codigo,
							estdescricao AS descricao
						  FROM
						  	territorios.estado";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Estado', 'estado',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)' ); 
				

				// --------------------- município ----------------------------------------------
				$stSql = "SELECT
							muncod AS codigo,
							estuf || ' - ' || replace(mundescricao, '\'', '') AS descricao
						  FROM
						  	territorios.municipio	
						  ORDER BY
						  	descricao";
				$where = array(
								array(
										"codigo" 	=> "estuf",
										"descricao" => "Estado <b style='size: 8px;'>(sigla)</b>",
										"numeric"	=> false
							   		  )
							   );
				$stSqlCarregados = "";				
				mostrarComboPopup( 'Município', 'municipio',  $stSql, $stSqlCarregados, 'Selecione o(s) Município(s)', $where ); 
				
				
				// --------------------- modalidade ----------------------------------------------
				$stSql = "SELECT
								mod.moeid AS codigo,
								mod.moedsc AS descricao
							FROM
								pse.modalidadeensino mod 
							ORDER BY 
								mod.moedsc";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Modalidade de Ensino', 'modalidade',  $stSql, $stSqlCarregados, 'Selecione a(s) Modalidade(s) de Ensino' );
				 
				
				// --------------------- nivel ----------------------------------------------
				$stSql = "SELECT
								niv.nieid AS codigo,
								niv.niedsc AS descricao
							FROM
								pse.nivelensino niv";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Nível de Ensino', 'nivel',  $stSql, $stSqlCarregados, 'Selecione o(s) Nível(is) de Ensino' ); 
				
				
				// --------------------- ano ----------------------------------------------
				$stSql = "SELECT
								distinct ano.paeanoreferencia as codigo,
								ano.paeanoreferencia as descricao
						FROM
							pse.pseanoescola ano";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Ano', 'ano',  $stSql, $stSqlCarregados, 'Selecione o(s) Ano(s)' ); 

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
				array('codigo' 	  => 'ano'		,
					  'descricao' => '1 - Ano' ),
				array('codigo' 	  => 'estado'	  ,
					  'descricao' => '2 - Estado'),	
				array('codigo' 	  => 'municipio'	 ,
					  'descricao' => '3 - Município'),
				array('codigo' 	  => 'escola'	  ,
					  'descricao' => '4 - Escola'),
				array('codigo' 	  => 'modalidade'  ,
					  'descricao' => '5 - Modalidade') ,
				array('codigo' 	  => 'nivel'		   ,
					  'descricao' => '6 - Nível de Ensino')
				);
}
?>