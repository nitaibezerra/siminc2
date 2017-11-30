<?php if(count($aBeneficiarioHistorico)){ ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover" >
            <thead>
            <tr class="text-center">
                <th width="10%">Ações</th>
                <th>behdsc</th>
                <th>behdata</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($aBeneficiarioHistorico as $dados){ ?>
                <tr>
                    <td class="text-center">
                        <a title="Alterar" class="editar-historico" href="?modulo=principal/beneficiario_form&acao=A&req=formulario-historico&behid=<?php echo $dados['behid']; ?>"><i class="fa fa-pencil"></i></a>
                        <a title="Excluir" class="excluir-historico" href="?modulo=principal/beneficiario_form&acao=A&req=excluir-historico&behid=<?php echo $dados['behid']; ?>" style="margin-left: 5px;"><i class="fa fa-close"></i></a>
                    </td>
                    <td><?php echo substr($dados['behdsc'], 0, 100); ?></td>
                    <td><?php echo formata_data($dados['behdata']); ?></td>
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
        $('.editar-historico').click(function(){
            jQuery("#div_formulario_historico").load(jQuery(this).attr('href'));
            return false;
        });

        $('.excluir-historico').click(function(){
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
                            jQuery("#div_listagem_historico").load('?modulo=principal/beneficiario_form&acao=A&req=listar-historico&benid=' + benid);
                        }
                    });
                });
            return false;
        });
    })
</script>