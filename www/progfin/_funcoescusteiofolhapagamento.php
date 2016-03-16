<?php

/**
 * Funções de liberação financeira.
 * $Id$
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
function processarArquivoCusteioFolha($arquivoLiberacoes, $usucpf, $exercicio) {
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

    // -- Verificando se o arquivo foi submetido pelo usuário
    if (!is_uploaded_file($arquivoLiberacoes['tmp_name'])) {
        return array(
            'msg' => 'Não foi possível validar o arquivo enviado.',
            'sucesso' => false
        );
    }

    /* Rodando o arquivo linha a linha */
    $handle = @fopen($arquivoLiberacoes['tmp_name'], "r");
    if ($handle) {
        while (!feof($handle)) {
            $buffer = fgets($handle);
            criarLinhaArquivoLote(trim($buffer));
        }
        fclose($handle);
    }

    if (!($db->commit())) {
        $db->rollback();
        return array(
            'msg' => 'Não foi possível criar os registros de Extração de Custeio da Folha de Pagamento.',
            'sucesso' => false
        );
    }
    return array(
        'msg' => 'Arquivo carregado com Sucesso.',
        'sucesso' => true,
        'processado' => true
    );
}

function limpaTabelaCarga() {
    global $db;
    $sql = <<<DML
    DELETE FROM progfin.cargacusteiofolhapagamento
DML;
    return (bool) $db->executar($sql);
}

function criarLinhaArquivoLote($dados) {
    global $db;
    #ver($dados,strlen($dados), d);
    /*
     * Separando os arquivos com o Final 1 e Final 2
     *  Os de final 1 tem 61 caracteres e os de final 2 tem 56 caracteres 
     *  de largura da string da linha
     */

    if (strlen($dados) == 60) {
        $cfp['cfpano'] = substr($dados, 0, 4);
        $cfp['cfpmes'] = substr($dados, 4, 2);
        $cfp['cfptipobeneficio'] = substr($dados, 6, 1);
        $cfp['cfptipofolha'] = substr($dados, 7, 2);
        $cfp['cfporgao'] = substr($dados, 9, 5);
        $cfp['cfpupag'] = substr($dados, 14, 9);
        $cfp['cfpsgregime'] = substr($dados, 23, 3);
        $cfp['cfpsituacao'] = substr($dados, 26, 2);
        $cfp['cfpctrl'] = substr($dados, 28, 1);
        $cfp['cfpcodigodespesa'] = substr($dados, 29, 9);
        $cfp['cfprubrica'] = substr($dados, 38, 5);
        $cfp['cfpvalor'] = substr($dados, 43, 15) . '.' . substr($dados, 58, 2);
        $cfp['cfpvalor'] = str_replace(',', '', number_format($cfp['cfpvalor'], 2));
    } else if (strlen($dados) == 55) {
        $cfp['cfpano'] = substr($dados, 0, 4);
        $cfp['cfpmes'] = substr($dados, 4, 2);
        $cfp['cfptipobeneficio'] = substr($dados, 6, 1);
        $cfp['cfptipofolha'] = substr($dados, 7, 2);
        $cfp['cfporgao'] = substr($dados, 9, 5);
        $cfp['cfpupag'] = substr($dados, 14, 9);

        /* Setado por padrão pelo gestor */
        $cfp['cfpsgregime'] = 'PEN';
        $cfp['cfpsituacao'] = '99';
        
        $cfp['cfpctrl'] = substr($dados, 23, 1);
        $cfp['cfpcodigodespesa'] = substr($dados, 24, 9);
        $cfp['cfprubrica'] = substr($dados, 33, 5);
        $cfp['cfpvalor'] = substr($dados, 38, 15) . '.' . substr($dados, 53, 2);
        $cfp['cfpvalor'] = str_replace(',', '', number_format($cfp['cfpvalor'], 2));
    } else {
        return " Formato do arquivo inválido . ";
    }

    $sql = "UPDATE  progfin.cargacusteiofolhapagamento
                            SET 
                                    cfpvalor = " . $cfp['cfpvalor'] . " 
                            WHERE 
                                cfpano = '" . $cfp['cfpano'] . "'
                        AND
                                cfpmes = '" . $cfp['cfpmes'] . "'
                        AND
                                cfptipobeneficio = '" . $cfp['cfptipobeneficio'] . "'
                        AND
                                cfptipofolha = '" . $cfp['cfptipofolha'] . "'
                        AND
                                cfporgao = '" . $cfp['cfporgao'] . "'
                        AND
                                cfpupag = '" . $cfp['cfpupag'] . "'
                        AND
                                cfpsgregime = '" . $cfp['cfpsgregime'] . "'
                        AND
                                cfpsituacao = '" . $cfp['cfpsituacao'] . "'
                        AND
                                cfpctrl = '" . $cfp['cfpctrl'] . "'
                        AND
                                cfpcodigodespesa = '" . $cfp['cfpcodigodespesa'] . "'
                        AND
                                cfprubrica = '" . $cfp['cfprubrica'] . "' RETURNING cfpid ";
    $cfpid = $db->pegaUm($sql);
    if ((bool) $cfpid) {
        #ver($cfpid, $sql, $cfp, strlen($dados), d);
        return true;
    } else {
        $sql = "INSERT INTO 
		    		progfin.cargacusteiofolhapagamento(
	    				cfpano, 
	    				cfpmes, 
	    				cfptipobeneficio, 
	    				cfptipofolha, 
	    				cfporgao, 
	    				cfpupag, 
	    				cfpsgregime, 
	    				cfpsituacao, 
	    				cfpctrl, 
	    				cfpcodigodespesa, 
	    				cfprubrica, 
	    				cfpvalor)
				VALUES(
                    	'{$cfp['cfpano']}',
                    	'{$cfp['cfpmes']}',
                    	'{$cfp['cfptipobeneficio']}',
                    	'{$cfp['cfptipofolha']}',
                    	'{$cfp['cfporgao']}',
                    	'{$cfp['cfpupag']}',
                    	'{$cfp['cfpsgregime']}',
                    	'{$cfp['cfpsituacao']}',
                    	'{$cfp['cfpctrl']}',
                    	'{$cfp['cfpcodigodespesa']}',
                    	'{$cfp['cfprubrica']}',
		    	{$cfp['cfpvalor']})";

        return (bool) $db->executar($sql);
    }
}

//
//function carregarDadosDoLote($llfid) {
//    global $db;
//    $sql = <<<DML
//SELECT llf.llfid,
//       TO_CHAR(llf.llfinclusao, 'DD/MM/YYYY às HH24:MI:SS') AS llfinclusao,
//       COUNT(1) AS qtdliberacoes,
//       usu.usunome,
//       esd.esddsc,
//       doc.docid
//  FROM progfin.loteliberacoesfinanceiras llf
//    INNER JOIN seguranca.usuario usu USING(usucpf)
//    INNER JOIN workflow.documento doc USING(docid)
//    INNER JOIN workflow.estadodocumento esd USING(esdid)
//    LEFT JOIN progfin.liberacoesfinanceiras lfn USING(llfid)
//  WHERE llf.llfid = %d
//    GROUP BY llf.llfid,
//             llf.llfinclusao,
//             usu.usunome,
//             esd.esddsc,
//             doc.docid
//DML;
//    $stmt = sprintf($sql, $llfid);
//    return $db->pegaLinha($stmt);
//}

/* Processar os dados de acordo com as regras de custeio da folha */

function processarDadosCusteioFolha($usucpf, $exercicio) {
    global $db;
    $mesAtual = $_REQUEST['mes'];

    /* Processando a Chave */
    $sql = <<<DML
SELECT DISTINCT
    cfp.cfpano,
    cfp.cfpmes,
    cfp.cfporgao,
    cfp.cfptipofolha,
    cfp.cfpano||cfp.cfpmes||cfp.cfporgao||cfp.cfptipofolha AS chave
FROM
    progfin.cargacusteiofolhapagamento cfp
WHERE
    cfp.cfpano||cfp.cfpmes||cfp.cfporgao||cfp.cfptipofolha NOT IN
    (
        SELECT
            dcf.dcfano||dcfmes||dcforgao||dcftipofolha
        FROM
            progfin.dadoscusteiofolhapagamento dcf )
    
DML;
    /* Inserindo as chaves que ainda não constam na tabela de dados */
    $chaves = $db->carregar($sql);
#    $ver($sql,d);
    if (is_array($chaves)) {
        foreach ($chaves as $chave) {
            #ver("entrou aqui!");
            $sql = <<<DML
                INSERT INTO progfin.dadoscusteiofolhapagamento(
                                                dcfano,
                                                dcfmes,
                                                dcforgao,
                                                dcftipofolha
                                                )
                                        VALUES(
                                                '{$chave['cfpano']}',
                                                '{$chave['cfpmes']}',
                                                '{$chave['cfporgao']}',
                                                '{$chave['cfptipofolha']}'
                                              )
DML;

            $db->executar($sql);
            $db->commit();
            //ver($sql,d);
        }
    }

    /* Recupera a chave para atualizar o Valor */
    $sql = <<<DML
        SELECT
            dcf.dcfano,
            dcf.dcfmes,
            dcf.dcftipofolha,
            dcf.dcforgao
        FROM
            progfin.dadoscusteiofolhapagamento dcf
        WHERE dcf.dcfmes = '{$mesAtual}'    
DML;
    $dadosChaves = $db->carregar($sql);

    /* Atualiza todos os valores para as Chaves */
    if ((is_array($dadosChaves))) {
        foreach ($dadosChaves as $dadosChave) {

            /* Processando a Carga de acodro com as REGRAS */
            $sql = <<<DML
                SELECT
                    rcc.rccnomecoluna,
                    rcc.rccelementodespesa,
                    rcc.rccrubrica,
                    rcc.rcctipooperacao
                FROM
                    progfin.regrascargacusteiofolhapagamento rcc
DML;
            $regrasColunas = $db->carregar($sql);

            if ((is_array($regrasColunas))) {
                foreach ($regrasColunas as $regra) {
                    /* Processa cada linha da carga de acordo com a regra, para aquela Coluna */
                    /* Zerando o valor para a Coluna */
                    $sql = "UPDATE "
                            . "progfin.dadoscusteiofolhapagamento "
                            . "SET {$regra['rccnomecoluna']} = 0 "
                            . "WHERE "
                            . " dcfano = '{$dadosChave['dcfano']}'
                                AND dcfmes = '{$mesAtual}'
                                AND dcftipofolha = '{$dadosChave['dcftipofolha']}'
                                AND dcforgao = '{$dadosChave['dcforgao']}'";
                    $db->carregar($sql);
                    $rubricas = explode(',', $regra['rccrubrica']);
                    /* Processando rubricas individualmente */
                    if (is_array($rubricas)) {
                        #ver($rubricas,d);
                        foreach ($rubricas as $rubrica) {
                            /* Pega o valor atual para a Regra, para somar ou subtrair */
                            $valorParaRegra = $db->pegaUm(""
                                    . "SELECT {$regra['rccnomecoluna']} FROM progfin.dadoscusteiofolhapagamento WHERE "
                                    . " dcfano = '{$dadosChave['dcfano']}'
                                        AND dcfmes = '{$mesAtual}'
                                        AND dcftipofolha = '{$dadosChave['dcftipofolha']}'
                                        AND dcforgao = '{$dadosChave['dcforgao']}'");

                            /* Controle que Soma (1 e 4) */
                            $sql = <<<DML
                                SELECT
                                    COALESCE(SUM(cfp.cfpvalor), 0)      AS total
                                FROM
                                    progfin.cargacusteiofolhapagamento cfp
                                WHERE
                                    cfp.cfprubrica = '{$rubrica}'
                                AND cfp.cfpcodigodespesa::INTEGER IN ({$regra['rccelementodespesa']})
                                AND cfp.cfpano = '{$dadosChave['dcfano']}'
                                AND cfp.cfpmes = '{$mesAtual}'
                                AND cfp.cfptipofolha = '{$dadosChave['dcftipofolha']}'
                                AND cfp.cfporgao = '{$dadosChave['dcforgao']}'
                                AND cfpctrl::integer IN (1,4)
DML;
                            #ver($sql,d);
                            $dadosUpdate = $db->carregar($sql);
                            $valorParaRegra = $valorParaRegra + $dadosUpdate[0]['total'];
                            $sql = "UPDATE "
                                    . "progfin.dadoscusteiofolhapagamento "
                                    . "SET {$regra['rccnomecoluna']} = {$valorParaRegra} "
                                    . "WHERE "
                                    . " dcfano = '{$dadosChave['dcfano']}'
                                AND dcfmes = '{$mesAtual}'
                                AND dcftipofolha = '{$dadosChave['dcftipofolha']}'
                                AND dcforgao = '{$dadosChave['dcforgao']}'";
                            $db->executar($sql);
                            $db->commit();
                            /* Controle que Subtrai (2 e 3) */
                            $sql = <<<DML
                                SELECT
                                    COALESCE(SUM(cfp.cfpvalor), 0)      AS total
                                FROM
                                    progfin.cargacusteiofolhapagamento cfp
                                WHERE
                                    cfp.cfprubrica = '{$rubrica}'
                                AND cfp.cfpcodigodespesa::INTEGER IN ({$regra['rccelementodespesa']})
                                AND cfp.cfpano = '{$dadosChave['dcfano']}'
                                AND cfp.cfpmes = '{$mesAtual}'
                                AND cfp.cfptipofolha = '{$dadosChave['dcftipofolha']}'
                                AND cfp.cfporgao = '{$dadosChave['dcforgao']}'
                                AND cfpctrl::integer IN (2,3)
DML;
                            $dadosUpdate = $db->carregar($sql);
                            $valorParaRegra = $valorParaRegra - $dadosUpdate[0]['total'];
                            $sql = "UPDATE "
                                    . "progfin.dadoscusteiofolhapagamento "
                                    . "SET {$regra['rccnomecoluna']} = {$valorParaRegra} "
                                    . "WHERE "
                                    . " dcfano = '{$dadosChave['dcfano']}'
                                AND dcfmes = '{$mesAtual}'
                                AND dcftipofolha = '{$dadosChave['dcftipofolha']}'
                                AND dcforgao = '{$dadosChave['dcforgao']}'";
                            $db->executar($sql);
                            $db->commit();
                            #ver($sql,d);
                        }
                    }
                }
            }
        }
    }

    return array(
        'msg' => "Arquivo(s) do mês {$mesAtual} Processado(s) com Sucesso.",
        'sucesso' => true
    );
}
