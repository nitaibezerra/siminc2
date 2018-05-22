<?php
    $listaPropostaDetalhe = (new Proposta_Model_PropostaDetalhe)->buscarListaFinanceiro((object)['proid' => (int)$proid]);
?>
<?php if(count($listaPropostaDetalhe)){ ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover" >
            <thead>
                <tr class="text-center">
                    <th width="10%">Ações</th>
                    <th>Nat. Desp.</th>
                    <th>IDUSO</th>
                    <th>Fonte</th>
                    <th>IDOC</th>
                    <th>Valor</th>
                    <th>Vl. Expansão</th>
                </tr>
            </thead>
            <tbody>
            <?php foreach($listaPropostaDetalhe as $dados): ?>
                <tr>
                    <td class="text-center">
                        <a title="Alterar" class="editar-detalhe"   href="?modulo=principal/proposta_form&acao=A&req=formulario-detalhe&prdid=<?php echo $dados['prdid']; ?>"><i class="fa fa-pencil"></i></a>
                        <a title="Excluir" class="excluir-detalhe"  href="?modulo=principal/proposta_form&acao=A&req=excluir-detalhe&prdid=<?php echo $dados['prdid']; ?>"  style="margin-left: 5px;"><i class="fa fa-close"></i></a>
                    </td>
                    <td align="center"><?php echo $dados['ndpcod']; ?></td>
                    <td align="center"><?php echo $dados['iducod']; ?></td>
                    <td align="center"><?php echo $dados['foncod']; ?></td>
                    <td align="center"><?php echo $dados['idocod']; ?></td>
                    <td align="right"><?php echo number_format($dados['prdvalor'], 2, ',', '.'); ?></td>
                    <td align="right"><?php echo number_format($dados['prdvalorexpansao'], 2, ',', '.'); ?></td>
                </tr>
            <?php endforeach; ?>
            </tbody>
        </table>
    </div>
<?php } else {
    echo '<div class="alert alert-danger">Nenhum registro cadastrado</div>';
} ?>

<script>
    $(function(){
        $('.editar-detalhe').click(function(){
            jQuery("#div_formulario_detalhe").load(jQuery(this).attr('href'));
            return false;
        });

        $('.excluir-detalhe').click(function(){
            href = $(this).attr('href');

            swal({
                    title: "Atenção!",
                    text: "Deseja realmente excluir o registro?",
                    type: "warning",
                    showCancelButton: true,
                    confirmButtonColor: "#DD6B55",
                    confirmButtonText: "Sim, estou certo!",
                    closeOnConfirm: true
                },
                function(){
                    $.ajax({
                        url: href,
                        success: function (proid) {
                            jQuery("#div_listagem_detalhe").load('?modulo=principal/proposta_form&acao=A&req=listar-detalhe&proid=' + proid);
                        }
                    });
                });
            return false;
        });
    });
</script>