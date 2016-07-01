<?php
/**
 * 
 */

/**
 * Funções de apoio às solicitações recurso.
 * @see _funcoessolicitacoesrecursos.php
 */
require(APPRAIZ . 'www/progfin/_funcoessolicitacoesrecursos.php');

$whereUO = '';
// -- Where UO
if (in_array(PFL_UO_EQUIPE_TECNICA, pegaPerfilGeral($_SESSION['usucpf']))) {
    $whereUO = <<<SQL
      AND EXISTS (SELECT 1
                    FROM progfin.usuarioresponsabilidade rpu
                    WHERE rpu.unicod = uni.unicod
                      AND rpu.usucpf = '%s'
                      AND rpu.pflcod = %d
                      AND rpu.rpustatus = 'A')
SQL;

    $whereUO = sprintf($whereUO, $_SESSION['usucpf'], PFL_UO_EQUIPE_TECNICA);
}

/* Chamada para quando for XLS, tem que ser aqui antes de montar o cabeçalho da página */
if ($_REQUEST['requisicao'] == 'exportarXLS') {
    $resultado = montaExtratoDinamicoLiberacoesFinanceirasProgfin($_REQUEST, $whereUO);
    $listagem = $resultado['listagem'];
    if(!$listagem->render(Simec_Listagem::SEM_REGISTROS_RETORNO)){
        echo "
            <script>
                alert('Nenhum registro encontrado.');
                window.location = window.location.href;
            </script>
        ";
    }
    die();
}

/*
 * Cabeçalho padrão SIMEC
 */
include APPRAIZ . "includes/cabecalho.inc";
$dados = $_POST['dados'];
?>
<script type="text/javascript" language="javascript">
    $(document).ready(function() {
        $('#clear').click(function() {
            window.location.href = 'progfin.php?modulo=principal/solicitacaorecurso/extrato&acao=A';
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

<form name="formBusca" id="formBusca" method="POST" role="form">
    <input type="hidden" name="requisicao" id="requisicao" />
    <div class="row col-md-12">
        <ol class="breadcrumb">
            <li><a href="progfin.php?modulo=inicio&acao=C">Programação Financeira</a></li>
            <li>Liberações Financeiras</li>
            <li class="active">Extrato de Solicitação de Recursos Financeiros</li>
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
                              FROM progfin.colunasextrato_lf
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
                           SELECT crlexpaddgroupby as codigo,
                                   crldsc AS descricao
                              FROM progfin.colunasextrato_lf
                              WHERE crlstatus = 'A'
                                    AND crltipo = 'QT'
                              ORDER BY crldsc
DML;
                        inputCombo('dados[colunas][quantitativo][]', $sql, null, 'cols-quant', $options);
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
                            SELECT clr.crlexpaddgroupby as crlcod,
                                   clr.crldsc,
                                   clr.crlquery
                              FROM progfin.colunasextrato_lf clr
                              WHERE crlstatus = 'A'
                                    AND crltipo = 'QL'
                                    AND clr.crlquery IS NOT NULL
                              ORDER BY crldsc
DML;
                    if ($filtros = $db->carregar($sql)) {
                        $options['titulo'] = 'Selecione um ou mais filtros';

                        foreach ($filtros as $filtro) {
                            if(!$filtro['crlquery'])
                                continue;

                            $queryFiltro = $filtro['crlquery'];
                            if (preg_match_all('/:[a-zA-z]+/', $queryFiltro, $matches, PREG_PATTERN_ORDER)
                                && is_array($matches)
                                && !empty($matches[0])
                            ) {
                                foreach ($matches[0] as $match) {
                                    $param = substr($match, 1);
                                    $queryFiltro = str_replace($match, $$param, $queryFiltro);
                                }
                            }

                            echo <<<HTML
                    <label class="control-label filtro" for="fil-{$filtro['crlcod']}">{$filtro['crldsc']}:</label>
                    <div class="form-group">
HTML;

                            inputCombo(
                                "dados[filtros][{$filtro['crlcod']}][]", $queryFiltro, $dados['filtros'][$filtro['crlcod']], "fil-{$filtro['crlcod']}", $options
                            );
                            echo <<<HTML
                    </div>
HTML;
                        }
                    }
                    ?>
                    <label class="control-label filtro" for="filtipopedido">Tipo de solicitação:</label>
                    <div class="form-group">
                        <select id="filtipopedido" name="dados[filtros][filtipopedido]" class="CampoEstilo form-control">
                            <option value="T" <?php echo 'T' == $dados['filtros']['filtipopedido']?'selected':'' ?>>Todos</option>
                            <option value="L" <?php echo 'L' == $dados['filtros']['filtipopedido']?'selected':'' ?>>Lote</option>
                            <option value="I" <?php echo 'I' == $dados['filtros']['filtipopedido']?'selected':'' ?>>Individuais</option>
                        </select>
                    </div>
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
<div>
      <?php
        $resultado = montaExtratoDinamicoLiberacoesFinanceirasProgfin($_REQUEST, $whereUO);
        $listagem = $resultado['listagem'];
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

        if ($_SESSION['superuser'] == 1) {
            echo "<br/><div style=\" color:#FFF \"> {$resultado['sql']} </div><br/><br/><br/>";
        }
        ?>
</div>
<script type="text/javascript" lang="javascript">
$(function(){
    $('#filtipopedido').chosen();
});
</script>