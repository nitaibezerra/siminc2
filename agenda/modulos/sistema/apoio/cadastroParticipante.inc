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
		echo $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', 240, 'N', 'muncod');
		exit;		
}

switch ( $_REQUEST['op'] ){
	case 'salvar':
		$participante 	 = new Participantes( $_POST['prtid'] );
		$dados 			 = $_POST;
		$dados['usucpf'] = $_SESSION['usucpf'];
		$dados['muncod'] = $dados['muncod'] ? $dados['muncod'] : null;
		$prtid 			 = $participante->popularDadosObjeto( $dados )
			   				 		    ->salvar();
		$db->commit();
		
		
		if($_REQUEST['acao'] == 'P'){
			die('<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
			 <script>
				alert(\'Operação realizada com sucesso!\');
				var prtid = window.opener.$(\'#prtid option:first\').val();
				if ( prtid == \'\' ){
					window.opener.$(\'#prtid option\').remove();
				}
				var option = $(\'<option>\').text(\'' . $participante->prtdsc . '\')
							   				.val(\'' . $prtid . '\');
				window.opener.$(\'#prtid\').append( option );	
				window.close();							   			
			 </script>');
		}else{
			die('<script>
					alert(\'Operação realizada com sucesso!\');
					location.href="agenda.php?modulo=sistema/apoio/cadastroParticipante&acao=A";
				 </script>');
		}
		break;
		
	case 'carregar':
		
		$sql = "SELECT 
				  	prtid, muncod, usucpf, prtdsc, prtdddfonecontato, prtfonecontato, 
				    prtemailcontato, prtdddcelularcontato, prtcelularcontato, prtdtinclusao, prtstatus
				FROM agenda.participantes
				WHERE
					prtstatus = 'A'
				and prtid = {$_REQUEST['prtid']}";									  
		
		$dados = $db->pegaLinha( $sql );
		
		extract($dados);
		
		if($muncod){
			$sql = "SELECT estuf FROM territorios.municipio	WHERE muncod = '".$muncod."'";
			$estuf = $db->pegaUm( $sql );
		}
		
		break;
		
	case 'excluir':

		$db->executar("UPDATE agenda.participantes SET prtstatus='I' WHERE prtid = {$_REQUEST['prtid']}");
		$db->commit();
		
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href="agenda.php?modulo=sistema/apoio/cadastroParticipante&acao=A";
			 </script>');
		break;
	
	case 'novo':
		$_REQUEST['op'] = 'novo';
		break;
	default:
		$_REQUEST['op'] = 'pesquisar';
		break;
		
}


if ($_REQUEST['acao'] != 'P'){
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
}else{
?>
<html>
	<head>
		<title>Cadastro de Participante</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<body marginheight="0" marginwidth="0">
<?php
}
monta_titulo( 'Cadastro de Participante', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.');
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function validarForm(){
	var errorText = new Array();
	
	if ( $.trim( $('[name=prtdsc]').val() ) == '' ){
		errorText.push('Nome');
	}
	
	/*
	if ( $.trim( $('[name=prtemailcontato]').val() ) == '' ){
		errorText.push('E-mail');
	}
	
	if ( $.trim( $('[name=estuf]').val() ) == '' ){
		errorText.push('UF');
	}
	
	if ( $.trim( $('[name=muncod]').val() ) == '' ){
		errorText.push('Município');
	}
	
	if ( $.trim( $('[name=prtdddfonecontato]').val() ) == '' || $.trim( $('[name=prtfonecontato]').val() ) == '' ){
		errorText.push('Telefone');
	}
	
	if ( $.trim( $('[name=prtdddcelularcontato]').val() ) == '' || $.trim( $('[name=prtcelularcontato]').val() ) == '' ){
		errorText.push('Celular');
	}
	*/
	
	if ( errorText.length > 0 ){
		errorText = errorText.join("\n");
		errorText = 'Os campos listados abaixo são de preenchimento obrigatório:\n' + errorText;
		alert( errorText );
		return false;
	}
	
	$('#etaid option').attr('selected', true);
	$('[name=op]').val('salvar');
	$('[name=formulario]').submit();
}

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

function alterarParticipante( prtid ){
	location.href='?modulo=sistema/apoio/cadastroParticipante&acao=A&op=carregar&prtid=' + prtid;
}

function excluirParticipante( prtid ){
	if ( confirm('Deseja apagar esse Participante?') ){
		location.href='?modulo=sistema/apoio/cadastroParticipante&acao=A&op=excluir&prtid=' + prtid;
	}	
}

function pesquisar(){
	$('[name=op]').val('pesquisar');
	$('[name=formulario]').submit();
}

//-->
</script>

<form name="formulario" method="post" action="">
<input name="op" id="op" value="" type="hidden">
<input type="hidden" name="prtid" id="prtid" value="<?php echo $prtid; ?>">

<table class="tabela" align="center">
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td>
		<?=campo_texto("prtdsc","S","S","Descrição",50,100,"","")?>
		</td>
	</tr>
	<?if($_REQUEST['op'] != 'pesquisar'){?>
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
			<?
				if($estuf){
					$sql = "SELECT 
						muncod AS codigo, 
						mundescricao AS descricao 
					FROM
						territorios.municipio
					WHERE
						estuf = '{$estuf}'
					ORDER BY
						mundescricao";
					$habilitaMun = 'S';
				}
				else{
					$sql = array();
					$habilitaMun = 'N';
				}
			?>
			<?=$db->monta_combo("muncod", $sql, $habilitaMun,'Selecione...','', '', '',240,'N','muncod'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">E-mail:</td>
			<td>
			<?=campo_texto("prtemailcontato","N","S","E-mail",50,100,"","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Telefone:<br><font style="font-size: 9px;">(DDD + Telefone)</font></td>
			<td>
			<?=campo_texto("prtdddfonecontato","N","S","DDD",3,2,"##","")?>
			<?=campo_texto("prtfonecontato","N","S","Telefone",9,9,"####-####","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Celular:<br><font style="font-size: 9px;">(DDD + Celular)</font></td>
			<td>
			<?=campo_texto("prtdddcelularcontato","N","S","DDD",3,2,"##","")?>
			<?=campo_texto("prtcelularcontato","N","S","Celular",9,9,"####-####","")?>
			</td>
		</tr>
		<tr bgcolor="#CCCCCC">
		   <td>&nbsp;</td>
		   <td>
		   	<input type="button" name="btalterar" value="Salvar" onclick="validarForm()" class="botao">
		   	<?if ($_REQUEST['acao'] == 'P'){?>
		   		<input type="button" name="btfechar" value="Fechar" onclick="window.close();" class="botao">
		   	<?}else{?>
		   		<input type="button" name="btvoltar" value="Voltar" onclick="location.href='agenda.php?modulo=sistema/apoio/cadastroParticipante&acao=A';" class="botao">
		   	<?}?>
		   </td>
		</tr> 	
	<?}else{?>
		<tr bgcolor="#CCCCCC">
		   <td>&nbsp;</td>
		   <td>
		   	<input type="button" name="btpesquisar" value="Pesquisar" onclick="pesquisar()" class="botao">
		   	<input type="button" name="btnovo" value="Novo Participante" onclick="location.href='?modulo=sistema/apoio/cadastroParticipante&acao=A&op=novo';" class="botao">
		   </td>
		</tr> 
	<?}?> 	
</table>


</form>

<?php 

	if($_REQUEST['op'] == 'pesquisar'){
		
		if($_REQUEST['prtdsc']) $filtro = " and prtdsc ilike '%".$_REQUEST['prtdsc']."%' ";
		
		$acao = "'<center>
						<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/alterar.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: alterarParticipante(\'' || prtid || '\');\"
		 					title=\"Alterar Cadastro Participante\">
		 				<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/excluir.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: excluirParticipante(\'' || prtid || '\');\"
		 					title=\"Excluir Cadastro Participante\">
	 				  </center>'";
			
			$sql = "SELECT
						$acao AS acao, 
					  	prtdsc||' ' as nome,
					  	prtemailcontato as email, 
					  	'('||prtdddfonecontato||') '||prtfonecontato as fone,
					  	'('||prtdddcelularcontato||') '||prtcelularcontato as celular
					FROM 
					  	agenda.participantes
					WHERE
						prtstatus = 'A'
						$filtro
					order by 2";									  
			
		$cabecalho = array("Ação", "Nome", "E-mail", "Telefone", "Celular");
		$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
	}
?>
	
<?php 
if ( $_REQUEST['acao'] == 'P' ){
?>	
	</body>
</html>
<?php
}
?>
