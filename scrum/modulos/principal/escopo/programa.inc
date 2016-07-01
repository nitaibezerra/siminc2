<?php
/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */

$retorno = processaRequisicao("Programa");
require_once APPRAIZ . 'scrum/modulos/principal/escopo/topo.inc';
?>


<div class="row">
    <div class="col-md-12">
        <div class="bs-docs-section">
            <div class="row">
                <div class="col-lg-12">
                    <div class="page-header">
                        <h1 id="forms">Programas</h1>
                    </div>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <div class="well">
                        <fieldset>
                            <form id="formPrincipal" name="formPrincipal" method="POST" class="form-horizontal">
                                <input type="hidden" name="action" id="action" value="" />
                                <input type="hidden" name="prgid" id="prgid" value="<?php echo isset($retorno['prgid'])?$retorno['prgid']:''; ?>" />
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Nome</label>
                                    <div class="col-lg-10">
                                        <?php
                                        echo campo_texto(
                                                'prgdsc',
                                                'N',
                                                'S',
                                                null,
                                                25,
                                                100,
                                                null,
                                                null,
                                                null,
                                                null,
                                                null,
                                                'id="prgdsc" required class="form-control" style="width=100%;"',
                                                null,
                                                (isset($retorno['prgdsc'])?$retorno['prgdsc']:null)
                                            ); ?>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Duração da sprint (em horas)</label>
                                    <div class="col-lg-10">
                                        <?php echo campo_texto(
                                            'prghrsprint',
                                            'N',
                                            'S',
                                            null,
                                            5,
                                            4,
                                            '####',
                                            null,
                                            null,
                                            null,
                                            null,
                                            'id="prghrsprint" required class="form-control" style="width=100%;"',
                                            null,
                                            (isset($retorno['prghrsprint'])?$retorno['prghrsprint']:null)
                                        ); ?>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <div class="col-lg-10 col-lg-offset-2">
                                        <?php if (isset($retorno['prgid']) & !empty($retorno['prgid'])): ?>
                                            <button class="btn btn-danger" id="voltar">Cancelar</button>
                                            <button class="btn btn-info" id="atualizar">Atualizar</button>
                                        <?php else: ?>
                                            <button class="btn btn-warning" type="reset">Limpar</button> 
                                            <button class="btn btn-success" id="inserir">Inserir</button>
                                            <!--<input class="botao" type="reset" value="Limpar" />-->
                                            <button class="btn btn-primary" id="buscar">Buscar</button>
                                        <?php endif; ?>
                                    </div>
                                </div>
                            </form>
                        </fieldset>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<?php listarProgramas($_GET); ?>
<script type="text/javascript" language="javascript">
jQuery(document).ready(function() {
    inicializarFormulario();
});
</script>
    