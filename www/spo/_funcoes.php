<?php

/**
 * @package SiMEC
 * @subpackage spo
 * @version $Id: _funcoes.php 103685 2015-10-15 18:27:31Z maykelbraz $
 */
if (!is_callable('chaveTemValor')) {

    /**
     * Verifica se existe uma chave em um array e se ela tem um valor definido.
     *
     * @param array $lista Lista para verificação da chave.
     * @param string $chave Chave do array para verificação.
     * @return bool
     */
    function chaveTemValor($lista, $chave) {
        if (!is_array($lista)) {
            $lista = array();
        }

        return isset($lista[$chave]) && !empty($lista[$chave]);
    }

}

if (!is_callable('getDocComment')) {

    /**
     * Recupera tag de um bloco de comentario.
     *
     * @param string $str Bloco de comentario.
     * @param string $tag Tag que deseja recuperar.
     * @return string
     */
    function getDocComment($str, $tag = '') {
        if (empty($tag))
            return $str;

        $matches = array();

        if (preg_match_all('/@(\w+)\s+(.*)\r?\n/m', $str, $matches)) {
            $result = array_combine($matches[1], $matches[2]);
        }

        if (isset($result[$tag]))
            return trim($result[$tag]);

        return '';
    }

}

if (!is_callable('mascaraCode')) {

    /**
     * Mascara codigo adicionando botao.
     *
     * @param string $code Bloco de codigo.
     * @return string
     */
    function mascaraCode($code) {
        return '<button type="button"
					class="btn btn-success"
					data-toggle="modal"
					data-target="#codeModal"
					data-xml="' . ((htmlentities($code, ENT_QUOTES, 'UTF-8'))) . '"
					data-code="' . ((htmlentities(substr($code, 0, 1200), ENT_QUOTES, 'UTF-8'))) . '...">
					Vizualizar XML</button>';
    }

}

if (!is_callable('mascaraHeader')) {

    /**
     * Mascara header adicionando e formatando codigo.
     *
     * @param string $header Bloco de codigo.
     * @return string
     */
    function mascaraHeader($header) {
        return "<pre class='prettyprint' style='text-align: left; width: 500px;'>{$header}</pre>";
    }

}

/**
 * Carregar tabelas de apoio.
 *
 * @global cls_banco $db
 * @param array $params $_REQUEST
 * @param array $map Servicos
 * @param object $fm FlashMessage
 * @return array|boolean
 */
function carregarProgramacaoCompleta($params, $map, $fm) {
    global $db;

    header("Keep-Alive: timeout=9999, max=9999");
    set_time_limit(0);

    $servicos = $params['servicos'];

    try {
        $ws = new Spo_Ws_Sof_Qualitativo($params['log'] ? 'spo' : null);

        if (!empty($servicos)) {
            $mensagem = 'Registros importados com sucesso.';

            foreach ($servicos as $servico) {
                $count = 0;
                $inserts = null;
                $informacoes = $map[$servico];
                $obterProgramacaoCompleta = new ObterProgramacaoCompleta();
                $obterProgramacaoCompleta->exercicio = $_SESSION['exercicio'];
                $obterProgramacaoCompleta->codigoMomento = $params['momento'];
                $obterProgramacaoCompleta->$informacoes['ws'] = true;
                $return = $ws->obterProgramacaoCompleta($obterProgramacaoCompleta)->return;

                if ($return->mensagensErro)
                    throw new Exception($return->mensagensErro);

                $resultados = $return->$informacoes['dto'];

                if ($resultados) {
                    foreach ($resultados as $resultado) {
                        $values = null;
                        $dados = get_object_vars($resultado);
                        $campos = strtolower(implode(', ', array_keys($dados)));
                        $valores = array();

                        foreach ($dados as $index => $value)
                            $valores[] = addslashes($value);

                        $inserts.= "insert into {$informacoes['tbl']} (" . $campos . ") values ('" . implode("', '", $valores) . "');\n";
                        $count++;
                    }

                    $delete = "delete from {$informacoes['tbl']}";

                    $db->executar($delete);
                    $db->executar($inserts);
                }

                $mensagem.= "<br>{$count} {$servico} importados.";

                $db->commit();
            }

            $fm->addMensagem($mensagem, Simec_Helper_FlashMessage::SUCESSO);

            return $servicos;
        } else {
            $mensagem = 'Nenhum serviço selecionado para importação.';

            $fm->addMensagem($mensagem, Simec_Helper_FlashMessage::AVISO);

            return array();
        }
    } catch (Exception $e) {
        $mensagem = "Ocorreu um problema ao importar os registros<br>{$e->getMessage()}";

        $fm->addMensagem($mensagem, Simec_Helper_FlashMessage::ERRO);

        return $servicos;
    }
}

/**
 * Carregar tabelas de apoio.
 *
 * @global cls_banco $db
 * @param array $params $_REQUEST
 * @param array $map Servicos
 * @param object $fm FlashMessage
 * @return array|boolean
 */
function carregarTabelasApoio($params, $map, $fm) {
    global $db;

    header("Keep-Alive: timeout=9999, max=9999");
    set_time_limit(0);

    $servicos = $params['servicos'];

    try {
        $ws = new Spo_Ws_Sof_Qualitativo($params['log'] ? 'spo' : null);

        if (!empty($servicos)) {
            $mensagem = 'Registros importados com sucesso.';

            foreach ($servicos as $servico) {
                $count = 0;
                $inserts = null;
                $informacoes = $map[$servico];
                $obterTabelasApoio = new ObterTabelasApoio();
                $obterTabelasApoio->exercicio = $_SESSION['exercicio'];
                $obterTabelasApoio->$informacoes['ws'] = true;
                $return = $ws->obterTabelasApoio($obterTabelasApoio)->return;

                if ($return->mensagensErro)
                    throw new Exception($return->mensagensErro);

                $resultados = $return->$informacoes['dto'];

                if ($resultados) {
                    foreach ($resultados as $resultado) {
                        $values = null;
                        $dados = get_object_vars($resultado);
                        $campos = strtolower(implode(', ', array_keys($dados)));
                        $valores = array();

                        foreach ($dados as $index => $value)
                            $valores[] = addslashes($value);

                        $inserts.= "insert into {$informacoes['tbl']} (" . $campos . ") values ('" . implode("', '", $valores) . "');\n";
                        $count++;
                    }

                    $delete = "delete from {$informacoes['tbl']}";

                    $db->executar($delete);
                    $db->executar($inserts);
                }

                $mensagem.= "<br>{$count} {$servico} importados.";

                $db->commit();
            }

            $fm->addMensagem($mensagem, Simec_Helper_FlashMessage::SUCESSO);

            return $servicos;
        } else {
            $servicos = array();

            $mensagem = 'Nenhum serviço selecionado para importação.';

            $fm->addMensagem($mensagem, Simec_Helper_FlashMessage::AVISO);

            return true;
        }
    } catch (Exception $e) {
        $db->rollback();

        $mensagem = "Ocorreu um problema ao importar os registros<br>{$e->getMessage()}";

        $fm->addMensagem($mensagem, Simec_Helper_FlashMessage::ERRO);

        return $servicos;
    }
}

/**
 * Obter programas por órgão
 */
function obterProgramasPorOrgao($exercicio, $orgao, $momento) {
    global $db;

    header("Keep-Alive: timeout=9999, max=9999");
    set_time_limit(0);

    try {
        $ws = new Spo_Ws_Sof_Qualitativo($params['log'] ? 'spo' : null);

        $obterProgramasPorOrgao = new ObterProgramasPorOrgao();
        $obterProgramasPorOrgao->exercicio = $exercicio;
        $obterProgramasPorOrgao->codigoOrgao = $orgao;
        $obterProgramasPorOrgao->codigoMomento = $momento;

        $return = $ws->obterProgramasPorOrgao($obterProgramasPorOrgao)->return;

        if ($return->mensagensErro){
            throw new Exception($return->mensagensErro);
        }

        $resultados = $return->registros;

        if ($resultados) {
            include_once  APPRAIZ . 'wssof/classes/Ws_ProgramasDto.inc';
            foreach ($resultados as $resultado) {
                $model = new Wssof_Ws_ProgramasDto();
                $model->realizarCarga($resultado);
                $model->commit();
                unset($model);
            }
        }

        return true;
    } catch (Exception $e) {
        $db->rollback();
        return false;
    }
}

/**
 * Obter Objetivos por Programa
 */
function obterObjetivosPorPrograma($exercicio, $momento) {
    global $db;

    header("Keep-Alive: timeout=9999, max=9999");
    set_time_limit(0);

    include_once  APPRAIZ . 'wssof/classes/Ws_ProgramasDto.inc';
    $modelProgramasDto = new Wssof_Ws_ProgramasDto();

    $programas = $modelProgramasDto->recuperarTodos('codigoprograma', ["exercicio = '$exercicio'", "codigomomento = '$momento'"]);

    try {
        $ws = new Spo_Ws_Sof_Qualitativo($params['log'] ? 'spo' : null);

        if(count($programas)){
            $obterObjetivosPorPrograma = new ObterObjetivosPorPrograma();
            $obterObjetivosPorPrograma->exercicio = $exercicio;
            $obterObjetivosPorPrograma->codigoMomento = $momento;
            foreach($programas as $programa){
                $obterObjetivosPorPrograma->codigoPrograma = $programa['codigoprograma'];

                $return = $ws->obterObjetivosPorPrograma($obterObjetivosPorPrograma)->return;

                if ($return->mensagensErro){
                    throw new Exception($return->mensagensErro);
                }

                $resultados = $return->registros;

                if ($resultados) {
                    include_once  APPRAIZ . 'wssof/classes/Ws_ObjetivosDto.inc';
                    foreach ($resultados as $resultado) {
                        $model = new Wssof_Ws_ObjetivosDto();
                        $model->realizarCarga($resultado);
                        $model->commit();
                        unset($model);
                    }
                }
            }
        }
        return true;
    } catch (Exception $e) {
        $db->rollback();
        return false;
    }
}

/**
 * Obter Objetivos por Programa
 */
function obterIniciativasPorObjetivo($exercicio, $momento) {
    global $db;

    header("Keep-Alive: timeout=9999, max=9999");
    set_time_limit(0);

    include_once  APPRAIZ . 'wssof/classes/Ws_ObjetivosDto.inc';
    $modelObjetivosDto = new Wssof_Ws_ObjetivosDto();

    $objetivos = $modelObjetivosDto->recuperarTodos('codigoobjetivo, codigoprograma', ["exercicio = '$exercicio'", "codigomomento = '$momento'"]);
    
    try {
        $ws = new Spo_Ws_Sof_Qualitativo($params['log'] ? 'spo' : null);

        if(count($objetivos)){
            $obterIniciativasPorObjetivo = new ObterIniciativasPorObjetivo();
            $obterIniciativasPorObjetivo->exercicio = $exercicio;
            $obterIniciativasPorObjetivo->codigoMomento = $momento;
            foreach($objetivos as $objetivo){
                $obterIniciativasPorObjetivo->codigoPrograma = $objetivo['codigoprograma'];
                $obterIniciativasPorObjetivo->codigoObjetivo = $objetivo['codigoobjetivo'];

                $return = $ws->obterIniciativasPorObjetivo($obterIniciativasPorObjetivo)->return;

                if ($return->mensagensErro){
                    throw new Exception($return->mensagensErro);
                }

                $resultados = $return->registros;

                if ($resultados) {
                    include_once  APPRAIZ . 'wssof/classes/Ws_IniciativasDto.inc';
                    foreach ($resultados as $resultado) {
                        $model = new Wssof_Ws_IniciativasDto();
                        $model->realizarCarga($resultado);
                        $model->commit();
                        unset($model);
                    }
                }
            }
        }
        return true;
    } catch (Exception $e) {
        $db->rollback();
        return false;
    }
}

/**
 * Obter Metas por Objetivo
 */
function obterMetasPorObjetivo($exercicio, $momento) {
    global $db;

    header("Keep-Alive: timeout=9999, max=9999");
    set_time_limit(0);

    include_once  APPRAIZ . 'wssof/classes/Ws_ObjetivosDto.inc';
    $modelObjetivosDto = new Wssof_Ws_ObjetivosDto();

    $objetivos = $modelObjetivosDto->recuperarTodos('codigoobjetivo, codigoprograma', ["exercicio = '$exercicio'", "codigomomento = '$momento'"]);

    try {
        $ws = new Spo_Ws_Sof_Qualitativo($params['log'] ? 'spo' : null);

        if(count($objetivos)){
            $obterMetasPorObjetivo = new ObterMetasPorObjetivo();
            $obterMetasPorObjetivo->exercicio = $exercicio;
            $obterMetasPorObjetivo->codigoMomento = $momento;
            foreach($objetivos as $objetivo){

                $obterMetasPorObjetivo->codigoPrograma = $objetivo['codigoprograma'];
                $obterMetasPorObjetivo->codigoObjetivo = $objetivo['codigoobjetivo'];

//ver($return, $obterMetasPorObjetivo, d);
                $return = $ws->obterMetasPorObjetivo($obterMetasPorObjetivo)->return;

                if ($return->mensagensErro){
                    throw new Exception($return->mensagensErro);
                }

                $resultados = $return->registros;

                if ($resultados) {
                    include_once  APPRAIZ . 'wssof/classes/Ws_MetasDto.inc';
                    foreach ($resultados as $resultado) {
                        $model = new Wssof_Ws_MetasDto();
                        $model->realizarCarga($resultado);
                        $model->commit();
                        unset($model);
                    }
                }
            }
        }
        return true;
    } catch (Exception $e) {
        $db->rollback();
        return false;
    }
}

/**
 * Consulta os ações em um exercicio de determinada unidade.
 *
 * @global cls_banco $db
 * @param int $exercicio Exercicio
 * @param int $momento Momento
 * @param bool $asJSON Indica se retorna um array ou uma string JSON.
 * @return array|string
 */
function carregarAcoes($exercicio, $unidadeOrcamentaria, $asJSON) {
    global $db;

    $params = array($exercicio);

    $sql = <<<DML
SELECT   DISTINCT(a.identificadorunico) AS codigo,
         a.identificadorunico || ' - ' || a.titulo AS descricao
  FROM   wssof.ws_acoesdto a
 WHERE	 a.exercicio = '%d'
DML;

    if (!is_null($unidadeOrcamentaria)) {
        $sql .= <<<DML
    	AND a.codigoorgao = '%d'
DML;
        $params[] = $unidadeOrcamentaria;
    }
    $sql .= <<<DML
ORDER BY 2
DML;

    $stmt = vsprintf($sql, $params);
    $data = $db->carregar($stmt);
    if (!$data) {
        $data = array();
    }
    // Se for retornar como JSON, faz o encode da descrição do tipo de crédito
    if ($asJSON) {
        foreach ($data as &$_data) {
            $_data['descricao'] = utf8_encode($_data['descricao']);
        }
        return simec_json_encode($data);
    }
    return $data;
}

/**
 * Consulta os localizadores em um exercicio em determinado momento.
 *
 * @global cls_banco $db
 * @param int $exercicio Exercicio
 * @param int $momento Momento
 * @param bool $asJSON Indica se retorna um array ou uma string JSON.
 * @return array|string
 */
function carregarLocalizadores($exercicio, $acao, $asJSON) {
    global $db;

    $params = array($exercicio);

    $sql = <<<DML
SELECT   DISTINCT(m.codigolocalizador) AS codigo,
         m.codigolocalizador || ' - ' || m.descricao  AS descricao
  FROM   wssof.ws_localizadoresdto m
 WHERE	 m.exercicio = '%d'
DML;

    if (!is_null($momento)) {
        $sql .= <<<DML
    	AND m.identificadorunicoacao = %d
DML;
        $params[] = $momento;
    }

    $sql .= <<<DML
ORDER BY 2
DML;

    $stmt = vsprintf($sql, $params);
    $data = $db->carregar($stmt);
    if (!$data) {
        $data = array();
    }
    // Se for retornar como JSON, faz o encode da descrição do tipo de crédito
    if ($asJSON) {
        foreach ($data as &$_data) {
            $_data['descricao'] = utf8_encode($_data['descricao']);
        }
        return simec_json_encode($data);
    }
    return $data;
}

function recuperarAcao($id) {
    global $db;

    $ws = new Spo_Ws_Sof_Quantitativo($params['log'] ? 'spo' : null);

    $sql = <<<DML
SELECT   *
  FROM   wssof.ws_acoesdto a
 WHERE	 a.identificadorunico = '%d'
DML;

    $params[] = $id;

    $stmt = vsprintf($sql, $params);

    return $db->recuperar($stmt);
}

function recuperarLocalizador($id) {
    global $db;

    $ws = new Spo_Ws_Sof_Quantitativo($params['log'] ? 'spo' : null);

    $sql = <<<DML
SELECT   *
  FROM   wssof.ws_localizadoresdto l
 WHERE	 l.codigolocalizador = '%d'
DML;

    $params[] = $id;

    $stmt = vsprintf($sql, $params);

    return $db->recuperar($stmt);
}

/**
 * Obter ações disponiveis para acompanhamento orçamentario.
 *
 * @global cls_banco $db
 * @param array $params $_POST
 * @return array|string
 */
function obterAcoesAcompanhamentoOrcamentario($params, $fm) {
    global $db;

    $ws = new Spo_Ws_Sof_Quantitativo($params['log'] ? 'spo' : null);

    $acao = recuperarAcao($params['acoes']);

    $localizador = recuperarLocalizador($params['localizador']);

    $filtro = new ObterAcoesDisponiveisAcompanhamentoOrcamentario();
    $filtro->exercicio = $_SESSION['exercicio'];
    $filtro->periodo = $params['periodo'];

    $return = $ws->obterAcoesDisponiveisAcompanhamentoOrcamentario($filtro)->return;

    if ($return->mensagensErro) {
        foreach ((array) $return->mensagensErro as $message) {
            $fm->addMensagem($message, Simec_Helper_FlashMessage::ERRO);
        }

        return false;
    } else {
        $insert = null;
        $count = 0;

        foreach ($return->acoes->acao as $acao) {
            if (is_array($acao->localizadores->localizador)) {
                foreach ((array) $acao->localizadores->localizador as $localizador) {
                    $localizador->snAcompanhamentoOpcional = $localizador->snAcompanhamentoOpcional ? 't' : 'f';
                    $insert.= "insert into wssof.ws_acoesacompanhamentoorcamentariodto ";
                    $insert.= "(codigoacao, codigoprograma, codigofuncao, codigosubfuncao, codigoorgao, codigoesfera, localizadores, dataultimaatualizacao, snacompanhamentoopcional) values ";
                    $insert.= "('{$acao->codigoAcao}', '{$acao->codigoPrograma}', '{$acao->codigoFuncao}', '{$acao->codigoSubFuncao}', '{$acao->codigoOrgao}', '{$acao->codigoEsfera}', '{$localizador->codigoLocalizador}', '" . date('Y-m-d H:i:s') . "', '{$localizador->snAcompanhamentoOpcional}');";
                    $count++;
                }
            } else {
                if($localizador && $localizador->codigoLocalizador){
                    $localizador->snAcompanhamentoOpcional = $localizador->snAcompanhamentoOpcional ? 't' : 'f';
                    $insert.= "insert into wssof.ws_acoesacompanhamentoorcamentariodto ";
                    $insert.= "(codigoacao, codigoprograma, codigofuncao, codigosubfuncao, codigoorgao, codigoesfera, localizadores, dataultimaatualizacao, snacompanhamentoopcional) values ";
                    $insert.= "('{$acao->codigoAcao}', '{$acao->codigoPrograma}', '{$acao->codigoFuncao}', '{$acao->codigoSubFuncao}', '{$acao->codigoOrgao}', '{$acao->codigoEsfera}', '{$localizador->codigoLocalizador}', '" . date('Y-m-d H:i:s') . "', '{$localizador->snAcompanhamentoOpcional}');";
                    $count++;
                }
            }
        }

        $delete = "delete from wssof.ws_acoesacompanhamentoorcamentariodto";

        $db->executar($delete);
        $db->executar($insert);

        $mensagem.= "({$count}) Ações disponiveis para acompanhamento orçamentário importadas.";

        $fm->addMensagem($mensagem, Simec_Helper_FlashMessage::SUCESSO);

        return true;
    }
}

/**
 * Obter ações disponiveis para acompanhamento orçamentario.
 *
 * @global cls_banco $db
 * @param array $params $_POST
 * @return array|string
 */
function obterProgramacaoCompletaQuantitativo($params, $fm) {
    global $db;

    $ws = new Spo_Ws_Sof_Quantitativo($params['log'] ? 'spo' : null);
    $paginacao = new PaginacaoDTO();
    $paginacao->pagina = $params['pagina'];
    $paginacao->registrosPorPagina = 2000;

    $filtro = new ObterProgramacaoCompletaQuantitativo();
    $filtro->exercicio = $_SESSION['exercicio'];
    $filtro->codigoMomento = $params['codigomomento'];
    $filtro->dataHoraReferencia = date('Y-m-d H:i:s');
    $filtro->paginacao = $paginacao;

    $return = $ws->obterProgramacaoCompletaQuantitativo($filtro)->return;

    if ($params['pagina'] == 1) {
        $delete = "delete from wssof.ws_propostadto;";
        $delete.= "delete from wssof.ws_financeirodto;";
        $delete.= "delete from wssof.ws_metaplanoorcamentariodto;";
        $delete.= "delete from wssof.ws_acoesacompanhamentoorcamentariodto;";

        $db->executar($delete);
        $db->commit();
    }

    if ($return->mensagensErro) {
        $mensagens = array();
        foreach ((array) $return->mensagensErro as $message) {
            $fm->addMensagem($message, Simec_Helper_FlashMessage::ERRO);
            $mensagens[] = $message;
        }

        print simec_json_encode(array('terminate' => true, 'erros' => $mensagens));
        die;
    } else {
        $insert = null;

        $count = 0;

        if (count($return->proposta)) {
            foreach ($return->proposta as $proposta) {
                $insert.= "INSERT INTO wssof.ws_propostadto (";
                $insert.= "codigoacao, codigoesfera, codigofuncao, codigolocalizador, codigomomento,";
                $insert.= "codigoorgao, codigoprograma, codigosubfuncao, codigotipodetalhamento,";
                $insert.= "codigotipoinclusaoacao, codigotipoinclusaolocalizador, exercicio,";
                $insert.= "expansaofisicaconcedida, expansaofisicasolicitada, identificadorunicoacao,";
                $insert.= "justificativa, justificativaexpansaoconcedida, justificativaexpansaosolicitada,";
                $insert.= "quantidadefisico, snatual, valorfisico) VALUES (";
                $insert.= "'{$proposta->codigoAcao}', '{$proposta->codigoEsfera}', '{$proposta->codigoFuncao}', '{$proposta->codigoLocalizador}', '{$proposta->codigoMomento}', ";
                $insert.= "'{$proposta->codigoOrgao}', '{$proposta->codigoPrograma}', '{$proposta->codigoSubFuncao}', '{$proposta->codigoTipoDetalhamento}', ";
                $insert.= "'{$proposta->codigoTipoInclusaoAcao}', '{$proposta->codigoTipoInclusaoLocalizador}', '{$proposta->exercicio}', ";
                $insert.= "'{$proposta->expansaoFisicaConcedida}', '{$proposta->expansaoFisicaSolicitada}', '{$proposta->identificadorUnicoAcao}', ";
                $insert.= "'{$proposta->justificativa}', '{$proposta->justificativaExpansaoConcedida}', '{$proposta->justificativaExpansaoSolicitada}', ";
                $insert.= "'{$proposta->quantidadeFisico}', '{$proposta->snAtual}', '{$proposta->valorFisico}');";

                if (is_array($return->proposta->financeiros)) {
                    foreach ($return->proposta->financeiros as $financeiro) {
                        $insert.= "INSERT INTO wssof.ws_financeirodto (";
                        $insert.= "codigoplanoorcamentario, expansaoconcedida, expansaosolicitada,";
                        $insert.= "fonte, idoc, iduso, identificadorplanoorcamentario, naturezadespesa,";
                        $insert.= "resultadoprimarioatual, resultadoprimariolei, valor) VALUES (";
                        $insert.= "'{$financeiro->codigoPlanoOrcamentario}', '{$financeiro->expansaoConcedida}', '{$financeiro->expansaoSolicitada}', ";
                        $insert.= "'{$financeiro->fonte}', '{$financeiro->idOC}', '{$financeiro->idUso}', '{$financeiro->identificadorPlanoOrcamentario}', '{$financeiro->naturezaDespesa}', ";
                        $insert.= "'{$financeiro->resultadoPrimarioAtual}', '{$financeiro->resultadoPrimarioLei}', '{$financeiro->valor}');";
                    }
                }

                if (is_array($return->proposta->metaPlanoOrcamentario)) {
                    foreach ($return->proposta->metaPlanoOrcamentario as $metaPlanoOrcamentario) {
                        $insert.= "INSERT INTO wssof.ws_metaplanoorcamentariodto ";
                        $insert.= "(expansaofisicaconcedida, expansaofisicasolicitada, identificadorunicoplanoorcamentario, quantidadefisico) VALUES ";
                        $insert.= "('{$metaPlanoOrcamentario->expansaoFisicaConcedida}', '{$metaPlanoOrcamentario->expansaoFisicaSolicitada}', '{$metaPlanoOrcamentario->identificadorUnicoPlanoOrcamentario}', '{$metaPlanoOrcamentario->quantidadeFisico}');";
                    }
                }

                if (is_array($return->proposta->receitas)) {
                    foreach ($return->proposta->receitas as $receita) {
                        $insert.= "INSERT INTO wssof.ws_receitadto (naturezareceita, valor) VALUES ('{$receita->naturezaReceita}', '{$receita->valor}');";
                    }
                }

                $count++;
            }

            $db->executar($insert);
            $db->commit();

            print simec_json_encode(array('terminate' => false, 'pagina' => $params['pagina']));
            die;
        } else {
            print simec_json_encode(array('terminate' => true));
            die;
        }
    }
}

/**
 * Consultar proposta completa.
 *
 * @global cls_banco $db
 * @param array $params $_POST
 * @return array|string
 */
function consultarProposta($params, $fm) {
    $ws = new Spo_Ws_Sof_Quantitativo($params['log'] ? 'spo' : null);

    $acao = recuperarAcao($params['acoes']);

    $localizador = recuperarLocalizador($params['localizador']);

    $proposta = new PropostaDTO();
    $proposta->exercicio = $_SESSION['exercicio'];
    $proposta->codigoLocalizador = $params['localizador'];
    $proposta->codigoMomento = $acao['codigomomento'];
    $proposta->codigoAcao = $acao['codigoacao'];
    $proposta->codigoEsfera = $acao['codigoesfera'];
    $proposta->codigoOrgao = $acao['codigoorgao'];
    $proposta->codigoFuncao = $acao['codigofuncao'];
    $proposta->codigoSubFuncao = $acao['codigosubfuncao'];
    $proposta->codigoPrograma = $acao['codigoprograma'];
    $proposta->codigoAcao = $acao['codigoacao'];
    $proposta->codigoTipoInclusaoAcao = $acao['codigotipoinclusaoacao'];
    $proposta->codigoTipoDetalhamento = $acao['codigotipoinclusaoacao'];
    $proposta->codigoTipoInclusaoLocalizador = $localizador['codigotipoinclusao'];

    $consultarProposta = new ConsultarProposta();
    $consultarProposta->proposta = $proposta;

    $return = $ws->consultarProposta($consultarProposta)->return;
    $return->acao = (object) $acao;
    $return->localizador = (object) $localizador;

    if ($return->mensagensErro) {
        foreach ((array) $return->mensagensErro as $message) {
            $fm->addMensagem($message, Simec_Helper_FlashMessage::ERRO);
        }
    }

    return $return;
}

/**
 * Executar o processamento das Ações/Localizadores/PTRES/POs.
 * Os dados já devem estar todos carregados no SCHEMA wssof
 * @global cls_banco $db
 * @param array $params $_POST
 * @return array|string
 */
function executarCargaLoa($params, $fm) {
    global $db;

    $execute = true;
    foreach ($params['tabelas'] as $nome => $check) {
        if (!$check) {
            $fm->addMensagem("Tabela {$nome} não foi carregada", Simec_Helper_FlashMessage::ERRO);
            $execute = false;
        }
    }

    if ($execute) {
        /*
         * Insere de Ações
         */
        $sql = <<<DML
        INSERT INTO monitora.acao
            SELECT
                nextval( 'monitora.acao_acaid_seq'::regclass )                                 AS acaid                          ,
                aca.codigoacao                                                                 AS acacod                         ,
                lpad( loc.codigolocalizador::text , 4 , '0' )                                  AS saccod                         ,
                lpad( loc.codigolocalizador::text , 4 , '0' )                                  AS loccod                         ,
                CASE WHEN aca.codigoesfera <> '' THEN aca.codigoesfera::INTEGER ELSE NULL END  AS esfcod                         ,
                aca.codigoorgao                                                                AS unicod                         ,
                'U'                                                                            AS unitpocod                      ,
                NULL                                                                           AS tincod                         ,
                aca.codigofuncao                                                               AS funcod                         ,
                NULL                                                                           AS unmcod                         ,
                CASE WHEN TRIM( loc.uf ) <> '' THEN UPPER( TRIM( loc.uf ) ) ELSE NULL END      AS regcod                         ,
                1                                                                              AS taccod                         ,
                NULL                                                                           AS osicod                         ,
                CASE WHEN aca.codigoproduto = '' THEN NULL ELSE aca.codigoproduto::INTEGER END AS procod                         ,
                '{$_SESSION['exercicio']}'                                                     AS prgano                         ,
                aca.codigoprograma                                                             AS prgcod                         ,
                NULL                                                                           AS prgid                          ,
                NULL                                                                           AS acacodppa                      ,
                '00'                                                                           AS sitcodestagio                  ,
                '00'                                                                           AS sitcodandamento                ,
                '00'                                                                           AS sitcodcronograma               ,
                NULL                                                                           AS acapercexecucao                ,
                aca.titulo                                                                     AS acadsc                         ,
                loc.descricao                                                                  AS sacdsc                         ,
                NULL                                                                           AS acadsccomentarios              ,
                NULL                                                                           AS acanomecoordenador             ,
                NULL                                                                           AS acadscunresp                   ,
                NULL                                                                           AS acadscunexecutora              ,
                NULL                                                                           AS acasnmedireta                  ,
                NULL                                                                           AS acasnmedesc                    ,
                NULL                                                                           AS acasnmelincred                 ,
                TRUE                                                                           AS acasnmetanaocumulativa         ,
                NULL                                                                           AS acamesinicio                   ,
                NULL                                                                           AS acaanoinicio                   ,
                NULL                                                                           AS acamestermino                  ,
                NULL                                                                           AS acaanotermino                  ,
                NULL                                                                           AS acavlrrealateanoanterior       ,
                NULL                                                                           AS acadsccomentsituacao           ,
                FALSE                                                                          AS acadscsituacaoatual            ,
                NULL                                                                           AS acadscresultadosespobt         ,
                NULL                                                                           AS acamesprevisaoconclusao        ,
                NULL                                                                           AS acaanoprevisaoconclusao        ,
                NULL                                                                           AS acadsccomentexecfisica         ,
                NULL                                                                           AS acadsccomentexecfinanceira     ,
                NULL                                                                           AS acadsccomentexecfisicabgu      ,
                NULL                                                                           AS acadsccomentexecfinanceirabgu  ,
                FALSE                                                                          AS acasnrap                       ,
                NULL                                                                           AS acadsccomentexecucao           ,
                NULL                                                                           AS acadsccomentexecucaorap        ,
                NULL                                                                           AS acasnfiscalseguridade          ,
                NULL                                                                           AS acasninvestatais               ,
                NULL                                                                           AS acasnoutrasfontes              ,
                NULL                                                                           AS cod_referencia                 ,
                NULL                                                                           AS acadscproduto                  ,
                NULL                                                                           AS acafinalidade                  ,
                aca.descricao                                                                  AS acadescricao                   ,
                aca.baselegal                                                                  AS acabaselegal                   ,
                NULL                                                                           AS acarepercfinanceira            ,
                NULL                                                                           AS acasnpadronizada               ,
                NULL                                                                           AS acasnsetpadronizada            ,
                NULL                                                                           AS acasntransfobrigatoria         ,
                NULL                                                                           AS acasntransfvoluntaria          ,
                FALSE                                                                          AS acasntransfoutras              ,
                NULL                                                                           AS acasndespesaobrigatoria        ,
                NULL                                                                           AS acasnbloqueioprogramacao       ,
                aca.detalhamentoimplementacao                                                  AS acadetalhamento                ,
                NULL                                                                           AS acavlrcustototal               ,
                NULL                                                                           AS acavlrcustoateanoanterior      ,
                NULL                                                                           AS acaqtdprevistoanocorrente      ,
                NULL                                                                           AS acaordemprioridade             ,
                NULL                                                                           AS acaobs                         ,
                NULL                                                                           AS acacodsof                      ,
                NULL                                                                           AS acaqtdcustototal               ,
                NULL                                                                           AS acacodreferenciasof            ,
                NULL                                                                           AS acavlrrepercfinanceira         ,
                aca.codigosubfuncao                                                            AS sfucod                         ,
                'A'                                                                            AS acastatus                      ,
                NULL                                                                           AS acasnemenda                    ,
                FALSE                                                                          AS acasnestrategica               ,
                NULL                                                                           AS acaqtdateanoanterior           ,
                NULL                                                                           AS acavlrcustoprevistoanocorrente ,
                FALSE                                                                          AS acasnbgu                       ,
                '@@@'                                                                          AS acaptres                       ,
                CURRENT_DATE                                                                   AS acadataatualizacao             ,
                NULL                                                                           AS irpcod                         ,
                aca.codigotipoinclusaoacao                                                     AS acatipoinclusao                ,
                loc.codigotipoinclusao                                                         AS acatipoinclusaolocalizador     ,
                unm.descricao                                                                  AS unmdsc                         ,
                pro.descricao                                                                  AS prodsc                         ,
                '9000'                                                                         AS descricaomomento               ,
                aca.unidaderesponsavel                                                         AS unidaderesponsavel             ,
                NULL                                                                           AS tipoinclusao                   ,
                NULL                                                                           AS tipoacao                       ,
                NULL                                                                           AS inicioacao                     ,
                NULL                                                                           AS terminoacao                    ,
                aca.titulo                                                                     AS acatitulo                      ,
                aca.identificadorunico                                                         AS ididentificadorunicosiop       ,
                aca.codigounidademedida                                                        AS unmcodsof                      ,
                pro.codigoproduto                                                              AS procodsof                      ,
                aca.codigotipoacao                                                             AS acatipocod                     ,
                tac.descricao                                                                  AS acatipodsc                     ,
                aca.codigoiniciativa                                                           AS acainiciativacod               ,
                ini.titulo                                                                     AS acainiciativadsc               ,
                aca.codigoobjetivo                                                             AS acaobjetivocod                 ,
                obj.enunciado                                                                  AS acaobjetivodsc                 ,
                esf.descricao                                                                  AS esfdsc                         ,
                fun.descricao                                                                  AS fundsc                         ,
                sfun.descricao                                                                 AS sfundsc                        ,
                prg.titulo                                                                     AS prgdsc                         ,
                prg.codigotipoprograma                                                         AS prgtipo                        ,
                aca.codigotipoinclusaoacao                                                     AS codigotipoinclusao             ,
                loc.codigotipoinclusao                                                         AS codtipoinclusaolocalizador     ,
                prop.quantidadefisico::integer                                                 AS metalocalizador                ,
                prop.valorfisico::numeric                                                      AS financeirolocalizador
            FROM wssof.ws_acoesdto aca
            JOIN wssof.ws_localizadoresdto loc ON aca.identificadorunico = loc.identificadorunicoacao
            LEFT JOIN wssof.ws_produtosdto pro ON aca.codigoproduto = pro.codigoproduto::text
            LEFT JOIN wssof.ws_unidadesmedidadto unm ON aca.codigounidademedida = unm.codigounidademedida
            LEFT JOIN wssof.ws_tiposacaodto tac ON aca.codigotipoacao = tac.codigotipoacao
            LEFT JOIN wssof.ws_iniciativasdto ini ON aca.codigoiniciativa = ini.codigoiniciativa
            LEFT JOIN wssof.ws_objetivosdto obj ON aca.codigoobjetivo = obj.codigoobjetivo
            LEFT JOIN wssof.ws_programasdto prg ON prg.codigoprograma = aca.codigoprograma
            LEFT JOIN wssof.ws_esferasdto esf ON esf.codigoesfera = aca.codigoesfera
            LEFT JOIN wssof.ws_funcoesdto fun ON fun.codigofuncao = aca.codigofuncao
            LEFT JOIN wssof.ws_subfuncoesdto sfun ON sfun.codigosubfuncao = aca.codigosubfuncao
            LEFT JOIN wssof.ws_propostadto prop ON aca.codigoacao = prop.codigoacao
                AND aca.codigoorgao = prop.codigoorgao
                AND loc.codigolocalizador = prop.codigolocalizador
                AND aca.exercicio = prop.exercicio
                AND aca.codigomomento = prop.codigomomento
            WHERE aca.codigoorgao || '.' || aca.codigoacao || '.'|| lpad( loc.codigolocalizador , 4 , '0' ) NOT IN (
                SELECT unicod || '.' || acacod || '.'|| loccod
                FROM monitora.acao
                WHERE prgano = '{$_SESSION['exercicio']}');
DML;
#ver($sql,d);
        $db->executar($sql);

        /*
         * Faz o UPDATE nos dados de TODAS as ações
         */
        $sql = <<<DML
        UPDATE monitora.acao
	    SET esfcod = CASE WHEN TRIM( aca.codigoesfera ) <> '' THEN aca.codigoesfera::INTEGER ELSE NULL END                               ,
            funcod                     = aca.codigofuncao                                                                                    ,
            regcod                     = CASE WHEN TRIM( loc.uf ) <> ''            THEN UPPER( TRIM( loc.uf ) )    ELSE NULL END             ,
            procod                     = CASE WHEN TRIM( aca.codigoproduto ) <> '' THEN aca.codigoproduto::INTEGER ELSE NULL END             ,
            prgcod                     = aca.codigoprograma                                                                                  ,
            acadsc                     = aca.titulo                                                                                          ,
            sacdsc                     = loc.descricao                                                                                       ,
            acadescricao               = aca.descricao                                                                                       ,
            acabaselegal               = aca.baselegal                                                                                       ,
            acadetalhamento            = aca.detalhamentoimplementacao                                                                       ,
            sfucod                     = aca.codigosubfuncao                                                                                 ,
            acadataatualizacao         = CURRENT_DATE                                                                                        ,
            acatipoinclusao            = aca.codigotipoinclusaoacao                                                                          ,
            acatipoinclusaolocalizador = loc.codigotipoinclusao                                                                              ,
            unmdsc                     = unm.descricao                                                                                       ,
            prodsc                     = pro.descricao                                                                                       ,
            descricaomomento           = '{$params['momento']}'                                                                              ,
            unidaderesponsavel         = aca.unidaderesponsavel                                                                              ,
            acatitulo                  = aca.titulo                                                                                          ,
            ididentificadorunicosiop   = aca.identificadorunico                                                                              ,
            unmcodsof                  = aca.codigounidademedida                                                                             ,
            procodsof                  = pro.codigoproduto                                                                                   ,
            acatipocod                 = aca.codigotipoacao                                                                                  ,
            acatipodsc                 = tac.descricao                                                                                       ,
            acainiciativacod           = aca.codigoiniciativa                                                                                ,
            acainiciativadsc           = ini.titulo                                                                                          ,
            acaobjetivocod             = aca.codigoobjetivo                                                                                  ,
            acaobjetivodsc             = obj.enunciado                                                                                       ,
            prgdsc                     = prg.titulo                                                                                          ,
            esfdsc                     = esf.descricao                                                                                       ,
            fundsc                     = fun.descricao                                                                                       ,
            sfundsc                    = sfun.descricao                                                                                      ,
            codtipoinclusaoacao        = aca.codigotipoinclusaoacao                                                                          ,
            codtipoinclusaolocalizador = loc.codigotipoinclusao                                                                              ,
            metalocalizador            = prop.quantidadefisico::integer                                                                               ,
            financeirolocalizador      = prop.valorfisico::numeric
        FROM wssof.ws_acoesdto aca
        JOIN wssof.ws_localizadoresdto loc ON aca.identificadorunico = loc.identificadorunicoacao
        LEFT JOIN wssof.ws_produtosdto pro ON aca.codigoproduto = pro.codigoproduto::TEXT
        LEFT JOIN wssof.ws_unidadesmedidadto unm ON aca.codigounidademedida = unm.codigounidademedida
        LEFT JOIN wssof.ws_tiposacaodto tac ON aca.codigotipoacao = tac.codigotipoacao
        LEFT JOIN wssof.ws_iniciativasdto ini ON aca.codigoiniciativa = ini.codigoiniciativa
        LEFT JOIN wssof.ws_objetivosdto obj ON aca.codigoobjetivo = obj.codigoobjetivo
        LEFT JOIN wssof.ws_programasdto prg ON prg.codigoprograma = aca.codigoprograma
        LEFT JOIN wssof.ws_esferasdto esf ON esf.codigoesfera = aca.codigoesfera
        LEFT JOIN wssof.ws_funcoesdto fun ON fun.codigofuncao = aca.codigofuncao
        LEFT JOIN wssof.ws_subfuncoesdto sfun ON sfun.codigosubfuncao = aca.codigosubfuncao
        LEFT JOIN wssof.ws_propostadto prop ON aca.codigoacao = prop.codigoacao
            AND aca.codigoorgao = prop.codigoorgao
            AND loc.codigolocalizador = prop.codigolocalizador
        WHERE aca.codigoorgao || '.' || aca.codigoacao || '.'|| lpad( loc.codigolocalizador , 4 , '0' ) || '.'|| '{$_SESSION['exercicio']}' = monitora.acao.unicod || '.' || monitora.acao.acacod || '.' ||monitora.acao.loccod || '.' || monitora.acao.prgano;
DML;
        $db->executar($sql);
        /*
         * Processamento de PTRES
         */
        $sql = <<<DML
        INSERT INTO  monitora.ptres
            SELECT
                *
            FROM
                (SELECT
                    nextval( 'monitora.ptres_ptrid_seq'::regclass ) AS ptrid ,
                    ptres                                           AS ptres ,
                    (SELECT acaid FROM monitora.acao aca WHERE aca.unicod = sex.unicod
                        AND aca.acacod = sex.acacod
                        AND aca.loccod = sex.loccod
                        AND aca.prgano = sex.exercicio
                        AND aca.acastatus = 'A' LIMIT 1)  AS acaid      ,
                    exercicio                AS ptrano     ,
                    funcod                   AS funcod     ,
                    sfucod                   AS sfucod     ,
                    prgcod                   AS prgcod     ,
                    acacod                   AS acacod     ,
                    loccod                   AS loccod     ,
                    unicod                   AS unicod     ,
                    NULL                     AS irpcod     ,
                    SUM( vlrdotacaoinicial ) AS ptrdotacao ,
                    'A'                      AS ptrstatus  ,
                    NOW()                    AS ptrdata    ,
                    plocod                   AS plocod     ,
                    esfcod                   AS esfcod
                FROM spo.siopexecucao sex
                WHERE exercicio = '{$_SESSION['exercicio']}'
                    AND ptres <> ''
                    AND ptres NOT IN (SELECT ptres FROM monitora.ptres WHERE ptrano = '{$_SESSION['exercicio']}')
                    AND sex.anoreferencia = '{$_SESSION['exercicio']}'
                GROUP BY
                    ptres     ,
                    exercicio ,
                    funcod    ,
                    sfucod    ,
                    prgcod    ,
                    acacod    ,
                    loccod    ,
                    unicod    ,
                    plocod    ,
                    esfcod
                ) foo
            WHERE acaid IS NOT NULL
                AND ptres NOT IN (SELECT DISTINCT ptres FROM monitora.ptres WHERE ptrano = '{$_SESSION['exercicio']}')
DML;
        $db->executar($sql);

        $sql = <<<DML
            UPDATE monitora.ptres ptr
                SET ptrdotacao = (SELECT SUM(sex.vlrdotacaoatual) AS dotacao FROM spo.siopexecucao sex
                    WHERE exercicio = '{$_SESSION['exercicio']}'
                    AND ptr.unicod = sex.unicod
                    AND ptr.acacod = sex.acacod
                    AND ptr.loccod = sex.loccod
                    AND ptr.plocod = sex.plocod)
            WHERE ptrano = '{$_SESSION['exercicio']}';
DML;
        $db->executar($sql);
        /*
         * Processamento de POs
         */
        $sql = <<<DML
            INSERT INTO monitora.planoorcamentario
                SELECT
                    *
                FROM
                    (SELECT
                        nextval('monitora.planoorcamentario_ploid_seq'::regclass ) AS ploid                     ,
                        aca.codigoprograma                                          AS prgcod                    ,
                        aca.codigoacao                                              AS acacod                    ,
                        aca.codigoorgao                                             AS unicod                    ,
                        plo.planoorcamentario                                       AS plocodigo                 ,
                        plo.identificadorunico                                      AS ploidentificadorunicosiop ,
                        plo.titulo                                                  AS plotitulo                 ,
                        plo.detalhamento                                            AS plodetalhamento           ,
                        pro.codigoproduto                                           AS ploproduto                ,
                        plo.codigounidademedida                                     AS plounidademedida          ,
                        false                                                       AS ploobrigatorio            ,
                        'A'                                                         AS plostatus                 ,
                        (SELECT acaid FROM monitora.acao WHERE unicod = aca.codigoorgao
                            AND acacod = aca.codigoacao
                            AND loccod = loc.codigolocalizador
                            AND prgano::INTEGER = aca.exercicio
                            AND acastatus = 'A' LIMIT 1 )                           AS acaid                     ,
                        plo.exercicio               AS exercicio ,
                        pro.descricao               AS prddsc    ,
                        unm.descricao               AS unmdsc    ,
                        mpo.quantidadefisico::         INTEGER
                    FROM wssof.ws_planosorcamentariosdto plo
                    JOIN wssof.ws_acoesdto aca ON identificadorunicoacao = aca.identificadorunico
                    JOIN wssof.ws_localizadoresdto loc ON loc.identificadorunicoacao = aca.identificadorunico
                    LEFT JOIN wssof.ws_produtosdto pro ON plo.codigoproduto = pro.codigoproduto::text
                    LEFT JOIN wssof.ws_unidadesmedidadto unm ON plo.codigounidademedida = unm.codigounidademedida
                    LEFT JOIN wssof.ws_metaplanoorcamentariodto mpo ON plo.identificadorunico = mpo.identificadorunicoplanoorcamentario
                    WHERE aca.exercicio = {$_SESSION['exercicio']}
                        AND plo.exercicio = '{$_SESSION['exercicio']}') foo
                WHERE acaid||'.'||plocodigo NOT IN (SELECT acaid||'.'||plocodigo FROM monitora.planoorcamentario)
DML;
        $db->executar($sql);

        /*
         * Atulizar os dados do PO (IMPLEMENTAR)
         */
        $sql = <<<DML
            UPDATE monitora.planoorcamentario
                SET
                    plocodigo = foo.planoorcamentario,
                    ploidentificadorunicosiop = foo.identificadorunico,
                    plotitulo = foo.titulo,
                    plodetalhamento = foo.detalhamento,
                    ploproduto = foo.codigoproduto,
                    plounidademedida = foo.codigounidademedida,
                    prddsc = foo.prodsc,
                    unmdsc = foo.unmdsc,
                    metafisica = foo.quantidadefisico
                FROM
                    (SELECT
                        plo.planoorcamentario,
                        plo.identificadorunico,
                        plo.titulo,
                        plo.detalhamento,
                        pro.codigoproduto,
                        plo.codigounidademedida,
                        pro.descricao as prodsc,
                        unm.descricao as unmdsc,
                        mpo.quantidadefisico::INTEGER as quantidadefisico,
                        (SELECT acaid FROM monitora.acao WHERE unicod = aca.codigoorgao
                            AND acacod = aca.codigoacao
                            AND loccod = loc.codigolocalizador
                            AND prgano::INTEGER = aca.exercicio
                            AND acastatus = 'A' LIMIT 1 ) AS acaoid
                    FROM wssof.ws_planosorcamentariosdto plo
                    JOIN wssof.ws_acoesdto aca ON identificadorunicoacao = aca.identificadorunico
                    JOIN wssof.ws_localizadoresdto loc ON loc.identificadorunicoacao = aca.identificadorunico
                    LEFT JOIN wssof.ws_produtosdto pro ON plo.codigoproduto = pro.codigoproduto::text
                    LEFT JOIN wssof.ws_unidadesmedidadto unm ON plo.codigounidademedida = unm.codigounidademedida
                    LEFT JOIN wssof.ws_metaplanoorcamentariodto mpo ON plo.identificadorunico = mpo.identificadorunicoplanoorcamentario
                    WHERE aca.exercicio = {$_SESSION['exercicio']}
                        AND plo.exercicio = '{$_SESSION['exercicio']}'
                )foo
                WHERE acaid = foo.acaoid
                    AND exercicio = '{$_SESSION['exercicio']}'
                    AND plocodigo = foo.planoorcamentario;
DML;
        $db->executar($sql);
        if ($db->commit()) {
            $fm->addMensagem("Processamento de carga executado com sucesso.", Simec_Helper_FlashMessage::SUCESSO);
        } else {
            $fm->addMensagem("Erro ao processar a carga.", Simec_Helper_FlashMessage::ERRO);
        }
    }
}

function obterAcompanhamentoFisicoFinanceiro($params, $fm) {
    global $db;

    $ws = new Spo_Ws_Sof_Quantitativo($params['log'] ? 'spo' : null);

    $paginacao = new PaginacaoDTO();
    $paginacao->pagina = $params['pagina'];
    $paginacao->registrosPorPagina = 2000;

    $filtro = new ConsultarAcompanhamentoFisicoFinanceiro();
    $filtro->exercicio = $_SESSION['exercicio'];
    $filtro->periodo = $params['periodo'];
    $filtro->momentoId = $params['momento'];
    $filtro->tipoCaptacao = $params['tipoCaptacao'];
    $filtro->paginacao = $paginacao;

    $return = $ws->consultarAcompanhamentoFisicoFinanceiro($filtro)->return;

    if ($return->mensagensErro) {
        foreach ((array) $return->mensagensErro as $message) {
            $fm->addMensagem($message, Simec_Helper_FlashMessage::ERRO);
        }
        print simec_json_encode(array('terminate' => true, 'pagina' => $params['pagina']));
        die;
    }
    if ($params['pagina'] == 1) {
        $db->executar('DELETE FROM wssof.ws_acompanhamentofisicofinanceirodto;');
        $db->commit();
    }
    if (count($return->registros)) {
        $insert = null;
        foreach ($return->registros as $chave => $dado) {
            $insert .= <<<DML
                INSERT INTO wssof.ws_acompanhamentofisicofinanceirodto
                    (acao, codigopo, descricaofuncao, descricaolocalizador, descricaoorgao, descricaoproduto, descricaoprodutopo, descricaosiorg,
                    descricaosubfuncao, descricaounidademedida, descricaounidademedidapo, descricaouo, dotacaoatual, dotacaoatualpo, dotacaoinicial,
                    dotacaoinicialpo, esfera, exercicio, funcao, liquidado, liquidadopo, liquidadorap, liquidadorappo, localizador, momento, momentoid,
                    orgao, orgaosiorg, pago, pagopo, periodo, produto, produtopo, programa, quantidademetaatual, quantidademetaatualpo, quantidademetaloa,
                    quantidademetaloapo, realizadoloa, realizadopo, realizadorap, reprogramadofinanceiro, reprogramadofisico, subfuncao, tipocaptacao,
                    tituloacao, titulopo, tituloprograma, unidademedida, unidademedidapo, uo, dataultimaatualizacao)
                VALUES('{$$dado['acao']}','{$dado['codigoPO']}','{$dado['descricaoFuncao']}','{$dado['descricaoLocalizador']}','{$dado['descricaoOrgao']}',
                '{$dado['descricaoProduto']}','{$dado['descricaoProdutoPO']}','{$dado['descricaoSiorg']}','{$dado['descricaoSubFuncao']}','{$dado['descricaoUnidadeMedida']}',
                '{$dado['descricaoUnidadeMedidaPO']}','{$dado['descricaoUo']}','{$dado['dotacaoAtual']}','{$dado['dotacaoAtualPO']}','{$dado['dotacaoInicial']}',
                '{$dado['dotacaoInicialPO']}','{$dado['esfera']}','{$dado['exercicio']}','{$dado['funcao']}','{$dado['liquidado']}','{$dado['liquidadoPO']}',
                '{$dado['liquidadoRAP']}','{$dado['liquidadoRAPPO']}','{$dado['localizador']}','{$dado['momento']}','{$dado['momentoId']}','{$dado['orgao']}',
                '{$dado['orgaoSiorg']}','{$dado['pago']}','{$dado['pagoPO']}','{$dado['periodo']}','{$dado['produto']}','{$dado['produtoPO']}','{$dado['programa']}',
                '{$dado['quantidadeMetaAtual']}','{$dado['quantidadeMetaAtualPO']}','{$dado['quantidadeMetaLOA']}','{$dado['quantidadeMetaLOAPO']}','{$dado['realizadoLOA']}',
                '{$dado['realizadoPO']}','{$dado['realizadoRAP']}','{$dado['reprogramadoFinanceiro']}','{$dado['reprogramadoFisico']}','{$dado['subfuncao']}',
                '{$dado['tipoCaptacao']}','{$dado['tituloAcao']}','{$dado['tituloPO']}','{$dado['tituloPrograma']}','{$dado['unidadeMedida']}',
                '{$dado['unidadeMedidaPO']}','{$dado['uo']}', 'now()');
DML;
        }
        $db->executar($insert);
        $db->commit();
        print simec_json_encode(array('terminate' => false, 'pagina' => $params['pagina']));
    } else {
        if ($params['pagina'] == 1) {
            $fm->addMensagem('Não foram retornados dados para esta carga.', Simec_Helper_FlashMessage::AVISO);
        }
        print simec_json_encode(array('terminate' => true, 'pagina' => $params['pagina']));
    }
    die;
}

function obterInformacaoCaptacaoPLOA($dados, $fm)
{
    global $db;

    $ws = new Spo_Ws_Sof_Quantitativo(isset($dados['loggar'])?'proporc':null);

    // -- Preparando dados da requisicao
    $param = new ParametroInformacaoCaptacaoPLOA();
    $param->captados = $dados['captados'];
    $param->captaveis = $dados['captaveis'];
    $param->codigoMomento = $dados['codigomomento'];
    $param->codigoOrgao = $dados['codigoOrgao'];
    if (!empty($dados['codigoTipoDetalhamento'])) {
        $param->codigoTipoDetalhamento = $dados['codigoTipoDetalhamento'];
    }
    if (!empty($dados['codigounidadeorcamentaria'])) {
        $param->codigoUnidadeOrcamentaria = $dados['codigounidadeorcamentaria'];
    }
    $param->exercicio = $dados['exercicio'];

    $obterInfoCaptacao = new ObterInformacaoCaptacaoPLOA();
    $obterInfoCaptacao->parametro = $param;

    // -- Executando a requisição
    $return = $ws->obterInformacaoCaptacaoPLOA($obterInfoCaptacao)->return;

    // -- Verificação de erros
    if ($return->mensagensErro) {
        foreach ((array)$return->mensagensErro as $message) {
            $fm->addMensagem($message, Simec_Helper_FlashMessage::ERRO);
        }
        return;
    }

    // -- Processando o retorno
    if (isset($dados['limpar'])) {
        $sql = <<<DML
DELETE FROM wssof.ws_informacaocaptacaoploadto;
DML;
    } else {
        $sql = '';
    }

    if (empty($return->registros)) {
        $fm->addMensagem('Nenhum registro retornado pelo webservice.', Simec_Helper_FlashMessage::ERRO);
        return false;
    }

    foreach ($return->registros->registro as $registro) {

        $insert = <<<DML
INSERT INTO wssof.ws_informacaocaptacaoploadto(
    exercicio,
    identificadorunicolocalizador,
    codigomomentolocalizador,
    identificadorunicoacao,
    codigomomentoacao,
    funcional,
    temproposta,
    codigomomentopropostaatual,
    propostavalida,
    codigotipodetalhamento,
    temjanela,
    codigomomentojanelaatual,
    podecaptar,
    porquenaopodecaptar
) VALUES (
    %d,
    %d,
    %d,
    %d,
    %d,
    '%s',
    '%s',
    %d,
    '%s',
    '%s',
    '%s',
    %d,
    '%s',
    '%s'
);
DML;
        $sql .= sprintf(
            $insert,
            $registro->exercicio,
            $registro->identificadorUnicoLocalizador,
            $registro->codigoMomentoLocalizador,
            $registro->identificadorUnicoAcao,
            $registro->codigoMomentoAcao,
            $registro->funcional,
            $registro->temProposta?'t':'f',
            $registro->codigoMomentoPropostaAtual,
            $registro->propostaValida?'t':'f',
            $registro->codigoTipoDetalhamento,
            $registro->temJanela?'t':'f',
            $registro->codigoMomentoJanelaAtual,
            $registro->podeCaptar?'t':'f',
            $registro->porQueNaoPodeCaptar
        );
    }

    $db->executar($sql);
    $db->commit();

    $fm->addMensagem('Registros importados com sucesso.');
}

/**
 * Verifica se a requisição é ajax.
 * Is request xmlHttpRequest
 * @return bool
 */
function isAjax() {
    return (!empty($_SERVER['HTTP_X_REQUESTED_WITH']) &&
            strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest');
}

/**
 * Executar o processamento das Ações/Localizadores/POs para a PLOA.
 * Os dados já devem estar todos carregados no SCHEMA wssof
 * @global cls_banco $db
 * @param array $params $_POST
 * @return array|string
 */
function executarCargaPloa($params, $fm) {
    global $db;
    $anoPloa = $_SESSION['exercicio'] + 1;
    $momento = $params['momento'];
    $execute = true;

    if ($execute) {
        /*
         * Insere de Ações com Localizador
         */
        $sql = <<<DML
/** Carregando os programas do exercício selecionado */
INSERT INTO elabrev.ppaprograma_orcamento(
    prgcod,
    prgano,
    prgdsc,
    prgdscpublicoalvo,
    prgdscjustificativa,
    prgdscestrategia,
    prgstatus)
SELECT codigoprograma AS prgcod,
       '{$_SESSION['exercicio']}' AS prgano,
       titulo AS prgdsc,
       publicoalvo AS prgdscpublicoalvo,
       justificativa AS prgdscjustificativa,
       estrategiaimplementacao AS prgdscestrategia,
       'A' AS prgstatus
  FROM wssof.ws_programasdto
  WHERE exercicio = '{$anoPloa}';

/** Carregando as ações do exercício selecionado */
INSERT
INTO
    elabrev.ppaacao_orcamento
    (
        acacod,
        saccod,
        loccod,
        esfcod,
        unicod,
        unitpocod,
        funcod,
        prgano,
        prgcod,
        acadsc,
        sacdsc,
        acadescricao,
        acabaselegal,
        sfucod ,
        acastatus,
        acapropostaenviada,
        acaidentificadorsiop,
        proddesc,
        acadscprosof,
        acadscunmsof
    )
SELECT
    aca.codigoacao                                AS acacod ,
    lpad( loc.codigolocalizador::text , 4 , '0' ) AS saccod ,
    lpad( loc.codigolocalizador::text , 4 , '0' ) AS loccod ,
    CASE
        WHEN aca.codigoesfera <> ''
        THEN aca.codigoesfera::INTEGER
        ELSE NULL
    END                    AS esfcod ,
    aca.codigoorgao        AS unicod ,
    'U'                    AS unitpocod ,
    aca.codigofuncao       AS funcod ,
    '{$_SESSION['exercicio']}' AS prgano,
    aca.codigoprograma     AS prgcod,
    aca.titulo             AS acadsc ,
    loc.descricao          AS sacdsc ,
    aca.descricao          AS acadescricao ,
    aca.baselegal          AS acabaselegal ,
    aca.codigosubfuncao    AS sfucod ,
    'A'                    AS acastatus ,
    false                  AS acapropostaenviada ,
    aca.identificadorunico AS acaidentificadorsiop ,
    pro.descricao      AS proddesc,
    pro.codigoproduto as acadscprosof,
    unm.descricao as acadscunmsof
FROM
    wssof.ws_acoesdto aca
JOIN
    wssof.ws_localizadoresdto loc
ON
    aca.identificadorunico = loc.identificadorunicoacao
LEFT JOIN
    wssof.ws_produtosdto pro
ON
    aca.codigoproduto = pro.codigoproduto::text
LEFT JOIN
    wssof.ws_programasdto prg
ON
    prg.codigoprograma = aca.codigoprograma
LEFT JOIN
    wssof.ws_esferasdto esf
ON
    esf.codigoesfera = aca.codigoesfera
LEFT JOIN
    wssof.ws_funcoesdto fun
ON
    fun.codigofuncao = aca.codigofuncao
LEFT JOIN
    wssof.ws_subfuncoesdto sfun
ON
    sfun.codigosubfuncao = aca.codigosubfuncao
LEFT JOIN
    wssof.ws_unidadesmedidadto unm
ON
    aca.codigounidademedida = unm.codigounidademedida
WHERE
    aca.exercicio ='{$anoPloa}'
AND aca.codigomomento = '$momento'
AND aca.codigoacao||'.'|| loc.codigolocalizador ||'.'||aca.codigoorgao NOT IN
    (
        SELECT
            acacod||'.'|| loccod ||'.'||unicod
        FROM
            elabrev.ppaacao_orcamento ao
        WHERE
            ao.prgano = '{$_SESSION['exercicio']}');

/** Atualizando o PRGID das ações importadas */
UPDATE elabrev.ppaacao_orcamento
  SET prgid = (SELECT prgid
                 FROM elabrev.ppaprograma_orcamento ppo
                 WHERE ppo.prgcod = ppaacao_orcamento.prgcod
                   AND ppo.prgano = ppaacao_orcamento.prgano
                   AND ppo.prgstatus = 'A' LIMIT 1)
  WHERE prgano = '{$_SESSION['exercicio']}';

/** Carregando as ações do exercício selecionado */
INSERT INTO elabrev.planoorcamentario
    (
    acaid,
    plocodigo,
    ploidentificadorunicosiop,
    plotitulo,
    plodetalhamento,
    ploproduto,
    plounidademedida,
    plostatus
    )
SELECT
    pao.acaid,
    wplo.planoorcamentario,
    wplo.identificadorunico,
    wplo.titulo,
    wplo.detalhamento,
    (
        SELECT
            wprd.descricao
        FROM
            wssof.ws_produtosdto wprd
        WHERE
            wplo.codigoproduto IS NOT NULL
        AND wplo.codigoproduto <> ''
        AND wprd.codigoproduto = wplo.codigoproduto::INTEGER),
    (
        SELECT
            wunm.descricao
        FROM
            wssof.ws_unidadesmedidadto wunm
        WHERE
            wunm.codigounidademedida = wplo.codigounidademedida),
    'A'
FROM
    wssof.ws_planosorcamentariosdto wplo
INNER JOIN
    elabrev.ppaacao_orcamento pao
ON
    (
        pao.acaidentificadorsiop = wplo.identificadorunicoacao)
WHERE
    pao.prgano = '{$_SESSION['exercicio']}'
AND wplo.codigomomento = '$momento'
AND pao.acaid || '.' || wplo.planoorcamentario NOT IN
    (
        SELECT
            acaid || '.' || plocodigo
        FROM
            elabrev.planoorcamentario
        INNER JOIN
            elabrev.ppaacao_orcamento
        USING
            (acaid) );

   UPDATE elabrev.ppaacao_orcamento
   SET prgid = (SELECT ppo.prgid
                 FROM elabrev.ppaprograma_orcamento ppo
                 WHERE ppo.prgano = ppaacao_orcamento.prgano
                   AND ppo.prgcod = ppaacao_orcamento.prgcod
                   AND ppo.prgstatus = 'A' LIMIT 1)
   WHERE prgano = '{$_SESSION['exercicio']}';

DML;

        $db->executar($sql);
        if ($db->commit()) {
            $fm->addMensagem("Processamento de carga executado com sucesso.", Simec_Helper_FlashMessage::SUCESSO);
        } else {
            $fm->addMensagem("Erro ao processar a carga.", Simec_Helper_FlashMessage::ERRO);
        }
    }
}

/*
 * Limpar toda a carga da PLOA
 */

function limparCargaPloa($params,$fm) {
    global $db;
    $anoApagar = $params['anoapagar'];

    $periodoApagar = $db->pegaUm("SELECT
                                    prfid
                                FROM
                                    proporc.periodoreferencia
                                WHERE
                                    prsano ='{$anoApagar}'
                                ORDER BY
                                    prfid DESC limit 1");

    $sql = <<<DML

        BEGIN
    ;
DELETE
FROM
    proporc.ploafinanceiro
WHERE
    dpaid IN
    (
        SELECT
            dpaid
        FROM
            elabrev.despesaacao
        WHERE
            acaid IN
            (
                SELECT
                    acaid
                FROM
                    elabrev.ppaacao_orcamento
                WHERE
                    prgano = '{$anoApagar}') );
DELETE
FROM
    elabrev.despesaacao
WHERE
    acaid IN
    (
        SELECT
            acaid
        FROM
            elabrev.ppaacao_orcamento
        WHERE
            prgano = '{$anoApagar}');
DELETE
FROM
    elabrev.planoorcamentario
WHERE
    acaid IN
    (
        SELECT
            acaid
        FROM
            elabrev.ppaacao_orcamento
        WHERE
            prgano = '{$anoApagar}');
DELETE
FROM
    elabrev.ppaacao_orcamento
WHERE
    prgano = '{$anoApagar}';
DELETE
FROM
    elabrev.ppaprograma_orcamento
WHERE
    prgano = '{$anoApagar}';
DELETE
FROM
    elabrev.planoorcamentario
WHERE
    acaid IN
    (
        SELECT
            acaid
        FROM
            elabrev.ppaacao_orcamento
        WHERE
            prgano = '{$anoApagar}' );
DELETE
FROM
    elabrev.ppaacao_orcamento
WHERE
    prgano = '{$anoApagar}';
DELETE
FROM
    proporc.despesaacao
WHERE
    dspid IN
    (
        SELECT
            dspid
        FROM
            proporc.despesa
        WHERE
            gdpid IN
            (
                SELECT
                    gdpid
                FROM
                    proporc.grupodespesa
                WHERE
                    prfid= {$periodoApagar}) );
DELETE
FROM
    proporc.despesafonterecurso
WHERE
    dspid IN
    (
        SELECT
            dspid
        FROM
            proporc.despesa
        WHERE
            gdpid IN
            (
                SELECT
                    gdpid
                FROM
                    proporc.grupodespesa
                WHERE
                    prfid= {$periodoApagar}) );
DELETE
FROM
    proporc.despesagnd
WHERE
    dspid IN
    (
        SELECT
            dspid
        FROM
            proporc.despesa
        WHERE
            gdpid IN
            (
                SELECT
                    gdpid
                FROM
                    proporc.grupodespesa
                WHERE
                    prfid= {$periodoApagar}) );
DELETE
FROM
    proporc.despesaplanoorcamentario
WHERE
    dspid IN
    (
        SELECT
            dspid
        FROM
            proporc.despesa
        WHERE
            gdpid IN
            (
                SELECT
                    gdpid
                FROM
                    proporc.grupodespesa
                WHERE
                    prfid= {$periodoApagar}) );
DELETE
FROM
   proporc.despesasubacao
WHERE
    dspid IN
    (
        SELECT
            dspid
        FROM
            proporc.despesa
        WHERE
            gdpid IN
            (
                SELECT
                    gdpid
                FROM
                    proporc.grupodespesa
                WHERE
                    prfid= {$periodoApagar}) );
DELETE
FROM
    proporc.despesaunidadeorcamentaria
WHERE
    dspid IN
    (
        SELECT
            dspid
        FROM
            proporc.despesa
        WHERE
            gdpid IN
            (
                SELECT
                    gdpid
                FROM
                    proporc.grupodespesa
                WHERE
                    prfid= {$periodoApagar}) );
DELETE
FROM
    proporc.limitesfonteunidadeorcamentaria
WHERE
    dspid IN
    (
        SELECT
            dspid
        FROM
            proporc.despesa
        WHERE
            gdpid IN
            (
                SELECT
                    gdpid
                FROM
                    proporc.grupodespesa
                WHERE
                    prfid= {$periodoApagar}) );
DELETE
FROM
    proporc.despesa
WHERE
    gdpid IN
    (
        SELECT
            gdpid
        FROM
            proporc.grupodespesa
        WHERE
            prfid= {$periodoApagar});
DELETE
FROM
    proporc.grupodespesa
WHERE
    prfid= {$periodoApagar};
COMMIT;

DML;
    #ver($sql,d);
    $db->executar($sql);
    if ($db->commit()) {
        $fm->addMensagem("Carga apagada com sucesso.", Simec_Helper_FlashMessage::SUCESSO);
    } else {
        $fm->addMensagem("Erro ao apagar a carga.", Simec_Helper_FlashMessage::ERRO);
    }
}

function executarCargaAlteracaoOrcamentaria($params, $fm)
{
    global $db;

    $mcrid = $params['periodo'];

    // -- query de localizadores - comum a todas consultas
    $sqlLocalizadores = <<<HTML
     localizadores AS (SELECT loc.codigolocalizador,
                              loc.descricao,
                              MAX(loc.codigotipoinclusao) AS codigotipoinclusao,
                              loc.identificadorunicoacao
                         FROM wssof.ws_localizadoresdto loc
                         GROUP BY loc.codigolocalizador,
                                  loc.descricao,
                                  loc.identificadorunicoacao)
HTML;

    // -- Importação do questionario do momento anterior
    $sqlImportacao = <<<DML
INSERT INTO altorc.perguntasmomento(pgmcod, pgmdsc, mcrid, pgmsomenteleitura, pgmexplicacaopergunta)
SELECT DISTINCT pgm.pgmcod, pgm.pgmdsc, {$mcrid}, pgm.pgmsomenteleitura, pgm.pgmexplicacaopergunta
  FROM altorc.perguntasmomento pgm
  WHERE pgm.mcrid = (SELECT MAX(pgm2.mcrid)
                       FROM altorc.perguntasmomento pgm2
                       WHERE pgm2.mcrid != {$mcrid})
    AND NOT EXISTS (SELECT 1
                      FROM altorc.perguntasmomento pgm3
                      WHERE pgm.pgmcod = pgm3.pgmcod
                        AND pgm3.mcrid = {$mcrid});


DML;

    // -- Tabelas utilizadas tanto na importação de novos registros qto na atualização de registros existentes
    $tabelasImportacaoAcao = <<<DML
WITH propostas AS (SELECT prp.identificadorunicoacao,
                          SUM(prp.quantidadefisico::integer) AS quantidadeproduto
                     FROM wssof.ws_propostadto prp
                     GROUP BY prp.identificadorunicoacao),
{$sqlLocalizadores},
     dados AS (SELECT '{$_SESSION['exercicio']}' AS exercicio,
                      aca.codigoesfera,
                      aca.codigoorgao, -- unidade orcamentaria
                      aca.codigofuncao,
                      aca.codigosubfuncao,
                      aca.codigoprograma,
                      aca.codigoacao,
                      loc.codigolocalizador,
                      loc.codigotipoinclusao,
                      aca.codigotipoinclusaoacao,
                      0.00 AS valacrescimo,
                      0.00 AS valreducao,
                      aca.titulo AS acaodescricao,
                      prg.titulo AS programadescricao,
                      loc.descricao AS localizadordescricao,
                      aca.codigoproduto,
                      COALESCE(pro.descricao, '-') AS produtodescricao,
                      CASE WHEN '' = aca.codigounidademedida THEN '-'
                             ELSE COALESCE(aca.codigounidademedida, '-')
                        END AS codigounidademedida,
                      COALESCE(unm.descricao, '-') AS unidademedidadescricao,
                      prp.quantidadeproduto,
                      {$mcrid} AS mcrid,
                      aca.identificadorunico
                 FROM wssof.ws_acoesdto aca
                   INNER JOIN localizadores loc ON(aca.identificadorunico = loc.identificadorunicoacao)
                   LEFT JOIN wssof.ws_programasdto prg ON(aca.codigoprograma = prg.codigoprograma)
                   LEFT JOIN wssof.ws_produtosdto pro ON(aca.codigoproduto = pro.codigoproduto::text)
                   LEFT JOIN wssof.ws_unidadesmedidadto unm ON(aca.codigounidademedida = unm.codigounidademedida)
                   LEFT JOIN propostas prp ON(aca.identificadorunico = prp.identificadorunicoacao))
DML;


    // -- Atualização de dados no snapshot da ação/localizador
    $sqlImportacao .= <<<DML
{$tabelasImportacaoAcao}

UPDATE altorc.snapshotacao
  SET tilcod = (SELECT codigotipoinclusao FROM dados WHERE unicod = dados.codigoorgao AND acacod = dados.codigoacao AND loccod = dados.codigolocalizador),
      tiacod = (SELECT codigotipoinclusaoacao FROM dados WHERE unicod = dados.codigoorgao AND acacod = dados.codigoacao AND loccod = dados.codigolocalizador),
      locdsc = (SELECT localizadordescricao FROM dados WHERE unicod = dados.codigoorgao AND acacod = dados.codigoacao AND loccod = dados.codigolocalizador),
      procod = (SELECT codigoproduto FROM dados WHERE unicod = dados.codigoorgao AND acacod = dados.codigoacao AND loccod = dados.codigolocalizador),
      prodsc = (SELECT produtodescricao FROM dados WHERE unicod = dados.codigoorgao AND acacod = dados.codigoacao AND loccod = dados.codigolocalizador),
      unmcod = (SELECT codigounidademedida FROM dados WHERE unicod = dados.codigoorgao AND acacod = dados.codigoacao AND loccod = dados.codigolocalizador),
      unmdsc = (SELECT unidademedidadescricao FROM dados WHERE unicod = dados.codigoorgao AND acacod = dados.codigoacao AND loccod = dados.codigolocalizador),
      proquantidade = (SELECT quantidadeproduto FROM dados WHERE unicod = dados.codigoorgao AND acacod = dados.codigoacao AND loccod = dados.codigolocalizador)
   WHERE mcrid = {$mcrid}
    AND EXISTS (SELECT 1
                  FROM dados
                  WHERE unicod = dados.codigoorgao
                    AND acacod = dados.codigoacao
                    AND loccod = dados.codigolocalizador);


DML;

    // -- Inserção de novos dados no snapshot da ação/localizador
    $sqlImportacao .= <<<DML
{$tabelasImportacaoAcao}

INSERT INTO altorc.snapshotacao(snaexercicio,
                                esfcod,
                                unicod,
                                funcod,
                                sfucod,
                                prgcod,
                                acacod,
                                loccod,
                                tilcod,
                                tiacod,
                                valacrescimo,
                                valreducao,
                                acadsc,
                                prgdsc,
                                locdsc,
                                procod,
                                prodsc,
                                unmcod,
                                unmdsc,
                                proquantidade,
                                mcrid,
                                acaidentificadorunicosiop)
SELECT dados.exercicio,
       dados.codigoesfera,
       dados.codigoorgao, -- unidade orcamentaria
       dados.codigofuncao,
       dados.codigosubfuncao,
       dados.codigoprograma,
       dados.codigoacao,
       dados.codigolocalizador,
       dados.codigotipoinclusao,
       dados.codigotipoinclusaoacao,
       dados.valacrescimo,
       dados.valreducao,
       dados.acaodescricao,
       dados.programadescricao,
       dados.localizadordescricao,
       dados.codigoproduto,
       dados.produtodescricao,
       dados.codigounidademedida,
       dados.unidademedidadescricao,
       dados.quantidadeproduto,
       dados.mcrid,
       dados.identificadorunico
  FROM dados
  WHERE NOT EXISTS (SELECT 1
                      FROM altorc.snapshotacao sna
                      WHERE sna.unicod = dados.codigoorgao
                        AND sna.acacod = dados.codigoacao
                        AND sna.loccod = dados.codigolocalizador
                        AND sna.mcrid = dados.mcrid);


DML;

    $sqlImportacao .= <<<DML
UPDATE wssof.ws_execucaoorcamentariadto
  SET dotatual = 0
  WHERE COALESCE(dotatual, '') = '';

UPDATE wssof.ws_execucaoorcamentariadto
  SET dotacaoinicial = 0
  WHERE COALESCE(dotacaoinicial, '') = '';

DO $$
DECLARE
    linha RECORD;
    v_snaid altorc.snapshotacao.snaid%TYPE;
    v_spoid altorc.snapshotplanoorcamentario.spoid%TYPE;

BEGIN
    FOR linha IN WITH execucao AS (SELECT exe.unidadeorcamentaria,
                                          exe.acao,
                                          exe.localizador,
                                          exe.planoorcamentario,
                                          exe.fonte,
                                          exe.natureza,
                                          exe.resultadoprimarioatual,
                                          exe.resultadoprimariolei,
                                          (CASE WHEN '' = exe.idoc THEN '9999'
                                                ELSE COALESCE(exe.idoc, '9999')
                                             END)::INTEGER AS idoc,
                                          (CASE WHEN '' = exe.iduso THEN '0'
                                                ELSE COALESCE(exe.iduso, '0')
                                             END)::INTEGER AS iduso,
                                          COALESCE(exe.dotacaoinicial, '')::NUMERIC AS dotacaoinicial,
                                          (CASE WHEN '' = exe.dotatual THEN '0.00'
                                                ELSE COALESCE(exe.dotatual, '')
                                             END)::NUMERIC AS dotatual
                                     FROM wssof.ws_execucaoorcamentariadto exe
                                     WHERE exe.planoorcamentario != '@'
                                       AND (exe.dotacaoinicial::numeric(17,2) > 0 OR exe.dotatual::numeric(17,2) > 0)
                                     ORDER BY exe.unidadeorcamentaria,
                                              exe.acao,
                                              exe.localizador,
                                              exe.fonte,
                                              exe.natureza,
                                              exe.resultadoprimarioatual),
                      programatica AS (SELECT DISTINCT aca.codigoacao, -- DISTINCT por causa do loc.codigotipoinclusao
                                                       aca.identificadorunico,
                                                       aca.codigoorgao,
                                                       loc.codigolocalizador
                                         FROM wssof.ws_acoesdto aca
                                           INNER JOIN wssof.ws_localizadoresdto loc ON (aca.identificadorunico = loc.identificadorunicoacao))
                 SELECT exe.planoorcamentario,
                        exe.fonte,
                        exe.natureza,
                        exe.resultadoprimarioatual,
                        exe.resultadoprimariolei,
                        exe.idoc,
                        exe.iduso,
                        exe.dotacaoinicial,
                        exe.dotatual,
                        sna.snaid,
                        plo.titulo
                   FROM execucao exe
                     INNER JOIN programatica prg ON(exe.unidadeorcamentaria = prg.codigoorgao
                                                    AND exe.acao = prg.codigoacao
                                                    AND exe.localizador = prg.codigolocalizador)
                     INNER JOIN altorc.snapshotacao sna ON(exe.unidadeorcamentaria = sna.unicod
                                                           AND exe.acao = sna.acacod
                                                           AND exe.localizador = sna.loccod)
                     INNER JOIN wssof.ws_planosorcamentariosdto plo ON(prg.identificadorunico = plo.identificadorunicoacao
                                                                       AND exe.planoorcamentario = plo.planoorcamentario)
                   WHERE sna.mcrid = {$mcrid} LOOP

      -- atualizar snapshot planoorcamentario
      UPDATE altorc.snapshotplanoorcamentario
        SET dotatual = linha.dotatual,
            dotinicial = linha.dotacaoinicial,
            idoccod = linha.idoc,
            idusocod = linha.iduso,
            rpcod = linha.resultadoprimarioatual,
            rpleicod = linha.resultadoprimariolei,
            plodsc = linha.titulo
        WHERE snaid = linha.snaid
          AND foncod = linha.fonte
          AND natcod = linha.natureza
          AND plocod = linha.planoorcamentario
        RETURNING spoid INTO v_spoid;

      -- se não tiver nada para atualizar, inserir snapshot planoorcamentario
      IF (v_spoid IS NULL) THEN
        INSERT INTO altorc.snapshotplanoorcamentario(foncod,
                                                     idoccod,
                                                     idusocod,
                                                     natcod,
                                                     rpcod,
                                                     rpleicod,
                                                     plocod,
                                                     snaid,
                                                     dotatual,
                                                     plodsc,
                                                     dotinicial)
          VALUES(linha.fonte,
                 linha.idoc,
                 linha.iduso,
                 linha.natureza,
                 linha.resultadoprimarioatual,
                 linha.resultadoprimariolei,
                 linha.planoorcamentario,
                 linha.snaid,
                 linha.dotatual,
                 linha.titulo,
                 linha.dotacaoinicial);
      END IF;
    END LOOP;
END$$
DML;

    $db->executar($sqlImportacao);
    if ($db->commit()) {
        $fm->addMensagem("Snapshot do momento de alteração de crédito realizado com sucesso.");
    } else {
        $fm->addMensagem("Erro ao executar o snapshot.", Simec_Helper_FlashMessage::ERRO);
    }
}
