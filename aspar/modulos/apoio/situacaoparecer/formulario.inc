<?PHP

switch ($_REQUEST['action']) {
    case('salvar'):
        $controllerSituacao_Parecer->gravar();
        break;
}

#CHAMADA DE PROGRAMA
include  APPRAIZ."includes/cabecalho.inc";

?>

<div class="wrapper wrapper-content animated fadeInRight">

    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Cadastro de Situação do Projeto</h5>
            <div class="ibox-tools">
                <a class="collapse-link">
                    <i class="fa fa-chevron-up"></i>
                </a>
            </div>
        </div>

        <div class="ibox-content" style="padding-bottom:1px;">
            <form method="post" name="formulario" id="formulario" class="form form-horizontal">
                <input name="action" type="hidden" value="salvar">
                <input name="stpid" id="stpid" type="hidden" value="<?=$modelSituacao_Parecer->stpid;?>">

                <?php
                    echo $simec->input('stpdsc', 'Descrição', $modelSituacao_Parecer->stpdsc, array('maxlength' => 100, 'required'=>'true', 'class' => 'inputText'));
                ?>

                <div class="form-group">
                    <div class="col-lg-6 text-right">
                        <button type="submit" id="btnSalvar" class="btn btn-primary"><span class="glyphicon glyphicon-thumbs-up"> </span> Salvar</button>
                    </div>                    
                    <div class="col-lg-6">
                        <button type="button" class="btn btn-danger" data-dismiss="modal"><span class="glyphicon glyphicon-hand-left"> </span> Fechar</button>                        
                    </div>
                </div>

            </form>
        </div>
    </div>
</div>