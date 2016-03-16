<? 
$estado = $db->pegaUm("SELECT d.esdid FROM pdeinterativo2013.pdinterativo p INNER JOIN workflow.documento d ON p.docid=d.docid WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
if(!$db->testa_superuser() && in_array(PDEESC_PERFIL_DIRETOR,pegaPerfilGeral()) && $estado != WF_ESD_ELABORACAO && $estado != WF_ESD_ELABORACAO_SEMPDE && $estado != WF_ESD_ELABORACAO_FEDERAL): ?>
<p align="center"><font color=red>PDE não esta em elaboração. Não é permitido alteração. Por favor visualize na aba <b>1.3 Visualizar PDE</b></font></p>
<? else: ?>
<script>

function planoestrategico_0_2_planoacao() {
	var pes_prb = jQuery("[name^='pesid_problemas[']");
	for(var i=0;i<pes_prb.length;i++) {
		if(pes_prb[i].value == '') {
			alert('Preencha os reponsaveis por problemas');
			return false;
		}
	}
	var pes_est = jQuery("[name^='pesid_estrategias[']");
	for(var i=0;i<pes_est.length;i++) {
		if(pes_est[i].value == '') {
			alert('Preencha os reponsaveis por estrategias');
			return false;
		}
	}
	var aoaid = jQuery("[name^='aoaid[']");
	for(var i=0;i<aoaid.length;i++) {
		if(aoaid[i].value == '') {
			alert('Selecione os objetivos');
			return false;
		}
	}
	
	var mets = jQuery("[name^='metas[']");
	for(var i=0;i<mets.length;i++) {
		mets[i].value=mascaraglobal('########', mets[i].value);
		if(mets[i].value == '') {
			alert('Preencha as metas');
			return false;
		}
	}
	
	divCarregando();
	document.getElementById('formulario').submit();
}

function gerenciarEstrategias(abaid,papid) {
	window.open('pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&requisicao=gerenciarEstrategias&abaid='+abaid+'&papid='+papid,'Estrategias','scrollbars=yes,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function gerenciarAcao(paeid,paaid) {
	window.open('pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&requisicao=gerenciarAcao&paeid='+paeid+'&paaid='+paaid,'Ação','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function excluirAcao(paaid) {
	var conf = confirm('Deseja realmente apagar a ação?');
	if(conf) {
		divCarregando();
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A",
	   		data: "requisicao=excluirAcao&paaid="+paaid,
	   		async: false,
	   		success: function(msg){alert(msg);}
	 		});
		carregarPlanoEstrategicoDimensao();
	}
}

function carregarPlanoEstrategicoDimensao() {
	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A",
   		data: "requisicao=carregarPlanoEstrategicoDimensao",
   		async: false,
   		success: function(msg){
   			document.getElementById('planoestrategico').innerHTML = msg;
   			extrairScript(msg);
   			divCarregado();
   			}
 		});
}

jQuery(document).ready(function() {
	document.getElementById('planoestrategico').innerHTML = "<p align=center>Carregando...</p>";
	carregarPlanoEstrategicoDimensao();
	exibeDimensao(4);
	jQuery("[class='tooltip']").tooltip({
		track: true
	});
});

function exibeDimensao(abaid)
{
	jQuery("[class^='dimensao_']").hide();
	jQuery("[class^='dimensao_" + abaid + "']").show();
	jQuery("[id^='img_mais_']").show();
	jQuery("[id^='img_menos_']").hide();
	jQuery("#img_menos_" + abaid).show();
	jQuery("#img_mais_" + abaid).hide();
}

function salvarFormularioParcialmente() {
	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A",
   		data: "requisicao=planoestrategico_0_2_planoacao&salvarparcial=true&"+jQuery('#formulario').serialize(),
   		success: function(msg){}
 		});
}

</script>
<style>
.bordapreto {border: 1px solid #000;}
</style>
<link rel="stylesheet" href="http://code.jquery.com/ui/1.10.0/themes/base/jquery-ui.css" />
<script src="http://code.jquery.com/jquery-1.8.3.js"></script>
<script src="http://code.jquery.com/ui/1.10.0/jquery-ui.js"></script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações:</font></td>
	<td class="blue">
	<p>Para facilitar a definição das ações, o MEC selecionou previamente alguns verbos de ação e os principais objetos para cada verbo. Neste caso, para construir a sentença, a escola deve escolher a Ação, indicar a Quantidade e definir o Objeto da ação. Depois, é necessário descrever o período em que a escola pretende realizar aquela ação e, no campo "Detalhamento ação", descrever algumas características da atividade que será realizada. </p>
	<p>Depois de definir a ação e seus objetivos, é necessário responder se são necessários recursos financeiros para realiza-la. Em caso afirmativo, selecione "Sim". O sistema exibirá o botão "Inserir". Neste caso, o GT deve indicar a "Categoria da despesa", escolher um "Item" daquela categoria, definir a "Unidade de referência", escolher “Quantidade” daquele item, descrever o "Valor unitário", informar a "Fonte" e escolher se o item será adquirido com recursos da 1ª parcela ou da 2ª parcela. Observe que o saldo de recursos de cada parcela e a natureza da despesa vão diminuindo à medida em que forem inseridos bens ou serviços.</p>
	<p>O sistema calculará o valor total e exibirá na rubrica capital ou custeio, de acordo com a classificação indicada na Portaria 448/2002. Caso exista divergência de classificação da natureza da despesa em relação aos critérios da sua secretaria, não inclua aquele item, a fim de que não haja problemas durante a execução do plano.</p>
	<p>E lembre-se! Antes de inserir as ações, observe o saldo de recursos do Ano 1 e Ano 2 e os respectivos valores de capital e custeio.</p>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Grandes Desafios:</td>
	<td>
	<hr>
	<?
	$dados['desabilitar'] = true;
	$dados['filtro'] = "critico";
	$lista_c = (array) listasGrandesDesafios($dados);
	$dados['filtro'] = "desafiooutros";
	$lista_d = (array) listasGrandesDesafios($dados);
	$lista_f = array_merge($lista_c, $lista_d);
	echo implode("<br>",$lista_f);

	?>
	</td>
</tr>
</table>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_3_visualizarplanoacao">
<input type="hidden" name="requisicao" value="planoestrategico_0_2_planoacao">
<div id="planoestrategico"></div>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
	<td>
		<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_1_priorizacao_problemas';" >
		<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_2_planoacao';planoestrategico_0_2_planoacao();">
		<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_3_visualizarplanoacao';planoestrategico_0_2_planoacao();">
		<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_3_visualizarplanoacao';" >
	</td>
</tr>
</table>
<? endif; ?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>