<?php

$listaHistorico = (new Planacomorc_Model_HistoricoPi())->recuperarListagem($pliid);

?>

<div class="row">
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Histórico de alteraçõesss</h5>
            </div>
            <div class="table-responsive">
                <table class="table table-bordered table-hover dataTables">
                    <thead>
                    <tr class="text-center">
                        <th>Data</th>
                        <th>Usuário</th>
                        <th>Código Antigo</th>
                        <th>Código Novo</th>
                    </tr>
                    </thead>
                    <tbody>
                    <?php foreach ($listaHistorico as $dados) { ?>
                        <tr>
                            <td><?= $dados['hisdata'] ?></td>
                            <td><?= $dados['usucpf'] ?></td>
                            <td><?= $dados['hpidscantigo'] ?></td>
                            <td><?= $dados['hpidscnovo'] ?></td>
                        </tr>
                    <?php } ?>
                    </tbody>
                </table>
            </div>
        </div>
    </div>
</div>