
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

    if(metaaba == 14 || metaaba == 17 || metaaba == 13 || metaaba == 11 || metaaba ==  12){
//        alert("Não foi calculada a situação das mesorregiões e municípios nesta meta nacional.");
        $('#pesquisa').css("display","inherit");
        $('#tabelaMesoregioes').css("display","none");
        $('#tabelaMunicipios').css("display","none");
        $('#tabelaEstados').css("display","table");
        $('#tabelaRegioes').css("display","table");
    }else{
        if(metaaba == 7 || metaaba == 15 || metaaba == 18 || metaaba == 19 || metaaba == 20)
        {
            $('#tabelaMesoregioes').css("display","none");
            $('#tabelaMunicipios').css("display","none");
            $('#tabelaEstados').css("display","none");
            $('#tabelaRegioes').css("display","none");
            $('#pesquisa').css("display","none");
        }else{
            $('#pesquisa').css("display","inherit");
            $('#tabelaMesoregioes').css("display","table");
            $('#tabelaMunicipios').css("display","table");
            $('#tabelaEstados').css("display","table");
            $('#tabelaRegioes').css("display","table");
        }
        //$('#tabelaMesoregioes').css("display","table");
        //$('#tabelaMunicipios').css("display","table");
    }

    // Esconder mesoregiões e municípios. Solicitação feita em 25/09/2015 pelo escritório de processos
    $('#tabelaMesoregioes').css("display","none");
    $('#tabelaMunicipios').css("display","none");
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
        data: "requisicaoAjax="+requisicaoAjax+"&regioes="+pegaSelecionados('slRegiao[]')+"&estados="+pegaSelecionados('slEstado[]')+"&mesoregioes="+pegaSelecionados('slMesoregiao[]')+"&municipios="+pegaSelecionados('slMunicipio[]'),
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
                listarSubmetas(pegaSelecionados('slRegiao[]'),pegaSelecionados('slEstado[]'),pegaSelecionados('slMesoregiao[]'),pegaSelecionados('slMunicipio[]'));
        }
    });
}

function listarSubmetas(regioes, estados, mesoregioes, municipios)
{
    jQuery.ajax({
        type: "POST",
        url: window.location,
        data: "requisicaoAjax=listagemPrincipal&metid="+$('#metid').val()+"&regioes="+regioes+"&estados="+estados+"&mesoregioes="+mesoregioes+"&municipios="+municipios,
        success: function(retorno){
            $('#divListagem').html(retorno);
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

$(document).ready(function() {
    listarSubmetas('','','','');
    selecionaAba(1);

    $('.linkExibirTabela').live('click', function(){
        if( $('#tabelaPne_' + $(this).attr('contador')).is(':hidden')) {
            $('#tabelaPne_' + $(this).attr('contador')).show();
            $(this).html('Ocultar tabela');
        } else {
            $('#tabelaPne_' + $(this).attr('contador')).hide();
            $(this).html('Exibir tabela');
        }
    });

});