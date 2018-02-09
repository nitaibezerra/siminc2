<?php if($listaMedida): ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover" >
            <thead>
            <tr class="text-center">
                <th width="10%">Ações</th>
                <th>Medida</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($listaMedida as $dados){ ?>
                <tr>
                    <td class="text-center">
                        <a title="Alterar" class="editar-medida" href="?modulo=apoio/classificacao_form&acao=A&req=formulario-medida&medid=<?php echo $dados['medid']; ?>"><i class="fa fa-pencil"></i></a>
                        <?php if($podeEditar): ?>
                            <a title="Excluir" class="excluir-medida" href="?modulo=apoio/classificacao_form&acao=A&req=excluir-medida&medid=<?php echo $dados['medid']; ?>" style="margin-left: 5px;"><i class="fa fa-close"></i></a>
                        <?php endif; ?>
                    </td>
                    <td><?php echo $dados['meddsc']; ?></td>
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
//alert(href);
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
                        success: function (claid) {
                            $("#div_listagem_medida").load('?modulo=apoio/classificacao_form&acao=A&req=listar-medida&claid='+ claid);
                        }
                    });
                });
            return false;
        });
    })
</script>