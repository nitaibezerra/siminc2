<?php
include_once APPRAIZ . 'includes/library/simec/Listagem.php';

//Declaração de objetos
$ted = new Ted_Model_TermoExecucaoDescentralizada();
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
//$situacao = new Ted_Model_Workflow();

//salva a coordenação responsavel
if (isset($_GET['cooid']) && Ted_Utils_Model::isAjax()) {
    $ted->cooid = (int) $_GET['cooid'];
    $ted->alterar();
    if ($ted->commit()) {
        echo 'Coordenação foi salva com sucesso!';
    } else {
        echo 'Falha ao tentar salvar a coordenação, contate o suporte!';
    }
    die;
}

//Verifica se existe tcpid setado, senão, efetua redirecionamento
$tcpid = Ted_Utils_Model::capturaTcpid(true);

Ted_Utils_Model::userIsAllowed();

//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
?>

<script>
    $(document).ready(function () {
       $('.detalhar-usuarios').click(function(){
          console.log($(this).id());
       });
    });
</script>

<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
        <li class="active">Tramitar</li>
    </ol>
    <nav class="col-md-2">
        <?php echo $gerenciadorAba->apresentaAbas(); ?>
    </nav>

    <div class="col-md-10">
        <div class="col-md-2"></div>
        <div class="col-md-8">
            <?php echo Ted_Utils_Model::montaInformacaoTermo(); ?>
        </div>
        <div class="col-md-2"></div>
    </div>

    <section class="col-md-10">
        <section class="well">
            <h4 class="text-center">Trâmite</h4>
        </section>

        <?php
        require_once APPRAIZ . 'includes/workflow.php';
        require_once APPRAIZ . 'www/ted/_condicao_workflow.inc';
        $docid = Ted_Utils_Model::getDocid($_GET['ted']);

        $pendenciaRco = new Ted_Model_RelatorioCumprimento_Business();
        echo $pendenciaRco->mostraPendencias();

        echo $pendenciaRco->termosPendenciaAprovacaoCoordenacao();
        ?>
        <div class="row">
            <div class="col-md-1"></div>
            <div class="col-md-4" style="margin: 0 auto;">
                <?php echo listaPendenciasTed($tcpid); ?>
            </div>
            <div class="col-md-7">
                <?php
                //echo listaDetalhesFluxo($tcpid);
                ?>
            </div>
        </div>
    </section>
</section>

<script type="text/javascript">
    /**
     *
     * @param cooid
     * @return {boolean}
     */
    function salvaCoordenacao(cooid) {
        if (!cooid) return false;

        var promisse = $.ajax({url: location.href + "&cooid=" + cooid});
        promisse.done(function (response) {
            bootbox.alert(response, function(resp){
                location.href=location.href;
            });
        });
    }
</script>