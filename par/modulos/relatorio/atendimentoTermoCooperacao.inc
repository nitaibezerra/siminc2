<?php

ini_set("memory_limit","-1");
set_time_limit(0);

//verifica_sessao();

if('1' == $_REQUEST['excluirconsulta']){
	$sql = "DELETE FROM public.parametros_tela WHERE prtid='".$_REQUEST['prtid']."'";
	$db->executar($sql);
	$db->commit();
	exit;
}
if('1' == $_REQUEST['tornarconsultapublica']){
	$sql = sprintf(
		"UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
		$_REQUEST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	exit;
}
if('1' == $_REQUEST['listarconsultas']){
	$sql = sprintf(
	"SELECT CASE WHEN prtpublico = false THEN '".(($db->testa_superuser())?"<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;":"")."<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' END AS acao, prtdsc AS descricao FROM public.parametros_tela WHERE mnuid = %d AND usucpf = '%s'",
	$_SESSION['mnuid'],
	$_SESSION['usucpf']);

	$db->monta_lista_simples( $sql, null, 50, 50, null, null, null );	
	exit;
}
if('1' == $_REQUEST['gravarconsulta']){
	unset($_REQUEST['modulo'], 
	$_REQUEST['acao'], 
	$_REQUEST['grfid'], 
	$_REQUEST['theme_simec'], 
	$_REQUEST['PHPSESSID']);
		  
	$dados = serialize($_REQUEST);
	
	$sql = "INSERT INTO public.parametros_tela(
            prtdsc, prtobj, prtpublico, prtdata, usucpf, mnuid)
    		VALUES ('".$_REQUEST['prtdsc']."', '".$dados."', FALSE, NOW(), '".$_SESSION['usucpf']."', '".$_SESSION['mnuid']."');";
	
	$db->executar($sql);

//	echo"<scrip type='text/javascript'>
//			alert('Consulta gravada com sucesso');
//		 </script>";
	
	$db->commit();
	exit;
}
if ('3'==$_REQUEST['buscar'])
{
	$sql = sprintf(	"SELECT prtobj FROM public.parametros_tela WHERE prtid = ".$_REQUEST['prtid'] );
	$itens = $db->pegaUm( $sql );
	
	$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
	
	extract( $dados );
	$_REQUEST = $dados;
}
if ( isset( $_REQUEST['buscar'] ) )
{
	include "atendimentoTermoCooperacaoResultado.inc";
	exit();
}

$agrupadorHtml =
<<<EOF
	<table>
		<tr valign="middle">
			<td>
				<select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 160px; height: 120px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
			</td>
			<td>
				<img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
				<!--
				<img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
				<img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
				-->
				<img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
			</td>
			<td>
				<select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 160px; height: 120px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
			</td>
			<td>
				<img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
				<img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
			</td>
		</tr>
	</table>
	<script type="text/javascript" language="javascript">
		limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
		limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
		{POVOAR_ORIGEM}
		{POVOAR_DESTINO}
		sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
	</script>
EOF;

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Relatório de Atendimento do Termo de Cooperação", "" );
//monta_titulo_cte( $titulo_modulo );

?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript">

	function exibirRelatorio()
	{
		var formulario = document.filtro;
		
		// verifica se tem algum agrupador selecionado
		agrupador = document.getElementById( 'agrupador' );
		if ( !agrupador.options.length )
		{
			alert( 'Escolha ao menos um item para agrupar o resultado.' );
			return;
		}
		
		selectAllOptions( agrupador );
//		selectAllOptions( document.getElementById( 'analista' ) );
		selectAllOptions( document.getElementById( 'estuf' ) );
		selectAllOptions( document.getElementById( 'muncod' ) );
		selectAllOptions( document.getElementById( 'ideb' ) );
		selectAllOptions( document.getElementById( 'dimcod' ) );
		selectAllOptions( document.getElementById( 'sbaid' ) );
		selectAllOptions( document.getElementById( 'secretaria' ) );
		selectAllOptions( document.getElementById( 'undid' ) );
		selectAllOptions( document.getElementById( 'prgid' ) );
		selectAllOptions( document.getElementById( 'indicador' ) );
		selectAllOptions( document.getElementById( 'fonte' ) );
		
		// submete formulario
		formulario.target = 'resultadoCteGeral';
		var janela = window.open( '', 'resultadoCteGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.submit();
		janela.focus();
	}

	function salvarConsulta(){
		document.getElementById('buscar').value = 2;
		
		var prtdsc = document.getElementById('prtdsc');
		
		if ( !prtdsc.value )
		{
			alert( 'Para salvar a consulta o título é obrigatório' );
			return;
		}
		
		// verifica se tem algum agrupador selecionado
		agrupador = document.getElementById( 'agrupador' );
		if ( !agrupador.options.length )
		{
			alert( 'Escolha ao menos um item para agrupar o resultado.' );
			return;
		}		
		
		selectAllOptions( agrupador );
//		selectAllOptions( document.getElementById( 'analista' ) );
		selectAllOptions( document.getElementById( 'estuf' ) );
		selectAllOptions( document.getElementById( 'muncod' ) );
		selectAllOptions( document.getElementById( 'ideb' ) );
		selectAllOptions( document.getElementById( 'dimcod' ) );
		selectAllOptions( document.getElementById( 'sbaid' ) );
		selectAllOptions( document.getElementById( 'secretaria' ) );
		selectAllOptions( document.getElementById( 'undid' ) );
		selectAllOptions( document.getElementById( 'prgid' ) );
		selectAllOptions( document.getElementById( 'indicador' ) );
		selectAllOptions( document.getElementById( 'fonte' ) );		
		
		
		var serializado = $('#formFiltro').serialize();
		
		jQuery.post("cte.php?modulo=relatorio/monitoramento/atendimento_termo_cooperacao&acao=A&gravarconsulta=1&" + serializado);
		jQuery("#td_consultas").load('cte.php?modulo=relatorio/monitoramento/atendimento_termo_cooperacao&acao=A', { "listarconsultas": "1" } );
		alert('Consulta gravada com sucesso');
	}

	function excluir_relatorio(prtid) {
		var conf = confirm("Deseja realmente excluir?");
		
		if(conf) {
			jQuery.post("cte.php?modulo=relatorio/monitoramento/atendimento_termo_cooperacao&acao=A", { "excluirconsulta": "1", "prtid": prtid });
			jQuery("#td_consultas").load('cte.php?modulo=relatorio/monitoramento/atendimento_termo_cooperacao&acao=A', { "listarconsultas": "1" } );
		}
	}
	
	function tornar_publico(prtid) {
		jQuery.post("cte.php?modulo=relatorio/monitoramento/atendimento_termo_cooperacao&acao=A", { "tornarconsultapublica": "1", "prtid": prtid });
		jQuery("#td_consultas").load('cte.php?modulo=relatorio/monitoramento/atendimento_termo_cooperacao&acao=A', { "listarconsultas": "1" } );
		alert("Consulta publicada.");
	}
	
	
	function carregar_relatorio(prtid) {
	
		document.getElementById('prtid').value=prtid;
		document.getElementById('buscar').value='3';	

		// submete formulario
		var formulario = document.filtro;
		formulario.target = 'resultadoCteGeral';
		var janela = window.open( '', 'resultadoCteGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.submit();
		janela.focus();
	
	}	

</script>
<form action="" method="post" name="filtro" id="formFiltro">
	<input type="hidden" name="buscar" id="buscar" value="1"/>
	<input type="hidden" name="prtid" id="prtid" />
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" valign="top">
				Título
			</td>
			<td>
				<? echo campo_texto('prtdsc', "S", "S", "Descrição da consulta", 40, 150, "", "", '', '', 0, 'id="prtdsc"' ); ?>
			</td>
		</tr>
		<!-- AGRUPADOR ----------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Agrupadores
			</td>
			<td width="80%">
				<?php
					
					// inicia agrupador
					$agrupador = new Agrupador( 'filtro', $agrupadorHtml );
					$destino = array();
					$origem = array(
						'estado' => array(
							'codigo'    => 'estado',
							'descricao' => 'UF'
						),
						'municipio' => array(
							'codigo'    => 'muncod',
							'descricao' => 'Município'
						),
						'ideb' => array(
							'codigo'    => 'ideb',
							'descricao' => 'Classificação IDEB'
						),
						'dimensao' => array(
							'codigo'    => 'dimensao',
							'descricao' => 'Dimensão'
						),
						'subacao' => array(
							'codigo'    => 'subacao',
							'descricao' => 'Subação'
						),
						'secretaria' => array(
							'codigo'    => 'secretaria',
							'descricao' => 'Secretaria/Órgão'
						),
						'unidade' => array(
							'codigo'    => 'unidade',
							'descricao' => 'Unidade de Medida'
						),
						'programa' => array(
							'codigo'    => 'programa',
							'descricao' => 'Programa'
						),
						'indicador' => array(
							'codigo'    => 'indicador',
							'descricao' => 'Indicador Painel'
						),
						'fonte' => array(
							'codigo'    => 'fonte',
							'descricao' => 'Fonte'
						),
//						'analista' => array(
//							'codigo'    => 'analista',
//							'descricao' => 'Analista'
//						),
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupador', null, $origem );
					$agrupador->setDestino( 'agrupador', null, $destino );
					$agrupador->exibir();
					
				?>
			</td>
		</tr>
		
		
		<!-- ANALISTA -------------------------------------------------------- -->
		<!-- 
		<tr>
			<td class="SubTituloDireita" valign="top">
				Analista
			</td>
			<td>
				<?php
				$sqlComboAnalista = "
					select distinct \"ANALISTA\" as codigo, \"ANALISTA\" as descricao
					from cte.ctemonitoramentointerno 
					order by \"ANALISTA\"
				";
				combo_popup( "analista", $sqlComboAnalista, "Analista", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		 -->
		 
		<!-- ESTADO -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Unidades da Federação
			</td>
			<td>
				<?php
				$sqlComboEstado = "
					select distinct \"UF\" as codigo, \"UF\" as descricao
					from cte.ctemonitoramentointerno 
					order by \"UF\"
				";
				combo_popup( "estuf", $sqlComboEstado, "Estado", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- MUNICIPIO -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Municípios
			</td>
			<td>
				<?php
				$sqlComboMunicipio = "
					select distinct \"MUNICÍPIO\" as codigo,  \"UF\" || ' - ' || \"MUNICÍPIO\" as descricao
					from cte.ctemonitoramentointerno 
					order by descricao
				";
				//dbg($sqlComboMunicipio, 1);					
				combo_popup( "muncod", $sqlComboMunicipio, "Município", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		
		<!-- CLASSIFICAÇÃO IDEB -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Classificação IDEB
			</td>
			<td>
				<?php
				$sqlComboIdeb = "
					select distinct \"CLASSIFICAÇÃO IDEB\" as codigo, \"CLASSIFICAÇÃO IDEB\" as descricao
					from cte.ctemonitoramentointerno
					order by \"CLASSIFICAÇÃO IDEB\"
				";
				combo_popup( "ideb", $sqlComboIdeb, "Classificação IDEB", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>		
		
		<!-- DIMENSÃO -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Dimensão
			</td>
			<td>
				<?php
				$sqlComboDimensao = "
					select distinct \"DIMENSÃO\" as codigo, \"DIMENSÃO\" as descricao
					from cte.ctemonitoramentointerno
					order by \"DIMENSÃO\"
				";
				combo_popup( "dimcod", $sqlComboDimensao, "Dimensão", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>		
		
		<!-- SUBAÇÃO -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Subação
			</td>
			<td>
				<?php
				$sqlComboSubacao = "
					select distinct \"SUBAÇÃO\" as codigo, \"SUBAÇÃO\" || ' - ' || \"DESCRIÇÃO DA SUBAÇÃO\" as descricao
					from cte.ctemonitoramentointerno 
					order by codigo
				";
				combo_popup( "sbaid", $sqlComboSubacao, "Subação", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>		
		
		<!-- SECRETARIA/ÓRGÃO  -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Secretaria/Órgão
			</td>
			<td>
				<?php
				$sqlComboSecretaria = "
					select distinct \"SECRETARIA/ÓRGÃO\" as codigo, \"SECRETARIA/ÓRGÃO\" as descricao
					from cte.ctemonitoramentointerno 
					order by \"SECRETARIA/ÓRGÃO\"
				";
				combo_popup( "secretaria", $sqlComboSecretaria, "Secretaria/Órgão", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>		
		
		<!-- UNIDADE DE MEDIDA -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Unidade de Medida
			</td>
			<td>
				<?php
				$sqlComboUnidade = "
					select distinct \"F10\" as codigo, \"F10\" as descricao
					from cte.ctemonitoramentointerno 
					order by \"F10\"
				";
				combo_popup( "undid", $sqlComboUnidade, "Unidade de Medida", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>		
		
		<!-- PROGRAMA -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Programa
			</td>
			<td>
				<?php
				$sqlComboPrograma = "
					select distinct \"PROGRAMA\" as codigo, \"PROGRAMA\" as descricao
					from cte.ctemonitoramentointerno 
					order by \"PROGRAMA\"
				";
				combo_popup( "prgid", $sqlComboPrograma, "Programa", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>		
		
		<!-- INDICADOR PAINEL -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Indicador Painel
			</td>
			<td>
				<?php
				$sqlComboIndicador = "
					select distinct \"INDICADOR PAINEL\" as codigo, \"INDICADOR PAINEL\" as descricao
					from cte.ctemonitoramentointerno 
					order by \"INDICADOR PAINEL\"
				";
				combo_popup( "indicador", $sqlComboIndicador, "Indicador Painel", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>		
		
		<!-- FONTE -------------------------------------------------------- -->
		<tr>
			<td class="SubTituloDireita" valign="top">
				Fonte
			</td>
			<td>
				<?php
				$sqlComboFonte = "
					select distinct \"Fonte\" as codigo, \"Fonte\" as descricao
					from cte.ctemonitoramentointerno 
					order by \"Fonte\"
				";
				combo_popup( "fonte", $sqlComboFonte, "Fonte", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>		

		<tr>
			<td class='SubTituloEsquerda' colspan=2><img src="../imagens/menos.gif" onclick="if(document.getElementById('tr_consultas').style.display == ''){document.getElementById('tr_consultas').style.display = 'none'}else{document.getElementById('tr_consultas').style.display = ''}" style="cursor:pointer;"> Consultas</td>
		</tr>
		<tr id="tr_consultas">
			<td class="SubTituloDireita" valign="top">
				Consultas
			</td>
			<td id="td_consultas">
			<?php
				$sql = sprintf(
				"SELECT CASE WHEN prtpublico = false THEN '".(($db->testa_superuser())?"<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;":"")."<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' END AS acao, prtdsc AS descricao FROM public.parametros_tela WHERE mnuid = %d AND usucpf = '%s'",
				$_SESSION['mnuid'],
				$_SESSION['usucpf']);
			
				$db->monta_lista_simples( $sql, null, 50, 50, null, null, null );
			?>
			</td>			
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				&nbsp;
			</td>
			<td class="SubTituloDireita" style="text-align:left;">
				<input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();" />
				<input type="button" name="enviarform" value="Salvar consulta" onclick="salvarConsulta();">
			</td>
		</tr>
		
	</table>
</form>