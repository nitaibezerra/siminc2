<?php if(count($aEmendadetalhe)){ ?>
    <table class="table table-striped table-bordered table-hover" >
        <thead>
        <tr class="text-center">
            <th width="10%">Ações</th>
            <th>GND</th>
            <th>Mod. Aplic.</th>
            <th>Fonte</th>
            <th>Valor</th>
        </tr>
        </thead>
        <tbody>
        <?php foreach($aEmendadetalhe as $dados){ ?>
            <tr>
                <td class="text-center">
                    <a title="Alterar" class="editar-detalhe" href="?modulo=principal/emenda_form&acao=A&req=formulario-detalhe&emdid=<?php echo $dados['emdid']; ?>"><i class="fa fa-pencil"></i></a>
                    <a title="Excluir" class="excluir-detalhe" href="?modulo=principal/emenda_form&acao=A&req=excluir-detalhe&emdid=<?php echo $dados['emdid']; ?>" style="margin-left: 5px;"><i class="fa fa-close"></i></a>
                </td>
                <td class="text-center"><?php echo $dados['gndcod']; ?></td>
                <td class="text-center"><?php echo $dados['mapcod']; ?></td>
                <td class="text-center"><?php echo $dados['foncod']; ?></td>
                <td class="text-right"><?php echo formata_valor($dados['emdvalor']); ?></td>
            </tr>
        <?php } ?>
        </tbody>
    </table>
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
                        success: function (emeid) {

                            alert(emeid);

                            jQuery("#div_listagem_detalhe").load('?modulo=principal/emenda_form&acao=A&req=listar-detalhe&emeid=' + emeid);
                        }
                    });
                });
            return false;
        });
    })
</script>