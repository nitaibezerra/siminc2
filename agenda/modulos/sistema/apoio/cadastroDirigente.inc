<?php

switch ( $_REQUEST['op'] ){
	case 'salvar':
		$dirigente 	 	 = new DirigentesMec( $_POST['drgid'] );
		$dados 			 = $_POST;
		$drgid 			 = $dirigente->popularDadosObjeto( $dados )
			   				 		 ->salvar();
		$db->commit();
		
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href = \'?modulo=sistema/apoio/cadastroDirigente&acao=A\';
			 </script>');
	case 'excluir':
		if ( $_REQUEST['drgid'] ){
			$msg 	   = 'Operação realizada com sucesso!';
			$dirigente = new DirigentesMec( $_REQUEST['drgid'] );
			$dirigente->popularDadosObjeto( array('drgstatus' => 'I') )
				   	  ->salvar();
			$db->commit();
		}else{
			$msg = 'Falha na execução da operação!';
		}	
			
		die('<script>
				alert(\'' . $msg . '\');
				location.href=\'?modulo=sistema/apoio/cadastroDirigente&acao=A\';
			 </script>');
}

if ( $_GET['acao'] == 'E' ){
	if ( empty($_REQUEST['drgid']) ){
		die('<script>
				location.href=\'?modulo=sistema/apoio/cadastroDirigente&acao=A\';
			 </script>');		
	}
	$drgid  = $_REQUEST['drgid'];
	
	$dirigente 	= new DirigentesMec( $drgid );
	$dados  	= $dirigente->getDados();
	extract( $dados );	
}

if ($_REQUEST['acao'] != 'P'){
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
}else{
?>
<html>
	<head>
		<title>Cadastro de Dirigente</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
<body marginheight="0" marginwidth="0">
<?php
}
monta_titulo( 'Cadastro de Dirigente', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.');
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function validarForm(){
	var errorText = new Array();
	
	if ( $.trim( $('[name=aevid]').val() ) == '' ){
		errorText.push('Área');
	}
	
	if ( $.trim( $('[name=drgnome]').val() ) == '' ){
		errorText.push('Nome');
	}
	
	if ( $.trim( $('[name=drgemail]').val() ) == '' ){
		errorText.push('E-mail');
	}
	
	if ( $.trim( $('[name=drgcargo]').val() ) == '' ){
		errorText.push('Cargo');
	}
	
//	if ( $.trim( $('[name=drgdddcelular]').val() ) == '' || $.trim( $('[name=drgcelular]').val() ) == '' ){
//		errorText.push('Celular');
//	}
	
	if ( errorText.length > 0 ){
		errorText = errorText.join("\n");
		errorText = 'Os campos listados abaixo são de preenchimento obrigatório:\n' + errorText;
		alert( errorText );
		return false;
	}
	
	$('[name=op]').val('salvar');
	$('[name=formDirigente]').submit();
}

function alterarDirigente( drgid ){
	location.href='?modulo=sistema/apoio/cadastroDirigente&acao=E&drgid=' + drgid;
}

function excluirDirigente( drgid ){
	if ( confirm('Deseja apagar esse dirigente?') ){
		$('[name=op]').val('excluir');
		$('[name=drgid]').val( drgid );
		$('[name=formDirigente]').submit();
	}	
}

//-->
</script>
<form name="formDirigente" method="post" action="">
<input name="op" id="op" value="" type="hidden">
<input name="drgid" id="drgid" value="<?php echo $drgid; ?>" type="hidden">
<table class="tabela" align="center">
	<tr>
		<td class="SubTituloDireita">Área:</td>
		<td>
		<?php
		$area = new AreaEnvolvida();
		$dados = $area->listaCombo();
		echo $db->monta_combo("aevid", $dados, 'S','Selecione...','', '', '',240,'S','aevid');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td>
		<?=campo_texto("drgnome","S","S","Descrição",60,100,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">E-mail:</td>
		<td>
		<?=campo_texto("drgemail","S","S","E-mail",40,50,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Cargo:</td>
		<td>
		<?=campo_texto("drgcargo","S","S","Cargo",40,50,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Celular:<br><font style="font-size: 9px;">(DDD + Celular)</font></td>
		<td>
		<?=campo_texto("drgdddcelular","N","S","DDD",3,2,"##","")?>
		<?=campo_texto("drgcelular","N","S","Celular",9,9,"####-####","")?>
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	   	<input type="button" name="btsalvar" value="Salvar" onclick="validarForm()" class="botao">
	   	<?php 
	   	if ( $drgid ){
	   	?>
	   		<input type="button" name="btevento" value="Cadastrar Dirigente" onclick="location.href='?modulo=sistema/apoio/cadastroDirigente&acao=A'" class="botao">
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
				<legend>Dirigentes Cadastrados</legend>
				<div style="height:300px; overflow:auto;">
				<?php
				$dirigente = new DirigentesMec();
				$sql 	= $dirigente->listaSQL( );
				
				$cabecalho = array("Ação", "Nome", "Cargo", "E-mail", "Área", "Celular");
				$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
				?>
				</div>
			</fieldset>
		</td>
	</tr>
</table>
<?php 
if ( $_REQUEST['acao'] == 'P' ){
?>	
	</body>
</html>
<?php
}
?>

