<?php

    /**
     * Tela de listagem de PI cadastrados no sistema.
     *
     * $Id: listapimanter.inc 102359 2015-09-11 18:26:07Z maykelbraz $
     *
     * @filesource
     */

    $requisicao = $_REQUEST['requisicao'];
    $filtropi = $_SESSION['filtropi'] = ($_POST? $_POST: $_SESSION['filtropi']);

    $perfis = pegaPerfilGeral();

    # Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
//    $listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));
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
            case 'limpar':
                $filtropi = $_SESSION['filtropi'] = array();
            break;
            case 'listapimanter-xls':
                # Lista de PIs
                $filtropi['exercicio'] = $_SESSION['exercicio'];
                $filtropi['usucpf'] = $_SESSION['usucpf'];
                $filtropi['unofundo'] = 'FALSE';

                $mPlanointerno = new Spo_Model_Planointerno();
                $sql = $mPlanointerno->listar((object) $filtropi);
                $listaPi = $mPlanointerno->carregar($sql);
                
                include_once APPRAIZ. "planacomorc/modulos/principal/unidade/listapimanter-xls.inc";
                die;
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

<script src="js/unidade/lista-pi-manter.js?v=4"></script>
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
                    <h5><?php echo $titulo_modulo; ?></h5>
                </div>
                <div class="ibox-content">
                    <form id="filtropi" name="filtropi" method="POST" role="form" class="form-horizontal">
                        <input name="requisicao" id="requisicao" value="" type="hidden">

                        <?php
                            # Se o usuário tiver Sub-Unidades vinculadas ao seu perfil o sistema não mostra opção de selecionar UO.
                            if (!$listaSubUnidadeUsuario) {
                                echo $simec->select('unicod', 'Unidade', $filtropi['unicod'], Spo_Model_Unidade::queryCombo((object)array(
                                    'prsano::INTEGER = '. (int)$_SESSION['exercicio'],
                                    'suo.unofundo = FALSE')), array('multiple' => 'true'));
                            }
                            echo '<div id=div_ungcod >';
                            echo $simec->select('ungcod', 'SubUnidade', $filtropi['ungcod'], Public_Model_SubUnidadeOrcamentaria::queryCombo((object) array(
                                'unofundo' => 'FALSE',
                                'exercicio' => $_SESSION['exercicio'],
                                'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), array('multiple' => 'true'));
                            echo '</div>';
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-warning btn-limpar" type="button">
                                    <span class="glyphicon glyphicon-remove-circle"></span> Limpar
                                </button>
                                <button class="btn btn-primary btn-buscar" type="submit">
                                    <span class="glyphicon glyphicon-search"></span> Buscar
                                </button>
<!--                                <button id="btn-exportar-xls" class="btn btn-primary btn-buscar" type="button">
                                    <i class="fa fa-file-excel-o"></i> Exportar XLS
                                </button>-->
                            </div>
                        </div>

                    </form>
                </div>
            </div>
        </div>
    </div>
</div>

<!--implements here list-->

<script>



</script>
