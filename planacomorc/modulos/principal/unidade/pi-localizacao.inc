<?php

    require_once APPRAIZ . "www/planacomorc/_funcoes.php";
    require_once APPRAIZ . "www/planacomorc/_constantes.php";
    require_once APPRAIZ . "www/planacomorc/_funcoespi.php";

    # Filtros
    $where = '';
    $where .= $_REQUEST['estuf'] ? " AND estuf = '{$_REQUEST['estuf']}'": NULL;
    $where .= $_REQUEST['mundescricao'] ? " AND mundescricao ILIKE('%" . utf8_decode($_REQUEST['mundescricao']) . "%')": NULL;

    $sqlListaLocalizacao = "
        SELECT DISTINCT
            muncod,
            estuf,
            mundescricao
        FROM territorios.municipio
        WHERE
            LENGTH(estuf) > 0
            $where
        ORDER BY
            mundescricao
    ";
?>
<script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
<link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
<div class="col-lg-12">
    <div class="well">
        <fieldset>
            <form id="formulario_localizacao" name="formulario_localizacao" method="post" action="" class="form-horizontal">
                <div class="form-group">
                    <label for="nome" class="col-lg-2 control-label">UF:</label>
                    <div class="col-lg-10">
                        <?php
                            $sql = "
                                SELECT
                                    estuf AS codigo,
                                    estdescricao AS descricao
                                FROM territorios.estado
                                ORDER BY
                                    estdescricao
                            ";
                            $db->monta_combo('estuf', $sql, 'S', 'Selecione', null, null, null, 400, 'N', 'estuf', null, (isset($_REQUEST['estuf']) ? $_REQUEST['estuf'] : null), null, 'class="form-control chosen-select" style="width=100%;""', null, null);
                        ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="mundescricao" class="col-lg-2 control-label">Municípios:</label>
                    <div class="col-lg-10">
                        <input type="text" value="<?= utf8_decode($_REQUEST['mundescricao']); ?>" class="CampoEstilo normal form-control" title="Municípios" id="mundescricao" name="mundescricao" maxlength="250" size="2">
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <input type="submit" id="btnPopupLocalizacaoPesquisar" class="btn btn-info" value="Pesquisar" name="botao" />
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
        "Município"
    ));

    $listagem->setQuery($sqlListaLocalizacao);
    $listagem->addAcao('select', array('func' => 'marcarDesmarcarLocalizacao', 'extra-params' => array('muncod')));
    $listagem->setIdLinha();
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

?>

<script>

    $(document).ready(function(){

        apresentarLinhasSelecionadasLocalizacao();

        $('#btnPopupLocalizacaoPesquisar').click(function(e) {
            e.stopPropagation();
            var params = parametrosFiltroLocalizacao();
            atualizaListagemLocalizacao(params);
            return false;
        });

    });

    function apresentarLinhasSelecionadasLocalizacao() {
        $('#table_localizacao input[name="listaLocalizacao[]"]').each(function(index, value){
            var id = $(this).val();
            var campo = $('tr#' + id + ' td:nth-child(1)').find('span');
            campo.attr('class', 'glyphicon glyphicon-remove');
            campo.attr('title', 'Remover item');
            campo.css('color', 'gray');
        });
    }

    function parametrosFiltroLocalizacao()
    {
        var params = {};
        $('#modal_localizacao #formulario_localizacao select').each(function() {
            var inputName = $(this).attr('name'),
            inputValue = $(this).val();
            params[inputName] = inputValue;
        });

        $('#modal_localizacao #formulario_localizacao input').each(function() {
            var inputName = $(this).attr('name'),
            inputValue = $(this).val();
            params[inputName] = inputValue;
        });
        return params;
    }

    function atualizaListagemLocalizacao(params){
        $('#modal_localizacao .modal-body').html('');
        $.post('planacomorc.php?modulo=principal/unidade/pi-localizacao&acao=A', params, function(response) {
            $('#modal_localizacao .modal-body').html(response);
            //console.log(response);
            delegatePaginacao();
            $('#modal_localizacao .chosen-select').chosen();
            $('#modal_localizacao .chosen-container').css('width', '100%');
        });
    }

    function delegatePaginacao(){
        $('.container-listing').on('click', 'li[class="pgd-item"]:not(".disabled")', function() {
            // -- definindo a nova página
            var novaPagina = $(this).attr('data-pagina');
            var params = parametrosFiltroLocalizacao();
            params['listagem[p]'] = novaPagina;
            atualizaListagemLocalizacao(params);
        });
    }
    
    function marcarDesmarcarLocalizacao(id, selecionado, adicionais) {

        if (selecionado == false) {
            // Linha nova da tabela principal de Responsáveis.
            var trHtml = '<tr style="height: 30px;" class="tr_localizacao_'+ id +'" >'
                + '                    <td style="text-align: left;">'+ $('tr#' + id + ' td:nth-child(2)').text() +'</td>'
                + '                    <td style="text-align: left;">'+ $('tr#' + id + ' td:nth-child(3)').text() +'</td>'
                + '                    <td style="text-align: center;">'
                + '                         <input type="hidden" value="'+ id +'" name="listaLocalizacao[]">'
                + '                         <span class="glyphicon glyphicon-trash btnRemoverLocalizacao link" data-localizacao="'+ id +'" >'
                + '                    </td>'
                + '                </tr>'
            ;
            $('#table_localizacao').append(trHtml);
        } else {
            $('.tr_localizacao_'+ id).remove();
        }
    }

</script>
<?php
    # Fechar Conexão quando não houver rodapé.
    $db->close();
    exit();
