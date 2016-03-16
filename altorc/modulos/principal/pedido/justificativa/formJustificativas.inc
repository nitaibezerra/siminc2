<?php
/**
 * Form de respostas das justificativas do pedido de ateração de crédito.
 * $Id: formJustificativas.inc 96803 2015-04-30 19:34:15Z maykelbraz $
 */
$query = <<<DML
SELECT pgm.pgmid,
       pgm.pgmdsc,
       prp.prptexto,
       pgm.pgmsomenteleitura,
       pgmexplicacaopergunta
  FROM altorc.perguntasmomento pgm
    LEFT JOIN altorc.perguntasrespostaspedido prp
      ON (pgm.pgmid = prp.pgmid AND prp.paoid = %d)
 WHERE pgm.mcrid = %d
 ORDER BY pgm.pgmcod
DML;
$stmt = sprintf($query, $dadospao['paoid'], $dadospao['mcrid']);

$dadosjustificativa = $db->carregar($stmt);
?>
<style type="text/css">
    .form-group img{display:none}
</style>
<script type="text/javascript" language="javascript" src="/includes/funcoes.js"></script>

<script type="text/javascript" language="javascript">
    $(document).ready(function() {
        $('.tooltip_link').tooltip();
        $('textarea').addClass('form-control');
        $('#salvarJustificativas').click(function() {
            $('.has-error').removeClass('has-error');
            var todasPreenchidas = true;
            $('textarea.form-control').each(function() {
                if ('' == $(this).val()) {
                    $(this).parent().parent().parent().addClass('has-error');
                    todasPreenchidas = false;
                }
            });
            if (!todasPreenchidas) {
                $('#modal-alert .modal-body').html('Antes de prosseguir, todas as justificativas devem ser preenchidas.');
                $('#modal-alert').modal({backdrop: 'static'});
                return false;
            }
        });
    });
</script>
<?php
$fm = new Simec_Helper_FlashMessage("altorc/pedido");
echo $fm->getMensagens();
?>
<style>
.tooltip-inner{background-color:royalblue!important}
</style>
<form name="filtrarfuncionais" id="filtrarfuncionais" method="POST" role="form" class="col-md-10 col-md-offset-1">
    <input type="hidden" name="requisicao" value="salvarJustificativas" />
    <?php
    $dadosjustificativa = is_array($dadosjustificativa) ? $dadosjustificativa : Array();
    foreach ($dadosjustificativa as $justificativa):
        ?>
        <div class="form-group row">
            <div>
                <label class="control-label" for="just-<?php echo $justificativa['pgmid']; ?>">
    <?php echo $justificativa['pgmdsc']; ?>
                </label>
                <br />
                <small><span class="glyphicon glyphicon-info-sign"></span> <?php echo $justificativa['pgmexplicacaopergunta']; ?></small>
            </div>
            <div>
                <?php
                $editavel = 'S';
                if ($justificativa['pgmsomenteleitura'] == 't') {
                    $editavel = 'N';
                }
                echo campo_textarea('just[' . $justificativa['pgmid'] . ']', 'S', $editavel, '', null, '5', '', '', '', '', '', '', $justificativa['prptexto']);
                ?>
            </div>
        </div>
    <?php endforeach; ?>
    <?php if ($podeSalvar && $mcridAtual == $dadospao['mcrid']) { ?>
        <button type="submit" class="btn btn-primary" id="salvarJustificativas">Salvar justificativas</button>
        <?php
    } else {
        gravacaoDesabilitada();
    }
    ?>
</form>
<br style="clear:both" />
<br />
<br />