<?php

$cMetappa = new Public_Controller_Metappa();
switch ($_REQUEST['req']) {
    case 'salvar':
        $cMetappa->salvar($_REQUEST);
        die;
    case 'excluir':
        $cMetappa->excluir($_REQUEST['mppid']);
        die;
}

$mMetappa = new Public_Model_MetaPpa($_REQUEST['mppid']);

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
                        <input name="mppid" id="mppid" type="hidden" value="<?php echo $mMetappa->mppid; ?>">
                        
                        <?php
                            echo $simec->input('mppcod', 'Código', $mMetappa->mppcod, ['required', 'maxlength' => 4]);
                            echo $simec->textarea('mppnome', 'Meta', $mMetappa->mppnome, ['required', 'maxlength' => 400]);
                            echo $simec->textarea('mppdsc', 'Descrição', $mMetappa->mppdsc, ['required', 'maxlength' => 1000]);
                            echo $simec->input('prsano', 'Ano', $mMetappa->prsano, ['class' => 'inteiro', 'required', 'maxlength' => 4]);
                            echo $simec->radioBoolean('mppquantificavel', 'Possui medida quantificável', $mMetappa->mppquantificavel);
                            echo '<div id="div_mppquantidade">';
                            echo $simec->radioBoolean('mppanualizavel', 'Anualizável', $mMetappa->mppanualizavel);
                            echo $simec->input('mppquantidade', 'Quantidade', $mMetappa->mppquantidade, ['class' => 'inteiro', 'required', 'inteiro']);
                            echo '</div>';
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/metappa&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mMetappa->mppid){ ?>
                                    <a href="?modulo=apoio/metappa_form&acao=A&req=excluir&mppid=<?php echo $mMetappa->mppid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>

<script>

    $(document).ready(function(){

        toggleQuantidade();
        $('#mppquantificavel').change(function(){
            toggleQuantidade();
        });

    });

    function toggleQuantidade(){
        if($('#mppquantificavel').is(':checked')){
            $('#div_mppquantidade').show('slow');
            $('#mppquantidade').attr('required', 'required');
        } else {
            $('#div_mppquantidade').hide('slow');
            $('#mppquantidade').removeAttr('required');
            $('#mppquantidade').val('');
        }
    }

</script>
