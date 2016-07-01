function replaceAll(string, token, newtoken) {
    while (string.indexOf(token) != -1) {
        string = string.replace(token, newtoken);
    }
    return string;
}

function verificaNavegadorIE() {
    var nom = navigator.appName;
    var browserIE = false;

    if (nom == 'Microsoft Internet Explorer') {
        browserIE = true;
    } else if (nom == 'Netscape') {
        browserIE = false;
    }

    return browserIE;
}

function executarScriptPai(funcao) {
    (verificaNavegadorIE()) ? window.opener.execScript(funcao) : window.opener
            .eval(funcao);
}

function limitarTextoCampo(campo, limiteMax) {
    var conteudo = campo.value;

    if (conteudo.length > limiteMax) {
        var texto = conteudo.substring(0, limiteMax);
        campo.value = texto;
    }
}

function ctrlDisplay(idAbre, idFecha) {
    var d = document;
    var displayAbre = (navigator.appName.indexOf('Explorer') > -1 ? 'block'
            : 'table-row');
    var displayFecha = 'none';

    if (typeof (idAbre) != 'object' && idAbre != '') {
        idAbre = new Array(idAbre);
    }

    if (typeof (idFecha) != 'object' && idFecha != '') {
        idFecha = new Array(idFecha);
    }

    // Abre
    for (i = 0; i < idAbre.length; i++) {
        obj = d.getElementById(idAbre[i]);
        obj.style.display = displayAbre;
    }

    // Fecha
    for (i = 0; i < idFecha.length; i++) {
        obj = d.getElementById(idFecha[i]);
        obj.style.display = displayFecha;
    }
}

function redireciona(url) {
    location.href = url;
}

function confirmExcluir(msg, url) {
    if (confirm(msg))
        location.href = url;
    return;
}

AbrirPopUp = function(url, nome, param) {
    window.open(url, replaceAll(nome,' ', '_'), param);
}