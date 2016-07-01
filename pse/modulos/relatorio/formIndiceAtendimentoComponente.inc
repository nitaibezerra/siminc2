<?php

//--------------- regiao x estados -----------------------------------------------------------------------------

if($_POST['ajaxRegioes'])
{
	header('content-type: text/html; charset=ISO-8859-1');
	
	if($_POST['ajaxRegioes'] != "tudo") 
	{
		$arrReg = explode(",",$_POST['ajaxRegioes']);
		
		$wh = " WHERE regcod in ('".implode("','",$arrReg)."') ";
	}
	else
	{
		$wh = "";
	}
	
	$stSql = "SELECT
				estuf        AS codigo,
				estdescricao AS descricao
			  FROM
			  	territorios.estado
			  ".$wh."
			  ORDER BY 
			  	descricao ";
	die( mostrarComboPopup( 'Estado', 'estado',  $stSql, "", 'Selecione o(s) Estado(s)', null, null, true  ) );
}

//--------------- estado x municípios -----------------------------------------------------------------------------

if($_POST['ajaxEstados'])
{
	header('content-type: text/html; charset=ISO-8859-1');
	
	if($_POST['ajaxEstados'] != "tudo") 
	{
		$arrUfs = explode(",",$_POST['ajaxEstados']);
		
		$wh = " WHERE estuf in ('".implode("','",$arrUfs)."') ";
	}
	else
	{
		$wh = "";
	}
	
	$stSql = "SELECT
				muncod AS codigo,
				estuf || ' - ' || mundescricao AS descricao
			  FROM
			  	territorios.municipio
			  ".$wh."
			  ORDER BY 
			  	descricao ";
	die( mostrarComboPopup( 'Município', 'municipio',  $stSql, "", 'Selecione o(s) Município(s)', null, null, true  ) );
}


if ($_POST['agrupador']){
	ini_set("memory_limit","256M");
	include_once("resultIndiceAtendimentoComponente.inc");
	exit;
}  

include_once APPRAIZ. '/includes/Agrupador.php';
include_once APPRAIZ . '/includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Índice de atendimento/Componentes', '&nbsp;' );
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
	selectAllOptions( formulario.regiao );
	selectAllOptions( formulario.estado );
	selectAllOptions( formulario.municipio );
	selectAllOptions( formulario.componente_acao );
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
				mostrarComboPopup( 'Portaria', 'anoportaria',  $stSql, "", 'Selecione o Ano' );		
		
		
				// --------------------- regiao ----------------------------------------------
				$stSql = "SELECT
							regcod       AS codigo,
							regdescricao AS descricao
						  FROM
						  	territorios.regiao ";
				
				mostrarComboPopup( 'Região', 'regiao',  $stSql, "", 'Selecione a(s) Região(ões)', null, 'filtraEstado' ); 
	
				
				// --------------------- estado ----------------------------------------------
				$stSql = "SELECT
							estuf        AS codigo,
							estdescricao AS descricao
						  FROM
						  	territorios.estado
						  ORDER BY 
						  	descricao ";
				?>
				<tr id="tr_estado">
				<?php
				mostrarComboPopup( 'Estado', 'estado',  $stSql, "", 'Selecione o(s) Estado(s)', null, 'filtraMunicipio', true  );
				?>
				</tr>
				<?
				
				// --------------------- municipio ----------------------------------------------
				$stSql = "SELECT
							muncod AS codigo,
							estuf || ' - ' || replace(mundescricao, '\'', '') AS descricao
						  FROM
						  	territorios.municipio	
						  WHERE
						  	1=1
						  ORDER BY
						  	descricao";
				?>
				<tr id="tr_municipio">
				<?php
				mostrarComboPopup( 'Município', 'municipio',  $stSql, "", 'Selecione o(s) Município(s)', null, null, true  );
				?>
				</tr>
				<?				
				
				// --------------------- componente -------------------------------------------				
				$stSql = "SELECT '1' as codigo , 'Componente I' as descricao 
						  UNION ALL
						  SELECT '2' as codigo , 'Componente II' as descricao
						  UNION ALL
						  SELECT '3' as codigo , 'Componente III' as descricao
				";
				mostrarComboPopup( 'Componente(s)', 'componente_acao',  $stSql, "", 'Selecione qual(is) componente(s) deseja visualizar' ); 
				
			?>
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio();"/>
		</td>
	</tr>
</table>
</form>
</body>
<script type="text/javascript">
var arrayRegioes = new Array();

function filtraEstado(obj)
{
	if(obj.checked == true)
	{
		arrayRegioes.push(obj.value);
	}
	else
	{
		for(var i=0; i<arrayRegioes.length; i++)
		{
			if(arrayRegioes[i] == obj.value)
			{
				arrayRegioes.splice(i, 1);
			}
		}
	}

	var regioes = arrayRegioes.join(",");
	regioes = (regioes == "") ? "tudo" : regioes;
	
	var req = new Ajax.Request('pse.php?modulo=relatorio/formIndiceAtendimentoComponente&acao=A', {
		asynchronous: false,
        method:     'post',
        parameters: '&ajaxRegioes=' + regioes,
        onComplete: function (res)
        {							        	
			document.getElementById("tr_estado").innerHTML = res.responseText;
        }
  	});
}



var arrayEstados = new Array();

function filtraMunicipio(obj)
{
	if(obj.checked == true)
	{
		arrayEstados.push(obj.value);
	}
	else
	{
		for(var i=0; i<arrayEstados.length; i++)
		{
			if(arrayEstados[i] == obj.value)
			{
				arrayEstados.splice(i, 1);
			}
		}
	}

	var estados = arrayEstados.join(",");
	estados = (estados == "") ? "tudo" : estados;
	
	var req = new Ajax.Request('pse.php?modulo=relatorio/formIndiceAtendimentoComponente&acao=A', {
		asynchronous: false,
        method:     'post',
        parameters: '&ajaxEstados=' + estados,
        onComplete: function (res)
        {							        	
			document.getElementById("tr_municipio").innerHTML = res.responseText;
        }
  	});
}


</script>
</html>
<?
function agrupador(){
	return array(
				array('codigo' 	  => 'anoportaria',
					  'descricao' => '1 - Ano da portaria'),
	
				array('codigo' 	  => 'regiao',
					  'descricao' => '2 - Região'),
	
				array('codigo' 	  => 'estado',
					  'descricao' => '3 - Estado'),

				array('codigo' 	  => 'municipio',
					  'descricao' => '4 - Município'),					
			
				array('codigo' 	  => 'componente_acao',
					  'descricao' => '5 - Componente/Ação'),
				
				array('codigo' 	  => 'subacao',
					  'descricao' => '6 - Subação')

				);
}
?>