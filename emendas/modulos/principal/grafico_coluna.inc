<div class="row">
    <div class="col-md-6">
        <table class="table table-striped table-bordered table-hover" >
            <thead>
            <tr class="text-center">
                <th><?php echo $item; ?></th>
                <?php
                    $dadosTabela = current($dadosAgrupados);
                    if($dadosTabela):
                        foreach($dadosTabela as $categoria => $descricao):
                ?>
                    <th><?php echo $categoria; ?></th>
                <?php
                        endforeach;
                    endif;
                ?>
            </tr>
            </thead>
            <tbody>
            <?php
                $aTotal = [];
                if($dadosAgrupados):
                    foreach($dadosAgrupados as $agrupador => $dadosTabela):
            ?>
                <tr>
                    <td><?php echo $agrupador; ?></td>
                    <?php
                        if($dadosTabela):
                            foreach($dadosTabela as $key => $descricao):
                        $aTotal[$key] += $descricao;
                    ?>
                        <td width="30%" class="text-right"><?php echo number_format($descricao, 0, ',', '.'); ?></td>
                    <?php 
                            endforeach;
                        endif;
                    ?>
                </tr>
            <?php
                    endforeach;
                endif;
            ?>
            </tbody>
            <tr>
                <th>TOTAL</th>
                <?php
                    if($dadosTabela):
                        foreach($dadosTabela as $key => $descricao):
                ?>endforeach;
                    endif;
                    <th class="text-right"><?php echo number_format($aTotal[$key], 0, ',', '.'); ?></th>
                <?php 
                        endforeach;
                    endif;
                ?>
            </tr>
        </table>
    </div>
    <div class="col-md-6">
        <?php
        $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
        if($colors){ $grafico->setColors($colors); }
        $grafico->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->gerarGrafico($dados);
        unset($grafico);
        ?>
    </div>
</div>

