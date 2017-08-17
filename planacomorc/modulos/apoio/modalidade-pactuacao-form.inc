<?php

switch ($_REQUEST['req']) {
    case 'salvar':
        $cCategoriaApropriacao = new Monitora_Controller_CategoriaApropriacao();
        $cCategoriaApropriacao->salvar($_REQUEST);
        die;
}

$mCategoriaApropriacao = new Monitora_Model_CategoriaApropriacao($_REQUEST['capid']);

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="capid" id="capid" type="hidden" value="<?php echo $mCategoriaApropriacao->capid; ?>">

                        <?php
                        echo $simec->input('capcod', 'Código', $mCategoriaApropriacao->capcod);
                        echo $simec->input('capdsc', 'Descrição', $mCategoriaApropriacao->capdsc);
                        echo $simec->radio('capsiconv', 'Identifica SICONV', $mCategoriaApropriacao->capsiconv, ['t'=>'Sim', 'f'=>'Não']);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <input type="submit" class="btn btn-primary" name="btg" value="Salvar" />
                                <a href="?modulo=apoio/modalidade-pactuacao&acao=A" class="btn btn-warning" id="btnVoltar" type="button">Voltar</a>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>