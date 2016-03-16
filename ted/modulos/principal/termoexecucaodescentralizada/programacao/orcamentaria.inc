<?php
/**
 * Página da tabela Orçamentário pertencente à aba "Programção Orçamentária e Financeira".
 *
 * @version $Id$
 */

$orcamento = new Ted_Model_Orcamento();
$dadosOrcamento = $orcamento->getByTcpid(Ted_Utils_Model::capturaTcpid());
$jsonDadosOrcamento = json_encode(array('orcamento' => $dadosOrcamento));
?>
<script type="text/javascript">
$(function(){
    $('.btn-adicionar', $('#dados-orcamentarios').prev()).click(function(){
        // -- Resetando seleções
        $('#orcamentaria_proanoreferencia').val('').trigger('chosen:updated');
        $('#orcamentaria_ndpid').val('').trigger('chosen:updated');
        // -- Limpando PTRES e PI
        $('#orcamentaria_ptrid option').remove();
        $('#orcamentaria_ptrid').attr('data-placeholder', 'Selecione um item').trigger('chosen:updated');
        $('#orcamentaria_pliid option').remove();
        $('#orcamentaria_pliid').attr('data-placeholder', 'Selecione um item').trigger('chosen:updated');
        // -- Abre a popup para criação do novo item
        $('#mdl-orcamentario .modal-title').empty().text('Novo item orçamentário');
        $('#mdl-orcamentario').modal();
    });
});

function editarOrcamentario(proid, ptrano, ptrid, pliid, ndpid, provalor)
{
    $('#orcamentaria_proid').val(proid);
    $('#orcamentaria_proanoreferencia')
        .val(ptrano)
        .trigger('chosen:updated')
        .attr('data-ptrid', ptrid) // -- atributos para atualização cascateada
        .attr('data-pliid', pliid) // -- atributos para atualização cascateada
        .change();

    $('#orcamentaria_ndpid').val(ndpid).trigger('chosen:updated');
    $('#orcamentaria_provalor').val(provalor).blur();

    $('#mdl-orcamentario .modal-title').empty().text('Alterar item orçamentário');
    $('#mdl-orcamentario').modal();
}

function apagarOrcamentario(proid)
{
    var $form = $('<form />', {method:'POST'});
    $form.append($('<input />', {type:'hidden',name:'action',value:'apagarOrcamentaria'}));
    $form.append($('<input />', {type:'hidden',name:'orcamentaria[proid]',value:proid}));
    $form.appendTo('body').submit();
}
</script>
<div class="row col-md-12">
<?php
$list = new Simec_Listagem();
$list->setId('dados-orcamentarios')
    ->setDados($dadosOrcamento)
    ->setCabecalho(array('Exercício', 'PTRES', 'Ação', 'Descrição da Ação', 'PI', 'Natureza de Despesa', 'Valor (R$)'))
    ->setCampos(array('proid', 'tcpid', 'ptrid', 'pliid', 'provalor', 'ndpid', 'proanoreferencia', 'prostatus'))
    ->esconderColunas('tcpid', 'prostatus', 'ptrid', 'pliid', 'plidsc', 'ndpid', 'ndpdsc', 'unicod', 'loccod', 'funcod', 'sfucod', 'prgcod')
    ->addAcao('edit', array('func' => 'editarOrcamentario', 'extra-params' => array('ptrano', 'ptrid', 'pliid', 'ndpid', 'provalor')))
    ->addAcao('delete', 'apagarOrcamentario')
    ->addCallbackDeCampo('provalor', 'mascaraMoeda')
    ->addCallbackDeCampo('plicod', 'formatarOrcamentarioPi')
    ->addCallbackDeCampo('ndpcod', 'formatarOrcamentarioNatureza')
    ->addCallbackDeCampo('ptres', 'formatarOrcamentarioPtres')
    ->turnOnPesquisator()
    ->addToolbarItem(Simec_Listagem_Renderer_Html_Toolbar::ADICIONAR)
    ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('provalor'))
    ->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);

// -- popup de adição de novos itens
bootstrapPopup(
    'Orçamentário',
    'mdl-orcamentario',
    dirname(__FILE__) . DIRECTORY_SEPARATOR . 'modalOrcamentaria.inc',
    array('cancelar', 'confirmar'),
    array('tamanho' => 'lg')
);
?>
</div>