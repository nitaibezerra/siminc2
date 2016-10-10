<?php
/**
 * Listagem de pedidos de alteração de Limites Orçamentários.
 * @version $Id: listar.inc 102361 2015-09-11 18:50:20Z maykelbraz $
 */
/**
 * Cabeçalho padrão do SIMEC.
 */
include APPRAIZ . "includes/cabecalho.inc";
/**
 * @todo REMOVER REFERENCIA DO RECORC
 */
include APPRAIZ . "www/recorc/_funcoes.php";

$perfis = pegaPerfilGeral();
if (isset($_REQUEST['aviso'])) {
    setFlashMessage(false, $_REQUEST['aviso']);
}
/* Marcando com o Default o status Análise SPO para o Perfil CGO/Equipe Orçamentária */
if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)) {
    if (!chaveTemValor($_REQUEST, 'wrfstatus') && $_REQUEST ['wrfstatus'] != 'todos' && $_REQUEST ['wrfstatus'] != 'undefined') {
        $_REQUEST['wrfstatus'] = STDOC_ANALISE_SPO;
    }
}
// -- Tratamento de perfis
$perfis = pegaPerfilGeral();

if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
    $sqlUO = <<<DML
        EXISTS (SELECT 1
         FROM progorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = uni.unicod)
DML;
    $where[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_UO_EQUIPE_TECNICA);
    $whereUO = " AND {$whereUO}";
}

if (in_array(PFL_SECRETARIA, $perfis)) {
    $sqlSECRETARIA = <<<DML
EXISTS (SELECT 1
         FROM progorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = uni.unicod)
DML;
    $where[] = $whereSecretaria = sprintf($sqlSECRETARIA, $_SESSION['usucpf'], PFL_SECRETARIA);
    $whereSecretaria = " AND {$whereSecretaria}";
}
if ($_REQUEST ['execucao'] == 'xls') {
    $where = array();
    // $where [] = "vie.exercicio = '{$_SESSION['exercicio']}'";
    if (chaveTemValor($_REQUEST, 'unicod')) {
        $where [] .= "uni.unicod = '{$_REQUEST['unicod']}'";
    }
    if (chaveTemValor($_REQUEST, 'anexo')) {
        if ($_REQUEST ['anexo'] != 'undefined') {
            $where [] .= " ccr.anxcod = '{$_REQUEST['anexo']}'";
        }
    }
    if (chaveTemValor($_REQUEST, 'wrfstatus')) {
        if ($_REQUEST ['wrfstatus'] != 'undefined' && $_REQUEST ['wrfstatus'] != 'todos') {
            $where [] .= " doc.esdid = '{$_REQUEST['wrfstatus']}'";
        }
    }
    if (chaveTemValor($_REQUEST, 'mes')) {
        $where [] .= " extract(month from pli.pdldatapedido) = '{$_REQUEST['mes']}'";
    }

    if (!empty($where)) {
        $where = 'WHERE ' . implode(' AND ', $where);
    } else {
        $where = 'WHERE 1=1 ';
    }
    $sql = <<<DML
        SELECT
            LPAD(pli.pdlid::varchar,5,'0')                             AS pedido,
            TO_CHAR(pli.pdldatapedido,'dd/mm/yyyy')                    AS data,
            pli.unicod|| ' - ' ||uni.unidsc                            AS unidade,
            ccr.anxcod                                                 AS anexo,
            pli.pdlreferencia                                          AS referencia,
            TO_CHAR(SUM(pde.pddvaloramplicacao), '999G999G999G990D99') AS ampliacao,
            TO_CHAR(SUM(pde.pddvalorreducao), '999G999G999G990D99')  AS reducao,
                case when  doc.esdid = 1070 or doc.esdid = 1102  then TO_CHAR(SUM(pde.pddvaloramplicacaoatendido), '999G999G999G990D99') else '0' end AS atendido_amplicacao,
                case when  doc.esdid = 1070 or doc.esdid = 1102 then TO_CHAR(SUM(pde.pddvalorreducaoatendido), '999G999G999G990D99') else '0' end AS atendido_reducao,
            esd.esddsc AS situacao,
            LPAD(pli.pdlnl::varchar,6,'0') as nl
        FROM
            progorc.pedidolimite pli
        LEFT JOIN
            progorc.pedidodetalhe pde
        ON
            pde.pdlid = pli.pdlid
        LEFT JOIN
            progorc.contacorrente ccr
        ON
            ccr.ccrid = pde.ccrid
        LEFT JOIN
            progorc.anexo ane
        ON
            ane.anxcod = ccr.anxcod
        INNER JOIN
            public.unidade uni
        ON
            uni.unicod = pli.unicod
        LEFT JOIN
            workflow.documento doc
        ON
            doc.docid = pli.docid
        LEFT JOIN
            workflow.estadodocumento esd
        ON
            esd.esdid = doc.esdid

        {$where}{$whereUO}{$whereSecretaria}
        GROUP BY
            pli.pdlid,
            ccr.anxcod,
            uni.unidsc,
            pli.unicod,
            pli.pdldatapedido,
            pli.docid,
            pli.pdlnl,
            doc.esdid,
            doc.docid,
            esd.esddsc,
            pli.pdlreferencia,
            pli.pdljustificativa
        ORDER BY pli.pdldatapedido DESC
DML;

    $cabecalho = array(
        "Pedido",
        "Data",
        "Unidade Orçamentária",
        "Anexo",
        "Ref.",
        "Ampliar",
        "Reduzir",
        "Atendido Ampliação",
        "Atendido Redução",
        "Situação",
        "NL"
    );
    ob_clean();
    header("Expires: 0");
    header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
    header("Pragma: no-cache");
    header("Content-type: application/xls; name=limite" . (date('d_m_Y')) . ".xls");
    header("Content-Disposition: attachment; filename=limite" . (date('d_m_Y')) . ".xls");
    header("Content-Description: MID Gera excel");
    $db->sql_to_excel($sql, 'limite' . (date('d_m_Y')), $cabecalho);

    $cabecalho = explode("&", $cabecalhoString);
    $align = explode("&", $alignString);

    $array_monta_lista = array(
        'sql' => $sql,
        'cabecalho' => $cabecalho,
        'align' => $align
    );
    return $array_monta_lista;
}

$formData = array();
if (chaveTemValor($_POST, 'action')) {
    // -- Coletando dados do formulário para processamentos
    $formData = $_POST ['data'];
    switch ($_POST ['action']) {
        case 'search' :
            break;
        case 'editar' :
            break;
    }
}
?>
<style>
    .center{text-align:center}
    .text-center{text-align:center}
    .table .amarelo{color:#DAA520 !important}
    th{text-align:center}
</style>
<script type="text/javascript" language="javascript">
    function editLimite(id, anexo) {
        window.location = "progorc.php?modulo=principal/limite/formulario&acao=A&execucao=lista&pdlid=" + id + "&anexo=" + anexo;
    }
    function deleteLimite(id, anexo) {
        if (confirm('Deseja realmente apagar o Pedido de Limite?')) {
            window.location = "progorc.php?modulo=principal/limite/formulario&acao=A&execucao=apagar&pdlid=" + id;
        }
    }

    jQuery(document).ready(function () {

        jQuery.expr[':'].contains = function (a, i, m) {
            return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
        };

        $("#textFind").keyup(function ()
        {
            $('table.table tbody tr td').removeClass('marcado');
            $('table.table tbody tr').removeClass('remover');
            stringPesquisa = $("#textFind").val();
            if (stringPesquisa) {
                //$('table.listagem tbody tr:contains('+stringPesquisa+')').addClass('voltar').removeClass('remover');
                $('table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                $('table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
            }
        });

        // -- Correção paliativa do bug #9920 do bootstrap.
        // -- Correção agendada para a versão 3.0.3 do bootstrap.
        // -- <https://github.com/twbs/bootstrap/issues/9920>
        $("input:radio").change(function () {
            $(this).prop('checked', true);
        });

        // -- Ação do botão buscar
        $('#buscar').click(function () {
            var uri = "progorc.php?modulo=principal/limite/listar&acao=A"
                    + '&unicod=' + $('#unicod').val()
                    + '&mes=' + $('#mes').val();
            var anexo = $('input[name=anexo]:checked').val();
            if ('todos' != anexo) {
                uri += '&anexo=' + anexo;
            }
            var wrfstatus = $('input[name=wrfstatus]:checked').val();
            uri += '&wrfstatus=' + wrfstatus;

            var aprstcc = $('input[name=aprstcc]:checked').val();
            uri += '&aprstcc=' + aprstcc;

            var referencia = $('select[name=pdlreferencia]').val();
            uri += '&ref=' + referencia;

            window.location = uri;
            return false;
        });
        // -- Ação do botão buscar
        $('#xls').click(function () {
            var uri = "progorc.php?modulo=principal/limite/listar&acao=A&execucao=xls"
                    + '&unicod=' + $('#unicod').val()
                    + '&mes=' + $('#mes').val();

            // -- Verificar porque ao clicar duas vezes em uma opção, o comando retorna undefined.
            var anexo = $('input[name=anexo]:checked').val();
            if ('todos' != anexo) {
                uri += '&anexo=' + anexo;
            }
            window.location = uri;
            return false;
        });
        $('#inserir').click(function () {
            var uri = "progorc.php?modulo=principal/limite/formulario&acao=A&execucao=lista"
                    + '&unicod=' + $('#unicod').val()
                    + '&mes=' + $('#mes').val();

//            // -- Verificar porque ao clicar duas vezes em uma opção, o comando retorna undefined.
            var wrfstatus = $('input[name=wrfstatus]:checked').val();
            if ('todos' != wrfstatus) {
                uri += '&wrfstatus=' + wrfstatus;
            }
            var anexo = $('input[name=anexo]:checked').val();
            if ('todos' != anexo) {
                uri += '&anexo=' + anexo;
            }
            validado = true;
            mensagem = '<b>';
            if ($('#unicod').val() == '') {
                mensagem += ' - Selecione uma Unidade Orçamentária (UO). <br/>';
                validado = false;
            }
            if ($('input[name=anexo]:checked').val() == '') {
                mensagem += ' - Selecione um Tipo. <br/>';
                validado = false;
            }
            if (validado) {
                window.location = uri;
            } else {
                {
                    bootbox.alert(mensagem +'</b>');
                }
            }

            return false;
        });
<?php if (chaveTemValor($_REQUEST, 'wrfstatus')): ?>
            $('#wrfstatus_<?php echo $_REQUEST['wrfstatus']; ?>').click();
<?php else: ?>
            $('#wrfstatus_todos').click();
<?php endif; ?>
    });

    function reload()
    {
        window.location = 'progorc.php?modulo=principal/limite/listar&acao=A';
    }

    function alteraJustificativa(statusJustificativa, idCaptacao, idPeriodo, descPeriodo, unidade, fonte, natureza, justificativa) {
        $('#statusJustificativa_' + statusJustificativa).each(function () { // -- só funcionou assim
            this.checked = true;
        });
        $('#capid').attr('value', idCaptacao);
        $('#periodo').attr('value', descPeriodo);
        $('#idPeriodo').attr('value', idPeriodo);
        $('#unidade').attr('value', unidade);
        $('#fonte').attr('value', fonte);
        $('#natureza').attr('value', natureza);
        $('#justificativa').val(justificativa);
        $('#modal-form').modal({'backdrop': 'static'});
    }

    function insereJustificativa(idPeriodo, descPeriodo, unidade, fonte, natureza) {
        $("input[name='statusJustificativa']").each(function () { // -- só funcionou assim
            this.checked = false;
        });
        $('#periodo').attr('value', descPeriodo);
        $('#idPeriodo').attr('value', idPeriodo);
        $('#unidade').attr('value', unidade);
        $('#fonte').attr('value', fonte);
        $('#natureza').attr('value', natureza);
        $('#justificativa').val('');
        $('#modal-form').modal({'backdrop': 'static'});
    }
</script>
<style type="text/css">
    .table .acertos{color:#E1004C !important}
    .text-center{text-align:left !important}
    .marcado{background-color:#C1FFC1 !important}
    .remover{display:none}
</style>
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li>Pedidos de Limites</li>
        <li class="active">Lista de Pedidos de Limites (<?php echo $_SESSION['exercicio']; ?>)</li>
    </ol>
    <div class="well">
        <fieldset>
            <form id="formPrincipal" name="formPrincipal" method="POST" class="form-horizontal">
                <input type="hidden" name="vieid" id="vieid" value="<?php echo $vieid ?>" />
                <div class="form-group">
                    <label for="inputPeriodo" class="col-lg-2 control-label">Mês Referência:</label>
                    <div class="col-lg-10">
                        <?php
                        $sql = "
                        SELECT
                            DISTINCT EXTRACT(MONTH FROM pli.pdldatapedido) as codigo,
                            case WHEN (EXTRACT(MONTH FROM pli.pdldatapedido) = 1) THEN 'Janeiro'
                             WHEN (EXTRACT(MONTH FROM pli.pdldatapedido) = 2) THEN 'Fevereiro'
                            when (EXTRACT(MONTH FROM pli.pdldatapedido) = 3) then 'Março'
                            WHEN (EXTRACT(MONTH FROM pli.pdldatapedido) = 4) THEN 'Abril'
                            WHEN (EXTRACT(MONTH FROM pli.pdldatapedido) = 5) THEN 'Maio'
                            WHEN (EXTRACT(MONTH FROM pli.pdldatapedido) = 6) THEN 'Junho'
                            WHEN (EXTRACT(MONTH FROM pli.pdldatapedido) = 7) THEN 'Julho'
                            WHEN (EXTRACT(MONTH FROM pli.pdldatapedido) = 8) THEN 'Agosto'
                            WHEN (EXTRACT(MONTH FROM pli.pdldatapedido) = 9) THEN 'Setembro'
                            WHEN (EXTRACT(MONTH FROM pli.pdldatapedido) = 10) THEN 'Outubro'
                            WHEN (EXTRACT(MONTH FROM pli.pdldatapedido) = 11) THEN 'Novembro'
                            WHEN (EXTRACT(MONTH FROM pli.pdldatapedido) = 12) THEN 'Dezembro' end as descricao
                        FROM progorc.pedidolimite pli WHERE EXTRACT(YEAR FROM pli.pdldatapedido) =  {$_SESSION['exercicio']}";
                        $mes = $_REQUEST ['mes'];
                        $db->monta_combo('mes', $sql, 'S', 'Selecione um Mês', null, null, null, null, 'N', 'mes', null, '', null, 'class="form-control chosen-select" style="width=100%;""', null, (isset($mes) ? $mes : null));
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Unidade Orçamentária (UO)</label>
                    <div class="col-lg-10">
                        <?php
                        /* Removendo Filtros para perfil CGO */
                        if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)) {
                            $whereUO = "";
                        }

                        $sql = "
                        SELECT
                            uni.unicod AS codigo,
                            uni.unicod || ' - ' || unidsc AS descricao
                        FROM public.unidade uni
                        WHERE
                            (uni.orgcod = '". CODIGO_ORGAO_SISTEMA. "'  OR  uni.unicod IN('74902', '73107'))
                            AND uni.unistatus = 'A'
                            {$whereUO}
                        ORDER BY uni.unicod
";
                        if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis) ||
                                in_array(PFL_SUPER_USUARIO, $perfis)) {
                            $unicod = $_REQUEST['unicod'];
                        } else {
                            $recuperaunicod = $db->pegaLinha($sql);
                            $unicod = $recuperaunicod['codigo'];
                        }
                        $opcoes = array('obrig' => 'true');
                        inputCombo('unicod', $sql, $unicod, 'unicod', $opcoes);
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputName" class="col-lg-2 control-label">Referência:</label>
                    <div class="col-md-10">
                        <?php
                        $sql = <<<DML
                            SELECT
                                refcod AS codigo,
                                refdsc AS descricao
                            FROM progorc.referencia
DML;
                        inputCombo('pdlreferencia', $sql, $_REQUEST['ref'], 'pdlreferencia', $opcoes);
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputName" class="col-lg-2 control-label">Tipo:</label>
                    <div class="col-md-10">
                        <?php
                        $anexo = $_REQUEST['anexo'] ? $_REQUEST['anexo'] : '';
                        /* Troca os tipos para < 2015 e >= 2015 */
                        if ($_SESSION['exercicio'] < 2015) {
                            $opcoes = array('Todos' => '', 'Anexo I' => 'I', 'Anexo II' => 'II', 'Anexo III' => 'III', 'Anexo IV' => 'IV', 'Anexo V' => 'V', 'Anexo VI' => 'VI');
                        } else {
                            $opcoes = array('Todos' => '', 'CA/IA-50' => 'CA-IA-50', 'CA/IA-80' => 'CA-IA-80', 'CA/IA-Demais' => 'CA-IA-Demais', 'UA' => 'UA');
                        }
                        inputChoices("anexo", $opcoes, $anexo, '');
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputName" class="col-lg-2 control-label"> Status do Pedido:</label>
                    <div class="col-md-10">
                        <?php
                        $wrfstatus = $_REQUEST['wrfstatus'] ? $_REQUEST['wrfstatus'] : 'todos';
                        $opcoes = array(
                            'Todos' => 'todos',
                            'Não Enviado' => STDOC_NAO_ENVIADO,
                            'Análise SPO' => STDOC_ANALISE_SPO,
                            'Acertos SPO' => STDOC_ACERTOS_SPO,
                            'Análise Secretaria' => STDOC_ANALISE_SECRETARIA,
                            'Acertos Secretaria' => STDOC_ACERTOS_SECRETARIA,
                            'Atendidos' => STDOC_ATENDIDO,
                            'Recusados' => STDOC_RECUSADO,
                            'Junta Orçamentária' => STDOC_JUNTA_ORCAMENTARIA
                        );
                        inputChoices("wrfstatus", $opcoes, $wrfstatus, '');
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <? $aprstcc = 1; ?>
                    <? if (isset($_REQUEST['aprstcc'])) $aprstcc = $_REQUEST['aprstcc']; ?>
                    <label class="col-md-2 control-label"> Apresentar Código da Conta:
                    </label>
                    <div class="col-md-10">
                        <div class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default <?= $aprstcc ? 'active' : ''; ?>">
                                <input type="radio" name="aprstcc" id="aprstcc_true" value="1" <?= $aprstcc ? 'checked' : ''; ?> /> Sim
                            </label>
                            <label class="btn btn-default <?= !$aprstcc ? 'active' : ''; ?>">
                                <input type="radio" name="aprstcc" id="aprstcc_false" value="0" <?= !$aprstcc ? 'checked' : ''; ?> /> Não
                            </label>
                        </div>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" type="reset" onclick="reload()">Limpar</button>
                        <button class="btn btn-primary" id="buscar" type="button">Buscar</button>
                        <?php if (!in_array(PFL_SECRETARIA, $perfis)) { ?><button class="btn btn-info" id="inserir" type="button">Inserir</button><?php } ?>
                        <button class="btn btn-danger" id="xls" type="button">Gerar XLS</button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
    <div>
        <?php
        $where = array();
        if (chaveTemValor($_REQUEST, 'mes') && $_REQUEST['mes'] != 'undefined') {
            $where [] .= " extract(month from pli.pdldatapedido) = '{$_REQUEST['mes']}'";
        }
        if (chaveTemValor($_REQUEST, 'unicod') && $_REQUEST['unicod'] != 'undefined') {
            $where [] .= "uni.unicod = '{$_REQUEST['unicod']}'";
        }
        if (chaveTemValor($_REQUEST, 'anexo') && $_REQUEST['anexo'] != 'undefined') {
            $where [] .= " ccr.anxcod = '{$_REQUEST['anexo']}'";
        }
        if (chaveTemValor($_REQUEST, 'wrfstatus') && $_REQUEST['wrfstatus'] != 'undefined' && $_REQUEST ['wrfstatus'] != 'todos') {
            $where [] .= " doc.esdid = '{$_REQUEST['wrfstatus']}'";
        }
        if (chaveTemValor($_REQUEST, 'ref') && $_REQUEST['ref'] != 'undefined') {
            $where [] .= " pli.pdlreferencia = '{$_REQUEST['ref']}'";
        }

        if (!empty($where)) {
            $where = 'WHERE ' . implode(' AND ', $where);
        } else {
            $where = 'WHERE 1=1';
        }
        if ($_SESSION['exercicio'] < 2015) {
            $tipoAnexo = "'Anexo ' || ccr.anxcod AS anexo,";
        } else {
            $tipoAnexo = "ccr.anxcod AS anexo,";
        }

// -- Query de listagem de acompanhamentos
        $sql = <<<DML
        SELECT pli.pdlid,
            ccr.anxcod,
            LPAD(pli.pdlid::varchar, 5 , '0') AS codigo_do_limite,
            TO_CHAR(pli.pdldatapedido, 'dd/mm/yyyy') AS data,
            pli.unicod || ' - ' || uni.unidsc AS unidade,
            unig.ungcod AS ug,
            unig.gescod AS gestao,
            $tipoAnexo
            ARRAY_TO_STRING(array_agg(ccrcod), ', ') as tipoconta,
            pli.pdlreferencia AS referencia,
            SUM(pde.pddvaloramplicacao) AS ampliacao,
            SUM(pde.pddvalorreducao) AS reducao,
            CASE WHEN doc.esdid = 1070 OR doc.esdid = 1102
                THEN SUM(pde.pddvaloramplicacaoatendido)
            ELSE 0
            END AS atendido_a,
            CASE WHEN doc.esdid = 1070 OR doc.esdid = 1102
                THEN SUM(pde.pddvalorreducaoatendido)
            ELSE 0
            END AS atendido_r,
            (SELECT TO_CHAR(hst.htddata, 'dd/mm/yyyy')
                FROM workflow.historicodocumento hst
                WHERE hst.docid = pli.docid
                ORDER BY hst.hstid DESC LIMIT 1) AS data_inclusao,
                doc.esdid AS status,
                COALESCE(LPAD(pli.pdlnl::varchar, 6, '0'), '-') AS pdlnl,
            (SELECT hst.aedid
                FROM workflow.historicodocumento hst
                WHERE hst.docid = pli.docid
                ORDER BY hst.hstid DESC LIMIT 1) AS troca,
            pli.pdljustificativa
        FROM progorc.pedidolimite pli
        LEFT JOIN progorc.uouggestao uog ON uog.unicod = pli.unicod
        LEFT JOIN public.unidadegestora unig ON unig.ungcod = uog.ungcod
        LEFT JOIN progorc.pedidodetalhe pde ON pde.pdlid = pli.pdlid AND (pde.pddvaloramplicacao + pde.pddvalorreducao) > 0
        LEFT JOIN progorc.contacorrente ccr ON ccr.ccrid = pde.ccrid
        LEFT JOIN progorc.anexo ane ON ane.anxcod = ccr.anxcod
        INNER JOIN public.unidade uni ON uni.unicod = pli.unicod
        LEFT JOIN workflow.documento doc ON doc.docid = pli.docid
        LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
        LEFT JOIN progorc.dotacaoconta dtc
            ON (dtc.unicod = pli.unicod
                AND dtc.exercicio::text = EXTRACT(YEAR FROM pli.pdldatapedido)::text
                AND dtc.ccrid = ccr.ccrid
                AND dtc.referencia = pli.pdlreferencia)
        {$where}{$whereUO}{$whereSecretaria}
            AND EXTRACT(YEAR FROM pli.pdldatapedido)::text = '{$_SESSION['exercicio']}'
        GROUP BY
            pli.pdlid,
            ccr.anxcod,
            uni.unidsc,
            pli.unicod,
            unig.ungcod,
            unig.gescod,
            pli.pdldatapedido,
            pli.docid,
            pli.pdlnl,
            doc.esdid,
            doc.docid,
            doc.docdatainclusao,
            esd.esddsc,
            pli.pdlreferencia,
            pli.pdljustificativa
        ORDER BY pli.pdldatapedido DESC
DML;
        $cabecalhoTabela = array(
            'Pedido',
            'Data do Pedido',
            'Unidade Orçamentária',
            'Tipo',
            'Conta',
            'Ref',
            '<center>Pedido (R$)</center>' => array('Ampliação', 'Redução'),
            '<center>Atendido (R$)</center>' => array('Ampliação', 'Redução'),
            'Data do Status',
            'Status',
            'NL'
        );
        if (!$aprstcc) {
            $cabecalhoTabela = array(
                'Pedido',
                'Data do Pedido',
                'Unidade Orçamentária',
                'Tipo',
                'Ref',
                'Dotação da Conta',
                '<center>Pedido (R$)</center>' => array('Ampliação', 'Redução'),
                '<center>Atendido (R$)</center>' => array('Ampliação', 'Redução'),
                'Data do Status',
                'Status',
                'NL'
            );
        }

        echo getFlashMessage();

        $listagem = new Simec_Listagem ();
        $listagem->turnOnPesquisator();
        $listagem->setCabecalho($cabecalhoTabela);
        $listagem->setQuery($sql);

// -- Ações da listagem de pedidos de alteração de limites orçamentários
        $acoes = array(
            'edit' => array('func' => 'editLimite', 'extra-params' => array('anxcod'))
        );
// -- Apenas o Super Usuário ou a CGO podem apagar a Solicitação
        if (in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
            $acoes['delete'] = array('func' => 'deleteLimite', 'extra-params' => array('anxcod'));
        }

        $listagem->addCallbackDeCampo('tipoconta', 'formataCampoArray');
        if (!$aprstcc) {
            $listagem->esconderColuna('tipoconta');
        }
        $listagem->esconderColunas(array('troca', 'ug', 'gestao', 'pdljustificativa', 'anxcod'))
                ->setAcoes($acoes)
                ->setTotalizador(
                        Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('ampliacao', 'reducao', 'atendido_a', 'atendido_r')
                )->addCallbackDeCampo(
                        array('ampliacao', 'reducao', 'atendido_a', 'atendido_r', 'dotacao'), 'mascaraMoeda'
                )->addRegraDeLinha(
                        array('campo' => 'status', 'op' => 'igual', 'valor' => STDOC_ACERTOS_SPO, 'classe' => 'acertos')
                )->addRegraDeLinha(
                        array('campo' => 'status', 'op' => 'igual', 'valor' => STDOC_ACERTOS_SECRETARIA, 'classe' => 'acertos')
                )->addRegraDeLinha(
                        array('campo' => 'troca', 'op' => 'igual', 'valor' => AEDID_SECRETARIA_SPO, 'classe' => 'amarelo')
                )->addCallbackDeCampo('status', 'processaStatus')
                ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        ?>
    </div>
</div>