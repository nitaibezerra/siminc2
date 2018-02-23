<?php

    header( 'Content-Type: application/vnd.ms-excel' );
    header( 'Expires: 0' );
    header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
    header( 'Content-Disposition: attachment; filename="relatorio_geral_pi' . '.xls"' );

    # Colunas do relatório
    $cabecalho = [
        'Id',
        'Código',
        'Unidade',
        'Título',
        'Funcional',
        'Situação',
        'Custeio',
        'Capital',
        'Provisionado',
        'Empenhado',
        'Liquidado',
        'Pago',
    ];

?>

<table width="100%" cellspacing="0">
    <thead>
        <tr>
            <?php foreach($cabecalho as $coluna){ ?>
                <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4"><?php echo $coluna; ?></th>
            <?php } ?>
        </tr>
    </thead>
    <tbody>
        <?php if($listaPi): ?>
            <tbody>
            <?php foreach($listaPi as $pi): ?>
                <tr>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo $pi['id']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo $pi['codigo_pi']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo $pi['sub_unidade']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo $pi['plititulo']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo $pi['funcional']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo $pi['situacao']; ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo formata_valor($pi['custeio']); ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo formata_valor($pi['capital']); ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo formata_valor($pi['autorizado']); ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo formata_valor($pi['empenhado']); ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo formata_valor($pi['liquidado']); ?>
                    </td>
                    <td style="border: 1px #e5d9c5 solid;">
                        <?php echo formata_valor($pi['pago']); ?>
                    </td>
                </tr>
            <?php endforeach; ?>
            </tbody>
        <?php endif; ?>
    </tbody>
</table>

<?php
die;

