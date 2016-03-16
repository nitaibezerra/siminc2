<?php

$escrita = verificaPermissãoEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);

echo carregaAbasProInfancia("par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=cronograma&preid=".$_GET['preid'], $_GET['preid'],$descricaoItem);
monta_titulo( 'Cronograma Físico-Financeiro', $obraDescricao."<br /> Clique na quinzena correspondente para definir o período de execução da etapa."  );

$preid = $_REQUEST['preid'] ? $_REQUEST['preid'] : $_SESSION['par']['preid'];
$_SESSION['par']['preid'] = $preid;

$oSubacaoControle = new SubacaoControle();

$PreC = new PreCronograma();

$arrItens['item_quinzena_1'] = !$_POST['item_quinzena_1'] ? $PreC->carregaPreCronogramaPorQuinzena($preid,1) : $_POST['item_quinzena_1'];
$arrItens['item_quinzena_2'] = !$_POST['item_quinzena_2'] ? $PreC->carregaPreCronogramaPorQuinzena($preid,2) : $_POST['item_quinzena_2'];


$tipoObra = $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);
$tipoFundacao = $oSubacaoControle->verificaTipoFundacao($preid);
$arItensComposicao = $oSubacaoControle->recuperarItensComposicaoCronograma($tipoObra, $preid, true , $tipoFundacao);
$arItensComposicao = $arItensComposicao ? $arItensComposicao : array();
//$nrTotal = $oSubacaoControle->recuperarValorTotalItensComposicaoCronograma($tipoObra, $preid, true, $tipoFundacao);

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$boAtivo = 'N';
$stAtivo = 'disabled="disabled"';
$travaCorrecao = true;
if( $esdid ){

	$arrReformulacao = Array(WF_TIPO_EM_CADASTRAMENTO, WF_TIPO_EM_CORRECAO, WF_TIPO_EM_ANALISE_DILIGENCIA, WF_TIPO_EM_REFORMULACAO, WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL);
	if( is_array($respSim) && !in_array($esdid, $arrReformulacao) ){
		$travaCorrecao = in_array(QUESTAO_CRONOGRAMA,$respSim);
	}
	
	$arrPerfil = array(PAR_PERFIL_EQUIPE_MUNICIPAL, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,PAR_PERFIL_PREFEITO,PAR_PERFIL_SUPER_USUARIO);
	$arrSituacao = array(WF_TIPO_VALIDACAO_DEFERIMENTO, WF_TIPO_EM_CORRECAO);
	if( in_array($esdid, $arrReformulacao) ){
		$boAtivo = 'S';
		$stAtivo = '';
	}elseif( in_array($esdid, $arrSituacao) && possuiPerfil($arrPerfil) && !$travaCorrecao){
		$boAtivo = 'S';
		$stAtivo = '';
	}else{
		$boAtivo = 'N';
		$stAtivo = 'disabled="disabled"';
	}
}

	# Código refeito em 22/10/2012. Regra para liberação da tela para (cadastramento e ateração) dos perfil abaixos listados nas seguintes situações também listadas abaixo.
	# Foi inserido os perfis Estaduais e a situação em Diligência.
	# Foi também inserido o os perfis. (não havia perfil, era verificado apenas o estado).
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	if(	in_array($esdid, $arrReformulacao) &&
		(
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
			in_array(PAR_PAR_PERFIL_PREFEITO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) ||
			in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
			in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil)
		)
	){
		$boAtivo = 'S';
		$stAtivo = '';
	}

//$sql = "select COALESCE(oi.obrpercexec, 0)from obr as.obr ainfraestrutura oi where oi.preid = $preid";

$sql = "SELECT COALESCE(obr.obrpercentultvistoria, 0) FROM obras2.obras obr WHERE obr.preid = $preid";

$percexec = $db->pegaUm( $sql );
// nova situação, se tiver mais de 0% de execução da obra... desabilitar
if((float)$percexec > 0) {
	$boAtivo = 'N';
	$stAtivo = 'disabled="disabled"';
}

$sql = "select
			pre1.preid as preid1,
			pre2.preid as preid2,
			pre1.ptoid as tipo1,
			pre2.ptoid as tipo2
		from
			obras.preobra pre1
		left join
			obras.preobra pre2 on pre1.preid = pre2.preidpai
		where
			pre1.preid = ".$preid;

$rsTipoObras = $db->pegaLinha($sql);

if($esdid == WF_TIPO_EM_REFORMULACAO){
	if(!empty($rsTipoObras['preid1']) && !empty($rsTipoObras['preid2'])){
// 		if($rsTipoObras['tipo1'] != $rsTipoObras['tipo2']){
			$boAtivo = 'S';
			$stAtivo = '';
// 		}
	}
}

/*
 * REGRA TEMPORARIA 02/05/2012
 * SOLICITADO PELO DANIEL AREAS
 * LIBERA EDIÇÃO PARA
 * MUNICIPIO COM OBRA
 * EM REFORMULAÇÃO
 *
 * MUNICIPIOS: SORRISO/MT
 */
//if(in_array($_SESSION['par']['muncod'], array(5107925)) && in_array($esdid, array(WF_TIPO_EM_REFORMULACAO)) ){
//	$boAtivo = 'S';
//	$stAtivo = '';
//}

if( ($_POST['item_quinzena_1'] || $_POST['item_quinzena_2']) && $boAtivo == 'S' ):
	$PreC->salvaCronogramaPorQuinzena($preid);
	echo '<script type="text/javascript"> alert("Operação realizada com sucesso.");</script>';
endif;
?>
<style>
	.marcado{background-color:#228B22;color:#FFFFFF;}
	.desmarcado{background-color:#E0EEEE;}
	.tabela_quinzena td{border:solid 1px black;height:15px;}
	.tabela_quinzena{width:100%}

</style>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">
<!--
jQuery.noConflict();

jQuery(document).ready(function(){

	jQuery('.enviar').click(function(){

		if(verificaFormulario() == false){
			alert('É necessário informar pelo menos uma quinzena para cada Ítem!');
			return false;
		}

		if(this.value == 'Salvar'){
			jQuery('#acao').val('salvar');
		}

		if(this.value == 'Salvar e próximo'){
			jQuery('#acao').val('proximo');
		}

		if(this.value == 'Salvar e anterior'){
			jQuery('#acao').val('anterior');
		}

		document.formulario.submit();
	});

	jQuery('.navegar').click(function(){

		if(this.value == 'Próximo'){
			aba = 'documento';
		}

		if(this.value == 'Anterior'){
			aba = 'planilhaOrcamentaria';
		}

		document.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba='+aba+'&preid='+<?php echo $preid ?>;
	});
});

function marcar(obj,itcid,mes,quinzena){
	if( jQuery(obj).attr("class") == "desmarcado" ){
		jQuery(obj).attr("class","marcado");
		jQuery("#item_" + itcid + "_mes_" + mes + "_quinzena_" + quinzena).val("1");
	}else{
		jQuery(obj).attr("class","desmarcado");
		jQuery("#item_" + itcid + "_mes_" + mes + "_quinzena_" + quinzena).val("");
	}
}

//-->
</script>
<?php echo cabecalho();?>
<?php if($boAtivo == 'S' && count($respSim)): ?>
	<?php
	$txtAjuda = "No Cronograma Físico-financeiro disponibilizado no sistema devem ser marcados os campos de acordo com as etapas de execução dos serviços, determinando o andamento geral da obra e o percentual a ser executado em função do tempo para cada grande item que compõe a Planilha Orçamentária.";
	$imgAjuda = "<img alt=\"{$txtAjuda}\" title=\"{$txtAjuda}\" src=\"/imagens/ajuda.gif\">";
	?>
	<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
		<tr>
			<td width="100" style="text-align: right;" class="SubTituloDireita">Ajuda:</td>
			<td width="90%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
				<?php echo $imgAjuda ?>
			</td>
		</tr>
	</table>
<?php endif; ?>
<?php if(!empty($arItensComposicao) && $arItensComposicao[0]): ?>
	<form name="formulario" action="" method="post">
	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="0" class="listagem">
		<thead>
			<tr>
				<td rowspan="2" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ordem</strong></td>
				<td rowspan="2" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
				<?php $qtdMeses = 9 ?>
				<?php for($i = 1 ; $i <= $qtdMeses ; $i++ ): ?>
					<td colspan="2" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Mês <?php echo $i ?></strong></td>
				<?php endfor; ?>
				<td rowspan="2" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor do Item (R$)</strong></td>
				<td rowspan="2" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>(%) Referente a Obra <br/> (A)</strong></td>
			</tr>
			<tr>
				<?php for($i = 1 ; $i <= $qtdMeses ; $i++ ): ?>
					<td style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="center" class="title" ><strong>Q1</strong></td>
					<td style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="center" class="title" ><strong>Q2</strong></td>
				<?php endfor; ?>
			</tr>
		</thead>
		<tbody>
		<?php $x = 0 ?>
		<?php foreach($arItensComposicao as $item): ?>
			<?php
			$cor = ($x % 2) ? "#F7F7F7" : "white";
			?>
			<tr bgcolor="<?php echo $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $cor ?>';">
				<td align="center"><?php echo $item['itcordem'] ?></td>
				<td><?php echo ucwords($item['itcdescricao']) ?></td>
				<?php for($i = 1 ; $i <= $qtdMeses ; $i++ ): ?>
					<td colspan="2" style="cursor:pointer">
						<table class="tabela_quinzena" cellspacing="1" cellpadding="0" >
							<tr>
								<td class="<?php echo $arrItens['item_quinzena_1'][ $item['itcid'] ][ $i ] ? "marcado" : "desmarcado" ?>" title="Clique aqui para Marcar / Desmarcar a Quinzena 1 do Mês <?php echo $i ?>" onclick="<?php echo $boAtivo == 'S' ? "marcar(this,'".$item['itcid']."','".$i."','1')" : ''; ?>" align="center"><input value="<?php echo $arrItens['item_quinzena_1'][ $item['itcid'] ][ $i ] ? "1" : "" ?>"  type="hidden" id="item_<?php echo $item['itcid'] ?>_mes_<?php echo $i ?>_quinzena_1" name="item_quinzena_1[<?php echo $item['itcid'] ?>][<?php echo $i ?>]" ></td>
								<td class="<?php echo $arrItens['item_quinzena_2'][ $item['itcid'] ][ $i ] ? "marcado" : "desmarcado" ?>" title="Clique aqui para Marcar / Desmarcar a Quinzena 2 do Mês <?php echo $i ?>" onclick="<?php echo $boAtivo == 'S' ? "marcar(this,'".$item['itcid']."','".$i."','2')" : ''; ?>" align="center"><input value="<?php echo $arrItens['item_quinzena_2'][ $item['itcid'] ][ $i ] ? "1" : "" ?>"  type="hidden" id="item_<?php echo $item['itcid'] ?>_mes_<?php echo $i ?>_quinzena_2" name="item_quinzena_2[<?php echo $item['itcid'] ?>][<?php echo $i ?>]" ></td>
							</tr>
						</table>
					</td>
				<?php endfor; ?>
				<td align="right">
					<?php $valor = $PreC->getPreItensComposicaoFilhos($item['itccodigoitem'],$preid,0,$tipoObra) ?>
					<span title="Valor Unitário * Quantidade do Ítem"><?php echo formata_valor( $valor ) ?><span>
					<?php $total += $valor ?>
					<?php $arrValores[$item['itcid']] = $valor; ?>
				</td>
				<td align="right" id="td_percent_<?php echo $item['itcid'] ?>" >
				</td>
			</tr>
			<?php $x++ ?>
		<?php endforeach; ?>
		</tbody>
	<tfoot>
		<tr bgcolor="#f0f0f0" height="30" >
			<td align="right" colspan="<?php echo 2 + ($qtdMeses * 2) ?>"><strong>Total:</strong></td>
			<td align="right"><strong><?php echo formata_valor($total) ?></strong></td>
			<td align="right"><strong><?php echo ($total > 0) ? '100%' : '' ?></strong></td>
		</tr>
	</tfoot>
	</table>
	<?php endif; ?>
<?php $arrValores = !$arrValores ? array() : $arrValores ?>
<script>
<?php foreach($arrValores as $itcid => $valor): ?>
	jQuery("#td_percent_<?php echo $itcid ?>").html("<?php echo formata_valor(round( (( !$valor || !$total ? 0 : $valor/$total ) * 100),2)) ?>");
<?php endforeach; ?>

function verificaFormulario(){
<?php foreach($arrValores as $itcid => $valor): ?>
		var valor_item_<?php echo $itcid ?> = 0;
		<?php for($i = 1; $i <= $qtdMeses; $i++) :?>
		if(jQuery("#item_<?php echo $itcid ?>_mes_<?php echo $i ?>_quinzena_1").val() == "1" || jQuery("#item_<?php echo $itcid ?>_mes_<?php echo $i ?>_quinzena_2").val()  == "1"){
			valor_item_<?php echo $itcid ?> += 1;
		}
		<?php endfor; ?>
	if(valor_item_<?php echo $itcid ?> == 0 && jQuery("#td_percent_<?php echo $itcid ?>").html() != "0,00"){
		return false;
	}
<?php endforeach; ?>
	return true;
}

</script>

<?php if(!empty($arItensComposicao) && $arItensComposicao[0]): ?>
	<table width="95%" align="center" bgcolor="#DCDCDC">
		<tr>
			<td align="left">
				<input class="navegar" type="button" value="Anterior" />
			</td>
			<td align="center">
				<?php
				if( $boAtivo == 'S' ){
				?>
					<input class="enviar" type="button" value="Salvar e anterior" <?php echo $stAtivo ?>/>
					<input class="enviar" type="button" value="Salvar" <?php echo $stAtivo ?>>
					<input class="enviar" type="button" value="Salvar e próximo" <?php echo $stAtivo ?>/>
					<input type="hidden" name="acao" id="acao" value="">
				<?php
				}
				?>
				<input class="fechar" type="button" value="Fechar" onclick="atualizarObra();" />
			</td>
			<td align="right">
				<input class="navegar" type="button" value="Próximo" />
			</td>
		</tr>
	</table>
<?php else: ?>
	<center><p>Não existem registros.</p></center>
<?php endif; ?>
</form>