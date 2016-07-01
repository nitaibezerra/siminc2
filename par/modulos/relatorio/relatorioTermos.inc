<?php
ini_set("memory_limit", "3000M");
set_time_limit(0);
# Inclui cabecalho
include APPRAIZ . 'includes/cabecalho.inc';

# Titulo
monta_titulo('Relatório de Termos', '');
?>
<form id="formulario" name="formulario" method="post" action="">
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
        <tr>
            <td class="SubTituloDireita"><b>Processo:</b></td>
            <td>
                <?php
                echo campo_texto('processo', 'N', 'S', '[#]', 50, 50, '#####.######/####-##', '', 'left', '', 0, '', '', $_REQUEST['processo']);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Número do Termo:</b></td>
            <td>
                <?php
                echo campo_texto('numerotermo', 'N', 'S', '[#]', 20, 15, '', '', 'left', '', 0, '', '', $_REQUEST['numerotermo']);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Ano do Termo:</b></td>
            <td>
                <?php
                echo campo_texto('anotermo', 'N', 'S', '[#]', 20, 4, '', '', 'left', '', 0, '', '', $_REQUEST['anotermo']);
                ?>
            </td>
        </tr>
        <tr id="trUf">
            <td class="SubTituloDireita">UF:</td>
            <td>
                <?php
                $obEstadoControle = new EstadoControle();
                $strEstado = $obEstadoControle->carregarUf(TRUE);
                $arrUf = array();
                if (!empty($_REQUEST['estuf'][0])) {
                    foreach ($_REQUEST['estuf'] as $value) {
                        $arrUf[] = array('codigo' => $value, 'descricao' => $value);
                    }
                }
                combo_popup("estuf", $strEstado, "Lista de Estados", "400x400", 0, array(), "", "S", false, false, 5, 400, '', '', '', '', $arrUf);
                ?>
            </td>
        </tr>
        <tr id="trMunicipio">
            <td class="SubTituloDireita"><b>Municípios</b></td>
            <td>
                <div onmouseover="return escape('Municípios');">
                    <select
                        multiple="multiple"
                        size="5"
                        name="listaMunicipio[]"
                        id="listaMunicipio"
                        ondblclick="abrirPopupListaMunicipio();"
                        class="CampoEstilo link"
                        onkeydown="javascript:combo_popup_remove_selecionados(event, 'listaMunicipio');"
                        style="width:400px;" >
                            <?php
                            if (!empty($_REQUEST['listaMunicipio'][0])) {
                                $obMunicipioControle = new MunicipioControle();
                                foreach ($_REQUEST['listaMunicipio'] as $value) {
                                    $arrMunicipio = $obMunicipioControle->carregarMunicipioPorMuncod($value);
                                    echo "<option value=\"{$arrMunicipio['codigo']}\">{$arrMunicipio['descricao']}</option>";
                                }
                            } else {
                                echo "<option value=\"\">Duplo clique para selecionar da lista</option>";
                            }
                            ?>
                    </select>
                </div>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Tipo:</b></td>
            <td>
                <input type="radio" value='SUBACAO' name="tipo" <?php echo ($_REQUEST['tipo'] == 'SUBACAO') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Subação</label>
                <input type="radio" value='PAR' name="tipo" <?php echo ($_REQUEST['tipo'] == 'PAR') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Par</label>
                <input type="radio" value='PAC' name="tipo" <?php echo ($_REQUEST['tipo'] == 'PAC') ? "checked='checked'" : '' ?> class="link" >
                <label for="" class="link">Pac</label>
                <input type="radio" value='T' name="tipo" <?php echo ($_REQUEST['tipo'] == 'T' || empty($_REQUEST['tipo'])) ? "checked='checked'" : '' ?> class="link" >
                <label for="todos" class="link">Todos</label>
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0"></td>
            <td align="left" bgcolor="#c0c0c0">
                <input type="button" name="btnPesquisar" id="btnPesquisar" value="Pesquisar" />
                &nbsp;
                <button type="reset" name="btnCancel" id="btnCancel">Limpar Filtros</button>
                &nbsp;
                <button type="submit" name="btnExcel" id="btnExcel">Exportar Excel</button>
                <input type="hidden" name="acaoBotao" value="" />
            </td>
        </tr>
    </table>
</form>

<?php
# Filtros da pesquisa
if (isset($_REQUEST['acaoBotao'])) {
    #Instancia de PreObra
    $obDocumentoPar = new DocumentoParModelo();
    $mixDocumentoPar = $obDocumentoPar->relatorioTermoCompromisso($_REQUEST, TRUE);
//    $cabecalho = array( 'Tipo de Documento', 'Documento Original', 'Subação', 'Pedido Reprogramação', 'Pedido Reprogramação - Subação');
    $cabecalho = array('UF', 'Entidade', 'Esfera', 'Número do Termo', 'Ano do Termo', 'Número do Processo', 'Documento', 'Fim da Vigência', 'Saldo Bancário', 'Subação', 'Reprogramação - Prazo', 'Reprogramação - Subação', 'Tipo');
    if ($_REQUEST['acaoBotao'] == 'btnExcel') {#Excel
        ob_clean();
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=SIMEC_Relat" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=SIMEC_RelatPAR" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");
        $db->monta_lista_tabulado($mixDocumentoPar, $cabecalho, 1000000, 5, 'N', '100%');
        exit;
    } else { #Listagem
        $db->monta_lista($mixDocumentoPar, $cabecalho, 25, 10, 'N', 'center', 'N');
    }
}
?>

<link href="../includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.css" rel="stylesheet" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>
<script type="text/javascript">
    $(document).ready(function () {
        $('#btnPesquisar, #btnExcel').click(function () {
            var estuf = document.getElementById('estuf');
            selectAllOptions(estuf);
            var muncod = document.getElementById('listaMunicipio');
            selectAllOptions(muncod);
            $('input[name=acaoBotao]').val($(this).attr('id'));
            $('#formulario').submit();
        });
        
        // Limpar Filtros
         $('#btnCancel').click(function () {
            window.location = window.location;
        });

        // Somente numeros
        $("input[name=anotermo]").bind("keyup blur focus", function (e) {
            e.preventDefault();
            var expre = /[^0-9]/g;
            if ($(this).val().match(expre))
                $(this).val($(this).val().replace(expre, ''));
        });
    });
 
    function abrirPopupListaMunicipio() {
        window.open('http://<?= $_SERVER['SERVER_NAME'] ?>/cte/combo_municipios_bandalarga.php?nomeCombo=listaMunicipio&nomeFormulario=formulario', 'municipios', 'width=400,height=400,scrollbars=1');
    }
</script>