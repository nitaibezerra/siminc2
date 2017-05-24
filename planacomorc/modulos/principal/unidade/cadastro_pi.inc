<?php
/**
 * Tela de cadastro de PIs
 * @see cadastro.inc
 * $Id: cadastro_pi.inc 101265 2015-08-14 13:05:26Z maykelbraz $
 */

/**
 * Funções de apoio ao cadastro de PIs.
 * @see _funcoespi.php
 */
include_once '_funcoespi.php';

include_once APPRAIZ . "planacomorc/classes/Cronograma.class.inc";
include_once APPRAIZ . "planacomorc/classes/Pi_Sniic.class.inc";
include_once APPRAIZ . "planacomorc/classes/Pi_Localizacao.class.inc";
include_once APPRAIZ . "planacomorc/classes/Pi_Convenio.class.inc";
include_once APPRAIZ . "planacomorc/classes/Pi_Responsavel.class.inc";
include_once APPRAIZ . "planacomorc/classes/Pi_Acao.class.inc";
include_once APPRAIZ . "planacomorc/classes/Pi_Cronograma.class.inc";

if ($_REQUEST['carregarComboUG']) {
    echo carregarComboUG($_REQUEST['unicod']);
    die();
}
if ($_REQUEST['carregarMetasPPA']) {
    carregarMetasPPA((int)$_REQUEST['oppid'], null);
    die();
}
if ($_REQUEST['carregarIndicadorPNC']) {
    carregarIndicadorPNC((int)$_REQUEST['mpnid'], null);
    die();
}
if ($_REQUEST['carregarSegmentoCultural']) {
    carregarSegmentoCultural((int)$_REQUEST['mdeid'], null);
    die();
}
if ($_REQUEST['carregarLimitesUnidade']) {
    $autorizado = carregarLimiteAutorizadoSubUnidade(
        (object) array(
            'ungcod' => $_REQUEST['ungcod']
    ));
    $detalhadoSubUnidade = carregarLimiteDetalhadoSubUnidade(
        (object) array(
            'ungcod' => $_REQUEST['ungcod']
    ));
    $disponivel = ($autorizado - $detalhadoSubUnidade);
    echo json_encode(
        array(
            'autorizado' => number_format($autorizado, 2, ',', '.'),
            'disponivel' => number_format($disponivel, 2, ',', '.')
    ));
    die();
}
if ($_REQUEST['carregarIniciativaPPA']) {
    carregarIniciativaPPA((int)$_REQUEST['oppid'], null);
    die();
}
if ($_REQUEST['carregarComboSubacaoFederais']) {
    carregarComboSubacaoUO($_REQUEST['unicod']);
    die();
}
if ($_REQUEST['sbaAjax']) {
    echo buscaDadosSubacao($_REQUEST['sbaAjax'], $_REQUEST['capidAjax']);
    die();
}
/**
 * Processa as requições enviadas a esta página.
 * @see cadastropi_requisicoes.inc
 */

require(APPRAIZ . 'planacomorc/modulos/principal/planointerno/cadastropi_requisicoes.inc');


// -- Identificando se o usuário atual faz parte do CGSO/CPMO
$pulaSolicitacao = pulaSolicitacao();

// -- Definições iniciais das variáveis de controle do formulário
$habilitado = 'S';
$obrigatorio = 'N';
$ehSolicitacao = true;
$dadosPI = array();

$perfis = pegaPerfilGeral();

// ******* Ao carregar a listagem de PTRES, sempre filtrar pela UO selecionada, independentemente do Perfil ; *****
/*if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
    $sqlUO = <<<DML
EXISTS (SELECT 1
         FROM planacomorc.usuarioresponsabilidade rpu
         WHERE rpu.usucpf = '%s'
           AND rpu.pflcod = %d
           AND rpu.rpustatus = 'A'
           AND rpu.unicod  = uni.unicod)
DML;
    $where[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
    $whereUO = " AND {$whereUO}";
}*/

$aSniic = $aConvenio = $aLocalizacao = $aResponsavel = [];
if (validaRequisicao('pliid')) { // -- Edição de PI

    $pliid = $_REQUEST['pliid'];

    // -- Carrega dados do PI e libera apenas o titulo e a descrição para edição
    $dadosPI = carregarPI($_REQUEST['pliid']);
    $enquadramento_pi = carregarEnquadramentoPI($_REQUEST['pliid']);
    //ver($dadosPI,d);
    $habilitado = 'N';
    $ehSolicitacao = false;
    $obrigatorio = 'S';

    $aSniic = (new Pi_Sniic())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aConvenio = (new Pi_Convenio())->recuperarTodos('*', array('pliid = ' . $pliid));
    $aLocalizacao = (new Pi_Localizacao())->recuperarPorPlanoInterno($pliid, $dadosPI['esfid']);
    $aResponsavel = (new Pi_Responsavel())->recuperarPorPlanoInterno($pliid);
    
    // -- Requisição de apagar transação - Não exibe os botões do rodapé, e nem valida o formulário
    if (validaRequisicao('apagar')) {
        $tipoTransacao = '-';
    }
} elseif (validaRequisicao('scpid')) { // -- Transações de cadastro de PI
    // -- Carrega os dados da transação de criação e, de acordo com o tipo:
    // -- E: apenas exibe os dados - read only de uma transação já executada
    // -- S: deixa o formulário aberto para edição e liga as regras de validação - edição de
    // -- informações para confirmar a transação.
    $dadosPI = carregarTransacao($_REQUEST['scpid']);
} else { // -- Criação de uma nova transação de cadastro de PI
}
/* Habilita a Edição para Super Usuário */
if ($db->testa_superuser()) {
    $habilitado = 'S';
}

function buscarPTRESdoPIInstituicoes($pliid, $sbaid) {
    global $db;

    #  ver($sbaid);
    /* Para o caso de um PI sem Subação */
    $campoNaoDetPi = " (COALESCE(ptr.ptrdotacao, 0.00) - COALESCE(SUM(dtp.valor), 0.00)) AS nao_det_pi, ";
    if ($sbaid && $sbaid != 'null' && $sbaid != '' && $sbaid != '0') {
        $filtroSubacao = " AND sbaid = {$sbaid}";
        $campoNaoDetPi = " (COALESCE(SUM(dts.valor), 0.00) - COALESCE(SUM(dtp.valor), 0.00)) AS nao_det_pi, ";
    }

    /* Filtros */
    $where .= $sbaid ? " AND dtp.ptrid IN (SELECT ptrid FROM monitora.pi_subacaodotacao WHERE sbaid = '" . $sbaid . "')" : '';
    $where .= $pliid ? " AND pip.pliid = $pliid " : "";

    $sql = <<<SQL
        SELECT
            ptr.ptrid,
            ptr.ptres,
            TRIM(aca.prgcod) || '.' || TRIM(aca.acacod) || '.' || TRIM(aca.loccod) || '.' || (CASE WHEN LENGTH(TRIM(aca.acaobjetivocod)) <= 0 THEN '-' ELSE TRIM(aca.acaobjetivocod) END) || '.' || TRIM(ptr.plocod) || ' - ' || aca.acatitulo AS descricao,
            aca.unicod || ' - ' || uni.unidsc as unidsc,
            COALESCE(ptr.ptrdotacao, 0.00) AS dotacaoatual,
            COALESCE(SUM(dts.valor), 0.00) AS det_subacao,
            (COALESCE(SUM(ptr.ptrdotacao), 0.00) - COALESCE(SUM(dts.valor), 0.00)) AS nao_det_subacao,
            COALESCE(SUM(dtp.valor), 0.00) AS det_pi,
            {$campoNaoDetPi}
            COALESCE((pemp.total), 0.00) AS empenhado,
            COALESCE(SUM(ptr.ptrdotacao), 0.00) - COALESCE(pemp.total, 0.00) AS nao_empenhado,
            (SELECT pipvalor FROM monitora.pi_planointernoptres JOIN monitora.pi_planointerno USING(pliid) WHERE ptrid = ptr.ptrid AND pliid = {$pliid} {$filtroSubacao} ) as pipvalor
        FROM monitora.ptres ptr
        INNER JOIN monitora.acao aca on ptr.acaid = aca.acaid
        INNER JOIN public.unidade uni on aca.unicod = uni.unicod
        LEFT JOIN monitora.pi_planointernoptres pip on ptr.ptrid = pip.ptrid
        LEFT JOIN (
            SELECT ptrid, SUM(sadvalor) AS valor
            FROM monitora.pi_subacaodotacao
            WHERE 1=1 {$filtroSubacao}
            GROUP BY ptrid) dts ON dts.ptrid = ptr.ptrid
        LEFT JOIN (
            SELECT pip.ptrid, SUM(pipvalor) AS valor
            FROM monitora.pi_planointernoptres pip
            JOIN monitora.pi_planointerno pli using (pliid)
            WHERE plistatus  = 'A'
                {$filtroSubacao}
            GROUP BY ptrid) dtp ON dtp.ptrid = ptr.ptrid
        LEFT JOIN siafi.uo_ptrempenho pemp ON (pemp.ptres = ptr.ptres AND pemp.exercicio = ptr.ptrano AND pemp.unicod = ptr.unicod)
        WHERE aca.acasnrap = FALSE
            AND aca.unicod NOT IN('26101','26291', '26290', '26298', '26443', '74902', '73107')
            AND aca.prgano='{$_SESSION['exercicio']}'
            AND ptr.ptrstatus = 'A'
            $where
        GROUP BY ptr.ptrid, ptr.ptres, aca.prgcod, aca.acaobjetivocod, aca.acacod,aca.unicod,aca.loccod,aca.acatitulo,uni.unidsc, ptr.ptrdotacao, pemp.total
        ORDER BY ptr.ptres
SQL;
    $result = is_array($result) ? $result : Array();
    #ver($sql);
    $result = $db->carregar($sql);
    if (is_array($result)) {
        foreach ($result as $key => $_) {
            $result[$key]['dotacaoatual'] = mascaraMoeda($result[$key]['dotacaoatual'], false);
            $result[$key]['det_subacao'] = mascaraMoeda($result[$key]['det_subacao'], false);
            $result[$key]['nao_det_subacao'] = mascaraMoeda($result[$key]['nao_det_subacao'], false);
            $result[$key]['det_pi'] = mascaraMoeda($result[$key]['det_pi'], false);
            $result[$key]['nao_det_pi'] = mascaraMoeda($result[$key]['nao_det_pi'], false);
            $result[$key]['empenhado'] = mascaraMoeda($result[$key]['empenhado'], false);
            $result[$key]['nao_empenhado'] = mascaraMoeda($result[$key]['nao_empenhado'], false);
            $result[$key]['pipvalor_'] = $result[$key]['pipvalor']; // -- Não formatado - para soma na interface
            $result[$key]['pipvalor'] = number_format($result[$key]['pipvalor'], 2, ',', '.');
        }
    }
    return $result;
}

/**
 * Cabeçalho padrão do simec.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/funcoesspo.js"></script>
<link rel="stylesheet" href="/library/bootstrap-switch/stylesheets/bootstrap-switch.css">
<script src="/library/bootstrap-switch/js/bootstrap-switch.min.js"></script>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li><a href="<?php echo MODULO; ?>.php?modulo=principal/unidade/listapimanter&acao=A">Plano Interno</a></li>
        <li class="active"><?php echo $dadosPI ? 'Alterar Plano Interno' : 'Cadastrar Plano Interno'; ?></li>
    </ol>
</div>
<div class="col-lg-12">
    <div class="well">
        <fieldset>
            <form id="formulario" name="formulario" method="post" action="" class="form-horizontal">
                <input type="hidden" name="evento" id="evento" value="S" />
                <input type="hidden" name="plicod" id="plicod" value="<?= $dadosPI['plicod']; ?>" />
                <input type="hidden" name="pliid" id="pliid" value="<?= $_REQUEST['pliid']; ?>" />
                <input type="hidden" name="picid" id="picid" value="<?= $dadosPI['picid']; ?>" />
                <input type="hidden" name="sbacod" id="sbacod" value="<?php echo $dadosPI['sbacod']; ?>" />
                <input type="hidden" name="sbaidAnterior" id="sbaidAnterior" value="" />
                <input type="hidden" name="ehSolicitacao" id="ehSolicitacao" value="<?php echo $ehSolicitacao; ?>" />
                <input type="hidden" name="tipotransacao" id="tipotransacao" value="E" />
                <input type="hidden" name="scpid" id="scpid" value="<?php echo $_REQUEST['scpid']; ?>" />

                <?php if ($_GET['apagar']): ?>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label"></label>
                    <div class="col-lg-10">
                        <input type="button" class="btn btn-danger" name="btassociar4" id="btassociar4" value="APAGAR"
                            onclick="removerpi();" />
                    </div>
                </div>
                <?php endif; ?>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="exercicio">Exercício:</label>
                    </div>
                    <div class="col-lg-10">
                        <p class="form-control-static">
                        <?php echo!empty($dadosPI['pliano']) ? $dadosPI['pliano'] : $_SESSION['exercicio']; ?>
                        </p>
                    </div>
                </div>
                <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset><legend>Dados do Plano interno</legend>
                            <div class="form-group">
                                <div class="col-md-2 text-right">
                                    <label class="control-label" for="plititulo">Título:</label>
                                </div>
                                <div class="col-lg-10">
                                    <input type="text" value="<?php
                                    if ($dadosPI['plititulo']) {
                                        echo $dadosPI['plititulo'];
                                    }
                                    ?>" class="CampoEstilo normal form-control obrigatorio" title="" id="plititulo" value="" maxlength="250" size="2" name="plititulo">
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-md-2 text-right">
                                    <label class="control-label" for="plidsc">Descrição / Finalidade:</label>
                                </div>
                                <div class="col-lg-10">
                                    <textarea class="obrigatorio txareanormal form-control"  rows="2" cols="60" name="plidsc" id="plidsc" maxlength="900"><?php
                                        if ($dadosPI['plidsc']) {
                                            echo $dadosPI['plidsc'];
                                        }
                                        ?></textarea>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="unicod">Unidade Solicitante:</label>
                    </div>
                    <div class="col-lg-10">
                    <?php
                    $obrigatorias = UNIDADES_OBRIGATORIAS;
                    // -- Se a requisição tiver um scpid, quer dizer que a tela foi aberta para homologar uma
                    // -- solicitação de PI, ou para ver uma transação executada anteriormente. Nos dois casos,
                    // -- o filtro de UOs deve exibir (ao menos) a UO escolhida no momento da criação da transação,
                    // -- portanto, não deve filtrar pela UO do usuário que a está visualizando.
                    // ******* Ao carregar a listagem de PTRES, sempre filtrar pela UO selecionada, independentemente do Perfil ; *****
                    /*if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)) {
                        $whereUO = "";
                    }*/

                    $sql = "
                        SELECT
                            uni.unicod AS codigo,
                            uni.unicod || ' - ' || unidsc AS descricao
                        FROM public.unidade uni
                        WHERE (uni.orgcod = '". CODIGO_ORGAO_SISTEMA. "')
                            AND uni.unistatus = 'A'
                            {$whereUO}
                        ORDER BY uni.unicod
";

                    if ($dadosPI['unicod']) {
                        $unicod = $dadosPI['unicod'];
                        $editavel = 'N';
                    }

                    $db->monta_combo('unicod', $sql, $editavel, 'Selecione', null, null, null, null, 'N', 'unicod', null, (isset($unicod) ? $unicod : null), null, 'class="form-control chosen-select" style="width=100%;"', null, null);
                    ?>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="ungcod">Sub-Unidade:</label>
                    </div>
                    <div class="col-lg-10 div_ungcod">
                        <?php
                        $ungcod = $dadosPI['ungcod'];
                        $ungdata = array();
                        if (!empty($ungcod)) {
                            $sql = <<<DML
                                SELECT ung.ungcod AS codigo,
                                       ung.ungcod || ' - ' || ung.ungabrev AS descricao
                                  FROM public.unidadegestora ung
                                  WHERE ung.ungstatus= 'A'
                                    AND ung.ungcod = '%s' {$filtroPerfilUG}
                                  ORDER BY ung.unicod
DML;
                            $ungdata = sprintf($sql, $ungcod);
                        }
                        $db->monta_combo('ungcod', $ungdata, 'S', 'Selecione', null, null, null, null, 'N', 'ungcod', null, (isset($ungcod) ? $ungcod : null), null, 'class="form-control chosen-select" style="width=100%;"', null, null);
                        ?>
                    </div>
                </div>
                <div style="display: none;" class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Subação:</label>
                    <div class="col-lg-10 carregaSubacao">
                    <?php if (!$dadosPI['pliid']) { ?>
                        <div class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default">
                                <input type="radio" name="semsubacao"
                                       id="semsubacao"
                                       value="s" onchange="subacao(this.value)"/>Sem Subação
                            </label>
                            <label class="btn btn-default active">
                                <input type="radio" name="semsubacao"
                                       id="semsubacao"
                                       value="c" onchange="subacao(this.value)" />Com Subação
                            </label>
                        </div>
                        <?php } ?>
                        <div id="cSubacao" class="cSubacao" style="display: block">
                        <?php
                        $sbaid = $dadosPI['sbaid'];
                        $sbadata = array();

                        if (!empty($unicod)) {
                            $sbadata = carregarComboSubacaoUO($unicod, $retornaSQL = true);
                        }
                        if ($sbaid && $dadosPI['pliid']) {
                            $db->monta_combo('sbaid', $sbadata, $habilitado, 'Selecione', 'selecionarsubacao', null, null, 400, 'N', 'sbaid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($dadosPi['sbaid']) ? $dadosPi['sbaid'] : null));
                            $sbadata = '';
                        }
                        if (!$dadosPI['pliid']) {
                            $db->monta_combo('sbaid', $sbadata, $habilitado, 'Selecione', 'selecionarsubacao', null, null, 400, 'N', 'sbaid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($dadosPi['sbaid']) ? $dadosPi['sbaid'] : null));
                            $sbadata = '';
                        } else {
                        ?>
                            <div  style="display:none"> <select name="sbaid" id="sbaid"></select></div> <?php
                        }
                        $plicodsubacao = substr($dadosPI['plicod'], 1, 4);
                        ?>
                        </div>
                        <div id="sSubacao" style="display:
                            <?php
                            if (!$sbaid && $dadosPI['pliid']) {
                                echo 'block';
                                $tipoSubacao = 's';
                            } else {
                                echo 'none';
                                 $tipoSubacao = 'c';
                            }
                            ?>">
                            <input type="hidden" name="tipoSubacao" id="tipoSubacao" value="<?php echo $tipoSubacao;?>">
                            <div class="input-group">
                                <span class="input-group-addon"><strong>Código Livre (4 dígitos):</strong></span>
                                <input type="text" name="plicodsubacao" class="CampoEstilo normal form-control obrigatorio" id="plicodsubacao" maxlength="4" onblur="semSubacao();
                                contar();" <?php
                                    if (!$sbaid && $dadosPI['pliid']) {
                                        echo "value=\"" . $dadosPI['plicodsubacao'] . "\" readonly='readonly' class='disabled'";
                                    }
                                ?>>
                            </div>
                        </div>
                    </div>
                </div>
                <div style="display: none;" class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Cadastrado SIAFI:</label>
                    <div class="col-lg-10">
                        <div class="make-switch switch-mini" data-on-label="S" data-off-label="N" data-off="danger">
                            <input type="checkbox" name="plicadsiafi" id="plicadsiafi"
                                value="T" <?php echo $dadosPI['plicadsiafi'] == 't' ? 'checked' : ''; ?> />
                        </div>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="eqdid">Enquadramento da Despesa:</label>
                    </div>
                    <div class="col-lg-10 carregaEnq">
                    <?php
                        $sql = <<<DML
                            SELECT
                                eqdid AS codigo,
                                eqddsc AS descricao
                            FROM monitora.pi_enquadramentodespesa
                            WHERE
                                eqdstatus = 'A'
                                AND eqdano = '{$_SESSION['exercicio']}'
                            ORDER BY
                                eqddsc
DML;
                        $eqdid = $dadosPI['eqdid'];
                        $db->monta_combo('eqdid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', null, null, null, 'N', 'eqdid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($eqdid) ? $eqdid : null));
                    ?>
                    </div>
                </div>
                <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset>
                            <legend class="legend_funcional">Detalhamento Orçamentário</legend>
                            <div class="form-group">
                                <div class="col-lg-12">
                                    <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid" onmouseover="tabindexcampo();"  class="table table-hover table-bordered table-hover" >
                                        <tr>
                                            <td rowspan="2" nowrap>
                                                <b>PTRES</b>
                                            </td>
                                            <td rowspan="2" style="width:45%" nowrap>
                                                <b>Funcional</b>
                                            </td>
                                            <td rowspan="2" nowrap>
                                                <b>Dotação Atual PO (R$)</b>
                                            </td>
                                            <td colspan="2" nowrap>
                                                <b><span id="detPi">Detalhado em PI (R$)</span></b>
                                            </td>
                                            <td colspan="2">
                                                <b>Empenho (R$)</b>
                                            </td>
                                            <td rowspan="2" align="center">
                                                <b>Valor do Projeto</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td nowrap><b>Detalhado</b></td>
                                            <td nowrap><b>Não&nbsp;Detalhado</b></td>
                                            <td nowrap><b>Empenhado</b></td>
                                            <td nowrap><b>Não&nbsp;Empenhado</b></td>
                                        </tr>
                                        <?php
                                            if ($dadosPI['pliid']) {
                                                $dadosPTRES = buscarPTRESdoPIInstituicoes($dadosPI['pliid'], $dadosPI['sbaid'], 'N');
                                                #ver($dadosPTRES);
                                                if ($dadosPTRES) {
                                                    $valortotalpi = 0;
                                                    $cor = 1;
                                                    foreach ($dadosPTRES as $ptres):
                                                        ?>
                                                        <tr style="height:30px;" id="ptres_<?php echo $ptres['ptrid']; ?>">
                                                            <td align="center"><?php echo $ptres['ptres']; ?></td>
                                                            <td align="left"><?php echo $ptres['descricao']; ?></td>
                                                            <td align="right"><?php echo $ptres['dotacaoatual']; ?></td>
                                                            <td align="right"><?php echo $ptres['det_pi']; ?></td>
                                                            <td align="right"><?php echo $ptres['nao_det_pi']; ?></td>
                                                            <td align="right"><?php echo $ptres['empenhado']; ?></td>
                                                            <td align="right">
                                                                <?php echo $ptres['nao_empenhado']; ?>
                                                                <input type="hidden" name="ptrid" value="<?php echo $ptres['ptrid']; ?>" />
                                                            </td>
                                                            <td align="center" id="td_valor_projeto">
                                                                <?php echo 'R$ '. $ptres['pipvalor']; ?>
                                                            </td>
                                                        </tr>
                                                        <?php
                                                        $cor++;
                                                        $valortotalpi = $valortotalpi + $ptres['pipvalor_'];
                                                    endforeach;
                                                }
                                            }
                                        ?>
                                    </table>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-10">
                                </div>
                                <div class="col-lg-2">
                                    <input type="button" id="btn_selecionar_functional" value="Selecionar Funcional/PTRES" class="btn btn-info" style="width: 100%;" />
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset><legend>Metas</legend>
                            <div class="form-group">
                                <div class="col-md-2 text-right">
                                <label for="oppid" class=" control-label">Objetivo PPA:</label>
                                </div>
                                <div class="col-lg-10">
                                    <?php
                                        $sql = <<<DML
                                            SELECT
                                                oppid AS codigo,
                                                oppcod || '-' || oppnome AS descricao
                                            FROM monitora.pi_objetivo_ppa
                                            WHERE
                                                oppstatus = 'A'
                                                AND prsano = '{$_SESSION['exercicio']}'
                                            ORDER BY
                                                oppcod
DML;
                                        $oppid = $dadosPI['oppid'];
                                        $db->monta_combo('oppid', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'oppid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($oppid)? $oppid: null));
                                    ?>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-md-2 text-right">
                                    <label for="mppid" class=" control-label">Metas PPA: </label>
                                </div>
                                <div class="col-lg-10 div_mppid">
                                    <?php carregarMetasPPA($dadosPI['oppid'], $dadosPI['mppid']); ?>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-md-2 text-right">
                                    <label for="ippid" class="control-label">Iniciativa PPA: </label>
                                </div>
                                <div class="col-lg-10 div_ippid">
                                    <?php carregarIniciativaPPA($dadosPI['oppid'], $dadosPI['ippid']); ?>
                                </div>
                            </div>
                            <div class="form-group grupo_pnc">
                                <div class="col-md-2 text-right">
                                    <label for="mpnid" class="control-label">Meta PNC: </label>
                                </div>
                                <div class="col-lg-10">
                                    <?php
                                        $sql = <<<DML
                                            SELECT
                                                mpncod::NUMERIC AS ordem,
                                                mpnid AS codigo,
                                                mpncod || '-' || mpnnome AS descricao
                                            FROM monitora.pi_meta_pnc
                                            WHERE
                                                mpnstatus = 'A'
                                                AND prsano  = '{$_SESSION['exercicio']}'
                                            ORDER BY
                                                ordem
DML;
                                    $mpnid = $dadosPI['mpnid'];
                                    $db->monta_combo('mpnid', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'mpnid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($mpnid)? $mpnid: null));
                                    ?>
                                </div>
                            </div>
                            <div class="form-group grupo_pnc">
                                <div class="col-md-2 text-right">
                                    <label for="ipnid" class="control-label">Indicador PNC: </label>
                                </div>
                                <div class="col-lg-10 div_ipnid">
                                    <?php carregarIndicadorPNC($dadosPI['mpnid'], $dadosPI['ipnid']); ?>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset><legend class="legend_sniic">Números SNIIC </legend>
                            <div class="form-group">
                                <div class="col-lg-10">
                                    <input type="text" placeholder="Digite um número e clique em adicionar." value="" class="CampoEstilo normal form-control" title="Número SNIIC" id="input_sniic"  maxlength="10" size="8" name="input_sniic" />
                                </div>
                                <div class="col-lg-2">
                                    <input type="button" id="btn_adicionar_sniic" value="Adicionar" class="btn btn-info" style="width: 100%;" />
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-10">
                                    <table id="table_sniic" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <?php foreach($aSniic as $sniic){ ?>
                                            <tr style="height: 30px;" id="tr_sniic_<?php echo $sniic['pisid']; ?>" >
                                                <td style="text-align: left;"><?php echo $sniic['pissniic']; ?></td>
                                                <td style="text-align: center;">
                                                    <input type="hidden" name="lista_sniic[]" value="<?php echo $sniic['pissniic']; ?>" />
                                                    <span class="glyphicon glyphicon-trash link btnRemoveSniic" data-sniic="<?php echo $sniic['pisid']; ?>" ></span>
                                                </td>
                                            </tr>
                                        <?php } ?>
                                    </table>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label for="mdeid" class="control-label">Área Cultural:</label>
                    </div>
                    <div class="col-lg-10">
                        <?php
                            $sql = <<<DML
                                SELECT
                                    MAX(mdeid) AS codigo,
                                    upper(public.removeacento(mdedsc)) AS descricao
                                FROM monitora.pi_modalidadeensino
                                WHERE
                                    mdestatus = 'A'
                                    AND mdeano = '{$_SESSION['exercicio']}' -- Resolver 2014
                                GROUP BY
                                    descricao
                                ORDER BY
                                    descricao ASC
DML;

                            $mdeid = $dadosPI['mdeid'];
                            $db->monta_combo('mdeid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', null, null, null, 'N', 'mdeid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($mdeid) ? $mdeid : null));
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="neeid">Segmento Cultural:</label>
                    </div>
                    <div class="col-lg-10 div_neeid">
                        <?php carregarSegmentoCultural($dadosPI['mdeid'], $dadosPI['neeid']); ?>
                    </div>
                </div>
                <div style="display:none;" class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="plilivre">Codificação da Unidade(livre):</label>
                    </div>
                    <div class="col-lg-10">
                    <?php
                    $plilivre = $dadosPI['plilivre'] ? $dadosPI['plilivre'] : '';
                    $opcoes = array(
                        'evtkeyup' => 'atualizarCodigoLivre();atualizarPrevisaoPI();',
                        'size' => 10
                    );
                    inputTexto('plilivre', $plilivre, 'idCodificacao', 2, false, $opcoes);
                    ?>
                    </div>
                </div>

            <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset><legend>Produto PI:</legend>
                            <div class="form-group">
                                <div class="col-md-2 text-right">
                                    <label for="pprid" class="control-label">Produto:</label>
                                </div>
                                <div class="col-lg-10">
                                    <?php
                                        $sql = <<<DML
                                            SELECT
                                                pprid AS codigo,
                                                pprnome AS descricao
                                            FROM monitora.pi_produto
                                            WHERE
                                                prsano = '{$_SESSION['exercicio']}'
                                                AND pprstatus = 'A'
                                            ORDER BY
                                                descricao
DML;

                                        $pprid = $dadosPI['pprid'];
                                        $db->monta_combo('pprid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', null, null, null, 'N', 'pprid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($pprid)? $pprid: null));
                                    ?>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-md-2 text-right">
                                    <label for="pumid" class="control-label">Unidade de Medida:</label>
                                </div>
                                <div class="col-lg-10">
                                    <?php
                                        $sql = <<<DML
                                            SELECT
                                                pumid AS codigo,
                                                pumdescricao AS descricao
                                            FROM monitora.pi_unidade_medida 
                                            WHERE
                                                prsano = '{$_SESSION['exercicio']}'
                                                AND pumstatus = 'A'
                                            ORDER BY
                                                descricao
DML;

                                        $pumid = $dadosPI['pumid'];
                                        $db->monta_combo('pumid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', null, null, null, 'N', 'pumid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($pumid) ? $pumid: null));
                                    ?>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-md-2 text-right">
                                    <label for="picquantidade" class="control-label">Quantidade:</label>
                                </div>
                                <div class="col-lg-10">
                                    <input type="text" value="<?php echo !empty($dadosPI['picquantidade']) ? $dadosPI['picquantidade'] : ''; ?>" class="CampoEstilo normal form-control obrigatorio" title="Quantidade" id="picquantidade" name="picquantidade" maxlength="50" size="10" />
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="capid">Modalidade de Pactuação:</label>
                    </div>
                    <div class="col-lg-10">
                        <?php
                        $sql = <<<DML
                        SELECT
                            capid AS codigo,
                            capdsc AS descricao
                        FROM monitora.pi_categoriaapropriacao
                        WHERE
                            capano = '{$_SESSION['exercicio']}'
                            AND capstatus = 'A'
                        ORDER BY
                            descricao
DML;
                        $capid = $dadosPI['capid'];
                        $db->monta_combo('capid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', null, null, null, 'N', 'capid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($capid) ? $capid : null));
                        ?>
                    </div>
                </div>
                <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset><legend class="legend_convenio">Números De Convênio</legend>
                            <div class="form-group">
                                <div class="col-lg-10">
                                    <input type="text" placeholder="Digite um número e clique em adicionar." value="" class="CampoEstilo normal form-control" title="Número de Convênio" id="input_convenio"  maxlength="10" size="8" name="input_convenio" />
                                </div>
                                <div class="col-lg-2">
                                    <input type="button"  id="btn_selecionar_convenio" value="Adicionar" class="btn btn-info" style="width: 100%;"/>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-10">
                                    <table id="table_convenio" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <?php foreach($aConvenio as $convenio){ ?>
                                            <tr style="height: 30px;" id="tr_convenio_<?php echo $convenio['pcoid']; ?>" >
                                                <td style="text-align: left;"><?php echo $convenio['pcoconvenio']; ?></td>
                                                <td style="text-align: center;">
                                                    <input type="hidden" name="lista_convenio[]" value="<?php echo $convenio['pcoconvenio']; ?>" />
                                                    <span class="glyphicon glyphicon-trash link btnRemoveConvenio" data-convenio="<?php echo $convenio['pcoid']; ?>" ></span>
                                                </td>
                                            </tr>
                                        <?php } ?>
                                    </table>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-2 text-right">
                        <label for="picted" class="control-label">TED:</label>
                    </div>
                    <div class="col-lg-10">
                        <div class="form-inline required">
                            <div class="form-group has-feedback col-xs-2">
                                <label class="input-group">
                                    <span class="input-group-addon">
                                        <input type="radio" name="picted" value="t" class="link" <?php echo !empty($dadosPI['picted']) && 't' == $dadosPI['picted'] ? 'checked="checked"' : ''; ?>>
                                    </span>
                                    <div class="form-control form-control-static input_check_s link">
                                        Sim
                                    </div>
                                    <span class="glyphicon form-control-feedback "></span>
                                </label>
                            </div>
                            <div class="form-group has-feedback col-xs-2">
                                <label class="input-group">
                                    <span class="input-group-addon">
                                        <input type="radio" name="picted" value="f" class="link" <?php echo !empty($dadosPI['picted']) && 'f' == $dadosPI['picted'] ? 'checked="checked"' : ''; ?>>
                                    </span>
                                    <div class="form-control form-control-static input_check_n link">
                                        Não
                                    </div>
                                    <span class="glyphicon form-control-feedback "></span>
                                </label>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-2 text-right">
                        <label for="picedital" class=" control-label">Edital:</label>
                    </div>
                    <div class="col-lg-10">
                        <div class="form-inline required">
                            <div class="form-group has-feedback col-xs-2">
                                <label class="input-group">
                                    <span class="input-group-addon">
                                        <input type="radio" name="picedital" value="t" class="link element_edital" <?php echo !empty($dadosPI['picedital']) && 't' == $dadosPI['picedital'] ? 'checked="checked"' : ''; ?>>
                                    </span>
                                    <div class="form-control form-control-static input_check_s link element_edital">
                                        Sim
                                    </div>
                                    <span class="glyphicon form-control-feedback "></span>
                                </label>
                            </div>
                            <div class="form-group has-feedback col-xs-2">
                                <label class="input-group">
                                    <span class="input-group-addon">
                                        <input type="radio" name="picedital" value="f" class="link element_edital" <?php echo !empty($dadosPI['picedital']) && 'f' == $dadosPI['picedital'] ? 'checked="checked"' : ''; ?>>
                                    </span>
                                    <div class="form-control form-control-static input_check_n link element_edital">
                                        Não
                                    </div>
                                    <span class="glyphicon form-control-feedback "></span>
                                </label>
                            </div>
                        </div>
                    </div>
                </div>
                <!--Inicio campos do Edital-->
                <div class="panel panel-default" id="div_edital">
                    <div class="panel-body">
                        <fieldset><legend>Previsão de Lançamento do Edital:</legend>
                            <div class="form-group">
                                <div class="col-md-2 text-right">
                                    <label for="mes" class="control-label">Mês:</label>
                                </div>
                                <div class="col-lg-10">
                                    <?php
                                        $sql = "
                                            SELECT
                                                mescod as codigo,
                                                mesdsc as descricao
                                            FROM public.meses 
                                            ORDER BY
                                                codigo
                                        ";
                                        $mescod = $dadosPI['mescod'];
                                        $db->monta_combo('mescod', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'mes', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($mescod) ? $mescod: null));
                                    ?>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <!--Fim Campos do Edital-->
                <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset>
                            <legend class="legend_localizacao">Localização do Projeto</legend>
                            <div class="form-group">
                                <div class="col-md-2 text-right">
                                    <label for="esfid" class="control-label">Tipo de localização:</label>
                                </div>
                                <div class="col-lg-10">
                                    <?php
                                        $sql = "
                                            SELECT
                                                esfid AS codigo,
                                                esfdsc AS descricao
                                            FROM territorios.esfera
                                            ORDER BY
                                                descricao
                                        ";
                                        $esfid = $dadosPI['esfid'];
                                        $db->monta_combo('esfid', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'esfid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, null);
                                    ?>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-12">
                                    <table id="table_localizacao" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <tr class="tr_head">
                                            <td style="text-align: left;"><b>UF</b></td>
                                            <td style="text-align: left;"><b>Município</b></td>
                                            <td style="text-align: center;"></td>
                                        </tr>
                                        <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_MUNICIPAL){
                                            foreach($aLocalizacao as $localizacao){ ?>
                                                <tr style="height: 30px;" class="tr_localizacao_<?php echo $localizacao['muncod']; ?>" >
                                                    <td style="text-align: left;"><?php echo $localizacao['estuf']; ?></td>
                                                    <td style="text-align: left;"><?php echo $localizacao['mundescricao']; ?></td>
                                                    <td style="text-align: center;">
                                                        <input type="hidden" value="<?php echo $localizacao['muncod']; ?>" name="listaLocalizacao[]">
                                                        <span class="glyphicon glyphicon-trash btnRemoverLocalizacao link" data-localizacao="<?php echo $localizacao['muncod']; ?>" >
                                                    </td>
                                                </tr>
                                            <?php }
                                        } ?>

                                    </table>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-12">
                                    <table id="table_localizacao_estadual" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <tr class="tr_head">
                                            <td style="text-align: left;"><b>UF</b></td>
                                            <td style="text-align: left;"><b>Nome</b></td>
                                            <td style="text-align: center;"></td>
                                        </tr>
                                        <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_ESTADUAL){
                                            foreach($aLocalizacao as $localizacao){ ?>
                                                <tr style="height: 30px;" class="tr_localizacao_estadual_<?php echo $localizacao['estuf']; ?>" >
                                                    <td style="text-align: left;"><?php echo $localizacao['estuf']; ?></td>
                                                    <td style="text-align: left;"><?php echo $localizacao['estdescricao']; ?></td>
                                                    <td style="text-align: center;">
                                                        <input type="hidden" value="<?php echo $localizacao['estuf']; ?>" name="listaLocalizacaoEstadual[]">
                                                        <span class="glyphicon glyphicon-trash btnRemoverLocalizacaoEstadual link" data-localizacao-estadual="<?php echo $localizacao['estuf']; ?>" >
                                                    </td>
                                                </tr>
                                            <?php }
                                        } ?>
                                    </table>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-12">
                                    <table id="table_localizacao_exterior" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <tr class="tr_head">
                                            <td style="text-align: left;"><b>País</b></td>
                                            <td style="text-align: center;"></td>
                                        </tr>
                                        <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_EXTERIOR){
                                            foreach($aLocalizacao as $localizacao){ ?>
                                                <tr style="height: 30px;" class="tr_localizacao_exterior_<?php echo $localizacao['paiid']; ?>" >
                                                    <td style="text-align: left;"><?php echo $localizacao['paidescricao']; ?></td>
                                                    <td style="text-align: center;">
                                                        <input type="hidden" value="<?php echo $localizacao['paiid']; ?>" name="listaLocalizacaoExterior[]">
                                                        <span class="glyphicon glyphicon-trash btnRemoverLocalizacaoExterior link" data-localizacao-exterior="<?php echo $localizacao['paiid']; ?>" >
                                                    </td>
                                                </tr>
                                            <?php }
                                        } ?>
                                    </table>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-10">

                                </div>
                                <div class="col-lg-2">
                                    <input type="button" id="btn_selecionar_localizacao_estadual" title="Inserir Localização Estadual/Distrito Federal" value="Inserir Localização Estadual/Distrito Federal" class="btn btn-info" style="width: 100%;" />
                                    <input type="button" id="btn_selecionar_localizacao_exterior" title="Inserir Localização no Exterior" value="Inserir Localização no Exterior" class="btn btn-info" style="width: 100%;" />
                                    <input type="button" id="btn_selecionar_localizacao" title="Inserir Localização municipal" value="Inserir Localização municipal" class="btn btn-info" style="width: 100%;" />
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset>
                            <legend class="legend_responsaveis">Responsáveis pelo Projeto</legend>
                            <div class="form-group">
                                <div class="col-lg-12">
                                    <table id="table_responsaveis" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <tr>
                                            <td style="text-align: left;"><b>Nome</b></td>
                                            <td style="text-align: left;"><b>Telefone</b></td>
                                            <td style="text-align: left;"><b>E-mail</b></td>
                                            <td style="text-align: center;"><b>Ação</b></td>
                                        </tr>
                                        <?php foreach($aResponsavel as $responsavel){ ?>
                                            <tr style="height: 30px;" class="tr_responsaveis_<?php echo $responsavel['pirid']; ?>" >
                                                <td style="text-align: left;"><?php echo $responsavel['usunome']; ?></td>
                                                <td style="text-align: left;"><?php echo $responsavel['usutelefone']; ?></td>
                                                <td style="text-align: left;"><?php echo $responsavel['usuemail']; ?></td>
                                                <td style="text-align: center;">
                                                    <input type="hidden" value="<?php echo $responsavel['usucpf']; ?>" name="listaResponsaveis[]">
                                                    <span class="glyphicon glyphicon-trash btnRemoverResponsaveis link" data-responsaveis="<?php echo $responsavel['pirid']; ?>" >
                                                </td>
                                            </tr>
                                        <?php } ?>
                                    </table>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-10">

                                </div>
                                <div class="col-lg-2">
                                    <input type="button" id="btn_selecionar_responsaveis" title="Inserir Responsáveis" value="Inserir Responsáveis" class="btn btn-info" style="width: 100%;" />
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset><legend>Custeio e Capital</legend>
                            <div class="form-group">
                                <div class="col-lg-12">
                                    <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <tr>
                                            <td style="text-align: left;"></td>
                                            <td colspan="3" style="text-align: center;">
                                                <b>Total(Custeio e Capital)</b>
                                            </td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Limite Autorizado na Sub-Unidade: </td>
                                            <td colspan="3" id="td_autorizado_sub_unidade" style="text-align: center;" >0,00</td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Limite Disponível na Sub-Unidade:</td>
                                            <td colspan="3" id="td_disponivel_sub_unidade" style="text-align: center;" >0,00</td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;"></td>
                                            <td style="text-align: left;"><b>Custeio </b></td>
                                            <td style="text-align: center;"><b>Capital</b></td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Saldo Autorizado na Funcional:</td>
                                            <td style="text-align: left;" id="td_autorizado_funcional_custeio" >0,00</td>
                                            <td style="text-align: center;" id="td_autorizado_funcional_capital" >0,00</td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Saldo Disponível na Funcional:</td>
                                            <td style="text-align: left;" id="td_disponivel_funcional_custeio" >0,00</td>
                                            <td style="text-align: center;" id="td_disponivel_funcional_capital" >0,00</td>
                                        </tr>
                                        <tr>
                                            <td id="" style="text-align: left;">Valor do Projeto:</td>
                                            <td style="text-align: left;">
                                                <input type="text" id="picvalorcusteio" name="picvalorcusteio" value="<?php echo !empty($dadosPI['picvalorcusteio']) ? number_format($dadosPI['picvalorcusteio'], 2, ',', '.') : ''; ?>" class="CampoEstilo normal form-control obrigatorio" />
                                            </td>
                                            <td style="text-align: center;">
                                                <input type="text" id="picvalorcapital" name="picvalorcapital" value="<?php echo !empty($dadosPI['picvalorcapital']) ? number_format($dadosPI['picvalorcapital'], 2, ',', '.') : ''; ?>" class="CampoEstilo normal form-control obrigatorio" />
                                            </td>
                                        </tr>
                                    </table>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset><legend>Cronogramas</legend>
                            <div class="form-group">
                                <div class="col-lg-12">
                                    <?php
                                    $sql = "select mescod, mesdsc from public.meses order by mescod";
                                    $meses = $db->carregar($sql);

                                    $modelPiCronograma =  new Pi_Cronograma();

                                    $piCronogramasAgrupado = [];
                                    if(!empty($dadosPI['pliid'])){
                                        $aPiCronograma = $modelPiCronograma->recuperarTodos('*', array('pliid = ' . $dadosPI['pliid']));

                                        foreach($aPiCronograma as $piCronograma){
                                            $piCronogramasAgrupado[$piCronograma['mescod']][$piCronograma['crvid']] = $piCronograma;
                                        }
                                    }

                                    $aCronogramas = $modelPiCronograma->recuperarCronogramas();
                                    $cronogramasAgrupado = [];
                                    foreach($aCronogramas as $cronograma){
                                        $cronogramasAgrupado[$cronograma['crodsc']][] = $cronograma;
                                    }
                                    ?>
                                    <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <tr>
                                            <td rowspan="2" style="text-align: center;"><b>Mês</b></td>
                                            <?php foreach($cronogramasAgrupado as $cronograma => $descricaoValores){ ?>
                                                <td style="text-align: center;" colspan="<?php echo count($descricaoValores); ?>"><b><?php echo $cronograma; ?></b></td>
                                            <?php } ?>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <?php foreach($cronogramasAgrupado as $cronograma => $descricaoValores){
                                                foreach($descricaoValores as $cronogramaValor){ ?>
                                                    <td style="text-align: center;"><?php echo $cronogramaValor['crvdsc']; ?></td>
                                                <?php }
                                            } ?>
                                        </tr>
                                        <?php foreach($meses as $mes){ ?>
                                            <tr style="height: 30px;">
                                                <td style="text-align: left;"><?php echo $mes['mesdsc']; ?></td>
                                                <?php foreach($aCronogramas as $cronogramaValor){
                                                    switch($cronogramaValor['croid']){
                                                        case Cronograma::K_FISICO:          $class = 'input_fisico';        break;
                                                        case Cronograma::K_ORCAMENTARIO:    $class = 'input_orcamentario';  break;
                                                        case Cronograma::K_FINANCEIRO:      $class = 'input_financeiro';    break;
                                                        default: $class = '';
                                                    }
                                                    ?>
                                                    <td>
                                                        <?php 
                                                        $pcrid = $pcrvalor = '';
                                                        if(isset($piCronogramasAgrupado[$mes['mescod']][$cronogramaValor['crvid']])){
                                                            $pcrid = $piCronogramasAgrupado[$mes['mescod']][$cronogramaValor['crvid']]['pcrid'];
                                                            $pcrvalor = $piCronogramasAgrupado[$mes['mescod']][$cronogramaValor['crvid']]['pcrvalor'];
                                                            
                                                            if($cronogramaValor['croid'] != Cronograma::K_FISICO){
                                                                $pcrvalor = number_format($pcrvalor, 2, ',', '.');
                                                            }
                                                        }
                                                        
                                                        ?>
                                                        <input name="cronograma[<?php echo $mes['mescod']; ?>][<?php echo $cronogramaValor['crvid']; ?>][pcrid]" value="<?php echo $pcrid; ?>" type="hidden">
                                                        <input name="cronograma[<?php echo $mes['mescod']; ?>][<?php echo $cronogramaValor['crvid']; ?>][pcrvalor]" value="<?php echo $pcrvalor; ?>" class="CampoEstilo normal form-control obrigatorio <?php echo $class ?>" type="text">
                                                    </td>
                                                <?php }?>
                                            </tr>
                                        <?php }?>
                                        <tr style="height: 30px; ">
                                            <td style="text-align: left;">Total:</td>
                                            <td style="font-weight: bold; text-align: center;" id="td_total_fisico">0</td>
                                            <td style="font-weight: bold; text-align: center;" colspan="2" id="td_total_orcamentario">R$ 0,00</td>
                                            <td style="font-weight: bold; text-align: center;" colspan="2" id="td_total_financeiro">R$ 0,00</td>
                                        </tr>
                                    </table>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div style="display: none;" class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Previsão PI:</label>
                    <div class="col-lg-10">
                        <table class="table table-striped table-bordered table-hover">
                            <tr style="text-align:center">
                                <td style="width:30px;height:20px"><b>Enquadramento</b></td>
                                <td colspan="3"><b>Sequencial</b></td>
                                <td style="width:30px;height:20px"><b>Segmento</b></td>
                                <td style="width:30px;height:20px"><b>Pactuação</b></td>
                                <td style="width:30px;height:20px"><b>Codificação</b></td>
                                <td style="width:30px;height:20px"><b>Área</b></td>
                            </tr>
                            <tr style="text-align:center">
                                <td style="width:30px;height:20px">
                                    <span id="enquadramento"><?php echo $dadosPI['eqdcod']; ?></span>
                                </td>
                                <td style="width:30px;height:20px" colspan="3">
                                    <span id="subacao"><?php echo (empty($dadosPI['sbacod']) ? $dadosPI['plicodsubacao'] : $dadosPI['sbacod']); ?></span>
                                </td>
                                <td style="width:30px;height:20px">
                                    <span id="nivel"><?php echo $dadosPI['neecod']; ?></span>
                                </td>
                                <td style="width:30px;height:20px">
                                    <span id="apropriacao"><?php echo $dadosPI['capcod']; ?></span>
                                </td>
                                <td style="width:30px;height:20px">
                                    <span id="codificacao"><?php echo $dadosPI['plilivre']; ?></span>
                                </td>
                                <td style="width:30px;height:20px">
                                    <span id="modalidade"><?php echo $dadosPI['mdecod']; ?></span>
                                </td>
                            </tr>
                            <tr>
                                <td colspan="8" align="center">
                                    <b>Código PI / SIAFI:
                                        <div id="codigoPi" style="display:inline;">
                                            <span id="enquadramento_i"><?php echo $dadosPI['eqdcod']; ?></span>
                                            <!--//<span id="subacao_i"><?php echo (empty($dadosPI['sbacod']) ? 'XXXX' : $dadosPI['sbacod']); ?></span>-->
                                            <span id="subacao_i"><?php echo (empty($dadosPI['sbacod']) ? $dadosPI['plicodsubacao'] : $dadosPI['sbacod']); ?></span>
                                            <span id="nivel_i"><?php echo $dadosPI['neecod']; ?></span>
                                            <span id="apropriacao_i"><?php echo $dadosPI['capcod']; ?></span>
                                            <span id="codificacao_i"><?php echo $dadosPI['plilivre']; ?></span>
                                            <span id="modalidade_i"><?php echo $dadosPI['mdecod']; ?></span>
                                        </div>
                                    </b>
                                </td>
                            </tr>
                        </table>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <input type="button" class="btn btn-primary" name="btg" onclick="submeter('<?php echo $plicod; ?>//');" value="Salvar" <?php echo $disabled; ?> />
                        
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <?php if('-' != $tipoTransacao): ?>
                            <?php if ($_POST['pliid'] && $plisituacao == 'E'): ?>
                                <input type="button" value="Enviar para Homologação" onclick="vincular('R', '<?php echo $_POST['pliid']; ?>')" style="cursor:pointer" class='btn-info' />
                            <?php endif; ?>
                        <?php endif; ?>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
</div>
<script type="text/javascript">
    // Enquadramentos
    intEnqFinalistico = "<?php echo ENQUADRAMENTO_FINALISTICO; ?>";
    // Localizações
    intEsfidEstadualDF = "<?php echo ESFERA_ESTADUAL_DISTRITO_FEDERAL; ?>";
    intEsfidExterior = "<?php echo ESFERA_EXTERIOR; ?>";
    intEsfidFederalBrasil = "<?php echo ESFERA_FEDERAL_BRASIL; ?>";
    intEsfidMunicipal = "<?php echo ESFERA_MUNICIPAL; ?>";

    $(document).ready(function() {

        var tipoTransacao = $('tipotransacao').value;
        if ('-' == tipoTransacao) {
            // -- Desabilita o restante do formulário
            $('plititulo').disable();
            $('plidsc').disable();
            $('btn_selecionar_acaptres').disable();
        }

        // Retira cor dos elementos marcados pela validação do formulário quando a validação for satisfeita(Quando o elemento for preenchido).
        initTirarCorValidacao();

        // Evento ao mudar opção de enquadramento
        $('#unicod').change(function(){
            carregarUG($(this).val());
        });
        
        $('div.div_ungcod').on('change', '#ungcod', function(){
            carregarLimitesUnidade($(this).val());
        });

        // Evento ao mudar opção de enquadramento
        $('#eqdid').change(function(){
            mudarFormularioFinalistico($(this).val());
        });
        // Evento ao carregar a tela
        mudarFormularioFinalistico($('#eqdid').val());

        // Evento ao mudar opção de Objetivos PPA
        $('#oppid').change(function(){
            carregarMetasPPA($(this).val());
            carregarIniciativaPPA($(this).val());
        });

        // Evento ao mudar opção de Metas PNC
        $('#mpnid').change(function(){
            carregarIndicadorPNC($(this).val());
        });

        // Evento ao mudar opção de Área Cultural
        $('#mdeid').change(function(){
            carregarSegmentoCultural($(this).val());
        });

        $('#input_sniic').mask('9999999999');

        $('#btn_adicionar_sniic').click(function(){
             var trHtml =
                '<tr style="height: 30px;" id="tr_sniic_' + $('#input_sniic').val()+ '" >'
                        + '<td style="text-align: left;">' + $('#input_sniic').val() + '</td>'
                        + '<td style="text-align: center;">'
                            + '<input type="hidden" name="lista_sniic[]" value="' + $('#input_sniic').val() + '" />'
                            + '<span class="glyphicon glyphicon-trash link btnRemoveSniic" data-sniic="' + $('#input_sniic').val()+ '" ></span>'
                        + '</td>'
                + '</tr>';
            $('#table_sniic').append(trHtml);
            $('#input_sniic').val('');
        });

        $('#table_sniic').on('click', '.btnRemoveSniic', function(){
            var sniic = $(this).attr('data-sniic');
            $('#tr_sniic_'+ sniic).remove();
        });

        $('#input_sniic').keypress(function(e){
            if(e.which == 13) {
                $('#btn_adicionar_sniic').click();
            }
        });

        $('#input_convenio').mask('9999999999');

        $('#picquantidade').mask('9999999999');

        $('#btn_selecionar_convenio').click(function(){
            var trHtml =
                '<tr style="height: 30px;" id="tr_convenio_' + $('#input_convenio').val()+ '" >'
                + '<td style="text-align: left;">' + $('#input_convenio').val() + '</td>'
                + '<td style="text-align: center;">'
                + '<input type="hidden" name="lista_convenio[]" value="' + $('#input_convenio').val() + '" />'
                + '<span class="glyphicon glyphicon-trash link btnRemoveConvenio" data-convenio="' + $('#input_convenio').val()+ '" ></span>'
                + '</td>'
                + '</tr>';
            $('#table_convenio').append(trHtml);
            $('#input_convenio').val('');
        });

        $('#table_convenio').on('click', '.btnRemoveConvenio', function(){
            var convenio = $(this).attr('data-convenio');
            $('#tr_convenio_'+ convenio).remove();
        });

        $('#input_convenio').keypress(function(e){
            if(e.which == 13) {
                $('#btn_selecionar_convenio').click();
            }
        });

        $('input.input_fisico').mask('9999999999');

        $('.element_edital').click(function() {
            setTimeout(function(){
                controlarEdital($('input[name=picedital]:checked').val());
            }, 500); // Meio segundo
        });

        $('#table_localizacao').on('click', '.btnRemoverLocalizacao', function(){
            var id = $(this).attr('data-localizacao');
            $('.tr_localizacao_'+ id).remove();
        });

        $('#table_localizacao_estadual').on('click', '.btnRemoverLocalizacaoEstadual', function(){
            var id = $(this).attr('data-localizacao-estadual');
            $('.tr_localizacao_estadual_'+ id).remove();
        });

        $('#table_localizacao_exterior').on('click', '.btnRemoverLocalizacaoExterior', function(){
            var id = $(this).attr('data-localizacao-exterior');
            $('.tr_localizacao_exterior_'+ id).remove();
        });

        $('#table_responsaveis').on('click', '.btnRemoverResponsaveis', function(){
            var cpf = $(this).attr('data-responsaveis');
            $('.tr_responsaveis_'+ cpf).remove();
        });

        // Evento ao carregar a tela
        controlarEdital($('input[name=picedital]:checked').val());

        $('#btn_selecionar_functional').click(function(){
            abrir_lista();
        });

        $('#btn_selecionar_responsaveis').click(function(){
            abrirModalResponsaveis();
        });

        $('#btn_selecionar_localizacao').click(function(){
            abrirModalLocalizacao();
        });

        $('#btn_selecionar_localizacao_estadual').click(function(){
            abrirModalLocalizacaoEstadual();
        });

        $('#btn_selecionar_localizacao_exterior').click(function(){
            abrirModalLocalizacaoExterior();
        });

        $('#esfid').change(function(){
            controlarTipoLocalizacao($(this).val());
        });

        $('#picvalorcusteio').keyup(function(){
            this.value = mascaraglobal('###.###.###.###,##', this.value);
            atualizarValorDoProjeto();
            mudarCorValorProjeto();
        });

        $('#picvalorcapital').keyup(function(){
            this.value = mascaraglobal('###.###.###.###,##', this.value);
            atualizarValorDoProjeto();
            mudarCorValorProjeto();
        });

        $('#picquantidade').keyup(function(){
            mudarCorCronogramaFisico();
            avisarCronogramaFisico();
        });

        $('.input_fisico').keyup(function(){
            mudarCorCronogramaFisico();
            avisarCronogramaFisico();
            atualizarTotalFisico();
        });

        $('.input_orcamentario').keyup(function(){
            this.value = mascaraglobal('###.###.###.###,##', this.value);
            mudarCorCronogramaOrcamentario();
            avisarCronogramaOrcamentario();
            atualizarTotalOrcamentario();
        });

        $('.input_financeiro').keyup(function(){
            this.value = mascaraglobal('###.###.###.###,##', this.value);
            mudarCorCronogramaFinanceiro();
            avisarCronogramaFinanceiro();
            atualizarTotalFinanceiro();
        });

        controlarTipoLocalizacao($('#esfid').val());

    });

    /**
     * Retira cor dos elementos marcados pela validação do formulário quando a validação for satisfeita(Quando o elemento for preenchido).
     * 
     * @returns VOID
     */
    function initTirarCorValidacao(){
        var listaiten = definirCamposObrigatorios();
        listaiten.push('mpnid', 'ipnid');

        $.each(listaiten , function(i , id){
            $('#formulario').on('change', '#' + id, function(){
                tirarCorVermelha(id);
            });
        });
    }

    /**
     * Tira a Cor Vermelha da validação do label do elemento e da tag pai do elemento.
     * 
     * @param string id
     * @returns VOID
     */
    function tirarCorVermelha(id){
        $item = $('#' + id);
        $($item).parent().parent().removeClass('has-error');
        $('label[for="'+ $item.attr('id') +'"]').removeClass('has-error');
    }

    /**
     * Exibe orientações ao usuário para preencher corretamente o Cronograma.
     *
     * @returns VOID
     */
    function avisarCronogramaFisico(){
        if(!validarCronogramaFisicoInferiorQuantidade()){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível salvar o formulário com a soma de todos os valores preenchidos nos meses do cronograma físico superior ao valor informado na Quantidade do Produto do PI. <br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, diminua os valores informados na coluna do cronograma físico ou aumente a Quantidade no Produto do PI.</span></p>');
        }
    }

    /**
     * Exibe orientações ao usuário para preencher corretamente o Cronograma.
     *
     * @returns VOID
     */
    function avisarCronogramaOrcamentario(){
        if(!validarCronogramaOrcamentarioInferiorValorProjeto()){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível salvar o formulário com a soma de todos os valores preenchidos nos meses do cronograma orçamentário superior ao valor informado no Valor do Projeto. <br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, diminua os valores informados nas colunas do cronograma orçamentário ou aumente o Valor do Projeto.</span></p>');
        }
    }

    /**
     * Exibe orientações ao usuário para preencher corretamente o Cronograma.
     *
     * @returns VOID
     */
    function avisarCronogramaFinanceiro(){
        if(!validarCronogramaFinanceiroInferiorValorProjeto()){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível salvar o formulário com a soma de todos os valores preenchidos nos meses do cronograma financeiro superior ao valor informado no Valor do Projeto. <br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, diminua os valores informados nas colunas do cronograma financeiro ou aumente o Valor do Projeto.</span></p>');
        }
    }

    /**
     * Muda a cor dos valores do cronograma Físico se o valor exceder o valor do projeto.
     *
     * @returns VOID
     */
    function mudarCorCronogramaFisico(){
        if(validarCronogramaFisicoInferiorQuantidade()){
            $('input.input_fisico').removeClass('validateRedText');
            $('#td_total_fisico').removeClass('validateRedText');
        } else {
            $('input.input_fisico').addClass('validateRedText');
            $('#td_total_fisico').addClass('validateRedText');
        }
    }

    /**
     * Muda a cor dos valores do cronograma orçamentário se o valor exceder o valor do projeto.
     *
     * @returns VOID
     */
    function mudarCorCronogramaOrcamentario(){
        if(validarCronogramaOrcamentarioInferiorValorProjeto()){
            $('input.input_orcamentario').removeClass('validateRedText');
            $('#td_total_orcamentario').removeClass('validateRedText');
        } else {
            $('input.input_orcamentario').addClass('validateRedText');
            $('#td_total_orcamentario').addClass('validateRedText');
        }
    }

    /**
     * Muda a cor dos valores do cronograma financeiro se o valor exceder o valor do projeto.
     *
     * @returns VOID
     */
    function mudarCorCronogramaFinanceiro(){
        if(validarCronogramaFinanceiroInferiorValorProjeto()){
            $('input.input_financeiro').removeClass('validateRedText');
            $('#td_total_financeiro').removeClass('validateRedText');
        } else {
            $('input.input_financeiro').addClass('validateRedText');
            $('#td_total_financeiro').addClass('validateRedText');
        }
    }

    /**
     * Verifica se o cronograma está preenchido.
     *
     * @returns Boolean
     */
    function validarCronogramaFisicoPreenchido(){
        var valido = false;
        var totalFisico = buscarTotalCronogramaFisico();

        if(totalFisico > 0){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se o cronograma está preenchido com valores inferiores a Quantidade do Produto do PI.
     *
     * @returns Boolean
     */
    function validarCronogramaFisicoInferiorQuantidade(){
        var valido = false;
        var quantidade = textToFloat($('#picquantidade').val());
        var totalFisico = buscarTotalCronogramaFisico();
        if(totalFisico <= quantidade){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se o cronograma está preenchido com valores inferiores ao valor do projeto.
     *
     * @returns Boolean
     */
    function validarCronogramaOrcamentarioInferiorValorProjeto(){
        var valido = false;
        var valorDoProjeto = buscarValorDoProjeto();
        var valorTotalOrcamentario = buscarTotalCronogramaOrcamentario();

        if(valorTotalOrcamentario <= valorDoProjeto){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se o cronograma está preenchido com valores inferiores ao valor do projeto.
     *
     * @returns Boolean
     */
    function validarCronogramaFinanceiroInferiorValorProjeto(){
        var valido = false;
        var valorDoProjeto = buscarValorDoProjeto();
        var valorTotalFinanceiro = buscarTotalCronogramaFinanceiro();

        if(valorTotalFinanceiro <= valorDoProjeto){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se o cronograma está preenchido.
     *
     * @returns Boolean
     */
    function validarCronogramaOrcamentarioPreenchido(){
        var valido = false;
        var valorTotalOrcamentario = buscarTotalCronogramaOrcamentario();

        if(valorTotalOrcamentario > 0){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se o cronograma está preenchido.
     *
     * @returns Boolean
     */
    function validarCronogramaFinanceiroPreenchido(){
        var valido = false;
        var valorTotalFinanceiro = buscarTotalCronogramaFinanceiro();

        if(valorTotalFinanceiro > 0){
            valido = true;
        }

        return valido;
    }

    /**
     * Busca o total informado de todos os meses no cronograma físico.
     *
     * @returns float
     */
    function buscarTotalCronogramaFisico(){
        var totalFisico = 0;
        $('input.input_fisico').each(function(data, key){
            fisico = textToFloat($(this).val());
            totalFisico += fisico;
        });

        return totalFisico;
    }

    /**
     * Busca o total informado de todos os meses no cronograma orcamentario.
     *
     * @returns float
     */
    function buscarTotalCronogramaOrcamentario(){
        var totalOrcamentario = 0;
        $('input.input_orcamentario').each(function(data, key){
            fisico = textToFloat($(this).val());
            totalOrcamentario += fisico;
        });

        return totalOrcamentario;
    }

    /**
     * Busca o total informado de todos os meses no cronograma financeiro.
     *
     * @returns float
     */
    function buscarTotalCronogramaFinanceiro(){
        var totalFinanceiro = 0;
        $('input.input_financeiro').each(function(data, key){
            fisico = textToFloat($(this).val());
            totalFinanceiro += fisico;
        });

        return totalFinanceiro;
    }

    /**
     * Calcula o valor da funcional.
     *
     * @returns VOID
     */
    function atualizarValorDoProjeto() {
        var valorCusteio = textToFloat($('#picvalorcusteio').val());
        var valorCapital = textToFloat($('#picvalorcapital').val());
        var total = valorCusteio + valorCapital;
        $('#td_valor_projeto').html(mascaraglobal('###.###.###.###,##', total.toFixed(2) ));
    }

    /**
     * Exibe o valor total do cronograma Físico na tela.
     *
     * @returns VOID
     */
    function atualizarTotalFisico() {
        var total = buscarTotalCronogramaFisico();
        $('#td_total_fisico').html( total.toFixed(0) );
    }

    /**
     * Exibe o valor total do cronograma Orçamentário na tela.
     *
     * @returns VOID
     */
    function atualizarTotalOrcamentario() {
        var total = buscarTotalCronogramaOrcamentario();
        $('#td_total_orcamentario').html('R$ '+ mascaraglobal('###.###.###.###,##', total.toFixed(2) ));
    }

    /**
     * Exibe o valor total do cronograma Financeiro na tela.
     *
     * @returns VOID
     */
    function atualizarTotalFinanceiro() {
        var total = buscarTotalCronogramaFinanceiro();
        $('#td_total_financeiro').html('R$ '+ mascaraglobal('###.###.###.###,##', total.toFixed(2) ));
    }

    /**
     * Soma o valor de capital e custeio e retorna o valor total do projeto.
     *
     * @returns float Valor do projeto
     */
    function buscarValorDoProjeto(){
        var valorCusteio = textToFloat($('#picvalorcusteio').val());
        var valorCapital = textToFloat($('#picvalorcapital').val());
        var valorDoProjeto = valorCusteio + valorCapital;

        return valorDoProjeto;
    }

    /**
     * Soma o valor de capital e custeio e retorna o valor total disponivel.
     *
     * @returns float Valor
     */
    function buscarValorDisponivelFuncional(){
        var valorCusteio = textToFloat($('#td_disponivel_funcional_custeio').text());
        var valorCapital = textToFloat($('#td_disponivel_funcional_capital').text());
        var disponivelFuncional = valorCusteio + valorCapital;

        return disponivelFuncional;
    }

    /**
     * Soma o valor de capital e custeio e retorna o valor total disponivel.
     *
     * @returns float Valor
     */
    function buscarValorAutorizadoFuncional(){
        var valorCusteio = textToFloat($('#td_autorizado_funcional_custeio').text());
        var valorCapital = textToFloat($('#td_autorizado_funcional_capital').text());
        var autorizadoFuncional = valorCusteio + valorCapital;

        return autorizadoFuncional;
    }

    /**
     * Muda a cor do valor de projeto capital e custeio quando o valor disponvel
     * da sub-unidade ou funcional for ultrapassado.
     *
     * @returns VOID
     */
    function mudarCorValorProjeto(){
        var disponivelUnidade = textToFloat($('#td_disponivel_sub_unidade').text());
        var disponivelFuncional = buscarValorDisponivelFuncional();
        var valorDoProjeto = buscarValorDoProjeto();

        if(valorDoProjeto > disponivelUnidade || valorDoProjeto > disponivelFuncional){
            $('#picvalorcusteio').addClass('validateRedText');
            $('#picvalorcapital').addClass('validateRedText');
            $('#td_valor_projeto').addClass('validateRedText');
        } else {
            $('#picvalorcusteio').removeClass('validateRedText');
            $('#picvalorcapital').removeClass('validateRedText');
            $('#td_valor_projeto').removeClass('validateRedText');
        }
    }

   /**
    * Transforma o valor de texto pra flutuante pra efetuar operações matematicas.
    *
    * @param string text
    * @return float numero
    */
    function textToFloat(text){
        var numero = 0;
        text = replaceAll(text, '.', '');
        text = replaceAll(text, ',', '.');
        if(parseFloat(text) > 0){
            numero = parseFloat(text);
        }

        return numero;
    }

    /**
     * Controla as opções do formulario conforme a opção de tipo de localização selecionada.
     *
     * @param int esfid Código da esfera preenchido pelo usuário
     * @return VOID
     */
    function controlarTipoLocalizacao(esfid){
        $('#btn_selecionar_localizacao').hide();
        $('#btn_selecionar_localizacao_estadual').hide();
        $('#btn_selecionar_localizacao_exterior').hide();
        $('#table_localizacao tr').hide();
        $('#table_localizacao_estadual tr').hide();
        $('#table_localizacao_exterior tr').hide();

        switch (esfid) {
            // Verifica se a esfera é Estadual/DF.
            case intEsfidEstadualDF:
                $('#table_localizacao_estadual tr').show('slow');
                $('#btn_selecionar_localizacao_estadual').show('slow');

                $('#table_localizacao tr').not('tr.tr_head').remove();
                $('#table_localizacao_exterior tr').not('tr.tr_head').remove();
            break;
            // Verifica se a esfera é Exterior.
            case intEsfidExterior:
                $('#table_localizacao_exterior tr').show('slow');
                $('#btn_selecionar_localizacao_exterior').show('slow');
                $('#table_localizacao tr').not('tr.tr_head').remove();
                $('#table_localizacao_estadual tr').not('tr.tr_head').remove();
            break;
            // Verifica se a esfera é Federal.
            case intEsfidFederalBrasil:
                $('#btn_selecionar_localizacao').hide('slow');
                $('#table_localizacao tr').hide('slow');
                $('#table_localizacao tr').not('tr.tr_head').remove();
                $('#table_localizacao_estadual tr').not('tr.tr_head').remove();
                $('#table_localizacao_exterior tr').not('tr.tr_head').remove();
            break;
            // Verifica se a esfera é Municipal.
            case intEsfidMunicipal:
                $('#table_localizacao tr').show('slow');
                $('#btn_selecionar_localizacao').show('slow');
                $('#table_localizacao_estadual tr').not('tr.tr_head').remove();
                $('#table_localizacao_exterior tr').not('tr.tr_head').remove();
            break;
        }
    }

    function abrirModalResponsaveis() {
        // Verifica se o modal terá que carregar a tela.
        if($('#modal_responsaveis .modal-body p').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/pi-responsaveis&acao=A&ungcod='+ $("#ungcod").val(),
                function(response) {
                    $('#modal_responsaveis .modal-body p').html(response);
                    $('.modal_dialog_responsaveis').show();
                    $('#modal_responsaveis').modal();
                    $('#modal_responsaveis .chosen-select').chosen();
                    $('#modal_responsaveis .chosen-container').css('width', '100%');
            });
        } else {
            $('#formulario_responsaveis input[name=ungcod]').val($("#ungcod").val());
            $('#modal_responsaveis').modal();
            $('#btnPopupResponsaveisPesquisar').click();
        }
    }

    function abrirModalLocalizacao() {
        // Verifica se o modal terá que carregar a tela.
        if($('#modal_localizacao .modal-body p').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/pi-localizacao&acao=A', function(response) {
                    $('#modal_localizacao .modal-body p').html(response);
                    $('.modal_dialog_localizacao').show();
                    $('#modal_localizacao').modal();
                    $('#modal_localizacao .chosen-select').chosen();
                    $('#modal_localizacao .chosen-container').css('width', '100%');
            });
        } else {
            $('#modal_localizacao').modal();
            $('#btnPopupLocalizacaoPesquisar').click();
        }
    }

    function abrirModalLocalizacaoEstadual() {
        // Verifica se o modal terá que carregar a tela.
        if($('#modal_localizacao_estadual .modal-body p').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/pi-localizacao-estadual&acao=A', function(response) {
                    $('#modal_localizacao_estadual .modal-body p').html(response);
                    $('.modal_dialog_localizacao_estadual').show();
                    $('#modal_localizacao_estadual').modal();
                    $('#modal_localizacao_estadual .chosen-select').chosen();
                    $('#modal_localizacao_estadual .chosen-container').css('width', '100%');
            });
        } else {
            $('#modal_localizacao_estadual').modal();
            $('#btnPopupLocalizacaoEstadualPesquisar').click();
        }
    }

    function abrirModalLocalizacaoExterior() {
        // Verifica se o modal terá que carregar a tela.
        if($('#modal_localizacao_exterior .modal-body p').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/pi-localizacao-exterior&acao=A', function(response) {
                    $('#modal_localizacao_exterior .modal-body p').html(response);
                    $('.modal_dialog_localizacao_exterior').show();
                    $('#modal_localizacao_exterior').modal();
                    $('#modal_localizacao_exterior .chosen-select').chosen();
                    $('#modal_localizacao_exterior .chosen-container').css('width', '100%');
            });
        } else {
            $('#modal_localizacao_exterior').modal();
            $('#btnPopupLocalizacaoExteriorPesquisar').click();
        }
    }

   /**
    * Controla ações para quando o botão edital estiver marcado ou desmarcado.
    */
    function controlarEdital(opcao){
        if(opcao == 't'){
            $('#div_edital').show('slow');
        } else {
            $('#div_edital').hide('slow');
        }
    }

    /**
     * Controla a exibição do formulario de acordo com o enquadramento selecionado.
     *
     * @param integer codigo Código selecionado pelo usuário.
     * @returns VOID
     */
    function mudarFormularioFinalistico(codigo){
        // Se o código for diferente 354 - Finalistico, o sistema oculta as opções PNC.
        if(codigo != intEnqFinalistico){
            // Oculta as opções Meta PNC e Indicador PNC.
            $('.grupo_pnc').hide('slow');
            // Apaga as opções selecionadas.
            $('#mpnid').val('').trigger("chosen:updated");
            $('#ipnid').val('').trigger("chosen:updated");
        } else {
            // Mostra as opções Meta PNC e Indicador PNC.
            $('.grupo_pnc').show('slow');
        }
    }

    /**
     * Carrega novo conteúdo para o select de Subações via requisição ajax.
     */
    function carregarUG(unicod) {
        $.post(window.location + '&carregarComboUG=ok&unicod=' + unicod, function(response) {
            $('#ungcod').remove('slow');
            $('.div_ungcod').html(response);
            $(".chosen-select").chosen();
        });
    }
    function carregarSubacao(unicod) {
//        var params = '&carregarComboSubacaoFederais=ok&unicod=' + unicod;
//        $.post(window.location + params, function(response) {
//            $('#sbaid').remove();
//            $('.cSubacao').html(response);
//            $(".chosen-select").chosen();
//        });
    }

    function carregarSemSubacao(unicod) {
        //alert(unicod);
        var params = 'carregarComboSubacaoInstituicoes=1&unicod=' + unicod;
        new Ajax.Request(
            window.location.href,
            {
                method: 'post',
                parameters: params,
                asynchronous: false,
                onComplete: function(response) {
                    //alert(response.responseText);
                    //limparFormulario(false);
                    $('plicodsubacao').update(response.responseText);
                }
            }
        );
    }

    /**
     * Carrega novo conteúdo para o select de Metas PPA via requisição ajax.
     */
    function carregarMetasPPA(codigo) {
        $.post(window.location + '&carregarMetasPPA=ok&oppid=' + codigo, function(response) {
            $('#mppid').remove();
            $('.div_mppid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega limites da Sub-Unidade via requisição ajax.
     */
    function carregarLimitesUnidade(codigo) {
        $.ajax({
            url: window.location + '&carregarLimitesUnidade=ok',
            type: "post",
            data: {'ungcod': codigo},
            dataType: 'json',
            success: function(data){
                $('#td_autorizado_sub_unidade').text(data.autorizado);
                $('#td_disponivel_sub_unidade').text(data.disponivel);
            }
        });
    }
    
    /**
     * Carrega novo conteúdo para o select de Indicadores PNC via requisição ajax.
     */
    function carregarIndicadorPNC(codigo) {
        $.post(window.location + '&carregarIndicadorPNC=ok&mpnid=' + codigo, function(response) {
            $('#ipnid').remove();
            $('.div_ipnid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega novo conteúdo para o select de Segmento Cultural via requisição ajax.
     */
    function carregarSegmentoCultural(codigo) {
        $.post(window.location + '&carregarSegmentoCultural=ok&mdeid=' + codigo, function(response) {
            $('#neeid').remove();
            $('.div_neeid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega novo conteúdo para o select de Metas PPA via requisição ajax.
     */
    function carregarIniciativaPPA(codigo) {
        $.post(window.location + '&carregarIniciativaPPA=ok&oppid=' + codigo, function(response) {
            $('#ippid').remove();
            $('.div_ippid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Apaga todo o conteúdo o formulário. O select de subação é opcional.
     */
    function limparFormulario(limparSubacao)
    {
        if (limparSubacao) {
            $('sbaid').length = 0;
            var sbaid = $('sbaid');
            sbaid.options[sbaid.options.length] = new Option('Selecione', '');
        }
        $('sbadsc').update('Descrição...');
        $('valor_dotacao').update('0,00');
        $('scpdotacaosubacao').value = '0,00';
        $('valor_detalhado').update('0,00');
        $('scpdetalhadopisubacao').value = '0,00';
        $('valor_empenhado').update('0,00');
        $('scpempenhadosubacao').value = '0,00';

        $('eqdid').length = 0;
        var eqdid = $('eqdid');
        eqdid.options[eqdid.options.length] = new Option('Selecione', '');
        $('plititulo').update();
        $('prefixotitulo').update();
        $('enquadramento').update();
        $('enquadramento_i').update();
        $('subacao').update('XXXX');
        $('subacao_i').update('XXXX');
        $('nivel').update();
        $('nivel_i').update();
        $('apropriacao').update();
        $('apropriacao_i').update();
        $('codificacao').update();
        $('codificacao_i').update();
        $('modalidade').update();
        $('modalidade_i').update();
    }

    function validar(plicod)
    {
        var msg = "";
        var valorNulo = "";
        var semDet = "";
        var tabela = document.getElementById('orcamento');
        var ehSolicitacao = $('ehSolicitacao').value;
        var tipoTransacao = $('tipotransacao').value;
        if ($('unicod').selectedIndex < 1) {
            msg += "O preenchimento do campo Unidade Orçamentária é obrigatório.\n";
        }
        // validando se existe ação selecionado/ valor
        if (!ehSolicitacao || 'E' == tipoTransacao) {
            if (tabela.rows.length == 5) {
                semDet += "Você deve selecionar, no mínimo, uma ação no Detalhamento Orçamentário.\n";
            }
            else {
                jQuery('.somar').each(function() {
                    var valor = jQuery(this).val();
                    if ('' === valor || '0' === valor || '0,00' === valor) {
                        valorNulo += "Valor do PTRES '" + jQuery(this).attr('data-ptres') + "' está vazio.\n";
                    }
                });
            }

        }
        if (semDet != "" && msg == "") {
            var resposta = confirm(valorNulo + "Deseja realmente gravar um PI sem Detalhamento?");
            if (resposta != true) {
                return false;
            } else {
                return true;
            }
        }
        if (valorNulo != "" && msg == "") {
            var resposta = confirm(valorNulo + "Deseja realmente gravar um PI sem valor previsto?");
            if (resposta != true) {
                return false;
            } else {
                return true;
            }
        }
        if (valorNulo == "" && msg == "" && semDet == "") {
            return true;
        }
    }

    function validapi(pi)
    {
        var retorno = true;

        var req = new Ajax.Request(window.location.href, {
            method: 'post',
            parameters: 'piAjax=' + pi + '&pliid=' + document.getElementById('pliid').value,
            asynchronous: false,
            onComplete: function(res) {
                x = res.responseText;
                retorno = valida2(x);
            }
        });

        return retorno;
    }

    function valida2(pi)
    {
        if (pi.indexOf('pijaexiste') != -1) {
            while (pi.substr(0, 1) == ' ') {
                pi = pi.substr(1, pi.length);
            }
        } else {
            pi = pi.replace(/ /g, '');
        }
        if (!pi || pi == '') {
            return true;
        } else {
            if (pi.substr(0, 10) == 'pijaexiste') {
                pi = pi.replace('pijaexiste', "");
                var alertaDisplay = '<div class="titulo_box">Atenção!</div><div class="texto_box" >Plano Interno já existe!</div><div class="conteudo_box" >Veja abaixo os dados o Plano Interno cadastrado :</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" ><input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
                displayStaticMessage(alertaDisplay, false);
                return false;
            }
            var alertaDisplay = '<div class="titulo_box">Atenção!</div><div class="texto_box" >Já existe(m) PI(s) criado(s) com esta estrutura. Veja abaixo a relação dos PI(s) encontrados:</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" >Deseja realmente criar?<br><center><input type="button" onclick=\'document.formulario.submit();\' value="Confirmar" /> <input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
            displayStaticMessage(alertaDisplay, false);
            return false;
        }
    }

    function removerpi()
    {
        var conf = confirm("Você realmente deseja excluir este PI?");
        if (conf) {
            document.getElementById('evento').value = 'E';
            document.formulario.submit();
        }
    }

    function Trim(str)
    {
        return str.replace(/^\s+|\s+$/g, "");
    }

    //coloca tabindex no campo valor
    function tabindexcampo() {
        var x = document.getElementsByTagName("input");
        var y = 1;
        for (i = 0; i < x.length; i++) {
            if (x[i].type == "text") {
                if (x[i].name.substr(0, 8) == 'plivalor') {
                    x[i].tabIndex = y;
                    y++;
                }
            }
        }
    }

    function semSubacao() {
        var nome = $("#plicodsubacao").val();
        $("#subacao").text(nome);
        $("#subacao_i").text(nome);
        //$("#prefixotitulo").html(nome + ' - ');
        $('#plititulo').attr('maxlength', 45 - 3 - Trim(nome).length);
    }

    function comSubacao() {
        var nome = jQuery("#plicodsubacao").val();
        $("#subacao").text(nome);
        $("#subacao_i").text(nome);
    }
    function vincular(situacao, pliid) {
        if (pliid) {
            var url = window.location.href;
            var parametros = "requisicao=vincular&pliid=" + pliid + '&situacao=' + situacao;
            var myAjax = new Ajax.Request(
                url,
                {
                    method: 'post',
                    parameters: parametros,
                    asynchronous: false,
                    onComplete: function(r) {
                        if (r.responseText) {
                            alert('Dados gravados com Sucesso.');
                            // feito isso por causa da presa.
                            //window.location.reload();
                            document.formulario.submit();
                        }
                    }
                }
            );
        }
    }

    function limpaDetalhamentoOrcamentario(){
        if($('#orcamento').find('tr')[4] != undefined){
            tamanho = $('#orcamento').find('tr').size();
            for(i = 2; i < tamanho - 2; tamanho--){
                $('#orcamento').find('tr')[i].remove();
            }
        }
    }

    function selecionarsubacao(sbaid) {
        //Limpar tabela de Detalhamento Orçamentário
        limpaDetalhamentoOrcamentario();

        var sbaidAnterior = document.getElementById("sbaidAnterior").value;
        if (!sbaidAnterior) {
            document.getElementById("sbaidAnterior").value = sbaid;
        }

        if (sbaid) {
            document.getElementById("subacao").innerHTML = "XXXX";
            document.getElementById("subacao_i").innerHTML = "XXXX";

            for (var i = (document.getElementById("orcamento").rows.length - 3); i > 2; i--) {
                document.getElementById("orcamento").deleteRow(i);
            }
            $.post(window.location + '&sbaAjax=' + sbaid, function(response) {
                var dados = response;
                dados = dados.split("!@#");

                $('#prefixotitulo').html(Trim(dados[1]) + ' - ');
                $('#plititulo').value = '';
                $('#plititulo').attr('maxlength', 45 - 3 - Trim(dados[1]).length);
                $("#subacao").html(dados[0]);
                $("#subacao_i").html(dados[0]);
                $("#sbadsc").html(dados[3]);
                $("#valor_dotacao").html(dados[4]);
                $('#scpdotacaosubacao').value = dados[4];
                $("#valor_detalhado").html(dados[5]);
                $('#scpdetalhadopisubacao').value = dados[5];
                $("#valor_empenhado").html(dados[6]);
                $('#scpempenhadosubacao').value = dados[6];
            });

            atualizaTituloCodigoLivre();
            document.getElementById("sbaidAnterior").value = sbaid;

        } else {
            document.getElementById("subacao").innerHTML = 'XXXX';
            document.getElementById("subacao_i").innerHTML = 'XXXX';
            document.getElementById('sbadsc').innerHTML = 'XXXX';

            for (var i = (document.getElementById("orcamento").rows.length - 3); i > 1; i--) {
                document.getElementById("orcamento").deleteRow(i);
            }
        }
    }

    function submeter(pliid) {
//        var codsubacao = $('#subacao').text();
//        if ('' === codsubacao) {
//            alert('O código da subação não pode ser deixado em branco.');
//            return false;
//        }

        // O número do PI será gerado somente após a aprovação do PI pela equipe de Coordenação.
//        var pi = document.getElementById("enquadramento").innerHTML
//            + codsubacao
//            + document.getElementById("nivel").innerHTML
//            + document.getElementById("apropriacao").innerHTML
//            + document.getElementById("codificacao").innerHTML
//            + document.getElementById("modalidade").innerHTML;
//        $("#plicod").val(pi);

//        var validado = true;
//        var l = $('#plicodsubacao').val();
//        var sub = l.length;
//        $('plititulo').value = $('prefixotitulo').textContent + $('plititulo').value;

        // Msgs personalizadas de validação.
        addMsgCustom = new Array();

        if($('input[name="ptrid"]').size() == 0){
            $('.legend_funcional').addClass('validateRedText');
            addMsgCustom.push('Funcional');
        } else {
            $('.legend_funcional').removeClass('validateRedText');
        }

        // Se não for adicionado sniic, acrescenta uma msg ao erro.
        if($('[name="lista_sniic[]"]').size() == 0){
            addMsgCustom.push('Números SNIIC');
            $('.legend_sniic').addClass('validateRedText');
        } else {
            $('.legend_sniic').removeClass('validateRedText');
        }
        // Se não for adicionado convenio, acrescenta uma msg ao erro.
        if($('[name="lista_convenio[]"]').size() == 0){
            addMsgCustom.push('Números De Convênio');
            $('.legend_convenio').addClass('validateRedText');
        } else {
            $('.legend_convenio').removeClass('validateRedText');
        }
        // Se não for selecionado nenhuma opção para edital, o sistema acrescenta uma mensagem de erro.
        if($('input[name=picedital]:checked').val() == undefined || $('input[name=picedital]:checked').val() == ""){
            $('label[for="picedital"]').addClass('validateRedText');
            addMsgCustom.push('Edital');
        } else {
            $('label[for="picedital"]').removeClass('validateRedText');
        }
        // Se não for selecionado nenhuma opção para ted, o sistema acrescenta uma mensagem de erro.
        if($('input[name=picted]:checked').val() == undefined || $('input[name=picted]:checked').val() == ""){
            $('label[for="picted"]').addClass('validateRedText');
            addMsgCustom.push('TED');
        } else {
            $('label[for="picted"]').removeClass('validateRedText');
        }

        // Se não for selecionado nenhum tipo de localização, o sistema acrescenta uma mensagem de erro.
        if($('#esfid').val() == ""){
            $('.legend_localizacao').addClass('validateRedText');
            addMsgCustom.push('Localização do Projeto');
        } else {
            switch($('#esfid').val()) {
                // Verifica se a esfera é Estadual/DF.
                case intEsfidEstadualDF:
                    // Verifica se existe não foi inserido item na lista de localização municipal.
                    if($('input[name="listaLocalizacaoEstadual[]"]').size() == 0){
                        $('.legend_localizacao').addClass('validateRedText');
                        addMsgCustom.push('Inserir Localização do Projeto Estadual/Distrito Federal');
                    } else {
                        $('.legend_localizacao').removeClass('validateRedText');
                    }
                break;
                // Verifica se a esfera é Exterior.
                case intEsfidExterior:
                    // Verifica se existe não foi inserido item na lista de localização no Exterior.
                    if($('input[name="listaLocalizacaoExterior[]"]').size() == 0){
                        $('.legend_localizacao').addClass('validateRedText');
                        addMsgCustom.push('Inserir Localização do Projeto no Exterior');
                    } else {
                        $('.legend_localizacao').removeClass('validateRedText');
                    }
                break;
                // Verifica se a esfera é Municipal.
                case intEsfidMunicipal:
                    // Verifica se existe não foi inserido item na lista de localização municipal.
                    if($('input[name="listaLocalizacao[]"]').size() == 0){
                        $('.legend_localizacao').addClass('validateRedText');
                        addMsgCustom.push('Inserir Localização do Projeto Municipal');
                    } else {
                        $('.legend_localizacao').removeClass('validateRedText');
                    }
                break;
                default:
                    $('.legend_localizacao').removeClass('validateRedText');
                break;
            }
        }

        // Se não for inserido nenhum Responsável, o sistema acrescenta uma mensagem de erro.
        if($('#table_responsaveis input[name="listaResponsaveis[]"]').size() == 0){
            $('.legend_responsaveis').addClass('validateRedText');
            addMsgCustom.push('Responsáveis pelo Projeto');
        } else {
            $('.legend_responsaveis').removeClass('validateRedText');
        }

        // Valida se o usuário preencheu Valor do Projeto - Custeio.
        if($('#picvalorcusteio').val() == ""){
            $('#valor_projeto').addClass('validateRedText');
            addMsgCustom.push('Custeio');
        } else {
            $('#valor_projeto').removeClass('validateRedText');
        }

        // Valida se o usuário preencheu Valor do Projeto - Capital.
        if($('#picvalorcapital').val() == ""){
            $('#valor_projeto').addClass('validateRedText');
            addMsgCustom.push('Capital');
        } else {
            $('#picvalorcapital').removeClass('validateRedText');
        }

        /*
         *
         * @todo Refatorar esse código de validação da parte Custeio e Capital dividindo em funções.
         */
        var disponivelUnidade = textToFloat($('#td_disponivel_sub_unidade').text());
        var disponivelFuncional = buscarValorDisponivelFuncional();
        var valorDoProjeto = buscarValorDoProjeto();

        if(valorDoProjeto > disponivelUnidade || valorDoProjeto > disponivelFuncional){
            $('#picvalorcusteio').addClass('validateRedText');
            $('#picvalorcapital').addClass('validateRedText');
            $('#td_valor_projeto').addClass('validateRedText');
            if(valorDoProjeto > disponivelUnidade){
                addMsgCustom.push('Valor do projeto superior ao limite disponível da Sub-Unidade');
            }
            if(valorDoProjeto > disponivelFuncional){
                addMsgCustom.push('Valor do projeto superior ao limite disponível da Funcional');
            }
        } else {
            $('#picvalorcusteio').removeClass('validateRedText');
            $('#picvalorcapital').removeClass('validateRedText');
            $('#td_valor_projeto').removeClass('validateRedText');
        }

        // Verifica se o cronograma físico foi preenchido.
        if(!validarCronogramaFisicoPreenchido()){
            $('input.input_fisico').addClass('validateRedText');
            $('#td_total_fisico').addClass('validateRedText');
            addMsgCustom.push('Cronograma Fisíco');
        } else {
            $('input.input_fisico').removeClass('validateRedText');
            $('#td_total_fisico').removeClass('validateRedText');
        }

        // Verifica se o cronograma orçamentário foi preenchido.
        if(!validarCronogramaOrcamentarioPreenchido()){
            $('input.input_orcamentario').addClass('validateRedText');
            $('#td_total_orcamentario').addClass('validateRedText');
            addMsgCustom.push('Cronograma Orçamentário');
        } else {
            $('input.input_orcamentario').removeClass('validateRedText');
            $('#td_total_orcamentario').removeClass('validateRedText');
        }

        // Verifica se o cronograma financeiro foi preenchido.
        if(!validarCronogramaFinanceiroPreenchido()){
            $('input.input_financeiro').addClass('validateRedText');
            $('#td_total_financeiro').addClass('validateRedText');
            addMsgCustom.push('Cronograma Financeiro');
        } else {
            $('input.input_financeiro').removeClass('validateRedText');
            $('#td_total_financeiro').removeClass('validateRedText');
        }

        // Verifica se o valor do cronograma Físico é superior ao informado no Produto do PI.
        if(!validarCronogramaFisicoInferiorQuantidade()){
            $('input.input_fisico').addClass('validateRedText');
            $('#td_total_fisico').addClass('validateRedText');
            addMsgCustom.push('Cronograma Fisíco com valor superior ao Produto do PI');
        } else {
            $('input.input_fisico').removeClass('validateRedText');
            $('#td_total_fisico').removeClass('validateRedText');
        }
        // Verifica se o valor do cronograma Orçamentário é superior ao valor do Projeto.
        if(!validarCronogramaOrcamentarioInferiorValorProjeto()){
            $('input.input_orcamentario').addClass('validateRedText');
            $('#td_total_orcamentario').addClass('validateRedText');
            addMsgCustom.push('Cronograma Orçamentário com valor superior ao Valor do Projeto');
        } else {
            $('input.input_orcamentario').removeClass('validateRedText');
            $('#td_total_orcamentario').removeClass('validateRedText');
        }
        // Verifica se o valor do cronograma financeiro é superior ao valor do Projeto.
        if(!validarCronogramaFinanceiroInferiorValorProjeto()){
            $('input.input_financeiro').addClass('validateRedText');
            $('#td_total_financeiro').addClass('validateRedText');
            addMsgCustom.push('Cronograma Financeiro com valor superior ao Valor do Projeto');
        } else {
            $('input.input_financeiro').removeClass('validateRedText');
            $('#td_total_financeiro').removeClass('validateRedText');
        }

        listaObrigatorios = definirCamposObrigatorios();
        validarFormulario(listaObrigatorios, 'formulario', 'validar('+ pliid +')', addMsgCustom);
    }

    /**
     * Controla a obrigatóriedade dos campos do formulario de acordo com o enquadramento selecionado.
     *
     * @returns Array
     */
    function definirCamposObrigatorios(){
        var codigoEnquadramento = $('#eqdid').val();
        var listaObrigatorios = ['plititulo', 'plidsc', 'unicod', 'ungcod','eqdid', 'oppid', 'picted', 'picedital', 'mppid', 'ippid', 'neeid', 'capid', 'pprid', 'pumid', 'mdeid', 'picquantidade'];

        // Se o código for diferente 354 - Finalistico, o sistema não define como obrigatório o preenchimento das opções PNC.
        if(codigoEnquadramento == intEnqFinalistico){
            listaObrigatorios.push('mpnid', 'ipnid');
        }

        var opcaoEdital = $('input[name=picedital]:checked').val();
        if(opcaoEdital == 't'){
            listaObrigatorios.push('mes');
        }

        return listaObrigatorios;
    }

    function submeterComSituacao(pliid, situacao) {
        var validado = true;

        if (!validar()) {
            return false;
        }

        if (validado) {
            document.formulario.plisituacao.value = situacao;
            document.formulario.submit();
        }
    }

    function alterarpi(pliid) {
        document.getElementById('evento').value = 'A';
        document.getElementById('pliid').value = pliid;
        document.formulario.submit();
    }

    /* Função para subustituir todos */
    function replaceAll(str, de, para) {
        var pos = str.indexOf(de);
        while (pos > -1) {
            str = str.replace(de, para);
            pos = str.indexOf(de);
        }
        return (str);
    }

    function abrir_lista() {
        // Verifica se o modal terá que carregar a tela
        if($('#modal-ptres .modal-body p').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/popupptres&acao=A&obrigatorio=n&unicod='+ $("#unicod").val(),
                function(response) {
                    $('#modal-ptres .modal-body p').html(response);
                    $('.modal-dialog-ptres').show();
                    $('#modal-ptres').modal();
                    $('#modal-ptres .chosen-select').chosen();
                    $('#modal-ptres .chosen-container').css('width', '100%');
            });
        } else {
            $('#formularioPopup input[name=unicod]').val($("#unicod").val());
            $('#modal-ptres').modal();
            $('#btnPopupPtresPesquisar').click();
        }
    }

    function atualizarPrevisaoPI() {
        if (document.getElementById('capid').value) {
            var apropriacao = document.getElementById('capid').options[document.getElementById('capid').selectedIndex].text.split(" - ");
            document.getElementById("apropriacao").innerHTML = apropriacao[0];
            document.getElementById("apropriacao_i").innerHTML = apropriacao[0];
        }

        if (document.getElementById('eqdid').value) {
            var enquadramento = document.getElementById('eqdid').options[document.getElementById('eqdid').selectedIndex].text.split(" - ");
            document.getElementById("enquadramento").innerHTML = enquadramento[0];
            document.getElementById("enquadramento_i").innerHTML = enquadramento[0];
        } else {
            document.getElementById("enquadramento").innerHTML = '';
            document.getElementById("enquadramento_i").innerHTML = '';
        }

        if (document.getElementById('neeid').value) {
            var nivel = document.getElementById('neeid').options[document.getElementById('neeid').selectedIndex].text.split(" - ");
            document.getElementById("nivel").innerHTML = nivel[0];
            document.getElementById("nivel_i").innerHTML = nivel[0];
        }
        if (document.getElementById('mdeid').value) {
            var modalidade = document.getElementById('mdeid').options[document.getElementById('mdeid').selectedIndex].text.split(" - ");
            document.getElementById("modalidade").innerHTML = modalidade[0];
            document.getElementById("modalidade_i").innerHTML = modalidade[0];
        }

        document.getElementById("idCodificacao").value = document.getElementById("idCodificacao").value.toUpperCase();
        document.getElementById("codificacao").innerHTML = document.getElementById("idCodificacao").value;
        document.getElementById("codificacao_i").innerHTML = document.getElementById("idCodificacao").value;

        atualizaTituloCodigoLivre();
        //semSubacao();
    }


    function atualizarCodigoLivre() {
        document.getElementById("idCodificacao").value = document.getElementById("idCodificacao").value.toUpperCase();
        document.getElementById("codificacao").innerHTML = document.getElementById("idCodificacao").value;
        atualizaTituloCodigoLivre();
    }

    function carregarComboEnquadramentoPorSubacao(sbaid) {
        var req = new Ajax.Request(window.location.href, {
            method: 'post',
            parameters: '&carregarComboEnquadramentoPorSubacao=1&sbaid=' + sbaid,
            asynchronous: false,
            onComplete: function(res) {
                if (res.responseText) {
                    $('comboEnquadramento').update(res.responseText);
                }
            }
        });
        atualizarPrevisaoPI();
    }

    function atualizaTituloCodigoLivre() {
        var idCodificacao = document.getElementById("idCodificacao").value;
        if (idCodificacao == '00') {
            var msg = "";
            if (!document.getElementById('capid').value) {
                msg += 'Favor escolha uma Categoria de Apropriação.\n';
            }

            if (msg) {
                alert(msg);
                return false;
            }
        }
    }
    function subacao(estilo) {
        if (estilo == 's') {
            $('#cSubacao').css("display", "none");
            $('#sSubacao').css("display", "block");
            $('#detSub').html('Detalhado em Subação (R$)');
            $('#detPi').html('Detalhado em PI (R$)');
            $('#prefixotitulo').parent().css('display','none');
            $('#plititulo').parent().removeClass('col-lg-9');
            $('#plititulo').parent().addClass('col-lg-10');
            $('#tipoSubacao').val('s');
        } else {
            $('#cSubacao').css("display", "block");
            $('#sSubacao').css("display", "none");
            $('#detSub').html('Detalhado na Subação (R$)');
            $('#detPi').html('Detalhado em PI na Subação (R$)');
            $('#prefixotitulo').parent().css('display','block');
            $('#plititulo').parent().removeClass('col-lg-10');
            $('#plititulo').parent().addClass('col-lg-9');
            $('#tipoSubacao').val('c');
        }
    }

    function calculovalorPI() {
        var tabela = document.getElementById('orcamento');
        var tot = 0;

        $('.somar').each(function() {
            var valor = $(this).val();

            if ('' == valor) {
                valor = '0';
            }

            valor = replaceAll(valor, '.', '');
            valor = replaceAll(valor, ',', '.');
            tot += parseFloat(valor);
        });

        var c = tot.toString();
        if (c.indexOf('.') == -1) {
            document.getElementById('valortotalpi').value = tot.toFixed(2);
        } else {
            document.getElementById('valortotalpi').value = Arredonda(tot, 2);
        }
        document.getElementById('valortotalpi').onkeyup();
    }

    function Arredonda(valor, casas) {
        var novo = Math.round(valor * Math.pow(10, casas)) / Math.pow(10, casas);
        var c = novo.toString();
        if (c.indexOf('.') == -1) {
            alert(novo);
            return novo;
        } else {
            return novo.toFixed(casas);
        }
    }

    function verificaDisponivel(campo, ptrid, vlold) {
        var linha = document.getElementById('ptres_' + ptrid);
        valordisp = parseFloat(replaceAll(replaceAll($(linha.cells[6]).text(),'.',''),',','.'))
        valoratual = parseFloat(replaceAll(replaceAll(campo.value, ".", ""), ",", "."));
        valorDotacao = parseFloat(replaceAll(replaceAll($(linha.cells[2]).text(),'.',''),',','.'))

        /* Troca a referência caso seja sem Subação */
        if(valoratual > valorDotacao){
            alert('O valor não pode ser maior do que o valor da Dotação Atual do PTRES.');
            $(campo).val('0');
            calculovalorPI(); return false;
        }
    }

    function contar() {
        var l = document.formulario.plicodsubacao.value.length;
        if (l < 4) {
            alert("Sua subação só possui " + l + " caracteres. Favor inserir 4 caracteres.")
        }
    }

    atualizarPrevisaoPI();

</script>
<div id="divTeste"></div>
<div class="row">
    <div class="col-md-12">
        <div id="modal-ptres" class="modal fade">
            <div class="modal-dialog-ptres">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Seleção de PTRES</h4>
                    </div>
                    <div class="modal-body">
                        <p></p>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <div id="modal_responsaveis" class="modal fade">
            <div class="modal_dialog_responsaveis">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Seleção de Responsáveis</h4>
                    </div>
                    <div class="modal-body">
                        <p></p>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <div id="modal_localizacao" class="modal fade">
            <div class="modal_dialog_localizacao">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Seleção de Localizações Municipais</h4>
                    </div>
                    <div class="modal-body">
                        <p></p>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <div id="modal_localizacao_estadual" class="modal fade">
            <div class="modal_dialog_localizacao_estadual">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Seleção de Localizações Estaduais</h4>
                    </div>
                    <div class="modal-body">
                        <p></p>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <div id="modal_localizacao_exterior" class="modal fade">
            <div class="modal_dialog_localizacao_exterior">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Seleção de Localizações no Exterior</h4>
                    </div>
                    <div class="modal-body">
                        <p></p>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
