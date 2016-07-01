<?php

if ($_POST['agrupador']){
	ini_set("memory_limit","256M");
	include("resultGestao.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Modelo de Gestão de Comunicação', '&nbsp;' );
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
	selectAllOptions( formulario.estado );
	selectAllOptions( formulario.municipio );
	selectAllOptions( formulario.frequencia );
	selectAllOptions( formulario.dificuldade );
	selectAllOptions( formulario.acomp );
	selectAllOptions( formulario.acomp2 );
	selectAllOptions( formulario.portaria );
	selectAllOptions( formulario.esfera );
	selectAllOptions( formulario.exer );
			
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

</script>
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
			
				// Filtros do relatório
				
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
		
				$stSql = "SELECT DISTINCT
								prsano as codigo,
								prsano as descricao
						FROM
								pse.programacaoexercicio
						ORDER BY
								descricao";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Exercicio', 'exer',  $stSql, $stSqlCarregados, 'Selecione o ano de exercicio' );
				
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
				mostrarComboPopup( 'Portaria', 'portaria',  $stSql, $stSqlCarregados, 'Selecione o Ano' );
		
				$stSql = "SELECT
							estuf AS codigo,
							estdescricao AS descricao
						  FROM
						  	territorios.estado";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Estado', 'estado',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)' ); 
				
				$stSql = "SELECT
							muncod AS codigo,
							estuf || ' - ' || replace(mundescricao, '\'', '') AS descricao
						  FROM
						  	territorios.municipio	
						  WHERE
						  	1=1
						  ORDER BY
						  	descricao";
				$stSqlCarregados = "";
				$where = array(
								array(
										"codigo" 	=> "estuf",
										"descricao" => "Estado <b style='size: 8px;'>(sigla)</b>",
										"numeric"	=> false
							   		  )
							   );
				mostrarComboPopup( 'Município', 'municipio',  $stSql, $stSqlCarregados, 'Selecione o(s) Município(s)', $where ); 
				
				$stSql = "SELECT
											iteid as codigo, 
											itedescricao as descricao
									FROM 
											pse.itempergunta
									WHERE
											perid=10";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Frequência de comunicação do representante com a escola', 'frequencia',  $stSql, $stSqlCarregados, 'Selecione com que frequência o representante da Secretaria de Educação se comunica com o responsável pelo PSE nas escolas que integram o programa' );
			
				
				$stSql = "SELECT
											iteid as codigo, 
											itedescricao as descricao
									FROM 
											pse.itempergunta
									WHERE
											perid=11";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Dificuldade do representante se comunicar com o(s) interlocutor(es) do programa', 'dificuldade',  $stSql, $stSqlCarregados, 'Selecione as dificuldades que o representante da Secretaria de Educação tem em se comunicar com o(s) interlocutor(es) do programa na escola' );
				
				
				$stSql = "SELECT
											iteid as codigo, 
											itedescricao as descricao
									FROM 
											pse.itempergunta
									WHERE
											perid=12";
				$stSqlCarregados = "";
				mostrarComboPopup( 'O representante da Secretaria de Educação acompanha a realização das atividades do componente I?', 'acomp',  $stSql, $stSqlCarregados, 'Selecione de que forma o representante da Secretaria de Educação acompanha a realização das atividades do componente I nas escolas que integram o programa' );
				
				
				$stSql = "SELECT
											iteid as codigo, 
											itedescricao as descricao
									FROM 
											pse.itempergunta
									WHERE
											perid=13";
				$stSqlCarregados = "";
				mostrarComboPopup( 'O representante da Secretaria de Educação acompanha a realização das atividades do componente II?', 'acomp2',  $stSql, $stSqlCarregados, 'Selecione de que forma o representante da Secretaria de Educação acompanha a realização das atividades do componente II nas escolas que integram o programa' );
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
				array('codigo' 	  => 'estado',
					  'descricao' => 'Estado'),				
				array('codigo' 	  => 'municipio',
					  'descricao' => 'Município')
				);
}
?>