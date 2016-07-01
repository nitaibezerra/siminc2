<?php
/**
 * Formulário popup de configuração de valores financeiros.
 * $Id: formFinanceiro.inc 101248 2015-08-13 20:22:05Z maykelbraz $
 */
?>
<style>
#natcod_chosen,#idusocod_chosen,#foncod_chosen,#idoccod_chosen,#rpcod_chosen,#rpleicod_chosen{width:100%!important}
#modal-alert .modal-body ul{text-align:left;margin-top:5px;list-style:circle}
@media (min-width: 992px) {.modal-lg {width:900px}}
.chosen-container{width:100%!important}
</style>
<script type="text/javascript" lang="JavaScript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript" lang="javascript">
$(document).ready(function(){
    $('#ndpid').change(function(){
        // -- Utilizado durante o cadastro para encontrar o DOCID
        $('#ndpcod').val($('#ndpid option:selected').text().substr(0, 8));
    });

    $('#formfinanceiro').submit(function(){
        $('#ploid').removeAttr('disabled');
    });

    $('#salvar-financeiro').click(salvarFinanceiro);
});
</script>
<div class="modal fade" id="modal-financeiro">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Alteração de limite financeiro</h4>
            </div>
            <div class="modal-body">
                <div class="panel panel-primary">
                    <div class="panel-heading">Dados da programática</div>
                    <table class="table">
                        <tr>
                            <td><strong>Unidade Orçamentária:</strong></td>
                            <td colspan="3"><?php echo "{$dados['unicod']} - {$dados['unidsc']}"; ?></td>
                        </tr>
                        <tr>
                            <td><strong>Programa:</strong></td>
                            <td colspan="3"><?php echo "{$dados['acao']['prgcod']} - {$dados['acao']['prgdsc']}"; ?></td>
                        </tr>
                        <tr>
                            <td><strong>Ação:</strong></td>
                            <td colspan="3"><?php echo "{$dados['acao']['acacod']} - {$dados['acao']['acadsc']}"; ?></td>
                        </tr>
                        <tr>
                            <td><strong>Localizador:</strong></td>
                            <td colspan="3"><?php echo "{$dados['acao']['loccod']} - {$dados['acao']['locdsc']}"; ?></td>
                        </tr>
                        <tr>
                            <td><strong>Saldo disponível (R$):</strong></td>
                            <td colspan="3"><?php echo mascaraNumero($dados['saldocoluna']); ?></td>
                        </tr>
                    </table>
                </div>

                <form name="formfinanceiro" id="formfinanceiro" method="POST" role="form" class="well">
                    <?php if ($podeEditarDados): ?>
                    <input type="hidden" name="requisicao" id="requisicao" />
                    <?php endif; ?>
                    <input type="hidden" name="dados[dpaid]" id="dpaid" />
                    <input type="hidden" name="dados[mtrid]" value="<?php echo $dados['mtrid']; ?>" />
                    <input type="hidden" name="dados[ndpcod]" id="ndpcod" />
                    <?php if (in_array($dados['unicod'], array('26101','26291', '26290', '26298', '74902', '73107'))): ?>
                    <div class="form-group row">
                        <div class="col-md-3">
                            <label class="control-label" for="ungcod">Unidade gestora:</label>
                        </div>
                        <div class="col-md-9">
                        <?php
                        $unicod = "ung.unicod = '%s'";
                        $whereAdicional = array(sprintf($unicod, $dados['unicod']));
                        inputComboUngcod($dados['ungcod'], $whereAdicional);
                        ?>
                        </div>
                    </div>
                    <div class="form-group row">
                        <div class="col-md-3">
                            <label class="control-label" for="sbaid">Subação:</label>
                        </div>
                        <div class="col-md-9">
                        <?php inputComboSubacaoPLOA($dados, $_SESSION['exercicio']); ?>
                        </div>
                    </div>
                    <?php endif; ?>
                    <div class="form-group row">
                        <div class="col-md-3">
                            <label class="control-label" for="plocodigo">Plano Orçamentário:</label>
                        </div>
                        <div class="col-md-9"><?php inputComboPOPLOA($dados); ?></div>
                    </div>
                    <div class="form-group row">
                        <div class="col-md-3">
                            <label class="control-label" for="ndpid">Natureza:</label>
                        </div>
                        <div class="col-md-9"><?php inputComboNaturezaPLOA($dados, $_SESSION['exercicio']); ?></div>
                    </div>
                    <div class="form-group row">
                        <div class="col-md-3">
                            <label class="control-label" for="foncod">Fonte:</label>
                        </div>
                        <div class="col-md-9"><?php inputComboFonteRecursoPLOA($dados); ?></div>
                    </div>
                    <div class="form-group row">
                        <div class="col-md-3">
                            <label class="control-label" for="">Valor (R$):</label>
                        </div>
                        <div class="col-md-9">
                            <?php
                            $opt = array('masc' => '##.###.###.###.###');
                            inputTexto('dados[plfvalor]', $dados['plfvalor'], 'plfvalor', 18, false, $opt);
                            ?>
                        </div>
                    </div>
                </form>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Cancelar</button>
                <button type="button" class="btn btn-primary" id="salvar-financeiro">Salvar</button>
            </div>
        </div>
    </div>
</div>