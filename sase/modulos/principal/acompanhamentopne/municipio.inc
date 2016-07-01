<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 26/08/2015
 * Time: 15:16
 */
global $db;

// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
require_once APPRAIZ . "includes/library/simec/Listagem.php";

include APPRAIZ . 'includes/cabecalho.inc';

$pneano = 2010;

$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$titulo = 'Acompanhamento PNE - Município';

if($_POST['acao']){
    switch($_POST['acao']){
        case 'carrega_legenda':
            ob_clean();
            echo getLegendaAcompanhamentoPne($_POST['subid']);
            exit;

        case 'carrega_tabela':
            ob_clean();
            $cor = '';
//            ver('Teste', d);
            $arrAnos = array(2015, 2025);
            $muncod = $_POST['muncod'];
            $subid  = $_POST['subid'];
            $metid  = $_POST['metid'];
            $estuf  = $_POST['estuf'];
            $pneano = $_POST['pneano'] ? $_POST['pneano'] : $pneano;

            $sql = <<<DML
                select legcor, legfxa1, legfxa2 from sase.legendaindicadores where subid = {$subid}
DML;
            $cores = $db->carregar($sql);

            $where = array();
//            if($subid  != '') { $where[]  = " tmun.subid = {$subid} "; }
            if($muncod != '' && $muncod != '0') { $where[]  = " tmun.muncod in ('".implode("','", $muncod)."') "; }
//            if($metid  != '') { $where[]  = " sub.metid = {$metid} "; }
            if($estuf != '' && $estuf != '0') { $where[]  = " mun.estuf in ('".implode("','", $estuf)."') "; }
//            if($pneano != '') { $where[]  = " pne.pneano = '{$pneano}'"; }

            $where = is_array($where) && count($where) > 0 ? 'and '.implode('and', $where) : '';
            $html = "";
            $sql = <<<DML
                with temp_mun as (select pnevalor, pnevalormeta, muncod, pneano from sase.pne where subid = {$subid} and pnetipo = 'M')
                select
                    est.estuf,
                    estdescricao,
                    mun.muncod,
                    mun.mundescricao,
                    tmun.pneano,
                    tmun.pnevalor,
                    tmun.pnevalormeta
                from territorios.estado est
                inner join territorios.municipio mun on est.estuf = mun.estuf
                inner join temp_mun tmun on tmun.muncod = mun.muncod
                where 1=1
                {$where}
                order by est.estdescricao, tmun.pneano, mun.mundescricao
DML;
//ver($sql);
            $res = $db->carregar($sql);
            $dados = array();
            if($res) {
//                ver($res, d);
                $dados = array();
                $estado = "";
                $municipio = "";
//ver($dados);
//                ver($res);
                switch ($metid) {
                    case(1): case(2): case(3): case(5): case(8): case(9): $pneano = 2010; break;
                    case(4): $pneano = 2010; break;
                    case(12): case(13): case(14): case(17): $pneano = 2012; break;
                    default: $pneano = 2013; break;
                }

                function retornaCor($v, $k, $i){
                    global $cor;
                    if($i != '') {
                        if ($v['legfxa1'] <= $i && $v['legfxa2'] >= $i) {
                            $cor = $v['legcor'];
                        }
                    } else {
                        $cor = '#FFFFFF';
                    }
                    return;
                };
                $coresEscuras = array(
                    '#3333FF',
                    '#006600'
                );
                $cabecalho = array();
                foreach ($res as $r) {
                    if(!isset($dados[$r['muncod']])){
                        $dados[$r['muncod']] = array(
                            'Estado' => $r['estdescricao'],
                            'Município' => $r['mundescricao']
                        );
                        $a = $arrAnos[0];
                        $dados[$r['muncod']][$pneano] = round($r['pnevalor'], 2);
                        array_walk($cores, 'retornaCor', $r['pnevalor']);
                        while ($a <= $arrAnos[1]) {
                            $dados[$r['muncod']][$a] = '';
                            $a++;
                        }
                    }
                    if(isset($dados[$r['muncod']][$r['pneano']])) {
                        if($r['pneano'] == $pneano){
                            if($r['pnevalor'] != ''){
                                $dados[$r['muncod']][$r['pneano']] = round($r['pnevalor'], 2);
                            } else {
                                $dados[$r['muncod']][$r['pneano']] = '';
                            }
                        } else {
                            if($r['pnevalormeta'] != ''){
                                $dados[$r['muncod']][$r['pneano']] = round($r['pnevalormeta'], 2);
                            } else {
                                $dados[$r['muncod']][$r['pneano']] = '';
                            }
                        }
                        //$dados[$r['muncod']][$r['pneano']] = $r['pneano'] == $pneano ? round($r['pnevalor'], 2) : round($r['pnevalormeta'], 2);
                    }
                    if(count($cabecalho) == 0){
                        $cabecalho = array_keys($dados[$r['muncod']]);
                    }
                }

                $c = array(
                    $cabecalho[0],
                    $cabecalho[1]
                );

                array_shift($cabecalho);
                array_shift($cabecalho);
                $c['Situação'] = $cabecalho;

                $lista = new Simec_Listagem();
                $lista
                    ->setDados($dados)
                    ->setCabecalho($c)
                    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            }
            exit;
    }
}

// Filtra os municipios
if( $_GET['filtroAjax'] && isset($_GET['estuf']) ){
    ob_clean();
    $sql = "select muncod, mundescricao from territorios.municipio";

    if ($_GET['estuf'] && $_GET['estuf'] != '0'){
        if(is_array($_GET['estuf'])) {
            $sql .= " where estuf in ('" . implode("','", $_GET['estuf']) . "') ";
        }
    }
    $sql .= " order by mundescricao asc ";
    $municipios = $db->carregar($sql);
    ?>
    <select name="muncod" class="form-control" id="muncod" multiple="multiple" onchange="carregarTabela()">
        <option value="">Todos</option>
        <?php
        foreach( $municipios as $chave => $municipio ){ ?>
            <option value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
        <?php }?>
    </select>
    <?php die();}


// Filtra os indicadores


if( $_GET['filtroAjax'] && $_REQUEST['metid'] ){
    ob_clean(); $indicadores =
    $sql = "select subid as codigo, subtitulo as descricao From sase.submeta";
    if( $_REQUEST['metid'] ) $sql .= " where metid = '" . $_REQUEST['metid'] . "' ";
    $sql .= " order by subtitulo asc ";
    $indicadores = $db->carregar($sql);
    ?>

    <select name="subid" class="form-control" id="subid" onchange="carregaLegenda()">
        <option value="">Filtrar indicador:</option>
        <?php
        foreach( $indicadores as $chave => $indicador ){ ?>
            <option value="<?=$indicador['codigo']?>"><?=$indicador['descricao']?></option>
        <?php }?>
    </select>
    <?php die();}

?>
<script>
    function filtraMunicipios(){
        estuf = '0';
        var est = jQuery('#estuf').val();
        if(Array.isArray(est)) {
            if (est[0] == "") {
                if (est.length > 1) {
                    est.shift();
                    estuf = est;
                }
            } else {
                estuf = est;
            }
        }
        console.log(estuf);
        jQuery.ajax({
            url:'',
            type:'GET',
            data:{filtroAjax:true,estuf:estuf},
            success: function( resposta ){
                jQuery('#divFiltroMunicipio .inputarea').html( resposta );
                jQuery('#muncod').multiselect('enable');
            }
        });
    }
    function filtraIndicadores(metid){
        jQuery.ajax({
            url:'',
            type:'GET',
            data:{filtroAjax:true,metid:metid},
            success: function( resposta ){
                jQuery('#divFiltroIndicador .inputarea').html( resposta );
                jQuery('#estuf').multiselect('enable');
                jQuery('#estuf').multiselect({
                    numberDisplayed: 1,
                    id: 'estuf'
                });
                jQuery('#todosMuncod').removeAttr('disabled');
            }
        });
    }

    function carregaLegenda(){
        subid = jQuery('#subid').val();
        jQuery.ajax({
            utl: '',
            type: 'POST',
            data: {acao: 'carrega_legenda', subid:subid},
            success: function(data){
                jQuery('#legendaMapa').html(data);
                jQuery("#estuf").removeAttr('disabled');
                jQuery('#estuf').multiselect('enable');
                jQuery('#estuf').multiselect({
                    numberDisplayed: 1,
                    id: 'estuf'
                });
                jQuery('#todosEstuf').removeAttr('disabled');
//                if(jQuery("#estuf").val() != ''){ carregaMapa(); }
//                if(jQuery('#muncod').val() != ''){ carregarTabela() }
            }
        });
    }

    function carregaMapa(){
        Mapas.buscaEstadoForm( jQuery("#estuf"), 'acompanhamento-pne-municipio' );
    }

    function carregarTabela(todos, pagina){
        var mun = jQuery('#muncod').val();
        var est = jQuery('#estuf').val();
        subid  = jQuery('#subid').val();
        todos = typeof todos === 'undefined' ? false : true;
        pagina = typeof pagina === 'undefined' ? false : pagina;
        muncod = '0';
        estuf = '0';
        console.log(Array.isArray(est), est.length, est, est[0]);
        if(Array.isArray(mun)) {

            if(mun[0] == ""){
                if(mun.length > 1){
                    mun.shift();
                    muncod = mun;
                }
            } else {
                muncod = mun;
            }

            if(est[0] == ""){
                if(est.length > 1){
                    est.shift();
                    estuf = est;
                }
            } else {
                estuf = est;
            }

            metid  = jQuery('#metid').val();
            var dados = {
                acao:'carrega_tabela',
                subid:subid,
                muncod:muncod,
                estuf:estuf,
                metid:metid
            };

            if(pagina){
                dados['listagem[p]'] = pagina;
            }

    //        carregaLegenda();
    //        Mapas.atualizaLegenda( jQuery("#estuf"), 'acompanhamento-pne-municipio-legenda' )
            jQuery.ajax({
                url: '',
                type: 'POST',
                data: dados,
                success: function(dados){
                    jQuery('#divTabela').html(dados);
    //                console.log(d)
                }
            });
        }
    }

    function toggleSelect(sel){
        if (multiselect_selected(sel)) {
            sel.multiselect('deselect_all');
            //multiselect_deselectAll(sel);
//            $btn.text("Selecionar Todos");
        } else {
            sel.multiselect('selectAll', false);
            //multiselect_selectAll(sel);
//            $btn.text("Desselecionar Todos");
        }
    }

</script>

<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<!-- dependencias -->
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<!-- http://hpneo.github.io/gmaps/ -->
<script type="text/javascript" src="/../includes/gmaps/gmaps.js"></script>
<script src="/sase/js/Mapas.js"></script>
<!-- http://www.downscripts.com/bootstrap-multiselect_javascript-script.html -->
<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<script src="/sase/js/functions.js"></script>
<!-- /dependencias -->

<style>
    .table th, td{
        padding: 0px !important;
        text-align: center !important;
        vertical-align: middle !important;
    }
    .table th{
        border-top: none !important;
    }
    .table table{
        width: 100% !important;
    }
    #divMapa{
        margin-bottom: 0px !important;
    }
    #map_canvas{
        height: 540px !important;
    }
    #legendaMapa{
        padding-top: 0px !important;
    }
    #map_canvastxt{
        float: none;
    }
    .txts{
        position: inherit !important;
        float: right !important;
        background-color: transparent !important;
    }
    .btn-todos{
        margin-bottom: 3px;
        padding: 0px 12px !important;
    }
</style>

<div id="container">
    <form class="form-horizontal" name="formMapa" id="formMapa" role="form" method="POST">
        <input type="hidden" name="acao" id="acao"/>

        <div class="row">
            <div class="col-lg-2">
                <div class="well">
                    <fieldset>
                        <legend>Filtros</legend>

                        <div class="form-group">
                            <label for="filtroEstado" class="col-lg-12">Ano Previsto:</label>
                            <div class="col-lg-12">
                                <select name="pneano" id="pneano" class="form-control" onchange="carregaMapa()">
                                    <option value="2010">2010</option>
                                    <?php
                                    $anos = array(2015, 2025);
                                    $a = $anos[0];
                                    while($a <= $anos[1]){
                                        $sel = $dadosMun['anoprevisto'] == $a ? 'selected' : '';
                                        echo "<option value=\"".$a."\" {$sel}>".$a."</option>";
                                        $a++;
                                    }?>
                                </select>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="filtroEstado" class="col-lg-12">Meta:</label>
                            <div class="col-lg-12">
                                <select name="metid" id="metid" class="form-control" onchange="javascript:if(this.value!=''){filtraIndicadores(this.value);}">
                                    <option value="">Filtrar meta</option>
                                    <?php
                                    $sql = "select metid as codigo, 'Meta '||metid as descricao from sase.meta order by metid";
                                    $estados = $db->carregar($sql);
                                    foreach( $estados as $chave => $estado ){ ?>
                                        <option value="<?=$estado['codigo']?>"><?=$estado['descricao']?></option>
                                    <?php }?>
                                </select>
                            </div>
                        </div>

                        <div class="form-group" id="divFiltroIndicador">
                            <label for="filtroMunicipio" class="col-lg-12">Indicador:</label>
                            <div class="col-lg-12 inputarea">
                                <?php $dis = ''; if(!$_POST['metid']){ $dis = 'disabled'; } ?>
                                <select name="subid" <?= $dis ?> class="form-control" id="subid" onchange="carregaLegenda()">
                                    <option value="">Filtrar indicador</option>
                                </select>
                            </div>
                        </div>

                        <div class="form-group" id="divFiltroEstado">
                            <label for="filtroEstado" class="col-lg-12">Estado:</label>
                            <div class="col-lg-12">
                                <select name="estuf" id="estuf" class="form-control" multiple="multiple" disabled onchange="javascript:carregaMapa(); filtraMunicipios();">
                                    <option value="">Todos</option>
                                    <?php
                                    $sql = "select estuf, estdescricao from territorios.estado order by estdescricao ASC";
                                    $estados = $db->carregar($sql);
                                    foreach( $estados as $chave => $estado ){ ?>
                                        <option <?=($estado['estuf']==$_POST['estuf'])?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
                                    <?php }?>
                                </select>
                            </div>
                        </div>

                        <div class="form-group" id="divFiltroMunicipio">
                            <label for="filtroMunicipio" class="col-lg-12">Município:</label>
                            <div class="col-lg-12 inputarea">
                                <?php $dis = ''; if(!$_POST['estuf']){ $dis = 'disabled'; } ?>
                                <select name="muncod" <?= $dis ?> class="form-control" id="muncod" multiple="multiple" onchange="carregarTabela()">
                                </select>
                            </div>
                        </div>

                    </fieldset>
                </div>
            </div>

            <div class="col-lg-10">
                <div class="well">
                    <fieldset>
                        <legend><div id="map_canvastxt"><?= $titulo ?></div></legend>
                        <div class="form-group" id="divMapa">
                            <div class="row">
                                <div id="local-mapa" class="col-lg-10 col-sm-10">
                                    <div class="panel panel-default">
                                        <div class="panel-body">
                                            <div id="directionsPanel" style="width: 300px"></div>
                                            <div id="map_canvas"></div>
                                        </div>
                                    </div>
                                </div>
                                <div class="col-lg-2 col-sm-2 panel panel-default">
                                    <fieldset>
                                        <legend>Legenda</legend>
                                        <div id="legendaMapa">

                                        </div>
                                    </fieldset>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="row">
                                <div class="col-lg-12 col-sm-12">
                                    <?php
                                    $anos = array(2015, 2025);
                                    ?>
                                    <div class="panel panel-default" id="divTabela">
<!--                                        <table class="table">-->
<!--                                            <tr>-->
<!--                                                <th width="20%" height="15">Estado</th>-->
<!--                                                <th width="20%" height="15">Município</th>-->
<!--                                                <th width="60%" height="15">Situação</th>-->
<!--                                            </tr>-->
<!--                                            <tr>-->
<!--                                                <td colspan="3" height="15" style="text-align: center;">-->
<!--                                                    Sem Registro-->
<!--                                                </td>-->
<!--                                            </tr>-->
<!--                                        </table>-->
                                    </div>
                                </div>
                            </div>
                        </div>
                    </fieldset>
                </div>
            </div>
        </div>

    </form>
</div>
<script>
    jQuery('documento').ready(function(){
        Mapas.inicializar( '#map_canvas' );
        jQuery('#estuf').multiselect({
            numberDisplayed: 1,
            id: 'estuf'
        });
        jQuery('#muncod').multiselect({
            numberDisplayed: 1,
            id: 'muncod'
        });
    });

    delegatePaginacao = function()
    {
        $('body').on('click', '.container-listing li[class="pgd-item"]:not(".disabled")', function(){
            // -- definindo a nova página
            var novaPagina = $(this).attr('data-pagina');
            // -- Submetendo o formulário
            carregarTabela(true, novaPagina);
        });
    };
</script>