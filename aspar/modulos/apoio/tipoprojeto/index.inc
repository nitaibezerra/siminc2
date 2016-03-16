<?php
$modelTipoProjeto = new Aspar_Model_TipoProjeto($_REQUEST['tpjid']);
$controllerTipoProjeto = new Aspar_Controller_TipoProjeto();
switch ($_REQUEST['action']) {
    case('excluir'):
        $controllerTipoProjeto->excluir();
        die;
        break;
    case('gravar'):
        $controllerTipoProjeto->gravar();        
        die;
        break;        
    case('carregarTipoProjeto'):
        $controllerTipoProjeto->recuperarPorId($_POST['tpjid']);
        die;
        break; 
}

include  APPRAIZ."includes/cabecalho.inc";
?>

<script src="/aspar/js/funcoes.js"></script>
<script src="/aspar/js/funcoesTipoProjeto.js"></script>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5> Listagem de Tipo de Projeto</h5>
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
                    $modelTipoProjeto->retornaLista();
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