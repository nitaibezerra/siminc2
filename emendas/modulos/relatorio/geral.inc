<?php

    header( 'Content-Type: application/vnd.ms-excel' );
    header( 'Expires: 0' );
    header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
    header( 'Content-Disposition: attachment; filename="relatorio_geral_pi' . '.xls"' );

    $cabecalho = [
        'ID Emenda',
        'Número',
        'Cod. Autor',
        'Autor',
        'Cod. Unidade',
        'Unidade',
        'Funcional',
        'Impositiva',
        'Justificativa',
        'GND',
        'Mod. Aplicação',
        'Fonte',
        'Valor',
        # BENEFICIARIOS
        'Id Beneficiario',
        'Código Subunidade',
        'Subunidade',
        'Código Subunidade delegada',
        'Subunidade delegada',
        'CNPJ',
        'Proponente',
        'Localização',
        'Paises',
        'UFs',
        'Municipios',
        'Número do Processo',
        'Data de Início da Vigência',
        'Modalidade de Pactuação',
        'TED',
        'Área Cultural',
        'Segmento Cultural',
        'Alterações',
        'Parecer Técnico',
        'Parecer Jurídico',
        'Impedimento',
        'Motivo',
        'Justivicativa',
        'Situação',
        'Empenhado',
        'Pago',
        
//        'Situação SICONV'
    ];

    # Lista de colunas que terão formatação de moeda.
    $listaColunaMoeda = array(
        'emdvalor',
        'empenhado',
        'pago'
    );

    $sql = (new Emendas_Model_Emenda)->montarSqlRelatorioGeral($_SESSION['exercicio']);
    $lista = $db->carregar($sql);
//ver($lista, d);
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
        <?php foreach($lista as $dado){ ?>
            <tr>
                <?php foreach($dado as $coluna => $valor){ ?>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo in_array($coluna, $listaColunaMoeda)? formata_valor($valor): $valor; ?></td>
                <?php } ?>
            </tr>
        <?php } ?>
    </tbody>
</table>

<?php
die;
