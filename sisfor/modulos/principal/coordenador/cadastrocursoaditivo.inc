<?php 
if($_REQUEST['requisicao']=='alterarCursoAditivo'){
	alterarCursoAditivo($_POST);
	exit();
}

if($_REQUEST['requisicao']=='atualizarSaldoPlanejamento2'){
	header('Content-Type: text/html; charset=iso-8859-1');
	atualizarSaldoPlanejamento2($_REQUEST);
	exit();	
}

if($_SESSION['sisfor']['iusd']){
	$cies = recuperarCoordenadorIES($_SESSION['sisfor']['iusd']); 
}

if($_SESSION['usucpf']){
	$perfil = pegarPerfil($_SESSION['usucpf']);
}

if($_SESSION['sisfor']['unicod']){
	$docidplan2 = pegarDocidPlanejamento2($_SESSION['sisfor']['unicod']);
	$esdidplan2 = pegarEstadoDocumento($docidplan2);
} 

if($esdidplan2 == WF_PLAN_ANALISE_MEC2 || in_array(PFL_EQUIPE_MEC,$perfil)){
	$habilitado = "N";
} else {
	$habilitado = "S";
}

$planejamento = carregarDadosPlanejamento();
extract($planejamento);

$saldo = $saldodisp - $saldoacoes; ?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">
	jQuery.noConflict();

    function alterarCursoAditivo(){
        var retorno = 'N';
        jQuery('input:checkbox[name=sifid[]]').each(function(){
        	id = jQuery(this).val();
            if(jQuery(this).is(':checked')){
				retorno = 'S';	
            	if(jQuery('#sifqtdvagaaditivo_'+id).val() == ''){
					alert('Informe a "Qtd. Vagas Aditivo".');
					jQuery('#sifqtdvagaaditivo_'+id).focus();
					retorno = 'A';	
					return false;
				}
				if(jQuery('#sifvalorloaaditivo_'+id).val() == ''){
					alert('Informe o "Valor LOA Aditivo".');
					jQuery('#sifvalorloaaditivo_'+id).focus();
					retorno = 'A';	
					return false;
				}
            }
        });
        
		if(retorno == 'S' ){
			jQuery('#formulario_curso').children(':first').after('<input id="requisicao" type="hidden" name="requisicao" value="salvarCursoAditivo" />');
	    	jQuery('#formulario_curso').submit();
		} else if(retorno == 'N'){
			alert('É necesssário selecionar pelo menos "UM"!');
			return false;
		}
    }

	function imprimirCurso(curid) {
		window.open('sisfor.php?modulo=principal/coordenador/impressaocurso&acao=A&curid='+curid,'Visualizar Curso','scrollbars=yes,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}		

	jQuery(document).ready(function(){
		<?php if($_REQUEST['sifid']){ ?>	
		jQuery('#formulario_curso').append('<input id="sifid" type="hidden" name="sifid" value="<?php echo $_REQUEST['sifid'];?>" />');
	    <?php } ?>
		jQuery('#formulario_curso').append('<input id="requisicao" type="hidden" name="requisicao" value="alterarCursoAditivo" />');
	    jQuery('#formulario_curso').append('<input type="hidden" id="unicod" name="unicod" value="<?php echo $_SESSION['sisfor']['unicod'];?>"/>');

	    <?php if($saldo && $saldo <= 0){ ?>
			jQuery('[name^=sifvalorloaaditivo_]').attr('readonly','readonly');
		<?php } ?>

		<?php if($esdid == WF_PLAN_ANALISE_MEC2 || in_array(PFL_EQUIPE_MEC,$perfil)){ ?>
			jQuery('input,checkbox').attr('disabled','disabled');
			jQuery('#btnCancelar').removeAttr('disabled');
		<?php } ?>
	});	

	function atualizarSaldo(loa){
		jQuery.ajax({
			url: 'sisfor.php?modulo=principal/coordenador/cadastrocursoaditivo&acao=A',
			dataType: 'json',				
			data: { requisicao: 'atualizarSaldoPlanejamento2', loa: loa},
			async: false,
			success: function(data){
				if(trim(data.resultado) == 'N'){
					jQuery('#saldo').html('R$ ' + data.saldo);
					alert('Não há mais saldo disponível!');
					jQuery('#sifvalorloa').focus();
					jQuery('#btnSalvar').attr('disabled','disabled');
				} else {
					jQuery('#saldo').html('R$ ' + data.saldo);
					jQuery('#btnSalvar').removeAttr('disabled','disabled');
				}
		    }
		});
	}
</script>

<br>

<?php monta_titulo('Cadastro Curso Aditivo - Fase 2',$cies); ?>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="subtituloCentro" width="25%">&nbsp;</td>
		<td class="subtituloEsquerda" width="25%">SEB / SECADI</td>
	</tr>
	<tr>
		<td class="subtituloDireita">Saldo total para a Fase 2:</td>
		<td class="subtituloCentro">R$ <?php echo formata_valor($saldodisp); ?></td>
	</tr>	
	<tr>
		<td class="subtituloDireita">Recurso Gasto:</td>
		<td class="subtituloCentro">R$ <?php echo formata_valor($saldoacoes); ?></td>
	</tr>	
	<tr>
		<td class="subtituloDireita">Aditivo Fase 1:</td>
		<td class="subtituloCentro">R$ <?php echo formata_valor($saldoaditivo); ?></td>
	</tr>		
	<tr>
		<td class="subtituloDireita">Saldo:</td>
		<td id="saldo" class="subtituloCentro">R$ <?php echo formata_valor($saldocomp); ?></td>
	</tr>		
</table>

<br>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">			
	<tr>
		<td class="subtituloDireita" width="25%">Orientações:</td>
		<td>
			<p align="justify" style="line-height: 20px;">
				A tabela abaixo exibe todos os cursos validados na Fase 1 do planejamento.
			</p>
		</td>
	</tr>				
</table>		

<?php $_REQUEST['sifid'] ? selecionarCursoAditivo($_REQUEST['sifid']) : selecionarCursoAditivo(); ?>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">			
	<tr>
		<td align="center" bgcolor="#CCCCCC" colspan="2">
			<input type="button" value="Salvar" id="btnSalvar" onclick="alterarCursoAditivo();" <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?> />
			<input type="button" value="Cancelar" id="btnCancelar" onclick="window.close();"/>
		</td>
	</tr>				
</table>	

<br>