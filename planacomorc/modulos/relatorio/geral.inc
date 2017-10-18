<?php
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

$cabecalho = [
    'ID Planejamento', 'Situação', 'Código', 'Código da UO', 'UO', 'Código da Unidade', 'Unidade', 'Unidade Delegada', 'Unidade Delegada', 'Enquadramento',
    'Item de Manutenção', 'Subitem de Manutenção', 'Código do Programa', 'Programa', 'Código do Objetivo', 'Objetivo', 'Código da Iniciativa', 'Iniciativa', 'Código da Ação', 'Ação', 'Código do Localizador',
    'Localizador', 'Código do PO', 'Plano Orçamentário', 'PTRES', 'Área', 'Meta PNC', 'Meta PPA', 'Título', 'Descrição', 'Segmento Cultural', 'Tipo de Instrumento / Modalidade de Pactuação',
    'Emenda parlamentar', 'Forma de execução - Edital', 'Localização da ação', 'País', 'Unidade Federativa',
    'Município', 'Produto PI', 'Unidade de Medida', 'Meta Física', 'Valor Total', 'Valor Total Custeio', 'Valor Total Capital',

    'Cronograma Físico<br />Capital Janeiro', 'Cronograma Físico<br />Capital Fevereivo', 'Cronograma Físico<br />Capital Março', 'Cronograma Físico<br />Capital Abril',
    'Cronograma Físico<br />Capital Maio', 'Cronograma Físico<br />Capital Junho', 'Cronograma Físico<br />Capital Julho', 'Cronograma Físico<br />Capital Agosto',
    'Cronograma Físico<br />Capital Setembro', 'Cronograma Físico<br />Capital Outubro', 'Cronograma Físico<br />Capital Novembro', 'Cronograma Físico<br />Capital Dezembro',

    'Cronograma Financeiro<br />Capital Janeiro', 'Cronograma Financeiro<br />Capital Fevereivo', 'Cronograma Financeiro<br />Capital Março', 'Cronograma Financeiro<br />Capital Abril',
    'Cronograma Financeiro<br />Capital Maio', 'Cronograma Financeiro<br />Capital Junho', 'Cronograma Financeiro<br />Capital Julho', 'Cronograma Financeiro<br />Capital Agosto',
    'Cronograma Financeiro<br />Capital Setembro', 'Cronograma Financeiro<br />Capital Outubro', 'Cronograma Financeiro<br />Capital Novembro', 'Cronograma Financeiro<br />Capital Dezembro',

    'Cronograma Financeiro<br />Custeio Janeiro', 'Cronograma Financeiro<br />Custeio Fevereivo', 'Cronograma Financeiro<br />Custeio Março', 'Cronograma Financeiro<br />Custeio Abril',
    'Cronograma Financeiro<br />Custeio Maio', 'Cronograma Financeiro<br />Custeio Junho', 'Cronograma Financeiro<br />Custeio Julho', 'Cronograma Financeiro<br />Custeio Agosto',
    'Cronograma Financeiro<br />Custeio Setembro', 'Cronograma Financeiro<br />Custeio Outubro', 'Cronograma Financeiro<br />Custeio Novembro', 'Cronograma Financeiro<br />Custeio Dezembro',

    'Cronograma Orçamentário<br />Capital Janeiro', 'Cronograma Orçamentário<br />Capital Fevereivo', 'Cronograma Orçamentário<br />Capital Março', 'Cronograma Orçamentário<br />Capital Abril',
    'Cronograma Orçamentário<br />Capital Maio', 'Cronograma Orçamentário<br />Capital Junho', 'Cronograma Orçamentário<br />Capital Julho', 'Cronograma Orçamentário<br />Capital Agosto',
    'Cronograma Orçamentário<br />Capital Setembro', 'Cronograma Orçamentário<br />Capital Outubro', 'Cronograma Orçamentário<br />Capital Novembro', 'Cronograma Orçamentário<br />Capital Dezembro',

    'Cronograma Orçamentário<br />Custeio Janeiro', 'Cronograma Orçamentário<br />Custeio Fevereivo', 'Cronograma Orçamentário<br />Custeio Março', 'Cronograma Orçamentário<br />Custeio Abril',
    'Cronograma Orçamentário<br />Custeio Maio', 'Cronograma Orçamentário<br />Custeio Junho', 'Cronograma Orçamentário<br />Custeio Julho', 'Cronograma Orçamentário<br />Custeio Agosto',
    'Cronograma Orçamentário<br />Custeio Setembro', 'Cronograma Orçamentário<br />Custeio Outubro', 'Cronograma Orçamentário<br />Custeio Novembro', 'Cronograma Orçamentário<br />Custeio Dezembro',

    'Valor Empenhado', 'Valor Liquidado', 'Valor Pago',
];

$dados = (new Pi_PlanoInterno())->getDadosGerais($_SESSION['exercicio']);

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
        <?php foreach($dados as $dado){ ?>
            <tr>
                <?php foreach($dado as $valor){ ?>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo $valor; ?></td>
                <?php } ?>
            </tr>
        <?php } ?>
    </tbody>
<?php

header( 'Content-Type: application/vnd.ms-excel' );
header( 'Expires: 0' );
header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
header( 'Content-Disposition: attachment; filename="siminc_detalhamentos_' . date("YmdHi") . '.xls"' );
die;