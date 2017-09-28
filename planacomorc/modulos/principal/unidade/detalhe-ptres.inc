<?php
$dado = count($dados) ? current($dados) : [];
?>


<div class="row">
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5><?php echo count($dado) ? $dado['funcional'] . ' - ' . $dado['acatitulo'] : "PI's da Funcional"; ?></h5>
            </div>
            <div class="ibox-content">
                <?php if(count($dados)){ ?>
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover " >
                            <thead>
                            <tr class="text-center">
                                <th>Id</th>
                                <th>Código</th>
                                <th>Título</th>
                                <th>Subunidade</th>
                                <th>Enquadramento</th>
                                <th>Custeio</th>
                                <th>Capital</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($dados as $dado){ ?>
                                <tr>
                                    <td><?php echo $dado['pliid']; ?></td>
                                    <td><?php echo $dado['plicod']; ?></td>
                                    <td><?php echo $dado['plititulo']; ?></td>
                                    <td><?php echo $dado['unosigla'] . ' - ' . $dado['suonome'] . ' (' . $dado['suocod'] . ')'; ?></td>
                                    <td><?php echo $dado['eqddsc']; ?></td>
                                    <td class="text-right"><?php echo number_format2($dado['picvalorcusteio'], 0, ',', '.'); ?></td>
                                    <td class="text-right"><?php echo number_format2($dado['picvalorcapital'], 0, ',', '.'); ?></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>
                <?php } else {
                    echo '<div class="alert alert-danger">Não há PI vinculado a esta Funcional</div>';
                } ?>
            </div>
        </div>
    </div>
</div>