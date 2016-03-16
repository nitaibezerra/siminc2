<?
$percmax = '1.5';
$sql = "SELECT 
			valormaterialdidatico
		FROM 
			projovemcampo.planodeimplementacao WHERE pimid='".$_SESSION['projovemcampo']['pimid']."'";
$valormaterialdidatico = $db->pegaUm($sql);

$meta = pegameta();

$montante = calcularMontante();
$habilita = 'S';
$perfil = pegaPerfilGeral();
if(in_array(PFL_CONSULTA, $perfil)){
	$habilita = 'N';
}
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function gravarTransporteDidatico() {

	var span_maximo = parseFloat(replaceAll(replaceAll(document.getElementById('span_maximo').innerHTML,".",""),",","."));
	var span_utilizado = parseFloat(replaceAll(replaceAll(document.getElementById('span_utilizado').innerHTML,".",""),",","."));

	if( span_utilizado > span_maximo ) {
		alert('Valor Utilizado maior do que Valor Máximo previsto');
		return false;
	}
	
	jQuery('#form').submit();
}

function calculaPorcentTotal() {
	var valormaterialdidatico=0;
	if(document.getElementById('valormaterialdidatico')) {
		if(document.getElementById('valormaterialdidatico').value!='') {
			valormaterialdidatico 	= parseFloat(replaceAll(replaceAll(document.getElementById('valormaterialdidatico').value,".",""),",","."));
		}
	}
	
	var montante = parseFloat('<?=calcularMontante($meta) ?>');
	var total = valormaterialdidatico;

	var final = (total*100)/montante;
	
	document.getElementById('span_utilizado').innerHTML = mascaraglobal('###.###.###,##',total.toFixed(2));
	document.getElementById('percutilizado').value = final.toFixed(2);

}

jQuery(document).ready(function() {
<? if($esdid!=ESD_EMELABORACAO && !$db->testa_superuser()) : ?>
<? $desabilitado = true; ?>
	jQuery("[name^='valormaterialdidatico']").attr("disabled","disabled");
	jQuery("[name^='valormaterialdidatico']").attr("disabled","disabled");
<? endif; ?>
	jQuery('input').blur(function(){
		calculaPorcentTotal();
	});
});

</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarTransporteDidatico">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Orientações:</td>
		<td>
			<font color=blue>
				Discrimina-se a seguir até 1,5% do montante repassado para o pagamento do material didático-pedagógico pelo Governo Federal até as escolas de sua base territorial.
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
			<td align="center"><? echo campo_texto('percmax', 'N', 'N', 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="percmax"', '',$percmax, '' ); ?> R$ <span id="span_maximo"><?=number_format($percmax*$montante/100,2,",",".") ?></span></td>
			<td align="center"><? echo campo_texto('percutilizado', 'N', 'N', 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="percutilizado"', '', ($valormaterialdidatico?number_format($valormaterialdidatico*100/$montante,2,",","."):''), '' ); ?> R$ <span id="span_utilizado"><?=number_format($valormaterialdidatico,2,",",".") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Recursos Utilizados</td>
		<td><? echo campo_texto('valormaterialdidatico', 'N', $habilita, 'Percentual Máximo previsto(%)', 15, 14, "###.###.###,##", "", '', '', 0, 'id="valormaterialdidatico"', 'calculaPorcentTotal();', (($valormaterialdidatico)?number_format($valormaterialdidatico,2,",","."):''), '' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<? if(!$desabilitado) : ?>
			<input type="button" name="salvar" value="Salvar" onclick="gravarTransporteDidatico();">
		<? endif; ?></td>
	</tr>
</table>
</form>
<?registarUltimoAcesso(); ?>
