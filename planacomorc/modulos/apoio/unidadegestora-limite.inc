<?php

$cUnidadegestoraLimite = new Planacomorc_Controller_Unidadegestoralimite();
switch ($_REQUEST['req']) {
    case 'salvar':
        $cUnidadegestoraLimite->salvar($_REQUEST);
        die;
}


$aLimites = (new Planacomorc_Model_UnidadegestoraLimite)->recuperarLimites($_SESSION['exercicio']);

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
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
                <div class="ibox-content" id="listagem">

                    <style>
                        .marcado{background-color:#C1FFC1!important}
                        .remover{display:none}
                    </style>

                    <script type="text/javascript" lang="JavaScript">
                        jQuery(document).ready(function(){
                            jQuery.expr[':'].contains = function(a, i, m) {
                                return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
                            };

                            jQuery("#textFind").keyup(function()
                            {
                                jQuery('#listagem table tbody tr td').removeClass('marcado');
                                jQuery('#listagem table tbody tr').removeClass('remover');
                                stringPesquisa = jQuery("#textFind").val();

                                console.log(stringPesquisa)

                                if (stringPesquisa) {
                                    jQuery('#listagem table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                                    jQuery('#listagem table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
                                }
                            });
                        });
                    </script>

                    <div>
                        <input class="normal form-control" type="text" id="textFind" placeholder="Digite aqui o que procura" style="width: 100%; margin-bottom: 10px;" />
                    </div>

                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover" >
                            <thead>
                            <tr class="text-center">
                                <th>Unidade</th>
                                <th>Subunidade</th>
                                <th>Limite</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aLimites as $dados){ ?>
                                <tr>
                                    <td><?php echo $dados['unocod'] . ' - ' . $dados['unonome']; ?></td>
                                    <td><?php echo $dados['suocod'] . ' - ' . $dados['suonome'] . ' - ' . $dados['suosigla']; ?></td>
                                    <td>
                                        <input name="limites[]" type="text" class="input_valor salvar-limite" value="<?php echo $dados['lmuvlr']; ?>" data-ungcod="<?php echo $dados['suocod']; ?>" data-lmuid="<?php echo $dados['lmuid']; ?>" >
                                    </td>
                                </tr>
                            <?php } ?>        
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    $(function(){
        $('.input_valor').keyup(function(){
            $(this).val(mascaraglobal('###.###.###.###,##', $(this).val()));
        }).keyup();

        $('.salvar-limite').change(function(){
            var lmuvlr = $(this).val() ? str_replace(['.', ','], ['', '.'], $(this).val()) : 0;
            var ungcod = $(this).data('ungcod');
            var lmuid = $(this).data('lmuid');
            $.ajax({
                url: 'planacomorc.php?modulo=apoio/unidadegestora-limite&acao=A&req=salvar&lmuvlr=' + lmuvlr + '&ungcod=' + ungcod + '&lmuid=' + lmuid
            });
        });
    })
</script>