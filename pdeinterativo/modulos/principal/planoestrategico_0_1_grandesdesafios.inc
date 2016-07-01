<? 
$estado = $db->pegaUm("SELECT d.esdid FROM pdeinterativo.pdinterativo p INNER JOIN workflow.documento d ON p.docid=d.docid WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
if(in_array(PDEESC_PERFIL_DIRETOR,pegaPerfilGeral()) && $estado != WF_ESD_ELABORACAO && $estado != WF_ESD_ELABORACAO_SEMPDE): ?>
<p align="center"><font color=red>PDE não esta em elaboração. Não é permitido alteração. Por favor visualize na aba <b>1.3 Visualizar PDE</b></font></p>
<? else: ?>
<script>

function planoestrategico_0_1_grandesdesafios() {
	var ideb = jQuery("[name^='respostaideb[']");
	for(var i=0;i<ideb.length;i++) {
		ideb[i].value=mascaraglobal('###',ideb[i].value);
		if(ideb[i].value == '') {
			alert('Preencha o valor do desafio IDEB');
			return false;
		}
		if(!isNumeric(ideb[i].value)) {
			alert('Valor do desafio IDEB não é númerico');
			return false;
		}
	}
	var taxasrendimento = jQuery("[name^='respostataxarendimento[']");
	for(var i=0;i<taxasrendimento.length;i++) {
		taxasrendimento[i].value=mascaraglobal('###',taxasrendimento[i].value);
		if(taxasrendimento[i].value == '') {
			alert('Preencha o valor do desafio Taxa de rendimento');
			return false;
		}
		if(!isNumeric(taxasrendimento[i].value)) {
			alert('Valor do desafio Taxa de rendimento não é númerico');
			return false;
		}
	}
	
	var provabrasil = jQuery("[name^='respostaprovabrasil[']");
	for(var i=0;i<provabrasil.length;i++) {
		provabrasil[i].value=mascaraglobal('###',provabrasil[i].value);
		if(provabrasil[i].value == '') {
			alert('Preencha o valor do desafio Prova Brasil');
			return false;
		}
		if(!isNumeric(provabrasil[i].value)) {
			alert('Valor do desafio Prova Brasil não é númerico');
			return false;
		}

	}


	document.getElementById('formulario').submit();
}

function listaOutrosDesafios() {
	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo.php?modulo=principal/planoestrategico&acao=A",
   		data: "requisicao=listasGrandesDesafios&filtro=desafiooutros",
   		async: false,
   		success: function(msg){document.getElementById('td_outrosdesafios').innerHTML = msg;}
 		});
}

function gerenciarOutrosDesafios() {
	window.open('pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=gerenciarOutrosDesafios','Desafios','scrollbars=no,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

jQuery(document).ready(function() {
	listaOutrosDesafios();
});
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações:</font></td>
	<td class="blue">
<p>Os Grandes Desafios, como o nome sugere, se referem aos resultados que a escola deseja alcançar após a execução do seu plano. Ou seja, deve responder à pergunta: “O que desejamos alcançar após dois anos de trabalho?”.</p>
<p>Para responder esta pergunta, resgatamos os problemas críticos da Dimensão 1 (Indicadores e taxas) para que a escola defina, percentualmente, quanto ela deseja avançar nos principais indicadores (IDEB, Taxas de rendimento e/ ou Prova Brasil). Caso não tenham sido identificados problemas críticos na Dimensão 1, a escola precisa definir, no mínimo, dois desafios a serem perseguidos, já que sempre é possível melhorar os indicadores. Neste caso, é preciso clicar no botão “Inserir outros desafios”, selecionar os desafios, clicar em “OK” e depois indicar, no campo respectivo, o percentual a ser melhorado. Veja o exemplo:</p>
<p>Se a escola tem ou definiu como grande desafio “Elevar a taxa de aprovação da escola em ### % em dois anos no Ensino Fundamental”, deve inserir no espaço em branco o percentual que ela deseja melhorar e orientar o plano para superar este desafio.</p>
<p>Recomendamos aos membros do Grupo de Trabalho que não sejam tão pessimistas, definindo desafios “fáceis” de alcançar, nem tão otimistas, fixando desafios tão elevados que desestimulem a equipe. Analisem o diagnóstico e sejam realistas. Boa sorte!</p>
	</td>
</tr>
</table>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_2_planosdeacao">
<input type="hidden" name="requisicao" value="planoestrategico_0_1_grandesdesafios">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloCentro">Indicadores e Taxas</td>
</tr>
<tr><td>
<?
$dados['filtro'] = "critico";
listasGrandesDesafios($dados);
?>
</td></tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloCentro">Outros Desafios</td>
</tr>
<tr>
	<td class="SubTituloEsquerda"><input type="button" value="Inserir outros desafios" onclick="gerenciarOutrosDesafios();"></td>
</tr>
<tr><td id="td_outrosdesafios"></td></tr>
</table>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
	<td>
		<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_0_orientacoes';" >
		<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_1_grandesdesafios';planoestrategico_0_1_grandesdesafios();">
		<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_2_planoacao';planoestrategico_0_1_grandesdesafios();">
		<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_2_planoacao';" >
	</td>
</tr>
</table>
<? endif; ?>