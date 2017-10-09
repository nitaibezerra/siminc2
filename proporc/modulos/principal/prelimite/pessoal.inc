<?php
/**
 * Arquivo de Acompanhamento Orçamentário.
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */

require_once APPRAIZ . 'includes/workflow.php';
$fm = new Simec_Helper_FlashMessage('proporc/prelimite');
$modeloPrelimites = new Proporc_Service_Prelimites();

/**
 * Captura de Requisições em AJAX.
 */
if(isAjax()){
    if($_POST['requisicao']){
        switch($_POST['requisicao']){
            case 'alterarUsuarioResponsavel':
                die($modeloPrelimites->atualizarResponsavel($_POST['prpid'], $_POST['usucpf']));
            case 'mostraModalAltUsuario':
                die($modeloPrelimites->modalAltUsuario($_POST['prpid'], $_POST['usucpf']));
        }
    }
}

if (isset($_POST['requisicao']) || !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    switch ($requisicao)
    {
        case 'salvarPrelimite':
            $resultado = $modeloPrelimites->cadastrar($_POST);
            break;
    }
    $fm->addMensagem(
        $resultado['msg'], $resultado['sucesso'] ? Simec_Helper_FlashMessage::SUCESSO : (isset($resultado['warning']) ? Simec_Helper_FlashMessage::AVISO : Simec_Helper_FlashMessage::ERRO)
    );
}

$prfid = $_GET['periodo'] ? $_GET['periodo'] : null;
$aba = 'formFiltroPrelimite.inc';
$tipo = 'S';

if(isset($_GET['requisicao']) || !empty($_GET['requisicao'])){
    switch($_GET['requisicao'])
    {
        case 'acessar':
            $aba = 'formPrelimite.inc';
            break;
        case 'filtro':
            break;
        case 'download-modelo':
            if(isset($_GET['id'])){
                die($modeloPrelimites->recuperaModelo($_GET['id'], true));
            }
            break;
        case 'download-modelo-preenchimento':
            if(isset($_GET['id'])){
                die($modeloPrelimites->recuperaModeloPreenchido($_GET['id'], true));
            }
            break;
    }
}

/**
 * Cabecalho padrão do Sistema.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<script>
    function verificaFormulario(idBotao, requisicao)
    {
        var result = true;
        $.each($('#'+idBotao).parent('form').serializeArray(),function(index,val){
            if(val.name == 'requisicao'){
                $('[name="'+val.name+'"]').val(requisicao);
                return;
            }
            if($('[name="'+val.name+'"]').is(':required') && val.value == ''){
                var label = $('[name="'+val.name+'"]').parent().parent().find('label').text().replace(':','');
                bootbox.alert('Selecione uma opção para o label ' + label);
                result = false;
            }
        });
        return result;
    }

    $(document).ready(function(){
        $('body').on('click',function(e){
            if($(e.target).attr('id') == 'buttonSend'){
                if(verificaFormulario('buttonSend', 'acessar')){
                    var url = $(e.target).attr('data-url');
                    var atributes = $('#buttonSend').parent('form').serialize();
                    window.location.href = url+'&'+atributes;
                }
            }else if($(e.target).attr('id') == 'buttonBack'){
                window.location.href = $(e.target).attr('data-url');
            }else if($(e.target).attr('id') == 'buttonClean'){
                window.location.href = $(e.target).attr('data-url');
            }else if($(e.target).attr('id') == 'buttonFind'){
                if(verificaFormulario('buttonFind', 'filtro')){
                    var url = $(e.target).attr('data-url');
                    var atributes = $('#buttonFind').parent('form').serialize();
                    window.location.href = url+'&'+atributes;
                }
            }
        });

        $('#dl-preenchimento').on('click',function(){
            window.location = 'proporc.php?modulo=principal/prelimite/pessoal&acao=A&requisicao=download-modelo-preenchimento&id='+ $(this).attr('data-prpid');
        });
    });

    function acessarPrelimite(id){
        window.location.href = 'proporc.php?modulo=principal/prelimite/pessoal&acao=A&requisicao=acessar&id='+id;
    }

    function modalAltUsuario(id, cpf){
        if(id == null){
            id = $('#altUsuario').attr('data-prpid');
            cpf = $('#altUsuario').attr('data-usucpf');
        }
        var url = 'proporc.php?modulo=principal/prelimite/pessoal&acao=A';
        var params = {requisicao: 'mostraModalAltUsuario', prpid: id, usucpf: cpf};

        $.post(url, params, function(response){
            $('#modal-confirm .modal-body').html(response);
            $('.modal-dialog').css('width', '60%');
            $('#modal-confirm .modal-title').html('Alterar Responsável');
            $('#modal-confirm .btn-primary').attr('onclick','alterarUsuarioResponsavel()').html('<span class="glyphicon glyphicon-ok"></span> Salvar');
            $('#modal-confirm .btn-default').html('Cancelar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
    }

    function alterarUsuarioResponsavel(){
        var url = 'proporc.php?modulo=principal/prelimite/pessoal&acao=A';
        var params = $('#formAltUsuario').serialize();
        $.post(url,params,function(result){
            if(result == true){
                bootbox.alert('Usuário responsável alterado com sucesso!',function(){
                    window.location.href = window.location.href;
                }).focus();

            }else{
                bootbox.alert('Falha ao atualizar usuário responsável.').focus();
            }
        });
    }
</script>
<link rel="stylesheet" href="/includes/spo.css">
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="proporc.php?modulo=inicio&acao=C">SPO - Proposta Orçamentária</a></li>
        <li><a href="<?= $aba == 'formPrelimite.inc' ? 'proporc.php?modulo=principal/prelimite/pessoal&acao=A' : ''?>">PLOA <?=$_SESSION['exercicio']?></a></li>
        <li class="active">Pré-limites Pessoal</li>
    </ol>
    <div class="row col-md-12">
        <?php
        $arquivo = '';
        // -- Exibindo mensagens do sistema
        echo $fm->getMensagens();
        require(dirname(__FILE__) . "/prelimite/".$aba);
        ?>
    </div>
</div>