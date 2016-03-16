<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 05/06/2015
 * Time: 10:58
 */

// Dependências
ob_start();
global $db;

$ravid = $_GET['ravid'];
$diretoria = isset($_GET['diretoria']) ? $_GET['diretoria'] : 1;

$title = "Relatório AE Supervisor Geral";
$meses = array(
    '01' => 'Janeiro',
    '02' => 'Fevereiro',
    '03' => 'Março',
    '04' => 'Abril',
    '05' => 'Maio',
    '06' => 'Junho',
    '07' => 'Julho',
    '08' => 'Agosto',
    '09' => 'Setembro',
    '10' => 'Outubro',
    '11' => 'Novembro',
    '12' => 'Dezembro'
);

include_once '../../sase/classes/AvaliadorEducacional.class.inc';
include_once '../../sase/classes/RelatorioAvaliadorRedeae.class.inc';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";
// Fim Dependências

$avaliadoreducacional = new AvaliadorEducacional();
$rel = new RelatorioAvaliadorRedeae();

$usucpf = $_SESSION['usucpf'];
$rel->arAtributos['usucpf'] = $usucpf;
$data = new Data();
$pflcod = PFLCOD_SASE_EXECUTIVO;
$estuf = $rel->pegaEstadoAvaliador('estuf');
$mes = date('n');

/**
 * Carrega as informações do relatório.
 * Dentro da variável '$rel'
 */
if (!empty($ravid) && $ravid != 'undefined'){
    $rel->carregarPorId($ravid);
    //ver($rel, d);
    $perCampos = $rel->retornaDisCampos($_SESSION['usucpf']);

    $dados = $rel->getDados();
    $usucpf = $rel->arAtributos['usucpf'];
    $ratperiodo = $rel->arAtributos['ratperiodo'];
    $ratdata1periodo = $data->formataData($rel->arAtributos['ratdata1periodo']);
    $ratdata2periodo = $data->formataData($rel->arAtributos['ratdata2periodo']);
    $mr = explode("/", $ratdata2periodo);
    $mes = $mr[1];
    $pflcod = $rel->arAtributos['pflcod'];
    $estuf = $rel->pegaEstadoAvaliador('estuf');

}

if ($_POST['acao']){
    $retorno = array(
        "return" => null,
        "msg"    => null,
        "ravid"  => null
    );
    switch ($_POST['acao']){
        case 'teste':
            ver(simec_utf8_decode_recursive($_POST), d);
            exit;

        case 'carregar':
            /**
             * Código encarregado de criar o relatório.
             */
            if ($rel->validaRelatorio()){
                $arAtr = array();
                $arAtr['pflcod']          = $_POST['pflcod'];
                $arAtr['ratdata1periodo'] = $_POST['ratdata1periodo'];
                $arAtr['ratdata2periodo'] = $_POST['ratdata2periodo'];
                $arAtr['ratperiodo']      = $_POST['ratperiodo'];
                $arAtr['usucpf']          = $_POST['usucpf'];
                $rel->popularDadosObjeto($arAtr);

                //ver($rel->arAtributos, d);

                $ravid = $rel->salvarRelatorio();

                if ($ravid) {
                    $retorno['return'] = "true";
                    $retorno['msg']    = "Relatório criado com sucesso.";
                    $retorno['ravid']  = $ravid;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                }

            } else {
                $retorno['return'] = "false";
                $retorno['msg']    = $rel->msg;
            }
            echo implode("|",$retorno);
            exit;

        case 'salvar':
            $_POST    = simec_utf8_decode_recursive($_POST);
            $_REQUEST = simec_utf8_decode_recursive($_REQUEST);
            /**
             * Código encarregado de salvar as informações do relatório.
             */

            /**
             * Salva o quadro de Atividades Execitadas no Período
             */
            if (!$rel->salvaAtividades()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar as Atividades Executadas no período.";
                    echo implode("|",$retorno);
                    exit;
                }
            }

            /**
             *  Salva o quadro de Informações a respeito do Plano Estadual de Educação
             */
            if (!$rel->salvaInformacoesPEESupervisorGeral()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar o quadro de Informações a respeito do Plano Estadual de Educação";
                    echo implode("|",$retorno);
                    exit;
                }
            }

            /**
             *  Salva o quadro de Ações propostas pelos AE Eupervisores para os municipios sem informação, sem comissão instituída ou sem alteração de etapa de trabalho
             */
            if (!$rel->salvaAcoesSuperGeral()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar o quadro de Ações propostas pelos AE Supervisores para os municipios sem informação, sem comissao instituída e sem alteração de etapa de trabalho";
                    echo implode("|",$retorno);
                    exit;
                }
            }

            /**
             *  Salva as informações complementares
             */
            if (!$rel->salvaInformacoesComplementares()){
                if ($rel->msg != ''){
                    $retorno['return'] = "false";
                    $retorno['msg']    = $rel->msg;
                    echo implode("|",$retorno);
                    exit;
                } else {
                    $retorno['return'] = "false";
                    $retorno['msg']    = "Erro ao salvar as Informações Complementares";
                    echo implode("|",$retorno);
                    exit;
                }
            }

            $retorno['return'] = true;
            $retorno['msg']    = "Dados do relatório salvos com sucesso.";
            $retorno['ravid']  = $ravid;
            echo implode("|",$retorno);

            exit;

        case 'imprimir':
            $_POST    = simec_utf8_decode_recursive($_POST);
            $_REQUEST = simec_utf8_decode_recursive($_REQUEST);
            ob_clean ();

            $html = $rel->geraRelatorioSuperGeral();
            $nome = $rel->pegaAvaliador();

            $content = http_build_query(array(
                'conteudoHtml' => utf8_encode($html)
            ));

            $context = stream_context_create(array(
                'http' => array (
                    'method' => 'POST',
                    'content' => $content
                )
            ));

            $contents = file_get_contents ( 'http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context );
            header ( 'Content-Type: application/pdf' );
            header ( 'Content-Disposition: attachment; filename=Relatorio_AE_'.$nome.'_'.$_REQUEST['ravid']);
            echo $contents;
            exit();
            break;
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
$prsano = trim($_SESSION['exercicio']);
//$perCampos = false;

//ver($ravid, $rel);
?>

<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="/includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">

<script src="/includes/funcoes.js"></script>
<script src="/includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>


<script>
    function salvar(acao, ajax){
        ajax = typeof ajax !== 'undefined' ? ajax : true;
        jQuery('#acao').val(acao);
        console.log(jQuery('#form-save').serialize());
        if (acao !== 'undefined'){
            if (ajax) {
                jQuery.ajax({
                    url: '',
                    type: 'POST',
                    data: jQuery('#form-save').serialize(),
                    //contentType: 'application/x-www-form-urlencoded; charset=ISO-8859-1',
                    //contentType: 'Content-type: text/plain; charset=iso-8859-1',
                    success: function (e) {
                        if (acao != 'teste') {
                            var a = e;
                            var r = a.split("|");
                            alert(r[1]);
                            console.log(r);
                            if (r[2] != '' && r[2] !== 'undefined') {
                                window.location.href = 'sase.php?modulo=relatorio/relResultPECoordenador&acao=A&diretoria=<?= $diretoria ?>&ravid=' + r[2];
                            }
                        } else {
                            alert(e);
                        }
                    }
                });
            } else {
                jQuery('[name=acao]').val(acao);
                jQuery('[name=form-save]').submit();
            }
        } else {
            alert('Ação não informada.');
        }
    }

    /**
     *
     */
    function montaPeriodo(){

        switch (jQuery('#ratperiodo').val()) {
            case '1':
                jQuery('#ratdata1periodo').val('01/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                jQuery('#ratdata2periodo').val('15/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                break;
            case '2':
                jQuery('#ratdata1periodo').val('16/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                jQuery('#ratdata2periodo').val((new Date(2015, jQuery('#mes').val(), 0)).getDate() + '/' + jQuery('#mes').val() + '/<?= $prsano ?>');
                break;
        }

        console.log(jQuery('#ratdata1periodo').val() + " à " + jQuery('#ratdata2periodo').val());
        jQuery('#hidmes').val(jQuery('#mes').val());
    }
</script>

<div id='cabecalhoSistema'>
    <center><h3><?=$titulo?></h3></center>
</div>

<div id="container">
    <div class="row">
        <div class="row col-md-12">
            <form id="form-save" method="post" name="form-save" role="form" class="form-horizontal">
                <input type="hidden" id="acao" name="acao" value=""/>
                <input type="hidden" id="ravid" name="ravid" value="<?= $ravid ?>"/>
                <input type="hidden" id="usucpf" name="usucpf" value="<?= $usucpf ?>"/>
<!--                <input type="hidden" id="ratperiodo" name="ratperiodo" value="1"/>-->
                <input type="hidden" id="pflcod" name="pflcod" value="<?= $pflcod ?>"/>
                <input type="hidden" id="ratdata1periodo" name="ratdata1periodo" value="<?= $ratdata1periodo ?>"/>
                <input type="hidden" id="ratdata2periodo" name="ratdata2periodo" value="<?= $ratdata2periodo ?>"/>
                <input type="hidden" id="hidmes" name="hidmes" value="<?= $mes ?>"/>

                <!-- --------------------------------------------------------------------------------------------- -->
                <!--                Informações gerais do relatório                                                -->
                <!-- --------------------------------------------------------------------------------------------- -->
                <div class="row col-md-11">
                    <div class="well">
                        <fieldset>
                            <legend>1. Informaçoes gerais do relatório</legend>
                            <div class="form-group">
                                <label for="pflcod" class="col-lg-4 col-md-4 control-label obrigatorio">Coordenador</label>
                                <div class="col-lg-8 col-md-8">
                                    <label style="font-weight: normal; margin-top: 8px;">
                                        <?php
                                        echo $rel->pegaAvaliador();
                                        ?>
                                    </label>
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="pflcod" class="col-lg-4 col-md-4 control-label obrigatorio">Estado</label>
                                <div class="col-lg-8 col-md-8">
                                    <label style="font-weight: normal; margin-top: 8px;">
                                        <?php
                                        echo $rel->pegaEstadoAvaliador();
                                        ?>
                                    </label>
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="pflcod" class="col-lg-4 col-md-4 control-label obrigatorio">Período do Relatório:</label>
                                <div class="col-lg-8 col-md-8">
                                    <select class="form-control chosen" id="ratperiodo" name="ratperiodo" onchange="montaPeriodo()">
                                        <option value="">Selecione...</option>
                                        <option value="1" <?= $ratperiodo == 1 ? 'selected="true"' : '' ?>>1° Quinzena</option>
                                        <option value="2" <?= $ratperiodo == 2 ? 'selected="true"' : '' ?>>2° Quinzena</option>
                                    </select>
                                </div>
                            </div>
                            <?php if ($ratdata1periodo != '' && $ratdata2periodo != ''){ ?>
                                <div class="form-group">
                                    <label class="col-lg-4 col-md-4 control-label">Período em Data</label>
                                    <div class="col-lg-8 col-md-8">
                                        <label style="font-weight: normal; margin-top: 8px;">
                                            <?php
                                            echo $ratdata1periodo . ' à ' . $ratdata2periodo;
                                            ?>
                                        </label>
                                    </div>
                                </div>
                            <?php } ?>
                            <div class="form-group">
                                <label for="dmdprazo" class="col-lg-4 col-md-4 control-label obrigatorio">Mês:</label>
                                <div class="col-lg-8 col-md-8 ">
                                    <select class="form-control chosen" id="mes" name="mes" onchange="montaPeriodo()">
                                        <?php
                                        foreach ($meses as $c => $v) {
                                            $s = $mes == $c ? 'selected="true"' : '';
                                            echo <<<HTML
                                                    <option {$s} value="{$c}">$v</option>
HTML;
                                        }
                                        ?>
                                    </select>
                                </div>
                            </div>
                            <div class="text-right">
                                <?php if (empty($ravid)) { ?>
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('carregar')">Carregar
                                    </button>
                                <?php } else { ?>
                                    <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar')">Salvar
                                    </button>
                                <?php } ?>
                                <?php if (!empty($ravid)) { ?>
                                    <button title="Gerar Relatorio" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('imprimir', false)">Gera Relatório
                                    </button>
                                <?php } ?>
                            </div>
                        </fieldset>
                    </div>
                </div>
                <!-- --------------------------------------------------------------------------------------------- -->
                <!--            Fim Informações gerais do relatório                                                -->
                <!-- --------------------------------------------------------------------------------------------- -->

                <?php if ($ravid != ''){ ?>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Açoes de workflow                                                              -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="col-md-1">
                        <?php
                        if( !$rel->docid && $_GET['ravid'] ){
                            //$docid = wf_cadastrarDocumento( TPDID_SASE_AVALIADOREDUCACIONAL, 'Documento Avaliador Educacional' );
                            $docid = cadastraDocumentoAE($pflcod, 'Documento Avaliador Educacional');
                            $rel->docid = $docid;
                            // ver($Assessoramento,d);
                            $rel->alterar();
                            $rel->commit();
                        }

                        if($_GET['ravid'] && $rel->docid) wf_desenhaBarraNavegacao( $rel->docid, array('docid'=>$rel->docid,'ravid'=>$rel->ravid, 'usucpf'=>$rel->arAtributos['usucpf'], 'ratdata1periodo'=>$data->formataData($rel->arAtributos['ratdata1periodo']), 'ratdata2periodo'=>$data->formataData($rel->arAtributos['ratdata2periodo'])) ); ?>

                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Açoes de workflow                                                              -->
                    <!-- --------------------------------------------------------------------------------------------- -->

                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Atividades exutadas no período                                                 -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>2. Atividades Executadas no Período</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getListaAtividadesDesenvolvidas($perCampos, true, $pflcod, true, $estuf); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Atividades exutadas no período                                                 -->
                    <!-- --------------------------------------------------------------------------------------------- -->

                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Informações a respeito do Plano Estadual de Educação                           -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>3. Informações a respeito do Plano Estadual de Educação</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getListaInformacoesPEESupervisorGeral($perCampos, true); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--        Fim Informações a respeito do Plano Estadual de Educação                               -->
                    <!-- --------------------------------------------------------------------------------------------- -->

                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Situaçao atual das etapas de trabalho propostas ao PNE                         -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>4. Situação atual com relação às etapas de trabalho propostas para a elaboração ou adequação dos Planos Municipais de Educação ao PNE.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getContadorSituacaoAtualEtapasPNE($estuf, $mes, true); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--        Fim Situação atual das etapas de trabalho propostas ao PNE                             -->
                    <!-- --------------------------------------------------------------------------------------------- -->

                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Municipios que apresentaram alteraçao de etapa no período                      -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>5. Municípios do Estado que apresentaram alteração na sua etapa de trabalho no período.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getListaMunComAlteracaoPEESupervisorGeral($estuf, $mes, true); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--        Fim Municipio que apresentam alteração de etapa no período                             -->
                    <!-- --------------------------------------------------------------------------------------------- -->

                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Municípios que permaneceram namesma etapa do período anterior                  -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>6. Municípios que permaneceram na mesma etapa do período anterior.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getListaMunSemAlteracaoEtapa($estuf, $mes, true); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Municípios que permaneceram namesma etapa do período anterior                  -->
                    <!-- --------------------------------------------------------------------------------------------- -->

                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Municipios ainda sem informação ou sem trabalho iniciado.                      -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>7. Municípios ainda sem informação ou sem trabalho iniciado.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getListaMunSemInfo($estuf, $mes, true); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Municípios ainda sem informação ou sem trabalho iniciado                       -->
                    <!-- --------------------------------------------------------------------------------------------- -->

                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--     Açoes propostas pelos AE Técnicos e Supervisores para os municípios.                      -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>8. Ações propostas pelos AE Supervisores para os municípios sem informação, sem comissão instituída e sem alteração de etapa de trabalho.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <?php echo $rel->getAcoesSuperGeral($estuf, $mes, $perCampos, true); ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!-- Fim Açoes propostas pelos AE Técnicos e Supervisores para os municípios.                      -->
                    <!-- --------------------------------------------------------------------------------------------- -->

                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                          Informações complementares                                           -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <legend>9. Informações complementares.</legend>
                                <div class="form-group">
                                    <div class="col-lg-12 col-md-12">
                                        <textarea class="form-control" maxlength="500" id="ravinfcomplementar" <?= $perCampos ? '' : 'disabled="disabled""' ?> name="ravinfcomplementar" rows="5"><?= $rel->arAtributos['ravinfcomplementar'] ?></textarea>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                      Fim Informações complementares                                           -->
                    <!-- --------------------------------------------------------------------------------------------- -->

                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--                Atividades exutadas no período                                                 -->
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <div class="row col-md-11">
                        <div class="well">
                            <fieldset>
                                <div class="col-lg-12 col-md-12">
                                    <div class="text-right">
                                        <?php if (empty($ravid)) { ?>
                                            <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('carregar')">Carregar
                                            </button>
                                        <?php } else { ?>
                                            <button title="Salvar" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('salvar')">Salvar
                                            </button>
                                        <?php } ?>
                                        <?php if (!empty($ravid)) { ?>
                                            <button title="Gerar Relatorio" class="btn btn-success" type="button" id="btnPesquisar" onclick="salvar('imprimir', false)">Gera Relatório
                                            </button>
                                        <?php } ?>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                    <!-- --------------------------------------------------------------------------------------------- -->
                    <!--            Fim Atividades exutadas no período                                                 -->
                    <!-- --------------------------------------------------------------------------------------------- -->

                <?php } ?>

            </form>
        </div>
    </div>
</div>
<script>
    $(document).ready(function(){
        $('.campoCpf').mask("999.999.999-99");
        $('.campoData').mask('99/99/9999');
        $('.campoData').datepicker();
        // Inicia o período com o valor marcado na combo 'Mês'
        montaPeriodo();
    });
</script>