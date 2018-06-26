<?php
// ver($_REQUEST,d);
$cObjetivoppa = new Public_Controller_Objetivoppa();
switch ($_REQUEST['req']) {
    case 'salvar':
        $cObjetivoppa->salvar($_REQUEST);
        die;
    case 'excluir':
        $cObjetivoppa->excluir($_REQUEST['oppid']);
        die;
    case 'excluirIniciativa':
        $cObjetivoppa->excluir($_REQUEST['oppid']);
        die;        
}

$mObjetivoppa = new Public_Model_ObjetivoPpa($_REQUEST['oppid']);
$mIniciativappa = (new Public_Model_IniciativaPpa())->recuperarTodos('*', ["prsano = '{$_SESSION['exercicio']}'", "ippstatus = 'A'", " oppid = {$_REQUEST['oppid']}"]);

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo;?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="oppid" id="oppid" type="hidden" value="<?php echo $mObjetivoppa->oppid; ?>">

                        <?php
                        echo $simec->input('oppcod', 'Código', $mObjetivoppa->oppcod, ['required', 'class'=>'inteiro','maxlength' => 4]);
                        echo $simec->input('oppnome', 'Nome', $mObjetivoppa->oppnome , ['required', 'maxlength' => 250]);
                        echo $simec->input('oppdsc', 'Descricao', $mObjetivoppa->oppdsc, ['required', 'maxlength' => 250]);
                        // ver($mObjetivoppa->oppid);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/objetivoppa&acao=A" class="btn btn-warning" id="btnVoltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mObjetivoppa->oppid){ ?>
                                    <a href="?modulo=apoio/objetivoppa_form&acao=A&req=excluir&oppid=<?php echo $mObjetivoppa->oppid; ?>" class="btn btn-danger link-excluir" id="btnExcluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="ippid" id="ippid" type="hidden" value="<?php echo $mIniciativappa->ippid; ?>">

                        <?php
                        echo $simec->input('ippcod', 'Código', $mIniciativappa->ippcod, ['required', 'class'=>'inteiro','maxlength' => 4]);
                        echo $simec->input('ippnome', 'Nome', $mIniciativappa->ippnome , ['required', 'maxlength' => 250]);
                        echo $simec->input('ippdsc', 'Descricao', $mIniciativappa->ippdsc, ['required', 'maxlength' => 250]);
                        // ver($mObjetivoppa->oppid);
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <?php if($mObjetivoppa->ippid){ ?>
                                    <a href="?modulo=apoio/objetivoppa_form&acao=A&req=excluirIniciativa&oppid=<?php echo $mObjetivoppa->ippid; ?>" class="btn btn-danger link-excluir" id="btnExcluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>        
    </div>
</div>
