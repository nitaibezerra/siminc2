<?php
/**
 * Gestão de relatórios do SIAFI.
 *
 * @version $Id: extrato.inc 89715 2014-11-05 13:14:22Z werteralmeida $
 */

$perfis = pegaPerfilGeral();
$fm = new Simec_Helper_FlashMessage('mensagemGravarConsulta');
#ver($_REQUEST);
/* o relatório vier do salvar relatório  */
if (isset($resultado['cexid']) || (isset($_REQUEST['cexid']) && $_REQUEST['cexid']!='')) {
    $cexid = $resultado['cexid']?$resultado['cexid']:$_REQUEST['cexid'];
    $dadosConsultaCarregada = $db->carregar("
        SELECT
            cex.cexid         ,
            cex.cexnome       ,
            cex.cexconteudo   ,
            cex.cexpublico
       FROM
            siafi.consultaextratosiop cex
       WHERE
            cexid = {$cexid}");
    $dadosConsultaCarregada = $dadosConsultaCarregada[0];
    $dadosFormulario = json_decode($dadosConsultaCarregada['cexconteudo'],true);
    #ver($dadosConsultaCarregada['cexconteudo'],d);
    $_REQUEST['requisicao'] = '';
    $_REQUEST['dados']['colunas']['qualitativo'] = array();
    $_REQUEST['dados']['colunas']['quantitativo'] = array();
    $_REQUEST['dados']['filtros'] = array();
    foreach ($dadosFormulario as $key => $value) {
        if($value['name']=='dados[colunas][qualitativo][]'){
            array_push($_REQUEST['dados']['colunas']['qualitativo'], $value['value']) ;
        }
         if($value['name']=='dados[colunas][quantitativo][]'){
            array_push($_REQUEST['dados']['colunas']['quantitativo'], $value['value']) ;
        }
    }
    foreach ($dadosFormulario as $key2 => $value2) {
        if (stristr($value2['name'], 'dados[filtros]')) {
            $nomeFiltro = str_replace('dados[filtros]','',$value2['name']);
            $nomeFiltro = str_replace(array('[',']'),'',$nomeFiltro);
            $_REQUEST['dados']['filtros'][$nomeFiltro][$key2] = $value2['value'];
        }
    }
        
    
    $_REQUEST['dados']['cols-qualit'] = $_REQUEST['dados']['colunas']['qualitativo'];
    $_REQUEST['dados']['cols-quant'] = $_REQUEST['dados']['colunas']['quantitativo'];
    
    #ver($_REQUEST);   #die();
    #ver(array_search('acacod' , $dadosFormulario), d);
}
/* Recebe o resultado da consulta */
$resultado = montaExtratoDinamicoSiop($_REQUEST);

/* Chamada para quando for XLS, tem que ser aqui antes de montar o cabeçalho da página */
if ($_REQUEST['requisicao'] == 'exportarXLS') {
    $resultado = montaExtratoDinamico($_REQUEST);
    $listagem = $resultado['listagem'];
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    die();
}

if (($_GET['action'] == 'apagar') && isset($_GET['cexid']) && is_numeric($_GET['cexid'])) {
    $db->executar("delete from siafi.consultaextratosiop where cexid = ".(int) $_GET['cexid']);
    if ($db->commit()) {
        echo simec_json_encode(array('result' => 'redirect'));
    } else {
        echo simec_json_encode(array('result' => 'fail'));
    }
    die;
}

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . 'includes/cabecalho.inc';
$dados = $_REQUEST['dados'];
?>
<style type="text/css">
    .filtros{clear:both}
    .control-label.filtro{margin-bottom:3px}
    .divNomeConsultaGravada{
        color: Green;
        float: right;
        font-size: 12px;
        font-weight: bold;
        text-align: right;
        width: 60%;
    }
</style>
<script type="text/javascript" language="javascript">
// -- Não apagar esta função
    function delegateFormFiltroPadrao() {
    }

    $(document).ready(function() {
       
        $('#clear').click(function() {
            window.location = 'siafi.php?modulo=principal/dadossiop/gerarrelatorio&acao=A';
        });
        $('#pesquisar').click(function() {
            $('#requisicao').attr('value', 'mostrarHTML');
            $('#pesquisar').html('Carregando...');
            $('#pesquisar').attr('disabled', 'disabled');
            $("body").prepend('<div class="ajaxCarregando"></div>');
            $(".ajaxCarregando").hide().html('Carregando, aguarde...').fadeIn();
            $('#formBusca').submit();
        });

        $('#exportar').click(function() {
            $('#requisicao').attr('value', 'exportarXLS');
            $('#formBusca').submit();
        });

        $('form.form-listagem').submit(function() {
            // -- Colunas qualitativas
            $('#cols_qualit_chosen .search-choice-close').each(function() {
                var index = $(this).attr('data-option-array-index');
                $('<input>').attr(
                        {'type': 'hidden', 'name': 'dados[cols-qualit][]', 'value': $('#cols-qualit option').eq(index).val()}
                ).appendTo('form.form-listagem');
            });
            // -- Colunas quantitativas
            $('#cols_quant_chosen .search-choice-close').each(function() {
                var index = $(this).attr('data-option-array-index');
                $('<input>').attr(
                        {'type': 'hidden', 'name': 'dados[cols-quant][]', 'value': $('#cols-quant option').eq(index).val()}
                ).appendTo('form.form-listagem');
            });
        });
    });
</script>
<form name="formBusca" id="formBusca" method="POST" role="form" class="form-listagem">
    <input type="hidden" name="requisicao" id="requisicao" />
    <div class="row col-md-12">
        <ol class="breadcrumb">
            <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li class="active">Dados do SIOP</li>
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
                              FROM siafi.colunasextratosiop
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
                        $(document).ready(function() {
                            $('#cols_qualit_chosen').css('display', 'none');
                            $('#cols_qualit_chosen').before('<span>Carregando...</span>');
                            $('#cols-qualit').trigger('chosen:generatelist');
                            <?php if (!empty($dados['cols-qualit'])):
                                foreach ($dados['cols-qualit'] as $col): ?>
                                    $('#cols_qualit_chosen .chosen-results li').eq($('#cols-qualit option[value="<?php echo $col; ?>"]').index()).mouseup();
                                <?php
                                endforeach;
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
                           SELECT crlcod AS codigo,
                                   crldsc AS descricao
                              FROM siafi.colunasextratosiop
                              WHERE crlstatus = 'A'
                                    AND crltipo = 'QT'
                              ORDER BY crldsc
DML;
                        inputCombo('dados[colunas][quantitativo][]', $sql, null, 'cols-quant', $options);
                        ?>
                    </div>
                    <script type="text/javascript" lang="JavaScript">
                        $(document).ready(function() {
                            $('#cols_quant_chosen').css('display', 'none');
                            $('#cols_quant_chosen').before('<span>Carregando...</span>');
                            $('#cols-quant').trigger('chosen:generatelist');
<?php if (!empty($dados['cols-quant'])): foreach ($dados['cols-quant'] as $col): ?>
                                    $('#cols_quant_chosen .chosen-results li').eq($('#cols-quant option[value="<?php echo $col; ?>"]').index()).mouseup();
        <?php
    endforeach;
endif;
?>
                            $('#cols_quant_chosen input').focus().blur();
                            $('#cols_quant_chosen').css('display', 'block').prev().remove();
                        });
                    </script>
                </fieldset>
                <fieldset>
                    <legend>Consultas Gravadas <div class="divNomeConsultaGravada"><?php echo $dadosConsultaCarregada['cexnome']; ?></div></legend>
                    <div class="form-group full"
                         style="
                         height: 180px;
                         overflow: auto;">
                         <?php
                         $sql = <<<DML
                                SELECT
                                 cex.cexid       ,
                                 cex.cexnome     ,
                                 usu.usunome      ,
                                 TO_CHAR( cex.dataalteracao , 'DD/MM/YYYY HH:MI' ) AS dataalteracao
                                FROM
                                 siafi.consultaextratosiop cex
                                INNER JOIN seguranca.usuario usu USING(usucpf)
                                WHERE
                                    cex.usucpf = '{$_SESSION['ususcpf']}'
                                    OR cex.cexpublico
                                ORDER BY
                                 cex.dataalteracao DESC
DML;
                         $listagem = new Simec_Listagem();
                         $cabecalho = array('Consulta', 'Criador', 'Última Alteração');
                         $listagem->addAcao('edit2', 'executarRelatorio');
                         $listagem->addAcao('delete', 'apagarRelatorio');
                         $listagem->addCallbackDeCampo('cexnome', 'alinhaParaEsquerda');
                         $listagem->setCabecalho($cabecalho);
                         $listagem->setQuery($sql);
                         $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
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
                              FROM siafi.colunasextratosiop clr
                              WHERE crlstatus = 'A'
                                    AND crltipo = 'QL'
                                    AND clr.crlquery IS NOT NULL
                              ORDER BY crldsc
DML;
                    #ver($sql,d);
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
            <!-- Dados para Salvar a Consulta-->
            
            <input type="hidden" name="cexid" id="cexid" value="<?php echo $dadosConsultaCarregada['cexid']; ?>" />
            <input type="hidden" name="cexnome" id="cexnome" value="<?php echo $dadosConsultaCarregada['cexnome']; ?>"  />
            <input type="hidden" name="cexconteudo" id="cexconteudo" value="<?php echo $dadosConsultaCarregada['cexconteudo']; ?>"  />
            <input type="hidden" name="cexpublico" id="cexpublico" value="<?php echo $dadosConsultaCarregada['cexpublico']; ?>"  />
            <input type="hidden" name="salvar" id="salvar" value=""  />
            <!-- FIM Dados para Salvar a Consulta-->

            <button type="button" class="btn btn-warning" id="clear">Limpar</button>
            <button type="button" class="btn btn-primary" id="pesquisar">Pesquisar</button>
            <button type="button" class="btn btn-danger" id="exportar">Exportar XLS</button>
            <?php if (isset($resultado['cexid'])) { ?>
                <button type="button" class="btn btn-primary" data-toggle="modal" data-target="#modalSalvarConsulta">Salvar alterações na Consulta</button>
            <?php } else { ?>
                <button type="button" class="btn btn-success" data-toggle="modal" data-target="#modalSalvarConsulta">Cadastrar Consulta</button>
            <?php } ?>
        </div>
        <br style="clear:both" />

        <?php
        /* Retorna as mensagens */
        echo $fm->getMensagens();

        $listagem = $resultado['listagem'];
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        if ($_SESSION['superuser'] == 1) {
            echo "<br/><div style=\" color:#FFF \"> {$resultado['sql']} </div><br/><br/><br/>";
        }
        ?>
    </div>
</form>

<!-- Salvar Consulta -->
<script type="text/javascript" lang="JavaScript">
    $(document).ready(function() {

        // -- Correção paliativa do bug #9920 do bootstrap.
        // -- Correção agendada para a versão 3.0.3 do bootstrap.
        // -- <https://github.com/twbs/bootstrap/issues/9920>
        $("input:radio").change(function() {
            $(this).prop('checked', true);
        });

        $("#salvarConsulta").click(function() {

            if (!$("#cexnome_form").val()) {
                $("#cexnome_form").parent().parent().addClass("has-error");
                return false;
            }

            /* Dados do From */
            $("#cexnome_form").parent().parent().removeClass("has-error").addClass("has-success");
            $("#cexconteudo").val(JSON.stringify($('#formBusca').serializeArray()));
            $("#cexnome").val($("#cexnome_form").val());
            $("#cexpublico").val($('input[name=cexpublico_form]:checked').val());
            $("#salvar").val('t');

            /* Enviando o form completo */
            $('#requisicao').attr('value', 'mostrarHTML');
            $('#salvarConsulta').html('Salvando...');
            $('#salvarConsulta').attr('disabled', 'disabled');

            $("body").prepend('<div class="ajaxCarregando"></div>');
            $(".ajaxCarregando").hide().html('Carregando, aguarde...').fadeIn();
            $('#formBusca').submit();


        });
        
    });
    /**
     * Carregar o relatório na tela
     */
    function executarRelatorio(cexid) {
        location.href = 'siafi.php?modulo=principal/dadossiop/gerarrelatorio&acao=A&cexid='+cexid;
    }

    function apagarRelatorio(cexid) {
        bootbox.confirm("Tem certeza que deseja excluir a consulta?", function(result) {
            if (!result) return false;

            var urlBase = "siafi.php?modulo=principal/dadossiop/gerarrelatorio&acao=A"
              , $request = $.ajax({url:urlBase+"&cexid="+cexid+"&action=apagar"});

            $request.done(function(data) {
                if (data.fail) {
                    bootbox.alert("Erro ao tentar excluir consulta.")
                } else {
                    bootbox.alert("Consulta excluida com sucesso.", function(){
                        location.href=urlBase;
                    });
                }
            });
        });
    }
</script>
<div class="modal fade" id="modalSalvarConsulta" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal"><span aria-hidden="true">&times;</span><span class="sr-only">Fechar</span></button>
                <h4 class="modal-title" id="myModalLabel">Salvar Consulta</h4>
            </div>
            <div class="modal-body">
                <div class="form-horizontal">

                    <div class="form-group">
                        <label class="control-label col-md-2" for="cexnome_form">Nome da Consulta: </label>
                        <div class="col-md-10">
                            <input id="cexnome_form" class="normal form-control " type="text" value="" maxlength="300" name="cexnome_form" style="text-align:left;">
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="control-label col-md-2" for="">Acesso:</label>

                        <div class="btn-group col-md-10" data-toggle="buttons">
                            <label class="btn btn-default active" for="privado" checked="checked">
                                <input id="privado" type="radio" checked="" value="true" name="cexpublico_form">
                                Privado
                            </label>
                            <label class="btn btn-default" for="publico">
                                <input id="publico" type="radio" value="false" name="cexpublico_form">
                                Público
                            </label>
                        </div>

                    </div>
                </div>

            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-danger" data-dismiss="modal">Cancelar</button>
                <button type="button" class="btn btn-primary" id="salvarConsulta">Salvar</button>
            </div>
        </div>
    </div>
</div>
