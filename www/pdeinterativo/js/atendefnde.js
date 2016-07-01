jQuery.noConflict();

function ajax(parametros,destino)
{
	jQuery.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=" + parametros,
		   success: function(msg){
				var tipo = jQuery('[id=' + destino + ']').attr("type");
		   		if(tipo){
		   			jQuery('[id=' + destino + ']').val(msg);
		   		}else{
		   			jQuery('[id=' + destino + ']').html(msg);
		   		}
		   }
		 });
}
	
function filtraMunicipio(estuf)
{
	ajax("filtraMunicipio&estuf=" + estuf,"td_muncod");
}

function filtraOrgao(tpocod)
{
	ajax("filtraOrgao&tpocod=" + tpocod,"td_orgid");
}

function respondePergunta(prgid,valor)
{
	var perg = jQuery("#prgid_" + prgid).html();
	var valorAntigo = jQuery("[name=hdn_prgid_" + prgid + "]").val();
	if(!valorAntigo){
		valorAntigo = "(*)"
	}
	jQuery("#prgid_" + prgid).html( perg.replace(valorAntigo,valor) );
	var perg = jQuery("#prgid_" + prgid).html();
	jQuery("#prgid_" + prgid).html( perg.replace("?",".") );
	jQuery("[name=hdn_prgid_" + prgid + "]").val(valor);
}

function exibeTRs()
{
	jQuery("[id^='tr_']").show();
}


function gerenciarProgramas(sprmodulo, spoid) {
	window.open('pdeinterativo.php?modulo=principal/diagnostico&acao=A&requisicao=gerenciarProgramas&sprmodulo='+sprmodulo+'&spoid='+spoid,'Programas','scrollbars=no,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function gerenciarProjetos(sprmodulo, sprid) {
	window.open('pdeinterativo.php?modulo=principal/diagnostico&acao=A&requisicao=gerenciarProjetos&sprmodulo='+sprmodulo+'&sprid='+sprid,'Projetos','scrollbars=no,height=300,width=500,status=no,toolbar=no,menubar=no,location=no');
}

function excluirProjeto(sprmodulo, sprid) {
	var conf = confirm('Deseja realmente excluir?');
	if(conf) {
	
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo.php?modulo=principal/diagnostico&acao=A",
	   		data: "requisicao=excluirProjeto&sprid="+sprid,
	   		async: false,
	   		success: function(msg){
	   				alert(msg);
					carregarProjetos(sprmodulo);
	   			}
	 		});
 	}
}

function excluirPrograma(sprmodulo, spoid) {
	var conf = confirm('Deseja realmente excluir?');
	if(conf) {
	
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo.php?modulo=principal/diagnostico&acao=A",
	   		data: "requisicao=excluirPrograma&spoid="+spoid,
	   		async: false,
	   		success: function(msg){
	   				alert(msg);
					amas(sprmodulo);
	   			}
	 		});
 	}
}

function carregarProgramas(sprmodulo) {

	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=carregarProgramas&sprmodulo="+sprmodulo,
   		async: false,
   		success: function(msg){
   				document.getElementById('programa_label').innerHTML = msg;
   				extrairScript(msg);
   			}
 		});

}

/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}


function gravarRespostaProgramasProjetos(smodulo,tipo,resposta) {

	if(resposta==false) {
		var desc;var req;var ret=true;var label;
		if(tipo=="G") {
			req="existePrograma";
			desc="PROGRAMAS";
			label="programa_label";
		} else {
			req="existeProjeto";
			desc="PROJETOS";
			label="projeto_label";
		}
		if(document.getElementById(label).childNodes[1]) {
			if(document.getElementById(label).childNodes[1].rows[0].cells.length!=1) {
				var conf = confirm("Essa resposta irá remover TODOS "+desc+" CADASTRADOS. Deseja continuar?");
				if(!conf) {
					ret=false;
				}
			}
		}
	 	
	 	if(!ret) {
	 		return false;
	 	}
	}

	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=gravarRespostaProgramasProjetos&smodulo="+smodulo+"&tipo="+tipo+"&resposta="+resposta,
   		async: false,
   		success: function(msg){}
 		});
 		
 	return true;

}

function carregarProjetos(sprmodulo) {

	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo.php?modulo=principal/diagnostico&acao=A",
   		data: "requisicao=carregarProjetos&sprmodulo="+sprmodulo,
   		async: false,
   		success: function(msg){
   				document.getElementById('projeto_label').innerHTML = msg;
   				extrairScript(msg);
   			}
 		});

}

function irTelaPrincipal() {
	window.location='pdeinterativo.php?modulo=principal/principalDiretor&acao=A';
}

function isUrl(s) {
	var regexp = /(ftp|http|https):\/\/(\w+:{0,1}\w*@)?(\S+)(:[0-9]+)?(\/|\/([\w#!:.?+=&%@!\-\/]))?/
	return regexp.test(s);
}

function verificaRespostasPerguntas()
{
	var erro = 0;
	var numPerg = jQuery("[name^='perg[']:enabled").length;
	var numPergresp = jQuery("[name^='perg[']:enabled:checked").length;
	
	if(numPerg > 0){
		numPerg = numPerg/4;
		if(numPerg != numPergresp){
			erro = 1;
		}else{
			erro = 0;
		}
	}else{
		erro = 0;
	}
	
	if(erro == 0){
		return true;
	}else{
		return false;
	}
}

