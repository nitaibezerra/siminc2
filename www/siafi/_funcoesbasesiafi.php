<?php

/*
 * Retorna a listagem de OBs
 */

function retornaListaOb($dados) {

    $resultado = new Siafi_Service_DadosExecucaoFinanceira();

    $listagem = new Simec_Listagem();
    $arrColunas = array('Data da Transação',
        'UO da Origem',
        'Nome da UO da Origem',
        'UG da Origem',
        'Nome da UG da Origem',
        'Código da OB',
        'CNPJ / CPF do Destino',
        'Valor total (R$)');
    $listagem->setCabecalho($arrColunas)
            ->setDados($resultado->retornarListaOb($dados))
            ->addCallbackDeCampo('valortotal', 'mascaraMoeda')
            ->addCallbackDeCampo('cpfcnpjfavorecido', 'formataCpfCnpj')
            ->addCallbackDeCampo('unidsc', 'alinhaParaEsquerda')
            ->addCallbackDeCampo('ungdsc', 'alinhaParaEsquerda')
            ->turnOnPesquisator()
            ->addAcao('view', 'verOb')
            ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('valortotal'))
            ->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
}

/*
 * Retorna os detalhes de Uma OB
 */

function retornaModalDetalheOb($dados) {
    $resultado = new Siafi_Service_DadosExecucaoFinanceira();
    $form = new Simec_View_Form('ver');
    $dadosOb = $resultado->retornarDetalheOb($dados);
    $dadosOb = $dadosOb[0];
    /* Formatando Valores */
    $dadosOb['cpfcnpjfavorecido'] = retornaNomeCpfCnpj($dadosOb['cpfcnpjfavorecido']);
    $dadosOb['valortotal'] = mascaraMoeda($dadosOb['valortotal'], false);
    $form
            ->addInputTextarea('uo', $dadosOb['unicod'] . ' - ' . $dadosOb['unidsc'], 'unidade', 30000, array('somentetexto' => true, 'flabel' => 'UO da Origem'))
            ->addInputTextarea('ug', $dadosOb['ungcod'] . ' - ' . $dadosOb['ungdsc'], 'unidade', 30000, array('somentetexto' => true, 'flabel' => 'UG da Origem'))
            ->addInputTextarea('codne', botaoDetalheNe($dadosOb['codne']), 'codne', 30000, array('somentetexto' => true, 'flabel' => 'NE da OB'))
            ->addInputTextarea('codob', '<b>' . $dadosOb['codob'] . '</b>', 'codob', 30000, array('somentetexto' => true, 'flabel' => 'Código da OB'))
            ->addInputTextarea('datatransacao', $dadosOb['datatransacao'], 'datatransacao', 30000, array('somentetexto' => true, 'flabel' => 'Data da Transação'))
            ->addInputTextarea('cpfcnpjfavorecido', $dadosOb['cpfcnpjfavorecido'], 'cpfcnpjfavorecido', 30000, array('somentetexto' => true, 'flabel' => 'CNPJ ou CPF do Favorecido'))
            ->addInputTextarea('valortotal', '<b>' . $dadosOb['valortotal'] . '</b>', 'valortotal', 30000, array('somentetexto' => true, 'flabel' => 'Valor total (R$)'))
            ->addInputTextarea('bancoorigem', $dadosOb['agenciaorigem'] . ' / ' . $dadosOb['contaorigem'], 'bancoorigem', 30000, array('somentetexto' => true, 'flabel' => 'Dados bancários Origem (Ag/Conta)'))
            ->addInputTextarea('bancodestino', $dadosOb['bancodestino'] . ' / ' . $dadosOb['agenciadestino'] . ' / ' . $dadosOb['contadestino'], 'bancodestino', 30000, array('somentetexto' => true, 'flabel' => 'Dados bancários Favorecido (Banco/Ag/Conta)'))
            ->addInputTextarea('numprocesso', $dadosOb['numprocesso'], 'numprocesso', 30000, array('somentetexto' => true, 'flabel' => 'Número do Processo'))
            ->addInputTextarea('codrelatorio', $dadosOb['codrelatorio'], 'codrelatorio', 30000, array('somentetexto' => true, 'flabel' => 'Número do Relatório'))
            ->addInputTextarea('codrp', $dadosOb['codrp'], 'codrp', 30000, array('somentetexto' => true, 'flabel' => 'Código do RP'))
            ->addInputTextarea('obs1', $dadosOb['obs1'] . ' ' . $dadosOb['obs2'], 'obs1', 30000, array('somentetexto' => true, 'flabel' => 'Observação'))
            ->render();

    die();
}

/*
 * Retorna os detalhes de Uma NE
 */

function retornaModalDetalheNe($dados) {
    $resultado = new Siafi_Service_DadosExecucaoFinanceira();
    $form = new Simec_View_Form('ver');
    $dadosNe = $resultado->retornarDetalheNe($dados);
    $dadosNe = $dadosNe[0];
    /* Formatando Valores */
    $dadosNe['cpfcnpjfavorecido'] = retornaNomeCpfCnpj($dadosNe['cpfcnpjfavorecido']);
    $dadosNe['valortotal'] = mascaraMoeda($dadosNe['valortotal'], false);
    $form
            ->addInputTextarea('uo', $dadosNe['unicod'] . ' - ' . $dadosNe['unidsc'], 'unidade', 30000, array('somentetexto' => true, 'flabel' => 'UO da Origem'))
            ->addInputTextarea('ug', $dadosNe['ungcod'] . ' - ' . $dadosNe['ungdsc'], 'unidade', 30000, array('somentetexto' => true, 'flabel' => 'UG da Origem'))
            ->addInputTextarea('necod', '<b>' . $dadosNe['necod'] . '</b>', 'necod', 30000, array('somentetexto' => true, 'flabel' => 'Código da NE'))
            ->addInputTextarea('datatransacao', $dadosNe['datatransacao'], 'datatransacao', 30000, array('somentetexto' => true, 'flabel' => 'Data da Transação'))
            ->addInputTextarea('cpfcnpjfavorecido', $dadosNe['cpfcnpjfavorecido'], 'cpfcnpjfavorecido', 30000, array('somentetexto' => true, 'flabel' => 'CNPJ ou CPF do Favorecido'))
            ->addInputTextarea('ptres', $dadosNe['ptres'], 'ptres', 30000, array('somentetexto' => true, 'flabel' => 'PTRES'))
            ->addInputTextarea('amparolegal', $dadosNe['amparolegal'], 'amparolegal', 30000, array('somentetexto' => true, 'flabel' => 'Amparo Legal'))
            ->addInputTextarea('refdispensa', $dadosNe['refdispensa'], 'refdispensa', 30000, array('somentetexto' => true, 'flabel' => 'Referência de Despesa'))
            ->addInputTextarea('contafinanceiro', $dadosNe['contafinanceiro'], 'contafinanceiro', 30000, array('somentetexto' => true, 'flabel' => 'Conta Financeiro'))
            ->addInputTextarea('observacao', $dadosNe['observacao'] . ' ' . $dadosNe['observacao'], 'observacao', 30000, array('somentetexto' => true, 'flabel' => 'Observação'))
            ->addInputTextarea('valortotal', '<b>' . $dadosNe['valortotal'] . '</b>', 'valortotal', 30000, array('somentetexto' => true, 'flabel' => 'Valor total (R$)'))
            ->render();

    die();
}

/*
 * Retorna o botão de Detalhar NE
 */

function botaoDetalheNe($necod) {
    $necod = "<span class=\"btn btn-primary btn-xs glyphicon glyphicon-eye-open botaoRetornarDadosNe \" value=\"{$necod}\" title=\"Ver detalhes sobre esta NE\" ></span> <b>{$necod}</b>";
    return $necod;
}

/*
 * Retorna a listagem de NEs
 */

function retornaListaNe($dados) {

    $resultado = new Siafi_Service_DadosExecucaoFinanceira();

    $listagem = new Simec_Listagem();
    $arrColunas = array('Data da Transação',
        'UO da Origem',
        'Nome da UO da Origem',
        'UG da Origem',
        'Nome da UG da Origem',
        'Código da NE',
        'CNPJ / CPF do Destino',
        'Valor total (R$)');
    $listagem->setCabecalho($arrColunas)
            ->setDados($resultado->retornarListaNe($dados))
            ->addCallbackDeCampo('valortotal', 'mascaraMoeda')
            ->addCallbackDeCampo('cpfcnpjfavorecido', 'formataCpfCnpj')
            ->addCallbackDeCampo('unidsc', 'alinhaParaEsquerda')
            ->addCallbackDeCampo('ungdsc', 'alinhaParaEsquerda')
            ->turnOnPesquisator()
            ->addAcao('view', 'verNe')
            ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('valortotal'))
            ->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
}

/*
 * Retorna a listagem de NSs
 */

function retornaListaNs($dados) {

    $resultado = new Siafi_Service_DadosExecucaoFinanceira();

    $listagem = new Simec_Listagem();
    $arrColunas = array('Data da Transação',
        'UO da Origem',
        'Nome da UO da Origem',
        'UG da Origem',
        'Nome da UG da Origem',
        'Código da NS',
        'CNPJ / CPF do Destino',
        'Valor total (R$)');
    $listagem->setCabecalho($arrColunas)
        ->setDados($resultado->retornarListaNs($dados))
        ->addCallbackDeCampo('valortotal', 'mascaraMoeda')
        ->addCallbackDeCampo('cpfcnpjfavorecido', 'formataCpfCnpj')
        ->addCallbackDeCampo('unidsc', 'alinhaParaEsquerda')
        ->addCallbackDeCampo('ungdsc', 'alinhaParaEsquerda')
        ->turnOnPesquisator()
        ->addAcao('view', 'verNs')
        ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('valortotal'))
        ->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
}

/*
 * Retorna os detalhes de Uma NE
 */

function retornaModalDetalheNs($dados) {
    $resultado = new Siafi_Service_DadosExecucaoFinanceira();
    $form = new Simec_View_Form('ver');
    $dadosNs = $resultado->retornarDetalheNs($dados);
    $dadosNs = $dadosNs[0];
    /* Formatando Valores */
    $dadosNs['cpfcnpjfavorecido'] = retornaNomeCpfCnpj($dadosNs['cpfcnpjfavorecido']);
    $dadosNs['valortotal'] = mascaraMoeda($dadosNs['valortotal'], false);
    $dadosNs['nes'] = explode(' ', $dadosNs['nes']);
    $dadosNs['nes'] = implode('<br/>', $dadosNs['nes']);
    $form
            ->addInputTextarea('uo', $dadosNs['unicod'] . ' - ' . $dadosNs['unidsc'], 'unidade', 30000, array('somentetexto' => true, 'flabel' => 'UO da Origem'))
            ->addInputTextarea('ug', $dadosNs['ungcod'] . ' - ' . $dadosNs['ungdsc'], 'unidade', 30000, array('somentetexto' => true, 'flabel' => 'UG da Origem'))
            ->addInputTextarea('nscod', '<b>' . $dadosNs['nscod'] . '</b>', 'nscod', 30000, array('somentetexto' => true, 'flabel' => 'Código da NS'))
            ->addInputTextarea('datatransacao', $dadosNs['datatransacao'], 'datatransacao', 30000, array('somentetexto' => true, 'flabel' => 'Data da Transação'))
            ->addInputTextarea('cpfcnpjfavorecido', $dadosNs['cpfcnpjfavorecido'], 'cpfcnpjfavorecido', 30000, array('somentetexto' => true, 'flabel' => 'CNPJ ou CPF do Favorecido'))
            ->addInputTextarea('nes', $dadosNs['nes'], 'nes', 30000, array('somentetexto' => true, 'flabel' => 'NE(s)'))
            ->addInputTextarea('observacao', $dadosNs['observacao'], 'observacao', 30000, array('somentetexto' => true, 'flabel' => 'Observação'))
            ->addInputTextarea('valortotal', '<b>' . $dadosNs['valortotal'] . '</b>', 'valortotal', 30000, array('somentetexto' => true, 'flabel' => 'Valor total (R$)'))
            ->render();

    die();
}
