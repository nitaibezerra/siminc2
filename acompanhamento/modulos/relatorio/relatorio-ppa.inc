<?php

    /**
     * Tela de listagem de PI cadastrados no sistema.
     *
     * $Id: listapimanter.inc 102359 2015-09-11 18:26:07Z maykelbraz $
     *
     * @filesource
     */
    $requisicao = $_REQUEST['req'];

    if($requisicao) {
        switch ($requisicao) {
            case 'carregarComboUG':
                echo montarComboUG((object) array(
                    'ungcod' => $filtropi['ungcod'],
                    'exercicio' => $_SESSION['exercicio'],
                    'unicod' => $_REQUEST['unicod']
                ));
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
            'SubUnidade',
            $filtros->ungcod,
            Public_Model_SubUnidadeOrcamentaria::queryCombo((object) array(
                'exercicio' => $filtros->exercicio,
                'unicod' => $filtros->unicod)),
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
                        echo $simec->select('janid', 'Janela', $filtropi['janid'], (new Acompanhamento_Model_Janela())->recuperarSqlCombo(null, ["prsano = '{$_SESSION['exercicio']}'", 'tipid = ' . Acompanhamento_Model_Tipo::K_TIPO_META_PPA]));
                        echo $simec->select('unicod', 'Unidade', $filtropi['unicod'], Spo_Model_Unidade::queryCombo((object)array(
                                'prsano::INTEGER = '. (int)$_SESSION['exercicio'],
                                'suo.unofundo = FALSE')), array('multiple' => 'true'));
                        echo '<div id=div_ungcod>';
                        echo $simec->select('ungcod', 'SubUnidade', $filtropi['ungcod'], Public_Model_SubUnidadeOrcamentaria::queryCombo((object)array(
                                'unofundo' => 'FALSE',
                                'exercicio' => $_SESSION['exercicio'],
                                'listaSubUnidadeUsuario' => $listaSubUnidadeUsuario)), array('multiple' => 'true'));
                        echo '</div>';
                        echo $simec->select('oppdsc', 'Objetivo PPA', $filtropi->oppdsc, (new Public_Model_ObjetivoPpa())->recuperarSqlCombo());

                        // Model Sobreescrita!!!   echo $simec->select('oppdsc', 'Meta PPA', $filtropi->oppdsc, (new carregarMetasPPA())->recuperarSqlCombo());

                        ?>
                        <div class="text-center">
                            <button class="btn btn-primary btn-buscar" type="submit">
                                <span class="glyphicon glyphicon-search"></span> Buscar
                            </button>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="ibox-title">
        <h5>Lista de Relatório</h5>
    </div>
    <div class="ibox-content">
        <div class="table-responsive">
            <table class="table table-striped table-bordered table-hover dataTables">
                <thead>
                <tr class="text-center">
                    <th>Janela</th>
                    <th>Unidade</th>
                    <th>SubUnidade</th>
                    <th>Objetivo PPA</th>
                    <th>Meta PPA</th>
                </tr>
                </thead>
                <tbody>
                <tr>
                    <td><?php echo w;?></td>
                    <td><?php echo w;?></td>
                    <td><?php echo w;?></td>
                    <td><?php echo w;?></td>
                    <td><?php echo w;?></td>
                </tr>
                </tbody>
            </table>
        </div>
    </div>
</div>

<!--implements here list-->

<script>
    
    $(document).ready(function(){

        // Evento ao mudar opção de UO
        $('#unicod').change(function(){
            carregarUG($(this).val());
        });

    });
    
    /**
     * Carrega novo conteúdo para a opções de Sub-Unidade via requisição ajax.
     * 
     */
    function carregarUG(unicod) {
        $.post(
            '?modulo=relatorio/relatorio-ppa&acao=A&req=carregarComboUG', { unicod: $('#unicod').val() }, function(response) {
                $('#div_ungcod').remove('slow');
                $('#div_ungcod').html(response);
                $(".chosen-select").chosen();
        });
    }


</script>
