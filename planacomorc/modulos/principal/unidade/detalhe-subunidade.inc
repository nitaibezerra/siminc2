<?php
$dado = count($dadosPi) ? current($dadosPi) : [];
?>


<div class="row">
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5><?php echo count($dado) ? $dado['unosigla'] . ' - ' . $dado['suonome'] . ' (' . $dado['suocod'] . ')' : "PI's da Unidade"; ?></h5>
            </div>
            <div class="ibox-content">
                <?php if(count($dadosPi)){ ?>
                    <div class="table-responsive">
                        <table class="table table-bordered table-striped table-hover " >
                            <thead>
                            <tr class="text-center">
                                <th>Id</th>
                                <th>Código</th>
                                <th>Título</th>
                                <th>Funcional</th>
                                <th>Enquadramento</th>
                                <th>Custeio</th>
                                <th>Capital</th>
                                <th>Total</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php
                            $total['custeio'] = $total['capital'] = 0;
                            foreach($dadosPi as $dado){
                                $total['custeio'] += $dado['picvalorcusteio'];
                                $total['capital'] += $dado['picvalorcapital'];
                                ?>
                                <tr>
                                    <td><?php echo $dado['pliid']; ?></td>
                                    <td><?php echo $dado['plicod']; ?></td>
                                    <td><?php echo $dado['plititulo']; ?></td>
                                    <td><?php echo $dado['funcional']; ?></td>
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
                    echo '<div class="alert alert-danger">Não há PI vinculado a esta Unidade</div>';
                } ?>
            </div>
        </div>
    </div>
    <div class="col-md-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>PI's Delegados</h5>
            </div>
            <div class="ibox-content">
                <?php if(count($dadosDelegado)){ ?>
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover " >
                            <thead>
                            <tr class="text-center">
                                <th>Id</th>
                                <th>Código</th>
                                <th>Título</th>
                                <th>Funcional</th>
                                <th>Enquadramento</th>
                                <th>Custeio</th>
                                <th>Capital</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($dadosDelegado as $dado){ ?>
                                <tr>
                                    <td><?php echo $dado['pliid']; ?></td>
                                    <td><?php echo $dado['plicod']; ?></td>
                                    <td><?php echo $dado['plititulo']; ?></td>
                                    <td><?php echo $dado['funcional']; ?></td>
                                    <td><?php echo $dado['eqddsc']; ?></td>
                                    <td class="text-right"><?php echo number_format2($dado['picvalorcusteio'], 0, ',', '.'); ?></td>
                                    <td class="text-right"><?php echo number_format2($dado['picvalorcapital'], 0, ',', '.'); ?></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>
                <?php } else {
                    echo '<div class="alert alert-danger">Não há PI delegado a esta Unidade</div>';
                } ?>
            </div>
        </div>
    </div>
</div>