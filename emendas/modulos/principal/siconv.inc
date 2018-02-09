<?php if(count($aSiconv)){ ?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover" >
            <thead>
            <tr class="text-center">
                <th colspan="2">Programa</th>
                <th colspan="2">Proposta</th>
                <th rowspan="2">Sequencial</th>
                <th rowspan="2">Nº Convênio</th>
                <th rowspan="2">Situação</th>
            </tr>
            <tr class="text-center">
                <th>Título</th>
                <th>Objeto</th>
                <th>Título</th>
                <th>Objeto</th>
            </tr>
            </thead>
            <tbody>
            <?php foreach($aSiconv as $dados){ ?>
                <tr>
                    <td></td>
                    <td></td>
                    <td></td>
                    <td><?php echo $dados['sicobjeto']; ?></td>
                    <td class="text-center"><?php echo $dados['sicsequencial']; ?></td>
                    <td class="text-center"><?php echo $dados['numeroconvenio']; ?></td>
                    <td><?php echo $dados['sitdsc']; ?></td>
                </tr>
            <?php } ?>
            </tbody>
        </table>
    </div>
<?php } else {
    echo '<div class="alert alert-danger">Nenhum registro encontrado no SICONV</div>';
} ?>