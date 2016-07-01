<?php
$modelPartidos = new Aspar_Model_Partidos($_REQUEST['prtid']);
$controllerPartidos = new Aspar_Controller_Partidos();
switch ($_REQUEST['action']) {
    case('excluir'):
        $controllerPartidos->excluir();
        die;
        break;
    case('gravar'):
        $controllerPartidos->gravar();        
        die;
        break;        
    case('carregarPartidos'):
        $controllerPartidos->recuperarPorId($_POST['prtid']);
        die;
        break; 
}

include  APPRAIZ."includes/cabecalho.inc";
?>

<script src="/aspar/js/funcoes.js"></script>
<script src="/aspar/js/funcoesPartidos.js"></script>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5> Listagem do Partido</h5>
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
                    $modelPartidos->retornaLista();
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