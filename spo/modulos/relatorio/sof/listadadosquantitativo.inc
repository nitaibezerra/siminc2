<?php
/**
 * Listagem de da dados das tabelas do serviço WSQuantitativo da SOF.
 *
 * $Id$
 * @filesource
 */

$fm = new Simec_Helper_FlashMessage('spo/integracao/sof/listadadosquantitativo');
require_once APPRAIZ . "includes/cabecalho.inc";
?>
<div class="col-lg-12">
    <?php
    $bc = new Simec_View_Breadcrumb();
    $bc->add('SIOP (SOF)')
        ->add('Relatório')
        ->add('Listar dados do quantitativo')
        ->render();

    echo $fm->getMensagens();

    $form = new Simec_View_Form('frmpesquisa');
    $form->setRequisicao('frmpesquisa')
        ->carregarDados($_REQUEST['frmpesquisa']);
    $nomesTabelas = array();
    $nomesTabelas[] = array('codigo' => 'ws_acoesacompanhamentoorcamentariodto', 'descricao' => 'Ações para acompanhamento orçamentário (ws_acoesacompanhamentoorcamentariodto)');
    $nomesTabelas[] = array('codigo' => 'ws_execucaoorcamentariadto', 'descricao' => 'Execução orçamentária (ws_execucaoorcamentariadto)');
    $nomesTabelas[] = array('codigo' => 'ws_execucaoorcamentariadto_carga', 'descricao' => 'Execução orçamentária carga (ws_execucaoorcamentariadto_carga)');
    $nomesTabelas[] = array('codigo' => 'ws_propostadto', 'descricao' => 'Execução orçamentária carga (ws_propostadto)');
    $nomesTabelas[] = array('codigo' => 'ws_financeirodto', 'descricao' => 'Execução orçamentária carga (ws_financeirodto)');
    $nomesTabelas[] = array('codigo' => 'ws_metaplanoorcamentariodto', 'descricao' => 'Execução orçamentária carga (ws_metaplanoorcamentariodto)');
    $nomesTabelas[] = array('codigo' => 'ws_informacaocaptacaoploadto', 'descricao' => 'Informações de captação da PLOA (ws_informacaocaptacaoploadto)');

    $options = array(
        'titulo' => 'Selecione uma tabela',
        'flabel' => 'Tabela de quantitativo'
    );

    sort($nomesTabelas);
    $form->addCombo('Tabela de quantitativo', 'nometabela', $nomesTabelas)
        ->addBotoes(array('limpar', 'buscar'))
        ->render();

    if (isset($_REQUEST['frmpesquisa']['nometabela']) && $_REQUEST['frmpesquisa']['nometabela'] != '')
    {
        $sql = "SELECT *,
            to_char(dataultimaatualizacao,'DD/MM/YYYY HH24:MI:SS') as dataultimaatualizacao
            FROM wssof.{$_REQUEST['frmpesquisa']['nometabela']} ORDER BY 1";

        $listagem = new Simec_Listagem();
        $listagem->setQuery($sql);
        $listagem->turnOnPesquisator();
        $listagem->setCabecalho('auto');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
    ?>
</div>