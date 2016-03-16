<?php
/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */

$retorno = processaRequisicao("Subprograma");
$titulo = 'Subprogramas';
require_once APPRAIZ . 'scrum/modulos/principal/escopo/topo.inc';
?>
<style>
    .simpleColorChooser{z-index: 1;}
</style>
<div class="row">
    <div class="col-md-12">
        <div class="bs-docs-section">
            <div class="row">
                <div class="col-lg-12">
                    <div class="page-header">
                        <h1 id="forms">Subprograma</h1>
                    </div>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <div class="well">
                        <fieldset>
                            <form id="formPrincipal" name="formPrincipal" method="POST" class="form-horizontal">
                                <input type="hidden" name="action" id="action" value="" />
                                <input type="hidden" name="subprgid" id="subprgid" value="<?php echo isset($retorno['subprgid'])?$retorno['subprgid']:''; ?>" />
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Nome</label>
                                    <div class="col-lg-10">
                                        <?php
                                            echo campo_texto(
                                                    'subprgdsc',
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
                                                    'id="subprgdsc"  required="required" class="form-control" style="width=100%;"',
                                                    null,
                                                    (isset($retorno['subprgdsc'])?$retorno['subprgdsc']:null)
                                                ); ?>
                                    <!--<span class="help-block">Exemplo: Simec.</span>-->
                                    </div>
                                </div>
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
                    null,
                    null,
                    null,
                    null,
                    'N',
                    'prgid',
                    null,
                    (isset($retorno['prgid'])?$retorno['prgid']:null),
                    null,
                    '  required="required" class="form-control" style="width=100%;"'
                );
                ?>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Sistema</label>
                                    <div class="col-lg-10">
                                        <?php
                $sql = <<<DML
SELECT sis.sisid AS codigo,
       sis.sisdsc AS descricao
  FROM seguranca.sistema sis
  WHERE sis.sisstatus = 'A'
DML;
                $db->monta_combo(
                    'sisid',
                    $sql,
                    'S',
                    'Selecione um programa',
                    null,
                    null,
                    null,
                    null,
                    'N',
                    'sisid',
                    null,
                    (isset($retorno['sisid'])?$retorno['sisid']:' '),
                    null,
                    'required="required" class="form-control" style="width=100%;"'
                );
                ?>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Sistemas do demandas</label>
                                    <div class="col-lg-10">
                                        <?php
                $sql = <<<DML
                        
                        select  
						s.sidid  AS codigo, 
						upper(s.sidabrev) || ' - ' || s.siddescricao AS descricao 
					from 
						demandas.sistemadetalhe s
					left join demandas.sistemacelula c on s.sidid = c.sidid  
					where  s.sidstatus = 'A'
					AND celid = 2 -- Celula do Daniel
					order by s.sidabrev
DML;
                $db->monta_combo(
                    'sidid',
                    $sql,
                    'S',
                    'Selecione um programa',
                    null,
                    null,
                    null,
                    null,
                    'N',
                    'sidid',
                    null,
                    (isset($retorno['sidid'])?$retorno['sidid']:' '),
                    null,
                    'required="required" class="chosen-select form-control" style="width=100%;"'
                );
                ?>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Cor</label>
                                    <div class="col-lg-10">
                                        <input class='simple_color normal' name="subprgcolor" id="subprgcolor"
                           value="<?php echo (isset($retorno['subprgcolor'])
                                              ?$retorno['subprgcolor']
                                              :'#2b60de'); ?>" />
                                    </div>
                                </div>
                                <div class="form-group">
                                    <div class="col-lg-10 col-lg-offset-2">
                                        <?php if (isset($retorno['subprgid']) & !empty($retorno['subprgid'])): ?>
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
<?php listarSubprogramas($_GET); ?>
<script type="text/javascript" language="javascript">
jQuery(document).ready(function() {
    inicializarFormulario();
    jQuery('.simple_color').simpleColor({
        cellWidth:20,
        cellHeight:20,
        border:'1px solid #333333'
    });
});
</script>
