<?

verificarTermoCompromisso(array("iusd"=>$_SESSION['sispacto'][$sis]['iusd'],"sis"=>$sis,"pflcod"=>$pflcod_avaliador));

$sql = "SELECT * FROM sispacto.grupoitensavaliacaocomplementar g 
		INNER JOIN sispacto.grupoitensavaliacaocomplementarperfil p ON p.gicid = g.gicid 
		INNER JOIN sispacto.itensavaliacaocomplementar i ON i.gicid = g.gicid 
		WHERE pflcod='".$pflcod_avaliador."'";

$dados = $db->carregar($sql);

if($dados[0]) {
	foreach($dados as $da) {
		$_arr[$da['gicdsc']][] = $da;
	}
}

$consulta_pfl = verificaPermissao();
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script>

jQuery(document).ready(function() {
  jQuery(window).keydown(function(event){
    if(event.keyCode == 13) {
      event.preventDefault();
      return false;
    }
  });
<? if($consulta_pfl) : ?>
	jQuery("#salvar").css('display','none');
	jQuery("#salvarcontinuar").css('display','none');
	jQuery("[name^='icc[']").attr('disabled','disabled');
<? endif; ?>

  
});


function avaliarComplementarEquipe(goto) {

	<? if($dados[0]) : ?>
	<? foreach($dados as $da) : ?>
	var num_liberados = jQuery("[name^='icc[<?=$da['iacid'] ?>]'][type^=radio]:enabled").length;
	if(num_liberados > 0) {
		var num_marcados = jQuery("[name^='icc[<?=$da['iacid'] ?>]']:checked").length;
		if(num_marcados==0) {
			alert('É obrigatório marcar todas as opções');
			return false;
		}
	}
	<? endforeach; ?>
	<? endif; ?>

	divCarregando();
	
	jQuery('#goto').val(goto);
	
	document.getElementById('formulario').submit();

}




</script>
<div id="modalAjuda" style="display:none;">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td id="ajudatd"></td>
</tr>
</table>
</div>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Avaliação Complementar Equipe</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao("/sispacto/sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliacaocomplementar"); ?></td>
</tr>
<tr>
	<td colspan="2" class="SubTituloCentro">
	<?
	carregarPeriodoReferencia(array('uncid'=>$_SESSION['sispacto'][$sis]['uncid'],'fpbid'=>$_REQUEST['fpbid'],'pflcod_avaliador'=>$pflcod_avaliador));
	?>
	</td>
</tr>
<? if($_REQUEST['fpbid']) : ?>
<tr>
	<td colspan="2">
	<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="iusdavaliador" value="<?=$_SESSION['sispacto'][$sis]['iusd'] ?>">
	<input type="hidden" name="goto" id="goto" value="">
	<input type="hidden" name="fpbid" id="fpbid" value="<?=$_REQUEST['fpbid'] ?>">
	<input type="hidden" name="requisicao" value="avaliarComplementarEquipe">
	<?
	
	$sql = "SELECT * FROM sispacto.itensavaliacaocomplementarcriterio";
	$itensavaliacaocomplementarcriterio = $db->carregar($sql);
	
	$sql = "SELECT * FROM sispacto.respostasavaliacaocomplementar WHERE iusdavaliador='".$_SESSION['sispacto'][$sis]['iusd']."' AND fpbid='".$_REQUEST['fpbid']."'";
	$respostasavaliacaocomplementar = $db->carregar($sql);
	
	if($respostasavaliacaocomplementar[0]) {
		foreach($respostasavaliacaocomplementar as $rac) {
			$_respostaac[$rac['iacid']] = $rac['iccid'];
		}
	}
	
	?>
	
	<? if($_arr) : ?>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro" width="30%">Fatores de avaliação</td>
		<td class="SubTituloCentro">Itens</td>
		<? montarAvaliacaoComplementar(array("itensavaliacaocomplementarcriterio" => $itensavaliacaocomplementarcriterio,"print" => "label")); ?>
	</tr>
	<? foreach($_arr as $fatores => $itens) : ?>
	<tr>
		<td rowspan="<?=count($itens) ?>" class="SubTituloDireita">
		<?=$fatores ?>
		<? 
		$consulta_av_com = false;
		if($itens[0]['gicselect']) {
			$sql = str_replace(array("{iusd}"),array($_SESSION['sispacto'][$sis]['iusd']),$itens[0]['gicselect']);
			$iusavaliado = $db->pegaLinha($sql);
			if($iusavaliado['iusd']) {
				echo "<br><b>".$iusavaliado['iusnome']."</b>";
			} else {
				echo "<br><span style=color:red;><b>Não possui</b><br>Procure COORDENADOR GERAL para verificar os motivos</span></b>";
				$consulta_av_com = true;
			}
		}
		?>
		</td>
		<td>
		<? echo $itens[0]['iacdsc']; ?>
		<? 
		if($iusavaliado) {
			echo "<input type=hidden name=iusavaliado[".$itens[0]['iacid']."] value=\"".$iusavaliado['iusd']."\">";
		}
		?>
		</td>
		<? montarAvaliacaoComplementar(array("consulta_av_com" => $consulta_av_com,"itensavaliacaocomplementarcriterio" => $itensavaliacaocomplementarcriterio,"print" => "radio","iacid" => $itens[0]['iacid'])); unset($itens[0]); ?>
	</tr>
	<? foreach($itens as $it) : ?>
	<tr>
		<td>
		<? echo $it['iacdsc']; ?>
		<?
		if($iusavaliado) {
			echo "<input type=hidden name=iusavaliado[".$it['iacid']."] value=\"".$iusavaliado['iusd']."\">";
		}
		?>
		</td>
		<? montarAvaliacaoComplementar(array("consulta_av_com" => $consulta_av_com,"itensavaliacaocomplementarcriterio" => $itensavaliacaocomplementarcriterio,"print" => "radio","iacid" => $it['iacid'])); ?>
	</tr>
	<? endforeach; ?>
	<? endforeach; ?>
	</table>
	<? endif; ?>
	</form>
	</td>
</tr>
<? endif; ?>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
	<? if($goto_ant) : ?>
	<input type="button" value="Anterior" onclick="divCarregando();window.location='<?=$goto_ant ?>';">
	<? endif; ?>
	<? if($goto_sal && !$consulta_av_com_todos) : ?>
	<input type="button" value="Salvar" id="salvar" onclick="avaliarComplementarEquipe('<?=$goto_sal ?>');">
	<? if($goto_pro && !$consulta_av_com_todos) : ?>
	<input type="button" value="Salvar e Continuar" id="salvarcontinuar" onclick="avaliarComplementarEquipe('<?=$goto_pro ?>');">	
	<? endif; ?>
	<? endif; ?>
	<? if($goto_pro) : ?>
	<input type="button" value="Próximo" onclick="divCarregando();window.location='<?=$goto_pro ?>';">
	<? endif; ?>
	</td>
</tr>
</table>