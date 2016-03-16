<?php
/**
 * Layout de gestão de grupos.
 *
 * $Id: grupo.inc 101146 2015-08-11 21:22:58Z maykelbraz $
 */

$grupoDespesa = new Proporc_Model_Grupodespesa();
$filtrogrupos = isset($_REQUEST['filtrogrupos'])?$_REQUEST['filtrogrupos']:array();
?>
<div class="row col-md-12">
    <?php
    require_once dirname(__FILE__) . '/formFiltros.inc';
    require_once dirname(__FILE__) . '/listarGrupos.inc';
    ?>
</div>
<?php
bootstrapPopup(
    'Grupo de despesa',
    'mdl-grupodespesa',
    dirname(__FILE__) . '/formGrupodespesa.inc',
    array('cancelar', 'confirmar')
);
?>
<script type="text/javascript">
/**
 * Abri a popup mdl-grupodespesa, carregando dados, se necessário.
 *
 * @param {int} id gdpid para carregamento dos dados.
 */
function showPopup(id)
{
    if (id) {
        // -- consultar dados do id
        $.get(window.location.href, {requisicao:'consultarGrupo', 'grupodespesa[gdpid]':id}, function(data){
            for (campo in data) {
                $('#mdl-grupodespesa #' + campo).val(data[campo]);
            }
            $('#mdl-grupodespesa').modal();
        }, 'json');
        return;
    }

    // -- Limpar dados da modal antes de exibi-la
    $('#mdl-grupodespesa').modal();
}

function editarGrupo(gdpid)
{
    showPopup(gdpid);
}

function listarDespesasDoGrupo(gdpid)
{
    var url = window.location.href;
    var posicaoAba = url.indexOf('&aba=');
    if (-1 !== posicaoAba) {
        url = url.substr(0, posicaoAba);
    }
    url += '&aba=categoria&gdpid=' + gdpid;
    window.location.assign(url);
}

function apagarGrupo(gdpid)
{
    bootbox.confirm('Tem certeza que desena apagar o grupo selecionado?', function(ok){
        if (ok) {

            // -- Verifica se existe detalhamento financeiro para o grupo antes de apagar
            var data = {
                requisicao:'contarDetalhamentoGrupo',
                'grupodespesa[gdpid]':gdpid
            };

            $.get(window.location.href, data, function(data){
                if (0 == data.numFinanceiros) {
                    var $form = $('<form />', {method:'POST', action:window.location.href});
                    $('<input />', {type:'hidden', name:'grupodespesa[gdpid]', value:gdpid}).appendTo($form);
                    $('<input />', {type:'hidden', name:'requisicao', value:'apagarGrupodespesa'}).appendTo($form);
                    $form.appendTo($('body')).submit();
                    $('body').append($form);
                    $form.submit();
                } else {
                    bootbox.alert('Não é possível excluir este grupo, pois ele já possui detalhamento.');
                    return false;
                }
            }, 'json');
        }
    });
}

function onFiltrogruposNovo()
{
    showPopup();
}

$(function(){
    $('.btn-limpar').click(function(){
        window.location.assign('/proporc/proporc.php?modulo=principal/despesas/despesas&acao=A');
        console.log('as');
    });
});
</script>
