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
		location.href = '?modulo=relatorio/formRelatorioGerencial&acao=A';
	</script>
	<?php
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
			location.href = '?modulo=relatorio/formRelatorioGerencial&acao=A';
		</script>
	<?php
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
		$_POST = $dados;//array_merge( $_REQUEST, $dados );
		unset( $_POST['salvar'] );
	}
	include "resultRelatorioGerencial.inc";
	exit;

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

if ($_POST['agrupador']){
	ini_set("memory_limit","256M");
	include("resultRelatorioGerencial.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório Gerencial', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript"><!--

function gerarRelatorio(tipo){
	var formulario = document.formulario;
	var agrupador  = document.getElementById( 'agrupador' );

	prepara_formulario();
	if (formulario.elements['agrupador'][0] == null){
		alert('Selecione pelo menos uma agrupador!');
		return false;
	}
	if (formulario.elements['agrupador2'][0] == null){
		alert('Selecione pelo menos uma coluna!');
		return false;
	}
//	if ( (formulario.elements['estado'][0].value == '' || formulario.elements['estado'][3] != null) && (formulario.elements['municipio'][0].value == '' || formulario.elements['estado'][3] != null) ){
//		alert('Selecione pelo menos um ou no máximo 3(três) Estados ou pelo menos 1(um) Município!');
//		return false;
//	}
	selectAllOptions(formulario.agrupador);
	selectAllOptions(formulario.agrupador2);

	if ( tipo == 'relatorio' ){

			formulario.action = 'par.php?modulo=relatorio/formRelatorioGerencial&acao=A';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';

	} else {

		if ( tipo == 'salvar' ){

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
			formulario.action = 'par.php?modulo=relatorio/formRelatorioGerencial&acao=A&salvar=1';
			formulario.target = '_self';

		}else if( tipo == 'exibir' ){

			selectAllOptions( formulario.agrupador 		 );
			selectAllOptions( formulario.agrupador2		 );
			selectAllOptions( formulario.estado	   		 );
			selectAllOptions( formulario.municipio 		 );
			selectAllOptions( formulario.statussubacao 	 );
			selectAllOptions( formulario.statuspar 		 );
			selectAllOptions( formulario.planointerno 	 );
			selectAllOptions( formulario.ptres		 	 );
			selectAllOptions( formulario.forma 	   		 );
			selectAllOptions( formulario.dimensao  		 );
			selectAllOptions( formulario.tiposubacao  	 );
			selectAllOptions( formulario.grupo  		 );
			selectAllOptions( formulario.itemcomposicao  );

			document.getElementById('pesquisa').value = '1';

			var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
		//	ajaxRelatorio();
			janela.focus();

		}else if(tipo == 'exibirxls') {
			selectAllOptions( formulario.agrupador 		 );
			selectAllOptions( formulario.agrupador2		 );
			selectAllOptions( formulario.estado	   		 );
			selectAllOptions( formulario.municipio 		 );
			selectAllOptions( formulario.statussubacao 	 );
			selectAllOptions( formulario.statuspar 		 );
			selectAllOptions( formulario.planointerno 	 );
			selectAllOptions( formulario.ptres		 	 );
			selectAllOptions( formulario.forma 	   		 );
			selectAllOptions( formulario.dimensao  		 );
			selectAllOptions( formulario.tiposubacao  	 );
			selectAllOptions( formulario.grupo  		 );
			selectAllOptions( formulario.itemcomposicao  );

			document.getElementById('pesquisa').value = '2';

		}
	}
	formulario.submit();
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
	gerarRelatorio( 'relatorio' );
}

</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
<input type="hidden" name="form" value="1"/>
<input type="hidden" name="pesquisa" id="pesquisa" value="1"/>
<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
<input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita">Título</td>
		<td>
			<?= campo_texto( 'titulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo"'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Colunas</td>
		<td>
			<?
			$padrao2 = array(
							array('codigo' 	  => 'quantidadesubacao',
								  'descricao' => 'Quantidade Planejada'),
							array('codigo' 	  => 'valorsubacao',
								  'descricao' => 'Valor Planejado'),
							array('codigo' 	  => 'quantidadesubacaoaprov',
								  'descricao' => 'Quantidade Aprovada'),
							array('codigo' 	  => 'valorsubacaoaprov',
								  'descricao' => 'Valor Aprovado'),
							array('codigo' 	  => 'status',
								  'descricao' => 'Status da Subação')
						);
			$matriz2 = array(
							array('codigo' => 'muncod',
									'descricao' => 'Código IBGE'),
							array('codigo' => 'situacaoempenho',
									'descricao' => 'Situação do Empenho'),
							array('codigo' => 'valorempenho',
									'descricao' => 'Valor do Empenho'),
							array('codigo' => 'valorpagamento',
									'descricao' => 'Valor do Pagamento'),
							array('codigo' => 'pi',
									'descricao' => 'P.I.'),
							array('codigo' => 'ptres',
									'descricao' => 'PTRES'),
							array('codigo' => 'empnumeroprocesso',
									'descricao' => 'Número do Processo'),
							array('codigo' => 'numerotermo',
									'descricao' => 'Número do Termo de Compromisso')
						);
			$campoAgrupador2 = new Agrupador( 'formulario' );
			$campoAgrupador2->setOrigem( 'agrupadorOrigem2', null, $matriz2 );
			$campoAgrupador2->setDestino( 'agrupador2', null, $padrao2 );
			$campoAgrupador2->exibir();
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Agrupador</td>
		<td>
			<?
			$padrao = array(
							array('codigo' 	  => 'uf',
								  'descricao' => 'UF'),
							array('codigo' 	  => 'local',
								  'descricao' => 'Município'),
							array('codigo' 	  => 'frmdsc',
								  'descricao' => 'Forma de Execução'),
							array('codigo' 	  => 'subacao',
								  'descricao' => 'Subação'),
						);
			$matriz = agrupador();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupador', null, $padrao );
			$campoAgrupador->exibir();
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
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<!--<tr>
		<td class="SubTituloDireita" valign="top">Itens de Composição</td>
		<td><input type="radio" name="itens" id="itens" value="S"> Com itens <input type="radio" name="itens" id="itens" value="N"> Sem itens <input type="radio" name="itens" id="itens" value="T"> Todos</td>
	</tr>
	-->
	<tr>
		<td class="SubTituloDireita" valign="top">Anos da Subação</td>
		<td>
			<input type="checkbox" name="anos[]" value="2011">2011
			<input type="checkbox" name="anos[]" value="2012">2012
			<input type="checkbox" name="anos[]" value="2013">2013
			<input type="checkbox" name="anos[]" value="2014">2014
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Brasil Profissionalizado</td>
		<td>
			<input type="checkbox" name="bp[]" value="sim">
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">
			Esfera
		</td>
		<td>
			<?php
				$arr = array(
								array( 'codigo' => 't', 'descricao' => 'Todas' ),
								array( 'codigo' => 'm', 'descricao' => 'Municipal' ),
								array( 'codigo' => 'e', 'descricao' => 'Estadual' )
				);

				$esferafiltro = $_POST['esferafiltro'];
				$db->monta_combo( "esferafiltro", $arr, 'S', '', '', '' );
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">
			Documentos validados
		</td>
		<td>
			Sim: <input type="radio" value="s" id="dopusucpfvalidacaofnde" name="dopusucpfvalidacaofnde" />
			Não: <input type="radio" value="n" id="dopusucpfvalidacaofnde" name="dopusucpfvalidacaofnde" />
		</td>
	</tr>
	<?php

		// Filtros do relatório

		//Filtro dos Documentos
		$stSql = "SELECT
						mdoid as codigo,
						mdonome as descricao
					FROM
						par.modelosdocumentos
					WHERE
						mdostatus = 'A'";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Documentos', 'documentos',  $stSql, $stSqlCarregados, 'Selecione o Documento' );

		//Filtro do PI da Subação
		$stSql = "SELECT DISTINCT
						plinumplanointerno as codigo,
						plinumplanointerno as descricao
					FROM
					par.planointerno";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Plano Interno', 'planointerno',  $stSql, $stSqlCarregados, 'Selecione o Plano Interno' );

		//Filtro do PTRES da Subação
		$stSql = "SELECT DISTINCT
						ptres AS codigo,
						ptres AS descricao
					FROM
						financeiro.empenhopar
					ORDER BY
						ptres";
		$stSqlCarregados = "";
		mostrarComboPopup( 'PTRES', 'ptres',  $stSql, $stSqlCarregados, 'Selecione o PTRES' );

		//Filtro do Programa da Subação
		$stSql = "SELECT DISTINCT
						prgid AS codigo,
						prgdsc AS descricao
					FROM
						par.programa
					WHERE
						prgstatus = 'A'
					ORDER BY
						prgdsc";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Programa', 'programa',  $stSql, $stSqlCarregados, 'Selecione o Programa' );

		//Filtro do Status do PAR
		$stSql = "SELECT
						esdid AS codigo,
						esddsc AS descricao
					FROM
						workflow.estadodocumento
					WHERE
						esdid IN (".WF_DIAGNOSTICO.", ".WF_ELABORACAO.", ".WF_ANALISE.")";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Status do PAR', 'statuspar',  $stSql, $stSqlCarregados, 'Selecione o(s) Status do PAR' );

		//Filtro do Status da Subação
		$stSql = "SELECT
						esdid AS codigo,
						esddsc AS descricao
					FROM
						workflow.estadodocumento
					WHERE
						esdid IN (".WF_SUBACAO_ELABORACAO.", ".WF_SUBACAO_ANALISE.", ".WF_SUBACAO_DILIGENCIA.", ".WF_SUBACAO_AGUARDANDO_ANALISE_GESTOR.", ".WF_SUBACAO_EMPENHO.", ".WF_SUBACAO_PAGAMENTO.", ".WF_SUBACAO_INDEFERIDA.", ".WF_SUBACAO_ANALISE_COMISSAO.")";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Status da Subação', 'statussubacao',  $stSql, $stSqlCarregados, 'Selecione o(s) Status' );

		//Filtro de Estado
		$stSql = "SELECT
					estuf AS codigo,
					estdescricao AS descricao
				  FROM
				  	territorios.estado";
		$stSqlCarregados = "";
//		mostrarComboPopup( 'Estado<br><label style="color:red;font-size:9px;" > Selecione entre 1 (um) e 3 (três) estados<br> ou pelo menos 1(um) município</label>', 'estado',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s) (Entre 1 (um) e 3 (três) )' );
		mostrarComboPopup( 'Estado', 'estado',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)' );

		//Filtro de Estado
		$stSql = "SELECT
					muncod AS codigo,
					estuf || ' - ' || mundescricao AS descricao
				  FROM
				  	territorios.municipio";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Município', 'municipio',  $stSql, $stSqlCarregados, 'Selecione o(s) Municípios(s)' );


		//Filtro de Forma de Execução
		$stSql = "SELECT
				      frmid as codigo,
				      frmdsc as descricao
				FROM
					par.formaexecucao
				ORDER BY
				    frmdsc";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Forma de Execução', 'forma',  $stSql, $stSqlCarregados, 'Selecione a(s) Forma(s) de Execução' );

		//Filtro de Dimensão
		$stSql = "SELECT
				      dimcod as codigo,
				      dimcod || ' - ' || dimdsc as descricao
				FROM
					par.dimensao
				WHERE
				      dimstatus = 'A' AND
				      itrid = '".INSTRUMENTO_DIAGNOSTICO_ESTADUAL."'
				ORDER BY
				      dimcod";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Dimensão', 'dimensao',  $stSql, $stSqlCarregados, 'Selecione a(s) Dimensão(ões)' );

		//Filtro de Tipo de Subação
		$stSql = "SELECT
					ptsid as codigo,
					frmdsc || ' - ' || ptsdescricao as descricao
				FROM
					par.propostatiposubacao pts
				INNER JOIN par.formaexecucao frm ON frm.frmid = pts.frmid
				WHERE
					ptsstatus = 'A'
				ORDER BY
					descricao";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Tipo de Subação', 'tiposubacao',  $stSql, $stSqlCarregados, 'Selecione o(s) Tipo(s) de Subação(ões)');

		//Filtro de Subação
		$stSql = "SELECT
						ppsid as codigo,
						d.dimcod || '.' || a.arecod || '.' || i.indcod || '.' || pps.ppsordem || ' - ' || pps.ppsdsc as descricao
					FROM par.dimensao d
					INNER JOIN par.area a ON a.dimid = d.dimid
					INNER JOIN par.indicador i ON i.areid = a.areid
					INNER JOIN par.propostasubacao pps ON pps.indid = i.indid
					ORDER BY
						descricao";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Subação', 'subacao',  $stSql, $stSqlCarregados, 'Selecione a(s) Subação(ões)');

		//Filtro de Grupo de Municípios
//		$stSql = "select * from (
//					(select
//						tpmid as codigo,
//						case when tpmid = 140 then 'Municípios de até 10.000 habitantes' when tpmid = 141 then 'Municípios de 10.001 a 20.000 habitantes' else tpmdsc end as descricao
//					from
//						territorios.tipomunicipio
//					where
//						tpmid IN (1, 16, 17, 139, 140, 141, 150, 151, 152, 154, 162, 167)
//					)
//					union
//					(select
//						gtmid as codigo,
//						gtmdsc as descricao
//					from
//						territorios.grupotipomunicipio
//					where
//						gtmid = 5)
//					order by descricao) as tbl";
		$stSql = "SELECT
					tpmid AS codigo,
					tpmdsc AS descricao
				  FROM
				  	territorios.tipomunicipio mtq
				  WHERE
				  	tpmid IN (1, 16, 17, 139, 140, 141, 150, 151, 152, 154, 156, 162, 167)";
			$stSqlCarregados = "";

		mostrarComboPopup( 'Grupo de Municípios', 'grupo',  $stSql, $stSqlCarregados, 'Selecione o(s) Grupo(s) de Município(s)', $where);

		//Filtro de Itens de Composição
		$stSql = "SELECT
					picid as codigo,
					picdescricao as descricao
				FROM
					par.propostaitemcomposicao
				WHERE
					picstatus = 'A'
				ORDER BY
					picdescricao";
		$stSqlCarregados = "";
		mostrarComboPopup( 'Itens de Composição', 'itemcomposicao',  $stSql, $stSqlCarregados, 'Selecione o(s) Item(ns) de Composição');

	?>

	<tr>
		<td align="center" colspan="2">
			<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio('exibir');"/>
			<input type="button" name="Gerar Relatório" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio('exibirxls');"/>
			<input type="button" value="Salvar Consulta" onclick="javascript:gerarRelatorio('salvar');" 						style="cursor: pointer;"/>
		</td>
	</tr>
</table>
</form>
</body>
</html>
<?
function agrupador(){
	return array(
				array('codigo' 	  => 'frmdsc',
					  'descricao' => 'Forma de Execução'),
				array('codigo' 	  => 'ano',
					  'descricao' => 'Ano'),
				array('codigo' 	  => 'local',
					  'descricao' => 'Município'),
				array('codigo' 	  => 'uf',
					  'descricao' => 'UF'),
				array('codigo' 	  => 'dimensao',
					  'descricao' => 'Dimensão'),
				array('codigo' 	  => 'area',
					  'descricao' => 'Área'),
				array('codigo' 	  => 'indicador',
					  'descricao' => 'Indicador'),
				array('codigo' 	  => 'acao',
					  'descricao' => 'Ação'),
				array('codigo' 	  => 'subacao',
					  'descricao' => 'Subação'),
				array('codigo' 	  => 'icodescricao',
					  'descricao' => 'Itens de Composição'),
				array('codigo' 	  => 'status',
					  'descricao' => 'Status da Subação')
				);
}