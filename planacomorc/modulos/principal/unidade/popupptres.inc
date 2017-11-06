<?php

/**
 * $Id: popupptres.inc 102359 2015-09-11 18:26:07Z maykelbraz $
 *
 */
if ($_REQUEST['detalhe']) {
    include APPRAIZ . "planacomorc/modulos/principal/unidade/modal-ptres-grafico.inc";
    die();
}

require_once APPRAIZ . "www/planacomorc/_funcoes.php";
require_once APPRAIZ . "www/planacomorc/_constantes.php";
require_once APPRAIZ . "www/planacomorc/_funcoespi.php";

if (isAjax() && isset($_POST['prgcod']) && isset($_POST['comboacao'])) {

    $sql = "SELECT ac.acacod as codigo, (ac.acacod || ' - ' || ac.acadsc) as descricao
            FROM monitora.acao ac
            WHERE
                ac.unicod IS NOT NULL AND
                ac.prgano = '" . $_SESSION['exercicio'] . "' AND
                ac.acastatus = 'A' AND
                ac.acasnrap = false AND
                ac.prgcod = '" . (int) $_POST['prgcod'] . "'
            GROUP BY ac.acacod, ac.acadsc
            ORDER BY ac.acacod";
    $db->monta_combo('acacod', $sql, 'S', 'Selecione', '', '', '', 400);
    exit;
}

// verificando se é undidade ou unidade gestora
$where .= $_POST['prgcod']? "AND UPPER(ptr.prgcod) LIKE('%" . strtoupper($_POST['prgcod']) . "%')" : '';
$where .= $_POST['acacod']? "AND UPPER(aca.acacod) LIKE('%" . strtoupper($_POST['acacod']) . "%')" : '';
$where .= $_REQUEST['unicod']? " AND aca.unicod = '{$_REQUEST['unicod']}'" : '';
$where .= $_REQUEST['ungcod']? " AND uni.suocod = '{$_REQUEST['ungcod']}' ": '';
$where .= $_POST['buscalivre']? "AND (TRIM(aca.prgcod||'.'||aca.acacod||'.'||aca.loccod||' - '||aca.acadsc) ILIKE('%" . $_POST['buscalivre'] . "%') OR dtl.ptres ILIKE '%" . $_POST['buscalivre'] . "%')": '';

if($_REQUEST['no_ptrid']){
    if(is_array($_REQUEST['no_ptrid'])){
        $where .= " AND ptr.ptrid NOT IN('". join($_REQUEST['no_ptrid'], "','"). "')";
    } else {
        $where .= " AND ptr.ptrid != ". (int)$_REQUEST['no_ptrid'];
    }
}
//ver($where);

$sql_lista = <<<DML
    SELECT
        ptr.ptrid,
        ptr.ptres || '<autorizadocusteio>' || COALESCE(ptr.ptrdotacaocusteio, 0.00) || '</autorizadocusteio><autorizadocapital>' || COALESCE(ptr.ptrdotacaocapital, 0.00) || '</autorizadocapital>' AS ptres,
        TRIM(aca.prgcod) || '.' || TRIM(aca.acacod) || '.' || TRIM(aca.loccod) || '.' || (CASE WHEN LENGTH(TRIM(aca.acaobjetivocod)) <= 0 THEN '-' ELSE COALESCE(TRIM(aca.acaobjetivocod), '') END) || '.' || TRIM(ptr.plocod) || ' - ' || aca.acatitulo || ': ' || ptr.plodsc AS descricao,
        SUM(COALESCE(ptr.ptrdotacao, 0.00)) AS dotacaoatual,
        COALESCE(SUM(pip.pipvalor), 0.00) AS det_pi,
        SUM(COALESCE(ptr.ptrdotacaocusteio, 0.00)) - COALESCE(SUM(pip.custeio), 0.00) AS nao_det_pi_custeio,
        SUM(COALESCE(ptr.ptrdotacaocapital, 0.00)) - COALESCE(SUM(pip.capital), 0.00) AS nao_det_pi_capital,
        COALESCE((pemp.total), 0.00) AS empenhado,
        SUM(COALESCE(ptr.ptrdotacao, 0.00)) - COALESCE(pemp.total, 0.00) AS nao_empenhado
    FROM monitora.ptres ptr
        JOIN monitora.acao aca on ptr.acaid = aca.acaid
        JOIN public.vw_subunidadeorcamentaria uni ON(aca.unicod = uni.unocod AND ptr.ptrano = uni.prsano)
        JOIN spo.ptressubunidade psu ON(ptr.ptrid = psu.ptrid AND uni.suoid = psu.suoid)
    LEFT JOIN (
        SELECT
            pip2.ptrid,
            SUM(pipvalor) AS pipvalor,
            SUM(picvalorcusteio) AS custeio,
            SUM(picvalorcapital) AS capital
        FROM monitora.pi_planointernoptres pip2
            JOIN monitora.pi_planointerno USING(pliid)
            JOIN planacomorc.pi_complemento pc USING(pliid)
        WHERE
            plistatus = 'A'
        GROUP BY
            pip2.ptrid
    ) pip on ptr.ptrid = pip.ptrid
    LEFT JOIN (
        SELECT
            ex.unicod,
            ex.ptres,
            ex.exercicio,
            sum(ex.vlrempenhado) AS total
        FROM spo.siopexecucao ex
        WHERE
            ex.exercicio = '{$_SESSION['exercicio']}'
        GROUP BY
            ex.unicod,
            ex.ptres,
            ex.exercicio
        ) pemp ON (pemp.ptres = ptr.ptres AND pemp.exercicio = ptr.ptrano AND pemp.unicod = ptr.unicod)
    WHERE
        ptr.ptrano = '{$_SESSION['exercicio']}'
        $where
        AND ptr.ptrstatus = 'A'
    GROUP BY
        ptr.ptrid,
        ptr.ptres,
        aca.prgcod,
        aca.acacod,
        aca.loccod,
        aca.acaobjetivocod,
        ptr.plocod,
        aca.unicod,
        uni.unonome,
        aca.acatitulo,
        pemp.total
    ORDER BY
        ptr.ptres
DML;
ver($sql_lista);
?>
<html>
    <head>
        <script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
        <script type="text/javascript" lang="JavaScript">
            function parametrosFiltro()
            {
                var params = {};
                $('#modal-ptres #formularioPopup select').each(function() {
                    var inputName = $(this).attr('name'),
                    inputValue = $(this).val();
                    params[inputName] = inputValue;
                });
                $('#modal-ptres #formularioPopup input:hidden').each(function() {
                    var inputName = $(this).attr('name'),
                    inputValue = $(this).val();
                    params[inputName] = inputValue;
                });
                return params;
            }

            function atualizaListagemPTRES(params){
                $('#modal-ptres .modal-body').html('');
                $.post('planacomorc.php?modulo=principal/unidade/popupptres&acao=A&obrigatorio=n', params, function(response) {
                    $('#modal-ptres .modal-body').html(response);
                    //console.log(response);
                    delegatePaginacao();
                    $('#modal-ptres .chosen-select').chosen();
                    $('#modal-ptres .chosen-container').css('width', '100%');
                });
            }

            function delegatePaginacao(){
                $('.container-listing').on('click', 'li[class="pgd-item"]:not(".disabled")', function() {
                    // -- definindo a nova página
                    var novaPagina = $(this).attr('data-pagina');
                    var params = parametrosFiltro();
                    params['listagem[p]'] = novaPagina;
                    atualizaListagemPTRES(params);
                });
            }

            function apresentaLinhasSelecionadas(tabelaOrigem, apartirDe) {
                var val;
                for (i = apartirDe; i < tabelaOrigem.find('tr').length; i++) {
                    val = tabelaOrigem.find('tr')[i].id.split("_");
                    //alert(val);
                    if (document.getElementById(val[1])) {
                        campo = $('tr#' + val[1] + ' td:nth-child(2)').find('span');
                        //alert(campo);
                        campo.attr('class', 'glyphicon glyphicon-remove');
                        campo.attr('title', 'Remover item');
                        campo.css('color', 'gray');
                    }
                }
            }

            $(document).ready(function(){
                apresentaLinhasSelecionadas($('#orcamento'), 2);

                $('#modal-ptres .btn-info').click(function(e) {
                    e.stopPropagation();
                    var params = parametrosFiltro();
                    atualizaListagemPTRES(params);
                    return false;
                });
            });
        </script>
    </head>
    <body>
        <div class="col-lg-12">
            <div class="well">
                <fieldset>
                    <form id="formularioPopup" name="formularioPopup" method="post" action="" class="form-horizontal">
                        <input type="hidden" name="unicod" value="<?= $_REQUEST['unicod'] ?>">
                        <input type="hidden" name="ungcod" value="<?= $_REQUEST['ungcod'] ?>">
                        <input type="hidden" name="no_ptrid" value="<?= $_REQUEST['no_ptrid'] ?>">
                        <div class="form-group">
                            <label for="inputUnidade" class="col-lg-2 control-label">Programa:</label>
                            <div class="col-lg-10">
                                <?php
                                    $sql = "
                                        SELECT
                                            p.prgcod as codigo,
                                            (p.prgcod || ' - ' || p.prgdsc) as descricao
                                        FROM monitora.programa p
                                        WHERE
                                            p.prgano = '" . $_SESSION['exercicio'] . "'
                                        ORDER BY
                                            prgcod";
                                    $db->monta_combo('prgcod', $sql, 'S', 'Selecione', 'carregaAcao', null, null, 400, 'N', 'prgcod', null, (isset($_POST['prgcod']) ? $_POST['prgcod'] : null), null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                                ?>
                            </div>
                        </div>
                        <div class="form-group">

                            <label for="inputUnidade" class="col-lg-2 control-label">Funcional:</label>
                            <div class="col-lg-10 comboAcao">
                                <?php
                                    $sql = "
                                        SELECT
                                            ac.acacod as codigo,
                                            (ac.acacod || ' - ' || ac.acadsc) as descricao
                                        FROM monitora.acao ac
                                        WHERE
                                            ac.unicod IS NOT NULL
                                            AND ac.prgano = '" . $_SESSION['exercicio'] . "'
                                            AND ac.acastatus = 'A'
                                            AND ac.acasnrap = FALSE
                                        GROUP BY
                                            ac.acacod,
                                            ac.acadsc
                                        ORDER BY
                                            ac.acacod";
                                    $db->monta_combo('acacod', $sql, 'S', 'Selecione', '', null, null, 400, 'N', 'acacod', null, (isset($_POST['acacod']) ? $_POST['acacod'] : null), null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                                ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-lg-10 col-lg-offset-2">
                                <input type="submit" id="btnPopupPtresPesquisar" class="btn btn-info" value="Pesquisar" name="botao" />
                            </div>
                        </div>
                    </form>
                </fieldset>
            </div>
        </div>
        <br>
        <?php
        $listagem = new Simec_Listagem();
        if ($_REQUEST['tipo'] == 'pi') {
            $listagem->setCabecalho(array(
                "PTRES",
                "Funcional",
                "Dotação Atual(R$)",
                '<center>Empenho (R$)</center>' => array('Empenhado', 'Não Empenhado'),
            ));
        } else {
            $listagem->setCabecalho(array(
                "PTRES",
                "Funcional",
                "Dotação Atual(R$)",
                '<center>Detalhado em PI (R$)</center>' => array(
                    'Detalhado',
                    'Não Detalhado Custeio',
                    'Não Detalhado Capital'),
                '<center>Empenho (R$)</center>' => array('Empenhado', 'Não Empenhado')
            ));
        }

        $listagem->setQuery($sql_lista);
        $listagem->addAcao('select', array('func' => 'resultado', 'extra-params' => array('acao')));
        $listagem->setIdLinha();
        $listagem->addCallbackDeCampo(array(
            'dotacaoatual',
            'det_subacao',
            'nao_det_subacao',
            'det_pi',
            'nao_det_pi',
            'nao_det_pi_custeio',
            'nao_det_pi_capital',
            'empenhado',
            'nao_empenhado',
            'saldo'), 'mascaraMoeda');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        ?>
        <script type="text/javascript">
            function carregaAcao(prgcod) {
                url = "planacomorc.php?modulo=principal/unidade/listarProgramaUG&acao=A";
                $.ajax({
                    url:url,
                    type: 'POST',
                    async: false,
                    data: {prgcod: prgcod, comboacao:true},
                    success: function(html) {
                        $('.comboAcao').html(html);
                        $(".chosen-select").chosen();
                        $('.comboAcao').find('select').removeAttr('style');
                        $('.comboAcao').find('select').addClass('chosen-select form-control').chosen();
                    },
                    error: function (jqXHR, textStatus, errorThrown) {
                        console.log(jqXHR);
                        console.log(textStatus);
                        console.log(errorThrown);
                    }
                });
            }

            /* CARREGANDO OS DADOS DE PTRES */
            /*var tabelaorigem = document.getElementById('orcamento');
            for (i = 2; i < tabelaorigem.rows.length - 2; i++) {
                if (document.getElementById("chk_" + tabelaorigem.rows[i].cells[0].innerHTML)) {
                    document.getElementById("chk_" + tabelaorigem.rows[i].cells[0].innerHTML).checked = true;
                }
            }*/
            /* FIM CARREGANDO OS DADOS DE PTRES */

            function desmarcarItem(id){
                itemMarcado = $('table#tb_render tr#'+ id + ' span.glyphicon.glyphicon-remove');
                $(itemMarcado).removeClass('glyphicon-remove');
                $(itemMarcado).addClass('glyphicon-ok');
            }

            function resultado(id, selecionado, adicionais) {
                var colHtmlPtres = $('tr#' + id + ' td:nth-child(2)').html();
                var ptres = colHtmlPtres.substr(0, colHtmlPtres.search('<'));

                if (!ptres) {
                    alert('Não existe PTRES. Entre em contato com o administrador do sistema.');
                    return false;
                }

                if (selecionado == false) {
                    // Retirando a marcação da tabela do modal de opções de PTRES
                    idTrDesmarcada = $('table#orcamento tr[id^="ptres_"]').attr('id');
                    if(idTrDesmarcada != undefined){
                        desmarcarItem(idTrDesmarcada.replace('ptres_', ''));
                    }
                    
                    // Retira o item selecionado anteriormente, caso exista na tabela orçamento.
                    $('table#orcamento tr[id^="ptres_"]').remove();
                    
                    var tabelaorigem = document.getElementById('orcamento');
                    var colunaPtres = document.getElementById('ptres_' + id);
                    if (colunaPtres != null) {
                        tabelaorigem.deleteRow(document.getElementById('ptres_' + id).rowIndex);
                    }

                    // -- Criando a nova linha na tabela da página de origem
                    var linha = tabelaorigem.insertRow(2);
                    linha.id = "ptres_" + id;
                    linha.style.height = '30px';

                    // -- O número da primeira linha da tabela onde serão inseridos os novos dados
                    var linha = 2;

                    // setando o ptres
                    var celPTRES = tabelaorigem.rows[linha].insertCell(0);
                    celPTRES.style.textAlign = "center"; 
                    celPTRES.innerHTML = '<span class="link btnVisualizarDetalhes" ptrid="'+ id+'" >'+ ptres+ '</span>';

                    var celFuncional = tabelaorigem.rows[linha].insertCell(1);
                    var funcional = $('tr#' + id + ' td:nth-child(3)').text();
                    celFuncional.innerHTML = '<span class="link btnVisualizarDetalhes" ptrid="'+ id+'" >'+ funcional+ '</span>';

                    var celDotacaoAtualPo = tabelaorigem.rows[linha].insertCell(2);
                    celDotacaoAtualPo.style.textAlign = "right";
                    var dotacaoAtualPO = $('tr#' + id + ' td:nth-child(4)').text();
                    celDotacaoAtualPo.innerHTML = dotacaoAtualPO;
                    
                    var vlrAutorizadoCusteio = textToFloat($('tr#' + id + ' td:nth-child(2) autorizadocusteio').text());
                    var vlrAutorizadoCapital = textToFloat($('tr#' + id + ' td:nth-child(2) autorizadocapital').text());
                    
                    $('#td_autorizado_funcional_custeio').text(number_format(vlrAutorizadoCusteio.toFixed(2), 2, ',', '.'));
                    $('#td_autorizado_funcional_capital').text(number_format(vlrAutorizadoCapital.toFixed(2), 2, ',', '.'));

                    var celDetalhadoPi = tabelaorigem.rows[linha].insertCell(3);
                    celDetalhadoPi.style.textAlign = "right";
                    celDetalhadoPi.id = "td_pi_detalhado";
                    var detalhado = $('tr#' + id + ' td:nth-child(5)').text();
                    celDetalhadoPi.innerHTML = detalhado;

                    $('#piDetalhado').val(detalhado);

                    var celNaoDetalhadoPi = tabelaorigem.rows[linha].insertCell(4);
                    celNaoDetalhadoPi.style.textAlign = "right";
                    celNaoDetalhadoPi.id = "td_pi_nao_detalhado";
                    var naoDetalhadoCusteio = $('tr#' + id + ' td:nth-child(6)').text();
                    var naoDetalhadoCapital = $('tr#' + id + ' td:nth-child(7)').text();
                    var naoDetalhado = textToFloat(naoDetalhadoCusteio) + textToFloat(naoDetalhadoCapital);
                    celNaoDetalhadoPi.innerHTML = number_format(naoDetalhado.toFixed(2), 2, ',', '.');
                    $('#piNaoDetalhado').val(number_format(naoDetalhado.toFixed(2), 2, ',', '.'));

                    $('#td_disponivel_funcional_custeio').text(naoDetalhadoCusteio);
                    $('#td_disponivel_funcional_capital').text(naoDetalhadoCapital);
                    $('#VlrFuncionalDisponivelCusteio').val(naoDetalhadoCusteio);
                    $('#VlrFuncionalDisponivelCapital').val(naoDetalhadoCapital);

                    var CelEmpenhado = tabelaorigem.rows[linha].insertCell(5);
                    CelEmpenhado.style.textAlign = "right";
                    var empenhado = $('tr#' + id + ' td:nth-child(8)').text();
                    CelEmpenhado.innerHTML = empenhado;

                    var celNaoEmpenhado = tabelaorigem.rows[linha].insertCell(6);
                    celNaoEmpenhado.style.textAlign = "right";
                    var naoEmpenhado = $('tr#' + id + ' td:nth-child(9)').text();
                    celNaoEmpenhado.innerHTML = naoEmpenhado + '<input type="hidden" name="ptrid" value="' +id +'" />';

                    var celValorDoProjeto = tabelaorigem.rows[linha].insertCell(7);
                    celValorDoProjeto.style.textAlign = "center";
                    celValorDoProjeto.id = "td_valor_projeto";
                    celValorDoProjeto.innerHTML = '';

                    // Atualizando o valor do projeto automaticamente e evitando que usuário altere o objetivo.
                    $.ajax({
                        url: window.location + '&recuperarObjetivoPorPtres=1',
                        type: "post",
                        data: {'ptrid': id},
                        async: false,
                        success: function(oppid){
                            if(oppid){
                                $('#oppid').val(oppid).prop('disabled', true).trigger("chosen:updated").prop('disabled', false).change();
                            } else {
                                $('#oppid').val('').prop('disabled', false).trigger("chosen:updated").change();
                            }
                        }
                    });
                    
                    var valorBaseLimiteDisponivelUnidade = textToFloat($('#VlrSUDisponivel').val());
                    var valorBaseProjeto = buscarValorBaseDoProjeto();
                    
                    var vlrBaseLimiteDisponivelUnidade = valorBaseLimiteDisponivelUnidade + valorBaseProjeto;
                    $('#VlrSUDisponivel').val(number_format(vlrBaseLimiteDisponivelUnidade.toFixed(2), 2, ',', '.'));

                    // Calcula valor do projeto declarado pelo usuário e informa na coluna valor do projeto.
                    atualizarValorDoProjeto();
                    mudarCorValorProjeto();
                    
                    // Valores já gravados na base e separados pra esse PI/Projeto
                    /**
                     * @todo verificar se é necessario subtrair pelo valor anterior da funcional 
                     */
                    $('#vlrPiCusteio').val('');
                    $('#vlrPiCapital').val('');
                    
                    $('#ptresCusteio').val(number_format(vlrAutorizadoCusteio.toFixed(2), 2, ',', '.'));
                    $('#ptresCapital').val(number_format(vlrAutorizadoCapital.toFixed(2), 2, ',', '.'));
                    $('#piNaoDetalhadoCusteio').val(naoDetalhadoCusteio);
                    $('#piNaoDetalhadoCapital').val(naoDetalhadoCapital);
                    
                    atualizarValorLimiteDisponivelFuncionalCapital();
                    atualizarValorLimiteDisponivelFuncionalCusteio();
                    atualizarValorDetalhado();
                    atualizarValorNaoDetalhado();
                    atualizarValorLimiteDisponivelUnidade();

                } else {
                    var tabelaorigem = document.getElementById('orcamento');

                    tabelaorigem.deleteRow(document.getElementById('ptres_' + id).rowIndex);
//                    calculovalorPI();
//                    //jogando ids desmarcados para verificar se existem no banco, se sim deletar vinculo.
//                    ptresApagar(id);
                    
                    desmarcarItem(id);
                }
                
                // fecha modal após a seleção
                $('#modal-ptres').modal('hide');
            }

            function ptresApagar(ptres) {
                var input = document.createElement("input");
                input.setAttribute("type", "hidden");
                input.setAttribute("name", "ids_apagar[]");
                input.setAttribute("value", ptres);
                document.getElementById("formulario").appendChild(input);
            }

            function detalhePtres(ptrid) {
                url = 'planacomorc.php?modulo=principal/ptres/detalheptresmodal&acao=A&ptrid=' + ptrid;
                if ($('#obrigatorio').val() == 'n') {
                    url = 'planacomorc.php?modulo=principal/unidade/detalheptresmodal&acao=A&ptrid=' + ptrid;
                }
                jQuery.ajax({
                    url: url,
                    async: false,
                    success: function(html) {
                        jQuery("#dialog").html(html);
                        jQuery("#dialog").dialog({
                            modal: true,
                            width: 900,
                            height: 400
                        });
                    }
                });
            }

        </script>
        <div id="dialog" />
    </body>
</html>
<?php
/* Fechar Conexão quando não houver rodapé. */
$db->close();
exit();
