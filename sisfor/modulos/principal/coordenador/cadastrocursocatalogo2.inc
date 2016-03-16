<?php 
if($_REQUEST['requisicao']=='salvarCursoCatalogo'){
	salvarCursoCatalogo($_POST);
	exit();
}

if($_REQUEST['requisicao']=='alterarCursoCatalogo'){
	alterarCursoCatalogo($_POST);
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

$saldo = $saldodisp - $saldoacoes; 
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">
	jQuery.noConflict();

    function salvarCursoCatalogo(){
        var retorno = 'N';
        jQuery('input:checkbox[name=curid[]]').each(function(){
        	id = jQuery(this).val();
            if(jQuery(this).is(':checked')){
				retorno = 'S';	
            	if(jQuery('#sifqtdvagas_'+id).val() == ''){
					alert('Informe a "Qtd. Vagas".');
					jQuery('#sifqtdvagas_'+id).focus();
					retorno = 'A';	
					return false;
				}
				if(jQuery('#sifvalorloa_'+id).val() == ''){
					alert('Informe o "Valor LOA".');
					jQuery('#sifvalorloa_'+id).focus();
					retorno = 'A';	
					return false;
				}
				if(jQuery('#sifvaloroutras_'+id).val() == ''){
					alert('Informe o "Valor Outras".');
					jQuery('#sifvaloroutras_'+id).focus();
					retorno = 'A';	
					return false;
				}
				if(jQuery('[name=sifaprovado2013_'+id+']:checked').length <= 0){
					alert('O campo "Curso aprovado em 2013?" é obrigatório!');
					jQuery('[name=sifaprovado2013_'+id+']').focus();
					retorno = 'A';	
					return false;
				}
            }
        });
        
		if(retorno == 'S' ){
			jQuery('#formulario_curso').children(':first').after('<input id="requisicao" type="hidden" name="requisicao" value="salvarCursoCatalogo" />');
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
		<?php if(!$_REQUEST['cnvid']){ ?>	
		jQuery('#formulario_curso').append('<input id="requisicao" type="hidden" name="requisicao" value="salvarCursoCatalogo" />');
		jQuery('#formulario_curso').append('<input id="siftipoplanejamento" type="hidden" name="siftipoplanejamento" value="2" />');
	    <?php } else { ?>
		jQuery('#formulario_curso').append('<input id="requisicao" type="hidden" name="requisicao" value="alterarCursoCatalogo" />');
		jQuery('#formulario_curso').append('<input id="siftipoplanejamento" type="hidden" name="siftipoplanejamento" value="2" />');
		jQuery('#formulario_curso').append('<input id="cnvid" type="hidden" name="cnvid" value="<?php echo $_REQUEST['cnvid'];?>" />');
	    <?php } ?>
	    jQuery('#formulario_curso').append('<input type="hidden" id="unicod" name="unicod" value="<?php echo $_SESSION['sisfor']['unicod'];?>"/>');

	    <?php if($saldodisp && $saldodisp <= 0){ ?>
			jQuery('[name^=sifvalorloa_]').attr('readonly','readonly');
		<?php } ?>

		<?php if($saldo && $saldo <= 0) { ?>
			jQuery('.SEB').attr('disabled','disabled');
			jQuery('.SEC').attr('disabled','disabled');
		<?php } ?>

		<?php if($esdid == WF_PLAN_ANALISE_MEC2 || in_array(PFL_EQUIPE_MEC,$perfil)){ ?>
			jQuery('input,checkbox').attr('disabled','disabled');
			jQuery('#btnCancelar').removeAttr('disabled');
		<?php } ?>
	});	

	function atualizarSaldo(loa){
		jQuery.ajax({
			url: 'sisfor.php?modulo=principal/coordenador/cadastrocursocatalogo2&acao=A',
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


<?php 

$recursoGasto = pegaValorNaoAplicadoFase2();

monta_titulo('Cadastro Curso Catálogo - Fase 2',$cies); ?>

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
		<td class="subtituloCentro">R$ <?php echo formata_valor($recursoGasto); ?></td>
	</tr>	
	<!-- 
	<tr>
		<td class="subtituloDireita">Aditivo Fase 1:</td>
		<td class="subtituloCentro">R$ <?php echo formata_valor($saldoaditivo); ?></td>
	</tr>
	-->	
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
				A tabela abaixo exibe todos os cursos ativos no Catálogo de Cursos 2014 do MEC. Para saber mais informações sobre determinado curso do Catálogo do MEC, clique no ícone 
				<img border="0" style="cursor:pointer;" src="../imagens/print.gif" title="IMPRESSÃO CURSO">.
			</p>
			<p align="justify" style="line-height: 20px;">
				Selecione o curso que a IES deseja ofertar, indique o número de vagas pretendidas e o valor estimado (indicando no campo correspondente a parcela proveniente da LOA 2014 e 
				a parcela que será realizada com recursos da IES) e clique em "Salvar".
			</p>
		</td>
	</tr>				
</table>		

<?php $_REQUEST['cnvid'] ? selecionarCursoCatalogo($_REQUEST['cnvid'],FASE02) : selecionarCursoCatalogo(NULL,FASE02); ?>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">			
	<tr>
		<td align="center" bgcolor="#CCCCCC" colspan="2">
			<input type="button" value="Salvar" id="btnSalvar" onclick="salvarCursoCatalogo();" <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?> />
			<input type="button" value="Cancelar" id="btnCancelar" onclick="window.close();"/>
		</td>
	</tr>				
</table>	

<br>