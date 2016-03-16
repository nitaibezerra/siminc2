<?php
ini_set("memory_limit", "128M");

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
	$db->sucesso( 'principal/analiseDadosPTA' );
}

switch ( $_REQUEST["requisicao"] ){
	case 'salvaranalise':
		if( salvaAnaliseDados2011( $_REQUEST ) ){
			echo "<script>alert('Operação realizada com sucesso!');
					window.location.href= window.location;</script>";
			exit();
		} else {
			echo "<script>alert('Falha na operação!');</script>";
		}
	break;
	case 'alteraAnalise':
		 buscaDadosAnaliseDados( $_REQUEST["anaid"] );
	break;
}

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];
$ptrid = $_SESSION["emenda"]["ptridAnalise"];

if( !$_SESSION["emenda"]["ptridAnalise"] ) {
	echo "<script>
			alert('A sessão do Plano de Trabalho foi perdida!');
			window.location.href = 'emenda.php?modulo=principal/listaPtaAnalise&acao=A';
		  </script>";
	die;
}

include_once APPRAIZ . 'includes/workflow.php';
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);

monta_titulo( 'Análise de Dados', $subtitulo_modulo );

if ( $ptrid ){
	echo cabecalhoPlanoTrabalho($ptrid);
	
	$docid = criarDocumento( $ptrid );
	$existePendencias = pegaConclusaoAnaliseDados( $ptrid );
	$admEditavel = buscaResidAdmAnalise( $ptrid, $_SESSION["usucpf"] );
	
	montaVisualizarPTA( $ptrid, false, $boReformulacao );
	
	//montaExecucaoOrcamentaria($ptrid);
}

$tituloAnalise = 'Lista de Vinculação das Unidades Gestoras';
if( $_SESSION['exercicio'] == '2011' ) $tituloAnalise = 'Vinculação das Unidades Gestoras';
?>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script src="../includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>
<script src="/includes/prototype.js"></script>
<script>

	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url){
		
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(560,345);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function displayStaticMessage(messageContent,cssClass){
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(560,250);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
		
		
	}
	
	function closeMessage(){
		messageObj.close();	
	}

	/*function concluirAnaliseDados(){
		if ( confirm( "Ao concluir a Análise, os seus dados não poderão mais ser alterados. Deseja realmente concluir a Análise?" ) ){
			document.getElementById('concluir').value = 'true';
			salvarAnaliseDados();
		}
	}*/

	function alteraAnaliseDados( anaid ){
		//displayMessage('?modulo=principal/popupAnalideDados&acao=A&anaid=' + anaid );
		window.open('?modulo=principal/popupAnalideDados&acao=A&anaid=' + anaid, 'Analise', 'scrollbars=no,height=550,width=850,status=no,toolbar=no,menubar=yes,location=no');
	}
	function excluirAnaliseDados( anaid ){
		window.location.href = 'emenda.php?modulo=principal/analiseDadosPTA&acao=A&anaid='+anaid+'&action=excluir';
	}

	function salvarAnaliseDados(){
		var form = document.getElementById('formulario');
		var input = "";
		var marcadoInput = false;
	
		for(i = 0; i < form.length; i++){
			if( form.elements[i].type == "checkbox" ){
				if( form.elements[i].checked == true ){
					input = form.elements[i].value;
					marcadoInput = true;
				}
			}
		}
		
		var msg = '';
		if( !marcadoInput ){
			msg += 'Selecione uma unidade.\n';
		} 

		if(msg){
			alert(msg);
		} else{
			$('requisicao').value = 'salvaranalise';
			form.submit();
		}
		
	}
</script>
	<?php abrePTAAnaliseRelatorio($ptrid, 'D'); ?>
<form action="" method="post" name="formulario" id="formulario" enctype="multipart/form-data">
	<input type="hidden" name="requisicao" id="requisicao" value="salvaranalise">
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td colspan="2" class="subtitulocentro"><?=$tituloAnalise; ?></td>
		</tr>
		<tr>
			<td>
				<?php 
					$existeExecucao = validaInformacoesGerais($ptrid);//$db->pegaUm("select count(1) from emenda.execucaofinanceira where ptrid = $ptrid and exfstatus = 'A'");
					if(!$existeExecucao){
						echo '<a href="emenda.php?modulo=principal/informacoesGerais&acao=A"><font color="red">Favor preencher as informações da programação orçamentária na aba "Informações Gerais" para prosseguir.</font></a>';
						//$existePendencias = true;
					}
				?>
			</td>
		</tr>
		<tr>
			<?if( $_SESSION['exercicio'] != '2011' ){?>
				<td width="100%" valign="top" style="background: none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
					<br/>
					<?php listaAnaliseDados( $ptrid ); ?>
					<br/>
				</td>
			<?} else { 
					$tituloLabel = 'Unidades que realizarão análises de mérito:';
					if( $_SESSION['exercicio'] == '2011' ) $tituloLabel = 'Unidades que realizarão análises de técnica:';
				?>
				<td style="width: 100%">
					<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
						<tr>
							<td class="subtitulodireita" style="width: 50%"><b><?=$tituloLabel; ?></b></td>
							<td>
							<?php
							 	if( !$boReformulacao )
									$filtro = 'and uniid != 11';
								
								$filtro2011 = array();
								if( $_SESSION['exercicio'] == '2011' ) $filtro2011 = ' and uniid not in (2, 5, 7) ';									
								/*$sql = "SELECT 
											ana.anaid,			
											ana.anasituacaoparecer, 
											ana.anaparecer,
											anx.anxdsc,
											arq.arqid,
											arq.arqnome || '.' || arq.arqextensao as arquivo
										FROM 
											emenda.analise ana
										LEFT JOIN
											emenda.anexo anx ON anx.anaid = ana.anaid
										LEFT JOIN
											public.arquivo arq on anx.arqid = arq.arqid
										WHERE 
											ana.ptrid = {$ptrid}";
								
								$dadosAnalise = $db->pegaLinha( $sql );*/
							
								$sql = "SELECT uniid FROM emenda.analise WHERE ptrid = {$ptrid} and anatipo = 'T'";
							
								$dadosUnidadeAnalise = $db->carregarColuna( $sql );
									
						 		$sql = "SELECT 
										  uniid as codigo,
										  unisigla as descricao
										FROM 
										  emenda.unidades
										WHERE
										  unistatus = 'A' $filtro $filtro2011";
							 	
							 	$arUnidade = $db->carregar($sql);
							 	
							 	echo '<table>';
							 	
							 	$i = 0;
							 	$y = 0;			 	
							 	
							 	foreach ($arUnidade as $chave => $valor) {
							 		
							 		foreach( $dadosUnidadeAnalise as $k=>$v ){
								 		if($valor["codigo"] == $v){
								 			$checked = 'checked';
								 			break;
								 		}else{
								 			$checked = '';
								 		}
							 		}
			
							 		if($i == 0){
							 			echo '<tr>';
							 		}
							 		#7, 5, 2
							 		if($i < 3){
							 			$i++;
							 			echo '<td><input type="checkbox" name="uniid[]" id="uniid[]" value="'.$valor['codigo'].'" '.$checked.'>'.$valor['descricao'].'<td>';
							 		}
							 					 		
							 		if($i == 3){
							 			if($y == 0){
							 				//echo '<td rowspan="3"><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/></td>';
							 				$y = 1;
							 			}			 			
							 			echo '</tr>';
							 			$i = 0;
							 		}
							 	}
							 	
							 	echo '</table>';
						 	?>
							</td>
						</tr>
					</table>
				</td>
			<?} ?>
			<td width="100%" valign="top" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
				<?php
					wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid, 'resid' => '' , 'tipo' => "D" ) );
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2" style="text-align: center">
				<?if( $_SESSION['exercicio'] == '2011' ){ ?>
					<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" style="cursor: pointer;" <?php if ($habilitaAnalise){ echo 'disabled="disabled"'; }else { echo 'onclick="salvarAnaliseDados();"'; } ?> >
				<?} ?>
				<input type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="history.back(-1);">
			</td>
		</tr>
	</table>
</form>