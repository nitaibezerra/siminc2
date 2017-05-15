<?php

    require_once APPRAIZ . "www/planacomorc/_funcoes.php";
    require_once APPRAIZ . "www/planacomorc/_constantes.php";
    require_once APPRAIZ . "www/planacomorc/_funcoespi.php";

    # Filtros
    $where = '';
    $where .= $_REQUEST['estuf'] ? " AND estuf = '{$_REQUEST['estuf']}'": NULL;
    $where .= $_REQUEST['estado'] ? " AND (estuf ILIKE('%" . utf8_decode($_REQUEST['estado']) . "%') OR estdescricao ILIKE('%" . utf8_decode($_REQUEST['estado']) . "%') )": NULL;

    $sqlListaLocalizacao = "
        SELECT
            estuf,
            estuf AS uf,
            estdescricao AS nome
        FROM territorios.estado
        WHERE
            estuf IS NOT NULL
            $where
        ORDER BY
            estuf
    ";
?>
<script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
<link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
<div class="col-lg-12">
    <div class="well">
        <fieldset>
            <form id="formulario_localizacao_estadual" name="formulario_localizacao_estadual" method="post" action="" class="form-horizontal">
                <div class="form-group">
                    <label for="estado" class="col-lg-2 control-label">Nome do estado ou Distrito Federal:</label>
                    <div class="col-lg-10">
                        <input type="text" value="<?= utf8_decode($_REQUEST['estado']); ?>" class="CampoEstilo normal form-control" title="Nome do estado ou Distrito Federal" id="mundescricao" name="estado" maxlength="250" size="2">
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <input type="submit" id="btnPopupLocalizacaoEstadualPesquisar" class="btn btn-info" value="Pesquisar" name="botao" />
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
        "UF",
        "Nome"
    ));

    $listagem->setQuery($sqlListaLocalizacao);
    $listagem->addAcao('select', array('func' => 'marcarDesmarcarLocalizacaoEstadual', 'extra-params' => array('estuf')));
    $listagem->setIdLinha();
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

?>

<script>

    $(document).ready(function(){

        apresentarLinhasSelecionadasLocalizacaoEstadual();

        $('#btnPopupLocalizacaoEstadualPesquisar').click(function(e) {
            e.stopPropagation();
            var params = parametrosFiltroLocalizacaoEstadual();
            atualizaListagemLocalizacaoEstadual(params);
            return false;
        });

    });

    function apresentarLinhasSelecionadasLocalizacaoEstadual() {
        $('#table_localizacao_estadual input[name="listaLocalizacaoEstadual[]"]').each(function(index, value){
            var id = $(this).val();
            var campo = $('tr#' + id + ' td:nth-child(1)').find('span');
            campo.attr('class', 'glyphicon glyphicon-remove');
            campo.attr('title', 'Remover item');
            campo.css('color', 'gray');
        });
    }

    function parametrosFiltroLocalizacaoEstadual()
    {
        var params = {};
        $('#modal_localizacao_estadual #formulario_localizacao_estadual select').each(function() {
            var inputName = $(this).attr('name'),
            inputValue = $(this).val();
            params[inputName] = inputValue;
        });

        $('#modal_localizacao_estadual #formulario_localizacao_estadual input').each(function() {
            var inputName = $(this).attr('name'),
            inputValue = $(this).val();
            params[inputName] = inputValue;
        });
        return params;
    }

    function atualizaListagemLocalizacaoEstadual(params){
        $('#modal_localizacao_estadual .modal-body').html('');
        $.post('planacomorc.php?modulo=principal/unidade/pi-localizacao-estadual&acao=A', params, function(response) {
            $('#modal_localizacao_estadual .modal-body').html(response);
            //console.log(response);
            delegatePaginacao();
            $('#modal_localizacao_estadual .chosen-select').chosen();
            $('#modal_localizacao_estadual .chosen-container').css('width', '100%');
        });
    }

    function delegatePaginacao(){
        $('.container-listing').on('click', 'li[class="pgd-item"]:not(".disabled")', function() {
            // -- definindo a nova página
            var novaPagina = $(this).attr('data-pagina');
            var params = parametrosFiltroLocalizacaoEstadual();
            params['listagem[p]'] = novaPagina;
            atualizaListagemLocalizacaoEstadual(params);
        });
    }
    
    function marcarDesmarcarLocalizacaoEstadual(id, selecionado, adicionais) {

        if (selecionado == false) {
            // Linha nova da tabela principal de Responsáveis.
            var trHtml = '<tr style="height: 30px;" class="tr_localizacao_estadual_'+ id +'" >'
                + '                    <td style="text-align: left;">'+ $('tr#' + id + ' td:nth-child(2)').text() +'</td>'
                + '                    <td style="text-align: left;">'+ $('tr#' + id + ' td:nth-child(3)').text() +'</td>'
                + '                    <td style="text-align: center;">'
                + '                         <input type="hidden" value="'+ id +'" name="listaLocalizacaoEstadual[]">'
                + '                         <span class="glyphicon glyphicon-trash btnRemoverLocalizacaoEstadual link" data-localizacao-estadual="'+ id +'" >'
                + '                    </td>'
                + '                </tr>'
            ;
            $('#table_localizacao_estadual').append(trHtml);
        } else {
            $('.tr_localizacao_estadual_'+ id).remove();
        }
    }

</script>
<?php
    # Fechar Conexão quando não houver rodapé.
    $db->close();
    exit();
