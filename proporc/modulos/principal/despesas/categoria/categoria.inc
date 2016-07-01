<?php
/**
 * Layout de gestão de categorias de despesa.
 * $Id: categoria.inc 101146 2015-08-11 21:22:58Z maykelbraz $
 */

$catDespesa = new Proporc_Model_Despesa();
$grpDespesa = new Proporc_Model_Grupodespesa();
$filtrocategorias = isset($_REQUEST['filtrocategorias'])?$_REQUEST['filtrocategorias']:array();
$filtrocategorias = array_filter($filtrocategorias, 'strlen');
?>
<div class="row col-md-12">
    <?php
    require_once dirname(__FILE__) . '/formFiltros.inc';
    require_once dirname(__FILE__) . '/listarDespesas.inc';
    ?>
</div>
<?php
bootstrapPopup(
    'Coluna de despesa',
    'mdl-categoriadespesa',
    dirname(__FILE__) . '/formDespesa.inc',
    array('cancelar', 'confirmar'),
    array('tamanho' => 'lg')
);
?>
<script type="text/javascript">
/**
 * Abri a popup mdl-categoriadespesa, carregando dados, se necessário.
 *
 * @param {int} id dspid para carregamento dos dados.
 */
function showPopup(id)
{
    $('#categoriadespesa_gdpid').val(
        $('#gdpid-filtro').val()
    ).trigger('chosen:updated');

    $('#categoriadespesa_dspid').val('');
    $('#categoriadespesa_dspnome').val('');
    $('#categoriadespesa_dspsigla').val('');
    $('#categoriadespesa_vlrmontante').val('0.00');
    $('#categoriadespesa_foncod').val([]).trigger('chosen:updated');
    $('#categoriadespesa_gndcod').val([]).trigger('chosen:updated');
    $('#categoriadespesa_dspobservacao').val('');

    if (id) {
        // -- consultar dados do id
        $.get(window.location.href, {requisicao:'consultarCategoria', 'categoriadespesa[dspid]':id}, function(data){
            for (campo in data) {
                $field = $('#mdl-categoriadespesa #categoriadespesa_' + campo);
                $field.val(data[campo]);
                if ($field.hasClass('chosen-select') || $field.hasClass('chosen-select-no-single')) {
                    $field.trigger('chosen:updated');
                }
                $('#mdl-categoriadespesa #categoriadespesa_vlrmontante').blur();
            }
            $('#mdl-categoriadespesa').modal();
        }, 'json');
        return;
    }

    // -- Limpar dados da modal antes de exibi-la
    $('#mdl-categoriadespesa').modal();
}

function editarCategoria(dspid)
{
    showPopup(dspid);
}

function apagarCategoria(dspid)
{
    bootbox.confirm('Tem certeza que deseja apagar a categoria selecionada?', function(ok){
        if (ok) {

            // -- Verifica se existe detalhamento financeiro para a coluna antes de apagar
            var data = {
                requisicao:'contarDetalhamentoCategoria',
                'categoriadespesa[dspid]':dspid
            };
            $.get(window.location.href, data, function(data){
                if (0 == data.numFinanceiros) {
                    var $form = $('<form />', {method:'POST', action:window.location.href});
                    $('<input />', {type:'hidden', name:'categoriadespesa[dspid]', value:dspid}).appendTo($form);
                    $('<input />', {type:'hidden', name:'requisicao', value:'apagarCategoriadespesa'}).appendTo($form);
                    $form.appendTo($('body')).submit();
                    $('body').append($form);
                    $form.submit();
                } else {
                    bootbox.alert('Não é possível excluir esta coluna, pois ela já possui detalhamento.');
                    return false;
                }
            }, 'json');
        }
    });
}

function editarFgu(dspid)
{
    var url = window.location.href;
    var posicaoAba = url.indexOf('&aba=');
    if (-1 !== posicaoAba) {
        url = url.substr(0, posicaoAba);
    }
    url += '&aba=fgu&dspid=' + dspid;
    window.location.assign(url);
}

function onFiltrocategoriasNovo()
{
    $('#gdpid').val(
        $('#gdpid-filtro').val()
    ).trigger('chosen:updated');
    showPopup();
}

$(function(){
    $('.btn-limpar').click(function(){
        window.location.assign('/proporc/proporc.php?modulo=principal/despesas/despesas&acao=A&aba=categoria');
        console.log('as');
    });

    $('#categoriadespesa').submit(function(){
        var msg = '';

        if ('' === $('#categoriadespesa_gdpid').val()) {
            msg += 'O campo "Grupo de despesa" não pode ser deixado em branco.<br />';
        }
        if ('' === $('#categoriadespesa_dspnome').val()) {
            msg += 'O campo "Nome" não pode ser deixado em branco.<br />';
        }
        if ('' === $('#categoriadespesa_foncod').val()) {
            msg += 'O campo "Fonte" não pode ser deixado em branco.<br />';
        }
        if ('' === $('#categoriadespesa_gndcod').val()) {
            msg += 'O campo "GND" não pode ser deixado em branco.<br />';
        }

        if ('' === msg) {
            return true;
        }

        alert(msg);
        return false;
    });
});
</script>