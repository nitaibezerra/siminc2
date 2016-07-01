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
		$contatoEvento 	 = new ContatosEvento( $_POST['cevid'] );
		$dados 			 = $_POST;
		$dados['usucpf'] = $_SESSION['usucpf'];
		$dados['muncod'] = $dados['muncod'] ? $dados['muncod'] : null;
		$cevid 			 = $contatoEvento->popularDadosObjeto( $dados )
			   				 		     ->salvar();
		$db->commit();
		
		if($_REQUEST['acao'] == 'P'){
			die('<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
			 <script>
				alert(\'Operação realizada com sucesso!\');
				var cevid = window.opener.$(\'#cevid option:first\').val();
				if ( cevid == \'\' ){
					window.opener.$(\'#cevid option\').remove();
				}
				var option = $(\'<option>\').text(\'' . $contatoEvento->cevcargo . ' ' . $contatoEvento->cevexpressao . ' ' . $contatoEvento->cevdsc . '\')
							   				.val(\'' . $cevid . '\');
				window.opener.$(\'#cevid\').append( option );	
				window.close();							   			
			 </script>');
		}else{
			die('<script>
					alert(\'Operação realizada com sucesso!\');
					location.href="agenda.php?modulo=sistema/apoio/cadastroContato&acao=A";
				 </script>');
		}
		break;
		
	case 'carregar':
		
		$sql = "SELECT 
				  	cevid, muncod, usucpf, cevdsc, cevexpressao, cevlogradouro, cevcomplemento, 
			        cevnumero, cevcep, cevdddfone, cevfone, cevdddcelular, cevcelular, 
			        cevobs, cevemail, cevdtinclusao, cevstatus, cevcargo
			  	FROM agenda.contatosevento
				WHERE
					cevstatus = 'A'
				and cevid = {$_REQUEST['cevid']}";									  
		
		$dados = $db->pegaLinha( $sql );
		
		extract($dados);
		
		if($muncod){
			$sql = "SELECT estuf FROM territorios.municipio	WHERE muncod = '".$muncod."'";
			$estuf = $db->pegaUm( $sql );
		}
		
		break;
		
	case 'excluir':

		$db->executar("UPDATE agenda.contatosevento SET cevstatus='I' WHERE cevid = {$_REQUEST['cevid']}");
		$db->commit();
		
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href="agenda.php?modulo=sistema/apoio/cadastroContato&acao=A";
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
		<title>Cadastro de Contato</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<body marginheight="0" marginwidth="0">
<?php
}
monta_titulo( 'Cadastro de Contato', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.');
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function validarForm(){
	var errorText = new Array();
	
	if ( $.trim( $('[name=cevdsc]').val() ) == '' ){
		errorText.push('Nome');
	}
	/*
	if ( $.trim( $('[name=cevemail]').val() ) == '' ){
		errorText.push('E-mail');
	}
	
	if ( $.trim( $('[name=cevdddfone]').val() ) == '' || $.trim( $('[name=cevfone]').val() ) == '' ){
		errorText.push('Telefone');
	}
	
	if ( $.trim( $('[name=cevdddcelular]').val() ) == '' || $.trim( $('[name=cevcelular]').val() ) == '' ){
		errorText.push('Celular');
	}
	
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

function alterarContato( cevid ){
	location.href='?modulo=sistema/apoio/cadastroContato&acao=A&op=carregar&cevid=' + cevid;
}

function excluirContato( cevid ){
	if ( confirm('Deseja apagar esse Contato?') ){
		location.href='?modulo=sistema/apoio/cadastroContato&acao=A&op=excluir&cevid=' + cevid;
	}	
}

function pesquisar(){
	$('[name=op]').val('pesquisar');
	$('[name=formulario]').submit();
}

//-->
</script>

<form name="formulario" method="post" action="">
<input type="hidden" name="op" id="op" value="">
<input type="hidden" name="cevid" id="cevid" value="<?php echo $cevid; ?>">

<table class="tabela" align="center">
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td>
		<?=campo_texto("cevdsc","S","S","Descrição",50,100,"","")?>
		</td>
	</tr>
	<?if($_REQUEST['op'] != 'pesquisar'){?>
		<tr>
			<td class="SubTituloDireita">E-mail:</td>
			<td>
			<?=campo_texto("cevemail","N","S","E-mail",50,100,"","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Telefone:<br><font style="font-size: 9px;">(DDD + Telefone)</font></td>
			<td>
			<?=campo_texto("cevdddfone","N","S","DDD",3,2,"##","")?>
			<?=campo_texto("cevfone","N","S","Telefone",9,9,"####-####","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Celular:<br><font style="font-size: 9px;">(DDD + Celular)</font></td>
			<td>
			<?=campo_texto("cevdddcelular","N","S","DDD",3,2,"##","")?>
			<?=campo_texto("cevcelular","N","S","Celular",9,9,"####-####","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Expressão de Tratamento:</td>
			<td>
			<?=campo_texto("cevexpressao","N","S","Expressão de Tratamento",50,100,"","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Cargo:</td>
			<td>
			<?=campo_texto("cevcargo","N","S","Cargo",40,50,"","")?>
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
			<td class="SubTituloDireita">CEP:</td>
			<td>
			<?=campo_texto("cevcep","N","S","CEP",9,9,"#####-###","")?>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita">Logradouro:</td>
			<td>
			<?=campo_texto("cevlogradouro","N","S","Logradouro",60,100,"","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Complemento:</td>
			<td>
			<?=campo_texto("cevcomplemento","N","S","Complemento",60,100,"","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Número:</td>
			<td>
			<?=campo_texto("cevnumero","N","S","Número",9,5,"#####","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Observação:</td>
			<td>
			<?= campo_textarea('cevobs', 'N', 'S', '', 70, 4, 250 ); ?>
			</td>
		</tr>
		<tr bgcolor="#CCCCCC">
		   <td>&nbsp;</td>
		   <td>
		   	<input type="button" name="btalterar" value="Salvar" onclick="validarForm()" class="botao">
		   	<?if ($_REQUEST['acao'] == 'P'){?>
		   		<input type="button" name="btfechar" value="Fechar" onclick="window.close();" class="botao">
		   	<?}else{?>
		   		<input type="button" name="btvoltar" value="Voltar" onclick="location.href='agenda.php?modulo=sistema/apoio/cadastroContato&acao=A';" class="botao">
		   	<?}?>
		   </td>
		</tr> 	
	<?}else{?>
		<tr bgcolor="#CCCCCC">
		   <td>&nbsp;</td>
		   <td>
		   	<input type="button" name="btpesquisar" value="Pesquisar" onclick="pesquisar()" class="botao">
		   	<input type="button" name="btnovo" value="Novo Contato" onclick="location.href='?modulo=sistema/apoio/cadastroContato&acao=A&op=novo';" class="botao">
		   </td>
		</tr> 
	<?}?> 	
</table>


</form>

<?php 

	if($_REQUEST['op'] == 'pesquisar'){
		
		if($_REQUEST['cevdsc']) $filtro = " and cevdsc ilike '%".$_REQUEST['cevdsc']."%' ";
		
		$acao = "'<center>
						<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/alterar.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: alterarContato(\'' || cevid || '\');\"
		 					title=\"Alterar Cadastro Contato\">
		 				<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/excluir.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: excluirContato(\'' || cevid || '\');\"
		 					title=\"Excluir Cadastro Contato\">
	 				  </center>'";
			
			$sql = "SELECT
						$acao AS acao, 
					  	cevdsc||' ' as nome,
					  	cevemail as email, 
					  	'('||cevdddfone||') '||cevfone as fone,
					  	'('||cevdddcelular||') '||cevcelular as celular
					FROM 
					  	agenda.contatosevento
					WHERE
						cevstatus = 'A'
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
