<table class="table table-striped table-bordered table-hover dataTablesPg5" >
    <thead>
    <tr class="text-center">
        <th width="10%">Ações</th>
        <th>Código</th>
        <th>Meta</th>
        <th>Ano</th>
        <th>Quantificavel</th>
        <th>Quantidade</th>
    </tr>
    </thead>
    <tbody>
    <?php foreach($aMetappa as $dados){ ?>
        <tr>
            <td class="text-center">
                <a title="Alterar" href="javascript:alterarCamposMeta(<?= $dados['mppid'] ?>,'<?= $dados['mppcod'] ?>','<?= $dados['mppnome'] ?>','<?= $dados['mppdsc'] ?>','<?= $dados['prsano'] ?>','<?= $dados['mppquantificavel'] ?>','<?= $dados['mppanualizavel'] ?>','<?= $dados['mppquantidade'] ?>');"><i class="fa fa-pencil"></i></a>
            </td>
            <td><?php echo $dados['mppcod']; ?></td>
            <td><?php echo $dados['mppnome']; ?></td>
            <td><?php echo $dados['prsano']; ?></td>
            <td><?php echo $dados['mppquantificavel'] == 't'? 'Sim': 'Não'; ?></td>
            <td><?php echo $dados['mppquantidade']; ?></td>
        </tr>
    <?php } ?>
    </tbody>
</table>