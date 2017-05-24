<?php
/**
 * Tela de listagem de PI cadastrados no sistema.
 *
 * $Id: listapimanter.inc 102359 2015-09-11 18:26:07Z maykelbraz $
 *
 * @filesource
 */

if (isset($_REQUEST['requisicao'])) {
    $requisicao = $_REQUEST['requisicao'];
    switch ($requisicao) {
        case 'detalharPi':
            $pi = new Spo_Model_Planointerno();
            $select = 't1.*, t2.eqdcod, t2.eqddsc, t3.neecod, t3.needsc, t4.capcod, t4.capdsc,';
            $select .= 't5.mdecod, t5.mdedsc, t6.unicod, t6.unidsc, t7.ungcod, t7.ungdsc,';
            $select .= 't8.sbacod, t8.sbadsc';

            $options = array(
                'join' => array('eqdid', 'neeid', 'capid'),
                'leftjoin' => array('mdeid', 'unicod', 'ungcod', 'sbaid')
            );
            $dadosPi = $pi->recuperarTodos($select, array("t1.pliid = {$_REQUEST['pliid']}"), '', $options);

            if (!empty($dadosPi)) {
                $dadosPi = current($dadosPi);
            }
            // -- Tratamento da subação
            if (empty($dadosPi['sbaid'])) {
                $sbacod = "Código livre - {$dadosPi['plicodsubacao']}";
            } else {
                $sbacod = "{$dadosPi['sbacod']} - {$dadosPi['sbatitulo']}";
            }

            // -- Tratamento da unidade orçamentária/unidade gestora
            if (!empty($dadosPi['unicod'])) {
                $unicod = "{$dadosPi['unicod']} - {$dadosPi['unidsc']}";
            } elseif (!empty($dadosPi['ungcod'])) {
                $dadosUnicod = $db->pegaLinha(Spo_Model_Unidade::getQueryUnidadeDaUngestora($dadosPi['ungcod']));
                $unicod = "{$dadosUnicod['unicod']} - {$dadosUnicod['unidsc']}";
            } else {
                $unicod = ' - ';
            }

            echo <<<HTML
        <style>
            .modal-dialog{width:80%}
            button{cursor:pointer}
            table.horizontal th{text-align:right;background-color:#EFEFEF!important}
        </style>
        <div style="width:50%;float:left">
            <table class="tabela table table-striped table-bordered table-hover text-center horizontal">
                <tr>
                    <th>Código do PI:</th>
                    <td><b>{$dadosPi['plicod']}</b></td>
                </tr>
                <tr>
                    <th>Enquadramento da Despesa:</th>
                    <td>{$dadosPi['eqdcod']} - {$dadosPi['eqddsc']}</td>
                </tr>
                <tr>
                    <th>Segmento Cultural:</th>
                    <td>{$dadosPi['neecod']} - {$dadosPi['needsc']}</td>
                </tr>
                <tr>
                    <th>Modalidade de Pactuação:</th>
                    <td>{$dadosPi['capcod']} - {$dadosPi['capdsc']}</td>
                </tr>
<!--
                <tr>
                    <th>Codificação da Unidade(livre):</th>
                    <td>{$dadosPi['plilivre']}</td>
                </tr>
!-->
                <tr>
                    <th>Área Cultural:</th>
                    <td>{$dadosPi['mdecod']} - {$dadosPi['mdedsc']}</td>
                </tr>
                <tr>
                    <th>Título:</th>
                    <td>{$dadosPi['plititulo']}</td>
                </tr>
                <tr>
                    <th>Descrição do PI:</th>
                    <td>{$dadosPi['plidsc']}</td>
                </tr>
                <tr>
                    <th>Unidade Orçamentária:</th>
                    <td>{$unicod}</td>
                </tr>
                <tr>
                    <th>Unidade Gestora:</th>
                    <td>{$dadosPi['ungcod']} - {$dadosPi['ungdsc']}</td>
                </tr>
            </table>
        </div>
HTML;
            // -- Grafico financeiro do PI
            echo <<<HTML
        <div id="graficoDestino" style="width:50%;float:left;overflow:hidden">
HTML;
            $params = array(
                'exercicio' => $_SESSION['exercicio'],
                'unicod' => $dadosPi['unicod'],
                'plicod' => $dadosPi['plicod']
            );
            $grafico = new Grafico(null, false);
            $grafico->setTipo(Grafico::K_TIPO_COLUNA)
                ->setId('graficoPi')
                ->setWidth('450px')
                ->setTitulo('Execução Orçamentária')
                ->gerarGrafico(Spo_Model_Planointerno::queryGraficoFinanceiro($params));
            echo <<<HTML
        </div>
        <div style="clear:both" />
HTML;
            $params = array(
                'exercicio' => $_SESSION['exercicio'],
                'pliid' => $dadosPi['pliid']
            );
            $list = new Simec_Listagem();
            $list->setQuery(Spo_Model_Planointerno::queryAcoes($params))
                ->setTitulo('Detalhamento Orçamentário')
                ->setId('list-ptres')
                ->setCabecalho(array(
                    'PTRES',
                    'Ação',
                    'Financeiro (R$)' => array(
                        'Dotação do PTRES',
                        'Empenhado no PI',
                        'Detalhado no PTRES')))
                ->esconderColunas('pliid', 'ptrid', 'acaid')
                ->addCallbackDeCampo(
                    array('dotacaoinicial', 'dotacaosubacao', 'empenhado', 'detalhadoptres'),
                    'mascaraMoeda')
                ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            die();
            // -- no break
    }
}

/**
 * Cabeçalho padrão do Simec.
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript">
$(function(){
    $('[data-toggle="popover"]').popover({placement:'top', trigger:'hover'});
    $('.abrir-obra').click(function(){
        abrirObra($(this).attr('data-obrid'));
    });
    $('.abrir-ted').click(function(){
        abrirTED($(this).attr('data-tcpid'));
    });
});

function onFiltropiNovo()
{
    window.location.assign('planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A');
}

function alterarPi(pliid)
{
    window.location.assign('planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A&pliid=' + pliid);
}

function removerPi(pliid)
{
    bootbox.confirm('Tem certeza que deseja apagar o PI?', function(confirmacao){
        if (confirmacao) {
            window.location.assign('planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A&apagar=true&pliid=' + pliid);
        }
    });
}

function detalhePI(pliid) {
   $('#detalhepi .modal-body').empty();
   $.post("planacomorc.php?modulo=principal/unidade/listapimanter&acao=A&requisicao=detalharPi&pliid=" + pliid, function(html) {
       $('#detalhepi .modal-body').html(html);
       $('#detalhepi').modal();
   });
}

/*
 * Abre o TED  em outra janela, no módulo de origem
 */
function abrirTED(ted)
{
    window.open('http://simec.mec.gov.br/ted/ted.php?modulo=principal/termoexecucaodescentralizada/previsao&acao=A&ted=' + ted);
}

/**
 * Abre a OBRA em outra janela, no módulo de origem
 */
function abrirObra(obrid)
{
    window.open('http://simec.mec.gov.br/obras/obras.php?modulo=principal/cadastro_pi&acao=A&obrid=' + obrid);
}
</script>
<div class="row col-md-12">
    <?php
    $bc = new Simec_View_Breadcrumb();
    $bc->add('Instituições Federais')
        ->add('Planos Internos')
        ->render();

    $filtropi = $_REQUEST['filtropi'];

    // -- Where UO para perfil PFL_GESTAO_ORCAMENTARIA_IFS - utilizado no combo
    $whereUO = '';
    if (in_array(PFL_GESTAO_ORCAMENTARIA_IFS, $perfis)) {
        $whereUO = <<<DML
AND EXISTS (SELECT 1
              FROM planacomorc.usuarioresponsabilidade rpu
              WHERE rpu.usucpf = '%s'
                AND rpu.pflcod = %d
                AND rpu.rpustatus = 'A'
                AND rpu.unicod  = uni.unicod)
DML;
        $whereUO = sprintf($whereUO, $_SESSION['usucpf'], PFL_GESTAO_ORCAMENTARIA_IFS);
    }

    $form = new Simec_View_Form('filtropi');
    $form->addInputTexto('planointerno', $filtropi['planointerno'], 'filtropi', 11, false, array('flabel' => 'Plano interno'))
        ->addInputCombo('unicod', Spo_Model_Unidade::queryCombo(true, $whereUO), $filtropi['unicod'], 'filtrounicod', array('flabel' => 'Unidade'));
    $config = array('flabel' => 'PTRES', 'multiple' => true);
    $form->addInputCombo('ptres', Spo_Model_Ptres::queryCombo($_SESSION['exercicio'], true), $filtropi['ptres'], 'filtroptres', $config)
        ->addInputTexto('descricao', $filtropi['descricao'], 'filtrodesc', 200, false, array('flabel' => 'Título/Descrição'))
        ->addInputChoices('obras', array('Todos' => null, 'Sim' => 'S', 'Não' => 'N'), $filtropi['obras'], 'obras', array('flabel' => 'Relacionado a obra'))
        ->addInputChoices('ted', array('Todos' => null, 'Sim' => 'S', 'Não' => 'N'), $filtropi['ted'], 'ted', array('flabel' => 'Relacionado ao TED'))
        ->addInputChoices('cadastramento', array(
            'Todos' => null,
            'SIMEC/SIAFI' => Spo_Model_Planointerno::CADASTRADO_SIAFI_SIMEC,
            'SIMEC' => Spo_Model_Planointerno::CADASTRADO_SIMEC,
            'SIAFI' => Spo_Model_Planointerno::CADASTRADO_SIAFI
        ), $filtropi['cadastramento'], 'cadastramento', array('flabel' => 'Cadastrado no'))
        ->addInputChoices('compativel', array(
            'Todos' => null, 'Sim' => 'S', 'Não' => 'N'
        ), $filtropi['compativel'], 'compativel', array('flabel' => 'Compatível com SIMEC'))
        ->addBotoes(array('limpar', 'buscar', 'novo'))
        ->render();
    unset($confi);

    // -- Lista de PIs
    $list = new Simec_Listagem();
    $params = array_merge(
        $_REQUEST['filtropi']?$_REQUEST['filtropi']:array(),
        array('exercicio' => $_SESSION['exercicio'], 'usucpf' => $_SESSION['usucpf'])
    );
    $list->setQuery(Spo_Model_Planointerno::queryInstituicoesFederais($params, true, $perfis))
        ->turnOnPesquisator()
        ->setFormFiltros('filtropi')
        ->esconderColunas('unidsc', 'valido')
        ->setCabecalho(array(
            'Código', 'Título', 'Unidade', 'Obras', 'TED', 'Cadastrado?', 'Orçamento do PI (R$)' => array(
                'Orçamento total', 'Empenhado total', 'Não empenhado total'
            )))
        ->addCallbackDeCampo('unicod', 'alinharUOEsquerda')
        ->addCallbackDeCampo('plititulo', 'formatarTituloPI')
        ->addCallbackDeCampo('obrid', 'formatarObrid')
        ->addCallbackDeCampo('tcpid', 'formatarTcpid')
        ->addCallbackDeCampo('cadastramento', 'formatarCadastramento')
        ->addCallbackDeCampo(array('vlrdotacao', 'vlrempenhado', 'vlrnaoempenhado'), 'mascaraMoeda')
        ->addAcao('edit', 'alterarPi')
        ->setAcaoComoCondicional('edit', array(
            array('campo' => 'obrid', 'valor' => 'N/A', 'op' => 'igual'),
            array('campo' => 'cadastramento', 'valor' => Spo_Model_Planointerno::CADASTRADO_SIAFI, 'op' => 'diferente')
        ))
        ->addAcao('delete', 'removerPi')
        ->setAcaoComoCondicional('delete', array(array(
            'campo' => 'cadastramento', 'valor' => Spo_Model_Planointerno::CADASTRADO_SIAFI, 'op' => 'diferente'
        )))
        ->addAcao('view', 'detalhePI')
        ->setAcaoComoCondicional('view', array(array(
            'campo' => 'cadastramento', 'valor' => Spo_Model_Planointerno::CADASTRADO_SIAFI, 'op' => 'diferente'
        )))
        ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

        // -- popup de detalhamento do PI
        bootstrapPopup('Dados do Plano Interno', 'detalhepi', '', array('fechar'), array('tamanho' => 'lg'));
    ?>
</div>