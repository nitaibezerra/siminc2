<?
$consulta = verificaPermissao();
?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function somarTotalVivemComunidade(tpaid) {

	var soma = 0;
	
	if(jQuery('#tpavivemcomunidadeabaixo6anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpavivemcomunidadeabaixo6anos_'+tpaid).val());
	}
	
	if(jQuery('#tpavivemcomunidade6anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpavivemcomunidade6anos_'+tpaid).val());
	}
	
	if(jQuery('#tpavivemcomunidade7anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpavivemcomunidade7anos_'+tpaid).val());
	}
	
	if(jQuery('#tpavivemcomunidade8anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpavivemcomunidade8anos_'+tpaid).val());
	}
	
	if(jQuery('#tpavivemcomunidade9anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpavivemcomunidade9anos_'+tpaid).val());
	}
	
	if(jQuery('#tpavivemcomunidade10anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpavivemcomunidade10anos_'+tpaid).val());
	}
	
	if(jQuery('#tpavivemcomunidade11anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpavivemcomunidade11anos_'+tpaid).val());
	}
	
	if(jQuery('#tpavivemcomunidadeacima11anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpavivemcomunidadeacima11anos_'+tpaid).val());
	}
	

	jQuery('#tpavivemcomunidadetotal_'+tpaid).val(soma);
}

function somarTotalBolsaFamilia(tpaid) {

	var soma = 0;
	
	if(jQuery('#tpabolsafamiliaabaixo6anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpabolsafamiliaabaixo6anos_'+tpaid).val());
	}
	
	if(jQuery('#tpabolsafamilia6anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpabolsafamilia6anos_'+tpaid).val());
	}
	
	if(jQuery('#tpabolsafamilia7anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpabolsafamilia7anos_'+tpaid).val());
	}
	
	if(jQuery('#tpabolsafamilia8anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpabolsafamilia8anos_'+tpaid).val());
	}
	
	if(jQuery('#tpabolsafamilia9anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpabolsafamilia9anos_'+tpaid).val());
	}
	
	if(jQuery('#tpabolsafamilia10anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpabolsafamilia10anos_'+tpaid).val());
	}
	
	if(jQuery('#tpabolsafamilia11anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpabolsafamilia11anos_'+tpaid).val());
	}
	
	if(jQuery('#tpabolsafamiliaacima11anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpabolsafamiliaacima11anos_'+tpaid).val());
	}
	

	jQuery('#tpabolsafamiliatotal_'+tpaid).val(soma);
}

function somarTotalPreEscola(tpaid) {

	var soma = 0;
	
	if(jQuery('#tpafreqpreescolaabaixo6anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqpreescolaabaixo6anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqpreescola6anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqpreescola6anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqpreescola7anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqpreescola7anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqpreescola8anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqpreescola8anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqpreescola9anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqpreescola9anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqpreescola10anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqpreescola10anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqpreescola11anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqpreescola11anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqpreescolaacima11anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqpreescolaacima11anos_'+tpaid).val());
	}
	

	jQuery('#tpafreqpreescolatotal_'+tpaid).val(soma);
}


function somarTotalCreche(tpaid) {

	var soma = 0;
	
	if(jQuery('#tpafreqcrecheabaixo6anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqcrecheabaixo6anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqcreche6anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqcreche6anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqcreche7anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqcreche7anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqcreche8anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqcreche8anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqcreche9anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqcreche9anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqcreche10anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqcreche10anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqcreche11anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqcreche11anos_'+tpaid).val());
	}
	
	if(jQuery('#tpafreqcrecheacima11anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafreqcrecheacima11anos_'+tpaid).val());
	}
	

	jQuery('#tpafreqcrechetotal_'+tpaid).val(soma);
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

function gravarInformacoesTurmasProfessor() {

	var validado = true;

	jQuery("[name^='tpaid[]']").each(function() {

		if(jQuery('#tpatotalalunos_'+jQuery(this).val()).val()=='' || jQuery('#tpatotalalunos_'+jQuery(this).val()).val()=='0') {
			alert('Total Por gênero não pode ser vazio ou zero. Turma : '+jQuery('#tpanometurma_'+jQuery(this).val()).val());
			validado = false;
			return false;
		}

		if(jQuery('#tpatotalalunosfaixaetaria_'+jQuery(this).val()).val()=='' || jQuery('#tpatotalalunosfaixaetaria_'+jQuery(this).val()).val()=='0') {
			alert('Total Por faixa etária não pode ser vazio ou zero. Turma : '+jQuery('#tpanometurma_'+jQuery(this).val()).val());
			validado = false;
			return false;
		}
		
		if(jQuery('#tpatotalalunos_'+jQuery(this).val()).val() != jQuery('#tpatotalalunosfaixaetaria_'+jQuery(this).val()).val()) {
			alert('O total Por gênero deve ser igual ao total Por Faixa Etária. Turma : '+jQuery('#tpanometurma_'+jQuery(this).val()).val());
			validado = false;
			return false;
		}
		
		if(jQuery('#tpafreqcrechetotal_'+jQuery(this).val()).val() != '') {
			if(parseInt(jQuery('#tpafreqcrechetotal_'+jQuery(this).val()).val()) > parseInt(jQuery('#tpatotalalunos_'+jQuery(this).val()).val())) {
				alert('O total que Frequentaram a creche não pode ser maior que o total da turma. Turma : '+jQuery('#tpanometurma_'+jQuery(this).val()).val());
				validado = false;
				return false;
			}
		}

		if(jQuery('#tpafreqpreescolatotal_'+jQuery(this).val()).val() != '') {
			if(parseInt(jQuery('#tpatotalfreqpreescola_'+jQuery(this).val()).val()) > parseInt(jQuery('#tpatotalalunos_'+jQuery(this).val()).val())) {
				alert('O total que Frequentaram a Pré-escola não pode ser maior que o total da turma. Turma : '+jQuery('#tpanometurma_'+jQuery(this).val()).val());
				validado = false;
				return false;
			}
		}

		if(jQuery('#tpabolsafamiliatotal_'+jQuery(this).val()).val() != '') {
			if(parseInt(jQuery('#tpatotalbolsafamilia_'+jQuery(this).val()).val()) > parseInt(jQuery('#tpatotalalunos_'+jQuery(this).val()).val())) {
				alert('O total que são Beneficiários do Bolsa Família não pode ser maior que o total da turma. Turma : '+jQuery('#tpanometurma_'+jQuery(this).val()).val());
				validado = false;
				return false;
			}
		}

		jQuery('#tpavivemcomunidadetotal_'+jQuery(this).val()).val(mascaraglobal('######',jQuery('#tpavivemcomunidadetotal_'+jQuery(this).val()).val()));
		if(jQuery('#tpatotalvivemcomunidade_'+jQuery(this).val()).val() != '') {
			if(parseInt(jQuery('#tpatotalvivemcomunidade_'+jQuery(this).val()).val()) > parseInt(jQuery('#tpatotalalunos_'+jQuery(this).val()).val())) {
				alert('O total que Vivem na comunidade onde se localiza a escola não pode ser maior que o total da turma. Turma : '+jQuery('#tpanometurma_'+jQuery(this).val()).val());
				validado = false;
				return false;
			}
		}

	});
	
	if(validado==false) {
		return false;
	}
	
	jQuery('#formulario').submit();

}

function desvincularTurma() {
	if(jQuery('#tpajustificativadesvinculacao').val()=='') {
		alert('Digite uma justificativa');
		return false;
	}
	jQuery('#formulario2').submit();
}

function mostrarJustificativa(tpaid) {

	jQuery('#tpaid').val(tpaid);

	jQuery("#modalJustificativa").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 400,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function inserirTurma() {
	if(jQuery('#estuf_endereco').val()=='') {
		alert('Selecione UF');
		return false;
	}
	
	if(jQuery('#muncod_endereco').val()=='') {
		alert('Selecione Município');
		return false;
	}
	
	if(jQuery('#tpacodigoescola').val()=='') {
		alert('Selecione Escola');
		return false;
	}

	if(jQuery('#tpanometurma').val()=='') {
		alert('Preencha o nome da turma');
		return false;
	}

	if(jQuery('#pk_cod_etapa_ensino').val()=='') {
		alert('Selecione a etapa de ensino');
		return false;
	}
	
	if(jQuery('#tpahorarioinicioturma_hr').val()=='') {
		alert('Selecione horário inicío');
		return false;
	}
	if(jQuery('#tpahorarioinicioturma_mi').val()=='') {
		alert('Selecione horário inicío');
		return false;
	}

	if(jQuery('#tpahorariofimturma_hr').val()=='') {
		alert('Selecione horário fim');
		return false;
	}
	if(jQuery('#tpahorariofimturma_mi').val()=='') {
		alert('Selecione horário fim');
		return false;
	}
	if(jQuery('#tpahorarioinicioturma_hr').val() > jQuery('#tpahorariofimturma_hr').val()) {
		alert('Horário fim não pode ser menor que Horário inicío');
		return false;
	}
	if(jQuery('#tpahorarioinicioturma_hr').val() == jQuery('#tpahorariofimturma_hr').val()) {
		if(jQuery('#tpahorarioinicioturma_mi').val() > jQuery('#tpahorariofimturma_mi').val()) {
			alert('Horário fim não pode ser menor que Horário inicío');
			return false;
		}
	}
	
	jQuery('#formulario3').submit();
	
}

function mostrarInsercaoTurmas() {

	jQuery("#modalInserirTurma").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 400,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function carregarMunicipiosPorUFEscola(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncod_endereco&name=muncod_endereco&estuf='+estuf+'&onclick=selecionarEscola','td_municipio3');
	} else {
		document.getElementById('td_municipio3').innerHTML = "Selecione uma UF";
	}
}

function selecionarEscola(muncod) {
	if(muncod) {
		ajaxatualizar('requisicao=carregarEscolasPorMunicipio&muncod='+muncod,'td_escola');
	} else {
		document.getElementById('td_escola').innerHTML = "Selecione Município";
	}
}

function exibirDadosTurma(es) {
	if(es) {
		jQuery('#dados_turma').css('display','');
	} else {
		jQuery('#dados_turma').css('display','none');
	}
}

function gravarConfirmacaoRegencia() {

	jQuery('#formulario').submit();

}

jQuery(document).ready(function() {
<? if($consulta) : ?>
jQuery("#salvarcontinuar").css('display','none');
jQuery("#salvar").css('display','none');
jQuery("[name^='incluirnovaturma']").css('display','none');
jQuery("#salvar").css('display','none');
jQuery("[name^='desvincularturma']").css('display','none');
jQuery("#formulario :input").attr("disabled", "disabled");
<? endif; ?>

});


</script>
<div id="modalJustificativa" style="display:none;">
<form method="post" id="formulario2" name="formulario2">
<input type="hidden" name="tpaid" id="tpaid">
<input type="hidden" name="requisicao" value="desvincularTurmaProfessor">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloEsquerda" colspan="2">Você está desvinculado de uma turma. Por favor, escreva uma justificativa.</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">Justificativa</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%"></td>
	<td><? echo campo_textarea( 'tpajustificativadesvinculacao', 'S', 'S', '', '70', '4', '200'); ?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="desvincularturma" value="Desvincular Turma" onclick="desvincularTurma();"></td>
</tr>
</table>
</form>
</div>
<div id="modalInserirTurma" style="display:none;">
<form method="post" id="formulario3" name="formulario3">
<input type="hidden" name="requisicao" value="inserirTurmaProfessor">
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloEsquerda" colspan="2">[ORIENTAÇÃO PARA INSERIR TURMA]</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">Justificativa</td>
</tr>
<tr>
	<td class="SubTituloDireita">UF</td>
	<td><?
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$db->monta_combo('estuf_endereco', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUFEscola', '', '', '', 'S', 'estuf_endereco', ''); 
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Município</td>
	<td id="td_municipio3">Selecione UF</td>
</tr>
<tr>
	<td class="SubTituloDireita">Escolas</td>
	<td id="td_escola">Selecione Município</td>
</tr>
<tr id="dados_turma" style="display:none;">
	<td colspan="2">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloCentro" colspan="4">Dados da turma</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">Nome da turma:</td>
			<td><?=campo_texto('tpanometurma', "S", "S", "Nome da turma", 30, 150, "", "", '', '', 0, 'id="tpanometurma"' ); ?></td>
			<td class="SubTituloDireita">Etapa:</td>
			<td>
			<?
			$sql = "SELECT pk_cod_etapa_ensino as codigo, no_etapa_ensino as descricao FROM educacenso_2013.tab_etapa_ensino WHERE pk_cod_etapa_ensino IN(4,5,6,12,14,15,16,22)";
			$db->monta_combo('pk_cod_etapa_ensino', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'pk_cod_etapa_ensino', ''); 
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Horário inicío:</td>
			<td>
			<select name="tpahorarioinicioturma_hr" class="CampoEstilo obrigatorio" style="width: auto"  id="tpahorarioinicioturma_hr">
			<option value="">Selecione</option>
			<option value="00">00</option>
			<option value="01">01</option>
			<option value="02">02</option>
			<option value="03">03</option>
			<option value="04">04</option>
			<option value="05">05</option>
			<option value="06">06</option>
			<option value="07">07</option>
			<option value="08">08</option>
			<option value="09">09</option>
			<option value="10">10</option>
			<option value="11">11</option>
			<option value="12">12</option>
			<option value="13">13</option>
			<option value="14">14</option>
			<option value="15">15</option>
			<option value="16">16</option>
			<option value="17">17</option>
			<option value="18">18</option>
			<option value="19">19</option>
			<option value="20">20</option>
			<option value="21">21</option>
			<option value="22">22</option>
			<option value="23">23</option>
			</select> : 
			<select name="tpahorarioinicioturma_mi" class="CampoEstilo obrigatorio" style="width: auto"  id="tpahorarioinicioturma_mi">
			<option value="">Selecione</option>
			<option value="00">00</option>
			<option value="05">05</option>
			<option value="10">10</option>
			<option value="15">15</option>
			<option value="20">20</option>
			<option value="25">25</option>
			<option value="30">30</option>
			<option value="35">35</option>
			<option value="40">40</option>
			<option value="45">45</option>
			<option value="50">50</option>
			<option value="55">55</option>
			</select>
			</td>
			<td class="SubTituloDireita">Horário fim:</td>
			<td>
			<select name="tpahorariofimturma_hr" class="CampoEstilo obrigatorio" style="width: auto"  id="tpahorariofimturma_hr">
			<option value="">Selecione</option>
			<option value="00">00</option>
			<option value="01">01</option>
			<option value="02">02</option>
			<option value="03">03</option>
			<option value="04">04</option>
			<option value="05">05</option>
			<option value="06">06</option>
			<option value="07">07</option>
			<option value="08">08</option>
			<option value="09">09</option>
			<option value="10">10</option>
			<option value="11">11</option>
			<option value="12">12</option>
			<option value="13">13</option>
			<option value="14">14</option>
			<option value="15">15</option>
			<option value="16">16</option>
			<option value="17">17</option>
			<option value="18">18</option>
			<option value="19">19</option>
			<option value="20">20</option>
			<option value="21">21</option>
			<option value="22">22</option>
			<option value="23">23</option>
			</select> : 
			<select name="tpahorariofimturma_mi" class="CampoEstilo obrigatorio" style="width: auto"  id="tpahorariofimturma_mi">
			<option value="">Selecione</option>
			<option value="00">00</option>
			<option value="05">05</option>
			<option value="10">10</option>
			<option value="15">15</option>
			<option value="20">20</option>
			<option value="25">25</option>
			<option value="30">30</option>
			<option value="35">35</option>
			<option value="40">40</option>
			<option value="45">45</option>
			<option value="50">50</option>
			<option value="55">55</option>
			</select>
			</td>
		</tr>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Inserir Turma" onclick="inserirTurma();"></td>
</tr>
</table>
</form>
</div>

<?

$turmasprofessores_regencia_nulo = pegarTurmasProfessores(array('iusd' => $_SESSION['sispacto2']['professoralfabetizador']['iusd'],'tpaconfirmaregencianulo' => true));

if($turmasprofessores_regencia_nulo[0]) :
?>
<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="confirmarRegenciaTurma">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="6">Dados Gerais das Turmas do Ciclo Alfabetização</td>
</tr>
	<tr>
		<td class="SubTituloDireita">Orientações</td>
		<td colspan="5">
		<p>Verifique as turmas apresentadas abaixo e confirme em quais destas turmas você esta regente. Em seguida clique em "Confirmar". Após a confirmação, você deverá preencher as informações solicitadas.</p>
		</td>
	</tr>

<tr>
	<td colspan="6" class="SubTituloDireita">Selecione a(s) turma(s) na(s) qual(is) atua como regente : </td>
</tr>
<?
foreach($turmasprofessores_regencia_nulo as $tp) :
?>
<tr>
	<td class="SubTituloDireita">Escola:</td>
	<td><?=$tp['tpacodigoescola']." - ".$tp['tpanomeescola'] ?> ( <?=$tp['estuf']." / ".$tp['mundescricao'] ?> )</td>
	<td class="SubTituloDireita">Turma:</td>
	<td><?=(($tp['tpacodigoturma'])?$tp['tpanometurma']." ( ".$tp['tpahorarioinicioturma']." às ".$tp['tpahorariofimturma']." )":"Nenhuma turma identificacada no CENSO 2012")." - ".$tp['tpaetapaturma'] ?></td>
	<td nowrap><input type="radio" name="tpaconfirmaregencia[<?=$tp['tpaid'] ?>]" value="TRUE" checked> <b>Sim</b> <input type="radio" name="tpaconfirmaregencia[<?=$tp['tpaid'] ?>]" value="FALSE"> <b>Não</b></td>
</tr>
<?
endforeach;
?>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td>
		<input type="button" value="Anterior" onclick="divCarregando();window.location='sispacto2.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=acompanhamentoavaliacaobolsas';"> 
		<input type="button" value="Confirmar" onclick="gravarConfirmacaoRegencia();">
	</td>
</tr>
</table>
<?
else :

$turmasprofessores = pegarTurmasProfessores(array('iusd' => $_SESSION['sispacto2']['professoralfabetizador']['iusd'],'tpastatus' => 'A','tpaconfirmaregencia' => 'TRUE'));
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Dados Gerais das Turmas do Ciclo Alfabetização</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao("/sispacto2/sispacto2.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&rfuparcela=".$rfuparcela); ?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><input type="button" name="incluirnovaturma" value="Incluir Nova Turma" onclick="mostrarInsercaoTurmas();"></td>
	</tr>
	<tr>
		<td colspan="2">
			<form method="post" id="formulario" enctype="multipart/form-data">
			<input type="hidden" name="requisicao" value="gerenciarInformacoesTurmasProfessor">
		
			<? if($turmasprofessores[0]) : ?> 
			<? foreach($turmasprofessores as $tp) : ?>
			<input type="hidden" name="tpaid[]" value="<?=$tp['tpaid'] ?>">
			<input type="hidden" id="tpanometurma_<?=$tp['tpaid'] ?>" value="<?=$tp['tpanometurma'] ?>">
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">

			<tr>
				<td class="SubTituloDireita" width="25%"><b>UF / Município:</b></td>
				<td><?=$tp['estuf']." / ".$tp['mundescricao'] ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="25%"><b>Escola:</b></td>
				<td><?=$tp['tpacodigoescola']." - ".$tp['tpanomeescola']." ( ".(($tp['tpaemailescola'])?$tp['tpaemailescola']:"e-mail não cadastrado no censo")." )" ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="25%"><b>Turma:</b></td>
				<td><?=(($tp['tpanometurma'])?$tp['tpanometurma']." ( ".$tp['tpahorarioinicioturma']." às ".$tp['tpahorariofimturma']." )":"Nenhuma turma identificacada no CENSO 2012")." - ".$tp['tpaetapaturma'] ?></td>
			</tr>
			<tr>
				<td colspan="2">
				
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td colspan="2" align="right" style="color:red;">Caso essa escola/turma não esteja mais vinculada a você, por favor clique no botão de "Desvincular Turma". <input type="button" name="desvincularturma" value="Desvincular Turma" onclick="mostrarJustificativa('<?=$tp['tpaid'] ?>');"></td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda" colspan="2">Número de alunos na turma</td>
			</tr>
			
			<tr>
			
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				
				<tr>
					<td class="SubTituloCentro">&nbsp;</td>
					<td class="SubTituloCentro" style="font-size: x-small">Meninos</td>
					<td class="SubTituloCentro" style="font-size: x-small">Meninas</td>
					<td class="SubTituloCentro" style="font-size: x-small">Total</td>
				</tr>
				
				<tr>
					<td class="SubTituloDireita"><b style="font-size: x-small">Total Por gênero:</b></td>
					<td align="center"><?=campo_texto('tpatotalmeninos['.$tp['tpaid'].']', "N", "S", "Meninos", 5, 5, "######", "", '', '', 0, 'id="tpatotalmeninos_'.$tp['tpaid'].'"', 'somarTotalGenero('.$tp['tpaid'].');', $tp['tpatotalmeninos'] ); ?></td>
					<td align="center"><?=campo_texto('tpatotalmeninas['.$tp['tpaid'].']', "N", "S", "Meninas", 5, 5, "######", "", '', '', 0, 'id="tpatotalmeninas_'.$tp['tpaid'].'"', 'somarTotalGenero('.$tp['tpaid'].');', $tp['tpatotalmeninas'] ); ?></td>
					<td align="center"><?=campo_texto('tpatotalalunos['.$tp['tpaid'].']', "N", "N", "Número de alunos na turma", 5, 5, "######", "", '', '', 0, 'id="tpatotalalunos_'.$tp['tpaid'].'"', '', ($tp['tpatotalmeninos']+$tp['tpatotalmeninas']) ); ?></td>
				</tr>
				
				
				</table>
			
			</tr>

			
			<tr>
				<td colspan="2">
				
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				
				<tr>
					<td class="SubTituloCentro">&nbsp;</td>
					<td class="SubTituloCentro" style="font-size: x-small">Abaixo de 6 anos</td>
					<td class="SubTituloCentro" style="font-size: x-small">6 anos completos</td>
					<td class="SubTituloCentro" style="font-size: x-small">7 anos completos</td>
					<td class="SubTituloCentro" style="font-size: x-small">8 anos completos</td>
					<td class="SubTituloCentro" style="font-size: x-small">9 anos completos</td>
					<td class="SubTituloCentro" style="font-size: x-small">10 anos completos</td>
					<td class="SubTituloCentro" style="font-size: x-small">11 anos completos</td>
					<td class="SubTituloCentro" style="font-size: x-small">Acima de 11 anos</td>
					<td class="SubTituloCentro" style="font-size: x-small">Total</td>
				</tr>
				
				<tr>
					<td class="SubTituloDireita"><b style="font-size: x-small">Total Por Faixa Etária:</b></td>
					<td><?=campo_texto('tpafaixaetariaabaixo6anos['.$tp['tpaid'].']', "N", "S", "Abaixo de 6 anos", 5, 5, "#########", "", '', '', 0, 'id="tpafaixaetariaabaixo6anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetariaabaixo6anos'] ); ?></td>
					<td><?=campo_texto('tpafaixaetaria6anos['.$tp['tpaid'].']', "N", "S", "6 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafaixaetaria6anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetaria6anos'] ); ?></td>
					<td><?=campo_texto('tpafaixaetaria7anos['.$tp['tpaid'].']', "N", "S", "7 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafaixaetaria7anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetaria7anos'] ); ?></td>
					<td><?=campo_texto('tpafaixaetaria8anos['.$tp['tpaid'].']', "N", "S", "8 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafaixaetaria8anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetaria8anos'] ); ?></td>
					<td><?=campo_texto('tpafaixaetaria9anos['.$tp['tpaid'].']', "N", "S", "9 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafaixaetaria9anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetaria9anos'] ); ?></td>
					<td><?=campo_texto('tpafaixaetaria10anos['.$tp['tpaid'].']', "N", "S", "10 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafaixaetaria10anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetaria10anos'] ); ?></td>
					<td><?=campo_texto('tpafaixaetaria11anos['.$tp['tpaid'].']', "N", "S", "11 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafaixaetaria11anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetaria11anos'] ); ?></td>
					<td><?=campo_texto('tpafaixaetariaacima11anos['.$tp['tpaid'].']', "N", "S", "Acima de 11 anos", 5, 5, "#########", "", '', '', 0, 'id="tpafaixaetariaacima11anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetariaacima11anos'] ); ?></td>
					<td><?=campo_texto('tpatotalalunosfaixaetaria['.$tp['tpaid'].']', "N", "N", "Número de alunos na turma", 5, 5, "#########", "", '', '', 0, 'id="tpatotalalunosfaixaetaria_'.$tp['tpaid'].'"', '', ($tp['tpafaixaetariaabaixo6anos']+$tp['tpafaixaetaria6anos']+$tp['tpafaixaetaria7anos']+$tp['tpafaixaetaria8anos']+$tp['tpafaixaetaria9anos']+$tp['tpafaixaetaria10anos']+$tp['tpafaixaetaria11anos']+$tp['tpafaixaetariaacima11anos']) ); ?></td>
				</tr>
				
				
				<tr>
					<td class="SubTituloDireita"><b style="font-size: x-small">Total Frequentaram a creche:</b></td>
					<td><?=campo_texto('tpafreqcrecheabaixo6anos['.$tp['tpaid'].']', "N", "S", "Abaixo de 6 anos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqcrecheabaixo6anos_'.$tp['tpaid'].'"', 'somarTotalCreche('.$tp['tpaid'].');', $tp['tpafreqcrecheabaixo6anos'] ); ?></td>
					<td><?=campo_texto('tpafreqcreche6anos['.$tp['tpaid'].']', "N", "S", "6 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqcreche6anos_'.$tp['tpaid'].'"', 'somarTotalCreche('.$tp['tpaid'].');', $tp['tpafreqcreche6anos'] ); ?></td>
					<td><?=campo_texto('tpafreqcreche7anos['.$tp['tpaid'].']', "N", "S", "7 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqcreche7anos_'.$tp['tpaid'].'"', 'somarTotalCreche('.$tp['tpaid'].');', $tp['tpafreqcreche7anos'] ); ?></td>
					<td><?=campo_texto('tpafreqcreche8anos['.$tp['tpaid'].']', "N", "S", "8 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqcreche8anos_'.$tp['tpaid'].'"', 'somarTotalCreche('.$tp['tpaid'].');', $tp['tpafreqcreche8anos'] ); ?></td>
					<td><?=campo_texto('tpafreqcreche9anos['.$tp['tpaid'].']', "N", "S", "9 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqcreche9anos_'.$tp['tpaid'].'"', 'somarTotalCreche('.$tp['tpaid'].');', $tp['tpafreqcreche9anos'] ); ?></td>
					<td><?=campo_texto('tpafreqcreche10anos['.$tp['tpaid'].']', "N", "S", "10 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqcreche10anos_'.$tp['tpaid'].'"', 'somarTotalCreche('.$tp['tpaid'].');', $tp['tpafreqcreche10anos'] ); ?></td>
					<td><?=campo_texto('tpafreqcreche11anos['.$tp['tpaid'].']', "N", "S", "11 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqcreche11anos_'.$tp['tpaid'].'"', 'somarTotalCreche('.$tp['tpaid'].');', $tp['tpafreqcreche11anos'] ); ?></td>
					<td><?=campo_texto('tpafreqcrecheacima11anos['.$tp['tpaid'].']', "N", "S", "Acima de 11 anos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqcrecheacima11anos_'.$tp['tpaid'].'"', 'somarTotalCreche('.$tp['tpaid'].');', $tp['tpafreqcrecheacima11anos'] ); ?></td>
					<td><?=campo_texto('tpafreqcrechetotal['.$tp['tpaid'].']', "N", "N", "Número de alunos na turma", 5, 5, "#########", "", '', '', 0, 'id="tpafreqcrechetotal_'.$tp['tpaid'].'"', '', ($tp['tpafreqcrecheabaixo6anos']+$tp['tpafreqcreche6anos']+$tp['tpafreqcreche7anos']+$tp['tpafreqcreche8anos']+$tp['tpafreqcreche9anos']+$tp['tpafreqcreche10anos']+$tp['tpafreqcreche11anos']+$tp['tpafreqcrecheacima11anos']) ); ?></td>
				</tr>
				
				<tr>
					<td class="SubTituloDireita"><b style="font-size: x-small">Total Frequentaram a Pré-escola:</b></td>
					<td><?=campo_texto('tpafreqpreescolaabaixo6anos['.$tp['tpaid'].']', "N", "S", "Abaixo de 6 anos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqpreescolaabaixo6anos_'.$tp['tpaid'].'"', 'somarTotalPreEscola('.$tp['tpaid'].');', $tp['tpafreqpreescolaabaixo6anos'] ); ?></td>
					<td><?=campo_texto('tpafreqpreescola6anos['.$tp['tpaid'].']', "N", "S", "6 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqpreescola6anos_'.$tp['tpaid'].'"', 'somarTotalPreEscola('.$tp['tpaid'].');', $tp['tpafreqpreescola6anos'] ); ?></td>
					<td><?=campo_texto('tpafreqpreescola7anos['.$tp['tpaid'].']', "N", "S", "7 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqpreescola7anos_'.$tp['tpaid'].'"', 'somarTotalPreEscola('.$tp['tpaid'].');', $tp['tpafreqpreescola7anos'] ); ?></td>
					<td><?=campo_texto('tpafreqpreescola8anos['.$tp['tpaid'].']', "N", "S", "8 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqpreescola8anos_'.$tp['tpaid'].'"', 'somarTotalPreEscola('.$tp['tpaid'].');', $tp['tpafreqpreescola8anos'] ); ?></td>
					<td><?=campo_texto('tpafreqpreescola9anos['.$tp['tpaid'].']', "N", "S", "9 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqpreescola9anos_'.$tp['tpaid'].'"', 'somarTotalPreEscola('.$tp['tpaid'].');', $tp['tpafreqpreescola9anos'] ); ?></td>
					<td><?=campo_texto('tpafreqpreescola10anos['.$tp['tpaid'].']', "N", "S", "10 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqpreescola10anos_'.$tp['tpaid'].'"', 'somarTotalPreEscola('.$tp['tpaid'].');', $tp['tpafreqpreescola10anos'] ); ?></td>
					<td><?=campo_texto('tpafreqpreescola11anos['.$tp['tpaid'].']', "N", "S", "11 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqpreescola11anos_'.$tp['tpaid'].'"', 'somarTotalPreEscola('.$tp['tpaid'].');', $tp['tpafreqpreescola11anos'] ); ?></td>
					<td><?=campo_texto('tpafreqpreescolaacima11anos['.$tp['tpaid'].']', "N", "S", "Acima de 11 anos", 5, 5, "#########", "", '', '', 0, 'id="tpafreqpreescolaacima11anos_'.$tp['tpaid'].'"', 'somarTotalPreEscola('.$tp['tpaid'].');', $tp['tpafreqpreescolaacima11anos'] ); ?></td>
					<td><?=campo_texto('tpafreqpreescolatotal['.$tp['tpaid'].']', "N", "N", "Número de alunos na turma", 5, 5, "#########", "", '', '', 0, 'id="tpafreqpreescolatotal_'.$tp['tpaid'].'"', '', ($tp['tpafreqpreescolaabaixo6anos']+$tp['tpafreqpreescola6anos']+$tp['tpafreqpreescola7anos']+$tp['tpafreqpreescola8anos']+$tp['tpafreqpreescola9anos']+$tp['tpafreqpreescola10anos']+$tp['tpafreqpreescola11anos']+$tp['tpafreqpreescolaacima11anos']) ); ?></td>
				</tr>
				
				<tr>
					<td class="SubTituloDireita"><b style="font-size: x-small">Total Beneficiários do Bolsa Família:</b></td>
					<td><?=campo_texto('tpabolsafamiliaabaixo6anos['.$tp['tpaid'].']', "N", "S", "Abaixo de 6 anos", 5, 5, "#########", "", '', '', 0, 'id="tpabolsafamiliaabaixo6anos_'.$tp['tpaid'].'"', 'somarTotalBolsaFamilia('.$tp['tpaid'].');', $tp['tpabolsafamiliaabaixo6anos'] ); ?></td>
					<td><?=campo_texto('tpabolsafamilia6anos['.$tp['tpaid'].']', "N", "S", "6 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpabolsafamilia6anos_'.$tp['tpaid'].'"', 'somarTotalBolsaFamilia('.$tp['tpaid'].');', $tp['tpabolsafamilia6anos'] ); ?></td>
					<td><?=campo_texto('tpabolsafamilia7anos['.$tp['tpaid'].']', "N", "S", "7 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpabolsafamilia7anos_'.$tp['tpaid'].'"', 'somarTotalBolsaFamilia('.$tp['tpaid'].');', $tp['tpabolsafamilia7anos'] ); ?></td>
					<td><?=campo_texto('tpabolsafamilia8anos['.$tp['tpaid'].']', "N", "S", "8 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpabolsafamilia8anos_'.$tp['tpaid'].'"', 'somarTotalBolsaFamilia('.$tp['tpaid'].');', $tp['tpabolsafamilia8anos'] ); ?></td>
					<td><?=campo_texto('tpabolsafamilia9anos['.$tp['tpaid'].']', "N", "S", "9 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpabolsafamilia9anos_'.$tp['tpaid'].'"', 'somarTotalBolsaFamilia('.$tp['tpaid'].');', $tp['tpabolsafamilia9anos'] ); ?></td>
					<td><?=campo_texto('tpabolsafamilia10anos['.$tp['tpaid'].']', "N", "S", "10 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpabolsafamilia10anos_'.$tp['tpaid'].'"', 'somarTotalBolsaFamilia('.$tp['tpaid'].');', $tp['tpabolsafamilia10anos'] ); ?></td>
					<td><?=campo_texto('tpabolsafamilia11anos['.$tp['tpaid'].']', "N", "S", "11 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpabolsafamilia11anos_'.$tp['tpaid'].'"', 'somarTotalBolsaFamilia('.$tp['tpaid'].');', $tp['tpabolsafamilia11anos'] ); ?></td>
					<td><?=campo_texto('tpabolsafamiliaacima11anos['.$tp['tpaid'].']', "N", "S", "Acima de 11 anos", 5, 5, "#########", "", '', '', 0, 'id="tpabolsafamiliaacima11anos_'.$tp['tpaid'].'"', 'somarTotalBolsaFamilia('.$tp['tpaid'].');', $tp['tpabolsafamiliaacima11anos'] ); ?></td>
					<td><?=campo_texto('tpabolsafamiliatotal['.$tp['tpaid'].']', "N", "N", "Número de alunos na turma", 5, 5, "#########", "", '', '', 0, 'id="tpabolsafamiliatotal_'.$tp['tpaid'].'"', '', ($tp['tpabolsafamiliaabaixo6anos']+$tp['tpabolsafamilia6anos']+$tp['tpabolsafamilia7anos']+$tp['tpabolsafamilia8anos']+$tp['tpabolsafamilia9anos']+$tp['tpabolsafamilia10anos']+$tp['tpabolsafamilia11anos']+$tp['tpabolsafamiliaacima11anos']) ); ?></td>
				</tr>

				<tr>
					<td class="SubTituloDireita"><b style="font-size: x-small">Total vivem na comunidade onde se localiza a escola:</b></td>
					<td><?=campo_texto('tpavivemcomunidadeabaixo6anos['.$tp['tpaid'].']', "N", "S", "Abaixo de 6 anos", 5, 5, "#########", "", '', '', 0, 'id="tpavivemcomunidadeabaixo6anos_'.$tp['tpaid'].'"', 'somarTotalVivemComunidade('.$tp['tpaid'].');', $tp['tpavivemcomunidadeabaixo6anos'] ); ?></td>
					<td><?=campo_texto('tpavivemcomunidade6anos['.$tp['tpaid'].']', "N", "S", "6 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpavivemcomunidade6anos_'.$tp['tpaid'].'"', 'somarTotalVivemComunidade('.$tp['tpaid'].');', $tp['tpavivemcomunidade6anos'] ); ?></td>
					<td><?=campo_texto('tpavivemcomunidade7anos['.$tp['tpaid'].']', "N", "S", "7 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpavivemcomunidade7anos_'.$tp['tpaid'].'"', 'somarTotalVivemComunidade('.$tp['tpaid'].');', $tp['tpavivemcomunidade7anos'] ); ?></td>
					<td><?=campo_texto('tpavivemcomunidade8anos['.$tp['tpaid'].']', "N", "S", "8 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpavivemcomunidade8anos_'.$tp['tpaid'].'"', 'somarTotalVivemComunidade('.$tp['tpaid'].');', $tp['tpavivemcomunidade8anos'] ); ?></td>
					<td><?=campo_texto('tpavivemcomunidade9anos['.$tp['tpaid'].']', "N", "S", "9 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpavivemcomunidade9anos_'.$tp['tpaid'].'"', 'somarTotalVivemComunidade('.$tp['tpaid'].');', $tp['tpavivemcomunidade9anos'] ); ?></td>
					<td><?=campo_texto('tpavivemcomunidade10anos['.$tp['tpaid'].']', "N", "S", "10 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpavivemcomunidade10anos_'.$tp['tpaid'].'"', 'somarTotalVivemComunidade('.$tp['tpaid'].');', $tp['tpavivemcomunidade10anos'] ); ?></td>
					<td><?=campo_texto('tpavivemcomunidade11anos['.$tp['tpaid'].']', "N", "S", "11 anos completos", 5, 5, "#########", "", '', '', 0, 'id="tpavivemcomunidade11anos_'.$tp['tpaid'].'"', 'somarTotalVivemComunidade('.$tp['tpaid'].');', $tp['tpavivemcomunidade11anos'] ); ?></td>
					<td><?=campo_texto('tpavivemcomunidadeacima11anos['.$tp['tpaid'].']', "N", "S", "Acima de 11 anos", 5, 5, "#########", "", '', '', 0, 'id="tpavivemcomunidadeacima11anos_'.$tp['tpaid'].'"', 'somarTotalVivemComunidade('.$tp['tpaid'].');', $tp['tpavivemcomunidadeacima11anos'] ); ?></td>
					<td><?=campo_texto('tpavivemcomunidadetotal['.$tp['tpaid'].']', "N", "N", "Número de alunos na turma", 5, 5, "#########", "", '', '', 0, 'id="tpavivemcomunidadetotal_'.$tp['tpaid'].'"', '', ($tp['tpavivemcomunidadeabaixo6anos']+$tp['tpavivemcomunidade6anos']+$tp['tpavivemcomunidade7anos']+$tp['tpavivemcomunidade8anos']+$tp['tpavivemcomunidade9anos']+$tp['tpavivemcomunidade10anos']+$tp['tpavivemcomunidade11anos']+$tp['tpavivemcomunidadeacima11anos']) ); ?></td>
				</tr>
				
				
				</table>
				
				
				</td>
			</tr>
		

			</table>
			

			<? endforeach; ?>
			
			<? else : ?>
			
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
				<tr>
					<td class="SubTituloCentro">Não foi encontrada nenhuma turma vinculada. Por favor faça vinculação clicando no botão "Incluir Turma".</td>
				</tr>
			
			</table>
			
			<? endif; ?>
			</form>
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2"><input type="button" id="incluirnovaturma" name="incluirnovaturma" value="Incluir Nova Turma" onclick="mostrarInsercaoTurmas();"></td>
	</tr>

	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sispacto2.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=acompanhamentoavaliacaobolsas';"> 
			<input type="button" id="salvar" value="Salvar" onclick="gravarInformacoesTurmasProfessor();">
		</td>
	</tr>
</table>

<?
endif;
?>


	
