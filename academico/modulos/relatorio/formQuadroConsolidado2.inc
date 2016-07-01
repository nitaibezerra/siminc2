<?php

if ($_POST['agrupador']){
	include("resultQuadroConsolidado2.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Monitoramento Acadêmico', '&nbsp;' );

?>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">
	
	function gerarRelatorio(){
		var formulario = document.formulario;
	
		if (formulario.elements['agrupador'][0] == null){
			alert('Selecione pelo menos um agrupador!');
			return false;
		}	
		
		selectAllOptions( formulario.agrupador );
		ajaxRelatorio();
		
	}
	
		/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
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
	
		var tipoensino = formulario.f_tipoensino.value;
		
		var exercicio_campo_flag 	   = formulario.exercicio_campo_flag.value;
		var unidade_campo_flag 		   = formulario.unidade_campo_flag.value;
		var campus_campo_flag 		   = formulario.campus_campo_flag.value;
		var programa_campo_flag 	   = formulario.programa_campo_flag.value;
		var classe_campo_flag 	   	   = formulario.classe_campo_flag.value;
		
		var exercicio_excludente = formulario.exercicio_campo_excludente;
		var unidade_excludente   = formulario.unidade_campo_excludente;
		var campus_excludente    = formulario.campus_campo_excludente;
		var programa_excludente  = formulario.programa_campo_excludente;
		var classe_excludente  	 = formulario.classe_campo_excludente;
		
		var exercicio_campo_excludente = 0;
		var unidade_campo_excludente   = 0;
		var campus_campo_excludente    = 0;
		var programa_campo_excludente  = 0;
		var classe_campo_excludente    = 0;
		
		if ( exercicio_excludente.checked ){
			exercicio_campo_excludente = exercicio_excludente.value;
		}
		if ( unidade_excludente.checked ){
			unidade_campo_excludente = unidade_excludente.value;
		}
		if ( campus_excludente.checked ){
			campus_campo_excludente = campus_excludente.value;
		}
		if ( programa_excludente.checked ){
			programa_campo_excludente = programa_excludente.value;
		}
		if ( classe_excludente.checked ){
			classe_campo_excludente = classe_excludente.value;
		}
		
		var exercicio = new Array();
		var unidade   = new Array();
		var campus 	  = new Array();
		var programa  = new Array();
		var classe    = new Array();
		
		if ( exercicio_campo_flag == '1' ){
			for(i=0; i < formulario.exercicio.options.length; i++){
				exercicio[i] = formulario.exercicio.options[i].value; 
			}
		}
		
		if ( unidade_campo_flag == '1' ){	
			for(i=0; i < formulario.unidade.options.length; i++){
				unidade[i] = formulario.unidade.options[i].value; 
			}
		}
		
		if ( campus_campo_flag == '1' ){			
			for(i=0; i < formulario.campus.options.length; i++){
				campus[i] = formulario.campus.options[i].value; 
			}
		}
		
		if ( programa_campo_flag == '1' ){			
			for(i=0; i < formulario.programa.options.length; i++){
				programa[i] = formulario.programa.options[i].value; 
			}	
		}
		
		if ( classe_campo_flag == '1' ){			
			for(i=0; i < formulario.classe.options.length; i++){
				classe[i] = formulario.classe.options[i].value; 
			}	
		}
					
		var param =  '&agrupador=' 	  			   + agrupador + 
					 '&f_tipoensino=' 			   + tipoensino +
					 '&exercicio=' 	  			   + exercicio +
					 '&exercicio_campo_excludente='+ exercicio_campo_excludente +
					 '&exercicio_campo_flag='	   + exercicio_campo_flag +
					 '&unidade=' 	  			   + unidade +
					 '&unidade_campo_excludente='  + unidade_campo_excludente +
					 '&unidade_campo_flag='	   	   + unidade_campo_flag +
					 '&campus=' 	  			   + campus +
					 '&campus_campo_excludente='   + campus_campo_excludente +
					 '&campus_campo_flag='	   	   + campus_campo_flag +
					 '&programa=' 	  			   + programa +
					 '&programa_campo_excludente=' + programa_campo_excludente +
					 '&programa_campo_flag='	   + programa_campo_flag +
					 '&classe=' 	  			   + classe +
					 '&classe_campo_excludente='   + classe_campo_excludente +
					 '&classe_campo_flag='	   	   + classe_campo_flag;
					 
	   var req = new Ajax.Request('academico.php?modulo=relatorio/formQuadroConsolidado2&acao=C', {
						        method:     'post',
						        parameters: param,
						        asynchronous: false,
						        onComplete: function (res)
						        {
							    	divRel.innerHTML = res.responseText;
						        }
							});
	}
</script>
<form name="formulario" id="formulario" action="" method="post">	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td class="SubTituloDireita" valign="top">Agrupadores</td>
			<td>
				<?php
				
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
				$db->monta_combo("f_tipoensino", $sql, 'S', "Todos", '', '', '', '', 'N', 'prgid');
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
								e.entnome AS descricao
							FROM 
								entidade.entidade e
							INNER JOIN
								academico.acumuladoprojetado ep ON e.entid = ep.entidentidade
							%s
							ORDER BY
								e.entnome, e.entid ";
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
			<td align="center" colspan="2">
				<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio();"/>
			</td>
		</tr>
	</table>
</form>
<div id="resultformulario">
<?php
//	if (!$_POST['agrupador']){
//		include "resultQuadroConsolidado2.inc";
//	}
?>
<div>

<?php
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