<?php

if ($_POST['agrupador']){
	ini_set("memory_limit","2048M");
	$true = true;
	include("relatorioConsolidacaoDemanda_resultado.inc");
	exit;
}
	
include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório Consolidacaçõ e Expansão das IFES', 'Demandas' );
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
function gerarRelatorio(req){
	var formulario = document.formulario;

	document.getElementById('req').value = req;
	
	if (formulario.elements['agrupador'][0] == null){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}	
	
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.estuf );
	selectAllOptions( formulario.muncod );
	selectAllOptions( formulario.universidade );
	selectAllOptions( formulario.campus );
	selectAllOptions( formulario.tbtid );
			
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
<input type="hidden" name="req" id="req" value="" />	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr> 
		<td class="SubTituloDireita" valign="top">Agrupadores</td>
		<td>
			<?
			
			$matriz = array(
						array('codigo' => 'estado',
							  'descricao' => 'UF'),				
						array('codigo' => 'municipio',
							  'descricao' => 'Município'),
						array('codigo' => 'universidade',
							  'descricao' => 'Universidade'),
						array('codigo' => 'campus',
							  'descricao' => 'Campus')
						);
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupador', null, null);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
		<?php
				// uf
				$stSql = " SELECT DISTINCT
								estuf as codigo,
								estdescricao as descricao 
							FROM 
								territorios.estado
							ORDER BY
								2";
				$stSqlCarregados = "";
				mostrarComboPopup( 'UF', 'estuf',  $stSql, $stSqlCarregados, 'Selecione o(s) UF(s)' ); 

				// munucipio
				$stSql = "SELECT 
								muncod as codigo, 
								estuf||' - '||mundescricao as descricao
							FROM 
								territorios.municipio
							WHERE
								1=1
							ORDER BY
								2";
				$stSqlCarregados = "";
				$where = Array( Array('codigo'=>'estuf', 'descricao'=>'UF') );
				mostrarComboPopup( 'Município', 'muncod',  $stSql, $stSqlCarregados, 'Selecione o(s) Município(s)', $where ); 
				
				// instituicao
				$stSql = "  SELECT DISTINCT
								e.entid AS codigo,
								CASE ef.funid
									WHEN " . ACA_ID_UNIVERSIDADE . " THEN 'Ed. Superior - ' || e.entnome
									ELSE 'Ed. Profissional - ' || e.entnome
								END AS descricao 
							FROM 
								entidade.entidade e
							INNER JOIN
								entidade.funcaoentidade ef ON ef.entid = e.entid
															  AND ef.funid IN ( '" . ACA_ID_UNIVERSIDADE . "' )		
							INNER JOIN
								academico.acumuladoprojetado ep ON e.entid = ep.entidentidade
							ORDER BY
								descricao, e.entid ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Universidade', 'universidade',  $stSql, $stSqlCarregados, 'Selecione a(s) Universidade(s)' ); 

				// Campus
				$stSql = "  SELECT
								e.entid as codigo, 
								upper(e3.entsig)||' - '||upper(e.entnome) as descricao
							FROM
								entidade.entidade e2
							INNER JOIN entidade.entidade 		e 	ON e2.entid = e.entid
							INNER JOIN entidade.funcaoentidade 	ef  ON ef.entid = e.entid
							INNER JOIN entidade.funentassoc 	ea  ON ea.fueid = ef.fueid 
							INNER JOIN entidade.entidade 		e3 	ON e3.entid = ea.entid
							LEFT  JOIN entidade.endereco 		ed  ON ed.entid = e.entid
							LEFT  JOIN territorios.municipio 	mun ON mun.muncod = ed.muncod
							WHERE
								e.entstatus = 'A' AND ef.funid = ".ACA_ID_CAMPUS."
							ORDER BY
								2";
				$stSqlCarregados = "";
				$where = Array( Array('codigo'=>'e3.entnome', 'descricao'=>'Universidade (Nome)'),Array('codigo'=>'e3.entsig', 'descricao'=>'Universidade (Sigla)') );
				mostrarComboPopup( 'Campus', 'campus',  $stSql, $stSqlCarregados, 'Selecione o(s) Campus(s)', $where ); 

				// Tipo da Demanda
				$stSql = "  SELECT 
								tbtid as codigo, 
								tbttipo as descricao
							FROM 
								academico.tabelatipo
							WHERE
								tbtid NOT IN (6,7)";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Tipos de Demanda', 'tbtid',  $stSql, $stSqlCarregados, 'Selecione o(s) Tipo(s) de Demanda' ); 
			
			?>
	<tr> 
		<td class="SubTituloDireita" valign="top">Demanda</td>
		<td>
			<?=campo_texto('tbrdemanda', 'N', 'S', '', 50, 100, '', '')?>
		</td>
	</tr>
	<tr> 
		<td class="SubTituloDireita" valign="top">Expandir Relatório</td>
		<td>
			<?
			$expandir = 0;
			$sql = "(SELECT
						1 as codigo,
						'Expandir' as descricao)
					UNION ALL
					(SELECT
						0 as codigo,
						'Não Expandir' as descricao)";	

			$db->monta_radio('expandir', $sql, '',1);
			?>
		</td>
	</tr>
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio(1);"/>
<!--			<input type="button" name="Gerar Relatório XLS" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio(2);"/>-->
		</td>
	</tr>
</table>
</form>
</body>
</html>