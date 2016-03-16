<?php
/**
 * Listagem de momentos de crédito.
 * $Id: listarMomentoCredito.inc 84475 2014-08-12 12:43:23Z Kamylasakamoto $
 */
$params = array($_SESSION['exercicio']);
$whereAdicional = array();

if (chaveTemValor($formData, 'mcrdsc')) {
    $whereAdicional[] = "mcr.mcrdsc ILIKE '%%%s%%'";
    $params[] = str_replace("'", "''", $formData['mcrdsc']);
}
if (chaveTemValor($formData, 'mcrcod')) {
    $whereAdicional[] = "mcr.mcrcod ILIKE '%%%s%%'";
    $params[] = str_replace("'", "''", $formData['mcrcod']);
}
if (chaveTemValor($formData, 'tcrid')) {
    $sql = <<<DML
EXISTS (SELECT 1
          FROM altorc.momentotipocredito mtc
          WHERE mtc.mcrid = mcr.mcrid
            AND mtc.tcrid IN(
DML;
    $params = array_merge($params, $formData['tcrid']);
    $sql .= implode(', ', array_fill(0, count($formData['tcrid']), '%d')) . '))';
    $whereAdicional[] = $sql;
}

$datas = array('mcrrefinicio', 'mcrreffim', 'mcrincinicio', 'mcrincfim', 'mcraltinicio', 'mcraltfim');
foreach ($datas as $data) {
    if (chaveTemValor($formData, $data)) {
        $whereAdicional[] = "mcr.{$data}::DATE = '%s'::DATE";
        $params[] = $formData[$data];
    }
}

if (!empty($whereAdicional)) {
    $whereAdicional = ' AND ' . implode(' AND ', $whereAdicional);
} else {
    $whereAdicional = '';
}

$query = <<<DML
SELECT mcr.mcrid,
       mcr.mcrdsc,
       mcr.mcrcod,
       COALESCE(TO_CHAR(mcr.mcrrefinicio, 'DD/MM/YYYY'), '-') AS mcrrefinicio,
       COALESCE(TO_CHAR(mcr.mcrreffim, 'DD/MM/YYYY'), '-') AS mcrreffim,
       COALESCE(TO_CHAR(mcr.mcrincinicio, 'DD/MM/YYYY'), '-') AS mcrincinicio,
       COALESCE(TO_CHAR(mcr.mcrincfim, 'DD/MM/YYYY'), '-') AS mcrincfim,
       COALESCE(TO_CHAR(mcr.mcraltinicio, 'DD/MM/YYYY'), '-') AS mcraltinicio,
       COALESCE(TO_CHAR(mcr.mcraltfim, 'DD/MM/YYYY'), '-') AS mcraltfim
  FROM altorc.momentocredito mcr
  WHERE mcr.mcrano = '%d'
    AND mcr.mcrstatus = 'A' {$whereAdicional}
DML;
$stmt = vsprintf($query, $params);
$list = new Simec_Listagem();
$list->setQuery($stmt)
    ->setCabecalho(
        array(
            'Descrição do momento',
            'Código do momento',
            'Período referência' => array('Início', 'Fim'),
            'Período de inclusão' => array('Início', 'Fim'),
            'Período de alteração' => array('Início', 'Fim')
        )
    )->setAcoes(array('edit' => 'editarMomentoCredito', 'delete' => 'apagarMomentoCredito'));
?>
<script type="text/javascript" language="javascript">
function apagarMomentoCredito(mcrid)
{
    $('#requisicao-momentocredito').val('apagarMomentoCredito');
    $('#mcrid').val(mcrid);

    $('#modal-confirm .modal-body p').html('Tem certeza que quer excluir o momento de crédito selecionado?');
    $('#modal-confirm').modal();
}

function editarMomentoCredito(mcrid)
{
    $('#requisicao-momentocredito').val('editarMomentoCredito');
    $('#mcrid').val(mcrid);
    $('#form-momentocredito').submit();
}

$('document').ready(function(){
    $('#modal-confirm .btn-primary').click(function(){
        $('#form-momentocredito').submit();
    });
});
</script>
<?php
// -- Saída do relatório
if (false === $list->render()): ?>
    <div class="alert alert-info col-md-4 col-md-offset-4 text-center">Nenhum registro encontrado</div>
<?php
endif;
?>