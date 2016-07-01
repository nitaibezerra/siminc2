<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";//simec_desenvolvimento

// carrega as bibliotecas internas do sistema
require_once 'config.inc';
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/Grafico.php";
include_once APPRAIZ . "includes/library/simec/Indicadores.php";
require_once "funcoes.php";

$indid = 552;
$indicadores = new Simec_Indicadores($indid, false);
?>


<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">
    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

    <meta http-equiv='Content-Type' content='text/html; charset=ISO-8895-1'>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <!-- Mainly scripts -->
    <script src="/zimec/public/temas/simec/js/jquery-2.1.1.js"></script>
    <script src="/zimec/public/temas/simec/js/bootstrap.min.js"></script>
    <script language="JavaScript" src="/estrutura/js/funcoes.js"></script>

    <script language="javascript" src="/includes/Highcharts-4.0.3/js/highcharts.js"></script>
    <script language="javascript" src="/includes/Highcharts-4.0.3/js/modules/exporting.js"></script>

    <!-- Data picker -->
    <script src="/zimec/public/temas/simec/js/plugins/datapicker/bootstrap-datepicker.js"></script>

    <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
    <!--<script src="js/html5shiv.js"></script>-->
    <!--<script src="js/respond.min.js"></script>-->
    <![endif]-->

    <link href="/zimec/public/temas/simec/fonts/awesome/fontawesome.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/fonts/glyphicons/glyphicons.css" rel="stylesheet">

    <link href="/zimec/public/temas/simec/css/bootstrap.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/iCheck/custom.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/chosen/chosen.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/colorpicker/bootstrap-colorpicker.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/gritter/jquery.gritter.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/nouslider/jquery.nouislider.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/datapicker/datepicker3.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/plugins/bootstrap-switch/bootstrap-switch.css" rel="stylesheet">

    <link href="/zimec/public/temas/simec/css/plugins/nanoscroll/nanoscroller.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/animate.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/style.min.css" rel="stylesheet">
    <link href="/zimec/public/temas/simec/css/simec.css" rel="stylesheet">

    <link href='../includes/loading.css' rel='stylesheet'>
    <script src="../library/jquery/jquery-isloading.min.js" type="text/javascript"></script>


    <!-- Custom and plugin javascript -->
    <script src="/zimec/public/temas/simec/js/plugins/pace/pace.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/slimscroll/jquery.slimscroll.min.js"></script>

    <!-- Chosen -->
    <script src="/zimec/public/temas/simec/js/plugins/chosen/chosen.jquery.js"></script>

    <!-- JSKnob -->
    <script src="/zimec/public/temas/simec/js/plugins/jsKnob/jquery.knob.js"></script>

    <!-- Switcher -->
    <script src="/zimec/public/temas/simec/js/plugins/bootstrap-switch/bootstrap-switch.js"></script>

    <!-- NanoScroller -->
    <script src="/zimec/public/temas/simec/js/plugins/nanoscroll/jquery.nanoscroller.min.js"></script>

    <!-- iCheck -->
    <script src="/zimec/public/temas/simec/js/plugins/iCheck/icheck.min.js"></script>

    <!-- File Input -->
    <script src="/zimec/public/temas/simec/js/plugins/fileinput/fileinput.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/fileinput/fileinput_locale_pt-BR.js"></script>

    <!-- Masked Input -->
    <script src="/zimec/public/temas/simec/js/plugins/maskedinput/jquery.maskedinput.min.js"></script>

    <!-- Menu -->
    <script src="/zimec/public/temas/simec/js/plugins/metisMenu/jquery.metisMenu.js"></script>

    <!-- Datatables -->
    <script src="/zimec/public/temas/simec/js/plugins/dataTables/jquery.dataTables.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/dataTables/dataTables.bootstrap.js"></script>

    <!-- Gritter -->
    <script src="/zimec/public/temas/simec/js/plugins/gritter/jquery.gritter.min.js"></script>

    <!-- Bootbox -->
    <script src="/zimec/public/temas/simec/js/plugins/bootbox/bootbox.min.js"></script>

    <!-- Validate -->
    <script src="/zimec/public/temas/simec/js/plugins/validate/jquery.validate.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/validate/jquery.validate.extend.js"></script>

    <!-- Inspina -->
    <script src="/zimec/public/temas/simec/js/simec-indicadores.js"></script>

</head>
<body class="top-navigation  pace-done">

    <!-- begin loader -->
    <div class="loading-dialog notprint" id="loading" style="top: 0px;">
        <div id="overlay" class="loading-dialog-content" style="background-color: #000">
            <div class="ui-dialog-content">
                <img src="../library/simec/img/loading.gif">
                <span style="color: #fff !important; ">
                    O sistema esta processando as informações. <br/>
                    Por favor aguarde um momento...
                </span>
            </div>
        </div>
    </div>
    <!-- end loader -->

    <div class="pace  pace-inactive"><div class="pace-progress" data-progress-text="100%" data-progress="99" style="transform: translate3d(100%, 0px, 0px);">
            <div class="pace-progress-inner"></div>
        </div>
        <div class="pace-activity"></div>
    </div>

    <!-- Header -->
    <header id="top" class="header" style="margin-bottom: 0">
        <div class="row">
            <div class="col-lg-6 col-xs-6 col-sm-6" style="margin-top: 5px;">
                <div class="text-left">
                    <img src="../estrutura/temas/default/img/logo-simec.png" class="img-responsive" width="200">
                </div>
            </div>

            <div class="col-lg-6 col-xs-6 col-sm-6 pull-right" style="margin-top: 5px;">
                <a href="http://www.brasil.gov.br/" class="brasil pull-right">
                    <img style="margin-right: 10px;" src="http://portal.mec.gov.br/templates/mec2014/images/brasil.png"
                         alt="Brasil - Governo Federal" class="img-responsive">
                </a>
            </div>
        </div>
    </header>
    <div id="wrapper">
        <div id="page-wrapper" class="gray-bg" style="min-height: 452px;">
            <div class="">
                    <div class="row">

                        <div class="col-lg-6">
                            <div class="ibox float-e-margins">
                                <div class="ibox-title">
                                    <h5>Dados do Indicador</h5>
                                    <div class="ibox-tools">
                                        <a class="collapse-link">
                                            <i class="fa fa-chevron-up"></i>
                                        </a>
                                    </div>
                                </div>
                                <div class="ibox-content">
                                    <?php $indicadores->gerarResumoIndicador($indid); ?>
                                </div>
                            </div>
                        </div>
                        <div class="col-lg-6">
                            <div class="ibox float-e-margins">
                                <div class="ibox-title">
                                    <h5>Dados do Indicador</h5>
                                    <div class="ibox-tools">
                                        <a class="collapse-link">
                                            <i class="fa fa-chevron-up"></i>
                                        </a>
                                    </div>
                                </div>
                                <div class="ibox-content">
                                    <?php $indicadores->gerarGraficoComparativo(); ?>
                                </div>
                            </div>
                        </div>
                        <div class="col-lg-12">
                            <div class="ibox float-e-margins">
                                <div class="ibox-title">
                                    <h5>Dados do Indicador</h5>
                                    <div class="ibox-tools">
                                        <a class="collapse-link">
                                            <i class="fa fa-chevron-up"></i>
                                        </a>
                                    </div>
                                </div>
                                <div class="ibox-content">
                                    <?php $indicadores->gerarGraficoDetalhes(); ?>
                                </div>
                            </div>
                        </div>
                        <div class="col-lg-12">
                            <div class="ibox float-e-margins">
                                <div class="ibox-title">
                                    <h5>Dados do Indicador</h5>
                                    <div class="ibox-tools">
                                        <a class="collapse-link">
                                            <i class="fa fa-chevron-up"></i>
                                        </a>
                                    </div>
                                </div>
                                <div class="ibox-content">
                                    <?php $indicadores->gerarTabelaRelatorio($indid); ?>
                                </div>
                            </div>
                        </div>
                </div>
            </div>
            <div class="footer">
                <div class="pull-right">
                    10GB of <strong>250GB</strong> Free.
                </div>
                <div>
                    <strong>Copyright</strong> Example Company © 2014-2015
                </div>
            </div>

        </div>
    </div>

    <div class="theme-config">
        <div class="theme-config-box">
            <div class="spin-icon">
                <i class="fa fa-cogs fa-spin"></i>
            </div>
            <div class="skin-setttings">
                <div class="title">Configuration</div>
                <div class="setings-item">
                        <span>
                            Collapse menu
                        </span>

                    <div class="switch">
                        <div class="onoffswitch">
                            <input type="checkbox" name="collapsemenu" class="onoffswitch-checkbox" id="collapsemenu">
                            <label class="onoffswitch-label" for="collapsemenu">
                                <span class="onoffswitch-inner"></span>
                                <span class="onoffswitch-switch"></span>
                            </label>
                        </div>
                    </div>
                </div>
                <div class="setings-item">
                        <span>
                            Fixed sidebar
                        </span>

                    <div class="switch">
                        <div class="onoffswitch">
                            <input type="checkbox" name="fixedsidebar" class="onoffswitch-checkbox" id="fixedsidebar">
                            <label class="onoffswitch-label" for="fixedsidebar">
                                <span class="onoffswitch-inner"></span>
                                <span class="onoffswitch-switch"></span>
                            </label>
                        </div>
                    </div>
                </div>
                <div class="setings-item">
                        <span>
                            Top navbar
                        </span>

                    <div class="switch">
                        <div class="onoffswitch">
                            <input type="checkbox" name="fixednavbar" class="onoffswitch-checkbox" id="fixednavbar">
                            <label class="onoffswitch-label" for="fixednavbar">
                                <span class="onoffswitch-inner"></span>
                                <span class="onoffswitch-switch"></span>
                            </label>
                        </div>
                    </div>
                </div>
                <div class="setings-item">
                        <span>
                            Boxed layout
                        </span>

                    <div class="switch">
                        <div class="onoffswitch">
                            <input type="checkbox" name="boxedlayout" class="onoffswitch-checkbox" id="boxedlayout">
                            <label class="onoffswitch-label" for="boxedlayout">
                                <span class="onoffswitch-inner"></span>
                                <span class="onoffswitch-switch"></span>
                            </label>
                        </div>
                    </div>
                </div>
                <div class="setings-item">
                        <span>
                            Fixed footer
                        </span>

                    <div class="switch">
                        <div class="onoffswitch">
                            <input type="checkbox" name="fixedfooter" class="onoffswitch-checkbox" id="fixedfooter">
                            <label class="onoffswitch-label" for="fixedfooter">
                                <span class="onoffswitch-inner"></span>
                                <span class="onoffswitch-switch"></span>
                            </label>
                        </div>
                    </div>
                </div>

                <div class="title">Skins</div>
                <div class="setings-item default-skin">
                        <span class="skin-name ">
                             <a href="#" class="s-skin-0">
                                 Default
                             </a>
                        </span>
                </div>
                <div class="setings-item blue-skin">
                        <span class="skin-name ">
                            <a href="#" class="s-skin-1">
                                Blue light
                            </a>
                        </span>
                </div>
                <div class="setings-item yellow-skin">
                        <span class="skin-name ">
                            <a href="#" class="s-skin-3">
                                Yellow/Purple
                            </a>
                        </span>
                </div>
            </div>
        </div>
    </div>
    <script>
        // SKIN Select
        $('.spin-icon').click(function () {
            $(".theme-config-box").toggleClass("show");
        });

        /** Função de fazer com que o sistema informe que esta havendo uma requisição ajax */
        $(document).ajaxSend(function (e, jqxhr, settings) {
            jQuery("#loading").fadeIn();

        }).ajaxStop(function(){
            jQuery("#loading").fadeOut();
        });

        /** Mensagem de carregando quando houver requisições em ajax */
        $.ajaxSetup({
            timeout: 0,
            error: function(xhr, status, error) {
                console.log("Ocorrência de erro no retorno AJAX: " + status + "\nError: " + error);
                jQuery("#loading").fadeOut();
                jQuery("#loading").fadeIn();

                setTimeout(function(){ jQuery("#loading").fadeOut();}, 1300);
            }
        });


    </script>
</body>
</html>

<?php
function montarUfs($regioes = array())
{
    global $db;

    $where = is_array($regioes) && count($regioes) ? " where regcod in ('" . implode("', '", $regioes) . "') " : '';
    ?>

    <table>
        <tr>
            <?php
            $sql = "SELECT	estuf, estdescricao
                    FROM territorios.estado
                    $where
                    ORDER BY estuf ";
            $arrDados = $db->carregar($sql);

            foreach ($arrDados as $dados) {
                $active = is_array($_POST['ufs']) && in_array($dados['estuf'], $_POST['ufs']);
                ?>
                <td>
                    <div class="btn-group" data-toggle="buttons">
                        <label class="btn btn-default <?php echo $active ? 'active' : ''; ?>">
                            <input type="checkbox" class="checkbox-uf" name="estuf[]" autocomplete="off" value="<?php echo $dados['estuf'];?>" <?php echo $active ? 'checked="checked"' : ''; ?>>
                            <img width="15px" src="/imagens/bandeiras/mini/<?php echo $dados['estuf']; ?>.png"><br>
                            <div>
                                <?php echo $dados['estuf'];?>
                            </div>
                        </label>
                    </div>
                </td>
            <?php } ?>
        </tr>
    </table>

<?php }

function montarMunicipio($requisicao = array())
{
    global $db;
    $join = array();

    if((isset($requisicao['estuf']) && is_array($requisicao['estuf'])) || (isset($requisicao['gtmid']) && is_array($requisicao['gtmid'])) || (isset($requisicao['tpmid']) && is_array($requisicao['tpmid']))){
        $where = ' where true ';

        if (isset($requisicao['estuf']) && is_array($requisicao['estuf'])) {
            $where .= " and m.estuf in ('" . implode ("', '", $requisicao['estuf']) . "') ";
        }
        if (isset($requisicao['tpmid']) && is_array($requisicao['tpmid'])) {
            $where .= " and mtm.tpmid in ('" . implode ("', '", $requisicao['tpmid']) . "') ";
            $join[] = ' inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod ';
        } elseif (isset($requisicao['gtmid']) && is_array($requisicao['gtmid'])) {
            $where .= " and tm.gtmid in ('" . implode ("', '", $requisicao['gtmid']) . "') ";
            $join[] = ' inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod ';
            $join[] = ' inner join territorios.tipomunicipio tm on tm.tpmid = mtm.tpmid ';
        }
    } else {
        $where = ' where false ';
    }


    $sql = "select distinct m.muncod, m.estuf, m.mundescricao
            from territorios.municipio m
            " . implode(' ', $join).  "
            $where
            order by m.estuf, m.mundescricao";

    $municipios = $db->carregar($sql);
    $municipios = $municipios ? $municipios : array();
    ?>

    <select name="muncod[]" id="muncod" class="form-control chosen-select" multiple data-placeholder="Selecione">
        <?php foreach ($municipios as $dado) { ?>
            <option <?php echo is_array($requisicao['muncod']) && in_array($dado['muncod'], $requisicao['muncod']) ? 'selected="selected"' : ''; ?> value="<?php echo $dado['muncod']; ?>"><?php echo $dado['estuf'] . ' - ' . $dado['mundescricao']; ?></option>
        <?php } ?>
    </select>
<?php }
?>
