<?

//inativa solicitação
if($_REQUEST['inativasbaid'] == 'OK' && $_REQUEST['sbaid'])
{
	//inativa atuação
	$sql = "UPDATE fiesabatimento.atuacaoprofissional SET atpstatus = 'I' WHERE sbaid={$_REQUEST['sbaid']}";
	$db->executar($sql);
	
	//inativa solicitação
	$sql = "UPDATE fiesabatimento.solicitacaoabatimento SET sbastatus = 'I' WHERE sbaid={$_REQUEST['sbaid']}";
	$db->executar($sql);
	
	//log de quem inativou
	$sql = "INSERT INTO fiesabatimento.solicitacaoabatimentoinativa(sbaid, saiusucpf, saidtinativa)
    		VALUES ({$_REQUEST['sbaid']}, '{$_SESSION['usucpf']}', now())";
	$db->executar($sql);
	
	$db->commit();
	
	echo '<script>
			alert("Solicitação inativada com sucesso!");
			location.href="fiesabatimento.php?modulo=principal/listasolicitacaoabatimento&acao=A";
		  </script>';
	die;
}


if($_REQUEST['requisicao'])
{
	$_REQUEST['requisicao']();
	die();
}
if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}

$prazo = testa_prazo_aprovacao();

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);

if($_REQUEST['idoid']) {
	$solicitante = $db->pegaLinha("SELECT * FROM fiesabatimento.identificacaodocente id
									INNER JOIN fiesabatimento.solicitacaoabatimento 	sb ON sb.idoid = id.idoid
									INNER JOIN fiesabatimento.atuacaoprofissional 		ap ON ap.idoid = id.idoid AND ap.sbaid = sb.sbaid
									WHERE id.idoid='".$_REQUEST['idoid']."' AND sb.sbaid = '".$_REQUEST['sbaid']."' AND sbastatus = 'A' AND ap.atpstatus = 'A'");
	if( $solicitante['idoid'] == '' ){
		$solicitante = $db->pegaLinha("SELECT * FROM fiesabatimento.identificacaodocente id
										INNER JOIN fiesabatimento.solicitacaoabatimento 	sb ON sb.idoid = id.idoid
										INNER JOIN fiesabatimento.atuacaoprofissional 		ap ON ap.idoid = id.idoid AND ap.sbaid = sb.sbaid
										WHERE sb.sbaid = ".$_REQUEST['sbaid']);
	}
	
	//pega id renovação
	$sbarenovacao = $db->pegaUm("SELECT sbarenovacao FROM fiesabatimento.solicitacaoabatimento WHERE sbaid = ".$_REQUEST['sbaid']);
	if($sbarenovacao == 't'){
		//$preidDescricao = $db->pegaUm("SELECT 'Renovação - ' || peremesinicioref || '/' || pereanoinicioref || ' à ' || peremesfimref || '/' || pereanofimref as descricao FROM fiesabatimento.parametrosrenovacao WHERE preid = ".$preid);
		$preidDescricao = "Renovação - {$solicitante['sbaanoinicio']} a {$solicitante['sbaanofim']}";
	}
	else{
		$preidDescricao = "Solicitação - {$solicitante['sbaanoinicio']} a {$solicitante['sbaanofim']}";
	}


	//$premes = $solicitante['premes'];
	//$preano = $solicitante['preano'];

	//$preidDescricao = "Solicitação - 01/2010 a {$premes}/{$preano}";
}

$dadosAtuacao = pegaAtuacaoSolicitacao($_REQUEST['sbaid'], $solicitante['sbaanoinicio'], $solicitante['sbaanofim'], $solicitante['sbarenovacao']);
$bloq = true;
if($dadosAtuacao){
	foreach( $dadosAtuacao as $escola ){
		if( $escola['atpidusuconfirmacao'] == '' ){
			$bloq = false;
		}
	}
}

if( $prazo ){
	$bloq = true;
}

//$docid = pegaDocidSolicitacao($_REQUEST['idoid'], $preid);
if( $docid == '' ){
	
	$sql = "SELECT
				docid
			FROM
				fiesabatimento.solicitacaoabatimento
			WHERE
				sbaid = ".$_REQUEST['sbaid'];
	
	$docid = $db->pegaUm( $sql );
}
$_SESSION['fiesabatimento_var']['cpfusuario'] = $solicitante['idocpf'];
$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = $docid");
$esdsc = $db->pegaUm("SELECT esddsc FROM workflow.estadodocumento WHERE esdid = $esdid");

$arrMeses = recuperaMesesFIES();
?>
<style type="text/css">  
.div_pop {  
    border: 1px solid #000;  
    width: 500px;  
    top: 40%;  
    left: 35%;  
    margin-top: -250px;  
    margin-left: -250px;  
    position: fixed; 
    margin: 2; 
    padding: 2; 
}  
</style> 
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>
$(document).ready(function(){

	$('#aguarde').hide();
	
	var offset = $("#sidebar").offset();
    var topPadding = 15;

    $('#meses').val($('[name*="rdn_confirmar_mes"]').length);

    $(window).scroll(function() {
        if ($(window).scrollTop() > offset.top) {
            $("#div_reabrir").stop().animate({
                marginTop: $(window).scrollTop() - offset.top + topPadding
            });
        } else {
            $("#div_reabrir").stop().animate({
                marginTop: 0
            });
        }
    });
	
	jQuery('.historico').click(function(){
		var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid=<?=$docid?>';
		window.open(
			url,
			'alterarEstado',
			'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
		);
	});
	/*
	$('.re_abre').click(function(){
		$('.div_pop').hide();
		$('#loader-container').show();
		$('#div_reabrir').show();
	});
	*/
	$('.re_fecha').click(function(){
		$('#loader-container').hide();
		$('.div_pop').hide();
	});
	
	<?php $arrHistorico = Array(WF_FIES1_APROVADA,WF_FIES1_REENVIO,WF_FIES1_REJEITADO);?>
	<?php if( in_array($esdid,$arrHistorico) || $bloq ){?>
		$('[type="checkbox"]').attr('disabled',true);
		$('[type="radio"]').attr('disabled',true);
		$('.re_abre').attr('disabled',true);
		$('[name="confirmar"]').attr('disabled',true);
	<?php } ?>

	$('[name="boAprovacao"]').click(function(){
		$('[name="confirmar"]').attr('disabled',false);
		if( $(this).val() == 'N2' ){
			$('#tr_exercicio').show();
		}else{
			$('#tr_exercicio').hide();
		}
		if( $(this).val() == 'N2' ){
			$('#tr_periodo').show();
			$('#div_confirma_periodo').slideDown();
		}else{
			$('#div_confirma_periodo').slideUp();
			$('#tr_periodo').hide();
		}
	});
	$('[name="boAprovacao"]::checked').each(function(){
		if( $(this).val() == 'N2' ){
			$('#tr_exercicio').show();
		}else{
			$('#tr_exercicio').hide();
		}
		if( $(this).val() == 'N2' ){
			$('#tr_periodo').show();
			$('#div_confirma_periodo').slideDown();
		}else{
			$('#div_confirma_periodo').slideUp();
			$('#tr_periodo').hide();
		}
	});
});


function confirmarSolicitacao()
{
	var pergunta = $('[name="boAprovacao"]::checked').val();

	if( pergunta == 'S' ){
		if( confirm('Concorda com tudo que foi declarado e deseja finalizar a análise dessa(s) atuação(ões) proficional(ais)?') ){
			jQuery("[name='formulario_solicitacao']").submit();
		}
	}else if( pergunta == 'N1' ){
		$('.div_pop').hide();
		$('#loader-container').show();
		$('#div_reabrir').show();
	}else if( pergunta == 'N2' ){
		$('#meses_confirmados').val($('[name*="rdn_confirmar_mes"]:checked').length);
		var qtd_meses = $('[name*="rdn_confirmar_mes"]:checked').length;
		
		/*
		if( qtd_meses < 1 ){
			alert('Informe pelo menos 1(um) mês.');
			return false;
		}
		*/
		
		var total_checado = 0;
		var flag_checado = false;
		jQuery.each( jQuery("[name^='rdn_confirmar_mes']") , function(k, obj){
			 
			 if(obj.checked == true){
			 	total_checado++;
			 }else{
			 	total_checado = 0;
			 }
			 
			 if(total_checado > 11) {
			 	flag_checado = true;
			 }
			 
		});
		
		if(!flag_checado){
			//alert("Favor informar no mínimo 12 meses ininterruptos!");
			alert("O professor deve possuir, a partir de 2010, 1 (um) ano de trabalho ininterrupto, na forma do regulamento, para se candidatar ao Abatimento 1% do FIES. A quantidade de meses informada pelo secretário não cumpre esse requisito mínimo para a aprovação.");
			return false;
		}
		
		if( confirm('Deseja finalizar a análise?') ){
			jQuery("[name='formulario_solicitacao']").submit();
		}
		
	}else if( pergunta == 'N3' ){
		$('.div_pop').hide();
		$('#loader-container').show();
		$('#div_rejeitar').show();
	}else{
		alert('Favor responder o campo \'Análise do Secretário\'.');
	}
}


<?php if($_SESSION['fiesabatimento_var']['alert']): ?>
jQuery(function() {
	alert('<?php echo $_SESSION['fiesabatimento_var']['alert'] ?>');
	<?php unset($_SESSION['fiesabatimento_var']['alert']) ?>
});
<?php endif; ?>

<?php if( $esdid == WF_FIES1_PENDENTE_DE_APROVACAO_PELO_SECRETARIO_DIRETOR_DE_ESCOLA_FEDERAL ){?>
	//jQuery('[type="radio"]').attr('disabled',true);
	//jQuery('[type="checkbox"]').attr('disabled',true);
<?php }?>
	
function confirmarMesAno(obj,cod_inep,ano,mes)
{
	
	var td = obj.parentNode;
	if(obj.checked == true){
		td.bgColor = "#80BC44";
		jQuery("[name='rdn_confirmar[" + cod_inep + "][" + ano + "]'][value='todos']").attr("checked",false);
		jQuery("[name='rdn_confirmar[" + cod_inep + "][" + ano + "]'][value='manual']").attr("checked",true);
	}else{
		td.bgColor = "#FFC211";
		jQuery("[name='rdn_confirmar[" + cod_inep + "][" + ano + "]'][value='todos']").attr("checked",false);
		jQuery("[name='rdn_confirmar[" + cod_inep + "][" + ano + "]'][value='manual']").attr("checked",true);
	}
	var total = jQuery("[name^='rdn_confirmar_mes[" + cod_inep + "][" + ano + "]']:checked").length;
	jQuery("#td_confirmados_" + cod_inep + "_" + ano).html(total);
	//mesRecursivo(obj,cod_inep,ano,mes);
}

function mesRecursivo(objx,cod_inep,ano,mes)
{
	
	
	if(objx.checked == false) objx = document.getElementById("rdn_confirmar_mes[" + cod_inep + "][" + ano + "][" + (mes-1) + "]");
	
	//limpo
	jQuery.each( jQuery("[name^='rdn_confirmar_mes[" + cod_inep + "]']") , function(k, obj){
		 obj.parentNode.bgColor = "#FFC211";
		 jQuery(obj).attr("checked",false);
	});
	jQuery("#td_confirmados_" + cod_inep + "_" + ano).html('0');
	
	
	
	//seleciono
	if(objx){
		var teste = true;
		var ano = new Array();
		jQuery.each( jQuery("[name^='rdn_confirmar_mes[" + cod_inep + "]']") , function(k, obj){
			 
			 if(teste==true){
			 	obj.parentNode.bgColor = "#80BC44";
			 }
			 
			 jQuery(obj).attr("checked",teste);
			 
			 
			 if(obj.name==objx.name){
			 	teste = false;
			 }
			 
			 anoAtual = obj.name.split("[")[2].replace("]","");
			
			 if(ano.indexOf(anoAtual) == -1){
			 	ano.push(anoAtual);
			 }
			 
		});
		
		var total = 0;
		for(i=0;i<ano.length;i++){
			total = jQuery("[name^='rdn_confirmar_mes[" + cod_inep + "][" + ano[i] + "]']:checked").length;
			jQuery("#td_confirmados_" + cod_inep + "_" + ano[i]).html(total);
		}
	}	 
}

function confirmarTodos(cod_inep,ano)
{
	jQuery.each( jQuery("[name^='rdn_confirmar_mes[" + cod_inep + "][" + ano + "]']") , function(k, obj){
		 obj.parentNode.bgColor = "#80BC44";
		 jQuery(obj).attr("checked",true);
	 });
	var total = jQuery("[name^='rdn_confirmar_mes[" + cod_inep + "][" + ano + "]']:checked").length;
	jQuery("#td_confirmados_" + cod_inep + "_" + ano).html(total);
}

function naoConfirmarTodos(cod_inep,ano)
{
	jQuery.each( jQuery("[name^='rdn_confirmar_mes[" + cod_inep + "][" + ano + "]']") , function(k, obj){
		 obj.parentNode.bgColor = "#FFC211";
		 jQuery(obj).attr("checked",false);
	 });
	jQuery("#td_confirmados_" + cod_inep + "_" + ano).html('0');
}

function reabrir(){
	
	var qtd = jQuery('[name="mocid[]"]::checked').length;
	if( qtd < 1 ){
		alert('Escolha pelo menos 1(um) motivo do envio para correção.');
		jQuery('[name="mocid[]"]').focus();
		return false;
	}
	var html = '';
	jQuery('[name="mocid[]"]::checked').each(function(){
		html += '<input type="hidden" name="mocid[]" value="'+jQuery(this).val()+'"/>';
	});
	jQuery('#motivos').html(html);
	
	if( confirm( 'Deseja enviar a solicitação para o professor corrigir?' ) ){
		var just = $('#reabrir_just').val();
		jQuery('#justificativa').val(just);
		jQuery('#requisicao').val('');
		jQuery('#req').val('reabrir');
		jQuery('#formulario_solicitacao').submit();
	}
}

function rejeitar(){
	var just = $('#rejeitar_just').val();
	if( just != null && confirm('Deseja rejeitar a solicitação?') ){
		jQuery('#justificativa').val(just);
		jQuery('#formulario_solicitacao').submit();
	}
}
	
</script>
<div id="loader-container" style="position: absolute; background-color: white; 
								  opacity: .6; width:110%; height:2000%; margin-top: -200px; 
								  margin-left: -20px; Z-index:22; display:none;" >
</div>
<center>
	<div style="background-color: white; Z-index:23; display:none;" id="div_reabrir" class="div_pop">
		<div>
			Identifique os campos incorretos:<br>
			<div style="text-align:left; margin-left: 40px; " >
			<?php 
			
			$sql = "SELECT
						mocid as codigo,
						mocdesc as descricao
					FROM
						fiesabatimento.motivocorrecao
					WHERE
						mocstatus = 'A'
					ORDER BY
						mocordem";
					
			$dadosJust = $db->carregar($sql);
			
			foreach( $dadosJust as $dadoJust ){
			?>
				<input type="checkbox" name="mocid[]" value="<?=$dadoJust['codigo'] ?>"/> <?=$dadoJust['descricao'] ?> <br>
			<?php 	
			}
			?>
			Favor detalhar o motivo:
			</div>
			<?=campo_textarea( 'reabrir_just', '', 'S', '', 70, 10, '', $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $value = null, $param = Array()) ?>
			<input type="button" onclick="reabrir()" value="Enviar para correção"> 
			<input type="button" class="re_fecha" value="Cancelar"> 
		</div>
	</div>
</center>
<center>
	<div style="background-color: white; Z-index:23; display:none;" id="div_rejeitar" class="div_pop">
		<div>
			Favor descrever o motivo da rejeição (o texto será apresentado ao professor solicitante):
			<?=campo_textarea( 'rejeitar_just', '', 'S', '', 70, 10, '', $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $value = null, $param = Array()) ?>
			<input type="button" onclick="rejeitar()" value="Rejeitar"> 
			<input type="button" class="re_fecha" value="Cancelar"> 
		</div>
	</div>
</center>
<form id="formulario_solicitacao" name="formulario_solicitacao" action="" method="post" >
	<div id="motivos"></div>
	<input type="hidden" id="requisicao" name="requisicao" value="confirmarSolicitacaoDiretor" />
	<input type="hidden" id="meses" name="meses" value="" />
	<input type="hidden" id="meses_confirmados" name="meses_confirmados" value="" />
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="justificativa" name="justificativa" value="" />
	<input type="hidden" id="idoid" name="idoid" value="<?=$_REQUEST['idoid'] ?>" />
	<input type="hidden" id="sbaid" name="sbaid" value="<?=$_REQUEST['sbaid'] ?>" />
	<input type="hidden" id="sbaanoinicio" name="sbaanoinicio" value="<?=$solicitante['sbaanoinicio'] ?>" />
	<input type="hidden" id="sbaanofim" name="sbaanofim" value="<?=$solicitante['sbaanofim'] ?>" />
	<input type="hidden" id="sbarenovacao" name="sbarenovacao" value="<?=$solicitante['sbarenovacao'] ?>" />
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita"windth="15%"><b>Orientações :</b></td>
			<td style="font-size:9pt;">
			<p>
				As informações do professor solicitante
				<?
				if($solicitante['sbarenovacao'] == 't'){
					echo ' da Renovação ';
				} 
				?>
				do abatimento (dados pessoais, atuação profissional e quantidade de meses trabalhados), estão descritas abaixo.
				<br> 
				Sr(a). Secretário(a), favor analisar as informações declaradas pelo professor, responda a pergunta do campo 'Análise do(a) Secretário(a) :' e em seguida clicar em 'Finalizar Análise' para concluir o processo de análise da atuação profissional.
			</p>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">Dados pessoais</td>
		</tr>
		<tr>
			<td colspan="2">
				<? htmlDadosUsuario($solicitante['idocpf']); ?>
			</td>
		</tr>
		 
		<tr>
			<td class="SubTituloDireita">Tipo :</td>
			<td><?=$preidDescricao;?></td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td class="SubTituloCentro">Atuação Profissional</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Atuação Profissional :</td>
			<td><? $arrDadosEscola = htmlAtuacaoSolicitacao($solicitante['sbaid'], true, false, $solicitante['sbaanoinicio'], $solicitante['sbaanofim'], $solicitante['sbarenovacao'], $sbames=12); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Análise do(a) Secretário(a) :</td>
			<td>
				O docente <?=$solicitante['idonome'] ?> informou na tabela acima a(s) atuação(ões) profissional(ais) exercida(s) em sua área de atuação.<br>
				<br>
				<b>O Sr. confirma que a(s) informação(ões) declarada(s) está(ão) correta(s)?<br></b>
				<br>
				<input type="radio" name="boAprovacao" value="S"  	<?=(trim($arrDadosEscola[0]['atprespsecretario']) == 'S' ? 'checked="checked"' : '') ?>/>  Sim, concordo com tudo que foi declarado. <br>
				<input type="radio" name="boAprovacao" value="N2" 	<?=(trim($arrDadosEscola[0]['atprespsecretario']) == 'N2' ? 'checked="checked"' : '') ?>/> Não, desejo informar os meses corretos e finalizar a análise. <br>
				<input type="radio" name="boAprovacao" value="N1" 	<?=(trim($arrDadosEscola[0]['atprespsecretario']) == 'N1' ? 'checked="checked"' : '') ?>/> Não, há um ou mais campos incorretos e desejo enviar para o professor corrigir. <br>
				<input type="radio" name="boAprovacao" value="N3" 	<?=(trim($arrDadosEscola[0]['atprespsecretario']) == 'N3' ? 'checked="checked"' : '') ?>/> Não, a(s) atuação(ões) está(ão) totalmete errada(s) e desejo rejeitar a solicitação. <br>
			</td>
		</tr>
		<tr id="tr_periodo" style="display:none">
			<td class="SubTituloDireita">Periodos de Atuação :</td>
			<td>
				<div id="div_confirma_periodo" style="display:none">
					<p>Observar os seguintes procedimentos para aprovação da quantidade de meses/ano solicitados pelo Professor:</p>
					<p>a) Para aprovação total de determinado ano (Confirmar todos os meses), selecionar o campo "Confirmar todos os 
					meses <input name="e1" type="radio" checked>" referente ao ano desejado.";</p>
					<p>b) Para aprovação parcial de determinado ano (aprovar alguns meses), selecionar o campo "Confirmação manual 
					<input name="e2" type="radio" checked>" referente ao ano desejado, selecionar os meses a serem aprovados.;</p>
					<p>c) Para rejeição total de determinado ano (Confirmar todos os meses), selecionar o campo "Confirmação manual 
					<input name="e3" type="radio" checked>" referente ao ano desejado, <b>não</b> selecionar os meses.</p>
					<p><b>Legenda:</b></p>
					<table border="0" cellpadding="1" >
						<tr>
							<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#FFC211"></div></td>
							<td><b>Declarado pelo Professor</b></td>
						</tr>
						<tr>
							<td width="13px" ><div style="width:12px;height:12px;border:solid 1px black;background-color:#80BC44"></div></td>
							<td><b>Aprovado pelo Secretário</b></td>
						</tr>
					</table>
					<br>
					<?php if($arrDadosEscola): ?>
						<?php $arrAtpid = Array();?>
						<?php foreach($arrDadosEscola as $escola): ?>
							<?php 	$arrConfirmacaoDiretor    = retornaConfirmacaoDiretor($escola['atpid']);
									$arrAtpid[] = $escola['atpid'];
							?>
							<input type="hidden" name="atuacoes_avaliadas[]" value="<?=$escola['atpid'] ?>" />
							<fieldset style="margin-bottom:15px">
								<legend><?php echo $escola['no_entidade'] ?></legend>
								<table align="center" class="listagem" width="100%" cellpadding="1" cellspacing="1" >
									<tr bgcolor="#f9f9f9" >
										<td class="SubTituloCentro" rowspan="2" >Ano</td>
										<td class="SubTituloCentro" colspan="2" >Qtde. Meses</td>
										<td class="SubTituloCentro" colspan="2">Confirmação</td>
										<?php foreach($arrMeses as $mes): ?>
											<td width="60px"  class="SubTituloCentro" rowspan="2" ><?php echo $mes['descricao'] ?></td>
										<?php endforeach; ?>
									</tr>
									<tr>
										<td class="SubTituloCentro" >Declarados pelo Professor</td>
										<td class="SubTituloCentro" >Confirmados pelo Secretário</td>
										<td class="SubTituloCentro" >Confirmar todos os meses</td>
										<td class="SubTituloCentro" >Confirmação manual</td>
									</tr>
									<?php $arrAnosEscola = recupaAnosEscola($escola, $solicitante['sbaanoinicio'], $solicitante['sbaanofim'], $solicitante['sbarenovacao'], $sbames=12); ?>
									<?php if($arrAnosEscola): ?>
										<?php $n=0;foreach($arrAnosEscola as $ano): ?>
											<?php $cor_tr = $n%2 ==1 ? "#ffffff" : "" ?>
											<tr bgcolor="<?php echo $cor_tr ?>" >
												<td align="center" ><?php echo $ano ?></td>
												<td align="center" id="td_confirmados_professor_<?php echo $escola['co_inep'] ?>_<?php echo $ano ?>" >0</td>
												<td align="center" id="td_confirmados_<?php echo $escola['co_inep'] ?>_<?php echo $ano ?>" >0</td>
												<td align="center" ><input type="radio" onclick="confirmarTodos('<?php echo $escola['co_inep'] ?>','<?php echo $ano ?>')" name="rdn_confirmar[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]" value="todos" /></td>
												<td align="center" ><input type="radio" onclick="naoConfirmarTodos('<?php echo $escola['co_inep'] ?>','<?php echo $ano ?>')" name="rdn_confirmar[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]" value="manual" /></td>
												<?php 

													/*
													if($preid){
														$DT_INICIO_PROGRAMA = '2013-01-01';
														$DT_FIM_PROGRAMA = '2013-12-31';
													}
													else{
														$DT_INICIO_PROGRAMA = '2010-01-01';
														$DT_FIM_PROGRAMA = '2014-12-31';
													}
													*/

													$DT_INICIO_PROGRAMA = "{$solicitante['sbaanoinicio']}-01-01";
													$DT_FIM_PROGRAMA = "{$solicitante['sbaanofim']}-12-31";


													$testeDtInicio = explode('-', $DT_INICIO_PROGRAMA);
													$testeDtInicio = $testeDtInicio[0].$testeDtInicio[1];
													
													$testeDtFim = explode('-', $DT_FIM_PROGRAMA);
													$testeDtFim = $testeDtFim[0].$testeDtFim[1];
													
												?>
												<?php foreach($arrMeses as $mes): ?>
													<?php 
														if($arrConfirmacaoDiretor[$ano][$mes['codigo']]){
															$cor = "#80BC44";
															$checked = "checked='checked'";
															$total_confirmados++;
														}else{
															$cor = "#FFC211";
															$checked = "";
														}
													
														$mescod = strlen($mes['codigo']) == 1 ? "0".$mes['codigo'] : $mes['codigo'];
														$dt_mescod = $ano.$mescod;
														
														$arrI = explode("-",$escola['dt_inicio']);
														$dt_ini = $arrI[0].$arrI[1];
														$dt_ini = !$dt_ini ? $testeDtInicio : $dt_ini;
														
														$arrF = explode("-",$escola['dt_fim']);
														$dt_fim = $arrF[0].$arrF[1];
														$dt_fim = !$dt_fim ? $testeDtFim : ($dt_fim > $testeDtFim ? $testeDtFim : $dt_fim);
														
														$permissaoCheckbox = ($dt_mescod >= $dt_ini && $dt_mescod <= $dt_fim) ? true : false;
	// 													$permissaoCheckbox = true; 
													?>
													<td align="center" <?php echo $permissaoCheckbox ? "bgcolor=\"$cor\"" : "" ?> >
														<?php if($permissaoCheckbox): ?>
															<input type="checkbox" <?php echo $checked ?> onclick="confirmarMesAno(this,'<?php echo $escola['co_inep'] ?>','<?php echo $ano ?>','<?php echo $mes['codigo'] ?>')" name="rdn_confirmar_mes[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>][<?php echo $mes['codigo'] ?>]" id="rdn_confirmar_mes[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>][<?php echo $mes['codigo'] ?>]" value="manual" />
														<?php else: ?>
															-
														<?php endif; ?>
													</td>
												<?php endforeach; ?>
											</tr>
											<script>
												jQuery("#td_confirmados_professor_<?php echo $escola['co_inep'] ?>_<?php echo $ano ?>").html(jQuery("[name^='rdn_confirmar_mes[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]']").length);
												jQuery("#td_confirmados_<?php echo $escola['co_inep'] ?>_<?php echo $ano ?>").html(jQuery("[name^='rdn_confirmar_mes[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]']:checked").length);
												if( jQuery("#td_confirmados_professor_<?php echo $escola['co_inep'] ?>_<?php echo $ano ?>").html() == jQuery("#td_confirmados_<?php echo $escola['co_inep'] ?>_<?php echo $ano ?>").html() ){
													jQuery("[name='rdn_confirmar[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]'][value='todos']").attr("checked",true);
												}else{
													jQuery("[name='rdn_confirmar[<?php echo $escola['co_inep'] ?>][<?php echo $ano ?>]'][value='manual']").attr("checked",true);
												}
											</script>
										<?php $n++;endforeach; ?>
									<?php endif; ?>
								</table>
							</fieldset>
							<?php endforeach; ?>
					<?php endif; ?>
				</div>
			</td>
		</tr>
		<tr id="tr_exercicio" style="display:none">
			<td class="SubTituloDireita">Efetivo execício :</td>
			<td>
				<b>O professor em questão está em efetivo exercício na(s) instituição(ões) apresentada(s) abaixo?<br></b>
				<table align="left" class="listagem" cellpadding="1" cellspacing="1" >
					<tr bgcolor="#f9f9f9" >
						<td rowspan="2">Escola</td>
						<td colspan="2">Em efetivo Exercício?</td>
					</tr>
					<tr bgcolor="#f9f9f9" >
						<td>Sim</td>
						<td>Não</td>
					</tr>
					<?php 
					if($arrDadosEscola){
						foreach( $arrDadosEscola as $escola ){
							if( in_array($esdid,$arrHistorico) || $bloq ){
								$teste = $escola['atpcompefetivoexercicio'];
							}else{
								$teste = $escola['atpprofefetivoexercicio'];
							}
							?>
							<tr>
								<td><?=$escola['no_entidade'] ?></td>
								<td><input type="radio" name="atpcompefetivoexercicio[<?=$escola['atpid'] ?>]" value="true" <?=($teste != 't' ? '' : 'checked="checked"') ?>/></td>
								<td><input type="radio" name="atpcompefetivoexercicio[<?=$escola['atpid'] ?>]" value="false" <?=($teste != 't' ? 'checked="checked"' : '') ?>/></td>
							</tr>
							<?php 
						}
					}
					?>
				</table>
			</td>
		</tr>
		<!--  
		<tr>
			<td class="SubTituloDireita">Meses trabalhados initerruptos :</td>
			<td>
				<?php 
				$dados = pegaAtuacaoSolicitacao($solicitante['sbaid'], $solicitante['sbaanoinicio'], $solicitante['sbaanofim'], $solicitante['sbarenovacao'], $sbames=12);
				
				$teste = calculaMeses( $dados, $solicitante['sbaanoinicio'], $solicitante['sbaanofim'], $sbames=12 );
				$meses = $teste['meses'];
				?>
				Quantidade de meses Informados/apurados 
				<?php //campo_texto("sbaqmtsolicitado", "N", "N", "Comprovados pelo professor/solicitante", 4, 3, "###", "", "", "", 0, "", "", $solicitante['sbaqmtsolicitado']); ?>
				<?=campo_texto("mesesininterruptos", "N", "N", "Comprovados pelo professor/solicitante", 4, 3, "###", "", "", "", 0, "", "", $meses); ?>
			</td>
		</tr>
		-->
				<!--  
		<tr>
			<td class="SubTituloDireita" >Aprovação, Reabertura ou Rejeição da solicitação : </td>
			<td>
				<p>
				As condições abaixo são necessárias para aprovação do abatimento. Os registros realizados na solicitação do abatimento indicam 
				que todas as condições foram atendidas pelo professor solicitante. Para ratificar, selecione a afirmação abaixo.
				<br>
				<font color="red">Obs.: A não seleção do item abaixo implicará na rejeição da solicitação do abatimento.</font>
				</p>
				<p>-->
				<?
// 				$sql = "SELECT mcrid, mcrdesc FROM fiesabatimento.motivorejeicao WHERE mcrstatus = 'A'";
// 				$tipos = $db->carregar($sql);
				
// 				foreach($tipos as $t){
					
// 					$checado = '';
// 					if($solicitante['sbaid']){
// 						$sql = "SELECT amrid 
// 								FROM fiesabatimento.atuacaomotivorejeicao 
// 								WHERE 
// 									amrstatus = 'A' 
// 									AND sbaid = ".$solicitante['sbaid']." 
// 									AND mcrid = ".$t['mcrid']."
// 									AND atpid in (".implode(',',$arrAtpid).")";
// 						$checado = $db->pegaUm($sql);
// 					}
// 					if($checado) $checado = "checked";
				?>
					<!--  
					<div style="float:left;margin-top:-5px;">
						<!-- <input type="checkbox" name="mcrid[]" value="<?=$t['mcrid']?>" <?=$checado?>> 
					</div>
					<div id="mcrid_<?=$t['mcrid']?>" style="float:left;"> - <?=$t['mcrdesc']?></div><br><br>
					-->
				<?//}?>
				<!--  
				<input type="checkbox" name="mcrid" id="mcrid" value="true" <?=( ( in_array($esdid,$arrHistorico) || $bloq ) ? 'checked="checked"' : '' ) ?>> 
				<b>O professor solicitante trabalhou no período do exercício informado (igual ou superior a 12 meses ininterruptos) na 
					função de Docente e cumpriu carga horária semanal igual ou superior a 20 horas semanais com vínculo empregatício temporário ou 
					efetivo vinculado a esta Secretaria.</b><br>
				<br>
				Finalize a aprovação da solicitação de abatimento, acionando a opção "Confirmar".
				</p>
			</td>
		</tr>
				-->
		<?php if( $esdsc != '' ){?>
		<!-- <tr>
			<td class="SubTituloDireita">Situação da Solicitação :</td>
			<td><?=$esdsc  ?></td>
		</tr> -->
		<?php }?>
		<tr>
			<td class="SubTituloCentro" ></td>
			<td class="SubTituloCentro" >
				<input type="button" name="voltar" value="Voltar" onclick="window.location='fiesabatimento.php?modulo=principal/listasolicitacaoabatimento&acao=A';">
				<?php if( $esdid == WF_FIES1_PENDENTE_DE_APROVACAO_PELO_SECRETARIO_DIRETOR_DE_ESCOLA_FEDERAL ||  $esdid == WF_FIES1_CANCELADA ){?>
					<!--  <input type="button" class="re_abre" value="Reabrir"> --> 
				<?php }?>
				<?php if( !(in_array($esdid,$arrHistorico) || $bloq) ){?>
					<input type="button" onclick="confirmarSolicitacao()" name="confirmar" value="Finalizar Análise" <?=($arrDadosEscola[0]['atprespsecretario'] != '' ? '' : 'disabled="disabled"') ?>> 
				<?php }?>
				<? if( $_REQUEST['idoid'] ) : ?>
					<!-- <a class="historico" style="cursor:pointer">Histórico</a> -->
				<? endif; ?> 
				
				<?
				$arrayPerfil = pegaPerfilGeral();
				if(in_array(PFL_SUPER_USUARIO,$arrayPerfil) || in_array(PFL_ADMINISTRADOR,$arrayPerfil)) {
					if( $esdid == WF_FIES1_ENVIADO_PROCESSAMENTO_BANCARIO) {
						?>
						<input type="button" name="inativar" value="Inativar Solicitação" onclick="alert('Não é possivel inativar, pois a solicitação já foi enviado para processamento bancário!');">
						<?
					}else{
						?>
						<input type="button" name="inativar" value="Inativar Solicitação" onclick="if(confirm('Deseja realmente inativar esta solicitação?')){location.href='fiesabatimento.php?modulo=principal/validacaoabatimento&acao=A&inativasbaid=OK&sbaid=<?= $_REQUEST['sbaid'] ?>';}">
						<?
					}
				}
				?>
			</td>
		</tr>
		<?php $arrHistorico = Array(WF_FIES1_APROVADA,WF_FIES1_REENVIO,WF_FIES1_REJEITADO);?>
		<?php if( in_array($esdid,$arrHistorico) ){?>
		<tr>
			<td class="SubTituloDireita">Finalização da solicitação:</td>
			<td>
			<?php 
				$sql = "SELECT DISTINCT
							hst.hstid,
							aed.aeddscrealizada
						FROM
							workflow.historicodocumento hst		
						INNER JOIN workflow.acaoestadodoc		aed ON aed.aedid = hst.aedid
						WHERE
							hst.docid = $docid AND aed.esdiddestino in (".WF_FIES1_APROVADA.",".WF_FIES1_REENVIO.",".WF_FIES1_REJEITADO.")
						ORDER BY
							1 DESC
						LIMIT 1";
				$dados = $db->carregar($sql);
				if( is_array($dados) ){
					foreach( $dados as $dado ){
						if( $temp != $dado['aeddscrealizada'] ){
							echo "Solicitação de Abatimento ".$dado['aeddscrealizada']."<br>";
							$temp = $dado['aeddscrealizada'];
						}
					}
				}
			?>
			</td>
		</tr>
		<?php }?>
	</table>
</form>

<?
/*
$atpid = 11046;
$docidAt = pegaDocidAtuacao($atpid);
			
if( $docidAt == '' ){
	$docidAt = wf_cadastrarDocumento( TPDID_ANALISE_SITUACAO, "Atuação Profissional - $sbaid - ".$_SESSION['fiesabatimento_var']['cpfusuario'] );
	
	$sql = "UPDATE fiesabatimento.atuacaoprofissional SET
				docid = $docidAt,
				atprespsecretario = NULL
				 
			WHERE
				atpid = $atpid;";
	$db->executar($sql);
	$db->commit();
}
*/
?>