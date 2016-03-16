<?php
require APPRAIZ . 'includes/library/simec/Listagem.php';

//Declaração de Objetos
$unidadeGestora = new Ted_Model_UnidadeGestora();
$formFiltraTermo = new Ted_Form_FiltraTermo();
$ted = new Ted_Model_TermoExecucaoDescentralizada();
$clausula = new Ted_Model_Responsabilidade();
$uo = new Ted_Model_UnidadeOrcamentaria();

$formFiltraTermo->getElement('unicod')->addMultiOptions($uo->getUO());
$formFiltraTermo->getElement('ungcodproponente')->addMultiOptions($unidadeGestora->pegaListaProponente());
$formFiltraTermo->getElement('ungcodconcedente')->addMultiOptions($unidadeGestora->pegaListaConcedente());
$formFiltraTermo->getElement('esdid')->addMultiOptions(Ted_Utils_Model::pegaEstadosTermo());

if (isset($_POST['search']) || isset($_POST['export'])) {
    $where[] = $ted->buildWhere($_POST);
    //$joins = $ted->buildJoins($_POST);
    $formFiltraTermo->populate($_POST);

    if ($_POST['export'] == 'xls') {
        $strQuery = $ted->getQueryListaTermos($where, $joins);
        //ver($_POST, $strQuery, d);

        $list = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
        $list->setCabecalho(array(
            'Termo',
            'SIAFI',
            'Unidade Gestora Proponente',
            'Unidade Gestora Concedente',
            'Título / Objeto da despesa',
            'Situação Documento',
            'Coordenação',
            'Vigencia'))
            ->esconderColunas(array('decricao'))
            ->setQuery($strQuery);

        $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
             ->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
        exit;
    }
}

//Cabeçalho do sistema
include APPRAIZ.'includes/cabecalho.inc';
?>
<style type="text/css">
.marcado {
    background-color: #C1FFC1 !important;
}
.remover {
    display: none;
}
</style>
<script src="/ted/js/gestaoploa.js"></script>
<script type="text/javascript">
/**
 * Redirect para navegação do termo,
 * caso exista identificador para o termo
 * @param tcpid
 */
function navegaTed(tcpid) {
    if (tcpid) {
        document.location.href="ted.php?modulo=principal/termoexecucaodescentralizada/proponente&acao=A&ted="+tcpid;
    }
}

$(function(){
    $("#exportarXls").on("click", function(){
        $("#export").val("xls");
    });

    $("#clear").on("click", function(){
        location.href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A";
    });
});
</script>
<div class="row col-md-12">
	<ol class="breadcrumb">
		<li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
		<li class="active">Listar Termos</li>
	</ol>
    <div class="well col-md-12">
        <?php echo $formFiltraTermo->showForm(); ?>
    </div>
</div>

<div class="col-md-12">
<?php
    $strQuery = $ted->getQueryListaTermos($where, $joins);
    //er($strQuery, d);

    $list = new Simec_Listagem();
    $list->setCabecalho(array(
        'Termo',
        'SIAFI',
        'Unidade Gestora Proponente',
        'Unidade Gestora Concedente',
        'Título / Objeto da despesa',
        'Situação Documento',
        'Coordenação',
        'Vigência'))
      ->addAcao('edit', 'navegaTed')
      ->setQuery($strQuery);

    $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
         ->turnOnPesquisator()
         ->addCampo(array('id' => 'searchFix', 'name' => 'search', 'type' => 'hidden'))
         ->setFormFiltros('filtroTed')
         ->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
?>
</div>