<?php
/**
 * Formulário de seleção de UNICODs.
 * $Id: formUO.inc 94769 2015-03-04 17:44:31Z werteralmeida $
 */
?>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript">
$(document).ready(function(){
    $('#back').click(function(){
        window.location.href = 'proporc.php?modulo=inicio&acao=C';
    });
    $('#next').click(function(){
        validarFormulario([], 'filtrouo', 'selecionarUnicod');
    });
});
</script>
<form name="filtrouo" id="filtrouo" method="POST" role="form">
    <input type="hidden" name="requisicao" id="requisicao" />
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="unicod">Unidade Orçamentária (UO):</label>
        </div>
        <div class="col-md-10">
            <?php
            $whereAdicional = array();
            if ($dadosuo) {
                $dml = new Simec_DB_DML("uni.unicod = :unicod");
                $dml->addParam('unicod', $dadosuo);
                $whereAdicional[] = (string)$dml;
            }

            inputComboUnicod($dados['unicod'], $whereAdicional);
            ?>
        </div>
    </div>
    <hr />
    <br />
    <button type="button" class="btn btn-warning" id="back">Voltar</button>
    <button type="button" class="btn btn-primary" id="next">Avançar</button>
</form>