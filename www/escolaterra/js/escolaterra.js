function ajaxatualizar(params,iddestinatario) {
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: params,
   		async: false,
   		success: function(html){
   			if(iddestinatario!='') {
   				document.getElementById(iddestinatario).innerHTML = html;
   			}
   		}
	});

}


function carregarMunicipiosPorUF2(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncod_nascimento&name=muncod_nascimento&estuf='+estuf,'td_municipio2');
	} else {
		document.getElementById('td_municipio2').innerHTML = "Selecione uma UF";
	}
}

function carregarMunicipiosPorUF3(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&id=muncod_endereco&name=muncod_endereco&estuf='+estuf,'td_municipio3');
	} else {
		document.getElementById('td_municipio3').innerHTML = "Selecione uma UF";
	}
}

function carregarMunicipiosPorUF4(estuf) {
	if(estuf) {
		ajaxatualizar('requisicao=carregarMunicipiosPorUF&onclick=buscarAgencias&id=muncod_agencias&name=muncod_agencias&estuf='+estuf,'td_municipio4');
	} else {
		document.getElementById('td_municipio4').innerHTML = "Selecione uma UF";
	}
}

function selecionarPeriodoReferencia(fpbid) {
	divCarregando();
	window.location=window.location+'&fpbid='+fpbid;
}

function salvarOrientacaoAdm() {
	
	if(jQuery('#oabdesc').val() == '') {
			alert('Orientação em branco');
			return false;
	}
	
	jQuery('#formulario_orientacao').submit();
	
}

function mostrarOrientacaoAdm(abaid) {
	
	if(abaid=='') {
		alert('Não foi encontrado o menu');
		return false;
	}
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: '&requisicao=carregarOrientacaoPorFiltro&abaid='+abaid,
   		async: false,
   		success: function(texto){
   			jQuery('#oabdesc').val(texto);
   			
   		}
	});
	
	jQuery('#abaid').val(abaid);

	jQuery("#modalOrientacaoAdm").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 800,
	                        height: 400,
	                        modal: true,
	                     	close: function(){} 
	                    });

}