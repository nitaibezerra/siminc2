<?php

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];
$ptrid = $_SESSION["emenda"]["ptridAnalise"];

$arPerfil = pegaPerfilArray( $_SESSION["usucpf"] );
if( in_array( SUPER_USUARIO, $arPerfil ) && !empty($_REQUEST['anaid']) && $_REQUEST['action'] == 'excluir' ){
	$sql = "DELETE FROM emenda.anexo WHERE anaid = ".$_REQUEST['anaid'];
	$db->executar( $sql );
	$sql = "UPDATE emenda.analise set anaidpai = null WHERE anaid = ".$_REQUEST['anaid'];
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.analisehistorico WHERE anaid = ".$_REQUEST['anaid'];
	$db->executar( $sql );
	$sql = "DELETE FROM emenda.analise WHERE anaid = ".$_REQUEST['anaid'];
	$db->executar( $sql );
	$db->commit();
	$db->sucesso( 'principal/analiseMeritoPTA' );
}

switch ( $_REQUEST["requisicao"] ){
	case "inativar":
		inativarParecerAnalise( 'M', $ptrid );
	case "excluirAnexoPorAnaid":
		excluirAnexoPorAnaid();
		exit;
	break;
}

if ( !$ptrid ){
	echo '<script>
			alert("A sessão do Plano de Trabalho foi perdida!");
			window.location = "emenda.php?modulo=principal/analiseDadosPTA&acao=A";
		  </script>';
	die;
}

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

include_once APPRAIZ . 'includes/workflow.php';
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );

$arMnuid = array();
if( !$boReformulacao && $estadoAtual != EM_SOLICITACAO_REFORMULACAO ){
	$arMnuid = array('6254');
} else {
	if( $boReformulacao ){
		$arMnuid = array('5501', '5499');
	}
}

$db->cria_aba( $abacod_tela, $url, '' , $arMnuid);

monta_titulo( 'Análise de Mérito', $subtitulo_modulo );

if ( $ptrid ){
	
	echo cabecalhoPlanoTrabalho($ptrid, true);
	$docid = criarDocumento( $ptrid );
	$existePendencias = pegaConclusaoAnaliseDados( $ptrid, 'M' );
	
	montaVisualizarPTA( $ptrid, false, $boReformulacao );
	
	if( !in_array($estadoAtual, array(EM_ANALISE_DE_MERITO, EM_ANALISE_DE_MERITO_IMPOSITIVO))  ){
		$habilAnaliseMerito = false;	
	}else{
		$habilAnaliseMerito = true;
	}
	
}

?>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script src="../includes/prototype.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>

<script language="JavaScript">
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url){
		
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(520,340);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function displayStaticMessage(messageContent,cssClass){
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(520,250);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
		
		
	}
	
	function closeMessage(){
		messageObj.close();	
	}

	/*function concluirAnaliseMerito(){
		if ( confirm( "Ao concluir a Análise, os seus dados não poderão mais ser alterados. Deseja realmente concluir a Análise?" ) ){
			document.getElementById('concluir').value = 'true';
			salvarAnaliseMerito();
		}
	}*/
	
	function alteraAnaliseMerito( anaid ){
		//displayMessage('?modulo=principal/popupAnalideMerito&acao=A&anaid=' + anaid );
		window.open('?modulo=principal/popupAnalideMerito&acao=A&anaid=' + anaid, 'Analise', 'scrollbars=no,height=450,width=850,status=no,toolbar=no,menubar=yes,location=no');
	}
	
	function excluirAnexoAnaid( anaid, arqid ){
		if(confirm("Deseja realmente excluir este anexo?")){
			var myAjax = new Ajax.Request(
				window.location.href,
				{
					method: 'post',
					parameters: "requisicao=excluirAnexoPorAnaid&anaid=" + anaid + "&arqid=" + arqid,
					asynchronous: false,
					onComplete: function(resp){
						document.getElementById('div_anexo').innerHTML = '<input type="file" name="arquivo" id="arquivo" />';
						document.getElementById('anxdsc').value = '';
						if(document.getElementById('img_anexo_' + arqid)){
							document.getElementById('img_anexo_' + arqid).style.display = 'none';
						}
					}
			});
		}
	}
	function inativarParecerAnalise(){
		window.location.href = 'emenda.php?modulo=principal/analiseMeritoPTA&acao=A&requisicao=inativar';
	}
	function excluirAnaliseDados( anaid ){
		window.location.href = 'emenda.php?modulo=principal/analiseMeritoPTA&acao=A&anaid='+anaid+'&action=excluir';
	}
	
</script>
<?php abrePTAAnaliseRelatorio($ptrid, 'M'); ?>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td colspan="2" class="subtitulocentro">Lista de Análise de Mérito</td>
	</tr>
	<tr>
		<td>
			<?php 
				$existeExecucao = validaInformacoesGerais($ptrid);//$db->pegaUm("select count(1) from emenda.execucaofinanceira where ptrid = $ptrid and exfstatus = 'A'");

				$exitePi = false;
				if(!$existeExecucao){
					echo '<a href="emenda.php?modulo=principal/informacoesGerais&acao=A"><font color="red">Favor preencher as informações da programação orçamentária na aba "Informações Gerais" para prosseguir.</font></a>';
					$exitePi = true;
				}
			?>
		</td>
	</tr>
	<tr>
		<td width="100%" valign="top" style="background: none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			<br/>
			<?php listaAnaliseMerito( $ptrid, $habilAnaliseMerito ); ?>
			<br/>
		</td>
		<td width="100%" valign="top" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			<?php 
				wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid , 'tipo' => "M" ) );
			?>
		</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2">
			<input type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="history.back(-1);">
			<?if( (in_array( ADMINISTRADOR_INST, $arPerfil ) || in_array( SUPER_USUARIO, $arPerfil )) && ($estadoAtual == EM_ANALISE_DE_MERITO || $estadoAtual != EM_ANALISE_DE_MERITO_IMPOSITIVO) ){ ?>
			<input type="button" value="Inativar Parecer" name="btnInativar" id="btnInativar" onclick="inativarParecerAnalise();">
			<?} ?>
		</td>
	</tr>
</table>