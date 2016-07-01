var arrIndicadores = [8,9,12,14,15];

function printFunction(){
    document.getElementById("apres").style.display="none"; //Menu de metas
    //document.getElementById("ti").style.display="block"; //Texto que deve aparecer no pdf
    document.getElementById("esconder").style.display="none";
    document.getElementById("barra-brasil").style.display="none";
    document.getElementById("h1contribuicao").style.display="inline";
    document.getElementById("mostraDados").style.display="block";
    document.getElementById("obs").style.display="none";
    document.getElementById("titulo-meta-some").style.display="none";
    document.getElementById("titulo-meta-aparece").style.display="inline";
    document.getElementById("titulo-meta-desaparece").style.display="none";


    window.print();

    document.getElementById("titulo-meta-desaparece").style.display="inherit";
    document.getElementById("obs").style.display="inline";
    document.getElementById("titulo-meta-some").style.display="inline";
    document.getElementById("titulo-meta-aparece").style.display="none";
    //document.getElementById("ti").style.display="none";
    document.getElementById("mostraDados").style.display="none";
    document.getElementById("apres").style.display="inherit";
    document.getElementById("h1contribuicao").style.display="none";
    document.getElementById("esconder").style.display="inherit";
    document.getElementById("barra-brasil").style.display="inherit";

}

function printAlerta(){
    window.print();

}

function retornaInicio(){
    window.location='http://pne.mec.gov.br';
}

function selecionaAba(metaaba)
{
    $('[id^="abametaf"]').css("color","#4488cc");
    $('[id^="abametaf"]').css("font-weight","");
    $('#abametaf'+metaaba).css("font-weight","bold");
    $('#abametaf'+metaaba).css("color","navy");

    if (jQuery.inArray(metaaba, [11,12,13,14,15,17,18,19,20]) >= 0) {
        $('#pesquisa').css("display","inherit");
        $('#tabelaMesoregioes').css("display","none");
        $('#tabelaMunicipios').css("display","none");
        $('#tabelaEstados').css("display","table");
        $('#tabelaRegioes').css("display","table");
    } else {
        $('#pesquisa').css("display","inherit");
        $('#tabelaMesoregioes').css("display","table");
        $('#tabelaMunicipios').css("display","table");
        $('#tabelaEstados').css("display","table");
        $('#tabelaRegioes').css("display","table");
        $('.metanaoinformada_'+metaaba).css("display","none");
    }
}

function onOffCampo( campo )
{
    var div_on = document.getElementById( campo + '_campo_on' );
    var div_off = document.getElementById( campo + '_campo_off' );
    var input = document.getElementById( campo + '_campo_flag' );
    if ( div_on.style.display == 'none' )
    {
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        input.value = '1';
    }
    else
    {
        div_on.style.display = 'none';
        div_off.style.display = 'block';
        input.value = '0';
    }
}

function pegaSelecionados(elemento)
{
    var result = '';
    var elemento = document.getElementsByName(elemento)[0];
    console.log(elemento);
    for (var i=0; i<elemento.options.length; i++){
        if (elemento.options[i].value != '')
        {
            result += "'"+elemento.options[i].value + "',";
        }
    }

    return result;
}


function selecionaSubmeta(metid)
{
    this.formulario.metid.value = metid;
    this.formulario.submit();
}

function atualizarRelacionadosRegiao(requisicao) {
    //alert(requisicao);

    //1-estado(chamado pela lista de regiao),2-mesoregiao(chamado pela lista de estado), 3-municipio (chamado pela lista de mesoregiao)
    if (requisicao == 1)
    {
        requisicaoAjax = 'listarEstados';
    }
    else if (requisicao == 2)
    {
        requisicaoAjax = 'listarMesoregioes';
    }
    else if (requisicao == 3)
    {
        requisicaoAjax = 'listarMunicipios';
    }

    jQuery.ajax({
        type: "POST",
        url: window.location,
        data: "requisicaoAjax="+requisicaoAjax+"&regioes="+pegaSelecionados('slRegiao[]')+"&estados="+pegaSelecionados('slEstado[]')+"&mesoregioes="+pegaSelecionados('slMesoregiao[]'),
        success: function(retorno)
        {
            if (requisicao == 1)
            {
                $('#tabelaEstados').html(retorno);
                atualizarRelacionadosRegiao(2);
            }
            else if (requisicao == 2)
            {
                $('#tabelaMesoregioes').html(retorno);
                atualizarRelacionadosRegiao(3);
            }
            else if (requisicao == 3)
            {
                $('#tabelaMunicipios').html(retorno);
            }

            if (requisicao == 3 || requisicao == 4)
                listarSubmetas(pegaSelecionados('slRegiao[]'),pegaSelecionados('slEstado[]'),pegaSelecionados('slMesoregiao[]'));
        }
    });
}

function carregaGrafico(valor, pneano, subiddial, pneid, pnevalormeta, pnetipometa, acao, div, tipo, anoprevisto){
    anoprevisto = $('#selAnoCorrente_'+subiddial).val();    
    jQuery.ajax({
        type: "POST",
        url: window.location,
        data: {acao:acao,
               valor:valor,
               pneano:pneano,
               subiddial:subiddial,
               pneid:pneid,
               pnevalormeta:pnevalormeta,
               pnetipometa:pnetipometa,
               tipo:tipo,
               anoprevisto:anoprevisto},
        success: function(retorno){
            $('#'+div).html(retorno);
        }
    });
}

function changeValue(campo, itrid){
    subiddial = $(campo).attr('subiddial'); 
    
    if($(campo).val() < 100 && jQuery.inArray(parseInt(subiddial),arrIndicadores) >= 0){
        alert('O art. 8° da Lei 13005/2014 diz que os Planos Estaduais e Municipais de Educação devem estar em consonância com o Plano Nacional de Educação. Esse valor que você indicou nao corresponde à universalização, como no PNE.');
    }
    if (itrid == '1'){
        if (confirm('Deseja alterar a meta do estado no ano previsto selecionado?')) {
            carregaGrafico($('#slider_'+subiddial).attr('valor'), $('#slider_'+subiddial).attr('pneano'), $('#slider_'+subiddial).attr('subiddial'), $('#slider_'+subiddial).attr('pneid'), $(campo).val(), $('#slider_'+subiddial).attr('tipometa'), 'altera_grafico', 'div_grfMun_'+subiddial, $('#selAnoCorrente_'+subiddial).val());
            $('#slider_'+subiddial).slider( "option", "value", parseInt($(campo).val()) );
        }
    } else {
        if (confirm('Deseja alterar a meta do município no ano previsto selecionado?')) {
            carregaGrafico($('#slider_'+subiddial).attr('valor'), $('#slider_'+subiddial).attr('pneano'), $('#slider_'+subiddial).attr('subiddial'), $('#slider_'+subiddial).attr('pneid'), $(campo).val(), $('#slider_'+subiddial).attr('tipometa'), 'altera_grafico', 'div_grfMun_'+subiddial, $('#selAnoCorrente_'+subiddial).val());
            $('#slider_'+subiddial).slider( "option", "value", parseInt($(campo).val()) );
        }
    }
}

function changeAnoPrevisto(subiddial){
    var anoprevisto = $('#selAnoCorrente_'+subiddial).val();
}

function naoInformado(valor, pneano, subiddial, metid, pneid, div, desabilitar){
    if(confirm('Esta ação irá zerar a meta deste indicador e marca-lo como \'Não informado\'. Deseja continuar?')) {
        var anoprevisto;        
        if ($('#selAnoCorrente_'+subiddial).val() == undefined){
            anoprevisto = pneano;            
        }else{
            anoprevisto = $('#selAnoCorrente_'+subiddial).val();
        }                
        jQuery.ajax({
            type: "POST",
            url: window.location,
            data:{ acao: "nao_informado",
                   valor: valor,
                   pneano: pneano,
                   subiddial: subiddial,
                   metid: metid,
                   pneid: pneid, 
                   anoprevisto: anoprevisto},
            success: function (data) {
                if (desabilitar) {
                                                $(this).attr('disabled', 'disabled');
                }
                $('#slider_'+subiddial).slider( "option", "value", 0 );
                $('#txtSlider_'+subiddial).val(0);
                $('#' + div).html(data);
            }
        });
    }
}

function validaIndicadores(t){
    var janela = window.open('', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    $('#tipo').val('mun');
    $('#requisicao').val("valida_indicadores");
    document.formulario.target = 'relatorio';
    document.formulario.submit();
//            jQuery.ajax({
//                type: "POST",
//                url: window.location,
//                data: "acao=valida_indicadores&tipo=est",
//                success: function (data) {
//                    alert(data);
//                }
//            });
}

function salvarNaoInformado(t) {
        jQuery.ajax({
        type: "POST",
        url: window.location,
        data: {requisicaoAjax:"salvarNaoInformado", 
               metid:$('#metid').val(), 
               pneid: $('#pneinformcomplementar_' + t).data('pneid'), 
               pneinformcomplementar: $('#pneinformcomplementar_' + t).val()},
        success: function(retorno) {
                        }
    });
}

function salvarMeta18(subiddial) {        
    jQuery.ajax({
    type: "POST",
    url: window.location,
    data: {"requisicaoAjax":"salvarMeta18", 
           "subiddial":subiddial, 
           "pnepossuiplanoremvigente": $("#pnepossuiplanoremvigenteH").val(), 
           "pneplanorefcaput": $("#pneplanorefcaputH").val(), 
           "pneanoprevisto": $('#pneanoprevisto').val()},
    success: function(retorno) {
    }
    });
}

function listarSubmetas(regioes, estados, mesoregioes, municipios, itrid)
{
    jQuery.ajax({
        type: "POST",
        url: window.location,
        data: "requisicaoAjax=listagemPrincipal&metid="+$('#metid').val()+"&regioes="+regioes+"&estados="+estados+"&mesoregioes="+mesoregioes+"&municipios="+municipios,
        success: function(retorno){
            $('#divListagem').html(retorno);

                                if (jQuery.inArray($('#metid').val(), [11,12,13,14,15,17,18,19,20]) >= 0) {
                        $('.metanaoinformada_'+$('#metid').val()).css("display","block");
            }

            $(".slider-range").slider({
                range: "min",
                step: 1,
                create: function(event, ui){
                    $(this).slider( "option", "value", $(this).attr('sliderval') );
                    $(this).slider( "option", "max", $(this).attr('maxval') );
                    if($(this).attr("tipometa") == "P"){
                        $(this).slider( "option", "step", 0.5 );
                    }
                    $('#txtSlider_'+$(this).attr('subiddial')).val($(this).attr('sliderval'));
                },
                slide: function( event, ui ) {
                    $('#txtSlider_'+$(this).attr('subiddial')).val(ui.value);
                },
                change: function(event, ui){
                    if(event.originalEvent) {                        
                        var anocorrente = '#selAnoCorrente_' + $(this).attr('subiddial');                        
                        if(ui.value < 100 && jQuery.inArray(parseInt($(this).attr('subiddial')),arrIndicadores) >= 0){
                            alert('O art. 8° da Lei 13005/2014 diz que os Planos Estaduais e Municipais de Educação devem estar em consonância com o Plano Nacional de Educação. Esse valor que você indicou nao corresponde à universalização, como no PNE.');
                        }
                        if (itrid == '1'){
                            if (confirm('Deseja alterar a meta do estado no ano previsto selecionado?')) {                                
                                carregaGrafico($(this).attr('valor'), $(this).attr('pneano'), $(this).attr('subiddial'), $(this).attr('pneid'), ui.value, $(this).attr('tipometa'), 'altera_grafico', 'div_grfMun_' + $(this).attr('subiddial'), $(anocorrente).val());
                            }
                        } else {                            
                            if (confirm('Deseja alterar a meta do município no ano previsto selecionado?')) {
                                carregaGrafico($(this).attr('valor'), $(this).attr('pneano'), $(this).attr('subiddial'), $(this).attr('pneid'), ui.value, $(this).attr('tipometa'), 'altera_grafico', 'div_grfMun_' + $(this).attr('subiddial'), $(anocorrente).val());
                            }
                        }
                    }
                }
            });
        }
    });
}

function abreNota(submeta){
    jQuery.ajax({
        type: "POST",
        url: window.location.href,
        data: "&requisicaoAjax=mostraTexto&submeta="+submeta,
        async: false,
        success: function(msg){

            var janela = window.open('','Popup','width=800,height=500');
            janela.document.write(msg);

        }
    });
}

function changeRadio(radio, tr, tresconde, radioNull){      
    if ($(radio).is(':checked')) {        
        $("#"+$(radio).attr('name')+'H').val($(radio).val());                
        if (tr!=''){$("."+tr).show();}
        if (tresconde!=''){$("."+tresconde).hide();}
        if (radioNull!=''){
            $("input[name="+radioNull+"]:radio").removeAttr('checked');
            $("#"+radioNull+'H').val('null');
        }
    }
}

$(document).ready(function() {
    listarSubmetas('','','','', '');
    selecionaAba(1);

    $('.linkExibirTabela').on('click', function(){
        if( $('#tabelaPne_' + $(this).attr('contador')).is(':hidden')) {
            $('#tabelaPne_' + $(this).attr('contador')).show();
            $(this).html('Ocultar tabela');
        } else {
            $('#tabelaPne_' + $(this).attr('contador')).hide();
            $(this).html('Exibir tabela');
        }
    });    
});