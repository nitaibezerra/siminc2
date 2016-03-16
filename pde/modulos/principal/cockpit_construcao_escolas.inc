<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

//permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
if(isset($_REQUEST['detalhe_investimento1'])){
    gerarGraficoInvestimento1($_REQUEST['detalhe_investimento1']);
    die;
}
if(isset($_REQUEST['detalhe_investimento2'])){
    gerarGraficoInvestimento2($_REQUEST['detalhe_investimento2']);
    die;
}
if(isset($_REQUEST['detalhe_investimento3'])){
    gerarGraficoInvestimento3($_REQUEST['detalhe_investimento3']);
    die;
}
?>
<!DOCTYPE HTML>
<html>
<head>
	<meta http-equiv="X-UA-Compatible" content="IE=9" />
	<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
	<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

	<script language="javascript" type="text/javascript" src="/library/jquery/jquery-1.10.2.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
	<script language="javascript" type="text/javascript" src="js/estrategico.js"></script>

	<link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
	<script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/jquery.mousewheel.js"></script>
	<script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/perfect-scrollbar.js"></script>

	<link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
	<script language="javascript" type="text/javascript" src="/library/jquery_totem/jquery.totemticker.min.js"></script>

	<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
	<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

	<script language="javascript" src="/estrutura/js/funcoes.js"></script>
	<script language="JavaScript" src="../includes/funcoes.js"></script>

	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
	<script>
		atualizaUsuario();

        jQuery(function(){
            jQuery('.filtrar_investimento1').click(function(){
                jQuery('#div_investimento1').load('estrategico.php?modulo=principal/cockpit_construcao_escolas&acao=A&detalhe_investimento1='+jQuery(this).val());
            });
            jQuery('.filtrar_investimento2').click(function(){
                jQuery('#div_investimento2').load('estrategico.php?modulo=principal/cockpit_construcao_escolas&acao=A&detalhe_investimento2='+jQuery(this).val());
            });
            jQuery('.filtrar_investimento3').click(function(){
                jQuery('#div_investimento3').load('estrategico.php?modulo=principal/cockpit_construcao_escolas&acao=A&detalhe_investimento3='+jQuery(this).val());
            });
        });
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_pronacampo.jpg');background-repeat:repeat-x;background-position:2px -10px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
	</style>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
			<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
				<img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;" />
				<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
			</div>
			<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_construcao_escolas&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="2" ><div style="margin:28px" >Construção de Escolas</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2023);" width="50%">
			<div>
				<?php exibirTitulo('indicador', 'Obras Aprovadas'); ?>
			</div>
            <?php
            $sql = "select
                        dpe.dpeanoref AS ano,
						tid2.tiddsc as dependencia,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
					where sh.indid in (2023)
					and sh.sehstatus <> 'I'
					group by ano, dependencia
					order by ano, dependencia";
            $dados = $db->carregar($sql, null, 3200);
            if($dados){
                $totalAgrupado = "";
                foreach ($dados as $dado){
                    $totalAgrupado += $dado['valor'];
                }
                $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'dependencia', 'valor');
                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaPDDECampo', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
                echo "<table align='center'>";
                echo "  <tr>";
                echo "      <td class='titulo_box bold'>Total: ".number_format($totalAgrupado,0,',','.')."</td>";
                echo "  </tr>";
                echo "</table>";
            }
            ?>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2023);" width="50%">
			<div>
				<?php exibirTitulo('financeiro', 'Investimento'); ?>
			</div>
            <?
            $arrEtapa = array();
            $sql = "SELECT
                        dpe.dpeanoref AS periodo,
                        sum(dsh.dshvalor) as total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2023)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo
                    ORDER BY periodo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrX = array();
                $arrA = array();
                $arrTotal = array();
                $vTotal = 0;
                foreach($arrDados as $dado){
                    $arrX[] = $dado['periodo'];
                    $vA = ROUND($dado['total']);
                    $arrA[] = $vA;
                    $vTotal += $vA;
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Investimento', 'data'=>$arrA),
                    array('name' => 'Investimento Acumulado', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoInvestimentos", "Obras Aprovadas",300, $formatoValores);
            }
            ?>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2079);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Comunidades Indígenas', 'Obras Aprovadas'); ?>
            </div>
            <?php
            $sql = "select
                        dpe.dpeanoref AS ano,
						tid1.tiddsc as tipo,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (2079)
					and sh.sehstatus <> 'I'
					group by ano, tipo
					order by ano, tipo";
            $dados = $db->carregar($sql, null, 3200);
            if($dados){
                $totalAgrupado = "";
                foreach ($dados as $dado){
                    $totalAgrupado += $dado['valor'];
                }

                $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'tipo', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaComunidadeIndigena', null, null, $formatoValores, null, "", 1000, 600, null, 300, true);
                echo "<table align='center'>";
                echo "  <tr>";
                echo "      <td class='titulo_box bold'>Total: ".number_format($totalAgrupado,0,',','.')."</td>";
                echo "  </tr>";
                echo "</table>";
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2078);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Comunidades Rurais', 'Obras Aprovadas'); ?>
            </div>
            <?php
            $sql = "select
                        dpe.dpeanoref AS ano,
						tid1.tiddsc as tipo,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (2078)
					and sh.sehstatus <> 'I'
					group by ano, tipo
					order by ano, tipo";
            $dados = $db->carregar($sql, null, 3200);
            if($dados){
                $totalAgrupado = "";
                foreach ($dados as $dado){
                    $totalAgrupado += $dado['valor'];
                }
                $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'tipo', 'valor');
                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaComunidadeRural', null, null, $formatoValores, null, "", 1000, 600, null, 300, true);
                echo "<table align='center'>";
                echo "  <tr>";
                echo "      <td class='titulo_box bold'>Total: ".number_format($totalAgrupado,0,',','.')."</td>";
                echo "  </tr>";
                echo "</table>";
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1815);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Comunidades Quilombolas', 'Obras Aprovadas'); ?>
            </div>
            <?php
            $sql = "select
                        dpe.dpeanoref AS ano,
						tid1.tiddsc as tipo,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (1815)
					and sh.sehstatus <> 'I'
					group by ano, tipo
					order by ano, tipo";
            $dados = $db->carregar($sql, null, 3200);
            if($dados){
                $totalAgrupado = "";
                foreach ($dados as $dado){
                    $totalAgrupado += $dado['valor'];
                }
                $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'tipo', 'valor');
                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaComunidadeQuilombola', null, null, $formatoValores, null, "", 1000, 600, null, 300, true);
                echo "<table align='center'>";
                echo "  <tr>";
                echo "      <td class='titulo_box bold'>Total: ".number_format($totalAgrupado,0,',','.')."</td>";
                echo "  </tr>";
                echo "</table>";
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(2079);">
                <?php exibirTitulo('financeiro', 'Investimento'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_investimento1" name="investimento1" value="1" checked="checked" onclick="" />Recursos repassados
                <input type="radio" class="filtrar_investimento1" name="investimento1" value="2" onclick="" />Escolas beneficiadas
            </div>
            <br>
            <div id="div_investimento1">
                <?php gerarGraficoInvestimento1(1); ?>
            </div>
        </td>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(2078);">
                <?php exibirTitulo('financeiro', 'Investimento'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_investimento2" name="investimento2" value="1" checked="checked" onclick="" />Recursos repassados
                <input type="radio" class="filtrar_investimento2" name="investimento2" value="2" onclick="" />Escolas beneficiadas
            </div>
            <br>
            <div id="div_investimento2">
                <?php gerarGraficoInvestimento2(1); ?>
            </div>
        </td>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(1815);">
                <?php exibirTitulo('financeiro', 'Investimento'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_investimento3" name="investimento3" value="1" checked="checked" onclick="" />Recursos repassados
                <input type="radio" class="filtrar_investimento3" name="investimento3" value="2" onclick="" />Escolas beneficiadas
            </div>
            <br>
            <div id="div_investimento3">
                <?php gerarGraficoInvestimento3(1); ?>
            </div>
        </td>
    </tr>
	<tr>
		<td class="fundo_td_laranja" colspan="3">
		<div style="text-align:center;" >
			<img src="../imagens/icones/icons/executiverel.png" style="vertical-align:middle;" />
			<input type="text" onclick="this.style.color='#000000';this.value='';" name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;' title='' class=' normal' />
			<img src="../imagens/icones/icons/Find.png" style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);" />
		</div>
		</td>
	</tr>
</table>
</body>
</html>
<?php
function gerarGraficoInvestimento1($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "SUM(dsh.dshvalor) AS valor";
        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }"
        );
    }else{
        $campo = "SUM(dsh.dshqtde) AS valor";
        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
        );
    }

    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                tid2.tiddsc AS modalidade,
                $campo
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            WHERE sh.indid in (2079)
            AND sh.sehstatus <> 'I'
            GROUP BY periodo, modalidade
            ORDER BY periodo, modalidade";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $arrX = array();
    $arrA = array();
    $arrB = array();
    $arrC = array();
    $arrSituacao = array();
    $arrTotal = array();
    $vTotal = 0;

    if($arrDados){
        foreach($arrDados as $dado){
            $arrSituacao[$dado['periodo']][$dado['modalidade']]['total'][] = $dado['valor'];
        }
        foreach($arrSituacao as $periodo => $sit){
            $vA = Round((is_array($sit['Construção']['total'])?array_sum($sit['Construção']['total']):0));
            $vB = Round((is_array($sit['Reforma']['total'])?array_sum($sit['Reforma']['total']):0));
            $vC = Round((is_array($sit['Ampliação']['total'])?array_sum($sit['Ampliação']['total']):0));
            $arrX[] = $periodo;
            $arrA[] = $vA;
            $arrB[] = $vB;
            $arrC[] = $vC;
            $vTotal += $vA + $vB + $vC;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'Construcao', 'data'=>$arrA),
            array('name' => 'Reforma', 'data'=>$arrB),
            array('name' => 'Ampliacao', 'data'=>$arrC),
            array('name' => 'Total', 'data'=>$arrTotal),
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoInvestimentoIndigena", "Comunidades Indígenas",300, $formatoValores);
    }
}

function gerarGraficoInvestimento2($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "SUM(dsh.dshvalor) AS valor";
        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }"
        );
    }else{
        $campo = "SUM(dsh.dshqtde) AS valor";
        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
        );
    }

    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                tid2.tiddsc AS modalidade,
                $campo
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            WHERE sh.indid in (2078)
            AND sh.sehstatus <> 'I'
            GROUP BY periodo, modalidade
            ORDER BY periodo, modalidade";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $arrX = array();
    $arrA = array();
    $arrB = array();
    $arrC = array();
    $arrSituacao = array();
    $arrTotal = array();
    $vTotal = 0;

    if($arrDados){
        foreach($arrDados as $dado){
            $arrSituacao[$dado['periodo']][$dado['modalidade']]['total'][] = $dado['valor'];
        }
        foreach($arrSituacao as $periodo => $sit){
            $vA = Round((is_array($sit['Construção']['total'])?array_sum($sit['Construção']['total']):0));
            $vB = Round((is_array($sit['Reforma']['total'])?array_sum($sit['Reforma']['total']):0));
            $vC = Round((is_array($sit['Ampliação']['total'])?array_sum($sit['Ampliação']['total']):0));
            $arrX[] = $periodo;
            $arrA[] = $vA;
            $arrB[] = $vB;
            $arrC[] = $vC;
            $vTotal += $vA + $vB + $vC;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'Construcao', 'data'=>$arrA),
            array('name' => 'Reforma', 'data'=>$arrB),
            array('name' => 'Ampliacao', 'data'=>$arrC),
            array('name' => 'Total', 'data'=>$arrTotal),
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoInvestimentoRural", "Comunidades Rurais",300, $formatoValores);
    }
}

function gerarGraficoInvestimento3($detalhe){

    global $db;

    if($detalhe==1){
        $campo = "SUM(dsh.dshvalor) AS valor";
        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 0, ',', '.') + '</b>'; }"
        );
    }else{
        $campo = "SUM(dsh.dshqtde) AS valor";
        $formatoValores = array(
            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
        );
    }

    $sql = "SELECT
                dpe.dpeanoref AS periodo,
                tid2.tiddsc AS modalidade,
                $campo
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            WHERE sh.indid in (1815)
            AND sh.sehstatus <> 'I'
            GROUP BY periodo, modalidade
            ORDER BY periodo, modalidade";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $arrX = array();
    $arrA = array();
    $arrB = array();
    $arrC = array();
    $arrSituacao = array();
    $arrTotal = array();
    $vTotal = 0;

    if($arrDados){
        foreach($arrDados as $dado){
            $arrSituacao[$dado['periodo']][$dado['modalidade']]['total'][] = $dado['valor'];
        }
        foreach($arrSituacao as $periodo => $sit){
            $vA = Round((is_array($sit['Construção']['total'])?array_sum($sit['Construção']['total']):0));
            $vB = Round((is_array($sit['Reforma']['total'])?array_sum($sit['Reforma']['total']):0));
            $vC = Round((is_array($sit['Ampliação']['total'])?array_sum($sit['Ampliação']['total']):0));
            $arrX[] = $periodo;
            $arrA[] = $vA;
            $arrB[] = $vB;
            $arrC[] = $vC;
            $vTotal += $vA + $vB + $vC;
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'Construcao', 'data'=>$arrA),
            array('name' => 'Reforma', 'data'=>$arrB),
            array('name' => 'Ampliacao', 'data'=>$arrC),
            array('name' => 'Total', 'data'=>$arrTotal),
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoInvestimentoQuilombola", "Comunidades Quilombolas",300, $formatoValores);
    }
}
?>