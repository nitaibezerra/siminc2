<?php

$join = '';

$boJoin = false;
if ($_POST['filtroIndicador']) {
	$filtro[] = " (ind.indnome ILIKE '%{$_POST['filtroIndicador']}%' OR ind.indid::text ILIKE '%{$_POST['filtroIndicador']}%') ";
	// Caso não seja informado o status do item(Painel ou checklist) ele filtra o nome somente dos indicadores permitidos  			
	if (!isset($_POST['cs_checklist']) && !isset($_POST['cs_painel'])) {
//		$boJoin = true;
		$filtro[] = " ((ind.indstatus = 'A') OR (ind.indstatus = 'I' AND ind.indhomologado = 't')) "; 			
	}
}

if (isset($_POST['cs_checklist']) && isset($_POST['cs_painel'])) {
	$filtro[] = " ((ind.indstatus = 'A') OR (ind.indstatus = 'I' AND ind.indhomologado = 't')) "; 			
} elseif (isset($_POST['cs_checklist'])) {
	$boJoin = true;
	$filtro[] = " ind.indstatus = 'I' AND ind.indhomologado = 't' "; 			
} elseif (isset($_POST['cs_painel']) || !$filtro) {
	$filtro[] = " ind.indstatus = 'A' "; 			
}			
if($boJoin){
	$join = " INNER JOIN pde.monitoraitemchecklist mic on mic.indid = ind.indid AND mic.micstatus = 'A'
		      INNER JOIN pde.atividade a2 on a2.atiid = mic.atiid AND a2.atistatus = 'A' and _atiprojeto = '{$_SESSION['projeto']}' ";
}

$sql = "SELECT 
			ind.indid || ' - ' || ind.indnome AS descricao, 
			ind.indid AS codigo
		FROM  painel.indicador ind
		$join
		WHERE " . implode(' AND ', $filtro) ."
		ORDER BY descricao";

$arDado = $db->carregar($sql);

//$cabecalho = array( array("label" => "<input type='checkbox' name='todos' value='todos'>", 
//						  "colunas" => array("&nbsp;")),
//				    array("label" => "Selecione o(s) Indicador(es)",
//				    	  "colunas" => array("Descrição")));
//$db->monta_lista( $sql, $cabecalho, 20, 10, 'N', '', '' );
?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title>Selecione o(s) Indicador(es) </title>
		<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-1.7.1.min.js"></script>
		<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script>
		var controladorIndicador = {
			checkAll : function ( obj ){
				if (obj.checked){
					$('[id^=indid_]').attr('checked', true);
					$('[name^=check_all]').attr('checked', true);
				}else{
					$('[id^=indid_]').attr('checked', false);
					$('[name^=check_all]').attr('checked', false);
				}
				
				window.opener.$('#td_indicador table tr').remove();
				
				$('[id^=indid_]').each(function (){
					controladorIndicador.checkIndicador( $(this).val() );
				});
			},
			
			checkIndicador : function ( indid ){
				if ( $('[id^=indid_]:not(:checked)').size() == 0 ){
					$('[name^=check_all]').attr('checked', true);
				}else{
					$('[name^=check_all]').attr('checked', false);
				}
				
				if ( window.opener.$('#td_indicador [name^=indid_vinculado]').size() == 0 ){
					controladorIndicador.createCabecalho();
				}
				
				if ( $('#indid_' + indid + ':checked').size() == 1 ){
					controladorIndicador.createRow( indid );
				}else{
					controladorIndicador.removeRow( indid );
				}
				
				if ( window.opener.$('#td_indicador [name^=indid_vinculado]').size() > 3 ){
					window.opener.$('#td_indicador').css({height : '150px', overflow : 'auto'});
				}else if ( window.opener.$('#td_indicador [name^=indid_vinculado]').size() < 4 ){
					window.opener.$('#td_indicador').css({height : '', overflow : ''});
				}
				
			},
			
			createCabecalho : function (){
				var tr  = $('<tr>');
				var td1 = $('<td>').attr('class',  'title')
								   .attr('valign', 'top')
								   .attr('align',  'center')
								   .css({'width' : '65px', 'border-right' : '1px solid #c0c0c0', 'border-bottom' : '1px solid #c0c0c0', 'border-left' : '1px solid #ffffff'})
								   .html('Ação');
				var td2 = $('<td>').attr('class',  'title')
								   .attr('valign', 'top')
								   .attr('align',  'center')
								   .css({'border-right' : '1px solid #c0c0c0', 'border-bottom' : '1px solid #c0c0c0', 'border-left' : '1px solid #ffffff'})
								   .html('Indicador');
			
				tr.append( td1 )
				  .append( td2 );
			
				if ( window.opener.$('#td_indicador table thead').size() == 0 ){
					var thead = $('<thead>');
					window.opener.$('#td_indicador table').append( thead );
				}
			
				window.opener.$('#td_indicador table tr').remove();
				window.opener.$('#td_indicador table thead').append( tr );
			
			},
			
			createRow : function ( indid ){
				var tr     = window.opener.$('<tr>');
				var td1    = window.opener.$('<td>');
				var input  = window.opener.$('<input>');
				var input2 = window.opener.$('<input>');
				var input3 = window.opener.$('<input>');
				var input4 = window.opener.$('<input>');
				var td2    = window.opener.$('<td>');
				
				input.attr('name', 'indid_vinculado[]')
					 .attr('type', 'hidden')
					 .val( indid );
					 
				input2.attr('id', 'indid_filtro_'+indid)
					  .attr('name', 'indid_filtro[]')
					  .attr('type', 'hidden')
					  .val('');
					  
				input3.attr('id', 'idvdsc_'+indid)
					  .attr('name', 'idvdsc[]')
					  .attr('type', 'hidden')
					  .val('');
					  
				input4.attr('id', 'idvmeta_'+indid)
					  .attr('name', 'idvmeta[]')
					  .attr('type', 'hidden')
					  .val('');
												
				td1.attr('align', 'center')
				   .append( input )
				   .append( input2 )
				   .append( input3 )
				   .append( input4 );
				
				tr.append( td1 )
				  .append( td2 );
				
				
				var acao = '<img class="link img_middle" src="../imagens/principal.gif" title="Detalhar Indicador" onclick="filtrarIndicador(' + indid + ')">' +
						   '&nbsp;' +  
						   '<img class="link img_middle" src="../imagens/excluir.gif" title="Excluir Indicador" onclick="excluirIndicador(' + indid + ')">';  
						   
				td1.append( acao );
				
				var label = $('#label_indid_' + indid).html();
				td2.append( label );
				
				window.opener.$('#td_indicador table tbody').append( tr );				
			},
			
			removeRow : function ( indid ){
				window.opener.$('#td_indicador table tbody tr td input[value=' + indid + ']').parents('tr:first').remove();	
			},
			
			checkIndicadorSelecionado : function (){
			
				window.opener.$('#td_indicador [name^=indid_vinculado]').each(function (){
					var indid = $(this).val();
					$('[id=indid_' + indid + ']').attr('checked', true);
				});
				
				if ( $('[id^=indid_]:not(:checked)').size() == 0 ){
					$('[name^=check_all]').attr('checked', true);
				}else{
					$('[name^=check_all]').attr('checked', false);
				}
			}
			
		};
		
		$(document).ready(function (){
			controladorIndicador.checkIndicadorSelecionado();
		});
			
		</script>
	</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<form action="" method="post" name="form_filtro">
	<table  class="tabela" style="width:100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#cdcdcd">
			<td align="center" width="100%" colspan="2" height="25" ><label style="color: rgb(0, 0, 0); font-size:12px;" class="TituloTela">Selecione o(s) Indicador(es) </label></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Indicador</td>
			<td>
				<input type="text" style="text-align:left;" name="filtroIndicador" size="31" maxlength="100" value="<?php echo $_POST['filtroIndicador']; ?>" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : left; width:33ex;"  title='' class=' normal' />
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita">Painel</td>
			<td>
				<input type="checkbox" name="cs_painel" title="Itens do Painel" value="P" <?php echo 'P' == $_POST['cs_painel'] ? 'checked="checked"' : '';?> />
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita">Itens do Checklist</td>
			<td>
				<input type="checkbox" name="cs_checklist" title="Itens do Checklist" value="C" <?php echo 'C' == $_POST['cs_checklist'] ? 'checked="checked"' : '';?> />
			</td>
		</tr>	
		<tr bgcolor="#cdcdcd">
			<td height="20">&nbsp;</td>
			<td><input type="submit" value="Filtrar"></td>
		</tr>
	</table>
</form>
<form name="vincular_indicador">
	<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
		<tr bgcolor="#cdcdcd">
			<td width="1" style="text-align: center;">
				<? if ( is_array( $arDado ) && count( $arDado ) ) : ?>
					<input type="checkbox" name="check_all[]" type="checkbox" value="" onclick="javascript:controladorIndicador.checkAll( this );"/>
				<? else: ?>
					-
				<? endif; ?>
			</td>
			<td valign="top">
				<strong>Selecione o(s) Indicador(es)</strong><input type="button" value="Ok" style="margin-left: 20px;" onclick="window.close();">
			</td>
		</tr>
		<tr bgcolor="#dcdcdc">
			<td width="1" style="text-align: center;">
				&nbsp;
			</td>
			<td valign="top" align="center">
				Descrição
			</td>
		</tr>
<?php
if ( $arDado ):
	$numLinha = 0;
	foreach ($arDado as $dado):
?>		
		<tr bgcolor="#<?=($numLinha % 2 == 0 ? 'f4f4f4' : 'e0e0e0') ?>">
			<td width="1" style="text-align: center;">
				<input type="checkbox" name="indid[<?=$dado['codigo']?>]" id="indid_<?=$dado['codigo']?>" type="checkbox" value="<?=$dado['codigo']?>" onclick="javascript:controladorIndicador.checkIndicador( <?=$dado['codigo']?> );"/>
			</td>
			<td valign="top" align="left">
				<label id="label_indid_<?=$dado['codigo']?>" for="indid_<?=$dado['codigo']?>"><?=$dado['descricao'] ?></label>
			</td>
		</tr>
<?php 
	$numLinha++;
	endforeach;
else:
?>
		<tr>
			<td align="center" colspan="2" style="color:#cc0000;">Não foram encontrados registros.</td>
		</tr>
<?php
endif;
?>		
		<tr bgcolor="#cdcdcd">
			<td width="1" style="text-align: center;">
				<? if ( is_array( $arDado ) && count( $arDado ) ) : ?>
					<input type="checkbox" name="check_all[]" type="checkbox" value="" onclick="javascript:controladorIndicador.checkAll( this );"/>
				<? else: ?>
					-
				<? endif; ?>
			</td>
			<td valign="top">
				<strong>Selecione o(s) Indicador(es)</strong><input type="button" value="Ok" style="margin-left: 20px;" onclick="window.close();">
			</td>
		</tr>
	</table>
</form>
</body>
</html>