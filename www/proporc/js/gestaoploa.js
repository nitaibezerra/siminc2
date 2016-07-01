/**
 * Funções javascript do módulo de proposta orçamentária - gestão da ploa.
 * $Id: gestaoploa.js 88652 2014-10-17 13:43:21Z lindalbertofilho $
 */

/**
 * Carrega de um financeiro para edição do valor. Os dados são carregados na popup de
 * financeiro. Após carregar os dados exibe a popup de financeiro para o usuário
 * prosseguir com as alterações necessárias.
 *
 * @param {Integer} dpaid ID despesa ação.
 * @param {Integer} ploid ID do plano orçamentário.
 * @param {Integer} sbaid ID da subação.
 * @param {String} ndpcod Código da natureza de despesa.
 * @param {String} foncod Código da fonte de recursos.
 * @param {Numeric} dpavalor Valor para a ação considerando grupo e coluna.
 * @returns {undefined}
 */
function editarFinanceiro(dpaid, ploid, ungcod, sbaid, ndpid, ndpcod, foncod, plfvalor)
{
    $('#ploid').val(ploid);
    $('#sbaid').val(sbaid);
    $('#ungcod').val(ungcod);
    $('#ndpid').val(ndpid);
    $('#ndpcod').val(ndpcod);
    $('#foncod').val(foncod);
    $('#plfvalor').val(plfvalor).blur();
    // -- Atualizando todos os chosen da modal
    $('#modal-financeiro .chosen-select').attr('disabled', true).trigger('chosen:updated');
    $('#modal-financeiro #ungcod').removeAttr('disabled').trigger('chosen:updated');
    $('#modal-financeiro #sbaid').removeAttr('disabled').trigger('chosen:updated');

    // -- Setando DPAID em alteração e setando o tipo de requisição
    $('#dpaid').val(dpaid);
    $('#requisicao').val('alterarFinanceiro');

    $('#modal-financeiro').modal();
}

/**
 *
 * @returns {undefined}
 */
function novoFinanceiro()
{
    var ploid = $('#filtro_ploid').val();
    // -- Validando se um plano orçamentário foi escolhido antes da inclusão de uma nova despesa
    if ('' == ploid) {
        $('#modal-alert .modal-body').text('Antes de prosseguir, você deve escolher um Plano orçamentário.');
        $('#modal-alert').modal();
        return;
    }

    // -- Limpando todas as seleções e valores
    $('#modal-financeiro .chosen-select').val('').removeAttr('disabled').trigger('chosen:updated');
    $('#plfvalor').val('');
    // -- Setando o PO selecionado para criação do novo item
    $('#ploid').val(ploid).attr('disabled', true).trigger('chosen:updated');
    // -- Limpando DPAID e setando o tipo de requisição
    $('#requisicao').val('novoFinanceiro');
    $('#modal-financeiro').modal();
}

/**
 * Verifica se há limite disponível para a coluna e submete o formulário financeiro.
 * @returns {undefined}
 */
function salvarFinanceiro(e)
{
    e.preventDefault();

    var novoValor = $('#plfvalor').val();
    if ('' == novoValor) {
        novoValor = '0';
    }

    if (checaLimite(novoValor.replace(/\./g, ''))) {
        validarFormulario(
            ['sbaid', 'ungcod', 'ndpid', 'foncod', 'plfvalor'],
            'formfinanceiro',
            $('#requisicao').val()
        );
    } else {
        $('#modal-alert .modal-body').text('Não existe saldo disponível para a sua solicitação.');
        $('#modal-alert').modal();
    }
}

/**
 * Verifica se há limite suficiente para atender a solicitação.
 * @param {Numeric} novoValor Valor para verificação dentro do saldo
 * @returns {Boolean}
 */
function checaLimite(novoValor)
{
    var saldoColuna = $('#saldocoluna').val();
    return (parseFloat(novoValor) <= parseFloat(saldoColuna));
}

function detalharFinanceiro(dpaid, mtrid)
{
    $.post(
        window.location,
        {requisicao:'detalharFinanceiro', 'dados[dpaid]':dpaid, 'dados[mtrid]':mtrid},
        function(html){
            $('#modal-info .modal-body').html(html);
            $('#modal-info').modal();
        }
    );
}

function excluirFinanceiro(dpaid)
{
    $('#dpaid').val(dpaid);
    $('#requisicao').val('excluirFinanceiro');
    $('#modal-confirm').modal();
}

function imprimirRelatorioQDD()
{
    var winrelatorio = window.open('', 'Imprimir', 'height=600,width=800');
    winrelatorio.document.write('<html><head><title></title>');
    winrelatorio.document.write('</head><body>');
    winrelatorio.document.write($('#relatorio-qdd').html());
    winrelatorio.document.write('</body></html>');
    winrelatorio.print();
    winrelatorio.close();
}

/**
 *
 * @param {type} relatorio
 * @returns {undefined}
 */
function exportarRelatorioQDD(relatorio)
{
    var unicod = $('#unicod').val();
    var ppoid = $('#ppoid').val();
    var exercicio = $('#exercicio_spo').val();

    var $formExportarRelatorio = $('#formexportqdd');
    if ($formExportarRelatorio[0]) {
        $('#export_req').val(relatorio);
        $formExportarRelatorio.submit();
        return;
    }

    var $formExportarRelatorio = $('<form />').attr({
        id: 'formexportqdd',
        method: 'post',
        target: 'formexportqdd'
    });    
    $formExportarRelatorio.append($('<input />').attr({name:'requisicao', value:relatorio, id:'export_req'}));
    $formExportarRelatorio.append($('<input />').attr({name:'dados[exercicio]', value:exercicio}));
    $formExportarRelatorio.append($('<input />').attr({name:'dados[ppoid]', value:ppoid}));
    $formExportarRelatorio.append($('<input />').attr({name:'dados[unicod]', value:unicod}));
    $formExportarRelatorio.append($('<input />').attr({name:'exportar', value:true}));
    $formExportarRelatorio.append($('<input />').attr({name:'relat', value:'planilha_lista'}));
    $formExportarRelatorio.append($('<input />').attr({name:'planilha', value:4}));    
    $formExportarRelatorio.submit();
}

function relatorioElabrev(requisicao, titulo, exercicio, ppoid)
{
    $.post(
        window.location,
        {
            requisicao:requisicao,
            'dados[unicod]':$('#unicod').val(),
            'dados[ppoid]':ppoid,
            'dados[exercicio]':exercicio
        },
        function (html){
            $('#modal-alert .modal-title').text(titulo);
            $('#modal-alert .modal-body').html(html.trim());
            $('#modal-alert').modal();
        }
    );
}

function relatorioElabrevQddPO(exercicio, ppoid, unidade)
{
    relatorioElabrev('showQDDPO', 'Quadro de Detalhamento das Despesas - PO - ' + unidade , exercicio, ppoid);
}

function relatorioElabrevQddSubacao(exercicio, ppoid , unidade)
{
    relatorioElabrev('showQDDSubacao', 'Quadro de Detalhamento das Despesas - Subação - ' + unidade, exercicio, ppoid);
}

function relatorioElabrevLimites(exercicio, ppoid, unidade)
{
    relatorioElabrev('showLimites', 'Limites Orçamentários da UO por Grupo de Coluna - ' + unidade, exercicio, ppoid);
}

function relatorioElabrevSintese(exercicio, ppoid, unidade)
{
    relatorioElabrev('showSinteseDespesas', 'Síntese de Despesas da UO - ' + unidade, exercicio, ppoid);
}