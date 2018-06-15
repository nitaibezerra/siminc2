<form id="formulario" name="formulario" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-medida" />
    <input name="ipnid" id="ipnid" type="hidden" value="<?php echo $mIndicadorpnc->ipnid; ?>">
    <input name="$mpnid" id="$mpnid" type="hidden" value="<?php echo $mIndicadorpnc->mpnid; ?>">

    <?php
    echo $simec->input('mpnid', 'mpnid', $mIndicadorpnc->mpnid);
    echo $simec->input('ipndsc', 'ipndsc', $mIndicadorpnc->ipndsc, ['required', 'maxlength' => 1000]);
    echo $simec->input('prsano', 'prsano', $mIndicadorpnc->prsano, ['required', 'maxlength' => 4]);
    echo $simec->input('ipncod', 'ipncod', $mIndicadorpnc->ipncod, ['required', 'maxlength' => 3]);
    ?>

    <div class="form-group">
        <div class="text-center">
            <button class="btn btn-primary" id="#btn-salvar-medida" type="submit"><i class="fa fa-check"></i>&nbsp;Salvar</button>
        </div>
    </div>
</form>

<script type="text/javascript">
    $(function(){

        $('#btn-salvar-medida').click(function(){
            options = {
                success: function() {
                    // Carregar a listagem
                    jQuery("#div_listagem_medida").load('?modulo=apoio/metapnc_form&acao=A&req=listar-medida&$mpnid='+$('#$mpnid').val());

                    // Zerar o formulário
                    jQuery("#div_formulario_medida").load('?modulo=apoio/metapnc_form&acao=A&req=formulario-medida&$mpnid='+$('#$mpnid').val());
                }
            }

            jQuery("#formulario-medida").ajaxForm(options).submit();
            return false;
        });

    });
</script>
