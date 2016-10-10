<?php

/* Monta a busca do Relatório  */
function montaRelatorio($post) {
    global $db;
    $listagem = new Simec_Listagem();
    $cabecalho = array();
    /* Retorna vazio caso não seja selecionada nenhuma coluna. */
    if (count($post['dados']['colunas']['qualitativo']) == 0 || count($post['dados']['quantitativo']) == 0) {
        $sql = "SELECT 1 WHERE 1 <> 1 ";
    }

    /* Tratando as colunas do Qualitativo */
    if (count($post['dados']['colunas']['qualitativo']) > 0) {
        foreach ($post['dados']['colunas']['qualitativo'] as $valor) {
            $titulo = $db->pegaLinha("SELECT crldsc FROM siafi.colunasrelatorio WHERE crlcod = '{$valor}'");
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
    if (count($post['dados']['colunas']['quantitativo']) > 0) {
        $select .= ", ";
        foreach ($post['dados']['colunas']['quantitativo'] as $valor) {
            $titulo = $db->pegaLinha("SELECT icbdscresumida FROM siafi.informacaocontabil WHERE icbcod = '{$valor}'");
            $titulo = $titulo['icbdscresumida'];
            $contas = $db->carregar("SELECT
                                        conconta
                                    FROM
                                        siafi.informacaoconta
                                    WHERE
                                        icbcod = '{$valor}'");
            $contasFiltrado = array();
            foreach ($contas as $valorConta) {
                array_push($contasFiltrado, $valorConta['conconta']);
            }
            $contasFiltrado = implode($contasFiltrado, "','");
            // Cabeçalho
            $contasComentario = str_replace("'", "", $contasFiltrado);
            $titulo = "<span class=\"glyphicon glyphicon-info-sign\" style=\"cursor:pointer;\" title=\"Contas: {$contasComentario}\" onclick=\"detalharContas('{$contasComentario}')\"></span> $titulo";
            array_push($cabecalho, $titulo);
            // Query
            $select .= " SUM(CASE WHEN sld.sldcontacontabil IN ('{$contasFiltrado}') THEN sld.sldvalor ELSE 0 END) as conta_{$valor} ,";
            $listagem->addCallbackDeCampo("conta_{$valor}", 'mascaraMoeda');
            $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, "conta_{$valor}");
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
        $sql = " SELECT {$select}
        FROM dw.saldo{$_SESSION['exercicio']} sld
        {$join}
        WHERE 1=1
        {$where}
        GROUP BY
        {$groupby}
        ORDER BY 1 ";
    }

    #ver($post, $sql, $cabecalho);
    /* Conectando diretamente ao banco do Financeiro (SIAFI) */
    $conexao = pg_connect("dbname= hostaddr= user= password= port=");
    $dados = pg_fetch_all(pg_query($sql));
    if (!is_array($dados)) {
        $dados = array();
    }
    $listagem->setDados($dados);
    $listagem->setCabecalho($cabecalho);
    $listagem->setFormOff();
    /* Mostrar a query em um hidden na tela */
    $saida['listagem'] = $listagem;
    $saida['sql'] = $sql;
    @pg_close($conexao);
    return $saida;
    ;
}

/* Função para montar o Relatório Dinâmico */

function montaExtratoDinamicoSiop($post) {

    global $db, $fm;

    /* Verificando se é para salvar a Consulta */
    if (isset($post['salvar']) && $post['salvar'] == 't') {
        /* Edita um relatório existente */
        #ver($post['salvar'],d);
        /* Salva um novo relatório */ 
            $sql = " INSERT INTO siafi.consultaextratosiop (cexnome, cexconteudo, usucpf, dataalteracao, cexpublico) "
                    . "VALUES ('{$post['cexnome']}', '{$post['cexconteudo']}', {$_SESSION['usucpf']}, NOW(), {$post['cexpublico']}) RETURNING cexid";
            $salvo = $db->carregar($sql);
            if ($salvo) {
                $saida['cexid'] = $salvo[0]['cexid'];
                $fm->addMensagem('Consulta salva com Sucesso.', Simec_Helper_FlashMessage::SUCESSO);
            } else {
                $fm->addMensagem('Erro ao gravar a Consulta.', Simec_Helper_FlashMessage::ERRO);
            }
//        if (isset($post['cexid']) && $post['cexid'] != '') {
//            $sql = " UPDATE siafi.consultaextratosiop SET "
//                    . " cexnome = '{$post['cexnome']}', "
//                    . " cexconteudo = '{$post['cexconteudo']}', "
//                    . " dataalteracao = NOW(), "
//                    . " cexpublico = {$post['cexpublico']}"
//                    . " WHERE cexid = {$post['cexid']}";
//            #echo $sql; die;
//            $cexid = $db->carregar($sql);
//            if ($cexid) {
//                $saida['cexid'] = $post['cexid'];
//                $fm->addMensagem('Consulta salva com Sucesso.', Simec_Helper_FlashMessage::SUCESSO);
//            } else {
//                $fm->addMensagem('Não foi possível localizar a captação selecionada.', Simec_Helper_FlashMessage::ERRO);
//            }
//        }
//        /* Salva um novo relatório */ else {
//            $sql = " INSERT INTO siafi.consultaextratosiop (cexnome, cexconteudo, usucpf, dataalteracao, cexpublico) "
//                    . "VALUES ('{$post['cexnome']}', '{$post['cexconteudo']}', {$_SESSION['usucpf']}, NOW(), {$post['cexpublico']}) RETURNING cexid";
//            $salvo = $db->carregar($sql);
//            if ($salvo) {
//                $saida['cexid'] = $salvo[0]['cexid'];
//                $fm->addMensagem('Consulta salva com Sucesso.', Simec_Helper_FlashMessage::SUCESSO);
//            } else {
//                $fm->addMensagem('Erro ao gravar a Consulta.', Simec_Helper_FlashMessage::ERRO);
//            }
//        }
    }

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

    #ver(count($post['dados']['cols-qualit']));

    /* Tratando as colunas do Qualitativo */
    if (count($post['dados']['cols-qualit']) > 0) {
        $campos = 1;
        foreach ($post['dados']['cols-qualit'] as $valor) {
            $result = $db->pegaLinha("SELECT crldsc, clrexpqualitativo, crlexpcallback FROM siafi.colunasextratosiop WHERE crlcod = '{$valor}' AND crltipo = 'QL'");
            $titulo = $result['crldsc'];
            // Cabeçalho
            array_push($cabecalho, $titulo);
            // Query
            if ($result['clrexpqualitativo'] <> '') {
                $select .= " {$result['clrexpqualitativo']} AS {$valor} ,";
            } else {
                $select .= " {$valor} ,";
            }
            /* Caso tenha função Callback */
            if ($result['crlexpcallback'] != '') {
                $listagem->addCallbackDeCampo("{$valor}", $result['crlexpcallback']);
            }
            $campos ++;
        }
        $select = substr($select, 0, strlen($select) - 1);
    }

    /* Tratando as colunas do Quantitativo */
    if (count($post['dados']['cols-quant']) > 0) {
        $select .= ", ";
        foreach ($post['dados']['cols-quant'] as $valor) {
            $titulo = $db->pegaLinha("SELECT crldsc FROM siafi.colunasextratosiop WHERE crlcod = '{$valor}' AND crltipo = 'QT'");
            $titulo = $titulo['crldsc'];
            array_push($cabecalho, $titulo);
            // Query
            /* Testa se a coluna quantitativa é de Expressão */
            $colunaExpressao = $db->pegaLinha("SELECT crlexpquantitativo, crlexpcallback, crlexpcomtotal, crlexpaddgroupby FROM siafi.colunasextratosiop WHERE crlcod = '{$valor}' AND crltipo = 'QT' AND crlexpquantitativo IS NOT NULL");

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
    /* Group BY */
    for ($i = 1; $i < $campos; $i++) {
        $montaGroupBy .= " $i,";
    }
    $montaGroupBy = substr($montaGroupBy, 0, strlen($montaGroupBy) - 1);
    $groupby = "{$montaGroupBy} {$groupby}";

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
            spo.siopexecucao sex
        JOIN public.unidade uni USING (unicod)
        WHERE
            exercicio = '{$_SESSION['exercicio']}'
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
    $listagem->turnOnPesquisator();
    /* Mostrar a query em um hidden na tela */
    $saida['listagem'] = $listagem;
    $saida['sql'] = $sql;

    /* Imprime de acordo com a chamada */
    if ($post['requisicao'] == 'exportarXLS') {
        $_REQUEST['_p'] = 'all';
        $listagem->render();
        die();
    } else {
        return $saida;
    }
}

function retornaSqlUoComboCompleta() {

    return "SELECT
                unicod                   AS codigo,
                unicod || ' - ' ||unidsc AS descricao
            FROM
                unidade
            WHERE
                orgcod = '". CODIGO_ORGAO_SISTEMA. "'
            OR  unicod IN ( '73107' ,
                           '74902')
            ORDER BY
                1";
}

function retornaSqlUgComboCompleta($unicod) {
    if ($unicod != '') {
        $filtro = " AND unicod = '{$unicod}' ";
    }
    return "SELECT
            ungcod                   AS codigo,
            ungcod || ' - ' ||ungdsc AS descricao
        FROM
            unidadegestora
        WHERE
            unicod IN
            (
                SELECT
                    unicod
                FROM
                    unidade
                WHERE
                    orgcod = '". CODIGO_ORGAO_SISTEMA. "'
                OR  unicod IN ( '73107' ,
                               '74902'))
        {$filtro}                   
        ORDER BY
            1";
}
/*
 * Mascara global
 */
function mascaraglobal($val, $mask) {
    $maskared = '';
    $k = 0;
    for ($i = 0; $i <= strlen($mask) - 1; $i++) {
        if ($mask[$i] == '#') {
            if (isset($val[$k]))
                $maskared .= $val[$k++];
        }
        else {
            if (isset($mask[$i]))
                $maskared .= $mask[$i];
        }
    }
    return $maskared;
}

/*
 * Formata CPF ou CNPJ
 */
function formataCpfCnpj($cpfcnpj) {
    $cpfcnpj = trim($cpfcnpj);
    if (strlen($cpfcnpj) == 11) {
        $cpfcnpj = mascaraglobal($cpfcnpj, '###.###.###-##');
    } else {
        $cpfcnpj = mascaraglobal($cpfcnpj, '##.###.###/####-##');
    }

    return $cpfcnpj;
}

/*
 * Retorna o nome do CPF ou CNPJ
 */
function retornaNomeCpfCnpj($cpfcnpj) {
    $cpfcnpj = trim($cpfcnpj);
    if (strlen($cpfcnpj) == 11) {
        $_POST['ajaxCPF'] = true;
        include_once APPRAIZ . "www/includes/webservice/cpf.php";
        $pessoaFisicaClient = new PessoaFisicaClient('http://ws.mec.gov.br/PessoaFisica/wsdl');
        $xmlstring = $pessoaFisicaClient->solicitarDadosResumidoPessoaFisicaPorCpf($cpfcnpj);
        $xml = simplexml_load_string($xmlstring);
        $json = json_encode($xml);
        $json = json_decode($json);
        $nome = $json->PESSOA->no_pessoa_rf;
        $cpfcnpj = mascaraglobal($cpfcnpj, '###.###.###-##').' - '.$nome;
    } else {
       # $_POST['ajaxPJ'] = $cpfcnpj;
        include_once APPRAIZ . "www/includes/webservice/pj.php";
        ob_clean();
        $pessoaJuridicaClient = new PessoaJuridicaClient('http://ws.mec.gov.br/PessoaJuridica/wsdl');
        $xmlstring = $pessoaJuridicaClient->solicitarDadosResumidoPessoaJuridicaPorCnpj($cpfcnpj);
        $xml = simplexml_load_string($xmlstring);
        $json = json_encode($xml);
        $json = json_decode($json);
        $nome = $json->PESSOA->no_empresarial_rf . ' ('.$json->PESSOA->no_responsavel_rf .' - '.mascaraglobal($json->PESSOA->nu_cpf_responsavel_rf, '###.###.###-##') .')';
        $cpfcnpj = mascaraglobal($cpfcnpj, '##.###.###/####-##').' - '.$nome ;
    }
    return $cpfcnpj;
}
