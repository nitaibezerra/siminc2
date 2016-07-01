<?php 
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if($_REQUEST['requisicao']=='salvarPropostaIES'){
	salvarPropostaIES($_POST);
	exit();
}

if($_REQUEST['requisicao']=='recuperarPropostaMEC'){
	recuperarPropostaMEC($_POST);
	exit();
}

extract($_REQUEST);

if($sifid){
	$propostaies = visualizarPropostaIES($_REQUEST);
	extract($propostaies);
}

if($_SESSION['sisfor']['iusd']){
	$cies = recuperarCoordenadorIES($_SESSION['sisfor']['iusd']); 
}

if($_SESSION['usucpf']){
	$perfil = pegarPerfil($_SESSION['usucpf']);
}

if($_SESSION['sisfor']['unicod']){
	$docid = pegarDocidPlanejamento($_SESSION['sisfor']['unicod']);
	$esdid = pegarEstadoDocumento($docid);
} 

if($esdid == WF_PLAN_ANALISE_MEC || in_array(PFL_EQUIPE_MEC,$perfil)){
	$habilitado = "N";
} else {
	$habilitado = "S";
}
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">
	function salvarPropostaIES(){
		<?php if($ieoid){ ?>
		if(jQuery('[name=sifopcao]:checked').length <= 0){
			alert('O campo "Opção" é obrigatório!');
			jQuery('[name=sifopcao]').focus();
			return false;
		}			
		<?php } ?>
		<?php if(!$oatid){ ?>
		if(jQuery('#sifqtdvagas').val() == ''){
			alert('O campo "Qtd. Vagas" é obrigatório!');
			jQuery('#sifqtdvagas').focus();
			return false;
		}	
		<?php } ?>
		if(jQuery('#sifvalorloa').val() == ''){
			alert('O campo "Valor LOA" é obrigatório!');
			jQuery('#sifvalorloa').focus();
			return false;
		}	
		<?php if(!$oatid){ ?>
		if(jQuery('#sifvaloroutras').val() == ''){
			alert('O campo "Valor Outras" é obrigatório!');
			jQuery('#sifvaloroutras').focus();
			return false;
		}			
		if(jQuery('[name=sifopcao]:checked').val() == '2' || jQuery('[name=sifopcao]:checked').val() == '3'){
			if(jQuery('[name=sifjustificativa]').val() == ''){
				alert('O campo "Justificativa" é obrigatório!');
				jQuery('[name=sifjustificativa]').focus();
				return false;
			}	
		}	
		<?php } ?>
		jQuery('#formulario').submit();
	}

	function selecionarOpcao(sifopcao){
		switch(sifopcao){
			case '1':
				divCarregando();
				jQuery.ajax({
					type: 'POST',
					url: 'sisfor.php?modulo=principal/coordenador/propostaies&acao=A',
					data: { requisicao: 'recuperarPropostaMEC', sifid: jQuery('#sifid').val(), ieoid: jQuery('#ieoid').val()},
					async: false,
					dataType: 'json',
					success: function(data){
						jQuery('#sifqtdvagas').attr('readonly','readonly');
						jQuery('#sifvalorloa').attr('readonly','readonly');
						jQuery('#sifvaloroutras').attr('readonly','readonly');
						jQuery('#sifqtdvagas').val(data.ieoqtdvagas);			
						jQuery('#sifvalorloa').val(data.valor_estimado);
						jQuery('#sifvaloroutras').val('0,00');
						jQuery('#sifvaloroutras').val('0,00');
						jQuery('[name=sifjustificativa]').val('');
						jQuery('#divJustificativa').hide();
						divCarregado();
				   	}
				});
				break;
			case '2':
					jQuery('#sifqtdvagas').attr('readonly','readonly');
					jQuery('#sifvalorloa').attr('readonly','readonly');
					jQuery('#sifvaloroutras').attr('readonly','readonly');				
					jQuery('#sifqtdvagas').val('0');
					jQuery('#sifvalorloa').val('0,00');
					jQuery('#sifvaloroutras').val('0,00');
					jQuery('#divJustificativa').show();
				break;
			case '3':
					jQuery('#sifqtdvagas').removeAttr('readonly');
					jQuery('#sifvalorloa').removeAttr('readonly');
					jQuery('#sifvaloroutras').removeAttr('readonly');
					jQuery('#sifqtdvagas').val('');
					jQuery('#sifvalorloa').val('');
					jQuery('#sifvaloroutras').val('');	
					jQuery('#divJustificativa').show();
				break;
		}					
	}
	
	jQuery(document).ready(function() {
		<?php if($ocuid || $cnvid){ 
		   $sifopcao = '3'; ?>
		   jQuery("input[name=sifopcao]").each(function(){
				if(jQuery(this).val() == '3'){
					jQuery(this).attr('checked','checked');
				} else { 
					jQuery(this).attr('disabled','disabled');
				}		
		   		jQuery(this).removeAttr('onclick');
		   });
		<?php } ?>
		<?php if($ieoid && ($sifopcao == '2' || $sifopcao == '3')){ ?>
			jQuery('#divJustificativa').show();
		<?php } ?>
		<?php if($sifopcao == '1' || $sifopcao == '2'){ ?>
			jQuery('#sifqtdvagas').attr('readonly','readonly');
			jQuery('#sifvalorloa').attr('readonly','readonly');
			jQuery('#sifvaloroutras').attr('readonly','readonly');		
		<?php } ?>
	});
</script>

<?php monta_titulo('Proposta IES',$cies); ?>

<br>

<form id="formulario" method="post" name="formulario" action="">
	<?php if($sifid){ ?>
	<input type="hidden" id="sifid" name="sifid" value="<?php echo $sifid; ?>"/>
	<?php } else { ?>
	<input type="hidden" id="unicod" name="unicod" value="<?php echo $unicod; ?>"/>
	<input type="hidden" id="ieoid" name="ieoid" value="<?php echo $ieoid; ?>"/>
	<?php } ?>
	<input type="hidden" id="requisicao" name="requisicao" value="salvarPropostaIES"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td colspan="2" class="subtituloCentro"><?php echo $curso; ?></td>
		</tr>			
		<tr>
			<td class="subtituloDireita" width="25%">Orientações:</td>
			<td>
				<p align="justify" style="line-height: 20px;">
				Analise os dados e indique se a Instituição "Aceita", "Rejeita" ou "Repactua" a proposta do MEC. Caso "aceite", o sistema replicará o número de vagas e valores sugeridos, 
				impedindo a edição. Caso a IES "rejeite", o sistema indica zero nas vagas e valores, impedindo a edição e abrindo um campo de Justificativa, onde deverá ser descrito, de 
				forma sucinta, o motivo da recusa. Caso "repactue", o sistema deixa os campos de vaga e valor em branco e também solicita uma Justificativa sobre o porquê da alteração sobre 
				a proposta (nas vagas, nos valores ou em ambos, se for o caso). Em todos os casos, não esqueça de clicar em "Salvar".
				</p>			
			</td>
		</tr>		
		<?php if($ieoid){ ?>
		<tr>
			<td class="subtituloDireita">Opção:</td>
			<td>
				<input id="sifopcao" type="radio" name="sifopcao" value="1" class="normal" style="margin-top: -1px;" onclick="selecionarOpcao(this.value);" <?php echo $sifopcao == '1' ? 'checked="checked"' : '' ?> <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?>/> Aceita
				<input id="sifopcao" type="radio" name="sifopcao" value="2" class="normal" style="margin-top: -1px;" onclick="selecionarOpcao(this.value);" <?php echo $sifopcao == '2' ? 'checked="checked"' : '' ?> <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?>/> Rejeita
				<input id="sifopcao" type="radio" name="sifopcao" value="3" class="normal" style="margin-top: -1px;" onclick="selecionarOpcao(this.value);" <?php echo $sifopcao == '3' ? 'checked="checked"' : '' ?> <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?>/> Repactua
			</td>
		</tr>			
		<?php } ?>
		<?php if(!$oatid){ ?>
		<tr>
			<td class="subtituloDireita">Qtd. Vagas:</td>
			<td><?php echo campo_texto('sifqtdvagas','N',$habilitado, 'Quantidade de Vagas', '25', '15', '#####', '', '', '', '','id="sifqtdvagas"'); ?></td>
		</tr>		
		<?php } ?>	
		<tr>
			<td class="subtituloDireita">Valor LOA:</td>
			<td>
				<?php echo campo_texto('sifvalorloa','N',$habilitado, 'Valor LOA', '25', '15', '[.###],##', '', '', '', '','id="sifvalorloa"'); ?>
			</td>
		</tr>	
		<?php if(!$oatid){ ?>
		<tr>
			<td class="subtituloDireita">Valor Outras:</td>
			<td><?php echo campo_texto('sifvaloroutras','N',$habilitado, 'Valor Outras', '25', '15', '[.###],##', '', '', '', '','id="sifvaloroutras"'); ?></td>
		</tr>		
		<tr id="divJustificativa" style="display:none;">
			<td class="subtituloDireita">Justificativa:</td>
			<td><?php echo campo_textarea('sifjustificativa','S',$habilitado, 'Justificativa', '70', '4', '1000'); ?></td>
		</tr>	
		<?php } ?>
		<tr>
			<td align="center" bgcolor="#CCCCCC" colspan="2">
				<input type="button" value="Salvar" id="btnSalvar" onclick="salvarPropostaIES();" <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?>/>
				<input type="button" value="Cancelar" id="btnCancelar" onclick="window.close();"/>
			</td>
		</tr>				
	</table>	
</form>	