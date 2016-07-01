<?php

if ($_POST['quest']){
	ini_set("memory_limit","256M");
	include("resultRelatorioQuestionarioSeesp.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Escolas', '&nbsp;' );
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
	selectAllOptions( formulario.perg6 );
	selectAllOptions( formulario.perg7 );
			
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
		<td class="SubTituloDireita" valign="top">
			Selecione o questionário desejado:
		</td>
		<td>
			<input type='radio' checked='checked' <?php if( QUESTIONARIO_VI  == $_REQUEST['quest'] ){echo 'checked=checked';} ?> value='<?=QUESTIONARIO_VI?>'  name='quest' id='quest' />Ação: Programa de Implantação de Salas de Recursos Multifuncionais – Edição 2008</br> 
			<input type='radio' <?php if( QUESTIONARIO_VII == $_REQUEST['quest'] ){echo 'checked=checked';} ?> value='<?=QUESTIONARIO_VII?>' name='quest' id='quest' />Ação: Programa Escola Acessível – Edição 2008
		</td>
	</tr>
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
		$stSql = "SELECT
					estuf AS codigo,
					estdescricao AS descricao
				  FROM
				  	territorios.estado";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Estado', 'estado',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)' ); 
	
		// Perguntas do quest VI
		$stSql = "SELECT
					perid as codigo,
					pertitulo as descricao
				FROM
					questionario.pergunta
				WHERE
					perid in ( SELECT
									perid
								FROM
									questionario.pergunta
								WHERE
									grpid IN ( SELECT
													grpid
												FROM
													questionario.grupopergunta
												WHERE
													queid IN ( ".QUESTIONARIO_VI." ) and grpordem = 2  
												) 
								)
				ORDER BY
					pertitulo, perid";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Perguntas do Questionário "Ação: Programa de Implantação de Salas de Recursos Multifuncionais – Edição 2008"', 'perg6',  $stSql, $stSqlCarregados, 'Selecione a(s) Pergunta(s) do Questionário "Ação: Programa de Implantação de Salas de Recursos Multifuncionais – Edição 2008"' ); 
		
		// Perguntas do quest VII
		$stSql = "SELECT
					perid as codigo,
					pertitulo as descricao
				FROM
					questionario.pergunta
				WHERE
					perid in ( SELECT
									perid
								FROM
									questionario.pergunta
								WHERE
									grpid IN ( SELECT
													grpid
												FROM
													questionario.grupopergunta
												WHERE
													queid IN ( ".QUESTIONARIO_VII." ) and grpordem = 2  
												) 
								)
				ORDER BY
					pertitulo";

		$stSqlCarregados = "";
		mostrarComboPopup( 'Perguntas do Questionário "Ação: Programa Escola Acessível – Edição 2008"', 'perg7',  $stSql, $stSqlCarregados, 'Selecione a(s) Pergunta(s) do Questionário "Ação: Programa Escola Acessível – Edição 2008"' ); 
		
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
				array('codigo' 	  => 'questionario',
					  'descricao' => 'Questionário'),	
				array('codigo' 	  => 'municipio',
					  'descricao' => 'Município'),
				array('codigo' 	  => 'estado',
					  'descricao' => 'Estado'),				
				array('codigo' 	  => 'escolas',
					  'descricao' => 'Escolas'),				
				array('codigo' 	  => 'perguntas',
					  'descricao' => 'Perguntas'),	
				array('codigo' 	  => 'respostas',
					  'descricao' => 'Respostas')		

				);
}
?>