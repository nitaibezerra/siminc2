<?php
function exibeResultado( $request ){
	
	global $db;
	
	include_once "relatorio_atuacoesProfissionaisResultado.inc";
}

function atualizaComboPOPUPMunicipio( $request ){
	
	global $db;
	
	extract($request);
	
	$where = Array();
	if( $estuf[0] != '' ){
		$where[] = "estuf in ('".implode("','",$estuf)."')";
	}
	
	$sql = "SELECT DISTINCT
				muncod as codigo,
				mundescricao||' - '||estuf as descricao
			FROM
				territorios.municipio
			".( ( $where[0] != '' ) ? "WHERE ".implode(" AND ",$where) : "" )."
			ORDER BY
				2";
	combo_popup( "muncod", $sql, "Município", "400x400", 0, array(), "", "S", false, false, 5, 400, $onpop = null, $onpush = null, $param_conexao = false, //15
				$where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, 'atualizaComboPOPUPEscola' );
}

function atualizaComboPOPUPEscola( $request ){

	global $db;
	
	extract($request);

	$where = Array();
	if( $estuf[0] != '' ){
		$where[] = "estufprofessor in ('".implode("','",$estuf)."')";
	}
	if( $muncod[0] != '' ){
		$where[] = "muncodprofessor in ('".implode("','",$muncod)."')";
	}
	if( $esfera[0] != '' ){
		$where[] = "esferaprofessor in ('".implode("','",$esfera)."')";
	}

	$sql = "SELECT DISTINCT
				atpinep as codigo, 
				atpdescricaoescola as descricao
			FROM 
				fiesabatimento.atuacaoprofissional
			".( ( $where != '' ) ? "WHERE ".implode(" AND ",$where) : "" )."
			ORDER BY 
				2";
	combo_popup( "atpinep", $sql, "Escola", "400x400", 0, array(), "", "S", false, false, 5, 400, $onpop = null, $onpush = null, $param_conexao = false, //15
				$where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, null );
}

if( $_POST['req'] ){
	$_POST['req']($_POST);
	die();
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório de Atuações Profissionais.";
monta_titulo( $titulo_modulo, '' );

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">

function atualizaComboPOPUPMunicipio( valor ){
	$('#loader-container').slideDown();
	$('#req').val('atualizaComboPOPUPMunicipio');
	selectAllOptions( document.getElementById( 'estuf' ) );
	selectAllOptions( document.getElementById( 'muncod' ) );
	$.ajax({
		type: "POST",
		url: window.location.href,
		data: "&"+$('#filtro').serialize(),
		async: false,
		success: function(msg){
			jQuery('#td_municipio').html(msg);
			$('#loader-container').slideUp();
		}
	});
}

function atualizaComboPOPUPEscola( valor ){
	$('#loader-container').slideDown();
	$('#req').val('atualizaComboPOPUPEscola');
	selectAllOptions( document.getElementById( 'estuf' ) );
	selectAllOptions( document.getElementById( 'muncod' ) );
	$.ajax({
		type: "POST",
		url: window.location.href,
		data: "&"+$('#filtro').serialize(),
		async: false,
		success: function(msg){
			jQuery('#td_escola').html(msg);
			$('#loader-container').slideUp();
		}
	});
}

$(document).ready(function()
{
	$('#loader-container').slideUp();

	$('.visualizar').click(function(){
		$('#loader-container').slideDown();
		//valida data
		var inicio 	= document.getElementById('sbadatasolicitacao_inicio');
		var fim 	= document.getElementById('sbadatasolicitacao_fim');
		if(inicio.value != '' && fim.value != ''){
			if(!validaData(inicio)){
				alert("Data Início Inválida.");
				inicio.focus();
				$('#loader-container').slideUp();
				return false;
			}		
			if(!validaData(fim)){
				alert("Data Fim Inválida.");
				fim.focus();
				$('#loader-container').slideUp();
				return false;
			}		
		}
		var inicio 	= document.getElementById('htddata_inicio');
		var fim 	= document.getElementById('htddata_fim');
		if(inicio.value != '' && fim.value != ''){
			if(!validaData(inicio)){
				alert("Data Início Inválida.");
				inicio.focus();
				$('#loader-container').slideUp();
				return false;
			}		
			if(!validaData(fim)){
				alert("Data Fim Inválida.");
				fim.focus();
				$('#loader-container').slideUp();
				return false;
			}		
		}
		
		if( $('#agrupador option').length < 1 ){
			alert("Selecione pelo menos um agrupador.");
			$('#loader-container').slideUp();
			return false;
		}
		
		selectAllOptions( document.getElementById( 'estuf' ) );
		selectAllOptions( document.getElementById( 'muncod' ) );
		selectAllOptions( document.getElementById( 'esdid' ) );
		selectAllOptions( document.getElementById( 'atpinep' ) );
		selectAllOptions( document.getElementById( 'agrupador' ) );
		
		$('#req').val('exibeResultado');
		$('#filtro').attr('target', 'resultado_solicitacoesAbatimento');
		var janela = window.open( '', 'resultado_solicitacoesAbatimento', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		$('#filtro').submit();
		janela.focus();
		$('#loader-container').slideUp();
	});
});

</script>
<div id="loader-container" style="text-align:center; position: absolute; background-color: white; opacity: .6; width:110%; height:2000%; margin-top: -200px; margin-left: -20px; Z-index:22;" >
	<div id="loader" style="margin-top:300px;">
		<img src="../imagens/wait.gif" border="0" align="middle">
		<span>Aguarde! Carregando Dados...</span>
	</div>
</div>
<form action="" method="post" name="filtro" id="filtro">
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" id="req" name="req" value=""/>	
	<input type="hidden" name="pesquisa" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
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
						'uf' => array(
							'codigo'    => 'estuf',
							'descricao' => 'UF'
						),
						'municipio' => array(
							'codigo'    => 'muncod',
							'descricao' => 'Município'
						),
						'estado' => array(
							'codigo'    => 'estado',
							'descricao' => 'Estado Atual da Atuação Profissional'
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
			<td colspan="2" bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')">
				<b>Filtros</b>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Unidades da Federação
			</td>
			<td>
				<?php
				$sql = "SELECT DISTINCT
							estuf as codigo,
							estdescricao as descricao
						FROM
							territorios.estado
						ORDER BY
							2";
				combo_popup( "estuf", $sql, "Estado", "400x400", 0, array(), "", "S", false, false, 5, 400, $onpop = null, $onpush = null, $param_conexao = false, //15
							$where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, 'atualizaComboPOPUPMunicipio(this);window.openner.atualizaComboPOPUPEscola' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Município
			</td>
			<td id="td_municipio">
				<?php
				$sql = "SELECT DISTINCT
							muncod as codigo,
							mundescricao||' - '||estuf as descricao
						FROM
							territorios.municipio
						ORDER BY
							2";
				combo_popup( "muncod", $sql, "Município", "400x400", 0, array(), "", "S", false, false, 5, 400, $onpop = null, $onpush = null, $param_conexao = false, //15
							$where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, 'atualizaComboPOPUPEscola' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Esfera da Escola
			</td>
			<td>
				<input type="checkbox" name="esfera[]" value="F" onclick="atualizaComboPOPUPEscola( '' )"/> Federal <br>
				<input type="checkbox" name="esfera[]" value="E" onclick="atualizaComboPOPUPEscola( '' )"/> Estadual <br>
				<input type="checkbox" name="esfera[]" value="M" onclick="atualizaComboPOPUPEscola( '' )"/> Municipal
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Escola
			</td>
			<td id="td_escola">
				<?php
				$sql = "SELECT DISTINCT
							atpinep as codigo, 
							atpdescricaoescola as descricao
						FROM 
							fiesabatimento.atuacaoprofissional
						ORDER BY 
							2";
				combo_popup( "atpinep", $sql, "Escola", "400x400", 0, array(), "", "S", false, false, 5, 400, $onpop = null, $onpush = null, $param_conexao = false, //15
							$where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, null );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Estado da Atuação Profissional
			</td>
			<td>
				<?php
				
				// relatorio_atuacoesProfissionais
				$sql = "SELECT 
							esdid as codigo,
							esddsc as descricao 
						FROM 
							workflow.estadodocumento 
						WHERE 
							tpdid = ".TPDID_ANALISE_SITUACAO."
						ORDER BY 
							2";
				combo_popup( "esdid", $sql, "Estado da Atuação Profissional", "400x400", 0, array(), "", "S", false, false, 5, 400, $onpop = null, $onpush = null, $param_conexao = false, //15
							$where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, null );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Período de Solicitação:</td>
			<td>
				De:
				&nbsp;&nbsp;
				<?= campo_data2( 'sbadatasolicitacao_inicio', 'N', 'S', '', 'S' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data2( 'sbadatasolicitacao_fim', 'N', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Período do Ultimo Trâmite:</td>
			<td>
				De:
				&nbsp;&nbsp;
				<?= campo_data2( 'htddata_inicio', 'N', 'S', '', 'S' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data2( 'htddata_fim', 'N', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" style="cursor: pointer;" class="visualizar" />
			</td>
		</tr>
	</table>
</form>