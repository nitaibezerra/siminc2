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
    
    # Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
    
    if($requisicao) {
        switch ($requisicao) {
            case 'exibirGrafico':
                include_once APPRAIZ . "planacomorc/modulos/principal/unidade/grafico-pi-dotacao.inc";
                die();
            break;
            case 'carregarComboUG':
                echo montarComboUG((object) array(
                    'ungcod' => $filtropi['ungcod'],
                    'exercicio' => $_SESSION['exercicio'],
                    'unicod' => $_REQUEST['unicod'],
                    'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario
                ));
                die();
            break;
            case 'detalharPiDelegadas':
                include_once APPRAIZ . "planacomorc/modulos/principal/unidade/sub-unidades-delegadas.inc";
                die();
            break;
        }
    }
    
    /**
     * Monta a combo de UGs filtrando por UO
     * 
     * @param $filtros 
     * @return VOID
     */
    function montarComboUG(stdClass $filtros) {
        global $simec;

        return $simec->select(
            'ungcod',
            'Unidade',
            $filtros->ungcod,
            Public_Model_SubUnidadeOrcamentaria::queryCombo((object) array(
                'exercicio' => $filtros->exercicio,
                'unicod' => $filtros->unicod,
                'listaSubUnidadeUsuario' => $filtros->listaSubUnidadeUsuario)),
            array('multiple' => 'true'));
    }

    /**
     * Cabeçalho padrão do sistema.
     */
    include APPRAIZ . "includes/cabecalho.inc";
?>

<script src="js/unidade/lista-pi-manter.js"></script>
<style>
/*    .tabela-listagem {
        font-size: 1em !important;
    }*/
</style>

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
                            # Se o usuário tiver Sub-Unidades vinculadas ao seu perfil o sistema não mostra opção de selecionar UO.
                            if (!$listaSubUnidadeUsuario) {
                                echo $simec->select('unicod', 'Unidade', $filtropi['unicod'], Spo_Model_Unidade::queryCombo((object)array(
                                    'prsano::INTEGER = '. (int)$_SESSION['exercicio'],
                                    'suo.unofundo = FALSE')), array('multiple' => 'true'));
                            }
                            echo '<div id=div_ungcod >';
                            echo $simec->select('ungcod', 'Unidade', $filtropi['ungcod'], Public_Model_SubUnidadeOrcamentaria::queryCombo((object) array(
                                'unofundo' => 'FALSE',
                                'exercicio' => $_SESSION['exercicio'],
                                'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), array('multiple' => 'true'));
                            echo '</div>';
                            echo $simec->select('ptres', 'PTRES', $filtropi['ptres'], Spo_Model_Ptres::queryCombo((object) array(
                                'exercicio' => $_SESSION['exercicio'],
                                'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), array('multiple' => 'true'));
                            echo $simec->input('descricao', 'Título/Descrição', $filtropi['descricao'], ['maxlength' => 200]);
                            echo $simec->select('eqdid', 'Enquadramento da Despesa', $filtropi['eqdid'], Spo_Model_Ptres::queryComboEnquadramento((object) array('exercicio' => $_SESSION['exercicio'])), array('multiple' => 'true'));
                            echo $simec->radio('pliemenda', 'Emenda', (!$filtropi['pliemenda'] ? '' : $filtropi['pliemenda']), array('t' => 'Sim', 'f' => 'Não', '' => 'Todos'));
                            echo $simec->select('esdid', 'Situação', $filtropi['esdid'], Spo_Model_Unidade::queryComboEstados((object) array('tpdid' => WF_TPDID_PLANEJAMENTO_PI)), array('multiple' => 'true'));
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
        $filtropi['unofundo'] = 'FALSE';
        $list = new Simec_Listagem();
        $list->setQuery(Spo_Model_Planointerno::listar((object) $filtropi))
            ->turnOnPesquisator()
            ->setFormFiltros('filtropi')
//            ->esconderColunas('unidsc', 'valido')
            ->setCabecalho(array(
                'id', 'Código', 'Unidade', 'Título', 'Funcional', 'Situação', 'Orçamento do PI (R$)' => array(
                    'Custeio', 'Capital', 'Empenhado', 'Liquidado', 'Pago'
                )))
            ->addCallbackDeCampo('id', 'exibirLinkEspelho')
//            ->addCallbackDeCampo('unicod', 'alinharUOEsquerda')
//            ->addCallbackDeCampo('plititulo', 'formatarTituloPI')
//            ->addCallbackDeCampo('obrid', 'formatarObrid')
//            ->addCallbackDeCampo('delegadas', 'exibirIconeDelegadas')
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
//            ->addAcao('chart', 'exibirGrafico')
//            ->addAcao('view', 'exibirEspelhoPi')
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
