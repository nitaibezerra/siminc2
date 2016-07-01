<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<?
$consulta_pfl = verificaPermissao();
?>
<script>

function somarTotalFaixaEtaria(tpaid) {

	var soma = 0;
	
	if(jQuery('#tpafaixaetaria5anos_'+tpaid).val()!='') {
		soma += parseInt(jQuery('#tpafaixaetaria5anos_'+tpaid).val());
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
		
		if(jQuery('#tpatotalfreqcreche_'+jQuery(this).val()).val() != '') {
			if(parseInt(jQuery('#tpatotalfreqcreche_'+jQuery(this).val()).val()) > parseInt(jQuery('#tpatotalalunos_'+jQuery(this).val()).val())) {
				alert('O total que Frequentaram a creche não pode ser maior que o total da turma. Turma : '+jQuery('#tpanometurma_'+jQuery(this).val()).val());
				validado = false;
				return false;
			}
		}
		if(jQuery('#tpatotalfreqpreescola_'+jQuery(this).val()).val() != '') {
			if(parseInt(jQuery('#tpatotalfreqpreescola_'+jQuery(this).val()).val()) > parseInt(jQuery('#tpatotalalunos_'+jQuery(this).val()).val())) {
				alert('O total que Frequentaram a Pré-escola não pode ser maior que o total da turma. Turma : '+jQuery('#tpanometurma_'+jQuery(this).val()).val());
				validado = false;
				return false;
			}
		}
		
		if(jQuery('#tpatotalbolsafamilia_'+jQuery(this).val()).val() != '') {
			if(parseInt(jQuery('#tpatotalbolsafamilia_'+jQuery(this).val()).val()) > parseInt(jQuery('#tpatotalalunos_'+jQuery(this).val()).val())) {
				alert('O total que são Beneficiários do Bolsa Família não pode ser maior que o total da turma. Turma : '+jQuery('#tpanometurma_'+jQuery(this).val()).val());
				validado = false;
				return false;
			}
		}
		
		jQuery('#tpatotalvivemcomunidade_'+jQuery(this).val()).val(mascaraglobal('######',jQuery('#tpatotalvivemcomunidade_'+jQuery(this).val()).val()));
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
	
	jQuery.ajax({
   		type: "POST",
   		url: 'sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A',
   		data: 'requisicao=verificarAprendizagemTurma&iusd=<?=$_SESSION['sispacto']['professoralfabetizador']['iusd'] ?>',
   		async: false,
   		success: function(retorno){
   			if(retorno=='TRUE') {
   			
   				var conf = confirm('A atualização dos "Dados das Turmas" acarretará o repreenchimento de todos os dados da "Aprendizagem da Turma". Deseja realmente atualizar os Dados das Turmas?');
   				
   				if(conf==false) {
   					validado = false;
   				}
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
<? if($consulta_pfl) : ?>
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
			$sql = "SELECT pk_cod_etapa_ensino as codigo, no_etapa_ensino as descricao FROM educacenso_2012.tab_etapa_ensino WHERE pk_cod_etapa_ensino IN(4,5,6,12,14,15,16,22)";
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

$turmasprofessores_regencia_nulo = pegarTurmasProfessores(array('iusd' => $_SESSION['sispacto']['professoralfabetizador']['iusd'],'tpaconfirmaregencianulo' => true));

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
		<input type="button" value="Anterior" onclick="divCarregando();window.location='sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=acompanhamentoavaliacaobolsas';"> 
		<input type="button" value="Confirmar" onclick="gravarConfirmacaoRegencia();">
	</td>
</tr>
</table>
<?
else :

$turmasprofessores = pegarTurmasProfessores(array('iusd' => $_SESSION['sispacto']['professoralfabetizador']['iusd'],'tpastatus' => 'A','tpaconfirmaregencia' => 'TRUE'));
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Dados Gerais das Turmas do Ciclo Alfabetização</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
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
				<td class="SubTituloDireita" width="25%"><b>Total Por gênero:</b></td>
				<td>
				<table width="300">
					<tr>
						<td width="60%">Meninos</td>
						<td><?=campo_texto('tpatotalmeninos['.$tp['tpaid'].']', "S", "S", "Meninos", 7, 6, "######", "", '', '', 0, 'id="tpatotalmeninos_'.$tp['tpaid'].'"', 'somarTotalGenero('.$tp['tpaid'].');', $tp['tpatotalmeninos'] ); ?></td>
					</tr>
					<tr>
						<td>Meninas</td>
						<td><?=campo_texto('tpatotalmeninas['.$tp['tpaid'].']', "S", "S", "Meninas", 7, 6, "######", "", '', '', 0, 'id="tpatotalmeninas_'.$tp['tpaid'].'"', 'somarTotalGenero('.$tp['tpaid'].');', $tp['tpatotalmeninas'] ); ?></td>
					</tr>
					<tr>
						<td align="right"><b>Total</b></td>
						<td><?=campo_texto('tpatotalalunos['.$tp['tpaid'].']', "N", "N", "Número de alunos na turma", 7, 6, "######", "", '', '', 0, 'id="tpatotalalunos_'.$tp['tpaid'].'"', '', ($tp['tpatotalmeninos']+$tp['tpatotalmeninas']) ); ?></td>
					</tr>

				</table>
				
				</td>
			</tr>
			
			<tr>
				<td class="SubTituloDireita" width="25%"><b>Total Por Faixa Etária:</b></td>
				<td>
				<table width="300">
					<tr>
						<td width="60%">Abaixo de 6 anos</td>
						<td><?=campo_texto('tpafaixaetaria5anos['.$tp['tpaid'].']', "S", "S", "Abaixo de 6 anos", 7, 6, "#########", "", '', '', 0, 'id="tpafaixaetaria5anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetaria5anos'] ); ?></td>
					</tr>
					<tr>
						<td width="60%">6 anos completos</td>
						<td><?=campo_texto('tpafaixaetaria6anos['.$tp['tpaid'].']', "S", "S", "6 anos completos", 7, 6, "#########", "", '', '', 0, 'id="tpafaixaetaria6anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetaria6anos'] ); ?></td>
					</tr>
					<tr>
						<td>7 anos completos</td>
						<td><?=campo_texto('tpafaixaetaria7anos['.$tp['tpaid'].']', "S", "S", "7 anos completos", 7, 6, "#########", "", '', '', 0, 'id="tpafaixaetaria7anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetaria7anos'] ); ?></td>
					</tr>
					<tr>
						<td>8 anos completos</td>
						<td><?=campo_texto('tpafaixaetaria8anos['.$tp['tpaid'].']', "S", "S", "8 anos completos", 7, 6, "#########", "", '', '', 0, 'id="tpafaixaetaria8anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetaria8anos'] ); ?></td>
					</tr>
					<tr>
						<td>9 anos completos</td>
						<td><?=campo_texto('tpafaixaetaria9anos['.$tp['tpaid'].']', "S", "S", "9 anos completos", 7, 6, "#########", "", '', '', 0, 'id="tpafaixaetaria9anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetaria9anos'] ); ?></td>
					</tr>
					<tr>
						<td>10 anos completos</td>
						<td><?=campo_texto('tpafaixaetaria10anos['.$tp['tpaid'].']', "S", "S", "10 anos completos", 7, 6, "#########", "", '', '', 0, 'id="tpafaixaetaria10anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetaria10anos'] ); ?></td>
					</tr>
					<tr>
						<td>11 anos completos</td>
						<td><?=campo_texto('tpafaixaetaria11anos['.$tp['tpaid'].']', "S", "S", "11 anos completos", 7, 6, "#########", "", '', '', 0, 'id="tpafaixaetaria11anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetaria11anos'] ); ?></td>
					</tr>
					<tr>
						<td width="60%">Acima de 11 anos</td>
						<td><?=campo_texto('tpafaixaetariaacima11anos['.$tp['tpaid'].']', "S", "S", "Acima de 11 anos", 7, 6, "#########", "", '', '', 0, 'id="tpafaixaetariaacima11anos_'.$tp['tpaid'].'"', 'somarTotalFaixaEtaria('.$tp['tpaid'].');', $tp['tpafaixaetariaacima11anos'] ); ?></td>
					</tr>

					<tr>
						<td align="right"><b>Total</b></td>
						<td><?=campo_texto('tpatotalalunosfaixaetaria['.$tp['tpaid'].']', "N", "N", "Número de alunos na turma", 7, 6, "#########", "", '', '', 0, 'id="tpatotalalunosfaixaetaria_'.$tp['tpaid'].'"', '', ($tp['tpafaixaetaria5anos']+$tp['tpafaixaetaria6anos']+$tp['tpafaixaetaria7anos']+$tp['tpafaixaetaria8anos']+$tp['tpafaixaetaria9anos']+$tp['tpafaixaetaria10anos']+$tp['tpafaixaetaria11anos']+$tp['tpafaixaetariaacima11anos']) ); ?></td>
					</tr>
				</table>
				
				</td>
			</tr>
			
			<tr>
				<td class="SubTituloDireita" width="25%"><b>Total Frequentaram a creche:</b></td>
				<td><?=campo_texto('tpatotalfreqcreche['.$tp['tpaid'].']', "S", "S", "Frequentaram a creche", 7, 6, "######", "", '', '', 0, 'id="tpatotalfreqcreche_'.$tp['tpaid'].'"', '', $tp['tpatotalfreqcreche'] ); ?></td>
			</tr>
			
			<tr>
				<td class="SubTituloDireita" width="25%"><b>Total Frequentaram a Pré-escola:</b></td>
				<td><?=campo_texto('tpatotalfreqpreescola['.$tp['tpaid'].']', "S", "S", "Frequentaram a Pré-escola", 7, 6, "######", "", '', '', 0, 'id="tpatotalfreqpreescola_'.$tp['tpaid'].'"', '', $tp['tpatotalfreqpreescola'] ); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="25%"><b>Total Beneficiários do Bolsa Família:</b></td>
				<td><?=campo_texto('tpatotalbolsafamilia['.$tp['tpaid'].']', "S", "S", "Beneficiários do Bolsa Família", 7, 6, "######", "", '', '', 0, 'id="tpatotalbolsafamilia_'.$tp['tpaid'].'"', '', $tp['tpatotalbolsafamilia'] ); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="25%"><b>Total Vivem na comunidade onde se localiza a escola:</b></td>
				<td><?=campo_texto('tpatotalvivemcomunidade['.$tp['tpaid'].']', "S", "S", "Vivem na comunidade onde se localiza a escola", 7, 6, "######", "", '', '', 0, 'id="tpatotalvivemcomunidade_'.$tp['tpaid'].'"', '', $tp['tpatotalvivemcomunidade'] ); ?> ( Quadra, Bairro, Rua, Vilarejo )</td>
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
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=acompanhamentoavaliacaobolsas';"> 
			<input type="button" id="salvar" value="Salvar" onclick="gravarInformacoesTurmasProfessor();">
		</td>
	</tr>
</table>

<?
endif;
?>


	
