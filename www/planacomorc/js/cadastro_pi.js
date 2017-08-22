
    /**
     * Ações efetuadas quando a tela de cadastro de PI é iniciada.
     * 
     * @returns VOID
     */
    function initCadastroPi(){
        
        var strComentarioEstadoAtual = $('#div_comentario_estado_atual').html();

        if(strComentarioEstadoAtual != ""){
            setTimeout(function() {
                toastr.options = {
                    closeButton: true,
                    progressBar: true,
                    showMethod: 'slideDown',
                    timeOut: false
                };
                toastr.success(strComentarioEstadoAtual, 'Enviado para correção');
            }, 1300);
        }

        var tipoTransacao = $('tipotransacao').value;
        if ('-' == tipoTransacao) {
            // -- Desabilita o restante do formulário
            $('plititulo').disable();
            $('plidsc').disable();
            $('btn_selecionar_acaptres').disable();
        }

        // Retira cor dos elementos marcados pela validação do formulário quando a validação for satisfeita(Quando o elemento for preenchido).
        initTirarCorValidacao();

        // Evento ao mudar opção de enquadramento
        $('#unicod').change(function(){
            carregarUG($(this).val());
        });

        $('div.div_ungcod').on('change', '#ungcod', function(){
            carregarLimitesUnidade($(this).val());
            carregarMetaPNC($(this).val());
        });
        
        $('#orcamento').on('click', '.btnVisualizarDetalhes', function(){
            visualizarRegistro($(this).attr('ptrid'));
        });

        $('#ungcod').change();
        carregarSaldoFuncional();

        // Evento ao mudar opção de enquadramento
        $('#eqdid').change(function(){
            mudarFormularioFinalistico($(this).val());
        });

        // Evento ao carregar a tela
        mudarFormularioFinalistico($('#eqdid').val());
        formatarTelaEnquadramentoComManutencaoItem();

        // Evento ao mudar opção de Objetivos PPA
        $('#oppid').change(function(){
            carregarMetasPPA($(this).val(), null, $('#ungcod').val());
            carregarIniciativaPPA($(this).val());
        });

        // Evento ao mudar opção de Metas PNC
        $('div.div_mpnid').on('change', '#mpnid', function(){
            carregarIndicadorPNC($(this).val());
        });

        // Evento ao mudar opção de Área Cultural
        $('#mdeid').change(function(){
            carregarSegmentoCultural($(this).val());
        });

        // Evento ao mudar opção de Manutenção Item
        $('#eqdid').change(function(){
            carregarManutencaoItem($(this).val());
        });

        // Evento ao mudar opção de Manutenção SubItem
        $('body').on('change', '#maiid', function(){
            carregarManutencaoSubItem($(this).val());
            atualizarTitulo();
        });
        
        // Evento ao mudar opção de Manutenção SubItem
        $('body').on('change', '#masid', function(){
            atualizarDescricao();
        });

        $('#btn_adicionar_sniic').click(function(){
             var trHtml =
                '<tr style="height: 30px;" id="tr_sniic_' + $('#input_sniic').val()+ '" >'
                        + '<td style="text-align: left;">' + $('#input_sniic').val() + '</td>'
                        + '<td style="text-align: center;">'
                            + '<input type="hidden" name="lista_sniic[]" value="' + $('#input_sniic').val() + '" />'
                            + '<span class="glyphicon glyphicon-trash link btnRemoveSniic" data-sniic="' + $('#input_sniic').val()+ '" ></span>'
                        + '</td>'
                + '</tr>';
            $('#table_sniic').append(trHtml);
            $('#input_sniic').val('');
        });

        $('#table_sniic').on('click', '.btnRemoveSniic', function(){
            var sniic = $(this).attr('data-sniic');
            $('#tr_sniic_'+ sniic).remove();
        });

        $('#input_sniic').keypress(function(e){
            if(e.which == 13) {
                $('#btn_adicionar_sniic').click();
            }
        });

        $('#btn_selecionar_convenio').click(function(){
            var trHtml =
                '<tr style="height: 30px;" id="tr_convenio_' + $('#input_convenio').val()+ '" >'
                + '<td style="text-align: left;">' + $('#input_convenio').val() + '</td>'
                + '<td style="text-align: center;">'
                + '<input type="hidden" name="lista_convenio[]" value="' + $('#input_convenio').val() + '" />'
                + '<span class="glyphicon glyphicon-trash link btnRemoveConvenio" data-convenio="' + $('#input_convenio').val()+ '" ></span>'
                + '</td>'
                + '</tr>';
            $('#table_convenio').append(trHtml);
            $('#input_convenio').val('');
        });

        $('#table_convenio').on('click', '.btnRemoveConvenio', function(){
            var convenio = $(this).attr('data-convenio');
            $('#tr_convenio_'+ convenio).remove();
        });

        $('#input_convenio').keypress(function(e){
            if(e.which == 13) {
                $('#btn_selecionar_convenio').click();
            }
        });

        $('#picedital').change(function() {
                controlarEdital($('#picedital').is(':checked'));
        });

        $('#table_localizacao').on('click', '.btnRemoverLocalizacao', function(){
            var id = $(this).attr('data-localizacao');
            $('.tr_localizacao_'+ id).remove();
        });

        $('#table_localizacao_estadual').on('click', '.btnRemoverLocalizacaoEstadual', function(){
            var id = $(this).attr('data-localizacao-estadual');
            $('.tr_localizacao_estadual_'+ id).remove();
        });

        $('#table_localizacao_exterior').on('click', '.btnRemoverLocalizacaoExterior', function(){
            var id = $(this).attr('data-localizacao-exterior');
            $('.tr_localizacao_exterior_'+ id).remove();
        });

        $('#table_responsaveis').on('click', '.btnRemoverResponsaveis', function(){
            var cpf = $(this).attr('data-responsaveis');
            $('.tr_responsaveis_'+ cpf).remove();
        });

        // Evento ao carregar a tela
        controlarEdital($('#picedital').is(':checked'));

        $('#btn_selecionar_functional').click(function(){
            abrir_lista();
        });

        $('#btn_selecionar_responsaveis').click(function(){
            abrirModalResponsaveis();
        });

        $('#btn_selecionar_localizacao').click(function(){
            abrirModalLocalizacao();
        });

        $('#btn_selecionar_localizacao_estadual').click(function(){
            abrirModalLocalizacaoEstadual();
        });

        $('#btn_selecionar_localizacao_exterior').click(function(){
            abrirModalLocalizacaoExterior();
        });

        $('#esfid').change(function(){
            controlarTipoLocalizacao($(this).val());
        });

        $('#picvalorcusteio').keyup(function(){
            this.value = mascaraglobal('###.###.###.###,##', this.value);
            atualizarValorDoProjeto();
            atualizarValorDetalhado();
            atualizarValorNaoDetalhado();
            atualizarValorLimiteDisponivelUnidade();
            atualizarValorLimiteDisponivelFuncionalCusteio();
            mudarCorValorProjeto();
        });

        $('#picvalorcapital').keyup(function(){
            this.value = mascaraglobal('###.###.###.###,##', this.value);
            atualizarValorDoProjeto();
            atualizarValorDetalhado();
            atualizarValorNaoDetalhado();
            atualizarValorLimiteDisponivelUnidade();
            atualizarValorLimiteDisponivelFuncionalCapital();
            mudarCorValorProjeto();
        });

        $('#picquantidade').keyup(function(){
            mudarCorCronogramaFisico();
            avisarCronogramaFisico();
        });

        $('.input_fisico').keyup(function(){
            mudarCorCronogramaFisico();
            avisarCronogramaFisico();
            atualizarTotalFisico();
        });

        $('.input_orcamentario').keyup(function(){
            this.value = mascaraglobal('###.###.###.###,##', this.value);
            atualizarTotalOrcamentario();
        });
        
        $('.input_orcamentario.custeio').keyup(function(){
            mudarCorCronogramaOrcamentarioCusteio();
            avisarCronogramaOrcamentarioSuperiorCusteio();
        });
        
        $('.input_orcamentario.capital').keyup(function(){
            mudarCorCronogramaOrcamentarioCapital();
            avisarCronogramaOrcamentarioSuperiorCapital();
        });
        
        $('.input_financeiro.custeio').keyup(function(){
            mudarCorCronogramaFinanceiroCusteio();
            avisarCronogramaFinanceiroSuperiorCusteio();
        });
        
        $('.input_financeiro.capital').keyup(function(){
            mudarCorCronogramaFinanceiroCapital();
            avisarCronogramaFinanceiroSuperiorCapital();
        });

        $('.input_financeiro').keyup(function(){
            this.value = mascaraglobal('###.###.###.###,##', this.value);
            atualizarTotalFinanceiro();
        });

        // Evento ao mudar clicar no código do PI
        $('#span-plicod').click(function(){
            var codPi = $('#span-plicod').html();
            $('#span-plicod').hide();
            $('#plicod').show().focus();
        });

        // Evento ao alterar o valor do código do PI
        $('#plicod').change(function(){
            $('#span-plicod').load('?modulo=principal/unidade/cadastro_pi&acao=A&alterarCodigoPi=ok&pliid='+$('#pliid').val() + '&plicod=' + $('#plicod').val());
        });

        // Evento ao mudar sair do campo de código do PI
        $('#plicod').blur(function(){
            $('#plicod').hide();
            $('#span-plicod').show();
        });

        $('#capid').change(function(){

            $.ajax({
                url: '?modulo=principal/unidade/cadastro_pi&acao=A&verificarPactuacaoConvenio=ok&capid='+$('#capid').val(),
                success: function($retorno){
                    if($retorno){
                        $('#div_siconv').show('slow');
                    } else {
                        $('#div_siconv').hide('slow');
                    }
                }
            });

        }).change();

        controlarTipoLocalizacao($('#esfid').val());

        if(strEsdPiCadastramento){
            $('#formulario input, #formulario textarea, #formulario select').prop('disabled', true);
            setTimeout($('#formulario select').prop('disabled', true).trigger("chosen:updated"), 3000);
        }

        atualizarTotalFisico();
        atualizarTotalOrcamentario();
        atualizarTotalFinanceiro();
        
        mudarCorValoresProjetosFisicoOrcamentarioFinanceiro();
    }

    /**
     * Atualiza o Titulo do PI conforme a opção selecionada em Manutenção Item.
     * 
     * @returns VOID
     */
    function atualizarTitulo(){
        var titulo = $('#maiid :checked').text();
        if(titulo != ''){
            $('#plititulo').val(titulo);
        }
    }
    
    /**
     * Atualiza a descrição do PI conforme a opção selecionada em Manutenção Subitem.
     * 
     * @returns VOID
     */
    function atualizarDescricao(){
        var descricao = $('#masid :checked').text();
        if(descricao != ''){
            $('#plidsc').val(descricao);
        }
    }

   /**
    * Valida informações de valores do Projeto(Capital e Custeio), Fisicos,
    * Orçamentários e Financeiros e caso estejam com inconformidades mudam a cor pra vermelho.
    *
    * @returns VOID
    */
    function mudarCorValoresProjetosFisicoOrcamentarioFinanceiro(){
        mudarCorCronogramaFisico();
        mudarCorCronogramaOrcamentarioCusteio();
        mudarCorCronogramaOrcamentarioCapital();
        mudarCorCronogramaFinanceiroCusteio();
        mudarCorCronogramaFinanceiroCapital();
    }

    /**
     * Retira cor dos elementos marcados pela validação do formulário quando a validação for satisfeita(Quando o elemento for preenchido).
     *
     * @returns VOID
     */
    function initTirarCorValidacao(){
        var listaiten = definirCamposObrigatorios();
        listaiten.push('mpnid', 'ipnid');

        $.each(listaiten , function(i , id){
            $('#formulario').on('change', '#' + id, function(){
                tirarCorVermelha(id);
            });
        });
    }

    /**
     * Tira a Cor Vermelha da validação do label do elemento e da tag pai do elemento.
     *
     * @param string id
     * @returns VOID
     */
    function tirarCorVermelha(id){
        $item = $('#' + id);
        $($item).parent().parent().removeClass('has-error');
        $('label[for="'+ $item.attr('id') +'"]').removeClass('has-error');
    }

    /**
     * Exibe orientações ao usuário para preencher corretamente o Cronograma.
     *
     * @returns VOID
     */
    function avisarCronogramaFisico(){
        if(!validarCronogramaFisicoInferiorQuantidade()){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível salvar o formulário com a soma de todos os valores preenchidos nos meses do cronograma físico superior ao valor informado na Quantidade do Produto do PI.<br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, diminua os valores informados na coluna do cronograma físico ou aumente a Quantidade no Produto do PI.</span></p>');
        }
    }

    /**
     * Exibe orientações ao usuário para preencher corretamente o Cronograma.
     *
     * @returns VOID
     */
    function avisarCronogramaOrcamentarioSuperiorCusteio(){
        if(!validarCronogramaOrcamentarioSuperiorValorProjetoCusteio()){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível salvar o formulário com a soma de todos os valores de CUSTEIO preenchidos nos meses do cronograma orçamentário superior ao valor informado no campo de CUSTEIO do Valor do Projeto.<br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, diminua os valores informados nos campos de CUSTEIO do cronograma orçamentário ou aumente o valor de CUSTEIO do Valor do Projeto.</span></p>');
        }
    }
    
   /**
    * Valida se o valor preenchido no cronograma orçamentário é inferior ou igual ao valor declarado em custeio para o valor do projeto.
    * 
    * @return boolean
    */
    function validarCronogramaOrcamentarioSuperiorValorProjetoCusteio(){
        var resultado = false;
        var valorCusteio = textToFloat($('#picvalorcusteio').val());
        var valorTotalCusteioCronograma = buscarTotalCusteioCronogramaOrcamentario();
        
        if(valorTotalCusteioCronograma <= valorCusteio){
            resultado = true;
        }
        
        return resultado;
    }
    
    /**
     * Exibe orientações ao usuário para preencher corretamente o Cronograma.
     *
     * @returns VOID
     */
    function avisarCronogramaOrcamentarioSuperiorCapital(){
        if(!validarCronogramaOrcamentarioSuperiorValorProjetoCapital()){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível salvar o formulário com a soma de todos os valores de CAPITAL preenchidos nos meses do cronograma orçamentário superior ao valor informado no campo de CAPITAL do Valor do Projeto.<br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, diminua os valores informados nos campos de CAPITAL do cronograma orçamentário ou aumente o valor de CAPITAL do Valor do Projeto.</span></p>');
        }
    }
    
   /**
    * Valida se o valor preenchido no cronograma orçamentário é inferior ou igual ao valor declarado em custeio para o valor do projeto.
    * 
    * @return boolean
    */
    function validarCronogramaOrcamentarioSuperiorValorProjetoCapital(){
        var resultado = false;
        var valorCapital = textToFloat($('#picvalorcapital').val());
        var valorTotalCapitalCronograma = buscarTotalCapitalCronogramaOrcamentario();
        
        if(valorTotalCapitalCronograma <= valorCapital){
            resultado = true;
        }
        
        return resultado;
    }
    
    /**
     * Exibe orientações ao usuário para preencher corretamente o Cronograma.
     *
     * @returns VOID
     */
    function avisarCronogramaFinanceiroSuperiorCapital(){
        if(!validarCronogramaFinanceiroSuperiorValorProjetoCapital()){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível salvar o formulário com a soma de todos os valores de CAPITAL preenchidos nos meses do cronograma financeiro superior ao valor informado no campo de CAPITAL do Valor do Projeto.<br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, diminua os valores informados nos campos de CAPITAL do cronograma financeiro ou aumente o valor de CAPITAL do Valor do Projeto.</span></p>');
        }
    }
    
   /**
    * Valida se o valor preenchido no cronograma orçamentário é inferior ou igual ao valor declarado em custeio para o valor do projeto.
    * 
    * @return boolean
    */
    function validarCronogramaFinanceiroSuperiorValorProjetoCapital(){
        var resultado = false;
        var valorCapital = textToFloat($('#picvalorcapital').val());
        var valorTotalCapitalCronograma = buscarTotalCapitalCronogramaFinanceiro();
        
        if(valorTotalCapitalCronograma <= valorCapital){
            resultado = true;
        }
        
        return resultado;
    }
    
    /**
     * Exibe orientações ao usuário para preencher corretamente o Cronograma.
     *
     * @returns VOID
     */
    function avisarCronogramaFinanceiroSuperiorCusteio(){
        if(!validarCronogramaFinanceiroSuperiorValorProjetoCusteio()){
            alert('<p style="text-align: justify;">&nbsp; &nbsp; Não será possível salvar o formulário com a soma de todos os valores de CUSTEIO preenchidos nos meses do cronograma financeiro superior ao valor informado no campo de CUSTEIO do Valor do Projeto.<br />&nbsp; &nbsp; <span style="color: #ff0000;">Por favor, diminua os valores informados nos campos de CUSTEIO do cronograma financeiro ou aumente o valor de CUSTEIO do Valor do Projeto.</span></p>');
        }
    }
    
   /**
    * Valida se o valor preenchido no cronograma orçamentário é inferior ou igual ao valor declarado em custeio para o valor do projeto.
    * 
    * @return boolean
    */
    function validarCronogramaFinanceiroSuperiorValorProjetoCusteio(){
        var resultado = false;
        var valorCusteio = textToFloat($('#picvalorcusteio').val());
        var valorTotalCusteioCronograma = buscarTotalCusteioCronogramaFinanceiro();
        
        if(valorTotalCusteioCronograma <= valorCusteio){
            resultado = true;
        }
        
        return resultado;
    }

    /**
     * Muda a cor dos valores do cronograma Físico se o valor exceder o valor do projeto.
     *
     * @returns VOID
     */
    function mudarCorCronogramaFisico(){
        if(validarCronogramaFisicoInferiorQuantidade()){
            $('input.input_fisico').removeClass('validateRedText');
            $('#td_total_fisico').removeClass('validateRedText');
        } else {
            $('input.input_fisico').addClass('validateRedText');
            $('#td_total_fisico').addClass('validateRedText');
        }
    }

    /**
     * Muda a cor dos valores das colunas de Custeio do cronograma orçamentário se o valor exceder o valor do projeto.
     *
     * @returns VOID
     */
    function mudarCorCronogramaOrcamentarioCusteio(){
        if(validarCronogramaOrcamentarioInferiorValorProjetoCusteio()){
            $('.input_orcamentario.custeio').removeClass('validateRedText');
            $('#td_total_orcamentario_custeio').removeClass('validateRedText');
        } else {
            $('.input_orcamentario.custeio').addClass('validateRedText');
            $('#td_total_orcamentario_custeio').addClass('validateRedText');
        }
    }
    
    /**
     * Muda a cor dos valores das colunas de Capital do cronograma orçamentário se o valor exceder o valor do projeto.
     *
     * @returns VOID
     */
    function mudarCorCronogramaOrcamentarioCapital(){
        if(validarCronogramaOrcamentarioInferiorValorProjetoCapital()){
            $('.input_orcamentario.capital').removeClass('validateRedText');
            $('#td_total_orcamentario_capital').removeClass('validateRedText');
        } else {
            $('.input_orcamentario.capital').addClass('validateRedText');
            $('#td_total_orcamentario_capital').addClass('validateRedText');
        }
    }

    /**
     * Muda a cor dos valores do cronograma financeiro se o valor exceder o valor do projeto.
     *
     * @returns VOID
     */
    function mudarCorCronogramaFinanceiroCusteio(){
        if(validarCronogramaFinanceiroInferiorValorProjetoCusteio()){
            $('.input_financeiro.custeio').removeClass('validateRedText');
            $('#td_total_financeiro_custeio').removeClass('validateRedText');
        } else {
            $('.input_financeiro.custeio').addClass('validateRedText');
            $('#td_total_financeiro_custeio').addClass('validateRedText');
        }
    }
    
    /**
     * Muda a cor dos valores do cronograma financeiro se o valor exceder o valor do projeto.
     *
     * @returns VOID
     */
    function mudarCorCronogramaFinanceiroCapital(){
        if(validarCronogramaFinanceiroInferiorValorProjetoCapital()){
            $('.input_financeiro.capital').removeClass('validateRedText');
            $('#td_total_financeiro_capital').removeClass('validateRedText');
        } else {
            $('.input_financeiro.capital').addClass('validateRedText');
            $('#td_total_financeiro_capital').addClass('validateRedText');
        }
    }

    /**
     * Verifica se o cronograma está preenchido.
     *
     * @returns Boolean
     */
    function validarCronogramaFisicoPreenchido(){
        var valido = false;
        var totalFisico = buscarTotalCronogramaFisico();

        if(totalFisico > 0){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se o cronograma está preenchido com valores superiores a Quantidade do Produto do PI.
     *
     * @returns Boolean
     */
    function validarCronogramaFisicoInferiorQuantidade(){
        var valido = false;
        var quantidade = textToFloat($('#picquantidade').val());
        var totalFisico = buscarTotalCronogramaFisico();
        if(totalFisico <= quantidade){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se a coluna de custeio do cronograma está preenchido com valores inferiores ao valor do projeto.
     *
     * @returns Boolean
     */
    function validarCronogramaOrcamentarioInferiorValorProjetoCusteio(){
        var valido = false;
        var valorDoProjeto = textToFloat($('#picvalorcusteio').val());
        var valorTotalOrcamentario = buscarTotalCusteioCronogramaOrcamentario();

        if(valorTotalOrcamentario <= valorDoProjeto){
            valido = true;
        }

        return valido;
    }
    
    /**
     * Verifica se a coluna de capital do cronograma está preenchido com valores inferiores ao valor do projeto.
     *
     * @returns Boolean
     */
    function validarCronogramaOrcamentarioInferiorValorProjetoCapital(){
        var valido = false;
        var valorDoProjeto = textToFloat($('#picvalorcapital').val());
        var valorTotalOrcamentario = buscarTotalCapitalCronogramaOrcamentario();

        if(valorTotalOrcamentario <= valorDoProjeto){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se a coluna de custeio do cronograma está preenchido com valores inferiores ao valor do projeto.
     *
     * @returns Boolean
     */
    function validarCronogramaFinanceiroInferiorValorProjetoCusteio(){
        var valido = false;
        var valorDoProjeto = textToFloat($('#picvalorcusteio').val());
        var valorTotalFinanceiro = buscarTotalCusteioCronogramaFinanceiro();

        if(valorTotalFinanceiro <= valorDoProjeto){
            valido = true;
        }

        return valido;
    }
    
    /**
     * Verifica se a coluna de capital do cronograma está preenchido com valores inferiores ao valor do projeto.
     *
     * @returns Boolean
     */
    function validarCronogramaFinanceiroInferiorValorProjetoCapital(){
        var valido = false;
        var valorDoProjeto = textToFloat($('#picvalorcapital').val());
        var valorTotalFinanceiro = buscarTotalCapitalCronogramaFinanceiro();

        if(valorTotalFinanceiro <= valorDoProjeto){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se o cronograma está preenchido.
     *
     * @returns Boolean
     */
    function validarCronogramaOrcamentarioPreenchido(){
        var valido = false;
        var custeio = buscarTotalCusteioCronogramaOrcamentario();
        var capital = buscarTotalCapitalCronogramaOrcamentario();
        
        valorTotalOrcamentario = custeio + capital;

        if(valorTotalOrcamentario > 0){
            valido = true;
        }

        return valido;
    }

    /**
     * Verifica se o cronograma está preenchido.
     *
     * @returns Boolean
     */
    function validarCronogramaFinanceiroPreenchido(){
        var valido = false;
        var custeio = buscarTotalCusteioCronogramaFinanceiro();
        var capital = buscarTotalCapitalCronogramaFinanceiro();
        
        valorTotalFinanceiro = custeio + capital;

        if(valorTotalFinanceiro > 0){
            valido = true;
        }

        return valido;
    }

    /**
     * Busca o total informado de todos os meses no cronograma físico.
     *
     * @returns float
     */
    function buscarTotalCronogramaFisico(){
        var totalFisico = 0;
        $('input.input_fisico').each(function(data, key){
            var valorMesFisico = textToFloat($(this).val());
            totalFisico += valorMesFisico;
        });

        return totalFisico;
    }

    /**
     * Busca o total de custeio informado de todos os meses no cronograma orcamentario.
     *
     * @returns float
     */
    function buscarTotalCusteioCronogramaOrcamentario(){
        var totalOrcamentario = 0;
        $('input.input_orcamentario.custeio').each(function(data, key){
            var valorMesOrcamento = textToFloat($(this).val());
            totalOrcamentario += valorMesOrcamento;
        });

        return totalOrcamentario;
    }
    
    /**
     * Busca o total de capital informado de todos os meses no cronograma orcamentario.
     *
     * @returns float
     */
    function buscarTotalCapitalCronogramaOrcamentario(){
        var totalOrcamentario = 0;
        $('input.input_orcamentario.capital').each(function(data, key){
            var valorMesOrcamento = textToFloat($(this).val());
            totalOrcamentario += valorMesOrcamento;
        });

        return totalOrcamentario;
    }

    /**
     * Busca o total de custeio informado de todos os meses no cronograma financeiro.
     *
     * @returns float
     */
    function buscarTotalCusteioCronogramaFinanceiro(){
        var totalFinanceiro = 0;
        $('input.input_financeiro.custeio').each(function(data, key){
            var valorMesFinanceiro = textToFloat($(this).val());
            totalFinanceiro += valorMesFinanceiro;
        });

        return totalFinanceiro;
    }
    
    /**
     * Busca o total de capital informado de todos os meses no cronograma financeiro.
     *
     * @returns float
     */
    function buscarTotalCapitalCronogramaFinanceiro(){
        var totalFinanceiro = 0;
        $('input.input_financeiro.capital').each(function(data, key){
            var valorMesFinanceiro = textToFloat($(this).val());
            totalFinanceiro += valorMesFinanceiro;
        });

        return totalFinanceiro;
    }

    /**
     * Calcula e exibi na tela o valor disponivel na Sub-Unidade.
     *
     * @returns VOID
     */
    function atualizarValorLimiteDisponivelUnidade() {
        var valorBaseLimiteDisponivelUnidade = textToFloat($('#VlrSUDisponivel').val());
        var valorBaseProjeto = buscarValorBaseDoProjeto();
        var valorProjeto = buscarValorDoProjeto();

        if(valorProjeto < valorBaseProjeto){
            valorDiferenca = (valorBaseProjeto - valorProjeto);
        } else {
            valorDiferenca = (valorProjeto - valorBaseProjeto);
        }

        var fltValorLimiteDisponivelUnidade = (valorBaseLimiteDisponivelUnidade - valorDiferenca);
        $('#td_disponivel_sub_unidade').html(mascaraglobal('###.###.###.###,##', fltValorLimiteDisponivelUnidade.toFixed(2)));
    }

    /**
     * Calcula e exibi na tela o valor disponivel na funcional de custeio.
     *
     * @returns VOID
     */
    function atualizarValorLimiteDisponivelFuncionalCusteio() {
        var valorBaseLimiteDisponivel = textToFloat($('#VlrFuncionalDisponivelCusteio').val());
        var valorBaseLimiteProjetoCusteio = textToFloat($('#vlrPiCusteio').val());
        var valorProjetoCusteio = textToFloat($('#picvalorcusteio').val());

        if(valorProjetoCusteio < valorBaseLimiteProjetoCusteio){
            valorCusteio = (valorBaseLimiteProjetoCusteio - valorProjetoCusteio);
        } else {
            valorCusteio = (valorProjetoCusteio - valorBaseLimiteProjetoCusteio);
        }
        var fltValorLimiteDisponivelFuncional = (valorBaseLimiteDisponivel - valorCusteio);
        $('#td_disponivel_funcional_custeio').html(mascaraglobal('###.###.###.###,##', fltValorLimiteDisponivelFuncional.toFixed(2)));
    }

    /**
     * Calcula e exibi na tela o valor disponivel na funcional de capital.
     *
     * @returns VOID
     */
    function atualizarValorLimiteDisponivelFuncionalCapital() {
        var valorBaseLimiteDisponivel = textToFloat($('#VlrFuncionalDisponivelCapital').val());
        var valorBaseLimiteProjetoCapital = textToFloat($('#vlrPiCapital').val());
        var valorProjetoCapital = textToFloat($('#picvalorcapital').val());

        if(valorProjetoCapital < valorBaseLimiteProjetoCapital){
            valorCapital = (valorBaseLimiteProjetoCapital - valorProjetoCapital);
        } else {
            valorCapital = (valorProjetoCapital - valorBaseLimiteProjetoCapital);
        }
        var fltValorLimiteDisponivelFuncional = (valorBaseLimiteDisponivel - valorCapital);
        $('#td_disponivel_funcional_capital').html(mascaraglobal('###.###.###.###,##', fltValorLimiteDisponivelFuncional.toFixed(2)));
    }

    /**
     * Calcula e exibi na tela o valor detalhado da funcional.
     *
     * @returns VOID
     */
    function atualizarValorDetalhado() {
        var fltValorBaseDoProjeto = buscarValorBaseDoProjeto();
        var fltValorDoProjeto = buscarValorDoProjeto();
        var fltValorDetalhado = textToFloat($('#piDetalhado').val());
        
        var fltValorDetalhadoAtual = (fltValorDetalhado - fltValorBaseDoProjeto) + fltValorDoProjeto;
        $('#td_pi_detalhado').html(mascaraglobal('###.###.###.###,##', fltValorDetalhadoAtual.toFixed(2)));
    }

    /**
     * Calcula e exibi na tela o valor não detalhado da funcional.
     *
     * @returns VOID
     */
    function atualizarValorNaoDetalhado() {
        var fltValorBaseDoProjeto = buscarValorBaseDoProjeto();
        var fltValorDoProjeto = buscarValorDoProjeto();
        var fltValorBaseNaoDetalhado = textToFloat($('#piNaoDetalhado').val());
        
        var fltValorNaoDetalhadoAtual = (fltValorBaseNaoDetalhado - (fltValorDoProjeto - fltValorBaseDoProjeto));
        $('#td_pi_nao_detalhado').html(mascaraglobal('###.###.###.###,##', fltValorNaoDetalhadoAtual.toFixed(2)));
    }

    /**
     * Calcula o valor da funcional.
     *
     * @returns VOID
     */
    function atualizarValorDoProjeto() {
        var valorCusteio = textToFloat($('#picvalorcusteio').val());
        var valorCapital = textToFloat($('#picvalorcapital').val());
        var total = valorCusteio + valorCapital;
        $('#td_valor_projeto').html(mascaraglobal('###.###.###.###,##', total.toFixed(2) ));
    }

    /**
     * Exibe o valor total do cronograma Físico na tela.
     *
     * @returns VOID
     */
    function atualizarTotalFisico() {
        var total = buscarTotalCronogramaFisico();
        $('#td_total_fisico').html( total.toFixed(0) );
    }

    /**
     * Exibe o valor total do cronograma Orçamentário na tela.
     *
     * @returns VOID
     */
    function atualizarTotalOrcamentario() {
        var custeio = buscarTotalCusteioCronogramaOrcamentario();
        var capital = buscarTotalCapitalCronogramaOrcamentario();
        $('#td_total_orcamentario_custeio').html('R$ '+ mascaraglobal('###.###.###.###,##', custeio.toFixed(2)));
        $('#td_total_orcamentario_capital').html('R$ '+ mascaraglobal('###.###.###.###,##', capital.toFixed(2)));
    }

    /**
     * Exibe o valor total do cronograma Financeiro na tela.
     *
     * @returns VOID
     */
    function atualizarTotalFinanceiro() {
        var custeio = buscarTotalCusteioCronogramaFinanceiro();
        var capital = buscarTotalCapitalCronogramaFinanceiro();
        $('#td_total_financeiro_custeio').html('R$ '+ mascaraglobal('###.###.###.###,##', custeio.toFixed(2)));
        $('#td_total_financeiro_capital').html('R$ '+ mascaraglobal('###.###.###.###,##', capital.toFixed(2)));
    }

    /**
     * Soma o valor de capital e custeio e retorna o valor total do projeto.
     *
     * @returns float Valor do projeto
     */
    function buscarValorDoProjeto(){
        var valorCusteio = textToFloat($('#picvalorcusteio').val());
        var valorCapital = textToFloat($('#picvalorcapital').val());
        var valorDoProjeto = valorCusteio + valorCapital;

        return valorDoProjeto;
    }

    /**
     * Soma o valor de capital e custeio e retorna o valor base total do projeto.
     *
     * @returns float Valor do projeto
     */
    function buscarValorBaseDoProjeto(){
        var valorCusteio = textToFloat($('#vlrPiCusteio').val());
        var valorCapital = textToFloat($('#vlrPiCapital').val());
        var valorBaseDoProjeto = valorCusteio + valorCapital;

        return valorBaseDoProjeto;
    }

    /**
     * Soma o valor de capital e custeio e retorna o valor total disponivel.
     * @todo considerar valor inicial base pra dimininuir antes de somar ao valor disponivel custeio e capita.
     * @returns float Valor
     */
    function buscarValorDisponivelFuncional(){
        var valorCusteio = textToFloat($('#td_disponivel_funcional_custeio').text());
        var valorCapital = textToFloat($('#td_disponivel_funcional_capital').text());
        var disponivelFuncional = valorCusteio + valorCapital;

        return disponivelFuncional;
    }

    /**
     * Soma o valor de capital e custeio e retorna o valor total disponivel.
     *
     * @returns float Valor
     */
    function buscarValorAutorizadoFuncional(){
        var valorCusteio = textToFloat($('#td_autorizado_funcional_custeio').text());
        var valorCapital = textToFloat($('#td_autorizado_funcional_capital').text());
        var autorizadoFuncional = valorCusteio + valorCapital;

        return autorizadoFuncional;
    }

    function atualizarSaldoFuncional(){
        // Saldo Autorizado
        dotacaoAtualPO = $('tr[id^=ptres_] td:nth-child(3)').eq(0).text();
        var vlrAutorizadoCusteioCapitalProvisorio = textToFloat(dotacaoAtualPO) / 2;

        $('#td_autorizado_funcional_custeio').text(mascaraglobal('###.###.###.###,##', vlrAutorizadoCusteioCapitalProvisorio.toFixed(2) ));
        $('#td_autorizado_funcional_capital').text(mascaraglobal('###.###.###.###,##', vlrAutorizadoCusteioCapitalProvisorio.toFixed(2) ));

        // Saldo Disponível
        var textNaoDetalhado = $('tr[id^=ptres_] td:nth-child(5)').eq(0).text();
        var vlrNaoDetalhadoCusteioCapitalProvisorio = textToFloat(textNaoDetalhado) / 2;

        var valorDisponivelCusteio = vlrNaoDetalhadoCusteioCapitalProvisorio;
        var valorDisponivelCapital = vlrNaoDetalhadoCusteioCapitalProvisorio;

        $('#td_disponivel_funcional_custeio').text(mascaraglobal('###.###.###.###,##', valorDisponivelCusteio.toFixed(2) ));
        $('#td_disponivel_funcional_capital').text(mascaraglobal('###.###.###.###,##', valorDisponivelCapital.toFixed(2) ));
    }
    
    function carregarSaldoFuncional(){
        // Saldo Autorizado
        dotacaoAtualPO = $('tr[id^=ptres_] td:nth-child(3)').eq(0).text();
        var vlrAutorizadoCusteioCapitalProvisorio = textToFloat(dotacaoAtualPO) / 2;

        $('#td_autorizado_funcional_custeio').text(mascaraglobal('###.###.###.###,##', vlrAutorizadoCusteioCapitalProvisorio.toFixed(2) ));
        $('#td_autorizado_funcional_capital').text(mascaraglobal('###.###.###.###,##', vlrAutorizadoCusteioCapitalProvisorio.toFixed(2) ));

        // Saldo Disponível
        var textNaoDetalhado = $('tr[id^=ptres_] td:nth-child(5)').eq(0).text();
        var vlrNaoDetalhadoCusteioCapitalProvisorio = textToFloat(textNaoDetalhado) / 2;
        
        var valorDisponivelCusteio = vlrNaoDetalhadoCusteioCapitalProvisorio;
        var valorDisponivelCapital = vlrNaoDetalhadoCusteioCapitalProvisorio;

        $('#td_disponivel_funcional_custeio').text(mascaraglobal('###.###.###.###,##', valorDisponivelCusteio.toFixed(2) ));
        $('#td_disponivel_funcional_capital').text(mascaraglobal('###.###.###.###,##', valorDisponivelCapital.toFixed(2) ));
        $('#VlrFuncionalDisponivelCapital').val(mascaraglobal('###.###.###.###,##', valorDisponivelCusteio.toFixed(2)));
        $('#VlrFuncionalDisponivelCusteio').val(mascaraglobal('###.###.###.###,##',valorDisponivelCapital.toFixed(2)));
    }

    /**
     * Muda a cor do valor de projeto capital e custeio quando o valor disponvel
     * da sub-unidade ou funcional for ultrapassado.
     *
     * @returns VOID
     */
    function mudarCorValorProjeto(){
        var disponivelUnidade = textToFloat($('#td_disponivel_sub_unidade').text());
        var disponivelFuncional = buscarValorDisponivelFuncional();
        var valorDoProjeto = buscarValorDoProjeto();

        if(valorDoProjeto > disponivelUnidade || valorDoProjeto > disponivelFuncional){
            $('#picvalorcusteio').addClass('validateRedText');
            $('#picvalorcapital').addClass('validateRedText');
            $('#td_valor_projeto').addClass('validateRedText');
        } else {
            $('#picvalorcusteio').removeClass('validateRedText');
            $('#picvalorcapital').removeClass('validateRedText');
            $('#td_valor_projeto').removeClass('validateRedText');
        }
    }

   /**
    * Transforma o valor de texto pra flutuante pra efetuar operações matematicas.
    *
    * @param string text
    * @return float numero
    */
    function textToFloat(text){
        var numero = 0;
        text = replaceAll(text, '.', '');
        text = replaceAll(text, ',', '.');
        if(parseFloat(text) > 0){
            numero = parseFloat(text);
        }

        return numero;
    }

    /**
     * Controla as opções do formulario conforme a opção de tipo de localização selecionada.
     *
     * @param int esfid Código da esfera preenchido pelo usuário
     * @return VOID
     */
    function controlarTipoLocalizacao(esfid){
        $('#btn_selecionar_localizacao').hide();
        $('#btn_selecionar_localizacao_estadual').hide();
        $('#btn_selecionar_localizacao_exterior').hide();
        $('#table_localizacao tr').hide();
        $('#table_localizacao_estadual tr').hide();
        $('#table_localizacao_exterior tr').hide();

        switch (esfid) {
            // Verifica se a esfera é Estadual/DF.
            case intEsfidEstadualDF:
                $('#table_localizacao_estadual tr').show('slow');
                $('#btn_selecionar_localizacao_estadual').show('slow');

                $('#table_localizacao tr').not('tr.tr_head').remove();
                $('#table_localizacao_exterior tr').not('tr.tr_head').remove();
            break;
            // Verifica se a esfera é Exterior.
            case intEsfidExterior:
                $('#table_localizacao_exterior tr').show('slow');
                $('#btn_selecionar_localizacao_exterior').show('slow');
                $('#table_localizacao tr').not('tr.tr_head').remove();
                $('#table_localizacao_estadual tr').not('tr.tr_head').remove();
            break;
            // Verifica se a esfera é Federal.
            case intEsfidFederalBrasil:
                $('#btn_selecionar_localizacao').hide('slow');
                $('#table_localizacao tr').hide('slow');
                $('#table_localizacao tr').not('tr.tr_head').remove();
                $('#table_localizacao_estadual tr').not('tr.tr_head').remove();
                $('#table_localizacao_exterior tr').not('tr.tr_head').remove();
            break;
            // Verifica se a esfera é Municipal.
            case intEsfidMunicipal:
                $('#table_localizacao tr').show('slow');
                $('#btn_selecionar_localizacao').show('slow');
                $('#table_localizacao_estadual tr').not('tr.tr_head').remove();
                $('#table_localizacao_exterior tr').not('tr.tr_head').remove();
            break;
        }
    }

    function abrirModalResponsaveis() {
        // Verifica se o modal terá que carregar a tela.
        if($('#modal_responsaveis .modal-body p').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/pi-responsaveis&acao=A&ungcod='+ $("#ungcod").val(),
                function(response) {
                    $('#modal_responsaveis .modal-body p').html(response);
                    $('.modal_dialog_responsaveis').show();
                    $('#modal_responsaveis').modal();
                    $('#modal_responsaveis .chosen-select').chosen();
                    $('#modal_responsaveis .chosen-container').css('width', '100%');
            });
        } else {
            $('#formulario_responsaveis input[name=ungcod]').val($("#ungcod").val());
            $('#modal_responsaveis').modal();
            $('#btnPopupResponsaveisPesquisar').click();
        }
    }

    function abrirModalLocalizacao() {
        // Verifica se o modal terá que carregar a tela.
        if($('#modal_localizacao .modal-body p').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/pi-localizacao&acao=A', function(response) {
                    $('#modal_localizacao .modal-body p').html(response);
                    $('.modal_dialog_localizacao').show();
                    $('#modal_localizacao').modal();
                    $('#modal_localizacao .chosen-select').chosen();
                    $('#modal_localizacao .chosen-container').css('width', '100%');
            });
        } else {
            $('#modal_localizacao').modal();
            $('#btnPopupLocalizacaoPesquisar').click();
        }
    }

    function abrirModalLocalizacaoEstadual() {
        // Verifica se o modal terá que carregar a tela.
        if($('#modal_localizacao_estadual .modal-body p').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/pi-localizacao-estadual&acao=A', function(response) {
                    $('#modal_localizacao_estadual .modal-body p').html(response);
                    $('.modal_dialog_localizacao_estadual').show();
                    $('#modal_localizacao_estadual').modal();
                    $('#modal_localizacao_estadual .chosen-select').chosen();
                    $('#modal_localizacao_estadual .chosen-container').css('width', '100%');
            });
        } else {
            $('#modal_localizacao_estadual').modal();
            $('#btnPopupLocalizacaoEstadualPesquisar').click();
        }
    }

    function abrirModalLocalizacaoExterior() {
        // Verifica se o modal terá que carregar a tela.
        if($('#modal_localizacao_exterior .modal-body p').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/pi-localizacao-exterior&acao=A', function(response) {
                    $('#modal_localizacao_exterior .modal-body p').html(response);
                    $('.modal_dialog_localizacao_exterior').show();
                    $('#modal_localizacao_exterior').modal();
                    $('#modal_localizacao_exterior .chosen-select').chosen();
                    $('#modal_localizacao_exterior .chosen-container').css('width', '100%');
            });
        } else {
            $('#modal_localizacao_exterior').modal();
            $('#btnPopupLocalizacaoExteriorPesquisar').click();
        }
    }

   /**
    * Controla ações para quando o botão edital estiver marcado ou desmarcado.
    */
    function controlarEdital(opcao){
        if(opcao == true){
            $('#div_edital').show('slow');
        } else {
            $('#div_edital').hide('slow');
        }
    }

    /**
     * Controla a exibição do formulario de acordo com o enquadramento selecionado.
     *
     * @param integer codigo Código selecionado pelo usuário.
     * @returns VOID
     */
    function mudarFormularioFinalistico(codigo){
        // Se o código for diferente de Finalistico, o sistema oculta as opções PNC.
        if(codigo != "" && codigo != intEnqFinalistico){
            // Oculta as opções Meta PNC e Indicador PNC.
            $('.grupo_pnc').hide('slow');
            // Apaga as opções selecionadas.
            $('#mpnid').val('').trigger("chosen:updated");
            $('#ipnid').val('').trigger("chosen:updated");
        } else {
            // Mostra as opções Meta PNC e Indicador PNC.
            $('.grupo_pnc').show('slow');
        }
    }

    /**
     * Carrega novo conteúdo para o select de Subações via requisição ajax.
     */
    function carregarUG(unicod) {
        $.post('?modulo=principal/unidade/cadastro_pi&acao=A&carregarComboUG=ok&unicod=' + unicod, function(response) {
            $('#ungcod').remove('slow');
            $('.div_ungcod').html(response);
            $(".chosen-select").chosen();
        });
    }
    function carregarSubacao(unicod) {
//        var params = '&carregarComboSubacaoFederais=ok&unicod=' + unicod;
//        $.post(window.location + params, function(response) {
//            $('#sbaid').remove();
//            $('.cSubacao').html(response);
//            $(".chosen-select").chosen();
//        });
    }

    function carregarSemSubacao(unicod) {
        //alert(unicod);
        var params = 'carregarComboSubacaoInstituicoes=1&unicod=' + unicod;
        new Ajax.Request(
            window.location.href,
            {
                method: 'post',
                parameters: params,
                asynchronous: false,
                onComplete: function(response) {
                    //alert(response.responseText);
                    //limparFormulario(false);
                    $('plicodsubacao').update(response.responseText);
                }
            }
        );
    }

    /**
     * Carrega novo conteúdo para o select de Metas PPA via requisição ajax.
     */
    function carregarMetasPPA(oppid, mppid, suocod) {
        $.post('?modulo=principal/unidade/cadastro_pi&acao=A&carregarMetasPPA=ok&oppid=' + oppid + '&suocod=' + suocod, function(response) {
            $('#mppid').remove();
            $('.div_mppid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega limites da Sub-Unidade via requisição ajax.
     */
    function carregarLimitesUnidade(codigo) {
        $.ajax({
            url: '?modulo=principal/unidade/cadastro_pi&acao=A&carregarLimitesUnidade=ok',
            type: "post",
            data: {'ungcod': codigo},
            dataType: 'json',
            success: function(data){
                $('#td_autorizado_sub_unidade').text(data.autorizado);
                var fltDisponivelUnidade = textToFloat(data.disponivel);
                var fltBaseProjeto = buscarValorBaseDoProjeto();
                var fltDisponivel = (fltDisponivelUnidade - fltBaseProjeto) + buscarValorDoProjeto();
                $('#td_disponivel_sub_unidade').text(mascaraglobal('###.###.###.###,##', fltDisponivel.toFixed(2)));
                $('#VlrSUDisponivel').val(mascaraglobal('###.###.###.###,##', fltDisponivel.toFixed(2)));
                
                mudarCorValorProjeto();
            }
        });
    }

    /**
     * Carrega novo conteúdo para o select de metas PNC via requisição ajax.
     */
    function carregarMetaPNC(codigo) {
        $.post('?modulo=principal/unidade/cadastro_pi&acao=A&carregarMetaPNC=ok&suocod=' + codigo, function(response) {
            $('#mpnid').remove();
            $('.div_mpnid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega novo conteúdo para o select de Indicadores PNC via requisição ajax.
     */
    function carregarIndicadorPNC(codigo) {
        $.post('?modulo=principal/unidade/cadastro_pi&acao=A&carregarIndicadorPNC=ok&mpnid=' + codigo, function(response) {
            $('#ipnid').remove();
            $('.div_ipnid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega novo conteúdo para o select de Segmento Cultural via requisição ajax.
     */
    function carregarSegmentoCultural(codigo) {
        $.post('?modulo=principal/unidade/cadastro_pi&acao=A&carregarSegmentoCultural=ok&mdeid=' + codigo, function(response) {
            $('#neeid').remove();
            $('.div_neeid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega novo conteúdo para o select de Manutenção Item via requisição ajax.
     */
    function carregarManutencaoItem(codigo) {
        $.post('?modulo=principal/unidade/cadastro_pi&acao=A&carregarManutencaoItem=ok&eqdid=' + codigo + '&maiid=' + intMaiid, function(response) {
            $('#maiid').remove();
            $('#masid').val('').trigger("chosen:updated");
            $('.div_maiid').html(response);
            formatarTelaEnquadramentoComManutencaoItem();
        });
    }
    
    function formatarTelaEnquadramentoComManutencaoItem(){
        // Verifica se o usuário já estiver preenchido o enquadramento(Caso de formulário de cadastro de novo PI).
        if($('#eqdid').val() != ""){
            // Se existir itens de manutenção oculta campos do formulário 
            if($('#maiid option').size() > 0){
                $('.grupo_manutencao').show('slow');
                // Desabilita a opção de modificar os campos de Título e Descrição.
                $('[name=plititulo]').attr('readonly', 'readonly');
                $('[name=plidsc]').attr('readonly', 'readonly');
                // Oculta os campos de metas PPA e PNC
                $('.div_metas_ppa_pnc').hide('slow');
                // Oculta os campos do Produto do PI
                $('.div_produto_pi').hide('slow');
            } else {
                $('.grupo_manutencao').hide('slow');
                // Habilita a opção de modificar os campos de Título e Descrição.
                $('[name=plititulo]').removeAttr('readonly');
                $('[name=plidsc]').removeAttr('readonly');
                // Exibe os campos de metas PPA e PNC
                $('.div_metas_ppa_pnc').show('slow');
                // Exibe os campos do Produto do PI
                $('.div_produto_pi').show('slow');
            }
        }
    }

    /**
     * Carrega novo conteúdo para o select de Manutenção SubItem via requisição ajax.
     */
    function carregarManutencaoSubItem(codigo) {
        $.post('?modulo=principal/unidade/cadastro_pi&acao=A&carregarManutencaoSubItem=ok&maiid=' + codigo, function(response) {
            $('#masid').remove();
            $('.div_masid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Carrega novo conteúdo para o select de Metas PPA via requisição ajax.
     */
    function carregarIniciativaPPA(codigo) {
        $.post('?modulo=principal/unidade/cadastro_pi&acao=A&carregarIniciativaPPA=ok&oppid=' + codigo, function(response) {
            $('#ippid').remove();
            $('.div_ippid').html(response);
            $(".chosen-select").chosen();
        });
    }

    /**
     * Apaga todo o conteúdo o formulário. O select de subação é opcional.
     */
    function limparFormulario(limparSubacao)
    {
        if (limparSubacao) {
            $('sbaid').length = 0;
            var sbaid = $('sbaid');
            sbaid.options[sbaid.options.length] = new Option('Selecione', '');
        }
        $('sbadsc').update('Descrição...');
        $('valor_dotacao').update('0,00');
        $('scpdotacaosubacao').value = '0,00';
        $('valor_detalhado').update('0,00');
        $('scpdetalhadopisubacao').value = '0,00';
        $('valor_empenhado').update('0,00');
        $('scpempenhadosubacao').value = '0,00';

        $('eqdid').length = 0;
        var eqdid = $('eqdid');
        eqdid.options[eqdid.options.length] = new Option('Selecione', '');
        $('plititulo').update();
        $('prefixotitulo').update();
        $('enquadramento').update();
        $('enquadramento_i').update();
        $('subacao').update('XXXX');
        $('subacao_i').update('XXXX');
        $('nivel').update();
        $('nivel_i').update();
        $('apropriacao').update();
        $('apropriacao_i').update();
        $('codificacao').update();
        $('codificacao_i').update();
        $('modalidade').update();
        $('modalidade_i').update();
    }

    function validar(plicod)
    {
        var msg = "";
        var valorNulo = "";
        var semDet = "";
        var tabela = document.getElementById('orcamento');
        var ehSolicitacao = $('ehSolicitacao').value;
        var tipoTransacao = $('tipotransacao').value;
        if ($('unicod').selectedIndex < 1) {
            msg += "O preenchimento do campo Unidade Orçamentária é obrigatório.\n";
        }
        // validando se existe ação selecionado/ valor
        if (!ehSolicitacao || 'E' == tipoTransacao) {
            if (tabela.rows.length == 5) {
                semDet += "Você deve selecionar, no mínimo, uma ação no Detalhamento Orçamentário.\n";
            }
            else {
                jQuery('.somar').each(function() {
                    var valor = jQuery(this).val();
                    if ('' === valor || '0' === valor || '0,00' === valor) {
                        valorNulo += "Valor do PTRES '" + jQuery(this).attr('data-ptres') + "' está vazio.\n";
                    }
                });
            }

        }
        if (semDet != "" && msg == "") {
            var resposta = confirm(valorNulo + "Deseja realmente gravar um PI sem Detalhamento?");
            if (resposta != true) {
                return false;
            } else {
                return true;
            }
        }
        if (valorNulo != "" && msg == "") {
            var resposta = confirm(valorNulo + "Deseja realmente gravar um PI sem valor previsto?");
            if (resposta != true) {
                return false;
            } else {
                return true;
            }
        }
        if (valorNulo == "" && msg == "" && semDet == "") {
            return true;
        }
    }

    function validapi(pi)
    {
        var retorno = true;

        var req = new Ajax.Request(window.location.href, {
            method: 'post',
            parameters: 'piAjax=' + pi + '&pliid=' + document.getElementById('pliid').value,
            asynchronous: false,
            onComplete: function(res) {
                x = res.responseText;
                retorno = valida2(x);
            }
        });

        return retorno;
    }

    function valida2(pi)
    {
        if (pi.indexOf('pijaexiste') != -1) {
            while (pi.substr(0, 1) == ' ') {
                pi = pi.substr(1, pi.length);
            }
        } else {
            pi = pi.replace(/ /g, '');
        }
        if (!pi || pi == '') {
            return true;
        } else {
            if (pi.substr(0, 10) == 'pijaexiste') {
                pi = pi.replace('pijaexiste', "");
                var alertaDisplay = '<div class="titulo_box">Atenção!</div><div class="texto_box" >Plano Interno já existe!</div><div class="conteudo_box" >Veja abaixo os dados o Plano Interno cadastrado :</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" ><input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
                displayStaticMessage(alertaDisplay, false);
                return false;
            }
            var alertaDisplay = '<div class="titulo_box">Atenção!</div><div class="texto_box" >Já existe(m) PI(s) criado(s) com esta estrutura. Veja abaixo a relação dos PI(s) encontrados:</div><div class="conteudo_box" >' + pi + '</div><div class="links_box" >Deseja realmente criar?<br><center><input type="button" onclick=\'document.formulario.submit();\' value="Confirmar" /> <input type="button" onclick=\'closeMessage();return false\' value="Cancelar" /></center>';
            displayStaticMessage(alertaDisplay, false);
            return false;
        }
    }

    function removerpi()
    {
        var conf = confirm("Você realmente deseja excluir este PI?");
        if (conf) {
            document.getElementById('evento').value = 'E';
            document.formulario.submit();
        }
    }

    function Trim(str)
    {
        return str.replace(/^\s+|\s+$/g, "");
    }

    //coloca tabindex no campo valor
    function tabindexcampo() {
        var x = document.getElementsByTagName("input");
        var y = 1;
        for (i = 0; i < x.length; i++) {
            if (x[i].type == "text") {
                if (x[i].name.substr(0, 8) == 'plivalor') {
                    x[i].tabIndex = y;
                    y++;
                }
            }
        }
    }

    function semSubacao() {
        var nome = $("#plicodsubacao").val();
        $("#subacao").text(nome);
        $("#subacao_i").text(nome);
        //$("#prefixotitulo").html(nome + ' - ');
        $('#plititulo').attr('maxlength', 45 - 3 - Trim(nome).length);
    }

    function comSubacao() {
        var nome = jQuery("#plicodsubacao").val();
        $("#subacao").text(nome);
        $("#subacao_i").text(nome);
    }
    function vincular(situacao, pliid) {
        if (pliid) {
            var url = window.location.href;
            var parametros = "requisicao=vincular&pliid=" + pliid + '&situacao=' + situacao;
            var myAjax = new Ajax.Request(
                url,
                {
                    method: 'post',
                    parameters: parametros,
                    asynchronous: false,
                    onComplete: function(r) {
                        if (r.responseText) {
                            alert('Dados gravados com Sucesso.');
                            // feito isso por causa da presa.
                            //window.location.reload();
                            document.formulario.submit();
                        }
                    }
                }
            );
        }
    }

    function limpaDetalhamentoOrcamentario(){
        if($('#orcamento').find('tr')[4] != undefined){
            tamanho = $('#orcamento').find('tr').size();
            for(i = 2; i < tamanho - 2; tamanho--){
                $('#orcamento').find('tr')[i].remove();
            }
        }
    }

    function selecionarsubacao(sbaid) {
        //Limpar tabela de Detalhamento Orçamentário
        limpaDetalhamentoOrcamentario();

        var sbaidAnterior = document.getElementById("sbaidAnterior").value;
        if (!sbaidAnterior) {
            document.getElementById("sbaidAnterior").value = sbaid;
        }

        if (sbaid) {
            document.getElementById("subacao").innerHTML = "XXXX";
            document.getElementById("subacao_i").innerHTML = "XXXX";

            for (var i = (document.getElementById("orcamento").rows.length - 3); i > 2; i--) {
                document.getElementById("orcamento").deleteRow(i);
            }
            $.post('?modulo=principal/unidade/cadastro_pi&acao=A&sbaAjax=' + sbaid, function(response) {
                var dados = response;
                dados = dados.split("!@#");

                $('#prefixotitulo').html(Trim(dados[1]) + ' - ');
                $('#plititulo').value = '';
                $('#plititulo').attr('maxlength', 45 - 3 - Trim(dados[1]).length);
                $("#subacao").html(dados[0]);
                $("#subacao_i").html(dados[0]);
                $("#sbadsc").html(dados[3]);
                $("#valor_dotacao").html(dados[4]);
                $('#scpdotacaosubacao').value = dados[4];
                $("#valor_detalhado").html(dados[5]);
                $('#scpdetalhadopisubacao').value = dados[5];
                $("#valor_empenhado").html(dados[6]);
                $('#scpempenhadosubacao').value = dados[6];
            });

            atualizaTituloCodigoLivre();
            document.getElementById("sbaidAnterior").value = sbaid;

        } else {
            document.getElementById("subacao").innerHTML = 'XXXX';
            document.getElementById("subacao_i").innerHTML = 'XXXX';
            document.getElementById('sbadsc').innerHTML = 'XXXX';

            for (var i = (document.getElementById("orcamento").rows.length - 3); i > 1; i--) {
                document.getElementById("orcamento").deleteRow(i);
            }
        }
    }

    function submeter(pliid) {
//        var codsubacao = $('#subacao').text();
//        if ('' === codsubacao) {
//            alert('O código da subação não pode ser deixado em branco.');
//            return false;
//        }

        // O número do PI será gerado somente após a aprovação do PI pela equipe de Coordenação.
//        var pi = document.getElementById("enquadramento").innerHTML
//            + codsubacao
//            + document.getElementById("nivel").innerHTML
//            + document.getElementById("apropriacao").innerHTML
//            + document.getElementById("codificacao").innerHTML
//            + document.getElementById("modalidade").innerHTML;
//        $("#plicod").val(pi);

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

        // Verifica se o cronograma físico foi preenchido.
        if(!validarCronogramaFisicoPreenchido()){
            $('input.input_fisico').addClass('validateRedText');
            $('#td_total_fisico').addClass('validateRedText');
            addMsgCustom.push('Cronograma Fisíco');
        } else {
            $('input.input_fisico').removeClass('validateRedText');
            $('#td_total_fisico').removeClass('validateRedText');
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

        // Verifica se o valor do cronograma Físico é igual ao informado no Produto do PI.
        if(!validarCronogramaFisicoIgualQuantidade()){
            $('input.input_fisico').addClass('validateRedText');
            $('#td_total_fisico').addClass('validateRedText');
            addMsgCustom.push('Soma dos valores do Cronograma Fisíco está diferente da quantidade informada para o Produto do PI');
        } else {
            $('input.input_fisico').removeClass('validateRedText');
            $('#td_total_fisico').removeClass('validateRedText');
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

    /**
     * Verifica se o valor do cronograma Físico é igual ao informado no Produto do PI.
     * 
     * @returns {Boolean}
     */
    function validarCronogramaFisicoIgualQuantidade(){
        var resultado = false;
        var qtdCronograma = buscarTotalCronogramaFisico();
        var qtdProdutoPi = textToFloat($('#picquantidade').val());
        
        if(qtdCronograma == qtdProdutoPi){
            resultado = true;
        }
        
        return resultado;
    }
    
    /**
     * Verifica se o valor do cronograma Orcamentario é igual ao informado no valor do projeto.
     * 
     * @returns {Boolean}
     */
    function validarCronogramaOrcamentarioCusteioIgualValorProjeto(){
        var resultado = false;
        var vlrCronograma = buscarTotalCusteioCronogramaOrcamentario();
        var vlorProjeto = textToFloat($('#picvalorcusteio').val());
        
        if(vlrCronograma == vlorProjeto){
            resultado = true;
        }
        
        return resultado;
    }
    
    /**
     * Verifica se o valor do cronograma Orcamentario é igual ao informado no valor do projeto.
     * 
     * @returns {Boolean}
     */
    function validarCronogramaOrcamentarioCapitalIgualValorProjeto(){
        var resultado = false;
        var vlrCronograma = buscarTotalCapitalCronogramaOrcamentario();
        var vlorProjeto = textToFloat($('#picvalorcapital').val());
        
        if(vlrCronograma == vlorProjeto){
            resultado = true;
        }
        
        return resultado;
    }

    /**
     * Verifica se o valor do cronograma Financeiro é igual ao informado no valor do projeto.
     * 
     * @returns {Boolean}
     */
    function validarCronogramaFinanceiroCusteioIgualValorProjeto(){
        var resultado = false;
        var vlrCronograma = buscarTotalCusteioCronogramaFinanceiro();
        var vlorProjeto = textToFloat($('#picvalorcusteio').val());
        
        if(vlrCronograma == vlorProjeto){
            resultado = true;
        }
        
        return resultado;
    }

    /**
     * Verifica se o valor do cronograma Financeiro é igual ao informado no valor do projeto.
     * 
     * @returns {Boolean}
     */
    function validarCronogramaFinanceiroCapitalIgualValorProjeto(){
        var resultado = false;
        var vlrCronograma = buscarTotalCapitalCronogramaFinanceiro()
        var vlorProjeto = textToFloat($('#picvalorcapital').val());
        
        if(vlrCronograma == vlorProjeto){
            resultado = true;
        }
        
        return resultado;
    }

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
            listaObrigatorios.push('oppid', 'mppid', 'ippid', 'pprid', 'pumid', 'picquantidade');
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

    function submeterComSituacao(pliid, situacao) {
        var validado = true;

        if (!validar()) {
            return false;
        }

        if (validado) {
            document.formulario.plisituacao.value = situacao;
            document.formulario.submit();
        }
    }

    function alterarpi(pliid) {
        document.getElementById('evento').value = 'A';
        document.getElementById('pliid').value = pliid;
        document.formulario.submit();
    }

    /* Função para subustituir todos */
    function replaceAll(str, de, para) {
        var pos = str.indexOf(de);
        while (pos > -1) {
            str = str.replace(de, para);
            pos = str.indexOf(de);
        }
        return (str);
    }

    function abrir_lista() {
        // Verifica se o modal terá que recarregar a tela.
        if($('#modal-ptres .modal-body p').size() <= 1){
            $.post('planacomorc.php?modulo=principal/unidade/popupptres&acao=A&obrigatorio=n&unicod='+ $("#unicod").val()+ '&no_ptrid='+ $('input[name=ptrid]').val(), function(response) {
                $('#modal-ptres .modal-body p').html(response);
                $('.modal-dialog-ptres').show();
                $('#modal-ptres').modal();
                $('#modal-ptres .chosen-select').chosen();
                $('#modal-ptres .chosen-container').css('width', '100%');
            });
        } else {
            $('#formularioPopup input[name=unicod]').val($("#unicod").val());
            $('#modal-ptres').modal();
            $('#btnPopupPtresPesquisar').click();
        }
    }

    function visualizarRegistro(ptrid) {
        $('#modal-confirm .modal-body p').empty();
        url = 'planacomorc.php?modulo=principal/unidade/popupptres&acao=A&detalhe=ok&ptrid=' + ptrid;
        $.post(url, function(html) {
            $('#modal-confirm .modal-body p').html(html);
            $('#modal-confirm .modal-title').html('Detalhamento PTRES');
            $('#modal-confirm .btn-primary').remove();
            $('#modal-confirm .btn-default').html('Fechar');
            $('.modal-dialog').show();
            $('#modal-confirm').modal();
        });
    }

    function atualizarPrevisaoPI() {
        if (document.getElementById('capid').value) {
            var apropriacao = document.getElementById('capid').options[document.getElementById('capid').selectedIndex].text.split(" - ");
            document.getElementById("apropriacao").innerHTML = apropriacao[0];
            document.getElementById("apropriacao_i").innerHTML = apropriacao[0];
        }

        if (document.getElementById('eqdid').value) {
            var enquadramento = document.getElementById('eqdid').options[document.getElementById('eqdid').selectedIndex].text.split(" - ");
            document.getElementById("enquadramento").innerHTML = enquadramento[0];
            document.getElementById("enquadramento_i").innerHTML = enquadramento[0];
        } else {
            document.getElementById("enquadramento").innerHTML = '';
            document.getElementById("enquadramento_i").innerHTML = '';
        }

        if (document.getElementById('neeid').value) {
            var nivel = document.getElementById('neeid').options[document.getElementById('neeid').selectedIndex].text.split(" - ");
            document.getElementById("nivel").innerHTML = nivel[0];
            document.getElementById("nivel_i").innerHTML = nivel[0];
        }
        if (document.getElementById('mdeid').value) {
            var modalidade = document.getElementById('mdeid').options[document.getElementById('mdeid').selectedIndex].text.split(" - ");
            document.getElementById("modalidade").innerHTML = modalidade[0];
            document.getElementById("modalidade_i").innerHTML = modalidade[0];
        }

        document.getElementById("idCodificacao").value = document.getElementById("idCodificacao").value.toUpperCase();
        document.getElementById("codificacao").innerHTML = document.getElementById("idCodificacao").value;
        document.getElementById("codificacao_i").innerHTML = document.getElementById("idCodificacao").value;

        atualizaTituloCodigoLivre();
        //semSubacao();
    }


    function atualizarCodigoLivre() {
        document.getElementById("idCodificacao").value = document.getElementById("idCodificacao").value.toUpperCase();
        document.getElementById("codificacao").innerHTML = document.getElementById("idCodificacao").value;
        atualizaTituloCodigoLivre();
    }

    function carregarComboEnquadramentoPorSubacao(sbaid) {
        var req = new Ajax.Request(window.location.href, {
            method: 'post',
            parameters: '&carregarComboEnquadramentoPorSubacao=1&sbaid=' + sbaid,
            asynchronous: false,
            onComplete: function(res) {
                if (res.responseText) {
                    $('comboEnquadramento').update(res.responseText);
                }
            }
        });
        atualizarPrevisaoPI();
    }

    function atualizaTituloCodigoLivre() {
        var idCodificacao = document.getElementById("idCodificacao").value;
        if (idCodificacao == '00') {
            var msg = "";
            if (!document.getElementById('capid').value) {
                msg += 'Favor escolha uma Categoria de Apropriação.\n';
            }

            if (msg) {
                alert(msg);
                return false;
            }
        }
    }
    function subacao(estilo) {
        if (estilo == 's') {
            $('#cSubacao').css("display", "none");
            $('#sSubacao').css("display", "block");
            $('#detSub').html('Detalhado em Subação (R$)');
            $('#detPi').html('Detalhado em PI (R$)');
            $('#prefixotitulo').parent().css('display','none');
            $('#plititulo').parent().removeClass('col-lg-9');
            $('#plititulo').parent().addClass('col-lg-9');
            $('#tipoSubacao').val('s');
        } else {
            $('#cSubacao').css("display", "block");
            $('#sSubacao').css("display", "none");
            $('#detSub').html('Detalhado na Subação (R$)');
            $('#detPi').html('Detalhado em PI na Subação (R$)');
            $('#prefixotitulo').parent().css('display','block');
            $('#plititulo').parent().removeClass('col-lg-9');
            $('#plititulo').parent().addClass('col-lg-9');
            $('#tipoSubacao').val('c');
        }
    }

    function calculovalorPI() {
        var tabela = document.getElementById('orcamento');
        var tot = 0;

        $('.somar').each(function() {
            var valor = $(this).val();

            if ('' == valor) {
                valor = '0';
            }

            valor = replaceAll(valor, '.', '');
            valor = replaceAll(valor, ',', '.');
            tot += parseFloat(valor);
        });

        var c = tot.toString();
        if (c.indexOf('.') == -1) {
            document.getElementById('valortotalpi').value = tot.toFixed(2);
        } else {
            document.getElementById('valortotalpi').value = Arredonda(tot, 2);
        }
        document.getElementById('valortotalpi').onkeyup();
    }

    function Arredonda(valor, casas) {
        var novo = Math.round(valor * Math.pow(10, casas)) / Math.pow(10, casas);
        var c = novo.toString();
        if (c.indexOf('.') == -1) {
            alert(novo);
            return novo;
        } else {
            return novo.toFixed(casas);
        }
    }

    function verificaDisponivel(campo, ptrid, vlold) {
        var linha = document.getElementById('ptres_' + ptrid);
        valordisp = parseFloat(replaceAll(replaceAll($(linha.cells[6]).text(),'.',''),',','.'))
        valoratual = parseFloat(replaceAll(replaceAll(campo.value, ".", ""), ",", "."));
        valorDotacao = parseFloat(replaceAll(replaceAll($(linha.cells[2]).text(),'.',''),',','.'))

        /* Troca a referência caso seja sem Subação */
        if(valoratual > valorDotacao){
            alert('O valor não pode ser maior do que o valor da Dotação Atual do PTRES.');
            $(campo).val('0');
            calculovalorPI(); return false;
        }
    }

    function contar() {
        var l = document.formulario.plicodsubacao.value.length;
        if (l < 4) {
            alert("Sua subação só possui " + l + " caracteres. Favor inserir 4 caracteres.")
        }
    }
