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

if(!isset($_GET['tpdid'])){
    $_GET['tpdid'] = 238;
    $title = 'Visualizar por Adequação';
} else {
    $title = '';
    switch ($_GET['tpdid']){
        case 238:
            $title = 'Visualizar por Adequação';
            break;
        case 240:
            $title = 'Visualizar por Plano de Carreira';
            break;
    }
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

if (!defined('LINK_PADRAO')) { define('LINK_PADRAO', 'sase/sase_mapa_planodecarreira.php'); }
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
            top: 230px !important; /* top: 290px !important; */
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
            <input type="hidden" id="tpdid" name="tpdid" value="<?= $_GET['tpdid'] ?>"/>
            <input type="hidden" id="acao" name="acao" value=""/>
            <input type="hidden" id="hidAcaoMapa" value="est"/>
        </div>
        <div id="containerPais">
            <div id="legendaMapa" class="legendaMapaAlternativo">
            </div>
            <div id="map_canvas"></div>
        </div>
        <div class="btn-group btn-toggle-Mapa">
<!--            <button data-toggle="tooltip" data-placement="bottom" title="Vizualização por estados" type="button" class="btn btn-default btn-mapa-type est" disabled="disabled"><i class="fa fa-map-o"></i></button>-->
<!--            <button data-toggle="tooltip" data-placement="bottom" title="Vizualização por municípios" type="button" class="btn btn-default btn-mapa-type mun"><i class="fa fa-map-marker"></i></button>-->
            <button type="button" data-toggle="tooltip" data-placement="bottom" data-original-title="<?= $title ?>" class="btn btn-default btn-mapa-search">Adequação</button>
        </div>
    </form>

    <script>
        var MapaPais = Mapas;
        var TipoMapa = 'est';
        var AcaoMapa = 'est';
        var tpdid = <?= isset($_GET['tpdid']) ? $_GET['tpdid'] : 238 ?>;

        jQuery('documento').ready(function() {
            $('[data-toggle="tooltip"]').tooltip();

            MapaPais.estilo = 'externo_blank_pais';
            MapaPais.inicializar( '#map_canvas' );
            MapaPais.buscaEstadoForm( '#uf', 'planocarreira-estado-externo' );
            MapaPais.atualizaLegenda( '#uf', 'planocarreira-estado-legenda-externo' );

            $(".btn-mapa-search").click(function() {
                //abrirFiltro();
                switch (tpdid){
                    case 238:
                        $(this).html('Plano de Carreira');
                        $(this).attr('data-original-title', 'Visualizaçao por Plano de Carreira');
                        tpdid = 240;
                        break;
                    case 240:
                        $(this).html('Adequação');
                        $(this).attr('data-original-title', 'Visualizaçao por Adequação');
                        tpdid = 238;
                        break;
                }
                $('#tpdid').val(tpdid);
                MapaPais.buscaEstadoForm( '#uf', 'planocarreira-estado-externo' );
                MapaPais.atualizaLegenda( '#uf', 'planocarreira-estado-legenda-externo' );
            });

        });

    </script>

<?php
// get_footer();