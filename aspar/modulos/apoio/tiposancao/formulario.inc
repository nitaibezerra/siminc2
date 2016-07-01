<?PHP

switch ($_REQUEST['action']) {
    case('salvar'):
        $controllerTipoSancao->gravar();
        break;
}

#CHAMADA DE PROGRAMA
include  APPRAIZ."includes/cabecalho.inc";

?>

<div class="wrapper wrapper-content animated fadeInRight">

    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Cadastro de Tipo de Sanção</h5>
            <div class="ibox-tools">
                <a class="collapse-link">
                    <i class="fa fa-chevron-up"></i>
                </a>
            </div>
        </div>

        <div class="ibox-content" style="padding-bottom:1px;">
            <form method="post" name="formulario" id="formulario" class="form form-horizontal">
                <input name="action" type="hidden" value="salvar">
                <input name="tpsid" id="tpsid" type="hidden" value="<?=$modelTipoSancao->tpsid;?>">

                <?php
                    echo $simec->input('tpsdsc', 'Descrição', $modelTipoSancao->tpsdsc, array('maxlength' => 100, 'required'=>'true', 'class' => 'inputText'));
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