<?php
/** ATENÇÃO **
 * &acao=A (Cadastro)
 * &acao=E (Edição)
 */

if ( $_POST['op'] == 'salvar' ){
	$agenda = new Agenda( $_POST['agdid'] );
	$dados 			 = $_POST;
	$dados['usucpf'] = $_SESSION['usucpf'];
	$agdid = $agenda->popularDadosObjeto( $dados )
		   			->salvar();
	$db->commit();
	
	$_SESSION['agenda']['agdid'] = $agdid;
	die('<script>
			alert(\'Operação realizada com sucesso!\');
			location.href=\'?modulo=principal/cadastroAgenda&acao=E\';
		 </script>');
}

if ( $_GET['acao'] == 'E' ){
	if ( empty($_REQUEST['agdid']) && empty($_SESSION['agenda']['agdid']) ){
		die('<script>
				location.href=\'?modulo=principal/cadastroAgenda&acao=A\';
			 </script>');		
	}
	$agdid 						 = ( $_REQUEST['agdid'] ? $_REQUEST['agdid'] : $_SESSION['agenda']['agdid']);
	$_SESSION['agenda']['agdid'] = $agdid;
	
	$agenda = new Agenda( $agdid );
	
	if ( !$agenda->possuiPermissaoTotalAgenda( $agdid ) ){
		die('<script>
				alert(\'Usuário não possui permissão de edição desta agenda!\');
				location.href=\'?modulo=principal/listaAgenda&acao=A\';
			 </script>');		
	}
	
	$dados  = $agenda->getDados();
	extract( $dados );	
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
if ( $_REQUEST['acao'] == 'E' ){
	$arAba = abaEdicaoAgenda();
	echo montarAbasArray($arAba, "?modulo=principal/cadastroAgenda&acao=E");
}
monta_titulo( 'Cadastro de Agenda', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function validarForm(){
	if ( $.trim( $('[name=agddsc]').val() ) == '' ){
		alert('O preenchimento do campo "Nome" é obrigatório.');
		return;
	}
	if ( $.trim( $('[name=agdnomeresp]').val() ) == '' ){
		alert('O preenchimento do campo "Nome" (responsável) é obrigatório.');
		return;
	}
	if ( $.trim( $('[name=agdcargoresp]').val() ) == '' ){
		alert('O preenchimento do campo "Cargo" (responsável) é obrigatório.');
		return;
	}
	$('[name=op]').val('salvar');
	$('[name=formulario]').submit();
}
//-->
</script>
<form name="formulario" method="post" action="">
<input name="op" id="op" value="" type="hidden">
<input name="agdid" id="agdid" value="<?php echo $agdid; ?>" type="hidden">
<table class="tabela" align="center">
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td>
		<?=campo_texto("agddsc","S","S","Nome",60,100,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Responsável da Unidade</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td>
		<?=campo_texto("agdnomeresp","S","S","Nome",60,255,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Cargo:</td>
		<td>
		<?=campo_texto("agdcargoresp","S","S","Cargo",60,100,"","")?>
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	   	<input type="button" name="btalterar" value="Salvar" onclick="validarForm()" class="botao">
	   	<?php 
	   	if ( $agdid ){
	   	?>
	   		<input type="button" name="btevento" value="Cadastrar Evento" onclick="window.location='';" class="botao">
	   	<?php 
	   	}
	   	?>	
	   </td>
	</tr> 	
</table>
</form>


