<?php
set_time_limit(50000);
ini_set("memory_limit", "5000M");
extract($_REQUEST);

$param = $_POST;
$where = array();
$where2 = array();

if ($param['obrid']) {
    $where[] = " pronumeroprocesso IN( SELECT DISTINCT pronumeroprocesso FROM obras2.vm_financeiro_obras WHERE obrid = {$param['obrid']} ) ";
}
if ($param['preid']) {
    $where[] = " pronumeroprocesso IN( SELECT DISTINCT pronumeroprocesso FROM obras2.vm_financeiro_obras WHERE preid = {$param['preid']} ) ";
}

if ($param['empesfera']) {
    $where[] = " empesfera = '" . $param['empesfera'] . "' ";
}

if ($param['estuf']) {
    $where[] = " estuf = '" . $param['estuf'] . "' ";
}

if ($param['muncod']) {
    $where[] = " muncod = '" . $param['muncod'] . "' ";
}

// Termo/Convenio
if ($param['convenio']) {
    $where[] = " termo_convenio = '{$param['convenio']}' ";
}
// Ano Termo/Convenio
if ($param['ano_convenio']) {
    $where[] = " ano_termo_convenio = '{$param['ano_convenio']}' ";
}

// processo
if ($param['processo']) {
    $where[] = "
                (
                    pronumeroprocesso = Replace(Replace(Replace( '{$param['processo']}','.',''),'/',''),'-','')
                )
            ";
}

$diff_de = (is_null($param['diff_de'])) ? 0 :  $param['diff_de'];
$diff_ate = (is_null($param['diff_ate'])) ? 5 :  $param['diff_ate'];


if ($param['diff_m'] && $param['diff_n'] && $param['diff_t']){
    $diff_de = 100;
    $diff_ate = 100;
}



if ($param['diff_m']) {
    $where2[] = " CASE WHEN diff IS NULL OR diff = 'N/A' THEN FALSE ELSE diff::float >= $diff_de AND diff::float <= $diff_ate END ";
}

if ($param['diff_n']) {
    $where2[] = " CASE WHEN diff IS NULL OR diff = 'N/A' THEN FALSE ELSE diff::float <= -$diff_de AND diff::float >= -$diff_ate END ";
}
if ($param['diff_t']) {
    $where2[] = " CASE WHEN diff IS NULL OR diff = 'N/A' THEN FALSE ELSE diff::float <= $diff_de AND diff::float >= -$diff_ate END ";
}

$processo = "'<a href=\"#\" onclick=\"pesquisaPorProcesso(\'' || pronumeroprocesso || '\')\"/> ' || pronumeroprocesso || ' </a>' as pronumeroprocesso,";
if ( $_REQUEST['pesquisa'] == 'xls' ){
    $processo = " pronumeroprocesso,";
}


$sql = "

    SELECT
        {$processo}
        termo_convenio || '/' || ano_termo_convenio as termo_convenio,
        origem,
        CASE WHEN empesfera = 'M' THEN 'Municipal' WHEN empesfera = 'E' THEN 'Estadual' END as empesfera,
        estuf,
        mundescricao,
        qtdObras,
        formataMonetario(tot_valor_pactuado) AS tot_valor_pactuado,
        formataMonetario(tot_valor_contratado) AS tot_valor_contratado,
        formataMonetario(tot_valor_repassado) AS tot_valor_repassado,
        agencia,
        conta,
        formataMonetario(saldo) as saldo,
        referencia_saldo,
        CASE WHEN fisico_ponderado = 'N/A' OR fisico_ponderado IS NULL THEN '0 ' ELSE formataMonetario(fisico_ponderado::numeric) END as fisico_ponderado,
        CASE WHEN financeiro_processo = 'N/A' OR financeiro_processo IS NULL THEN '0 ' ELSE formataMonetario(financeiro_processo::numeric) END as financeiro_processo,
        CASE WHEN diff = 'N/A' OR diff IS NULL THEN '0 ' ELSE formataMonetario(trunc(diff::numeric, 2))  END as diff
    FROM obras2.vm_fisico_financeiro_processo
    WHERE 1 = 1 " . (count($where) ? " AND " . implode(' AND ', $where) : "") . "
     " . (count($where2) ? " AND (" . implode(' OR ', $where2) . ")" : "") . "
";
$cabecalho = array("Nº Processo", "Nº Termo de Compromisso/Convênio", "Fonte", "Esfera", "UF", "Município", "Qtd Obras", "Valor Pactuado", "Valor Contratado", "Valor Repassado FNDE", "Agência", "Conta-Corrente", "Saldo em Conta", "Referência a Saldo", "% Físico do Processo", "% Financeiro do Processo","Situação do Equilíbrio Físico-Financeiro");
// Gera o XLS do relatório
//ver($sql, d);
if ( $_REQUEST['pesquisa'] == 'xls' ){
    $db->sql_to_xml_excel($sql, 'Relatorio_Fisico_Financeiro', $cabecalho, '');
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<html>
<head>
    <title> <?php echo NOME_SISTEMA; ?> </title>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
</head>
<body>
<center>
    <!--  Cabeçalho Brasão -->
    <?php echo monta_cabecalho_relatorio( '95' ); ?>
</center>
</body>
</html>
<?php
//ver($sql, d);
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', '','');
?>

<script type="text/javascript">
    function pesquisaPorProcesso(processo) {
        var janela = window.open('obras2.php?modulo=relatorio/relatorioControleFinanceiro&acao=A&form=true&nprocesso=' + processo, 'RelatorioFinanceiro', 'width=1150,height=700,status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no,fullscreen=yes');
        janela.focus();
        return false;
    }
</script>