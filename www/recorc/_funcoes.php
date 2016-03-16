<?php

/**
 * Funções gerais do módulo RECORC.
 * @version $Id: _funcoes.php 100540 2015-07-28 18:42:12Z maykelbraz $
 */
include_once APPRAIZ . 'includes/workflow.php';
/**
 * Funções de apoio do módulo Planacomorc.
 * @see funcoesspo.php
 */
require_once APPRAIZ . 'includes/funcoesspo.php';
require APPRAIZ . 'includes/library/simec/DB/DML.php';

/**
 * Remove parametros de busca da URI base do sistema.
 */
function limpaURI() {
    $tmpURI = explode('&', $_SERVER['REQUEST_URI']);
    $_SERVER['REQUEST_URI'] = "{$tmpURI[0]}&{$tmpURI[1]}";
}

//Email quando registro retornar para acertos
function emailAcertosUO($usuemail) {
    global $db;
    $sql = "SELECT usunome
  FROM seguranca.usuario where usuemail = '{$usuemail}'";
    $usuNome = $db->pegaUm($sql);

    $remetente = array("email" => $usuemail);
    $destinatario = $usuemail;
    $assunto = "[SIMEC]  SPO - Receita Orçamentária";
    $conteudo = "Prezado(a) " . $usuNome . ",<br><br>Um de seus Pedidos de Receita foi devolvido para que sejam efetuadas correções.<br><br>
Acesse o módulo SPO - Receita Orçamentária no SIMEC para maiores detalhes. <br><br>Atenciosamente,<br><br>SIMEC<br>www.simec.mec.gov.br
<br/><br><b>Esta é uma mensagem gerada automaticamente pelo sistema.<br/>Por favor, não responda.</b><br/><br/>";

    if (!strstr($_SERVER['HTTP_HOST'], "simec-local")) {
        enviar_email($remetente, $destinatario, $assunto, $conteudo);
    }
    return true;
}

/**
 * Verifica se existe uma chave em um array e se ela tem um valor definido.
 *
 * @param array $lista Lista para verificação da chave.
 * @param string $chave Chave do array para verificação.
 * @return bool
 */
function chaveTemValor(array $lista, $chave) {
    return isset($lista[$chave]) && !empty($lista[$chave]);
}

function preparaData($data) {
    list($dia, $mes, $ano) = explode('/', $data);
    return "{$ano}-{$mes}-{$dia}";
}

/**
 * Seta uma mensagem para ser exibida com 'getFlashMessage'.
 *
 * @param bool $sucesso Indica se é uma mensagem de sucesso(true), ou falha(false).
 * @param string $msg Mensagem para ser formatada e exibida.
 * @todo Remover esta função
 */
function setFlashMessage($sucesso, $msg) {
    setFlashMessageArray(
            array('sucesso' => $sucesso, 'msg' => $msg)
    );
}

/**
 * Seta uma mensagem para ser exibida com 'getFlashMessage'.
 * O array deve ter os campos 'sucesso'(bool) e 'msg'(string).
 * @param array $msg Mensagem para exibição.
 * @todo Remover esta função
 */
function setFlashMessageArray(array $msg) {
    if (!isset($msg['sucesso']) || !isset($msg['msg'])) {
        trigger_error('Parâmetro inválido.');
    }
    $_SESSION['recorc']['msg'] = $msg;
}

/**
 * Exibe uma mensagem como um alert formatado. Pode receber um array, ou pegar o último valor
 * salvo com setFlashMessage. O array deve ter os campos 'sucesso' e 'msg'.
 *
 * @param array $msg
 * @return type
 * @todo Remover esta função
 */
function getFlashMessage(array $msg = null) {
    if (is_null($msg)) {
        $msg = $_SESSION['recorc']['msg'];
        unset($_SESSION['recorc']['msg']);
    }
    if (empty($msg)) {
        return;
    }

    // -- Formatando a mensagem para exibição
    $textoAlerta = <<<HTML
<div class="alert alert-%s col-md-offset-3 col-md-6 text-center">
    <button class="close" data-dismiss="alert">×</button>
    %s
</div>
<br style="clear:both" />
HTML;

    if ($msg['sucesso']) {
        $textoAlerta = sprintf($textoAlerta, 'success', $msg['msg']);
    } else {
        $textoAlerta = sprintf($textoAlerta, 'danger', $msg['msg']);
    }

    return $textoAlerta;
}

function inserirJustificativa($capid, $dados) {
    global $db;
    if (empty($capid)) {
        throw new Exception('Não foi identificada uma captação apara anexar sua análise.');
    }

    // -- Atualizando a análise da SPO
    $dmlString = <<<DML
UPDATE recorc.captacao
  SET capretornosof = :capretornosof,
      capjustifsof = :capjustifsof,
      capvalorfinal = :capvalorfinal
  WHERE capid = :capid
DML;
    $dml = new Simec_DB_DML($dmlString);
    $dml->addParam('capretornosof', $dados['statusJustificativa'])
            ->addParam('capjustifsof', $dados['justificativaDesc'])
            ->addParam('capvalorfinal', $dados['capvalorfinal'])
            ->addParam('capid', $dados['capid']);

    if (!$db->executar($dml)) {
        throw new Exception('Não foi possível salvar sua análise.');
    }
    $db->commit();
}

/**
 * @todo transformar em um só método com tramitarParaCadastradoNoSIOP
 * @param type $docid
 * @param type $esdidorigem
 * @param type $statusJustificativa
 * @param type $justificativa
 */
function tramitarAposAnalise($docid, $esdidorigem, $statusJustificativa, $justificativa) {
    // -- Pega a ação que deve ser executada
    $aedid = pegaAEDID($esdidorigem, $statusJustificativa);

    // -- Atualizando o status do workflow
    wf_alterarEstado($docid, $aedid, $justificativa, array());
}

/**
 *
 * @todo transformar em um só método com tramitarAposAnalise
 * @param type $docid
 * @param type $esdidorigem
 */
function tramitarParaCadastradoNoSIOP($docid, $esdidorigem) {
    // -- Pega a ação de quede ser executada
    $aedid = pegaAEDID($esdidorigem, null);
    wf_alterarEstado($docid, $aedid, '', array());
}

/**
 * Com base no estado atual do documento e no novo status, descobre o ID da transição que
 * deverá ser executada no workflow.
 *
 * @param int $esdidorigem O ESDID de origem do documento atual.
 * @param int $novostatus O próximo status do documento, identificado por 'A' ou 'R'.
 * @return string|boolean
 */
function pegaAEDID($esdidorigem, $novostatus) {
    switch ("{$esdidorigem}{$novostatus}") {
        case STDOC_ENVIADO_SOF . 'A':
            return AEDID_ENVIADO_APROVADO_SOF;
        case STDOC_ENVIADO_SOF . 'R':
            return AEDID_ENVIADO_REPROVADO_SOF;
        case STDOC_APROVADO_SOF . 'R':
            return AEDID_APROVADO_SOF_REPROVADO_SOF;
        case STDOC_REPROVADO_SOF . 'A':
            return AEDID_REPROVADO_SOF_APROVADO_SOF;
        case STDOC_APROVADO_SOF: // -- $novostatus está vazio
            return AEDID_APROVADO_SOF_ENVIADO_SOF;
        case STDOC_REPROVADO_SOF: // -- $novostatus está vazio
            return AEDID_REPROVADO_SOF_ENVIADO_SOF;

        // -- Transições automáticas entre Não iniciado, Alterado e De Acordo
        case STDOC_EM_PREENCHIMENTO . 'A':
            return AEDID_EM_PREENCHIMENTO_PARA_ALTERADO;
        case STDOC_EM_PREENCHIMENTO . 'C':
            return AEDID_EM_PREENCHIMENTO_PARA_DE_ACORDO;
        case STDOC_DE_ACORDO . 'A':
            return AEDID_DE_ACORDO_PARA_ALTERADO;
        case STDOC_ALTERADO . 'C':
            return AEDID_ALTERADO_PARA_DE_ACORDO;
        case STDOC_ACERTOS_UO . 'C':
            return AEDID_ACERTOS_UO_PARA_DE_ACORDO;

        // -- Transições automáticas entre ANALISE SPO e CADASTRADO SIOP
        case STDOC_ANALISE_SPO . null:
            return AEDID_ANALISE_SPO_PARA_CADASTRADO_SIOP;
        default:
            return false;
    }
}

function deletarVinculacaoExercico($id) {
    global $db;
    $sql = "DELETE FROM recorc.vinculacaoexercicio WHERE vieid = $id";
    $db->executar($sql);
    $db->commit();
    setFlashMessage(true, 'Vinculação excluída com Sucesso!');
    header('Location: recorc.php?modulo=principal/tabelaapoio/vinculacaoExercicio&acao=A');
    die();
}

function listaEdicaoVinculacaoExercicio($id) {
    global $db;
    $sql = "select * FROM recorc.vinculacaoexercicio WHERE vieid = $id";
    $vinculacaoAtual = $db->pegaLinha($sql);
    $unicod = $vinculacaoAtual ['unicod'];
    $foncod = $vinculacaoAtual ['foncod'];
    $nrccod = $vinculacaoAtual ['nrccod'];
    $prfid = $vinculacaoAtual ['prfid'];
    header('Location: recorc.php?modulo=principal/tabelaapoio/vinculacaoExercicio&acao=A&execucao=mostrarAlterar&vieid=' . $id . '&prfid=' . $prfid . '&unicod=' . $unicod . '&foncod=' . $foncod . '&nrccod=' . $nrccod . '');
    die();
}

function editarVinculacaoExercicio($unicod, $foncod, $nrccod, $prsano, $prfid, $id) {
    global $db;
    $sql = "UPDATE recorc.vinculacaoexercicio SET unicod = '$unicod', foncod = '$foncod', nrccod = '$nrccod', exercicio = '$prsano', prfid = '$prfid' where vieid =  $id";
    $db->executar($sql);
    $db->commit();
    setFlashMessage(true, 'Vinculação atualizada com Sucesso!');
    header('Location: recorc.php?modulo=principal/tabelaapoio/vinculacaoExercicio&acao=A');
    die();
}

function inserirVinculacaoExercicio($unicod, $foncod, $nrccod, $prsano, $prfid) {
    global $db;
    $sql = "INSERT INTO recorc.vinculacaoexercicio (unicod, foncod, nrccod, exercicio, prfid) values ('$unicod', '$foncod','$nrccod', '$prsano', '$prfid')";
    $db->executar($sql);
    $db->commit();
    echo "<script>window.location = 'recorc.php?modulo=principal/tabelaapoio/vinculacaoExercicio&acao=A&prfid=$prfid&unicod=$unicod&foncod=$foncod&nrccod=$nrccod&execucao=pesquisa';</script>";
    setFlashMessage(true, 'Vinculação adicionada com Sucesso!');
    die();
}

/**
 * Caso o documento não estaja criado cria um novo
 *
 * @param string $capid
 * @return integer
 * @todo Abstrair dentro do módulo do workflow.
 */
function criarDocumento($capid) {
    global $db;

    $docid = pegarDocid($capid);

    if (!$docid) {
        // recupera o tipo do documento
        $tpdid = TPDID_RECORC_1;
        // descrição do documento
        $docdsc = "Captação de Receita Orçamentária N°" . $capid;
        // cria documento do WORKFLOW
        $docid = wf_cadastrarDocumento($tpdid, $docdsc);
        // atualiza o plano de trabalho
        $sql = "UPDATE recorc.captacao SET  docid = " . $docid . "  WHERE capid = " . $capid;
        $db->executar($sql);
        $db->commit();
    }

    return $docid;
}

/**
 * Pega o id do documento do plano de trabalho
 *
 * @param integer $capid
 * @return integer
 * @todo Abstrair dentro do módulo do workflow.
 */
function pegarDocid($capid) {
    global $db;
    $sql = "Select	docid
			FROM recorc.captacao
			WHERE capid = " . $capid;
    return $db->pegaUm($sql);
}

/**
 * Pega o estado atual do workflow
 *
 * @param integer $capid
 * @return integer
 * @todo Abstrair dentro do módulo do workflow.
 */
function pegarEstadoAtual($capid) {
    global $db;
    $docid = pegarDocid($capid);
    if ($docid) {
        $sql = "SELECT ed.esdid
                FROM workflow.documento d
                    JOIN  workflow.estadodocumento ed on ed.esdid = d.esdid
                WHERE d.docid = " . $docid;
        $estado = (integer) $db->pegaUm($sql);
        return $estado;
    }
    return false;
}

function recuperaPeriodo($status) {
    global $db;
    if ($status == 'anterior') {
        $sql = "SELECT prfid
		FROM
		recorc.periodoreferencia
		WHERE
		prfstatus = 'A'
		ORDER BY
		prfid DESC LIMIT 1 OFFSET 1";
    } elseif ($status == 'atual') {
        $sql = "SELECT prfid
				FROM
				recorc.periodoreferencia
				WHERE
				prfstatus = 'A'
                                 AND CURRENT_DATE BETWEEN prfpreenchimentoinicio AND prfpreenchimentofim";
    } elseif ($status == 'atualformulario') {
        $sql = "SELECT prfid
				FROM
				recorc.periodoreferencia
				WHERE
				prfstatus = 'A'
                                 AND CURRENT_DATE BETWEEN prfpreenchimentoinicio AND prfpreenchimentofim";
        $periodo = $db->carregar($sql);
        return $periodo;
    }
    $periodo = $db->pegaUm($sql);
    if (!$periodo && $status == 'atual') {
        $sql = "SELECT
MAX( prfid ) as prfid
FROM
recorc.periodoreferencia
WHERE
prfstatus = 'A'
";
        $periodo = $db->pegaUm($sql);
    }
    return $periodo;
}

function enviarEmailCaptacao($usuario, $email) {

    $msg = "Prezado(a) " . $usuario . ",

Existem formulários de alteração de previsão de receita que precisam ser readequados.
Acesse o SIMEC para maiores informações

www.simec.mec.gov.br";

    enviar_email(array('nome' => 'Alteração de Receita', 'email' => 'spo.planejamento@mec.gov.br'), 'maykel.braz@mec.gov.br', 'Análise pendente', $msg);
    return true;
}

function processaRetornoSiop($mensagensErro, $capid, $tipo = 'E') {
    global $db;
    if (is_array($mensagensErro)) {
        foreach ($mensagensErro as $msg) {
            processaRetornoSiop($msg, $capid, $tipo);
        }
        return;
    }

    $query = <<<DML
INSERT INTO recorc.respostasiop(rpsmensagem, rpstipo, capid)
  VALUES(:rpsmensagem, :rpstipo, :capid)
DML;
    $dml = new Simec_DB_DML($query);
    $dml->addParam('rpsmensagem', $mensagensErro)
            ->addParam('rpstipo', $tipo)
            ->addParam('capid', $capid);
    $db->executar($dml);
    $db->commit();
}

function formatarEstadoCaptacao($esdid, $dados) {
    switch ($esdid) {
        case STDOC_ENVIADO_SOF;
            return <<<HTML
<center><span style="color:#C09853" class="glyphicon glyphicon-exclamation-sign"></span></center>
HTML;
        case STDOC_APROVADO_SOF;
            return <<<HTML
<center><span style="color:green" class="glyphicon glyphicon-thumbs-up"></span></center>
HTML;
        case STDOC_REPROVADO_SOF;
            return <<<HTML
<center><span style="color:red" class="glyphicon glyphicon-thumbs-down"></span></center>
HTML;
        default:
            return <<<HTML
<center><span style="color:orange" class="glyphicon glyphicon-minus"></span></center>
HTML;
    }
}

function consultarRetornoSiop($capid) {
    global $db;

    $query = <<<DML
SELECT TO_CHAR(rpsdata, 'DD/MM/YYYY às HH24:MI:SS') AS rpsdata,
       rpsmensagem,
       rpstipo
  FROM recorc.respostasiop rps
  WHERE rps.capid = %d
  ORDER BY rpsdata DESC
DML;
    $cabecalho = array(
        'Quando',
        'Mensagem',
        'Tipo'
    );
    $stmt = sprintf($query, $capid);
    $list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
    $list->setQuery($stmt)
            ->setCabecalho($cabecalho);
    return $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}

function tabelaDePrevisoes($previsoes) {
    if (empty($previsoes)) {
        return <<<HTML
<div
    class="alert alert-warning text-center col-md-6 col-md-offset-3">
    <p>
        <i class="glyphicon glyphicon-exclamation-sign"></i> Não há
        previsões carregadas.
    </p>
</div>
HTML;
    }

    // -- Tabela de previsões
    $html = <<<HTML
<div class="panel-group" id="accordion">
  <div class="panel panel-default">
    <div class="panel-heading">
      <h4 class="panel-title">
        <a data-toggle="collapse" data-parent="#accordion" href="#detalhamento-previsoes">
          Previsões SOF (R$)
        </a>
      </h4>
    </div>
    <div id="detalhamento-previsoes" class="panel-collapse collapse">
        <div class="panel-body">
            <table class="table table-striped table-text-centered" style="margin-bottom:0">
                <tbody>
                    <tr>
                        <th>Janeiro</th>
                        <th>Fevereiro</th>
                        <th>Março</th>
                        <th>Abril</th>
                        <th>Maio</th>
                        <th>Junho</th>
                    </tr>
                    <tr>
HTML;
    // -- Valores do primeiro semestre
    $mesN = date("n");
    $valorMes = '';
    for ($i = 0; $i <= 5; $i ++) {
        if (isset($previsoes [$i + 1])) {
            $valorMes = mascaraMoeda($previsoes[$i + 1], false) . '&nbsp;' . ($i + 1 <= $mesN ? '(A)' : '(P)');
            $html .= "<td>{$valorMes}</td>";
        } else {
            $html .= "<td>-</td>";
        }
    }
    $html .= <<<HTML
                    </tr>
                    <tr>
                        <th>Julho</th>
                        <th>Agosto</th>
                        <th>Setembro</th>
                        <th>Outubro</th>
                        <th>Novembro</th>
                        <th>Dezembro</th>
                    </tr>
                    <tr>
HTML;
    $valorMes = '-';
    for ($i = 6; $i <= 11; $i ++) {
        if (isset($previsoes [$i + 1])) {
            $valorMes = mascaraMoeda($previsoes [$i + 1], false) . '&nbsp;' . ($i + 1 <= $mesN ? '(A)' : '(P)');
            $html .= "<td>{$valorMes}</td>";
        } else {
            $html .= "<td>-</td>";
        }
    }
    $html .= <<<HTML
                    </tr>
                </tbody>
            </table>
        </div>
    </div>
</div>
HTML;
    return $html;
}

function seletorPeriodo($vieid, $exercicio) {
    global $db;

    $sql = <<<DML
SELECT vie.vieid AS codigo,
       prf.prfdsc AS descricao,
       prf.prfid,
       (SELECT 1 WHERE NOW()::DATE BETWEEN prf.prfdatainicio AND prf.prfdatafim) AS periodo_corrente
  FROM (SELECT *
          FROM recorc.vinculacaoexercicio vie1
          WHERE vie1.vieid = {$vieid}
        UNION
        SELECT *
          FROM recorc.vinculacaoexercicio vie2
          WHERE EXISTS (SELECT 1
                          FROM recorc.vinculacaoexercicio vie3
                          WHERE vie2.unicod = vie3.unicod
                            AND vie2.foncod = vie3.foncod
                            AND vie2.nrccod = vie3.nrccod
                            AND vie3.exercicio = '{$exercicio}'
                            AND vie3.vieid = {$vieid})) vie
    INNER JOIN recorc.periodoreferencia prf USING(prfid)
    WHERE prf.exercicio = '{$exercicio}'
  ORDER BY prfdsc ASC
DML;

    $html = '';
    $resultado = $db->carregar($sql);
    if(is_array($resultado)){
        foreach ($resultado as $valor) {
            $periodoCorrente = $active = $checked = '';
            if ('1' == $valor['periodo_corrente']) {
                $periodoCorrente = 'periodoCorrente ';
            }
            if ($valor['codigo'] == $vieid) {
                $active = 'active ';
                $checked = 'checked="checked"';
                $prfid = $valor['prfid'];
            }
            $html .= <<<HTML
    <label class="{$periodoCorrente} btn btn-default {$active}">
        <input type="radio" name="vieid" id="vieid_{$valor['codigo']}" value="{$valor['codigo']}" {$checked} />{$valor['descricao']}
    </label>
HTML;
        }
    }

    $html = <<<HTML
<div class="col-md-10" id="buttons_vieid">
    <div class="btn-group" data-toggle="buttons">
    {$html}
    </div>
</div>
HTML;

    return $html;
}

function mostrarAnalise($capid, $esdid) {
    return (
            isset($capid) && !empty($capid) && in_array($esdid, array(STDOC_ENVIADO_SOF, STDOC_APROVADO_SOF, STDOC_REPROVADO_SOF))
            );
}

function carregarCaptacao($vieid, $usucpf) {
    global $db;

    // -- Consultando dados
    $sql = <<<DML
SELECT vie.*,
       cpt.*,
       uni.unidsc,
       fon.fondsc,
       nrc.nrcdsc,
       wfd.esdid,
       cpt.capvaloruo::NUMERIC(15,0) AS capvaloruo, -- removendo as casas depois da virgula
       cpt.capvalorfinal::NUMERIC(15,0) AS capvalorfinal, -- removendo as casas depois da virgula
       usu.usunome,
       usu.usufoneddd,
       usu.usufonenum,
       usu.usuemail
  FROM recorc.vinculacaoexercicio vie
    LEFT JOIN public.unidade uni USING(unicod)
    LEFT JOIN public.fonterecurso fon USING(foncod)
    LEFT JOIN public.naturezareceita nrc
      ON (nrc.nrccod = vie.nrccod AND nrc.nrcano = vie.exercicio)
    LEFT JOIN recorc.captacao cpt
      ON (vie.exercicio = cpt.exercicio
          AND vie.unicod = cpt.unicod
          AND vie.foncod = cpt.foncod
          AND vie.nrccod = cpt.nrccod
          AND vie.prfid = cpt.prfid)
    LEFT JOIN workflow.documento wfd USING(docid)
    LEFT JOIN seguranca.usuario usu USING(usucpf)
  WHERE vie.vieid = %d
DML;
    $stmt = sprintf($sql, $vieid);
    if (!$dados = $db->pegaLinha($stmt)) {
        throw new Exception('Não foi possível carregar a captação solicitada.');
    }

    // -- Verificando se existe um capid e, se necessário, criando um novo
    if (!$dados['capid']) {
        // -- Tenta criar uma nova captação
        criarNovaCaptacao($vieid, $usucpf);
        // -- Executa novamente o método de carregar a captação, mas, desta vez, a captação
        // -- já estará criada e o método seguirá o retorno dos dados da captação.
        return carregarCaptacao($vieid, $usucpf);
    }

    return $dados;
}

/**
 *
 * @param type $vieid
 * @param type $usucpf
 * @todo Fazer esta inserção utilizando o VIEID e não o conjunto de dados da previsão (unicod, prfid, foncod, nrccod).
 */
function criarNovaCaptacao($vieid, $usucpf) {
    global $db;

    $sql = <<<DML
INSERT INTO recorc.captacao(prfid, unicod, foncod, nrccod, exercicio, usucpf)
  SELECT vie.prfid,
         vie.unicod,
         vie.foncod,
         vie.nrccod,
         vie.exercicio,
         '%s'
    FROM recorc.vinculacaoexercicio vie
    WHERE vieid = %d
      AND NOT EXISTS (SELECT 1
                        FROM recorc.captacao cap
                        WHERE cap.prfid = vie.prfid
                          AND cap.unicod = vie.unicod
                          AND cap.foncod = vie.foncod
                          AND cap.nrccod = vie.nrccod)
  RETURNING capid
DML;
    $stmt = sprintf($sql, $usucpf, $vieid);

    if (!($capid = $db->pegaUm($stmt))) {
        throw new Exception('Não foi possível criar uma nova captação para a previsão.');
    }
    $db->commit();

    // -- Criando um novo docid para a captaçaõ
    criarDocumento($capid);

    return $capid;
}

function carregarPrevisoes($vieid) {
    global $db;
    $sql = <<<DML
WITH previsao AS (
  SELECT mes::INTEGER,
         valor::numeric(15,0) AS valor
    FROM recorc.previsaoreceita prt
      INNER JOIN recorc.vinculacaoexercicio vie USING(exercicio, prfid, unicod, foncod, nrccod)
    WHERE vie.vieid = %d)
SELECT mes,
       valor AS valor
  FROM previsao
UNION -- Somatorio de todos os meses presentes para a combinacao unicod+nrccod+foncod+prfid+exercicio
SELECT 99,
       COALESCE(sum(valor), 0) as valor
  FROM previsao
  ORDER BY mes -- NAO TROCAR, O 99 deve SEMPRE ser o ULTIMO retornado pela consulta
DML;
    $stmt = sprintf($sql, $vieid);

    if (!($dadosdb = $db->carregar($stmt))) {
        return array();
    }
    // -- Total das previsões
    $previsoes = array(
        'total' => @array_pop(array_pop($dadosdb)),
        'mensais' => array()
    );

    // -- Previsões mensais
    foreach ($dadosdb as $previsao) {
        $previsoes['mensais'][$previsao['mes']] = $previsao['valor'];
    }

    return $previsoes;
}

function salvarCaptacao($dados, $docid, $esdidatual) {
    global $db;

    if (!$dados['capid']) {
        throw new Exception('Não foi informado um registro de captação para atualização.');
    }

    $dmlString = <<<DML
UPDATE recorc.captacao
  SET justificativa = :justificativa,
      metodologia = :metodologia,
      memoriacalculo = :memoriacalculo,
      capvaloruo = :capvaloruo,
      captipo = :captipo
  WHERE capid = :capid
  RETURNING capid
DML;
    $dml = new Simec_DB_DML($dmlString);
    $dml->addParam('justificativa', $dados['justificativa'])
            ->addParam('metodologia', $dados['metodologia'])
            ->addParam('memoriacalculo', $dados['memoriacalculo'])
            ->addParam('capvaloruo', $dados['capvaloruo'])
            ->addParam('captipo', $dados['captipo'])
            ->addParam('capid', $dados['capid']);

    if (!($db->executar($dml))) {
        throw new Exception('Não foi possível atualizar os dados da captação.');
    }

    switch ($esdidatual) {
        case STDOC_EM_PREENCHIMENTO:
        case STDOC_ALTERADO:
        case STDOC_DE_ACORDO:
        case STDOC_ACERTOS_UO:
            atualizaEstadoDocumento($docid, $esdidatual, $dados);
            break;
    }

    $db->commit();
}

function atualizarDadosUsuario($dadosAtuais, $novosDados) {
    global $db;

    if (($dadosAtuais['usufoneddd'] != $novosDados['usufoneddd']) || ($dadosAtuais['usufonenum'] != $novosDados['usufonenum']) || ($dadosAtuais['usuemail'] != $novosDados['usuemail'])) {
        ver($dadosAtuais, $novosDados);
        $sql = <<<DML
UPDATE seguranca.usuario
  SET usufoneddd = '%s',
      usufonenum = '%s',
      usuemail = '%s'
  WHERE usucpf = '%s'
DML;
        $stmt = sprintf(
                $sql, str_replace(array('.', ',', '-', ' '), '', $novosDados['usufoneddd']), str_replace(array('.', ',', '-', ' '), '', $novosDados['usufonenum']), str_replace(array('.', ',', '-', ' '), '', $novosDados['usuemail']), $dadosAtuais['usucpf']
        );

        if (!$db->executar($stmt)) {
            throw new Exception('Não foi possível atualizar os dados do usuário.');
        }
        $db->commit();
        return true;
    }
    return false;
}

function atualizaEstadoDocumento($docid, $esdidAtual, $dados) {
    $aedidTransicao = pegaAEDID($esdidAtual, $dados['captipo']);
    wf_alterarEstado($docid, $aedidTransicao, '', array());
}

function incluirArquivo($capid, $files) {
    if (empty($capid)) {
        throw new Exception('Não foi informado um identificador de captação para associar ao arquivo.');
    }

    if (empty($files['file']['name'])) {
        throw new Exception('Nenhum arquivo foi enviado para inclusão.');
    }

    // -- Verificando a extensao do arquivo enviado
    $extensoesAceitas = array('pdf', 'xls', 'xlsx');
    if (!in_array(strtolower(end(explode('.', $files['file']['name']))), $extensoesAceitas)) {
        throw new Exception('Por favor, envie arquivos com a extensão pdf ou xls.');
    }

    // -- Verificando erros de upload
    if (0 != $files['file']['error']) {
        $uploadMaxFilesize = ini_get('upload_max_filesize');
        $errors = array(
            UPLOAD_ERR_OK => 'Arquivo carregado com sucesso.',
            UPLOAD_ERR_INI_SIZE => "O tamanho do arquivo é maior que o permitido. (Limite: {$uploadMaxFilesize})",
            UPLOAD_ERR_PARTIAL => 'Ocorreu um problema durante a transferência do arquivo.',
            UPLOAD_ERR_NO_FILE => 'O arquivo enviado estava vazio.',
            UPLOAD_ERR_NO_TMP_DIR => 'O servidor não pode processar o arquivo.',
            UPLOAD_ERR_CANT_WRITE => 'O servidor não pode processar o arquivo.',
            UPLOAD_ERR_EXTENSION => 'O arquivo recebido não é um arquivo válido.'
        );

        throw new Exception("Não foi possível incluir o arquivo enviado.<br />Motivo: {$errors[$files['file']['error']]}");
    }

    // -- Salvando o arquivo no filesystem
    $descricao = current(explode(".", $files['file']['name']));
    $campos = array(
        'angdsc' => "'" . $descricao . "'",
        'capid' => $capid
    );

    $file = new FilesSimec("anexogeral", $campos, "recorc");
    if (!$arqid = $file->setUpload($_FILES['file']['name'], '', true, 'arqid')) {
        throw new Exception('Não foi possível incluir o arquivo enviado.');
    }
}

/* Função para montar o Relatório Dinâmico */

function montaExtratoDinamicoRecorc($post) {
    global $db;

    if (count($post['dados']['cols-qualit']) > 0) {
        $post['dados']['cols-qualit'] = array_filter($post['dados']['cols-qualit'], "removeCampoComEspaco");
    }
    if (count($post['dados']['cols-quant']) > 0) {
        $post['dados']['cols-quant'] = array_filter($post['dados']['cols-quant'], "removeCampoComEspaco");
    }
    if (count($post['dados']['filtros']) > 0) {
        foreach ($post['dados']['filtros'] as $key => $value) {
            $post['dados']['filtros'][$key] = array_filter($post['dados']['filtros'][$key], "removeCampoComEspaco");
        };
    };

    $listagem = new Simec_Listagem();
    /* Muda o tipo do objeto  */
    if ($post['requisicao'] == 'exportarXLS') {
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
    }
    $cabecalho = array();
    /* Retorna vazio caso não seja selecionada nenhuma coluna. */
    if (count($post['dados']['cols-qualit']) == 0 || count($post['dados']['cols-qualit']) == 0) {
        $sql = "SELECT 1 WHERE 1 <> 1 ";
    }

    /* Tratando as colunas do Qualitativo */
    if (count($post['dados']['cols-qualit']) > 0) {
        foreach ($post['dados']['cols-qualit'] as $valor) {
            $titulo = $db->pegaLinha("SELECT crldsc FROM recorc.colunasextrato WHERE crlcod = '{$valor}' AND crltipo = 'QL'");
            $titulo = $titulo['crldsc'];
            // Cabeçalho
            array_push($cabecalho, $titulo);
            // Query
            $select .= " {$valor} ,";
        }
        $select = substr($select, 0, strlen($select) - 1);
        $groupby = $select;
    }

    /* Tratando as colunas do Quantitativo */
    if (count($post['dados']['cols-quant']) > 0) {
        $select .= ", ";
        foreach ($post['dados']['cols-quant'] as $valor) {
            $titulo = $db->pegaLinha("SELECT crldsc FROM recorc.colunasextrato WHERE crlcod = '{$valor}' AND crltipo = 'QT'");
            $titulo = $titulo['crldsc'];
            array_push($cabecalho, $titulo);
            // Query
            /* Testa se a coluna quantitativa é de Expressão */
            $colunaExpressao = $db->pegaLinha("SELECT crlexpquantitativo, crlexpcallback, crlexpcomtotal, crlexpaddgroupby FROM recorc.colunasextrato WHERE crlcod = '{$valor}' AND crltipo = 'QT' AND crlexpquantitativo IS NOT NULL");

            if (!$colunaExpressao) {
                $select .= " SUM({$valor}) AS {$valor} ,";
                $listagem->addCallbackDeCampo("{$valor}", 'mascaraMoeda');
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, "{$valor}");
            } else {
                $select .= " {$colunaExpressao['crlexpquantitativo']} AS {$valor} ,";
                /* Caso tenha função Callback */
                if ($colunaExpressão['crlexpcallback'] != '') {
                    $listagem->addCallbackDeCampo("{$valor}", $colunaExpressao['crlexpcallback']);
                }
                /* Caso seja para totalizar */
                if ($colunaExpressao['crlexpcallback']) {
                    $listagem->addCallbackDeCampo("{$valor}", $colunaExpressao['crlexpcallback']);
                }
                $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, "{$valor}");
                $groupby .= $colunaExpressao['crlexpaddgroupby'];
            }
        }
        $select = substr($select, 0, strlen($select) - 1);
    }

    /* Filtros */
    if (count($post['dados']['filtros']) > 0) {
        foreach ($post['dados']['filtros'] as $chave => $valor) {
            /* @TODO  Lembrar de tratar tipo de dado depois que organizar a tabela */
            $valor = implode($valor, "','");
            $where .= " AND $chave IN ('{$valor}')";
        }
    }

    /* Montando a Query */
    if ($select != '' && $groupby != '') {
        $sql = " SELECT DISTINCT {$select}
        FROM
            recorc.vwalteracoesprevisaocompleta vpc
        {$join}
        WHERE
            vpc.exercicio = '{$_SESSION['exercicio']}'
        {$where}
        GROUP BY
        {$groupby}
        ORDER BY 1 ";
    }

    #ver($post, $sql, $cabecalho, d);
    $dados = $db->carregar($sql);
    if (!is_array($dados)) {
        $dados = array();
    }
    $listagem->setDados($dados);
    $listagem->setCabecalho($cabecalho);
    $listagem->setFormOff();
    /* Mostrar a query em um hidden na tela */
    $saida['listagem'] = $listagem;
    $saida['sql'] = $sql;
    /* Imprime de acordo com a chamada */

    return $saida;
}

function removeCampoComEspaco($var) {
    return ($var !== '');
}

function podeSalvarCaptacao($parametros, $considerarDeAcordo = true) {
    global $db;
    $perfis = pegaPerfilGeral();
    /* Verifica Período Vigente */
    $sql = "SELECT
                    COUNT( 0 )
                   FROM
                    recorc.periodoreferencia prf
                   WHERE
                    prfid = {$parametros['prfid']}
                   AND
                    NOW() BETWEEN prfpreenchimentoinicio AND prf.prfpreenchimentofim";

    $result = $db->pegaLinha($sql);
    $periodoVigente = $result['count'];

    /* Aberto em qualquer período para SU / CGO */
    if (in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)) {
        return true;
    }

    /* Deixa aberto caso esteja em Ajustes UO, sempre. */
    if($parametros['esdid'] == STDOC_ACERTOS_UO){
        return true;
    }

    /* desabilita para edição, caso seja UO e já tenha enviado */
    if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis) && isset($parametros['capid'])) {
        /* desabilita para edição, caso o período de preenchimento já esteja terminado */
        if ($periodoVigente) {
            if ($parametros['esdid'] == STDOC_EM_PREENCHIMENTO
                || $parametros['esdid'] == STDOC_ACERTOS_UO
                || $parametros['esdid'] == STDOC_ALTERADO
                || ($parametros['esdid'] == STDOC_DE_ACORDO && $considerarDeAcordo)) {

                return true;
            }
        } else {
            return false;
        }
    }
}

function pesquisaProjecaoReferencia($unicod,$tipo)
{
    global $db;
    $exercicio = $_SESSION['exercicio'] -1;
    $sql = <<<DML
        SELECT
            exercicio AS ano,
            valor
        FROM progorc.projecaospo
        WHERE tipo = '{$tipo}'
            AND unicod = '{$unicod}'
            AND (exercicio = '{$_SESSION['exercicio']}' OR exercicio = '{$exercicio}')
        ORDER BY ano;
DML;
    $dados = $db->carregar($sql);
    if(!$dados){
        return simec_json_encode(array('resultado' => false));
    }
    $estado = STDOC_ATENDIDO;
    $sql = <<<DML
        SELECT
            CAST(COALESCE(SUM(pddvaloramplicacaoatendido) - SUM(pddvalorreducaoatendido),0)AS NUMERIC(12,2)) AS limite
        FROM progorc.pedidolimite pdl
        INNER JOIN progorc.pedidodetalhe pdd ON (pdl.pdlid = pdd.pdlid)
        INNER JOIN workflow.documento doc ON pdl.docid = doc.docid
        WHERE pdl.pdlreferencia = '{$tipo}'
            AND pdl.unicod = '{$unicod}'
            AND date_part('year',pdl.pdldatapedido) = '{$_SESSION['exercicio']}'
            AND doc.esdid = {$estado}
DML;
    $total = $db->pegaUm($sql);
    $diferenca = number_format($dados[1]['valor'] - $total,2,'.','');
    return simec_json_encode(array_merge($dados,array('limite' => $total,'diferenca' => $diferenca, 'resultado' => true)));

}