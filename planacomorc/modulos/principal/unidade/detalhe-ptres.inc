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
                                <th>Total</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php
                            $total['custeio'] = $total['capital'] = 0;
                            foreach($dados as $dado){
                                $total['custeio'] += $dado['picvalorcusteio'];
                                $total['capital'] += $dado['picvalorcapital'];
                                ?>
                                <tr>
                                    <td><?php echo $dado['pliid']; ?></td>
                                    <td><?php echo $dado['plicod']; ?></td>
                                    <td><?php echo $dado['plititulo']; ?></td>
                                    <td><?php echo $dado['unosigla'] . ' - ' . $dado['suonome'] . ' (' . $dado['suocod'] . ')'; ?></td>
                                    <td><?php echo $dado['eqddsc']; ?></td>
                                    <td class="text-right"><?php echo number_format($dado['picvalorcusteio'], 0, ',', '.'); ?></td>
                                    <td class="text-right"><?php echo number_format($dado['picvalorcapital'], 0, ',', '.'); ?></td>
                                    <td class="text-right"><?php echo number_format(($dado['picvalorcusteio'] + $dado['picvalorcapital']), 0, ',', '.'); ?></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                            <tfooter>
                                <tr>
                                    <th colspan="5">TOTAL</th>
                                    <th class="text-right"><?php echo number_format($total['custeio'], 0, ',', '.'); ?></th>
                                    <th class="text-right"><?php echo number_format($total['capital'], 0, ',', '.'); ?></th>
                                    <th class="text-right"><?php echo number_format(($total['custeio'] + $total['capital']), 0, ',', '.'); ?></th>
                                </tr>
                            </tfooter>
                        </table>
                    </div>
                <?php } else {
                    echo '<div class="alert alert-danger">Não há PI vinculado a esta Funcional</div>';
                } ?>
            </div>
        </div>
    </div>
</div>