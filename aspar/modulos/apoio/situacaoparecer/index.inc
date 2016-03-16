<?php
$modelSituacao_Parecer = new Aspar_Model_SituacaoParecer($_REQUEST['stpid']);
$controllerSituacao_Parecer = new Aspar_Controller_SituacaoParecer();
switch ($_REQUEST['action']) {
    case('excluir'):
        $controllerSituacao_Parecer->excluir();
        die;
        break;
    case('gravar'):
        $controllerSituacao_Parecer->gravar();        
        die;
        break;        
    case('carregarSituacaoParecer'):
        $controllerSituacao_Parecer->recuperarPorId($_POST['stpid']);
        die;
        break; 
}

include  APPRAIZ."includes/cabecalho.inc";
?>

<script src="/aspar/js/funcoes.js"></script>
<script src="/aspar/js/funcoesSituacaoParecer.js"></script>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5> Listagem de Situação do Parecer</h5>
            <div class="ibox-tools">
                <a class="collapse-link">
                    <i class="fa fa-chevron-up"></i>
                </a>
            </div>
        </div>
        <div class="ibox-content">
            <div>
                <button type="button" class="btn btn-warning btnNovo" data-toggle="modal" data-target="#myModal">
                  <span class="fa fa-plus-circle"> </span> Adicionar
                </button>                
            </div>
            <div>
                <?PHP
                #GERA LISTAGEM DAS TAREFAS. EMCAMINHAMENTO.
                    $modelSituacao_Parecer->retornaLista();
                ?>
            </div>
        </div>
    </div>
</div>


<!-- Modal -->
<div class="modal fade" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title" id="myModalLabel">Modal title</h4>
      </div>
      <div class="modal-body">
        <?php include_once "formulario.inc";?>    
      </div>
      <div class="modal-footer">
      </div
    </div>
  </div>
</div>