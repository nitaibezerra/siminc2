<?php
/* Chamada para quando for XLS, tem que ser aqui antes de montar o cabeçalho da página */
if ($_REQUEST['requisicao'] == 'exportarXLS') {
    $_REQUEST['dados']['cols-qualit'] = $_REQUEST['dados']['colunas']['qualitativo'];
    $_REQUEST['dados']['cols-quant'] = $_REQUEST['dados']['colunas']['quantitativo'];
    $resultado = montaExtratoDinamicoExtratoFolha($_REQUEST);
    $listagem = $resultado['listagem'];
    if (!$listagem->render(Simec_Listagem::SEM_REGISTROS_RETORNO)) {
        echo "
            <script>
                alert('Nenhum registro encontrado.');
                window.location = window.location.href;
            </script>
        ";
    }
    die();
}

// -- Setando o novo exercicio escolhido via seletor no cabeçalho da página
if (isset($_REQUEST ['exercicio'])) {
    $_SESSION ['exercicio'] = $_REQUEST ['exercicio'];
}

include APPRAIZ . "includes/cabecalho.inc";
$dados = $_POST['dados'];
?>
<style type="text/css">
    .marcado{background-color:#C1FFC1!important}
    .remover{display:none}
    .filtros{clear:both}
    .control-label.filtro{margin-bottom:3px}
</style>
<script type="text/javascript" language="javascript">
    $(document).ready(function () {
        $('#clear').click(function () {
            window.location.href = 'progfin.php?modulo=relatorio/custeiofolhapagamento/extratocompleto&acao=A';
        });
        /* Tem que usar e todos os "Extratores automáticos"  */
        $('#formBusca, .form-listagem').submit(function () {
            // -- Colunas qualitativas
            $('#cols_qualit_chosen .search-choice-close').each(function () {
                var index = $(this).attr('data-option-array-index');
                $('<input>').attr(
                        {'type': 'hidden', 'name': 'dados[cols-qualit][]', 'value': $('#cols-qualit option').eq(index).val()}
                ).appendTo(this);
            });
            // -- Colunas quantitativas
            $('#cols_quant_chosen .search-choice-close').each(function () {
                var index = $(this).attr('data-option-array-index');
                $('<input>').attr(
                        {'type': 'hidden', 'name': 'dados[cols-quant][]', 'value': $('#cols-quant option').eq(index).val()}
                ).appendTo(this);
            });

            if ($(this).hasClass('form-listagem')) {
                $('#requisicao').attr('value', 'mostrarHTML');
            }
        });
        $('#pesquisar').click(function () {
            $('#requisicao').attr('value', 'mostrarHTML');
            $('#pesquisar').html('Carregando...');
            $('#pesquisar').attr('disabled', 'disabled');
            $("body").prepend('<div class="ajaxCarregando"></div>');
            $(".ajaxCarregando").hide().html('Carregando, aguarde...').fadeIn();
            $('#formBusca').submit();
        });
        $('#exportar').click(function () {
            $('#requisicao').attr('value', 'exportarXLS');
            $("body").prepend('<div class="ajaxCarregando"></div>');
            $(".ajaxCarregando").hide().html('Carregando, aguarde...').fadeIn();
            $('#formBusca').submit();
        });
    });
</script>

<form name="formBusca" id="formBusca" method="POST" role="form">
    <input type="hidden" name="requisicao" id="requisicao" />
    <div class="row col-md-12">
        <ol class="breadcrumb">
            <li><a href="progfin.php?modulo=inicio&acao=C">Programação Financeira</a></li>
            <li>Extração de Custeio da Folha de Pagamento</li>
            <li class="active">Gestão de lotes</li>
        </ol>
        <div class="row col-md-12 well">
            <div class="col-md-6">
                <fieldset>
                    <legend>Colunas Qualitativas</legend>
                    <div class="form-group full">
                        <?php
                        $sql = <<<DML
                            SELECT crlexpaddgroupby AS codigo,
                                   crldsc AS descricao
                              FROM progfin.colunasextrato_fp
                              WHERE crlstatus = 'A'
                                    AND crltipo = 'QL'
                              ORDER BY crldsc
DML;
                        $options = array(
                            'titulo' => 'Selecione ao menos uma coluna',
                            'multiple' => 'multiple'
                        );

                        inputCombo('dados[colunas][qualitativo][]', $sql, null, 'cols-qualit', $options);
                        ?>
                    </div>
                    <script type="text/javascript" lang="JavaScript">
                        $(document).ready(function () {
                            $('#cols_qualit_chosen').css('display', 'none');
                            $('#cols_qualit_chosen').before('<span>Carregando...</span>');
                            $('#cols-qualit').trigger('chosen:generatelist');
<?php if (!empty($dados['cols-qualit'])): foreach ($dados['cols-qualit'] as $col): ?>
                                    $('#cols_qualit_chosen .chosen-results li').eq($('#cols-qualit option[value="<?php echo $col; ?>"]').index()).mouseup();
    <?php endforeach;
endif;
?>
                            $('#cols_qualit_chosen input').focus().blur();
                            $('#cols_qualit_chosen').css('display', 'block').prev().remove();
                        });
                    </script>
                </fieldset>
                <br />
                <fieldset>
                    <legend>Colunas Quantitativas</legend>
                    <div class="form-group full">
                        <?php
                        $sql = <<<DML
                            SELECT
                                rcc.rccnomecoluna as codigo,
                                rcc.rccdsccoluna as descricao
                            FROM
                                progfin.regrascargacusteiofolhapagamento rcc
DML;
                        inputCombo('dados[colunas][quantitativo][]', $sql, null, 'cols-quant', $options);
                        ?>
                    </div>
                    <script type="text/javascript" lang="JavaScript">
                        $(document).ready(function () {
                            $('#cols_quant_chosen').css('display', 'none');
                            $('#cols_quant_chosen').before('<span>Carregando...</span>');
                            $('#cols-quant').trigger('chosen:generatelist');
<?php if (!empty($dados['cols-quant'])): foreach ($dados['cols-quant'] as $col): ?>
                                    $('#cols_quant_chosen .chosen-results li').eq($('#cols-quant option[value="<?php echo $col; ?>"]').index()).mouseup();
    <?php endforeach;
endif;
?>
                            $('#cols_quant_chosen input').focus().blur();
                            $('#cols_quant_chosen').css('display', 'block').prev().remove();
                        });
                    </script>
                </fieldset>
            </div>
            <div class="col-md-6">
                <fieldset>
                    <legend>Filtros</legend>
                    <?php
                    $sql = <<<DML
                            SELECT clr.crlexpaddgroupby AS  crlcod,
                                   clr.crldsc,
                                   clr.crlquery
                              FROM progfin.colunasextrato_fp clr
                              WHERE crlstatus = 'A'
                                    AND crltipo = 'QL'
                                    AND clr.crlquery IS NOT NULL
                              ORDER BY crldsc
DML;
                    #ver($sql,d);
                    if ($filtros = $db->carregar($sql)) {
                        $options['titulo'] = 'Selecione um ou mais filtros';

                        foreach ($filtros as $filtro) {
                            if (!$filtro['crlquery'])
                                continue;

                            echo <<<HTML
                    <label class="control-label filtro" for="fil-{$filtro['crlcod']}">{$filtro['crldsc']}:</label>
                    <div class="form-group">
HTML;
                            inputCombo(
                                    "dados[filtros][{$filtro['crlcod']}][]", $filtro['crlquery'], $dados['filtros'][$filtro['crlcod']], "fil-{$filtro['crlcod']}", $options
                            );
                            echo <<<HTML
                    </div>
HTML;
                        }
                    }
                    ?>
                </fieldset>
            </div>
        </div>

        <div class="form-group">
            <button type="button" class="btn btn-warning" id="clear">Limpar</button>
            <button type="button" class="btn btn-primary" id="pesquisar">Pesquisar</button>
            <button type="button" class="btn btn-danger" id="exportar">Exportar XLS</button>
        </div>
    </div>
</form>
<br style="clear:both" />

<?php
$resultado = montaExtratoDinamicoExtratoFolha($_REQUEST);
$listagem = $resultado['listagem'];
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
if ($_SESSION['superuser'] == 1) {
    echo "<br/><div style=\" color:#FFF \"> {$resultado['sql']} </div><br/><br/><br/>";
}
?>


