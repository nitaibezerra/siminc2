<?php
ini_set("memory_limit", "50000M");

	if($_REQUEST['requisicaoAjax']){
		header('content-type: text/html; charset=ISO-8859-1');
		$_REQUEST['requisicaoAjax']();
		exit;
	}

	if ( isset( $_REQUEST['form'] ) == true ){

		include "resultRelatorioPagamentos.inc";
		exit;
	}

	
	
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';

	monta_titulo( 'Relatório de Pagamentos', '&nbsp;' );
	
	
	
	function filtrarMunicipio()
	{
	#Municipio
	$munSql = " select muncod as codigo, mundescricao as descricao  from territorios.municipio ";
	
	$estadosFiltrar = trim( $_REQUEST['estados'],",");
	
	if ($estadosFiltrar)
	{
	$munSql.=" where estuf in (".str_replace("\\","", $estadosFiltrar).")";
	}
	
	mostrarComboPopup( 'Município', 'slMunicipio',  $munSql, $stSqlCarregados, 'Selecione os municípios', null,'',true );
	die;
	}
	
	?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js">
</script>
<script type="text/javascript">

	function getEstados()
	{
		var estados = '';
		
		var elemento = document.getElementsByName('slEstado[]')[0];
			
		for (var i=0; i<elemento.options.length; i++){
			if (elemento.options[i].value != '')
			{
				estados += "'" + elemento.options[i].value + "',";
			}
		}
		
		return estados;
	}


	function ajaxEstado()
	{
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=filtrarMunicipio&estados="+getEstados(),
			success: function(retorno){
				jQuery('#idMunicipio').html(retorno);
			}});
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

	/**
	 * Alterar visibilidade de um bloco.	 
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

	function gerarRelatorio(tipo){
		
		var formulario = document.formulario;
		var tipo_relatorio = tipo;

		prepara_formulario();

		if (tipo_relatorio == 'relatorio')
		{
			formulario.action = 'painel.php?modulo=relatorio/relatorioIndicadores&acao=A';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
			
		}

		else if(tipo_relatorio == 'visual'){

			document.getElementById('tipo_relatorio').value = 'visual';
			var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
		}
		else{

			document.getElementById('tipo_relatorio').value = 'xls';
			var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
		}
		formulario.submit();
	}
</script>
	<form name="formulario" id="formulario" action="" method="post">
	
		<input type="hidden" name="form" value="1" /> 
		<input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value="" /> 

		<table id="tabela_filtros2" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr>
				<td class="subtituloesquerda" colspan="2"><strong>Filtros</strong>
				</td>
			</tr>
			<tr>
				<td><?php 	
				#UF
				$ufSql = "
				Select 	estuf as codigo,
				estdescricao as descricao
				From territorios.estado est
				Order by estdescricao
				";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Estado', 'slEstado',  $ufSql, '', 'Selecione o(s) Estado(s)',null,'ajaxEstado',false );
				?>
				</td>
			</tr>
			<tr id='idMunicipio'>
				<?
				#Municipio
				$munSql = " select muncod as codigo, mundescricao as descricao  from territorios.municipio ";

				mostrarComboPopup( 'Município', 'slMunicipio',  $munSql, $stSqlCarregados, 'Selecione os municípios', null,'',true );
				?>
			</tr>
			<tr>
				<td class="subtitulodireita">&nbsp;</td>
				<td align="center"><input type="button" name="Gerar Relatório"
					value="Gerar Relatório"
					onclick="javascript:gerarRelatorio('visual');" /> <input
					type="button" name="Gerar Relatório" value="Visualizar XLS"
					onclick="javascript:gerarRelatorio('xls');" />
				</td>
			</tr>
		</table>
	</form>
