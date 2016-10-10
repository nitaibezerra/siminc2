<?php
include APPRAIZ . "includes/cabecalho.inc";
require_once APPRAIZ . "includes/classes/Modelo.class.inc";
require_once APPRAIZ . "planacomorc/classes/Pi_Subacao.class.inc";
require_once APPRAIZ . "planacomorc/classes/Pi_SubacaoDotacao.class.inc";
require_once APPRAIZ . "planacomorc/classes/Pi_SubacaoHistorico.class.inc";
require_once APPRAIZ . "www/planacomorc/_funcoespi.php";
require_once APPRAIZ . "www/planacomorc/planotrabalhoUN/_funcoes.php";
require_once APPRAIZ . "www/planacomorc/planotrabalhoUN/_constantes.php";

if ($_REQUEST['requisicao'] == 'vincular') {

    extract($_POST);

    $retorno = false;
    $pi_SubacaoHistorico = new Pi_SubacaoHistorico();
    $pi_SubacaoHistorico->sbaid = $sbaid;
    $pi_SubacaoHistorico->usucpf = $_SESSION['usucpf'];
    $pi_SubacaoHistorico->sahsituacao = $situacao;
    $pi_SubacaoHistorico->salvar();

    $pi_Subacao = new Pi_Subacao($sbaid);
    $pi_Subacao->sbasituacao = $situacao;
    $pi_Subacao->salvar();

    if ($pi_SubacaoHistorico->commit()) {
        $retorno = true;
    }
    unset($pi_Subacao);
    unset($pi_SubacaoHistorico);
    echo $retorno;
    die;
}

if ($_POST['requisicao'] == 'existeCod') {
    extract($_POST);

    if ($sbaid) {
        $sql = <<<DML
            SELECT 
                sub.sbaid 
            FROM monitora.pi_subacao sub
            LEFT JOIN monitora.pi_subacaounidade sub2 ON sub.sbaid = sub2.sbaid 
            WHERE sub.sbaid != '{$sbaid}' 
                AND sub.sbacod = '{$sbacod}' 
                AND sub2.unicod = '{$_SESSION['monitora_var']['unicod']}'
DML;
        $existe_sbaid = $db->pegaUm($sql);
        $sql = <<<DML
            SELECT sbaplanotrabalho FROM monitora.pi_subacao WHERE sbaid = '{$sbaid}' AND sbastatus = 'A'
DML;
        $existe_sbaplanotrabalho = $db->pegaUm($sql);
        $existe_sbaplanotrabalho = ($existe_sbaplanotrabalho == 't') ? true : false;
    } else {
        $sql = <<<DML
            SELECT sub.sbaid FROM monitora.pi_subacao sub
            LEFT JOIN monitora.pi_subacaounidade sub2 ON sub.sbaid = sub2.sbaid 
            WHERE sub.sbacod = '{$sbacod}' 
                AND sub2.unicod = '{$_SESSION['monitora_var']['unicod']}' 
                AND  sbastatus = 'A'
DML;
        $existe_sbaid = $db->pegaUm($sql);
    }
    if ($existe_sbaid) {
        echo 1;
        die;
    }
    echo 0;
    die;
}

if ($_POST['acaoForm']) {
    extract($_POST);
    #ver($_POST,d);
    if ($sbaid) {
        $sql = <<<DML
            SELECT sbaplanotrabalho FROM monitora.pi_subacao WHERE sbaid = '{$sbaid}' AND sbastatus = 'A'
DML;
        $existe_sbaplanotrabalho = $db->pegaUm($sql);
        $existe_sbaplanotrabalho = ($existe_sbaplanotrabalho == 't') ? true : false;
    }

    $msg = "";
    
    if ($sbaplanotrabalho && !$existe_sbaplanotrabalho) {
        // obtém dados da atividade vinculada à ação
        $atividade = retornaTarefaUnidade($_SESSION['monitora_var']['unicod'], $_SESSION['exercicio']);

        $subacao = array("subatv" => true,
            "sbaid" => $sbaid,
            "sbatitulo" => $sbatitulo,
            "sbacod" => $sbacod,
            "sbaobras" => $sbaobras,
            "sbadsc" => $sbadsc);

        if (!$sbaid = atividade_inserir($atividade['atiid'], $titulo, $subacao, false)) {
            die("<script>alert('Ocorreu um erro, por favor tente mais tarde.');window.location='monitora.php?modulo=principal/planotrabalhoUN/cadsubacaoUN&acao=A';</script>");
        }
        // atualiza projeto da sessão
        $sql = sprintf("select _atiprojeto from pde.atividade where atiid = %d", $atividade['atiid']);
        $_SESSION['projeto'] = (integer) $db->pegaUm($sql);
        // atualiza dados da árvore
        atividade_calcular_dados($atividade['atiid']);
    } elseif (!$sbaplanotrabalho && $existe_sbaplanotrabalho) {
        $atividade = retornaTarefaUnidade($_SESSION['monitora_var']['unicod'], $_SESSION['exercicio']);
        $sql = <<<DML
            SELECT a.atiid FROM pde.atividade a
            INNER JOIN monitora.pi_subacaoatividade sa ON a.atiid = sa.atiid
            WHERE a.atistatus = 'A' 
                AND a.unicod = '{$_SESSION['monitora_var']['unicod']}' 
                AND atiidpai = {$atividade['atiid']} 
                AND sa.sbaid = {$sbaid}
DML;
        $atiid = $db->pegaUm($sql);
        $sql = <<<DML
            SELECT COUNT(1) FROM pde.atividade WHERE atiidpai = {$atiid} AND atistatus = 'A'
DML;
        $existeAtividadeFilha = $db->pegaUm($sql);
        if ($existeAtividadeFilha) {
            $msg = 'O campo Migrar Plano de Trabalho não pode ser alterado pois existem Atividades relacionadas.\n\n';
        }
        $sql = <<<DML
            SELECT count(1) FROM monitora.pi_planointerno WHERE sbaid = {$sbaid} AND plistatus = 'A'
DML;
        $existePiVinculado = $db->pegaUm($sql);
        if ($existePiVinculado) {
            $msg .= 'O campo Migrar Plano de Trabalho não pode ser alterado pois existem Planos Interno relacionadas.';
        }

        if (!$msg) {
            $sql = <<<DML
                UPDATE pde.atividade SET atistatus='I' WHERE atiid='{$atiid}'
DML;
            $db->executar($sql);

            $obPi_Subacao = new Pi_Subacao($sbaid);
            $obPi_Subacao->deletaSubacaoAtividade($sbaid);
            $obPi_Subacao->sbadsc = $sbadsc;
            $obPi_Subacao->sbatitulo = $sbatitulo;
            $obPi_Subacao->sbacod = $sbacod;
            $obPi_Subacao->sbaobras = $sbaobras == true ? 't' : 'f';
            $obPi_Subacao->sbaplanotrabalho = 'false';
            $obPi_Subacao->usucpf = $_SESSION['usucpf'];
            $obPi_Subacao->sbaobras = $sbaobras;
            $obPi_Subacao->sbasigla = $sbasigla;
            $obPi_Subacao->salvar();
        } else {

            $obPi_Subacao = new Pi_Subacao($sbaid);
            $obPi_Subacao->sbadsc = $sbadsc;
            $obPi_Subacao->sbatitulo = $sbatitulo;
            $obPi_Subacao->sbacod = $sbacod;
            $obPi_Subacao->sbaobras = $sbaobras == true ? 't' : 'f';
            $obPi_Subacao->usucpf = $_SESSION['usucpf'];
            $obPi_Subacao->sbaobras = $sbaobras;
            $obPi_Subacao->sbasigla = $sbasigla;
            $obPi_Subacao->sbaplanotrabalho = 'false';
            $obPi_Subacao->salvar();
        }
    } else {

        if (!$_REQUEST['unicod']) {
            $unicod = $_REQUEST['unicod_disable'];
        } else {
            $unicod = $_REQUEST['unicod'];
        }

        if (!$sbaid) {
            $sql = <<<DML
                SELECT pis.sbaid FROM monitora.pi_subacao pis 
                INNER JOIN monitora.pi_subacaounidade psu ON pis.sbaid = psu.sbaid
                WHERE sbacod = '{$sbacod}' 
                    AND pis.sbaano = '{$_SESSION['exercicio']}' AND unicod = '{$unicod}'
DML;
            $sbaidV = $db->PegaUm($sql);
            if (!$sbaidV) {
                $obPi_Subacao = new Pi_Subacao($sbaid);
                $obPi_Subacao->sbadsc = $sbadsc;
                $obPi_Subacao->sbatitulo = $sbatitulo;
                $obPi_Subacao->sbacod = $sbacod;
                $obPi_Subacao->sbaobras = $sbaobras == true ? 't' : 'f';
                $obPi_Subacao->sbaplanotrabalho = $sbaplanotrabalho;
                $obPi_Subacao->usucpf = $_SESSION['usucpf'];
                $obPi_Subacao->sbaobras = $sbaobras;
                $obPi_Subacao->sbasigla = $sbasigla;
                $obPi_Subacao->sbaano = $sbaano;
                $obPi_Subacao->salvar();
                $sbaid = $obPi_Subacao->sbaid;
            } else {
                echo '<script>alert(\'Subação: ' . $sbacod . ' já cadastrado para esta unidade.\');location.href= \'planacomorc.php?modulo=principal/unidade/cadastro_subacoes&acao=A\'</script>';
                die();
            }
        } else {
            // ver($_POST,d);
            global $db;
            if ($_REQUEST['sbaobras'] == 'true') {
                $sbaobras = 't';
            } else {
                $sbaobras = 'f';
            }
            $sqlUpdate = <<<DML
                UPDATE monitora.pi_subacao
                    SET sbadsc = '{$_REQUEST['sbadsc']}', sbasigla = '{$_REQUEST['sbasigla']}', sbatitulo = '{$_REQUEST['sbatitulo']}', 
                        sbaobras = '$sbaobras' 
                WHERE sbaid = {$_REQUEST['sbaid']}
DML;
            $db->executar($sqlUpdate);
            $db->commit();
        }
    }

    if ($sbaid) {
        if ($sbaplanotrabalho) {
            $obPi_Subacao = new Pi_Subacao($sbaid);
            $obPi_Subacao->sbaplanotrabalho = $sbaplanotrabalho;
            $obPi_Subacao->salvar();
        }
        $obPi_Subacao = new Pi_Subacao($sbaid);
        $obPi_Subacao->deletaSubacaoDotacao($sbaid);
    }

    $obPi_SubacaoDotacao = new Pi_SubacaoDotacao();
    $obPi_SubacaoDotacao->sbaid = $sbaid;
    $obPi_SubacaoDotacao->ptrid = $acaid;
    $obPi_SubacaoDotacao->sadvalor = $plivalor;

    $obPi_SubacaoDotacao->salvar($sbaid, $acaid, $plivalor);

    if ($sbaid) {
        $sql = <<<DML
            DELETE FROM monitora.pi_subacaounidade WHERE sbaid = {$sbaid}
DML;
        $db->executar($sql);
    }
    if (!$_REQUEST['unicod']) {
        $unicod = $_REQUEST['unicod_disable'];
    } else {
        $unicod = $_REQUEST['unicod'];
    }
    
    $sql = <<<DML
        INSERT INTO monitora.pi_subacaounidade(sbaid, unicod, unitpocod) VALUES ('{$sbaid}','{$unicod}', 'U');
DML;
    $db->executar($sql);
    $db->commit();

    if ($msg) {
        echo "<script>alert('$msg');</script>";
    }

    $_REQUEST['acao'] = 'A';
    $db->sucesso("principal/unidade/listasubacoes");
    unset($obPi_Subacao);
    unset($obPi_SubacaoDotacao);
    die;
}
if ($_GET['sbaid']) {
    $sql = <<<DML
        SELECT sbaid FROM monitora.pi_subacao WHERE sbaid = {$_GET['sbaid']} AND sbastatus = 'A'
DML;
    $subacao = $db->pegaUm($sql);

    if (!$subacao) {
        echo "
            <script>
                alert('A Subação informada não existe!');
                history.back(-1);
            </script>";
        die;
    }
}

if ($_REQUEST['sbaidExcluir']) {
    $sql = <<<DML
        SELECT COUNT(1) FROM monitora.pi_planointerno WHERE sbaid = {$_REQUEST['sbaidExcluir']} AND plistatus = 'A'
DML;
    $existePiVinculado = $db->pegaUm($sql);
    
    if ($existePiVinculado > 0) {
        echo "
            <script>
                alert('Subação não pode ser excluida, pois existe pi vinculado para essa subação.');
                history.back(-1);
            </script>";
        die;
    } else {
        //global $db;
        /* Apagando de verdade */
        $sqlP = <<<DML
            DELETE FROM monitora.pi_subacaohistorico WHERE sbaid = {$_REQUEST['sbaidExcluir']}
DML;
        $db->executar($sqlP);

        $sql1 = <<<DML
            DELETE FROM monitora.pi_subacaodotacao WHERE sbaid = {$_REQUEST['sbaidExcluir']}
DML;
        $db->executar($sql1);

        $sql2 = <<<DML
            DELETE FROM monitora.pi_subacaounidade WHERE sbaid = {$_REQUEST['sbaidExcluir']}
DML;
        $db->executar($sql2);

        $sql3 = <<<DML
            DELETE FROM monitora.pi_subacao WHERE sbaid = {$_REQUEST['sbaidExcluir']}
DML;
        $db->executar($sql3);
        $db->commit();
        unset($_REQUEST['sbaidExcluir']);
        //unset($obPi_Subacao);
        $_REQUEST['acao'] = 'A';
        $db->sucesso("principal/unidade/listasubacoes");
    }
    die;
}

$obPi_Subacao = new Pi_Subacao($_GET['sbaid']);

if ($obPi_Subacao->sbaid) {
    $sql_lista = <<<DML
        SELECT
            dtl.ptrid,
            dtl.ptres,
            trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) AS
            descricao,
            uni.unicod,
            uni.unidsc,
            SUM(dtl.ptrdotacao)                               AS dotacaoinicial,
            COALESCE(CAST(SUM(dt.valor) AS VARCHAR),'0.00')    AS det_subacao,
            COALESCE(CAST(SUM(dt2.valorpi) AS VARCHAR),'0.00') AS det_pi,
            SUM(pte.total)                                     AS empenhado,
            SUM(sad.sadvalor)                                  AS valorcadastrado
        FROM monitora.acao aca
        INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
        INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
        INNER JOIN monitora.pi_subacaodotacao sad ON dtl.ptrid = sad.ptrid
        LEFT JOIN (
            SELECT
                ptrid,
                SUM( sadvalor ) AS valor
            FROM monitora.pi_subacaodotacao
            GROUP BY ptrid ) dt ON dtl.ptrid = dt.ptrid
        LEFT JOIN (
            SELECT
                sbaid,
                ptrid,
                SUM(dtl.valorpi) AS valorpi
            FROM monitora.v_pi_detalhepiptres dtl
            GROUP BY sbaid, dtl.ptrid ) dt2 ON dtl.ptrid = dt2.ptrid AND dt2.sbaid = sad.sbaid 
        LEFT JOIN (
            SELECT ex.unicod, ex.ptres, ex.exercicio, sum(ex.vlrempenhado) AS total
            FROM spo.siopexecucao ex
            WHERE ex.unicod  NOT IN('26101','26291', '26290', '26298', '26443', '74902', '73107')    
                AND ex.exercicio = '{$_SESSION['exercicio']}'
            GROUP BY ex.unicod, ex.ptres, ex.exercicio
            ) pte ON (pte.ptres = dtl.ptres AND pte.exercicio = '{$_SESSION['exercicio']}' AND pte.unicod = dtl.unicod)
        WHERE aca.prgano = '{$_SESSION['exercicio']}'
            AND aca.acasnrap = false
            AND sad.sbaid = {$obPi_Subacao->sbaid}
        GROUP BY
            dtl.ptrid,
            dtl.ptres,
            uni.unicod,
            trim(aca.prgcod||'.'||aca.acacod||'.'||aca.unicod||'.'||aca.loccod||' - '||aca.acadsc) ,
            uni.unidsc
DML;

    $ptresacao = $db->carregar($sql_lista);
}#ver($sql_lista);
$ptresacao = ($ptresacao) ? $ptresacao : array();
#ver($ptresacao,d);
$perfis = pegaPerfilGeral();

if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
    $sqlUO = <<<DML
        EXISTS (
            SELECT 1
            FROM planacomorc.usuarioresponsabilidade rpu
            WHERE rpu.usucpf = '%s'
                AND rpu.pflcod = %d
                AND rpu.rpustatus = 'A'
                AND rpu.unicod  = uni.unicod)
DML;
    $where[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
    $whereUO = " AND {$whereUO}";
}
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/funcoesspo.js"></script>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Instituições Federais</li>
        <li >
            <a href="planacomorc.php?modulo=principal/unidade/listasubacoes&acao=A">Lista de Subações</a>
        </li>
        <li><?= $obPi_Subacao->sbaid ? 'Alterar Subações' : 'Cadastrar Subações' ?></li>
    </ol>
</div>
<div class="col-lg-12">
    <div class="well">
        <fieldset>
            <form id="formulario" name="formulario" method="post" action="" class="form-horizontal">
                <input type="hidden" name="acaoForm" id="acaoForm" value="1" />
                <input type="hidden" name="sbaid" id="sbaid" value="<?php echo $obPi_Subacao->sbaid; ?>" />
                <input type="hidden" name="sbaano" id="sbaano" value="<?php echo $_SESSION['exercicio']; ?>" />
                
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="sbatitulo">Título:</label>
                    </div>
                    <div class="col-lg-10">
                        <?php
                        $sbatitulo = $obPi_Subacao->sbatitulo;
                        inputTexto('sbatitulo', $sbatitulo, 'sbatitulo', 255, false);
                        ?>                       
                    </div>
                </div>
                <div class="form-group">
                    <label for="sbasigla" class="col-lg-2 control-label">Título Abreviado / Sigla:</label>
                    <div class="col-lg-10">
                        <?php
                        $sbasigla = $obPi_Subacao->sbasigla;
                        inputTexto('sbasigla',$sbasigla,'sbasigla',25,false);
                        ?>                       
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="sbacod">Código:</label>
                    </div>
                    <div class="col-lg-10">
                        <?php
                        $sbacod = $obPi_Subacao->sbacod;
                        $bloq = $sbacod ? 'readonly="readonly"' : '';
                        inputTexto('sbacod',$sbacod,'sbacod',4,false,array('evtkeyup' => 'this.value = this.value.toUpperCase();','complemento' => $bloq));
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="unicod">Unidade Orçamentária:</label>
                    </div>
                    <div class="col-lg-10">
                        <?php
                        if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)) {
                            $whereUO = "";
                        }
                        $obrigatorias = UNIDADES_OBRIGATORIAS;

                        $sql = "
                            SELECT
                                uni.unicod AS codigo,
                                uni.unicod || ' - ' || unidsc AS descricao
                            FROM public.unidade uni
                            WHERE (uni.orgcod = '". CODIGO_ORGAO_SISTEMA. "' OR  uni.unicod IN('74902', '73107'))
                                AND uni.unistatus = 'A'
                                {$whereUO}
                                AND uni.unicod not in ($obrigatorias)
                            ORDER BY 1
";
                        if ($_REQUEST['sbaid']) {
                            $sqlUni = <<<DML
                                SELECT unicod FROM monitora.pi_subacao pis inner join monitora.pi_subacaounidade psu on pis.sbaid = psu.sbaid where psu.sbaid = {$_REQUEST['sbaid']}
DML;
                            $unicod = $db->pegaUm($sqlUni);

                            if ($unicod) {
                                $bloqueado = 'N';
                            }
                        }

                        $db->monta_combo('unicod', $sql, $bloqueado, 'Selecione', '', null, null, null, 'N', 'unicod', null, (isset($unicod) ? $unicod : null), null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-md-2 text-right">
                        <label class="control-label" for="sbadsc">Descrição / Funcionalidade:</label>
                    </div>
                    <div class="col-lg-10">
                        <?php
                        $sbadsc = $obPi_Subacao->sbadsc;
                        inputTextArea('sbadsc', $sbadsc, 'sbadsc', 500, array('rows' => 5));
                        ?>
                    </div>
                </div>
                <?php
                $sbaobras = $obPi_Subacao->sbaobras;
                ?>
                <div class="form-group">
                    <label for="sbaobras" class="col-lg-2 control-label">Permitir Obras:</label>
                    <div class="col-lg-10">                       
                        <section class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default <?= ($sbaobras == "t") ? 'active' : '' ?>">
                                <input type="radio" name="sbaobras" value="true"  onchange="" <?= ($sbaobras == "t") ? 'checked="checked"' : '' ?>>
                                <span class="glyphicon glyphicon-thumbs-up" style="color:green"></span>
                                Sim
                            </label>
                            <label class="btn btn-default <?= ($sbaobras != "t") ? 'active' : '' ?>">
                                <input type="radio" name="sbaobras" value="false"  onchange="" <?= ($sbaobras != "t") ? 'checked="checked"' : '' ?>>
                                <span class="glyphicon glyphicon-thumbs-down" style="color:red"></span>
                                Não
                            </label>
                        </section>
                    </div>                        
                </div>
                <div class="panel panel-default">
                    <div class="panel-heading"><strong class="">Ações</strong></div>
                    <table cellpadding="5" class="table table-bordered table-hover" border="0" id="orcamento">
                        <tr>
                            <td colspan="10" align="center">
                                <b>Detalhamento Orçamentário</b>&nbsp;
                            </td>
                        </tr>
                        <tr style="text-align:center">
                            <td rowspan="2" nowrap style="font-weight:bold">PTRES</td>
                            <td rowspan="2" style="font-weight:bold" nowrap>Ação</b></td>
                            <td rowspan="2" style="font-weight:bold" nowrap>Dotação (R$)</td>
                            <td colspan="2" style="font-weight:bold" nowrap>Subações (R$)</td>
                            <td colspan="2" style="font-weight:bold" nowrap>PIs (R$)</td>
                            <td colspan="2" style="font-weight:bold" nowrap>Empenho (R$)</td>
                            <td rowspan="2" style="width:100px;font-weight:bold" nowrap>Valor previsto (R$)</td>
                        </tr>
                        <tr style="text-align:center">
                            <td style="font-weight:bold" nowrap>Detalhado</td>
                            <td style="font-weight:bold" nowrap>Não&nbsp;Detalhado</td>
                            <td style="font-weight:bold" nowrap>Detalhado</td>
                            <td style="font-weight:bold" nowrap>Não&nbsp;Detalhado</td>
                            <td style="font-weight:bold" nowrap>Empenhado</td>
                            <td style="font-weight:bold" nowrap>Não&nbsp;Empenhado</td>
                        </tr>
                        <?php
                        if ($ptresacao[0]) {
                            $valortotal = $cor = 0;
                            foreach ($ptresacao as $acpl):                                    
                                $detSubacao = mascaraMoeda($acpl['det_subacao'],false);
                                $naoDetSubacao = mascaraMoeda($acpl['dotacaoinicial'] - $acpl['det_subacao'],false);
                                $detPI = mascaraMoeda($acpl['det_pi'],false);
                                $naoDetEmPI = mascaraMoeda(($acpl['dotacaoinicial'] - $acpl['det_pi']),false);
                                $empenhado = mascaraMoeda($acpl['empenhado'],false);
                                $naoEmpenhado = mascaraMoeda($acpl['dotacaoinicial'] - $acpl['empenhado'],false);
                                $valCadastrado = mascaraMoeda($acpl['valorcadastrado'], false);
                                $dotacaoInicial = mascaraMoeda($acpl['dotacaoinicial'],false);
                                /* não dar espaço entre este input e o td */
                                echo <<<HTML
                        <tr id="ptres_{$acpl['ptrid']}">
                            <td align="center">{$acpl['ptres']}</td>
                            <input type='hidden' value='{$acpl['ptrid']}' name='acaid[{$acpl['ptres']}]'/>
                            <td align="left">{$acpl['descricao']}</td>
                            <td align="right">{$dotacaoInicial}</td>
                            <td align="right">{$detSubacao}</td>
                            <td align="right">{$naoDetSubacao}</td>
                            <td align="right">{$detPI}</td>
                            <td align="right">{$naoDetEmPI}</td>
                            <td align="right">{$empenhado}</td>
                            <td align="right">{$naoEmpenhado}</td>                                
                            <td align="center">
                                <input type="text" name="plivalor[{$acpl['ptres']}][{$acpl['ptrid']}]" class="normal somar CampoEstilo normal form-control obrigatorio valorDetalhadoPTRES"
                                    value="{$valCadastrado}" onKeyUp="this.value = mascaraglobal('###.###.###.###,##', this.value);
                                    calculovalorPI()" onmouseover="MouseOver(this);" onfocus="MouseClick(this);
                                    this.select()" onmouseout="MouseOut(this);" style="text-align:right;width:20ex"
                                    data-ptres="{$ptres['ptres']}" onblur="this.value = mascaraglobal('###.###.###.###,##', this.value);
                                    verificaDisponivel(this, '{$acpl['ptrid']}', '{$valCadastrado}');" />
                            </td>
                        </tr>
HTML;
                                $cor++;
                                $valortotal = $valortotal + $acpl['valorcadastrado'];
                            endforeach;
                        }
                        ?>
                        <tr>
                            <td align="right" valign="top" colspan="9"><b>TOTAL :</b></td>
                            <td align="center" valign="top">
                                <input type="text" name="valortotal" id="valortotal" value="<?php echo number_format($valortotal, 2, ',', '.'); ?>"
                                    onKeyUp="this.value = mascaraglobal('###.###.###.###,##', this.value);"
                                    disabled class="disabled CampoEstilo normal form-control obrigatorio"  onmouseover="MouseOver(this);"
                                    onfocus="MouseClick(this); this.select()"
                                    onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align:right;width:20ex" />
                            </td>
                        </tr>
                        <tr>
                            <td align="right" colspan="10">
                                <input type="button" onclick="abrir_lista();" id="btn_selecionar_acaptres" class="btn btn-info" value="Selecionar Ação/PTRES">
                            </td>
                        </tr>
                    </table>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <input type="button" class="botao btn btn-primary" value="Salvar" onclick="enviaForm();" />
                        <input type="button" class="botao btn btn-success" name="btn" value="Novo" onclick="window.location = '?modulo=principal/unidade/cadastro_subacoes&acao=A';">
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
</div>
<?php
if (boPerfilSubacao()) {
    $btExluir = "<a style=\"margin: 0 -5px 0 5px;\" style=\"cursor:hand\" href=\"#\" onclick=\"excluirSubacao(' || s.sbaid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>";
} else {
    $btExluir = "<a style=\"margin: 0 -5px 0 5px;\" style=\"cursor:hand\" href=\"#\" \"><img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\"></a></center>";
}

$sql = <<<DML
    SELECT
        '<center><a href=\"monitora.php?modulo=principal/unidade/cadastro_subacoes&acao=A&sbaid='|| s.sbaid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\"></a>
        $btExluir' AS acao,	
        '<center>'||sbacod||'</center>', 
        sbatitulo,
        to_char(sbadata, 'DD/MM/YYYY HH24:MI:SS') AS sbadata, 
        CASE 
            WHEN sbasituacao = 'P' THEN ' <font color=\"red\">Pendente de Aprovação</font> '
            WHEN sbasituacao = 'A' THEN ' <font color=\"green\">Aprovado</font> '
            WHEN sbasituacao = 'R' THEN ' <font color=\"#3F85FF\">Revisado</font> '
            WHEN sbasituacao = 'E' THEN ' <font color=\"#EAC117\">Enviado para Revisão</font> ' 
        END as situacao
    FROM monitora.pi_subacao s
    LEFT JOIN monitora.pi_subacaounidade su ON su.sbaid = s.sbaid 
    WHERE su.unicod = '{$_SESSION['monitora_var']['unicod']}'
DML;

$cabecalho = array("Ação", "Código", "Título", "Data", "Situação");
$db->monta_lista($sql, $cabecalho, 200, 10, 'N', 'center', '');
?>

<script>
    function enviaForm(pliid) {
        validarFormulario(['sbatitulo', 'sbacod', 'unicod', 'sbadsc'], 'formulario', 'validar()');
    }
    function abrir_lista() {
        unicod = $('#unicod').val();
        url = "planacomorc.php?modulo=principal/unidade/popupptres&acao=A&obrigatorio=n&unicod="+ unicod ;
        
        $.post(url, function(response) {
            $('#modal-ptres .modal-body').html(response);
            $('#modal-ptres .modal-title').html('Seleção de PTRES');
            $('#modal-ptres .btn-primary').remove();
            $('#modal-ptres .btn-default').html('Fechar');
            $('.modal-dialog-ptres').show();
            $('#modal-ptres').modal();
            $('#modal-ptres .chosen-select').chosen();
            $('#modal-ptres .chosen-container').css('width', '100%');
        });
    }

    function validar() {
        var msg = "";
        
        // validando se existe ação selecionado/ valor
        var tabela = document.getElementById('orcamento');
        if (tabela.rows.length == 4) {
            msg += "Você deve selecionar, no mínimo, uma ação no Detalhamento Orçamentário.\n";
        } else {
            jQuery('.somar').each(function() {
                var valor = jQuery(this).val();
                if ('' === valor || '0' === valor || '0,00' === valor) {
                    msg += "Valor do PTRES: '" + jQuery(this).attr('data-ptres') + "' é obrigatório.\n";
                }
            });
        }
        if (msg != "") {
            alert(msg);
            return false;
        } else {
            return true;
        }
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

    function calculovalorPI() {
        var tabela = document.getElementById('orcamento');
        var tot = 0;

        jQuery('.somar').each(function() {
            var valor = jQuery(this).val();

            if ('' == valor) {
                valor = '0';
            }

            valor = replaceAll(valor, '.', '');
            valor = replaceAll(valor, ',', '.');
            tot += parseFloat(valor);
        });

        var c = tot.toString();
        if (c.indexOf('.') == -1) {
            document.getElementById('valortotal').value = tot.toFixed(2);
        } else {
            document.getElementById('valortotal').value = Arredonda(tot, 2);
        }
        document.getElementById('valortotal').onkeyup();
    }

    function verificaDisponivel(campo, ptrid, vlold) {
        var linha = document.getElementById('ptres_' + ptrid);
        var celNaoDetSubacao = 4;
        
        valordisp = parseFloat(replaceAll(replaceAll(linha.cells[celNaoDetSubacao].innerHTML, ".", ""), ",", "."));
        valoratual = parseFloat(replaceAll(replaceAll(campo.value, ".", ""), ",", "."));
        valordigitado = parseFloat(replaceAll(replaceAll(vlold, ".", ""), ",", "."));
        
        //alert(valordisp + ' - '+valoratual + ' - '+ valordigitado);
        
        if (valoratual > (valordisp + valordigitado)) {
            alert('Valor não pode ser maior que o "Não Detalhado da Subação".');
            campo.value = vlold;
            calculovalorPI();
        }
    }

    function excluirSubacao(sbaid) {
        if (confirm('Deseja excluir este registro')) {
            window.location.href = "/monitora/monitora.php?modulo=principal/unidade/cadastro_subacoes&acao=A&sbaidExcluir=" + sbaid;
            return true;
        } else {
            return false;
        }
    }

    function vincular(situacao) {
        var sbaid = '<?php echo $obPi_Subacao->sbaid; ?>';
        var url = window.location.href;
        var parametros = "requisicao=vincular&sbaid=" + sbaid + '&situacao=' + situacao;
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
                        window.location.reload();
                    }
                }
            }
        );
    }

    function detfin(ptrid) {
//	var sbaid = '<?php echo $obPi_Subacao->sbaid; ?>';
//	if(sbaid){
//		janela = window.open('/monitora/monitora.php?modulo=principal/planotrabalhoUN/detalhafinanceiro_subacaoUN&acao=A&ptrid='+ptrid+'&sbaid='+sbaid, 'janela2', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width='+(screen.width-420)+',height=280' ); janela.focus();
//	}
    }

    function marcaPermiteObras() {
        var chPermiteObras = document.getElementById("checkPermiteObras");
        var hSbaObra = document.getElementById("sbaobras");

        if (chPermiteObras.checked)
            hSbaObra.value = "true";
        else
            hSbaObra.value = "false";
    }
    $(".valorDetalhadoPTRES").blur();
</script>
<div id="dibDegug"></div>
    
<div id="modal-ptres" class="modal fade">
    <div class="modal-dialog-ptres" style="width:90%;margin:0 auto;"> 
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
        
    