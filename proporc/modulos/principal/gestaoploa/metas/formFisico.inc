<?php
/**
 * Formulário de configuração da meta física do localizador.
 * $Id: formFisico.inc 87083 2014-09-19 18:56:39Z maykelbraz $
 */
?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" lang="JavaScript">
$(document).ready(function(){
    $('#acaqtdefinanceiro').blur().attr('disabled', true);
    $('#acaqtdefisico').blur();
    $('#acaqtdefisico').keyup(function(){
        calculaMedia('#acaqtdefinanceiro', '#acaqtdefisico', '#fisico_media', true);
    });
    calculaMedia('#acaqtdefinanceiro', '#acaqtdefisico', '#fisico_media', true);
    $('#salvar-fisico').click(function(){
        $('#requisicao').val('salvarFisico');
        $('#formfisico').submit();
    });
    <?php if (!$podeEditarDados): ?>
    $('#formfisico :input').prop('disabled', true);
    <?php endif; ?>

    <?php if (empty($dados['acao']['acadscprosof'])): ?>
    $('#acaqtdefisico').prop('disabled', true);
    <?php endif; ?>
});
</script>
<div class="row">
    <div class="col-md-offset-1 col-md-10">
        <div class="panel panel-primary">
            <div class="panel-heading"><strong>Informações do produto</strong></div>
            <table class="table">
                <tbody>
                    <tr>
                        <td><strong>Produto:</strong></td>
                        <td style="text-align:left"><?php echo $dados['acao']['acadscprosof']; ?></td>
                        <td><strong>Unidade de medida:</strong></td>
                        <td><?php echo $dados['acao']['acadscunmsof']; ?></td>
                    </tr>
                    <tr>
                        <td><strong>Especificação do produto:</strong></td>
                        <td colspan="3" style="text-align:left"><?php echo $dados['acao']['proddesc']; ?></td>
                    </tr>
                </tbody>
            </table>
        </div>
    </div>
</div>
<div class="well row col-md-12">
    <form name="formfisico" id="formfisico" method="POST" role="form">
        <?php if ($podeEditarDados): ?>
        <input type="hidden" name="requisicao" id="requisicao" />
        <?php endif; ?>
        <input type="hidden" name="dados[acaid]" id="acaid" value="<?php echo $dados['acaid']; ?>" />
        <div class="form-group row">
            <div class="col-md-2">
                <label class="control-label" for="acaqtdefisico">Quantidade:</label>
            </div>
            <div class="col-md-10">
                <?php
                $opcoes = array('masc' => '##.###.###.###');
                inputTexto('dados[acaqtdefisico]', $dados['acao']['acaqtdefisico'], 'acaqtdefisico', 17, false, $opcoes);
                ?>
            </div>
        </div>
        <div class="form-group row">
            <div class="col-md-2">
                <label class="control-label">Custo médio:</label>
            </div>
            <div class="col-md-10">
                <?php
                $opcoes = array('habil' => 'N');
                inputTexto('fisico_media', $dados['fisico_media'], 'fisico_media', 17, true, $opcoes);
                ?>
            </div>
        </div>
        <div class="form-group row">
            <div class="col-md-2">
                <label class="control-label">Custo total (R$):</label>
            </div>
            <div class="col-md-10">
                <?php
                $options = array('masc' => '###.###.###.###');
                inputTexto('acaqtdefinanceiro', $dados['acao']['acaqtdefinanceiro'], 'acaqtdefinanceiro', 17, false, $options); ?>
                <input type="hidden" id="h_acaqtdefinanceiro" value="<?php echo $dados['acao']['acaqtdefinanceiro']; ?>" />
            </div>
        </div>
        <div class="form-group row">
            <div class="col-md-2">
                <label class="control-label" for="memcalculo">Memória de cálculo:</label>
            </div>
            <div class="col-md-10">
                <?php inputTextArea('dados[memcalculo]', $dados['acao']['memcalculo'], 'memcalculo', 5000); ?>
            </div>
        </div>
        <hr />
        <?php if ($podeEditarDados): ?>
        <button type="button" class="btn btn-primary" id="salvar-fisico">Salvar</button>
        <?php else: echo <<<HTML
<div class="alert alert-danger text-center col-lg-6 col-lg-offset-3"><strong>Sem permissão para edição!</strong></div>
HTML;
        endif; ?>
    </form>
</div>