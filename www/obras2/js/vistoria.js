var arrFotosGaleria = new Array();

function validasituacao(id, superuser) {

    if (!document.formulario.supdata.value && ( superuser == 1 )) {
        alert("Favor inserir a Data do Acompanhamento!");
        document.formulario.supdata.focus();
        document.formulario.staid.value = "";
        return false;
    }

    var obData = new Data();
    var d = document;

    if (id == 1 || id == 2 || id == 3 || id == 6) {
        /*
         if( d.formulario.supdata.value != ""
         && d.formulario.obrdtinicio.value != ""
         && d.formulario.obrdttermino.value != "" ){

         if( !(obData.comparaData( d.formulario.supdata.value, d.formulario.obrdtinicio.value, '>=' )
         && obData.comparaData( d.formulario.supdata.value, d.formulario.obrdttermino.value, '<=' )) ){
         alert( "Para inserir uma vistoria com a situação Em Execução a Data da Vistoria deve estar no intervalo entre a Data de Inicío de Execução da Obra ("+d.formulario.obrdtinicio.value+") e Término da Obra ("+d.formulario.obrdttermino.value+")!" );
         document.formulario.supdata.focus();
         return false;
         }
         }
         */
    }

    if (id == 4) {
        if (d.formulario.supdata.value != "" && d.formulario.fprdtiniciofaseprojeto.value != "" && d.formulario.fprdtconclusaofaseprojeto.value != "") {
            if (!(obData.comparaData(d.formulario.supdata.value, d.formulario.fprdtiniciofaseprojeto.value, '>=') && obData.comparaData(d.formulario.supdata.value, d.formulario.fprdtconclusaofaseprojeto.value, '<='))) {
                alert("Para inserir um acompanhamento com a situação Em Elaboração de Projetos a Data do acompanhamento deve estar no intervalo entre a Data de Inicío Programado (" + d.formulario.fprdtiniciofaseprojeto.value + ") e Término Programado do Projeto (" + d.formulario.fprdtconclusaofaseprojeto.value + ")!");
                document.formulario.supdata.focus();
                return false;
            }
        }
    }

    if (id == 5) {
        if (d.formulario.supdata.value != "" && d.formulario.dtiniciolicitacao.value != "" && d.formulario.dtfinallicitacao.value != "") {
            if (!(obData.comparaData(d.formulario.supdata.value, d.formulario.dtiniciolicitacao.value, '>=') && obData.comparaData(d.formulario.supdata.value, d.formulario.dtfinallicitacao.value, '<='))) {
                alert("Para inserir um acompanhamento com a situação Em Licitação a Data do acompanhamento deve estar no intervalo entre a Data de Inicío Programado (" + d.formulario.dtiniciolicitacao.value + ") e Data de Término Programado (" + d.formulario.dtfinallicitacao.value + ") da Licitação !");
                document.formulario.supdata.focus();
                return false;
            }
        }
    }

    return true;

}
/*
 function verificasituacao( id ){

 var staid = window.document.getElementById('staid');

 document.getElementById('msg_paralisacao').style.display = 'none';

 if ( id == 2  ){

 if (document.selection){
 document.getElementById('msg_paralisacao').style.display = 'block';
 tr_tplid.style.display  = 'block';
 tr_hprobs.style.display = 'block';
 tr1.style.display = 'block';
 tr2.style.display = 'block';
 tr3.style.display = 'block';
 //tr4.style.display = 'block';
 //tr5.style.display = 'block';
 tr6.style.display = 'block';
 tr7.style.display = 'block';
 tr8.style.display = 'block';
 tr9.style.display = 'block';
 tr10.style.display = 'block';
 //tr11.style.display = 'block';
 //tr12.style.display = 'block';
 }else{
 document.getElementById('msg_paralisacao').style.display = 'table-row';
 tr_tplid.style.display  = 'table-row';
 tr_hprobs.style.display = 'table-row';
 tr1.style.display = 'table-row';
 tr2.style.display = 'table-row';
 tr3.style.display = 'table-row';
 //tr4.style.display = 'table-row';
 //tr5.style.display = 'table-row';
 tr6.style.display = 'table-row';
 tr7.style.display = 'table-row';
 tr8.style.display = 'table-row';
 tr9.style.display = 'table-row';
 tr10.style.display = 'table-row';
 //tr11.style.display = 'table-row';
 //tr12.style.display = 'table-row';
 }

 }else if ( id == 4 ){

 if (document.selection){
 tr_elaboracao.style.display  = 'block';
 tr_elaboracao1.style.display = 'block';
 tr_elaboracao2.style.display = 'block';

 }else{
 tr_elaboracao.style.display  = 'table-row';
 tr_elaboracao1.style.display = 'table-row';
 tr_elaboracao2.style.display = 'table-row';

 }

 tr1.style.display = 'none';
 tr2.style.display = 'none';
 tr3.style.display = 'none';
 //tr4.style.display = 'none';
 //tr5.style.display = 'none';
 tr6.style.display = 'none';
 tr7.style.display = 'none';
 tr8.style.display = 'none';
 tr9.style.display = 'none';
 tr10.style.display = 'none';
 //tr11.style.display = '';
 //tr12.style.display = 'none';

 }else if ( id == 5 ){

 tr_tplid.style.display 	     = 'none';
 tr_hprobs.style.display      = 'none';
 tr_elaboracao.style.display  = 'none';
 tr_elaboracao1.style.display = 'none';
 tr_elaboracao2.style.display = 'none';
 //tr12.style.display = 'none';

 tr1.style.display    = 'none';
 tr2.style.display    = 'none';
 tr3.style.display    = 'none';
 //tr4.style.display  = 'none';
 //tr5.style.display  = 'none';
 tr6.style.display    = 'none';
 tr7.style.display    = 'none';
 tr8.style.display    = 'none';
 tr9.style.display    = 'none';
 tr10.style.display   = 'none';
 //tr11.style.display   = '';
 //tr12.style.display = 'none';

 document.formulario.obrlincambiental.value  = null;
 document.formulario.obraprovpatrhist.value  = null;
 document.formulario.obrdtprevprojetos.value = null;
 document.formulario.tplid.value  = null;
 document.formulario.hprobs1.value = null;
 tr1.style.display = 'none';
 tr2.style.display = 'none';
 tr3.style.display = 'none';
 //tr4.style.display = 'none';
 //tr5.style.display = 'none';
 tr6.style.display = 'none';
 tr7.style.display = 'none';
 tr8.style.display = 'none';
 tr9.style.display = 'none';
 tr10.style.display = 'none';
 //tr11.style.display = '';
 //tr12.style.display = 'none';

 }else if (id == 99){

 tr1.style.display = 'none';
 tr2.style.display = 'none';
 tr3.style.display = 'none';
 //tr4.style.display = 'none';
 //tr5.style.display = 'none';
 tr6.style.display = 'none';
 tr7.style.display = 'none';
 tr8.style.display = 'none';
 tr9.style.display = 'none';
 tr10.style.display = 'none';
 //tr11.style.display = 'none';
 //tr12.style.display = 'none';

 }else{

 tr_tplid.style.display 	     = 'none';
 tr_hprobs.style.display      = 'none';
 tr_elaboracao.style.display  = 'none';
 tr_elaboracao1.style.display = 'none';
 tr_elaboracao2.style.display = 'none';
 document.formulario.obrlincambiental.value  = null;
 document.formulario.obraprovpatrhist.value  = null;
 document.formulario.obrdtprevprojetos.value = null;
 document.formulario.tplid.value  = null;
 document.formulario.hprobs1.value = null;

 if (document.selection){
 tr1.style.display = 'block';
 tr2.style.display = 'block';
 tr3.style.display = 'block';
 //tr4.style.display = 'block';
 //tr5.style.display = 'block';
 tr6.style.display = 'block';
 tr7.style.display = 'block';
 tr8.style.display = 'block';
 tr9.style.display = 'block';
 tr10.style.display = 'block';
 //tr11.style.display = 'block';
 //tr12.style.display = 'block';
 }else{
 tr1.style.display = 'table-row';
 tr2.style.display = 'table-row';
 tr3.style.display = 'table-row';
 //tr4.style.display = 'table-row';
 //tr5.style.display = 'table-row';
 tr6.style.display = 'table-row';
 tr7.style.display = 'table-row';
 tr8.style.display = 'table-row';
 tr9.style.display = 'table-row';
 tr10.style.display = 'table-row';
 //tr11.style.display = 'table-row';
 //tr12.style.display = 'table-row';
 }

 }

 }
 */
/*
 function enviaFormulario( superuser, imagem ){

 var staid = window.document.getElementById('staid');

 //	if( stoid.value == 1 ){
 //
 //		var obrcustocontrato = document.getElementById('obrcustocontrato').value;
 //		var totalvalor 	     = document.getElementById('totalvalor').value;
 //
 //		if( Math.round(totalvalor) < Math.round(obrcustocontrato && ( superuser == 1 ) ) ){
 //			alert("Aqui Para inserir uma Vistoria com Situação da Obra Em Execução é necessário preencher o Cronograma Físico-Financeiro!");
 //			return false;
 //		}
 //
 //	}

 if( staid.value == 2 ){

 if(document.getElementById('tplid').value == "") {
 alert("É obrigatório selecionar o tipo de paralisação");
 return false;
 }

 }

 if(!$('[name=supobs]').val()){
 alert("Para cadastrar um acompanhamento, é necessário preencher o Relatório Técnico!");
 return false;
 }


 var num = $('#fotos_supervisao li');

 if( num.size() <= 0 ){
 if (confirm("Deseja inserir ao menos uma foto antes de salvar?")){
 ImageComponent(imagem);
 return false;
 }
 //alert("Para cadastrar um acompanhamento, é necessário anexar ao menos uma foto!");
 //return false;
 }

 var arrFotosSupervisao = $( "#fotos_supervisao").sortable( "serialize");
 $( "#hdn_fotos_supervisao").val(arrFotosSupervisao);

 if ( $("#fotos_galeria")[0] ){
 var arrFotosGaleria    = $( "#fotos_galeria").sortable( "serialize");
 $( "#hdn_fotos_galeria").val(arrFotosGaleria);
 }

 if (validaVistoria("formulario", superuser) && validasituacao(staid.value, superuser)){
 document.getElementById("formulario").submit();
 document.getElementById("salva_vistoria").disabled = "disabled";
 }

 }
 */

function validaVistoria(formu, superuser) {
    var form = document.getElementById(formu);
    var numelements = form.elements.length;

    var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
    var validacao = true;

    var dataObra = document.getElementById('supdatafimobra');
    if (dataObra.value == '') {
        mensagem += 'Data Prevista de Conclusão da Objeto \n';
        validacao = false;
    }

    var vistoriador = document.formulario.entidvistoriador;
    if (vistoriador.value == '') {
        mensagem += 'Vistoriador \n';
        validacao = false;
    }

//	if( stpid.value == '' ){
//		mensagem += 'Situação atual \n';
//		validacao = false;
//	}

    var supobs = document.formulario.supobs;

    if (supobs.value.length == 0 && ( superuser == 1 )) {
        mensagem += 'Observação do Acompanhamento \n';
        validacao = false;
    }

    if (supobs.value.length > 5000) {
        alert('O Campo Observação do Acompanhamento deve ter no máximo 5000 caracteres!');
        return false;
    }

// if (document.formulario.tpsid.value == ''){
// mensagem += 'Tipo da Vistoria \n';
// validacao = false;
// }


    var supdata = document.formulario.supdata.value;
    if (supdata == '') {
        mensagem += 'Data do Acompanhamento \n';
        validacao = false;
    }

    var staid = document.formulario.staid.value;
    if (staid == '') {
        mensagem += 'Estágio \n';
        validacao = false;
    }

    if (staid == 2) {

        if (typeof document.formulario.tplid != "undefined") {
            var tplid = document.formulario.tplid.value;
            var staid = document.formulario.staid.value;
            if (tplid == 1 && staid ==2) {
                stpid = document.formulario.stpid.value;
                if (stpid == '') {
                    mensagem += 'Subtipo de paralização \n';
                    validacao = false;
                }
            }
        }
        if (typeof document.formulario.stpid != "undefined") {
            var stpid = document.formulario.stpid.value;
            var hprobs1 = document.formulario.hprobs1.value;
            //Se Subtipo da paralização for igual a outros é obrigatório preenchimento do campo
            if (stpid == 2 && hprobs1 == '') {
                mensagem += 'AÇÕES ADOTADAS PARA RESOLUÇÃO DO EMBARGO \n';
                validacao = false;
            }
        }

        var v = 0;

        var tplid = document.formulario.tplid.value;
        //if ( tplid == '' && ( superuser == 1 ) ){
        if (tplid == '') {
            mensagem += 'Tipo da Paralisação \n';
            validacao = false;
        }

        var hprobs = document.formulario.hprobs1.value;
        if (hprobs == '' && ( superuser == 1 )) {
            v = 1;
            mensagem += 'Observação da Paralisação111 \n';
            validacao = false;
        }
        if (hprobs == '' && v==0) {
            if(tplid == 2 || tplid == 3) {
                mensagem += 'Medidas Administrativas e Judiciais Adotadas \n';
            }
            if(tplid == 1){
                mensagem += 'AÇÕES ADOTADAS PARA RESOLUÇÃO DO EMBARGO \n';
            }
            validacao = false;
            if(tplid > 4){
                validacao = true;
            }
        }

    }

    if (staid == 4) {
        var obrdtprevprojetos = document.formulario.obrdtprevprojetos.value;
        //if ( obrdtprevprojetos == '' || ( superuser == 1 ) ){
        /*Verifica se o campo "Previsão de entrega do(s) projeto(s)" está vazio. Alteração feita dia 16/12/2010 as 12:22 H.*/
        if (obrdtprevprojetos == '') {
            mensagem += 'Previsão de entrega do(s) projeto(s)\n';
            validacao = false;
        }
    }

//	if ( staid ){
//		if ( staid != 4 && staid != 5 && ( superuser == 1 ) ){
    /*
     var supprojespecificacoes = document.getElementsByName("supprojespecificacoes");
     if (supprojespecificacoes.checked == false){
     mensagem += 'Projeto/Especificações \n';
     validacao = false;
     }

     var supplacaobra = document.formulario.supplacaobra;
     if (supplacaobra.checked == false){
     mensagem += 'Placa da Obra \n';
     validacao = false;
     }
     */
    /*
     *
     * var supplacalocalterreno =
     * document.formulario.supplacalocalterreno; if
     * (supplacalocalterreno.checked == false){ mensagem += 'Placa
     * Indicativa do Programa/Localização do Terreno \n'; validacao =
     * false; }
     *
     */
    /*
     var qlbid = document.formulario.qlbid.value;
     if (qlbid == ''){
     mensagem += 'Qualidade da Execução da Obra \n';
     validacao = false;
     }

     var dcnid = document.formulario.dcnid.value;
     if (dcnid == ''){
     mensagem += 'Desempenho da Construtora \n';
     validacao = false;
     }
     */
//		}
//	}

    if (document.formulario.supdata.value != "") {
        if (!validaData(document.formulario.supdata)) {
            alert("A data informada é inválida");
            document.formulario.supdata.focus();
            return false;
        }
    }

    if (!validacao) {
        alert(mensagem);
    }

    return validacao;
}

function alteraValor(id, percObra, valor) {

    supervisao = document.getElementById("supvlrinfsuperivisor_" + id);
    item_exec_sobra = document.getElementById("percexecsobreobra_" + id);
    perc_real_obra = document.getElementById("percrealobra_" + id);

    if (supervisao.value != "") {

        percObra = Number(percObra) / 100;
        num = (Number(supervisao.value.replace(",", ".")) * percObra);
        perc_real_obra.value = num;
        item_exec_sobra.value = num.toFixed(2).toString().replace(".", ",");

    } else {

        if (Number(valor.replace(",", ".")) == 0) {
            num = Number((supervisao.value.replace(",", ".") * percObra.toString().replace(",", ".")) / 100);
            item_exec_sobra.value = num.toFixed(2).toString().replace(".", ",");
        } else {
            num = Number((supervisao.value.replace(",", ".") * percObra.toString().replace(",", ".")) / 100);
            item_exec_sobra.value = num.toFixed(2).toString().replace(".", ",");
        }

    }
}

function obras_verificaPercentual(id) {

    supervisao = document.getElementById("supvlrinfsuperivisor_" + id);
    supervisao.value = supervisao.value.replace(",", ".");
    supervisao.value = mascaraglobal('###,##', Number(supervisao.value).toFixed(2));
    supervisao.value = supervisao.value.replace(",", ".");


    if (supervisao.value > 100.00) {
        supervisao.value = supervisao.value.replace(".", ",")
        alert('O valor do campo "(%) da Supervisão" não pode ser maior do que 100.');
        supervisao.value = valor_antigo[id].toFixed(2);
        ;
    }


    if (supervisao.value == '') {
        supervisao.value = valor_antigo[id].toFixed(2);
    }

    supervisao.value = supervisao.value.replace(".", ",");
}

function obras_calculaTotalVistoria() {

    var x = document.getElementById("formulario");

    var valor = 0;
    var soma_s = 0;

    for (var i = 0; i < x.length; i++) {

        // Soma dos valores do percentual executado sobre a obra.
//		if ( x.elements[i].id.search(/percexecsobreobra_/) >= 0 ){
        if (x.elements[i].id.search(/percrealobra_/) >= 0) {
            valor = Number(x.elements[i].value.replace(",", "."));
            soma_s += valor;
        }

    }

    // Atualiza o valor do percentual sobre a obra.
    //document.getElementById("sobreobra").innerHTML  = soma_s.toFixed(2).toString().replace(".",",");
    document.getElementById("sobreobra").innerHTML = "<input type='text' size='10' readonly='Yes' name='percsupatual' value='" + soma_s.toFixed(2).toString().replace(".", ",") + "' style='text-align:right;'>";
}

function repositorioGaleria(idDiv, id, src) {

    if (document.getElementById("galeria_" + id).checked == true) {

        var img = "<img id='" + id + "' src='" + src + "' style='margin: 0px;opacity: 1' class='imageBox_theImage' onclick='javascript:window.open(\"" + src + "\",\"imagem\",\"width=850,height=600,resizable=yes\")'/>";

        var hidden = "<input type='hidden' value='" + id + "' id='galeria_" + id + "' name='galeria[" + id + "]'/>";

        document.getElementById(idDiv + "Galeria").innerHTML = img + hidden;

    } else {
        document.getElementById(idDiv + "Galeria").innerHTML = "";
    }

}

function obrEnviaFotosGaleria() {

    var campos = document.getElementsByTagName("input");

    for (i = 0; i < campos.length; i++) {

        if (campos[i].type == "hidden") {

            foto = campos[i].id.substr(0, 8);

            if (foto == "galeria_") {
                arrFotosGaleria.push(campos[i].value);
            }
        }
    }

    var url = caminho_atual + '?modulo=principal/inserir_vistoria&acao=A&subacao=galeria&AJAX=1';
    var parametros = "imagens=" + arrFotosGaleria;

    var myAjax = new Ajax.Request(
        url,
        {
            method: 'post',
            parameters: parametros,
            asynchronous: false,
            onComplete: function (resp) {
                alert(resp.responseText);
            }

        });
}

function validaDataVistoria() {

//	var data = document.getElementById('supdata').value;
    var data = '';
    var text_alert = '';
    var campo = '';

    // Controle data da Vistoria Instituição
    if (document.getElementById('supdata') !== null) {
        data = document.getElementById('supdata').value;
        text_alert = 'Data do Acompanhamento não pode ser mais que ';
        campo = document.getElementById('supdata');
    }
    // Controle data da Vistoria MEC
    else if (document.getElementById('sfndedtsupervisao') !== null) {
        data = document.getElementById('sfndedtsupervisao').value;
        text_alert = 'Data da Supervisão MEC não pode ser mais que ';
        campo = document.getElementById('sfndedtsupervisao');
    }
    // Controle data da Vistoria Empresa
    else if (document.getElementById('suedtsupervisao') !== null) {
        data = document.getElementById('suedtsupervisao').value;
        text_alert = 'Data da Supervisão Empresa não pode ser mais que ';
        campo = document.getElementById('suedtsupervisao');
    }
//        // Controle data da Vistoria MI
//        else if(document.getElementById('') !== null ){
//            data = document.getElementById('').value;
//            text_alert = 'Data da Supervisão MI não pode ser mais que ';
//            campo = document.getElementById('');
//        }


    var diav = data.substring(0, 2);
    var mesv = data.substring(3, 5);
    var anov = data.substring(6, 10);
    var dataV = new Date(mesv + '/' + diav + '/' + anov);

    var currentTime = new Date();

    var dia = currentTime.getDate();
    var mes = currentTime.getMonth() + 1;
    var ano = currentTime.getFullYear();
    var hoje = new Date(mes + '/' + dia + '/' + ano);

    if (dataV > hoje) {
        alert(text_alert + dia + '/' + mes + '/' + ano);
        campo.value = '';
        campo.focus();
    }

};