<?php
if ($_POST['requisicao'] == 'ajax_check') {
    // Item que est� requisitando
    $itemCheck = $_POST['itenpad'];

    // Caso tenha pesquisado outra pad, � necess�rio esvaziar os arrays das marcadas e modificar o valor da pad atual
    if ($_SESSION['PAD_ATUAL'] != $_POST['pad_atual']) {
        // Atualiza a pad atual
        $_SESSION['PAD_ATUAL'] = $_POST['pad_atual'];
        // retira os itens da outra pad
        unset($_SESSION['ITENS_MARCADOS']);
    }

    //caso seja ele incrementa array de marcados 
    if ($_POST['check']) {
        $_SESSION['ITENS_MARCADOS'][$itemCheck] = $itemCheck;
    } else {
        unset($_SESSION['ITENS_MARCADOS'][$itemCheck]);
    }

    $_SESSION['PAD_ATUAL'] = $_POST['pad_atual'];
    die();
}

$isXls = $_REQUEST['isXls'];
if (!$isXls) {
    ?>
    <link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
    <?php
}

if ($isXls) {
    $file_name = "campanha_resumido.xls";
    header("Pragma: public");
    header("Expires: 0");
    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
    header("Content-Type: application/force-download");
    header("Content-Type: application/octet-stream");
    header("Content-Type: application/download");
    header("Content-Disposition: attachment;filename=" . $file_name);
    header("Content-Transfer-Encoding: binary ");
}

if ($_REQUEST['tiporelatorio'] == 'html') {
    echo listaCampanha($_REQUEST, $_REQUEST['tiporelatorio']);
    die();
}
 
// incluindo as classes necessárias
//$oItemPad = new ItemPad();
//$oPad = new Pad();
////$oHistorico = new HistoricoDespachoMemorando();
//$oCampanha = new Campanha();
$oFornecedor = new Fornecedor();

if (!$isXls) {
//$dadosUltimoDespacho = $oHistorico->carrega_ultimo_despacho();
// monta cabeçalho do sistema
    include_once APPRAIZ . 'includes/cabecalho.inc';
    print '<br/>';

    $arMnuid = array();
    $db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
    monta_titulo('Gastos por Campanha Resumido', '');

    extract($_POST);
    ?>

    <link type="text/css" rel="stylesheet" href="./css/default.css" />
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

    <script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
    <script language='javascript' type='text/javascript' src='./js/default.js'></script>
    <script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
    <script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
    <script language='javascript' type='text/javascript' src='../includes/tinymce/tiny_mce.js'></script>

    <script language='javascript' type='text/javascript'>
        /**
         * Aciona o filtro
         * @name pesquisar
         * @param requisicao - Requisição que será executada
         * @return void
         */
        function pesquisar(requisicao) {
            $('btnPesquisar').disable();
            if (validaFormularioPesquisa()) {
                $('requisicao').setValue(requisicao);
                $('formularioPesquisa').submit();
            }
        }

        /**
         * Valida os campos obrigatórios
         * @name validaFormularioPesquisa
         * @return bool
         */
        function validaFormularioPesquisa() {

            if ($('padnumero').getValue() == '') {
                var msg = decodeURIComponent(escape('O campo "Nº da PAD" é obrigatório!'));
                alert(msg);
                $('padnumero').focus();
                $('btnPesquisar').enable();
                return false;
            }

            return true;
        }

        /**
         * Abre a tela do relatório
         * @name imprimir
         * @return void
         */
        function imprimir(isXls)
        {
            $('btnImprimir').disable();
            $('btnImprimirXls').disable();
            $('btnPesquisar').disable();

            //pega os valores das checkbox que estiverem checked
            //e seta nos campos hidden
            pegaValores();

            if (validaFormularioMemorando()) {

                $('btnImprimir').enable();
                $('btnImprimirXls').enable();
                $('btnPesquisar').enable();

                var dataemissao = $F('dataemissao');
                var nummemorando = $F('nummemorando');
                var objeto = $F('objeto');


                var el = $('fiscal').form;
                var fiscal = '';
                var checked = $(el).getInputs('radio', 'fiscal').find(
                        function(re) {
                            return re.checked;
                        }
                );
                if (checked) {
                    var fiscal = $F(checked);
                }

                var el = $('chefe').form;
                var chefe = '';
                var checked = $(el).getInputs('radio', 'chefe').find(
                        function(re) {
                            return re.checked;
                        }
                );
                if (checked) {
                    var chefe = $F(checked);
                }

                var obs = $F('obs');

                var ipaids = '';

                for (var x = 1; x <= $F('totItens'); x++) {
                    field = 'item_' + x;
                    if ($F(field) != '') {
                        ipaids += $F(field) + ',';
                    }
                }

                var padnumero = $F('padnumero');

                tinyMCE.triggerSave();
                var despacho = $F('despacho');
                despacho = encodeURIComponent(despacho);

                if (isXls)
                {
                    AbrirPopUp('?modulo=relatorio/memorando/visualizarMemorando&acao=A&ipaids=' + ipaids + '&dataemissao=' + dataemissao + '&nummemorando=' + nummemorando + '&objeto=' + objeto + '&fiscal=' + fiscal + '&chefe=' + chefe + '&obs=' + obs + '&padnumero=' + padnumero + '&despacho=' + despacho + '&isXls=1', 'visualizaMemorando', 'scrollbars=yes,width=800,height=570');
                }
                else
                {
                    AbrirPopUp('?modulo=relatorio/memorando/visualizarMemorando&acao=A&ipaids=' + ipaids + '&dataemissao=' + dataemissao + '&nummemorando=' + nummemorando + '&objeto=' + objeto + '&fiscal=' + fiscal + '&chefe=' + chefe + '&obs=' + obs + '&padnumero=' + padnumero + '&despacho=' + despacho + '&isXls=0', 'visualizaMemorando', 'scrollbars=yes,width=800,height=570');
                }
            }
        }

        /**
         * Valida o filtro do relatório
         * @name validaFormularioMemorando
         * @return bool
         */
        function validaFormularioMemorando() {

            if ($('dataemissao').getValue() == '') {
                var msg = decodeURIComponent(escape('O campo "Data de Emissão" é obrigatório!'));
                alert(msg);
                $('dataemissao').focus();
                $('btnImprimir').enable();
                $('btnImprimirXls').enable();
                $('btnPesquisar').enable();
                return false;
            }
            else {
                var data = new Data();
                if (!data.comparaData($F('dataemissao'), '01/01/2000', '>=')) {
                    var msg = decodeURIComponent(escape('A data informada é inválida'));
                    alert(msg);
                    $('dataemissao').focus();
                    $('btnImprimir').enable();
                    $('btnImprimirXls').enable();
                    $('btnPesquisar').enable();
                    return false;
                }

            }

            if ($('nummemorando').getValue() == '') {
                var msg = decodeURIComponent(escape('O campo "Nº Memorando" é obrigatório!'));
                alert(msg);
                $('nummemorando').focus();
                $('btnImprimir').enable();
                $('btnImprimirXls').enable();
                $('btnPesquisar').enable();
                return false;
            }

            if ($('objeto').getValue() == '') {
                var msg = decodeURIComponent(escape('O campo "Objeto" é obrigatório!'));
                alert(msg);
                $('objeto').focus();
                $('btnImprimir').enable();
                $('btnImprimirXls').enable();
                $('btnPesquisar').enable();
                return false;
            }


            var fiscal = '';
            for (var i = 0; i < document.formularioMemorando.fiscal.length; i++) {
                if (document.formularioMemorando.fiscal[i].checked) {
                    fiscal = document.formularioMemorando.fiscal[i].value;
                }
            }
            if (fiscal == '') {
                var msg = decodeURIComponent(escape('O campo "Fiscal do Contrato" é obrigatório!'));
                alert(msg);
                $('btnImprimir').enable();
                $('btnImprimirXls').enable();
                $('btnPesquisar').enable();
                return false;
            }

            var chefe = '';
            for (var i = 0; i < document.formularioMemorando.chefe.length; i++) {
                if (document.formularioMemorando.chefe[i].checked) {
                    chefe = document.formularioMemorando.chefe[i].value;
                }
            }
            if (chefe == '') {
                var msg = decodeURIComponent(escape('O campo "Chefe da ACS" é obrigatório!'));
                alert(msg);
                $('btnImprimir').enable();
                $('btnImprimirXls').enable();
                $('btnPesquisar').enable();
                return false;
            }

            tinyMCE.triggerSave();
            if ($F('despacho') == '') {
                var msg = decodeURIComponent(escape('O campo "Despacho" é obrigatório!'));
                alert(msg);
                $('despacho').focus();
                $('btnImprimir').enable();
                $('btnImprimirXls').enable();
                $('btnPesquisar').enable();
                return false;
            }

            var item = '';
            item = $$('input[name=checkitempad]').filter(function(el) {
                return $F(el);
            }).size();

            if (item < 1) {
                alert('Favor selecionar algum item!');
                $('btnImprimir').enable();
                $('btnImprimirXls').enable();
                $('btnPesquisar').enable();
                return false;
            }

            return true;
        }

        /**
         * Pega os checkbox marcados e seta os campos hidden correspondentes
         * @name pegaValores
         * @return void
         */
        function pegaValores() {
            var field = '';
            var setou = '';
            var totItens = $F('totItens');
            var form = $('formularioItens');
            var i = form.getElements('checkbox');

            //limpa os campos hidden
            for (var x = 1; x <= totItens; x++) {
                field = 'item_' + x;
                $(field).clear();
            }

            //seta os valores
            i.each(function(item) {
                if (item.checked) {
                    setou = '';
                    for (var x = 1; x <= totItens; x++) {
                        field = 'item_' + x;
                        if ($F(field) == '' && setou != 'S') {
                            $(field).setValue(item.value);
                            setou = 'S';
                        }
                    }
                }
            });
        }


        /**
         * Habilita/desabilita o botão imprimir ao selecionar/deselecionar os itens da pad
         * @name habilitaImpressao
         * @return void
         */
        function habilitaImpressao(value, check, npad) {

            // Caso tenha sido marcado
            if (check)
            {
                ischeck = 1;
            }
            else
            {
                ischeck = 0;
            }
            // Submete via ajax para poder registrar os que foram marcados qundo houver a troca de p�ginas
            new Ajax.Updater('check', "publicidade.php?modulo=relatorio/memorando/emitirMemorando&acao=A", {
                method: 'post',
                asynchronous: false,
                parameters: {'requisicao': 'ajax_check', 'itenpad': value, 'check': ischeck, 'pad_atual': npad},
            });

            var item = '';
            item = $$('input[name=checkitempad]').filter(function(el) {
                return $F(el);
            }).size();

            if (item >= 1) {
                $('btnImprimir').enable();
                $('btnImprimirXls').enable();
            }
            else if (item < 1) {
                $('btnImprimir').disable();
                $('btnImprimirXls').disable();
            }
        }

        tinyMCE.init({
            mode: "exact",
            elements: "despacho",
            theme: "advanced",
            plugins: "noneditable,visualchars,fullscreen,searchreplace,-ListaCampo",
            language: 'pt',
            // Theme options
            theme_advanced_buttons1: "bold,italic,underline,strikethrough,|,search,replace,|,bullist,numlist,|,justifyleft,justifycenter,justifyright,justifyfull,|,ltr,rtl,|,listaCampoFixo",
            theme_advanced_buttons2: "visualchars,charmap,styleselect,formatselect,fontselect,fontsizeselect,|,forecolor,|,fullscreen,|,listaCampoDinamico",
            theme_advanced_buttons3: "",
            theme_advanced_toolbar_location: "top",
            theme_advanced_toolbar_align: "left",
        });

        function exibirAlertaNaoTemItens()
        {
            alert('A PAD pesquisada n�o est� como Faturada.');
        }
    </script>

    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
    <script type="text/javascript">
        jQuery.noConflict();

        var toogleCheckboxes = function() {

            if (jQuery("#action_toogleCheckboxes").is(':checked')) {

                jQuery('table.lista_itens_pad > tbody > tr').each(function(index) {
                    var linha = jQuery(this);
                    var objCheck = linha.find('td:eq(0)').find('input[type=checkbox]');
                    var vlCheck = objCheck.val();
                    var fnClick = objCheck.attr('onclick');

                    fnClick = fnClick.replace('javascript:habilitaImpressao( ', '');
                    fnClick = fnClick.replace('javascript:habilitaImpressao(', '');
                    fnClick = fnClick.replace(' );', '');
                    fnClick = fnClick.replace(');', '');
                    fnClick = fnClick.trim();
                    fnClick = fnClick.split(',');
                    var npad = fnClick[2].trim();

                    objCheck.attr('checked', 'checked');

                    habilitaImpressao(vlCheck, true, npad);
                });
            } else {
                jQuery('table.lista_itens_pad > tbody > tr').each(function(index) {
                    var linha = jQuery(this);
                    var objCheck = linha.find('td:eq(0)').find('input[type=checkbox]');
                    var vlCheck = objCheck.val();
                    var fnClick = objCheck.attr('onclick');

                    fnClick = fnClick.replace('javascript:habilitaImpressao( ', '');
                    fnClick = fnClick.replace('javascript:habilitaImpressao(', '');
                    fnClick = fnClick.replace(' );', '');
                    fnClick = fnClick.replace(');', '');

                    fnClick = fnClick.trim();
                    fnClick = fnClick.split(',');
                    var npad = fnClick[2].trim();

                    objCheck.removeAttr('checked');

                    habilitaImpressao(vlCheck, false, npad);
                });
            }
        };

        jQuery(document).ready(function() {
            jQuery('#gerar_xls').click(function() {
                jQuery('#isXls').val(1);
                jQuery('#formularioPesquisa').submit();
            });
            if (jQuery('table.lista_itens_pad').length > 0) {
                var allCheck = true;
                jQuery('table.lista_itens_pad > tbody > tr').each(function(index) {
                    var linha = jQuery(this);
                    var objCheck = linha.find('td:eq(0)').find('input[type=checkbox]');
                    if (!objCheck.is(':checked')) {
                        allCheck = false;
                    }
                });

                if (allCheck) {
                    jQuery("#action_toogleCheckboxes").attr('checked', 'checked');
                } else {
                    jQuery("#action_toogleCheckboxes").removeAttr('checked');
                }
                
            }
                jQuery('#relatorio').click(function() {
                jQuery('#requisicao').val('listar');
                jQuery('#isXls').val('');
                forid = jQuery('#forid').val();
                ano = jQuery('#ano').val();
                datainicial = jQuery('#datainicial').val();
                datafinal = jQuery('#datafinal').val();
                jQuery("#formularioPesquisa").submit();
                gerarRelatorio('html', forid, ano, datainicial, datafinal);
            });
            
               //gerando relat�rio via pop, submetendo a mesma tela.
        function gerarRelatorio(tipo, forid, ano, datainicial, datafinal) {
            var url = "publicidade.php?modulo=relatorio/gastos/campanha&acao=A";
            var params = "&tiporelatorio=" + tipo + "&forid=" + forid + "&ano=" + ano + "&datainicial=" + datainicial + "&datafinal=" + datafinal;
            window.open(url + params, 'relatorio', 'width=980,height=660,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        }

        });

    </script>

    <form name="formularioPesquisa" id="formularioPesquisa" method="post">
        <input id="isXls" name="isXls" type="hidden">

        <table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">

            <tr>
                <td class="SubtituloDireita"> Ag&ecirc;ncia: </td>
                <td class="campo"><?php $oFornecedor->monta_combo_fornecedor($forid, 'N', 1); ?></td>
            </tr>
            <tr>
                <td class="SubtituloDireita"> Exerc�cio: </td>
                <td class="campo">
                    <?php echo montaComboExRel(); ?>
                </td>

            </tr>
            <tr>
                <td class="SubtituloDireita"> Per&iacute;odo: </td>
                <td class="campo">
                    <?php echo campo_data2('datainicial', 'N', 'S', 'Informe a Data Inicial', 'N', '', '', $datainicial); ?> a 
                    <?php echo campo_data2('datafinal', 'N', 'S', 'Informe a Data Final', 'N', '', '', $datafinal); ?>
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> 
                </td>
            </tr>

            <tr class="buttons">
                <td colspan='2' align='center'>
                    <input type='hidden' name='requisicao' id='requisicao' />
                    <input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Pesquisar' onclick="javascript:formularioPesquisa.submit();">
                    <input type='button' class="botao"  value='Gerar Xls' id="gerar_xls">
                    <input type='button' class="botao" name='relatorio' id='relatorio' value='Imprimir' onclick="javascript:imprimir();">
                </td>
            </tr>
        </table>
    </form>

<?php } ?>
 <?php
    echo listaCampanha($_REQUEST, 'pagina');
    ?>