<?php if(count($aBeneficiarioDetalhe)){ ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover bedidTables" >
            <thead>
            <tr class="text-center">
                <th width="10%">Ações</th>
                <th>GND</th>
                <th>Vl. Emenda</th>
                <th>Vl. Beneficiário</th>
                <th>Vl. Priorizado</th>
                <th>Vl. Contingenciado</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($aBeneficiarioDetalhe as $dados){ ?>
                <tr>
                    <td class="text-center">
                        <?php if($podeEditar){ ?>
                        <a title="Alterar" class="editar-detalhe" href="?modulo=principal/beneficiario_form&acao=A&req=formulario-detalhe&bedid=<?php echo $dados['bedid']; ?>&emeid=<?php echo $dados['emeid']; ?>"><i class="fa fa-pencil"></i></a>
                        <a title="Excluir" class="excluir-detalhe" href="?modulo=principal/beneficiario_form&acao=A&req=excluir-detalhe&bedid=<?php echo $dados['bedid']; ?>&emeid=<?php echo $dados['emeid']; ?>" style="margin-left: 5px;"><i class="fa fa-close"></i></a>
                       <?php } ?>
                    </td>
                    <td><?php echo $dados['gndcod']; ?></td>
                    <td class="text-right"><?php echo number_format($dados['emdvalor'], 2, ',', '.'); ?></td>
                    <td class="text-right"><?php echo number_format($dados['bedvalor'], 2, ',', '.'); ?></td>
                    <td class="text-right"><?php echo number_format($dados['bedvalorpriorizado'], 2, ',', '.'); ?></td>
                    <td class="text-right"><?php echo number_format(($dados['bedvalor'] - $dados['bedvalorpriorizado']), 2, ',', '.'); ?></td>
                </tr>
            <?php } ?>
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
                        success: function (benid) {
                            jQuery("#div_listagem_detalhe").load('?modulo=principal/beneficiario_form&acao=A&req=listar-detalhe&benid=' + benid);
                            jQuery("#div_formulario_detalhe").load('?modulo=principal/beneficiario_form&acao=A&req=formulario-detalhe&benid=' + benid + '&emeid='+$('#emeid').val());
                        }
                    });
                });
            return false;
        });
    })
</script>
