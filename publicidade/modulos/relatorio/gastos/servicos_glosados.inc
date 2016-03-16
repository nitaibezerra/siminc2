<?php
$isXls = $_REQUEST['isXls'];
if (!$isXls) {
    ?>
    <link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
    <?php
}

if ($isXls) {
    $file_name = "gastos_por_categoria.xls";
    header("Pragma: public");
    header("Expires: 0");
    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
    header("Content-Type: application/force-download");
    header("Content-Type: application/octet-stream");
    header("Content-Type: application/download");
    header("Content-Disposition: attachment;filename=" . $file_name);
    header("Content-Transfer-Encoding: binary ");
}

if ($_REQUEST['tiporelatorio']) {
    echo listaServicos($_REQUEST, 'html');
    die();
}    

// incluindo as classes necess√°rias
//$oItemPad = new ItemPad();
//$oPad = new Pad();
//$oHistorico = new HistoricoDespachoMemorando();
//$oCampanha = new Campanha();
$oFornecedor = new Fornecedor();

//$dadosUltimoDespacho = $oHistorico->carrega_ultimo_despacho();
// monta cabe√ßalho do sistema

if (!$isXls) {
    include_once APPRAIZ . 'includes/cabecalho.inc';
    print '<br/>';

    $arMnuid = array();
    $db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o t√≠tulo da tela
    monta_titulo('RelatÛrio ServiÁos Glosados', '');

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

        $('#formularioPesquisa #ano').chosen();
        $('#formularioPesquisa #forid_fornecedor').chosen();
        $('#formularioPesquisa #forid').chosen();
        /**
         * Aciona o filtro
         * @name pesquisar
         * @param requisicao - Requisi√ß√£o que ser√° executada
         * @return void
         */
        function pesquisar(requisicao) {
            $('#isXls').val("");
            $('#formularioPesquisa').submit();

        }




    </script>

    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

    <script>
        $(document).ready(function() {
            $('#gerar_xls').click(function() {
                $('#isXls').val(1);
                $('#formularioPesquisa').submit();
            });
            $('#relatorio').click(function() {
                $('#requisicao').val('listar');
                $('#isXls').val("");
                forid = $('#forid').val();
                ano = $('#ano').val();
                datainicial = $('#datainicial').val();
                datafinal = $('#datafinal').val();
                $("#formularioPesquisa").submit();
                gerarRelatorio('html', forid, ano, datainicial, datafinal);
            });
        });

        //gerando relatÛrio via pop, submetendo a mesma tela.
        function gerarRelatorio(tipo, forid, ano, datainicial, datafinal) {
            var url = "publicidade.php?modulo=relatorio/gastos/servicos_glosados&acao=A";
            var params = "&tiporelatorio=" + tipo + "&forid=" + forid + "&ano=" + ano + "&datainicial=" + datainicial + "&datafinal=" + datafinal;
            window.open(url + params, 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        }



    </script>
    <form name="formularioPesquisa" id="formularioPesquisa" method="post">
        <table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
            <input id="isXls" name="isXls" type="hidden">

            <tr>
                <td class="SubtituloDireita"> Ag&ecirc;ncia: </td>
                <td class="campo"><?php $oFornecedor->monta_combo_fornecedor($forid, 'N', 1); ?></td>
            </tr>




            <tr>
                <td class="SubtituloDireita"> ExercÌcio: </td>
                <td class="campo">
                   <?php echo montaComboExRel(); ?>
                </td>

            </tr>
            <tr>
                <td class="SubtituloDireita"> Per&iacute;odo: </td>
                <td class="campo">
                    <?php echo campo_data2('datainicial', 'N', 'S', 'Informe a Data Inicial', 'N', '', '', $datainicial); ?> a 
                    <?php echo campo_data2('datafinal', 'N', 'S', 'Informe a Data Final', 'N', '', '', $datafinal); ?>
                    <img border="0" title="Indica campo obrigat√≥rio." src="../imagens/obrig.gif"> 
                </td>
            </tr>



            <tr class="buttons">
                <td colspan='2' align='center'>
                    <input type='hidden' name='requisicao' id='requisicao' />
                    <input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Pesquisar' onclick="javascript:pesquisar();">
                    <input type='button' class="botao"  value='Gerar Xls' id="gerar_xls">
                    <input type='button' class="botao" name='relatorio' id='relatorio' value='Imprimir' >
                </td>
            </tr>
        </table>
    </form>
<?php }     
    echo listaServicos($_REQUEST, 'pagina');
    ?>     
