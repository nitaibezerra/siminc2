<?php
/**
 * Gestão de relatórios do SIAFI.
 *
 * @version $Id: extrato.inc 98655 2015-06-16 13:52:40Z maykelbraz $
 */

$perfis = pegaPerfilGeral();
/* Chamada para quando for XLS, tem que ser aqui antes de montar o cabeçalho da página */
if ($_REQUEST['requisicao'] == 'exportarXLS') {
    $resultado = montaExtratoDinamico($_REQUEST);
    $listagem = $resultado['listagem'];
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}
/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
$dados = $_POST['dados'];
//ver($_REQUEST);
?>
<script type="text/javascript">
$(document).ready(function() {
    $('#clear').click(function() {
        window.location.href = 'progorc.php?modulo=relatorio/limites/extrato&acao=A';
    });
    $('#pesquisar').click(function() {
        // -- Colunas qualitativas
        $('#cols_qualit_chosen .search-choice-close').each(function(){
            var index = $(this).attr('data-option-array-index');
            $('<input>').attr(
                {'type':'hidden','name':'dados[cols-qualit][]','value':$('#cols-qualit option').eq(index).val()}
            ).appendTo('form');
        });
        // -- Colunas quantitativas
        $('#cols_quant_chosen .search-choice-close').each(function(){
            var index = $(this).attr('data-option-array-index');
            $('<input>').attr(
                {'type':'hidden','name':'dados[cols-quant][]','value':$('#cols-quant option').eq(index).val()}
            ).appendTo('form');
        });

        $('#requisicao').attr('value', 'mostrarHTML');
        $('#pesquisar').html('Carregando...');
        $('#pesquisar').attr('disabled', 'disabled');
        $("body").prepend('<div class="ajaxCarregando"></div>');
        $(".ajaxCarregando").hide().html('Carregando, aguarde...').fadeIn();
        $('#formBusca').submit();
    });
    $('#exportar').click(function() {

        // -- Colunas qualitativas
        $('#cols_qualit_chosen .search-choice-close').each(function(){
            var index = $(this).attr('data-option-array-index');
            $('<input>').attr(
                {'type':'hidden','name':'dados[cols-qualit][]','value':$('#cols-qualit option').eq(index).val()}
            ).appendTo('form');
        });
        // -- Colunas quantitativas
        $('#cols_quant_chosen .search-choice-close').each(function(){
            var index = $(this).attr('data-option-array-index');
            $('<input>').attr(
                {'type':'hidden','name':'dados[cols-quant][]','value':$('#cols-quant option').eq(index).val()}
            ).appendTo('form');
        });

        $('#requisicao').attr('value', 'exportarXLS');
        $("body").prepend('<div class="ajaxCarregando"></div>');
        $(".ajaxCarregando").hide().html('Carregando, aguarde...').fadeIn();
        $('#formBusca').submit();
    });

});
</script>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Extrato dos Pedidos</li>
    </ol>
</div>
<form name="formBusca" id="formBusca" method="POST" role="form">
    <input type="hidden" name="requisicao" id="requisicao" />
    <div class="row col-md-12">
        <div class="row col-md-12 well">
            <div class="col-md-6">
                <fieldset>
                    <legend>Colunas Qualitativas</legend>
                    <div class="form-group full">
                        <?php
                        $sql = <<<DML
                            SELECT crlcod AS codigo,
                                   crldsc AS descricao
                              FROM progorc.colunasextrato
                              WHERE crlstatus = 'A'
                                    AND crltipo = 'QL'
                              ORDER BY crldsc
DML;
                        $options = array(
                            'titulo' => 'Selecione ao menos uma coluna',
                            'multiple' => 'multiple'
                        );
                        inputCombo('dados[colunas][qualitativo][]', $sql, $dados['colunas']['qualitativo'], 'cols-qualit', $options);
                        ?>
                    </div>
                    <script type="text/javascript" lang="JavaScript">
                    $(document).ready(function(){
                        $('#cols_qualit_chosen').css('display', 'none');
                        $('#cols_qualit_chosen').before('<span>Carregando...</span>');
                        $('#cols-qualit').trigger('chosen:generatelist');
                    <?php if (!empty($dados['cols-qualit'])): foreach ($dados['cols-qualit'] as $col): ?>
                        $('#cols_qualit_chosen .chosen-results li').eq($('#cols-qualit option[value="<?php echo $col; ?>"]').index()).mouseup();
                    <?php endforeach; endif; ?>
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
                           SELECT crlcod AS codigo,
                                   crldsc AS descricao
                              FROM progorc.colunasextrato
                              WHERE crlstatus = 'A'
                                    AND crltipo = 'QT'
                              ORDER BY crlid
DML;
                        inputCombo('dados[colunas][quantitativo][]', $sql, $dados['colunas']['quantitativo'], 'cols-quant', $options);
                        ?>
                    </div>
                    <script type="text/javascript" lang="JavaScript">
                    $(document).ready(function(){
                        $('#cols_quant_chosen').css('display', 'none');
                        $('#cols_quant_chosen').before('<span>Carregando...</span>');
                        $('#cols-quant').trigger('chosen:generatelist');
                    <?php if (!empty($dados['cols-quant'])): foreach ($dados['cols-quant'] as $col): ?>
                        $('#cols_quant_chosen .chosen-results li').eq($('#cols-quant option[value="<?php echo $col; ?>"]').index()).mouseup();
                    <?php endforeach; endif; ?>
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
                            SELECT clr.crlcod,
                                   clr.crldsc,
                                   clr.crlquery
                              FROM progorc.colunasextrato clr
                              WHERE crlstatus = 'A'
                                    AND crltipo = 'QL'
                                    AND clr.crlquery IS NOT NULL
                              ORDER BY crldsc
DML;
                    if ($filtros = $db->carregar($sql)) {
                        $options['titulo'] = 'Selecione um ou mais filtros';

                        foreach ($filtros as $filtro) {
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
        <br style="clear:both" />
        <?php
        $resultado = montaExtratoDinamico($_REQUEST);
        $listagem = $resultado['listagem'];
        $listagem->turnOnPesquisator();
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        if ($_SESSION['superuser'] == 1) {
            echo "<br/><div style=\" color:#FFF \"> {$resultado['sql']} </div><br/><br/><br/>";
        }
        ?>
    </div>
</form>