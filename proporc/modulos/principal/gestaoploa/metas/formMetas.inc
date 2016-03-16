<?php
/**
 * Popup de alteração da meta do PO.
 * $Id: formMetas.inc 82579 2014-07-08 14:34:25Z maykelbraz $
 */
?>
<style>
#natcod_chosen,#idusocod_chosen,#foncod_chosen,#idoccod_chosen,#rpcod_chosen,#rpleicod_chosen{width:100%!important}
#modal-alert .modal-body ul{text-align:left;margin-top:5px;list-style:circle}
@media (min-width: 992px) {.modal-lg {width:900px}}
.chosen-container{width:100%!important}
</style>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" lang="JavaScript">
$(document).ready(function(){
    $('#salvar-metas').click(function(){
        $('#requisicao').val('salvarMeta');
        $('#formmetas').submit();
    });
});
</script>
<div class="modal fade" id="modal-metas">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Alteração de meta de PO</h4>
            </div>
            <div class="modal-body">
                <div class="panel panel-primary">
                    <div class="panel-heading">Dados do PO</div>
                    <table class="table">
                        <tr>
                            <td style="width:25px"><strong>Programática:</strong></td>
                            <td>
                            <?php
                            echo "{$dados['unicod']}.{$dados['acao']['prgcod']}.{$dados['acao']['acacod']}.{$dados['acao']['loccod']}";
                            ?>
                            </td>
                        </tr>
                        <tr>
                            <td><strong>PO:</strong></td>
                            <td><span id="plodsc_l"></span></td>
                        </tr>
                        <tr>
                            <td><strong>Produto:</strong></td>
                            <td><span id="prddsc_l"></span></td>
                        </tr>
                        <tr>
                            <td nowrap><strong>Unidade de medida:</strong></td>
                            <td><span id="unmdsc_l"></span></td>
                        </tr>
                    </table>
                </div>
                <form name="formmetas" id="formmetas" method="POST" role="form" class="well">
                    <input type="hidden" name="requisicao" id="requisicao" />
                    <input type="hidden" name="dados[ploid]" id="ploid" />
                    <input type="hidden" name="dados[acaid]" id="f_acaid" value="<?php echo $dados['acaid']; ?>" />
                    <div class="form-group row">
                        <div class="col-md-3">
                            <label class="control-label" for="mpovalor">Nova meta:</label>
                        </div>
                        <div class="col-md-9">
                            <?php
                            $opt = array('masc' => '##.###.###.###.###');
                            inputTexto('dados[mpovalor]', 0, 'mpovalor', 18, false, $opt);
                            ?>
                        </div>
                    </div>
                    <div class="form-group row">
                        <div class="col-md-3">
                            <label class="control-label" for="memcalculo">Memória de cálculo:</label>
                        </div>
                        <div class="col-md-9">
                            <?php inputTextArea('dados[memcalculo]', $dados['acao']['memcalculo'], 'memcalculo', 5000); ?>
                        </div>
                    </div>
                </form>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Cancelar</button>
                <button type="button" class="btn btn-primary" id="salvar-metas">Salvar</button>
            </div>
        </div>
    </div>
</div>