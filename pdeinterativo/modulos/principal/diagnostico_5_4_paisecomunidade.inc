<?
monta_titulo( "Pais e comunidade - Comunidade Escolar", "&nbsp;");


$sql = "SELECT * FROM pdeinterativo.respostapaiscomunidade WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
$respostapaiscomunidade = $db->pegaLinha($sql);

?>
<script>

function diagnostico_5_4_paisecomunidade() {

	if(jQuery("[name^='rpcpossuiconcelho']:checked").length == 0) {
		alert('Selecione se a escola possui Conselho Escolar?');
		return false;
	}
	
	if(document.getElementById('rpcpossuiconcelho_TRUE').checked) {

		if(jQuery("[name^='rpcperiodicidade']:checked").length == 0) {
			alert('Selecione a periodicidade');
			return false;
		}
		
		if(document.getElementById('div_membrosconselho').childNodes[0].rows[0].cells.length == 1) {
			alert('Insira um membro do conselho escolar');
			return false;
		}
		
		if(jQuery("[name='rpcunidadeexecutora']:checked").length == 0) {
			alert('Selecione se O Conselho Escolar é também Unidade Executora da escola.');
			return false;
		}
	
	}
	
	if(!verificaRespostasPerguntas()){
		alert('Favor responder todas as perguntas!');
		return false;
	}
	
	var totalPerguntas=parseInt(jQuery("[name^='perg[']:enabled").length/4);
	var max = parseInt(Math.floor(totalPerguntas/2))+1;
	var numRespS = parseInt(jQuery("[name^='perg['][value=<?=OPP_SEMPRE ?>]:enabled:checked").length);
	var numRespM = parseInt(jQuery("[name^='perg['][value=<?=OPP_MAIORIA_DAS_VEZES ?>]:enabled:checked").length);
	var totresp = numRespS+numRespM;
	if(totresp>max) {
		if(document.getElementById('tr_justificativasevidencias').style.display=='none') {
			document.getElementById('tr_justificativasevidencias').style.display='';
		}
		if(jQuery('#juedescricao').val()=='') {
			alert('Foram assinaladas mais de 50% das respostas com as opções “Sempre” e “Na maioria das vezes”. À luz dos resultados analisados nas dimensões 1 e 2, justifique as respostas.');
			return false;
		}
	} else {
		if(document.getElementById('tr_justificativasevidencias').style.display=='') {
			alert('A justificativa não é mais necessária e será removida.');
			document.getElementById('tr_justificativasevidencias').style.display='none';
			jQuery('juedescricao').val('');
		}
	}
	
	divCarregando();
	document.getElementById('formulario').submit();
}

function gerenciarMembroConselho(apeid, pesid) {
	window.open('pdeinterativo.php?modulo=principal/diagnostico&acao=A&requisicao=gerenciarMembroConselho&apeid='+apeid+'&pesid='+pesid,'Direção','scrollbars=no,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function carregarMembrosConselho() {

	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=carregarMembrosConselho",
   		async: false,
   		success: function(msg){jQuery('#div_membrosconselho').html(msg);}
 		});
	 		
}

function excluirMembroConselho(pesid) {
	var conf = confirm('Deseja realmente excluir?');
	
	if(conf) {

		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo.php?modulo=principal/diagnostico&acao=A",
	   		data: "requisicao=excluirMembroConselho&pesid="+pesid,
	   		async: false,
	   		success: function(msg){
	   			carregarMembrosConselho();
	   			alert(msg);
	   		}
	 		});
 		
 	}
	 		
}

function habilitaPerguntas()
{
	jQuery("#div_conselho [name^='perg[']").attr("disabled","");
}

function desabilitaPerguntas()
{
	jQuery("#div_conselho [name^='perg[']").attr("disabled","disabled");
}


jQuery(document).ready(function() {
	<? if($respostapaiscomunidade['rpcpossuiconcelho']=="t"): ?>
	carregarMembrosConselho();
	<? endif; ?>
	if(jQuery("[name='rpcpossuiconcelho']:checked").val() == "FALSE"){
		desabilitaPerguntas();
	}else{
		habilitaPerguntas();
	}
});

</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações</font></td>
	<td class="blue">
	<p>A escola é uma parte muito importante do processo educativo, mas não é a única. A gestão democrática inclui a interação permanente entre a escola e a comunidade na qual ela se insere e com a qual interage. A família é parte da comunidade e também exerce um papel determinante na formação cidadã.</p>
	<p>A seguir, vamos conhecer um pouco mais sobre as relações entre a escola e a comunidade escolar, com ênfase no papel do Conselho Escolar. Para tanto, leia os comandos, preencha o quadro e assinale em que medida o Grupo de Trabalho concorda com as perguntas. Quando as respostas “sempre” e “na maioria das vezes” somarem mais de 50% em relação ao total de perguntas, é necessário que a escola apresente, no final da tela, justificativas ou evidências sobre esses aspectos, à luz dos resultados apresentados nas dimensões 1 e 2. Ou seja, se a comunidade escolar é atuante e engajada, porque os resultados da escola não são melhores?</p>
	</td>
</tr>
</table>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_5_sintesedimensao5">
<input type="hidden" name="requisicao" value="diagnostico_5_4_paisecomunidade">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%">Comunicação</td>
	<td>
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'C' and prgstatus = 'A' and prgdetalhe='Comunicação' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Participação</td>
	<td>
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'C' and prgstatus = 'A' and prgdetalhe='Participação' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<? $juedescricao = $db->pegaUm("select juedescricao from pdeinterativo.justificativaevidencias where abacod='diagnostico_5_4_paisecomunidade' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'"); ?>
<tr id="tr_justificativasevidencias" <?=(($juedescricao)?'':'style="display:none"') ?>>
	<td class="SubTituloDireita" width="10%">Justificativas/ Evidências</td>
	<td>
	<? echo campo_textarea( 'juedescricao', 'S', 'S', '', '100', '5', '1000'); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Colegiado/Conselho Escolar</td>
	<td>
	<img src="../imagens/exclama.gif"> A escola possui Conselho Escolar? <input type="radio" id="rpcpossuiconcelho_TRUE" name="rpcpossuiconcelho" value="TRUE" <?=(($respostapaiscomunidade['rpcpossuiconcelho']=="t")?"checked":"") ?> onclick="if(this.checked){habilitaPerguntas();document.getElementById('div_conselhoescolar').style.display='';document.getElementById('div_conselho').style.display='';carregarMembrosConselho();}"> Sim <input type="radio" name="rpcpossuiconcelho" value="FALSE" <?=(($respostapaiscomunidade['rpcpossuiconcelho']=="f")?"checked":"") ?> onclick="if(this.checked){desabilitaPerguntas();document.getElementById('div_conselhoescolar').style.display='none';document.getElementById('div_conselho').style.display='none';}"> Não <br/>
	<div id="div_conselhoescolar" <?=(($respostapaiscomunidade['rpcpossuiconcelho']=="t")?"":"style=\"display:none\"") ?> >
	<img src="../imagens/exclama.gif"> Qual a periodicidade com que o Conselho Escolar se reúne? 
		<input type="radio" name="rpcperiodicidade" value="E"  <?=(($respostapaiscomunidade['rpcperiodicidade']=="E")?"checked":"") ?> > Semanal 
		<input type="radio" name="rpcperiodicidade" value="M"  <?=(($respostapaiscomunidade['rpcperiodicidade']=="M")?"checked":"") ?> > Mensal
		<input type="radio" name="rpcperiodicidade" value="B"  <?=(($respostapaiscomunidade['rpcperiodicidade']=="B")?"checked":"") ?> > Bimestral
		<input type="radio" name="rpcperiodicidade" value="T"  <?=(($respostapaiscomunidade['rpcperiodicidade']=="T")?"checked":"") ?> > Trimestral
		<input type="radio" name="rpcperiodicidade" value="S" <?=(($respostapaiscomunidade['rpcperiodicidade']=="S")?"checked":"") ?> > Semestral 
		<input type="radio" name="rpcperiodicidade" value="A" <?=(($respostapaiscomunidade['rpcperiodicidade']=="A")?"checked":"") ?> > Anual
		<input type="radio" name="rpcperiodicidade" value="O"  <?=(($respostapaiscomunidade['rpcperiodicidade']=="O")?"checked":"") ?> > Outra
	<br /><img src="../imagens/exclama.gif"> O Conselho Escolar é também Unidade Executora da escola?
		<input type="radio" name="rpcunidadeexecutora" value="S"  <?=(($respostapaiscomunidade['rpcunidadeexecutora']=="S")?"checked":"") ?> > Sim
		<input type="radio" name="rpcunidadeexecutora" value="N"  <?=(($respostapaiscomunidade['rpcunidadeexecutora']=="N")?"checked":"") ?> > Não
	</div>
	<br/>
	<div id="div_conselho" <?=(($respostapaiscomunidade['rpcpossuiconcelho']=="t")?"":"style=\"display:none\"") ?> >
	<p><b>Clique no botão abaixo para inserir informações sobre os Membros do Conselho.</b></p>
	<p><input type="button" value="Inserir Membros do Conselho" onclick="gerenciarMembroConselho('<?=APE_MEMBROSCONSELHO ?>','');"></p>
	<div id="div_membrosconselho"></div>
	<br />
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Colegiado' order by prgid"; ?>
	<?php quadroPerguntas($sql) ?>
	</div>
	</td>
</tr>
</table>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita">O que deseja fazer agora?</td>
	<td>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_3_demaisprofissionais';" >
	<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_4_paisecomunidade';diagnostico_5_4_paisecomunidade();">
	<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_5_sintesedimensao5';diagnostico_5_4_paisecomunidade();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_5_sintesedimensao5';" >
	</td>
</tr>
</table>
</form>