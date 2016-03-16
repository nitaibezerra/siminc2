$(document).ready(function(){
	
	$('.baixar').click(function(){
		var arqid = $(this).attr('id');
		return windowOpen( 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&download=s&arqid='+arqid,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	});
	
	$('.excluirLinha').live('click',function(){
		if(confirm('Deseja excluir o registro?')){
			var id = $(this).attr('id');

            if (id == "") {
                $($(this).parent().parent()).remove();
                return false;
            }

			$.ajax({
				url		: 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=parecertecnico',
				type	: 'post',
				data	: 'requisicao=excluirAnexo&arqid='+id,
				success	: function(e){
					if(e=='1'){
						$('#tr_'+id).remove();
						alert('Operação realizada com sucesso.');					
					}else{
						alert('Falha ao tentar excluir o anexo!');
					}
				}
			});
		}
	});

	$('.inserirAnex').click(function(e){
        e.preventDefault();
		$('.voltar, .salvar, .continuar').show();
		$('.navegar').hide();
		var tamanho = $('#anexos tr:last').attr('id').replace('tr_','');
		if( tamanho == 'titulo' ){
			tamanho = 0;
		}
		tamanho = parseInt(tamanho)+1;
		$.ajax({
			type: "POST",
			url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
			data: "req=novaLinhaAnexo&cod="+tamanho,
			async: false,
			success: function(msg){
					$('#anexos tr:last').after(msg);
			}
		});
	});
	
	$('.inserirAnex2').click(function(){
		$('.voltar, .salvar, .continuar').show();
		$('.navegar').hide();
		var tamanho = $('#anexos tr:last').attr('id').replace('tr_','');
		if( tamanho == 'titulo' || tamanho == '' ){
			tamanho = 0;
		}
		tamanho = parseInt(tamanho)+1;
		$.ajax({
			type: "POST",
			url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
			data: "req=novaLinhaAnexo2&cod="+tamanho,
			async: false,
			success: function(msg){
					$('#anexos tr:last').after(msg);
			}
		});
	});
	
	$('.inserirResp').click(function(){
		return windowOpen( '?modulo=principal/termoCooperacao/inserir_representante&acao=A&busca=entnumcpfcnpj','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	});
	
	$('.excluir').live('click',function(){
		if ($(this).attr('id') == $('#rdProponente:checked').val() ){
			alert('Não é possível excluir o proponente atual');
		}else{
			if (confirm('Deseja realmente desvincular o representante da unidade gestora ?')){
				//apaga o removido da lista de entidades
				var valores =  $('#entidades').val();
				valores = valores.replace($(this).attr('id'), "");
				valores =valores.replace(",,",",");
				$('#entidades').val(valores);

				$('#linha_'+$(this).attr('id')).remove();
				$('#tb_inserirResp').show();

			}
		}
	});
	
	$('.alterar').live('click',function(){
		var index = 'linha_'+entid;
		var entid = $(this).attr('id');
		return windowOpen('?modulo=principal/termoCooperacao/inserir_representante&acao=A&busca=entnumcpfcnpj&entid='+entid+'&tr='+index,'blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
	});
	
	$('.navegarSalvar').click(function(){
		var valida = validarCampos();
		$('#acaoAba').val(this.id);
		$('#req').val('salvarTermo');		
		if(valida){
			$('#formulario').submit();
		}
	});
	
	$('.salvar').click(function(){

		var valida = validarCampos();
		$('#req').val('salvarTermo');

        /*
		if($('[name=crdmesliberacao[]]') != 'undefined'){
			$('[name=crdmesliberacao[]]').attr('disabled', false);
		}
		if($('[name=crdmesexecucao[]]') != 'undefined'){
			$('[name=crdmesexecucao[]]').attr('disabled', false);
		}
        */

		if(valida){

            var abaAtual = $('[name=abaAtual]').val();

            if (abaAtual != 'previsao') {
                $('#formulario').submit();
            } else {
                $.ajax({
                    type: "POST",
                    url: "/elabrev/elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao",
                    data: $('#formulario').serialize(),
                    success: function(data){
                        $("body").append(data);
                        //console.log(data);
                    }
                });
            }
		}
	});
	
	jQuery("#dialog").dialog({
	      autoOpen: false,
	      show: {
	        effect: "blind",
	        duration: 1000
	      },
	      hide: {
	        effect: "explode",
	        duration: 1000
	      }
	});
  
	jQuery(".ui-dialog-titlebar").hide();

	jQuery('#fechar_dialog').live('click', function(){
        jQuery('#logindoc').val("");
        jQuery('#senhadoc').val("");
		jQuery("#dialog").dialog( "close" );
	});


	jQuery('#logar_dialog').click(function(){
		
		var login = jQuery('#logindoc').val();
		var senha = jQuery('#senhadoc').val();
		var tcpid = jQuery('#tcpiddoc').val();
		
		if(login == ''){
			alert('Informe o Login!');
			jQuery('#logindoc').focus();
			return false;
		}
		
		if(senha == ''){
			alert('Informe a Senha!');
			jQuery('#senhadoc').focus();
			return false;
		}
		
		if(tcpid == ''){
			alert('O código do termo não existe!');			
			return false;
		}

		jQuery.ajax({
			url		: window.location.href,
			type	: 'post',
			data	: '&req=gerarProcessoFNDE&tipo=url&tcpid=' + tcpid + '&login=' + login + '&senha=' + senha,
			success	: function(response){
                //console.log(response);
                alert(jQuery(response).text());
                location.reload();
			}
		});

        //location.href=window.location.href+'&req=gerarProcessoFNDE&tipo=url&tcpid=' + tcpid + '&login=' + login + '&senha=' + senha;
		
	});

    /**
     * Checa o retorno para efetivação da NC enviada ao SIGEF
     */
    $(".ncCheck").click(function(){

        var $userName = $("#sigefusername").val()
          , $password = $("#sigefpassword").val();

        if (!$userName || !$password) {
            alert("Preencha o usuário e a senha do SIGEF!");
            return false;
        }

        $.ajax({
            type: "POST",
            url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=enviarNCfnde",
            data: "req=verifica_nc_sigef&sigefusername="+$userName+"&sigefpassword="+$password,
            beforeSend:function(){

            },
            success: function(data){
                $('body').append(data);
                location.reload();
                //console.log(data);
            }
        });
        return false;
    });
});

function gerarProcessoFNDE( tcpid ){
	jQuery("#dialog").dialog( "open" );
	jQuery('#tcpiddoc').val(tcpid);
	//window.location.href = window.location.href + '&req=gerarProcessoFNDE&tipo=url&tcpid=' + tcpid;
}

function salvaDiretoria(valor){
	if (valor != '')
		window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=tramite&dircodSalvar='+valor;
}

function atualizaComboMunicipio( estuf ){
	$.ajax({
		type: "POST",
		url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
		data: "req=atualizaComboMunicipio&estuf="+estuf,
		async: false,
		success: function(msg){
			jQuery('#td_muncod').html(msg);
		}
	});
	return true;
}

function atualizaUG( ungcod ){
	$.ajax({
		type: "POST",
		url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
		data: "req=dadosUGAjax&ungcod="+ungcod,
		dataType: 'json',
		async: false,
		success: function(msg){	
			$('#divgescod').html(msg.gescod);		
			$('#ungendereco').val(msg.ungendereco);
			$('#ungbairro').val(msg.ungbairro);
			$('#estuf').val(msg.estuf);
			atualizaComboMunicipio( msg.estuf );
			$('#muncod').val(msg.muncod);
			$('#ungcep').val(msg.ungcep);
			$('#ungfone').val(msg.ungfone);
			$('#ungemail').val(msg.ungemail);
            $('#divungcnpj').html(mascaraglobal("##.###.###/####-##", msg.ungcnpj));
			atualizaSecretariaResponsavel( ungcod );
			if(ungcod == '153173'){
				$('#tr_ungpolitica').show();
			}
			else{
				$('#codpoliticafnde').val('');
				$('#tr_ungpolitica').hide();
			}
		}
	});
	return true;
}

function atualizaSecretariaResponsavel(ungcod){
	
	$('[name=ungendereco], [name=ungbairro], [name=estuf], [name=muncod], [name=ungcep], [name=ungfone], [name=ungemail]').attr('disabled', false);
	
	$.ajax({
		type: "POST",
		url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
		data: "req=litaResponsavelUgConc&ungcod="+ungcod,
		async: false,
		success: function(msg){
			jQuery('#responsaveis').html(msg);
			var usucpf = $('#usucpf').val();
			if(typeof usucpf != "undefined"){
				$('#volta').show();
				$('#salva').show();
				$('#continua').show();
			}else if(typeof usucpf == "undefined"){
				$('#volta').hide();
				$('#salva').hide();
				$('#continua').hide();
			}
			$('[name=ungendereco], [name=ungbairro], [name=estuf], [name=muncod], [name=ungcep], [name=ungfone], [name=ungemail]').attr('disabled', true);
		}
	});
	return true;
}

function validarCampos(){
	var erro = 0;
	
	$.each($("#formulario input[type=text], #formulario select, #formulario textarea"), function(i,v){
		if($(v).attr('name') != 'tcpiddoc' && $(v).attr('name') != 'senhadoc' && $(v).attr('name') != 'logindoc'){		
			if( $(v).attr('name') != '' && ( $(v).val() == '' && $(v).attr('id') != 'tcpobsrelatorio' && $(v).attr('id') != 'codpoliticafnde' &&  $(v).attr('name') != 'emeid' ) ){
				//alert($(v).attr('name'));
				erro = 1;
			}
		}
	});
	
	if($('#ungcod').val() == '153173' && $('#codpoliticafnde').val() == ''){
		erro = 1;
	}
	
	if( $('[name="tcptipoemenda"]:checked').val() == 'S' && $('[name="emeid"]').val() == '' ){
		erro = 1;
	}
	
	var abaAtual = $('[name=abaAtual]').val();
	if(abaAtual == 'cronograma'){
		var valor = 0;
		$.each($('input[type=text]'), function(i,v){
			if( ( $(v).attr('id') == 'crdvalor' ) ){
				valor = valor + parseFloat($(v).val().replace(',00', '').replace('.',''));
			}
		});
		$.ajax({
			type	: "POST",
			url		: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
			data	: "req=validarValorCronograma",
			dataType: 'json',
			async	: false,
			success: function(msg){
				var valorPrev = parseFloat(msg.valor_prev);
				var valorCron = valor; //parseFloat(msg.valor_crono);
				if(valorCron > valorPrev){
					alert('O valor total Cronograma de Desembo R$ '+mascaraglobal('[.###],##',valorCron.toFixed(2))+' é maior que o total da Previsão Orçamentaria R$ '+mascaraglobal('[.###],##',valorPrev.toFixed(2)));
					erro = 2;
				}
			}
		}); 
	}

	if(erro == 1){
		alert('Existem campos em branco no formulário, todos os campos devem ser preenchidos!');
		validacao = false;
	}else if(erro == 2) {
		validacao = false;
	}else{
		validacao = true;
	}
	return validacao;
}