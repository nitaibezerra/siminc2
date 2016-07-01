function abrirMemorando( idMeno )
{
	window.opener.location = 'fabrica.php?modulo=sistema/geral/memorando/visualizar&acao=A&memo=' + idMeno;
	window.close();
}


function mostraDiciplinas()
{
	if( $('#sinalArvore').html() == '+'){
		$('#sinalArvore').html('-');
		$('#disciplinasContratadas-conteudo').show();
	}else{
		$('#sinalArvore').html('+');
		$('#disciplinasContratadas-conteudo').hide();
	}
}

function voltar(url){
	window.location = url;
}

function Excluir(url, msg) {
    if(confirm(msg)) {
        window.location = url;
    }
}


function selecionarProjeto(prjid) {

    document.getElementById('tdmodulo').innerHTML = "Carregando...";
	
    $.ajax({
        type: "POST",
        url: "fabrica.php?modulo=principal/abrirSolicitacao&acao=A",
        data: "requisicao=pegarModulosPorProjeto&prjid="+prjid,
        success: function(msg){
            document.getElementById('tdmodulo').innerHTML = msg;
        }
    });

	
}


function abreListaAnalise(nome, titulo)
{
    var arr = nome.split("_");
	
    var ansid = arr[0];
    var dspid = arr[1];
    var tpeid = arr[2];
	
    if(!ansid){
        alert('Sessão expirou! \nFavor entre novamente.');
        return false;
    }

    //var popUp = window.open('fabrica.php?modulo=principal/popListaAnalise&acao=A&nome='+nome+'&titulo='+titulo+'', 'popListaAnalise', 'height=500,width=400,scrollbars=yes,top=50,left=200');
    var popUp = window.open('fabrica.php?modulo=principal/popFaseProdutos&acao=A&ansid='+ansid+'&dspid='+dspid+'&tpeid='+tpeid, 'popListaAnalise', 'height=500,width=800,scrollbars=yes,top=50,left=200');
    popUp.focus();
}

function atualizarConsultaProdutos() {

    var form = document.getElementById('formulario');
    var vals = '';
	
    for(i=0;i<form.elements.length;i++) {
        if(form.elements[i].id.substr(0,5) == "dspid") {
            if(form.elements[i].checked == true) {
                vals = vals + '&vars[]='+form.elements[i].id.substr(6);
            }
        }
    }
	
    if(vals=='') vals = '&vars[]=0';
	
    $.ajax({
        type: "POST",
        url: "fabrica.php?modulo=principal/abrirSolicitacao&acao=A",
        data: "requisicao=alterarConsultaProtudosEsperados"+vals
    });

}

function submeterAnaliseSolicitacaoServico() {
	
    if(document.getElementById('sidid').value == '') {
        alert('Selecione um sistema');
        return false;
    }
	
    if(document.getElementById('tpsid').value.length == 0) {
        alert('Selecione um tipo de serviço');
        return false;
    }
	
    var form = document.getElementById('formulario');
	
    /*var validacao = false;
	for(i=0;i<form.elements.length;i++) {
		if(form.elements[i].id.substr(0,5) == "dspid") {
			if(form.elements[i].checked == true) {
				validacao = true;
			}
		}
	}
	
	if(!validacao) {
		alert('Selecione uma disciplina contratada');
		return false;
	}
	
	selectAllOptions( document.getElementById( 'prdid' ) );
	
	if(document.getElementById('prdid').options[0].value == '') {
		alert('Selecione um produto');
		return false;
	}*/
	
    if(document.getElementsByName('ansgarantia')[0].checked == true) {
        if(document.getElementById('odsidorigem').value.length == 0) {
            alert('Preencha a O.S. garantia');
            return false;
        }
    }
	
    if(document.getElementById('ansdsc').value.length == 0) {
        alert('Preencha a descrição detalhada');
        return false;
    }
	
    if(document.getElementById('ansprevinicio').value.length == 0) {
        alert('Preencha a previsão de início');
        return false;
    }

    if(document.getElementById('ansprevtermino').value.length == 0) {
        alert('Preencha a previsão de termino');
        return false;
    }
	
    var dataInicio = $("#ansprevinicio").val();
    var dataInicioConvertida = dataInicio.substring(6,10) + dataInicio.substring(3,5) + dataInicio.substring(0,2);
	
    var dataFim = $("#ansprevtermino").val();
    var dataFimConvertida = dataFim.substring(6,10) + dataFim.substring(3,5) + dataFim.substring(0,2);
	
    if (dataInicioConvertida > dataFimConvertida){
        alert('Período informado inválido');
        return false;
    }
	
    /*
	var fiscal = document.getElementById( 'fiscal' );
	selectAllOptions( fiscal );
	
	if ( !fiscal.options[0].value ){
		alert( 'Favor selecionar o fiscal!' );
		return false;
	}
	*/
    if(document.getElementById('fiscal').value.length == 0) {
        alert( 'Favor selecionar o fiscal!' );
        return false;
    }
      
    //    if(document.getElementById('ctrid').value == '') {
    //		alert( 'Favor selecionar o contrato!' );
    //		return false;
    //	}
        
    divCarregando();
    document.getElementById('formulario').submit();
}

function displayStaticMessage(messageContent,cssClass) {
    messageObj.setHtmlContent(messageContent);
    messageObj.setSize(600,600);
    messageObj.setCssClassMessageBox(cssClass);
    messageObj.setSource(false);	// no html source since we want to use a static message here.
    messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
    messageObj.display();
}

function closeMessage() {
    messageObj.close();	
}

function selecionarRequisitante() {

    var unidsc='';
	
    $.ajax({
        type: "POST",
        url: "fabrica.php?modulo=principal/abrirSolicitacao&acao=A",
        data: "requisicao=pegarUnidadeUsuario&usucpf="+document.getElementById('usucpf').value,
        async: false,
        success: function(msg){
            unidsc = msg;
        }
    });
 		
    var opts = document.getElementById('usucpf');
    var usunome = opts.options[opts.selectedIndex].innerHTML;
    document.getElementById('usunomerequisitante').value=usunome;
    document.getElementById('usucpfrequisitante').value=opts.value;
    document.getElementById('unidadedorequisitante').innerHTML=unidsc;
	
    closeMessage();
}

function buscarUsuario() {

    var HTML='';
    divCarregando();
	
    $.ajax({
        type: "POST",
        url: "fabrica.php?modulo=principal/abrirSolicitacao&acao=A",
        data: "requisicao=telaBuscarUsuarios",
        async: false,
        success: function(msg){
            HTML = msg;
        }
    });

    divCarregado();
    displayStaticMessage(HTML,'');
}

function submeterSolicitacaoServico() {
	
    if(document.getElementById('usucpfrequisitante').value.length == 0) {
        alert('Selecione um requisitante');
        return false;
    }
	
    if(document.getElementById('scsnecessidade').value.length == 0) {
        alert('Descreva sua solicitação');
        return false;
    }

    if(document.getElementById('scsjustificativa').value.length == 0) {
        alert('Descreva sua justificativa');
        return false;
    }

    if(document.getElementById('sidid').value == '') {
        alert('Selecione o sistema');
        return false;
    }

    if(document.getElementById('scsprevatendimento').value.length == 0) {
        alert('Preencha a expectativa de atendimento');
        return false;
    }
	
    divCarregando();
    document.getElementById('formulario').submit();
}

function submeterDetalhamentoSolicitacaoServico() {
	
    if(document.getElementById('odsdetalhamento').value.length == 0) {
        alert('Preencha a descrição detalhada');
        return false;
    }
	
    if(document.getElementById('odsdtprevinicio').value.length == 0) {
        alert('Preencha a previsão de início');
        return false;
    }

    if(document.getElementById('odsdtprevtermino').value.length == 0) {
        alert('Preencha a previsão de témino');
        return false;
    }

    if(document.getElementById('odsqtdpfestimada').value.length == 0) {
        alert('Preencha a Qtd. de P.F. Estimado');
        return false;
    }
	
    divCarregando();
    document.getElementById('formulario_').submit();
}

function atualizarConsultaDisciplina(tpeid) {
	
    $.ajax({
        type: "POST",
        url: "fabrica.php?modulo=principal/abrirSolicitacao&acao=A",
        data: "requisicao=alterarConsultaDisciplina&tpeid="+tpeid
    });

}

function salvar() {
    divCarregando();
    document.getElementById('formulario_').submit();
}

function inserirProfissionais() {
    selectAllOptions( document.getElementById( 'usucpf' ) );
    divCarregando();
    document.getElementById('formulario_').submit();
}

function carregarTipoServico(sidid)
{
    loadingComboTipoServico();
    if(sidid){
        $.ajax({
            type: "POST",
            url: "fabrica.php?modulo=principal/abrirSolicitacao&acao=A",
            data: "requisicao=carregarTipoServicoPorSistema&sidid=" + sidid,
            async: false,
            success: function(msg){
                $("#td_tpsid").html(msg);
            }
        });
    }else{
        $("#td_tpsid").html('<select class="CampoEstilo obrigatorio" name="tpsid" id="tpsid" disabled="disabled" onclick="alert(\'Favor selecionar o sistema!\')" ><option>Selecione...</option></select>')
    }
}

function loadingComboTipoServico()
{
    $("#td_tpsid").html('<select class="CampoEstilo obrigatorio" name="tpsid" id="tpsid" disabled="disabled" onclick="alert(\'Favor selecionar o sistema!\')" ><option>Selecione...</option></select>');
}


/**
 * Realiza todos os cálculos referente a tela de formulário da tela de memorando
 */
function calcularMemorando()
{
	if( $(this).attr("tagpersonalida") == "1" ){
	
	    $("#mensagem-erro").hide();
		
	    var idSelecionada, 
	        idValorQtdePF, 
	        idValorQtdePFGlosa,
            idValorQtdePFGlosaApos,
	        idTotalAReceberOS, 
	        valorQtdePF, 
	        valorQtdePFGlosa, 
	        valorAReceberOS, 
	        totalValorQtdePF        = 0,
	        totalValorQtdePFGlosa   = 0,
            totalValorQtdePFGlosaApos   = 0,
	        totalAReceberOS         = 0,
	    	totalComPorcentagemDeDisciplina = 0;

	
	
	    $('.selecionadas:checked').each(function( index ){
	        idSelecionada       = $( this ).val();
	        idValorQtdePF       = '#'+ idSelecionada +'memorandoQtdeValorPF';
	        idValorQtdePFGlosa  = '#'+ idSelecionada +'memorandoQtdePFGlosa';
            idValorQtdePFGlosaApos  = '#'+ idSelecionada +'memorandoQtdePFGlosaApos';
	        idTotalAReceberOS   = '#'+ idSelecionada +'memorandoTotalAReceber';
	        idTotalComPorcentagemDeDisciplina = '#'+ idSelecionada +'memorandoTotalComPorcentagemDeDisciplina';
	
	        valorQtdePF         =  new Number( $( idValorQtdePF ).text().replace( ',',  '.' ) );
	        valorQtdePFGlosa    =  new Number( $( idValorQtdePFGlosa ).text().replace( ',',  '.' ) );
            valorQtdePFGlosaApos    =  new Number( $( idValorQtdePFGlosaApos ).text().replace( ',',  '.' ) );
	        
	        valorAReceberOS     =  $(idTotalAReceberOS).text().replace('.', '');
	        valorAReceberOS     =  valorAReceberOS.replace(',', '.');
	        valorAReceberOS     =  valorAReceberOS.replace('R$', '');
	        valorAReceberOS     =  new Number( valorAReceberOS );
	        
	        valorComPorcentagemDeDisciplina =  $( idTotalComPorcentagemDeDisciplina ).text().replace('.', '');
	        valorComPorcentagemDeDisciplina =  valorComPorcentagemDeDisciplina.replace(',', '.');
	        valorComPorcentagemDeDisciplina =  valorComPorcentagemDeDisciplina.replace('R$', '');
	        valorComPorcentagemDeDisciplina =  new Number( valorComPorcentagemDeDisciplina );
	
	        totalValorQtdePF        		+= valorQtdePF;
	        totalValorQtdePFGlosa   		+= valorQtdePFGlosa;
            totalValorQtdePFGlosaApos  		+= valorQtdePFGlosaApos;
	        totalAReceberOS         		+= valorAReceberOS;
	        totalComPorcentagemDeDisciplina += valorComPorcentagemDeDisciplina;
	    });
	
	    Memorando.valorTotal    	 = totalAReceberOS;
	    Memorando.valorTotalPercDisc = totalComPorcentagemDeDisciplina;
	
	    $('#memorandoSubTotalPF').text( mascaraglobal('[#].###,##',totalValorQtdePF.toFixed(2) ) );
	    $('#memorandoSubTotalGlosa').text( mascaraglobal('[#].###,##',totalValorQtdePFGlosa.toFixed(2) ));
        $('#memorandoSubTotalGlosaApos').text( mascaraglobal('[#].###,##',totalValorQtdePFGlosaApos.toFixed(2) ));
	    $('#valor_total_memorando').text( mascaraglobal('[#].###,##',totalAReceberOS.toFixed(2) ) );
	    $('#valorTotalComPorcentagemDeDisciplina').text( mascaraglobal('[#].###,##',totalComPorcentagemDeDisciplina.toFixed(2) ) );
	    $('#glosaMemorando').trigger('change');

	}else{
		
		alert("A Solicitação de Serviço selecionada, não existem as providências: Termo de Solicitação de Serviço ou Termo de Recebimento Provisório Gerados.");
		
		var trvermelha = $(this).parentsUntil("tr").parent()[0];
		$(trvermelha).removeClass();
		$(trvermelha).addClass("alerta-vermelho");
		
		return false;
	}    
}


$(document).ready(function() {
    if (jQuery.browser.msie) {
        jQuery.ajaxSetup({
            cache   : false            
        });
    }
    
    if( $("#modalFormularioSalvarGlosa").length > 0 )
    {
        $("#modalFormularioSalvarGlosa").dialog({
            autoOpen: false,
            dialogClass: 'modalFabrica',
            modal: true,
            title: 'Glosa da Ordem de Serviço',
            width: 500,
            resizable : false,
            draggable: true,
            buttons: {
                "Fechar": function(){
                    $("label.error").hide();
                    $("#glosaqtdepf").removeClass('error');
                    $("#glosajustificativa").removeClass('error');
                    $(this).dialog("close");
                }

            }
        });
    }

    
    $("#inserirGlosa").click(function(){
        $("#modalFormularioSalvarGlosa").dialog('open');
    });
    
    if( $("#formularioSalvarGlosa").length > 0 )
    {
        $("#formularioSalvarGlosa").validate({
            rules:{
                glosaqtdepf: "required",
                glosajustificativa: "required"
            },
            messages:{
                glosajustificativa: "Campo Obrigatório" 
            }
        });
    }
	
	
    $('.listagem thead tr th').addClass('title');

    /**
     * Victor Martins Machado - 09/10/2014
     * 
     * Foi solicitado que, na página de novo memorando, a pesquisa só deverá ser realizada quando o usuário
     * clicar no botão "Pesquisar".
     */
    //$("#empresaContratada").change(function(event){
    $("#pesquisarMemorando").click(function(event){
            
        $.ajax({
            beforeSend: function(){

                $("#dialogAjax").show();
                $('#glosaMemorando option').attr('selected', '');
                $('#glosaMemorando option[value=""]').attr('selected', 'selected');
                $('#glosaMemorando').trigger('change');

            },
            type: 'post',
            url:'geral/memorando/alterar_tabela_memorando.php',
            cache: false,
            dataType: 'html',
            data: 'empresaContratada=' + $('#empresaContratada').val() + '&formmemotpdpsid='+$('#formmemotpdpsid').val() + '&memoid='+$('#memo').val() + '&anomemorando='+$('#anomemorando').val(),
            success: function(data){
                dados = $.parseJSON(data);
                $("#tabelaDinamicaMemorando").empty();
                $("#textoMemorando").html('');
                tinyMCE.get('textoMemorando').setContent(dados.textoMemorando);
                $("#tabelaDinamicaMemorando").html(dados.tabela);
                Memorando.valorTotal = dados.totalmemo;
                $('#glosaMemorando option').attr('selected', '');
                $('#glosaMemorando option[value=""]').attr('selected', 'selected');
                $('#glosaMemorando').trigger('change');

                var totalOS         = $('.selecionadas').length,
                totalSelecionadas   = $('.selecionadas:checked').length;

                if( totalOS == totalSelecionadas && totalOS != 0 )
                {
                  //  $('#checkall').attr('checked', true);
                }

                $('#checkall').click(function(){
                    var checked = $('#checkall').is(':checked');
                    $('.selecionadas[tagpersonalida=1]').attr('checked', checked);
                });
            },
            complete: function(){
                $("#dialogAjax").hide();
                $(".selecionadas").click( calcularMemorando );
            }
        });
    });
    
    /**
     * Victor Martins Machado - 09/10/2014
     * 
     * Foi solicitado que, na página de novo memorando, a pesquisa só deverá ser realizada quando o usuário
     * clicar no botão "Pesquisar".
     */
    /*$("#formmemotpdpsid").change(function(event){
        
    	$("#empresaContratada").trigger('change');
    });
	
    $("#anomemorando").change(function(event){
    	$("#empresaContratada").trigger('change');
    });*/
    
    $("img.botao-excluir-memorando").click(function(){
        var id = $(this).attr('id');
        
        $("#dialogAjax").show();
        
        $.post( 'geral/memorando/remove_memorando.php', 'memo='+id, function(data){
            $("#dialogAjax").hide();
            alert(data.retorno);
            if (data.status == true){
                parent.location='?modulo=sistema/geral/memorando/listar&acao=A';
            }
        });
    });
    
    if( $("#formularioMemorando").length > 0 )
    {
        $("#formularioMemorando").validate({
            rules:{
            	empresaContratada: "required",
                numeroMemorando: "required",
                dataMemorando: "required",
                servidorPublicoResponsavel: "required"
            }
        });
    }
	
	
    if( $("#form-listaArtefatosGc").length > 0 )
    {
        $("#form-listaArtefatosGc").validate({
            rules:{
                "gc-fiscais": "required"
            }
        });
    }
    
	
    $("#emitirMemorando").click(function(event){
        event.preventDefault();
        var params = '';
        $('#textoMemorando').val(tinyMCE.get('textoMemorando').getContent());
        if ($(".selecionadas:checked").length > 0){
        	if ($("#formularioMemorando").valid()){
        		
        		$("#emitirMemorando").val("Aguarde, processando...");
   				$("#emitirMemorando").attr("disabled",true);
        		
                params = $('#formularioMemorando').serialize();
                $.post( 'geral/memorando/emitir_memorando.php', params, function(data){
                    if(data.status == true)
                    {
                        alert('Memorando emitido com sucesso');
                        parent.location='?modulo=sistema/geral/memorando/visualizar&acao=A&memo='+data.retorno;
                    }else{
                        alert(data.retorno);
                        
                        $("#emitirMemorando").val("Emitir Memorando");
   						$("#emitirMemorando").attr("disabled",false);
                        
                    }
                }, 'json' );
        	}
        } else {
            $("#mensagem-erro").css('display','block');
        }
    });
	
    $("#salvarMemorando").click(function(event){
    	
        event.preventDefault();

        if ($("#formularioMemorando").valid() ){
        	
	        if( $('#memovlrajuste').val() != "" ){
	        	if( $('#memodscajuste').val() == ""){
	        		$("#mensagem-erro-descricao-ajuste").show();
	        		return false;
	        	}
	        }
	
	        //TODO: Colocar a validação de memorando nas respectivas páginas de criação e edição
	        $('#textoMemorando').val( tinyMCE.get('textoMemorando').getContent() );
	        if ($(".selecionadas:checked").length > 0){
	            
	            //Verifica se existe memorando com o mesmo numero no servidor
	        	$.ajax({
                    beforeSend: function(){
                        $("#dialogAjax").show();
                    },
                    type: 'post',
                    url:'geral/memorando/valida_memorando.php',
                    cache: false,
                    dataType: 'json',
                    data: $("#formularioMemorando").serialize(),
                    success: function(isInvalido){
                        if (isInvalido){
                            $("#mensagem-erro-numero-memorando").show();
                            $("#numeroMemorando").addClass('error');
                        } else {
                                                    
                            if ($("#memo").val()==""){
                                $.ajax({
                                    type: 'post',
                                    url:'geral/memorando/novo_memorando.php',
                                    cache: false,
                                    dataType: 'json',
                                    data: $("#formularioMemorando").serialize(),
                                    success: function(data){
                                        $("#dialogAjax").hide();
                                        alert('Dados inseridos com sucesso.');
                                        parent.location='?modulo=sistema/geral/memorando/formulario&acao=A&memo='+data.memoid+'&formmemotpdpsid='+data.formmemotpdpsid;
                                    }
                                });
                            } else {
                                $.ajax({
                                    type: 'post',
                                    url:'geral/memorando/editar_memorando.php',
                                    cache: false,
                                    dataType: 'json',
                                    data: $("#formularioMemorando").serialize(),
                                    complete: function(){
                                        //var idMemorando = $("#memo").val();
                                        //var formmemotpdpsid = $("#formmemotpdpsid").val();
                                        //parent.location='?modulo=sistema/geral/memorando/formulario&acao=A&memo='+idMemorando+'&formmemotpdpsid='+formmemotpdpsid;
                                        $("#dialogAjax").hide();
                                        alert('Dados alterados com sucesso.');
                                        var idMemorando = $("#memo").val();
                                        var formmemotpdpsid = $("#formmemotpdpsid").val();
                                        parent.location='?modulo=sistema/geral/memorando/formulario&acao=A&memo='+idMemorando+'&formmemotpdpsid='+formmemotpdpsid;
                                    }
                                });
                            }
                        }
                    },
                    complete: function(){
                        $("#dialogAjax").hide();
                    }
                });

	        } else {
	            $("#mensagem-erro").css('display','block');
	        }
        }
    });
	

    $("#numeroMemorando, .selecionadas").click(function(){
        $("#mensagem-erro-numero-memorando").hide();
        $("#numeroMemorando").removeClass('error');
    });

    
    
    $(".selecionadas").click(calcularMemorando);
	
    //provisorio para estilização da tabela
    $(".dataTables_paginate span").click(function(){
        var idTbody = $(".dataTables_wrapper table tbody").attr('id');
        if (idTbody == "resultadoPesquisa-listaAuditoriaGc"){
			
            var contador = 0;
            $(".dataTables_wrapper #resultadoPesquisa-listaAuditoriaGc td").each(function(index){
                if (contador == 0 || contador == 1 || contador == 5 || contador == 6 || contador == 7){
                    $(this).addClass("alignCenter");
                }
                contador++;
                if (contador == 8){
                    contador = 0;
                }
            });
        }
		
        $(".abrirModalArtefatosCasoDeUso").click(function(event){
            event.preventDefault();
            var id = $(this).attr('id');
            var nomeProduto = $(this).attr('title');
            $.ajax({
                beforeSend: function(){
                    $("#dialogAjax").show();
                },
                type: 'post',
                url:'geral/gerencia-configuracao/recuperarArtefato.php',
                cache: false,
                dataType: 'html',
                data: "idServicoFaseProduto=" + id,
                success: function(data){
                    servicoFaseProduto = $.parseJSON(data);
					
                    $("#idServicoFaseProdutoCasoDeUso").val(servicoFaseProduto.id);
					
                    $("#repositorioCasoDeUso").val(servicoFaseProduto.repositorio);
					
                    if (servicoFaseProduto.padraoNome == "t"){
                        $("#padraoNomeSimCasoDeUso").attr('selected', 'selected');
                    } else if (servicoFaseProduto.padraoNome == "f") {
                        $("#padraoNomeNaoCasoDeUso").attr('selected', 'selected');
                    }
					
                    if (servicoFaseProduto.padraoDiretorio == "t"){
                        $("#padraoDiretorioSimCasoDeUso").attr('selected', 'selected');
                    } else if (servicoFaseProduto.padraoDiretorio == "f") {
                        $("#padraoDiretorioNaoCasoDeUso").attr('selected', 'selected');
                    }
					
                    if (servicoFaseProduto.encontrado == "t"){
                        $("#encontradoSimCasoDeUso").attr('selected', 'selected');
                    } else if (servicoFaseProduto.encontrado == "f") {
                        $("#encontradoNaoCasoDeUso").attr('selected', 'selected');
                    }
					
                    if (servicoFaseProduto.atualizado == "t"){
                        $("#atualizadoSimCasoDeUso").attr('selected', 'selected');
                    } else if (servicoFaseProduto.atualizado == "f") {
                        $("#atualizadoNaoCasoDeUso").attr('selected', 'selected');
                    }
					
                    if (servicoFaseProduto.necessario == "t"){
                        $("#necessarioSimCasoDeUso").attr('selected', 'selected');
                    } else if (servicoFaseProduto.necessario == "f") {
                        $("#necessarioNaoCasoDeUso").attr('selected', 'selected');
                    }
					
                    $("#modalArtefatosCasoDeUso").dialog({
                        title: "Artefato: " + nomeProduto
                        });
					
                    $("#modalArtefatosCasoDeUso").dialog("open");
                },
				
                complete: function(){
                    $("#dialogAjax").hide();
                }
				
            });
        });
		
       
    });
	
	
});