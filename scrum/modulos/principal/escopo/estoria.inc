<?php
/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */

$retorno = processaRequisicao("Estoria");

//ver($retorno,d);

$titulo = 'Estórias';
require_once APPRAIZ . 'scrum/modulos/principal/escopo/topo.inc';
?>


<div class="row">
    <div class="col-md-12">
        <div class="bs-docs-section">
            <div class="row">
                <div class="col-lg-12">
                    <div class="page-header">
                        <h1 id="forms">Estória</h1>
                    </div>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <div class="well">
                        <form id="formPrincipal" name="formPrincipal" method="POST" class="form-horizontal">
                        <fieldset>
                            <input type="hidden" name="action" id="action" value="" />
                            <input type="hidden" name="estid" id="estid" value="<?php echo isset($retorno['estid'])?$retorno['estid']:''; ?>" />
                            <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Programa</label>
                                    <div class="col-lg-10">
                                        <?php
                $sql = <<<DML
SELECT prg.prgid AS codigo,
       prg.prgdsc AS descricao
  FROM scrum.programa prg
DML;
                $db->monta_combo(
                    'prgid',
                    $sql,
                    'S',
                    'Selecione um programa',
                    'carregaComboSubprograma',
                    null,
                    null,
                    null,
                    'N',
                    'prgid',
                    null,
                    (isset($retorno['prgid'])?$retorno['prgid']:null)
                    , ''
                    , 'class="form-control" style="width=100%;" required="required"'
                );
                if (isset($retorno['prgid'])): ?>
                    <script type="text/javascript" language="javascript">
                        jQuery(document).ready(function(){
                            carregaComboSubprograma(<?php echo $retorno['prgid']; ?>);
                        });
                    </script>
                <?php   if (isset($retorno['subprgid'])): ?>
                    <script type="text/javascript" language="javascript">
                        jQuery(document).ready(function(){
                            jQuery('#subprgid').val(<?php echo $retorno['subprgid']; ?>);
                        });
                    </script>
                <?php   endif;
                endif; ?>
                                    </div>
                                </div>
                            <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Subprograma</label>
                                    <div class="col-lg-10">
                                        <?php
                $db->monta_combo(
                    'subprgid',
                    array(),
                    'S',
                    'Selecione um subprograma',
                    null,
                    null,
                    null,
                    null,
                    'N',
                    'subprgid',
                    null,
                    (isset($retorno['subprgid'])?$retorno['subprgid']:null),
                    null,
                    'class="form-control" style="width=100%;" required="required"'
                );
                ?>
                                    </div>
                                </div>
                            <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Título</label>
                                    <div class="col-lg-10">
                                        <?php
                    echo campo_texto(
                            'esttitulo',
                            'N',
                            'S',
                            null,
                            25,
                            50,
                            null,
                            null,
                            null,
                            null,
                            null,
                            'id="esttitulo"  required="required" class="form-control" style="width=100%;"',
                            null,
                            (isset($retorno['esttitulo'])?$retorno['esttitulo']:null)
                        );
                ?>
                                    </div>
                                </div>
                            <div class="form-group">
                                <label for="inputEmail" class="col-lg-2 control-label">Descrição</label>
                                <div class="col-lg-10">
                                    <textarea class="form-control" rows="3" id="estdsc" name="estdsc" required><?php echo (isset($retorno['estdsc'])) ? $retorno['estdsc'] : null ?></textarea>
                                    <span class="help-block">Descreva a estória.</span>
                                </div>
                            </div>
                            <div class="form-group">
                                <div class="col-lg-10 col-lg-offset-2">
                                        <?php if (isset($retorno['estid']) & !empty($retorno['estid'])): ?>
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
                        </fieldset>
                            </form>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<?php listarEstorias($_GET); ?>
<script type="text/javascript" language="javascript">
jQuery(document).ready(function() {
//    jQuery('#estdsc').attr('required', 'required');
    inicializarFormulario();
});
</script>