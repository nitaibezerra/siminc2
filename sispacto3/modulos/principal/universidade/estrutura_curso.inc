<?

verificarTermoCompromisso(array("iusd"=>$_SESSION['sispacto3']['universidade']['iusd']));

$estado = wf_pegarEstadoAtual( $_SESSION['sispacto3']['universidade']['docidestruturaformacao'] );

if($estado['esdid'] != ESD_ELABORACAO_COORDENADOR_IES) {
	$consulta = true;
}

$_SESSION['sispacto3']['universidade']['ecuid'] = pegarEstruturaCurso(array("uncid" => $_SESSION['sispacto3']['universidade']['uncid']));

$estruturacurso = carregarEstruturaCurso(array("ecuid"=>$_SESSION['sispacto3']['universidade']['ecuid']));
$articulacaoinstitucional = carregarArticulacaoInstitucional(array("ecuid"=>$_SESSION['sispacto3']['universidade']['ecuid']));
$info = carregarCadastroIESProjeto(array("uncid"=>$_SESSION['sispacto3']['universidade']['uncid']));

if($estruturacurso) {
	extract($estruturacurso);
}

if($articulacaoinstitucional) {
	extract($articulacaoinstitucional);
}

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

<? if($consulta) : ?>
jQuery(function() {
jQuery("[id='cadastrarabrangencia']").remove();
jQuery("[type='text']").attr('class','disabled');
jQuery("[type='text']").attr('disabled','disabled');
jQuery("[type='radio']").attr('disabled','disabled');
jQuery("[name='salvar']").css('display','none');
jQuery("[name='salvarcontinuar']").css('display','none');
jQuery("[name='importar']").css('display','none');
jQuery("[name='estuf_endereco'],[name='ecuobsplanoatividades'],[name='muncod_endereco'],[name='ainseducjustificativa'],[name='ainundimejustificativa'],[name='ainuncmejustificativa']").attr('disabled','disabled');
});
<? endif; ?>

function salvarEstruturaCurso(goto) {

	if(jQuery('#estuf_endereco').val()=='') {
		alert('UF em branco');
		return false;
	}
	
	if(document.getElementById('muncod_endereco')) {
		if(jQuery('#muncod_endereco').val()=='') {
			alert('Município em branco');
			return false;
		}
	} else {
		alert('UF em branco');
		return false;
	}
	
	var validado=true;
	
	jQuery("[name^='aundatainicioprev[']").each(function(key, obj) {
		if(obj.value=='') {
			alert('PERÍODO DE EXECUÇÃO - Data Início em branco');
			validado = false;
			return false;
		}
		
		if(!validaData(obj)) {
			alert('PERÍODO DE EXECUÇÃO - Data Início inválida');
			validado = false;
			return false;
		}
		
		if(parseInt(obj.value.split("/")[2].toString() + obj.value.split("/")[1].toString() + obj.value.split("/")[0].toString())<'20121001') {
			alert('PERÍODO DE EXECUÇÃO - Data Início não pode ser inferior a outubro/ 2012');
			validado = false;
			return false;
		}

		if(parseInt(obj.value.split("/")[2].toString() + obj.value.split("/")[1].toString() + obj.value.split("/")[0].toString()) < '<?=str_replace("-","",$info['universidade']['uncdatainicioprojeto']) ?>') {
			alert('O Término do período de execução não pode ser menor que Início da Vigência do projeto');
			validado = false;
			return false;
		}
		
		
	});
	
	if(!validado) {
		return false;
	}
	
	jQuery("[name^='aundatafimprev[']").each(function(key, obj) {
		if(obj.value=='') {
			alert('PERÍODO DE EXECUÇÃO - Data Término em branco');
			validado = false;
			return false;
		}
	
		if(!validaData(obj)) {
			alert('PERÍODO DE EXECUÇÃO - Data Término inválida');
			validado = false;
			return false;
		}
		
		if('<?=$info['universidade']['uncdatafimprojeto'] ?>'=='') {
			alert('Preencha a Vigência do projeto na aba Dados gerais do projeto');
			validado = false;
			return false;
		}
		
		var nova_data1 = parseInt(jQuery("[name^='aundatainicioprev[']")[key].value.split("/")[2].toString() + jQuery("[name^='aundatainicioprev[']")[key].value.split("/")[1].toString() + jQuery("[name^='aundatainicioprev[']")[key].value.split("/")[0].toString());
		var nova_data2 = parseInt(obj.value.split("/")[2].toString() + obj.value.split("/")[1].toString() + obj.value.split("/")[0].toString());
		
		if(nova_data2 > '<?=str_replace("-","",$info['universidade']['uncdatafimprojeto']) ?>') {
			alert('O Término do período de execução não pode ser maior que Término da Vigência do projeto');
			validado = false;
			return false;
		}
		
		if (nova_data2 < nova_data1) {
			alert('PERÍODO DE EXECUÇÃO - Data Término não pode ser inferior à data de início');
			validado = false;
			return false;
		}
	});
	
	if(!validado) {
		return false;
	}
	
	var ainseduc_r = jQuery("[name^='ainseduc']:enabled:checked").length;
	if(ainseduc_r==0) {
		alert('Marque se foi feita articulação com a SEDUC');
		return false;
	}
	
	if(jQuery('#ainseducjustificativa').val()=='') {
		alert('Preencha a justificativa da SEDUC');
		return false;
	}
	
	var ainundime_r = jQuery("[name^='ainundime']:enabled:checked").length;
	if(ainundime_r==0) {
		alert('Marque se foi feita articulação com a UNDIME');
		return false;
	}
	
	if(jQuery('#ainundimejustificativa').val()=='') {
		alert('Preencha a justificativa da UNDIME');
		return false;
	}

	var ainuncme_r = jQuery("[name^='ainuncme']:enabled:checked").length;
	if(ainuncme_r==0) {
		alert('Marque se foi feita articulação com a UNCME');
		return false;
	}
	
	if(jQuery('#ainuncmejustificativa').val()=='') {
		alert('Preencha a justificativa da UNCME');
		return false;
	}
	
    divCarregando();
    
    jQuery('#goto').val(goto);

	document.getElementById('formulario').submit();

}

function carregarSubAtividades(atiid) {
	ajaxatualizar('requisicao=carregarSubAtividades&atiid='+atiid,'td_subatividade');
}

function definirAbrangencia() {
	ajaxatualizar('requisicao=definirAbrangencia&uncid=<?=$_SESSION['sispacto3']['universidade']['uncid'] ?>&ecuid=<?=$_SESSION['sispacto3']['universidade']['ecuid'] ?>','div_abrangencia');
}

function abrirMunicipioAbrangencia() {
	window.open('sispacto3.php?modulo=principal/universidade/inserirmunicipioabrangencia&acao=A&ecuid=<?=$_SESSION['sispacto3']['universidade']['ecuid'] ?>','Documento','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function abrirDetalhamentoAbrangencia(muncod, esfera, obj) {

	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode.parentNode;
	
	if(obj.title=="mais") {
		obj.title    = "menos";
		obj.src      = "../imagens/menos.gif";
		var nlinha   = tabela.insertRow(linha.rowIndex);
		var ncol     = nlinha.insertCell(0);
		ncol.colSpan = 8;
		ncol.id      = 'coluna_'+nlinha.rowIndex;
		ajaxatualizar('requisicao=carregarDetalhamentoAbrangencia&ecuid=<?=$_SESSION['sispacto3']['universidade']['ecuid'] ?>&muncod='+muncod+'&esfera='+esfera,'coluna_'+nlinha.rowIndex);
	} else {
		obj.title    = "mais";
		obj.src      = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}
	 
}

function excluirAbrangencia(abrid) {
	var conf = confirm("Deseja realmente excluir este Município?");
	if(conf) {
		divCarregando();
		ajaxatualizar('requisicao=excluirAbrangencia&abrid='+abrid,'');
		ajaxatualizar('requisicao=definirAbrangencia&uncid=<?=$_SESSION['sispacto3']['universidade']['uncid'] ?>&ecuid=<?=$_SESSION['sispacto3']['universidade']['ecuid'] ?>','div_abrangencia');
		divCarregado();
	}
}

</script>
<form method="post" id="formulario">
<input type="hidden" name="requisicao" value="atualizarEstruturaCurso">
<input type="hidden" name="curid" value="<?=$_SESSION['sispacto3']['universidade']['curid'] ?>">
<input type="hidden" name="uncid" value="<?=$_SESSION['sispacto3']['universidade']['uncid'] ?>">
<input type="hidden" name="ecuid" value="<?=$_SESSION['sispacto3']['universidade']['ecuid'] ?>">
<input type="hidden" name="goto" id="goto" value="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Estrutura do Curso</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
	</tr>
	<tr>
		<td colspan="2" align="right"><input type="button" name="importar" value="Importar SISPACTO 2014" onclick="importarInformacoesSispacto('estrutura_curso');"></td>
	</tr>
	<?
	$dadoscurso = carregarCurso(array("curid"=>$_SESSION['sispacto3']['universidade']['curid']));
	$cursonome = $dadoscurso['curid']." - ".$dadoscurso['curdesc']; 
	?>
	<tr>
	
	<td colspan="2">
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloDireita" width="20%">Curso</td>
		<td><?=$cursonome ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Sede do Curso</td>
		<td>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
				<tr>
					<td align="right" width="10%">UF</td>
					<td><?
					$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
					$db->monta_combo('estuf_endereco', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '', 'S', 'estuf_endereco', '', $estuf); 
					?></td>
				</tr>
				<tr>
					<td align="right" width="10%">Município</td>
					<td id="td_municipio3">
					<? 
					if($estuf) : 
						$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$estuf."' ORDER BY mundescricao";
						$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_endereco', '', $muncod);
					else :
						echo "Selecione UF";
					endif;
					?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Abrangência</td>
		<td>
		<p><input type="button" name="cadastrarabrangencia" id="cadastrarabrangencia" value="Inserir Município" onclick="abrirMunicipioAbrangencia();"></p>
		<div id="div_abrangencia">
		<? definirAbrangencia(array("consulta"=>$consulta,"ecuid"=>$_SESSION['sispacto3']['universidade']['ecuid'],"uncid"=>$_SESSION['sispacto3']['universidade']['uncid'])); ?>
		</div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Plano de Atividades</td>
		<td>
		<? carregarPlanoAtividades(array("ecuid"=>$_SESSION['sispacto3']['universidade']['ecuid'])); ?>
		<br/>
		<p>Comentários sobre o cronograma (plano de atividades):</p>
		<?=campo_textarea('ecuobsplanoatividades', 'N', 'S', '', '100', '12', '5000'); ?>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">Articulação Institucional</td>
		<td>
			<table>
				<tr>
					<td>Foi feita articulação com a SEDUC?</td>
					<td><input type="radio" name="ainseduc" value="TRUE" <?=(($ainseduc=="t")?"checked":"") ?>> Sim <input type="radio" name="ainseduc" value="FALSE" <?=(($ainseduc=="f")?"checked":"") ?>> Não</td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=campo_textarea('ainseducjustificativa', 'N', 'S', '', '70', '4', '250'); ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com a UNDIME?</td>
					<td><input type="radio" name="ainundime" value="TRUE" <?=(($ainundime=="t")?"checked":"") ?>> Sim <input type="radio" name="ainundime" value="FALSE" <?=(($ainundime=="f")?"checked":"") ?>> Não</td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=campo_textarea('ainundimejustificativa', 'N', 'S', '', '70', '4', '250'); ?></td>
				</tr>
				<tr>
					<td>Foi feita articulação com a UNCME?</td>
					<td><input type="radio" name="ainuncme" value="TRUE" <?=(($ainuncme=="t")?"checked":"") ?>> Sim <input type="radio" name="ainuncme" value="FALSE" <?=(($ainuncme=="f")?"checked":"") ?>> Não</td>
				</tr>
				<tr>
					<td colspan="2">Comente<br><?=campo_textarea('ainuncmejustificativa', 'N', 'S', '', '70', '4', '250'); ?></td>
				</tr>
			</table>
		</td>
	</tr>
	</table>
	
	</td>
	
	<td valign="top" width="5%">
		<?
		/* Barra de estado atual e ações e Historico */
		wf_desenhaBarraNavegacao( $_SESSION['sispacto3']['universidade']['docidestruturaformacao'], array('aba' => $_REQUEST['aba']) );
		?>
	</td>
	
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
		<?=criarBotoesNavegacao(array('url' => $_SERVER['REQUEST_URI'],'funcao' => 'salvarEstruturaCurso')) ?>
		</td>
	</tr>
</table>
</form>