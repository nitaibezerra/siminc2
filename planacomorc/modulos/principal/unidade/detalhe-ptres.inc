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
                                <th>Ações</th>
                                <th>Id</th>
                                <th>Código</th>
                                <th>Título</th>
<!--                                <th>Unidade</th>
                                <th>Enquadramento</th>-->
                                <th>Custeio</th>
                                <th>Capital</th>
                                <!--<th>Total</th>-->
                                <th>Empenhado</th>
                                <th>Pago</th>
                                <th>Liquidado</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php
                            $total['custeio'] = $total['capital'] = $total['empenhado'] = $total['pago'] = $total['liquidado'] = 0;
                            foreach($dados as $dado){
                                $total['custeio'] += $dado['picvalorcusteio'];
                                $total['capital'] += $dado['picvalorcapital'];
                                $total['empenhado'] += $dado['empenhado'];
                                $total['pago'] += $dado['pago'];
                                $total['liquidado'] += $dado['liquidado'];
                                ?>
                                <tr>
                                    <td class="text-center"><a href="planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A&pliid=<?php echo $dado['pliid']; ?>" class="btn btn-primary btn-sm glyphicon glyphicon-pencil"></a></td>
                                    <td><?php echo $dado['pliid']; ?></td>
                                    <td><?php echo $dado['plicod']; ?></td>
                                    <td><?php echo $dado['plititulo']; ?></td>
<!--                                    <td><?php echo $dado['unosigla'] . ' - ' . $dado['suonome'] . ' (' . $dado['suocod'] . ')'; ?></td>
                                    <td><?php echo $dado['eqddsc']; ?></td>-->
                                    <td class="text-right"><?php echo number_format($dado['picvalorcusteio'], 0, ',', '.'); ?></td>
                                    <td class="text-right"><?php echo number_format($dado['picvalorcapital'], 0, ',', '.'); ?></td>
<!--                                    <td class="text-right"><?php echo number_format(($dado['picvalorcusteio'] + $dado['picvalorcapital']), 0, ',', '.'); ?></td>-->
                                    <td class="text-right"><?php echo number_format($dado['empenhado'], 0, ',', '.'); ?></td>
                                    <td class="text-right"><?php echo number_format($dado['pago'], 0, ',', '.'); ?></td>
                                    <td class="text-right"><?php echo number_format($dado['liquidado'], 0, ',', '.'); ?></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                            <tfooter>
                                <tr>
                                    <th colspan="4">TOTAL</th>
                                    <th class="text-right"><?php echo number_format($total['custeio'], 0, ',', '.'); ?></th>
                                    <th class="text-right"><?php echo number_format($total['capital'], 0, ',', '.'); ?></th>
<!--                                    <th class="text-right"><?php echo number_format(($total['custeio'] + $total['capital']), 0, ',', '.'); ?></th>-->
                                    <th class="text-right"><?php echo number_format($total['empenhado'], 0, ',', '.'); ?></th>
                                    <th class="text-right"><?php echo number_format($total['pago'], 0, ',', '.'); ?></th>
                                    <th class="text-right"><?php echo number_format($total['liquidado'], 0, ',', '.'); ?></th>
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