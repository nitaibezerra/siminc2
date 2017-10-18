<?php
include_once '_funcoespi.php';

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

$oPlanoInterno = new Pi_PlanoInterno();
/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

$aPropostas = $oPlanoInterno->recuperarPropostasSiminc($_SESSION['exercicio'], $aFiltro);
?>


<div class="ibox float-e-margins">
	<div class="ibox-title">
		<h5>Listagem de Propostas Orçamentárias</h5>
	</div>
	<div class="ibox-content">

        <div class="table-responsive">
            <table class="table table-striped table-bordered table-hover dataTables-example" >
                <thead>
                <tr>
                    <th>Ações</th>
                    <th>Funcional</th>
                    <th>Unidade</th>
                    <th>Unidade</th>
                    <th>Enq. Despesa</th>
                    <th>Qtd. Física</th>
                    <th>Vl. Custeio</th>
                    <th>Vl. Capital</th>
                    <th>Vl. Total</th>
                </tr>
                </thead>
                <tbody>
                    <?php foreach($aPropostas as $dados){ ?>
                        <tr>
                            <td>
                                <a href="planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A&replicarProposta=<?php echo $dados['ppiid']; ?>"><span class="fa fa-plus-circle" title="Criar PI"></span></a>
                            </td>
                            <td><?php echo substr($dados['funcionalcampobusca'], 0, strpos($dados['funcionalcampobusca'], ' - ')); ?></td>
                            <td><?php echo $dados['unonome']; ?></td>
                            <td><?php echo $dados['suonome']; ?></td>
                            <td><?php echo $dados['eqdnome']; ?></td>
                            <td><?php echo number_format($dados['ppiqtdmeta'], 2, ',', '.'); ?></td>
                            <td><?php echo number_format($dados['ppivlrcusteio'], 2, ',', '.'); ?></td>
                            <td><?php echo number_format($dados['ppivlrcapital'], 2, ',', '.'); ?></td>
                            <td><?php echo number_format($dados['total'], 2, ',', '.'); ?></td>
                        </tr>
                    <?php } ?>
                </tbody>
            </table>
        </div>

	</div>
</div>

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

</script>
