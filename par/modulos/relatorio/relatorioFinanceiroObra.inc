<?php
# Inclui cabecalho
include APPRAIZ . 'includes/cabecalho.inc';

# Titulo
monta_titulo('Relatório Financeiro de Obras', '');
?>
<form id="formulario" name="formulario" method="post" action="">
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
        <tr>
            <td class="SubTituloDireita"><b>Id Obra:</b></td>
            <td>
                <?php
                echo campo_texto('obrid', 'N', 'S', '[#]', 20, 8, '', '', 'left', '', 0, '', '', $_REQUEST['obrid']);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Preid:</b></td>
            <td>
                <?php
                echo campo_texto('preid', 'N', 'S', '[#]', 20, 8, '', '', 'left', '', 0, '', '', $_REQUEST['preid']);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Processo:</b></td>
            <td>
                <?php
                echo campo_texto('empnumeroprocesso', 'N', 'S', '[#]', 50, 50, '#####.######/####-##', '', 'left', '', 0, '', '', $_REQUEST['empnumeroprocesso']);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Nome da Obra:</b></td>
            <td>
                <?php
                echo campo_texto('predescricao', 'N', 'S', '', 100, 100, '', '', 'left', '', 0, '', '', $_REQUEST['predescricao']);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Tipo da Obra</b></td>
            <td>
                <?php
                $obPreTipoObraControle = new PreTipoObraControle();
                $arrPreTipoObra = $obPreTipoObraControle->carregarPreTipoObra();
                $db->monta_combo("ptoid", $arrPreTipoObra, 'S', 'Selecione...', '', '', '', '', '', 'tprid', '', $_REQUEST['ptoid'], null, null, ' link ');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Esfera:</b></td>
            <td>
                <input type="radio" value='M' name="preesfera" <?php echo ($_REQUEST['preesfera'] == 'M') ? "checked='checked'" : '' ?> class="link" >
                <label for="preesferaMunicipal" class="link">Municípal</label>
                <input type="radio" id="preesferaEstadual" value='E' name="preesfera" <?php echo ($_REQUEST['preesfera'] == 'E') ? "checked='checked'" : '' ?> class="link" >
                <label for="preesferaEstadual" class="link">Estadual</label>
                <input type="radio" id="todos" value='T' name="preesfera" <?php echo ($_REQUEST['preesfera'] == 'T' || empty($_REQUEST['preesfera'])) ? "checked='checked'" : '' ?> class="link" >
                <label for="todos" class="link">Todos</label>
            </td>
        </tr>
        <tr id="trUf" style="display:none">
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
        <tr id="trMunicipio" style="display:none">
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
                            }else{
                                echo "<option value=\"\">Duplo clique para selecionar da lista</option>";
                            }
                        ?>
                    </select>
                </div>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Possui Empenho:</b></td>
            <td>
                <input type="radio" value='S' name="empenho" <?php echo ($_REQUEST['empenho'] == 'S') ? "checked='checked'" : '' ?> class="link" >
                <label for="empenhoS" class="link">Sim</label>
                <input type="radio" value='N' name="empenho" <?php echo ($_REQUEST['empenho'] == 'N') ? "checked='checked'" : '' ?> class="link" >
                <label for="preesferaEstadual" class="link">Não</label>
                <input type="radio" value='T' name="empenho" <?php echo ($_REQUEST['empenho'] == 'T' || empty($_REQUEST['empenho'])) ? "checked='checked'" : '' ?> class="link" >
                <label for="todos" class="link">Todos</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Possui Pagamento:</b></td>
            <td>
               <input type="radio" value='S' name="pagamento" <?php echo ($_REQUEST['pagamento'] == 'S') ? "checked='checked'" : '' ?> class="link" >
                <label for="pagamentoS" class="link">Sim</label>
                <input type="radio" value='N' name="pagamento" <?php echo ($_REQUEST['pagamento'] == 'N') ? "checked='checked'" : '' ?> class="link" >
                <label for="preesferaEstadual" class="link">Não</label>
                <input type="radio" value='T' name="pagamento" <?php echo ($_REQUEST['pagamento'] == 'T' || empty($_REQUEST['pagamento'])) ? "checked='checked'" : '' ?> class="link" >
                <label for="todos" class="link">Todos</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Data de vencimento:</b></td>
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
    $obPreObraControle = new PreObraControle();
    $mixObra = $obPreObraControle->relatorioFinanceiroObra($_REQUEST, TRUE);

    $cabecalho = array("ID Obra", "Preid", "Nome da Obra", "UF", "Município", "IBGE", "Esfera", "Valor da Obra", "Valor Empenho", "Valor Pago", "Valor Restante", "Processo", "Tipo de Obra", "Programa");
    if ($_REQUEST['acaoBotao'] == 'btnExcel') {#Excel
        ob_clean();
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=SIMEC_RelatFinanceiro" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=SIMEC_RelatFinanceiro" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");
        $db->monta_lista_tabulado($mixObra, $cabecalho, 1000000, 5, 'N', '100%');
        exit;
    } else { #Listagem
        $db->monta_lista($mixObra, $cabecalho, 25, 10, 'N', 'center', 'N');
    }
}
?>

<script type="text/javascript" src="../par/js/jquery-1.11.1.min.js"></script>
<script type="text/javascript" src="../includes/jQuery/jquery-migrate-1.2.1.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">
    $(document).ready(function () {
        $('#btnPesquisar, #btnExcel').click(function(){
            var estuf = document.getElementById('estuf');
            selectAllOptions( estuf );
            var muncod = document.getElementById('listaMunicipio');
            selectAllOptions( muncod );
            $('input[name=acaoBotao]').val($(this).attr('id'));
            $('#formulario').submit();
        });

        // Habilita somente quando restricao por = Termo
        $('input[name=preesfera]').click(function () {
            if ($(this).val() === 'M') {
                ocultarCampos();
                $('#trMunicipio').show();
            } else if ($(this).val() === 'E') {
                ocultarCampos();
                $('#trUf').show();
                $('#trMunicipio').show();
            } else {
                ocultarCampos();
            }
        });

        //Limpar Tela
        $('input[name=btnCancel]').click(function () {
            ocultarCampos();
        });

        // Somente numeros
        $("input[name=obrid], input[name=preid]").bind("keyup blur focus", function (e) {
            e.preventDefault();
            var expre = /[^0-9]/g;
            if ($(this).val().match(expre))
                $(this).val($(this).val().replace(expre, ''));
        });
    });

    /**
     * Oculta os Campos
     * @returns VOID
     */
    function ocultarCampos() {
        $('#trMunicipio').hide();
        $('#trUf').hide();
    }

    /**
     * Abre janela popup para selecionar municipios
     * @returns VOID
     */
    function abrirPopupListaMunicipio() {
        window.open('http://<?= $_SERVER['SERVER_NAME'] ?>/cte/combo_municipios_bandalarga.php?nomeCombo=listaMunicipio&nomeFormulario=formulario', 'municipios', 'width=400,height=400,scrollbars=1');
    }
</script>
