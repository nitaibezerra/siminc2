<?php
if( $_REQUEST['reqAjax'] ){
	$_REQUEST['reqAjax']($_REQUEST);
	die();
}

//verifica perfis
$perfis 		= arrayPerfil();
$superPerfis 	= array(PERFIL_ADMINISTRADOR, PERFIL_SUPER_USUARIO, PERFIL_CONSULTA_GERAL);
$arIntersec 	= array_intersect($superPerfis, $perfis);

$arHspidPermitido = array();
if ( count($arIntersec) == 0 ){
	$sql = "SELECT 
				hspid 
			FROM 
				contratos.usuarioresponsabilidade 
			WHERE 
				usucpf='".$_SESSION['usucpf']."' AND
				hspid IS NOT NULL AND		 
				rpustatus='A'";
	
	$arHspidPermitido = $db->carregarColuna( $sql );
	$arHspidPermitido = ($arHspidPermitido ? $arHspidPermitido : array(0));
}
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
		location.href = 'contratos.php?modulo=relatorio/relatorio_empenho&acao=A';
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
			location.href = 'contratos.php?modulo=relatorio/relatorio_empenho&acao=A';
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

	switch($_REQUEST['tipoRelatorio']) {
		case 'html':
			include "relatorio_empenho_resultado.inc";
			exit;
		case 'xml':
			include "relatorio_empenho_resultadoxls.inc";
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
	unset( $_REQUEST['tipoRelatorio'] );
	$titulo = $_REQUEST['titulo'];
	
	$agrupador2 = array();
	
	if ( $_REQUEST['agrupador'] ){
		
		foreach ( $_REQUEST['agrupador'] as $valorAgrupador ){
			array_push( $agrupador2, array( 'codigo' => $valorAgrupador, 'descricao' => $valorAgrupador ));
		}
		
	}
	
}


if ( isset( $_REQUEST['tipoRelatorio'] ) ){
	include "relatorio_empenho_resultado.inc";
	
// 	switch($_REQUEST['tipoRelatorio']) {
// 		case '1':
// 			include "geral_resultado.inc";
// 			exit;
// 		case '2':
// 			include "geralxls_resultado.inc";
// 			exit;
// 	}
}

include APPRAIZ . 'includes/cabecalho.inc';
// include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
// $titulo_modulo = "Relatório Quantitativo";
// monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );
monta_titulo( $titulo_modulo, 'Selecione os filtros desejados' );


?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">
	jQuery.noConflict();

	function contratos_exibeRelatorioGeralXLS(){
		
		var formulario = document.filtro;
//		var agrupador  = document.getElementById( 'agrupador' );
		
		// Tipo de relatorio
		formulario.tipoRelatorio.value='xls';
		
//		selectAllOptions( formulario.agrupador );
//		selectAllOptions( document.getElementById( 'sitid' ) );	
		prepara_formulario();	
		
/*
		if ( !agrupador.options.length ){
			alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
			return false;
		}
*/
		formulario.submit();
	}
	
	function contratos_exibeRelatorioGeral(tipo){
		
		var formulario = document.filtro;
//		var agrupador  = document.getElementById( 'agrupador' );

		// Tipo de relatorio
		formulario.tipoRelatorio.value='html';

		switch(tipo) {
			case 'salvar':
//				selectAllOptions( formulario.agrupador );
				prepara_formulario();
				
				if ( formulario.titulo.value == '' ) {
					alert( 'É necessário informar o título do relatório!' );
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
				formulario.action = 'contratos.php?modulo=relatorio/relatorio_empenho&acao=A&salvar=1';
				formulario.target = '_self';

				formulario.submit();
			
				break;

			case 'exibir':

//				selectAllOptions( formulario.agrupador );
				prepara_formulario();
//				selectAllOptions( document.getElementById( 'sitid' ) );
/*				
				if ( !agrupador.options.length ){
					alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
					return false;
				}
*/
			case 'carregar':
				formulario.action = 'contratos.php?modulo=relatorio/relatorio_empenho&acao=A';
				window.open( '', 'relatorio', 'width=1100,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				formulario.target = 'relatorio';
				formulario.submit();
				break;

		}

		
	}
	
	
	function tornar_publico( prtid ){
//		location.href = '?modulo=<?//= $modulo ?>&acao=R&prtid='+ prtid + '&publico=1';
		document.filtro.publico.value = '1';
		document.filtro.prtid.value = prtid;
		document.filtro.target = '_self';
		document.filtro.submit();
	}
	
	function excluir_relatorio( prtid ){
		if ( confirm('Deseja excluir o relatório?') ){
			document.filtro.excluir.value = '1';
			document.filtro.prtid.value = prtid;
			document.filtro.target = '_self';
			document.filtro.submit();
		}
	}
	
	function carregar_consulta( prtid ){
		document.filtro.carregar.value = '1';
		document.filtro.prtid.value = prtid;
		document.filtro.target = '_self';
		document.filtro.submit();
	}
	
	function carregar_relatorio( prtid ){
		document.filtro.prtid.value = prtid;
		contratos_exibeRelatorioGeral( 'carregar' );
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
//-->
</script>
<form action="?modulo=relatorio/relatorio_empenho&acao=A" method="post" name="filtro" id="filtro"> 
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="tipoRelatorio" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" width="200">Título</td>
			<td>
				<?= campo_texto( 'titulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo"'); ?>
			</td>
		</tr>
<!-- 		
		<tr>
			<td class="SubTituloDireita">Agrupadores</td>
			<td>
				<?php

// 					// Início dos agrupadores
// 					$agrupador = new Agrupador('filtro','');
					
// 					// Dados padrão de destino (nulo)
// 					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
					
// 					// Dados padrão de origem
// 					$origem = array(
// 						'numcontrato' => array(
// 							'codigo'    => 'numcontrato',
// 							'descricao' => 'Nº do Contrato'
// 						),
// 						'contratante' => array(
// 							'codigo'    => 'contratante',
// 							'descricao' => 'Contratante'
// 						),
// 						'contratado' => array(
// 							'codigo'    => 'contratado',
// 							'descricao' => 'Contratado'
// 						)												
// 					);
					
// 					// exibe agrupador
// 					$agrupador->setOrigem( 'naoAgrupador', null, $origem );
// 					$agrupador->setDestino( 'agrupador', null, $destino );
// 					$agrupador->exibir();
				?>
			</td>
		</tr>
 -->		
		</table>
		
		<!-- OUTROS FILTROS -->
	
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
			<!--
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
				<tr>
					<td><span style="color:#a0a0a0;padding:0 30px;">nenhum filtro</span></td>
				</tr>
			</table>
			-->
		</div>
	
		<div id="outros_div_filtros_on" style="display:none;">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
					<tr>
						<td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
						<?php
						
							if( $db->testa_superuser() || possuiPerfil(PERFIL_SUPERVISORMEC) || 
								possuiPerfil(PERFIL_ADMINISTRADOR) ){
							 	$bt_publicar = "<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;";
							} 
						
							$sql = sprintf(
								"SELECT Case when prtpublico = true and usucpf = '%s' then '{$bt_publicar}<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' else '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' end as acao, '' || prtdsc || '' as descricao FROM public.parametros_tela WHERE mnuid = %d AND prtpublico = TRUE",
								$_SESSION['usucpf'],
								$_SESSION['mnuid'],
								$_SESSION['usucpf']
							);
							/*
							$sql = sprintf(
								"SELECT 'abc' as acao, prtdsc FROM public.parametros_tela WHERE mnuid = %d AND prtpublico = TRUE",
								$_SESSION['mnuid']
							);
							*/
							$cabecalho = array('Ação', 'Nome');
						?>
						<td><?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null ); ?>
						</td>
					</tr>
			</table>
		</div>
		<!-- FIM OUTROS FILTROS -->
		
		<!-- MINHAS CONSULTAS -->
		
		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td onclick="javascript:onOffBloco( 'minhasconsultas' );">
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
		
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" width="200">
				Objeto:
			</td>
			<td>
				<?=campo_texto( 'filtro_ctrobj', 'N', 'S', '', 40, 200, '', '' ); ?>
			</td>
		</tr>						
		<tr>
			<td class="SubTituloDireita">
				Número:
			</td>
			<td>
				<?=campo_texto( 'filtro_ctrnum', 'N', 'S', '', 4, 10, '', '' ); ?>
			</td>
		</tr>						
		<tr>
			<td class="SubTituloDireita">
				Ano:
			</td>
			<td>
				<?=campo_texto( 'filtro_ctrano', 'N', 'S', '', 4, 4, '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Número Modalidade:
			</td>
			<td>
				<?=campo_texto( 'filtro_ctrnummod', 'N', 'S', '', 10, 200, '', '' ); ?>
			</td>
		</tr>						
		<tr>
			<td class="SubTituloDireita">
				Processo de Contratação:
			</td>
			<td>
				<?=campo_texto( 'filtro_ctrproccontr', 'N', 'S', '', 40, 50, '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Processo de Execução:
			</td>
			<td>
				<?=campo_texto( 'filtro_ctrprocexecctr', 'N', 'S', '', 40, 50, '', '' ); ?>
			</td>
		</tr>						
		<tr>
			<td class="SubTituloDireita">
				Processo de Execução Financeira:
			</td>
			<td>
				<?=campo_texto( 'filtro_ctrprocexecfin', 'N', 'S', '', 40, 50, '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Data de Início da Vigência:
			</td>
			<td>
				<?php 
					$dtinicio = $_POST['dtinicio']; 
					$dtfim 	  = $_POST['dtfim'];
					echo campo_data2("dtinicio","N","S","Data de Início","N") 
				?> 
				até 
				<?php 
					echo campo_data2("dtfim","N","S","Data de Término","N") 
				?>
			</td>
		</tr>						
		<tr>
			<td class="SubTituloDireita">
				Data de Término da Vigência:
			</td>
			<td>
			<?php 
				$dtinicio2 = $_POST['dtinicio2']; 
				echo campo_data2("dtinicio2","N","S","Data de Início","N") 
			?> 
			até 
			<?php 
				$dtfim2 = $_POST['dtfim2']; 
				echo campo_data2("dtfim2","N","S","Data de Término","N") 
			?>			
			</td>
		</tr>						
			<?php
				// Contratante
            	$stSql = "SELECT 
								estid AS codigo, 
								estareasigla || ' - ' || estarea AS descricao 
							FROM 
								corporativo.estrutura 
							WHERE 
								estid in (3,4,5,6,7,8) AND
								eststatus = 'A' ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Interessado', 'estid',  $stSql, $stSqlCarregados, 'Selecione o(s) Interessado(s)' );
				
				// Contratante
				$stSql = "SELECT
								hspid AS codigo,
								CASE
									WHEN mun.muncod IS NULL THEN hspabrev || ' - ' || hspdsc
									ELSE hspabrev || ' - ' || hspdsc || ' - ' || mun.mundescricao || '/' || mun.estuf
								END AS descricao
							  FROM
								contratos.hospital h
							  LEFT JOIN
								territoriosgeo.municipio mun ON mun.muncod = h.muncod
							  WHERE
								" . ( count($arHspidPermitido) > 0 ? "h.hspid IN (" . implode(", ", $arHspidPermitido) . ") AND " : "" ) . "
								hspstatus = 'A'
							  ORDER BY
								hspabrev, hspdsc";
				$stSqlCarregados = "";
				$arWhere = array(
									array(
											"codigo" 	=> "h.hspabrev",
											"descricao" => "Sigla"
										 ),
									array(
											"codigo" 	=> "h.hspdsc",
											"descricao" => "Contratante"
										 )
								);
				mostrarComboPopup( 'Contratante', 'hspid',  $stSql, $stSqlCarregados, 'Selecione o(s) Contratante(s)', $arWhere);
				
				// Contratado
				$stSql = "SELECT
								DISTINCT
								entid AS codigo,
								entnome AS descricao
							  FROM
								contratos.ctcontrato c
							  JOIN
								entidade.entidade e ON e.entid = c.entidcontratada
							  WHERE
								" . ( count($arHspidPermitido) > 0 ? "c.hspid IN (" . implode(", ", $arHspidPermitido) . ") AND " : "" ) . "
								entstatus = 'A' 
							  ORDER BY
								entnome";
				$stSqlCarregados = "";
				$arWhere = array(
									array(
											"codigo" 	=> "e.entnome",
											"descricao" => "Contratado"
									)
							);
				mostrarComboPopup( 'Contratado', 'entid',  $stSql, $stSqlCarregados, 'Selecione o(s) Contratado(s)', $arWhere);
							
				// Modalidade
				$stSql = "SELECT
	                      	modid AS codigo,
	                        moddsc AS descricao
	                      FROM
	                        contratos.ctmodalidadecontrato
						  ORDER BY
							moddsc ASC";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Modalidade', 'modid',  $stSql, $stSqlCarregados, 'Selecione a(s) Modalidade(s)' );
				
				// Tipo contrato
				$stSql = "SELECT 
                            tpcid AS codigo, 
                            tpcdsc AS descricao
                          FROM
                            contratos.cttipocontrato
						  ORDER BY
							tpcdsc ASC";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Tipo contrato', 'tpcid',  $stSql, $stSqlCarregados, 'Selecione o(s) Tipo(s) de Contrato(s)' );
				
				// Vigência
				$stSql = " SELECT
								sitid AS codigo,
								sitdsc AS descricao
							FROM
								contratos.ctsituacaocontrato
							WHERE tpcstatus='A'
							ORDER BY
								sitdsc";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Vigência', 'sitid',  $stSql, $stSqlCarregados, 'Selecione a(s) Vigência(s)' );
			?>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" 	 onclick="contratos_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
				<input type="button" value="Visualizar XLS"  onclick="contratos_exibeRelatorioGeralXLS();" 		style="cursor: pointer;"/>
				<input type="button" value="Salvar Consulta" onclick="contratos_exibeRelatorioGeral('salvar');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
<script>

function mostraResponsavel( valor ){

	var tr   = document.getElementById('trResponsavel');
	var resp = document.getElementById('responsavel');
	
	if( valor == 'nao' ){
		resp.value = '';
		tr.style.display = 'none';
	}else{
		tr.style.display = 'table-row';
	}
	
}

</script>
