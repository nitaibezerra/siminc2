<?php
ini_set("memory_limit", "50000M");




if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
}

/* Componente Historico de consulta.
 * Transforma consulta em pública.
 * Código implementado em 17/08/2012.
 */
if ( $_REQUEST['prtid'] && $_REQUEST['publico'] ){
	$sql = sprintf(
			"UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
			$_REQUEST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	$db->sucesso('relatorio/relatorioIndicadores');
	die;
}



// remove consulta
if ( $_REQUEST['prtid'] && $_REQUEST['excluir'] == 1 ) {
		$sql = sprintf(
				"DELETE from public.parametros_tela WHERE prtid = %d",
				$_REQUEST['prtid']
		);
		$db->executar( $sql );
		$db->commit();
		?>
<script type="text/javascript">
			location.href = '?modulo=relatorio/relatorioIndicadores&acao=A';
		</script>
<?
die;
		}
		
		
		
		
		if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] == 1 ){
				
				
			$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = ".$_REQUEST['prtid'] );
			$itens = $db->pegaUm( $sql );
			$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
			
			extract( $dados );
			
			$_REQUEST = $dados;
			$_POST = $dados;
			
			unset( $_REQUEST['form'] );
			unset( $_REQUEST['pesquisa'] );
			unset( $_REQUEST['salvar']);
			$titulo = $_REQUEST['titulo'];
			$_POST['tipo_relatorio'] = 'visual';

		}
		
		
		
		
		if ( isset( $_REQUEST['salvar'] ) == true  ){
			if ( $_REQUEST['prtid'] ){
				$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
				$itens = $db->pegaUm( $sql );
				$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
				$_POST = $dados;//array_merge( $_REQUEST, $dados );
				unset( $_POST['salvar'] );
			}

			include "resultRelatorioIndicadores.inc";
			exit;
		}
		
		
		//ver($_REQUEST,d);

		function verificarIndicadores()
		{
			extract ($_POST);
			
			$indicadores = trim($indicadores,',');
			
			global $db;
			$sql = "select distinct(i.umeid), u.umedesc, i.indqtdevalor from painel.indicador i
			inner join painel.unidademeta u on i.umeid = u.umeid
			where indid in ({$indicadores})";
			
			$umeid = "";
			$umedesc = "";
			$umes = $db->carregar($sql);
			
			$indqtdevalor = false;
			if ( $umes ){
				foreach ( $umes as $ume )
				{
					if ($ume['indqtdevalor'] == 't')
						$indqtdevalor = true;
					
					$umeid .=$ume['umeid']. ","; 
					$umedesc .=$ume['umedesc']. ",";
				}
			}
			
			if ($indqtdevalor)
			{
			 $umeid = 'dshvalor,'.$umeid;
			 $umedesc = 'Valor,'.$umedesc;
			} 
				
			$umeid = trim( $umeid,",");
			$umedesc = trim( $umedesc,",");
			
			echo ($umeid."|".$umedesc);
			die;
		}
		
		function verificarIndicadoresNaoAgrupando()
		{
			extract ($_POST);
				
			$indicadores = trim($indicadores,',');
				
			global $db;
			$sql = "select i.umeid, u.umedesc || ' - ' || i.indid as umedesc, i.indqtdevalor from painel.indicador i
			inner join painel.unidademeta u on i.umeid = u.umeid
			where indid in ({$indicadores})";
				
			$umeid = "";
			$umedesc = "";
			$umes = $db->carregar($sql);
				
			$indqtdevalor = false;
			if ( $umes ){
			foreach ( $umes as $ume )
			{
			if ($ume['indqtdevalor'] == 't')
				$indqtdevalor = true;
					
				$umeid .=$ume['umeid']. ",";
			$umedesc .=$ume['umedesc']. ",";
			}
			}
				
			//if ($indqtdevalor)
			//{
			//$umeid = 'dshvalor,'.$umeid;
			//$umedesc = 'Valor,'.$umedesc;
			//}/
		
			$umeid = trim( $umeid,",");
			$umedesc = trim( $umedesc,",");
			
			echo ($umeid."|".$umedesc);
			die;
		}
		
		
		function verificarDetalhesIndicadores()
		{
			extract ($_POST);
				
			$indicadores = trim($indicadores,',');
				
			global $db;
			$sql = "select * from painel.detalhetipoindicador where indid in ({$indicadores})";
			//detalhetipodadosindicador
			$umeid = "";
			$umedesc = "";
			$dscs = $db->carregar($sql);
			if ( $dscs ){
			foreach ( $dscs as $d )
			{
			$umeid .=$ume['umeid']. ",";
			$umedesc .=$ume['umedesc']. ",";
			}
			}
				
			$umeid = trim( $umeid,",");
			$umedesc = trim( $umedesc,",");
				
			echo ($umeid."|".$umedesc);
			die;
		}
		
		
		function colunasOrigem(){
			/*
			return array(

					array(
							'codigo'    => 'dshvalor',
							'descricao' => '01. Valor'
					)
			);
			*/
		}

		function AgrupadorOrigem(){
			return array(
					array(
							'codigo'    => 'indnome',
							'descricao' => '01. Indicador'
					),
					array(
							'codigo'    => 'paiid',
							'descricao' => '02. Brasil'
					),
					array(
							'codigo'    => 'dshuf',
							'descricao' => '03. Estado'
					)
					,
					array(
							'codigo'    => 'mesdsc',
							'descricao' => '04. Mesorregião'
					)
					,
					array(
							'codigo'    => 'micdsc',
							'descricao' => '05. Microrregião'
					)
					,
					array(
							'codigo'    => 'mundescricao',
							'descricao' => '06. Município'
					)
					,
					array(
							'codigo'    => 'tiddsc',
							'descricao' => '07. Detalhe 01'
					)
					,
					array(
							'codigo'    => 'tiddsc2',
							'descricao' => '08. Detalhe 02'
					)
			);
		}

		function filtrarMesoregiao( )
		{
			#Mesoregiao
			$mesSql = " Select	mescod AS codigo,
							mesdsc AS descricao
							From territorios.mesoregiao";

			$estadosFiltrar = trim( $_REQUEST['estados'],",");

			if ($estadosFiltrar)
			{
				$mesSql.=" where estuf in (".str_replace("\\","", $estadosFiltrar).")";
			}

			$mesSql.= " order by mesdsc";

			$stSqlCarregados = "";

			mostrarComboPopup( 'Mesorregião', 'slMesoregiao',  $mesSql, $stSqlCarregados, 'Selecione as Mesoregiões', null,ajaxMicroregiao,false );
		}

		function filtrarMicroregiao( )
		{
			#Microregiao
			$micSql = " Select	miccod AS codigo,
							micdsc AS descricao
							From territorios.microregiao";

			$estadosFiltrar = trim( $_REQUEST['estados'],",");

			if ($estadosFiltrar)
			{
				$micSql.=" where mescod in (select mescod from territorios.mesoregiao where estuf in (".str_replace("\\","", $estadosFiltrar)."))";
			}

			$mesoFiltrar = trim( $_REQUEST['mesoregioes'],",");

			if ($mesoFiltrar)
			{
				if (substr_count($micSql,"where") == 0)
				{
					$micSql.=" where ";
				}
				else
				{
					$micSql.=" and ";
				}

					
				$micSql.=" mescod in (".str_replace("\\","", $mesoFiltrar).")";
			}

			$micSql.= " order by micdsc";

			$stSqlCarregados = "";

			mostrarComboPopup( 'Microrregião', 'slMicroregiao',  $micSql, $stSqlCarregados, 'Selecione as Microregiões', null,ajaxMunicipio,true);
		}

		function filtrarMunicipio()
		{
			#Municipio
			$munSql = " select muncod as codigo, mundescricao as descricao  from territorios.municipio ";

			$estadosFiltrar = trim( $_REQUEST['estados'],",");

			if ($estadosFiltrar)
			{
				$munSql.=" where estuf in (".str_replace("\\","", $estadosFiltrar).")";
			}

			$mesoFiltrar = trim( $_REQUEST['mesoregioes'],",");

			if ($mesoFiltrar)
			{

				if (substr_count($munSql,"where") == 0)
				{
					$munSql.=" where ";
				}
				else
				{
					$munSql.=" and ";
				}

				$munSql.=" mescod in (".str_replace("\\","", $mesoFiltrar).")";
			}

			$microFiltrar = trim( $_REQUEST['microregioes'],",");
				
			if ($microFiltrar)
			{
				if (substr_count($munSql,"where") == 0)
				{
					$munSql.=" where ";
				}
				else
				{
					$munSql.=" and ";
				}
				 
				$munSql.=" miccod in (".str_replace("\\","", $microFiltrar).")";
			}

			$munSql.= " order by mundescricao";

			$stSqlCarregados = "";

			mostrarComboPopup( 'Município', 'slMunicipio',  $munSql, $stSqlCarregados, 'Selecione os municípios', null,'',true );
		}

		
	if ($_POST['colunas'] ){
		/* configurações */
		ini_set("memory_limit", "16M");
		set_time_limit(0);
		/* FIM configurações */

		include("resultRelatorioIndicadores.inc");
		exit;
	}

	include APPRAIZ . 'includes/Agrupador.php';
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';

	monta_titulo( 'Relatório de Indicadores', '&nbsp;' );
	?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js">
</script>
<script type="text/javascript">

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

	
	function getMesoRegioes()
	{
		var mesoregioes = '';
		
		var elemento = document.getElementsByName('slMesoregiao[]')[0];	
		for (var i=0; i<elemento.options.length; i++){
			if (elemento.options[i].value != '')
			{
				mesoregioes += "'" + elemento.options[i].value + "',";
			}
		}
		
		return mesoregioes;
	}

	var popIndicadores = null;

	function enviaIndicadores()
	{
		var elementoOrigem   = document.getElementsByName('slIndicadores[]')[0];
		var elementoDestino  = popIndicadores.document.getElementsByName('slIndicadoresSelecionados')[0];

		var novoOption = null ;
		elementoDestino.options.length = 0;
		for (var i=0; i<elementoOrigem.options.length; i++){
			
			novoOption = document.createElement('option');
			novoOption.value = elementoOrigem.options[i].value;
			novoOption.text  = elementoOrigem.options[i].text;
			
			elementoDestino.add(novoOption, null);
		}
	}
	
	function ajaxMesoregiao(){
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=filtrarMesoregiao&estados="+getEstados(),
			success: function(retorno){
				jQuery('#idMesoregiao').html(retorno);
				
			}});
			
			ajaxMicroregiao();
			ajaxMunicipio();
	}
	
	function ajaxMicroregiao(){
	
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=filtrarMicroregiao&estados="+getEstados()+"&mesoregioes="+getMesoRegioes(),
			success: function(retorno){
				jQuery('#idMicroregiao').html(retorno);
			}});
			ajaxMunicipio();
	}
	
	function ajaxMunicipio(){
		var microregioes = '';
		var elemento = document.getElementsByName('slMicroRegiao[]')[0];	

		$("#slMicroRegiao option").each(function()
		{
		 if ( $(this).val() != '')
		 { 
			microregioes += "'" + $(this).val() + "',";
		 }
		});

		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=filtrarMunicipio&estados="+getEstados()+"&mesoregioes="+getMesoRegioes()+"&microregioes="+microregioes,
			success: function(retorno){
				jQuery('#idMunicipio').html(retorno);
			}});
	}
	
	function selecionaIndicadores(){
		popIndicadores = window.open('/painel/painel.php?modulo=relatorio/selecionaIndicadores&acao=A','Indicadores','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
		popIndicadores.addEventListener('load', enviaIndicadores, false);
	}

	function gerarRelatorio(tipo){

		var formulario = document.formulario;
		var tipo_relatorio = tipo;

		prepara_formulario();
					
		selectAllOptions( formulario.colunas );
		selectAllOptions( formulario.agrupador );
		selectAllOptions( formulario.slIndicadores );
		selectAllOptions( formulario.slEstado );
		$("#slMesoregiao option").attr('selected', 'selected');
		$("#slMicroregiao option").attr('selected', 'selected');
		$("#slMunicipio option").attr('selected', 'selected');


		
		
		if (tipo_relatorio == 'relatorio')
		{

 
				//formulario.action = 'painel.php?modulo=relatorio/relatorioIndicadores&acao=A';
		//	window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			//formulario.target = 'relatorio';
			

			document.getElementById('tipo_relatorio').value = 'visual';
			var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
			
		}

		else if(tipo_relatorio == 'visual'){

			if (formulario.elements['slIndicadores'][0] == null){
				alert('Selecione pelo menos um indicador!');
				return false;
			}

			if (formulario.elements['colunas'][0] == null){
				alert('Selecione pelo menos uma colunal!');
				return false;
			}
			
			if (document.getElementById('agruparunidades').value == '1')
			{
				if (formulario.elements['agrupador'][0] == null){
					alert('Selecione pelo menos um agrupador!');
					return false;
				}
			}
			
			document.getElementById('tipo_relatorio').value = 'visual';
			var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
		}
		else if ( tipo_relatorio == 'salvar' ){
			
			if ( formulario.titulo.value == '' ) {
				alert( 'É necessário informar a descrição do relatório!' );
				formulario.titulo.focus();
				return;
			}
			var nomesExistentes = new Array();
			<?php
				$sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela Where usucpf ='".$_SESSION['usucpf']."'and mnuid =".$_SESSION['mnuid'].";";
				$nomesExistentes = $db->carregar( $sqlNomesConsulta );
				if ( $nomesExistentes ){
					foreach ( $nomesExistentes as $linhaNome )
					{
						print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
					}
				}
			?>
			var confirma = true;
			var i, j = nomesExistentes.length;
			for ( i = 0; i < j; i++ ){
				if ( nomesExistentes[i] == formulario.titulo.value ){
					confirma = confirm( 'Deseja alterar a consulta já existente?' );
					break;
				}
			}
	
			if ( !confirma ){
				return;
			}
			formulario.action = 'painel.php?modulo=relatorio/relatorioIndicadores&acao=A&salvar=1';
			formulario.target = '_self';
			
		}
		else{
			
			if (formulario.elements['slIndicadores'][0] == null){
				alert('Selecione pelo menos um indicador!');
				return false;
			}

			if (formulario.elements['colunas'][0] == null){
				alert('Selecione pelo menos uma colunal!');
				return false;
			}
			
			if (document.getElementById('agruparunidades').value == '1')
			{
				if (formulario.elements['agrupador'][0] == null){
					alert('Selecione pelo menos um agrupador!');
					return false;
				}
			}

			document.getElementById('tipo_relatorio').value = 'xls';
			var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
		}
		formulario.submit();
		//janela.focus();
	}
	
	function carregarCombos()
	{
		ajaxMicroregiao();
		ajaxMunicipio();
	}

	function tornar_publico( prtid ){
		document.formulario.publico.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
		document.formulario.submit();
	}

	function excluir_relatorio( prtid ){
		document.formulario.excluir.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
		document.formulario.submit();
	}

	function carregar_consulta( prtid ){
		document.formulario.carregar.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
		document.formulario.submit();
	}

	function carregar_relatorio( prtid ){
		document.formulario.prtid.value = prtid;
		document.formulario.carregar.value = "1";
		gerarRelatorio( 'relatorio' );
	}


	function gerenciarAgrupadores()
	{
		$('#naoAgrupador').empty();
		$('#agrupador').empty();
		
		if ($('#agruparunidades').val() == '1')
		{
			$('#naoAgrupador').append('<option value=indnome>01. Indicador</option>');
			$('#naoAgrupador').append('<option value=paiid>02. Brasil</option>');
			$('#naoAgrupador').append('<option value=dshuf>03. Estado</option>');
			$('#naoAgrupador').append('<option value=mesdsc>04. Mesorregião</option>');
			$('#naoAgrupador').append('<option value=micdsc>05. Microrregião</option>');
			$('#naoAgrupador').append('<option value=mundescricao>06. Município</option>');
			$('#naoAgrupador').append('<option value=tiddsc>07. Detalhe 01</option>');
			$('#naoAgrupador').append('<option value=tiddsc2>08. Detalhe 01</option>');
		}
	}
	
	
	
	function verificarIndicadores(agrupar)
	{
		//if ($('#slIndicadores option').length > 1 )
		//{
			var indicadores = '';

			$('#naoColunas').empty();
			$('#colunas').empty();
			
			$("#slIndicadores option").each(function(){
				if ( $(this).val() != '')
				{ 
				 indicadores	+= $(this).val() + ",";
				}
			});
			
			
			if (agrupar)
				link = "requisicaoAjax=verificarIndicadores&indicadores="+indicadores;
			else
				link = "requisicaoAjax=verificarIndicadoresNaoAgrupando&indicadores="+indicadores;
			
			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: link,
				success: function(retorno){
						

						var ids   = retorno.split('|')[0];
						var descs = retorno.split('|')[1];

						var arrids = ids.split(',');
						var arrdescs = descs.split(',');
						
						var ct = $('#naoColunas option').length;
						var auxct = '';
						$.each(arrids, function(index, value) {
							ct+=1;
							if (ct < 10)
							{
								auxct = '0' + ct;
							}
							else
							{
								auxct = ct;
							}

							if ( value == 'dshvalor')
								$('#naoColunas').append('<option value="'+value+'" >'+auxct + '. '  +arrdescs[index]+'</option>');
							else
								$('#naoColunas').append('<option value="umeid:'+value+':'+arrdescs[index]+'" >'+auxct + '. '  +arrdescs[index]+'</option>');
						});
						
						if (!agrupar)
						{
							ct = ct + 1;
							if (ct < 10)
								auxct = '0' + ct;
							else
								auxct = ct;
							
							$('#naoColunas').append('<option value=dshuf >'+auxct+'. uf</option>');
							
							
							ct = ct + 1;
							if (ct < 10)
								auxct = '0' + ct;
							else
								auxct = ct;
							
							$('#naoColunas').append('<option value=mundescricao >'+auxct+'. Município</option>');
						}
						
					}
				});
		//}
	}


	function verificarDetahesIndicadores()
	{
		if ($('#slIndicadores option').length > 0 )
		{
			var indicadores = '';
			
			$("#slIndicadores option").each(function(){
				if ( $(this).val() != '')
				{ 
				 indicadores	+= $(this).val() + ",";
				}
			});

			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: "requisicaoAjax=verificarDetalhesIndicadores&indicadores="+indicadores,
				success: function(retorno){
						

						var ids   = retorno.split('|')[0];
						var descs = retorno.split('|')[1];
						
						var arrids = ids.split(',');
						var arrdescs = descs.split(',');
						
						var ct = $('#naoColunas option').length;
						$.each(arrids, function(index, value) {
							ct+=1;
							$('#naoColunas').append('<option value="umeid:'+value+':'+arrdescs[index]+'" >'+ct + '. '  +arrdescs[index]+'</option>');
						});
					}
				});
		}
	}
	

	$(document).ready(function() {
		carregarCombos();
	});
	
</script>
	<form name="formulario" id="formulario" action="" method="post">
		<input type="hidden" name="form" value="1" /> <input type="hidden"
			name="tipo_relatorio" id="tipo_relatorio" value="" /> <input
			type="hidden" name="publico" value="" />
		<input type="hidden" name="agruparunidades" id="agruparunidades" value="">
		<!-- indica se foi clicado para tornar o relatório público ou privado -->
		<input type="hidden" name="prtid" value="" />
		<!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
		<input type="hidden" name="carregar" value="" />
		<!-- indica se foi clicado para carregar o relatório -->
		<input type="hidden" name="excluir" value="" />
		<!-- indica se foi clicado para excluir o relatório já gravado -->

		<table id="tabela_filtros" class="tabela" align="center"
			bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr>
				<td class="SubTituloDireita">Definir título da consulta</td>
				<td><?= campo_texto( 'titulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo"'); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Definir observações para consulta</td>
				<td>
					<?= campo_textarea( 'obs_consulta', 'N', 'S', '', 69, 3, '', '', '', '', '', 'id="obs_consulta"'); ?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="10%">Indicadores</td>
				</td>
				<td class="subtituloesquerda" colspan="2"><select
					name="slIndicadores[]" multiple="multiple" id="slIndicadores"
					size="8" onclick="selecionaIndicadores();" style='width: 700px;'
					class='CampoEstilo'>
				</select>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="10%">Colunas</td>
				<td><?php				
				#Montar e exibe colunas
				$arrayColunas = colunasOrigem();
				$colunas = new Agrupador('formulario');
				$colunas->setOrigem('naoColunas', null, $arrayColunas);
				$colunas->setDestino('colunas', null);
				$colunas->exibir();
				?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="10%">Agrupadores</td>
				<td><?php
				#Montar e exibe agrupadores
				$arrayAgrupador = AgrupadorOrigem();
				$agrupador = new Agrupador('formulario');
				$agrupador->setOrigem('naoAgrupador', null, $arrayAgrupador);
				$agrupador->setDestino('agrupador', null);
				$agrupador->exibir();
				?>
				</td>
			</tr>
		</table>

		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td onclick="javascript:onOffBloco( 'outros' );">
				<img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
				Relatórios Gerenciais
				<input type="hidden" id="outros_flag" name="outros_flag" value="0" />
			</td>
		</tr>
	</table>	
	<div id="outros_div_filtros_off">
	</div>
	<div id="outros_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
				<tr>
					<td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
					<td>
					<?php
						$sql = sprintf("
									Select	Case
											  When prtpublico = true and usucpf = '%s' 
												Then '<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
												Else '<img border=\"0\" src=\"../imagens/usuario_bloqueado.gif\" title=\" Sem ação \">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir_01.gif\" title=\" Sem ação \">'  
											End as acao, 
											'<b>' || prtdsc || '</b>' as descricao,
											'<b>' || prtobservacao || '</b>' as observacao
									From public.parametros_tela 
									Where mnuid = %d and prtpublico = TRUE
								",
								$_SESSION['usucpf'],
								$_SESSION['mnuid'],
								$_SESSION['usucpf']);
						
						$cabecalho = array('Ação', 'Nome', 'Observações');
						$db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null );
					?>
					</td>
				</tr>
		</table>
	</div>
		
		<!-- MINHAS CONSULTAS -->

		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1"
			cellpadding="3" style="border-bottom: none; border-top: none;">
			<tr>
				<td onclick="javascript:onOffBloco( 'minhasconsultas' );">
					<!-- --> <img border="0" src="/imagens/mais.gif"
					id="minhasconsultas_img" />&nbsp; Minhas Consultas <input
					type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag"
					value="0" />
				</td>
			</tr>
		</table>
		<div id="minhasconsultas_div_filtros_off"></div>
		<div id="minhasconsultas_div_filtros_on" style="display: none;">
			<table class="tabela" align="center" bgcolor="#f5f5f5"
				cellspacing="1" cellpadding="3" style="border-top: none;">
				<tr>
					<td width="195" class="SubTituloDireita" valign="top">Consultas</td>
						<?php
							$sql = sprintf("
									Select Case 
											When prtpublico = false 
												Then '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
												Else '<img border=\"0\" src=\"../imagens/grupo_bloqueado.gif\" title=\" Sem ação \">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
											End as acao, 
											'<b>' || prtdsc || '</b>' as descricao,
											'<b>' || prtobservacao || '</b>' as observacao
									From public.parametros_tela 
									Where mnuid = %d AND usucpf = '%s'",
									$_SESSION['mnuid'],
									$_SESSION['usucpf']
							);
							$cabecalho = array('Ação', 'Nome', 'Observações');	
						?>
					<td>
						<?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, 'N', '95%', null ); ?>
					</td>
				</tr>
			</table>
		</div>
		<!-- FIM MINHAS CONSULTAS -->
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
				mostrarComboPopup( 'Estado', 'slEstado',  $ufSql, '', 'Selecione o(s) Estado(s)',null,'ajaxMesoregiao',false );
				?>
				</td>
			</tr>
			<tr>
				<td colspan="2"><?php 
				filtrarMesoregiao();
				?>
				</td>
			</tr>
			<tr>
				<td colspan="2">
					<table width="595" id="idMicroregiao">
					</table>
				</td>
			</tr>
			<tr>
				<td colspan="2">
					<table width="595" id="idMunicipio">
					</table>

				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">&nbsp;</td>
				<td align="center"><input type="button" name="Gerar Relatório"
					value="Gerar Relatório"
					onclick="javascript:gerarRelatorio('visual');" /> <input
					type="button" name="Gerar Relatório" value="Visualizar XLS"
					onclick="javascript:gerarRelatorio('xls');" /> <input type="button"
					value="Salvar Consulta"
					onclick="javascript:gerarRelatorio('salvar');"
					style="cursor: pointer;" />
				</td>
			</tr>
		</table>
	</form>
