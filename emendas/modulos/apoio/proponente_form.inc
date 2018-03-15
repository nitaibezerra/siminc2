<?php

$cProponente = new Emendas_Controller_Proponente();
switch ($_REQUEST['req']) {
    case 'salvar':
        $atributos = array(
            'prtid' => $_REQUEST['prtid'],
            'pronome' => $_REQUEST['pronome'],
            'procnpj' => str_replace(array('.', '/', '-'), '', $_REQUEST['procnpj']),
            'protelefone' => str_replace(array('(', ')', ' ', '-'), '', $_REQUEST['protelefone']),
            'proemail' => $_REQUEST['proemail'],
        );
        if($_REQUEST['proid']){
            $atributos['proid'] = $_REQUEST['proid'];
        }
        $cProponente->salvar($atributos);
    die;
    case 'excluir':
        $cProponente->excluir($_REQUEST['proid']);
    die;
}

$mProponente = new Emendas_Model_Proponente($_REQUEST['proid']);

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
                        <input name="proid" id="proid" type="hidden" value="<?php echo $mProponente->proid; ?>">
                        
                        <?php                         
                            echo $simec->select('prtid', 'Tipo de Proponente', $mProponente->prtid, Emendas_Model_Proponentetipo::buscarTodosCombo((object) array()), ['required']);
                            echo $simec->input('pronome', 'Nome', $mProponente->pronome, ['maxlength' => 300]);
                            echo $simec->cnpj('procnpj', 'CNPJ', $mProponente->procnpj);
                            echo $simec->telefone('protelefone', 'Telefone', $mProponente->protelefone, ['required', 'maxlength' => 15]);
                            echo $simec->email('proemail', 'E-mail', $mProponente->proemail, ['required', 'maxlength' => 300]);
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/proponente&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mProponente->proid){ ?>
                                    <a href="?modulo=apoio/proponente_form&acao=A&req=excluir&proid=<?php echo $mProponente->proid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>