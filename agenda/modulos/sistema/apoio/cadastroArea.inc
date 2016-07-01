<?php
/** ATENÇÃO **
 * &acao=A (Cadastro)
 * &acao=E (Edição)
 */

switch( $_POST['op'] ){
	case 'salvar':
		$area  = new AreaEnvolvida( $_POST['aevid'] );
		$dados = $_POST;
		$agdid = $area->popularDadosObjeto( $dados )
			   		  ->salvar();
		$db->commit();
		
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=sistema/apoio/cadastroArea&acao=A\';
			 </script>');
	case 'excluir':
		if ( $_REQUEST['aevid'] ){
			$msg 	= 'Operação realizada com sucesso!';
			$area = new AreaEnvolvida( $_REQUEST['aevid'] );
			$area->popularDadosObjeto( array('aevstatus' => 'I') )
				   ->salvar();
			$db->commit();
		}else{
			$msg = 'Falha na execução da operação!';
		}	
			
		die('<script>
				alert(\'' . $msg . '\');
				location.href=\'?modulo=sistema/apoio/cadastroArea&acao=A\';
			 </script>');
}

if ( $_GET['acao'] == 'E' ){
	if ( empty($_REQUEST['aevid']) ){
		die('<script>
				location.href=\'?modulo=sistema/apoio/cadastroArea&acao=A\';
			 </script>');		
	}
	$aevid  = $_REQUEST['aevid'];
	
	$area 	= new AreaEnvolvida( $aevid );
	$dados  = $area->getDados();
	extract( $dados );	
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Cadastro de Área', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function validarForm(){
	var errorText = new Array();
	
	if ( $.trim( $('[name=aevdsc]').val() ) == '' ){
		errorText.push('Área');
	}
	
	if ( $.trim( $('[name=aevsigla]').val() ) == '' ){
		errorText.push('Sigla');
	}
	
	if ( errorText.length > 0 ){
		errorText = errorText.join("\n");
		errorText = 'Os campos listados abaixo são de preenchimento obrigatório:\n' + errorText;
		alert( errorText );
		return false;
	}
	
	$('[name=op]').val('salvar');
	$('[name=formArea]').submit();
}

function alterarArea( aevid ){
	location.href='?modulo=sistema/apoio/cadastroArea&acao=E&aevid=' + aevid;
}

function excluirArea( aevid ){
	if ( confirm('Deseja apagar essa área?') ){
		$('[name=op]').val('excluir');
		$('[name=aevid]').val( aevid );
		$('[name=formArea]').submit();
	}
}
//-->
</script>
<form name="formArea" method="post" action="">
<input name="op" id="op" value="" type="hidden">
<input name="aevid" id="aevid" value="<?php echo $aevid; ?>" type="hidden">
<table class="tabela" align="center">
	<tr>
		<td class="SubTituloDireita">Área:</td>
		<td>
		<?=campo_texto("aevdsc","S","S","Nome",60,100,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Sigla:</td>
		<td>
		<?=campo_texto("aevsigla","S","S","Nome",40,20,"","")?>
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	   	<input type="button" name="btalterar" value="Salvar" onclick="validarForm()" class="botao">
	   	<?php 
	   	if ( $aevid ){
	   	?>
	   		<input type="button" name="btevento" value="Cadastrar Área" onclick="location.href='?modulo=sistema/apoio/cadastroArea&acao=A'" class="botao">
	   	<?php 
	   	}
	   	?>	
	   </td>
	</tr> 	
</table>
</form>
<table class="tabela" align="center">
	<tr>
		<td colspan="2" height="310" style="background-color: #F0F0F0;">
			<fieldset style="height:310px; background-color: #FFFFFF;">
				<legend>Áreas Cadastradas</legend>
				<div style="height:300px; overflow:auto;">
				<?php
				$area = new AreaEnvolvida();
				$sql 	= $area->listaSQL( );
				
				$cabecalho = array("Ação", "Área", "Sigla");
				$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
				?>
				</div>
			</fieldset>
		</td>
	</tr>
</table>


