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

if ($_REQUEST['carregarComboUG'] == 'ok') {
    echo carregarComboUG($_REQUEST['unicod']);
    die();
}
if ($_REQUEST['carregarComboSubacaoFederais'] == 'ok') {
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

if (validaRequisicao('pliid')) { // -- Edição de PI
    // -- Carrega dados do PI e libera apenas o titulo e a descrição para edição
    $dadosPI = carregarPI($_REQUEST['pliid']);
    $enquadramento_pi = carregarEnquadramentoPI($_REQUEST['pliid']);
    //ver($dadosPI,d);
    $habilitado = 'N';
    $ehSolicitacao = false;
    $obrigatorio = 'S';

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
            trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acatitulo) AS descricao,
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
        GROUP BY ptr.ptrid, ptr.ptres,aca.prgcod, aca.acacod,aca.unicod,aca.loccod,aca.acatitulo,uni.unidsc, ptr.ptrdotacao, pemp.total
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

                    $db->monta_combo('unicod', $sql, $editavel, 'Selecione', 'carregarUG', null, null, null, 'N', 'unicod', null, (isset($unicod) ? $unicod : null), null, 'class="form-control chosen-select" style="width=100%;"', null, null);
                    ?>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="ungcod">Sub-Unidade:</label>
                    </div>
                    <div class="col-lg-10 carregaUG">
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
                        $db->monta_combo('ungcod', $ungdata, 'S', 'Selecione', 'carregarSubacao', null, null, null, 'N', 'ungcod', null, (isset($ungcod) ? $ungcod : null), null, 'class="form-control chosen-select" style="width=100%;"', null, null);
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
                        <b>Detalhamento Orçamentário</b>
                    </div>
                    <table cellpadding="5" border="0" width="98%" align="center" id="orcamento" style="border:#c9c9c9 1px solid" onmouseover="tabindexcampo();"  class="table table-hover table-bordered table-hover" >
                        <tr>
                            <td rowspan="2" nowrap><b>PTRES</b><input type="hidden" name="pliptres"></td>
                            <td rowspan="2" style="width:45%" nowrap><b>Funcional</b></td>
                            <td rowspan="2" nowrap><b>Dotação Atual PO (R$)</b></td>
                            <td style="display:none;" colspan="2" nowrap><b>
                                <span id="detSub">
                                <?php
                                if ($sbaid && $dadosPI['pliid']) {
                                    echo 'Detalhado na Subação (R$)';
                                }if (!$sbaid && $dadosPI['pliid']) {
                                    echo 'Detalhado em Subação (R$)';
                                }if (!$dadosPI['pliid']) {
                                    echo 'Detalhado na Subação (R$)';
                                } ?>
                                </span></b>
                            </td>
                            <td colspan="2" nowrap><b>
                                <span id="detPi">
                                <?php
                                if ($sbaid && $dadosPI['pliid']) {
                                    echo 'Detalhado em PI (R$)';
                                }if (!$sbaid && $dadosPI['pliid']) {
                                    echo 'Detalhado em PI (R$)';
                                }if (!$dadosPI['pliid']) {
                                    echo 'Detalhado em PI (R$)';
                                } ?>
                                </span></b>
                            </td>
                            <td colspan="2"><b>Empenho (R$)</b></td>
                            <td rowspan="2" align="center"><b>Valor Previsto (Anual)</b></td>
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
                                    <tr style="height:30px;"
                                        id="ptres_<?php echo $ptres['ptrid']; ?>">
                                        <td align="center"><?php echo $ptres['ptres']; ?></td>
                                        <td align="left"><?php echo $ptres['descricao']; ?></td>
                                        <td align="right"><?php echo $ptres['dotacaoatual']; ?></td>
                                        <td align="right"><?php echo $ptres['det_subacao']; ?></td>
                                        <td align="right"><?php echo $ptres['nao_det_subacao']; ?></td>
                                        <td align="right"><?php echo $ptres['det_pi']; ?></td>
                                        <td align="right"><?php echo $ptres['nao_det_pi']; ?></td>
                                        <td align="right"><?php echo $ptres['empenhado']; ?></td>
                                        <td align="right"><?php echo $ptres['nao_empenhado']; ?></td>
                                        <?php /* não dar espaço entre este input e o td */ ?>
                                        <td align="center">
                                            <input type="text" name="plivalored[<?php echo $ptres['ptrid']; ?>]"
                                                   id="plivalored[<?php echo $ptres['ptrid']; ?>]" value="<?php echo $ptres['pipvalor']; ?>"
                                                   onKeyUp="this.value = mascaraglobal('###.###.###.###,##', this.value);calculovalorPI();"
                                                   class="normal somar CampoEstilo normal form-control obrigatorio" onmouseover="MouseOver(this);" style="text-align:right;width:20ex"
                                                   onfocus="MouseClick(this); this.select();" onmouseout="MouseOut(this);"
                                                   data-ptres="<?php echo $ptres['ptres']; ?>"
                                                   onblur="verificaDisponivel(this, '<?php echo $ptres['ptrid']; ?>', '<?php echo $ptres['pipvalor']; ?>');
                                                           MouseBlur(this)"/>
                                        </td>
                                    </tr>
                                    <?php
                                    $cor++;
                                    $valortotalpi = $valortotalpi + $ptres['pipvalor_'];
                                endforeach;
                            }
                        }
                        ?>
                        <tr style="display: none; height:30px">
                            <td align="right" valign="top" colspan="9"><b>TOTAL:</b></td>
                            <td align="center" valign="top">
                                <input type="text" name="valortotalpi" id="valortotalpi"
                                       value="<?php echo number_format($valortotalpi, 2, ',', '.'); ?>" disabled
                                       onKeyUp="this.value = mascaraglobal('###.###.###.###,##', this.value);"
                                       class="disabled CampoEstilo normal form-control obrigatorio" onmouseover="MouseOver(this);"
                                       onfocus="MouseClick(this); this.select();" onmouseout="MouseOut(this);"
                                       onblur="MouseBlur(this);" style="text-align:right;width:20ex" />
                            </td>
                        </tr>
                        <tr>
                            <td align="right" colspan="10">
                                <input type="button" onclick="abrir_lista();" id="btn_selecionar_acaptres"
                                       value="Selecionar Funcional/PTRES" class="btn btn-info"/>
                            </td>
                        </tr>
                    </table>
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
                                                oppnome AS descricao
                                            FROM monitora.pi_objetivo_ppa
                                            WHERE
                                                oppstatus = 'A'
                                                AND prsano = '{$_SESSION['exercicio']}'
                                            ORDER BY
                                                oppnome
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
                                <div class="col-lg-10">
                                    <?php
                                        $sql = <<<DML
                                            SELECT
                                                mppid AS codigo,
                                                mppdesc AS descricao
                                            FROM monitora.pi_metas_ppa
                                            WHERE
                                                mppstatus = 'A'
                                                AND prsano = '{$_SESSION['exercicio']}'
                                            ORDER BY
                                                mppdesc
DML;
                                        $mppid = $dadosPI['mppid'];
                                        $db->monta_combo('mppid', $sql, 'S', 'Selecione', 'carregaMetaPnc', null, null, null, 'N', 'mppid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($mppid)? $mppid: null));
                                    ?>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-md-2 text-right">
                                    <label for="ippid" class="control-label">Iniciativa PPA: </label>
                                </div>
                                <div class="col-lg-10">
                                    <?php
                                        $sql = <<<DML
                                            SELECT
                                                ippid AS codigo,
                                                ippnome AS descricao
                                            FROM monitora.pi_iniciativa_ppa
                                            WHERE
                                                ippstatus = 'A'
                                                AND prsano = '{$_SESSION['exercicio']}'
                                            ORDER BY
                                                ippnome
DML;
                                        $ippid = $dadosPI['ippid'];
                                        $db->monta_combo('ippid', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'ippid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($ippid)? $ippid: null));
                                    ?>
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
                                                mpnid AS codigo,
                                                mpnnome AS descricao
                                            FROM monitora.pi_meta_pnc
                                            WHERE
                                                mpnstatus = 'A'
                                                AND prsano  = '{$_SESSION['exercicio']}'
                                            ORDER BY
                                                mpnnome
DML;
                                    $mpnid = $dadosPI['mpnid'];
                                    $db->monta_combo('mpnid', $sql, 'S', 'Selecione', 'carregaIndicadorPnc', null, null, null, 'N', 'mpnid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($mpnid)? $mpnid: null));
                                    ?>
                                </div>
                            </div>
                            <div class="form-group grupo_pnc">
                                <div class="col-md-2 text-right">
                                    <label for="ipnid" class="control-label">Indicador PNC: </label>
                                </div>
                                <div class="col-lg-10">
                                <?php
                                    $sql = <<<DML
                                        SELECT
                                            ipnid AS codigo,
                                            ipndesc AS descricao
                                        FROM monitora.pi_indicador_pnc
                                        WHERE
                                            ipnstatus = 'A'
                                            AND prsano = '{$_SESSION['exercicio']}'
                                        ORDER BY
                                            ipndesc
DML;
                                    $ipnid = $dadosPI['ipnid'];
                                    $db->monta_combo('ipnid', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'ipnid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($ipnid)? $ipnid: null));
                                ?>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset><legend>Números SNIIC </legend>
                            <div class="form-group">
                                <div class="col-lg-10">
                                    <input type="text" value="<?php
                                    if ($dadosPI['plititulo']) {
                                        echo $dadosPI['plititulo'];
                                    }
                                    ?>" class="CampoEstilo normal form-control obrigatorio" title="" id="plititulo"  maxlength="250" size="2" name="plititulo">
                                </div>
                                <div class="col-lg-2">
                                    <input type="button" onclick="" id="btn_selecionar_sniic" value="ADICIONAR" class="btn btn-info" style="width: 100%;"/>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-10">
                                <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                    <tr style="height: 30px;">
                                        <td style="text-align: left;">090421131325</td>
                                        <td style="text-align: center;">
                                            <span class="glyphicon glyphicon-trash"></span>
                                        </td>
                                    </tr>
                                    <tr style="height: 30px;">
                                        <td style="text-align: left;">293983982020</td>
                                        <td style="text-align: center;">
                                            <span class="glyphicon glyphicon-trash"></span>
                                        </td>
                                    </tr>
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
                    <div class="col-lg-10">
                    <?php
                    $sql = <<<DML
                        SELECT
                            neeid AS codigo,
                            needsc AS descricao
                        FROM monitora.pi_niveletapaensino
                        WHERE
                            neeano = '{$_SESSION['exercicio']}'
                            AND neestatus = 'A'
                        ORDER BY
                            descricao
DML;
                    $neeid = $dadosPI['neeid'];
                    $db->monta_combo('neeid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', null, null, null, 'N', 'neeid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($neeid) ? $neeid : null));
                    ?>
                    </div>
                </div>
                <div style="display:none;" class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="idCodificacao">Codificação da Unidade(livre):</label>
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
                                    <label for="pprid" class="control-label">Selecionar:</label>
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

                                        $mdeid = $dadosPI['pumid'];
                                        $db->monta_combo('pumid', $sql, 'S', 'Selecione', 'atualizarPrevisaoPI', null, null, null, 'N', 'pumid', null, '', null, 'class="form-control chosen-select" style="width=100%;"', null, (isset($pumid) ? $pumid: null));
                                    ?>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-md-2 text-right">
                                    <label for="mdeid" class="control-label">Quantidade:</label>
                                </div>
                                <div class="col-lg-10">
                                    <input type="text" value="" class="CampoEstilo normal form-control obrigatorio" title="Quantidade" id="quantidade" name="quantidade" maxlength="50" size="10" />
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
                        <fieldset><legend>Números De Convênio</legend>
                            <div class="form-group">
                                <div class="col-lg-10">
                                    <input type="text" value="" class="CampoEstilo normal form-control obrigatorio" title="" id="plititulo"  maxlength="250" size="2" name="plititulo">
                                </div>
                                <div class="col-lg-2">
                                    <input type="button" onclick="" id="btn_selecionar_sniic" value="ADICIONAR" class="btn btn-info" style="width: 100%;"/>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-10">
                                    <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">090421131325</td>
                                            <td style="text-align: center;">
                                                <span class="glyphicon glyphicon-trash"></span>
                                            </td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">293983982020</td>
                                            <td style="text-align: center;">
                                                <span class="glyphicon glyphicon-trash"></span>
                                            </td>
                                        </tr>
                                    </table>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">TED:</label>
                    <div class="col-lg-10">
                        <div class="make-switch switch-mini" data-on-label="S" data-off-label="N" data-off="danger">
                            <input type="checkbox" name="plicadsiafi" id="plicadsiafi"
                                   value="T" <?php echo $dadosPI['plicadsiafi'] == 't' ? 'checked' : ''; ?> />
                        </div>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Edital:</label>
                    <div class="col-lg-10">
                        <div class="make-switch switch-mini" data-on-label="S" data-off-label="N" data-off="danger">
                            <input type="checkbox" name="plicadsiafi" id="plicadsiafi"
                                   value="T" <?php echo $dadosPI['plicadsiafi'] == 't' ? 'checked' : ''; ?> />
                        </div>
                    </div>
                </div>
                <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset><legend>Localização do Projeto</legend>
                            <div class="form-group">
                                <div class="col-md-1 text-right">
                                    <label for="mdeid" class="control-label">Selecionar:</label>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-10">
                                    <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <tr>
                                            <td style="text-align: left;"><b>UF</b></td>
                                            <td style="text-align: left;"><b>Município</b></td>
                                            <td style="text-align: center;"></td>

                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">DF</td>
                                            <td style="text-align: left;">Brasília</td>
                                            <td style="text-align: center;"><span class="glyphicon glyphicon-trash"</td>

                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">MA</td>
                                            <td style="text-align: left;">São Luiz</td>
                                            <td style="text-align: center;"><span class="glyphicon glyphicon-trash"</td>
                                        </tr>
                                    </table>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset><legend>Responsáveis pelo Projeto</legend>
                            <div class="form-group">
                                <div class="col-lg-10">
                                    <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <tr>
                                            <td style="text-align: left;"><b>Nome</b></td>
                                            <td style="text-align: left;"><b>Telefone</b></td>
                                            <td style="text-align: left;"><b>E-mail</b></td>
                                            <td style="text-align: center;"><b>Ação</b></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Fulano da Silva</td>
                                            <td style="text-align: left;">+55 61 6489465116</td>
                                            <td style="text-align: left;">email@email.email</td>
                                            <td style="text-align: center;"><span class="glyphicon glyphicon-trash"</td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Adler Adriel</td>
                                            <td style="text-align: left;">+55 61 65156189641894</td>
                                            <td style="text-align: left;">email@email.email</td>
                                            <td style="text-align: center;"><span class="glyphicon glyphicon-trash"</td>
                                        </tr>
                                    </table>
                                </div>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <div class="panel panel-default">
                    <div class="panel-body">
                        <fieldset><legend>Custeio e Capital</legend>
                            <div class="form-group">
                                <div class="col-lg-10">
                                    <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <tr>
                                            <td style="text-align: left;"></td>
                                            <td colspan="3" style="text-align: center;"><b>Total(Custeio e Capital)</b></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Limite Autorizado na Sub-Unidade: </td>
                                            <td colspan="3" style="text-align: center;">R$25.769.250,00</td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Limite Disponível na Sub-Unidade:</td>
                                            <td colspan="3" style="text-align: center;">R$25.769.250,00</td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;"></td>
                                            <td style="text-align: left;"><b>Custeio </b></td>
                                            <td style="text-align: center;"><b>Capital</b></td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Saldo Autorizado na Funcional:</td>
                                            <td style="text-align: left;">R$1.500.00,00</td>
                                            <td style="text-align: center;">R$500.00,00</td>
                                        </tr>                                        <tr>
                                            <td style="text-align: left;">Saldo Disponível na Funcional:</td>
                                            <td style="text-align: left;">R$1.500.00,00</td>
                                            <td style="text-align: center;">R$500.00,00</td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: left;">Valor do Projeto:</td>
                                            <td style="text-align: left;"><input class="CampoEstilo normal form-control obrigatorio" type="text"> </td>
                                            <td style="text-align: center;"><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
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
                                <div class="col-lg-10">
                                    <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                        <tr>
                                            <td rowspan="2" style="text-align: center;"><b>Mês</b></td>
                                            <td style="text-align: left;"><b>Cronograma Físico</b></td>
                                            <td colspan="2" style="text-align: center;"><b>Cronograma Orçamentário</b></td>
                                            <td colspan="2" style="text-align: center;"><b>Cronograma Financeiro</b></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Previsto</td>
                                            <td style="text-align: left;">Previsto Custeio</td>
                                            <td style="text-align: left;">Previsto Capital</td>
                                            <td style="text-align: left;">Previsto Custeio</td>
                                            <td style="text-align: left;">Previsto Capital</td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Janeiro</td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Fevereiro</td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Março</td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Abril</td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Maio</td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Junho</td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Julho</td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Agosto</td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Setembro</td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Outubro</td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Novembro</td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                        </tr>
                                        <tr style="height: 30px;">
                                            <td style="text-align: left;">Dezembro</td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                            <td><input class="CampoEstilo normal form-control obrigatorio" type="text"></td>
                                        </tr>
                                        <tr style="height: 30px; ">
                                            <td style="text-align: left;">Total:</td>
                                            <td>R$</td>
                                            <td>R$</td>
                                            <td>R$</td>
                                            <td>R$</td>
                                            <td>R$</td>
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
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <?php if('-' != $tipoTransacao): ?>
                            <?php if ($_POST['pliid'] && $plisituacao == 'E'): ?>
                                <input type="button" value="Enviar para Homologação" onclick="vincular('R', '<?php echo $_POST['pliid']; ?>')" style="cursor:pointer" class='btn-info' />
                            <?php endif; ?>
                            <input type="button" class="btn btn-primary" name="btg" onclick="submeter('<?php echo $plicod; ?>');" value="Salvar" <?php echo $disabled; ?> />
                        <?php endif; ?>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
</div>
<script type="text/javascript">
    $(document).ready(function() {
        
        // Evento ao mudar opção de enquadramento
        $('#eqdid').change(function(){
            mudarFormularioFinalistico($(this).val());
        });
        
        var tipoTransacao = $('tipotransacao').value;
        if ('-' == tipoTransacao) {
            // -- Desabilita o restante do formulário
            $('plititulo').disable();
            $('plidsc').disable();
            $('btn_selecionar_acaptres').disable();
        }
    });
    
    /**
     * Controla a exibição do formulario de acordo com o enquadramento selecionado.
     * 
     * @param integer codigo Código selecionado pelo usuário.
     * @returns VOID
     */
    function mudarFormularioFinalistico(codigo){
        // Se o código for diferente 354 - Finalistico, o sistema oculta as opções PNC.
        if(codigo != 354){
            // Oculta as opções Meta PNC e Indicador PNC.
            $('.grupo_pnc').hide();
            // Apaga as opções selecionadas.
            $('#mpnid').val('').trigger("chosen:updated");
            $('#ipnid').val('').trigger("chosen:updated");
        } else {
            // Mostra as opções Meta PNC e Indicador PNC.
            $('.grupo_pnc').show();
        }
    }

    /**
     * Carrega novo conteúdo para o select de Subações via requisição ajax.
     */
    function carregarUG(unicod) {
        $.post(window.location + '&carregarComboUG=ok&unicod=' + unicod, function(response) {
            $('#ungcod').remove();
            $('.carregaUG').html(response);
            $(".chosen-select").chosen();
            carregarSubacao();
        });
    }
    function carregarSubacao(unicod) {
        var params = '&carregarComboSubacaoFederais=ok&unicod=' + unicod;
        $.post(window.location + params, function(response) {
            $('#sbaid').remove();
            $('.cSubacao').html(response);
            $(".chosen-select").chosen();
        });
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
        var codsubacao = $('#subacao').text();
//        if ('' === codsubacao) {
//            alert('O código da subação não pode ser deixado em branco.');
//            return false;
//        }

        var pi = document.getElementById("enquadramento").innerHTML
            + codsubacao
            + document.getElementById("nivel").innerHTML
            + document.getElementById("apropriacao").innerHTML
            + document.getElementById("codificacao").innerHTML
            + document.getElementById("modalidade").innerHTML;
        $("#plicod").val(pi);

        var validado = true;
        var l = $('#plicodsubacao').val();
        var sub = l.length;
        $('plititulo').value = $('prefixotitulo').textContent + $('plititulo').value;
        validarFormulario(['plititulo', 'plidsc', 'unicod', 'ungcod','eqdid', 'oppid', 'mppid', 'ippid', 'mpnid','ipnid',  'neeid',  'capid','pprid','pumid','mdeid'], 'formulario', 'validar('+ pliid +')');
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
        $.post('planacomorc.php?modulo=principal/unidade/popupptres&acao=A&obrigatorio=n&sbaid=' + $("#sbaid").val(), function(response) {
            $('#modal-ptres .modal-body p').html(response);
            $('#modal-ptres .modal-title').html('Seleção de PTRES');
            $('#modal-ptres .btn-primary').remove();
            $('#modal-ptres .btn-default').html('Fechar');
            $('.modal-dialog-ptres').show();
            $('#modal-ptres').modal();
            $('#modal-ptres .chosen-select').chosen();
            $('#modal-ptres .chosen-container').css('width', '100%');
        });
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
        //valordisp = parseFloat(replaceAll(replaceAll(linha.cells[6].innerHTML, ".", ""), ",", "."));
        //valordisp = parseFloat($(linha.cells[6]).text().replace('.','').replace(',','.'));
        valordisp = parseFloat(replaceAll(replaceAll($(linha.cells[6]).text(),'.',''),',','.'))
        valoratual = parseFloat(replaceAll(replaceAll(campo.value, ".", ""), ",", "."));
        //valorDotacao = parseFloat(replaceAll(replaceAll(linha.cells[2].innerHTML, ".", ""), ",", "."));
        valorDotacao = parseFloat(replaceAll(replaceAll($(linha.cells[2]).text(),'.',''),',','.'))
        mensagem = 'Para PI com Subação o valor não pode ser maior do que o valor não Detalhado em PI.';
        /* Troca a referência caso seja sem Subação */
        if (jQuery("input[name=semsubacao]:checked").val() == 's' || $('#tipoSubacao').val() == 's') {
            //valorNES = parseFloat(replaceAll(replaceAll(linha.cells[4].innerHTML, ".", ""), ",", "."));
            //valorNES = parseFloat($(linha.cells[4]).text().replace('.','').replace(',','.'));
            valorNES = parseFloat(replaceAll(replaceAll($(linha.cells[4]).text(),'.',''),',','.'))
            //valorNEP = parseFloat(replaceAll(replaceAll(linha.cells[6].innerHTML, ".", ""), ",", "."));
            //valorNEP = parseFloat($(linha.cells[6]).text().replace('.','').replace(',','.'));
            valorNEP = parseFloat(replaceAll(replaceAll($(linha.cells[6]).text(),'.',''),',','.'));
            if (valorNES > valorNEP) {
                valordisp = valorNEP;
                mensagem = 'Para PI sem Subação o valor não pode ser maior do que o valor Não Detalhado em PI. ';
            }else {
                valordisp = valorNES;
                mensagem = 'Para PI sem Subação o valor não pode ser maior do que o valor Não Detalhado em Subação. ';
            }

            //alert(mensagem);
        }
        if (valoratual > (valordisp + parseFloat(replaceAll(replaceAll(vlold, ".", ""), ",", ".")))) {
            alert(mensagem);
            campo.value = vlold;
            calculovalorPI(); return false;
        }
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

    function apresentaLinhasSelecionadas(tabelaOrigem, apartirDe) {
        var val;
        for (i = apartirDe; i < tabelaOrigem.find('tr').length; i++) {
            val = tabelaOrigem.find('tr')[i].id.split("_");
            // alert(val[1]);
            if (document.getElementById(val[1])) {
                campo = $('tr#' + val[1] + ' td:nth-child(2)').find('span');
                campo.attr('class', 'glyphicon glyphicon-trash');
                campo.attr('title', 'Remover item');
                campo.css('color', 'gray');
            }
        }
    }

</script>
<div id="divTeste"></div>
<div class="row">
    <div class="col-md-12">
        <div id="modal-ptres" class="modal fade">
            <div class="modal-dialog-ptres">
                <div class="modal-content">
                    <div class="modal-header">
                        <button class="close" aria-hidden="true" data-dismiss="modal" type="button">×</button>
                        <h4 class="modal-title">Aviso!</h4>
                    </div>
                    <div class="modal-body">
                        <p></p>
                    </div>
                    <div class="modal-footer">
                        <button class="btn btn-default" data-dismiss="modal" type="button">Não</button>
                        <button class="btn btn-primary" type="button">Sim</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>