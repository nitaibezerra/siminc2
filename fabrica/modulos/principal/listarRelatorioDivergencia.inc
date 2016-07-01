<?php

include APPRAIZ."fabrica/classes/Contrato.class.inc";


if($_REQUEST['requisicao'] && $_REQUEST['ctrid']){
	$ctrid = $_REQUEST['ctrid'];
	$contrato = new Contrato($ctrid);
	$contrato->$_REQUEST['requisicao']();
	if($_REQUEST['reload']){
		Header("Location: fabrica.php?modulo=principal/listarContrato&acao=A");
	}
}


// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$titulo = "Relatório de Divergência";
monta_titulo( $titulo, '&nbsp;' );

$contrato = new Contrato();

if($_REQUEST['requisicao'] && !$_REQUEST['ctrid']){
	$contrato->$_REQUEST['requisicao']();
}


extract($_POST);

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script>
	function carregaContratos(valor) {
		$.ajax({
		   type: "POST",
		   url: "fabrica.php?modulo=principal/relatoriodivergencia/ajaxContrato&acao=A",
		   dataType:'json',
		   data: { entid: valor },
		   beforeSend: function() {
			   $('#contrato option:first').nextAll().remove();
			   $('#vigencia option:first').nextAll().remove();
		   },
		   success: function(data) {
			   for(var i = 0; i < data.length; i++) {
				   var item = data[i];
				   $('#contrato').append('<option value="' + item.codigo + '">' + item.descricao + '</option>');
			   }
		   }
		});
	}

	function carregaVigencias(valor) {
		$.ajax({
			   type: "POST",
			   url: "fabrica.php?modulo=principal/relatoriodivergencia/ajaxVigencia&acao=A",
			   dataType:'json',
			   data: { ctrid: valor },
			   beforeSend: function() {
				   $('#vigencia option:first').nextAll().remove();
			   },
			   success: function(data) {
				   for(var i = 0; i < data.length; i++) {
					   var item = data[i];
					   $('#vigencia').append('<option value="' + item.codigo + '">' + (item.datainicio + ' - ' + item.datafim) + '</option>');
				   }
			   }
			});
	}

	function pesquisarMemorandos() {
		if($('#empresa').val() == '' || $('#contrato').val() == '' || $('#vigencia').val() == '') {
			alert('Favor informar os campos obrigatórios');
			return;
		}

		$.ajax({
		   type: "POST",
		   url: "fabrica.php?modulo=principal/relatoriodivergencia/ajaxOrdemServico&acao=A",
		   dataType:'html',
		   data: {
			   ctrid: $('#contrato').val(),
			   vgcid: $('#vigencia').val()
		   },
		   success: function(data) {
			   $('#RelatorioDivergenciaResultadoPesquisa').html(data);
		   }
		});
	}

	$(document).ready(function() {
		$('#empresa option:first').attr('selected', true);
		$('#contrato option:first').attr('selected', true);
		$('#vigencia option:first').attr('selected', true);
	});
</script>
<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>
<form name="form_listar_contrato" id="form_listar_contrato" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%" >Empresa:</td>
			<td>
				<?php $sql = "SELECT distinct
                                ent.entid as codigo,
                                ent.entnome as descricao
                        FROM
                                fabrica.contrato ctr
                        INNER JOIN
                                fabrica.contratosituacao cs
                                on cs.ctrid=ctr.ctrid and cs.ctsstatus='A'
                        INNER JOIN
                                fabrica.tiposituacaocontrato tsc
                                on tsc.tscid=cs.tscid and tsc.tscstatus='A'
						INNER JOIN entidade.entidade ent
			        			on ent.entid = ctr.entidcontratado and ent.entstatus = 'A'
                                and ctr.ctrstatus = 'A'
                        ORDER BY codigo asc";

                    $db->monta_combo("entnome_empresa",$sql,'S',"-- Selecione --",'carregaContratos','','', '', 'S', 'empresa','', '', '');
                ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%" >Contrato:</td>
			<td>
				<?php $db->monta_combo("contrato",array(),'S',"-- Selecione --",'carregaVigencias','','', '', 'S', 'contrato','', '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%" >Vigência:</td>
			<td>
				<?php $db->monta_combo("vigencia",array(),'S',"-- Selecione --",'','','', '', 'S', 'vigencia','', '', ''); ?>
			</td>
		</tr>		
		<tr>
			<td colspan="2" class="TituloTabela center">
				<input type="button" onclick="pesquisarMemorandos()" value="Pesquisar" >
			</td>
		</tr>
	</table>
</form>
<div id="RelatorioDivergenciaResultadoPesquisa">

</div>