<?php
/**
 * Funções de liberação financeira.
 * @version $Id: _funcoesliberacoes.php 102315 2015-09-10 17:45:07Z maykelbraz $
 */

/**
 * Funções do workflow
 * @see workflow.php
 */
include_once APPRAIZ . 'includes/workflow.php';

/**
 *
 * @param type $fileUpload
 * @param type $exercicio
 * @return type
 */
function processarArquivoLiberacoes($arquivoLiberacoes, $dados, $usucpf, $exercicio)
{
    global $db;

    if (!is_array($arquivoLiberacoes) || empty($arquivoLiberacoes)) {
        return array(
            'msg' => 'Não foi possível processar sua requisição. Nenhum arquivo foi enviado.',
            'sucesso' => false
        );
    }

    // -- Validação do upload
    $erros = array(
        UPLOAD_ERR_OK => 'Arquivo carregado com sucesso.',
        UPLOAD_ERR_INI_SIZE => 'O tamanho do arquivo é maior que o permitido.',
        UPLOAD_ERR_PARTIAL => 'Ocorreu um problema durante a transferência do arquivo.',
        UPLOAD_ERR_NO_FILE => 'O arquivo enviado estava vazio.',
        UPLOAD_ERR_NO_TMP_DIR => 'O servidor não pode processar o arquivo.',
        UPLOAD_ERR_CANT_WRITE => 'O servidor não pode processar o arquivo.',
        UPLOAD_ERR_EXTENSION => 'O arquivo recebido não é um arquivo válido.'
    );

    if (UPLOAD_ERR_OK != $arquivoLiberacoes['error']) {
        return array(
            'msg' => "Não foi possível carregar o arquivo de liberações. Motivo: {$erros[$arquivoLiberacoes['error']]}",
            'sucesso' => false
        );
    }

    if (substr($arquivoLiberacoes['name'], -4) != '.csv') {
        return array(
            'msg' => 'Não foi possível carregar o arquivo de liberações. Motivo: Apenas arquivos <strong>.csv</strong> são aceitos.',
            'sucesso' => false
        );
    }

    // -- Verificando se o arquivo foi submetido pelo usuário
    if (!is_uploaded_file($arquivoLiberacoes['tmp_name'])) {
        return array(
            'msg' => 'Não foi possível validar o arquivo enviado.',
            'sucesso' => false
        );
    }

    // -- salvando os dados do arquivo
    if (!($llfid = criarNovoLoteLiberacoes($dados, $usucpf, $exercicio))) {
        return array(
            'msg' => 'Não foi possível criar um novo lote de liberações.',
            'sucesso' => false
        );
    }

    // -- Processando cada linha do lote
    if (!($liberacoes = file($arquivoLiberacoes['tmp_name']))) {
        return array(
            'msg' => 'Não foi possível ler o arquivo de lote enviado.',
            'sucesso' => false
        );
    }
    // -- Excluindo a primeira linha (título do arquivo)
    array_shift($liberacoes);
    $separador = $liberacoes[0][6];
    foreach ($liberacoes as $liberacao) {
        $dadosLiberacao = explode($separador, $liberacao);
        $dadosLiberacao = array_map('trim', $dadosLiberacao);
        // -- Observação da liberacao financeira
        $dadosLiberacao[1] = str_replace("'", "''", $dadosLiberacao[1]);

        // -- Criando cada uma das liberações individualmente
        criarNovaLiberacaoFinanceiraLote($dadosLiberacao, $llfid);
    }

    // -- Atualizando com o docid
    criarDocumento($llfid);

    if (!($db->commit())) {
        $db->rollback();
        return array(
            'msg' => 'Não foi possível criar os registros de liberação financeira.',
            'sucesso' => false
        );
    }

    return array(
        'msg' => 'O lote de liberações financeiras foi criado e está aguardando envio.',
        'sucesso' => true,
        'llfid' => $llfid
    );
}

/**
 * Cria um novo lote de liberações.
 * @global cls_banco $db
 * @param string $usucpf O número do CPF de quem criou o lote.
 * @param string $exercicio O exercicio em que o lote foi carregado.
 * @return type
 */
function criarNovoLoteLiberacoes($dados, $usucpf, $exercicio)
{
    global $db;

    $sql = <<<DML
INSERT INTO progfin.loteliberacoesfinanceiras(siafi_username, siafi_password, siafi_ug, usucpf, llfano)
  VALUES('%s', '%s', '%s', '%s', '%s')
  RETURNING llfid
DML;
    $stmt = sprintf(
        $sql,
        AES256_CBC_enc(str_replace(array('.', '-'), '', $dados['siafi_usuario'])),
        AES256_CBC_enc($dados['siafi_password']),
        $dados['siafi_ug'],
        $usucpf,
        $exercicio
    );

    return $db->pegaUm($stmt);
}

function criarNovaLiberacaoFinanceiraLote($dados, $llfid)
{
    global $db;

    $sql = <<<DML
INSERT INTO progfin.liberacoesfinanceiras(
    ungcodfavorecida, lfnobservacao, stccod, ftrcod, vincod, ctgcod, lfnvalorsolicitado, lfnvalorautorizado, lfnvaloratendido, llfid
) VALUES('%s', '%s', '%s', '%s', '%s', '%s', %f, %f, %f, %d)
DML;

    // -- Tratamento da fonte de recursos - a fonte de recurso é sempre precedida por '0'
    $dados[3] = str_pad($dados[3], 10, '0', STR_PAD_LEFT);

    $param = array(
        $dados[0], // -- ungcodfavorecida
        $dados[1], // -- lfnobservacao
        $dados[2], // -- stccod
        $dados[3], // -- ftrcod
        $dados[4], // -- vincod
        $dados[5], // -- ctgcod
        $dados[6], // -- lfnvalorsolicitado
        $dados[6], // -- lfnvalorautorizado
        $dados[6], // -- lfnvaloratendido
        $llfid, // -- llfid -- identificador do lote
    );

    $stmt = vsprintf($sql, $param);
    return (bool)$db->executar($stmt);
}


/**
 * Recupera o docid do lote ou cria um novo.
 *
 * @param integer $llfid Identificador do lote.
 * @param integer $tpdid Tipo de documento.
 * @return integer
 */
function criarDocumento($llfid, $tpdid = TPDOC_LOTE_LIBERACOES_FINANCEIRAS) {
    global $db;

    $llfid = (null !== $llfid) ? $llfid : 0;
    $docid = pegarDocid($llfid, $tpdid);

    if (!$docid) {

        // descrição do documento
        if ($tpdid == TPDOC_LOTE_LIBERACOES_FINANCEIRAS) {
            $docdsc = "Lote de liberações financeiras N° " . $llfid;
            $table = 'progfin.loteliberacoesfinanceiras';
            $campo = 'llfid';
        } else {
            $docdsc = "Liberações financeiras N° " . $llfid;
            $table = 'progfin.liberacoesfinanceiras';
            $campo = 'lfnid';
        }

        // cria documento do WORKFLOW
        $docid = wf_cadastrarDocumento($tpdid, $docdsc);
        // atualiza o plano de trabalho
        $sql = "UPDATE {$table} SET docid = {$docid} WHERE {$campo} = {$llfid}";
        $db->executar($sql);
    }

    return $docid;
}
/**
 * Pega o id do documento do lote de liberações financeiras.
 *
 * @param integer $llfid
 * @return integer
 */
function pegarDocid($llfid, $tpdid) {
    global $db;

    if ($tpdid == TPDOC_LOTE_LIBERACOES_FINANCEIRAS)
        $sql = "SELECT docid FROM progfin.loteliberacoesfinanceiras WHERE llfid = {$llfid}";
    else
        $sql = "SELECT docid FROM progfin.liberacoesfinanceiras WHERE lfnid = {$llfid}";

    return $db->pegaUm($sql);
}

function carregarDadosDoLote($llfid)
{
    global $db;
    $sql = <<<DML
SELECT llf.llfid,
       TO_CHAR(llf.llfinclusao, 'DD/MM/YYYY às HH24:MI:SS') AS llfinclusao,
       COUNT(1) AS qtdliberacoes,
       COUNT(CASE WHEN 'S' = COALESCE(lfn.lfntransferencia, '-') THEN 1 ELSE NULL END) AS qtdatendidos,
       SUM(lfn.lfnvaloratendido) AS totallote,
       SUM(CASE WHEN 'S' = COALESCE(lfn.lfntransferencia, '-') THEN lfn.lfnvaloratendido ELSE 0 END) AS totalloteatendido,
       usu.usunome,
       esd.esdid,
       esd.esddsc,
       doc.docid,
       llf.siafi_ug
  FROM progfin.loteliberacoesfinanceiras llf
    INNER JOIN seguranca.usuario usu USING(usucpf)
    INNER JOIN workflow.documento doc USING(docid)
    INNER JOIN workflow.estadodocumento esd USING(esdid)
    LEFT JOIN progfin.liberacoesfinanceiras lfn USING(llfid)
  WHERE llf.llfid = %d
    GROUP BY llf.llfid,
             llf.llfinclusao,
             usu.usunome,
             esd.esdid,
             esd.esddsc,
             doc.docid,
             llf.siafi_ug
DML;
    $stmt = sprintf($sql, $llfid);
    return $db->pegaLinha($stmt);
}


function carregarErrosDaLiberacao($dados, $mode = 'html')
{
    global $db;
    $sql = <<<DML
SELECT lfe.lfnmensagem
  FROM progfin.liberacoesfinanceiraserro lfe
  WHERE lfe.lfnid = %d
DML;
    $stmt = sprintf($sql, $dados['lfnid']);
    if (!($data = $db->carregar($stmt))) {
        $data = array();
    }

    // -- Se for retornar como JSON, faz o encode da descrição do tipo de crédito
    if ('json' == $mode) {
        foreach ($data as &$_data) {
            $_data['lfnmensagem'] = utf8_encode($_data['lfnmensagem']);
        }
        return simec_json_encode($data);
    } elseif ('html' == $mode) {
        $html = '';
        foreach ($data as $_data) {
            $html = "<li>{$_data['lfnmensagem']}</li>";
        }
        if ('' != $html) {
            $html = <<<HTML
<blockquote><ul>{$html}</ul></blockquote>
HTML;
        } else {
            $html = '&nbsp;';
        }
        return $html;
    }
    return $data;
}
