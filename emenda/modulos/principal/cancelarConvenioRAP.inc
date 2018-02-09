<?php

require_once APPRAIZ . "emenda/classes/WSEmpenho.class.inc";
require_once APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Cancelamento Convênio RAP', '');

$sql = "SELECT DISTINCT
				case when pc.pmcdatacancelamento is not null then
            		'<center><input type=\"checkbox\" id=\"pmcid[' || pc.pmcid || ']\" name=\"pmcid[]\" value=\"' || pc.pmcid || '\" disabled=\"disabled\" checked />'
            	else
                	'<center><input type=\"checkbox\" id=\"pmcid[' || pc.pmcid || ']\" name=\"pmcid[]\" value=\"' || pc.pmcid ||'_'|| pt.ptrid || '\" />'
                end ||'<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Consultar\" onclick=\"abreMinutaConvenio('||pt.ptrid||');\" border=\"0\"></center>' as checkbox,
				(pt.ptrcod ||'/'|| pt.ptrexercicio) as n_pta,
                pt.ptrnumprocessoempenho as n_do_processo,
                ef.exfnumempenhooriginal as n_da_nota_de_empenho,
				sum(ef.exfvalor) as valor_do_convenio,
                (pt.ptrnumconvenio ||'/'|| pt.ptranoconvenio) as n_do_convenio_no_fnde,
				pc.pmcnumconveniosiafi as n_do_convenio_no_siafi
			FROM emenda.planotrabalho pt			
				inner join emenda.execucaofinanceira ef on ef.ptrid=pt.ptrid
				inner join emenda.ptminutaconvenio pc on pc.ptrid=pt.ptrid
				inner join emenda.analise an on an.ptrid=pt.ptrid
			
			WHERE
				pt.ptrstatus = 'A'
				and ef.exfstatus = 'A'
				and pc.pmcstatus = 'A'
                and pt.ptranoconvenio = '".$_SESSION['exercicio']."'
                and pc.pmcnumconveniosiafi is not null
               -- and pt.ptrcod = 386
			GROUP BY pt.ptrcod, pt.ptrexercicio, ef.exfnumempenhooriginal,pc.pmcnumconveniosiafi,
				 pt.ptrnumconvenio, pt.ptranoconvenio, pt.ptrnumprocessoempenho, pc.pmcdatacancelamento, pc.pmcid, pt.ptrid
			ORDER BY n_pta";
	
$arDados = $db->carregar( $sql );

$boHabilita = empty($arDados) ? 'disabled="disabled"' : '';

?>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script src="../includes/prototype.js"></script>
<script type="text/javascript">
	
	/*function CancelarConvenio(){
		document.getElementById('requisicao').value = 'cancelar';
		document.getElementById('formularioCancelar').submit();
	}*/
	
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url){
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(560,345);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function closeMessage(){
		messageObj.close();	
	}

	function CancelarConvenio(){
		var form = $('formularioCancelar');
		
		if( $('justificativa').value == "" ){
			alert('Informe a justificativa para o cancelamento do convênio');
			return false;
		}
		var pmcid = '';
		for(i = 0; i < form.length; i++){
			if( form.elements[i].type == "checkbox" ){
				if( form.elements[i].checked == true && form.elements[i].disabled == false ){
					if( pmcid == '' ){
						pmcid = form.elements[i].value;
					} else {
						pmcid = pmcid + '|' + form.elements[i].value;
					}
				}
			}
		}
		if( pmcid == '' ){
			alert('Selecione pelo menos um convênio!');
			return false;
		}
		
		var url = "ajax_emenda.php";
		var pars = $('formularioCancelar').serialize( true );
		pars['execucaoPTA'] = 'salvarCancelamento';
		
		var myAjax = new Ajax.Request(
				url, 
				{
					method: 'post', 
					parameters: pars, 
					asynchronous: false,
					onComplete: function(r)
					{
						if( trim(r.responseText) == '1'){
							displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_empenho=cancelarConvenio&pmcid='+pmcid);
						}else{
							alert('Falha na operação.');
							//$('divDebug').innerHTML = r.responseText; 
						}
					}
				});
	}
	
	function abreMinutaConvenio( ptrid ){
		var janela = window.open('emenda.php?modulo=principal/minutaConvenio&acao=A&ptridAnalise='+ptrid,"relatorio", "scrollbars=yes,height=600,width=800");
		janela.focus();
	}
</script>
<form action="" method="post" id="formularioCancelar" name="formularioCancelar">
<input type="hidden" value="" name="requisicao" id="requisicao"/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" width="95%" cellPadding=3 align="center">
<tr>
	<td class="subtitulodireita" style="width: 20%">Justificativa:</td>
	<td><?=campo_textarea('justificativa', 'S', 'S', 'Justificativa', 98, 10, 10000, '', '', '', '', 'Justificativa');?></td>
</tr>
<tr bgcolor="#D0D0D0">
	<td colspan="2">
		<input type="button" value="Cancelar Convênio" <?=$boHabilita; ?> onclick="CancelarConvenio();" style="cursor: pointer;" />
	</td>
</tr>
</table>
<?
	if( $arDados ){
?>
<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
	<tr>				
		<td align="Center" class="title"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong></strong></td>
		<td align="Center" class="title"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nº PTA</strong></td>			
		<td align="Center" class="title"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nº do Processo PTA</strong></td>			
		<td align="Center" class="title"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nº da Nota de Empenho</strong></td>			
		<td align="Center" class="title"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor Empenhado</strong></td>
		<td align="Center" class="title"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nº do Convênio do FNDE</strong></td>
		<td align="Center" class="title"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nº do Convênio no SIAFI</strong></td>
	</tr>
</thead><?
		foreach ($arDados as $key => $value) { 
			$key % 2 ? $cor = "" : $cor = "#f7f7f7";?>
							
			<tr bgcolor="<?=$cor; ?>" onmouseout="this.bgColor='<?=$cor; ?>'" onmouseover="this.bgColor='#ffffcc';">
				<td><?=$value['checkbox']; ?></td>
				<td><?=$value['n_pta']; ?></td>
				<td><?=$value['n_do_processo']; ?></td>
				<td><?=$value['n_da_nota_de_empenho']; ?></td>
				<td><?=$value['valor_do_convenio']; ?></td>
				<td><?=$value['n_do_convenio_no_fnde']; ?></td>
				<td><?=$value['n_do_convenio_no_siafi']; ?></td>
			</tr>
		<?} ?>
		</table>
		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tr bgcolor="#ffffff">
				<td><b>Total de Registros: <?=count($arDados); ?></b></td>
				<td></td>
			</tr>
		</table>
	<?} else {?>
		<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
			<tbody>
			<tr>
				<td align="center" style="color: rgb(204, 0, 0);">Não foram encontrados Registros.</td>
			</tr>
			<tbody>
		</table>
	<?} ?>
</form>
<div id="divDebug"></div>