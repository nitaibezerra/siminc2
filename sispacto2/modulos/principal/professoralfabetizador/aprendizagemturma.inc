<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<?
$consulta = verificaPermissao();

$turmasprofessores = pegarTurmasProfessores(array('iusd' => $_SESSION['sispacto2']['professoralfabetizador']['iusd'],'tpastatus' => 'A','tpaconfirmaregencia' => 'TRUE'));

if(!$turmasprofessores[0]) {
	$al = array("alert"=>"Não existem turmas regenciadas por você. Acesse o 1º mês de referência e defina as turmas regentes","location"=>"sispacto2.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias");
	alertlocation($al);
}

?>
<script>

jQuery(document).ready(function() {
	<? if($consulta) : ?>
	jQuery("#salvar").css('display','none');
	jQuery("#formulario :input").attr("disabled", "disabled");
	<? endif; ?>

	});

function somarTotalGenero(tpaid) {

	var soma = 0;
	
	if(jQuery('#tpatotalmeninos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpatotalmeninos_'+tpaid).val());
	}
	
	if(jQuery('#tpatotalmeninas_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpatotalmeninas_'+tpaid).val());
	}

	jQuery('#tpatotalalunos_'+tpaid).val(soma);
}

function somarTotalFaixaEtaria(tpaid) {

	var soma = 0;
	
	if(jQuery('#tpafaixaetariaabaixo6anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafaixaetariaabaixo6anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafaixaetaria6anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafaixaetaria6anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafaixaetaria7anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafaixaetaria7anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafaixaetaria8anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafaixaetaria8anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafaixaetaria9anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafaixaetaria9anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafaixaetaria10anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafaixaetaria10anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafaixaetaria11anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafaixaetaria11anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafaixaetariaacima11anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafaixaetariaacima11anos_'+tpaid).val());
	}
	

	jQuery('#tpatotalalunosfaixaetaria_'+tpaid).val(soma);
}


</script>
<form method="post" id="formulario">
<input type="hidden" name="requisicao" value="gravarAprendizagemTurma">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Aprendizagem da Turma
	<?php 
	switch($cattipo) {
		case 'M':
			echo " ( Matemática )";
			break;
		case 'P';
			echo " ( Português )";
			break;
	}
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao("/sispacto2/sispacto2.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&rfuparcela=".$rfuparcela); ?></td>
</tr>
<? foreach($turmasprofessores as $turma) : ?>
<? 
 
if(($turma['tpatotalmeninos']+$turma['tpatotalmeninas'])==0) {
	$al = array("alert"=>"Não foram definidos valores da turma selecionada. Acesse o 1º mês de referência para realizar as definições.","location"=>"sispacto2.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias");
	alertlocation($al);
}

if(($turma['tpatotalmeninos'.$rfuparcela]+$turma['tpatotalmeninas'.$rfuparcela]) > 0) $gravado = $rfuparcela;

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
	<td colspan="2">
	<br>
	<span style="font-size:x-small;">Confirma os quantitativos de alunos em nas suas turmas? <b>(Caso haja desistências ou inserções, favor inserir e ao final do preenchimento clique em salvar para carregar as questões de aprendizagem)</b></span>
	<br><br>
	<input type="hidden" name="tpaid[]" value="<?=$turma['tpaid'] ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	
	<tr>
		<td class="SubTituloCentro">&nbsp;</td>
		<td class="SubTituloCentro" style="font-size: x-small">Meninos</td>
		<td class="SubTituloCentro" style="font-size: x-small">Meninas</td>
		<td class="SubTituloCentro" style="font-size: x-small">Total</td>
		<td class="SubTituloCentro">&nbsp;</td>
		<td class="SubTituloCentro" style="font-size: x-small"> < 6 anos</td>
		<td class="SubTituloCentro" style="font-size: x-small">6 anos</td>
		<td class="SubTituloCentro" style="font-size: x-small">7 anos</td>
		<td class="SubTituloCentro" style="font-size: x-small">8 anos</td>
		<td class="SubTituloCentro" style="font-size: x-small">9 anos</td>
		<td class="SubTituloCentro" style="font-size: x-small">10 anos</td>
		<td class="SubTituloCentro" style="font-size: x-small">11 anos</td>
		<td class="SubTituloCentro" style="font-size: x-small"> > 11 anos</td>
		<td class="SubTituloCentro" style="font-size: x-small">Total</td>
		
	</tr>
	<tr>
		<td class="SubTituloDireita"><b style="font-size: x-small">Total Por gênero:</b></td>
		<td align="center"><?=campo_texto('tpatotalmeninos['.$turma['tpaid'].']', "N", "S", "Meninos", 3, 3, "######", "", '', '', 0, 'id="tpatotalmeninos_'.$turma['tpaid'].'"', 'somarTotalGenero('.$turma['tpaid'].');', $turma['tpatotalmeninos'.$gravado] ); ?></td>
		<td align="center"><?=campo_texto('tpatotalmeninas['.$turma['tpaid'].']', "N", "S", "Meninas", 3, 3, "######", "", '', '', 0, 'id="tpatotalmeninas_'.$turma['tpaid'].'"', 'somarTotalGenero('.$turma['tpaid'].');', $turma['tpatotalmeninas'.$gravado] ); ?></td>
		<td align="center"><?=campo_texto('tpatotalalunos['.$turma['tpaid'].']', "N", "N", "Número de alunos na turma", 3, 3, "######", "", '', '', 0, 'id="tpatotalalunos_'.$turma['tpaid'].'"', '', ($turma['tpatotalmeninos'.$gravado]+$turma['tpatotalmeninas'.$gravado]) ); ?></td>
		<td class="SubTituloDireita"><b style="font-size: x-small">Total Por Faixa Etária:</b></td>
		<td><?=campo_texto('tpafaixaetariaabaixo6anos['.$turma['tpaid'].']', "N", "S", "Abaixo de 6 anos", 3, 3, "#########", "", '', '', 0, 'id="tpafaixaetariaabaixo6anos_'.$turma['tpaid'].'"', 'somarTotalFaixaEtaria('.$turma['tpaid'].');', $turma['tpafaixaetariaabaixo6anos'.$gravado] ); ?></td>
		<td><?=campo_texto('tpafaixaetaria6anos['.$turma['tpaid'].']', "N", "S", "6 anos completos", 3, 3, "#########", "", '', '', 0, 'id="tpafaixaetaria6anos_'.$turma['tpaid'].'"', 'somarTotalFaixaEtaria('.$turma['tpaid'].');', $turma['tpafaixaetaria6anos'.$gravado] ); ?></td>
		<td><?=campo_texto('tpafaixaetaria7anos['.$turma['tpaid'].']', "N", "S", "7 anos completos", 3, 3, "#########", "", '', '', 0, 'id="tpafaixaetaria7anos_'.$turma['tpaid'].'"', 'somarTotalFaixaEtaria('.$turma['tpaid'].');', $turma['tpafaixaetaria7anos'.$gravado] ); ?></td>
		<td><?=campo_texto('tpafaixaetaria8anos['.$turma['tpaid'].']', "N", "S", "8 anos completos", 3, 3, "#########", "", '', '', 0, 'id="tpafaixaetaria8anos_'.$turma['tpaid'].'"', 'somarTotalFaixaEtaria('.$turma['tpaid'].');', $turma['tpafaixaetaria8anos'.$gravado] ); ?></td>
		<td><?=campo_texto('tpafaixaetaria9anos['.$turma['tpaid'].']', "N", "S", "9 anos completos", 3, 3, "#########", "", '', '', 0, 'id="tpafaixaetaria9anos_'.$turma['tpaid'].'"', 'somarTotalFaixaEtaria('.$turma['tpaid'].');', $turma['tpafaixaetaria9anos'.$gravado] ); ?></td>
		<td><?=campo_texto('tpafaixaetaria10anos['.$turma['tpaid'].']', "N", "S", "10 anos completos", 3, 3, "#########", "", '', '', 0, 'id="tpafaixaetaria10anos_'.$turma['tpaid'].'"', 'somarTotalFaixaEtaria('.$turma['tpaid'].');', $turma['tpafaixaetaria10anos'.$gravado] ); ?></td>
		<td><?=campo_texto('tpafaixaetaria11anos['.$turma['tpaid'].']', "N", "S", "11 anos completos", 3, 3, "#########", "", '', '', 0, 'id="tpafaixaetaria11anos_'.$turma['tpaid'].'"', 'somarTotalFaixaEtaria('.$turma['tpaid'].');', $turma['tpafaixaetaria11anos'.$gravado] ); ?></td>
		<td><?=campo_texto('tpafaixaetariaacima11anos['.$turma['tpaid'].']', "N", "S", "Acima de 11 anos", 3, 3, "#########", "", '', '', 0, 'id="tpafaixaetariaacima11anos_'.$turma['tpaid'].'"', 'somarTotalFaixaEtaria('.$turma['tpaid'].');', $turma['tpafaixaetariaacima11anos'.$gravado] ); ?></td>
		<td><?=campo_texto('tpatotalalunosfaixaetaria['.$turma['tpaid'].']', "N", "N", "Número de alunos na turma", 3, 3, "#########", "", '', '', 0, 'id="tpatotalalunosfaixaetaria_'.$turma['tpaid'].'"', '', ($turma['tpafaixaetariaabaixo6anos'.$gravado]+$turma['tpafaixaetaria6anos'.$gravado]+$turma['tpafaixaetaria7anos'.$gravado]+$turma['tpafaixaetaria8anos'.$gravado]+$turma['tpafaixaetaria9anos'.$gravado]+$turma['tpafaixaetaria10anos'.$gravado]+$turma['tpafaixaetaria11anos'.$gravado]+$turma['tpafaixaetariaacima11anos'.$gravado]) ); ?></td>
	</tr>
	
	
	</table>

	
	<?
	if($gravado) {

		$sql = "SELECT catdsc||'<input type=hidden name=catid[".$turma['tpaid']."][] value=\"'||catid||'\"><input type=hidden id=\"catdsc_'||catid||'\" value=\"'||catdsc||'\">', 
							   '<center><input type=\"text\" style=\"text-align:;\" name=\"actsim[".$turma['tpaid']."]['||catid||']\" size=\"5\" maxlength=\"2\" value=\"'||COALESCE((SELECT actsim::text FROM sispacto2.aprendizagemconhecimentoturma c WHERE c.catid=ac.catid AND tpaid=".$turma['tpaid']."),'')||'\" onkeyup=\"this.value=mascaraglobal(\'##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"actsim_{$turma['tpaid']}_'||catid||'\" title=\"Sim\" class=\"obrigatorio normal\"></center>' as sim, 
							   '<center><input type=\"text\" style=\"text-align:;\" name=\"actparcialmente[".$turma['tpaid']."]['||catid||']\" size=\"5\" maxlength=\"2\" value=\"'||COALESCE((SELECT actparcialmente::text FROM sispacto2.aprendizagemconhecimentoturma c WHERE c.catid=ac.catid AND tpaid=".$turma['tpaid']."),'')||'\" onkeyup=\"this.value=mascaraglobal(\'##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"actparcialmente_{$turma['tpaid']}_'||catid||'\" title=\"Parcialmente\" class=\"obrigatorio normal\"></center>' as parcialmente, 
							   '<center><input type=\"text\" style=\"text-align:;\" name=\"actnao[".$turma['tpaid']."]['||catid||']\" size=\"5\" maxlength=\"2\" value=\"'||COALESCE((SELECT actnao::text FROM sispacto2.aprendizagemconhecimentoturma c WHERE c.catid=ac.catid AND tpaid=".$turma['tpaid']."),'')||'\" onkeyup=\"this.value=mascaraglobal(\'##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"actnao_{$turma['tpaid']}_'||catid||'\" title=\"Não\" class=\"obrigatorio normal\"></center>' as nao FROM sispacto2.aprendizagemconhecimento ac WHERE cattipo='{$cattipo}'";
		
		$cabecalho = array("CONHECIMENTO/CAPACIDADE","SIM","PARCIALMENTE","NÃO");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	
	}
	
	?>
	</td>
</tr>

<? endforeach; ?>

<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
		<input type="button" value="Anterior" onclick="divCarregando();window.location='sispacto2.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=planoatividade';">
		<input type="button" value="Salvar" id="salvar" onclick="gravarAprendizagemTurma();">
	</td>
</tr>
</table>
</form>

<script>

function gravarAprendizagemTurma() {
	
	var validado = true;
	
	<? foreach($turmasprofessores as $turma) : ?>

	if(jQuery('#tpatotalalunos_<?=$turma['tpaid'] ?>').val()!=jQuery('#tpatotalalunosfaixaetaria_<?=$turma['tpaid'] ?>').val()) {
		alert('Os totais "Por gênero" e "Por faixa etária" devem ser iguais');
		return false;
	}
	
	<? if($gravado) : ?>
	
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
		
		if((actsim+actparcialmente+actnao)!= jQuery('#tpatotalalunos_<?=$turma['tpaid'] ?>').val()) {
			alert('O somatório das colunas (Sim+Parcialmente+Não) deve ser igual ao total de alunos da turma');
			validado = false;
			return false;
		}

	});
	
	if(validado == false) {
		return false;
	}

	<? endif; ?>
	
	<? endforeach; ?>
	
	if(validado) {
		jQuery('#formulario').submit();
	}

	
}

</script>



	
