<?php
/**
 * Form de alteração do valor físico do localizador.
 * $Id: formFisico.inc 85258 2014-08-21 21:00:16Z maykelbraz $
 */
?>
<script type="text/javascript" language="javascript">
/**
 * Se o input for deixado vazio, retorna seu valor inicial.
 */
function resetValor(input)
{
    if ('' == $(input).val()) {
        $(input).val($(input).attr('data-default-value'));
    }
}
</script>
<div class="page-header">Físico</div>
<?php
$fm = new Simec_Helper_FlashMessage('altorc/pedido');
echo $fm->getMensagens();
?>
<div class="col-md-10 col-md-offset-1 row">
    <form name="localizadorFisico" id="localizadorFisico" method="POST" role="form">
        <input type="hidden" name="requisicao" value="salvarFisico" />
        <div class="col-md-5">
            <div class="panel panel-default">
                <table class="table">
                    <tr>
                        <td><strong>Produto:</strong></td>
                        <td><?php echo $locdados['prodsc']; ?></td>
                    </tr>
                    <tr>
                        <td><strong>Unidade de medida:</strong></td>
                        <td><?php echo $locdados['unmdsc']; ?></td>
                    </tr>
                    <tr>
                        <td><strong>Quantidade:</strong></td>
                        <td><?php echo number_format($locdados['proquantidade'], 0, ',', '.'); ?></td>
                    </tr>
                </table>
            </div>
        </div>
        <div class="col-md-5">
            <div class="well well-sm">
                <table class="table" style="margin-bottom:7px">
                    <thead>
                        <tr>
                            <th><strong>Acréscimo</strong></th>
                            <th><strong>Redução</strong></th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td>
                            <?php
                            $valacrescimo = number_format($locdados['valacrescimo'], 0, ',', '.');
                            $onBlur = "resetValor(this)";
                            $complemento = <<<HTML
id="valacrescimo" class="form-control" data-default-value="{$valacrescimo}"
HTML;
                            echo campo_texto('valacrescimo', "N", 'S', "", '', '', "###.###.###.###", "", '', '', 0, $complemento, '', '', $onBlur);
                            ?>
                            </td>
                            <td>
                            <?php
                            $valreducao = number_format($locdados['valreducao'], 0, ',', '.');
                            $complemento = <<<HTML
id="valreducao" class="form-control" data-default-value="{$valreducao}"
HTML;
                            echo campo_texto('valreducao', "N", 'S', "", '', '', "###.###.###.###", "", '', '', 0, $complemento, '', '', $onBlur);
                            ?>
                            </td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </div>
        <div class="col-md-2 text-center" id="botaoGravarFisico">
        <?php if($podeSalvar && $mcridAtual == $dadospao['mcrid']) { ?>
            <button type="submit" class="btn btn-primary">Gravar Físico</button>
        <?php } else {
              gravacaoDesabilitada(null, 8, 2);
              } ?>
        </div>
    </form>
    <br class="clear:both" />
    <br />
</div>