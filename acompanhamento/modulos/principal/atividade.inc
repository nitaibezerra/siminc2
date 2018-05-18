<?php if(count($aAtividade)){ ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover dataTables" >
            <thead>
            <tr class="text-center">
                <th width="10%">Ações</th>
                <th>atidsc</th>
                <th>atidata</th>
                <th>atiproduto</th>
                <th>atiquantidade</th>
                <th>estuf</th>
                <th>muncod</th>
                <th>mpnid</th>
                <th>suoid</th>
                <th>janid</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($aAtividade as $dados){ ?>
                <tr>
                    <td class="text-center">
                        <a title="Alterar" class="editar-atividade" href="?modulo=principal/monitorar-pnc&acao=A&req=formulario-atividade&atiid=<?php echo $dados['atiid']; ?>"><i class="fa fa-pencil"></i></a>
                        <a title="Excluir" class="excluir-atividade" href="?modulo=principal/monitorar-pnc&acao=A&req=excluir-atividade&atiid=<?php echo $dados['atiid']; ?>" style="margin-left: 5px;"><i class="fa fa-close"></i></a>
                    </td>
                    <td><?php echo $dados['atidsc']; ?></td>
                    <td><?php echo $dados['atidata']; ?></td>
                    <td><?php echo $dados['atiproduto']; ?></td>
                    <td><?php echo $dados['atiquantidade']; ?></td>
                    <td><?php echo $dados['estuf']; ?></td>
                    <td><?php echo $dados['muncod']; ?></td>
                    <td><?php echo $dados['mpnid']; ?></td>
                    <td><?php echo $dados['suoid']; ?></td>
                    <td><?php echo $dados['janid']; ?></td>
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
        $('.editar-atividade').click(function(){
            jQuery("#div_formulario_atividade").load(jQuery(this).attr('href'));
            return false;
        });

        $('.excluir-atividade').click(function(){
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
                            jQuery("#div_listagem_atividade").load('?modulo=principal/monitorar-pnc&acao=A&req=listar-atividade&benid=' + benid);
                        }
                    });
                });
            return false;
        });
    })
</script>
