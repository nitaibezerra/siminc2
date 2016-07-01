<?php

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include APPRAIZ. 'includes/classes/relatorio.class.inc';
monta_titulo( $titulo_modulo, '' );

function mostra_array1() {
    global $db;
    $res = "";
    $sql1 = "SELECT
      sum(faturacontrato.ftcvalor) as total,
      estrutura.estareasigla
    FROM
      contratos.faturacontrato,
      corporativo.estrutura,
      contratos.contratosetorresponsavel
    WHERE
      faturacontrato.ctrid = contratosetorresponsavel.ctrid AND
      contratosetorresponsavel.estid = estrutura.estid
    group by
      estrutura.estareasigla";

    $rs1 = $db->carregar($sql1);

    for ($x = 0; $x < count($rs1); $x++) {
        $res .= "['" . $rs1[$x]['estareasigla'] . "'," . $rs1[$x]['total'] . "],";
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
                name: 'Valores por área',
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
            <h4> Faturas Pendentes </h4>
        </div>
        <div class="bloco-menu" id="meus-dados-<?=$i;?>">
            <ul>
                 <?
                    $cabecalho = ['Ação', 'Nº da fatura', 'Nº do contrato', 'Fornecedor', 'Valor', 'Data Limite', 'Data de Lançamento'];

                    $sql = "SELECT
                              faturacontrato.ctrid,
                              faturacontrato.ftcnumero,
                              ctcontrato.ctrnum || ' / ' || ctcontrato.ctrano as ctrnum,
                              ctcontrato.ctrnomecontratada,
                              faturacontrato.ftcvalor,
                              faturacontrato.ftcdatalimite,
                              TO_CHAR(faturacontrato.ftcdatacriacao, 'DD/MM/YYYY') as ftcdatacriacao
                            FROM
                              contratos.faturacontrato,
                              workflow.documento,
                              contratos.ctcontrato,
                              workflow.estadodocumento
                            WHERE
                              documento.docid = faturacontrato.docid AND
                              ctcontrato.ctrid = faturacontrato.ctrid AND
                              estadodocumento.esdid = documento.esdid AND
                              (estadodocumento.esddsc = 'Triagem' or estadodocumento.esddsc = 'Aguardando Aprovação')";

                    $rs = $db->carregar($sql);

                    for ($x = 0; $x < count($rs); $x++) {
                        $descricao = $db->pegaUm("select ftcdescricao from contratos.faturacontrato where ftcnumero = '{$rs[$x]['ftcnumero']}'");
                        $obj = $db->pegaUm("select ctrobj from contratos.ctcontrato where ctrid = {$rs[$x]['ctrid']}");
                        $rs[$x]['ftcnumero'] = "<a href='' title='$descricao'>{$rs[$x]['ftcnumero']}</a>";
                        $rs[$x]['ctrid'] = "<a href='javascript:selecionarContrato({$rs[$x]['ctrid']})'><img src='../imagens/consultar.gif'/></a>";
                        $rs[$x]['ctrnum'] = "<a title='$obj'>{$rs[$x]['ctrnum']}</a>";
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
            <h4> Valores das faturas de todos os contratos </h4>
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

    <div class="bloco-info">
        <input type="hidden" value="<?=$i;?>" class="contador"/>
        <div class="bloco-header">
            <h4> Ordens bancárias sem vínculo com fatura </h4>
        </div>
        <div class="bloco-menu" id="meus-dados-<?=$i;?>">
            <ul>
                <?
                $cabecalho = ['Ação', 'OB', 'Descrição', 'Valor'];

                $sql = "SELECT
                          ob_siafi.obsid,
                          ob_siafi.ob,
                          ob_siafi.obsob,
                          ob_siafi.valor

                        FROM
                          contratos.ob_siafi,
                          contratos.empenho_siafi
                        WHERE
                          empenho_siafi.nu_empenho = ob_siafi.empenho AND
                          empenho_siafi.epsid in (select epsid from contratos.faturacontrato) limit 10";

                $rs = $db->carregar($sql);

for ($x = 0; $x < count($rs); $x++) {
    $dados = $db->carregar("SELECT
  empenhovinculocontrato.ctrid,
  empenho_siafi.nu_empenho,
  ctcontrato.entidcontratada
FROM
  contratos.ob_siafi,
  contratos.empenho_siafi,
  contratos.empenhovinculocontrato,
  contratos.ctcontrato
WHERE
  empenho_siafi.nu_empenho = ob_siafi.empenho AND
  empenhovinculocontrato.epsid = empenho_siafi.epsid AND
  ctcontrato.ctrid = empenhovinculocontrato.ctrid
  and ob = '{$rs[$x]['ob']}'");

    $rs[$x]['obsid'] = "<a href=\"javascript:janela('?modulo=principal/popUpFatura&acao=A&requisicao=popUpOB&numempenho={$dados[0]['nu_empenho']}&ob={$rs[$x]['ob']}', 'detalheempenho', 500, 600);\" title=\"Visualizar as ordens bancárias\" class=\"notprint\" style=\"float:left\">
															        	<img border=\"0\" src=\"/imagens/icone_lupa.png\">
															        </a>";
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
            <h4> Valores por área </h4>
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

</div>