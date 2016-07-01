<?
if($_SESSION['sisfor']['iusd']){
	$cies = recuperarCoordenadorIES($_SESSION['sisfor']['iusd']);
} 


monta_titulo('Execução 2014',$cies);

$execucao = $db->pegaLinha("SELECT unicod, unidsc, loa2014, empenhadas, planejamentofinalizado2014, justificativaempenho2014, round(((empenhadas::numeric/loa2014::numeric)*100),1) as percentual, (loa2014::numeric-empenhadas::numeric) as diferenca FROM sisfor.execucao2014 WHERE unicod='".$_SESSION['sisfor']['unicod']."'");

$justificativaempenho2014 = $execucao['justificativaempenho2014'];

?>
<script>
function finalizarCadastramento() {
	jQuery('#confirmacao').css('display','');
}

jQuery(document).ready(function() {
	calcularOrcamentoExecucao();
	jQuery('#sp_totalsaldo').html('< R$ '+jQuery('#totalsaldonaoempenhado').val()+' >');

	var totalsaldo     = parseFloat(replaceAll(replaceAll(jQuery('#totalsaldonaoempenhado').val(),'.',''),',','.'));
	var totalaprovado  = parseFloat(replaceAll(replaceAll(jQuery('#totalaprovado').html(),'.',''),',','.'));
	var saldofinal     = totalaprovado - <?=$execucao['empenhadas'] ?>;

	if(totalsaldo != saldofinal) {
		jQuery('#diferencaempenho').css('display','');
		
	}
	
});

function calcularOrcamentoExecucao() {

	var totalempenhado  = 0;
	var totalsaldo      = 0;
	
	jQuery("[id^='sifvalorempenhado_']").each(function() {
	
		var sifid = replaceAll(jQuery(this).attr('id'),'sifvalorempenhado_','');
		
		var valortotal  = parseFloat(replaceAll(replaceAll(jQuery('#orcvlrunitario_'+sifid).html(),'.',''),',','.'));
		
		valorempenhado = 0;
		if(jQuery('#sifvalorempenhado_'+sifid).val()!='') {
			valorempenhado  = parseFloat(replaceAll(replaceAll(jQuery('#sifvalorempenhado_'+sifid).val(),'.',''),',','.'));
		}
		

		var saldo          = valortotal-valorempenhado;


		if(saldo < 0) {
			jQuery('#sifsaldonaoempenhado_'+sifid).val('-'+mascaraglobal('###.###.###,##',saldo.toFixed(2)));
		} else {
			jQuery('#sifsaldonaoempenhado_'+sifid).val(mascaraglobal('###.###.###,##',saldo.toFixed(2)));
		}

		totalempenhado  += valorempenhado;
		totalsaldo           += saldo;

	});

	
	jQuery('#totalempenhado').val(mascaraglobal('###.###.###,##',totalempenhado.toFixed(2)));
	jQuery('#totalsaldonaoempenhado').val(mascaraglobal('###.###.###,##',totalsaldo.toFixed(2)));
	
}

function salvarExecucao2014() {
	jQuery('#formulario').submit();
}

function confirmarFinalizacao() {

	if(jQuery('#diferencaempenho').css('display')=='') {
		if(jQuery('#sifjustificativaempenho2014').val()=='') {
			alert('Preencha a justificativa');
			return false;
		}
	}

	jQuery('#requisicao').val('confirmarFinalizacaoExecucao2014');
	jQuery('#formulario').submit();
	
}

function retornarExecucao2014() {

	jQuery('#requisicao').val('retornarExecucao2014');
	jQuery('#formulario').submit();
	
}

</script>

<form id="formulario" method="post" name="formulario" action="">
<input type="hidden" name="requisicao" id="requisicao" value="salvarExecucao2014">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">
	<tr>
		<td class="subtituloDireita" width="25%">Orientações:</td>
		<td>[ORIENTACOES]</td>
	</tr>
	<tr>
		<td class="subtituloDireita" width="25%">Nome da IES:</td>
		<td><?=$execucao['unidsc'] ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita" width="25%">Código da UO:</td>
		<td><?=$execucao['unicod'] ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita" width="25%">Valor total LOA 2014:</td>
		<td>R$ <?=number_format ( $execucao['loa2014'] , 2 , "," , "." ) ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita" width="25%">Valor empenhado 2014 (SIAFI):</td>
		<td>R$ <?=number_format ( $execucao['empenhadas'] , 2 , "," , "." ) ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita" width="25%">Percentual de empenho 2014:</td>
		<td><?=$execucao['percentual'] ?>%</td>
	</tr>
	<tr>
		<td colspan="2"><?
		
		$sql = "
		(
		select 
			case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
			     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
			     when s.ocuid is not null then oc.ocunome 
			     when s.oatid is not null then oatnome end as curso,
			s.sifprofmagisterio,
			'<span style=float:right; id=orcvlrunitario_'||s.sifid||'>'||trim(coalesce(to_char((select sum(orcvlrunitario) as orcvlrunitario from sisfor.orcamento where sifid=s.sifid and orcstatus='A'),'999g999g999d99'),''))||'<span>' as valortotal,
			'<span style=float:right;><input type=\"text\" style=\"text-align:right;\" ".(($execucao['planejamentofinalizado2014']=='t')?"disabled":"")." name=\"sifvalorempenhado['||s.sifid||']\" size=\"16\" maxlength=\"14\" value=\"'||case when sifvalorempenhado > 0 then trim(coalesce(to_char(sifvalorempenhado,'999g999g999d99'),'')) else '0,00' end||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calcularOrcamentoExecucao();\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'###.###.###,##\',this.value);calcularOrcamentoExecucao();\" id=\"sifvalorempenhado_'||s.sifid||'\" title=\"Valor Unitário (R$)\" class=\"obrigatorio normal\"></span>' as empenhadoscurso,
			'<span style=float:right;><input type=\"text\" style=\"text-align:right;\" name=\"sifsaldonaoempenhado\" size=\"16\" maxlength=\"14\" disabled value=\"0,00\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'###.###.###,##\',this.value);\" id=\"sifsaldonaoempenhado_'||s.sifid||'\" title=\"Valor Unitário (R$)\" class=\"obrigatorio normal\"></span>' as saldonaoempenhado
		from sisfor.sisfor s
			inner join workflow.documento d on d.docid = s.docidprojeto
			left join workflow.historicodocumento h on h.hstid = d.hstid
			inner join workflow.estadodocumento e on e.esdid = d.esdid
			inner join public.unidade u on u.unicod = s.unicod
			left join workflow.documento dctur on dctur.docid = s.docidcomposicaoequipe
			left join workflow.estadodocumento ectur on ectur.esdid = dctur.esdid  
			left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
			left join catalogocurso2014.curso cur on cur.curid = ieo.curid
			left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
			left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
			left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
			left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
			left join seguranca.usuario usu on usu.usucpf = s.usucpf
			left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
			left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
			left join sisfor.outraatividade oat on oat.oatid = s.oatid 
			left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
		where sifstatus='A' and s.unicod='".$_SESSION['sisfor']['unicod']."' and d.esdid IN(".ESD_PROJETO_VALIDADO.",".ESD_PROJETO_BLOQUEADO.") 
		order by unidsc
		) union all (
		select 
			'<span style=float:right;>TOTAIS</span>' as total,
			sum(s.sifprofmagisterio) as sifprofmagisterio,
			'<span style=float:right; id=totalaprovado>'||trim(coalesce(to_char(sum((select sum(orcvlrunitario) as orcvlrunitario from sisfor.orcamento where sifid=s.sifid and orcstatus='A')),'999g999g999d99'),''))||'<span>' as valortotal,
			'<span style=float:right;><input type=\"text\" style=\"text-align:right;\" name=\"totalempenhado\" size=\"16\" maxlength=\"14\" value=\"0,00\" disabled onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'###.###.###,##\',this.value);\" id=\"totalempenhado\" title=\"Valor Unitário (R$)\" class=\"obrigatorio normal\"></span>' as empenhadoscurso,
			'<span style=float:right;><input type=\"text\" style=\"text-align:right;\" name=\"totalsaldonaoempenhado\" size=\"16\" maxlength=\"14\" value=\"0,00\" disabled onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'###.###.###,##\',this.value);\" id=\"totalsaldonaoempenhado\" title=\"Valor Unitário (R$)\" class=\"obrigatorio normal\"></span>' as saldonaoempenhado
		from sisfor.sisfor s
			inner join workflow.documento d on d.docid = s.docidprojeto
			left join workflow.historicodocumento h on h.hstid = d.hstid
			inner join workflow.estadodocumento e on e.esdid = d.esdid
			inner join public.unidade u on u.unicod = s.unicod
			left join workflow.documento dctur on dctur.docid = s.docidcomposicaoequipe
			left join workflow.estadodocumento ectur on ectur.esdid = dctur.esdid  
			left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
			left join catalogocurso2014.curso cur on cur.curid = ieo.curid
			left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
			left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
			left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
			left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
			left join seguranca.usuario usu on usu.usucpf = s.usucpf
			left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
			left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
			left join sisfor.outraatividade oat on oat.oatid = s.oatid 
			left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 
		where sifstatus='A' and s.unicod='".$_SESSION['sisfor']['unicod']."' and d.esdid IN(".ESD_PROJETO_VALIDADO.",".ESD_PROJETO_BLOQUEADO.") 

		)";
			
		$cabecalho = array("CURSO VALIDADO","VAGAS","VALOR APROVADO","VALOR EMPENHADO","SALDO NÃO EMPENHADO");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true);
		
		?></td>
	</tr>
	<? if($execucao['planejamentofinalizado2014']=='t') : ?>
	<tr id="confirmacao">
		<td colspan="2">
		
		<span style="font-size:large;"><b>CONCLUSÃO</b></span><br><br>
		<p>Conforme quadro acima, será necessário disponibilizar <span style="font-size:large;" id="sp_totalsaldo"></span> do Orçamento de 2015 para assegurar a execução dos cursos aprovados em 2014 e cujos valores não foram integralmente empenhados.</p>
		
		<div id="diferencaempenho" style="display:none;">
		<p>Identificamos que o valor empenhado nos cursos esta diferente do valor total empenhado pela IES. Por favor insira a justificativa</p>
		<p><? echo campo_textarea( 'justificativaempenho2014', 'N', 'N', '', '70', '4', '500'); ?></p>
		</div>
		
		<span style="font-size:large;"><b>Deseja modificar?</b></span><br><br>
		<p><input type="button" name="confirmanao" value="Sim" onclick="retornarExecucao2014();" disabled></p>
		
		
		</td>
	</tr>
	<? else : ?>
	<tr>
		<td class="subtituloDireita" width="25%">&nbsp;</td>
		<td><input type="button" name="salvar" value="Salvar" onclick="salvarExecucao2014();"> <input type="button" name="finalizar" value="Desejo finalizar o cadastramento" onclick="finalizarCadastramento();"></td>
	</tr>
	<tr id="confirmacao" style="display:none;">
		<td colspan="2">
		
		<span style="font-size:large;"><b>CONCLUSÃO</b></span><br><br>
		<p>Conforme quadro acima, será necessário disponibilizar <span style="font-size:large;" id="sp_totalsaldo"></span> do Orçamento de 2015 para assegurar a execução dos cursos aprovados em 2014 e cujos valores não foram integralmente empenhados.</p>
		
		<div id="diferencaempenho" style="display:none;">
		<p>Identificamos que o valor empenhado nos cursos esta diferente do valor total empenhado pela IES. Por favor insira a justificativa</p>
		<p><? echo campo_textarea( 'justificativaempenho2014', 'S', 'S', '', '70', '4', '500'); ?></p>
		</div>
		
		<span style="font-size:large;"><b>Confirma?</b></span><br><br>
		<p><input type="button" name="confirmasim" value="Sim" onclick="confirmarFinalizacao();" disabled> <input type="button" name="confirmanao" value="Não" onclick="jQuery('#confirmacao').css('display','')"></p>
		
		</td>
	</tr>
	<? endif; ?>

</table>
</form>