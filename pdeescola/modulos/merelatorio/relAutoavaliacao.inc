<?php

// transforma consulta em pública
if ( $_REQUEST['prtid'] && $_REQUEST['publico'] ){
	$sql = sprintf(
		"UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
		$_REQUEST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	?>
	<script type="text/javascript">
		location.href = '?modulo=<?= $modulo ?>&acao=A';
	</script>
	<?
	die;
}
// FIM transforma consulta em pública

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
			//location.href = '?modulo=<?= $modulo ?>&acao=A';
			location.href = 'pdeescola.php?modulo=relatorio/relAutoavaliacao&acao=A';
		</script>
	<?
	die;
}
// FIM remove consulta

// remove flag de submissão de formulário
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] ){
	unset( $_REQUEST['form'] );
}
// FIM remove flag de submissão de formulário

// exibe consulta
if ( isset( $_REQUEST['form'] ) == true ){
	if ( $_REQUEST['prtid'] ){
		$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
		$itens = $db->pegaUm( $sql );
		$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
		$_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
		unset( $_REQUEST['salvar'] );
	}
	switch($_REQUEST['pesquisa']) {
		case '1':
			include "relAutoavaliacao_resultado.inc";
			exit;
		case '2':
			include "relAutoavaliacaoXls_resultado.inc";
			exit;
	}
	
}

// carrega consulta do banco
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] == 1 ){
	
	$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = ".$_REQUEST['prtid'] );
	$itens = $db->pegaUm( $sql );
	$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
	extract( $dados );
	$_REQUEST = $dados;
	unset( $_REQUEST['form'] );
	unset( $_REQUEST['pesquisa'] );
	$titulo = $_REQUEST['titulo'];
	
	$agrupador2 = array();
	
	if ( $_REQUEST['agrupador'] ){
		
		foreach ( $_REQUEST['agrupador'] as $valorAgrupador ){
			array_push( $agrupador2, array( 'codigo' => $valorAgrupador, 'descricao' => $valorAgrupador ));
		}
		
	}
	
}

if ( isset( $_REQUEST['pesquisa'] ) || isset( $_REQUEST['tipoRelatorio'] ) ){
	switch($_REQUEST['pesquisa']) {
		case '1':
			include "relAutoavaliacao_resultado.inc";
			exit;
		case '2':
			include "relAutoavaliacaoXls_resultado.inc";
			exit;
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório de Autoavaliação do PDE Escola";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<!-- CARREGANDO DADOS AJAX -->
<div id="loader-container" style="display:none;">
    <div id="loader">
    	<img src="../imagens/wait.gif" border="0" align="middle">
   		<span>Aguarde! Carregando Dados...</span>
	</div>
</div>

<form action="" method="post" name="filtro"> 
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		  
		<tr>
			<td class="SubTituloDireita">Título</td>
			<td>
				<?= campo_texto( 'titulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo"'); ?>
			</td>
		</tr>				
		<tr>
			<td class="SubTituloDireita">Agrupadores</td>
			<td>
				<?php

					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
					
					// Dados padrão de origem
					$origem = array(						
						'estuf' => array(
							'codigo'    => 'estuf',
							'descricao' => 'UF'
						),
						'municipio' => array(
							'codigo'    => 'municipio',
							'descricao' => 'Município'
						)												
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupador', null, $origem );
					$agrupador->setDestino( 'agrupador', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		</table>	
			
		<!-- MINHAS CONSULTAS -->		
		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td onclick="javascript:onOffBloco( 'minhasconsultas' );" >
					<!-- -->  
					<img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
					Minhas Consultas
					<input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />					
				</td>
			</tr>
		</table>		
		<div id="minhasconsultas_div_filtros_off">
		</div>
		<div id="minhasconsultas_div_filtros_on" style="display:none;">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
					<tr>
						<td width="195" class="SubTituloDireita" valign="top">Consultas</td>
						<?php
							
							$sql = sprintf(
								"SELECT 
									CASE WHEN prtpublico = false THEN '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
																	   <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
																	   <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
																 ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
																 	   <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
									END as acao, 
									'' || prtdsc || '' as descricao 
								 FROM 
								 	public.parametros_tela 
								 WHERE 
								 	mnuid = %d AND usucpf = '%s'",
								$_SESSION['mnuid'],
								$_SESSION['usucpf']
							);
							
							$cabecalho = array('Ação', 'Nome');
						
						?>
						<td>
							<?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, 'N', '80%', null ); ?>
						</td>
					</tr>
			</table>
		</div>		
		<!-- FIM MINHAS CONSULTAS -->
		
		<!-- MEUS FILTROS -->								
		<table class="tabela" style=" border-bottom:none;" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
						
				<?php
				
					$stSqlCarregados = "";
															
					/********************************************************************
					 * UF */
					$stSql = " SELECT
									estuf AS codigo,
									estdescricao AS descricao
								FROM 
									territorios.estado
								ORDER BY
									estdescricao ";
					
					mostrarComboPopup( 'UF', 'estuf',  $stSql, $stSqlCarregados, 'Selecione a(s) UF(s)' );
					
					/********************************************************************
					 * QUESTÕES */
					$stSql = " SELECT
									qapid AS codigo,
									qapdescricao AS descricao
								FROM 
									pdeescola.questaoavaliacaoplano
								WHERE qapidpai IS NOT NULL
								ORDER BY
									qapdescricao ";
					
					mostrarComboPopup( 'Questões', 'qapid',  $stSql, $stSqlCarregados, 'Selecione a(s) Questão(es)' );
																						
				?>				
				
		</table>
		
		<table class="tabela" style="border-top:none; border-bottom:none;" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" id="tabelaAgrupa">			
		</table>
			
		<table class="tabela" style="border-top:none;" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >			
			<tr>
				<td bgcolor="#CCCCCC" width="195"></td>
				<td bgcolor="#CCCCCC">
					<input type="button" value="Visualizar" onclick="pdeescola_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>					 
					<input type="button" value="Visualizar XLS" onclick="pdeescola_exibeRelatorioGeralXLS();" style="cursor: pointer;"/>					
					<input type="button" value="Salvar Consulta" onclick="pdeescola_exibeRelatorioGeral('salvar');" style="cursor: pointer;"/>
					
					<!-- input type="button" value="Teste" onclick="testeT()" style="cursor: pointer;"/ -->
										
				</td>
			</tr>
		</table>
</form>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">
	
	function testeT(){
		selectAllOptions( document.getElementById( 'estuf' ) );
		var estuf = document.getElementById('estuf');
		for(x=0; x<estuf.length; x++){
			alert(estuf.value);	
		}
		
	}

	function pdeescola_exibeRelatorioGeralXLS(){		
		
		var formulario = document.filtro;
		var agrupador  = document.getElementById( 'agrupador' );
		
		// Tipo de relatorio
		formulario.pesquisa.value='2';
		
		prepara_formulario();
		selectAllOptions( formulario.agrupador );
		
		if ( !agrupador.options.length ){
			alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
			agrupador.focus();
			return false;
		}			
		
		selectAllOptions( agrupador );				
		selectAllOptions( document.getElementById( 'estuf' ) ); // UF		
		
		formulario.submit();
		
	}
	
	function pdeescola_exibeRelatorioGeral(tipo){
		
		var formulario = document.filtro;
		var agrupador  = document.getElementById( 'agrupador' );

		// Tipo de relatorio
		formulario.pesquisa.value='1';
		
		prepara_formulario();
		selectAllOptions( formulario.agrupador );
		
		if ( tipo == 'relatorio' ){
			
			formulario.action = 'pdeescola.php?modulo=relatorio/relAutoavaliacao&acao=A';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
			
		}else {
		
			if ( tipo == 'planilha' ){
			
				if ( !agrupador.options.length ){
					alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
					agrupador.focus();
					return false;
				}			
				
				formulario.action = 'pdeescola.php?modulo=relatorio/relAutoavaliacao&acao=A&tipoRelatorio=xls';
				
			}else if ( tipo == 'salvar' ){
				
				if ( formulario.titulo.value == '' ) {
					alert( 'É necessário informar a descrição do relatório!' );
					formulario.titulo.focus();
					return;
				}
				var nomesExistentes = new Array();
				<?php
					$sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela";
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
				formulario.action = 'pdeescola.php?modulo=relatorio/relAutoavaliacao&acao=A&salvar=1';
				formulario.target = '_self';
		
			}else if( tipo == 'exibir' ){
			
				if ( !agrupador.options.length ){
					alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
					agrupador.focus();
					return false;
				}				
				
				selectAllOptions( agrupador );
				selectAllOptions( document.getElementById( 'estuf' ) ); // UF		
				
				formulario.target = 'resultadoFtGeral';
				var janela = window.open( '', 'resultadoFtGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				janela.focus();
			}
		}
		
		formulario.submit();
		
	}	
	
	function tornar_publico( prtid ){
		document.filtro.publico.value = '1';
		document.filtro.prtid.value = prtid;
		document.filtro.target = '_self';
		document.filtro.submit();
	}
	
	function excluir_relatorio( prtid ){				
		document.filtro.excluir.value = '1';
		document.filtro.prtid.value = prtid;
		document.filtro.target = '_self';
		document.filtro.submit();
	}
	
	function carregar_consulta( prtid ){
		document.filtro.carregar.value = '1';
		document.filtro.prtid.value = prtid;
		document.filtro.target = '_self';
		document.filtro.submit();
	}
	
	function carregar_relatorio( prtid ){
		document.filtro.prtid.value = prtid;
		pdeescola_exibeRelatorioGeral( 'relatorio' );
	}
	
	/* Função para substituir todos */
	function replaceAll(str, de, para){
	    var pos = str.indexOf(de);
	    while (pos > -1){
			str = str.replace(de, para);
			pos = str.indexOf(de);
		}
	    return (str);
	}
	/* Função para adicionar linha nas tabelas */

	/* CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
	function criarrequisicao() {
		return window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
	}
	/* FIM - CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
	
	/* FUNÇÃO QUE TRATA O RETORNO */
	var pegarretorno = function () {
		try {
				if ( evXmlHttp.readyState == 4 ) {
					if ( evXmlHttp.status == 200 && evXmlHttp.responseText != '' ) {
						// criando options
						var x = evXmlHttp.responseText.split("&&");
						for(i=1;i<(x.length-1);i++) {
							var dados = x[i].split("##");
							document.getElementById('usrs').options[i] = new Option(dados[1],dados[0]);
						}
						var dados = x[0].split("##");
						document.getElementById('usrs').options[0] = new Option(dados[1],dados[0]);
						document.getElementById('usrs').value = cpfselecionado;
					}
					if ( evXmlHttp.dispose ) {
						evXmlHttp.dispose();
					}
					evXmlHttp = null;
				}
			}
		catch(e) {}
	};
	/* FIM - FUNÇÃO QUE TRATA O RETORNO */
			
				
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
	
	/**
	 * Alterar visibilidade de um campo.	 
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
