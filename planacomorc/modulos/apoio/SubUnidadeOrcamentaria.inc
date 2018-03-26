<?php if($lista): ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover" >
            <thead>
            <tr class="text-center">
                <th width="10%">Ações</th>
                <th>Sub-Unidade Orçamentaria</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($lista as $dados){?>
                <tr>
                    <td class="text-center">
                        <a title="Alterar" class="editar-medida" href="?modulo=apoio/unidadeOrcamentaria_form&acao=A&req=formulario-medida&suoid=<?php echo $dados['suoid']; ?>"><i class="fa fa-pencil"></i></a>
                        <?php if($podeEditar): ?>
                          <a title="excluir" name="req" value="excluir-medida" id="excluir-medida" href="?modulo=apoio/unidadeOrcamentaria_form&acao=A&req=excluir-medida&unoid=<?=$dados['unoid'];?>&suoid=<?=$dados['suoid'];?>" style="margin-left: 5px;"><i class="fa fa-close"></i></a>
                        <?php endif; ?>
                    </td>
                    <td><?php echo $dados['suonome']; ?></td>
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
        $('.editar-medida').click(function(){
            jQuery("#div_formulario_medida").load(jQuery(this).attr('href'));
            return false;
        });

        $('.excluir-medida').click(function(){
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
                        success: function (unoid) {
                            $("#div_listagem_medida").load('?modulo=apoio/unidadeOrcamentaria_form&acao=A&req=excluir-medida&suoid='+ suoid);
                        }
                    });
                });
            return false;
        });
    })
</script>
