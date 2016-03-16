function pesquisar(){		
	var myajax = new Ajax.Request('emenda.php?modulo=principal/listaEntidadesBeneficiadas&acao=A', {
		        method:     'post',
		        parameters: '&pesquisaAjax=true&enbcnpj='+$('enbcnpj').value+
		        							   '&enbnome='+$('enbnome').value,
		        asynchronous: false,
		        onComplete: function (res){
					$('lista').innerHTML = res.responseText;
		        }
		  });
}
function excluir(enbid){
	if(confirm("Tem certeza que deseja excluir este registro?")){
	
		var myAjax = new Ajax.Request('emenda.php?modulo=principal/listaEntidadesBeneficiadas&acao=A', {
					        method:     'post',
					        parameters: '&excluirAjax=true&enbid='+enbid,
					        onComplete: function (res){	
								if(res.responseText == "1" ){
									alert('Operação realizada com sucesso!');
									pesquisar(); 
								}else{
									alert('Operação não realizada!');
								}
					        }
					  });
	}
}

function EntidadeBeneficiada(enbid){
	window.open('emenda.php?modulo=principal/cadastrarEntidadeBeneficiada&acao=A&enbid='+enbid,'page','height=500,width=500,toolbar=no,location=no,status=yes,menubar=no,scrollbars=no,resizable=no,fullscreen=no');
}
function consultarSituacaoEntidade( enbcnpj, enbid ){
	var parms = '';
	if( enbcnpj ){
		parms = '&consultarSituacaoEntidade=true&enbcnpj='+enbcnpj+'&enbid='+enbid;
	} else {
		parms = '&consultarSituacaoEntidade=true&enbcnpj=&enbid=';
	}
	$('loader-container').show();
	var myajax = new Ajax.Request('emenda.php?modulo=principal/listaEntidadesBeneficiadas&acao=A', {
			        method:     'post',
			        parameters: parms,
			        asynchronous: false,
			        onComplete: function (res){
						pesquisar();
			        }
			  });
	$('loader-container').hide();
}

function enviarEmailEntidade(){
	var largura = 673;
	var altura = 500;
	//pega a resolução do visitante
		w = screen.width;
		h = screen.height;
		
		//divide a resolução por 2, obtendo o centro do monitor
		meio_w = w/2;
		meio_h = h/2;
		
		//diminui o valor da metade da resolução pelo tamanho da janela, fazendo com q ela fique centralizada
		altura2 = altura/2;
		largura2 = largura/2;
		meio1 = meio_h-altura2;
		meio2 = meio_w-largura2;
		
		window.open('emenda.php?modulo=principal/enviarEmailEmenda&acao=A','','height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+'');
}