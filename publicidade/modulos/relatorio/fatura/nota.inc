<?php
if ($_POST['requisicao'] == 'ajax_check') {
    // Item que est· requisitando
    $itemCheck = $_POST['itenpad'];

    // Caso tenha pesquisado outra pad, È necess·rio esvaziar os arrays das marcadas e modificar o valor da pad atual
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



// incluindo as classes necess√°rias
$oItemPad = new ItemPad();
$oPad = new Pad();
//$oHistorico = new HistoricoDespachoMemorando();
$oCampanha = new Campanha();
$oFornecedor = new Fornecedor();

//$dadosUltimoDespacho = $oHistorico->carrega_ultimo_despacho();
// monta cabe√ßalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o t√≠tulo da tela
monta_titulo('Levantamento dos ServiÁos pelo N˙mero de Fatura', '');

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
     * @param requisicao - Requisi√ß√£o que ser√° executada
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
     * Valida os campos obrigat√≥rios
     * @name validaFormularioPesquisa
     * @return bool
     */
    function validaFormularioPesquisa() {

        if ($('padnumero').getValue() == '') {
            var msg = decodeURIComponent(escape('O campo "N¬∫ da PAD" √© obrigat√≥rio!'));
            alert(msg);
            $('padnumero').focus();
            $('btnPesquisar').enable();
            return false;
        }

        return true;
    }

    /**
     * Abre a tela do relat√≥rio
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
     * Valida o filtro do relat√≥rio
     * @name validaFormularioMemorando
     * @return bool
     */
    function validaFormularioMemorando() {

        if ($('dataemissao').getValue() == '') {
            var msg = decodeURIComponent(escape('O campo "Data de Emiss√£o" √© obrigat√≥rio!'));
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
                var msg = decodeURIComponent(escape('A data informada √© inv√°lida'));
                alert(msg);
                $('dataemissao').focus();
                $('btnImprimir').enable();
                $('btnImprimirXls').enable();
                $('btnPesquisar').enable();
                return false;
            }

        }

        if ($('nummemorando').getValue() == '') {
            var msg = decodeURIComponent(escape('O campo "N¬∫ Memorando" √© obrigat√≥rio!'));
            alert(msg);
            $('nummemorando').focus();
            $('btnImprimir').enable();
            $('btnImprimirXls').enable();
            $('btnPesquisar').enable();
            return false;
        }

        if ($('objeto').getValue() == '') {
            var msg = decodeURIComponent(escape('O campo "Objeto" √© obrigat√≥rio!'));
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
            var msg = decodeURIComponent(escape('O campo "Fiscal do Contrato" √© obrigat√≥rio!'));
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
            var msg = decodeURIComponent(escape('O campo "Chefe da ACS" √© obrigat√≥rio!'));
            alert(msg);
            $('btnImprimir').enable();
            $('btnImprimirXls').enable();
            $('btnPesquisar').enable();
            return false;
        }

        tinyMCE.triggerSave();
        if ($F('despacho') == '') {
            var msg = decodeURIComponent(escape('O campo "Despacho" √© obrigat√≥rio!'));
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
     * Habilita/desabilita o bot√£o imprimir ao selecionar/deselecionar os itens da pad
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
        // Submete via ajax para poder registrar os que foram marcados qundo houver a troca de p·ginas
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
        alert('A PAD pesquisada n„o est· como Faturada.');
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
    });

</script>

<?php 
 $array_tipos = array(
                    array('codigo' => '1', 'descricao' => 'Fornecedor'),
                    array('codigo' => 'agencia', 'descricao' => 'AgÍncia'));
 ?>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
    <table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">

        <tr>
            <td class="SubtituloDireita">N∫ da fatura</td>
            <td class="campo">
                <?=campo_texto('fatura','N','S','',13,8,'','','left','',0,'id="fatura"');?>
            </td>

        </tr>



        <tr>
            <td class="SubtituloDireita"> Tipo de fatura: </td>
            <td class="campo">
                <?php $sql = "select catdsc as descricao, catid as codigo from publicidade.categoria cat where catstatus = 'A'"; ?>
                <?= $db->monta_combo("tipo_fatura",$array_tipos , "S", "", "", "", "", "", "", "tipo_fatura", "", "") ?>
            </td>

        </tr>

       


        <tr class="buttons">
            <td colspan='2' align='center'>
                <input type='hidden' name='requisicao' id='requisicao' />
                <input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Pesquisar' onclick="javascript:formularioPesquisa.submit();">
            </td>
        </tr>
    </table>
</form>


<table class="tabela" style="width:100% !important;" align="center" >


    <?php
         if(!empty($_POST[tipo_fatura])){          
    
            $sql = "select CASE WHEN ite.ipastatus = 'A' THEN 'Aprovado' 
			      ELSE 
				  CASE WHEN ite.ipastatus = 'C' THEN 'Cancelado'
				      ELSE
					  CASE WHEN ite.ipastatus = 'F' THEN 'Faturado'
					      ELSE
						  CASE WHEN ite.ipastatus = 'P' THEN 'Pago'
						      ELSE
							  CASE WHEN ite.ipastatus = 'G' THEN 'Glosado'
							  END
					      END
					  END
			      END	
	      END AS situacao, ite.ipanumitempad ,* from publicidade.faturamentoitempad fip"
            . " join publicidade.itenspad ite on ite.ipaid =fip.ipaid ";
            
            
            
           if(!empty($_POST[tipo_fatura])){               
               
               if($_POST[tipo_fatura] == "agencia"){
                   $sql .= "where fipnumfaturaagencia = '".$_POST[fatura]."'";    
               }else{
                   $sql .= "where fipnumfaturafornecedor = '".$_POST[fatura]."'";    
               }
           
           }       
            $registros = $db->carregar($sql);
         }
            $total = 0;
            if (!empty($registros)) {
                ?>   
                <tr>
                    <td style="text-align: left;font-weight: bold;">N˙mero</td>
                    <td style="text-align: left;font-weight: bold;">Fatura Fornecedor</td>
                    <td style="text-align: left;font-weight: bold;">Fatura Agencia</td>
                    <td style="text-align: left;font-weight: bold;">DescriÁ„o</td>
     
                    <td style="text-align: left;font-weight: bold;">SituaÁ„o</td>
                    <td style="text-align: left;font-weight: bold;">Valor</td>
                </tr>
                <?php
                foreach ($registros as $value) {
                    $total += $value[fipvalortotal];
                    ?>
                    <tr>
                        <td style="text-align: left"><?php echo $value[ipanumitempad] ?></td>
                        <td style="text-align: left"><?php echo $value[fipnumfaturafornecedor] ?></td>
                        <td style="text-align: left"><?php echo $value[fipnumfaturaagencia] ?></td>
                        <td style="text-align: left"><?php echo $value[ipadsc] ?></td>
                    
                        <td style="text-align: left"><?php echo $value[situacao] ?></td>
                        <td style="text-align: left"><?php echo 'R$ ' . number_format($value[fipvalortotal], 2, ',', '.');?></td>
                    </tr>

                <?php
                }
                
                ?>
                    <tr><td colspan="5" style="text-align: right"></td><td style="background-color: #cccccc;text-align: left"><b>Total: <?php echo 'R$ ' . number_format($total, 2, ',', '.'); ?></b></td> </tr>
                <?php
            }       
        ?>        
</table>