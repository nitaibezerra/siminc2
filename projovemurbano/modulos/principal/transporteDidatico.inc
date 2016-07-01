<?

$sql = "SELECT * FROM projovemurbano.transportematerial WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
$transportematerial = $db->pegaLinha($sql);

if(!$transportematerial) {
	$sql = "INSERT INTO projovemurbano.transportematerial(
            pjuid)
    		VALUES ('".$_SESSION['projovemurbano']['pjuid']."') RETURNING tmaid;";
	
	$_SESSION['projovemurbano']['tmaid'] = $db->pegaUm($sql);
	$db->commit();

} else {
	$_SESSION['projovemurbano']['tmaid'] = $transportematerial['tmaid'];	
}

if($_SESSION['projovemurbano']['muncod']) {
	$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
	$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta WHERE cmecodibge='".$_SESSION['projovemurbano']['muncod']."'");		
	if($sugestaoampliacao['suaverdade']=="t") {
		if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
	}
} 
if($_SESSION['projovemurbano']['estuf']) {
	$sugestaoampliacao = $db->pegaLinha("SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
	$meta = $db->pegaUm("SELECT cmemeta FROM projovemurbano.cargameta c INNER JOIN territorios.estado e ON e.estcod::numeric=c.cmecodibge WHERE c.cmetipo='E' AND e.estuf='".$_SESSION['projovemurbano']['estuf']."'");		
	if($sugestaoampliacao['suaverdade']=="t") {
		if($sugestaoampliacao['suametaajustada']) $meta = $sugestaoampliacao['suametaajustada'];
	}
}

$habilita = 'S';
$perfil = pegaPerfilGeral();
if(in_array(PFL_CONSULTA, $perfil)){
	$habilita = 'N';
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function gravarTransporteDidatico() {

	if(parseFloat(document.getElementById('tmaperutilizado').value) > parseFloat(document.getElementById('tmapercmax').value)) {
		alert('Percentual Utilizado(%) maior do que Percentual Máximo previsto(%)');
		return false;
	}

	jQuery('#form').submit();
}

function calculaPorcentTotal() {
	var tmarecursoutilizado=0;
	if(document.getElementById('tmarecursoutilizado')) {
		if(document.getElementById('tmarecursoutilizado').value!='') {
			tmarecursoutilizado 	= parseFloat(replaceAll(replaceAll(document.getElementById('tmarecursoutilizado').value,".",""),",","."));
		}
	}
	
	var montante = parseFloat('<?=calcularMontante($meta) ?>');
	var total = tmarecursoutilizado;

	var final = (total*100)/montante;
	
	document.getElementById('span_utilizado').innerHTML = mascaraglobal('###.###.###,##',total.toFixed(2));
	document.getElementById('tmaperutilizado').value = final.toFixed(1);

}

<? if($esdid!=ESD_EMELABORACAO && !$db->testa_superuser()) : ?>
<? $desabilitado = true; ?>
jQuery(document).ready(function() {
	jQuery("[name^='tmarecursoutilizado']").attr("disabled","disabled");
	jQuery("[name^='tmarecursoutilizado']").attr("disabled","disabled");
});
<? endif; ?>

</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarTransporteDidatico">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Orientações:</td>
		<td>
		<font color=blue>
		<p>Para os Estados e DF: discrimina-se a seguir até 1,5% do montante repassado para pagamento do transporte do material didático-pedagógico entregue pelo Governo Federal, da capital até os Municípios.</p>
		</font>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Percentual da Ação</td>
		<td>
		
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Percentual Máximo previsto(%)</td>
			<td class="SubTituloCentro">Percentual utilizado(%)</td>
		</tr>
		<tr>
			<td align="center"><? echo campo_texto('tmapercmax', 'N', 'N', 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="tmapercmax"', '', (($transportematerial['tmapercmax'])?$transportematerial['tmapercmax']:'1.5'), '' ); ?> R$ <span id="span_maximo"><?=number_format(calcularMontante($meta)*(($transportematerial['tmapercmax'])?$transportematerial['tmapercmax']:'1.5')/100,2,",",".") ?></span></td>
			<td align="center"><? echo campo_texto('tmaperutilizado', 'N', 'N', 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="tmaperutilizado"', '', (($transportematerial['tmaperutilizado'])?$transportematerial['tmaperutilizado']:''), '' ); ?> R$ <span id="span_utilizado"><?=number_format($transportematerial['tmarecursoutilizado'],2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Recursos Utilizados</td>
		<td><? echo campo_texto('tmarecursoutilizado', 'N', $habilita, 'Percentual Máximo previsto(%)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="tmarecursoutilizado"', 'calculaPorcentTotal();', (($transportematerial['tmarecursoutilizado'])?number_format($transportematerial['tmarecursoutilizado'],2,",","."):''), '' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<? if(!$desabilitado) : ?>
			<input type="button" name="salvar" value="Salvar" onclick="gravarTransporteDidatico();">
		<? endif; ?></td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>
