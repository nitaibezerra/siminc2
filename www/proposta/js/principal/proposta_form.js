
/**
 * Comportamentos executados no momento em que a tela está pronta e carregada.
 * 
 * @returns VOID
 */
function initPropostaForm(){
    
    $('#eqdid, #suoid').change(function(){
        carregarFuncional();
    });

    // Carrega RP do Enquadramento caso exista enquadramento selecionado
    if($('#eqdid').val() !== ''){
        carregarRp();
    }

    $('#eqdid').change(function(){
        carregarRp();
    });

    // Carrega dados complementares da Funcional se existir funcional selecionada
    if($('#ptrid').val() !== ''){
        carregaQuantidadesFuncional();
        carregarLimitesPrePi();
        carregarSubunidadesDaFuncional();
    }

    $('body').on('change', '#ptrid', function(){
        carregaQuantidadesFuncional();
        carregarLimitesPrePi();
        carregarSubunidadesDaFuncional();
    });

}

/**
 * Carrega RP do Enquadramento
 * 
 * @returns VOID
 */
function carregarRp(){
    $('#span-rp').load('?modulo=principal/proposta_form&acao=A&req=carregar-rp&eqdid=' + $('#eqdid').val());
}

/**
 * Carrega opções para Funcional.
 * 
 * @returns VOID
 */
function carregarFuncional(){
    $('#span-funcional').load('?modulo=principal/proposta_form&acao=A&req=carregar-funcional&eqdid=' + $('#eqdid').val() + '&suoid=' + $('#suoid').val());
}

/**
 * Carrega Subunidades Vinculadas à Funcional
 * 
 * @returns VOID
 */
function carregarSubunidadesDaFuncional(){
    $('#span-subunidades-ptres').load('?modulo=principal/proposta_form&acao=A&req=recuperar-subunidades-ptres&ptrid=' + $('#ptrid').val());
}

/**
 * Carrega limites Pré-PI
 * 
 * @returns VOID
 */
function carregarLimitesPrePi(){
    $.ajax({
        url: '?modulo=principal/proposta_form&acao=A&req=recuperar-valores-ptres&ptrid=' + $('#ptrid').val() + '&suoid=' + $('#suoid').val(),
        dataType: 'json',
        success: function(dados){
            $('#limite_valor_custeio').html(number_format(parseFloat(dados.plivalorcusteio), 0, ',', '.'));
            $('#limite_valor_capital').html(number_format(parseFloat(dados.plivalorcapital), 0, ',', '.'));
            $('#limite_expansao_custeio').html(number_format(parseFloat(dados.plivalorcusteioadicional), 0, ',', '.'));
            $('#limite_expansao_capital').html(number_format(parseFloat(dados.plivalorcapitaladicional), 0, ',', '.'));
        }
    });
}

/**
 * Carrega Quantidade, Unidade e Produto da Funcional do localizador e PO.
 * 
 * @returns VOID
 */
function carregaQuantidadesFuncional(){
    $.ajax({
        url: '?modulo=principal/proposta_form&acao=A&req=recuperar-dados-ptres&ptrid=' + $('#ptrid').val(),
        dataType: 'json',
        success: function(dados){
            $('#unidade_acao').html(dados.unidade_acao);
            $('#produto_acao').html(dados.produto_acao);
            $('#unidade_po').html(dados.unidade_po);
            $('#produto_po').html(dados.produto_po);
            $('#locquantidadeproposta').val(number_format(parseFloat(dados.locquantidadeproposta), 0, ',', '.'));
        }
    });
}

