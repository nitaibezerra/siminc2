<?php
/**
 * Layout de configurações adicionais de despesa.
 *
 * $Id: adicionais.inc 102352 2015-09-11 14:52:35Z maykelbraz $
 */

$dspid = (int)$_REQUEST['dspid'];
$exercicio = $_SESSION['exercicio'];

$catDespesa = new Proporc_Model_Despesa();
list($dadosdespesa) = $catDespesa->recuperarTodos(
    't1.dspnome, t2.gdpnome',
    array("t1.dspid = {$dspid}"),
    '',
    array('join' => 'gdpid')
);

// -- painel
painelDetalheCategoriaDespesa($dadosdespesa);

$form = new Simec_View_Form('adicionais');
$form->addHidden('dspid', $dspid);

$sql = Proporc_Model_Despesaacao::queryTodasAsAcoes($exercicio);
$sqlSelecionados = Proporc_Model_Despesaacao::queryAcoesSelecionadas($dspid, $exercicio);
$configInput = array('multiple' => true, 'flabel' => 'Ações', 'titulo' => 'Selecione as Ações', '__strict__' => true);
$form->addInputCombo('acacod', $sql, $db->carregarColuna($sqlSelecionados), 'acacod', $configInput);

$sql = Proporc_Model_Despesasubacao::queryTodasAsSubacoes($exercicio);
$sqlSelecionados = Proporc_Model_Despesasubacao::querySubacoesSelecionadas($dspid, $exercicio);
$configInput = array('multiple' => true, 'flabel' => 'Subações', 'titulo' => 'Selecione as Subações');
$form->addInputCombo('sbaid', $sql, $db->carregarColuna($sqlSelecionados), 'sbaid', $configInput);

$sqlSelecionados = Proporc_Model_Despesaplanoorcamentario::queryPlanosSelecionados($dspid, $exercicio);
$configInput = array(
    'flabel' => 'Planos Orçamentários',
    'cabecalho' => array('Programática', 'Descrição'),
    'campos' => array('codigo', 'programatica', 'descricao'),
    'acoes' => array(array('delete', 'apagarPlanoOrcamentario')),
    'totalizador' => 'linhas',
    'callbacks' => array(
        array('descricao', 'alinhaEsquerdaComId'),
        array('programatica', 'inputHidden')
    ),
    'id' => 'adicionaisplanoorcamentario'
);
$form->addInputLista('fplocod', $sqlSelecionados, $configInput);
$form->addBotoes(array('salvar'))
    ->setRequisicao('salvarDespesaAdicionais');
$form->render();

// -- popup de adição de novos itens
bootstrapPopup(
    'Seleção de Plano Orçamentário',
    'mdl-planoorcamentario',
    dirname(__FILE__) . '/modalPlanoOrcamentario.inc',
    array('fechar'),
    array('tamanho' => 'lg')
);
?>
<script type="text/javascript">
var listaPlanosOrcamentarios = new Array();

function apagarPlanoOrcamentario(id)
{
    // -- removendo item do DOM
    $('#adicionais table.tabela-listagem p[data-id]').each(function(){
        if (id === $(this).attr('data-id')) {
            $(this).closest('tr').remove();

            return false; // -- break
        }
    });

    // -- removendo item da listaPlanosOrcamentarios
    listaPlanosOrcamentarios.splice(listaPlanosOrcamentarios.indexOf(id), 1);
}

function toggleItensLista()
{
    // -- marcando os elementos que já fazem parte da lista de POs selecionados
    $('#mdl-planoorcamentario form.form-listagem span[data-id]').each(function(){

        var tr = $(this).closest('tr');
        if (-1 !== listaPlanosOrcamentarios.indexOf($(this).attr('data-id'))) {
            // -- troca o ícone da ação de seleção
            $('span.glyphicon-remove', tr)
                .removeClass('glyphicon-remove')
                .addClass('glyphicon-ok')
                .css('color', 'green');
        } else {
            // -- troca o ícone da ação de seleção
            $('span.glyphicon-ok', tr)
                .removeClass('glyphicon-ok')
                .addClass('glyphicon-remove')
                .css('color', 'grey');
        }
    });
}

$(function(){
    $('.btn-adicionar').click(function(){
        toggleItensLista();
        $('#mdl-planoorcamentario').modal();
    });

    // -- inicializando a lista de POs selecionados
    $('#adicionaisplanoorcamentario p[data-id!=""]').each(function(){
        listaPlanosOrcamentarios.push($(this).attr('data-id'));
    });
});
</script>