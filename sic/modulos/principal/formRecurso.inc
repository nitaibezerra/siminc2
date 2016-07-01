<?php 

if($_REQUEST['requisicao'] == 'salvaDados'){
	
	if($_REQUEST['slcpergunta1instancia']){
		$arUpdate[] = "slcpergunta1instancia = '{$_REQUEST['slcpergunta1instancia']}'";
	}
	
	if($_REQUEST['slcpergunta2instancia']){
		$arUpdate[] = "slcpergunta2instancia = '{$_REQUEST['slcpergunta2instancia']}'";
	}
	
	if($_REQUEST['slcresposta1instancia']){
		$arUpdate[] = "slcresposta1instancia = '{$_REQUEST['slcresposta1instancia']}'";
	}
	
	if($_REQUEST['slcresposta2instancia']){
		$arUpdate[] = "slcresposta2instancia = '{$_REQUEST['slcresposta2instancia']}'";
	}
	
	if(is_array($arUpdate)){
		$sql = "update sic.solicitacao set
					".implode(',', $arUpdate)."
				where slcid = {$_SESSION['sic']['slcid']}";

		$db->executar($sql);
		
		if($db->commit()){
			$db->sucesso('principal/formRecurso');
		}
	}		
}

if($_SESSION['sic']['slcid']){
	
	$sql = "select
				slcpergunta1instancia,
				slcpergunta2instancia,
				slcresposta1instancia,
				slcresposta2instancia
			from
				sic.solicitacao
			where 
				slcid = {$_SESSION['sic']['slcid']}";
	
	$rs = $db->pegaLinha($sql);
	
	if($rs) extract($rs);
}

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";

$slcid = $_SESSION['sic']['slcid'] ? $_SESSION['sic']['slcid'] : $_REQUEST['slcid'];

if(!$_SESSION['sic']['slcid']){
	$_SESSION['sic']['slcid'] = $slcid; 
}

// Recupera dados do workflow
$docid = criaDocumento($slcid);
$esdid = pegaEstadoAtual($docid);

$boAtivo = 'S';
if(in_array($esdid, array(WF_ESDID_FINALIZADO_1_INSTANCIA,WF_ESDID_FINALIZADO_2_INSTANCIA))){
	$boAtivo = 'N';	
}

if( checkPerfil(array(SIC_PERFIL_ORGAO_CONTROLE)) ){
	$boAtivo = 'N';
}

echo "<br/>";
$titulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo('Recurso ao Pedido de Acesso à Informação', $titulo2);
echo "<br/>";

$abacod_tela = 57548;

$db->cria_aba( $abacod_tela, $url, '');

if(in_array($esdid,array(WF_ESDID_RECURSO_1_INSTANCIA,WF_ESDID_ANALISE_1_RECURSO))){
	$stTitulo = "Recurso 1º Instância";
}else if(in_array($esdid,array(WF_ESDID_RECURSO_2_INSTANCIA,WF_ESDID_ANALISE_2_RECURSO))){
	$stTitulo = "Recurso 2º Instância";
}else{
	$stTitulo = "Recurso";
}

?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script>
	$(function(){
		
		jQuery("[name=slcpergunta1instancia]").addClass("required");
		jQuery("[name=slcresposta1instancia]").addClass("required");	
		jQuery("[name=slcpergunta2instancia]").addClass("required");
		jQuery("[name=slcresposta2instancia]").addClass("required");
			
		jQuery("#formulario_id").validate();

		$('#bt_salvar').click(function(){
			$('#requisicao_id').val('salvaDados');
			$('#formulario_id').submit();
		});
		
	});
</script>
<form name="formulario" id="formulario_id" method="post" action="">	
	<input type="hidden" name="slcid" id="slcid" value="<?php echo $slcid ? $slcid : '' ?>" />	
	<input type="hidden" name="requisicao" id="requisicao_id" value="" />
	<input type="hidden" name="proximo_dia_util" id="proximo_dia_util_id" value="" />
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td height="25" class="subtituloCentro" colspan="2"><?php echo $stTitulo; ?></td>		
			<td rowspan="10" width="85" valign="top" align="center">								
				<?php
				$oculta = true;
				if(checkPerfil(array(SIC_PERFIL_ADMINISTRADOR))){
					$oculta = false;
				}
				wf_desenhaBarraNavegacao( $docid , array('slcid' => $slcid),  array('historico'=>$oculta) );
				?>
			</td>
		</tr>
		
		<?php if($esdid == WF_ESDID_RECURSO_1_INSTANCIA): ?>		
			<tr>
				<td width="140" class="subtituloDireita">Pergunta&nbsp;</td>
				<td>
					<?php 
					echo campo_textarea('slcpergunta1instancia', 'S', $boAtivo, '', 74, 15, 4000);
					?>
				</td>
			</tr>
		<?php endif; ?>
		
		<?php if($esdid == WF_ESDID_RECURSO_2_INSTANCIA): ?>
			<tr>
				<td width="140" class="subtituloDireita">Pergunta&nbsp;</td>
				<td>
					<?php 
					echo campo_textarea('slcpergunta2instancia', 'S', $boAtivo, '', 74, 15, 4000);
					?>
				</td>
			</tr>
		<?php endif; ?>
		
		<?php if($esdid == WF_ESDID_ANALISE_1_RECURSO): ?>
			<tr>
				<td width="140" class="subtituloDireita">Pergunta&nbsp;</td>
				<td valign="top">
					<?php //echo wordwrap($slcpergunta1instancia, 150, '<br/>', 1); ?>
					<?php echo $slcpergunta1instancia; ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">Resposta&nbsp;</td>
				<td>
					<?php 
					echo campo_textarea('slcresposta1instancia', 'S', $boAtivo, '', 88, 17, 4000);
					?>
				</td>
			</tr>
		<?php endif; ?>
		
		<?php if($esdid == WF_ESDID_ANALISE_2_RECURSO): ?>
			<tr>
				<td width="140" class="subtituloDireita">Pergunta&nbsp;</td>
				<td valign="top">
					<?php echo $slcpergunta2instancia; ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">Resposta&nbsp;</td>
				<td>
					<?php 
					echo campo_textarea('slcresposta2instancia', 'S', $boAtivo, '', 88, 17, 4000);
					?>
				</td>
			</tr>
		<?php endif; ?>
		
		<?php if(in_array($esdid,array(WF_ESDID_FINALIZADO_1_INSTANCIA,WF_ESDID_FINALIZADO_2_INSTANCIA))): ?>
		
			<?php if($slcpergunta1instancia): ?>
				<tr>
					<td width="140" class="subtituloDireita">Pergunta 1º Instância&nbsp;</td>
					<td><?php echo $slcpergunta1instancia; ?></td>
				</tr>
				<tr>
					<td class="subtituloDireita">Resposta 1º Instância&nbsp;</td>
					<td><?php echo $slcresposta1instancia; ?></td>
				</tr>
			<?php endif; ?>
			
			<?php if($slcpergunta2instancia): ?>
				<tr>
					<td width="140" class="subtituloDireita">Pergunta 2º Instância&nbsp;</td>
					<td><?php echo $slcpergunta2instancia; ?></td>
				</tr>
				<tr>
					<td class="subtituloDireita">Resposta 2º Instância&nbsp;</td>
					<td><?php echo $slcresposta2instancia; ?></td>
				</tr>
			<?php endif; ?>
						
		<?php endif; ?>
		
		<?php if($boAtivo == 'S'): ?>
			<tr>
				<td class="subtituloDireita">&nbsp;</td>
				<td class="subtituloEsquerda">
					<input type="button" value="Salvar" id="bt_salvar" />
				</td>				
			</tr>
		<?php endif; ?>
	</table>
</form>
<?php if($docid && false): ?>
	<div style="position:absolute;right:80px;top:270px;">
		<?php
		$oculta = true;
		if(checkPerfil(array(SIC_PERFIL_ADMINISTRADOR))){
			$oculta = false;
		}
		wf_desenhaBarraNavegacao( $docid , array('slcid' => $slcid),  array('historico'=>$oculta) );
		?>
	</div>
<?php endif; ?>