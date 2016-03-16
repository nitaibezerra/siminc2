<?

monta_titulo( "Direção - Gestão", "&nbsp;");

?>
<script>

function excluirPessoa(pesid, tpeid) {
	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=excluirPessoa&pesid="+pesid+"&tpeid="+tpeid,
   		async: false,
   		success: function(msg){alert(msg);carregarInfoEquipeGestora_4_1();}
 		});
}

function carregarInfoEquipeGestora_4_1() {
	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=carregarInfoEquipeGestora_4_1",
   		async: false,
   		success: function(msg){document.getElementById('td_infoEquipeGestora_4_1').innerHTML = msg;}
 		});
}

jQuery(document).ready(function() {
	carregarInfoEquipeGestora_4_1();
});

function gerenciarDirecao(apeid, pesid) {
	window.open('pdeinterativo.php?modulo=principal/diagnostico&acao=A&requisicao=gerenciarDirecao&apeid='+apeid+'&pesid='+pesid,'Direção','scrollbars=no,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function diagnostico_4_1_direcao() {
	if(!verificaRespostasPerguntas()){
		alert('Favor responder todas as perguntas!');
		return false;
	}
	
	if(jQuery('#validarExisteViceDiretor').val()=='0') {
		var chk_naoExisteVice = parseInt(jQuery("[name^='chk_naoExisteVice']:enabled:checked").length);
		if(chk_naoExisteVice==0) {
			alert('Marque a opção "Não existe Vice-Diretor"');
			return false;
		}
	}
	
	if(jQuery('#validarExisteSecretario').val()=='0') {
		var chk_naoExisteSecretario = parseInt(jQuery("[name^='chk_naoExisteSecretario']:enabled:checked").length);
		if(chk_naoExisteSecretario==0) {
			alert('Marque a opção "Não existe Secretário(a) da escola"');
			return false;
		}
	}
	
	if(jQuery('#validarExisteEquipePedagogica').val()=='0') {
		var chk_naoExisteEquipe = parseInt(jQuery("[name^='chk_naoExisteEquipe']:enabled:checked").length);
		if(chk_naoExisteEquipe==0) {
			alert('Marque a opção "Não existe Equipe Pedagógica"');
			return false;
		}
	}
	
	var tabela_direcao = document.getElementById('tbl_direcao');
	if(tabela_direcao.rows[2].cells[5].innerHTML=="") {
		alert('Clique em alterar os dados do Diretor e preencha o restante dos dados');
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

function naoExisteViceDiretor()
{
	if(jQuery("[name='chk_naoExisteVice']:checked").val()){
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo.php?modulo=principal/diagnostico&acao=A",
	   		data: "requisicao=naoExisteViceDiretor",
	   		async: false,
	   		success: function(msg){
	   				if(msg){
	   					jQuery("[name='chk_naoExisteVice']").attr("checked","");
	   					alert(msg);
	   				}
	   				
	   			}
	 		});
	 }
}

function naoExisteSecretario()
{
	if(jQuery("[name='chk_naoExisteSecretario']:checked").val()){
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo.php?modulo=principal/diagnostico&acao=A",
	   		data: "requisicao=naoExisteSecretario",
	   		async: false,
	   		success: function(msg){
	   				if(msg){
	   					jQuery("[name='chk_naoExisteSecretario']").attr("checked","");
	   					alert(msg);
	   				}
	   				
	   			}
	 		});
	 }
}

function naoExisteEquipe()
{
	if(jQuery("[name='chk_naoExisteEquipe']:checked").val()){
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo.php?modulo=principal/diagnostico&acao=A",
	   		data: "requisicao=naoExisteEquipe",
	   		async: false,
	   		success: function(msg){
	   				if(msg){
	   					jQuery("[name='chk_naoExisteEquipe']").attr("checked","");
	   					alert(msg);
	   				}
	   				
	   			}
	 		});
	 }
}

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações</font></td>
	<td class="blue">
	<p>Um bom diagnóstico sobre a situação da escola precisa identificar se a equipe gestora está bem preparada para realizar suas atividades. É o que vamos fazer nesta tela, inserindo informações relacionadas ao perfil da equipe gestora e dos colaboradores que lhe dão suporte.</p>
	<p>Preencha o quadro abaixo com todas as informações solicitadas. Observe que, excetuando o perfil "Diretor(a)", poderão ser inseridos tantos nomes quantas forem as pessoas que desempenham as demais funções (vice-diretor(a), secretário(a) da escola e equipe pedagógica). Caso não exista algum dos perfis indicados, deixe os espaços em branco. Para inserir nomes, clique no botão "Incluir". Caso deseje modificar os dados inseridos, clique em "Editar". Se desejar excluir algum nome, clique em "Excluir".</p>
	<p>Depois de preencher o quadro, responda às perguntas.</p>
	</td>
</tr>
</table>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_2_processos">
<input type="hidden" name="requisicao" value="diagnostico_4_1_direcao">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%">Equipe Gestora</td>
	<td id="td_infoEquipeGestora_4_1">Carregando...</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Liderança</td>
	<td>
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Liderança'"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Acompanhamento</td>
	<td>
	<?php $sql = "select * from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Acompanhamento'"; ?>
	<?php quadroPerguntas($sql) ?>
	</td>
</tr>
<? $juedescricao = $db->pegaUm("select juedescricao from pdeinterativo.justificativaevidencias where abacod='diagnostico_4_1_direcao' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'"); ?>
<tr id="tr_justificativasevidencias" <?=(($juedescricao)?'':'style="display:none"') ?>>
	<td class="SubTituloDireita" width="10%">Justificativas/ Evidências</td>
	<td>
	<? echo campo_textarea( 'juedescricao', 'S', 'S', '', '100', '5', '1000'); ?>
	</td>
</tr>
</table>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
	<td>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_0_orientacoes';" >
	<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_1_direcao';diagnostico_4_1_direcao();">
	<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_2_processos';diagnostico_4_1_direcao();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_2_processos';" >
	</td>
</tr>
</table>
</form>