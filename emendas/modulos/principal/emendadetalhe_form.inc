<form id="formulario-detalhe" name="formulario-detalhe" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-detalhe" />
    <input name="emdid" id="emdid" type="hidden" value="<?php echo $mEmendadetalhe->emdid; ?>">
    <input name="emeid" type="hidden" value="<?php echo $mEmendadetalhe->emeid; ?>">

    <?php
    echo $simec->select('gndcod', 'GND', $mEmendadetalhe->gndcod, (new Public_Model_Gnd())->recuperarSqlCombo(['gndcod', 'gnddsc']));
    echo $simec->select('mapcod', 'Mod. Aplicação', $mEmendadetalhe->mapcod, (new Public_Model_ModalidadeAplicacao())->recuperarSqlCombo(['mapcod', 'mapdsc']));
    echo $simec->select('foncod', 'Fonte', $mEmendadetalhe->foncod, (new Public_Model_FonteRecurso())->recuperarSqlCombo(['foncod', 'fondsc'], ['grfid = 1']));
    echo $simec->input('emdvalor', 'Valor', $mEmendadetalhe->emdvalor, ['class'=>'valor']);
    ?>

    <div class="form-group">
        <div class="text-center">
            <button class="btn btn-primary" type="button" id="btn-salvar-detalhe"><i class="fa fa-check"></i>&nbsp;Salvar</button>
        </div>
    </div>
</form>

<script type="text/javascript">
    $(function(){
        $('#btn-salvar-detalhe').click(function(){
            options = {
                success : function() {
                    // Carregar a listagem
                    jQuery("#div_listagem_detalhe").load('?modulo=principal/emenda_form&acao=A&req=listar-detalhe&emeid='+$('#emeid').val());

                    // Zerar o formulário
                    jQuery("#div_formulario_detalhe").load('?modulo=principal/emenda_form&acao=A&req=formulario-detalhe&emeid='+$('#emeid').val());
                }
            }

            jQuery("#formulario-detalhe").ajaxForm(options).submit();
        });
    });
</script>