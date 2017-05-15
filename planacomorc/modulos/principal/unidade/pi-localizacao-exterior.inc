<?php

    require_once APPRAIZ . "www/planacomorc/_funcoes.php";
    require_once APPRAIZ . "www/planacomorc/_constantes.php";
    require_once APPRAIZ . "www/planacomorc/_funcoespi.php";

    # Filtros
    $where = '';
    $where .= $_REQUEST['pais'] ? " AND paidescricao ILIKE('%" . utf8_decode($_REQUEST['pais']) . "%')": NULL;

    $sqlListaLocalizacao = "
        SELECT
            paiid,
            paidescricao
        FROM territorios.pais
        WHERE
            paidescricao IS NOT NULL
            $where
        ORDER BY
            paidescricao
    ";
?>
<script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
<link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
<div class="col-lg-12">
    <div class="well">
        <fieldset>
            <form id="formulario_localizacao_exterior" name="formulario_localizacao_exterior" method="post" action="" class="form-horizontal">
                <div class="form-group">
                    <label for="pais" class="col-lg-2 control-label">Nome do país:</label>
                    <div class="col-lg-10">
                        <input type="text" value="<?= utf8_decode($_REQUEST['pais']); ?>" class="CampoEstilo normal form-control" title="Nome do país" id="pais" name="pais" maxlength="250" size="2">
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <input type="submit" id="btnPopupLocalizacaoExteriorPesquisar" class="btn btn-info" value="Pesquisar" name="botao" />
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
</div>
<br>
<?php
    $listagem = new Simec_Listagem();
    $listagem->setCabecalho(array(
        "País"
    ));

    $listagem->setQuery($sqlListaLocalizacao);
    $listagem->addAcao('select', array('func' => 'marcarDesmarcarLocalizacaoExterior', 'extra-params' => array('paiid')));
    $listagem->setIdLinha();
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

?>

<script>

    $(document).ready(function(){

        apresentarLinhasSelecionadasLocalizacaoExterior();

        $('#btnPopupLocalizacaoExteriorPesquisar').click(function(e) {
            e.stopPropagation();
            var params = parametrosFiltroLocalizacaoExterior();
            atualizaListagemLocalizacaoExterior(params);
            return false;
        });

    });

    function apresentarLinhasSelecionadasLocalizacaoExterior() {
        $('#table_localizacao_exterior input[name="listaLocalizacaoExterior[]"]').each(function(index, value){
            var id = $(this).val();
            var campo = $('tr#' + id + ' td:nth-child(1)').find('span');
            campo.attr('class', 'glyphicon glyphicon-remove');
            campo.attr('title', 'Remover item');
            campo.css('color', 'gray');
        });
    }

    function parametrosFiltroLocalizacaoExterior()
    {
        var params = {};
        $('#modal_localizacao_exterior #formulario_localizacao_exterior select').each(function() {
            var inputName = $(this).attr('name'),
            inputValue = $(this).val();
            params[inputName] = inputValue;
        });

        $('#modal_localizacao_exterior #formulario_localizacao_exterior input').each(function() {
            var inputName = $(this).attr('name'),
            inputValue = $(this).val();
            params[inputName] = inputValue;
        });
        return params;
    }

    function atualizaListagemLocalizacaoExterior(params){
        $('#modal_localizacao_exterior .modal-body').html('');
        $.post('planacomorc.php?modulo=principal/unidade/pi-localizacao-exterior&acao=A', params, function(response) {
            $('#modal_localizacao_exterior .modal-body').html(response);
            //console.log(response);
            delegatePaginacao();
            $('#modal_localizacao_exterior .chosen-select').chosen();
            $('#modal_localizacao_exterior .chosen-container').css('width', '100%');
        });
    }

    function delegatePaginacao(){
        $('.container-listing').on('click', 'li[class="pgd-item"]:not(".disabled")', function() {
            // -- definindo a nova página
            var novaPagina = $(this).attr('data-pagina');
            var params = parametrosFiltroLocalizacaoExterior();
            params['listagem[p]'] = novaPagina;
            atualizaListagemLocalizacaoExterior(params);
        });
    }
    
    function marcarDesmarcarLocalizacaoExterior(id, selecionado, adicionais) {

        if (selecionado == false) {
            // Linha nova da tabela principal de Responsáveis.
            var trHtml = '<tr style="height: 30px;" class="tr_localizacao_exterior_'+ id +'" >'
                + '                    <td style="text-align: left;">'+ $('tr#' + id + ' td:nth-child(2)').text() +'</td>'
                + '                    <td style="text-align: center;">'
                + '                         <input type="hidden" value="'+ id +'" name="listaLocalizacaoExterior[]">'
                + '                         <span class="glyphicon glyphicon-trash btnRemoverLocalizacaoExterior link" data-localizacao-exterior="'+ id +'" >'
                + '                    </td>'
                + '                </tr>'
            ;
            $('#table_localizacao_exterior').append(trHtml);
        } else {
            $('.tr_localizacao_exterior_'+ id).remove();
        }
    }

</script>
<?php
    # Fechar Conexão quando não houver rodapé.
    $db->close();
    exit();
