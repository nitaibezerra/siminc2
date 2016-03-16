<?php

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// carregando o menu
montaAbaInicio("conjur.php?modulo=principal/recuperaGuiaDistribuicao&acao=A");

$titulo = "Guias de Tramitação";
monta_titulo( $titulo, '&nbsp;' );

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<?php //include autocomplete?>
<script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
<?php //include data?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script>
$(document).ready(function()
{

	jQuery('.altera').click(function(){

		var gdiid = jQuery(this).attr('id');
		
		window.open( 'conjur.php?modulo=principal/guiaDistribuicao&acao=A&gdiid='+gdiid, 'guia', 'width=1300,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	});

	$('#filtrar').click(function(){
		$('#filtro').submit();
	});

	$('#limpar').click(function(){
		$('#prcid_dsc').val('Selecione...');
		$('#prcid').val('');
		$('#data_inicio').val('');
		$('#data_fim').val('');
	});

//Complemento campo_popup_autocomplete
	
	jQuery('.campo_popup_autocomplete').autocomplete("/geral/campopopup.php?nome=prcid&autocomplete=1", {
	  	cacheLength:50,
	  	max:50,
		width: 440,
		scrollHeight: 220,
		delay: 1000,
		selectFirst: true,
		autoFill: false
	});

	jQuery('.campo_popup_autocomplete').result(function(event, data, formatted) {
	     if (data) {

	     	// Extract the data values
	     	var campoHidden = jQuery(this).attr('id').replace('_dsc','');
			
			var descricao = data[0];
			var id = data[1];

			if( id == '' ){
		    	jQuery(this).val('');
		    	return false;
			}

	     	jQuery('#'+campoHidden).val( id );
		  	jQuery('#'+campoHidden+'_retorno').val( descricao );
		}
	});

	jQuery('.campo_popup_autocomplete').blur(function (){
		jQuery(this).val(jQuery(this).val());
	  	var campoHidden = jQuery(this).attr('id').replace('_dsc','');
	  	if( jQuery(this).val() != '' && jQuery('#'+campoHidden+'_retorno').val() != '' ){
			if( jQuery(this).val() != jQuery('#'+campoHidden+'_retorno').val() ){
				jQuery(this).val('Selecione...')
				jQuery('#'+campoHidden).val('')
				jQuery('#'+campoHidden+'_retorno').val('')
			}
		}else{
			jQuery(this).val('Selecione...')
		}
	});
	
});
</script>
<form method="POST" name="filtro" id="filtro">
<table align="center" border="0"  cellpadding="0" cellspacing="0" class="tabela">
	<tr>
		<td style="background-color:#e9e9e9; color:#404040; width:600px; text-align:left;font-weight:bold;font-size:12px;">
			<br>
			&nbsp;Processo:<br>&nbsp;
			<?php 
				$sql = "SELECT DISTINCT
							prc.prcid as codigo,
							prc.prcnumsidoc as descricao
						FROM
							conjur.processoconjur prc
						INNER JOIN conjur.itemguiadistribuicao i ON i.prcid = prc.prcid
						WHERE
							prc.prcstatus = 'A'
						ORDER BY
							2";
				$prcid['value'] = $_POST['prcid'];
				$prcid['descricao'] = $_POST['prcid_dsc'];
				campo_popup('prcid',$sql,'Selecione','','400x800','20', Array( 0 =>Array('descricao'=>'Numero do Processo', 'codigo'=>'prc.prcnumsidoc')), 
							1, false, false, '', '', Array('class'=>'campo_popup_autocomplete'));
			?>
		</td>
	</tr>
	<tr>
		<td style="background-color:#e9e9e9; color:#404040; width:600px; text-align:left;font-weight:bold;font-size:12px;">
			<br>
			&nbsp;Data de emissão da guia:<br>&nbsp;
			De:&nbsp;
			<?php
			if($_POST['data_fim']) $data_fim = $_POST['data_fim'];
			else {
				$data_fim = date("d/m/Y");
				$_POST['data_fim'] = $data_fim; 
			}
			
			if($_POST['data_inicio']) $data_inicio = $_POST['data_inicio'];
			else {
				$data_inicio = formata_data($db->pegaUm("SELECT DATE '".formata_data_sql($data_fim)."' - INTERVAL '5 DAYS'"));
				$_POST['data_inicio'] = $data_inicio; 
			}
			?>

			<?=campo_data2('data_inicio', 'N', 'S', 'Data Início', '##/##/####')?>&nbsp;até&nbsp;
		    <?=campo_data2('data_fim', 'N', 'S', 'Data Fim', '##/##/####')?>
		</td>
	</tr>
	<tr>
		<td style="background-color:#e9e9e9; color:#404040; width:600px; text-align:left;font-weight:bold;font-size:12px;">
			<br>
			<center>
				<input type="button" id="filtrar" value="Filtrar"/>
				<input type="button" id="limpar" value="Limpar"/>
			</center>
			<br>
		</td>
	</tr>
</table>
</form>
<table align="center" border="0"  cellpadding="0" cellspacing="0" class="tabela">
	<tr>
		<td style="background-color:#e9e9e9; color:#404040; width:600px; text-align:center;font-weight:bold;font-size:12px;">
			<center>
				Guias de Distribuição
			</center>
		</td>
	</tr>
	<tr>
		<td style="background-color:#e9e9e9; color:#404040; width:600px; text-align:center;font-weight:bold;font-size:12px;">
			<span style="float:left;font-weight:bold;">
				&nbsp;
				<img border="0" align="absmiddle" onclick="window.open( 'conjur.php?modulo=principal/processosTramitados&acao=A', 'guia', 'width=800,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );" title="Gerar Guia de Tramitações Antigas" style="cursor: pointer" src="../imagens/gif_inclui.gif">
				Gerar Guia de Tramitações Antigas
			</span>
		</td>
	</tr>
</table>
<?php
		
	$where = Array();
	
	array_push($where, " gdistatus = 'A' ");
	
	if( $_POST ){
		
		extract($_POST);
		if($_POST['prcid']){
			array_push($where, " prcid = '".$_POST['prcid']."' ");
		}
		
		if($data_inicio != "" && $data_fim != "") {
			array_push($where, " ( gdidt >= '".formata_data_sql($data_inicio)."' AND gdidt <= '".formata_data_sql($data_fim)."' ) ");
		} elseif($data_inicio != "") {
			array_push($where, " gdidt >= '".formata_data_sql($data_inicio)."' ");
		} elseif($data_fim != "") {
			array_push($where, " gdidt <= '".formata_data_sql($data_fim)."' ");
		} 
	}
	
	$sql = "SELECT DISTINCT
				'<img border=\"0\" class=\"altera\" id=\"'|| g.gdiid ||'\" title=\"Abrir Guia de Distribuição\" style=\"cursor: pointer\" src=\"../imagens/alterar.gif\">' as acoes,
				gdidsc,
				to_char(gdidt,'DD-MM-YYYY') as data,
				o.esddsc,
				a.aeddscrealizada,
				usunome
			FROM
				conjur.guiadistribuicao g
			INNER JOIN conjur.itemguiadistribuicao i ON i.gdiid = g.gdiid
			INNER JOIN workflow.historicodocumento h ON h.hstid = i.hstid
			INNER JOIN workflow.acaoestadodoc      a ON a.aedid = h.aedid
			INNER JOIN workflow.estadodocumento    o ON o.esdid = a.esdidorigem
			INNER JOIN workflow.estadodocumento    d ON d.esdid = a.esdiddestino
			LEFT  JOIN seguranca.usuario u ON u.usucpf = g.usucpf
			WHERE
				".implode('AND',$where)."
			ORDER BY
				3 DESC, 2 DESC";

	$alinhamento = array( 'center', 'left', 'center', 'center','center', 'center' );
	$cabecalho   = array("Ação","Guia de Distribuição", "Data de emissão da Guia", "Estado de Origem", "Tramitação", "Emissor da Guia");
	$db->monta_lista($sql, $cabecalho, 999, 20, '', '100%', '', '', '', $alinhamento);
?>
