<?php
/**
 * Listagem de períodos de referência.
 * $Id: listagemperiodoreferencia.inc 102347 2015-09-11 14:15:27Z maykelbraz $
 */

// -- Se for feita uma requisição de edição (load) ignora os valores de $formData
if ('load' == $_REQUEST['action']) {
    unset($formData);
    $formData = array();
}

// --  Verificando filtros
$whereParcial = array();
if (chaveTemValor($formData, 'prfdsc')) {
    $whereParcial[] = "prf.prfdsc ILIKE '%{$formData['prfdsc']}%'";
}
if (chaveTemValor($formData, 'prfdatainicio')) {
    $formData['prfdatainicio'] = preparaData($formData['prfdatainicio']);
    $whereParcial[] = "prf.prfdatainicio = '{$formData['prfdatainicio']}'";
}
if (chaveTemValor($formData, 'prfdatafim')) {
    $formData['prfdatafim'] = preparaData($formData['prfdatafim']);
    $whereParcial[] = "prf.prfdatafim = '{$formData['prfdatafim']}'";
}
if (chaveTemValor($formData, 'prfpreenchimentoinicio')) {
    $formData['prfpreenchimentoinicio'] = preparaData($formData['prfpreenchimentoinicio']);
    $whereParcial[] = "prf.prfpreenchimentoinicio = '{$formData['prfpreenchimentoinicio']}'";
}
if (chaveTemValor($formData, 'prfpreenchimentofim')) {
    $formData['prfpreenchimentofim'] = preparaData($formData['prfpreenchimentofim']);
    $whereParcial[] = "prf.prfpreenchimentofim = '{$formData['prfpreenchimentofim']}'";
}
if ($whereParcial) {
    $whereParcial[0] = ' AND ' . $whereParcial[0];
}

// -- Query de consulta de referências
$sql = <<<DML
SELECT prf.prfid,
       prf.prfdsc,
       to_char(prf.prfdatainicio, 'DD/MM/YYYY') || ' até ' || to_char(prf.prfdatafim, 'DD/MM/YYYY') AS validade,
       to_char(prf.prfpreenchimentoinicio, 'DD/MM/YYYY') || ' até ' || to_char(prf.prfpreenchimentofim, 'DD/MM/YYYY') AS preenchimento
  FROM recorc.periodoreferencia prf
  WHERE prf.exercicio = '{$_SESSION['exercicio']}'
    AND prf.prfstatus = 'A'
%s
DML;
$sql = sprintf($sql, implode(' AND ', $whereParcial));

$config['actions'] = array(
    'edit' => 'editPeriodoReferencia',
    'delete' => 'deletePeriodoReferencia'
);

$listagem = new Simec_Listagem();
$listagem->setCabecalho(array(
    'Descrição',
    'Período de Validade',
    'Período de Preenchimento'));
$listagem->setQuery($sql);
$listagem->setAcoes(array(
    'edit' => 'editPeriodoReferencia',
    'delete' => 'deletePeriodoReferencia'
));
$listagem->addCallbackDeCampo(array(
    'vlrloa', 'valor_sof', 'capvaloruo'), 'mascaraMoeda');
$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
if (false === $listagem->render()):
    ?>
    <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
        Nenhum registro encontrado
    </div>
    <?php
endif;
