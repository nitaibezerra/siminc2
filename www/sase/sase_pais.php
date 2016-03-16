<?php

// exit('<img src="texto_nome_entidade_territorio.php?estuf=GO"/>');
$_REQUEST['baselogin'] = "simec_espelho_producao";

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../' ) );

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf']){
    $_SESSION['usucpforigem'] = '';
    $auxusucpf = '';
    $auxusucpforigem = '';
    $_SESSION['sisid'] = 183;
}else{
    $auxusucpf = $_SESSION['usucpf'];
    $auxusucpforigem = $_SESSION['usucpforigem'];
}


include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

$db = new cls_banco();

// Classes
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'sase/classes/Mapa/MetaDados.class.inc';
include_once APPRAIZ . 'sase/classes/Mapa/Poligonos.class.inc';
include_once APPRAIZ . 'sase/classes/Mapa/Mapas.class.inc';
include_once APPRAIZ . 'sase/classes/Assessoramento.class.inc';
include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
include_once APPRAIZ . 'sase/classes/QuestoesPontuaisPar.class.inc';
include_once APPRAIZ . 'sase/classes/Territorio.class.inc';

get_header2();

if ($_REQUEST['acao']){
    switch($_REQUEST['acao']) {
        case 'montaBalao':
            $estuf = $_REQUEST['estuf'];
            $sql = "select aseleipne from sase.assessoramentoEstado where estuf = '{$estuf}'";
            $arqid = $db->pegaUm($sql);
            if (empty($arqid) && $arqid == '') {
                $attr = 'disabled';
            }
            echo <<<HTML
            <link rel='StyleSheet' href="../../library/bootstrap-3.0.0/css/bootstrap.css" type="text/css" media='screen'/>
            <script src="../library/jquery/jquery-1.11.1.min.js" type="text/javascript" charset="ISO-8895-1"></script>
            <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>
            <script>
                function abrirMapa(estuf){
                    window.open(
                        'http://simec-local/sase/sase_mapas.php?uf='+estuf,
                        'estado',
                        'width=804px, height=631px, scrollbars=no, status=no, toolbar=no, menubar=no, resizable=no, fullscreen=no');
                }

                function baixarLei(arqid){
                    jQuery('[name=acao]').val('download');
                    jQuery('[name=arqid]').val(arqid);
                    jQuery('[name=formCadastroLista]').submit();
                }
            </script>
            <form id="form-save" method="post" name="formCadastroLista" role="form" class="form-horizontal">
                <input type="hidden" name="acao" id="acao" />
                <input type="hidden" name="arqid" id="arqid" />
                <input type="button" name="btnSalvar" id="btnSalvar" onclick="abrirMapa('{$estuf}')" value="Municípios" class="btn btn-primary"/>
                <input type="button" name="btnSalvar" id="btnSalvar" {$attr} onclick="baixarLei('{$arqid}')" value="Lei PNE" class="btn btn-primary"/>
            </form>
HTML;
            exit;

        case 'download':
            ob_clean();
            $estuf = $_REQUEST['estuf'];
            $sql = "select aseleipne from sase.assessoramentoEstado where estuf = '{$estuf}'";
            $arqid = $db->pegaUm($sql);

            if ($arqid == '') {
                echo "		<script>
		                    alert('Lei PNE nao registrado.');
							//window.location.href = 'sase_pais.php';
							window.close();
						</script>";
            } else {
                $caminho = APPRAIZ . 'arquivos/sase/' . floor($arqid / 1000) . '/' . $arqid;
                if (!is_file($caminho)) {
                    echo "		<script>
		                    alert('Arquivo não encontrado.');
							//window.location.href = 'sase_pais.php';
							window.close();
						</script>";
                } else {
                    $sql = "SELECT * FROM public.arquivo WHERE arqid = " . $arqid;
                    $arquivo = $db->carregar($sql);
                    $filename = str_replace(" ", "_", $arquivo['arqnome'] . '.' . $arquivo['arqextensao']);


                    header('Content-type: ' . $arquivo['arqtipo']);
                    header('Content-Disposition: inline; filename=' . $filename);
                    readfile($caminho);
                }
            }
            exit();

            break;
    }
}

if (!defined('LINK_PADRAO')) { define('LINK_PADRAO', 'sase/sase_pais.php'); }
?>

    <style> .conteudo-sase{ width:100%; } </style>

    <!-- SASE -->
    <link rel='stylesheet' href="/sase/css/estilo_alternativo.css" type="text/css" media='screen'/>
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.min.css">
    <link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.css">
    <link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" media="screen">

    <!-- dependencias -->
    <script type="text/javascript" src="../library/bootstrap-3.0.0/js/bootstrap.min.js" charset="utf-8"></script>
    <script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
    <script type="text/javascript" src="http://google-maps-utility-library-v3.googlecode.com/svn/trunk/infobox/src/infobox.js"></script>
    <script type="text/javascript" src="/../includes/gmaps/gmaps.js"></script>

    <script src="/sase/js/Mapas.js"></script>
    <script src="/sase/js/jquery.blockUI.js"></script>

<?php
$width = '684';
$height = '510';
$width2 = '290';
?>
    <style>
        body{
            padding:0px;
            margin:0px;
            background: #f2f2f2;
            width: <?= $width ?>px;
        }
        #map_canvas{
            width:<?=$width?>px;
            height:<?=$height?>px;
            padding:0px;
            margin:0px;
            background: #f2f2f2 !important;
            position:absolute;
            border:none;
        }
        #legendaMapa{
            text-align: right;
            margin-left: 275px;
            position: relative;
            top: 220px !important; /* top: 290px !important; */
        }
        #containerPais{
            width:<?=$width2?>px; /* 535 */
            height:<?=$height?>;
            background: #f2f2f2;
            z-index: 1 !important;
        }
        .btn-mapa-type {
            width: 40px;
        }
        .btn-mapa-search {
            height: 28px;
            font-size: 11px;
        }
        .btn-toggle-Mapa{
            z-index: 1;
            position: absolute;
            margin-left: <?=$width-230?>;
            top: 10px;
        }
        .btn-Mapa{
            position: absolute;
            z-index: 1;
            width: 86px;
        }
        #loading{
            background-color: transparent;
        }
    </style>

    <div class="loading-dialog" id="loading" style="display: none;">
        <div id="overlay" class="loading-dialog-content">
            <div class="ui-dialog-content">
                <img src="../library/simec/img/loading.gif">
            </div>
        </div>
    </div>

    <form id="form-save" method="post" name="formCadastroLista" role="form" class="form-horizontal">
        <div style="float: right;">
            <input type="hidden" id="uf" value=""/>
            <input type="hidden" id="estuf" name="estuf" value=""/>
            <input type="hidden" id="acao" name="acao" value=""/>
            <input type="hidden" id="hidAcaoMapa" value="est"/>
        </div>
        <div id="containerPais">
            <div id="legendaMapa" class="legendaMapaAlternativo">
            </div>
            <div id="map_canvas"></div>
        </div>
        <div class="btn-group btn-toggle-Mapa">
            <button data-toggle="tooltip" data-placement="bottom" title="Vizualização por estados" type="button" class="btn btn-default btn-mapa-type est" disabled="disabled"><i class="fa fa-map-o"></i></button>
            <button data-toggle="tooltip" data-placement="bottom" title="Vizualização por municípios" type="button" class="btn btn-default btn-mapa-type mun"><i class="fa fa-map-marker"></i></button>
            <button type="button" class="btn btn-default btn-mapa-search"><i class="fa fa-search"></i> Pesquisar Lei PEE/PME</button>
        </div>
    </form>

    <script>
        function abrirMapa(estuf) {
            window.open(
                'sase_mapas.php?uf='+estuf,
                'estado',
                'width=804px, height=631px, scrollbars=no, status=no, toolbar=no, menubar=no, resizable=no, fullscreen=no');
        }

        function abrirFiltro() {
            window.open(
                'sase_filtro.php',
                'filtrar',
                'width=804px, height=631px, scrollbars=no, status=no, toolbar=no, menubar=no, resizable=no, fullscreen=no');
        }

        function baixarLei(estuf) {
            window.open("sase_pais.php?acao=download&estuf="+estuf, "_blank");
        }

        function baixarLeiEstado(estuf) {
            window.open("sase_pais.php?acao=downloadEstado&estuf="+estuf, "_blank");
        }

        html_municipio = "<div id=\"infobox\" style=\"padding:5px\" ><iframe src=\"sase_pais.php?acao=montaBalao&estuf={estuf}\" frameborder=0 scrolling=\"auto\" height=\"48px\" width=\"220px\" ></iframe></div>";

        var MapaPais = Mapas;
        var TipoMapa = 'est';
        var AcaoMapa = 'est';

        jQuery('documento').ready(function() {
            $('[data-toggle="tooltip"]').tooltip();

            MapaPais.estilo = 'externo_blank_pais';
            MapaPais.inicializar( '#map_canvas' );
            MapaPais.buscaEstadoForm( '#uf', 'pais-externo' );
            MapaPais.atualizaLegenda( '#uf', 'pais-legenda-externo' );

            $(".btn-mapa-type").click(function() {
                if (!$(this).is(":disabled")) {
                    if (TipoMapa == 'est'){
                        MapaPais.buscaEstadoForm( '#uf', 'municipio-externo' );
                        MapaPais.atualizaLegenda( '#uf', 'municipio-legenda-externo' );
                        $('.est').attr('disabled', null).addClass('link');
                        $('.mun').attr('disabled', 'disabled').removeClass('link');
                        TipoMapa = 'mun';
                    } else {
                        MapaPais.buscaEstadoForm('#uf', 'pais-externo');
                        MapaPais.atualizaLegenda('#uf', 'pais-legenda-externo');
                        $('.mun').attr('disabled', null).removeClass('link');
                        $('.est').attr('disabled', 'disabled').addClass('link');
                        TipoMapa = 'est';
                    }
                }
            });

            $(".btn-mapa-search").click(function() {
                abrirFiltro();
            });

            $(".btn-Mapa").click(function() {
                if (AcaoMapa == 'est'){
                    $('#hidAcaoMapa').val('mun');
                    $(this).attr('value', 'Leis PEE');
                    AcaoMapa = 'mun';
                } else {
                    $('#hidAcaoMapa').val('est');
                    $(this).attr('value', 'Leis PME');
                    AcaoMapa = 'est';
                }
            });

        });

    </script>

<?php
// get_footer();