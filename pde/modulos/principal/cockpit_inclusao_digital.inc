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
                jQuery('#div_investimento1').load('estrategico.php?modulo=principal/cockpit_inclusao_digital&acao=A&detalhe_investimento1='+jQuery(this).val());
            });
            jQuery('.filtrar_investimento2').click(function(){
                jQuery('#div_investimento2').load('estrategico.php?modulo=principal/cockpit_inclusao_digital&acao=A&detalhe_investimento2='+jQuery(this).val());
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_inclusao_digital&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Inclusão Digital</div></td>
	</tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(227);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Laboratórios Adquiridos'); ?>
            </div>
            <?php
            $sql = "select
               dpe.dpeanoref AS ano,
               tid1.tiddsc as dependencia,
               SUM(dsh.dshqtde) AS valor
            from painel.seriehistorica sh
            inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
            inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            where sh.indid in (227)
            and sh.sehstatus <> 'I'
            group by ano, dependencia
            order by ano, dependencia";
            $dados = $db->carregar($sql, null, 3200);
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'dependencia', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b> R$ '+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaPDDECampo2', null, null, $formatoValores, null, "", 1000, 600, null, 300, true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(750);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Projetores Proinfo entregues', 'Rural'); ?>
            </div>
            <?php
            $sql = "select
                        dpe.dpeanoref AS ano,
                        'Projetores' AS tipo,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					where sh.indid in (750)
					and dsh.tidid2 IN (2800) --Rural
					and sh.sehstatus <> 'I'
					group by ano, tipo
					order by ano, tipo";
            $dados = $db->carregar($sql, null, 3200);
            if($dados){
                $valorAcumulado = 0;
                $testaAno="";
                foreach($dados as $dado){
                    $valorAcumulado += $dado['valor'];
                    if($testaAno!=$dado['ano']){
                        $dados[] = array(
                            'ano'=>$dado['ano'],
                            'tipo'=>'Projetores Acumulados',
                            'valor'=>$valorAcumulado,
                        );
                        $testaAno=$dado['ano'];
                    }
                }
                $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'tipo', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaProjetores', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(748);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Alunos beneficiados por escola<br>conectada à internet'); ?>
            </div>
            <?php
            $sql = "select
                        dpe.dpeanoref AS ano,
                        'Alunos' AS tipo,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					where sh.indid in (748)
					and sh.sehstatus <> 'I'
					group by ano, tipo
					order by ano, tipo";
            $dados = $db->carregar($sql, null, 3200);
            if($dados){
                $valorAcumulado = 0;
                $testaAno="";
                foreach($dados as $dado){
                    $valorAcumulado += $dado['valor'];
                    if($testaAno!=$dado['ano']){
                        $dados[] = array(
                            'ano'=>$dado['ano'],
                            'tipo'=>'Alunos Acumulados',
                            'valor'=>$valorAcumulado,
                        );
                        $testaAno=$dado['ano'];
                    }
                }
                $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'tipo', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaAlunos', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(227);">
                <?php exibirTitulo('financeiro', 'Laboratórios Adquiridos', 'Investimento (Rural)'); ?>
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
        <td class="fundo_padrao" colspan="2">
            <div class="link" onclick="abreIndicadorPopUp(750);">
                <?php exibirTitulo('financeiro', 'Projetores Proinfo entregues', 'Investimento (Rural)'); ?>
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
                $campo
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.indid in (227)
            AND dsh.tidid1 IN (96) --Rural
            AND sh.sehstatus <> 'I'
            GROUP BY periodo
            ORDER BY periodo";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $arrX = array();
    $arrA = array();
    $arrTotal = array();
    $vTotal = 0;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrX[] = Round($dado['periodo']);
            $arrA[] = Round($dado['valor']);
            $vTotal += Round($dado['valor']);
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'Total', 'data'=>$arrA),
            array('name' => 'Total Acumulado', 'data'=>$arrTotal),
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoInvestimentoLaboratorio", "Laboratórios adquiridos",300, $formatoValores);
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
                $campo
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
            WHERE sh.indid in (750)
            AND dsh.tidid2 IN (2800) --Rural
            AND sh.sehstatus <> 'I'
            GROUP BY periodo
            ORDER BY periodo";
    $arrDados = $db->carregar( $sql, null, 3200 );
    $arrX = array();
    $arrA = array();
    $arrTotal = array();
    $vTotal = 0;
    if($arrDados){
        foreach($arrDados as $dado){
            $arrX[] = Round($dado['periodo']);
            $arrA[] = Round($dado['valor']);
            $vTotal += Round($dado['valor']);
            $arrTotal[] = $vTotal;
        }

        $aDados = array(
            array('name' => 'Total', 'data'=>$arrA),
            array('name' => 'Total Acumulado', 'data'=>$arrTotal),
        );

        echo geraGraficoLinha($aDados, $arrX, "graficoInvestimentoProjetores", "Projetores Entregues",300, $formatoValores);
    }
}
?>