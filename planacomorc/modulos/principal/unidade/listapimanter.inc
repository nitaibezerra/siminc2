<?php
    /**
     * Tela de listagem de PI cadastrados no sistema.
     *
     * $Id: listapimanter.inc 102359 2015-09-11 18:26:07Z maykelbraz $
     *
     * @filesource
     */
//    include_once '_funcoespi.php';

    $requisicao = $_REQUEST['requisicao'];
    if($requisicao) {
        switch ($requisicao) {
            case 'detalharPi':
                include_once APPRAIZ . "planacomorc/modulos/principal/unidade/detalhes.inc";
                die();
            break;
            case 'detalharPiDelegadas':
                include_once APPRAIZ . "planacomorc/modulos/principal/unidade/sub-unidades-delegadas.inc";
                die();
            break;
        }
    }

    /**
     * Cabeçalho padrão do Simec.
     */
    include APPRAIZ . "includes/cabecalho.inc";
?>

<script src="js/unidade/lista-pi-manter.js"></script>

<div class="row col-md-12" style="margin-bottom: 30px;">
    <?php
        $bc = new Simec_View_Breadcrumb();
        $bc->add('Unidades Orçamentárias')->add('Planos Internos')->render();

        $filtropi = $_REQUEST['filtropi'];

        // -- Where UO para perfil PFL_GESTAO_ORCAMENTARIA_IFS - utilizado no combo
        $whereUO = '';
        if (in_array(PFL_SUBUNIDADE, $perfis)) {
            $whereUO = <<<DML
                AND EXISTS (
                    SELECT 1
                    FROM planacomorc.usuarioresponsabilidade rpu
                        JOIN public.unidadegestora ung on rpu.ungcod = ung.ungcod
                    WHERE
                        rpu.usucpf = '%s'
                        AND rpu.pflcod = %d
                        AND rpu.rpustatus = 'A'
                        AND uni.unicod  = ung.unicod
                )
DML;
            $whereUO = sprintf($whereUO, $_SESSION['usucpf'], PFL_SUBUNIDADE);
        }

        $form = new Simec_View_Form('filtropi');
        $form
            ->addInputTexto('plicod', $filtropi['plicod'], 'plicod', 11, false, array('flabel' => 'Código PI'))
            ->addInputCombo('unicod', Spo_Model_Unidade::queryCombo(true, $whereUO), $filtropi['unicod'], 'unicod', array('flabel' => 'Unidade', 'multiple' => 'true'))
            ->addInputCombo('ungcod', Spo_Model_Unidade::queryComboSubUnidade((object) array('exercicio' => $_SESSION['exercicio'], 'whereUO' => $whereUO)), $filtropi['ungcod'], 'ungcod', array('flabel' => 'Sub-Unidade', 'multiple' => 'true'))
            ->addInputCombo('ptres', Spo_Model_Ptres::queryCombo($_SESSION['exercicio'], true), $filtropi['ptres'], 'ptres', array('flabel' => 'PTRES', 'multiple' => true))
            ->addInputTexto('descricao', $filtropi['descricao'], 'descricao', 200, false, array('flabel' => 'Título/Descrição'))
            ->addBotoes(array('limpar', 'buscar', 'novo'))
            ->render();
        
//inputCombo($whereUO, $dados, $form, $id, $opcoes);

        // -- Lista de PIs
        $list = new Simec_Listagem();
        $params = array_merge(
            $_REQUEST['filtropi']?$_REQUEST['filtropi']:array(),
            array(
                'exercicio' => $_SESSION['exercicio'],
                'usucpf' => $_SESSION['usucpf'])
        );
//ver($_REQUEST['filtropi']);
        $list->setQuery(Spo_Model_Planointerno::listar((object) array(
            'exercicio' => $_SESSION['exercicio'],
            'usucpf' => $_SESSION['usucpf'],
            'plicod' => $_REQUEST['filtropi']['plicod'],
            'unicod' => $_REQUEST['filtropi']['unicod'],
            'ungcod' => $_REQUEST['filtropi']['ungcod'],
            'ptres' => $_REQUEST['filtropi']['ptres'],
            'descricao' => $_REQUEST['filtropi']['descricao']
            )))
            ->turnOnPesquisator()
            ->setFormFiltros('filtropi')
//            ->esconderColunas('unidsc', 'valido')
            ->setCabecalho(array(
                'id', 'Código', 'Sub-Unidade', 'SUO Delegada', 'Título', 'PTRES', 'Funcional', 'Situação', 'Orçamento do PI (R$)' => array(
                    'Custeio', 'Capital', 'Total', 'Empenhado', 'Pago', 'Liquidado'
                )))
//            ->addCallbackDeCampo('unicod', 'alinharUOEsquerda')
//            ->addCallbackDeCampo('plititulo', 'formatarTituloPI')
//            ->addCallbackDeCampo('obrid', 'formatarObrid')
            ->addCallbackDeCampo('delegadas', 'exibirIconeDelegadas')
//            ->addCallbackDeCampo('cadastramento', 'formatarCadastramento')
            ->addCallbackDeCampo(array('custeio', 'capital', 'total', 'empenhado', 'pago', 'liquidado'), 'mascaraMoeda')
            ->addAcao('edit', 'alterarPi')
//            ->setAcaoComoCondicional('edit', array(
//                array('campo' => 'obrid', 'valor' => 'N/A', 'op' => 'igual'),
//                array('campo' => 'cadastramento', 'valor' => Spo_Model_Planointerno::CADASTRADO_SIAFI, 'op' => 'diferente')
//            ))
            ->addAcao('delete', 'removerPi')
//            ->setAcaoComoCondicional('delete', array(array(
//                'campo' => 'cadastramento', 'valor' => Spo_Model_Planointerno::CADASTRADO_SIAFI, 'op' => 'diferente'
//            )))
            ->addAcao('view', 'detalhePI')
//            ->setAcaoComoCondicional('view', array(array(
//                'campo' => 'cadastramento', 'valor' => Spo_Model_Planointerno::CADASTRADO_SIAFI, 'op' => 'diferente'
//            )))
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

        # Popup de detalhamento do PI
        bootstrapPopup('Dados do Plano Interno', 'detalhepi', '', array('fechar'), array('tamanho' => 'lg'));

        # Popup com lista de Unidades delegadas
        bootstrapPopup('Dados do Plano Interno', 'detalhePiDelegadas', '', array('fechar'), array('tamanho' => 'lg'));
    ?>
</div>

<script type="text/javascript">
    $(document).ready(function(){
        initListaPiManter();
    });
</script>
