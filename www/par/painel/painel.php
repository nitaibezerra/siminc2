<?php

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once "GraficoPAR.php";
include '_funcoes_painel_par.php';
include '../_funcoes.php';
include_once '../../obras2/_funcoes_obras_par.php';

$db = new cls_banco();

if( $_REQUEST['req'] ){
	ob_clean();
	$_REQUEST['req']();
	die();
}

$_POST['inuid'] = $_POST['inuid'] ? $_POST['inuid'] : null;

?>
<html>
<head>
<script language="javascript" type="text/javascript" src="../../library/jquery/jquery-1.11.1.min.js"></script>

<link rel='stylesheet' type='text/css' href='../../library/jquery/jquery-ui-1.10.3/themes/base/jquery-ui.css'/>
<link rel='stylesheet' type='text/css' href='../../library/jquery/jquery-ui-1.10.3/themes/bootstrap/jquery-ui-1.10.3.custom.min.css'/>

<link rel="stylesheet" href="../../library/bootstrap-file-upload-9.5.1/blueimp/css/blueimp-gallery.min.css">
<link rel="stylesheet" href="../../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload.css">
<link rel="stylesheet" href="../../library/bootstrap-file-upload-9.5.1/css/jquery.fileupload-ui.css">
<link rel="stylesheet" href="../../library/bootstrap-3.0.0/css/bootstrap.css">
<script src="../../library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>
        
<script src="../../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="../../library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="../../library/jquery/jquery.simple-color.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="../../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="../../library/jquery/jquery-isloading.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css' />
<link rel='stylesheet' type='text/css' href='css/cockpit.css' />
<script language="javascript" src="/includes/Highcharts-4.0.3/js/highcharts.js"></script>
<script language="javascript" src="/includes/Highcharts-4.0.3/js/modules/exporting.js"></script>
<script language="javascript" src="/estrutura/js/funcoes.js"></script>

<link rel='stylesheet' type='text/css' href='../../includes/loading.css'/>
<script src="../../library/jquery/jquery-isloading.min.js" type="text/javascript" charset="ISO-8895-1"></script>

<!-- begin loader -->
<div class="loading-dialog" id="loading">
   <div id="overlay" class="loading-dialog-content">
      <div class="ui-dialog-content">
         <img src="../../library/simec/img/loading.gif">
         <span>
             O sistema esta processando as informações. <br/>
             Por favor aguarde um momento...
          </span>
      </div>
  </div>
</div>
<!-- end loader -->

<style type="text/css">
.quadros {
	background-image: url('../../library/jquery/jquery-ui-1.10.3/themes/dark-hive/images/ui-bg_loop_25_000000_21x21.png');
}

.tdlista0{
	 border: 1px solid #333333; background-color: #AFAFAF; font-weight: bold; color: #333333;"
}

.tdlista1{
	 border: 1px solid #333333; background-color: #DBDBDB; font-weight: bold; color: #333333;"
}

.tdlistaTitulo{
	 border: 1px solid #333333; background-color: #FFFFFF; font-weight: bold; color: #333333; vertical-align: m"
}

.subtitulo {
	font-weight: bold;
}

#div-ciclos {
	height: 900px;
}

#div-qtd {
	height: 550px;
}

.fundo_titulo {
	background-image: url('fundo_enem.jpg');
	background-repeat: no-repeat;
	background-position: left;
	height: 150px;
	margin-left: 0;
	background-color: #FFFFFF;
}

;
.tabela_listagem {
	background-color: #FFFFFF;
	color: #000000;
}

.filtro_listagem {
	width: 70%;
}

.span_grupo {
	margin-right: 20px;
}

.mostraDetalhe{
	cursor: pointer;
}

.mostraDetalhe:hover{
	color: #FFFFCC;
	background-color: #444444;
}

.popup_convenios:hover{
	background-color: #444444;
	color: #FFFFCC;
}
.botao_tr:hover{
	background-color: #62abea;
}
.botao_tr{
	width: 100%;
	background-color: #428bca;
	color:white;
	margin-top: 1px;
	padding-top : 6px;
	padding-bottom : 6px;
	border-radius: 5px; 
	cursor:pointer;
	text-align: center;
	font-weight: bold;
	margin-top:3px;
}
</style>
<script>


function recuperaInuid( estuf, muncod ){

	var inuid;
	
	$.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "req=recuperaInuid&estuf="+estuf+"&muncod="+muncod,
   		async: false,
   		success: function( msg ){
	   		inuid = msg;
   		}
	});

	return inuid;
}

function atualizaTitulo( inuid ){
	
	$.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "req=atualizaTitulo&inuid="+inuid,
   		async: false,
   		success: function(msg){
   			$('#titulo').html(msg);
   		}
	});
}

function atualizarTabelas( inuid ){

	var esfera 	= $('#esfera').val();
	var muncod 	= $('#muncod').val();
	var anoprocesso	= $('#anoprocesso').val();

	if( esfera == 'M' && muncod == '' ){
		esfera = 'EM';
	}
	
	$.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "req=atualizarTabelas&inuid="+inuid+"&esfera="+esfera+'&anoprocesso='+anoprocesso+"&"+$('#formPesquisa').serialize(),
   		async: false,
   		dataType: 'json',
   		success: function( dados ){
   			$('#td_valor_pactuado').html(dados.valor_pactuado);
   			$('#td_valor_empenhado').html(dados.valor_empenhado);
   			$('#td_valor_repassado').html(dados.valor_repassado);
   			$('#td_valor_saldo_conta').html(dados.valor_saldo_conta);
   			
   			$('#qtd_financiadas_par').html(dados.qtd_financiadas_par);
   			$('#qtd_financiadas_pac').html(dados.qtd_financiadas_pac);
   			
   			$('#qtd_termos_par').html(dados.qtd_termos_par);
   			$('#qtd_termos_pac').html(dados.qtd_termos_pac);
   		}
	});
}

function atualizarFiltroGrafico( numero, campo, valor ){
	
	$.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "req=atualizarFiltroGrafico&numero="+numero+"&campo="+campo+"&valor="+valor+"&"+$('#formPesquisa').serialize(),
   		async: false,
   		success: function( msg ){
   			$('#filtro'+numero).val(msg);
   		}
	});
}

function atualizarGrafico1( inuid ){

	var esfera 	= $('[name="esfera"]').val();
	var estuf 	= $('[name="estuf"]').val();
	var muncod 	= $('[name="muncod"]').val();
	var anoprocesso	= $('[name="anoprocesso"]').val();

	if( esfera == 'M' && muncod == '' ){
		esfera = 'EM';
	}
	
	$.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "req=atualizarGrafico1&inuid="+inuid+"&esfera="+esfera+"&estuf="+estuf+"&muncod="+muncod+'&anoprocesso='+anoprocesso+$('#formPesquisa').serialize(),
   		async: false,
   		success: function(msg){
   			$('#div_grafico_1').html(msg);
   		}
	});
}

function atualizarGrafico2( inuid, filtro ){

	if( filtro == undefined ){
		filtro = '';
	}

	var esfera 	= $('[name="esfera"]').val();
	var estuf 	= $('[name="estuf"]').val();
	var muncod 	= $('[name="muncod"]').val();
	var anoprocesso	= $('[name="anoprocesso"]').val();

	if( esfera == 'M' && muncod == '' ){
		esfera = 'EM';
	}
		
	$.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "req=atualizarGrafico2&inuid="+inuid+"&filtro="+filtro+"&esfera="+esfera+"&estuf="+estuf+"&muncod="+muncod+'&anoprocesso='+anoprocesso+$('#formPesquisa').serialize(),
   		async: false,
   		success: function(msg){
   			$('#div_grafico_2').html(msg);
   		}
	});
}

$(document).ready(function(){

	$(document).on('click','.obra',function(){

		var obrid = $(this).attr('obrid');

		window.open('http://<?=$_SERVER['HTTP_HOST'] ?>/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid='+obrid, 'minuta', 'scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
	});

	$(document).on('click','.preobra',function(){

		var preid = $(this).attr('preid');
		var tooid = $(this).attr('tooid');

		if( tooid == '1' ){
			window.open('http://<?=$_SERVER['HTTP_HOST'] ?>/par/par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid='+preid, 'minuta', 'scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
		}else{
			window.open('http://<?=$_SERVER['HTTP_HOST'] ?>/par/par.php?modulo=principal/subacaoObras&acao=A&preid='+preid, 'minuta', 'scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
		}
	});

	$(document).on('click','.termo',function(){

		var dopid 	= $(this).attr('dopid');

		window.open('http://<?=$_SERVER['HTTP_HOST'] ?>/par/par.php?modulo=principal/documentoParObras&acao=A&req=formVizualizaDocumento&dopid=' + dopid, 'minuta', 'scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
	});

	$(document).on('click','.termo_pac',function(){

		var terid 	= $(this).attr('terid');

		window.open('http://<?=$_SERVER['HTTP_HOST'] ?>/par/par.php?modulo=principal/gerarTermoObra&acao=A&requisicao=download&terid='+terid, 
	        	'modelo', 
				"height=600,width=400,scrollbars=yes,top=0,left=0" );
	});

	$(document).on('click','.saldoprocesso',function(){

		var processo = $(this).attr('processo');

		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=graficoSaldoProcesso&buscasaldoprocesso="+processo,
	   		async: false,
	   		success: function(msg){
	   			$( "#dialog" ).attr('title','Saldo do Processo');
	   			$( "#dialog" ).html(msg);
				$( "#dialog" ).dialog({
					resizable: true,
					height:720,
					width:1280,
					modal: true,
					buttons: {
						"Fechar": function() {
							$( this ).dialog( "close" );
						}
					}
				});
	   		}
		});
	});

	$(document).on('click','.restricoes',function(){

		var obrid = $(this).attr('obrid');

		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=listaRestricoes&obrid="+obrid,
	   		async: false,
	   		success: function(msg){
	   			$( "#dialog" ).attr('title','Restrições');
	   			$( "#dialog" ).html(msg);
				$( "#dialog" ).dialog({
					resizable: true,
					height:720,
					width:1280,
					modal: true,
					buttons: {
						"Fechar": function() {
							$( this ).dialog( "close" );
						}
					}
				});
	   		}
		});
	});

	$(document).on('click','.mostraDetalhe',function(){

		var inuid 	= $('#inuid').val();
		var tipo 	= $(this).attr('funcao');
		var esfera 	= $('#esfera').val();
		var muncod 	= $('#muncod').val();
		var anoprocesso	= $('#anoprocesso').val();
		var detalhe_programa_nome 	= $('#detalhe_programa_nome').val();

		if( esfera == 'M' && muncod == '' ){
			esfera = 'EM';
		}

		$('.titulo_detalhe').css('color', 'white');
		$('.titulo_detalhe').css('font-weight', 'normal');
		$('.mostraDetalhe').css('color', 'white');
		$('.mostraDetalhe').css('font-weight', 'normal');
		$('.titulo_'+tipo).css('color', '#3276B1');
		$('.titulo_'+tipo).css('font-weight', 'bold');
		$(this).css('color', '#3276B1');
		$(this).css('font-weight', 'bold');

		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=atualizaDetalhe&inuid="+inuid+"&tipo="+tipo+"&esfera="+esfera+"&detalhe_programa_nome="+detalhe_programa_nome+'&anoprocesso='+anoprocesso,
	   		async: false,
	   		success: function(msg){
	   			$('#td_detalhe').html(msg);
	   		}
		});
	});

	$(document).on('click','.excell',function(){

		var inuid 	= $('#inuid').val();
		var tipo 	= $(this).attr('funcao');

		window.open(window.location.href+"?req=excell&inuid="+inuid+"&tipo="+tipo,"Exportar XLS","height=40,width=40,scrollbars=yes,top=50,left=200");
	});

	$('#voltar').click(function(){

		window.location.href = 'http://<?=$_SERVER['HTTP_HOST'] ?>/par/par.php?modulo=inicio&acao=C';
	});

	$('.popup_convenios').click(function(){

		var estuf 	= $('#estuf').val();
		var anoprocesso	= $('#anoprocesso').val();

		if( estuf == '' ){
			estuf = 'DF';
		}
		window.open('http://<?=$_SERVER['HTTP_HOST'] ?>/par/par.php?modulo=relatorio/painel/visualizaConvenio&acao=A&estuf='+estuf+'&anoprocesso='+anoprocesso, 
	        	'modelo', 
				"height=400,width=800,scrollbars=yes,top=0,left=0" );
	});

	$('#buscar').click(function(){

		var prgid 	= $('#prgid').val();
		var esfera 	= $('#esfera').val();
		var estuf 	= $('#estuf').val();
		var anoprocesso	= $('#anoprocesso').val();
		if( estuf == '' && esfera != 'N' ){
			alert('Favor escolher o estado.');
			return false;
		}
		
		var muncod 	= $('#muncod').val();
		if( muncod == undefined ){
			muncod = '';
		}

		var inuid 	= recuperaInuid( estuf, muncod );

		$('#inuid').val( inuid );

		$('#td_detalhe').html('');
		$('.titulo_detalhe').css('color', 'white');
		$('.titulo_detalhe').css('font-weight', 'normal');
		$('.mostraDetalhe').css('color', 'white');
		$('.mostraDetalhe').css('font-weight', 'normal');

		atualizaTitulo( inuid );
		atualizarTabelas( inuid );
		atualizarGrafico1( inuid );
		atualizarGrafico2( inuid, '' );
		if( esfera == 'N' ){
			$('#div_grafico_3').show();
		}else{
			$('#div_grafico_3').hide();
		}
	});

	$('#limpar_dados').click(function(){

		$('#prgid').val('');
		$('#esfera').val('E');
		$('#estuf').val('');
		$('#anoprocesso').val('');
		$('#tr_municipio').hide();
		$('#td_municipio').html('');
		$('#inuid').val('');
	});
	
	$('#esfera').change(function(){

		var esfera = $('#esfera').val();

		$('#estuf').val('');
		$('#tb_convenio').hide();
		$('#tr_estado').hide();
		$('#tr_municipio').hide();
		$('#tr_processo').hide();
		$('#tr_termo').hide();

		if( esfera == 'M' ){
			
			$('#tr_estado').show();
			$('#tr_municipio').show();
			$('#tr_processo').show();
			$('#tr_termo').show();
			$.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "req=atualizaMunicipios",
		   		async: false,
		   		success: function(msg){
		   			$( "#td_municipio" ).html(msg);
		   		}
			});

		}else if( esfera == 'E' ){
			$('#tr_estado').show();
			$('#tr_processo').show();
			$('#tr_termo').show();
			$('#tb_convenio').show();
		}
	});
	
	$('#estuf').change(function(){

		var esfera = $('#esfera').val();

		if( esfera == 'M' ){

			var estuf = $('#estuf').val();
			
			$.ajax({
		   		type: "POST",
		   		url: window.location.href,
		   		data: "req=atualizaMunicipios&estuf="+estuf,
		   		async: false,
		   		success: function(msg){
		   			$( "#td_municipio" ).html(msg);
		   		}
			});
		}
	});

	atualizaTitulo( '' );
	atualizarTabelas( '' );
});	

function registrarSessaoPainel(idArea) {
	var detalhe = $('[name="detalhe_programa_nome"]').val();
	if( detalhe == idArea ){
		$('[name="detalhe_programa_nome"]').val('');
	} else {
    	$('[name="detalhe_programa_nome"]').val(idArea);
	}
}; 
</script>
</head>
<body style="background-image: url('fundo1.jpg'); background-repeat: repeat;">
	<div id="dialog"></div>
	<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
		<tr>
			<td class="titulo_pagina fundo_titulo" style="background-repeat:repeat-x;">
				<div>
					<img style="float: left" src="../../imagens/icones/icons/control.png" style="vertical-align:middle;" />
					<div style="float: left" class="titulo_box">
						SIMEC<br />
						<span class="subtitulo_box">Monitoramento Estratégico PAR</span>
					</div>
				</div>
			</td>
		</tr>
	</table>
	<table border="0" align="center" width="99%" cellspacing="4" cellpadding="5" class="tabela_painel">
		<tr>
			<td style="background-color: #1d1b1b; width: 190px;">
				<!-- FILTROS -->
				<form method="post" name="formPesquisa" id="formPesquisa" >
					<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
						<tr>
							<td>
								<div style="text-align: left !important;">
									<input type="button" id="voltar" value="Voltar" /> 
								</div>
							</td>
						</tr>
						<tr>
							<td>
								Esfera <br />
								<?php
								$sql = Array(
											0 => array('codigo' => 'N', 'descricao' => 'Nacional'),
											1 => Array('codigo' => 'E', 'descricao' => 'Estadual'), 
											2 => Array('codigo' => 'M', 'descricao' => 'Municipal') 
										);
								$db->monta_combo( "esfera", $sql, 'S', '', '', '','',168, '', 'esfera' );
								?>
							</td>
						</tr>
						<tr id="tr_estado" style="display: none">
							<td>
								Estado <br />
								<?php
								$sql = "SELECT e.estuf as codigo, e.estdescricao as descricao FROM territorios.estado e ORDER BY e.estdescricao ASC";
								$db->monta_combo( "estuf", $sql, 'S', 'Unidades Federais', '', '','',168, '', 'estuf' );
								?>
							</td>
						</tr>
						<tr id="tr_municipio" style="display: none">
							<td id="td_municipio">
							</td>
						</tr>
						<tr >
							<td>
								Ano <br />
								<?php
								$anoprocesso = $_POST['anoprocesso'];
								$sql = "select distinct  substring(prpnumeroprocesso,12,4) as codigo,  substring(prpnumeroprocesso,12,4) as descricao 
										from par.processopar 
										where substring(prpnumeroprocesso,12,4) <> '' and prpstatus = 'A' 
										order by substring(prpnumeroprocesso,12,4)";
								$db->monta_combo( "anoprocesso", $sql, 'S', 'Todos', '', '', '', 168, '', 'anoprocesso' );
								?>
							</td>
						</tr>
						<tr id="tr_processo" style="display: none">
							<td>
							Número de Processo: <br>
							<?php
								$filtro = simec_htmlentities( $_REQUEST['numeroprocesso'] );
								$numeroprocesso = $filtro;
								echo campo_texto( 'numeroprocesso', 'N', 'S', '', 20, 20, '#####.######/####-##', '');
							?>
							</td>
					
						</tr>
						<tr id="tr_termo" style="display: none">
							<td>
							Número do Termo: <br>
							<?php
								$dopnumerodocumento = $_REQUEST['dopnumerodocumento'];
								echo campo_texto( 'dopnumerodocumento', 'N', 'S', '', 20, 20, '[#]', '');
							?>
							</td>
					
						</tr>
						<tr>
							<td>
								<div style="text-align: left !important;">
									<input type="button" id="buscar" value="Buscar" /> 
									<input type="button" id="limpar_dados" value="Limpar Filtros" />
								</div>
							</td>
						</tr>
						<tr style="display:none">
							<td>
								<table border="0" align="left" width="99%" cellspacing="4"
									cellpadding="5" class="quadros tabela_painel" id="tb_convenio"
									style="text-align: center; border: solid 3px #FFFFFF;margin-top: 3px; float: left;">
									<tr style="margin-top: 0px; margin-bottom: 0px;">
										<td class="titulo_detalhe">Convênios</td>
									</tr>
									<tr>
										<td class="popup_convenios" style="cursor: pointer;">  
											2007 - 2010
										</td>
									</tr>
								</table>
							</td>
						</tr>
					</table>
					<input type="hidden" name="inuid" id="inuid" value="<?=$_POST['inuid'] ?>" />
				</form>
				<!-- FILTROS - FIM -->
			</td>
			<td class="fundo_padrao " align="middle" style="background-color: #1d1b1b;">
				<table border="0" align="center" width="100%" cellspacing="5" cellpadding="0" class="tabela_painel">
					<tr>
						<td colspan="2">
							<!-- TITULO -->
							<table border="0" align="left" width="100%" cellspacing="4" cellpadding="5" class="quadros tabela_painel" style="text-align: center; border: solid 3px #FFFFFF; margin-top: 3px;">
								<tr>
									<td class="subtitulo" id="titulo" ></td>
								</tr>
							</table>
							<!-- TITULO - FIM -->
						</td>
					</tr>
					<tr>
						<td width="50%">
							<table border="0" align="left" width="100%" cellspacing="4"
								cellpadding="5" class="quadros tabela_painel" id="tb_valores"
								style="text-align: center; border: solid 3px #FFFFFF; margin-top: 3px;">
								<tr>
									<td class="subtitulo titulo_detalhes_valor_pactuado titulo_detalhe">Valor Pactuado</td>
									<td class="subtitulo titulo_detalhes_valor_empenhado titulo_detalhe">Valor Empenhado</td>
									<td class="subtitulo titulo_detalhes_valor_repassado titulo_detalhe">Valor Repassado</td>
									<td class="subtitulo titulo_detalhes_valor_saldo_conta titulo_detalhe">Saldo em Conta</td>
								</tr>
								<tr>
									<td class="mostraDetalhe" funcao="detalhes_valor_pactuado" id="td_valor_pactuado">R$ </td>
									<td class="mostraDetalhe" funcao="detalhes_valor_empenhado" id="td_valor_empenhado">R$ </td>
									<td class="mostraDetalhe" funcao="detalhes_valor_repassado" id="td_valor_repassado">R$ </td>
									<td class="mostraDetalhe" funcao="detalhes_valor_saldo_conta" id="td_valor_saldo_conta">R$ </td>
								</tr>
							</table>
						</td>
						<td width="50%">
							<table border="0" align="left" width="48%" cellspacing="4"
								cellpadding="5" class="quadros tabela_painel" id="tb_financiado"
								style="text-align: center; border: solid 3px #FFFFFF; margin-left: 13px; margin-top: 3px;">
								<tr>
									<td class="subtitulo titulo_detalhes_financiadas_par titulo_detalhes_financiadas_pac titulo_detalhe" colspan="2">QTD de obras Financiadas</td>
								</tr>
								<tr>
									<td class="mostraDetalhe" funcao="detalhes_financiadas_par" id="qtd_financiadas_par">PAR: </td>
									<td class="mostraDetalhe" funcao="detalhes_financiadas_pac" id="qtd_financiadas_pac">PAC: </td>
								</tr>
							</table>
							<table border="0" align="left" width="49%" cellspacing="4"
								cellpadding="5" class="quadros tabela_painel" id="tb_termo"
								style="text-align: center; border: solid 3px #FFFFFF; margin-left: 5px; margin-top: 3px;float:right;">
								<tr style="margin-top: 0px; margin-bottom: 0px;">
									<td colspan="2" class="titulo_detalhes_termos_pac titulo_detalhes_termos_par titulo_detalhe">QTD de Termos</td>
								</tr>
								<tr>
									<td class="mostraDetalhe" funcao="detalhes_termos_par" id="qtd_termos_par">PAR: </td>
									<td class="mostraDetalhe" funcao="detalhes_termos_pac" id="qtd_termos_pac">PAC: </td>
								</tr>
							</table>
						</td>
					</tr>
					<tr>
						<td id="div_grafico_1">
							<?=atualizarGrafico1() ?>
						</td>
						<td id="div_grafico_2">
							<?=atualizarGrafico2() ?>
						</td>
					</tr>
					<tr id="div_grafico_3">
						<td colspan="2">
							<?=atualizarGrafico3() ?>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td colspan="2" style=" background-color:#1d1b1b" id="td_detalhe" >
			</td>
		</tr>
	</table>
</body>
</html>
<?php 
prepararDetalheFuncionalProgramatica();
?>