<?php

    header( 'Content-Type: application/vnd.ms-excel' );
    header( 'Expires: 0' );
    header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
    header( 'Content-Disposition: attachment; filename="relatorio_proposta_painel_funcional'. '.xls"');

?>

<table width="100%" cellspacing="0">
    <thead>
        <tr class="text-center">
            <th rowspan="2" style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Unidade</th>
            <th rowspan="2" style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Funcional</th>
            <th colspan="2" style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Custeio</th>
            <th colspan="2" style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Capital</th>
            <th colspan="2" style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Total</th>                     
        </tr>
        <tr class="text-center">
            <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Pré-PI</th>
            <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Detalhado</th>
            <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Pré-PI</th>
            <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Detalhado</th>
            <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Pré-PI</th>
            <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4">Detalhado</th>
        </tr>
    </thead>
    <tbody>
        <?php if($listaFuncional): ?>
            <tbody>
            <?php foreach($listaFuncional as $funcional): ?>
                <tr>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo $funcional['unosigla'] . ' - ' . $funcional['suonome'] . ' (' . $funcional['suocod'] . ')'; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo $funcional['funcional'] . ' - ' . $funcional['acatitulo'] . ': ' . $funcional['plodsc'] . ' (RP ' . $funcional['irpcod'] . ')'; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo formata_valor($funcional['ptrdotacao_custeio']); ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo formata_valor($funcional['picvalorcusteio']); ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo formata_valor($funcional['ptrdotacao_capital']); ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo formata_valor($funcional['picvalorcapital']); ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo formata_valor(($funcional['ptrdotacao_custeio'] + $funcional['ptrdotacao_capital'])); ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo formata_valor(($funcional['picvalorcusteio'] + $funcional['picvalorcapital'])); ?>
                    </td>
                </tr>
            <?php endforeach; ?>
            </tbody>
        <?php endif; ?>
    </tbody>
</table>

<?php
die;
