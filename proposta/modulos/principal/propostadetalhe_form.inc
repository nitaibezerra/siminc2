<?php

    $mPropostaDetalhe = new Proposta_Model_PropostaDetalhe($prdid);
    if(!$mPropostaDetalhe->prdid){
        $mPropostaDetalhe->proid = $proid;
    }
?>

<form id="formulario-detalhe" name="formulario-detalhe" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-detalhe" />
    <input name="prdid" id="prdid" type="hidden" value="<?php echo $mPropostaDetalhe->prdid; ?>">
    <input name="proid" type="hidden" value="<?php echo $mPropostaDetalhe->proid; ?>">

    <?php
        echo $simec->select('ndpid', 'Natureza Despesa', $mPropostaDetalhe->ndpid, (new Public_Model_NaturezaDespesa())->recuperarSqlCombo(['ndpcod', 'ndpdsc']), ['required']);
        echo $simec->select('iducod', 'IDUSO', ($mPropostaDetalhe->iducod ? $mPropostaDetalhe->iducod : 0), (new Public_Model_IdentifUso())->recuperarSqlCombo(['iducod', 'idudsc']), ['required']);
        echo $simec->select('foncod', 'Fonte', ($mPropostaDetalhe->foncod ? $mPropostaDetalhe->foncod : 105), (new Public_Model_FonteRecurso())->recuperarSqlCombo(['foncod', 'fondsc'], ['grfid = 1']), ['required']);
        echo $simec->select('idoid', 'IDOC', ($mPropostaDetalhe->idoid ? $mPropostaDetalhe->idoid : 1), (new Public_Model_Idoc())->recuperarSqlCombo(['idocod', 'idodsc']), ['required']);
        echo $simec->valor('prdvalor', 'Valor', $mPropostaDetalhe->prdvalor, ['required']);
        echo $simec->valor('prdvalorexpansao', 'Valor Expansão', $mPropostaDetalhe->prdvalorexpansao);
    ?>

    <div class="form-group">
        <div class="text-center">
            <button class="btn btn-primary" type="button" id="btn-salvar-detalhe"><i class="fa fa-check"></i>&nbsp;Salvar</button>
            <a href="?modulo=proposta/propostadetalhe&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
            <?php if($mPropostaDetalhe->prdid){ ?>
                <a href="?modulo=proposta/propostadetalhe_form&acao=A&req=excluir&prdid=<?php echo $mPropostaDetalhe->prdid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
            <?php } ?>
        </div>
    </div>
</form>


<script type="text/javascript">
    $(function(){
        
        $('#prdvalor').keyup(function(){
            validarValor();
        });
        
        $('#btn-salvar-detalhe').click(function(){
            options = {
                success : function() {
                    // Carregar a listagem
                    jQuery("#div_listagem_detalhe").load('?modulo=principal/proposta_form&acao=A&req=listar-detalhe&proid='+$('#proid').val());

                    // Zerar o formulário
                    jQuery("#div_formulario_detalhe").load('?modulo=principal/proposta_form&acao=A&req=formulario-detalhe&proid='+$('#proid').val());
                }
            }

            jQuery("#formulario-detalhe").ajaxForm(options).submit();
        });
    });

    function validarValor(){
        var textoNatureza = $('#ndpid option:selected').text();
        if(textoNatureza != ""){
            var numeroNatureza = $('#ndpid option:selected').text().substr(0,1);
            if(numeroNatureza <= 3){
                validarMaximoValorCusteio();
            } else {
//                validarMaximoValorCapital();
            }
        }
    }

    function validarMaximoValorCusteio(){

        var limiteValorCusteio = $('#limite_valor_custeio').text()? str_replace(['.', ','], ['', '.'], $('#limite_valor_custeio').text()): 0;
        var valorCusteioForm = $('input[name="prdvalor"]').val()? str_replace(['.', ','], ['', '.'], $('input[name="prdvalor"]').val()): 0;
        var totalValorCusteioLista = $('input[name="totalCusteioValor"]').val()? str_replace(['.', ','], ['', '.'], $('input[name="totalCusteioValor"]').val()): 0;
        
        var totalValorCusteio = textToFloat(valorCusteioForm) + textToFloat(totalValorCusteioLista);
        /*
         * Se o valor informado no formulário mais o valor total da lista dos que
         * são de custeio for maior que o limite de custeio da funcional pra 
         * Unidade o sistema exibe uma critica.
         */
        if(totalValorCusteio > limiteValorCusteio){
            swal(
                'Atenção',
                'A soma de valores de custeio '+ number_format(totalValorCusteio.toFixed(2), 2, ',', '.')+ ' ultrapassou o Limite Pré-Pi de '+ $('#limite_valor_custeio').text()+ '. Por favor, informe um número inferior ou ajuste os valores cadastrados.',
                'error'
            );
            $('input[name="prdvalor"]').val('');
        }
    }
    
</script>