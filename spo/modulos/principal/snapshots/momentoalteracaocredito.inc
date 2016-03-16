<?php
/**
 * Criação do snapshot de dados do sistema Alteração Orçamentária.
 *
 * $Id$
 * @filesource
 */
if (empty($_REQUEST['momento'])) {
    $_REQUEST['momento'] = '9000';
}

$fm = new Simec_Helper_FlashMessage('spo/integracao/sof/carga');

if (chaveTemValor($_REQUEST, 'requisicao')) {
    switch ($_REQUEST['requisicao']) {
        case 'executarCarga':
            executarCargaAlteracaoOrcamentaria($_REQUEST, $fm);
            break;
    }
}

$mapProgramacaoCompleta = array(
    'Ações' => array(
        'ws' => 'retornarAcoes',
        'dto' => 'acoesDTO',
        'tbl' => 'wssof.ws_acoesdto',
        'url' => '?modulo=integracao/sof/qualitativo/programacaocompleta&acao=A',
        'url-params' => array('frmVinculacao' => array(
                'momento' => $_REQUEST['momento'],
                'servico' => array('Ações')
        ))
    ),
    'Programas' => array(
        'ws' => 'retornarProgramas',
        'dto' => 'programasDTO',
        'tbl' => 'wssof.ws_programasdto',
        'url' => '?modulo=integracao/sof/qualitativo/programacaocompleta&acao=A',
        'url-params' => array('frmVinculacao' => array(
            'momento' => $_REQUEST['momento'],
            'servico' => array('Programas')
        ))
    ),
    'Localizadores' => array(
        'ws' => 'retornarLocalizadores',
        'dto' => 'localizadoresDTO',
        'tbl' => 'wssof.ws_localizadoresdto',
        'url' => '?modulo=integracao/sof/qualitativo/programacaocompleta&acao=A',
        'url-params' => array('frmVinculacao' => array(
                'momento' => $_REQUEST['momento'],
                'servico' => array('Localizadores')
        ))
    ),
    'Planos Orçamentários' => array(
        'ws' => 'retornarPlanosOrcamentarios',
        'dto' => 'planosOrcamentariosDTO',
        'tbl' => 'wssof.ws_planosorcamentariosdto',
        'url' => '?modulo=integracao/sof/qualitativo/programacaocompleta&acao=A',
        'url-params' => array('frmVinculacao' => array(
                'momento' => $_REQUEST['momento'],
                'servico' => array('Planos Orçamentarios')
        ))
    ),
);

$mapTabelasApoio = array(
    'Produtos' => array(
        'ws' => 'retornarProdutos',
        'dto' => 'produtosDTO',
        'tbl' => 'wssof.ws_produtosdto',
        'url' => '?modulo=integracao/sof/qualitativo/tabelasapoio&acao=A',
        'url-params' => array('frmVinculacao' => array(
            'momento' => $_REQUEST['momento'],
            'servico' => array('Produtos')
        ))
    ),
    'Unidades de Medida' => array(
        'ws' => 'retornarUnidadesMedida',
        'dto' => 'unidadesMedidaDTO',
        'tbl' => 'wssof.ws_unidadesmedidadto',
        'url' => '?modulo=integracao/sof/qualitativo/tabelasapoio&acao=A',
        'url-params' => array('frmVinculacao' => array(
            'momento' => $_REQUEST['momento'],
            'servico' => array('Unidades de medida')
        ))
    ),
);

$mapTabelasProgCompleQuant = array(
    'Programação Completa' => array(
        'ws' => '',
        'dto' => '',
        'tbl' => 'wssof.ws_propostadto',
        'url' => '?modulo=integracao/sof/quantitativo/obterprogramacaocompletaquantitativo&acao=A',
        'url-params' => array('frmConsultar' => array(
                'codigoMomento' => $_REQUEST['momento']
        ))
    )
);

$mapTabelasExecucao = array(
    'Execução Financeira com Fonte / Natureza de Despesa' => array(
        'ws' => '',
        'dto' => '',
        'tbl' => 'wssof.ws_execucaoorcamentariadto',
        'url' => '?modulo=integracao/sof/quantitativo/consultarExecucaoOrcamentaria&acao=A',
        'url-params' => array('consultarexecucaoorcamentaria' => array(
            'selecaoRetorno' => array(
                "acao",
                "dotAtual",
                "dotacaoInicial",
                "fonte",
                "idoc",
                "iduso",
                "localizador",
                "natureza",
                "planoOrcamentario",
                "resultadoPrimarioAtual",
                "resultadoPrimarioLei",
                "unidadeOrcamentaria"
            )
        )),
    )
);

/**
 * Cabeçalho padrão SIMEC.
 * @see cabecalho.inc
 */
require_once APPRAIZ . "includes/cabecalho.inc";
?>
<link rel="stylesheet" href="/library/bootstrap-toggle/css/bootstrap-toggle.min.css">
<script src="/library/bootstrap-toggle/js/bootstrap-toggle.min.js"></script>
<script type="text/javascript">$(document).ready(function () {
        $('[data-toggle="tooltip"]').tooltip();
    });</script>
<style>.red {color: red; font-weight:bold;}.green {color: green; font-weight:bold;}.bold {font-weight:bold; }</style>
<script>
$(document).ready(function () {
    $("#limparBase").click(function () {
        exercicio = <?php echo $_SESSION['exercicio']; ?>;
        bootbox.confirm('Você realmente deseja apagar os dados do snapshot?', function (re) {
            if (re) {
                location.href = "spo.php?modulo=principal/snapshots/momentoalteracaocredito&acao=A&acao=A&requisicao=limparCarga";
            }
        });
    });

    $('#frmProcessamento').submit(function(){
        var enviar = true;
        $('input[name^="tabelas"]').each(function(){
            if ('0' === $(this).val()) {
                enviar = false;
                return false;
            }
        });
        if (!enviar) {
            alert('Antes de realizar o processamento, todas as cargas exibidas acima devem estar completas.');
            return false;
        }

        if (!$('#momento').val()) {
            alert('Antes de realizar o processamento você deve selecionar um momento.');
            return false;
        }
        if (!$('#periodo').val()) {
            alert('Antes de realizar o processamento você deve selecionar um período.');
            return false;
        }
    });
});
</script>
<div class="col-lg-12">
    <?php
    $bc = new Simec_View_Breadcrumb();
    $bc->add('Snapshots')
        ->add('Carga de Alteração Orçamentária')
        ->render();

    echo $fm->getMensagens();
    ?>
    <div class="well">
        <div>
            Baixe com a data de <b>HOJE</b> os dados abaixo.
            <br/>
            Execute a carga somente após as 08:20 da manhã (as 06, 07 e 08 horas a carga automática pode apagar os dados de financeiro)!
        </div>
        <form id="frmProcessamento" name="frmProcessamento" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
            <input type="hidden" name="requisicao" value="executarCarga"/>
            <br />
            <div class="form-group">
                <label for="servicos[]" class="col-lg-2 control-label">
                    Qualitativo > Programação completa:
                </label>
                <div class="col-lg-10">
                    <?php foreach ($mapProgramacaoCompleta as $name => $info) : ?>
                        <?php
                        $query = "select dataultimaatualizacao from {$info['tbl']}
                                          where to_char(dataultimaatualizacao, 'YYYY-MM-DD') = to_char(current_date, 'YYYY-MM-DD')"
                                . "AND (SELECT MAX(exercicio) FROM wssof.ws_acoesdto LIMIT 1) = '{$_SESSION['exercicio']}'";
                        $recurso = $db->executar($query);
                        $check = $db->conta_linhas($recurso);
                        ?>
                        <input type="hidden" name="tabelas[<?php echo $name; ?>]" value="<?php echo $check >= 0 ? '1' : '0'; ?>" />
                        <?php if ($check >= 0) : ?>
                            <i class="glyphicon glyphicon-ok green"></i>
                        <?php else : ?>
                            <i class="glyphicon glyphicon-remove red"></i>
                        <?php endif; ?>
                        <?php echo $name; ?> <span class="badge" style="font-weight: normal;"><?php echo $check + 1; ?></span>
                        <?php
                        if ($info['url']) {
                            $url = "{$info['url']}&" . http_build_query($info['url-params'], '', '&');
                            echo <<<HTML
<a class="btn btn-xs btn-primary" href="{$url}" target="_blank"><span class="glyphicon glyphicon-flash"></span></a>
HTML;
                        }
                        ?>
                        <br />
                    <?php endforeach; ?>
                </div>
            </div>
            <div class="form-group">
                <label for="servicos[]" class="col-lg-2 control-label">
                    Qualitativo > Tabelas de apoio:
                </label>
                <div class="col-lg-10">
                    <?php foreach ($mapTabelasApoio as $name => $info) : ?>
                        <?php
                        $query = "select dataultimaatualizacao from {$info['tbl']}
                                          where to_char(dataultimaatualizacao, 'YYYY-MM-DD') = to_char(current_date, 'YYYY-MM-DD')"
                                . "AND (SELECT MAX(exercicio) FROM wssof.ws_acoesdto LIMIT 1) = '{$_SESSION['exercicio']}'";
                        $recurso = $db->executar($query);
                        $check = $db->conta_linhas($recurso);
                        ?>
                        <input type="hidden" name="tabelas[<?php echo $name; ?>]" value="<?php echo $check >= 0 ? '1' : '0'; ?>" />
                        <?php if ($check >= 0) : ?>
                            <i class="glyphicon glyphicon-ok green"></i>
                        <?php else : ?>
                            <i class="glyphicon glyphicon-remove red"></i>
                        <?php endif; ?>
                        <?php echo $name; ?> <span class="badge" style="font-weight: normal;"><?php echo $check + 1; ?></span>
                        <?php
                        if ($info['url']) {
                            $url = "{$info['url']}&" . http_build_query($info['url-params'], '', '&');
                            echo <<<HTML
<a class="btn btn-xs btn-primary" href="{$url}" target="_blank"><span class="glyphicon glyphicon-flash"></span></a>
HTML;
                        }
                        ?>
                        <br />
                    <?php endforeach; ?>
                </div>
            </div>
            <div class="form-group">
                <label for="" class="col-lg-2 control-label">
                    Quantitativo > Programação completa:
                </label>
                <div class="col-lg-10">
                    <?php
                    foreach ($mapTabelasProgCompleQuant as $name => $info) :
                        $query = "select dataultimaatualizacao from {$info['tbl']}
                                          where to_char(dataultimaatualizacao, 'YYYY-MM-DD') = to_char(current_date, 'YYYY-MM-DD')"
                                . "AND (SELECT MAX(exercicio) FROM wssof.ws_acoesdto LIMIT 1) = '{$_SESSION['exercicio']}' ";
                        $recurso = $db->executar($query);
                        $check = $db->conta_linhas($recurso);
                        ?>
                        <input type="hidden" name="tabelas[<?php echo $name; ?>]" value="<?php echo $check >= 0 ? '1' : '0'; ?>" />
                        <?php if ($check >= 0) : ?>
                            <i class="glyphicon glyphicon-ok green"></i>
                        <?php else : ?>
                            <i class="glyphicon glyphicon-remove red"></i>
                        <?php endif; ?>
                        <?php echo $name; ?> <span class="badge" style="font-weight: normal;"><?php echo $check + 1; ?></span>
                        <?php
                        if ($info['url']) {
                            $url = "{$info['url']}&" . http_build_query($info['url-params'], '', '&');
                            echo <<<HTML
<a class="btn btn-xs btn-primary" href="{$url}" target="_blank"><span class="glyphicon glyphicon-flash"></span></a>
HTML;
                        }
                        ?>
                        <?php endforeach; ?>
                </div>
            </div>
            <div class="form-group">
                <label for="" class="col-lg-2 control-label">
                    Carga da Execução Orçamentária :
                </label>
                <div class="col-lg-10">
                    <?php
                    foreach ($mapTabelasExecucao as $name => $info) :

                        $query = <<<DML
SELECT dataultimaatualizacao
  FROM {$info['tbl']}
  WHERE TO_CHAR(dataultimaatualizacao, 'YYYY-MM-DD') = TO_CHAR(CURRENT_DATE, 'YYYY-MM-DD')
    AND (SELECT MAX(exercicio)
           FROM wssof.ws_acoesdto
           LIMIT 1) = '{$_SESSION['exercicio']}'
DML;
                        $recurso = $db->executar($query);
                        $check = $db->conta_linhas($recurso);
                        ?>
                        <input type="hidden" name="tabelas[<?php echo $name; ?>]" value="<?php echo $check >= 0 ? '1' : '0'; ?>" />
                        <?php if ($check >= 0) : ?>
                            <i class="glyphicon glyphicon-ok green"></i>
                        <?php else : ?>
                            <i class="glyphicon glyphicon-remove red"></i>
                        <?php endif; ?>
                        <?php echo $name; ?> <span class="badge" style="font-weight: normal;"><?php echo $check + 1; ?></span>
                        <?php
                        if ($info['url']) {
                            $url = "{$info['url']}&" . http_build_query($info['url-params'], '', '&');
                            echo <<<HTML
<a class="btn btn-xs btn-primary" href="{$url}" target="_blank"><span class="glyphicon glyphicon-flash"></span></a>
HTML;
                        }
                        ?>
                    <?php endforeach; ?>
                </div>
            </div>
            <div class="form-group">
                <label for="momento" class="col-lg-2 control-label">Momento:</label>
                <div class="col-lg-10">
<?php
$sql = <<<DML
SELECT DISTINCT codigomomento AS codigo,
                codigomomento || ' - ' || descricao AS descricao
  FROM wssof.ws_momentosdto m
  WHERE m.snativo = '1'
  ORDER BY codigo
DML;

$options = array(
    'titulo' => 'Selecione ao menos um momento',
    'obrig' => 'S'
);

inputCombo('momento', $sql, $_REQUEST['momento'], 'momento', $options);
?>
                </div>
            </div>
            <div class="form-group">
                <label for="momento" class="col-lg-2 control-label">Período:</label>
                <div class="col-lg-10">
<?php
$sql = <<<DML
SELECT mcrid  AS codigo,
       mcrdsc AS descricao
  FROM altorc.momentocredito
  WHERE mcrano = '{$_SESSION['exercicio']}'
    AND (current_date between mcrrefinicio AND mcrreffim OR current_date <= mcrrefinicio)
  ORDER BY mcrdsc
DML;

$options = array(
    'titulo' => 'Selecione o período',
    'obrig' => 'S'
);
inputCombo('periodo', $sql, $_REQUEST['periodo'], 'periodo', $options);
?>
                </div>
            </div>
            <div class="form-group">
                <div class="col-lg-10 col-lg-offset-2">
                    <button class="btn btn-success" id="inserir" type="submit"><i class="glyphicon glyphicon-upload"></i> Executar</button>
                </div>
            </div>
        </form>
    </div>
</div>