
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

        carregarLimitesUnidade($('#ungcod').val());
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
        
        $('#table_anexos').on('click', '.btnRemoverAnexos', function(){
            var id = $(this).attr('data-anexos');
            $('.tr_anexos_'+ id).remove();
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
        
        $('#btn_inserir_anexos').click(function(){
            abrirModalUpload();
        });
        
        $('#btnSalvarAnexo').click(function(){
            $('#formularioAnexo').submit();
        });

        // Evento de terminar de carregar arquivos
        Dropzone.options.formularioAnexo = {
            init: function() {
                
                this.on("success", function(file, response){
                    var jsonResponse = $.parseJSON(response);
                    inserirNovoAnexo(jsonResponse);
//                    console.log(jsonResponse.arqid);
//                    console.log(jsonResponse.arqnome);
//                    console.log(jsonResponse.arqdescricao);
                });

                this.on("queuecomplete", function(file){

                    // Armazena o objeto Dropzone para chamar métodos
                    objFormularioAnexo = this;
                    // Chama mensagem de sucesso
                    swal({
                      title: "",
                      text: "Arquivos salvos com sucesso!",
                      timer: 2000,
                      showConfirmButton: false,
                      type: "success"
                    }, function(){
                        // Fecha o swal alert
                        swal.close();
                        // limpa campo de upload
                        objFormularioAnexo.removeAllFiles();
                        // fecha modal após a seleção
                        $('#modal_upload').modal('hide');
                    });
                });
            }
        };

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

        $('#pprid').change(function(){
            formatarTelaProdutoNaoAplica($(this).val());
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
        
        formatarTelaProdutoNaoAplica($('#pprid').val());
    }
