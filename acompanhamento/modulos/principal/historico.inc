<?php if(count($aHistorico)){ ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover bedidTables" >
            <thead>
            <tr class="text-center">
                <th>Data</th>
                <th>Usuário</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($aHistorico as $dados){ ?>
                <tr>
                    <td><?php echo $dados['hisdata']; ?></td>
                    <td><?php echo $dados['usunome']; ?></td>
                </tr>
            <?php } ?>
            </tbody>
        </table>
    </div>
<?php } else {
    echo '<div class="alert alert-danger">Nenhum histórico vinculado</div>';
} ?>