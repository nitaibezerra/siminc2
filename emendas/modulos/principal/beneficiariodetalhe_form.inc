<form id="formulario-detalhe" name="formulario-detalhe" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-detalhe" />
    <input name="bedid" id="bedid" type="hidden" value="<?php echo $mBeneficiarioDetalhe->bedid; ?>">
    <input name="benid" id="benid" type="hidden" value="<?php echo $mBeneficiarioDetalhe->benid; ?>">
    <input name="emeid" id="emeid" type="hidden" value="<?php echo $emeid; ?>">
    <?php
    $valoresGnd = (new Emendas_Model_EmendaDetalhe())->recuperarValorGnd(null, ['emeid = ' . $emeid]);
    foreach ($valoresGnd as $linha) {
        echo '<input type="hidden" name="vlrGnd'.$linha['emdid'].'" id="vlrGnd'.$linha['emdid'].'" value="'. number_format($linha['emdvalor'], 2, ',', '.').'">';
    }
    $valorSomaGnd = (new Emendas_Model_BeneficiarioDetalhe)->recuperarSomaGnd($mBeneficiarioDetalhe->benid);
    foreach ($valorSomaGnd as $linha) {
        echo '<input type="hidden" name="vlrSomaGnd'.$linha['emdid'].'" id="vlrSomaGnd'.$linha['emdid'].'" value="'. number_format($linha['valor_total'], 2, ',', '.').'">';
    }    
    ?>
    
    
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
        echo $simec->valor('bedvalorcontingenciado', 'Vl. Contingenciado', ($valorContingenciado ? $valorContingenciado : null));
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
                    jQuery("#div_formulario_detalhe").load('?modulo=principal/beneficiario_form&acao=A&req=formulario-detalhe&benid='+$('#benid').val()+'&bedid='+$('#bedid').val()+'&emeid='+$('#emeid').val());
                }
            }

            jQuery("#formulario-detalhe").ajaxForm(options).submit();
        });
        
        $("#bedvalor").keyup(function(){
            var vlrBedTotal = $("#vlrGnd"+$("#emdid").val()).val() ? $("#vlrGnd"+$("#emdid").val()).val() : 0;
            var vlrSomaTotal = $("#vlrSomaGnd"+$("#emdid").val()).val() ? $("#vlrSomaGnd"+$("#emdid").val()).val() : 0;
            var vlrBed = $(this).val() ? $(this).val() : 0;
            var totalGeral = textToFloat(vlrBed) + textToFloat(vlrSomaTotal);   
            vlrBedTotal = textToFloat(vlrBedTotal);
            if(totalGeral > vlrBedTotal){
                swal({
                    title: 'Atenção',
                    text: 'A soma dos valores do Beneficiario '+ number_format(totalGeral.toFixed(2), 2, ',', '.')+ ' ultrapassou o Limite GND de '+ number_format(vlrBedTotal.toFixed(2), 2, ',', '.')+ '. Por favor, informe um número inferior ou ajuste os valores cadastrados.',
                    type: "error"
                },
                function(){
                    $("#bedvalor").val('');
                    $("#bedvalorpriorizado").val('');
                });
            }         
        });
    });
</script>