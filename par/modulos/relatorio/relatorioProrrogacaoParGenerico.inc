<?php
# Inclui cabecalho
include APPRAIZ . 'includes/cabecalho.inc';

# Titulo
monta_titulo('Relatório de Prorrogação do PAR Genérico', '');
?>
<form id="formulario" name="formulario" method="post" action="">
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
        <tr>
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
        <tr>
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
            <td class="SubTituloDireita"><b>Processo:</b></td>
            <td>
                <?php
                echo campo_texto('prpnumeroprocesso', 'N', 'S', '[#]', 50, 50, '#####.######/####-##', '', 'left', '', 0, '', '', $_REQUEST['prpnumeroprocesso']);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Numero do Termo:</b></td>
            <td>
                <?php
                echo campo_texto('dopnumerodocumento', 'N', 'S', '[#]', 20, 8, '', '', 'left', '', 0, '', '', $_REQUEST['dopnumerodocumento']);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Vigência de Termos que estão vencendo em:</b></td>
            <td id="td_muncod">
                De: <?php echo campo_data2('dtvigenciainicio', 'N', 'S', '', 'S', '', '', $_REQUEST['dtvigenciainicio']); ?>
                Até: <?php echo campo_data2('dtvigenciafim', 'N', 'S', '', 'S', '', '', $_REQUEST['dtvigenciafim'] ); ?>
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0"></td>
            <td align="left" bgcolor="#c0c0c0">
                <input type="button" name="btnPesquisar" id="btnPesquisar" value="Pesquisar" />
                &nbsp;
                <input type="reset" id="btnCancel" name="btnCancel" value="Limpar Filtros" />
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
    $obProcessoParControle = new ProcessoParControle();
    $mixProcessoPar = $obProcessoParControle->relatorioProrrogacaoParGenerico($_REQUEST, TRUE);

    $cabecalho = array("Nº do Processo", "N° do Termo", "Descrição do Termo", "UF", "Município", "Valor do Termo", "Valor Empenhado", "Pagamento Solicitado", "Pagamento Efetivado", "Vigência", "Saldo Bancário(CC + CP + Fundo)", "QtdItem/ QtdContrato");
    if ($_REQUEST['acaoBotao'] == 'btnExcel') {#Excel
        ob_clean();
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=SIMEC_RelatFinanceiro" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=SIMEC_RelatFinanceiro" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");
        $db->monta_lista_tabulado($mixProcessoPar, $cabecalho, 1000000, 5, 'N', '100%');
        exit;
    } else { #Listagem
        array_unshift($cabecalho, '');
        $db->monta_lista($mixProcessoPar, $cabecalho, 25, 10, 'N', 'center', 'N');
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

    // Somente numeros
    $("input[name=dopnumerodocumento]").bind("keyup blur focus", function (e) {
        e.preventDefault();
        var expre = /[^0-9]/g;
        if ($(this).val().match(expre))
            $(this).val($(this).val().replace(expre, ''));
    });
});

/**
 * Abre janela popup para selecionar municipios
 * @returns VOID
 */
function abrirPopupListaMunicipio() {
    window.open('http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/combo_municipios_bandalarga.php?nomeCombo=listaMunicipio&nomeFormulario=formulario', 'municipios', 'width=400,height=400,scrollbars=1');
}

function carregaTermoMinuta(dopid){
    window.open('par.php?modulo=principal/visualizaTermoCompromisso&acao=A&dopid='+dopid,'visualizatermo','scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
}
</script>
