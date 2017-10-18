<?php

    require_once APPRAIZ . "www/planacomorc/_funcoes.php";
    require_once APPRAIZ . "www/planacomorc/_constantes.php";
    require_once APPRAIZ . "www/planacomorc/_funcoespi.php";

    # Filtros
    $where = '';
    $where .= $_REQUEST['ungcod'] ? " AND rpu.ungcod = '{$_REQUEST['ungcod']}'": NULL;
    $where .= $_REQUEST['nome'] ? " AND u.usunome ILIKE('%" . utf8_decode($_REQUEST['nome']) . "%')": NULL;
    $where .= $_REQUEST['email'] ? " AND u.usuemail ILIKE('%" . $_REQUEST['email'] . "%')": NULL;

    $sqlListaUsuarios = "
        SELECT DISTINCT
            rpu.usucpf,
            u.usunome,
            (CASE WHEN LENGTH(u.usufoneddd) > 0 THEN '(' || u.usufoneddd || ')' ELSE '' END) || u.usufonenum,
            u.usuemail
        FROM planacomorc.usuarioresponsabilidade rpu
            JOIN seguranca.usuario u ON rpu.usucpf = u.usucpf-- SELECT * FROM seguranca.usuario
        WHERE
            rpu.rpustatus = 'A'
            $where
        ORDER BY
            u.usunome
    ";
?>
<script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
<link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
<div class="col-lg-12">
    <div class="well">
        <fieldset>
            <form id="formulario_responsaveis" name="formulario_responsaveis" method="post" action="" class="form-horizontal">
                <input type="hidden" name="ungcod" value="<?= $_REQUEST['ungcod']; ?>">
                <div class="form-group">
                    <label for="nome" class="col-lg-2 control-label">Nome:</label>
                    <div class="col-lg-10">
                        <input type="text" value="<?= utf8_decode($_REQUEST['nome']); ?>" class="CampoEstilo normal form-control" title="Nome" id="nome" name="nome" maxlength="250" size="2">
                    </div>
                </div>
                <div class="form-group">
                    <label for="email" class="col-lg-2 control-label">E-mail:</label>
                    <div class="col-lg-10">
                        <input type="text" value="<?= $_REQUEST['email']; ?>" class="CampoEstilo normal form-control" title="E-mail" id="email" name="email" maxlength="250" size="2">
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-2">
                        <input type="submit" id="btnPopupResponsaveisPesquisar" class="btn btn-info" value="Pesquisar" name="botao" />
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
        "Nome",
        "Telefone",
        "E-mail"
    ));

    $listagem->setQuery($sqlListaUsuarios);
    $listagem->addAcao('select', array('func' => 'marcarDesmarcar', 'extra-params' => array('usucpf')));
    $listagem->setIdLinha();
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
<script>

    $(document).ready(function(){
        apresentarLinhasSelecionadasResponsaveis();

        $('#btnPopupResponsaveisPesquisar').click(function(e) {
            e.stopPropagation();
            var params = parametrosFiltroResponsaveis();
            atualizaListagemResponsaveis(params);
            return false;
        });

    });

    function apresentarLinhasSelecionadasResponsaveis() {
        $('#table_responsaveis input[name="listaResponsaveis[]"]').each(function(index, value){
            var id = $(this).val();
            var campo = $('tr#' + id + ' td:nth-child(1)').find('span');
            campo.attr('class', 'glyphicon glyphicon-remove');
            campo.attr('title', 'Remover item');
            campo.css('color', 'gray');
        });
    }

    function parametrosFiltroResponsaveis()
    {
        var params = {};
        $('#modal_responsaveis #formulario_responsaveis select').each(function() {
            var inputName = $(this).attr('name'),
            inputValue = $(this).val();
            params[inputName] = inputValue;
        });

        $('#modal_responsaveis #formulario_responsaveis input').each(function() {
            var inputName = $(this).attr('name'),
            inputValue = $(this).val();
            params[inputName] = inputValue;
        });
        return params;
    }

    function atualizaListagemResponsaveis(params){
        $('#modal_responsaveis .modal-body').html('');
        $.post('planacomorc.php?modulo=principal/unidade/pi-responsaveis&acao=A', params, function(response) {
            $('#modal_responsaveis .modal-body').html(response);
            //console.log(response);
            delegatePaginacao();
            $('#modal_responsaveis .chosen-select').chosen();
            $('#modal_responsaveis .chosen-container').css('width', '100%');
        });
    }

    function delegatePaginacao(){
        $('.container-listing').on('click', 'li[class="pgd-item"]:not(".disabled")', function() {
            // -- definindo a nova página
            var novaPagina = $(this).attr('data-pagina');
            var params = parametrosFiltroResponsaveis();
            params['listagem[p]'] = novaPagina;
            atualizaListagemResponsaveis(params);
        });
    }

    function marcarDesmarcar(id, selecionado, adicionais) {

        if (selecionado == false) {

            // Linha nova da tabela principal de Responsáveis.
            var trHtml = '<tr style="height: 30px;" class="tr_responsaveis_'+ id +'" >'
                + '                    <td style="text-align: left;">'+ $('tr#' + id + ' td:nth-child(2)').text() +'</td>'
                + '                    <td style="text-align: left;">'+ $('tr#' + id + ' td:nth-child(3)').text() +'</td>'
                + '                    <td style="text-align: left;">'+ $('tr#' + id + ' td:nth-child(4)').text() +'</td>'
                + '                    <td style="text-align: center;">'
                + '                         <input type="hidden" value="'+ id +'" name="listaResponsaveis[]">'
                + '                         <span class="glyphicon glyphicon-trash btnRemoverResponsaveis link" data-responsaveis="'+ id +'" >'
                + '                    </td>'
                + '                </tr>'
            ;
            $('#table_responsaveis').append(trHtml);

        } else {
            $('.tr_responsaveis_'+ id).remove();
        }
    }

</script>

<?php
/* Fechar Conexão quando não houver rodapé. */
$db->close();
exit();
