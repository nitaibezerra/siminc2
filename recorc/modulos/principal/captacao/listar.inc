<?php
/**
 * Listagem de Captações.
 * @version $Id: listar.inc 100540 2015-07-28 18:42:12Z maykelbraz $
 */

$fm = new Simec_Helper_FlashMessage('recorc/captacao');

$perfis = pegaPerfilGeral();

/* Marcando com o Default o status Análise SPO para o Perfil CGO/Equipe Orçamentária */
if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)) {
    if (!chaveTemValor($_REQUEST, 'wrfstatus') && $_REQUEST['wrfstatus'] != 'todos' && $_REQUEST['wrfstatus'] != 'undefined' ) {
        $_REQUEST['wrfstatus'] = STDOC_ANALISE_SPO;
    }
}

$formData = array();
if (isset($_REQUEST['execucao'])) {
    switch ($_REQUEST['execucao']) {
        case 'estadoPedido':
            $query = <<<DML
SELECT cap.capjustifsof,
       cap.capvalorfinal,
       doc.esdid,
       COALESCE(TO_CHAR(cap.datacadastrosiop, 'DD/MM/YYYY'), '-') AS datacadastrosiop
  FROM recorc.captacao cap
    INNER JOIN workflow.documento doc USING(docid)
  WHERE cap.capid = %d
DML;
            $stmt = sprintf($query, $_REQUEST['capid']);
            if (!$dadospedido = $db->pegaLinha($stmt)) {
                $resposta = array('sucesso' => false);
                die(simec_json_encode($resposta));
            }
            $relatorio = consultarRetornoSiop($_REQUEST['capid']);
            $resposta = array(
                'sucesso' => true,
                'resultado' => $dadospedido['esdid'],
                'valor' => ($dadospedido['capvalorfinal']?'R$ '.mascaraNumero($dadospedido['capvalorfinal'], true):'-'),
                'justificativa' => utf8_encode($dadospedido['capjustifsof']?$dadospedido['capjustifsof']:'-'),
                'cadastrosiop' => $dadospedido['datacadastrosiop'],
                'siop' => simec_htmlspecialchars(utf8_encode($relatorio))
            );

            die(simec_json_encode($resposta));
        case 'respostaSiop':
            echo consultarRetornoSiop($_REQUEST['capid']);
            die();
        case 'siop':
            $vieid = $_REQUEST['vieid'];
            $select = <<<DML
SELECT cap.nrccod AS "codigoNaturezaReceita",
       cap.unicod AS "codigoUnidadeRecolhedora",
       cap.foncod AS "subNatureza",
       cap.justificativa,
       cap.metodologia,
       cap.memoriacalculo AS "memoriaDeCalculo",
       cap.capvaloruo AS "valoresBaseExterna",
       cap.capid,
       prf.codcaptacaosiop,
       cap.docid,
       doc.esdid,
       prf.exerciciosiop
  FROM recorc.vinculacaoexercicio vie
    INNER JOIN recorc.captacao cap USING(prfid, unicod, foncod, nrccod, exercicio)
    INNER JOIN recorc.periodoreferencia prf USING(prfid)
    INNER JOIN workflow.documento doc USING(docid)
  WHERE vie.vieid = %d
DML;
            $stmt = sprintf($select, $vieid);
            if (!($dados = $db->pegaLinha($stmt))) {
                $fm->addMensagem('Não foi possível localizar a captação selecionada.', Simec_Helper_FlashMessage::ERRO);
                break;
            }

            // -- Ajustando o campo de valor para criação da requisição
            $dados['valoresBaseExterna'] = array(
                array(
                    'exercicio' => $dados['exerciciosiop'],
                    'valor' => $dados['valoresBaseExterna']
                )
            );

            $ws = new Spo_Ws_Sof_Receita('spo', Spo_Ws_Sof_Receita::PRODUCTION);
            $retorno = $ws->captarBaseExterna($dados['codcaptacaosiop'], SIGLA_SISTEMA, array($dados));

            // -- Primeira tentativa de envio - registro completo
            if (!$retorno->return->sucesso && 'Nada a alterar' !== $retorno->return->mensagensErro) {

                // -- Segunda tentativa de envio - sem subnatureza (fonte)
                $retorno = $ws->captarBaseExterna($dados['codcaptacaosiop'], SIGLA_SISTEMA, array($dados), true);
                if (!$retorno->return->sucesso && 'Nada a alterar' !== $retorno->return->mensagensErro) {

                    processaRetornoSiop($retorno->return->mensagensErro, $dados['capid']);
                    $fm->addMensagem(
                        'Ocorreu um erro ao cadastrar a captação no SIOP.'
                        . ' <a href="javascript:detalharRespostaSiop(' . $dados['capid'] . ')">Veja a lista detalhada</a>.',
                        Simec_Helper_FlashMessage::ERRO
                    );
                    header('Location: ' . $_SERVER['HTTP_REFERER']);
                    die();
                }
            }
            processaRetornoSiop(
                $retorno->return->mensagensErro?$retorno->return->mensagensErro:'Cadastrado com sucesso',
                $dados['capid'],
                'S'
            );

            // -- Tramitando o registro automaticamente, após envio com sucesso ao siop
            tramitarParaCadastradoNoSIOP($dados['docid'], $dados['esdid']);

            // -- Salvando a hora do envio
            $sql = <<<DML
UPDATE recorc.captacao
  SET datacadastrosiop = NOW()
  WHERE capid = %d
DML;
            $sql = sprintf($sql, $dados['capid']);
            $db->executar($sql);
            $db->commit();

            $fm->addMensagem('Cadastrado no SIOP com sucesso.');
            header('Location: ' . $_SERVER['HTTP_REFERER']);
            die();
    }
}

/* Variaveis da tela */
if (isset($_REQUEST['prfid']) && $_REQUEST['prfid'] != '') {
    $prfid = $_REQUEST['prfid'];
} else {
    $prfid = recuperaPeriodo('atual');
}

$where = array();
$where[] = "vie.exercicio = '{$_SESSION['exercicio']}'";
if ($prfid && 'todos' != $prfid) {
    $where[] .= "vie.prfid = '{$prfid}'";
}
if (chaveTemValor($_REQUEST, 'unicod')) {
    $where[] .= "uni.unicod = '{$_REQUEST['unicod']}'";
}
if (chaveTemValor($_REQUEST, 'foncod')) {
    $where[] .= " fon.foncod = '{$_REQUEST['foncod']}'";
}
if (chaveTemValor($_REQUEST, 'nrccod')) {
    $where[] .= " nrc.nrccod = '{$_REQUEST['nrccod']}'";
}
if (chaveTemValor($_REQUEST, 'wrfstatus') && $_REQUEST ['wrfstatus'] != 'todos') {
    if ('A' == $_REQUEST['wrfstatus'] || 'C' == $_REQUEST['wrfstatus']) {
        $where[] .= " cap.captipo = '{$_REQUEST['wrfstatus']}'";
        $where[] .= " doc.esdid = " . STDOC_EM_PREENCHIMENTO;
    } elseif ('NI' == $_REQUEST['wrfstatus']) {
        $where[] .= " doc.esdid IS NULL";
    } elseif(!empty($_REQUEST['wrfstatus'])){
        $where[] .= " doc.esdid = {$_REQUEST['wrfstatus']}";
    }else{
        $where[] .= " doc.esdid = {$_REQUEST['wrfstatus']}";
    }
}

// -- Tratamento de perfis
$perfis = pegaPerfilGeral();
if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
    $sqlUO = <<<DML
EXISTS (SELECT 1
          FROM recorc.usuarioresponsabilidade rpu
          WHERE rpu.usucpf = '%s'
            AND rpu.pflcod = %d
            AND rpu.rpustatus = 'A'
            AND rpu.unicod  = uni.unicod)
DML;
    $where[] = $whereUO = sprintf($sqlUO, $_SESSION['usucpf'], PFL_UO_EQUIPE_TECNICA);
    $whereUO = " AND {$whereUO}";
}

if (!empty($where)) {
    $where = 'WHERE ' . implode(' AND ', $where);
} else {
    $where = '';
}

/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script typ="text/javascript" lang="JavaScript" src="js/recorc.js"></script>
<script type="text/javascript" language="javascript">
// -- Variáveis de estado de documentação
var pedidoAprovado = <?php echo STDOC_APROVADO_SOF; ?>,
    pedidoReprovado = <?php echo STDOC_REPROVADO_SOF; ?>;

jQuery(document).ready(function() {
    // -- Correção paliativa do bug #9920 do bootstrap.
    // -- Correção agendada para a versão 3.0.3 do bootstrap.
    // -- <https://github.com/twbs/bootstrap/issues/9920>
    $("input:radio").change(function() {
        $(this).prop('checked', true);
    });

    $('#modal-alert .modal-header').text('Relatório de comunicação com o SIOP');
    $('#modal-alert .modal-dialog').css('width', '70%');

    // -- Ação do botão buscar
    $('#buscar').click(function() {
        var uri = "recorc.php?modulo=principal/captacao/listar&acao=A"
                + '&unicod=' + $('#unicod').val()
                + '&nrccod=' + $('#nrccod').val()
                + '&foncod=' + $('#foncod').val()
                + '&prfid=' + ($('input[name=prfid]:checked').val() === undefined ? 'todos' : $('input[name=prfid]:checked').val());
        // -- Verificar porque ao clicar duas vezes em uma opção, o comando retorna undefined.
        var wrfstatus = $('input[name=wrfstatus]:checked').val();
        uri += '&wrfstatus=' + wrfstatus;
        window.location = uri;
        return false;
    });
    $('#cadastrarJust').click(function() {
        window.location = "recorc.php?modulo=principal/captacao/listar&acao=A&execucao=inserirJustificativa"
                + '&statusJustificativa=' + $("input[name='statusJustificativa']:checked").val()
                + '&capid=' + $('#capid').val()
                + '&prfid=' + $('#idPeriodo').val()
                + '&unicod=' + $('#unidade').val()
                + '&foncod=' + $('#fonte').val()
                + '&nrccod=' + $('#natureza').val()
                + '&justificativa=' + $('#justificativa').val()
    });
<?php if (chaveTemValor($_REQUEST, 'wrfstatus')): ?>
        $('#wrfstatus_<?php echo $_REQUEST['wrfstatus']; ?>').click();
<?php else: ?>
        $('#wrfstatus_todos').click();
<?php endif; ?>
<?php if ($prfid): ?>
        $('#prfid_<?php echo $prfid; ?>').click();
<?php else: ?>
        $('#prfid_todos').click();
<?php endif; ?>
});

function reload()
{
    window.location = 'recorc.php?modulo=principal/captacao/listar&acao=A';
}

function novo()
{
    window.location = 'recorc.php?modulo=principal/captacao/formulario&acao=A';
}

function estadoPedido(vieid, capid)
{
    $.post(window.location.href,{execucao:'estadoPedido',capid:capid},function(data){
        var data = JSON.parse(data);
        var classeLabel = 'label-default',
            textoLabel = 'SEM RESPOSTA';
        $('#modal-resultado').removeClass('label-success')
                             .removeClass('label-info')
                             .removeClass('label-danger')
                             .removeClass('label-default');
        if (pedidoAprovado == data.resultado) {
            classeLabel = 'label-success';
            textoLabel = 'APROVADO';
        } else if (pedidoReprovado == data.resultado) {
            classeLabel = 'label-danger';
            textoLabel = 'REPROVADO';
        } else if ('' == textoLabel) { // -- ???
            classeLabel = 'label-info';
            textoLabel = 'PARCIAL';
        }
        $('#modal-resultado').text(textoLabel).addClass(classeLabel);
        $('#modal-valor').text(data.valor);
        $('#modal-justificativa').text(data.justificativa);
        $('#modal-siop :nth-child(2)').html($('<div/>').html(data.siop).text());
        $('#modal-data-siop').text(data.cadastrosiop);
        $('#modal-form').modal();
    });
}

function editCaptacao(id) {
    window.location = "recorc.php?modulo=principal/captacao/formulario&acao=A&vieid=" + id;
}
</script>
<style type="text/css">
rigth{float:right}
rigth{float:center}
.table .acertos_uo{color:#E1004C !important}
.table .preto{color:#000000 !important}
.table .cinza{color:#778899 !important}
.table .azul{color:#1E90FF !important}
.text-center{text-align:left !important}
.apenasTexto{border:none;background-color:#FFFFFF}
.periodoCorrente{color:#CD3333;font-weight:bold}
.periodoCorrente.active{color:yellow!important}
</style>
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Lista de Naturezas de Receitas (<?php echo $_SESSION['exercicio']; ?>)</li>
    </ol>
    <div class="well">
        <fieldset>
            <form id="formPrincipal" name="formPrincipal" method="POST"
                  class="form-horizontal">
                <input type="hidden" name="vieid" id="vieid"
                       value="<?php echo $vieid ?>" />
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Unidade Orçamentária (UO)</label>
                    <div class="col-lg-10">
                        <?php
                        $sql = <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || unidsc AS descricao
  FROM public.unidade uni
  WHERE uni.unicod IN (SELECT DISTINCT unicod
                         FROM recorc.vinculacaoexercicio
                         WHERE exercicio = '{$_SESSION['exercicio']}')
    AND uni.unistatus = 'A'
    {$whereUO}
ORDER BY uni.unicod
DML;
                        $unicod = $_REQUEST['unicod'];
                        $db->monta_combo('unicod', $sql, 'S', 'Selecione uma Unidade', null, null, null, null, 'N', 'unicod', null, '', null, 'class="form-control chosen-select" style="width=100%;""', null, (isset($unicod) ? $unicod : null));
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputNatureza" class="col-lg-2 control-label">Natureza da Receita</label>
                    <div class="col-lg-10">
                        <?php
                        $sqlNatureza = <<<DML
SELECT DISTINCT nrccod AS codigo,
                nrccod || ' - ' || nrcdsc AS descricao
  FROM public.naturezareceita
  WHERE nrccod IN (SELECT DISTINCT nrccod
                     FROM recorc.vinculacaoexercicio
                     WHERE exercicio = '{$_SESSION['exercicio']}')
                     order by 1
DML;
                        $nrccod = $_REQUEST['nrccod'];
                        $db->monta_combo('nrccod', $sqlNatureza, 'S', 'Selecione uma Natureza', null, null, null, null, 'N', 'nrccod', null, '', null, 'class="form-control chosen-select" style="width=100%;" required="required""', null, (isset($nrccod) ? $nrccod : null));
                        ?> </div>
                </div>
                <div class="form-group">
                    <label for="inputNatureza" class="col-lg-2 control-label">Fonte de Recurso</label>
                    <div class="col-lg-10">
                        <?php
                        $sqlFonte = <<<DML
SELECT DISTINCT foncod AS codigo,
                foncod || ' - ' || fondsc AS descricao
  FROM public.fonterecurso
  WHERE foncod IN (SELECT DISTINCT foncod
                     FROM recorc.vinculacaoexercicio
                     WHERE exercicio = '{$_SESSION['exercicio']}')
                     order by 1
DML;
                        $foncod = $_REQUEST['foncod'];
                        $db->monta_combo('foncod', $sqlFonte, 'S', 'Selecione uma Fonte', null, null, null, null, 'N', 'foncod', null, '', null, 'class="form-control chosen-select" style="width=100%;" required="required""', null, (isset($foncod) ? $foncod : null));
                        ?> </div>
                </div>
                <div class="form-group">
                    <label for="inputName" class="col-lg-2 control-label">Período de referência:</label>
                    <div class="col-md-10">
                        <div class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default">
                                <input type="radio" name="prfid" id="prfid_todos" value="todos" />Todos
                            </label>
                            <?php
                        $sql = <<<DML
SELECT prfid AS codigo,
       prfdsc AS descricao,
       (SELECT 1
          WHERE NOW()::DATE BETWEEN prf.prfdatainicio AND prf.prfdatafim) AS periodo_corrente
  FROM recorc.periodoreferencia prf
  WHERE exercicio = '{$_SESSION['exercicio']}'
    AND prfstatus = 'A'
ORDER BY prfdsc ASC
DML;
                            $dados = $db->carregar($sql);
                            if($dados){
                                foreach ($dados as $valor) {
                                    $compClass = '';
                                    if ('1' == $valor['periodo_corrente']) {
                                        $compClass = 'periodoCorrente';
                                    }

                                    echo <<<DML
    <label class="btn btn-default {$compClass}">
        <input type="radio" name="prfid" id="prfid_{$valor['codigo']}" value="{$valor['codigo']}" />{$valor['descricao']}
    </label>
DML;
                                }
                            }
                            ?>
                        </div>
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputName" class="col-lg-2 control-label">Status da Alteração:</label>
                    <div class="col-md-10">
                        <div class="btn-group" data-toggle="buttons">
                            <label class="btn btn-default">
                                <input type="radio" name="wrfstatus" id="wrfstatus_todos" value="todos" />Todos
                            </label>
                            <label class="btn btn-default">
                                <input type="radio" name="wrfstatus" id="wrfstatus_<?php echo STDOC_EM_PREENCHIMENTO; ?>"
                                       value="<?php echo STDOC_EM_PREENCHIMENTO; ?>" /> Em preenchimento
                            </label>
                            <label class="btn btn-default">
                                <input type="radio" name="wrfstatus" id="wrfstatus_<?php echo STDOC_ALTERADO; ?>"
                                       value="<?php echo STDOC_ALTERADO; ?>" /> Alterado
                            </label>
                            <label class="btn btn-default">
                                <input type="radio" name="wrfstatus" id="wrfstatus_<?php echo STDOC_DE_ACORDO; ?>"
                                       value="<?php echo STDOC_DE_ACORDO; ?>" /> De acordo
                            </label>
                            <label class="btn btn-default">
                                <input type="radio" name="wrfstatus"
                                       id="wrfstatus_<?php echo STDOC_ANALISE_SPO; ?>"
                                       value="<?php echo STDOC_ANALISE_SPO; ?>" />Em Análise SPO
                            </label>
                            <label class="btn btn-default">
                                <input type="radio" name="wrfstatus"
                                       id="wrfstatus_<?php echo STDOC_ACERTOS_UO; ?>"
                                       value="<?php echo STDOC_ACERTOS_UO; ?>" />Acertos UO
                            </label>
                            <label class="btn btn-default">
                                <input type="radio" name="wrfstatus"
                                       id="wrfstatus_<?php echo STDOC_ENVIADO_SOF; ?>"
                                       value="<?php echo STDOC_ENVIADO_SOF; ?>" />Cadastrado SIOP
                            </label>
                            <label class="btn btn-default">
                                <input type="radio" name="wrfstatus"
                                       id="wrfstatus_<?php echo STDOC_APROVADO_SOF; ?>"
                                       value="<?php echo STDOC_APROVADO_SOF; ?>" />Aprovado SOF
                            </label>
                            <label class="btn btn-default">
                                <input type="radio" name="wrfstatus"
                                       id="wrfstatus_<?php echo STDOC_REPROVADO_SOF; ?>"
                                       value="<?php echo STDOC_REPROVADO_SOF; ?>" />Reprovado SOF
                            </label>
                        </div>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <button class="btn btn-warning" type="reset" onclick="reload()">Limpar</button>
                        <button class="btn btn-primary" id="buscar" type="button">Buscar</button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
    <?php echo $fm->getMensagens(); ?>
    <div>
        <?php
        /* Filtro para a Consulta */
        if (!isset($_REQUEST ['prfid'])) {
            $prfid = recuperaPeriodo('atual'); /* Colocar o período atual vigente */
        } else {
            $prfid = $_REQUEST ['prfid'];
        }
        $prfidAnterior = recuperaPeriodo('anterior');
        $empre = STDOC_EM_PREENCHIMENTO;
        $sql = <<<DML
SELECT DISTINCT
    *
FROM
(
SELECT vie.vieid AS codVinculacao,
       CASE WHEN per.prfdatafim < CURRENT_DATE THEN 'C'
            WHEN (CURRENT_DATE BETWEEN per.prfdatainicio AND per.prfdatafim) AND per.prfdsc NOT ILIKE '%PLOA%' THEN 'P'
            WHEN (CURRENT_DATE BETWEEN per.prfdatainicio AND per.prfdatafim) AND per.prfdsc ILIKE '%PLOA%' THEN 'A'
         END AS cor,
       COALESCE(per.prfdsc, '-') AS periodo,
       '<span class=text-center>'||uni.unicod || ' - ' || '</span>'|| unidsc AS unidade,
       '<span class=text-center>'||fon.foncod || ' - ' || '</span>'|| fon.fondsc AS fonte,
       '<span class=text-center>'||nrc.nrccod || ' - ' || '</span>'|| nrc.nrcdsc AS natureza,
       COALESCE(SUM(valor), 0) AS capvalorsof,
       CASE WHEN capvaloruo IS NULL THEN '0' ELSE capvaloruo END AS capvaloruo,
       CASE WHEN esd.esdid IS NULL THEN 'Não iniciado'
            ELSE esd.esddsc
         END AS situacao,
       doc.esdid,
       cap.capid
  FROM recorc.vinculacaoexercicio vie
    LEFT JOIN recorc.captacao cap ON (vie.unicod = cap.unicod
                                      AND vie.foncod = cap.foncod
                                      AND vie.nrccod = cap.nrccod
                                      AND vie.exercicio = cap.exercicio
                                      AND vie.prfid = cap.prfid)
    LEFT JOIN recorc.periodoreferencia per ON per.prfid = vie.prfid
    LEFT JOIN workflow.documento doc ON doc.docid = cap.docid
    LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
    LEFT JOIN recorc.previsaoreceita prv ON
                             prv.unicod = vie.unicod
                             AND prv.foncod = vie.foncod
                             AND prv.nrccod = vie.nrccod
                             AND prv.prfid = vie.prfid
                             AND prv.exercicio = vie.exercicio
    JOIN public.unidade uni ON uni.unicod = vie.unicod
    JOIN public.fonterecurso fon ON fon.foncod = vie.foncod
    JOIN public.naturezareceita nrc ON nrc.nrccod = vie.nrccod AND nrc.nrcano = '{$_SESSION['exercicio']}'
    {$where}
    GROUP BY vie.vieid,
             vie.prfid,
             per.prfdatafim,
             per.prfdatainicio,
             per.prfdsc,
             cap.capvaloruo,
             doc.esdid,
             cap.captipo,
             cap.capid,
             esd.esdid,
             esd.esddsc,
             uni.unicod,
             uni.unidsc,
             fon.foncod,
             fon.fondsc,
             nrc.nrcid,
             nrc.nrcdsc,
             nrc.nrccod
    ORDER BY uni.unicod::integer,
             fon.foncod::integer,
             nrcid::integer
    ) tabela
DML;

        $cabecalhoTabela = array(
            'Período',
            'Unidade Orçamentária',
            'Fonte de Recursos',
            'Natureza de Receita',
            'Valor SOF (R$)',
            'Valor UO (R$)',
            'Status Alteração',
            'Retorno SOF'
        );
#ver($sql);
        $listagem = new Simec_Listagem();
        $listagem->turnOnPesquisator()
            ->setCabecalho($cabecalhoTabela);
        $listagem->setQuery($sql);
        $listagem->addAcao('edit', 'editCaptacao');
        if (array_intersect($perfis, array(PFL_SUPER_USUARIO, PFL_CGO_EQUIPE_ORCAMENTARIA))) {
            $listagem->addAcao('send', 'gravarSiop');
        }
        $listagem->addAcao('info', array('func' => 'estadoPedido', 'extra-params' => array('capid')));
        $listagem->addCallbackDeCampo(array('capvaloruo', 'capvalorsof'), 'mascaraMoeda')
            ->addCallbackDeCampo('esdid', 'formatarEstadoCaptacao')
            ->addCallbackDeCampo(array('unidade', 'fonte', 'natureza'), 'alinhaParaEsquerda');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('capvaloruo', 'capvalorsof'));
        $listagem->addRegraDeLinha(array('campo' => 'situacao', 'op' => 'igual', 'valor' => 'Acertos UO', 'classe' => 'acertos_uo'))
            ->addRegraDeLinha(array('campo' => 'cor', 'op' => 'igual', 'valor' => 'P', 'classe' => 'preto' ))
            ->addRegraDeLinha(array('campo' => 'cor', 'op' => 'igual', 'valor' => 'A', 'classe' => 'azul'))
            ->addRegraDeLinha(array('campo' => 'cor', 'op' => 'igual', 'valor' => 'C', 'classe' => 'cinza'));
        $listagem->esconderColunas(array('cor', 'capid'));
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        ?>
    </div>
    <div class="modal fade" id="modal-form">
        <div class="modal-dialog" style="width:70%">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal"
                            aria-hidden="true">&times;</button>
                    <h4 class="modal-title">Condição do Pedido</h4>
                </div>
                <div class="modal-body">
                    <div class="panel panel-default">
                        <div class="panel-heading">
                            <h3 class="panel-title">Resultado da análise</h3>
                        </div>
                        <table class="table table-bordered table-striped">
                            <tbody>
                                <tr>
                                    <td style="width:25%" nowrap><b>Resultado:</b></td>
                                    <td><span id="modal-resultado" class="label"></span></td>
                                </tr>
                                <tr>
                                    <td nowrap><b>Valor aprovado:</b></td>
                                    <td><span id="modal-valor"></span></td>
                                </tr>
                                <tr>
                                    <td nowrap><b>Data do cadastro no SIOP:</b></td>
                                    <td><span id="modal-data-siop"></span></td>
                                </tr>
                                <tr>
                                    <td nowrap><b>Justificativa:</b></td>
                                    <td id="modal-justificativa"></td>
                                </tr>
                            </tbody>
                        </table>
                    </div>
                    <div class="panel panel-default" id="modal-siop">
                        <div class="panel-heading">
                            <h3 class="panel-title">Histórico de comunicação com o SIOP</h3>
                        </div>
                        <div></div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>