<?php

if ($_POST['quest']){
	ini_set("memory_limit","256M");
	include("resultRelatorioGeral.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório Geral', '&nbsp;' );
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
	selectAllOptions( formulario.estado	   );
	selectAllOptions( formulario.dimensao  );
	selectAllOptions( formulario.areas     );
	selectAllOptions( formulario.indicador );
	selectAllOptions( formulario.plano     );
	selectAllOptions( formulario.programa  );
	selectAllOptions( formulario.status    );
	selectAllOptions( formulario.forma 	   );
			
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

//function ajaxRelatorio(){
//	var formulario = document.formulario;
//	var divRel 	   = document.getElementById('resultformulario');
//
//	divRel.style.textAlign='center';
//	divRel.innerHTML = 'carregando...';
//
//	var agrupador = new Array();	
//	for(i=0; i < formulario.agrupador.options.length; i++){
//		agrupador[i] = formulario.agrupador.options[i].value; 
//	}	
//	
//	var tipoensino = new Array();
//	for(i=0; i < formulario.f_tipoensino.options.length; i++){
//		tipoensino[i] = formulario.f_tipoensino.options[i].value; 
//	}		
//	
//	var param =  '&agrupador=' + agrupador + 
//				 '&f_tipoensino=' + tipoensino +
//				 '&f_tipoensino_campo_excludente=' + formulario.f_tipoensino_campo_excludente.checked +
//				 '&f_tipoensino_campo_flag=' + formulario.f_tipoensino_campo_flag.value;
//	 
//    var req = new Ajax.Request('academico.php?modulo=inicio&acao=C', {
//						        method:     'post',
//						        parameters: param,
//						        onComplete: function (res)
//						        {
//							    	divRel.innerHTML = res.responseText;
//						        }
//							});
//	
//}
--></script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">	
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
	<?php
	
		// Filtros do relatório
		
		//Filtro de Estado
		$stSql = "SELECT
					estuf AS codigo,
					estdescricao AS descricao
				  FROM
				  	territorios.estado";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Estado', 'estado',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)' ); 
		
		//Filtro de Dimensão
		$stSql = "SELECT
				      dimid as codigo,
				      dimcod || ' - ' || dimdsc as descricao
				FROM
					cte.dimensao
				WHERE
				      dimstatus = 'A' AND
				      itrid = '".INSTRUMENTO_DIAGNOSTICO_ESTADUAL."'
				ORDER BY
				      dimcod";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Dimensão', 'dimensao',  $stSql, $stSqlCarregados, 'Selecione a(s) Dimensão(ões)' ); 
		
		//Filtro de Áreas
		$stSql = "SELECT
				      a.ardid as codigo,
				      d.dimcod || '.' || a.ardcod || ' - ' || substr( a.arddsc, 0, 95 ) || '...' as descricao
				FROM
					cte.areadimensao a
				INNER JOIN cte.dimensao d ON d.dimid = a.dimid
				WHERE
				      a.ardstatus = 'A' AND
				      d.itrid = '" . INSTRUMENTO_DIAGNOSTICO_ESTADUAL . "'
				ORDER BY
				      d.dimcod,
				      a.ardcod";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Áreas', 'areas',  $stSql, $stSqlCarregados, 'Selecione a(s) Área(s)' ); 
		
		//Filtro de Indicador
		$stSql = "SELECT
				      i.indid as codigo,
				      d.dimcod || '.' || a.ardcod || '.' || i.indcod || ' - ' || substr( i.inddsc, 0, 95 ) || '...' as descricao
				FROM
					cte.indicador i
				INNER JOIN cte.areadimensao a ON a.ardid = i.ardid
				INNER JOIN cte.dimensao d ON d.dimid = a.dimid
				WHERE
				      i.indstatus = 'A' AND
				      d.itrid = '" . INSTRUMENTO_DIAGNOSTICO_ESTADUAL . "'
				ORDER BY
				      d.dimcod,
				      a.ardcod,
				      i.indcod";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Indicador', 'indicador',  $stSql, $stSqlCarregados, 'Selecione o(s) Indicador(es)' ); 
		
		//Filtro de Plano Interno
		$stSql = "SELECT
				      plicod as codigo,
				      plicod || ' - ' ||plidsc as descricao
				FROM
					cte.programa p
				INNER JOIN financeiro.planointerno pi ON pi.plicod = p.prgplanointerno
				ORDER BY
					prgplanointerno";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Plano Interno', 'plano',  $stSql, $stSqlCarregados, 'Selecione o(s) Plano(s) Interno(s)' ); 
		
		//Filtro de Programa
		$stSql = "SELECT
				      prgid as codigo,
				      prgdsc as descricao
				FROM
					cte.programa                        
				ORDER BY
				    prgdsc";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Programa', 'programa',  $stSql, $stSqlCarregados, 'Selecione o(s) Programa(s)' ); 
		
		//Filtro de Status Subação
		$stSql = "SELECT
				      ssuid as codigo,
				      ssudescricao as descricao
				FROM
					cte.statussubacao
				ORDER BY
				    ssudescricao";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Status Subação', 'status',  $stSql, $stSqlCarregados, 'Selecione o(s) Status de Subação' ); 
		
		//Filtro de Forma de Execução
		$stSql = "SELECT
				      frmid as codigo,
				      frmdsc as descricao
				FROM
					cte.formaexecucao
				WHERE
				    frmbrasilpro = false AND
				    frmtipo      = 'E'
				ORDER BY
				    frmdsc";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Forma de Execução', 'forma',  $stSql, $stSqlCarregados, 'Selecione a(s) Forma(s) de Execução' ); 
		
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
				array('codigo' 	  => 'dimensao',
					  'descricao' => 'Dimensão'),	
				array('codigo' 	  => 'area',
					  'descricao' => 'Área'),				
				array('codigo' 	  => 'indicador',
					  'descricao' => 'Indicador'),				
				array('codigo' 	  => 'codibge',
					  'descricao' => 'Código do IBGE'),				
				array('codigo' 	  => 'numconvenio',
					  'descricao' => 'Número do Convênio'),				
				array('codigo' 	  => 'vlrconvenio',
					  'descricao' => 'Valor do Convênio'),				
				array('codigo' 	  => 'vlrempenhado',
					  'descricao' => 'Valor Empenhado'),				
				array('codigo' 	  => 'vlrpago',
					  'descricao' => 'Valor Pago'),				
				array('codigo' 	  => 'vlraempenhar',
					  'descricao' => 'Valor a Empenhar'),				
				array('codigo' 	  => 'vlrapagar',
					  'descricao' => 'Valor a Pagar'),				
				array('codigo' 	  => 'porcentexec',
					  'descricao' => '% de Execução'),				
				array('codigo' 	  => 'prestadoconta',
					  'descricao' => 'Prestado Conta'),				
				array('codigo' 	  => 'acao',
					  'descricao' => 'Ação'),				
				array('codigo' 	  => 'subacao',
					  'descricao' => 'Subação'),				
				array('codigo' 	  => 'formaexecucao',
					  'descricao' => 'Forma de Execução'),				
				array('codigo' 	  => 'programa',
					  'descricao' => 'Programa'),				
				array('codigo' 	  => 'statussubacao',
					  'descricao' => 'Status da Subação'),				
				array('codigo' 	  => 'planointerno',
					  'descricao' => 'Plano Interno')
				);
}