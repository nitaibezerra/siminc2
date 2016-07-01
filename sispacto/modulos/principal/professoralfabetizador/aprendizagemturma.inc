<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<?
$consulta_pfl = verificaPermissao();
$turmasprofessores = pegarTurmasProfessores(array('iusd' => $_SESSION['sispacto']['professoralfabetizador']['iusd'],'tpastatus' => 'A','tpaconfirmaregencia' => 'TRUE'));

if(!$turmasprofessores[0]) {
	$al = array("alert"=>"Não existem turmas regenciadas por você. Acesse a aba Dados da Turma e defina as turmas regentes","location"=>"sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dadosturmas");
	alertlocation($al);
}

?>
<form method="post" id="formulario">
<input type="hidden" name="requisicao" value="gravarAprendizagemTurma">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Aprendizagem da Turma</td>
</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao('/sispacto/sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=aprendizagemturma'); ?></td>
	</tr>

<? foreach($turmasprofessores as $turma) : ?>
<? 
 
if(($turma['tpatotalmeninos']+$turma['tpatotalmeninas'])==0) {
	$al = array("alert"=>"Não foram definidos valores da turma selecionada. Você esta sendo direcionado para tela para realizar as definições.","location"=>"sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dadosturmas");
	alertlocation($al);
}

?>
<tr>
	<td class="SubTituloDireita">UF / Município : </td>
	<td><?=$turma['estuf']." / ".$turma['mundescricao']	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">INEP / Escola : </td>
	<td><?=$turma['tpacodigoescola']." / ".$turma['tpanomeescola']	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Turma : </td>
	<td><?=$turma['tpanometurma']." / ".$turma['tpahorarioinicioturma']." - ".$turma['tpahorariofimturma']	?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><b>Número de alunos :</b> </td>
	<td><b><?=($turma['tpatotalmeninos']+$turma['tpatotalmeninas']) ?></b></td>
</tr>
<tr>
	<td colspan="2">
	<?

	$sql = "SELECT catdsc||'<input type=hidden name=catid[".$turma['tpaid']."][] value=\"'||catid||'\"><input type=hidden id=\"catdsc_'||catid||'\" value=\"'||catdsc||'\">', 
						   '<center><input type=\"text\" style=\"text-align:;\" name=\"actsim[".$turma['tpaid']."]['||catid||']\" size=\"5\" maxlength=\"2\" value=\"'||COALESCE((SELECT actsim::text FROM sispacto.aprendizagemconhecimentoturma c WHERE c.catid=ac.catid AND tpaid=".$turma['tpaid']."),'')||'\" onkeyup=\"this.value=mascaraglobal(\'##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"actsim_{$turma['tpaid']}_'||catid||'\" title=\"Meninos\" class=\"obrigatorio normal\"></center>' as sim, 
						   '<center><input type=\"text\" style=\"text-align:;\" name=\"actparcialmente[".$turma['tpaid']."]['||catid||']\" size=\"5\" maxlength=\"2\" value=\"'||COALESCE((SELECT actparcialmente::text FROM sispacto.aprendizagemconhecimentoturma c WHERE c.catid=ac.catid AND tpaid=".$turma['tpaid']."),'')||'\" onkeyup=\"this.value=mascaraglobal(\'##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"actparcialmente_{$turma['tpaid']}_'||catid||'\" title=\"Meninos\" class=\"obrigatorio normal\"></center>' as parcialmente, 
						   '<center><input type=\"text\" style=\"text-align:;\" name=\"actnao[".$turma['tpaid']."]['||catid||']\" size=\"5\" maxlength=\"2\" value=\"'||COALESCE((SELECT actnao::text FROM sispacto.aprendizagemconhecimentoturma c WHERE c.catid=ac.catid AND tpaid=".$turma['tpaid']."),'')||'\" onkeyup=\"this.value=mascaraglobal(\'##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"actnao_{$turma['tpaid']}_'||catid||'\" title=\"Meninos\" class=\"obrigatorio normal\"></center>' as nao FROM sispacto.aprendizagemconhecimento ac";
	
	$cabecalho = array("CONHECIMENTO/CAPACIDADE","SIM","PARCIALMENTE","NÃO");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	
	?>
	</td>
</tr>

<? endforeach; ?>

<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
		<input type="button" value="Anterior" onclick="divCarregando();window.location='sispacto.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=planoatividade';">
		<input type="button" value="Salvar" id="salvar" onclick="gravarAprendizagemTurma();">
	</td>
</tr>
</table>
</form>

<script>

jQuery(document).ready(function() {
<? if($consulta_pfl) : ?>
alert('ddd');
jQuery("#salvarcontinuar").css('display','none');
jQuery("#salvar").css('display','none');
jQuery("[name^='incluirnovaturma']").css('display','none');
jQuery("#salvar").css('display','none');
jQuery("[name^='desvincularturma']").css('display','none');
jQuery("#formulario :input").attr("disabled", "disabled");
<? endif; ?>

});

function gravarAprendizagemTurma() {
	
	var validado = true;
	
	<? foreach($turmasprofessores as $turma) : ?>

	jQuery("[name^='catid[<?=$turma['tpaid'] ?>][]']").each(function() {

			
		var actsim = 0;
		var actparcialmente = 0;
		var actnao = 0;
	
		if(jQuery('#actsim_<?=$turma['tpaid'] ?>_'+jQuery(this).val()).val()!='') {
			actsim = parseInt(jQuery('#actsim_<?=$turma['tpaid'] ?>_'+jQuery(this).val()).val());
		}
		
		if(jQuery('#actparcialmente_<?=$turma['tpaid'] ?>_'+jQuery(this).val()).val()!='') {
			actparcialmente = parseInt(jQuery('#actparcialmente_<?=$turma['tpaid'] ?>_'+jQuery(this).val()).val());
		}

		if(jQuery('#actnao_<?=$turma['tpaid'] ?>_'+jQuery(this).val()).val()!='') {
			actnao = parseInt(jQuery('#actnao_<?=$turma['tpaid'] ?>_'+jQuery(this).val()).val());
		}
		
		if((actsim+actparcialmente+actnao)!= <?=($turma['tpatotalmeninos']+$turma['tpatotalmeninas']) ?>) {
			alert('O somatório das colunas (Sim+Parcialmente+Não) deve ser igual ao total de alunos da turma');
			validado = false;
			return false;
		}

	});
	
	if(validado == false) {
		return false;
	}
	
	<? endforeach; ?>
	
	if(validado) {
		jQuery('#formulario').submit();
	}

	
}

</script>



	
