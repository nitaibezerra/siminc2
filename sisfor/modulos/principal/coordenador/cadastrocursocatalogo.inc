<?php 
if($_REQUEST['requisicao']=='salvarCursoCatalogo'){
	salvarCursoCatalogo($_POST);
	exit();
}

if($_REQUEST['requisicao']=='alterarCursoCatalogo'){
	alterarCursoCatalogo($_POST);
	exit();
}

if($_REQUEST['requisicao']=='excluirCursoCatalogo'){
	header('Content-Type: text/html; charset=iso-8859-1');
	excluirCursoCatalogo($_REQUEST['cnvid']);
	listarCursoCatalogo();
	exit();
} 

if($_REQUEST['requisicao']=='atualizarSaldo'){
	header('Content-Type: text/html; charset=iso-8859-1');
	atualizarSaldo($_REQUEST);
	exit();	
}

if($_SESSION['sisfor']['iusd']){
	$cies = recuperarCoordenadorIES($_SESSION['sisfor']['iusd']); 
}

if($_REQUEST['cnvid']){
	$propostaies = visualizarPropostaIES($_REQUEST);
	extract($propostaies);
	
	if($coordenacao){
		$coord = verificarCoordenacao($coordenacao);
	}
	
	$saldoseb = verificarSaldoSEB();
	$saldosecadi = verificarSaldoSECADI();
	
	if($coord == 'seb'){
		$saldo = $saldoseb;
	} else {
		$saldo = $saldosecadi;
	} 	
} else {
	$saldoseb = verificarSaldoSEB();
	$saldosecadi = verificarSaldoSECADI();	
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

$seb = carregarDadosSEB();
$secadi = carregarDadosSECADI();

$sebsaldo = formata_valor(desformata_valor($seb['saldoies']) - desformata_valor($seb['valortotal']));
$secadisaldo = formata_valor(desformata_valor($secadi['saldoies']) - desformata_valor($secadi['valortotal'])); 
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

	function excluirCursoCatalogo(cnvid){
		if(confirm("Deseja realmente excluir o Curso?")){
			divCarregando();
			jQuery.ajax({
				url: 'sisfor.php?modulo=principal/coordenador/cadastrocursocatalogo&acao=A',
				data: { requisicao: 'excluirCursoCatalogo', cnvid: cnvid},
				async: false,
				success: function(data) {
					alert('Curso excluído com sucesso!');
					jQuery("#divListaCursoCatalogo").html(data);
					divCarregado();
			    }
			});
		}
	}    

	function imprimirCurso(curid) {
		window.open('sisfor.php?modulo=principal/coordenador/impressaocurso&acao=A&curid='+curid,'Visualizar Curso','scrollbars=yes,height=400,width=950,status=no,toolbar=no,menubar=no,location=no');	
	}		

	jQuery(document).ready(function(){
		<?php if(!$_REQUEST['cnvid']){ ?>	
		jQuery('#formulario_curso').append('<input id="requisicao" type="hidden" name="requisicao" value="salvarCursoCatalogo" />');
		jQuery('#formulario_curso').append('<input id="siftipoplanejamento" type="hidden" name="siftipoplanejamento" value="1" />');
	    <?php } else { ?>
		jQuery('#formulario_curso').append('<input id="requisicao" type="hidden" name="requisicao" value="alterarCursoCatalogo" />');
		jQuery('#formulario_curso').append('<input id="siftipoplanejamento" type="hidden" name="siftipoplanejamento" value="1" />');
		jQuery('#formulario_curso').append('<input id="cnvid" type="hidden" name="cnvid" value="<?php echo $_REQUEST['cnvid'];?>" />');
	    <?php } ?>
	    jQuery('#formulario_curso').append('<input type="hidden" id="unicod" name="unicod" value="<?php echo $_SESSION['sisfor']['unicod'];?>"/>');

	    <?php if($saldo && $saldo <= 0){ ?>
			jQuery('[name^=sifvalorloa_]').attr('readonly','readonly');
		<?php } ?>

		<?php if(($saldoseb && $saldoseb <= 0) || ($sebsaldo <=0)) { ?>
			jQuery('.SEB').attr('disabled','disabled');
		<?php } ?>

		<?php if(($saldosecadi && $saldosecadi <= 0) || ($secadisaldo <=0)){ ?>
			jQuery('.SEC').attr('disabled','disabled');
		<?php } ?>	

		<?php if($esdid == WF_PLAN_ANALISE_MEC || in_array(PFL_EQUIPE_MEC,$perfil)){ ?>
			jQuery('input,checkbox').attr('disabled','disabled');
			jQuery('#btnCancelar').removeAttr('disabled');
		<?php } ?>
	});	

	function atualizarSaldo(loa,coordid){
		if(jQuery('#coordid').val() == ''){
			alert('Selecione a secretaria!');
			jQuery('#sifvalorloa').val('');
			jQuery('#coordid').focus();
		} else {
			jQuery.ajax({
				url: 'sisfor.php?modulo=principal/coordenador/cadastrocursocatalogo&acao=A',
				dataType: 'json',				
				data: { requisicao: 'atualizarSaldo', loa: loa, coordid: coordid},
				async: false,
				success: function(data){
					if(trim(data.resultado) == 'N'){
						jQuery('#saldo'+data.secretaria).html('R$ ' + data.saldo);						
						alert('Não há mais saldo disponível nesta Secretaria!');
						jQuery('#sifvalorloa').focus();
						jQuery('#btnSalvar').attr('disabled','disabled');						
					} else {
						jQuery('#saldo'+data.secretaria).html('R$ ' + data.saldo);
						jQuery('#btnSalvar').removeAttr('disabled','disabled');
					}
			    }
			});
		}
	}
</script>

<br>

<?php monta_titulo('Cadastro Curso Catálogo',$cies); ?>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="subtituloCentro" width="25%">&nbsp;</td>
		<td class="subtituloEsquerda" width="25%">SEB</td>
		<td class="subtituloEsquerda" width="25%">SECADI</td>
	</tr>
	<tr>
		<td class="subtituloDireita" width="25%">Saldo Disponível:</td>
		<td class="subtituloCentro">&nbsp;R$ <?php echo $seb['saldoies']; ?></td>
		<td class="subtituloCentro">&nbsp;R$ <?php echo $secadi['saldoies']; ?></td>
	</tr>	
	<tr>
		<td class="subtituloDireita" width="25%">Recurso Gasto:</td>
		<td class="subtituloCentro">&nbsp;R$ <?php echo $seb['valoratividade']; ?></td>
		<td class="subtituloCentro">&nbsp;R$ <?php echo $secadi['valoratividade']; ?></td>
	</tr>	
	<tr>
		<td class="subtituloDireita" width="25%">Saldo:</td>
		<td id="saldoseb" class="subtituloCentro">&nbsp;R$ <?php echo formata_valor(desformata_valor($seb['saldoies']) - desformata_valor($seb['valoratividade'])); ?></td>
		<td id="saldosecadi" class="subtituloCentro">&nbsp;R$ <?php echo formata_valor(desformata_valor($secadi['saldoies']) - desformata_valor($secadi['valoratividade'])); ?></td>		
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

<?php $_REQUEST['cnvid'] ? selecionarCursoCatalogo($_REQUEST['cnvid'],FASE01) : selecionarCursoCatalogo(NULL,FASE01); ?>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">			
	<tr>
		<td align="center" bgcolor="#CCCCCC" colspan="2">
			<input type="button" value="Salvar" id="btnSalvar" onclick="salvarCursoCatalogo();" <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?> />
			<input type="button" value="Cancelar" id="btnCancelar" onclick="window.close();"/>
		</td>
	</tr>				
</table>	

<br>
