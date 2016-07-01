<?php
/**
 * Gestão de relatórios do SIAFI.
 *
 * @version $Id: gerarrelatorio.inc 86821 2014-09-16 18:15:12Z werteralmeida $
 */
/**
 * Classe para criação de listagens.
 *
 * @see Simec_Listagem.
 */
require (APPRAIZ . 'includes/library/simec/Listagem.php');

/**
 * Funções compartilhadas pelos sistemas SPO.
 * @see funcoesspo.php
 */
require (APPRAIZ . 'includes/funcoesspo.php');


$perfis = pegaPerfilGeral();
//if (isset($_REQUEST['aviso'])) {
//    setFlashMessage(false, $_REQUEST['aviso']);
//}

/**
 * Cabeçalho padrão do Simec.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
$dados = $_POST['dados'];
?>
<style type="text/css">
    .marcado{background-color:#C1FFC1!important}
    .remover{display:none}
    .filtros{clear:both}
    .control-label.filtro{
        margin-bottom:3px
    }
</style>
<script type="text/javascript" language="javascript">
    $(document).ready(function() {
        $.expr[':'].contains = function(a, i, m) {
            return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
        };
        $("#textFind").keyup(function()
        {
            $('table.table tbody tr td').removeClass('marcado');
            $('table.table tbody tr').removeClass('remover');
            stringPesquisa = $("#textFind").val();
            if (stringPesquisa) {
                //$('table.listagem tbody tr:contains('+stringPesquisa+')').addClass('voltar').removeClass('remover');
                $('table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                $('table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
            }
        });
        $('#pesquisar').click(function() {
            // Carregando ...

            $("body").prepend('<div class="ajaxCarregando"></div>');
            $(".ajaxCarregando").hide().html('Carregando, aguarde...').fadeIn();
            $('#pesquisar').html('Carregando ...');
            $('#pesquisar').attr('disabled', 'disabled');
            $('#formBusca').submit();
        });

    });

    /* Monta a modal de detalhe das contas  */
    function detalharContas(contas) {

        alert(contas);

    }

    /* Botão limpar */
    function reload()
    {
        window.location = 'siafi.php?modulo=principal/dadossiafi/gerarrelatorio&acao=A';
    }
</script>
<form name="formBusca" id="formBusca" method="POST" role="form">
    <div class="row col-md-12">
        <ol class="breadcrumb">
            <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li class="active">Dados SIAFI</li>
            <li class="active">Gerar Relatório</li>
        </ol>
        <div class="row col-md-12 well">

            <div class="col-md-6">
                <fieldset>
                    <legend>Colunas Qualitativas</legend>
                    <div class="form-group full">
                        <?php
                        $sql = <<<DML
                            SELECT crlcod AS codigo,
                                   crldsc AS descricao
                              FROM siafi.colunasrelatorio
                              WHERE crlstatus = 'A'
                              ORDER BY crldsc
DML;
                        $options = array(
                            'titulo' => 'Selecione ao menos uma coluna',
                            'multiple' => 'multiple'
                        );
                        inputCombo('dados[colunas][qualitativo][]', $sql, $dados['colunas']['qualitativo'], 'cols-qualit', $options);
                        ?>
                    </div>
                </fieldset>
                <br />
                <fieldset>
                    <legend>Colunas Quantitativas</legend>
                    <div class="form-group full">
                        <?php
                        $sql = <<<DML
SELECT icbcod AS codigo,
       icbdscresumida AS descricao
  FROM siafi.informacaocontabil
  WHERE icbvisualizaconta = 't'
  ORDER BY icbdscresumida
DML;
                        inputCombo('dados[colunas][quantitativo][]', $sql, $dados['colunas']['quantitativo'], 'cols-quant', $options);
                        ?>
                    </div>
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
  FROM siafi.colunasrelatorio clr
  WHERE clr.crlquery IS NOT NULL
DML;
                    if ($filtros = $db->carregar($sql)) {
                        $options['titulo'] = 'Selecione um ou mais filtros';

                        foreach ($filtros as $filtro) {
                            echo <<<HTML
                    <label class="control-label filtro" for="fil-{$filtro['crlcod']}">{$filtro['crldsc']}:</label>
                    <div class="form-group">
HTML;
                            $sqlFiltro = str_replace('2014', $_SESSION['exercicio'], $filtro['crlquery']);
                            inputCombo(
                                    "dados[filtros][{$filtro['crlcod']}][]", $sqlFiltro, $dados['filtros'][$filtro['crlcod']], "fil-{$filtro['crlcod']}", $options
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

            <button class="btn btn-warning" type="reset" onclick="reload()">Limpar</button>
            <button class="btn btn-primary" id="pesquisar" type="button">Buscar</button>
            <button class="btn btn-danger" id="xls" type="button">Exportar XLS</button>
        </div>
        <br style="clear:both" />

        <label for=textFind title="Pesquisa apenas os dados que já estão sendo exibidos na tabela!">Pesquisa rápida: </label>
        <input class="normal form-control" type="text" id="textFind" title="Pesquisa apenas os dados que já estão sendo exibidos na tabela!">
        <br style="clear:both" />
        <?php
        $resultado = montaRelatorio($_REQUEST);
        $listagem = $resultado['listagem'];
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        if ($_SESSION['superuser'] == 1) {
            echo "<br/><div style=\" color:#FFF \"> {$resultado['sql']} </div><br/><br/><br/>";
        }
        ?>
    </div>
</form>