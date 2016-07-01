<?php
// empreendimento || obra || orgao
verificaSessao('obra');

$_SESSION['obras2']['obrid'] = ( $_GET['obrid'] ? $_GET['obrid'] : $_SESSION['obras2']['obrid'] );
$obrid                       = $_SESSION['obras2']['obrid'];

function validaDataPHP($data) {
    $data = new DateTime($data);
}

if($_POST['xls'] == 'gerar'){
    gerarXlsItens();
}

if ($_POST['evt'] == 'salvar') {

    $cronograma = new Cronograma();
    $cronograma->cronogramaObra($obrid);

//    if ( $_POST['atualizarNivel'] == 1 ){
//        updateLevelCronograma( $_POST['vl_contrato_atualizado'], $crtid, $obrid, 'etapa');
//    }
    // ver( $_POST ,d);
    
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
    
    /**
     * Ao se fazer o update do cronograma, deve-se travar novamente o mesmo quando houver vistorias ou quando o GESTOR MEC/SUPERUSER travar no checkbox;
     */    
    if ($_POST['obrtravaedicaocronograma'] || $_POST['obrcronogramaservicocontratado'] || $_POST['obrcronogramaservicocontratadojustificativa']) {
        $arObra['obrtravaedicaocronograma'] = 't';

        if ($_POST['obrtravaedicaocronograma'] == 'N'){
            $arObra['obrtravaedicaocronograma'] = 'f';
        }

        $obra->popularDadosObjeto($arObra)->salvar(true, true, $arCamposNulo);
        $obra->commit();
    }


    $itens    = new ItensComposicaoObras();
    $allItens = $itens->getAllIcoid($obrid);

    $removeItens = array_diff($allItens, (is_array($_POST['icoid']))?$_POST['icoid']:array($_POST['icoid']) );

    if($_GET['acao'] != 'E') {
        $itens->desabilitarItensPeloIcoid($removeItens, $obrid);
    }

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
    
    /**
     * @description A aba de cronograma tem edição livre desde que a obra esteja em 
     *              contratação ou execução, então faz-se importante que seja gravado 
     *              no registro de atividades as alterações do cronograma.
     */
    
    $sql_empreendimento_id = 'SELECT emp.empid 
                              FROM obras2.empreendimento emp
                              INNER JOIN obras2.obras oi ON  oi.empid = emp.empid
                              WHERE  oi.obrid = '.$obrid;
    
    $empid = $db->pegaUm($sql_empreendimento_id);
    
    include_once APPRAIZ . "includes/classes/modelo/obras2/RegistroAtividade.class.inc";
    $regAtividade                 = new RegistroAtividade();
    $arDado                       = array();
    $arDado['arqid']              = null;
    $arDado['obrid']              = $obrid;
    $arDado['usucpf']             = $_SESSION['usucpf'];
    $arDado['empid']              = $empid;
    $arDado['rgadscsimplificada'] = 'Atualização dos dados do cronograma.';
    $arDado['rgadsccompleta']     = 'Foram modificados os seguintes itens do cronograma: ' . implode(' - ', $itens_modificados);
    $arDado['rgaautomatica']      = true;
    $arCamposNulo2                = array();

    if ( empty($arDado['arqid']) ){
        $arCamposNulo2[] = 'arqid'; 
    }

    $rga = $regAtividade->popularDadosObjeto( $arDado )
                        ->salvar(true, true, $arCamposNulo2);
    $regAtividade->commit();
    
    
    $vistoria        = new Supervisao();
    $ultima_vistoria = $vistoria->pegaUltSupidByObra($obrid);
    
    if($ultima_vistoria){
        $email = new Email();
        $email->enviaEmailEdicaoCronogramaObra($_SESSION['obras2']['obrid']);

        $cronograma = new Cronograma();
        $cronograma->verificaInconformidadesAtrasoCronograma($obrid);
    }
    
    if($_GET['acao'] == 'E'){
        die("<script>
                alert('Operação realizada com sucesso!');
                window.location.href = window.location.href;
             </script>");
    }else{
        die("<script>
                alert('Operação realizada com sucesso!');
                window.location = '?modulo=principal/etapas_da_obra&acao=A&obrid={$obrid}';
             </script>");
        
    }
    
            
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

$dadosContrato = $obraContrato->pegaDadosContratoObra($obrid);
$dtLimiteInicioEtapa = formata_data($dadosContrato['ocrdtordemservico']);
$dtLimiteFimEtapa = formata_data($dadosContrato['crtdttermino']);

$obrcronogramaservicocontratadojustificativa = $obra->obrcronogramaservicocontratadojustificativa;
$obrcronogramaservicocontratado              = $obra->obrcronogramaservicocontratado;

$docid = pegaDocidObra($obrid);
//$_GET['acao'] = 'V';

if($_GET['acao'] == 'E') {

    ?>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <?php
    $habilitado = false;
    $habilita = 'N';
    
} elseif ($_GET['acao'] != 'V') {
    // Inclusão de arquivos padrão do sistema
    include APPRAIZ . 'includes/cabecalho.inc';
    // Cria as abas do módulo
    echo '<br>';
    if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
        $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros);
    } else {
        $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros);
    }

    $contrato = new Contrato();
    $crtid    = $contrato->pegaCrtidPorObrid($obrid);

    if ($crtid) {
        $habilitado = true;
        $habilita = 'S';
    } else {
        $habilitado = false;
        $habilita = 'N';
    }
} else{

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
}

$desabilitaWF = false;

if($_GET['acao'] != 'E') {


    if (possui_perfil(array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO))) {
        $habilitado = false;
        $habilita = 'N';
    }

    if(possuiTravaCronograma($obrid)) {
        $habilitado = false;
        $habilita = 'N';
    }

    $esd = wf_pegarEstadoAtual($docid);
    if($esd['esdid'] == ESDID_OBJ_CONTRATACAO){
        $habilitado = true;
        $habilita = 'S';
    }

    if($obraMI){
        $habilitado = false;
        $habilita = 'N';
    }

    $flag_obrtravaedicaocronograma = $obra->getTravaCronograma($obrid);

    echo cabecalhoObra($obrid);

    echo alertaObraMi($obrid);

    monta_titulo($titulo_modulo, '<b>Após inserir, atualizar ou excluir uma etapa, clique em salvar.</b></td></tr><tr><td style="background: #f00; color: #fff"><img src="/imagens/atencao.png" /> Se o total dos itens do cronograma for diferente do valor do contrato lançado na aba Contratação, o sistema bloqueará a inserção de vistorias. Para a tramitação para execução o valor restante no cronograma deve ser sempre igual à zero.');

    echo cabecalhoCronograma($obrid);

}else{
    
    echo cabecalhoObra($obrid);
    monta_titulo($titulo_modulo, '<b>Após atualizar uma etapa, clique em salvar.</b>');
    $habilitado = true;
    $habilita = 'S';
    $desabilitaWF = true;
    
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">

    jQuery(document).ready(function() {
        jQuery('#aguarde').hide();
    });
    
    function gerarXls(){
         var formulario = document.formulario;
            document.getElementById('xls').value = 'gerar';
            formulario.submit();
    }

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
                                    location.href = '?modulo=principal/detalhe_da_etapa&acao=A&icoid=' + dataLine.icoid;
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

        if (obData.comparaData(data.value, '<?=$dtLimiteFimEtapa?>', '>')) {
            alert('A data de término não pode ser maior que a data de término do contrato.');
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

                            <? if($obra->getTravaCronograma($obrid) == 'f' && possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE))): ?>
                                <a id="editar_cronograma" href="/obras2/obras2.php?modulo=principal/editar_cronograma&acao=A&obrid=<?=$obrid?>">
                                    <img src="/imagens/lapis.png" style="cursor:pointer;" border="0" title="Editar Cronograma">&nbsp;&nbsp;Editar Cronograma
                                </a>
                            <? endif; ?>
                            <?php if ($habilitado && !$desabilitaWF): ?>
                                    <a href="#" onclick="inserirEtapas();">
                                        <img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Etapa">&nbsp;&nbsp;Inserir Etapa
                                    </a>
                            <?php endif; ?>
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
            <?php
                // Barra de estado WORKFLOW
                if ($habilitado && !$desabilitaWF) {
                    wf_desenhaBarraNavegacao($docid, array('obrid' => $obrid));
                }
            ?>
            </td>
        </tr>        
        <?php if($_GET['acao'] != 'E'){ ?>
            <tr>
                <td class="SubTituloDireita" width="30%" nowrap="nowrap">TODOS OS SERVIÇOS QUE COMPOEM A PLANILHA PACTUADA COM O FNDE FORAM CONTRATADOS?</td>
                <td>
                    <input type="radio" name="obrcronogramaservicocontratado" id="obrcronogramaservicocontratado_s" value="S" 
                    <?php echo (!$habilitado ? 'disabled="disabled"' : '' ) ?> 
                    <?php echo $obrcronogramaservicocontratado == 'S' ? 'checked="checked"' : '' ?> 
                    onclick="managerServicoContratado('S');">
                    <label for="obrcronogramaservicocontratado_s">Sim</label>
                    <input type="radio" name="obrcronogramaservicocontratado" id="obrcronogramaservicocontratado_n" value="N" 
                           <?= (!$habilitado ? 'disabled="disabled"' : '' ) ?> <?php echo $obrcronogramaservicocontratado == 'N' ? 'checked="checked"' : '' ?> onclick="managerServicoContratado('N');">
                    <label for="obrcronogramaservicocontratado_n">Não</label>
                </td>
            </tr>

            <?php
                if(possuiPerfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO)) && possuiTravaCronograma($obrid)){
            ?>
            <tr>
                <td class="SubTituloDireita" width="30%" nowrap="nowrap">CRONOGRAMA TRAVADO PARA EDIÇÃO DO SUPERVISOR ?</td>
                <td>
                    <input type="radio" name="obrtravaedicaocronograma" id="obrtravaedicaocronograma_s" value="S" 
                    <?php
                          echo $flag_obrtravaedicaocronograma == 't' ? 'checked="checked"' : '' ?> >
                    <label for="obrtravaedicaocronograma_s">Sim</label>
                    <input type="radio" name="obrtravaedicaocronograma" id="obrtravaedicaocronograma_n" value="N" 
                    <?php
                          echo ($flag_obrtravaedicaocronograma == 'f' || $flag_obrtravaedicaocronograma == NULL) ? 'checked="checked"' : '' ?> >
                    <label for="obrtravaedicaocronograma_n">Não</label>
                </td>
            </tr>

            <?php } ?>

            <tr id="tr_servico_contratado_justificativa" style="display: <?php echo ($obrcronogramaservicocontratado == 'N' ? '' : 'none') ?>">
                <td class="SubTituloDireita" width="30%" nowrap="nowrap" valign="top">Justificativa:</td>
                <td>
                <?php
                    echo campo_textarea('obrcronogramaservicocontratadojustificativa', 'S', 'S', '', '70', '8', '2000', '', 0, '', '', false, '', '');
                ?>
                </td>
            </tr>
            <?php
            if ($habilitado || (possuiPerfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO)) && possuiTravaCronograma($obrid))):
                ?>
                    <tr bgcolor="#C0C0C0">
                        <td colspan="2">
                            <!--<div style="float: left; margin-left: 150px;">-->
                            <center>
                                <input type="button" id="salvar" value="Salvar" style="cursor: pointer" onclick="validaForm();">
                                <input type="button" value="Voltar" style="cursor: pointer" onclick="location.href = '?modulo=principal/cadObra&acao=A'">
                            </center>
                            <!--</div>-->
                        </td>
                    </tr>
            <?php
        endif;
        ?>
        <tr bgcolor="#C0C0C0">
            <td colspan="2">
                    <center><input type="button" id="btnGerarXls" value="Gerar XLS" style="cursor: pointer" onclick="javascript: gerarXls()"></center>
            </td>
        </tr>        
        <?php }else{ 
            if ($habilitado):
                ?>
                    <tr bgcolor="#C0C0C0">
                        <td colspan="2">
                            <center>
                                <input type="button" id="salvar" value="Salvar" style="cursor: pointer" onclick="validaForm();">
                                <input type="button" value="Voltar" style="cursor: pointer" onclick="javascript: window.close()">
                            </center>
                        </td>
                    </tr>
            <?php
        endif;         
        } ?>        
    </table>
        <?php
//            $itens   = new ItensComposicaoObras();
//            $arItens = $itens->getItemComposicaoByObra($obrid);
        ?>
</form>

<script type="text/javascript">
    function managerServicoContratado(valor) {
        if (valor == 'N') {
            $('#tr_servico_contratado_justificativa').show();
        } else {
            $('#tr_servico_contratado_justificativa').hide();
            $('#obrcronogramaservicocontratadojustificativa').val('');
        }
    }

    function verificaDataInicio(data) {
        var obData = new Data();

        if (obData.comparaData(data.value, '<?=$dtLimiteInicioEtapa?>', '<')) {
            alert('A data de início não pode ser menor que a data de assinatura da ordem de serviço.');
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

            var dtTermino = new String($(obj).parent().parent().find('#icodterminoitem_' + id).val()).split('/');
            dtTermino = new Date(dtTermino[2], new Number(dtTermino[1]) - 1, dtTermino[0]);

            if (dtInicio > dtTermino) {
                msg += '* Nenhuma Data de Início pode ser maior que a Data de Término.\n';
                result = false;
                return false;
            }
        });

        if ($('[name=obrcronogramaservicocontratado]:checked').val() == 'N') {
            if ($('#obrcronogramaservicocontratadojustificativa').val() == '') {
                msg += 'O campo justificativa é obrigatório.\n';
            }
        }

        if (msg != '') {
            alert('Validação do formulário:\n\n' + msg);
        } else {
            $('#evt').val('salvar');
            document.formulario.submit();
        }
    }

<?php
$cronograma = new Cronograma();
$itens   = new ItensComposicaoObras();
$arItens = $itens->getItemComposicaoByObra($obrid);

if (is_array($arItens) && count($arItens) > 0) {
    foreach ($arItens as $key => $data) {

        $item = $cronograma->verificaCronogramaExecucao($obrid, $data['icoid']);

        if($item[0]['situacao'] == 'V')
            $data['itcdesc'] = "<span class='item-vermelho' style='color: #EF0303; font-weight: ;' title='{$item[0]['observacao']}'>" . $data['itcdesc'] . "</span>";
        else if($item[0]['situacao'] == 'D')
            $data['itcdesc'] = "<span class='item-amarelo' style='color: #f0ad4e; font-weight: ;' title='{$item[0]['observacao']}'>" . $data['itcdesc'] . "</span>";

        $data['icodtinicioitem'] = formata_data($data['icodtinicioitem']);
        $data['icodterminoitem'] = formata_data($data['icodterminoitem']);
//         $data = array_map('utf8_encode', $data);
        echo "gerenteEdificacao.addLine(" . simec_json_encode($data) . "); ";
    }
}
?>

    $(function(){
        $('.item-vermelho').parents('td').next().next().find('input').css({borderColor: '#EF0303',borderLeft: '#EF0303 3px solid'})
        $('.item-amarelo').parents('td').next().next().find('input').css({borderColor: '#f0ad4e',borderLeft: '#f0ad4e 3px solid'})
    });
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

<?php if($_GET['acao'] == 'E') { ?>

<script type="text/javascript">

    jQuery(document).ready(function() {
        
        setTimeout(function() {
        
        var tabela = $('#tabela_etapas');
        
        var str = '';
        
        $('[name^=icovlritem]').each(function(){
            $(this).attr('disabled', 'disabled');
        });
        $('[name^=icopercsobreestrutura]').each(function(){
            $(this).attr('disabled', 'disabled');
        });
        
        $('#tabela_etapas').find('[id*=tr_etapa_]').each(function(i, obj) {
            $(obj).find('td:eq(0)').html('');
            $(obj).find('td:eq(1)').html('');
        });
        var c1 = 0;
//        //Remove 2 primeiras colunas do cabeçalho
        $('#tabela_etapas > thead > tr > td').each(function(){
            if(c1 < 2){
                $(this).remove();
            }
            c1++;
        });
        
        $('#tabela_etapas').find('[id*=tr_etapa_]').each(function(i, obj) {
            $(obj).find('td:eq(0)').remove('');
            $(obj).find('td:eq(0)').remove('');
        });
        
        c1 = 0;
        $('#tr_total > td').each(function(){
            if(c1 < 2){
                $(this).remove();
            }
            c1++;
        });
        c1 = 0;
        $('#tr_vlcontrato > td').each(function(){
            if(c1 < 2){
                $(this).remove();
            }
            c1++;
        });
        
        $('#tr_vlrestante > td').eq(0).attr('colspan', '2');
        
    });
    
    }, 500);
    
</script>

<?php  } ?>

<? // ver($arItens); ?>

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


function gerarXlsItens() {
    global $db;
    
    $obrid   = $_SESSION['obras2']['obrid'];
    $itens   = new ItensComposicaoObras();
    $arItens = $itens->getItemComposicaoByObra($obrid);
    $arItens = !empty($arItens) ? $arItens : array();
    
    $arrCronograma = array();
    $vl_total_crono = 0;
    foreach ($arItens as $key => $value) {
        $vl_total_crono += $value['icovlritem'];
    }
    
    foreach ($arItens as $key => $value) {
        $data_i = date_create($value['icodtinicioitem']); 
        $data_i = date_format ( $data_i, 'd/m/Y' );
        $data_f = date_create($value['icodterminoitem']); 
        $data_f = date_format ( $data_f, 'd/m/Y' );
        $vl     = number_format($value['icovlritem'], 2, ',', '.');
        if($vl_total_crono != 0){
            $percent = $value['icovlritem']*100/$vl_total_crono;
            $percent = number_format($percent, 2, ',', '.');
        }else{
            $percent = '0,00';
        }
        $vl2     = number_format($vl_total_crono, 2, ',', '.');
        $arrCronograma[] = array($value['itcdesc'], $data_i, $data_f, $vl, $percent, $vl2);
    }

//    Não necessário por enquanto
//    $obraContrato = new ObrasContrato();
//    $obra         = new Obras($obrid);
//    $obraMI       = ($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C) ? true : false;
//    if ($obraMI) {
//        $ordemServico = new OrdemServicoMI();
//        $dados_os_espelho = $ordemServico->getArrayServicosExternosImplantacao($obrid);
//    }
    
    $cabecalho = array( "Descrição do Item", "Data de Início", "Data de Término", "Valor do Item", "(%) Referente a Estrutura", "Total Obra");
    
    ob_clean();
    ini_set("memory_limit", "512M");
    header("Content-type: application/excel; name=CronogramaObra.xls");
    header("Content-Disposition: attachment; filename=CronogramaObra.xls");
    $db->sql_to_xml_excel($arrCronograma, 'CronogramaObra', $cabecalho);
}

    ?>


<?php

/**
 * Verifica se o cronograma possui alguma trava de edição.
 * O cronograma da obra convencional deve ser travado para modificações pelo supervisor unidade após a primeira vistoria.
 *
 * @param int $obrid
 * @return boolean - TRUE não pode editar, FALSE pode
 */
function possuiTravaCronograma($obrid){
    /**
    -> Verificar se possui vistorias;
     */

    $obra     = new Obras($obrid);
    $vistoria = new Supervisao();

    $ultima_vistoria               = $vistoria->pegaUltSupidByObra($obrid);

    if($ultima_vistoria){
        return true;
    }

    return false;

}

?>

<script type="text/javascript">

    $(function(){
        $('#editar_cronograma').click(function(){
            return confirm("É necessário inserir uma vistoria para concluir a edição do cronograma.\nDeseja realmente editar o cronograma?")
        })
    })

</script>