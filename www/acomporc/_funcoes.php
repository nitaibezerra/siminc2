<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 * @global type $db
 * @param int $prfid
 * @param string $tipo
 * @return string
 */
function apresentaComboQuestionario($prfid, $tipo, $func)
{
    global $db;
    if($prfid && $tipo){
        $dados = $db->carregar(sprintf(retornaQueryQuestionario(),$prfid,$tipo));
        $db->close();
        $ar = array();
        if($dados){
            foreach($dados as $d){
                $ar[] = array('codigo' => $d['codigo'], 'descricao' => $d['descricao']);
            }
        }
        $html = inputCombo('qstid', $ar, NULL, 'questao', array('return' => TRUE, 'acao' => $func));
    }else{
        $html = retornaTextoPadrao();
    }
    return $html;
}

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

/**
 * Apresenta texto HTML(tag <p>) "Nenhum valor passado."
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 * @return string
 */
function retornaTextoPadrao()
{
    return <<<HTML
        <p class="form-control-static">Nenhum valor passado.</p>
HTML;
}

/**
 * Utilizar Função sprintf(prfid)
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 * @return string
 */
function retornaQueryComboAcao()
{
    return <<<DML
        SELECT DISTINCT
            unicod ||'.'||acacod AS codigo,
            unicod ||'.'||acacod AS descricao
        FROM acomporc.snapshotlocalizador
        WHERE prfid = %s
        ORDER BY 1
DML;
}

/**
 * Utilizar Função sprintf(prfid)
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 * @return string
 */
function retornaQueryComboSubacao()
{
    return <<<DML
        SELECT DISTINCT
            sbacod as codigo,
            sbacod as descricao
        FROM acomporc.snapshotsubacao
        WHERE prfid = %s
        ORDER BY 1
DML;
}

/**
 * Utilizar Função sprintf(prfid,qsttipo)
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 * @return string
 */
function retornaQueryQuestionario()
{
    return <<<DML
        SELECT
            qstid as codigo,
            qstnome as descricao
        FROM acomporc.questionario
        WHERE prfid = %s
            AND qsttipo = '%s';
DML;
}

/**
 * Utilizar Função sprintf(exercicio)
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 * return string
 */
function retornaQueryComboRelatorioTCU()
{
    return <<<DML
        SELECT DISTINCT
            acacod AS codigo,
            acacod AS descricao
        FROM acomporc.relgestao rld
        WHERE rld.prfid = '%s'
        ORDER BY 1
DML;
}

/**
 * Utilizar Função sprintf(prsano, prftipo)
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 * return string
 */
function retornaQueryComboPeriodo()
{
    return <<<DML
        SELECT
            prfid as codigo,
            prftitulo as descricao
        FROM acomporc.periodoreferencia
        WHERE prsano = '%s'
            AND prftipo = '%s'
DML;
}

/**
 * Utilizar Função sprintf(unicod, prfid)
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 * return string
 */
function retornaQueryComboAcaoSnapshot()
{
    return <<<DML
        SELECT DISTINCT
            ssl.acacod as codigo,
            ssl.acacod as descricao
        FROM acomporc.snapshotlocalizador ssl
        WHERE ssl.unicod = '%s'
            AND ssl.prfid = '%s'
        ORDER BY ssl.acacod
DML;
}

/**
 * Função de Callback do campo de progresso da coluna.
 * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
 * @param integer $valor
 * @return string
 */
function callbackProgressBar($valor)
{
    return outputBar(true, array('class' => 'progress-bar progress-bar-info progress-bar-striped active' , 'value' => $valor, 'spClass' => ''));
}

/**
 * Função de Callback do campo de Meta Física.
 * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
 * @param integer $analise
 * @param array $dados
 * @return string
 */
function callbackCampoAnalise($analise,$dados)
{
    if($dados['aspid'] == 'NULL'){
        $dados['aspid'] = '';
    }

    $html = <<<HTML
        <input type="hidden" name="aspid[{$dados['ptres']}]" value="{$dados['aspid']}">
        <textarea class="form-control verificacao" cols="8" rows="5" name="analiseexecucao[{$dados['ptres']}]"
            style="margin: 0px; height: 150px; width: 300px;">{$analise}</textarea>
HTML;
    return $html;
}

/**
 * Função de Callback do campo de Meta Física.
 * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
 * @param integer $fisico
 * @param array $dados
 * @return string
 */
function callbackCampoMetaFisica($fisico,$dados)
{
    $html = <<<HTML
        <input class="form-control" name="metafisicareprogramada[{$dados['ptres']}]" value="{$fisico}"
            type="text" onkeyup="this.value=mascaraglobal('#.###.###.###',this.value);"
            onblur="this.value=mascaraglobal('#.###.###.###',this.value);">
HTML;
    return $html;
}

function apresentaComboTipo()
{
    echo <<<SCRIPT
    <script>
        $(document).ready(function(){
            $('#tipo').on('change',function(){
                if($(this).val() != ''){
                    window.location.href= window.location.href + '&tipo='+$(this).val();
                }
            });
        });
    </script>
SCRIPT;
    $tipos = array(
        array('codigo' => 'A', 'descricao' => 'Ação'),
        array('codigo' => 'S', 'descricao' => 'Subação'),
        array('codigo' => 'T', 'descricao' => 'Relatório TCU'));
    $html = inputCombo('tipo', $tipos, $valor, 'tipo',array('return' => true));
    echo <<<HTML
    <div class="well">
        {$html}
    </div>
HTML;

}

function retornaComboUnidadeOrcamentaria($perfis)
{
    $whereUO = verificaPerfilQuery($perfis);
    $sql = <<<DML
        SELECT
            uni.unicod AS codigo,
            uni.unicod || ' - ' || unidsc AS descricao
        FROM public.unidade uni
        WHERE uni.unicod IN (
            SELECT DISTINCT unicod
            FROM recorc.vinculacaoexercicio
            WHERE exercicio = '{$_SESSION['exercicio']}'
            )
            AND uni.unistatus = 'A'
            AND unicod IN('26101', '26298')
            {$whereUO}
        ORDER BY uni.unicod
DML;
    return $sql;
}

function verificaPerfilQuery($perfis)
{
    $whereUO = '';
    if (in_array(PFL_RELATORIO_TCU, $perfis)) {
        $whereUO = <<<DML
            AND EXISTS (
                SELECT 1
                FROM acomporc.relgdados rld
                INNER JOIN acomporc.relgestao rlg USING(rlgid)
                INNER JOIN acomporc.usuarioresponsabilidade rpu USING(rldid)
                WHERE rlg.unicod = uni.unicod
                    AND rpu.usucpf = '%s'
                    AND rpu.pflcod = '%s'
                    AND rpu.rpustatus = 'A'
                )
DML;
        $whereUO = sprintf($whereUO, $_SESSION['usucpf'], PFL_RELATORIO_TCU);
    }
    return $whereUO;
}

function filtroAcao($perfis, $acacod = null, $unicod = null)
{
    // -- Queries da consulta de ações - select
    $whereAcao = '';
    if (in_array(PFL_RELATORIO_TCU, $perfis)) {
        $whereAcao = <<<DML
            AND EXISTS (
                SELECT 1
                FROM acomporc.relgdados rld
                INNER JOIN acomporc.relgestao rlg USING(rlgid)
                INNER JOIN acomporc.usuarioresponsabilidade rpu USING(rldid)
                WHERE rlg.acacod = aca.acacod
                    AND rpu.usucpf = '%s'
                    AND rpu.pflcod = '%s'
                    AND rpu.rpustatus = 'A')
DML;
    $whereAcao = sprintf($whereAcao, $_SESSION['usucpf'], PFL_RELATORIO_TCU);
    }

    $strSQL = <<<HTML
        SELECT
            DISTINCT aca.acacod AS codigo, aca.unicod || '.' || aca.acacod ||' - '|| aca.acatitulo AS descricao
        FROM monitora.acao aca
        WHERE aca.prgano = '%s'
            AND aca.acastatus = 'A'
            %where-filtro-unicod%
            {$whereAcao}
        ORDER BY 2
HTML;

    if (empty($unicod)) {
        $strSQL = str_replace('%where-filtro-unicod%', "AND aca.unicod IN('26101', '26298')", $strSQL);
    } else {
        $strSQL = str_replace('%where-filtro-unicod%', "AND aca.unicod = '{$unicod}'", $strSQL);
    }
    $stmtAcao = sprintf($strSQL, $_SESSION['exercicio']);
    inputCombo('acacod', $stmtAcao, $acacod, 'acacod');
}

function formatarStatusIcone($esdid)
{
    switch ($esdid) {
        case ESDID_TCU_EM_PREENCHIMENTO:
            return '<span class="glyphicon glyphicon-minus" style="color:#f0ad4e"></span>';
        case ESDID_TCU_ANALISE_SPO:
        case ESDID_TCU_ACERTOS_UO:
            return '<span class="glyphicon glyphicon-transfer" style="color:#428bca"></span>';
        case ESDID_TCU_CONCLUIDO:
            return '<span class="glyphicon glyphicon-check" style="color:#5cb85c"></span>';
        default:
            return $esdid;
    }
}

function formatarAcaTipo($acatipo)
{
    switch ($acatipo) {
        case 'N':
            return '<span class="label label-success">Prevista na LOA</span>';
            // no break
        case 'R':
            return '<span class="label label-primary">Não prevista na LOA</span>';
            // no break
    }
}

function consultarPermissoes($rlgid)
{
    global $db;
    $sql = <<<DML
        SELECT COUNT(1) AS qtd,
            rld.rldtipo
        FROM acomporc.relgdados rld
        INNER JOIN acomporc.usuarioresponsabilidade rpu ON(rld.rldid = rpu.rldid AND rpu.rpustatus = 'A')
        WHERE rld.rlgid = %d
            AND rpu.usucpf = '%s'
        GROUP BY rld.rldtipo
DML;
    $stmt = sprintf($sql, $rlgid, $_SESSION['usucpf']);

    if (!$dados = $db->carregar($stmt)) {
        return array();
    }

    $retorno = array();
    foreach ($dados as $linha) {
        $retorno[] = $linha['rldtipo'];
    }
    return $retorno;
}

/**
 * Recebe um template html de um relatório com coringas e os substituí por valores informados em um conjunto de dados.
 *
 * O campo exercicio-anterior é um campo calculado. Campos numéricos e monetários recebem máscara.
 * @param string $templateHtml String contendo o html de um relatório.
 * @param array $dados Conjunto de dados para substituição no template.
 * @return string
 */
function preencheTemplate($templateHtml, array $dados)
{
    $camposNumericos = array(
        'rldmontanteprevisto',
        'rldmontantereprogramado',
        'rldmontanterealizado',
        'rldrapearealizado'
    );
    $camposMonetarios = array(
        'rlddotacaoinicial',
        'rlddotacaofinal',
        'rlddespempenhada',
        'rlddespliquidada',
        'rlddesppaga',
        'rldrapinscprocessado',
        'rldrapinscnaoprocessado',
        'rldrapeaem0101',
        'rldrapeavalorliquidado',
        'rldrapeavalorcancelado'
    );

    // -- Campo calculado exercicio-anterior
    $dados['exercicio-anterior'] = $dados['exercicio'] - 1;

    // -- Processando cada campo e substituíndo o valor no template
    foreach ($dados as $campo => $valor) {
        // -- Processando o campo de ação prioritária
        if ('rldacaoprioritaria' == $campo) {
            $rldacaoprioritaria_t = $rldacaoprioritaria_f = '';
            $valor = $valor?$valor:'f';
            ${"rldacaoprioritaria_{$valor}"} = 'X';
            $templateHtml = str_replace(
                array('%rldacaoprioritaria_t%', '%rldacaoprioritaria_f%'),
                array($rldacaoprioritaria_t, $rldacaoprioritaria_f),
                $templateHtml
            );
            continue;
        }

        // -- Processando o campo de tipo de ação prioritária
        if ('rldacaoprioritariatipo' == $campo) {
            $rldacaoprioritariatipo_p = $rldacaoprioritariatipo_b = $rldacaoprioritariatipo_o = '';
            $valor = strtolower($valor);
            if ('t' == $dados['rldacaoprioritaria']) {
                ${"rldacaoprioritariatipo_{$valor}"} = 'X';
            }
            $templateHtml = str_replace(
                array('%rldacaoprioritariatipo_p%', '%rldacaoprioritariatipo_b%', '%rldacaoprioritariatipo_o%'),
                array($rldacaoprioritariatipo_p, $rldacaoprioritariatipo_b, $rldacaoprioritariatipo_o),
                $templateHtml
            );
            continue;
        }

        // -- Formatando valores monetários
        if (in_array($campo, $camposMonetarios)) {
            $valor = mascaraMoeda($valor, false);
        }

        // -- Formatando valores numéricos
        if (in_array($campo, $camposNumericos)) {
            $valor = mascaraNumero($valor);
        }

        // -- Substituíndo os curingas por valores
        $templateHtml = str_replace("%{$campo}%", $valor, $templateHtml);
    }

    return $templateHtml;
}

/**
 * Gerencia a exportação do relatório, seja em XLS ou PDF.
 *
 * @param string $formato Formato de expotação do relatório. Valores válidos "pdf" e "xls".
 * @param integer $rlgid ID do relatório.
 * @param integer $rldid ID do componente do relatório (acao, po ou acaoRap).
 * @throws Exception Lançada caso a função receba um tipo inválido de saída do relatório.
 */
function exportarRelatorio($formato, $rlgid, $rldid = null, $prfid)
{
    if (($formato != 'xls') && ($formato != 'pdf')) {
        throw new Exception('Extensão desconhecida. Apenas "pdf" e "xls" são extensões válidas.');
    }
    $relatorioGestao = new Acomporc_Service_RelatorioGestaoTCU();
    // -- Carregando os dados do relatório
    $dados = $relatorioGestao->consultarDadosTCU($rlgid, $rldid);

    // -- Processa as linhas de dados e carrega o template para substituição
    $relatorioHtml = array();

    foreach ($dados as $dadosRelatorio) {
        $templateHtml = file_get_contents(
            APPRAIZ . "acomporc/modulos/principal/relatoriogestao/relatorio/aba/{$dadosRelatorio['rldtipo']}/html-{$formato}.php"
        );

        $relatorioHtml[] = preencheTemplate($templateHtml, $dadosRelatorio)
            . relatorioQuestionario($dadosRelatorio['rldid'], $prfid);
    }

    // -- Construíndo o arquivo completo e adicionando o CSS
    $relatorioHtml = "<style type=\"text/css\">".file_get_contents(
        APPRAIZ . "www/acomporc/css/relatorio.css"
    ) ."</style>". implode('<hr class="quadro-tcu" />', $relatorioHtml);

    // -- Gerando a saída do relatório para exportação

    $call = "html2" . ucfirst($formato);
    $call($relatorioHtml);
}

function relatorioQuestionario($rldid, $prfid)
{
    $modeloQuestionario = new Acomporc_Service_Questionario();
    $questionario = $modeloQuestionario->relatorioQuestionario($rldid, $prfid, 'T');

    $html = '';
    if ($questionario) {

        $html = <<<HTML
<div class="quadro-tcu">
    <br />
    <table border="1">
        <thead>
            <tr style="text-align:center;font-weight:bold">
                <th colspan="7">{$questionario[0]['questionario']}</th>
            </tr>
        </thead>
        <tbody>
HTML;

        $i = 1;
        foreach ($questionario as $questao){
            $html .= <<<HTML
            <tr><td colspan="7" style="font-weight:bold">{$i}) {$questao['pergunta']}</td></tr>
            <tr><td colspan="7"><span style="font-weight:bold">Resposta: </span>{$questao['resposta']}</td></tr>
HTML;
            $i++;
        }
        $html .= <<<HTML
        </tbody>
    </table>
</div>
HTML;
    }

    return $html;
}

/**
 * Imprime um conteúdo em formato Xls, trocando os readers de resposta da requisição.
 * @param string $content Conteúdo para conversão em Xls.
 */
function html2Xls($content)
{
	header("Content-Type:   application/vnd.ms-excel; charset=utf-8");
	header("Content-type:   application/x-msexcel; charset=utf-8");
	header("Content-Disposition: attachment; filename=relatorio-tcu.xls");
	header("Expires: 0");
	header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
	header("Cache-Control: private", false);

    echo $content;
    die();
}

/**
 * Imprime um conteúdo em formato Pdf, trocando os readers de resposta da requisição.
 * @param string $content Conteúdo para conversão em Pdf.
 */
function html2Pdf($content)
{
    // -- Preparando a requisição ao webservice de conversão de HTML para PDF do MEC.
    $content = http_build_query(
        array ('conteudoHtml' => utf8_encode($content))
    );

    $context = stream_context_create(
        array(
            'http' => array(
                'method' => 'POST',
                'content' => $content
            )
        )
    );

    // -- Fazendo a requisição de conversão
    $contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);

    header('Content-Type: application/pdf');
    header("Content-Disposition: attachment; filename=relatorio-tcu.pdf");
    echo $contents;
    exit();
}

function apresentarCoordenadorSubacao($dados)
{
    $sql = <<<DML
        SELECT usu.usucpf AS codigo,
            usu.usucpf || ' - ' || usu.usunome AS descricao
        FROM seguranca.perfilusuario pfu
        LEFT JOIN seguranca.usuario usu USING(usucpf)
        WHERE pfu.pflcod = %d
DML;
    $stmt = sprintf($sql, PFL_COORDENADORSUBACAO);
    $combo = inputCombo('usucpf', $stmt, $dados['usucpf'], 'usucpf',array('return'=>true));

    echo <<<HTML
    <div class="col-md-12">
        <form class="form-horizontal" id="form-responsavel" method="POST" role="form">
            <input type="hidden" name="sbacod" value="{$dados['sbacod']}" />
            <input type="hidden" name="periodo" value="{$dados['periodo']}" />
            <input type="hidden" name="requisicao" value="salvarResponsavel" />
            <div class="form-group row">
                <label class="control-label col-md-2" for="usucpf">Responsável: </label>
                <div class="col-md-10">
                    {$combo}
                </div>
            </div>
        </form>
    </div>
    <script type="text/javascript" lang="JavaScript">
        $('#usucpf').chosen();
        $('#usucpf_chosen').css('width', '100%');
    </script>
HTML;
}

function apresentarMonitorInterno($dados)
{
    $sql = <<<DML
        SELECT usu.usucpf AS codigo,
            usu.usucpf || ' - ' || usu.usunome AS descricao
        FROM seguranca.perfilusuario pfu
        LEFT JOIN seguranca.usuario usu USING(usucpf)
        WHERE pfu.pflcod = %d
DML;
    $stmt = sprintf($sql, PFL_MONITOR_INTERNO);
    $combo = inputCombo('usucpf', $stmt, $dados['usucpf'], 'usucpf',array('return'=>true));
    $pflcod = PFL_MONITOR_INTERNO;
    echo <<<HTML
    <div class="col-md-12">
        <form class="form-horizontal" id="form-responsavel" method="POST" role="form">
            <input name="requisicao" type="hidden" value="salvarUsuarioResponsabilidade">
            <input type="hidden" name="rpuid" value="{$dados['rpuid']}">
            <input type="hidden" name="unicod" value="{$dados['unicod']}">
            <input type="hidden" name="prfid" value="{$dados['prfid']}">
            <div class="form-group">
                <label for="prfid" class="col-lg-2 control-label"></label>
                <div class="col-lg-10">
                    <span class="label label-info">Monitor Interno</span>
                </div>
            </div>
            <div class="form-group">
                <div class="col-md-2">
                    <label for="prfid" class="control-label col-md-2" for="usucpf">Responsável:</label>
                </div>
                <div class="col-md-10">
                    {$combo}
                </div>
            </div>
        </form>
    </div>
    <script type="text/javascript" lang="JavaScript">
        $('#usucpf').chosen();
        $('#usucpf_chosen').css('width', '100%');
    </script>
HTML;
}

function apresentarValidadorAcao($dados)
{
    $sql = <<<DML
        SELECT usu.usucpf AS codigo,
            usu.usucpf || ' - ' || usu.usunome AS descricao
        FROM seguranca.perfilusuario pfu
        LEFT JOIN seguranca.usuario usu USING(usucpf)
        WHERE pfu.pflcod = %d
DML;
    $stmt = sprintf($sql, PFL_VALIDADORACAO);
    $combo = inputCombo('usucpf', $stmt, $dados['usucpf'], 'usucpf',array('return'=>true));
    $pflcod = PFL_VALIDADORACAO;
    echo <<<HTML
    <div class="col-md-12">
        <form class="form-horizontal" id="form-responsavel" method="POST" role="form">
            <input name="requisicao" type="hidden" value="salvarUsuarioResponsabilidade">
            <input type="hidden" name="rpuid" value="{$dados['rpuid']}">
            <input type="hidden" name="acacod" value="{$dados['acacod']}">
            <input type="hidden" name="unicod" value="{$dados['unicod']}">
            <input type="hidden" name="prfid" value="{$dados['prfid']}">
            <input type="hidden" name="pflcod" value="{$pflcod}">
            <div class="form-group">
                <label for="prfid" class="col-lg-2 control-label"></label>
                <div class="col-lg-10">
                    <span class="label label-info">Validador Ação</span>
                </div>
            </div>
            <div class="form-group">
                <div class="col-md-2">
                    <label for="prfid" class="control-label col-md-2" for="usucpf">Responsável:</label>
                </div>
                <div class="col-md-10">
                    {$combo}
                </div>
            </div>
        </form>
    </div>
    <script type="text/javascript" lang="JavaScript">
        $('#usucpf').chosen();
        $('#usucpf_chosen').css('width', '100%');
    </script>
HTML;
}

function apresentarCoordenadorAcao($dados)
{
    $sql = <<<DML
        SELECT usu.usucpf AS codigo,
            usu.usucpf || ' - ' || usu.usunome AS descricao
        FROM seguranca.perfilusuario pfu
        LEFT JOIN seguranca.usuario usu USING(usucpf)
        WHERE pfu.pflcod = %d
DML;
    $stmt = sprintf($sql, PFL_COORDENADORACAO);
    $combo = inputCombo('usucpf', $stmt, $dados['usucpf'], 'usucpf',array('return'=>true));
    $pflcod = PFL_COORDENADORACAO;
    echo <<<HTML
    <div class="col-md-12">
        <form class="form-horizontal" id="form-responsavel" method="POST" role="form">
            <input name="requisicao" type="hidden" value="salvarUsuarioResponsabilidade">
            <input type="hidden" name="rpuid" value="{$dados['rpuid']}">
            <input type="hidden" name="acacod" value="{$dados['acacod']}">
            <input type="hidden" name="unicod" value="{$dados['unicod']}">
            <input type="hidden" name="prfid" value="{$dados['prfid']}">
            <input type="hidden" name="pflcod" value="{$pflcod}">
            <div class="form-group">
                <label for="prfid" class="col-lg-2 control-label"></label>
                <div class="col-lg-10">
                    <span class="label label-info">Coordenador Ação</span>
                </div>
            </div>
            <div class="form-group">
                <div class="col-md-2">
                    <label for="prfid" class="control-label col-md-2" for="usucpf">Responsável:</label>
                </div>
                <div class="col-md-10">
                    {$combo}
                </div>
            </div>
        </form>
    </div>
    <script type="text/javascript" lang="JavaScript">
        $('#usucpf').chosen();
        $('#usucpf_chosen').css('width', '100%');
    </script>
HTML;
}

function reorganizaArrayDadosUsuario($dados, $retorno = array('usucpf' => null,'usunome' => null,'usuemail' => null,'rpudata' => null,'foneddd' => null,'fonenum' => null))
{
    if($dados == null || $dados == ''){
        return $retorno;
    }
    $num = 0;
    foreach($retorno as $key => $value){
        $retorno[$key] = $dados[$num];
        $num++;
    }
    return $retorno;
}

function statusNoSIOP($status, $dados, $id) {
    $html = <<<HTML
    <span class="glyphicon glyphicon-%s" style="color:%s" %s></span>
HTML;
    switch ($status) {
        case 'E':
            return sprintf($html, 'thumbs-down', '#D9534F;cursor:pointer;', ' onclick="exibirLogEnvio(' . $dados['aclid'] . ')"');
        case 'S':
            return sprintf($html, 'thumbs-up', '#5CB85C', ' onclick="exibirLogEnvio(' . $dados['aclid'] . ')"');
        default:
            return sprintf($html, 'minus', '#F0AD4E', '');
    }
}

function statusNoSIOPIn($status, $aclid) {
    $html = <<<HTML
        <button class="btn btn-default btn-block" %s>
            <span class="glyphicon glyphicon-%s" style="color:%s;"></span>
            SIOP
        </button>
HTML;
    switch ($status) {
        case 'E':
            return sprintf($html, 'onclick="exibirLogEnvio(' . $aclid . ')"','thumbs-down', '#D9534F');
        case 'S':
            return sprintf($html, 'onclick="exibirLogEnvio(' . $aclid . ')"','thumbs-up', '#5CB85C');
        default:
            return sprintf($html, 'disabled','minus', '#F0AD4E');
    }
}

/**
 * Consulta a tabela de log em busca de registros para a ação programática.
 * @global cls_banco $db Conexão com a base de dados
 * @param type $idAcaoProgramatica
 */
function exibirLogEnvio($dados) {
    global $db;
    $sql = <<<DML
        SELECT
            TO_CHAR(datacriacao, 'DD/MM/YYYY HH24:MI:SS') AS datacriacao,
            COALESCE(wslmsgretorno, '<center>Não informado</center>') AS descricao
        FROM elabrev.ws_log
        WHERE id_acao_programatica IN (
            SELECT aca.id_acao_programatica
            FROM planacomorc.acompanhamento_acao aca
            JOIN acomporc.acompanhamentolocalizador acl ON (aca.docid = acl.docid)
            WHERE aca.id_periodo_referencia = {$dados['prfid']}
                AND acl.aclid = {$dados['aclid']})
        ORDER BY datacriacao DESC
        LIMIT 5
DML;
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    $listagem->addCallbackDeCampo(array('descricao' ), 'alinhaParaEsquerda');
    $listagem->setCabecalho(array('Data', 'Descrição'));
    $listagem->setQuery($sql);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}

function apresentaUsuarioCPF($nome, $dados){
    if($dados['usucpf']){
        return formatar_cpf($dados['usucpf']) .' - '. $nome;
    }
    return 'Não Encontrado';
}

function apresentaUsuarioTelefone($nome, $dados){
    if ($dados['usufoneddd']) {
        return <<<HTML
<p style="text-align:left">({$dados['usufoneddd']}) {$dados['usufonenum']} - {$nome}</p>
HTML;
    }
    return '<span class="label label-warning">Não Encontrado</span>';
}

function retornaPeriodoAtual($tipo){
    global $db;
    $sql = "SELECT
            prfid
        FROM
            acomporc.periodoreferencia
        WHERE
            prftipo = '{$tipo}'
        AND prsano = '{$_SESSION['exercicio']}'
        ORDER BY
            prfinicio DESC
        LIMIT 1";

    return $db->pegaUm($sql);
}

function comporProgramatica($esfcod, $linha){
    return "{$esfcod}.{$linha['unicod']}.{$linha['funcod']}.{$linha['sfucod']}.{$linha['prgcod']}.<b>{$linha['acacod']}</b>.{$linha['loccod']}";
}

function formatarAcao($loccod, $linha)
{
    return <<<HTML
<p style="text-align:left">{$linha['acacod']} - {$linha['acatitulo']}<br /><b>{$loccod}</b> - {$linha['sacdsc']}</p>
HTML;
}
