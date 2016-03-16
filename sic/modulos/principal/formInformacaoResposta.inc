<?php 
if($_REQUEST['requisicao'] == 'pegarPedido'){
	
	$sql = "select usucpfresponsavel from sic.solicitacao where slcid = {$_REQUEST['slcid']}";
	$rs = $db->pegaUm($sql);
	
	if(empty($rs)){
		$sql = "update sic.solicitacao set 
					usucpfresponsavel = '{$_SESSION['usucpf']}' 
				where slcid = {$_REQUEST['slcid']}";
		$db->executar($sql);
		$db->commit();
		die('true');
	}else{
		die('false');
	}	
}

if($_REQUEST['requisicao'] == 'salvaDados'){
	
	if($_REQUEST['slcid']){
		
		$_REQUEST['slcpublicar'] 			= $_REQUEST['slcpublicar'] ? "'".$_REQUEST['slcpublicar']."'" : 'null';
		$_REQUEST['slcprorrogado']			= $_REQUEST['slcprorrogado'] ? "'".$_REQUEST['slcprorrogado']."'" : 'null';
		$_REQUEST['tppid']					= $_REQUEST['tppid'] ? "'".$_REQUEST['tppid']."'" : 'null';
		$_REQUEST['traid']					= $_REQUEST['traid'] ? "'".$_REQUEST['traid']."'" : 'null';
		$_REQUEST['slcexistenoportaldomec'] = $_REQUEST['slcexistenoportaldomec'] ? "'".$_REQUEST['slcexistenoportaldomec']."'" : 'null'; 
				
		$slcdtprorrogacaotemp = $db->pegaUm("select slcdtprorrogacao from sic.solicitacao where slcid = {$_REQUEST['slcid']}");
		
		$stUpdateDataInclusao = '';
		if($_REQUEST['slcprorrogado'] == "'t'" && !$slcdtprorrogacaotemp){
			$stUpdateDataInclusao = ", slcdtprorrogacao = '".date('Y-m-d H:i:s')."'";
			enviaEmailProrrogacao($_REQUEST['slcid']);
		}	

		if($_REQUEST['slcprorrogado'] == "'f'" && $slcdtprorrogacaotemp){
			$stUpdateDataInclusao = ", slcdtprorrogacao = null";
			$_REQUEST['slcjustificativa'] = '';
		}
		if($_REQUEST['classificacaoconteudo']=='f'||$_REQUEST['classificacaoconteudo']=='t'){
			$classificacaoconteudo = ",classificacaoconteudo = '{$_REQUEST['classificacaoconteudo']}'";
		}
		$sql = "update sic.solicitacao set	
					--tppid					= {$_REQUEST['tppid']},				
					traid					= {$_REQUEST['traid']},
					slcresposta 			= '{$_REQUEST['slcresposta']}',
					slcjustificativa 		= '{$_REQUEST['slcjustificativa']}',
					slcprorrogado 			= {$_REQUEST['slcprorrogado']},
					usucpfalteracao 		= '{$_SESSION['usucpf']}',
					slcpublicar				= {$_REQUEST['slcpublicar']},
					slcexistenoportaldomec	= {$_REQUEST['slcexistenoportaldomec']}
					--slcclassificacao				= '{$_REQUEST['slcclassificacao']}'
					$classificacaoconteudo
					{$stUpdateDataInclusao}									
				where slcid = {$_REQUEST['slcid']}";
		$db->executar($sql);
		$db->commit();
		$db->sucesso('principal/formInformacaoResposta');
	}	
}

if(!$_SESSION['sic']['slcid']) {
	echo "<script>
			alert('Selecione um pedido');
			window.location='sic.php?modulo=inicio&acao=C';
		  </script>";
	exit;
}

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";

echo "<br/>";
$titulo2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';
monta_titulo('Resposta ao Pedido de Acesso à Informação', $titulo2);
echo "<br/>";

$docid = pegaDocid($_SESSION['sic']['slcid']);
$esdid = pegaEstadoAtual($docid);

$abacod_tela = 57548;

$arMnuid = array();
if(!in_array($esdid,array(WF_ESDID_RECURSO_1_INSTANCIA,WF_ESDID_RECURSO_2_INSTANCIA,WF_ESDID_ANALISE_1_RECURSO,WF_ESDID_ANALISE_2_RECURSO))){
	$arMnuid[] = 11052;
}

$db->cria_aba( $abacod_tela, $url, '', $arMnuid);

$slcid = $_SESSION['sic']['slcid'];

$sql = "select 
			tpsid,
			slcresposta,
			slcjustificativa,
			slcprorrogado,
			usucpfresponsavel,
			slcdtinclusao,
			tppid,
			slcprorrogado,
			slcexistenoportaldomec,
			slcpublicar,
			slcclassificacao,
			traid,
			classificacaoconteudo
		from 
			sic.solicitacao sc 
		where 
			sc.slcid = {$_SESSION['sic']['slcid']}";
// ver($sql);
$rs = $db->pegaLinha($sql);
// ver($rs);

if($rs){
	extract($rs);
}



$usucpfresponsavel = $rs["usucpfresponsavel"];

$boAtivo = 'N';
if(in_array($esdid, array(WF_ESDID_ANALISE_AREA_RESPONSAVEL, WF_ESDID_ANALISE_NAI))){

	if((checkPerfil(array(SIC_PERFIL_ADMINISTRADOR)) && !empty($usucpfresponsavel))||checkperfil(array(SIC_PERFIL_SUPER_USUARIO))){
		$boAtivo = 'S';
	}else if(checkperfil(array(SIC_PERFIL_AREA_RESPONSAVEL, SIC_PERFIL_SUPER_USUARIO)) && $usucpfresponsavel == $_SESSION['usucpf']){
		$boAtivo = 'S';	
	}
}

if( checkPerfil(array(SIC_PERFIL_ORGAO_CONTROLE)) ){
	$boAtivo = 'N';
}
$dias = 0;
$oi = checkPerfil(array(SIC_PERFIL_ORGAO_CONTROLE));
// Verifica dias para resposta
if($slcdtinclusao){
	
	$arData = explode('-', $slcdtinclusao);
	
	$timestamp2 = mktime(0,0,0,$arData[1],$arData[2],$arData[0]);
	$timestamp1 = mktime(0,0,0,date('m'),date('d'),date('Y'));
	
	$dias = 20-(($timestamp1-$timestamp2) / (60 * 60 * 24));
	
}

?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script>
	$(function(){

		jQuery("[name=tppid]").addClass("required");
		jQuery("[name=slcresposta]").addClass("required");		
		jQuery("[name=slcprorrogado]").addClass("required");
		jQuery("[name=slcexistenoportaldomec]").addClass("required");		
		jQuery("[name=slcpublicar]").addClass("required");
		jQuery("[name=slcclassificacao]").addClass("required");		
				
		jQuery("#formulario_id").validate();

		<?php if(isset($slcprorrogado) && $slcprorrogado == "t"):?>
			$('[name=tppid], [name=slcresposta], [name=slcprorrogado], [name=slcexistenoportaldomec], [name=slcpublicar], [name=slcclassificacao]').removeClass('required');
		<?php endif; ?>
		
		$('[name=slcprorrogado]').click(function(){
			if(this.value == 't'){
				$('[name=tppid], [name=slcresposta], [name=slcprorrogado], [name=slcexistenoportaldomec], [name=slcpublicar], [name=slcclassificacao]').removeClass('required');
			}else{
				$('[name=tppid], [name=slcresposta], [name=slcprorrogado], [name=slcexistenoportaldomec], [name=slcpublicar], [name=slcclassificacao]').addClass('required');
			}
		});

		$('[name=slcprorrogado]').change(function(){
			if(this.value == 't'){
				$('#tr_justificativa').show();
			}else{
				$('#tr_justificativa').hide();
			}
		});

		$('#bt_salvar').click(function(){

			<? if($esdid == WF_ESDID_ANALISE_NAI){ ?>
// 				if($('[name=slcclassificacao]:checked').length < 1){
// 					alert('A campo Item critíco é obrigatório!');
// 					$('[name=slcclassificacao]').focus();
// 					return false;
// 				}
			<?}?>
			if($('[name=slcprorrogado]:checked').val() == 't'){
				if($('[name=slcjustificativa]').val() == ''){
					alert('A justificativa é obrigatória!');
					$('[name=slcprorrogado]').focus();
					return false;
				}
			}
			if($('[name=slcprorrogado]:checked').val() == 't'){
				if($('[name=slcjustificativa]').val() == ''){
					alert('A justificativa é obrigatória!');
					$('[name=slcprorrogado]').focus();
					return false;
				}
			}
			
			if($('[name*="classificacaoconteudo"]:checked').length<'1'){
				alert('É obrigatório o preenchimento  do  campo: Este pedido de acesso ou sua respectiva resposta contém informações sujeitas a restrição de acesso, conforme previsto na Lei 12.527/2011?');
				$('[name*="classificacaoconteudo"]:checked').focus();
				return false;
			}
			$('#requisicao_id').val('salvaDados');
			$('#formulario_id').submit();
		});

	});

	function pegarPedido(slcid)
	{
		if(confirm('Deseja pegar esse pedido?')){
			$.ajax({
				url		: 'sic.php?modulo=principal/formInformacaoResposta&acao=A',
				type	: 'post',
				data	: 'requisicao=pegarPedido&slcid='+slcid,
				success	: function(e){
					if(e == 'false'){
						alert('Esta demanda já possui responsável!');
					}else{
						document.location.href = 'sic.php?modulo=principal/formInformacaoResposta&acao=A';
					}
				}
			});
		}
	}
</script>
<form name="formulario" id="formulario_id" method="post" action="">
	<input type="hidden" name="slcid" id="slcid" value="<?php echo $_SESSION['sic']['slcid'] ?>" />	
	<input type="hidden" name="requisicao" id="requisicao_id" value="" />	
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<?php if(empty($usucpfresponsavel) && (checkPerfil(SIC_PERFIL_AREA_RESPONSAVEL) || checkPerfil(SIC_PERFIL_ADMINISTRADOR))): ?>
			<tr>
				<td class="subtituloDireita">&nbsp;</td>
				<td>
					<input type="button" value="Pegar pedido" onclick="pegarPedido(<?php echo $_SESSION['sic']['slcid'] ?>)"/>
				</td>
			</tr>
		<?php endif; ?>
		<!--  
		<tr>
			<td width="180" class="subtituloDireita">Prioridade</td>
			<td>
				<?php 
				$sql = "select
							tppid as codigo, 
							tppdsc as descricao
						from 
							sic.tipoprioridade
						order by 
							tppdsc";
				$db->monta_combo('tppid', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'S');
				?>
			</td>
		</tr>
		-->
		<tr>
			<td width="180" class="subtituloDireita">Tipo de resposta</td>
			<td>
				<?php 
				$sql = "select
							traid as codigo, 
							tradsc as descricao
						from 
							sic.tiporespostaanalise
						order by 
							tradsc";
				$db->monta_combo('traid', $sql, $boAtivo, 'Selecione...', '', '', '', '', 'S');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="180">Resposta</td>
			<td>
				<?php 
				echo campo_textarea('slcresposta', 'S', $boAtivo, '', 80, 10, 8000);
				?>
			</td>
		</tr>
		<?php // if($esdid == WF_ESDID_ANALISE_NAI): ?>
			<tr>
				<td class="subtituloDireita" width="180">O item é crítico?</td>
				<td>
					<input type="radio" name="slcclassificacao" id="slcclassificacao_vd" value="verde" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : 'disabled="disabled"' ?> <?php echo trim($slcclassificacao) == 'verde' ? 'checked' : '' ?>/>&nbsp;<img src="../imagens/av_verde.gif" />&nbsp;
					<input type="radio" name="slcclassificacao" id="slcclassificacao_am" value="amarelo" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : 'disabled="disabled"' ?> <?php echo trim($slcclassificacao) == 'amarelo' ? 'checked' : '' ?>/>&nbsp;<img src="../imagens/av_amarelo.gif" />&nbsp;
					<input type="radio" name="slcclassificacao" id="slcclassificacao_vm" value="vermelho" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : 'disabled="disabled"' ?> <?php echo trim($slcclassificacao) == 'vermelho' ? 'checked' : '' ?>/>&nbsp;<img src="../imagens/av_vermelho.gif" />&nbsp;
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
		<?php // endif; ?>
		<tr>
			<td class="subtituloDireita" width="180">A informação já existe no portal do MEC?</td>
			<td>
				<input type="radio" name="slcexistenoportaldomec" id="slcexistenoportaldomec_t" value="t" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?> <?php echo $slcexistenoportaldomec == 't' ? 'checked' : '' ?>/>&nbsp;Sim&nbsp;
				<input type="radio" name="slcexistenoportaldomec" id="slcexistenoportaldomec_f" value="f" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?> <?php echo $slcexistenoportaldomec == 'f' ? 'checked' : '' ?>/>&nbsp;Não&nbsp;
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		
		<tr>
			<td class="subtituloDireita" width="180">Publicar resposta</td>
			<td>
				<input type="radio" name="slcpublicar" id="slcpublicar_t" value="t" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?> <?php echo $slcpublicar == 't' ? 'checked' : '' ?>/>&nbsp;Sim&nbsp;
				<input type="radio" name="slcpublicar" id="slcpublicar_f" value="f" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?> <?php echo $slcpublicar == 'f' ? 'checked' : '' ?>/>&nbsp;Não&nbsp;
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="180">Este pedido de acesso ou sua respectiva resposta contém informações sujeitas a restrição de acesso, conforme previsto na Lei 12.527/2011?</td>
			<td>
				<input type="radio" name="classificacaoconteudo"  id="classificacaoconteudo _t" value="t" <?php echo ( $boAtivo != 'S'||$classificacaoconteudo=='t')? 'disabled="disabled"' : '' ?> <?php echo $classificacaoconteudo  == 't' ? 'checked' : '' ?>/>&nbsp;Sim&nbsp;
				<input type="radio" name="classificacaoconteudo"  id="classificacaoconteudo _f" value="f" <?php echo ( $boAtivo != 'S'||$classificacaoconteudo=='t')? 'disabled="disabled"' : '' ?> <?php echo $classificacaoconteudo  == 'f' ? 'checked' : '' ?>/>&nbsp;Não&nbsp;
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		
		<?php if($dias >= 1): ?>
			<tr>
				<td class="subtituloDireita" width="180">Prorrogado</td>
				<td>
					<input type="radio" name="slcprorrogado" id="slcprorrogado_t" value="t" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?> <?php echo $slcprorrogado == 't' ? 'checked' : '' ?>/>&nbsp;Sim&nbsp;
					<input type="radio" name="slcprorrogado" id="slcprorrogado_f" value="f" <?php echo $boAtivo != 'S' ? 'disabled="disabled"' : '' ?> <?php echo $slcprorrogado == 'f' ? 'checked' : '' ?>/>&nbsp;Não&nbsp;
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
		<?php endif; ?>
		<tr id="tr_justificativa" style="display:none;">
			<td class="subtituloDireita" width="180">Justificativa</td>
			<td>
				<?php 
				echo campo_textarea('slcjustificativa', 'S', $boAtivo, '', 80, 10, 4000);
				?>
			</td>
		</tr>		
		<tr>
			<td class="subtituloDireita"></td>
			<td class="subtituloEsquerda">
				<?php if($boAtivo == 'S'): ?>
					<input type="button" value="Salvar" id="bt_salvar" />
				<?php endif; ?>
			</td>
		</tr>
	</table>
</form>
<?php if($slcprorrogado == 't'): ?>
	<script>
		$(function(){
			$('#tr_justificativa').show();
		});
	</script>
<?php endif; ?>
<div style="position:absolute;right:80px;top:270px;">
	<?php 
	$oculta = true;
	if(checkPerfil(array(SIC_PERFIL_ADMINISTRADOR))){
		$oculta = false;
	}
	wf_desenhaBarraNavegacao( $docid , array('slcid' => $slcid),  array('historico'=>$oculta) );
	?>
</div>