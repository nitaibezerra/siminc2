<?php
/**
 * Verifica se a requisição é ajax.
 * Is request xmlHttpRequest
 * @return bool
 */
function isAjax()
{
    return (!empty($_SERVER['HTTP_X_REQUESTED_WITH']) &&
        strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest');
}


function removeNaoNulo($input)
{
    if($input != ''){
        return false;
    }
    return true;
}

function arquivoPreenchido($prpid)
{
    $modeloPrelimites = new Proporc_Service_Prelimites();
    if($modeloPrelimites->recuperaModeloPreenchido($prpid)){
        return <<<HTML
        <button class="btn btn-primary" data-prpid="$prpid" id="dl-preenchimento" type="button">
            <span class="glyphicon glyphicon-download-alt"></span>
            Download
        </button>
HTML;
    }else{
        return <<<HTML
        -
HTML;
    }
}

function notificacao_tramitacao($docid, $prpid)
{
    include_once APPRAIZ . "spo/autoload.php";
    include_once APPRAIZ . "www/proporc/_constantes.php";

    $dados = capturaDadosTramitacao($docid);
    $prelimite = capturaDadosPreLimite($prpid);
    $dadosNotificacao = array(
        'unicod' => $prelimite['unicod'],
        'origem' => $dados['origem'],
        'destino' => $dados['destino'],
        'prpid' => $prpid,
        'usucpf' => $prelimite['usucpf']);
    if(verificaResponsavelTramitacao($dados['estado_origem'], $dados['estado_destino'])){
        $dadosNotificacao['usucpf'] = $prelimite['usucpfresponsavel'];
    }
    return enviaNotificacao($dadosNotificacao);
}

function capturaDadosTramitacao($docid)
{
    $query = <<<DML
        SELECT
            esd1.esddsc AS origem,
            esd2.esddsc AS destino,
            esd1.esdid AS estado_origem,
            esd2.esdid AS estado_destino
        FROM workflow.historicodocumento hd
        INNER JOIN workflow.acaoestadodoc aed ON (hd.aedid = aed.aedid)
        INNER JOIN workflow.estadodocumento esd1 ON (aed.esdidorigem = esd1.esdid)
        INNER JOIN workflow.estadodocumento esd2 ON (aed.esdiddestino = esd2.esdid)
        WHERE hd.docid = $docid
        ORDER BY hd.hstid DESC
        limit 1;
DML;
    global $db;
    return $db->pegaLinha($query);
}

function capturaDadosPreLimite($prpid)
{
    $query = <<<DML
        SELECT
            unicod,
            usucpf,
            usucpfresponsavel
        FROM proporc.prelimites_pessoal
        WHERE prpid = $prpid;
DML;
    global $db;
    return $db->pegaLinha($query);
}

function verificaResponsavelTramitacao($origem,$destino)
{
    if($origem == ESTADO_PRELIMITE_EM_PREENCHIMENTO && $destino == ESTADO_PRELIMITE_ANALISE_SPO){
        return true;
    }else if($origem == ESTADO_PRELIMITE_ANALISE_SPO && $destino == ESTADO_PRELIMITE_AJUSTES_UO){
        return false;
    }else if($origem == ESTADO_PRELIMITE_AJUSTES_UO && $destino == ESTADO_PRELIMITE_ANALISE_SPO){
        return true;
    }
}

function enviaNotificacao($dados)
{
    if($dados['usucpf'] == ''){
        echo <<<SCRIPT
            <script>alert('Não encontramos o CPF do usuário para notificá-lo.');
                //window.close();
            </script>
SCRIPT;
        return true;
    }
    return cadastrarAvisoUsuario(array(
        'sisid' => SISID,
        'usucpf' => $dados['usucpf'],
        'mensagem' => 'O Pré-limite da Unidade '.$dados['unicod']. ' foi tramitado de '. $dados['origem'] . ' para '. $dados['destino'].'. Favor, acessar.',
        'url' => 'proporc.php?modulo=principal/prelimite/pessoal&acao=A&requisicao=acessar&id='.$dados['prpid']));
}

function podeSalvarPrelimite()
{
    $hoje = new DateTime('now');
    $limite = new DateTime('2015-05-27 23:59:59');

    if ($hoje > $limite){
        return false;
    }
    return true;
}

function concatenaProgramatica($prgcod, $linha) {
    return "{$prgcod}.{$linha['acacod']}.{$linha['loccod']}";
}

function concatenaAcao($acacod, $linha) {
    return <<<HTML
<p style="text-align:left">{$acacod} - {$linha['acadsc']} - {$linha['sacdsc']}</p>
HTML;
}

function cbStatusDespesa($status) {
    switch ($status) {
        case 'S': return '<span class="label label-default">Sem alterações</span>';
        case 'I': return '<span class="label label-warning">Incompleto</span>';
        case 'A': return '<span class="label label-success">Alterado</span>';
        default: return '<span class="label label-danger">SEM TRATAMENTO</span>';
    }
}

function concatenaPO($plotitulo, $linha) {
    return <<<HTML
<p style="text-align:left">{$linha['plocodigo']} - {$plotitulo}</p>
HTML;
}

function formatarResultadoEnvio($respsiop, $_, $id) {
    switch ($respsiop) {
        case 'E': return <<<HTML
<span class="glyphicon glyphicon-thumbs-down" style="color:red;cursor:pointer" onclick="detalharProgramatica({$id})"></span>
HTML;
        case 'S': return '<span class="glyphicon glyphicon-thumbs-up" style="color:green"></span>';
        default: return $respsiop;
    }
}

function formatarStatusFonte($statusfonte) {
    switch ($statusfonte) {
        case 'P': return '<span class="label label-danger">Excedida</span>';
        case 'O': return '<span class="label label-success">Ok</span>';
    }
    return $statusfonte;
}

function formatarAcaidComoCheckbox($acaid_2, $dados) {
    if (('A' == $dados['stsfinanceiro']) && ('A' == $dados['stsfisico']) && ('O' == $dados['statusfonte'])) {
        return <<<HTML
<input type="checkbox" name="dados[acaid][{$acaid_2}]"
    data-toggle="toggle" data-on="<span class='glyphicon glyphicon-ok'></span>"
    data-off="&nbsp;" data-size="mini" class="make-switch" />
HTML;
    }
    return '<center>-</center>';
}

function concatenaProgramaticaCompleta($prgcod, $linha) {
    return "{$linha['esfcod']}.{$linha['unicod']}.{$linha['funcod']}.{$linha['sfucod']}.{$prgcod}.{$linha['acacod']}.{$linha['loccod']}";
}

function alinhaEsquerdaComId($campo, $dados, $id){
    return <<<HTML
    <p style="text-align:left!important" data-id="{$id}">{$campo}</p>
HTML;
}

function inputHidden($programatica, $dados, $id)
{
    return <<<HTML
<input type="hidden" name="adicionais[plocod][]" value="{$id}" />{$programatica}
HTML;
}

function formatarUnicod($unicod, $linha)
{
    return <<<HTML
<abbr data-toggle="tooltip" data-placement="top" title="{$unicod} - {$linha['unidsc']}">{$unicod}</abbr>
<input type="hidden" name="limitecategoria[unicod][]" value="{$unicod}" />
<input type="hidden" name="limitecategoria[foncod][]" value="{$linha['foncod']}" />
HTML;
}

function formatarVlrlimite($vlrlimite)
{
    return inputTexto(
        'limitecategoria[vlrlimite][]',
        mascaraMoeda($vlrlimite, false),
        null,
        21,
        true,
        array('return' => true)
    );
}

function formatarCheckbox($id)
{
    return <<<HTML
<input type="checkbox" name="confirmacoes[limites][]" value="{$id}"
    data-toggle="toggle" data-on="<span class='glyphicon glyphicon-ok'></span>"
    data-off="&nbsp;" data-size="mini" class="make-switch" />
HTML;
}

function formatarAlteracaoValor($novovalor, $dados)
{
    list($indicador, $cor) = ((float)$novovalor > (float)$dados['valor'])
            ?array('up', 'green'):array('down', 'red');
        $indicador = <<<HTML
 <span class="glyphicon glyphicon-arrow-{$indicador}" aria-hidden="true" style="color:{$cor}"></span>
HTML;
    return str_replace(('</p>'), "{$indicador}</p>", mascaraMoeda($novovalor));
}
