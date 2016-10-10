<?php
/**
 * Listagem de registros prontos para tramitação.
 * $Id: listarUosQdd.inc 100030 2015-07-13 14:09:52Z werteralmeida $
 */
$fm = new Simec_Helper_FlashMessage("{$_SESSION['sisdiretorio']}/qdd");
/**
 * Funções de apoio à gestão da PLOA.
 * @see _funcoesgestaoploa.php
 */
require(APPRAIZ . "www/{$_SESSION['sisdiretorio']}/_funcoesgestaoploa.php");

#ver($_REQUEST['tipo']);

if (isset($_REQUEST['requisicao']) && !empty($_REQUEST['requisicao'])) {
    $requisicao = $_REQUEST['requisicao'];
    switch ($requisicao) {
        case 'showQDDPO':
            // -- Construíndo o relatório
            #ver($_REQUEST['dados'],d);
            ob_start();
            require_once(APPRAIZ . 'elabrev/modulos/relatorio/orcamento/relatorioQuadroCreditosPo.inc');
            //$qddHTML = ob_get_flush();
            $qddHTML = ob_get_contents();
            ob_end_clean();
            if ($_REQUEST['exportar']) { // -- Exportar relatório XLS
                echo removeLinks($qddHTML);
                die();
            }
            echo <<<HTML
<div class="btn-group col-lg-5 col-lg-offset-7">
HTML;
            echo exerciciosElabrev($_REQUEST['dados']['exercicio'], 'relatorioElabrevQddPO', 'QDD');

            echo <<<HTML
    <button type="button" class="btn btn-default btn-success" onclick="abrirFecharTodos()"
            id="detalhar-qdd"><span class="glyphicon glyphicon-plus"></span> Detalhar</button>
    <button type="button" class="btn btn-default btn-danger" onclick="imprimirRelatorioQDD()">
        <span class="glyphicon glyphicon glyphicon-print"></span> Imprimir</button>
    <button type="button" class="btn btn-default btn-warning" onclick="exportarRelatorioQDD('showQDDPO')">
        <span class="glyphicon glyphicon-save"></span> Exportar</button>
</div>
<br style="clear:both" />
<br />
<div id="relatorio-qdd">
HTML;
            echo removeLinks($qddHTML);
            echo <<<HTML
</div>
HTML;
            die();
        case 'showLimites':
            ob_start();
            require_once(APPRAIZ . 'elabrev/modulos/principal/propostaorcamentaria/consultalimites.inc');
            //$qddHTML = ob_get_flush();
            $qddHTML = ob_get_contents();
            ob_end_clean();
            if ($_POST['exportar']) { // -- Exportar relatório XLS
                echo removeLinks($qddHTML);
                die();
            }
            echo <<<HTML
<div class="btn-group col-lg-4 col-lg-offset-8">
HTML;
            echo exerciciosElabrev($_POST['dados']['exercicio'], 'relatorioElabrevLimites',$dados['unicod'] .' '. $dados['unidsc']);
            echo <<<HTML
    <button type="button" class="btn btn-default btn-danger" onclick="imprimirRelatorioQDD()">
        <span class="glyphicon glyphicon glyphicon-print"></span> Imprimir</button>
    <button type="button" class="btn btn-default btn-warning" onclick="exportarRelatorioQDD('showLimites')">
        <span class="glyphicon glyphicon-save"></span> Exportar</button>
</div>
<br style="clear:both" />
<br />
<div id="relatorio-qdd">
HTML;
            echo removeLinks($qddHTML);
            echo <<<HTML

</div>
HTML;

            die();
        case 'showQDDSubacao':
            ob_start();
            require_once(APPRAIZ . 'elabrev/modulos/relatorio/orcamento/relatorioQuadroCreditosSubacao.inc');
            //$qddHTML = ob_get_flush();
            $qddHTML = ob_get_contents();
            ob_end_clean();
            if ($_POST['exportar']) { // -- Exportar relatório XLS
                echo removeLinks($qddHTML);
                die();
            }
            echo <<<HTML
<div class="btn-group col-lg-4 col-lg-offset-8">
HTML;
            echo exerciciosElabrev($_POST['dados']['exercicio'], 'relatorioElabrevQddSubacao',$dados['unicod'] .' '. $dados['unidsc']);
            echo <<<HTML
    <button type="button" class="btn btn-default btn-danger" onclick="imprimirRelatorioQDD()">
        <span class="glyphicon glyphicon glyphicon-print"></span> Imprimir</button>
    <button type="button" class="btn btn-default btn-warning" onclick="exportarRelatorioQDD('showQDDSubacao')">
        <span class="glyphicon glyphicon-save"></span> Exportar</button>
</div>
<br style="clear:both" />
<br />
<div id="relatorio-qdd">
HTML;
            echo removeLinks($qddHTML);
            echo <<<HTML
</div>
HTML;
            die();
        case 'showSinteseDespesas':
            if ($_POST['exportar']) { // -- Exportar relatório XLS
                require_once(APPRAIZ . 'elabrev/modulos/relatorio/orcamento/relatorioSintese.inc');
                die();
            }
            echo <<<HTML
<div class="btn-group col-lg-4 col-lg-offset-8">
HTML;
            echo exerciciosElabrev($_POST['dados']['exercicio'], 'relatorioElabrevSintese',$dados['unicod'] .' '. $dados['unidsc']);
            echo <<<HTML
    <button type="button" class="btn btn-default btn-danger" onclick="imprimirRelatorioQDD()">
        <span class="glyphicon glyphicon glyphicon-print"></span> Imprimir</button>
    <button type="button" class="btn btn-default btn-warning" onclick="exportarRelatorioQDD('showSinteseDespesas')">
        <span class="glyphicon glyphicon-save"></span> Exportar</button>
</div>
<br style="clear:both" />
<br />
<div id="relatorio-qdd">
HTML;
            require_once(APPRAIZ . 'elabrev/modulos/relatorio/orcamento/relatorioSintese.inc');
            echo <<<HTML
</div>
HTML;
            die();    
        default:
            die($requisicao);
    }
}
/**
 * Cabecalho padrão do SIMEC.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<script type="text/javascript" src="js/gestaoploa.js"></script>
<script type="text/javascript" lang="JavaScript">
    $(document).ready(function () {
        $('#modal-alert .modal-dialog').addClass('modal-lg');
    });

    function abrirQddUo(unicod, exercicio) {
        var requisicao = '<?php echo $_REQUEST['tipo'] ?>';
        var titulo = 'QDD';
        $.post(
                window.location,
                {
                    requisicao: requisicao,
                    'dados[unicod]': unicod,
                    'dados[ppoid]': 2,
                    'dados[exercicio]': '<?echo $_SESSION['exercicio'];?>'
                },
        function (html) {
            $('#modal-alert .modal-title').text(titulo);
            $('#modal-alert .modal-body').html(html.trim());
            $('#modal-alert').modal();
        }
        );

    }
</script>
<style type="text/css">
label{margin-top:10px;cursor:pointer}
.panel-collapse table{margin-bottom:0}
.glyphicon-chevron-right,.glyphicon-info-sign{font-size:.8em;margin-right:5px}
.modal-body{text-align:left}
@media(min-width:992px){.modal-lg{width:100%}}
</style>
<br />
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active"><?php echo "QDDs e Outros - " . $_SESSION ['exercicio']; ?></li>
    </ol>
</div>
<?php
// -- Nova chamada para exibição de mensagens de limite de fonte excedido
echo $fm->getMensagens();
?>

<input type="hidden" name="requisicao" id="requisicao_envio" />
<?php
$sql = "SELECT
            unicod,
            unicod || ' - ' || unidsc AS unidade
        FROM
            public.unidade
        WHERE
            unistatus = 'A'
        AND orgcod = '". CODIGO_ORGAO_SISTEMA. "'
        ORDER BY
            2; ";
$acoes = array(
    'view' => array(
        'func' => 'abrirQddUo'
    )
);
$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);

$listagem->setCabecalho(array('Unidade Orçamentária (UO)'))
        ->setAcoes($acoes)
        ->setQuery($sql)
        ->addCallbackDeCampo('unidade', 'alinharEsquerda')
        ->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
