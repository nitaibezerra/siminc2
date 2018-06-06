<?php if($lista): ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover" >
            <thead>
            <tr class="text-center">
                <th width="10%">Ações</th>
                <th>Sub-item</th>
                <th>Status</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($lista as $dados){?>
                <tr>
                    <td class="text-center">
                        <a title="Alterar" class="editar-subitem" href="?modulo=apoio/item-manutencao-form&acao=A&req=formulario-subitem&masid=<?php echo $dados['masid']; ?>"><i class="fa fa-pencil"></i></a>
                        <?php if($podeEditar): ?>
                          <a title="excluir" name="req" value="excluir-subitem" id="excluir-subitem" href="?modulo=apoio/item-manutencao-form&acao=A&req=excluir-subitem&maiid=<?=$dados['maiid'];?>&masid=<?=$dados['masid'];?>" style="margin-left: 5px;"><i class="fa fa-close"></i></a>
                        <?php endif; ?>
                    </td>
                    <td><?php echo $dados['masnome']; ?></td>
                    <td><?php echo $dados['masstatus'] == 'A' ? 'Ativo' : 'Inativo'; ?></td>
                </tr>
            <?php } ?>
            </tbody>
        </table>
    </div>
<?php else: ?>
    <div class="alert alert-danger">Nenhum registro cadastrado</div>
<?php endif; ?>

<script>
    $(function(){
        $('.editar-subitem').click(function(){
            jQuery("#div_formulario_subitem").load(jQuery(this).attr('href'));
            return false;
        });

        $('.excluir-subitem').click(function(){
            href = $(this).attr('href');
// alert(href);
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
                        success: function (maiid) {
                            $("#div_listagem_subitem").load('?modulo=apoio/item-manutencao-form&acao=A&req=excluir-subitem&masid='+ masid);
                        }
                    });
                });
            return false;
        });
    })
</script>
