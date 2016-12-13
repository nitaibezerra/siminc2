<?php
/**
 * Cria a listagem de questionários.
 *
 * @version $Id$
 * @package SiMEC
 * @subpackage planejamento-acompanhamento-orcamentario
 */

/**
 * Funções de apoio à construção do formulário.
 * @see _funcoesquestionariosubacao.php
 */
include_once '_funcoesquestionariosubacao.php';

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit();
}

/**
 * Componente de listagem do novo layout.
 *
 * @see Simec_Listagem
 */
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

/**
 * Classe de Questionário
 */
require_once APPRAIZ . 'planacomorc/classes/Questionario.class.inc';

/**
 * Cabecalho padrão do simec.
 *
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript">

function abrirQuestionario() {
    var url ='planacomorc.php?modulo=principal/questionariosubacoes/formularioquestionario&acao=A';

    $.post(url,function(data){
        $('#modal-questionario .modal-body').html(data);
        $('#modal-questionario .modal-dialog').css('width','50%');
        $('#modal-questionario .modal-title').html('Novo questionário');
        $('#modal-questionario .btn-primary').html('Salvar');
        $('#modal-questionario .btn-primary').attr('onclick','inserirQuestionario()');
        $('#modal-questionario .btn-default').html('Fechar');
        $('#modal-questionario .modal-dialog').show();        
        $('#modal-questionario').modal();        
    });
}

function inserirQuestionario() {
    if (jQuery('#qstnome').val() == '') {
        alert('Nome do questionário em branco');
        return false;
    }

    if (jQuery('#percod').val() == '') {
        alert('Período em branco');
        return false;
    }

    selectAllOptions(document.getElementById('id_subacoes'));
    document.getElementById('formquestionario').submit();

}

function carregarQuestionario(qstid) {
    window.location = 'planacomorc.php?modulo=principal/questionariosubacoes/listaquestionario&acao=A&requisicao=carregarQuestionario&qstid=' + qstid;
}

function excluirQuestionario(qstid) {
    var conf = confirm('Deseja realmente excluir o Questionário?');
    if (conf) {
        window.location = 'planacomorc.php?modulo=principal/questionariosubacoes/listaquestionario&acao=A&requisicao=excluirQuestionario&qstid=' + qstid;
    }
}
</script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<?
$questionario = new Questionario(Questionario::CAIXA_AZUL);
?>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
        <li>Subações <?=$_SESSION['exercicio'];?></li>
        <li class="active">Questionário de Acompanhamento de Subações</li>
    </ol>		
    <div class="well">
        <div class="form-horizontal">
            <? $questionario->filtroPeriodo(); ?>
            <input type="button" class="btn btn-success" value="Inserir novo" onclick="abrirQuestionario();">
        </div>
    </div>
<?php
$questionario->renderListagem();
?>
</section>
<div class="modal fade" id="modal-questionario">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title"></h4>
            </div>
            <div class="modal-body">                
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
                <button type="button" class="btn btn-primary">Save changes</button>
            </div>
        </div>
    </div>
</div>