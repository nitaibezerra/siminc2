<?php

    header( 'Content-Type: application/vnd.ms-excel' );
    header( 'Expires: 0' );
    header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
    header( 'Content-Disposition: attachment; filename="lista_de_emendas' . '.xls"' );

    # Colunas do relatório
    $cabecalho = [
        'Emenda',
        'Unidade',
        'Autor',
        'UF',
        'Id Beneficiário',
        'Beneficiário',
        'Subunidade',
        'Situação',
        'Nº Proposta',
        'Situação SICONV',
        'PI',
        'Valor',
        'Empenhado',
        'Pago'
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
        <?php if($aEmenda): ?>
            <tbody>
            <?php foreach($aEmenda as $dados): ?>
                <tr>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo substr($dados['emenumero'], 0, 4). ' '. substr($dados['emenumero'], 4); ?></td>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo $dados['unidade']; ?></td>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo $dados['autnome']; ?></td>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo $dados['estuf']; ?></td>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo $dados['benid']; ?></td>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo $dados['pronome']; ?></td>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo $dados['subunidade']; ?></td>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo $dados['esddsc']; ?></td>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo $dados['numero_proposta']; ?></td>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo $dados['sitdsc']; ?></td>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo $dados['pliid'] ?></td>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo formata_valor(($dados['custeio'] + $dados['capital'])); ?></td>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo formata_valor($dados['empenhado']); ?></td>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo formata_valor($dados['pago']); ?></td>
                </tr>
            <?php endforeach; ?>
            </tbody>
        <?php endif; ?>
    </tbody>
</table>

<?php
die;

