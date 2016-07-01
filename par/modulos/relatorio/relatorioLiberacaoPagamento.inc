<?php
if ($_POST['requisicao'] == 'listarSubacaoPorDopid') {
    ob_clean();
    $obSubacaoControle = new SubacaoControle();
    $arrParam = array('dopid' => $_POST['dopid'], 'inuid'=> $_POST['inuid']);
    $mixSubacao = $obSubacaoControle->listarSubacaoPorDopid($arrParam, TRUE);
    
    $cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Valor Subação", "Valor Empenho", "Valor Pago", "Valor total dos Contratos", "Valor total das Notas", "Último Valor Liberado", "Valor a Liberar", "");
    $db->monta_lista_simples($mixSubacao,$cabecalho,100,0,'N','95%','N');
    
    $strTblBotao = '<table class="listagem" width="95%" cellspacing="0" cellpadding="2" border="0" align="center"><tr>
            <td bgcolor="#c0c0c0" align="center">
                <input type="button" value="Salvar" id="btnSalvar" name="btnSalvar" ref="'.$_POST['dopid'].'">
            </td>
        </tr></table>';
    echo $strTblBotao;
    exit();
}

#Listar as subacoes itens composicao
if ($_POST['requisicao'] == 'listarSubacaoItensComposicaoPorSbdid') {
    ob_clean();
    $obSubacaoItensComposicaoControle = new SubacaoItensComposicaoControle();
    $mixSubacaoItensComposicao = $obSubacaoItensComposicaoControle->listarSubacaoItensComposicaoPorSbdid($_POST['sbdid'], TRUE);
    
    $cabecalho = array("Descrição do Item", "Quantidade do Termo", "Quantidade recebida", "Contratos Anexados", "Notas Fiscais Anexadas", "Histórico de Parecer" );
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
# Inclui cabecalho
include APPRAIZ . 'includes/cabecalho.inc';

# Titulo
monta_titulo('Validaçao de Execuçao', '');

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
                <input type="radio" value='' name="esfera" <?php echo ($_REQUEST['esfera'] == '') ? "checked='checked'" : '' ?> class="link" >
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
            <td class="SubTituloDireita"><b>Com TR:</b></td>
            <td>
                <input type="radio" value='S' name="comtr" <?php echo ($_REQUEST['comtr'] == 'S') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Sim</label>
                <input type="radio" value='N' name="comtr" <?php echo ($_REQUEST['comtr'] == 'N') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Não</label>
                <input type="radio" value='' name="comtr" <?php echo ($_REQUEST['comtr'] == '') ? "checked='checked'" : '' ?> class="link" >
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
                <input type="radio" value='' name="comnota" <?php echo ($_REQUEST['comnota'] == '') ? "checked='checked'" : '' ?> class="link" >
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
                <input type="radio" value='' name="comcontrato" <?php echo ($_REQUEST['comcontrato'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Todos</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"><b>Processos disponíveis para pagamento:</b></td>
            <td>
                <input type="radio" value='S' name="processo_disponivel" <?php echo ($_REQUEST['processo_disponivel'] == 'S' || $_REQUEST['processo_disponivel'] == '') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Sim</label>
                <input type="radio" value='N' name="processo_disponivel" <?php echo ($_REQUEST['processo_disponivel'] == 'N') ? "checked='checked'" : '' ?> class="link" >
                <label class="link">Não</label>
                <input type="radio" value='T' name="processo_disponivel" <?php echo ($_REQUEST['processo_disponivel'] == 'T') ? "checked='checked'" : '' ?> class="link" >
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

<?php
# Filtros da pesquisa
if (isset($_REQUEST['acaoBotao'])) {
    #Instancia de ProcessoPar
    $obProcessoParControle = new ProcessoParControle();
    $mixProcessoPar = $obProcessoParControle->relatorioLiberacaoPagamento($_REQUEST);
     
//    include APPRAIZ.'includes/library/simec/Lista_jQuery_DataTables.php';
//
//    $param['nome'] = 'listaLiberacaoParamento';
//    $param['titulo'] = 'Listas de Liberação de Pagamentos';
//    $param['descricaoBusca'] = 'PAR';
//    $param['instrucaoBusca'] = '(Nome da Obra, Tipo da Obra ou Situação)';
//    $param['nomeXLS'] = "Lista de Obras $descricao-$dataAtual";
//    $param['numeroColunasXLS']	= '1,2,3,4,5,6,7,8';
//    $param['arrDados'] = $mixProcessoPar;		
//    $param['arrCabecalho'] = array("Ação", "UF", "IBGE", "Município", "Processo", "Termo", "Tipo de Termo");
//    $param['arrSemSpan'] = array( 'N', '', '', 'N', '', '', '', '', 'N' );
//    $param['arrCampoNumerico'] = array();
//
//    $lista = new listaDT();
//    echo $lista->lista($param);
    
    $cabecalho = array("Ação", "UF", "IBGE", "Município", "Processo", "Termo", "Tipo de Termo", "Valor do Termo", "");
    $db->monta_lista($mixProcessoPar, $cabecalho, 25, 10, 'N', 'center', 'N');
}
?>

<script type="text/javascript" src="../par/js/jquery-1.11.1.min.js"></script>
<script type="text/javascript" src="../includes/jQuery/jquery-migrate-1.2.1.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">
    $(document).ready(function () {
        
        $('#btnPesquisar').click(function(){
            var estuf = document.getElementById('estuf');
            selectAllOptions( estuf );
            var muncod = document.getElementById('listaMunicipio');
            selectAllOptions( muncod );
            $('input[name=acaoBotao]').val($(this).attr('id'));
            $('#formulario').submit();
        });
        
        $( "#btnSalvar" ).live( "click", function() {
            var agrupador = $(this).attr('ref');
            
            var arrVlr = {};
            $('.'+agrupador).each(function(){
                if ($(this).val() !== '') {
                    arrVlr[$(this).attr('sbdid')] = $(this).val();
                }
            });
            
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: {requisicao:'salvarValorLiberacao', sbdids:arrVlr},
                async: false,
                success: function(booResultado){
                    if (booResultado === '1') {
                        alert('Salvo com Sucesso!');
                    } else{
                        alert('Falha ao salvar!');
                    }
                }
            });
        });
        
        $( "table.listagem td > input[type=text]" ).live( "change", function() {
            var sbdid =  $(this).attr('sbdid');
            var vlr_maximo_liberar =  parseFloat($('input[name=vlr_dif_'+sbdid+']').val());
            var vlr_a_liberar = $(this).val().replace('.','');
            vlr_a_liberar = vlr_a_liberar.replace(',', '.');
            vlr_a_liberar =  parseFloat(vlr_a_liberar);
            
            if (vlr_a_liberar < 0) {
                alert('O Valor não pode ser negativo.');
                $(this).val('');
                return false;
            }
            
            if (vlr_a_liberar > vlr_maximo_liberar) {
                alert('O valor a ser liberado não pode ultrapassar: R$ '+vlr_maximo_liberar.toFixed(2));
                $(this).val('');
                return false;
            }
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
    
    //CArregar Subacoes
    function carregarPainelInterno(idImg, dopid, inuid) {
	var img = $('#'+idImg);
	var tr = $('#tr_' + dopid);
	var td = $('#td_' + dopid);
	if (tr.css('display') === 'none') {
            $.ajax({
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
    function carregarSubacaoItensComposicao(idImg, sbdid) {
	var img = $('#'+idImg);
	var tr = $('#tr_' + sbdid);
	var td = $('#td_' + sbdid);
	if (tr.css('display') === 'none') {
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=listarSubacaoItensComposicaoPorSbdid&sbdid="+sbdid,
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
    
    // Abrir popup de subacao
    function listarSubacao(sbaid, ano, inuid){
	var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid + "&anoatual=" + ano + "&inuid=" + inuid;
	janela(local,800,600,"Subação");
    }
</script>
