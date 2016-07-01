<html>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
</head>
<body>

<?php

$titulo = "Geração de Guia de Tramitações Antigas";
monta_titulo( $titulo, '&nbsp;' );

?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
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
	$('#selectall').click(function()
	{
		if( $(this).is(':checked') )
		{
			$('[name=guia[]]').each(function()
			{
				if( $(this).not(':checked') ) $(this).attr('checked', true);
			});
		}
		else
		{
			$('[name=guia[]]').each(function()
			{
				if( $(this).is(':checked') ) $(this).attr('checked', false);
			});
		}
	});

	//Complemento campo_popup_autocomplete
	
	jQuery('#aedid_dsc').autocomplete("/geral/campopopup.php?nome=aedid&autocomplete=1", {
	  	cacheLength:50,
	  	max:50,
		width: 440,
		scrollHeight: 220,
		delay: 1000,
		selectFirst: true,
		autoFill: false
	});

	jQuery('#prcint_dsc').autocomplete("/geral/campopopup.php?nome=prcint&autocomplete=1", {
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

	$('#filtrar').click(function(){
		if( $('#aedid').val() == '' ){
			alert('Campo Obrigatório.');
			$('#aedid').focus();
			return false;
		}
		$('#filtro').submit();
	});

	$('#limpar').click(function(){
		$('#aedid_dsc').val('Selecione...');
		$('#aedid').val('');
		$('#prcint_dsc').val('Selecione...');
		$('#prcint').val('');
		$('#data_inicio').val('');
		$('#data_fim').val('');
	});
});

function geraGuia()
{
	var form = document.getElementById('formguia');
	
	var qtd = 0;
	$('.guia').each(function()
	{
		if( $(this).is(':checked') )
		{
			qtd++;
		}
	});

	if( qtd == 0 )
	{
		alert('Pelo menos um processo deve ser selecionado.');
		return;		
	}

	form.action	= 'profeinep.php?modulo=principal/guiaDistribuicao&acao=A';
	var janela 	= window.open( '', 'guia', 'width=1300,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	janela.focus();
	form.target = 'guia';
	form.submit();
}
</script>
<form method="POST" name="filtro" id="filtro">
<table align="center" border="0"  cellpadding="0" cellspacing="0" class="tabela">
	<tr>
		<td colspan="2" style="background-color:#e9e9e9; color:#404040; width:600px; text-align:left;font-weight:bold;font-size:12px;">
			<br>
			&nbsp;Origem e Tramitação:<br>&nbsp;
			<?php 
				$sql = "SELECT DISTINCT
							aed.aedid as codigo,
							e.esddsc||' - '||aed.aeddscrealizar as descricao  
						FROM 	
							workflow.acaoestadodoc aed 
						INNER JOIN workflow.estadodocumento e ON e.esdid = aed.esdidorigem AND tpdid = ".TIPODOC."
						WHERE 
							aedstatus = 'A' 
							AND aed.aedid NOT IN (53) 
							AND trim(aed.aeddscrealizar) <> ''
						ORDER  BY 
							2";
				$aedid['value'] = $_POST['aedid'];
				$aedid['descricao'] = $_POST['aedid_dsc'];
				campo_popup('aedid',$sql,'Selecione','','400x800','50', Array( 0 =>Array('descricao'=>'Estado Origem', 'codigo'=>'e.esddsc'),1 =>Array('descricao'=>'Tramitação', 'codigo'=>'aed.aeddscrealizar')), 
							1, false, false, '', 'S', Array('class'=>'campo_popup_autocomplete'));
			?>
		</td>
	</tr>
	<tr>
		<td colspan="2" style="background-color:#e9e9e9; color:#404040; width:600px; text-align:left;font-weight:bold;font-size:12px;">
			<br>
			&nbsp;Interessado:<br>&nbsp;
			<?php 
				$sql = "SELECT DISTINCT
							prcnomeinteressado as codigo,
							substring(prcnomeinteressado from 1 for 100) as descricao
						FROM
							profeinep.processoprofeinep prc
						WHERE
							prc.prcstatus = 'A'
						ORDER BY
							2";
				$prcint['value'] = $_POST['prcint'];
				$prcint['descricao'] = $_POST['prcint_dsc'];
				campo_popup('prcint',$sql,'Selecione','','400x800','70', Array( 0 =>Array('descricao'=>'Interessado', 'codigo'=>'prcnomeinteressado')), 
							1, false, false, '', '', Array('class'=>'campo_popup_autocomplete'));
			?>
		</td>
	</tr>
	<tr>
		<td colspan="2" style="background-color:#e9e9e9; color:#404040; width:600px; text-align:left;font-weight:bold;font-size:12px;">
			<br>
			&nbsp;Data de entrada do processo:<br>&nbsp;
			De:&nbsp;
			<?php $data_inicio = $_POST['data_inicio'];?>
			<?php $data_fim = $_POST['data_fim'];?>
			<?=campo_data2('data_inicio', 'N', 'S', 'Data Início', '##/##/####')?>&nbsp;até&nbsp;
		    <?=campo_data2('data_fim', 'N', 'S', 'Data Fim', '##/##/####')?>
		</td>
	</tr>
	<tr>
		<td style="background-color:#e9e9e9; color:#404040; width:600px; text-align:right;font-weight:bold;font-size:12px;">
				<br>
				<input type="button" id="filtrar" value="Filtrar"/>
				<input type="button" id="limpar" value="Limpar"/>
				</br><br>
		</td>
		<td style="background-color:#e9e9e9; color:#404040; width:600px; text-align:left;font-weight:bold;font-size:12px;">
			<div style="display: <?=$_POST['aedid'] ? '' : 'none' ?>"><input type="button" value="Gerar guia" onclick="geraGuia();" /></div></td>
	</tr>
</table>
</form>
<table align="center" border="0"  cellpadding="0" cellspacing="0" class="tabela" style="display: <?=$_POST['aedid'] ? '' : 'none' ?> ">
	<tr>
		<td style="background-color:#e9e9e9; color:#404040; width:600px; text-align:center;font-weight:bold;font-size:12px;">
			<span style="float:left;font-weight:bold;"><input type="checkbox" id="selectall" />Selecionar Todos</span>
			Processos
		</td>
	</tr>
</table>
<?php 
	if($_POST['aedid']){
		
		extract($_POST);
		
		$where = Array();
		
		array_push($where, " prc.prcstatus = 'A' ");
		array_push($where, " aed.aedid = ".$_POST['aedid']." ");
		
		if($prcint){
			array_push($where, " removeacento(prc.prcnomeinteressado) ilike removeacento('".$prcint."%') ");
		}
		
		if($data_inicio != "" && $data_fim != "") {
			array_push($where, " ( prc.prcdtentrada >= '".formata_data_sql($data_inicio)."' AND prc.prcdtentrada <= '".formata_data_sql($data_fim)."' ) ");
		} elseif($data_inicio != "") {
			array_push($where, " prc.prcdtentrada >= '".formata_data_sql($data_inicio)."' ");
		} elseif($data_fim != "") {
			array_push($where, " prc.prcdtentrada <= '".formata_data_sql($data_fim)."' ");
		}
		
		$sql = "SELECT DISTINCT
					prc.prcid,
					prc.prcnumsidoc,
					to_char(hst.htddata, 'DD/MM/YYYY') as data,
					aed.esdidorigem,
					aed.aeddscrealizar,
					hst.hstid,
					prc.prcnomeinteressado
				FROM
					profeinep.processoprofeinep prc
				INNER JOIN profeinep.coordenacao coo ON coo.coonid = prc.cooid AND coo.coostatus = 'A'
				INNER JOIN profeinep.estruturaprocesso    esp ON esp.prcid = prc.prcid
				INNER JOIN workflow.documento 	       doc ON doc.docid = esp.docid
				INNER JOIN workflow.estadodocumento    esd ON esd.esdid = doc.esdid
				INNER JOIN workflow.historicodocumento hst ON hst.docid = doc.docid
												   			  AND hst.hstid NOT IN (SELECT g.hstid FROM profeinep.itemguiadistribuicao g WHERE g.prcid = prc.prcid)
				INNER JOIN workflow.acaoestadodoc	   aed ON aed.aedid = hst.aedid
				WHERE
					".implode('AND',$where)."
				ORDER BY
					aed.aeddscrealizar ASC";
		$processos = $db->carregar($sql);
	}
?>
<form method="post" id="formguia">
<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" style="display: <?=$_POST['aedid'] ? '' : 'none' ?> ">
	<thead>
		<tr>
			<td valign="top" bgcolor="" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><strong>Ação</strong></td>
			<td valign="top" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><strong>Número do Processo</strong></td>
			<td valign="top" bgcolor="" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><strong>Data da Tramitação</strong></td>
			<td valign="top" bgcolor="" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><strong>Tramitação</strong></td>
			<td valign="top" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title"><strong>Interessado</strong></td>
		</tr>
	</thead>
	<tbody>
	<?php if($processos): ?>
		<?php foreach($processos as $processo): ?>
		<tr bgcolor="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
			<td align="center" title="Ação"><input type="checkbox" class="guia" value="<?=$processo['prcid']?>" name="guia[<?=$processo['hstid']?>]"></td>
			<td align="left" title="Número do Processo" style="color:#0066cc;"><?=$processo['prcnumsidoc']?></td>
			<td align="center" title="Data de entrada do processo"><?=$processo['data']?></td>
			<td align="center" title="Situação"><?=$processo['aeddscrealizar']?></td>
			<td align="center" title="Interessado"><?=$processo['prcnomeinteressado']?></td>
		</tr>
		<?php endforeach; ?>
	<?php else: ?>
		<tr>
			<td align="center" style="color:#cc0000;" colspan="5">Não foram encontrados Registros.</td>
		</tr>
	<?php endif; ?>
	</tbody>
</table>
</form>

<table align="center" border="0"  cellpadding="0" cellspacing="0" class="tabela" style="display: <?=$_POST['aedid'] ? '' : 'none' ?> ">
	<tr>
		<td style="background-color:#e9e9e9; color:#404040; width:600px; text-align:center;font-weight:bold;font-size:12px;">
			<input type="button" value="Gerar guia" onclick="geraGuia();" />
		</td>
	</tr>
</table>

</body>
</html>