<?php

include_once APPRAIZ . "includes/library/simec/Grafico.php";

include_once '_funcoespi.php';

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

$oPlanoInterno = new Pi_PlanoInterno();
/**
 * Cabeçalho padrão do simec.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

// @TODO - Retirar ano fico de 2017 e colocar $_SESSION['exercicio'];

$aPropostas = $oPlanoInterno->recuperarExecucaoOrcamentaria(2017, $aFiltro);
?>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Pagamentos por Unidades</h5>
        </div>
        <div class="ibox-content">
            <?php
            $estatistica = $oPlanoInterno->recuperarEstatisticaPagamento(2017);
            $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
            $grafico->setColors("'#FFD700', '#55BF3B', '#FF6A6A'")->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->gerarGrafico($estatistica);
            ?>
        </div>
    </div>

    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Painel de Acompanhamento</h5>
        </div>
        <div class="ibox-content">

            <div class="table-responsive">
                <table class="table table-striped table-bordered table-hover dataTables-example" >
                    <thead>
                    <tr>
                        <th>Ações</th>
                        <th>PI</th>
                        <th>Sub-Unidade</th>
                        <th>Funcional</th>
                        <th>PTRES</th>
                        <th>Vl. Autorizado</th>
                        <th>Vl. Pago</th>
                        <th>Vl. Liquidar</th>
                    </tr>
                    </thead>
                    <tbody>
                        <?php foreach($aPropostas as $dados){ ?>
                            <tr>
                                <td>
                                    <a href="javascript: detalhePI('<?php echo $dados['pliid']; ?>')"><span class="fa fa-plus-circle" title="Criar PI"></span></a>
                                </td>
                                <td><?php echo $dados['planointerno']; ?></td>
                                <td><?php echo $dados['unidadeorcamentaria']; ?></td>
                                <td><?php echo $dados['acao']; ?></td>
                                <td><?php echo $dados['numeroptres']; ?></td>
                                <td class="text-right"><?php echo number_format($dados['autorizado'], 0, ',', '.'); ?></td>
                                <td class="text-right"><?php echo number_format($dados['pago'], 0, ',', '.'); ?></td>
                                <td class="text-right"><?php echo number_format($dados['empenhadoaliquidar'], 0, ',', '.'); ?></td>
                            </tr>
                        <?php } ?>
                    </tbody>
                </table>
            </div>

        </div>
    </div>
</div>

<?php
// -- popup de detalhamento do PI
bootstrapPopup('Dados do Plano Interno', 'detalhepi', '', array('fechar'), array('tamanho' => 'lg'));
?>

<script>
    $(document).ready(function(){
        $('.dataTables-example').DataTable({
            pageLength: 100,
            responsive: true,
            dom: '<"html5buttons"B>lTfgitp',
            "language": {
                "url": "/zimec/public/temas/simec/js/plugins/dataTables/Portuguese-Brasil.json"
            }
        });
    });

    function detalhePI(pliid) {
        $('#detalhepi .modal-body').empty();
        $.post("planacomorc.php?modulo=principal/unidade/listapimanter&acao=A&requisicao=detalharPi&pliid=" + pliid, function(html) {
            $('#detalhepi .modal-body').html(html);
            $('#detalhepi').modal();
        });
    }

</script>
