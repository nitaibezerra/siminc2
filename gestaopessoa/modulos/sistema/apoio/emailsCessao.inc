<?php
/** ATENÇÃO **
 * &acao=A (Cadastro)
 * &acao=E (Edição)
 */

switch( $_POST['op'] ){
	case 'salvar':
		
		if ($_REQUEST['emcid'] == '')
		{
		$sql = "insert into gestaopessoa.emailcessao (orcid, emcusuario,emcdsc) values ({$_REQUEST['orcid']},
		'{$_REQUEST['emcusuario']}', '{$_REQUEST['emcdsc']}')";
		
		$db->executar($sql);
				
		$db->commit();
		
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=sistema/apoio/emailsCessao&acao=A\';
			 </script>');
		}
		else
		{
			$sql = "update  gestaopessoa.emailcessao set orcid = {$_REQUEST['orcid']},
			emcusuario = '{$_REQUEST['emcusuario']}',
			emcdsc = '{$_REQUEST['emcdsc']}' where emcid = {$_REQUEST['emcid']} ";
			
			
			$db->executar($sql);
			
			$db->commit();
			
			die('<script>
			alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=sistema/apoio/emailsCessao&acao=A\';
			 </script>');
			
		}
	case 'excluir':
		if ( $_REQUEST['emcid'] ){
			
			$sql = "delete from gestaopessoa.emailcessao where emcid = {$_REQUEST['emcid']}";

			$db->executar($sql);
			
			$db->commit();
			$msg 	= 'Operação realizada com sucesso!';
		}else{
			$msg = 'Falha na execução da operação!';
		}	
			
		die('<script>
				alert(\'' . $msg . '\');
				location.href=\'?modulo=sistema/apoio/emailsCessao&acao=A\';
			 </script>');
}

if ( $_GET['acao'] == 'E' ){
	if ( empty($_REQUEST['aevid']) ){
		die('<script>
				location.href=\'?modulo=sistema/apoio/emailsCessao&acao=A\';
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
	
	var emailReg = /^([\w-\.]+@([\w-]+\.)+[\w-]{2,4})?$/;
  	
  	if(  $('[name=emcusuario]').val() == '' ) {
    	alert('informe o usuário');
    	return;
  	}
  	
  	if(  $('[name=emcdsc]').val() == '' ) {
    	alert('informe o email');
    	return;
  	} 
  	  	
  	if( !emailReg.test( $('[name=emcdsc]').val() )) {
    	alert('email inválido');
    	return;
  	}
  	
  	if(  $('[name=emcdsc]').val() != $('[name=confirmacao]').val() ) {
    	alert('email diferente da confirmação');
    	return;
  	} 
	
	$('[name=op]').val('salvar');
	$('[name=formArea]').submit();
}

function alterar( emcid, orcid, emcusuario, emcdsc ){
	$('[name=emcid]').val( emcid );
	$('[name=orcid]').val( orcid );
	$('[name=emcusuario]').val( emcusuario );
	$('[name=emcdsc]').val( emcdsc );
	$('[name=confirmacao]').val( emcdsc );
}

function excluir( emcid ){
	if ( confirm('Deseja apagar esse email?') ){
		$('[name=op]').val('excluir');
		$('[name=emcid]').val( emcid );
		$('[name=formArea]').submit();
	}
}
//-->
</script>
<form name="formArea" method="post" action="">
<input name="op" id="op" value="" type="hidden">
<input name="emcid" id="emcid" value="<?php echo $emcid; ?>" type="hidden">
<table class="tabela" align="center">
	<tr>
		<td class="SubTituloDireita">Origem:</td>
		<td>
		<? 
		$sql = "select orcid as codigo, orcdsc as descricao from gestaopessoa.origemcessao where orcid != 2 order by orcdsc";
		
		$db->monta_combo( 'orcid', $sql, 'S', '', '', '','','','','orcid',''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome do Usuário:</td>
		<td>
		<?=campo_texto("emcusuario","S","S","Nome",40,20,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Email:</td>
		<td>
		<?=campo_texto("emcdsc","S","S","Email",40,20,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Confirmação de Email:</td>
		<td>
		<?=campo_texto("confirmacao","S","S","Nome",40,20,"","")?>
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	   	<input type="button" name="btalterar" value="Salvar" onclick="validarForm()" class="botao">	
	   </td>
	</tr> 	
</table>
</form>
<table class="tabela" align="center">
	<tr>
		<td align=left colspan="2" height="310" style="background-color: #F0F0F0;">
			<fieldset style="height:310px; background-color: #FFFFFF;">
				<legend>Emails Cadastrados</legend>
				<div style="height:300px; overflow:auto;">
				<?php
				$sql = "SELECT
				'<a onclick=\"return alterar( '|| emcid ||  ', ' ||o.orcid || ', \'' || emcusuario || '\', \'' || emcdsc || '\');\"><img border=0 src=\"../imagens/alterar.gif\" /></a> 
				 <a style=\"cursor:pointer\"  onclick=\"return excluir( '|| emcid ||  ');\" >
				 <img border=0 src=\"../imagens/excluir.gif\" /></a>', 
				orcdsc, emcusuario,emcdsc
				 FROM gestaopessoa.emailcessao e inner join gestaopessoa.origemcessao o 
				 on e.orcid = o.orcid
				ORDER BY 
				 emcid";
				
				$cabecalho = array("Ação", "Unidade", "Usuário", "EMail");
				$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
				?>
				</div>
			</fieldset>
		</td>
	</tr>
</table>


