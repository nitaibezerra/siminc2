<?php
// empreendimento || obra || orgao
verificaSessao('obra');

include APPRAIZ . 'includes/cabecalho.inc';
// Cria as abas do módulo
echo '<br>';

$_SESSION['obras2']['obrid'] = ( $_GET['obrid'] ? $_GET['obrid'] : $_SESSION['obras2']['obrid'] );
$obrid                       = $_SESSION['obras2']['obrid'];

$obra = new Obras($obrid);
if ($obra->getTravaCronograma($obrid) == 't') {
    die("<script>
          alert('Cronograma não disponível para edição.');
            window.location = '?modulo=principal/etapas_da_obra&acao=A&obrid={$obrid}';
         </script>");
}


function validaDataPHP($data) {
    $data = new DateTime($data);
}

$cronograma = new Cronograma();
$arDadosCronograma = $cronograma->getCronogramaEdicao($obrid);
$cronograma->popularDadosObjeto($arDadosCronograma);

if($_POST['cancelar']){
    $cronograma->crostatus = 'I';
    $cronograma->salvar();
    $cronograma->commit();

    die("<script>
          alert('Operação cancelada!');
            window.location = '?modulo=principal/etapas_da_obra&acao=A&obrid={$obrid}';
         </script>");
}


if ($_POST['evt'] == 'salvar') {


    $cronograma->croobs = $_POST['croobs'];
    $cronograma->salvar();

    //verificar se as data de inicio precisa ser alterada
    if (!empty($_POST['dtInicioObra'])) {
        $arObra['obrinicio'] = formata_data_sql($_POST['dtInicioObra']);
    }

    //verificar se as data de termino precisa ser alterada
    if (!empty($_POST['dtTerminoObra'])) {
        $arObra['obrtermino'] = formata_data_sql($_POST['dtTerminoObra']);
    }

    $arCamposNulo = array();
    if ($_POST['obrcronogramaservicocontratado']) {
        $arObra['obrcronogramaservicocontratado'] = $_POST['obrcronogramaservicocontratado'];
    } else {
        $arCamposNulo[] = 'obrcronogramaservicocontratado';
        $arObra['obrcronogramaservicocontratado'] = null;
    }

    if ($_POST['obrcronogramaservicocontratadojustificativa']) {
        $arObra['obrcronogramaservicocontratadojustificativa'] = $_POST['obrcronogramaservicocontratadojustificativa'];
    } else {
        $arCamposNulo[] = 'obrcronogramaservicocontratadojustificativa';
        $arObra['obrcronogramaservicocontratadojustificativa'] = null;
    }
    
    $obra = new Obras($obrid);
    

    $obra->popularDadosObjeto($arObra)->salvar(true, true, $arCamposNulo);

    $itens    = new ItensComposicaoObras();
    $allItens = $itens->getAllIcoid($obrid, $cronograma->croid);

    $removeItens = array_diff($allItens, (is_array($_POST['icoid']))?$_POST['icoid']:array($_POST['icoid']) );

    $itens->desabilitarItensPeloIcoidECroid($removeItens, $cronograma->croid);
    
    $itens_modificados = array();

    for ($i = 0; $i < count($_POST['itcid']); $i++) {
        if (!empty($_POST['itcid'][$i])) {
            $arDados = array(
//	          'atiid'            					=> $atiid,
                'icoid'                 => ($_POST['icoid'][$i] ? $_POST['icoid'][$i] : null),
                'itcid'                 => ($_POST['itcid'][$i] ? $_POST['itcid'][$i] : null),
                'obrid'                 => $obrid,
                'icopercsobreestrutura' => ($_POST['icopercsobreestrutura'][$i] ? desformata_valor($_POST['icopercsobreestrutura'][$i]) : null),
                'icovlritem'            => ($_POST['icovlritem'][$i] ? desformata_valor($_POST['icovlritem'][$i]) : null),
                'icodtinicioitem'       => ($_POST['icodtinicioitem'][$i] ? formata_data_sql($_POST['icodtinicioitem'][$i]) : null),
                'icodterminoitem'       => ($_POST['icodterminoitem'][$i] ? formata_data_sql($_POST['icodterminoitem'][$i]) : null),
                'icostatus'             => 'A',
                'icodtinclusao'         => (empty($_POST['icoid'][$i]) ? 'NOW()' : null),
                'icoordem'              => ($i + 1),
                'croid'                 => $cronograma->croid
            );
            array_push($itens_modificados, $arDados['icoid']);

            $itens->popularDadosObjeto($arDados)->salvar();
            $itens->clearDados();
            
        }
    }
    
    $itens->commit();
    
    $sql_empreendimento_id = 'SELECT emp.empid
                              FROM obras2.empreendimento emp
                              INNER JOIN obras2.obras oi ON  oi.empid = emp.empid
                              WHERE  oi.obrid = '.$obrid;
    
    $vistoria        = new Supervisao();
    $ultima_vistoria = $vistoria->pegaUltSupidByObra($obrid);
    
//    if($ultima_vistoria){
//        $email = new Email();
//        $email->enviaEmailEdicaoCronogramaObra($_SESSION['obras2']['obrid']);
//    }
    
    die("<script>
            window.location = '?modulo=principal/inserir_vistoria&acao=A&obrid={$obrid}&croid=".$cronograma->croid."';
         </script>");
        

            
}

$obraContrato = new ObrasContrato();
$obra         = new Obras($obrid);
$obraMI       = ($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C) ? true : false;

if($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C){
    $itensComposicao  = new ItensComposicaoObras();
    $ocrvalorexecucao = $itensComposicao->getValorTotalItens($obrid);

    $osMI = new OrdemServicoMI();
    $arr_servicos_externos = $osMI->getArrayServicosExternosImplantacao($obrid);
    
} else {
    $ocrvalorexecucao = $obraContrato->getValorContrato($obrid);
}

if($obraMI){
    $ufObra = $obra->pegaUfObra($obrid);
    $ocrvalorexecucao = $obraContrato->valorContratoObraMi($obra->tpoid,$ufObra);
}

$obrcronogramaservicocontratadojustificativa = $obra->obrcronogramaservicocontratadojustificativa;
$obrcronogramaservicocontratado              = $obra->obrcronogramaservicocontratado;

$docid = pegaDocidObra($obrid);
//$_GET['acao'] = 'V';


    ?>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <?php
    $db->cria_aba($abacod_tela, $url, $parametros);
//	criaAbaVisualizacaoObra();
//	$somenteLeitura = true;
    $habilitado = false;
    $habilita = 'N';




echo cabecalhoObra($obrid);
monta_titulo($titulo_modulo, '<b>Após atualizar uma etapa, clique em salvar.</b>');
echo cabecalhoCronograma($obrid);
$habilitado = true;
$habilita = 'S';


?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">

    jQuery(document).ready(function() {
        jQuery('#aguarde').hide();
    });
    

    function inserirEtapas() {
        return windowOpen('obras2.php?modulo=principal/inserir_etapas&acao=A', 'blank', 'height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function gerenteEdificacao() {

        var imgDesabUp = $(document.createElement('img')).attr({src: '/imagens/seta_cimad.gif',
            border: 0,
            title: 'Subir'});

        var imgUp = $(document.createElement('img')).attr({src: '/imagens/seta_cima.gif',
            border: 0,
            title: 'Subir'})
                .css({cursor: 'pointer'});

        var imgDesabDown = $(document.createElement('img')).attr({src: '/imagens/seta_baixod.gif',
            border: 0,
            title: 'Descer'});

        var imgDown = $(document.createElement('img')).attr({src: '/imagens/seta_baixo.gif',
            border: 0,
            title: 'Descer'})
                .css({cursor: 'pointer'});

        this.addLine = function(dataLine) {
            dataLine = dataLine || '';

            if (dataLine.itcid == '') {
                alert('Falha!\nFaltam dados da etapa.');
                return false;
            }

            var idTR = 'tr_etapa_' + dataLine.itcid;

            var clone = $('#tr_matrix').clone()
                    .attr('id', idTR)
                    .show();

            clone.html(replaceAll(clone.html(), 'id="icodtinicioitem[]"', 'id="icodtinicioitem_' + dataLine.itcid + '"'));
            clone.html(replaceAll(clone.html(), "document.getElementById('icodtinicioitem[]')", "document.getElementById('icodtinicioitem_" + dataLine.itcid + "')"));

            clone.html(replaceAll(clone.html(), 'id="icodterminoitem[]"', 'id="icodterminoitem_' + dataLine.itcid + '"'));
            clone.html(replaceAll(clone.html(), "document.getElementById('icodterminoitem[]')", "document.getElementById('icodterminoitem_" + dataLine.itcid + "')"));

            prepareRemoveImg(clone, idTR, dataLine);

            $('#tr_matrix').before(clone);

            if (dataLine != '') {

<?php
if ($habilitado):
    ?>
                    if (dataLine.icoid) {
                        var img = $(document.createElement('img')).attr({src: '/imagens/ico_config_i.gif',
                            border: 0,
                            title: 'Ir para detalhamento da etapa'})
                                .css({cursor: 'pointer'})
                                .click(function() {
                                    location.href = '?modulo=principal/detalhe_da_etapa&acao=A&croid=<?=$cronograma->croid?>&icoid=' + dataLine.icoid;
                                });
                        clone.find('td')
                                .eq(1)
                                .find('img')
                                .before(img);
                        clone.find('td')
                                .eq(1)
                                .find('img')
                                .eq(1)
                                .before('&nbsp;')
                                .before('&nbsp;');
                    }
    <?php
endif;
?>
                loadData(dataLine, clone);
            }
<?php
if ($habilitado):
    ?>
                prepareArrowImg();
    <?php
endif;
?>
            colorManager();
        }


        this.removeLine = function(id) {
            $('#' + id).remove();
            colorManager();
            prepareArrowImg();

            calculoEdificacao.manager();
        }

        this.moveUp = function(tr) {
            tr = $(tr);
            tr.insertBefore(tr.prev());
            colorManager();
            prepareArrowImg();
        }

        this.moveDown = function(tr) {
            tr = $(tr);
            tr.insertAfter(tr.next());
            colorManager();
            prepareArrowImg();
        }

        function loadData(dataLine, line) {
            var hiddenID = $(document.createElement('input')).attr({name: 'icoid[]',
                type: 'hidden'})
                    .val((dataLine.icoid ? dataLine.icoid : ''));
            line.find('[id*=itcdesc]').html(dataLine.itcdesc)
                    .after(hiddenID);

            line.find('[name*=itcid]').val(dataLine.itcid);
            line.find('[name*=icodtinicioitem]').val(dataLine.icodtinicioitem);
            line.find('[name*=icodterminoitem]').val(dataLine.icodterminoitem);
            line.find('[name*=icovlritem]').val(dataLine.icovlritem);

            calculoEdificacao.manager(line.find('[name*=icovlritem]')[0]);

        }

        function prepareArrowImg() {
            var indMaxTr = ($('#tabela_etapas').find('[id*=tr_etapa_]').length - 1);
            $('#tabela_etapas').find('[id*=tr_etapa_]').each(function(i, obj) {

                var cloneImgUp = (i == 0
                        ? $(imgDesabUp).clone()
                        : $(imgUp).clone()
                        .bind('click', function() {
                            gerenteEdificacao.moveUp(obj);
                        }));
                var cloneImgDown = (i == indMaxTr
                        ? $(imgDesabDown).clone()
                        : $(imgDown).clone()
                        .bind('click', function() {
                            gerenteEdificacao.moveDown(obj);
                        }));

                $(obj).find('td:eq(0)').html('')
                        .append(cloneImgUp)
                        .append('&nbsp;')
                        .append(cloneImgDown);
            });
        }

        function prepareRemoveImg(clone, idTR, dataLine) {
<?php
if ($habilitado == false):
    ?>
                $(clone).find('td:eq(1)').html('');
    <?php
else:
    ?>
                dataLine.qtddetalhamento = dataLine.qtddetalhamento || 0;
                if (typeof dataLine == 'string' || dataLine.qtddetalhamento == 0) {
                    $(clone).find('td:eq(1) span img').bind('click', function() {
                        if (confirm("Deseja remover esse registro?")) {
                            gerenteEdificacao.removeLine(idTR);
                        }
                    });
                } else {
                    $(clone).find('td:eq(1) span img')
                            .attr('src', '/imagens/excluir_01.gif')
                            .attr('title', 'A etapa não pode ser excluída, pois possui detalhamento(s).')
                            .bind('click', function() {
                                alert('A etapa não pode ser excluída, pois possui detalhamento(s).');
                            });
                }
<?php
endif;
?>
        }

        function colorManager() {
            var countLine = 0;
            var colorLine = new Array('#FFFFFF', '#E0E0E0');
            $('#tabela_etapas').find('[id*=tr_etapa_]').each(function(i, obj) {
                $(obj).attr('bgcolor', colorLine[countLine]);
                countLine = (countLine == 0 ? 1 : 0);
            });
        }
    }

    var gerenteEdificacao = new gerenteEdificacao();

    function calculoEdificacao() {
        var obCalc = new Calculo();
        var percentTotal = new Number(0);
        var itemEdicao = '';
        var vlrItemAnterior = '';
//	var vlrContrato  = obCalc.converteMonetario( $('#vl_contrato').val() );

        this.manager = function(obj) {
            obj = obj || '';

            if (obj != '') {
                formatarNumeroMonetario(obj);
            }

            calculaTotal();
            calculaRestante();
        }

        this.setDefaultValue = function(obj) {
            if (obj != itemEdicao) {
                itemEdicao = obj;
                vlrItemAnterior = obj.value;
            }
        }

        function calculaPercent(obj, ind) {
            var vlrContrato = obCalc.converteMonetario($('#vl_contrato').val());
            var vlrItem = obCalc.converteMonetario(obj.value);
            var percentItem = mascaraglobal("[###.]###,##", ((vlrItem / vlrContrato) * 100).toFixed(2));
            percentItem = formatarNumeroMonetario({value: percentItem});
            $('[name*=icopercsobreestrutura]').eq(ind)
                    .val(percentItem);

        }

        function calculaRestante() {
            var vlrRest = obCalc.operacao($('#vl_contrato').val(), $('#totalv').val(), '-');
            $('#rest_totalv').val(mascaraglobal("[###.]###,##", vlrRest));
        }

        function calculaTotal() {
            var vlrContrato = obCalc.converteMonetario($('#vl_contrato').val());
            var vlrTotal = new String('0');
            $('[name*=icovlritem]').not(':last')
                    .each(function(i, objItem) {
                        var vlrItem = $(objItem).val();
                        vlrTotal = obCalc.operacao(vlrTotal, vlrItem, '+');
                        vlrTotal = mascaraglobal("[###.]###,##", vlrTotal);

                        calculaPercent(objItem, i);
                    });
            $('#totalv').val(vlrTotal);

            percentTotal = obCalc.operacao(vlrTotal, vlrContrato, '/', 20);
            percentTotal = obCalc.operacao(percentTotal, 100, '*', 20);
            percentTotal = new Number(percentTotal).toFixed(2);
            $('#total').val(formatarNumeroMonetario({value: percentTotal}));

            var percentRest = obCalc.operacao('100', percentTotal, '-');
            percentRest = formatarNumeroMonetario({value: percentRest});
            $('#rest_total').val(percentRest);

            if (obCalc.comparar(vlrTotal, $('#vl_contrato').val(), '>')) {

//            if (confirm('A soma das etapas excede o valor da estrutura.\nDeseja aumentar o valor da estrutura e em caso da soma das estruturas exceder o valor da obra aumentá-la também?')){
//                $('#vl_contrato').val( vlrTotal )
//                                 .css({color:'red'});
//                $('#vl_contrato_atualizado').val( vlrTotal );
//                $('#atualizarNivel').val( 1 );
//            }else{
                var pulaAlert;
                if (typeof itemEdicao != 'object') {
                    pulaAlert = true;
                    itemEdicao = new Object();
                    itemEdicao.value = 0;
                } else {
                    pulaAlert = false;
                }
                vlrItemAnterior = vlrItemAnterior || 0;
                recalculaTotal = (itemEdicao.value != vlrItemAnterior ? true : false);

                if (recalculaTotal || pulaAlert) {
                    alert('A soma das etapas excede o valor do contrato.');
                    itemEdicao.value = vlrItemAnterior;
                }
//            }
                if (recalculaTotal) {
                    calculaTotal();
                }
            }

        }

        function formatarNumeroMonetario(obj) {
            var valor = new String(obCalc.retiraZeroEsquerda(obj.value));

            if (valor.length == 2) {
                valor = '0' + valor;
            } else if (valor.length == 1) {
                valor = '00' + valor;
            } else if (valor.length == 0) {
                valor = '000';
            }

            obj.value = mascaraglobal("[###.]###,##", valor);
            return obj.value;
        }

    }

    calculoEdificacao = new calculoEdificacao();

    function verificaDataTermino(data) {
        var obData = new Data();

        if (obData.comparaData(data.value, $('#crtdatainicioLabel').html(), '<')) {
            alert('A data de término não pode ser menor que a data de início do projeto.');
            data.value = '';
            data.focus();
            return;
        }

        if (obData.comparaData(data.value, $('#crtdatafimLabel').html(), '>')) {
            alert('A data de término não pode ser maior que a data de conclusão do projeto.');
            data.value = '';
            data.focus();
            return;
        }

        // verificando se a data de termino não é menor que a data de inicio
        var nome = ($(data).attr('id')).split('_');
        if (nome[1]) {
            if (obData.comparaData(data.value, $('#icodtinicioitem_' + nome[1]).val(), '<')) {
                alert('A data de término não pode ser menor que a data de início.');
                data.value = '';
                data.focus();
                return;
            }
        }
    }

</script>

<form method="post" id="formulario" name="formulario">

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="">
    <tr>
        <td width="265" class="subtitulodireita">Justificativa: </td>
        <td>
            <?php
            $croobs = $cronograma->croobs;
            echo campo_textarea( 'croobs', 'S', 'S', '', 100, 20, 5000 );
            ?>
        </td>
    </tr>
    </table>
    <input type="hidden" name="evt" id="evt" value="">
    <input type="hidden" name="vl_contrato_atualizado" id="vl_contrato_atualizado" value="0">
    <input type="hidden" name="atualizarNivel" 	       id="atualizarNivel"         value="0">
    <input type="hidden" name="dtInicioObra" 	       id="dtInicioObra" 	   value="">
    <input type="hidden" name="dtTerminoObra" 	       id="dtTerminoObra" 	   value="">
    <input type="hidden" name="xls" 	               id="xls"                    value="">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tbEtapasDaObra">
        <tr>
            <center><label class="TituloTela" style="color:#000000;">Lista das Etapas da Obra</label></center>
            <!--<td class="SubTituloTelaCentro" colspan="2">Lista das Etapas da Obra</td>-->
        </tr>
        <tr>
            <td colspan="2">
                <table id="tabela_etapas" width="95%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
                    <thead>
                        <tr id="cabecalho">
                            <td width="5%"  valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Ordem</strong></td>
                            <td width="5%"  valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Ação</strong></td>
                            <td width="30%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Descrição</strong></td>
                            <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Data de Início</strong></td>
                            <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Data de Término</strong></td>
                            <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>Valor do Item (R$)</strong></td>
                            <td width="10%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor = '#c0c0c0';" onmouseout="this.bgColor = '';"><strong>(%) Referente a Estrutura</strong></td>
                        </tr>
                    </thead>
                    <tbody>
                        <? // if($_GET['acao'] != 'E'){ ?>
                            <tr id="tr_matrix" style="display:none;">
                                <td align="center"></td>
                                <td align="center">
                                    <span>
                                        <img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir'>
                                    </span>
                                </td>
                                <td>
                                    <span id="itcdesc[]"></span>
                                    <input type="hidden" name="itcid[]" value="">
                                </td>
                                <td align="center"><?php echo campo_data2('icodtinicioitem[]', 'S', $habilita, '', 'S', '', ' verificaDataInicio(this); ', '', ' verificaDataInicio(this); '); ?></td>
                                <td align="center"><?php echo campo_data2('icodterminoitem[]', 'S', $habilita, '', 'S', '', ' verificaDataTermino(this); ', '', ' verificaDataTermino(this); '); ?></td>
                                <td align="center"><?php echo campo_texto('icovlritem[]', 'S', $habilita, '', 14, 14, '', '', 'right', '', 0, 'onkeypress="calculoEdificacao.setDefaultValue( this )";', 'calculoEdificacao.manager( this );'); ?></td>
                                <td align="center"><?php echo campo_texto('icopercsobreestrutura[]', 'N', 'N', '', 5, 6, '', '', 'right', '', 0); ?> %</td>
                            </tr>
                        <? // }else{ ?>
<!--                            <tr id="tr_matrix" style="display:none;">
                                <td>
                                    <span id="itcdesc[]"></span>
                                    <input type="hidden" name="itcid[]" value="">
                                </td>
                                <td align="center"><?php echo campo_data2('icodtinicioitem[]', 'S', $habilita, '', 'S', '', ' verificaDataInicio(this); ', '', ' verificaDataInicio(this); '); ?></td>
                                <td align="center"><?php echo campo_data2('icodterminoitem[]', 'S', $habilita, '', 'S', '', ' verificaDataTermino(this); ', '', ' verificaDataTermino(this); '); ?></td>
                                <td align="center"><?php echo campo_texto('icovlritem[]', 'S', $habilita, '', 14, 14, '', '', 'right', '', 0, 'onkeypress="calculoEdificacao.setDefaultValue( this )";', 'calculoEdificacao.manager( this );'); ?></td>
                                <td align="center"><?php echo campo_texto('icopercsobreestrutura[]', 'N', 'N', '', 5, 6, '', '', 'right', '', 0); ?> %</td>
                            </tr>-->
                        <? // } ?>
                    </tbody>
                    <tfoot>
                        <tr id="tr_total" bgcolor="#FFFFFF">
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td align="right"><strong>Total</strong></td>
                            <td align="center">
                                <input class='disabled' style="text-align:right" type='text' id='totalv' size='15' maxlength='14' value='<?php echo number_format($somav, 2, ',', '.'); ?>' disabled="disabled">
                            </td>
                            <td align="center">
                                <input class='disabled' style="text-align:right" type='text' id='total' size='6' maxlength='6' value='<?php echo number_format($soma, 2, ',', '.'); ?>' disabled="disabled"> %
                            </td>
                        </tr>
                        <tr id="tr_vlcontrato" bgcolor="#FFFFFF">
                            <td></td>
                            <td></td>
                            <td></td>
                            <td></td>
                            <td align="right"><strong><?php if($obraMI){echo "Valor do Cronograma Padrão";}else{echo "Valor do Contrato";}?></strong></td>
                            <td align="center">
                                <input class='disabled' style="text-align:right" type='text' name='vl_contrato' id='vl_contrato' size='15' maxlength='14' value='<?php echo number_format($ocrvalorexecucao, 2, ',', '.'); ?>' disabled="disabled">
                            </td>
                            <td align="center">
                                <input class=disabled style="text-align:right" type='text' id='vl_porcento' size='6' maxlength='6' value='100,00' disabled="disabled"> %
                            </td>
                        </tr>
                        <tr id="tr_vlrestante" bgcolor="#FFFFFF">
                            <td colspan="4">
                                <a href="#" onclick="inserirEtapas();">
                                    <img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Etapa">&nbsp;&nbsp;Inserir Etapa
                                </a>
                            </td>
                            <td align="right"><strong>Valor Restante</strong></td>
                            <td align="center">
                                <input class='disabled' style="text-align:right" type='text' id='rest_totalv' size='15' maxlength='14' value='<?php echo number_format($ocrvalorexecucao - $somav, 2, ',', '.'); ?>' disabled="disabled">
                            </td>
                            <td align="center">
                                <input class='disabled' style="text-align:right" type='text' id='rest_total' size='6' maxlength='6' value='<?php echo number_format(100 - $soma, 2, ',', '.'); ?>' disabled="disabled"> %
                            </td>
                        </tr>
                    </tfoot>
                </table>
            </td>
            <td rowspan="3" align="right" valign="top" width="1">
            </td>
        </tr>        

            <tr bgcolor="#C0C0C0">
                <td colspan="2">
                    <center>
                        <input type="button" id="salvar" value="Salvar" style="cursor: pointer" onclick="validaForm();">
                        <input type="submit" id="cancelar" name="cancelar" value="Cancelar Edição" style="cursor: pointer" >
                    </center>
                </td>
            </tr>
    </table>
</form>
<script type="text/javascript">

    function verificaDataInicio(data) {
        var obData = new Data();

        if (obData.comparaData(data.value, $('#crtdatainicioLabel').html(), '<')) {
            alert('A data de início não pode ser menor que a data de início do projeto.');
            data.value = '';
            data.focus();
            return;
        }

        if (obData.comparaData(data.value, $('#crtdatafimLabel').html(), '>')) {
            alert('A data de início não pode ser maior que a data de término do projeto.');
            data.value = '';
            data.focus();
            return;
        }

        // verificando se a data de inicio não é maior que a data de termino
        var nome = ($(data).attr('id')).split('_');
        if (nome[1]) {
            if (obData.comparaData(data.value, $('#icodterminoitem_' + nome[1]).val(), '>')) {
                alert('A data de início não pode ser maior que a data de término.');
                data.value = '';
                data.focus();
                return;
            }
        }

//    if( obData.comparaData(data.value,$('#eobinicioLabel').html(),'<') ){
//        if(confirm("A data de início é maior que a data de início da estrutura, deseja altera-lá?")) {
//            $('#eobinicioLabel').html(data.value);
//            $('#eobinicioLabel').css('color','red');
//            $('#dtInicioObra').val(data.value);
//        }else{
//            data.value = '';
//        }
//    }

    }


    function validaForm() {
        var obCalc = new Calculo();
        var result = true;
        var msg = '';

        var arDtInicio = new Array();
        $('[name*=icodtinicioitem]').not(':last')
                .each(function(i, obj) {
                    arDtInicio.push(obj.value);
                });
        if (jQuery.inArray('', arDtInicio) >= 0) {
            msg += '* O campo Data de Início é de preenchimento obrigatório.\n';
            result = false;
        }

        var arDtTermino = new Array();
        $('[name*=icodterminoitem]').not(':last')
                .each(function(i, obj) {
                    arDtTermino.push(obj.value);
                });
        if (jQuery.inArray('', arDtTermino) >= 0) {
            msg += '* O campo Data de Término é de preenchimento obrigatório.\n';
            result = false;
        }

        $('[name*=icodtinicioitem]').not(':last').each(function(i, obj) {
            var id = new String($(obj).attr('id')).split('_');
            id = id[1];

            var dtInicio = new String($(obj).val()).split('/');
            dtInicio = new Date(dtInicio[2], new Number(dtInicio[1]) - 1, dtInicio[0]);

            var dtTermino = new String($('#icodterminoitem_' + id).val()).split('/');
            dtTermino = new Date(dtTermino[2], new Number(dtTermino[1]) - 1, dtTermino[0]);

            if (dtInicio > dtTermino) {
                msg += '* Nenhuma Data de Início pode ser maior que a Data de Término.\n';
                result = false;
                return false;
            }
        });

        if ($('#croobs').val() == '') {
            msg += 'O campo justificativa é obrigatório.\n';
        }

        if($('#totalv').val() != $('#vl_contrato').val()){
            msg += 'A soma do valor das etapas deve ser igual ao valor do contrato.\n';
        }

        if (msg != '') {
            alert('Validação do formulário:\n\n' + msg);
        } else {
            $('#evt').val('salvar');
            document.formulario.submit();
        }
    }

<?php
$itens   = new ItensComposicaoObras();
$arItens = $itens->getItemComposicaoByCronograma($cronograma->croid);

if (is_array($arItens) && count($arItens) > 0) {
    foreach ($arItens as $key => $data) {
        $data['icodtinicioitem'] = formata_data($data['icodtinicioitem']);
        $data['icodterminoitem'] = formata_data($data['icodterminoitem']);
        $data = array_map('utf8_encode', $data);
        echo "gerenteEdificacao.addLine(" . simec_json_encode($data) . "); ";
    }
}
?>
</script>

<?php

        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formulario'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#btnGerarXls').attr('disabled', false);
                      jQuery('#xls').attr('disabled', false);
                   }
                 ";
            echo '</script>';
        }
?>

<?php
/**
 * Demanda #231610
 */
if($obraMI){
    $ordemServico = new OrdemServicoMI();
    $dados_os_espelho = $ordemServico->getArrayServicosExternosImplantacao($obrid);
    
    if(!empty($dados_os_espelho)){
?>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td>
                   <center><label class="TituloTela" style="color:#000000;">Serviços Externos</label></center>
                </td>
            </tr>
            <tr>        
                <td>
                    <table class="tabela listagem" style="text-align: center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
                         <thead>
                             <tr>
                                 <td><b>Item</b></td>
                                 <td><b>Valor Unitário</b></td>
                                 <td><b>Quantidade</b></td>
                                 <td><b>Unidade de medida</b></td>
                                 <td><b>Valor Total</b></td>
                             </tr>
                         </thead>
                         <tbody>
                             <?php
                                $vl_tot = 0;
                                foreach ($dados_os_espelho as $servico) {
                                    echo '<tr>';
                                    echo '<td>'.$servico['item'].'</td>';
                                    echo '<td>'.$servico['valor_unitario'].'</td>';
                                    echo '<td>'.$servico['quantidade'].'</td>';
                                    echo '<td>'.$servico['unidade_medida'].'</td>';
                                    echo '<td>'.$servico['valor_total'].'</td>';
                                    echo '</tr>';

                                    $vl = str_replace('.', '', $servico['valor_total']);
                                    $vl = str_replace(',', '.', $vl);
                                    $vl = (float)$vl;
                                    $vl_tot += $vl;
                                }
                                echo ' 
                                     <tr>
                                         <th><b>TOTAL</b></th>
                                         <td>&nbsp;</td>
                                         <td>&nbsp;</td>
                                         <td>&nbsp;</td>
                                         <td>'.number_format($vl_tot,2,',','.').'</td>
                                     </tr>  
                                     ';
                             ?>
                         </tbody>
                    </table>
                </td>
            </tr>
        </table>
<?php }} 

    ?>
