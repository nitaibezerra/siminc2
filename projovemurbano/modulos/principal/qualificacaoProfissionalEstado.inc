<?
criaSessaoQualificacaoProfissional();
$qualificacaoprofissional = $db->pegaLinha("SELECT * FROM projovemurbano.qualificacaoprofissional WHERE qprid='".$_SESSION['projovemurbano']['qprid']."'");

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

$montante = calcularMontante($meta);

$habilita = 'S';
$perfil = pegaPerfilGeral();
if(in_array(PFL_CONSULTA, $perfil)){
	$habilita = 'N';
}
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/projovemurbano.js"></script>
<script>
jQuery(document).ready(function() {
	carregarListaCursosEstado();
});

function carregarListaCursosEstado() {
	jQuery.ajax({
   		type: "POST",
   		url: "projovemurbano.php?modulo=principal/planoImplementacao&acao=A",
   		data: "requisicao=carregarListaCursosEstado",
   		async: false,
   		success: function(msg){document.getElementById('listacursos_estado').innerHTML = msg;}
 		});
}

function gravarQualificacaoProfissional() {

	if(parseFloat(document.getElementById('qprpercutilizado').value) > parseFloat(document.getElementById('qprpercmax').value)) {
		alert('Percentual Utilizado(%) maior do que Percentual Máximo previsto(%)');
		return false;
	}

	jQuery('#form').submit();
}

function calcularValorTotal(desid) {
	var qtdmeses = parseFloat(document.getElementById('pgaqtdmeses_'+desid).value);
	var pgavlrmes=0;
	if(document.getElementById('pgavlrmes_'+desid).value!='') {
		pgavlrmes 	= parseFloat(replaceAll(replaceAll(document.getElementById('pgavlrmes_'+desid).value,".",""),",","."));
	}
	
	var total = qtdmeses*pgavlrmes;
	
	document.getElementById('pgavlrtotal_'+desid).value = mascaraglobal('###.###.###,##',total.toFixed(2));
}

function calculaPorcentTotal() {
	var total=0;
	<? 
	$sql = "SELECT desid FROM projovemurbano.despesas WHERE desstatus='A'";
	$desids = $db->carregarColuna($sql);
	?>
	<? if($desids) : ?>
		<? foreach($desids as $desid) : ?>
		var pgavlrtotal_<?=$desid ?>=0;
		if(document.getElementById('pgavlrtotal_<?=$desid ?>').value!='') {
			pgavlrtotal_<?=$desid ?> 	= parseFloat(replaceAll(replaceAll(document.getElementById('pgavlrtotal_<?=$desid ?>').value,".",""),",","."));
		}
		total += pgavlrtotal_<?=$desid ?>;
		<? endforeach; ?>
	<? endif; ?>
	
	var montante = parseFloat('<?=calcularMontante($meta) ?>');
	
	var final = (total*100)/montante;
	
	document.getElementById('span_totalutilizado').innerHTML = mascaraglobal('###.###.###,##',total.toFixed(2));
	document.getElementById('qprpercutilizado').value = final.toFixed(1);

}

function controleDivs(div,obj) {

	if(obj.checked) {
		document.getElementById(div).style.display='';
	} else {
		document.getElementById(div).style.display='none';
	}
}

function validarTiposArcos(arcid) {
	var selects  = jQuery("[name^='arcidmunicipio[']");
	var contagem = new Array();
	var ultimonome = new Array();
	var novo=0;
	
	jQuery.each(selects, function(i, v) { 
	  if(v.value!='') {
		  if(!contagem[v.value]) {
		  	contagem[v.value]=0;
		  	novo++;
		  }
		  contagem[v.value] = contagem[v.value]+1;
		  ultimonome[v.value] = v.name;
	  }
	});
	
	if(novo>5) {
		alert('Somente 5 tipos de arcos');
		jQuery("[name^='"+ultimonome[arcid]+"']").val('');
	}
}

function selecionarInstituicaoMunicipio(muncod,instituicao,nucid) {

	if(nucid=='') {
		alert('É necessário cadastrar núcleo');
		jQuery('#instituicao_'+nucid).val('');
		return false;
	}
	
	jQuery.ajax({
   		type: "POST",
   		url: "projovemurbano.php?modulo=principal/planoImplementacao&acao=A",
   		data: "requisicao=buscarCursosPorInstituicoesMunicipio&nucid="+nucid+"&ccuibge="+muncod+"&ccuinstituicao="+instituicao,
   		async: false,
   		success: function(dados){
   				jQuery('#nucleo_'+nucid).html(dados);
   			}
 		});
}

function calcularQuantidadeCursosMunicpio(ccuid, muncod, obj) {
	var campos  = jQuery("[id^='cupqtdestudantes_"+ccuid+"_']");
	var total=0;
	jQuery.each(campos, function(i, v) {
	if(v.value!='') {
		total = total + parseFloat(replaceAll(replaceAll(v.value,".",""),",","."));
	} 
	});
	
	if(total > jQuery('#totalvagas_'+ccuid+'_'+muncod).val()) {
		alert('Número de vagas não pode ultrapassar o número ofertado');
		if(obj) {
			obj.value='';
		}
	} else {
		jQuery('#totalutilizado_'+ccuid+'_'+muncod).val(mascaraglobal('###.###.###,##',total.toFixed(0)));
	}
	
}

<? if($esdid!=ESD_EMELABORACAO && !$db->testa_superuser()) : ?>
<? $desabilitado = true; ?>
jQuery(document).ready(function() {
	jQuery("[name^='qprarco']").attr("disabled","disabled");
	jQuery("[name^='qpredutecnica']").attr("disabled","disabled");

	jQuery("[name^='arcidmunicipio[']").attr("disabled","disabled");
	jQuery("[name^='arcidmunicipio[']").attr("className","disabled");
	
	jQuery("[name^='pgavlrmes[']").attr("disabled","disabled");
	jQuery("[name^='pgavlrmes[']").attr("className","disabled");
});
<? endif; ?>


</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="gravarQualificacaoProfissional">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="35%">Orientações:</td>
		<td>
		<font color=blue>
		<p>A Qualificação Profissional será ofertada por meio da Formação Técnica Geral - FTG e da Formação Técnica Específica FTE e do Projeto de Orientação Profissional - POP. A Formação Técnica Específica, por sua vez, poderá ser desenvolvida por meio de programas nacionais de educação técnica em formação inicial e continuada ou por meio de arcos ocupacionais ou por meio das duas opções (Arcos Ocupacionais e PRONATEC).</p>
		<p>Para a oferta da FTG, por meio dos arcos ocupacionais, apresenta-se, a seguir, as despesas possíveis para esta execução: locação de espaços e equipamentos, aquisição de material de consumo, bem como pagamento de monitores para as atividades práticas da qualificação profissional quando for necessário, conforme definido na Resolução CD/FNDE nº 60/2011.</p>
		<p>ATENÇÃO: Para cada núcleo apenas um arco.</p>
		<p>Portanto:<br/>
		Para os Estados 1 arco por Município e até o máximo de 5 arcos para cada Estado.</p>
		<p>Para os Municípios 1 arco por núcleo e até o máximo de 3 arcos para cada Município.</p>
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
			<td align="center"><? echo campo_texto('qprpercmax', 'N', 'N', 'Percentual Máximo previsto(%)', 6, 5, "###.#", "", '', '', 0, 'id="qprpercmax"', '', (($qualificacaoprofissional['qprpercmax'])?$qualificacaoprofissional['qprpercmax']:'7.0'), '' ); ?>  R$ <span id=""><?=number_format((($qualificacaoprofissional['qprpercmax'])?$qualificacaoprofissional['qprpercmax']:'7.0')*$montante/100,2,",",".") ?></span></td>
			<td align="center"><? echo campo_texto('qprpercutilizado', 'N', 'N', 'Percentual utilizado(%)', 6, 5, "###.#", "", '', '', 0, 'id="qprpercutilizado"', '', (($qualificacaoprofissional['qprpercutilizado'])?$qualificacaoprofissional['qprpercutilizado']:'0.0'), '' ); ?> R$ <span id="span_totalutilizado"><?=$db->pegaUm("SELECT COALESCE(trim(to_char(SUM(pgaqtdmeses*pgavlrmes),'999g999g999d99')),'0,00') FROM projovemurbano.previsaogasto WHERE qprid='".$_SESSION['projovemurbano']['qprid']."'") ?></span></td>
		</tr>
		</table>
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de oferta</td>
		<td>
		<p><input type="hidden" name="qprarco" value="FALSE"><input type="checkbox" name="qprarco" value="TRUE" <?=(($qualificacaoprofissional['qprarco']=="t")?"checked":"") ?> onclick="controleDivs('div_arcos',this);" > Arcos</p>
		<div id="div_arcos" <?=(($qualificacaoprofissional['qprarco']=="t")?"":"style=\"display:none;\"") ?> >
		<?
		$pmupossuipolo = $db->pegaUm("SELECT pmupossuipolo FROM projovemurbano.polomunicipio pmu WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
		
		if($pmupossuipolo=="t") {
			
			$sql = "SELECT * FROM projovemurbano.polomunicipio pmu 
					INNER JOIN projovemurbano.polo pol ON pol.pmuid=pmu.pmuid 
					INNER JOIN projovemurbano.associamucipiopolo amp ON amp.polid=pol.polid 
					INNER JOIN projovemurbano.municipio mun ON mun.munid=amp.munid 
					INNER JOIN territorios.municipio m ON m.muncod=mun.muncod 
					WHERE pmu.pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND pol.polstatus='A' AND mun.munstatus='A'";
		
		} else {

			$sql = "SELECT * FROM projovemurbano.polomunicipio pmu 
					INNER JOIN projovemurbano.municipio mun ON mun.pmuid=pmu.pmuid 
					INNER JOIN territorios.municipio m ON m.muncod=mun.muncod 
					WHERE pmu.pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND mun.munstatus='A'";
			
		}
		
		$municipios = $db->carregar($sql);
		
		?>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Município</td>
			<td class="SubTituloCentro">Arcos</td>
		</tr>
		<? 
		if($municipios[0]) :
		foreach($municipios as $value) : 
		$arcid = $db->pegaUm("SELECT arcid FROM projovemurbano.arcoqualificacao WHERE muncod='".$value['muncod']."' AND qprid='".$_SESSION['projovemurbano']['qprid']."'");
		?>
		<tr>
			<td><?=$value['mundescricao'] ?></td>
			<td><? $db->monta_combo('arcidmunicipio['.$value['muncod'].']', "SELECT arcid as codigo, arcdesc as descricao FROM projovemurbano.arcos WHERE arcstatus='A'", 'S', 'Selecione', 'validarTiposArcos', '', '', '200', 'S', '', '', $arcid); ?></td>
		</tr>
		<?
		endforeach;
		endif;
		?>
		</table>
		<br>
		<?
		
		$sql = "SELECT desdesc,
					   '<input type=\"text\" class=\"obrigatorio normal\" title=\"Valor\" id=\"pgavlrmes_'||des.desid||'\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calcularValorTotal(\''||des.desid||'\');calculaPorcentTotal();\" value=\"'||COALESCE((SELECT DISTINCT CASE WHEN pgavlrmes IS NULL THEN '' ELSE trim(to_char(pgavlrmes,'999g999g999d99')) END FROM projovemurbano.previsaogasto WHERE desid=des.desid and qprid='".$_SESSION['projovemurbano']['qprid']."'),'')||'\" maxlength=\"14\" size=\"16\" name=\"pgavlrmes['||desid||']\" style=\"text-align:;\">' as inp1, 
					   '<input type=\"text\" class=\"disabled\" readonly title=\"Valor\" id=\"pgaqtdmeses_'||des.desid||'\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'#######\',this.value);\" value=\"'||COALESCE((SELECT DISTINCT CASE WHEN pgaqtdmeses IS NULL THEN '10' ELSE trim(pgaqtdmeses::text) END FROM projovemurbano.previsaogasto WHERE desid=des.desid and qprid='".$_SESSION['projovemurbano']['qprid']."'),'10')||'\" maxlength=\"7\" size=\"8\" name=\"pgaqtdmeses['||desid||']\" style=\"text-align:;\">' as inp2,
					   '<input type=\"text\" class=\"disabled\" readonly title=\"Valor\" id=\"pgavlrtotal_'||des.desid||'\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" value=\"'||COALESCE((SELECT DISTINCT CASE WHEN pgaqtdmeses IS NOT NULL AND pgavlrmes IS NOT NULL THEN trim(to_char(pgaqtdmeses*pgavlrmes,'999g999g999d99')) ELSE '' END FROM projovemurbano.previsaogasto WHERE desid=des.desid and qprid='".$_SESSION['projovemurbano']['qprid']."'),'')||'\" maxlength=\"14\" size=\"16\" name=\"rgavalor['||desid||']\" style=\"text-align:;\">' as inp3
				FROM projovemurbano.despesas des 
				WHERE desstatus='A'";
		
		$cabecalho=array("Despesas","Valor/mês(R$)","Meses(10)","Valor total(R$)");
		
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		
		?>
		</div>
		
		<p><input type="hidden" name="qpredutecnica" value="FALSE"><input type="checkbox" name="qpredutecnica" value="TRUE" <?=(($qualificacaoprofissional['qpredutecnica']=="t")?"checked":"") ?>  onclick="controleDivs('div_pronatec',this);" > Programas Nacionais de Educação Técnica em Formação Inicial e Continuada</p>
		<div id="div_pronatec" <?=(($qualificacaoprofissional['qpredutecnica']=="t")?"":"style=\"display:none;\"") ?> >
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Município</td>
			<td class="SubTituloCentro">Núcleo</td>
			<td class="SubTituloCentro">Intituição</td>
			<td class="SubTituloCentro">Cursos/Vargas</td>
		</tr>
		<? 
		if($municipios[0]) :
		foreach($municipios as $value) :
		$nucleos = pegarNucleosPorMunicipio($value['munid']);
		?>
		<tr>
			<td width="35%" rowspan="<?=count($nucleos) ?>">
			<p><b><?=$value['mundescricao'] ?> / <?=$value['estuf'] ?></b></p>
			<?
			$instituicoes = $db->carregar("SELECT DISTINCT ccuinstituicao as codigo, ccuinstituicao as descricao FROM projovemurbano.cargacurso WHERE ccuibge='".$value['muncod']."' ORDER BY ccuinstituicao");
			
			if($instituicoes[0]) :
			
			foreach($instituicoes as $instituicao) :
			?>
			<p>
			<b><?=$instituicao['descricao'] ?></b><br>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" width="100%">
				<?
				$cursos = $db->carregar("SELECT * FROM projovemurbano.cargacurso WHERE ccuinstituicao='".$instituicao['descricao']."' AND ccuibge='".$value['muncod']."'");
				
				if($cursos[0]) :
				?>
				<tr>
				<td class="SubTituloCentro">Curso</td>
				<td class="SubTituloCentro">Vagas Ofertadas</td>
				<td class="SubTituloCentro">Vagas Utilizadas</td>
				</tr>
				<?
				foreach($cursos as $curso) :
				?>
					<tr>
					<td width="50%"><?=$curso['ccucurso'] ?></td>
					<td><input size="6" maxlength="5" type="text" id="totalvagas_<?=$curso['ccuid'] ?>_<?=$value['muncod'] ?>" name="totalvagas_<?=$curso['ccuid'] ?>_<?=$value['muncod'] ?>" class="disabled" readonly value="<?=$curso['ccuqtdvagas'] ?>"></td>
					<td><input size="6" maxlength="5" type="text" id="totalutilizado_<?=$curso['ccuid'] ?>_<?=$value['muncod'] ?>" name="totalutilizado_<?=$curso['ccuid'] ?>_<?=$value['muncod'] ?>" class="disabled" readonly value="<?=$db->pegaUm("SELECT SUM(cupqtdestudantes) FROM projovemurbano.cursoprojovemurbano c
																																		 																																						 INNER JOIN projovemurbano.nucleo n ON n.nucid=c.nucid
																																		 																																						 WHERE ccuid='".$curso['ccuid']."' AND munid='".$value['munid']."' AND cupstatus='A'") ?>"></td>
					</tr>
				<?
				endforeach;
				endif;
				?>
			</table>
			</p>
			<?
			endforeach;
			endif;
			?>
			</td>
			<td width="15%"><?=(($nucleos[0]['entnome'])?$nucleos[0]['entnome']:"Núcleo não cadastrados") ?></td>
			<td width="15%">
			<select id="instituicao_<?=$nucleos[0]['nucid'] ?>" style="width: 150px;" class="CampoEstilo" name="instituicao[<?=$nucleos[0]['nucid'] ?>]" onchange="selecionarInstituicaoMunicipio('<?=$value['muncod'] ?>',this.value,'<?=$nucleos[0]['nucid'] ?>');">
			<option value="">Selecione</option>
			<? 
			if($instituicoes[0]) :
			foreach($instituicoes as $instuticao) :
				echo '<option value="'.$instuticao['codigo'].'" '.(($nucleos[0]['nucinstituicoes']==$instuticao['codigo'])?'selected':'').'>'.$instuticao['descricao'].'</option>';
			endforeach;
			endif;
			?>
			</select>
			<? if($nucleos[0]['nucinstituicoes']) : ?>
			<script>
			jQuery(document).ready(function() {
			selecionarInstituicaoMunicipio('<?=$value['muncod'] ?>','<?=$nucleos[0]['nucinstituicoes'] ?>','<?=$nucleos[0]['nucid'] ?>');
			});
			</script>
			<? endif; ?>

			</td>
			<td id="nucleo_<?=$nucleos[0]['nucid'] ?>">
			&nbsp;
			</td>
		</tr>
		<?
		
		if($nucleos[0]) unset($nucleos[0]);
		
		if(is_array($nucleos) > 0) :
		foreach($nucleos as $nucleo) :
		?>
		<tr>
			<td><?=$nucleo['entnome'] ?></td>
			<td>
			<select id="instituicao_<?=$nucleo['nucid'] ?>" style="width: 150px;" class="CampoEstilo" name="instituicao[<?=$nucleo['nucid'] ?>]" onchange="selecionarInstituicaoMunicipio('<?=$value['muncod'] ?>',this.value,'<?=$nucleo['nucid'] ?>');">
			<option value="">Selecione</option>
			<? 
			if($instituicoes[0]) :
			foreach($instituicoes as $instuticao) :
				echo '<option value="'.$instuticao['codigo'].'" '.(($nucleo['nucinstituicoes']==$instuticao['codigo'])?'selected':'').'>'.$instuticao['descricao'].'</option>';
			endforeach;
			endif;
			?>
			</select>
			<? if($nucleo['nucinstituicoes']) : ?>
			<script>
			jQuery(document).ready(function() {
			selecionarInstituicaoMunicipio('<?=$value['muncod'] ?>','<?=$nucleo['nucinstituicoes'] ?>','<?=$nucleo['nucid'] ?>');
			});
			</script>
			<? endif; ?>
			</td>
			<td id="nucleo_<?=$nucleo['nucid'] ?>">
			&nbsp;
			</td>
		</tr>
		<?
		endforeach;
		endif;
		
		endforeach;
		endif;
		
		?>
		</table>
		</div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=generoAlimenticios';">
		<? if(!$desabilitado) : ?> 
			<input type="button" name="salvar" value="Salvar" onclick="gravarQualificacaoProfissional();">
		<? endif; ?> 
		<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=demaisAcoes';"></td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>