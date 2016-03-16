<?php
include_once APPRAIZ . 'includes/SpoComunicados/classes/Comunicado.php';

$comunicado = new Comunicado(MODULO);
$message = new Simec_Helper_FlashMessage(MODULO.'/comunicados');

if ($_GET ['requisicao'] == 'excluir') {
    $comunicado->deletar($_GET['arqid']);
}

include APPRAIZ . "includes/cabecalho.inc";
?>
<script>
    function voltar(){
        window.location = $('#modulo').val() + '.php?modulo=inicio&acao=C';
    }

    function editLimite(arqid) {
        window.location = $('#modulo').val() + ".php?modulo=principal/comunicado/cadastrar&acao=A&arqid="+arqid;
    }

    function deleteLimite(arqid){
        bootbox.confirm('Você realmente deseja excluir este arquivo?',function(value){
            if(value){
                window.location += '&requisicao=excluir&arqid='+arqid;
            }
        });
    }
    function comunicadoEmail(arqid) {
        window.location = $('#modulo').val() + ".php?modulo=principal/comunicado/comunicadoEmail&acao=A&arqid="+arqid;
    }
</script>
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li>Comunicados</li>
        <li class="active">Listagem de Arquivos </li>
    </ol>
    <div class="form-group">
        <button class="btn btn-warning" id="voltar" type="button" onclick="voltar();">Voltar</button>
        <input type="hidden" id="modulo" value="<?php echo MODULO; ?>" />
    </div>
    <?php echo $message->getMensagens(); ?>
    <?php $comunicado->listar(); ?>
</div>