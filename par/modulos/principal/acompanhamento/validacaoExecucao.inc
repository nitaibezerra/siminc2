<?php
#Titulo Ajax
if ($_REQUEST['requisicao'] == 'SuperTitleAjax') {
    ob_clean();
    $sql = "SELECT
                e.empnumero,
                ve.saldo::numeric(20,2),
                (SELECT
                    COALESCE(SUM(pg.pagvalorparcela),0)::numeric(20,2) as v_valorpago
                FROM par.pagamentosubacao pags
                INNER JOIN par.pagamento pg on pg.pagid = pags.pagid
                INNER JOIN par.empenho em ON em.empid = pg.empid AND empstatus = 'A' AND pg.pagstatus = 'A' AND trim(pg.pagsituacaopagamento) IN ('EFETIVADO', '2 - EFETIVADO', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA')
                WHERE em.empid = ve.empid
                ) as pago,
                (ve.saldo - (SELECT
                    COALESCE(SUM(pg.pagvalorparcela),0)::numeric(20,2) as v_valorpago
                FROM par.pagamentosubacao pags
                INNER JOIN par.pagamento pg on pg.pagid = pags.pagid
                INNER JOIN par.empenho em ON em.empid = pg.empid AND empstatus = 'A' AND pg.pagstatus = 'A' AND trim(pg.pagsituacaopagamento) IN ('EFETIVADO', '2 - EFETIVADO', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA')
                WHERE em.empid = ve.empid)) as a_pagar
            FROM par.v_saldo_empenho_por_subacao ve
            INNER JOIN par.empenho e ON e.empid = ve.empid
            WHERE ve.empid = {$_REQUEST['empid']}";

    $arrEmpenho = $db->carregar($sql);

    $strTabela = "<table>
                        <tr>
                            <th>Empenho</th>
                            <th>Valor</th>
                            <th>Pago</th>
                            <th>A Pagar</th>
                        </tr>";
    foreach($arrEmpenho as $empenho){
        $strTabela .="<tr>
                            <td>{$empenho['empnumero']}</td>
                            <td>R$: ".number_format($empenho['saldo'],2,',','.')."</td>
                            <td>R$: ".number_format($empenho['pago'],2,',','.')."</td>
                            <td>R$: ".number_format($empenho['a_pagar'],2,',','.')." </td>
                      </tr>";
    }
    $strTabela .="</table>";

    echo $strTabela;
    exit();
}

#Lista Subacao por Dopid
if ($_POST['requisicao'] == 'listarSubacaoPorDopid') {
    ob_clean();
    $obSubacaoControle = new SubacaoControle();
    $arrParam = array('dopid' => $_POST['dopid'], 'inuid'=> $_POST['inuid']);
    $mixSubacao = $obSubacaoControle->listarSubacaoPorDopid($arrParam, TRUE);
    
    $cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Valor Subação","Nota de Empenho", "Valor Empenho", "Valor Pago", "Valor Contrato/Item", "Valor total dos Contratos", "Valor Nota/Item","Valor total das Notas", "Valor máximo a liberar", "Último Valor Liberado", "Valor a Liberar", "");
    $db->monta_lista_simples($mixSubacao,$cabecalho,100,0,'N','95%','N');
    
    $strTblBotao = '<table class="listagem" width="95%" cellspacing="0" cellpadding="2" border="0" align="center"><tr>
            <td bgcolor="#c0c0c0" align="center">
                <input type="button" value="Salvar" onclick="salvarValorLiberar('.$_POST['dopid'].')" name="btnSalvar">
            </td>
        </tr></table>';
    echo $strTblBotao;
    exit();
}

#Listar as subacoes itens composicao
if ($_POST['requisicao'] == 'listarSubacaoItensComposicaoPorSbdid') {
    ob_clean();
    $arrParam = array('sbdid' => $_POST['sbdid'], 'dopid'=> $_POST['dopid']);
    $obSubacaoItensComposicaoControle = new SubacaoItensComposicaoControle();
    $mixSubacaoItensComposicao = $obSubacaoItensComposicaoControle->listarSubacaoItensComposicaoPorSbdid($arrParam, TRUE);
    
    $cabecalho = array("Descrição do Item", "Quantidade do Termo", "Quantidade recebida", "Contratos Anexados", "Notas Fiscais Anexadas", "Histórico de Diligência" );
    $db->monta_lista_simples($mixSubacaoItensComposicao,$cabecalho,100,0,'N','95%','N');
    exit();
}

#Salvar Valor Liberação
if ($_POST['requisicao'] == 'salvarValorLiberacao') {
    
    $obSubacaoDetalheLiberacaoPagamentoControle = new SubacaoDetalheLiberacaoPagamentoControle();
    $booResultado = $obSubacaoDetalheLiberacaoPagamentoControle->salvarValorLiberacao($_POST);
    echo $booResultado;
    exit();
}

#Salvar Diligencia
if ($_POST['requisicao'] == 'salvarDiligencia') {
    global $db;
    $nuCpf = $_SESSION['usucpf'];
    $txDiligencia = utf8_decode($_POST['txDiligencia']);
    if ($_POST['tipo'] == 'Contrato') {
        $sql = "update par.contratos set condiligencia = '{$txDiligencia}', condtdiligencia = NOW(), concpfdiligencia = '{$nuCpf}' where conid = {$_POST['id']}";
    }else {
        $sql = "update par.notasfiscais set ntfdiligencia = '{$txDiligencia}', ntfdtdiligencia = NOW(), ntfcpfdiligencia = '{$nuCpf}' where ntfid = {$_POST['id']}";
    }
    
    $retorno = 'falha';
    if ($db->executar($sql)) {
        $retorno = 'sucesso';
        $db->commit();
    }
    
    echo $retorno;
    exit();
}
# Inclui cabecalho
include APPRAIZ . 'includes/cabecalho.inc';

# Titulo
monta_titulo('Validação de Execução', '');

?>
<form id="formulario" name="formulario" method="post" action="">
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
        <tr id="trUf">
            <td class="SubTituloDireita">Esfera:</td>
            <td>
                <input type="radio" value='municipal' name="esfera" <?php echo ($_REQUEST['esfera'] == 'municipal') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Municipal</label>
                <input type="radio" value='estadual' name="esfera" <?php echo ($_REQUEST['esfera'] == 'estadual') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Estadual</label>
                <input type="radio" value='T' name="esfera" <?php echo ($_REQUEST['esfera'] == 'T' || $_REQUEST['esfera'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Todos</label>
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
                            }else{
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
            <td class="SubTituloDireita"><b>Termo de Compromisso:</b></td>
            <td>
               <?php
                    echo campo_texto( 'dopnumerodocumento', 'N', 'S', '', 20, 50, '[#]', '', '', '', '', '', '', $_REQUEST['dopnumerodocumento']);
		?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Com TR:</b></td>
            <td>
                <input type="radio" value='S' name="comtr" <?php echo ($_REQUEST['comtr'] == 'S') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Sim</label>
                <input type="radio" value='N' name="comtr" <?php echo ($_REQUEST['comtr'] == 'N') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Não</label>
                <input type="radio" value='T' name="comtr" <?php echo ($_REQUEST['comtr'] == 'T' || $_REQUEST['comtr'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Todos</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Com Nota:</b></td>
            <td>
                <input type="radio" value='S' name="comnota" <?php echo ($_REQUEST['comnota'] == 'S') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Sim</label>
                <input type="radio" value='N' name="comnota" <?php echo ($_REQUEST['comnota'] == 'N') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Não</label>
                <input type="radio" value='T' name="comnota" <?php echo ($_REQUEST['comnota'] == 'T' || $_REQUEST['comnota'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Todos</label>
            </td>
        </tr>
        <tr style="<?php echo ($_REQUEST['comnotavalidada'] == 'S') ? "" : "display: none"; ?>" id="trComNotaValidada">
            <td class="SubTituloDireita"><b>Com Nota Validada:</b></td>
            <td>
                <input type="radio" value='S' name="comnotavalidada" <?php echo ($_REQUEST['comnotavalidada'] == 'S') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Sim</label>
                <input type="radio" value='N' name="comnotavalidada" <?php echo ($_REQUEST['comnotavalidada'] == 'N') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Não</label>
                <input type="radio" value='T' name="comnotavalidada" <?php echo ($_REQUEST['comnotavalidada'] == 'T' || $_REQUEST['comnotavalidada'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Todos</label>
            </td>
        </tr>
        <tr style="<?php echo ($_REQUEST['comnotadiligenciada'] == 'S') ? "" : "display: none"; ?>" id="trComNotaDiligenciada">
            <td class="SubTituloDireita"><b>Com Nota Diligenciada:</b></td>
            <td>
                <input type="radio" value='S' name="comnotadiligenciada" <?php echo ($_REQUEST['comnotadiligenciada'] == 'S') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Sim</label>
                <input type="radio" value='N' name="comnotadiligenciada" <?php echo ($_REQUEST['comnotadiligenciada'] == 'N') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Não</label>
                <input type="radio" value='T' name="comnotadiligenciada" <?php echo ($_REQUEST['comnotadiligenciada'] == 'T' || $_REQUEST['comnotadiligenciada'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Todos</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Com Contrato:</b></td>
            <td>
                <input type="radio" value='S' name="comcontrato" <?php echo ($_REQUEST['comcontrato'] == 'S') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Sim</label>
                <input type="radio" value='N' name="comcontrato" <?php echo ($_REQUEST['comcontrato'] == 'N') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Não</label>
                <input type="radio" value='T' name="comcontrato" <?php echo ($_REQUEST['comcontrato'] == 'T' || $_REQUEST['comcontrato'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Todos</label>
            </td>
        </tr>
        <tr style="<?php echo ($_REQUEST['comcontrato'] == 'S') ? "" : "display: none"; ?>" id="trComContratoDiligenciado">
            <td class="SubTituloDireita"><b>Com Contrato Diligenciado:</b></td>
            <td>
                <input type="radio" value='S' name="comcontratodiligenciado" <?php echo ($_REQUEST['comcontratodiligenciado'] == 'S') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Sim</label>
                <input type="radio" value='N' name="comcontratodiligenciado" <?php echo ($_REQUEST['comcontratodiligenciado'] == 'N') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Não</label>
                <input type="radio" value='T' name="comcontratodiligenciado" <?php echo ($_REQUEST['comcontratodiligenciado'] == 'T' || $_REQUEST['comcontratodiligenciado'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Todos</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Pagamento:</b></td>
            <td>
                <input type="radio" value='sempagamento' name="pagamento" <?php echo ($_REQUEST['pagamento'] == 'sempagamento') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Sem Pagamento</label>
                <input type="radio" value='parcial' name="pagamento" <?php echo ($_REQUEST['pagamento'] == 'parcial') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Pagamento Parcial</label>
                <input type="radio" value='T' name="pagamento" <?php echo ($_REQUEST['pagamento'] == 'T' || $_REQUEST['pagamento'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Todos</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Com Valor Liberado:</b></td>
            <td>
                <input type="radio" value='S' name="vlrliberado" <?php echo ($_REQUEST['vlrliberado'] == 'S') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Sim</label>
                <input type="radio" value='N' name="vlrliberado" <?php echo ($_REQUEST['vlrliberado'] == 'N') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Não</label>
                <input type="radio" value='T' name="vlrliberado" <?php echo ($_REQUEST['vlrliberado'] == 'T' || $_REQUEST['vlrliberado'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Todos</label>
            </td>
        </tr>
        <tr>
            <td bgcolor="#c0c0c0"></td>
            <td align="left" bgcolor="#c0c0c0">
                <input type="button" name="btnPesquisar" id="btnPesquisar" value="Filtrar" />
                <input type="hidden" name="acaoBotao" value="" />
            </td>
        </tr>
    </table>
</form>

<div id="dialog_acoes"></div>

<?php
# Filtros da pesquisa
if (isset($_REQUEST['acaoBotao'])) {
    #Instancia de ProcessoPar
    $obProcessoParControle = new ProcessoParControle();
    $mixProcessoPar = $obProcessoParControle->relatorioLiberacaoPagamento($_REQUEST, TRUE);

    $cabecalho = array("Ação", "UF", "IBGE", "Município", "Processo", "Termo", "Tipo de Termo", "Valor do Termo", "");
    $db->monta_lista($mixProcessoPar, $cabecalho, 50, 15, '', 'center', 'N', '', '', '', '', '');
}
?>

<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<link rel="stylesheet" href="../includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script type="text/javascript">
    jQuery(document).ready(function () {

        jQuery('#btnPesquisar').click(function(){
            var estuf = document.getElementById('estuf');
            selectAllOptions( estuf );
            var muncod = document.getElementById('listaMunicipio');
            selectAllOptions( muncod );
            jQuery('input[name=acaoBotao]').val(jQuery(this).attr('id'));
            jQuery('#formulario').submit();
        });

        jQuery('input[name=comnota]').click(function(){
            if (jQuery(this).val() == 'S') {
                jQuery('#trComNotaValidada').css('display','');
                jQuery('#trComNotaDiligenciada').css('display','');
            } else {
                jQuery('#trComNotaValidada').css('display','none');
                jQuery('#trComNotaDiligenciada').css('display','none');
            }
        });
        
        jQuery('input[name=comcontrato]').click(function(){
            if (jQuery(this).val() === 'S') {
                jQuery('#trComContratoDiligenciado').css('display','');
            } else {
                jQuery('#trComContratoDiligenciado').css('display','none');
            }
        });

        $('a.media').media();

    });

    /**
     * Abre janela popup para selecionar municipios
     * @returns VOID
     */
    function abrirPopupListaMunicipio() {
        window.open('http://<?php echo $_SERVER['SERVER_NAME'] ?>/cte/combo_municipios_bandalarga.php?nomeCombo=listaMunicipio&nomeFormulario=formulario', 'municipios', 'width=400,height=400,scrollbars=1');
    }

    //CArregar Subacoes
    function carregarPainelInterno(idImg, dopid, inuid) {
        var img = jQuery('#'+idImg);
        var tr = jQuery('#tr_' + dopid);
        var td = jQuery('#td_' + dopid);
        if (tr.css('display') === 'none') {
            jQuery.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=listarSubacaoPorDopid&dopid="+dopid+"&inuid="+inuid,
                async: false,
                success: function(msg){
                    if (msg !== '') {
                        img.attr('src', '../imagens/menos.gif');
                        td.html(msg);
                        td.show();
                        tr.show();
                    }
                }
            });
        } else {
            img.attr('src', '../imagens/mais.gif');
            tr.hide();
            td.hide();
        }
    }

    // Carregar Itens composicao
    function carregarSubacaoItensComposicao(idImg, sbdid, dopid) {
        var img = jQuery('#'+idImg);
        var tr = jQuery('#tr_' + sbdid);
        var td = jQuery('#td_' + sbdid);
        img.attr('clicado','S');
        if (tr.css('display') === 'none') {
            jQuery.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=listarSubacaoItensComposicaoPorSbdid&sbdid="+sbdid+"&dopid="+dopid,
                async: false,
                success: function(msg){
                    if (msg !== '') {
                        img.attr('src', '../imagens/menos.gif');
                        td.html(msg);
                        td.show();
                        tr.show();
                    }
                }
            });
        } else {
            img.attr('src', '../imagens/mais.gif');
            tr.hide();
            td.hide();
        }
    }

    // Historico de Acompanhamento
    function historicoAcompanhamento(id, tipoacompanhamento){
        return window.open('par.php?modulo=principal/popupHistoricoAcompanhamento&acao=A&id='+id+'&tipoacompanhamento='+tipoacompanhamento,
            'Histórico de Acompanhamento',
            "height=400,width=900,scrollbars=yes,top=50,left=200" ).focus();
    }
    
    // Historico de Diligencia
    function historicoDiligencia(id, tipoacompanhamento){
        return window.open('par.php?modulo=principal/popupHistoricoDiligencia&acao=A&id='+id+'&tipoacompanhamento='+tipoacompanhamento,
            'Histórico de Acompanhamento',
            "height=400,width=900,scrollbars=yes,top=50,left=200" ).focus();
    }

    // Abrir popup de subacao
    function listarSubacao(sbaid, ano, inuid){
        var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid + "&anoatual=" + ano + "&inuid=" + inuid;
        janela(local,800,600,"Subação");
    }

    function salvarValorLiberar(agrupador) {

        var arrVlr = {};
        var arrContrato = {};
        var arrNota = {};
        var intQtd = 0;

        // Agrupa Numeros de contratos selecionados/validados
        //jQuery('.contrato_' + agrupador).each(function(){
        //    if (jQuery(this).is(':checked')) {
        //        arrContrato[jQuery(this).attr('conid')] = jQuery(this).val();
        //    }
        //});*/

        // Agrupa Numeros de notas selecionados/validados
        jQuery('.notas_' + agrupador).attr('checked', 'checked');
        jQuery('.notas_' + agrupador).each(function(){
         //   if (jQuery(this).is(':checked')) {
            arrNota[jQuery(this).attr('ntfid')] = jQuery(this).val();
        //    }
        });

        //Agrupa SBDID
        jQuery('.' + agrupador).each(function () {
            if (jQuery(this).val() !== '') {
                var sbdid = jQuery(this).attr('sbdid');
                var vlr_maximo_liberar = parseFloat(jQuery('input[name=vlr_dif_' + sbdid + ']').val());
                var vlr_a_liberar = jQuery(this).val().replace('.', '');
                vlr_a_liberar = vlr_a_liberar.replace(',', '.');
                vlr_a_liberar = parseFloat(vlr_a_liberar);

                if (vlr_a_liberar < 0) {
                    alert('O Valor não pode ser negativo.');
                    jQuery(this).val('');
                    intQtd++;
                }

                if (jQuery('#'+sbdid).attr('clicado') != 'S') {
                    alert('Verifique os Itens antes de salvar.');
                    jQuery(this).val('');
                    intQtd++;
                    return false;
                }

                if (vlr_a_liberar > vlr_maximo_liberar) {
                    alert('O valor a ser liberado não pode ultrapassar: R$ ' + vlr_maximo_liberar.toFixed(2));
                    jQuery(this).val('');
                    intQtd++;
                }

                arrVlr[sbdid] = jQuery(this).val();
            }
        });

        if (intQtd == 0) {
            if (!isEmpty(arrVlr)) {
                jQuery.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: {requisicao: 'salvarValorLiberacao', sbdids: arrVlr, /*conids: arrContrato, */ntfids: arrNota},
                    async: false,
                    success: function (booResultado) {
                        if (booResultado === '1') {
                            alert('Salvo com Sucesso!');
                            jQuery('#' + agrupador).click();
                            jQuery('#' + agrupador).click();
                        } else {
                            alert('Falha ao salvar!');
                        }
                    }
                });
            } else {
                alert('O campo Valor a Liberar deve ser preenchido.');
            }
        }
    }

    var isEmpty = function(obj){
        return Object.keys(obj).length === 0;
    }

    function downloadArquivo(arqid){
        return window.open('par.php?modulo=principal/popupDownload&acao=A&arqid='+arqid,
            'Download Arquivo',
            "height=15,width=15,scrollbars=yes,top=50,left=200" ).focus();
    }

    function popupParecer(icoid){
        window.open('par.php?modulo=principal/popupAcompanhamentoParecer&acao=A&icoid=' + icoid,  '' ,'width=800,height=400,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 300) / 2);
    }

    function visualizarDocumento(dopid){
        window.open('par.php?modulo=principal/documentoPar&acao=A&req=formVizualizaDocumento&dopid=' + dopid,  '' ,'width=800,height=400,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1,location=no, left='+ (document.documentElement.clientWidth - 780) / 2 + ",top=" + (document.documentElement.clientHeight - 300) / 2);
    }

    function visualizarDocumentoPdf( arqid, tipo, identificador, id, dopid, icoid ){
        var title = tipo + ': #' + identificador;
        var url = '/par/par.php?modulo=principal/acompanhamento/visualizarDocumentoPdf&acao=A';
        jQuery("#dialog_acoes").load(url, {arqid: arqid , tipo: tipo, id: id, dopid: dopid, icoid: icoid}, function(){
            jQuery("#dialog_acoes").dialog({
                modal: true,
                width: 1000,
                title: title,
                position: ['center', 'top'],
                buttons: {
                    Fechar: function() {
                        jQuery("#divPopUp").html('');
                        jQuery( this ).dialog( "close" );
                    }
                }
            });
        });
    }

    function removeTitle(obj){
        obj.parent().parent().removeAttr('title')
    }
</script>