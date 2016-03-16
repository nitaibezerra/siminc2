<?php
// salva consulta
if ( $_REQUEST['salvar'] == 1) {

	$titulo = $_REQUEST['titulo'];
	$prtobj = serialize($_REQUEST);
	$prtpublico = "false";
	$usucpf = $_SESSION['usucpf'];
	$mnuid = $_SESSION['mnuid'];
	$sql = "insert into public.parametros_tela (prtdsc,prtobj,prtpublico,prtdata,usucpf,mnuid) values ('$titulo','$prtobj',$prtpublico,now(),'$usucpf',$mnuid);";
	$db->executar( $sql );
	$db->commit();
	?>
		<script type="text/javascript">
			alert('Consulta salva com sucesso!');
			location.href = 'conjur.php?modulo=relatorio/relatorio_historicoProcessos&acao=A';
		</script>
	<?
	die;
}
// FIM salva consulta

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
		location.href = 'conjur.php?modulo=relatorio/relatorio_historicoProcessos&acao=A';
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
			location.href = 'conjur.php?modulo=relatorio/relatorio_historicoProcessos&acao=A';
		</script>
	<?
	die;
}
// FIM remove consulta


// exibe consulta
if ( isset( $_REQUEST['form'] ) == true ){
	if ( $_REQUEST['prtid'] ){
		$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
		$itens = $db->pegaUm( $sql );
		$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
		$_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
		unset( $_REQUEST['salvar'] );
	}
	// exibe consulta
	if ( isset( $_REQUEST['form'] ) == true && !$_REQUEST['carregar']){
		switch($_REQUEST['pesquisa']) {
			case '1':
				include "relatorio_historicoProcessos_resultado.inc";
				exit;
			case '2':
				include "relatorio_historicoProcessos_resultado.inc";
				exit;
		}
		
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
	
	include "relatorio_historicoProcessos_resultado.inc";
	exit;
	
}

// exibe consulta
if ( isset( $_REQUEST['form'] ) == true && !$_REQUEST['carregar']){
	switch($_REQUEST['pesquisa']) {
		case '1':
			include "relatorio_historicoProcessos_resultado.inc";
			exit;
		case '2':
			include "relatorio_historicoProcessos_resultado.inc";
			exit;
	}
	
}

// remove flag de submissão de formulário
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] )
{
	$carregarAux = $_REQUEST['carregar'];
	unset( $_REQUEST['form'] );
}
// FIM remove flag de submissão de formulário


include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório Histórico de Processos";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/funcoes.js" ></script>
<script type="text/javascript">

$(document).ready(function()
{

});

function abrepopupAdvogados(){
    window.open('http://<?=$_SERVER['SERVER_NAME']?>/conjur/combo_advogados_bandalarga.php','Advogados','width=500,height=500,scrollbars=1');
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
	formulario.target = 'historicoProcessos';
	var janela = window.open( '?modulo=relatorio/relatorio_historicoProcessos_resultado&acao=A', 'historicoProcessos', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.submit();
	janela.focus();
}

function carregar_relatorio( prtid ){
	document.formulario.carregar.value = '1';
	document.formulario.prtid.value = prtid;
	formulario.target = 'historicoProcessos';
	var janela = window.open( '?modulo=relatorio/relatorio_historicoProcessos_resultado&acao=A', 'historicoProcessos', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.submit();
	janela.focus();
}

function salvarRelatorioProcesso()
{
	if ( formulario.titulo.value == '' ) {
		alert( 'É necessário informar a descrição do relatório!' );
		formulario.titulo.focus();
		return;
	}

	selectAllOptions( colunas );	
	selectAllOptions( agrupador );
	selectAllOptions( document.getElementById( 'ordenador' ) );
	
	
	selectAllOptions( document.getElementById( 'coonid' ) );
	selectAllOptions( document.getElementById( 'advid' ) );
	selectAllOptions( document.getElementById( 'aedid' ) );
	
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
	formulario.action = 'conjur.php?modulo=relatorio/relatorio_historicoProcessos&acao=A&salvar=1';
	formulario.target = '_self';
	formulario.submit();
}

function exibeRelatorioProcesso(tipoXLS){
	document.getElementById('req').value = tipoXLS;
	var formulario = document.formulario;
	var agrupador  = document.getElementById( 'agrupador' );
	var colunas  = document.getElementById( 'colunas' );

	if ( !colunas.options.length ){
		alert( 'Favor selecionar ao menos uma coluna!' );
		return false;
	}
	
	if ( !agrupador.options.length ){
		alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
		return false;
	}
	
	selectAllOptions( colunas );	
	selectAllOptions( agrupador );
	selectAllOptions( document.getElementById( 'ordenador' ) );
	
	
	selectAllOptions( document.getElementById( 'coonid' ) );
	selectAllOptions( document.getElementById( 'advid' ) );
	selectAllOptions( document.getElementById( 'aedid' ) );
	
	//valida data
	if(formulario.htddata_inicio.value != '' && formulario.htddata_fim.value != ''){

		if(!validaData(formulario.htddata_inicio)){
			alert("Data Início Inválida.");
			formulario.dtinicio.focus();
			return false;
		}		
		if(!validaData(formulario.htddata_fim)){
			alert("Data Fim Inválida.");
			formulario.dtinicio.focus();
			return false;
		}		

		if( !validaDataMaior(formulario.htddata_inicio, formulario.htddata_fim) ){
			alert("Data inicial maior que data final.");
			formulario.dtinicio.focus();
			return false;
		}
	}
	
	if(document.getElementById('req').value != 'geraxls'){
		formulario.target = 'historicoProcessos';
		var janela = window.open( '?modulo=relatorio/relatorio_historicoProcessos_resultado&acao=A', 'historicoProcessos', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.submit();
		janela.focus();
	}else{
		formulario.target = '';
		formulario.submit();
	}
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
</script>


<form action="" method="post" name="formulario" id="filtro">
	
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" id="req" name="req" value=""/>	
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
	
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita">Título:</td>
			<td><?php echo campo_texto("titulo","N","S","Título do Relatório",60,90,"","")?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Agrupadores:</td>
			<td>
				<?php

					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de origem
					$origem = array(
						'coodsc' => array(
							'codigo'    => 'coodsc',
							'descricao' => 'Unidade'
						),
						'entnome' => array(
							'codigo'    => 'entnome',
							'descricao' => 'Advogado'
						),
						'aeddscrealizar' => array(
							'codigo'    => 'aeddscrealizar',
							'descricao' => 'Ação'
						),
						'htddata' => array(
							'codigo'    => 'htddata',
							'descricao' => 'Data'
						),
						'prcdesc' => array(
							'codigo'    => 'prcdesc',
							'descricao' => 'Processo'
						)
						
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupador', null, $origem );
					$agrupador->setDestino( 'agrupador', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Colunas:</td>
			<td>
				<?php

					// Início das colunas
					$colunas = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = isset( $colunas2 ) ? $colunas2 : array();
					
					// Dados padrão de origem
					$origem = array(
						'descproc' => array(
							'codigo'    => 'descproc',
							'descricao' => 'Assunto'
						),
						'entnome' => array(
							'codigo'    => 'entnome',
							'descricao' => 'Advogado'
						),
						'aeddscrealizar' => array(
							'codigo'    => 'aeddscrealizar',
							'descricao' => 'Situação'
						),
						'htddata' => array(
							'codigo'    => 'htddata',
							'descricao' => 'Data'
						),
						'prcnomeinteressado' => array(
							'codigo'    => 'prcnomeinteressado',
							'descricao' => 'Interessado'
						),
						'unidadefisica' => array(
							'codigo'    => 'unidadefisica',
							'descricao' => 'Unidade Física'
						),
						'comentario' => array(
							'codigo'    => 'comentario',
							'descricao' => 'Comentário'
						),
						
					);
					
										
					// exibe coluna
					$colunas->setOrigem( 'naoColunas', null, $origem );
					$colunas->setDestino( 'colunas', null, $destino );
					$colunas->exibir();
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Ordenadores:</td>
			<td>
				<?php 
					// Início dos agrupadores
					$ordem = new Agrupador('filtro','');
					
					// Dados padrão de origem
					$origem = array(
						'htddata' => array(
							'codigo'    => 'data',
							'descricao' => 'Data'
						),
						'entnome' => array(
							'codigo'    => 'entnome',
							'descricao' => 'Advogado'
						),
						'coodsc' => array(
							'codigo'    => 'coodsc',
							'descricao' => 'Unidade'
						),
						'aeddscrealizar' => array(
							'codigo'    => 'aeddscrealizar',
							'descricao' => 'Ação'
						),
						'prcdesc' => array(
							'codigo'    => 'prcdesc',
							'descricao' => 'Processo'
						)
						
					);
					
					// exibe agrupador
					$ordem->setOrigem( 'naoOrdenador', null, $origem );
					$ordem->setDestino( 'ordenador', null, $destino );
					$ordem->exibir();
				?>
			</td>
		</tr>
		</table>
		
		<!-- OUTROS FILTROS -->
	
		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td onclick="javascript:onOffBloco( 'outros' );">
					<img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
					Relatórios Públicos
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
		<script language="javascript">	//alert( document.formulario.agrupador_combo.value );	</script>

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

    <!--Advogado-->
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
            <tr>
                <td bgcolor="#DCDCDC" align="right">Advogados</td>
                <td>
                    <select multiple="multiple" name="advid[]" onDblClick="abrepopupAdvogados();" id="advid" style="width:400px;" >
                            <option value="">Duplo clique para selecionar da lista</option>
                    </select>
                </td>
            </tr>
            <?
				// Coordenação
				$stSql = " SELECT
								coonid AS codigo,
								coodsc AS descricao
							FROM 
								conjur.coordenacao
							ORDER BY
								2 ";
				mostrarComboPopup( 'Unidade:', 'coonid',  $stSql, $stSqlCarregados, 'Selecione a(s) Unidade(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);
				
				// Situação Atual
				$stSql = "SELECT
							esdid as codigo,
							esddsc as descricao
						  FROM
							workflow.estadodocumento
						  WHERE
							tpdid = " . NOVO_TIPODOC . "
						  ORDER BY
							2";
				mostrarComboPopup( 'Situação Atual:', 'esdid',  $stSql, $stSqlCarregados, 'Selecione a(s) Situação(ões)' ,null,null,null,null,$arrVisivel,$arrOrdem);	
				
				// Situação - Ação
				$stSql = "SELECT
								aedid AS codigo,
								esddsc||' - '||aeddscrealizar AS descricao 
							FROM 
								workflow.estadodocumento doc
							INNER JOIN workflow.acaoestadodoc aed ON aed.esdidorigem = doc.esdid
							WHERE 
								tpdid = " . NOVO_TIPODOC . " AND
								esdstatus = 'A' AND
								aeddscrealizar != ''
							ORDER BY
								2";
				mostrarComboPopup( 'Situação - Ação:', 'aedid',  $stSql, $stSqlCarregados, 'Selecione a(s) Situação(ões)/Ação(ões)' ,null,null,null,null,$arrVisivel,$arrOrdem);				
			?>
		<tr>
			<td class="SubTituloDireita">Período de Tramitação:</td>
			<td>
				<?= campo_data2( 'htddata_inicio', 'N', 'S', '', 'S' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data2( 'htddata_fim', 'N', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Expandir Relatório:</td>
			<?
			$arExp = array(
							array("codigo" => 'true',
								  "descricao" => 'Sim'),
							array("codigo" => '',
								  "descricao" => 'Não')								
						  );
			?>
			<td><?=$db->monta_combo("expandir", $arExp, 'S','','', '', '',240,'N','expandir', '', '', 'Expandir'); ?></td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar"    onclick="exibeRelatorioProcesso('');" style="cursor: pointer;"/>
				<input type="button" value="VisualizarXLS" onclick="exibeRelatorioProcesso('geraxls');">
				<input type="button" value="Salvar Consulta" onclick="salvarRelatorioProcesso();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
