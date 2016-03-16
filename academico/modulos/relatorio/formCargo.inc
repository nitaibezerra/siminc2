<?php

if ($_POST['agrupador']){
	ini_set("memory_limit","2048M");
	$true = true;
	include("resultGeral.inc");
	exit;
}
//Quando recuperado o parâmetro enviado via Ajax, valor será testado logicamente nas segunites condições:  
if($_POST['tipo'] == 'educacao'){
	//Se " educacaoId == 1 " serão apresentadas apenas as Instituições da Educação Superior.
	if($_POST['educacaoId'] == '1'){ 
			$whereTipoEnsino  =  " WHERE ef.funid = ' " . ACA_ID_UNIVERSIDADE . " ' ";
	//Senão se " educacaoId == 2 " serão apresentadas apenas  as Instituições da Educação Profissional.
	}else if ($_POST['educacaoId'] == '2'){
			$whereTipoEnsino  =  " WHERE ef.funid = ' " . ACA_ID_ESCOLAS_TECNICAS . " ' ";
	}
}	
include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Monitoramento Acadêmico', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">
function gerarRelatorio(){
	var formulario = document.formulario;

	if (formulario.elements['agrupador'][0] == null){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}	
	
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.exercicio );
	selectAllOptions( formulario.unidade );
	selectAllOptions( formulario.campus );
	selectAllOptions( formulario.programa );
	selectAllOptions( formulario.classe );	
			
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

function ajaxRelatorio(){
	var formulario = document.formulario;
	var divRel 	   = document.getElementById('resultformulario');

	divRel.style.textAlign='center';
	divRel.innerHTML = 'carregando...';

	var agrupador = new Array();	
	for(i=0; i < formulario.agrupador.options.length; i++){
		agrupador[i] = formulario.agrupador.options[i].value; 
	}	
	
	var tipoensino = new Array();
	for(i=0; i < formulario.f_tipoensino.options.length; i++){
		tipoensino[i] = formulario.f_tipoensino.options[i].value; 
	}		
	
	var param =  '&agrupador=' + agrupador + 
				 '&f_tipoensino=' + tipoensino +
				 '&f_tipoensino_campo_excludente=' + formulario.f_tipoensino_campo_excludente.checked +
				 '&f_tipoensino_campo_flag=' + formulario.f_tipoensino_campo_flag.value;
	 
    var req = new Ajax.Request('academico.php?modulo=inicio&acao=C', {
						        method:     'post',
						        parameters: param,
						        onComplete: function (res)
						        {
							    	divRel.innerHTML = res.responseText;
						        }
							});
	
}
//Quando selecionado o radiobutton em um tipo de Educação, passa por parâmetro o valor do mesmo, via Ajax.   
function selecionaEducacao(idEducacao){
	var eduId = idEducacao.value;
	var url = '?modulo=relatorio/formCargo&acao=C'; 
	new Ajax.Request(url,{
		method: 'post',
		parameters: '&tipo=educacao&educacaoId=' + eduId, 
			onComplete: function(res){
				res.responseText;
			}
		}
	);
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
			$campoAgrupador->setDestino( 'agrupador', null, array(
																	array('codigo' => 'tipoensino',
																		  'descricao' => 'Tipo Ensino')			
																	));
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Tipo de Ensino</td>	
		<td>
		<?php
			$sql = " SELECT
							orgid AS codigo,
							orgdesc AS descricao
						FROM
							academico.orgao 
						Where
							orgstatus = 'A'
						ORDER BY 2 ASC;";
			$db->monta_combo("f_tipoensino", $sql, 'S', "Todos", 'selecionaEducacao(this);', '', '', '', 'N', 'prgid');
	    ?>		

		</td>
	</tr>
		<?php
			
				// Filtros do relatório
			
				// Exercício
				$stSql = " SELECT DISTINCT
								prtano AS codigo,
								prtano AS descricao
							FROM 
								academico.portarias
							%s
							ORDER BY
								prtano ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Exercício', 'exercicio',  $stSql, $stSqlCarregados, 'Selecione o(s) Exercício(s)' ); 
			
				// Unidade
				$stSql = " SELECT DISTINCT
								e.entid AS codigo,
								CASE ef.funid
									WHEN " . ACA_ID_UNIVERSIDADE . " THEN 'Ed. Superior - ' || e.entnome
									ELSE 'Ed. Profissional - ' || e.entnome
								END AS descricao 
							FROM 
								entidade.entidade e
							INNER JOIN
								entidade.funcaoentidade ef ON ef.entid = e.entid
															  AND ef.funid IN ( '" . ACA_ID_UNIVERSIDADE . "', '" . ACA_ID_ESCOLAS_TECNICAS . "' )		
							INNER JOIN
								academico.acumuladoprojetado ep ON e.entid = ep.entidentidade
							$whereTipoEnsino 
							%s
							ORDER BY
								descricao, e.entid ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Unidade', 'unidade',  $stSql, $stSqlCarregados, 'Selecione a(s) Unidade(s)' ); 
							
				// Campus
				$stSql = " SELECT DISTINCT
								e.entid AS codigo,
								e.entnome AS descricao
							FROM 
								entidade.entidade e
							INNER JOIN
								academico.acumuladoprojetado ep ON e.entid = ep.entidcampus
							%s
							ORDER BY
								e.entnome, e.entid ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Campus', 'campus',  $stSql, $stSqlCarregados, 'Selecione o(s) Campus(s)' ); 
			
				// Programa
				$stSql = " SELECT
								prgid AS codigo,
								prgdsc AS descricao
							FROM 
								academico.programa
							%s
							ORDER BY
								prgdsc ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Programa', 'programa',  $stSql, $stSqlCarregados, 'Selecione o(s) Programa(s)' ); 
				
				// Classe
				$stSql = " SELECT
								clsid AS codigo,
								clsdsc AS descricao
							FROM 
								academico.classes
							%s
							ORDER BY
								clsdsc ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Classe', 'classe',  $stSql, $stSqlCarregados, 'Selecione a(s) Classe(s)' ); 
				
				
			?>
			
		<tr>
			<td class="SubTituloDireita">Período de Inclusão <br>do Edital de Homologação:</td>
			<td>
				<?= campo_data( 'dtini', 'S', 'S', '', '' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data( 'dtfim', 'S', 'S', '', '' ); ?>
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
<?
function agrupador(){
	return array(
				array('codigo' => 'tipoensino',
					  'descricao' => 'Tipo Ensino'),				
				array('codigo' => 'portaria',
					  'descricao' => 'Portaria'),
				array('codigo' => 'unidade',
					  'descricao' => 'Unidade'),
				array('codigo' => 'campus',
					  'descricao' => 'Campus'),
				array('codigo' => 'classe',
					  'descricao' => 'Classe'),
				array('codigo' => 'programa',
					  'descricao' => 'Programa'),
				array('codigo' => 'ano',
					  'descricao' => 'Ano')				
				);
}
?>
</html>