<?php

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include APPRAIZ. 'includes/classes/relatorio.class.inc';

monta_titulo( $titulo_modulo, '' );

function mostra_array1() {
    global $db;
    $res = "";
    $sql1 = "SELECT
  ctsituacaocontrato.sitdsc,
  count(ctsituacaocontrato.sitdsc) as qtd
FROM
  contratos.ctcontrato,
  contratos.ctsituacaocontrato
WHERE
  ctsituacaocontrato.sitid = ctcontrato.sitid
group by
  ctsituacaocontrato.sitdsc";

    $rs1 = $db->carregar($sql1);

    for ($x = 0; $x < count($rs1); $x++) {
        $res .= "['" . $rs1[$x]['sitdsc'] . "'," . $rs1[$x]['qtd'] . "],";
    }
    echo substr($res, 0, -1);
}

function mostra_array2() {
    global $db;
    $res = "";
    $sql1 = "SELECT
  estadodocumento.esddsc,
  sum(faturacontrato.ftcvalor) as valor
FROM
  contratos.faturacontrato,
  workflow.estadodocumento,
  workflow.documento
WHERE
  documento.docid = faturacontrato.docid AND
  documento.esdid = estadodocumento.esdid
group by
  estadodocumento.esddsc";

    $rs1 = $db->carregar($sql1);

    for ($x = 0; $x < count($rs1); $x++) {
        $res .= "['" . $rs1[$x]['esddsc'] . "'," . $rs1[$x]['valor'] . "],";
    }
    echo substr($res, 0, -1);
}
?>
<script src="/includes/JQuery/jquery-1.10.2.min.js"></script>
<script src="/includes/Highcharts-3.0.0/js/highcharts4.js"></script>
<script type="text/javascript">

    function selecionarContrato( ctrid )
    {
        window.location.href = 'contratos.php?modulo=principal/cadContrato&acao=A&ctrid='+ctrid;
    }

    $(function () {
        $('#container').highcharts({
            chart: {
                plotBackgroundColor: null,
                plotBorderWidth: null,
                plotShadow: false
            },
            title: {
                text: ''
            },
            tooltip: {
                pointFormat: '{series.name}: <b>{point.y}</b>',
                percentageDecimals: 1
            },
            plotOptions: {
                pie: {
                    allowPointSelect: true,
                    cursor: 'pointer',
                    dataLabels: {
                        enabled: true,
                        color: '#000000',
                        connectorColor: '#000000',
                        formatter: function() {
                            return '<b>'+ this.point.name +'</b>: '+ this.point.y;
                        }
                    }
                }
            },
            series: [{
                type: 'pie',
                name: 'Contratos por situação',
                data: [<? mostra_array1() ?>]
            }]
        });

        $('#container2').highcharts({
            chart: {
                plotBackgroundColor: null,
                plotBorderWidth: null,
                plotShadow: false
            },
            title: {
                text: ''
            },
            tooltip: {
                pointFormat: '{series.name}: <b>{point.y}</b>',
                percentageDecimals: 1
            },
            plotOptions: {
                pie: {
                    allowPointSelect: true,
                    cursor: 'pointer',
                    dataLabels: {
                        enabled: true,
                        color: '#000000',
                        connectorColor: '#000000',
                        formatter: function() {
                            return '<b>'+ this.point.name +'</b>: '+ this.point.y;
                        }
                    }
                }
            },
            series: [{
                type: 'pie',
                name: 'Valores por status',
                data: [<? mostra_array2() ?>]
            }]
        });

        $('#container3').highcharts({
            chart: {
                plotBackgroundColor: null,
                plotBorderWidth: null,
                plotShadow: false
            },
            title: {
                text: 'Browser market shares at a specific website, 2010'
            },
            tooltip: {
                pointFormat: '{series.name}: <b>{point.percentage}%</b>',
                percentageDecimals: 1
            },
            plotOptions: {
                pie: {
                    allowPointSelect: true,
                    cursor: 'pointer',
                    dataLabels: {
                        enabled: true,
                        color: '#000000',
                        connectorColor: '#000000',
                        formatter: function() {
                            return '<b>'+ this.point.name +'</b>: '+ this.percentage +' %';
                        }
                    }
                }
            },
            series: [{
                type: 'pie',
                name: 'Browser share',
                data: [
                    ['Firefox',   45.0],
                    ['IE',       26.8],
                    {
                        name: 'Chrome',
                        y: 12.8,
                        sliced: true,
                        selected: true
                    },
                    ['Safari',    8.5],
                    ['Opera',     6.2],
                    ['Others',   0.7]
                ]
            }]
        });
    });


</script>

<?php

global $db;

if($_REQUEST['consulta'] == '')
	$_REQUEST['consulta'] = 'html';

//verifica perfis
$perfis 		= arrayPerfil();
$superPerfis 	= array(PERFIL_ADMINISTRADOR,PERFIL_SUPER_USUARIO,PERFIL_CONSULTA_GERAL);
$arIntersec 	= array_intersect($superPerfis, $perfis);

$arHspidPermitido = array();

?>

<style>

    #col_esq {
        width: 60%;
        min-height: 300px;
        float: left;
        margin-top: 20px;
    }

    #col_dir {
        width: 38%;
        min-height: 300px;
        float: right;
        margin-top: 20px;
    }

    div,p,ul,ol,li,form,fieldset,input,textarea,p,blockquote{
        margin:0;
        padding:0;
    }

    h1,h2,h3,h4,h5,h6 {
        font-size:100%;
        font-weight:normal;
    }

    .content{
        width: 100%;
        margin: auto;
    }
    .content-left{
        width: 24%;
        float: left;

    }
    .content-left p {
        color: #fff;
        padding: 16px;
        background-color: #cc0000;
        margin-top: 15px;
        font-weight: bold;
        text-align: center;
    }
    .content-right{
        width: 75%;
        float: right;
    }

    #meus-dados li:nth-of-type(2n) {
        background: #eee;
    }

    .bloco-menu {
        border: 1px solid #ccc;
    }
    .bloco-menu ul li{
        list-style: none;
        line-height: 18px;
        padding: 5px;
    }
    .bloco-info{
        margin-top: 15px;
    }
    .trecho{
        text-decoration-style: inherit;
    }
</style>

<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<div id="col_esq">
    <div class="bloco-info">
        <input type="hidden" value="<?=$i;?>" class="contador"/>
        <div class="bloco-header">
            <h4> Contratos que eu sou fiscal/gestor </h4>
        </div>
        <div class="bloco-menu" id="meus-dados-<?=$i;?>">
            <ul>
                 <?
                    $cabecalho = ['Ação', 'Número', 'Ano', 'Contratada', 'Situação', 'Valor Inicial', 'Término Vigência'];

                    $sql = "SELECT DISTINCT
                                ctcontrato.ctrid,
                                ctcontrato.ctrnum,
                                ctcontrato.ctrano,
                                ctcontrato.ctrnomecontratada,
                                ctsituacaocontrato.sitdsc,
                                ctcontrato.ctrvlrinicial,
                                TO_CHAR(ctcontrato.ctrdtfimvig, 'DD/MM/YYYY') as ctrdtfimvig
                                FROM
                                contratos.ctcontrato,
                                contratos.fiscalcontrato,
                                entidade.entidade,
                                contratos.gestorcontrato,
                                contratos.ctsituacaocontrato
                                WHERE
                                ctsituacaocontrato.sitid = ctcontrato.sitid and
                                fiscalcontrato.ctrid = ctcontrato.ctrid AND
                                entidade.entid = fiscalcontrato.entid and
                                entnome != '' ";

                    $rs = $db->carregar($sql);

                 for ($x = 0; $x < count($rs); $x++) {
                     $obj = $db->pegaUm("select ctrobj from contratos.ctcontrato where ctrid = {$rs[$x]['ctrid']}");
                     $rs[$x]['ctrid'] = "<a href='javascript:selecionarContrato({$rs[$x]['ctrid']})'><img src='../imagens/consultar.gif'/></a>
                                         <a href='http://simec-local/contratos/contratos.php?modulo=principal/addNotaContratos&acao=A&ctrid={$rs[$x]['ctrid']}'><img src='../imagens/gif_inclui.gif'/></a>";
                     $rs[$x]['ctrnomecontratada'] = "<a class='informacao' title='$obj'>{$rs[$x]['ctrnomecontratada']}</a>";
                 }

                    $param['totalLinhas'] = true;
                    $db->monta_lista_array($rs, $cabecalho, 50, 20, '', '100%', '', '', '', '', '', $param);
                 ?>
            </ul>
        </div>
    </div>

    <div class="bloco-info">
        <input type="hidden" value="<?=$i;?>" class="contador"/>
        <div class="bloco-header">
            <h4> Status de minhas faturas </h4>
        </div>
        <div class="bloco-menu" id="meus-dados-<?=$i;?>">
            <ul>
                <?
                    $cabecalho = ['Status', 'Número', 'Valor'];

                    $sql = "SELECT
                              estadodocumento.esddsc,
                              faturacontrato.ftcnumero,
                              faturacontrato.ftcvalor
                            FROM
                              contratos.faturacontrato,
                              workflow.estadodocumento,
                              workflow.documento
                            WHERE
                              documento.docid = faturacontrato.docid AND
                              documento.esdid = estadodocumento.esdid
                            ORDER BY
                            estadodocumento.esddsc";

                $rs = $db->carregar($sql);

                for ($x = 0; $x < count($rs); $x++) {
                    $rs[$x]['ftcnumero'] = "<a href=''>{$rs[$x]['ftcnumero']}</a>";
                }

                $coluna = array();
                $coluna[] = array('label' => 'Valor', 'campo' => 'ftcvalor');

                $agp = array(
                    "agrupador" => array(
                        array('label' => 'Status', 'campo' => 'esddsc'),
                        array('label' => 'Nº da fatura', 'campo' => 'ftcnumero')
                    ),
                    "agrupadoColuna" => array(
                        "ftcvalor")
                );

                $rel = new montaRelatorio();
                $rel->setEspandir(false);
                $rel->setAgrupador($agp, $rs);
                $rel->setColuna($coluna);
                $rel->setTotNivel(true);
                $rel->setTotalizador(true);
                $rel->setTolizadorLinha(true);
                $rel->setMonstrarTolizadorNivel(true);
                echo $rel->getRelatorio();
                /*$param['totalLinhas'] = true;
                $db->monta_lista_array($rs, $cabecalho, 50, 20, '', '100%', '', '', '', '', '', $param);*/

                ?>
            </ul>
        </div>
    </div>

    <!-- <div class="bloco-info">
        <input type="hidden" value="<?=$i;?>" class="contador"/>
        <div class="bloco-header">
            <h4> Contratos com divergências </h4>
        </div>
        <div class="bloco-menu" id="meus-dados-<?=$i;?>">
            <ul>
                <? $db->monta_lista_array($rs, $cabecalho, 50, 20, '', '100%', '', '', '', '', '', $param); ?>
            </ul>
        </div>
    </div> -->

    <div class="bloco-info">
        <input type="hidden" value="<?=$i;?>" class="contador"/>
        <div class="bloco-header">
            <h4> Contratos expirando </h4>
        </div>
        <div class="bloco-menu" id="meus-dados-<?=$i;?>">
            <ul>
                <?
                $cabecalho = ['Ação', 'Número', 'Ano', 'Contratada', 'Situação', 'Valor Inicial', 'Término Vigência', 'Dias'];
                    $sql = "SELECT DISTINCT
                                ctcontrato.ctrid,
                                ctcontrato.ctrnum,
                                ctcontrato.ctrano,
                                ctcontrato.ctrnomecontratada,
                                ctsituacaocontrato.sitdsc,
                                ctcontrato.ctrvlrinicial,
                                TO_CHAR(ctcontrato.ctrdtfimvig, 'DD/MM/YYYY') as ctrdtfimvig,
                                ctcontrato.ctrdtfimvig - ctcontrato.ctrdtiniciovig as dias
                                FROM
                                contratos.ctcontrato,
                                contratos.fiscalcontrato,
                                entidade.entidade,
                                contratos.gestorcontrato,
                                contratos.ctsituacaocontrato
                                WHERE
                                ctsituacaocontrato.sitid = ctcontrato.sitid and
                                fiscalcontrato.ctrid = ctcontrato.ctrid AND
                                entidade.entid = fiscalcontrato.entid and
                                entnome != '' ";

                    $rs = $db->carregar($sql);

                    for ($x = 0; $x < count($rs); $x++) {
                        $obj = $db->pegaUm("select ctrobj from contratos.ctcontrato where ctrid = {$rs[$x]['ctrid']}");
                        $rs[$x]['ctrid'] = "<a href='javascript:selecionarContrato({$rs[$x]['ctrid']})'><img src='../imagens/consultar.gif'/></a>";
                        $rs[$x]['ctrnomecontratada'] = "<a class='informacao' title='$obj'>{$rs[$x]['ctrnomecontratada']}</a>";
                    }

                    $param['totalLinhas'] = true;
                    $db->monta_lista_array($rs, $cabecalho, 50, 20, '', '100%', '', '', '', '', '', $param); ?>
            </ul>
        </div>
    </div>
    <div class="bloco-info" style="text-align: center">
        <a style="background-color: #003F7E; color: white; padding: 8px;" href="http://simec-local/contratos/contratos.php?modulo=principal/inicioContrato&acao=A">Lista todos os contratos</a>
    </div>
</div>
<div id="col_dir">

    <div class="bloco-info">
        <input type="hidden" value="<?=$i;?>" class="contador"/>
        <div class="bloco-header">
            <h4> Contratos por situação </h4>
        </div>
        <div class="bloco-menu" id="meus-dados-<?=$i;?>">
            <ul>
                <div id="container" style="min-width: 400px; height: 400px; margin: 0 auto"></div>
            </ul>
        </div>
    </div>

    <div class="bloco-info">
        <input type="hidden" value="<?=$i;?>" class="contador"/>
        <div class="bloco-header">
            <h4> Valores por status </h4>
        </div>
        <div class="bloco-menu" id="meus-dados-<?=$i;?>">
            <ul>
                <div id="container2" style="min-width: 400px; height: 400px; margin: 0 auto"></div>
            </ul>
        </div>
    </div>

    <!-- <div class="bloco-info">
        <input type="hidden" value="<?=$i;?>" class="contador"/>
        <div class="bloco-header">
            <h4> Gráfico de divergencias </h4>
        </div>
        <div class="bloco-menu" id="meus-dados-<?=$i;?>">
            <ul>
                <div id="container3" style="min-width: 400px; height: 400px; margin: 0 auto"></div>
            </ul>
        </div>
    </div> -->
</div>