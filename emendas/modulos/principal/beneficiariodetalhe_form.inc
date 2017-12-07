<form id="formulario-detalhe" name="formulario-detalhe" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-detalhe" />
    <input name="bedid" id="bedid" type="hidden" value="<?php echo $mBeneficiarioDetalhe->bedid; ?>">
    <input name="benid" type="hidden" value="<?php echo $mBeneficiarioDetalhe->benid; ?>">

    <div class="col-sm-6">

        <?php
        echo $simec->select('emdid', 'GND/Valor', $mBeneficiarioDetalhe->emdid, (new Emendas_Model_EmendaDetalhe())->recuperarSqlCombo(null, ['emeid = ' . (int)$mBeneficiario->emeid]));
        echo $simec->valor('bedvalorpriorizado', 'Vl. Priorizado', $mBeneficiarioDetalhe->bedvalorpriorizado);
        ?>
    </div>
    <div class="col-sm-6">
        <?php
        echo $simec->valor('bedvalor', 'Vl. Beneficiário', $mBeneficiarioDetalhe->bedvalor);
        $valorContingenciado = ($mBeneficiarioDetalhe->bedvalor - $mBeneficiarioDetalhe->bedvalorpriorizado);
        echo $simec->valor('bedvalorcontingenciado', 'Vl. Contingencia', ($valorContingenciado ? $valorContingenciado : null));
        ?>

    </div>

    <div class="form-group">
        <div class="text-center">
            <button class="btn btn-primary" type="button" id="btn-salvar-detalhe"><i class="fa fa-check"></i>&nbsp;Salvar</button>
        </div>
    </div>
</form>


<script type="text/javascript">
    $(function(){

        $('#formulario-detalhe').on('change', '#bedvalor', function(){
            if(!$('#bedvalorpriorizado').val()){
                $('#bedvalorpriorizado').val($(this).val());
            }
        });

        $('#formulario-detalhe').on('keyup', '#bedvalorpriorizado', function(){
            setTimeout(function(){
                if($('#bedvalor').val() && $('#bedvalorpriorizado').val()){
                    var valorBeneficiario = str_replace(['.', ','], ['', '.'], $('#bedvalor').val());
                    var valorPriorizado = str_replace(['.', ','], ['', '.'], $('#bedvalorpriorizado').val());
                    var valorContingenciado = parseFloat(valorBeneficiario) - parseFloat(valorPriorizado)

                    $('#bedvalorcontingenciado').val(number_format(valorContingenciado, 2, ',', '.'));
                }
            }, 100);
        });

        $('#formulario-detalhe').on('keyup', '#bedvalorcontingenciado', function(){
            setTimeout(function(){
                if($('#bedvalor').val() && $('#bedvalorcontingenciado').val()){
                    var valorBeneficiario = str_replace(['.', ','], ['', '.'], $('#bedvalor').val());
                    var valorContingenciado = str_replace(['.', ','], ['', '.'], $('#bedvalorcontingenciado').val());
                    var valorPriorizado = (valorBeneficiario) - parseFloat(valorContingenciado)

                    $('#bedvalorpriorizado').val(number_format(valorPriorizado, 2, ',', '.'));
                }
            }, 100);

        });


        $('#btn-salvar-detalhe').click(function(){
            options = {
                success : function() {
                    // Carregar a listagem
                    jQuery("#div_listagem_detalhe").load('?modulo=principal/beneficiario_form&acao=A&req=listar-detalhe&benid='+$('#benid').val());

                    // Zerar o formulário
                    jQuery("#div_formulario_detalhe").load('?modulo=principal/beneficiario_form&acao=A&req=formulario-detalhe&benid='+$('#benid').val());
                }
            }

            jQuery("#formulario-detalhe").ajaxForm(options).submit();
        });
    });
</script>