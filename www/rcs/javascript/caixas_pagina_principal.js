/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function aleatorio(){
    var numPossibilidades = 12;
    var aleat = Math.random() * numPossibilidades;
    aleat = Math.floor(aleat);
    return aleat;
}

function definrCoresCaixas(){
    var corPri = ["#18BC9C", "#EEB422", "#FF6347", "#00CED1", "#473C8B", "yellowgreen", "darksalmon", "royalblue", "#483D8B", "#FFD700", "#CD853F", "#838B8B", "#6CA6CD"];
    var corSec = ["#AFEEEE", "#EEDD82", "#FFA07A", "#AFEEEE", "#6959CD", "#BCEE68", "#FFDAB9", "#87CEEB", "#7B68EE", "#EEDD82", "#DEB887", "#C1CDCD", "#87CEFF"];

    //#CAIXA 1
    var indice_a = aleatorio();
    $('#divCaixa_1').css("background-color", corPri[indice_a]);
    $('#btnCaixa_1').mouseover(function(){
        $('#btnCaixa_1').css("background-color", corSec[indice_a]);
    });
    $('#btnCaixa_1').mouseout(function(){
        $('#btnCaixa_1').css("background-color", "#FFF");
    });

    //#CAIXA 2
    var indice_m = aleatorio();
    $('#divCaixa_2').css("background-color", corPri[indice_m]);
    $('#btnCaixa_2').mouseover(function(){
        $('#btnCaixa_2').css("background-color", corSec[indice_m]);
    });
    $('#btnCaixa_2').mouseout(function(){
        $('#btnCaixa_2').css("background-color", "#FFF");
    });

    //#CAIXA 3
    var indice_T = aleatorio();
    $('#divCaixa_3').css("background-color", corPri[indice_T]);
    $('#btnCaixa_3').mouseover(function(){
        $('#btnCaixa_3').css("background-color", corSec[indice_T]);
    });
    $('#btnCaixa_3').mouseout(function(){
        $('#btnCaixa_3').css("background-color", "#FFF");
    });

    //#ACESSO A URL - LOCATION
    $('.btnOn').click(function() {
        var url = $(this).attr('data-request');
        if(!url){
            alert('Seu perfil não tem permissão para acessar esse módulo. Entre em contato com o Administrador sistema e solicite a permissão!');
            return;
        }
        location.href = url;
    });
}