<?php

    $mPropostaDetalhe = new Proposta_Model_PropostaDetalhe($prdid);
    if(!$mPropostaDetalhe->prdid){
        $mPropostaDetalhe->proid = $proid;
    }
    
    $totais = $mPropostaDetalhe->buscarTotais();
?>

<form id="formulario-detalhe" name="formulario-detalhe" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-detalhe" />
    <input name="prdid" id="prdid" type="hidden" value="<?php echo $mPropostaDetalhe->prdid; ?>">
    <input name="proid" type="hidden" value="<?php echo $mPropostaDetalhe->proid; ?>">

    <input type="hidden" name="totalCusteioValor" value="<?= $totais['custeioValor']; ?>" />
    <input type="hidden" name="totalCapitalValor" value="<?= $totais['capitalValor']; ?>" />
    <input type="hidden" name="totalCusteioExpansao" value="<?= $totais['custeioExpansao']; ?>" />
    <input type="hidden" name="totalCapitalExpansao" value="<?= $totais['capitalExpansao']; ?>" />
    
    <?php
        echo $simec->select('ndpid', 'Natureza Despesa', $mPropostaDetalhe->ndpid, (new Public_Model_NaturezaDespesa())->recuperarSqlCombo(['ndpcod', 'ndpdsc']), ['required']);
        echo $simec->select('iducod', 'IDUSO', ($mPropostaDetalhe->iducod ? $mPropostaDetalhe->iducod : 0), (new Public_Model_IdentifUso())->recuperarSqlCombo(['iducod', 'idudsc']), ['required']);
        echo $simec->select('foncod', 'Fonte', ($mPropostaDetalhe->foncod ? $mPropostaDetalhe->foncod : 105), (new Public_Model_FonteRecurso())->recuperarSqlCombo(['foncod', 'fondsc'], ['grfid = 1']), ['required']);
        echo $simec->select('idoid', 'IDOC', ($mPropostaDetalhe->idoid ? $mPropostaDetalhe->idoid : 1), (new Public_Model_Idoc())->recuperarSqlCombo(['idocod', 'idodsc']), ['required']);
        echo $simec->moedaInteiro('prdvalor', 'Valor', $mPropostaDetalhe->prdvalor, ['required']);
        echo $simec->moedaInteiro('prdvalorexpansao', 'Valor Expansão', $mPropostaDetalhe->prdvalorexpansao);
    ?>

    <div class="form-group">
        <div class="text-center">
            <button class="btn btn-primary" type="button" id="btn-salvar-detalhe"><i class="fa fa-check"></i>&nbsp;Salvar</button>
            <a href="?modulo=principal/proposta_form&acao=A&proid=<?= $mPropostaDetalhe->proid; ?>" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
            <?php if($mPropostaDetalhe->prdid){ ?>
                <a href="?modulo=proposta/propostadetalhe_form&acao=A&req=excluir&prdid=<?php echo $mPropostaDetalhe->prdid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
            <?php } ?>
        </div>
    </div>
</form>


<script type="text/javascript">
    $(function(){
        
        $('#prdvalor').keyup(function(){
            setTimeout(function(){
                validarValor();
            }, 500);
        });
        
        $('#prdvalorexpansao').keyup(function(){
            setTimeout(function(){
                validarExpansao();
            }, 500);
        });
        
        $('#ndpid').change(function(){
            validarValor();
            validarExpansao();
        });
        
        $('#btn-salvar-detalhe').click(function(){
            if (verificaPreenchimentoCamposFormularioDetalhe()){
                if(verificarFormularioCusteio() === true){
                    if(validarMaximoValorCusteio() === true && validarMaximoExpansaoCusteio() === true){
                        submeterFormularioDetalhe();
                    }
                } else {
                    if(validarMaximoValorCapital() === true && validarMaximoExpansaoCapital() === true){
                        submeterFormularioDetalhe();
                    }
                }

                validarValor();
            }

        });
    });

    function submeterFormularioDetalhe(){
        options = {
            success: function() {
                // Carregar a listagem
                jQuery("#div_listagem_detalhe").load('?modulo=principal/proposta_form&acao=A&req=listar-detalhe&proid='+$('#proid').val());

                // Zerar o formulário
                jQuery("#div_formulario_detalhe").load('?modulo=principal/proposta_form&acao=A&req=formulario-detalhe&proid='+$('#proid').val());
            }
        };

        jQuery("#formulario-detalhe").ajaxForm(options).submit();
    }

    function verificaPreenchimentoCamposFormularioDetalhe(){
        var textoNatureza = $('#ndpid option:selected').text();
        if(textoNatureza === ""){
            swal({title: "Atenção!",text: "O campo Natureza Despesa está em branco!",type: "warning"},function(){$('#ndpid').focus();});
            return false;
        }
        var IDUSO = $('#iducod option:selected').text();
        if(IDUSO === ""){
            swal({title: "Atenção!",text: "O campo IDUSO está em branco!",type: "warning"},function(){$('#iducod').focus();});
            return false;
        }
        var foncod = $('#foncod option:selected').text();
        if(foncod === ""){
            swal({title: "Atenção!",text: "O campo Fonte está em branco!",type: "warning"},function(){$('#foncod').focus();});
            return false;
        }
        var idoid = $('#idoid option:selected').text();
        if(idoid === ""){
            swal({title: "Atenção!",text: "O campo IDOC está em branco!",type: "warning"},function(){$('#idoid').focus();});
            return false;
        }
        var prdvalor = $('#prdvalor').val();
        if(prdvalor === ""){
            swal({title: "Atenção!",text: "O campo Valor está em branco!",type: "warning"},function(){$('#prdvalor').focus();});
            return false;
        }
        return true;
    }
    /**
     * Verifica se o cadastro de dado financeiro é de custeio ou capital
     * 
     * @returns {Boolean} retorna true caso seja de custeio
     */
    function verificarFormularioCusteio(){
        var textoNatureza = $('#ndpid option:selected').text();
        var resultado = false;
        if(textoNatureza !== ""){
            var numeroNatureza = $('#ndpid option:selected').text().substr(0,1);
            if(numeroNatureza <= 3){
                resultado = true;
            }
        }
        
        return resultado;
    }

    /*
     * Se o Valor informado no formulário mais o valor total referentes a 
     * custeio ou capital for maior que o limite de custeio ou capital 
     * respectivamete, o sistema exibe uma critica.
     */
    function validarValor(){
        if(verificarFormularioCusteio() === true){
            if(validarMaximoValorCusteio() === false){
                var totalValorCusteio = buscarTotalValorCusteio();
                swal({
                    title: 'Atenção',
                    text: 'A soma de valores de custeio '+ number_format(totalValorCusteio.toFixed(2), 2, ',', '.')+ ' ultrapassou o Limite Pré-Pi de '+ $('#limite_valor_custeio').text()+ '. Por favor, informe um número inferior ou ajuste os valores cadastrados.',
                    type: "error"
                },
                function(){
                    $('input[name="prdvalor"]').val('');
                });
            }
        } else {
            if(validarMaximoValorCapital() === false){
                var totalValorCapital = buscarTotalValorCapital();
                swal({
                    title: 'Atenção',
                    text: 'A soma de valores de capital '+ number_format(totalValorCapital.toFixed(2), 2, ',', '.')+ ' ultrapassou o Limite Pré-Pi de '+ $('#limite_valor_capital').text()+ '. Por favor, informe um número inferior ou ajuste os valores cadastrados.',
                    type: "error"
                },
                function(){
                    $('input[name="prdvalor"]').val('');
                });
            }
        }
    }
    
    /*
     * Se a Expansão informada no formulário mais o valor total referentes a 
     * custeio ou capital for maior que o limite de custeio ou capital 
     * respectivamete, o sistema exibe uma critica.
     */
    function validarExpansao(){
        if(verificarFormularioCusteio() === true){
            if(validarMaximoExpansaoCusteio() === false){
                var totalExpansaoCusteio = buscarTotalExpansaoCusteio();
                swal({
                    title: 'Atenção',
                    text: 'A soma de Expansão de custeio '+ number_format(totalExpansaoCusteio.toFixed(2), 2, ',', '.')+ ' ultrapassou o Limite Pré-Pi de '+ $('#limite_expansao_custeio').text()+ '. Por favor, informe um número inferior ou ajuste os valores cadastrados.',
                    type: "error"
                },
                function(){
                    $('input[name="prdvalorexpansao"]').val('');
                });
            }
        } else {
            if(validarMaximoExpansaoCapital() === false){
                var totalExpansaoCapital = buscarTotalExpansaoCapital();
                swal({
                    title: 'Atenção',
                    text: 'A soma de Expansão de capital '+ number_format(totalExpansaoCapital.toFixed(2), 2, ',', '.')+ ' ultrapassou o Limite Pré-Pi de '+ $('#limite_expansao_capital').text()+ '. Por favor, informe um número inferior ou ajuste os valores cadastrados.',
                    type: "error"
                },
                function(){
                    $('input[name="prdvalorexpansao"]').val('');
                });
            }
        }
    }
    
    /**
     * Busca total de custeio para o campo Valor
     * 
     * @returns {buscarTotalValorCusteio.totalValorCusteio}
     */
    function buscarTotalValorCusteio(){
        var valorCusteioForm = $('input[name="prdvalor"]').val()? $('input[name="prdvalor"]').val(): 0;
        var totalValorCusteioLista = $('input[name="totalCusteioValor"]').val()? $('input[name="totalCusteioValor"]').val(): 0;
        var totalValorCusteio = textToFloat(valorCusteioForm) + textToFloat(totalValorCusteioLista);
        
        return totalValorCusteio;
    }
    
    /**
     * Busca total de custeio para o campo Expansao
     * 
     * @returns {buscarTotalExpansaoCusteio.totalExpansaoCusteio}
     */
    function buscarTotalExpansaoCusteio(){
        var expansaoCusteioForm = $('input[name="prdvalorexpansao"]').val()? $('input[name="prdvalorexpansao"]').val(): 0;
        var totalExpansaoCusteioLista = $('input[name="totalCusteioExpansao"]').val()? $('input[name="totalCusteioExpansao"]').val(): 0;
        var totalExpansaoCusteio = textToFloat(expansaoCusteioForm) + textToFloat(totalExpansaoCusteioLista);
        
        return totalExpansaoCusteio;
    }
    
    /**
     * Busca total de capital para o campo Expansao
     * 
     * @returns {buscarTotalExpansaoCapital.totalExpansaoCapital}
     */
    function buscarTotalExpansaoCapital(){
        var expansaoCapitalForm = $('input[name="prdvalorexpansao"]').val()? $('input[name="prdvalorexpansao"]').val(): 0;
        var totalExpansaoCapitalLista = $('input[name="totalCapitalExpansao"]').val()? $('input[name="totalCapitalExpansao"]').val(): 0;
        var totalExpansaoCapital = textToFloat(expansaoCapitalForm) + textToFloat(totalExpansaoCapitalLista);
        
        return totalExpansaoCapital;
    }
    
    /**
     * Busca total de capital para o campo Valor
     * 
     * @returns {buscarTotalValorCapital.totalValorCapital}
     */
    function buscarTotalValorCapital(){
        var valorCapitalForm = $('input[name="prdvalor"]').val()? $('input[name="prdvalor"]').val(): 0;
        var totalValorCapitalLista = $('input[name="totalCapitalValor"]').val()? $('input[name="totalCapitalValor"]').val(): 0;
        var totalValorCapital = textToFloat(valorCapitalForm) + textToFloat(totalValorCapitalLista);
        
        return totalValorCapital;
    }
    
    /**
     * Verifica se o maximo de expansao custeio foi alcançado
     * 
     * @returns {Boolean}
     */
    function validarMaximoExpansaoCusteio(){
        var limiteExpansaoCusteio = $('#limite_expansao_custeio').text()? textToFloat($('#limite_expansao_custeio').text()): 0;
        var totalExpansaoCusteio = buscarTotalExpansaoCusteio();
        var resultado = true;

        if(totalExpansaoCusteio > limiteExpansaoCusteio){
            resultado = false;
        }
        
        return resultado;
    }
    
    /**
     * Verifica se o maximo de expansao capital foi alcançado
     * 
     * @returns {Boolean}
     */
    function validarMaximoExpansaoCapital(){
        var limiteExpansaoCapital = $('#limite_expansao_capital').text()? textToFloat($('#limite_expansao_capital').text()): 0;
        var totalExpansaoCapital = buscarTotalExpansaoCapital();
        var resultado = true;

        if(totalExpansaoCapital > limiteExpansaoCapital){
            resultado = false;
        }
        
        return resultado;
    }
    
    /**
     * Verifica se o maximo de valor custeio foi alcançado
     * 
     * @returns {Boolean}
     */
    function validarMaximoValorCusteio(){
        var limiteValorCusteio = $('#limite_valor_custeio').text()? textToFloat($('#limite_valor_custeio').text()): 0;
        var totalValorCusteio = buscarTotalValorCusteio();
        var resultado = true;

        if(totalValorCusteio > limiteValorCusteio){
            resultado = false;
        }
        
        return resultado;
    }
    
    /**
     * Verifica se o maximo de valor capital foi alcançado
     * 
     * @returns {Boolean}
     */
    function validarMaximoValorCapital(){
        var limiteValorCapital = $('#limite_valor_capital').text()? textToFloat($('#limite_valor_capital').text()): 0;
        var totalValorCapital = buscarTotalValorCapital();
        var resultado = true;

        if(totalValorCapital > limiteValorCapital){
            resultado = false;
        }
        
        return resultado;
    }
    
</script>