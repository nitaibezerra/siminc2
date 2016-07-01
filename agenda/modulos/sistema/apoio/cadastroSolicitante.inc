<?php

switch ( $_REQUEST['op'] ){
	case 'salvar':
		$solicitante 	 = new Solicitante( $_POST['eveid'] );
		$dados 			 = $_POST;
		$dados['usucpf'] = $_SESSION['usucpf'];
		$sltid 			 = $solicitante->popularDadosObjeto( $dados )
			   				 		   ->salvar();
		if ( !empty($_POST['etaid'][0]) ){	
			$db->executar("delete from agenda.solicitanteentidade where sltid = $sltid");
			foreach( $_POST['etaid'] as $etaid ){		   				 		   
				$solicitanteEntidade = new SolicitanteEntidade();
				$dados 				 = array('etaid' => $etaid,
							   				 'sltid' => $sltid);
				$solicitanteEntidade->clearDados()
									->popularDadosObjeto( $dados )
					   				->salvar();
			}				   							   				 			
		}
		$db->commit();
		
		if($_REQUEST['acao'] == 'P'){
			die('<script>
					alert(\'Operação realizada com sucesso!\');
					window.opener.$(\'#sltid_dsc\').val(\'' . $solicitante->sltcargo . ' ' . $solicitante->sltexpressao . ' ' . $solicitante->sltdsc . '\');
					window.opener.$(\'#sltid\').val(\'' . $sltid . '\');
					window.opener.carregaEntidadeBySolicitante(\'' . $sltid . '\');
					window.close();
				 </script>');
		}else{
			die('<script>
					alert(\'Operação realizada com sucesso!\');
					location.href="agenda.php?modulo=sistema/apoio/cadastroSolicitante&acao=A";
				 </script>');
		}
		break;
		
	case 'carregar':
		
		$sql = "SELECT 
				  	sltid, usucpf, sltdsc, sltdddfone, sltfone, sltdddcelular, sltcelular, sltexpressao, 
				  	sltcargo, sltemail, sltobs
				FROM 
				  	agenda.solicitante
				WHERE
					sltstatus = 'A'
				and sltid = {$_REQUEST['sltid']}";									  
		
		$dados = $db->pegaLinha( $sql );
		
		extract($dados);
		break;
		
	case 'excluir':
		$db->executar("delete from agenda.solicitanteentidade where sltid = {$_REQUEST['sltid']}");
		$db->executar("UPDATE agenda.solicitante SET sltstatus='I' WHERE sltid = {$_REQUEST['sltid']}");
		
		$db->commit();
		
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href="agenda.php?modulo=sistema/apoio/cadastroSolicitante&acao=A";
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
		<title>Cadastro de Solicitante</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<body marginheight="0" marginwidth="0">
<?php
}
monta_titulo( 'Cadastro de Solicitante', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.');
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function validarForm(){
	var errorText = new Array();
	
	if ( $.trim( $('[name=sltdsc]').val() ) == '' ){
		errorText.push('Nome');
	}
	/*
	if ( $.trim( $('[name=sltemail]').val() ) == '' ){
		errorText.push('E-mail');
	}
	
	if ( $.trim( $('[name=sltexpressao]').val() ) == '' ){
		errorText.push('Expressão de Tratamento');
	}
	*/
	
	if ( $.trim( $('[name=sltcargo]').val() ) == '' ){
		errorText.push('Cargo');
	}
	/*
	if ( $.trim( $('[name=sltdddfone]').val() ) == '' || $.trim( $('[name=sltfone]').val() ) == '' ){
		errorText.push('Telefone');
	}
	
	if ( $.trim( $('[name=sltdddcelular]').val() ) == '' || $.trim( $('[name=sltcelular]').val() ) == '' ){
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

function alterarSolicitante( sltid ){
	location.href='?modulo=sistema/apoio/cadastroSolicitante&acao=A&op=carregar&sltid=' + sltid;
}

function excluirSolicitante( sltid ){
	if ( confirm('Deseja apagar esse Solicitante?') ){
		location.href='?modulo=sistema/apoio/cadastroSolicitante&acao=A&op=excluir&sltid=' + sltid;
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
<input name="sltid" id="sltid" value="<?php echo $sltid; ?>" type="hidden">
<table class="tabela" align="center">
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td>
		<?=campo_texto("sltdsc","S","S","Descrição",50,100,"","")?>
		</td>
	</tr>
	<?
	if($_REQUEST['op'] != 'pesquisar'){?>
		<tr>
			<td class="SubTituloDireita">Cargo:</td>
			<td>
			<?=campo_texto("sltcargo","S","S","Cargo",50,100,"","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">E-mail:</td>
			<td>
			<?=campo_texto("sltemail","N","S","E-mail",50,100,"","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Expressão de Tratamento:</td>
			<td>
			<?=campo_texto("sltexpressao","N","S","Expressão de Tratamento",50,100,"","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Telefone:<br><font style="font-size: 9px;">(DDD + Telefone)</font></td>
			<td>
			<?=campo_texto("sltdddfone","N","S","DDD",3,3,"###","")?>
			<?=campo_texto("sltfone","N","S","Telefone",9,9,"####-####","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Celular:<br><font style="font-size: 9px;">(DDD + Celular)</font></td>
			<td>
			<?=campo_texto("sltdddcelular","N","S","DDD",3,3,"###","")?>
			<?=campo_texto("sltcelular","N","S","Celular",9,9,"####-####","")?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Observação:</td>
			<td>
			<?= campo_textarea('sltobs', 'N', 'S', '', 72, 5, 255 ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">Vincular Entidade/Pessoa</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Entidade/Pessoa:</td>
			<td>
			<?php
			$entidade = new EntidadeAgenda();
			$sql 	  = $entidade->listaComboPopupSQL();
			//$etaid 	  = $entidade->carregaCampoPopup( $etaid );
			if($sltid){
				$sql = "SELECT 
						e.etaid AS codigo,
						e.etadsc AS descricao
					FROM
						agenda.entidadeagenda e
					inner join agenda.solicitanteentidade s on s.etaid = e.etaid
					WHERE
						s.sltid = $sltid";
		
				$etaid = $db->carregar( $sql );
			}
			
			combo_popup('etaid', $sql, 'Entidade/Pessoa', "400x400", 0, array(), "", "S", false, false, 5, 400 );
			?>
			
			&nbsp;&nbsp;&nbsp;
			<a href="javascript:janela('?modulo=sistema/apoio/cadastroEntidade&acao=P',600,530,'popupcadastro');">[Nova Entidade/Pessoa]</a>
			</td>
		</tr>
		<tr bgcolor="#CCCCCC">
		   <td>&nbsp;</td>
		   <td>
		   	<input type="button" name="btalterar" value="Salvar" onclick="validarForm()" class="botao">
		   	<?if ($_REQUEST['acao'] == 'P'){?>
		   		<input type="button" name="btfechar" value="Fechar" onclick="window.close();" class="botao">
		   	<?}else{?>
		   		<input type="button" name="btvoltar" value="Voltar" onclick="location.href='agenda.php?modulo=sistema/apoio/cadastroSolicitante&acao=A';" class="botao">
		   	<?}?>
		   </td>
		</tr> 	
	<?}else{?>
		<tr bgcolor="#CCCCCC">
		   <td>&nbsp;</td>
		   <td>
		   	<input type="button" name="btpesquisar" value="Pesquisar" onclick="pesquisar()" class="botao">
		   	<input type="button" name="btnovo" value="Novo Solicitante" onclick="location.href='?modulo=sistema/apoio/cadastroSolicitante&acao=A&op=novo';" class="botao">
		   </td>
		</tr> 
	<?}?>
</table>
</form>

<?php 

	if($_REQUEST['op'] == 'pesquisar'){
		
		if($_REQUEST['sltdsc']) $filtro = " and sltdsc ilike '%".$_REQUEST['sltdsc']."%' ";
		
		$acao = "'<center>
						<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/alterar.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: alterarSolicitante(\'' || sltid || '\');\"
		 					title=\"Alterar Cadastro Solicitante\">
		 				<img
		 					align=\"absmiddle\"
		 					src=\"/imagens/excluir.gif\"
		 					style=\"cursor: pointer\"
		 					onclick=\"javascript: excluirSolicitante(\'' || sltid || '\');\"
		 					title=\"Excluir Cadastro Solicitante\">
	 				  </center>'";
			
			$sql = "SELECT
						$acao AS acao, 
					  	sltdsc as nome,
					  	sltcargo as cargo, 
					  	sltemail as email, 
					  	'('||sltdddfone||') '||sltfone as fone,
					  	'('||sltdddcelular||') '||sltcelular as celular
					FROM 
					  	agenda.solicitante
					WHERE
						sltstatus = 'A'
						$filtro
					order by 2";									  
			
		$cabecalho = array("Ação", "Nome", "Cargo", "E-mail", "Telefone", "Celular");
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
