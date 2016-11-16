<?php
require_once APPRAIZ . "www/spo/_funcoes.php";
require_once APPRAIZ . "includes/funcoesspo.php";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . "spo/ws/sof/Quantitativo.php";

$fm = new Simec_Helper_FlashMessage('spo/integracao/sof/carga');

if (chaveTemValor($_REQUEST, 'requisicao')) {
    switch ($_REQUEST['requisicao']) {
        case 'executarCarga':
            executarCargaPloa($_REQUEST, $fm);
            break;
        case 'limparCarga':
            limparCargaPloa($_REQUEST, $fm);
            break;        
    }
}

$mapProgramacaoCompleta = array
    (
    'Ações' => array('ws' => 'retornarAcoes', 'dto' => 'acoesDTO', 'tbl' => 'wssof.ws_acoesdto'),
    'Localizadores' => array('ws' => 'retornarLocalizadores', 'dto' => 'localizadoresDTO', 'tbl' => 'wssof.ws_localizadoresdto'),
    'Planos Orçamentários' => array('ws' => 'retornarPlanosOrcamentarios', 'dto' => 'planosOrcamentariosDTO', 'tbl' => 'wssof.ws_planosorcamentariosdto'),
    'Programas' => array('ws' => 'retornarProgramas', 'dto' => 'programasDTO', 'tbl' => 'wssof.ws_programasdto'),
        #'Quantitativos (Metas, etc)' => array('ws' => 'obterProgramacaoCompletaQuantitativo', 'dto' => '', 'tbl' => ''),
);

ksort($mapProgramacaoCompleta);

$mapTabelasApoio = array
    (
    'Esferas' => array('ws' => 'retornarEsferas', 'dto' => 'esferasDTO', 'tbl' => 'wssof.ws_esferasdto'),
    'Funções' => array('ws' => 'retonarFuncoes', 'dto' => 'funcoesDTO', 'tbl' => 'wssof.ws_funcoesdto'),
    'Produtos' => array('ws' => 'retornarProdutos', 'dto' => 'produtosDTO', 'tbl' => 'wssof.ws_produtosdto'),
    'Subfunções' => array('ws' => 'retornarSubFuncoes', 'dto' => 'subFuncoesDTO', 'tbl' => 'wssof.ws_subfuncoesdto'),
    'Tipos de ações' => array('ws' => 'retornarTiposAcao', 'dto' => 'tiposAcaoDTO', 'tbl' => 'wssof.ws_tiposacaodto'),
    'Unidades de medida' => array('ws' => 'retornarUnidadesMedida', 'dto' => 'unidadesMedidaDTO', 'tbl' => 'wssof.ws_unidadesmedidadto')
);

ksort($mapTabelasApoio);

$mapTabelasProgCompleQuant = array(
    'Informação de Captação PLOA' => array('ws' => '', 'dto' => '', 'tbl' => 'wssof.ws_informacaocaptacaoploadto')
);

require_once APPRAIZ . "includes/cabecalho.inc";

$anoPloa = $_SESSION['exercicio']+1;
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
            exercicio = <?php echo $_SESSION['exercicio'];?>;
            bootbox.confirm('Você realmente deseja apagar os dados do snapshot para '+exercicio + ' ?', function (re) {
                if (re) {
                    location.href = "spo.php?modulo=principal/processamento/cargaploa&acao=A&requisicao=limparCarga&anoapagar="+exercicio;
                }
            });
        });
    });
</script>    
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="spo.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li>Snapshots</li>
        <li class="active">Snapshots Dados Carga da PLOA <b><?php echo $exercicio; ?>/<?php echo $anoPloa; ?></b></li>
    </ol>
<?php echo $fm->getMensagens(); ?>
    <div class="well">
        <div> 
            Baixe com a data de <b>HOJE</b> os dados abaixo, para o exercício <b>POSTERIOR</b>.
            <br/>
            Execute a carga no exercício atual!
        </div>
        <form id="frmProcessamento" name="frmProcessamento" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
            <input type="hidden" name="requisicao" value="executarCarga"/>
            <div class="form-group">
                <div class="col-lg-10">
                    <input type="hidden" name="execucao" id="execucao" value="cadastrar">
                    <input type="hidden" name="vacid" id="vacid" value="<?= isset($resultSet['vacid']) ? $resultSet['vacid'] : null; ?>" />
                    <input type="hidden" name="vaeid" id="vaeid" value="<?= isset($resultSet['vaeid']) ? $resultSet['vaeid'] : null; ?>" />
                </div>
            </div>
            <div class="form-group">
                <label for="servicos[]" class="col-lg-2 control-label">
                    Qualitativo / Programação completa:
                </label>
                <div class="col-lg-10">
                    <?php foreach ($mapProgramacaoCompleta as $name => $info) : ?>
                        <?php
                        $query = "select dataultimaatualizacao from {$info['tbl']}
                                          where to_char(dataultimaatualizacao, 'YYYY-MM-DD') = to_char(current_date, 'YYYY-MM-DD')"
                                . "AND (SELECT MAX(exercicio) FROM wssof.ws_acoesdto LIMIT 1) = '{$anoPloa}'";
                        $recurso = $db->executar($query);
                        $check = $db->conta_linhas($recurso);
                        ?>
                        <input type="hidden" name="tabelas[<?php echo $name; ?>]" value="<?php echo $check >= 0 ? '1' : '0'; ?>" />
                        <?php if ($check >= 0) : ?>
                            <i class="glyphicon glyphicon-ok green"></i>
                        <?php else : ?>
                            <i class="glyphicon glyphicon-remove red"></i>
                        <?php endif; ?>
                        <?php echo $name; ?> <span class="badge" style="font-weight: normal;"><?php echo $check + 1; ?></span><br>
                    <?php endforeach; ?>
                </div>
            </div>
            <div class="form-group">
                <label for="servicos[]" class="col-lg-2 control-label">
                    Qualitativo / Tabelas de Apoio:
                </label>
                <div class="col-lg-10">
                    <?php foreach ($mapTabelasApoio as $name => $info) : ?>
                        <?php
                        $query = "select dataultimaatualizacao from {$info['tbl']}
                                          where to_char(dataultimaatualizacao, 'YYYY-MM-DD') = to_char(current_date, 'YYYY-MM-DD')"
                                . "AND (SELECT MAX(exercicio) FROM wssof.ws_acoesdto LIMIT 1) = '{$anoPloa}'";
                        $recurso = $db->executar($query);
                        $check = $db->conta_linhas($recurso);
                        ?>
                        <input type="hidden" name="tabelas[<?php echo $name; ?>]" value="<?php echo $check >= 0 ? '1' : '0'; ?>" />
                        <?php if ($check >= 0) : ?>
                            <i class="glyphicon glyphicon-ok green"></i>
                        <?php else : ?>
                            <i class="glyphicon glyphicon-remove red"></i>
                        <?php endif; ?>
                        <?php echo $name; ?> <span class="badge" style="font-weight: normal;"><?php echo $check + 1; ?></span><br>
<?php endforeach; ?>
                </div>
            </div>

            <div class="form-group">
                <label for="" class="col-lg-2 control-label">
                    Quantitativo / Geral:
                </label>
                <div class="col-lg-10">
                <?php
                foreach ($mapTabelasProgCompleQuant as $name => $info) :

                     $query = "select dataultimaatualizacao from {$info['tbl']}
                                          where to_char(dataultimaatualizacao, 'YYYY-MM-DD') = to_char(current_date, 'YYYY-MM-DD')"
                        . "AND exercicio = '{$_SESSION['exercicio']}' ";
                        $recurso = $db->executar($query);
//ver($query);
                    $check = $db->conta_linhas($recurso);
                ?>
                    <input type="hidden" name="tabelas[<?php echo $name; ?>]" value="<?php echo $check >= 0 ? '1' : '0'; ?>" />
                <?php if ($check >= 0) : ?>
                    <i class="glyphicon glyphicon-ok green"></i>
                <?php else : ?>
                    <i class="glyphicon glyphicon-remove red"></i>
                <?php endif; ?>
                <?php echo $name; ?> <span class="badge" style="font-weight: normal;"><?php echo $check+1; ?></span><br>
                <?php
                endforeach; ?>
                </div>
            </div>

            <div class="form-group">
                <label for="momento" class="col-lg-2 control-label">
                    Momento
                </label>
                <div class="col-lg-10">
                    <?php
                    if ($_REQUEST['momento'] == '') {
                        $_REQUEST['momento'] = '4000';
                    }
                    $sql = "SELECT distinct
                                codigomomento as codigo,
                                codigomomento || ' - ' || descricao as descricao
                        FROM wssof.ws_momentosdto m
                        WHERE m.snativo = '1'
                        ORDER BY codigo";

                    $options = array(
                        'titulo' => 'Selecione ao menos um momento',
                        'obrig' => 'S'
                    );

                    inputCombo('momento', $sql, $_REQUEST['momento'], 'cols-qualit', $options);
                    ?>
                </div>
            </div>
            <div class="form-group">
                <div class="col-lg-10 col-lg-offset-2">
                    <button class="btn btn-danger" type="button" id="limparBase"><spam class="glyphicon glyphicon-fire"> </spam> Limpar a carga Atual</button>
                    <button class="btn btn-success" id="inserir" type="submit"><i class="glyphicon glyphicon-upload"></i> Executar</button>
                </div>
            </div>
        </form>
    </div>
</div>