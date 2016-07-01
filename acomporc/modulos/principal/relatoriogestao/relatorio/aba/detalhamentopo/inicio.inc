<?php
/**
 * Aba de criação de POs.
 * @version $Id: inicio.inc 95729 2015-03-24 17:43:21Z lindalbertofilho $
 */
?>
<div class="row col-md-12">
    <?php if ($dados['rldid']): ?>
    <div class="col-md-6 col-md-offset-3">
        <div class="panel panel-primary">
            <div class="panel-heading">Responsável</div>
            <table class="table">
                <thead>
                    <tr>
                        <th>Nome</th>
                        <th>CPF</th>
                        <?php if (!in_array(PFL_RELATORIO_TCU, $perfis)): ?>
                        <th>&nbsp;</th>
                        <?php endif; ?>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td style="width:50%"><?php echo $dados['usunome']?$dados['usunome']:'<center>-</center>'; ?></td>
                        <td><?php echo $dados['usucpf']?formatar_cpf($dados['usucpf']):'<center>-</center>'; ?></td>
                        <?php if (!in_array(PFL_RELATORIO_TCU, $perfis)): ?>
                        <td style="width:5%">
                            <button class="btn btn-primary" id="mostar-modal-alterar-responsavel"
                                    data-usucpf="<?php echo $dados['usucpf']; ?>"
                                    data-rldid="<?php echo $dados['rldid']; ?>">
                                <span class="glyphicon glyphicon-user"></span> Definir responsável
                            </button>
                        </td>
                        <?php endif; ?>
                    </tr>
                </tbody>
            </table>
        </div>
    </div>
    <?php endif; ?>
    <br style="clear:both" />
    <div class="well">
    <?php
    /**
     * Formulário de criação de Po.
     * @see formPo.inc
     */
    require_once(dirname(__FILE__) . '/formPo.inc');
    ?>
    </div>
</div>
<br />