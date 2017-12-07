<?php if(count($aBeneficiarioDetalhe)){ ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover bedidTables" >
            <thead>
            <tr class="text-center">
                <th width="10%">Ações</th>
                <th>benid</th>
                <th>emdid</th>
                <th>bedvalor</th>
                <th>bedvalorpriorizado</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($aBeneficiarioDetalhe as $dados){ ?>
                <tr>
                    <td class="text-center">
                        <a title="Alterar" class="editar-detalhe" href="?modulo=principal/beneficiario_form&acao=A&req=formulario-detalhe&bedid=<?php echo $dados['bedid']; ?>"><i class="fa fa-pencil"></i></a>
                        <a title="Excluir" class="excluir-detalhe" href="?modulo=principal/beneficiario_form&acao=A&req=excluir-detalhe&bedid=<?php echo $dados['bedid']; ?>" style="margin-left: 5px;"><i class="fa fa-close"></i></a>
                    </td>
                    <td><?php echo $dados['benid']; ?></td>
                    <td><?php echo $dados['emdid']; ?></td>
                    <td><?php echo $dados['bedvalor']; ?></td>
                    <td><?php echo $dados['bedvalorpriorizado']; ?></td>
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
                        }
                    });
                });
            return false;
        });
    })
</script>
