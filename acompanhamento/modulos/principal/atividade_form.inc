<form id="formulario" name="formulario" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-atividade" />
    <input name="atiid" id="atiid" type="hidden" value="<?php echo $mAtividade->atiid; ?>">

    <?php
    echo $simec->input('atidsc', 'atidsc', $mAtividade->atidsc);
    echo $simec->input('atidata', 'atidata', $mAtividade->atidata, ['required']);
    echo $simec->input('atiproduto', 'atiproduto', $mAtividade->atiproduto, ['required', 'maxlength' => 2000]);
    echo $simec->input('atiquantidade', 'atiquantidade', $mAtividade->atiquantidade, ['required']);
    echo $simec->input('estuf', 'estuf', $mAtividade->estuf, ['required', 'maxlength' => 2]);
    echo $simec->input('muncod', 'muncod', $mAtividade->muncod, ['required', 'maxlength' => 7]);
    echo $simec->input('mpnid', 'mpnid', $mAtividade->mpnid);
    echo $simec->input('suoid', 'suoid', $mAtividade->suoid);
    echo $simec->input('janid', 'janid', $mAtividade->janid);
    ?>

    <div class="form-group">
        <div class="text-center">
            <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
            <a href="?modulo=apoio/atividade&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
            <?php if($mAtividade->atiid){ ?>
                <a href="?modulo=apoio/atividade_form&acao=A&req=excluir&atiid=<?php echo $mAtividade->atiid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
            <?php } ?>
        </div>
    </div>
</form>

<script type="text/javascript">
    $(function(){

        $('#formulario-atividade').on('change', '#bedvalor', function(){
            if(!$('#bedvalorpriorizado').val()){
                $('#bedvalorpriorizado').val($(this).val());
            }
        });

        $('#btn-salvar-atividade').click(function(){
            options = {
                success : function() {
                    // Carregar a listagem
                    jQuery("#div_listagem_atividade").load('?modulo=principal/monitorar-pnc&acao=A&req=listar-atividade&atiid='+$('#atiid').val());

                    // Zerar o formulário
                    jQuery("#div_formulario_atividade").load('?modulo=principal/monitorar-pnc&acao=A&req=formulario-atividade&atiid='+$('#atiid').val());
                }
            }

            jQuery("#formulario-atividade").ajaxForm(options).submit();
        });
    });
</script>