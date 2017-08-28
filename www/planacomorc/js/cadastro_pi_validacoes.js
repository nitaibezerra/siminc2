
    /**
     * Controla a obrigatóriedade dos campos do formulario de acordo com o enquadramento selecionado.
     *
     * @returns Array
     */
    function definirCamposObrigatorios(){
        var codigoEnquadramento = $('#eqdid').val();
        var listaObrigatorios = ['plititulo', 'plidsc', 'unicod', 'ungcod','eqdid', 'neeid', 'capid', 'mdeid'];

        // Se o formulário possui opções de manutenção item o sistema define como obrigatório o preenchimento dos itens de manutenção.
        if($('#maiid option').size() > 0){
            listaObrigatorios.push('maiid', 'masid');
        // Se o formulario não possui as opções de manutenção item o sistema lista como obrigatório as opções Objetivo PPA, Metas PPA, Iniciativa PPA
        } else {
            listaObrigatorios.push('oppid', 'mppid', 'pprid');
            
            // Verifica se o usuário escolheu um produto diferente de não se aplica para verificar a validação do cronograma físico.
            if($('#pprid').val() != intProdNaoAplica ){
                listaObrigatorios.push('pumid', 'picquantidade');
            }
        }

        // Se o código for diferente de Finalistico, o sistema não define como obrigatório o preenchimento das opções PNC.
        if(codigoEnquadramento == intEnqFinalistico){
            listaObrigatorios.push('mpnid', 'ipnid');
        }

        if($('#picedital').is(':checked')){
            listaObrigatorios.push('mes');
        }

        return listaObrigatorios;
    }

    function submeter(pliid) {
//        var codsubacao = $('#subacao').text();
//        if ('' === codsubacao) {
//            alert('O código da subação não pode ser deixado em branco.');
//            return false;
//        }

//        O número do PI será gerado somente após a aprovação do PI pela equipe de Coordenação.
//        var pi = document.getElementById("enquadramento").innerHTML
//            + codsubacao
//            + document.getElementById("nivel").innerHTML
//            + document.getElementById("apropriacao").innerHTML
//            + document.getElementById("codificacao").innerHTML
//            + document.getElementById("modalidade").innerHTML;
//        $("#plicod").val(pi);
//
//        var validado = true;
//        var l = $('#plicodsubacao').val();
//        var sub = l.length;
//        $('plititulo').value = $('prefixotitulo').textContent + $('plititulo').value;

        // Msgs personalizadas de validação.
        addMsgCustom = new Array();

        if($('input[name="ptrid"]').size() == 0){
            $('.legend_funcional').addClass('validateRedText');
            addMsgCustom.push('Funcional');
        } else {
            $('.legend_funcional').removeClass('validateRedText');
        }

        // Se não for adicionado sniic, acrescenta uma msg ao erro.
    //        if($('[name="lista_sniic[]"]').size() == 0){
    //            addMsgCustom.push('Números SNIIC');
    //            $('.legend_sniic').addClass('validateRedText');
    //        } else {
    //            $('.legend_sniic').removeClass('validateRedText');
    //        }
        // Se não for adicionado convenio, acrescenta uma msg ao erro.
    //        if($('[name="lista_convenio[]"]').size() == 0){
    //            addMsgCustom.push('Números De Convênio');
    //            $('.legend_convenio').addClass('validateRedText');
    //        } else {
    //            $('.legend_convenio').removeClass('validateRedText');
    //        }

        // Se não for selecionado nenhum tipo de localização, o sistema acrescenta uma mensagem de erro.
        if($('#esfid').val() == ""){
            $('.legend_localizacao').addClass('validateRedText');
            addMsgCustom.push('Localização do Projeto');
        } else {
            switch($('#esfid').val()) {
                // Verifica se a esfera é Estadual/DF.
                case intEsfidEstadualDF:
                    // Verifica se existe não foi inserido item na lista de localização municipal.
                    if($('input[name="listaLocalizacaoEstadual[]"]').size() == 0){
                        $('.legend_localizacao').addClass('validateRedText');
                        addMsgCustom.push('Inserir Localização do Projeto Estadual/Distrito Federal');
                    } else {
                        $('.legend_localizacao').removeClass('validateRedText');
                    }
                break;
                // Verifica se a esfera é Exterior.
                case intEsfidExterior:
                    // Verifica se existe não foi inserido item na lista de localização no Exterior.
                    if($('input[name="listaLocalizacaoExterior[]"]').size() == 0){
                        $('.legend_localizacao').addClass('validateRedText');
                        addMsgCustom.push('Inserir Localização do Projeto no Exterior');
                    } else {
                        $('.legend_localizacao').removeClass('validateRedText');
                    }
                break;
                // Verifica se a esfera é Municipal.
                case intEsfidMunicipal:
                    // Verifica se existe não foi inserido item na lista de localização municipal.
                    if($('input[name="listaLocalizacao[]"]').size() == 0){
                        $('.legend_localizacao').addClass('validateRedText');
                        addMsgCustom.push('Inserir Localização do Projeto Municipal');
                    } else {
                        $('.legend_localizacao').removeClass('validateRedText');
                    }
                break;
                default:
                    $('.legend_localizacao').removeClass('validateRedText');
                break;
            }
        }

        // Se não for inserido nenhum Responsável, o sistema acrescenta uma mensagem de erro.
        if($('#table_responsaveis input[name="listaResponsaveis[]"]').size() == 0){
            $('.legend_responsaveis').addClass('validateRedText');
            addMsgCustom.push('Responsáveis pelo Projeto');
        } else {
            $('.legend_responsaveis').removeClass('validateRedText');
        }

        // Valida se o usuário preencheu Valor do Projeto - Custeio.
        if($('#picvalorcusteio').val() == ""){
            $('#valor_projeto').addClass('validateRedText');
            addMsgCustom.push('Custeio');
        } else {
            $('#valor_projeto').removeClass('validateRedText');
        }

        // Valida se o usuário preencheu Valor do Projeto - Capital.
        if($('#picvalorcapital').val() == ""){
            $('#valor_projeto').addClass('validateRedText');
            addMsgCustom.push('Capital');
        } else {
            $('#picvalorcapital').removeClass('validateRedText');
        }

        /*
         *
         * @todo Refatorar esse código de validação da parte Custeio e Capital dividindo em funções.
         */
        var disponivelUnidade = textToFloat($('#td_disponivel_sub_unidade').text());
        var disponivelFuncional = buscarValorDisponivelFuncional();
        var valorDoProjeto = buscarValorDoProjeto();

        if(valorDoProjeto > disponivelUnidade || valorDoProjeto > disponivelFuncional){
            $('#picvalorcusteio').addClass('validateRedText');
            $('#picvalorcapital').addClass('validateRedText');
            $('#td_valor_projeto').addClass('validateRedText');
            if(valorDoProjeto > disponivelUnidade){
                addMsgCustom.push('Valor do projeto superior ao limite disponível da Sub-Unidade');
            }
            if(valorDoProjeto > disponivelFuncional){
                addMsgCustom.push('Valor do projeto superior ao limite disponível da Funcional');
            }
        } else {
            $('#picvalorcusteio').removeClass('validateRedText');
            $('#picvalorcapital').removeClass('validateRedText');
            $('#td_valor_projeto').removeClass('validateRedText');
        }

        // Verifica se o usuário escolheu um enquadramento que não tem item de manutenção para verificar a validação do cronograma físico.
        if($('#maiid option').size() == 0){

            // Verifica se o usuário escolheu um produto diferente de não se aplica para verificar a validação do cronograma físico.
            if($('#pprid').val() != intProdNaoAplica ){

                // Verifica se o cronograma físico foi preenchido.
                if(!validarCronogramaFisicoPreenchido()){
                    $('input.input_fisico').addClass('validateRedText');
                    $('#td_total_fisico').addClass('validateRedText');
                    addMsgCustom.push('Cronograma Fisíco');
                } else {
                    $('input.input_fisico').removeClass('validateRedText');
                    $('#td_total_fisico').removeClass('validateRedText');
                }

                // Verifica se o valor do cronograma Físico é igual ao informado no Produto do PI.
                if(!validarCronogramaFisicoIgualQuantidade()){
                    $('input.input_fisico').addClass('validateRedText');
                    $('#td_total_fisico').addClass('validateRedText');
                    addMsgCustom.push('Soma dos valores do Cronograma Fisíco está diferente da quantidade informada para o Produto do PI');
                } else {
                    $('input.input_fisico').removeClass('validateRedText');
                    $('#td_total_fisico').removeClass('validateRedText');
                }
            }

        }

        // Verifica se o cronograma orçamentário foi preenchido.
        if(!validarCronogramaOrcamentarioPreenchido()){
            $('input.input_orcamentario').addClass('validateRedText');
            $('#td_total_orcamentario_custeio').addClass('validateRedText');
            $('#td_total_orcamentario_capital').addClass('validateRedText');
            addMsgCustom.push('Cronograma Orçamentário');
        } else {
            $('input.input_orcamentario').removeClass('validateRedText');
            $('#td_total_orcamentario_custeio').removeClass('validateRedText');
            $('#td_total_orcamentario_capital').removeClass('validateRedText');
        }

        // Verifica se o cronograma financeiro foi preenchido.
        if(!validarCronogramaFinanceiroPreenchido()){
            $('input.input_financeiro').addClass('validateRedText');
            $('#td_total_financeiro').addClass('validateRedText');
            addMsgCustom.push('Cronograma Financeiro');
        } else {
            $('input.input_financeiro').removeClass('validateRedText');
            $('#td_total_financeiro').removeClass('validateRedText');
        }

        // Verifica se o valor do cronograma CUSTEIO é superior ao valor do Projeto.
        if(!validarCronogramaOrcamentarioCusteioIgualValorProjeto()){
            $('.input_orcamentario.custeio').addClass('validateRedText');
            addMsgCustom.push('Soma dos valores de CUSTEIO do Cronograma Orçamentário diferente do valor de CUSTEIO do Valor do Projeto');
        } else {
            $('.input_orcamentario.custeio').removeClass('validateRedText');
        }

        // Verifica se o valor do cronograma CAPITAL é superior ao valor do Projeto.
        if(!validarCronogramaOrcamentarioCapitalIgualValorProjeto()){
            $('.input_orcamentario.capital').addClass('validateRedText');
            addMsgCustom.push('Soma dos valores de CAPITAL do Cronograma Orçamentário diferente do valor de CAPITAL do Valor do Projeto');
        } else {
            $('.input_orcamentario.capital').removeClass('validateRedText');
        }

        // Verifica se o valor do cronograma CUSTEIO é superior ao valor do Projeto.
        if(!validarCronogramaFinanceiroCusteioIgualValorProjeto()){
            $('.input_financeiro.custeio').addClass('validateRedText');
            addMsgCustom.push('Soma dos valores de CUSTEIO do Cronograma Financeiro diferente do valor de CUSTEIO do Valor do Projeto');
        } else {
            $('.input_financeiro.custeio').removeClass('validateRedText');
        }

        // Verifica se o valor do cronograma CAPITAL é superior ao valor do Projeto.
        if(!validarCronogramaFinanceiroCapitalIgualValorProjeto()){
            $('.input_financeiro.custeio').addClass('validateRedText');
            addMsgCustom.push('Soma dos valores de CAPITAL do Cronograma Financeiro diferente do valor de CAPITAL do Valor do Projeto');
        } else {
            $('.input_financeiro.custeio').removeClass('validateRedText');
        }

        listaObrigatorios = definirCamposObrigatorios();
        validarFormulario(listaObrigatorios, 'formulario', 'validar('+ pliid +')', addMsgCustom);
    }
