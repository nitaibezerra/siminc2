<?php
//teste de comentario
$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];

$ptrid = $_SESSION["emenda"]["ptridAnalise"];

switch ( $_REQUEST["requisicao"] ){
	case "inativar":
		inativarParecerAnalise( 'T', $ptrid );
	break;
	case "excluirAnexoPorAnaid":
		excluirAnexoPorAnaid();
		exit;
	break;
	case "excluir":
		$sql = "DELETE FROM emenda.analisehistorico WHERE anaid = ".$_GET['anaid'];
		$db->executar( $sql );
		$sql = "DELETE FROM emenda.anexo WHERE anaid = ".$_GET['anaid'];
		$db->executar( $sql );
		$sql = "DELETE FROM emenda.analise WHERE anaid = ".$_GET['anaid'];
		$db->executar( $sql );
		if( $db->commit() ){
			echo '<script>
				alert("Operação Realizada com Sucesso!");
				window.location = "emenda.php?modulo=principal/analiseTecnicaPTA&acao=A";
			  </script>';
		}
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

include_once APPRAIZ . 'includes/workflow.php';
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );


monta_titulo( 'Análise Técnica', $subtitulo_modulo );

if ( $ptrid ){
	
	echo cabecalhoPlanoTrabalho($ptrid, true);
	$docid = criarDocumento( $ptrid );
	$existePendencias = pegaConclusaoAnaliseDados( $ptrid, 'T' );
	
	montaVisualizarPTA( $ptrid, true, $boReformulacao );
	
	/*if( pegarEstadoAtual( $ptrid ) != EM_ANALISE_TECNICA ){
		$habilAnaliseTecnica = false;	
	}else{*/
		$habilAnaliseTecnica = true;
	//}
	
}

?>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script src="../includes/prototype.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
<script language="javascript" type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<?php
	$arFluxo = verificaFluxo( $_SESSION["emenda"]["ptridAnalise"] );
	
	if( $arFluxo ){
		$fluxo = implode(', ', $arFluxo);
	}
?>
</body>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

	jQuery.noConflict();
	jQuery(document).ready(function(){
		jQuery.each([ <?=$fluxo ?> ], function( index, value ) {
			jQuery('#td_acao_'+value).css('display', 'none');
		});
	});
	
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
	
	/*function concluirAnaliseTecnica(){
		if ( confirm( "Ao concluir a Análise, os seus dados não poderão mais ser alterados. Deseja realmente concluir a Análise?" ) ){
			document.getElementById('concluir').value = 'true';
			salvarAnaliseTecnica();
		}
	}*/
	
	function mostraParecer(anaid){
		window.open('?modulo=principal/popupParecer&acao=A&anaid=' + anaid, 'Parecer', 'scrollbars=yes,height=500,width=850,status=no,toolbar=no,menubar=yes,location=no');
	}
	
	function alteraAnaliseTecnica( anaid ){
		//displayMessage('?modulo=principal/popupAnaliseTecnica&acao=A&anaid=' + anaid );
		window.open('?modulo=principal/popupAnaliseTecnica&acao=A&anaid=' + anaid, 'Analise', 'scrollbars=no,height=800,width=1300,status=no,toolbar=no,menubar=yes,location=no,fullscreen=yes,scrollbars=yes');
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
		window.location.href = 'emenda.php?modulo=principal/analiseTecnicaPTA&acao=A&requisicao=inativar';
	}
	function inserirParecerAnalise(){
		window.open('?modulo=principal/popupAnaliseTecnica&acao=A&&acaoTipo=button', 'Analise', 'scrollbars=no,height=500,width=850,status=no,toolbar=no,menubar=yes,location=no,scrollbars=yes');
	}
	function excluirAnalise( anaid ){
		window.location.href = 'emenda.php?modulo=principal/analiseTecnicaPTA&acao=A&requisicao=excluir&anaid='+anaid;
	}
	
</script>
<?php abrePTAAnaliseRelatorio($ptrid, 'T', $boReformulacao); ?>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td colspan="2" class="subtitulocentro">Lista de Análise Técnica</td>
	</tr>
	<tr>
		<td>
			<?php
				$arTipos = pegaTipoReformulacao( $ptrid );
				
				$arCodigo = array();
				foreach ($arTipos as $v) {
					array_push( $arCodigo, $v['codigo'] );
				}
				if( !in_array(1, $arCodigo) ){
					$existeExecucao = validaInformacoesGerais($ptrid);//$db->pegaUm("select count(1) from emenda.execucaofinanceira where ptrid = $ptrid and exfstatus = 'A'");
					$exitePi = false;
					
					if(!$existeExecucao){
						//echo '<a href="emenda.php?modulo=principal/informacoesGerais&acao=A"><font color="red">Favor preencher as informações da programação orçamentária na aba "Informações Gerais" para prosseguir.</font></a>';
						$exitePi = true;
					}
				}
			?>
		</td>
	</tr>
	<tr>
		<td width="100%" valign="top" style="background: none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			<br/>
			<?php listaAnaliseTecnica( $ptrid, $habilAnaliseTecnica ); ?>
			<br/>
		</td>
		<td width="100%" valign="top" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
			<?php
				wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid , 'tipo' => "T" ) );	
			?>
		</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2">
			<input type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="history.back(-1);">
			<?if( in_array( SUPER_USUARIO, $arPerfil ) && in_array($estadoAtual, array(EM_ANALISE_TECNICA, EM_ANALISE_TECNICA_IMPOSITIVO, EM_ANALISE_TECNICA_REFORMULACAO)) ){ ?>
				<input type="button" value="Inativar Parecer" name="btnInativar" id="btnInativar" onclick="inativarParecerAnalise();">
			<?}
			$arrCPF = array('05646593638', '72571659120');
			
			if( in_array( $_SESSION['usucpf'], $arrCPF ) ){?>
				<input type="button" value="Inserir Parecer" name="btnInserir" id="btnInserir" onclick="inserirParecerAnalise();">
			<?} ?>
		</td>
	</tr>
</table>