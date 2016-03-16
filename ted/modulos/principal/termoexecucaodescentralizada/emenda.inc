<?php
//Verifica se existe tcpid setado, senão, efetua redirecionamento
Ted_Utils_Model::capturaTcpid(true);

Ted_Utils_Model::userIsAllowed();

/**
 * Janela popUp para o sistema de Emendas
 */
if (isset($_GET['popup']) && $_GET['popup'] == 'show') {
    $habilitabtn = true;
    $_REQUEST['pedid'] = '';
    include_once APPRAIZ.'www/emenda/emendaImpositivo.php';
    die;
}

//Declaração de objetos
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$modelEmenda = new Ted_Model_Emenda();

//Cabeçalho do sistema
include APPRAIZ.'includes/cabecalho.inc';
?>
<script type="text/javascript">
/**
 * Monstra uma janela modal com os dados de impedimento
 * @param emeid
 * @return void(0)
 */
var preview = function preview(id, emdid, edeid, valor) {
	
    var largura = 920
      , altura = 700
      , w = screen.width
      , h = screen.height
      , meio1 = (h/2)-(altura/2)
      , meio2 = (w/2)-(largura/2);

    window.open('ted.php?modulo=principal/termoexecucaodescentralizada/emenda&acao=A&ted=<?=$_GET['ted'];?>&popup=show&emdid='+emdid+'&valor='+valor+'&edeid='+edeid
               ,'Emenda Empendimento',
               'height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+',scrollbars=yes,location=no,toolbar=no,menubar=no');
}
</script>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
        <li class="active">Emenda Impositivo</li>
    </ol>

    <nav class="col-md-2">
        <?php echo $gerenciadorAba->apresentaAbas(); ?>
    </nav>

    <div class="col-md-10">
        <div class="col-md-2"></div>
        <div class="col-md-8">
            <?php echo Ted_Utils_Model::montaInformacaoTermo();?>
        </div>
        <div class="col-md-2"></div>
    </div>

    <section class="col-md-10">
        <section class="well">
            <h4 class="text-center">Dados Oçamentários</h4>
        </section>
        <?php $modelEmenda->getGrid(); ?>
    </section>
</section>