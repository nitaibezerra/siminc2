<?php
/**
 * Lista de planos orçamentários para seleção.
 *
 * $Id: modalPlanoOrcamentario.inc 101111 2015-08-11 14:36:32Z maykelbraz $
 * @filesource
 */
global $dspid, $exercicio;

$form = new Simec_View_Form('planoorcamentario');
$queryUnidades = Proporc_Model_Despesaplanoorcamentario::queryUnidades($dspid, $exercicio);
$queryAcoes = Proporc_Model_Despesaplanoorcamentario::queryAcoes($dspid, $exercicio);

$form->addInputCombo('unicod', $queryUnidades, null, 'filtro-unicod', array('flabel' => 'Unidade Orçamentária'))
    ->addInputCombo('acacod', $queryAcoes, null, 'filtro-acacod', array('flabel' => 'Ação'))
    ->addBotoes(array('buscar'))
    ->setRequisicao('filtrarPlanoorcamentario')
    ->render();

$list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
$list->setQuery(Proporc_Model_Despesaplanoorcamentario::querySelecaoDePlanoOrcamentario($exercicio))
    ->turnOnPesquisator()
    ->addToolbarItem(Simec_Listagem_Renderer_Html_Toolbar::INVERTER)
    ->setFormFiltros('planoorcamentario')
    ->setCabecalho(array('Programática', 'Plano Orçamentário'))
    ->addCallbackDeCampo('descricao', 'alinhaEsquerdaComId')
    ->addAcao('select', array(
        'func' => 'selecionarPlanoOrcamentario',
        'desmarcado' => true,
        'extra-params' => array('programatica', 'descricao')
    ))
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
<script type="text/javascript">
function selecionarPlanoOrcamentario(id, marcar, data)
{
    if (marcar) { // -- insere item na tabela do formulário
        var prototipo = $('#adicionais table.tabela-listagem')
                .attr('data-prototipo')
                .replace(/%1%/g, id)
                .replace('%2%', data.programatica)
                .replace('%3%', data.descricao);

        // -- removendo mensagem de lista vazia
        if ($('#adicionais table.tabela-listagem tr.lista-vazia')[0]) {
            $('#adicionais table.tabela-listagem tr.lista-vazia').remove();
        }

        // -- adicionando um novo elemento à tabela principal
        $('#adicionais table.tabela-listagem').append(prototipo);

        // -- armazenando o elemento na lista de verificacao rapida - adicionais.inc
        listaPlanosOrcamentarios.push(id);
    } else { // -- removendo item da tabela do formulário
        apagarPlanoOrcamentario(id); // -- adicionais.inc
    }
}

// -- gerenciamento do botão marcar da toolbar da listagem
$('#mdl-planoorcamentario').on('click', '.btn-marcar', function(){
    var marcar = (undefined !== $('.glyphicon-ok', this)[0]),
        classebotao = marcar?'.glyphicon-remove':'.glyphicon-ok';
    $('#mdl-planoorcamentario tr:not([class="listagem-remover"]) ' + classebotao).each(function(){
        // -- show modal
        $(this).click();
        // -- hide modal
    });

    if (marcar) {
        $('span', this).addClass('glyphicon-remove')
            .removeClass('glyphicon-ok');
    } else {
        $('span', this).removeClass('glyphicon-remove')
            .addClass('glyphicon-ok');
    }
}).on('submit', '#planoorcamentario', function(e){
    e.preventDefault();
    $.get($('#planoorcamentario').attr('action'), $('#planoorcamentario').serialize(), function(html){
        $('#mdl-planoorcamentario form.form-listagem').empty().html(html);
        // -- restaurando popovers e pesquisator
        enablePesquisator();
        toggleItensLista();
    }, 'html');
});
</script>