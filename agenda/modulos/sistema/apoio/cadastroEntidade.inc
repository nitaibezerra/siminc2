<?php
switch ($_REQUEST['ajax']){
	case 'municipio':
		header('content-type: text/html; charset=ISO-8859-1');
		
		$estuf = $_POST['estuf'];
		$sql = "SELECT 
					muncod AS codigo, 
					mundescricao AS descricao 
				FROM
					territorios.municipio
				WHERE
					estuf = '{$estuf}'
				ORDER BY
					mundescricao";
		echo $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', 240, 'S', 'muncod');
		exit;		
		break;
}

switch ( $_REQUEST['op'] ){
	case 'salvar':
		$entidade 	 	 = new EntidadeAgenda( $_POST['eveid'] );
		$dados 			 = $_POST;
		$dados['usucpf'] = $_SESSION['usucpf'];
		$dados['muncod'] = $dados['muncod'] ? $dados['muncod'] : null;
		$etaid 			 = $entidade->popularDadosObjeto( $dados )
			   				 		->salvar();
		
		$arSltid			 = ($_POST['sltid'][0] ? $_POST['sltid'] : array());			   				 		
		$sltid 				 = $_GET['sltid'];
		if ($sltid){
			$arSltid[] = $sltid; 	
		}
		$arSltid = array_unique( $arSltid );
		if ( count($arSltid) ){
			$solicitanteEntidade = new SolicitanteEntidade();
			foreach ($arSltid as $sltid){
				$dados 				 = array('etaid' => $etaid,
							   				 'sltid' => $sltid);
				$solicitanteEntidade->clearDados()
									->popularDadosObjeto( $dados )
					   				->salvar();
			}	   							   				 		
		}			   				 		
		$db->commit();
		
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				window.opener.$(\'#etaid_dsc\').val(\'' . $entidade->etadsc . '\');
				window.opener.$(\'#etaid\').val(\'' . $etaid . '\');
				window.close();
			 </script>');
	case 'excluir':
//		if ( $_REQUEST['eveid'] ){
//			$msg 	= 'Operação realizada com sucesso!';
//			$evento = new Evento( $_REQUEST['eveid'] );
//			$evento->popularDadosObjeto( array('evestatus' => 'I') )
//				   ->salvar();
//			$db->commit();
//		}else{
//			$msg = 'Falha na execução da operação!';
//		}	
//			
//		die('<script>
//				alert(\'' . $msg . '\');
//				location.href=\'?modulo=principal/cadastroEvento&acao=A\';
//			 </script>');
}

if ($_REQUEST['acao'] != 'P'){
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
}else{
?>
<html>
	<head>
		<title>Cadastro de Solicitante</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<body marginheight="0" marginwidth="0">
<?php
}
monta_titulo( 'Cadastro de Entidade/Pessoa', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.');
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function carregarMunicipio( estuf ){
					var td	= $('#td_municipio');
					if ( estuf != '' ){
						var url = location.href;
						$.ajax({
									  url  		 : url,
									  type 		 : 'post',
									  data 		 : {ajax  : 'municipio', 
									  		  	    estuf : estuf},
									  dataType   : "html",
									  async		 : false,
									  beforeSend : function (){
									  	divCarregando();
										td.find('select option:first').attr('selected', true);
									  },
									  error 	 : function (){
									  	divCarregado();
									  },
									  success	 : function ( data ){
									  	td.html( data );
									  	divCarregado();
									  }
								});	
					}else{
						td.find('select option:first').attr('selected', true);
						td.find('select').attr('selected', true)
										 .attr('disabled', true);
					}			
}

function validarForm(){
	var errorText = new Array();
	
	if ( $.trim( $('[name=etadsc]').val() ) == '' ){
		errorText.push('Nome');
	}
	
	/*
	if ( $.trim( $('[name=estuf]').val() ) == '' ){
		errorText.push('UF');
	}
	
	if ( $.trim( $('[name=muncod]').val() ) == '' ){
		errorText.push('Município');
	}
	*/
	
	if ( errorText.length > 0 ){
		errorText = errorText.join("\n");
		errorText = 'Os campos listados abaixo são de preenchimento obrigatório:\n' + errorText;
		alert( errorText );
		return false;
	}
	
	$('#sltid option').attr('selected', true);
	$('[name=op]').val('salvar');
	$('[name=formulario]').submit();
}

//-->
</script>
<form name="formulario" method="post" action="">
<input name="op" id="op" value="" type="hidden">
<input name="etaid" id="etaid" value="<?php echo $etaid; ?>" type="hidden">
<table class="tabela" align="center">
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td>
		<?=campo_texto("etadsc","S","S","Descrição",60,100,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">E-mail:</td>
		<td>
		<?=campo_texto("etaemail","N","S","E-mail",40,100,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">UF:</td>
		<td>
		<?
		$sql = "SELECT
					estuf AS codigo,
					estuf || ' - ' || estdescricao AS descricao
				FROM
					territorios.estado
				ORDER BY
					estuf";
		?>
		<?=$db->monta_combo("estuf", $sql, 'S','Selecione...','carregarMunicipio', '', '',240,'N','estuf'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Município:</td>
		<td id="td_municipio">
		<?=$db->monta_combo("muncod", array(), 'N','Selecione...','', '', '',240,'N','muncod'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">CEP:</td>
		<td>
		<?=campo_texto("etacep","N","S","CEP",9,9,"#####-###","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Logradouro:</td>
		<td>
		<?=campo_texto("etalogradouro","N","S","Logradouro",60,100,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Complemento:</td>
		<td>
		<?=campo_texto("etalogracomplemento","N","S","Complemento",60,100,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Número:</td>
		<td>
		<?=campo_texto("etalogranumero","N","S","Número",9,5,"#####","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Telefone:<br><font style="font-size: 9px;">(DDD + Telefone)</font></td>
		<td>
		<?=campo_texto("etadddfone","N","S","DDD",3,3,"###","")?>
		<?=campo_texto("etafone","N","S","Telefone",9,9,"####-####","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Celular:<br><font style="font-size: 9px;">(DDD + Celular)</font></td>
		<td>
		<?=campo_texto("etadddcelular","N","S","DDD",3,3,"###","")?>
		<?=campo_texto("etacelular","N","S","Celular",9,9,"####-####","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Observação:</td>
		<td>
		<?= campo_textarea('etaobs', 'N', 'S', '', 82, 5, 255 ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Vincular Solicitante</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Solicitante:</td>
		<td>
		<?php
		$solicitante = new Solicitante();
		$sql 	  = $solicitante->listaComboPopupSQL();
//		$etaid 	  = $entidade->carregaCampoPopup( $etaid );
		
		combo_popup('sltid', $sql, 'Solicitante', "400x400", 0, array(), "", "S", false, false, 5, 400 );
		?>
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	   	<input type="button" name="btsalvar" value="Salvar" onclick="validarForm()" class="botao">
	   </td>
	</tr> 	
</table>
</form>
<?php 
if ( $_REQUEST['acao'] == 'P' ){
?>	
	</body>
</html>
<?php
}
?>

