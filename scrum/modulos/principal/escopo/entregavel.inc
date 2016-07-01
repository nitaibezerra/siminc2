<?php


//ver($_SESSION,d);
/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */
$retorno = processaRequisicao("Entregavel");
//$titulo = 'Entregáveis';
require_once APPRAIZ . 'scrum/modulos/principal/escopo/topo.inc';
//ver($retorno,d);
if (is_null($retorno) && isset($_GET)) {
    foreach ($_GET as $k => $v) {
        $retorno[$k] = $v;
    }
}
?>
            <div class="row">
                <div class="col-lg-12">
                    <div class="page-header">
                        <h1 id="forms">Entregáveis</h1>
                    </div>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-12">
                    <div class="well">
                        <form name="formPrincipal" id="formPrincipal" method="POST" class="form-horizontal">
                            <fieldset>
                                <input type="hidden" name="action" id="action" value="" />
                                <input type="hidden" name="entid" id="entid" value="<?php echo isset($retorno['entid']) ? $retorno['entid'] : ''; ?>" />
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
                                                'prgid', $sql, 'S', 'Selecione um programa', 'carregaComboSubprograma', null, null, null, 'N', 'prgid', null, (isset($retorno['prgid']) ? $retorno['prgid'] : null), '', 'class="form-control " style="width=100%;"'
                                        );
                                        if (isset($retorno['prgid'])):
                                            ?>
                                            <script type="text/javascript" language="javascript">
                                                jQuery(document).ready(function() {
                                                    carregaComboSubprograma(<?php echo $retorno['prgid']; ?>);
                                                });
                                            </script>
                                            <?php if (isset($retorno['subprgid'])): ?>
                                                <script type="text/javascript" language="javascript">
                                                    jQuery(document).ready(function() {
                                                        jQuery('#subprgid').val(<?php echo $retorno['subprgid']; ?>);
                                                    });
                                                </script>
    <?php endif;
endif;
?>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Subprograma</label>
                                    <div class="col-lg-10">
<?php
$db->monta_combo(
        'subprgid', array(), 'S', 'Selecione um subprograma', 'carregaComboEstoria', null, null, null, 'N', 'subprgid', null, (isset($retorno['subprgid']) ? $retorno['subprgid'] : null)
        , ''
        , 'class="form-control" style="width=100%;"'
);
if (isset($retorno['subprgid'])):
    ?>
                                            <script type="text/javascript" language="javascript">
                                                jQuery(document).ready(function() {
                                                    carregaComboEstoria(<?php echo $retorno['subprgid']; ?>);
                                                });
                                            </script>
                                            <?php if (isset($retorno['estid'])): ?>
                                                <script type="text/javascript" language="javascript">
                                                    jQuery(document).ready(function() {
                                                        jQuery('#estid').val(<?php echo $retorno['estid']; ?>);
                                                    });
                                                </script>
                                            <?php
                                            endif;
                                        endif;
                                        ?>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Estória</label>
                                    <div class="col-lg-10">
<?php
$db->monta_combo(
        'estid', array(), 'S', 'Selecione uma estória', null, null, null, null, 'N', 'estid', null, (isset($retorno['estid']) ? $retorno['estid'] : null)
        , ''
        , 'class="form-control"  required="required"  style="width=20%;"'
);
?>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Solicitante</label>
                                    <div class="col-lg-10">
                                        <input type="text" name="" value="<?php echo $_SESSION['usunome'] ?>" class="form-control" disabled="disabled">
                                        <input type="hidden" name="usucpfsol" value="<?php echo $_SESSION['usucpf'] ?>" >
                                        
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Descrição</label>
                                    <div class="col-lg-10 ">
                                        <textarea class="form-control" rows="3" id="entdsc" name="entdsc" required><?php echo (isset($retorno['entdsc'])) ? $retorno['entdsc'] : null ?></textarea>
                                        <span class="help-block">Descreva a demanda.</span>
                                        <?php
//                                        echo campo_textarea(
//                                                'entdsc', 'N', 'S', null, null, 10, 2000, null, null, null, null, null, (isset($retorno['entdsc']) ? $retorno['entdsc'] : null)
//                                        );
                                        ?>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label">Status</label>
                                    <div class="col-lg-10">
                                        <?php
                                        $sql = <<<DML
    SELECT ets.entstid AS codigo, ets.entstdsc AS descricao
    FROM scrum.entstatus ets ORDER BY ets.entstid ASC
DML;
                                        if (isset($retorno['entstid']))
                                        {
                                            $entstid = $retorno['entstid'];
                                            $habilitado = 'S';
                                            $status = 'Selecione um status';
                                            $required = 'S';
                                        } else
                                        {
                                            $entstid = null;
                                            $habilitado = 'N';
                                            $status = 'EM CRIAÇÃO';
                                            $required = 'N';
                                        }

                                        $sql = sprintf($sql, $entstid);
                                        $db->monta_combo(
                                                'entstid', $sql, $habilitado, $status, null, null, null, null, $required, 'entstid'
                                                , ''
                                                , ''
                                                , ''
                                                , 'class="form-control" style="width=100%;"'
                                        );
                                        ?>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <div class="col-lg-10 col-lg-offset-2">
                                        <?php if (isset($retorno['entid']) & !empty($retorno['entid'])): ?>
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
                    <div>
                        <fieldset>
                        <div>
                            <?php listarEntregaveis($_GET); ?>
                        </div>
                        </fieldset>
                    </div>
                </div>
            </div>




<!--<form id="formPrincipal" name="formPrincipal" method="POST">-->
<!--    <input type="hidden" name="action" id="action" value="" />
    <input type="hidden" name="entid" id="entid" value="<?php echo isset($retorno['entid']) ? $retorno['entid'] : ''; ?>" />
    <center>
        <table class="tabela" cellspacing="1" cellpadding="3" align="center">
<?php // if ($db->testa_superuser()):  ?>
<?php if (false): ?>
                <tr>
                    <td class="SubTituloDireita">Responsável:</td>
                    <td><?php
                        $sql = <<<DML
SELECT usu.usucpf AS codigo,
       usu.usunome AS descricao
  FROM seguranca.usuario usu
  WHERE usu.usucpf = '%s'
DML;
                        $sql = sprintf($sql, $retorno['usucpfresp']);
                        $db->monta_combo(
                                'usucpfresp', $sql, 'N', '', null, null, null, null, 'N', 'usucpfresp', null, (isset($retorno['usucpfresp']) ? $retorno['usucpfresp'] : null), null
                        );
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Tempo de execução (em horas):</td>
                    <td><?php
                    echo campo_texto(
                            'enthrsexec', 'N', 'S', null, 5, 4, '####', null, null, null, null, 'id="enthrsexec"', null, (isset($retorno['enthrsexec']) ? $retorno['enthrsexec'] : null)
                    );
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Ordem de execução (na sprint):</td>
                    <td><?php
                        echo campo_texto(
                                'entordsprint', 'N', 'S', null, 5, 4, '####', null, null, null, null, 'id="entordsprint"', null, (isset($retorno['entordsprint']) ? $retorno['entordsprint'] : null)
                        );
                        ?>
                    </td>
                </tr>
                    <?php endif; ?>
            <tr class="tabela_rodape">
                <td>&nbsp;</td>
                <td>
                    <?php if (isset($retorno['entid']) & !empty($retorno['entid'])): ?>
                        <button class="botao" id="atualizar">Atualizar</button>
                        <button class="botao" id="voltar">Voltar</button>
                    <?php else: ?>
                        <button class="botao" id="inserir">Inserir</button>
                        <input class="botao" type="reset" value="Limpar" />
                        <button class="botao" id="buscar">Buscar</button>
                    <?php endif; ?>
                </td>
            </tr>
        </table>
    </center>-->
<!--</form>-->
<br />
                    
<script type="text/javascript" language="javascript">
    jQuery(document).ready(function() {
        jQuery('.obrigatorio').attr('required', 'required');
        inicializarFormulario();
    });
</script>