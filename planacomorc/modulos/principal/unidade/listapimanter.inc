<?php

    /**
     * Tela de listagem de PI cadastrados no sistema.
     *
     * $Id: listapimanter.inc 102359 2015-09-11 18:26:07Z maykelbraz $
     *
     * @filesource
     */

    $requisicao = $_REQUEST['requisicao'];
    $filtropi = $_REQUEST;
    
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

    <?php
        $bc = new Simec_View_Breadcrumb();
        $bc->add('Unidades Orçamentárias')->add('Planos Internos')->render();

        // -- Where UO para perfil PFL_GESTAO_ORCAMENTARIA_IFS - utilizado no combo
        $whereUO = '';
        if (in_array(PFL_SUBUNIDADE, $perfis)) {
//            $whereUO = <<<DML
//                AND EXISTS (
//                    SELECT
//                        1
//                    FROM planacomorc.usuarioresponsabilidade rpu
//                        JOIN public.unidadegestora ung on rpu.ungcod = ung.ungcod
//                    WHERE
//                        rpu.usucpf = '%s'
//                        AND rpu.pflcod = %d
//                        AND rpu.rpustatus = 'A'
//                        AND uni.unocod  = ung.unocod
//                )
//DML;
//            $whereUO = sprintf($whereUO, $_SESSION['usucpf'], PFL_SUBUNIDADE);
        }
        
    ?>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Pesquisa avançada por PI</h5>
                </div>
                <div class="ibox-content">
                    <form id="filtropi" name="filtropi" method="POST" role="form" class="form-horizontal">
                        <input name="" id="filtropi_requisicao" value="" type="hidden">

                        <?php
                            echo $simec->input('plicod', 'Código do PI', $filtropi['plicod'], ['maxlength' => 11]);
                            echo $simec->select('unicod', 'Unidade', $filtropi['unicod'], Spo_Model_Unidade::queryCombo(true, $whereUO), array('multiple' => 'true'));
                            echo $simec->select('ungcod', 'Sub-Unidade', $filtropi['ungcod'], Spo_Model_Ptres::queryCombo($_SESSION['exercicio'], true), array('multiple' => 'true'));
                            echo $simec->select('ptres', 'PTRES', $filtropi['ptres'], Spo_Model_Unidade::queryComboSubUnidade((object) array('exercicio' => $_SESSION['exercicio'], 'whereUO' => $whereUO)), array('multiple' => 'true'));
                            echo $simec->input('descricao', 'Título/Descrição', $filtropi['descricao'], ['maxlength' => 200]);
                            echo $simec->select('eqdid', 'Enquadramento da Despesa', $filtropi['eqdid'], Spo_Model_Ptres::queryComboEnquadramento((object) array('exercicio' => $_SESSION['exercicio'], 'whereUO' => $whereUO)), array('multiple' => 'true'));
                            echo $simec->radio('pliemenda', 'Emenda', $filtropi['pliemenda'], array('t' => 'Sim', 'f' => 'Não', '' => 'Todos'));
                            echo $simec->select('esdid', 'Situação', $filtropi['esdid'], Spo_Model_Unidade::queryComboEstados((object)array()), array('multiple' => 'true'));
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-warning btn-limpar" type="reset">
                                    <span class="glyphicon glyphicon-remove-circle"></span> Limpar
                                </button>
                                <button class="btn btn-primary btn-buscar" type="submit">
                                    <span class="glyphicon glyphicon-search"></span> Buscar
                                </button>
                                <button class="btn btn-success btn-novo" type="button">
                                    <span class="glyphicon glyphicon-plus"></span>
                                    Novo
                                </button>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>
    
<div class="row col-md-12" style="margin-bottom: 30px;">
    <?php
        # Lista de PIs
        $filtropi['exercicio'] = $_SESSION['exercicio'];
        $filtropi['usucpf'] = $_SESSION['usucpf'];
        $list = new Simec_Listagem();
        $list->setQuery(Spo_Model_Planointerno::listar((object) $filtropi))
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
