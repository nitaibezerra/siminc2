<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit('estrategico.php?modulo=principal/cockpit_mais_medicos&acao=A');

if(isset($_REQUEST['detalhe_vaga1'])){
    gerarGraficoVaga1($_REQUEST['detalhe_vaga1']);
    die;
}

if(!empty($_REQUEST['atualizarMetaResultado'])){
    carregarDetalhesMetaResultado(array('esiid'=>$_REQUEST['esiid']));
    die;
}
if(!empty($_REQUEST['atualizarVagas'])){
    carregarDetalhesVagas(array('esiid'=>$_REQUEST['esiid']));
    die;
}
if(!empty($_REQUEST['atualizarVagas2'])){
    carregarDetalhesVagas2(array('esiid'=>$_REQUEST['esiid']));
    die;
}
if(!empty($_REQUEST['atualizarVagasPrivadas'])){
    carregarDetalhesVagasPrivadas(array('esiid'=>$_REQUEST['esiid']));
    die;
}
if(!empty($_REQUEST['atualizarVagasPrivadas2'])){
    carregarDetalhesVagasPrivadas2(array('esiid'=>$_REQUEST['esiid']));
    die;
}
if(!empty($_REQUEST['atualizarVagasPrivadas3'])){
    carregarDetalhesVagasPrivadas3(array('esiid'=>$_REQUEST['esiid']));
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
            jQuery('.filtrar_vaga1').click(function(){
                jQuery('#div_vaga1').load('estrategico.php?modulo=principal/cockpit_graduacao_medicina&acao=A&detalhe_vaga1='+jQuery(this).val());
            });
        });

        function detalharMetaResultado(esiid, obj) {
            var tabela = obj.parentNode.parentNode.parentNode;
            var linha = obj.parentNode.parentNode;
            if(obj.title=="mais") {
                obj.title    = "menos";
                obj.src      = "../imagens/menos.gif";
                var nlinha   = tabela.insertRow(linha.rowIndex+1);
                var ncol     = nlinha.insertCell(0);
                ncol.colSpan = 9;
                ncol.id      = 'dtl2_coluna_'+(nlinha.rowIndex+1);
                jQuery('#'+ncol.id).html('Carregando...');
                jQuery('#'+ncol.id).load('estrategico.php?modulo=principal/cockpit_graduacao_medicina&acao=A&esiid='+esiid+'&atualizarMetaResultado=1');
                divCarregado();
            } else {
                obj.title    = "mais";
                obj.src      = "../imagens/mais.gif";
                tabela.deleteRow(linha.rowIndex+1);
            }
        }

        function detalharVagas(esiid, obj) {
            var tabela = obj.parentNode.parentNode.parentNode;
            var linha = obj.parentNode.parentNode;
            if(obj.title=="mais") {
                obj.title    = "menos";
                obj.src      = "../imagens/menos.gif";
                var nlinha   = tabela.insertRow(linha.rowIndex+1);
                var ncol     = nlinha.insertCell(0);
                ncol.colSpan = 6;
                ncol.id      = 'dtl2_coluna_'+(nlinha.rowIndex+1);
                jQuery('#'+ncol.id).html('Carregando...');
                jQuery('#'+ncol.id).load('estrategico.php?modulo=principal/cockpit_graduacao_medicina&acao=A&esiid='+esiid+'&atualizarVagas=1');
                divCarregado();
            } else {
                obj.title    = "mais";
                obj.src      = "../imagens/mais.gif";
                tabela.deleteRow(linha.rowIndex+1);
            }
        }

        function detalharVagas2(esiid, obj) {
            var tabela = obj.parentNode.parentNode.parentNode;
            var linha = obj.parentNode.parentNode;
            if(obj.title=="mais") {
                obj.title    = "menos";
                obj.src      = "../imagens/menos.gif";
                var nlinha   = tabela.insertRow(linha.rowIndex+1);
                var ncol     = nlinha.insertCell(0);
                ncol.colSpan = 6;
                ncol.id      = 'dtl2_coluna_'+(nlinha.rowIndex+1);
                jQuery('#'+ncol.id).html('Carregando...');
                jQuery('#'+ncol.id).load('estrategico.php?modulo=principal/cockpit_graduacao_medicina&acao=A&esiid='+esiid+'&atualizarVagas2=1');
                divCarregado();
            } else {
                obj.title    = "mais";
                obj.src      = "../imagens/mais.gif";
                tabela.deleteRow(linha.rowIndex+1);
            }
        }

        function detalharVagasPrivadas(esiid, obj) {
            var tabela = obj.parentNode.parentNode.parentNode;
            var linha = obj.parentNode.parentNode;
            if(obj.title=="mais") {
                obj.title    = "menos";
                obj.src      = "../imagens/menos.gif";
                var nlinha   = tabela.insertRow(linha.rowIndex+1);
                var ncol     = nlinha.insertCell(0);
                ncol.colSpan = 6;
                ncol.id      = 'dtl2_coluna_'+(nlinha.rowIndex+1);
                jQuery('#'+ncol.id).html('Carregando...');
                jQuery('#'+ncol.id).load('estrategico.php?modulo=principal/cockpit_graduacao_medicina&acao=A&esiid='+esiid+'&atualizarVagasPrivadas=1');
                divCarregado();
            } else {
                obj.title    = "mais";
                obj.src      = "../imagens/mais.gif";
                tabela.deleteRow(linha.rowIndex+1);
            }
        }

        function detalharVagasPrivadas2(esiid, obj) {
            var tabela = obj.parentNode.parentNode.parentNode;
            var linha = obj.parentNode.parentNode;
            if(obj.title=="mais") {
                obj.title    = "menos";
                obj.src      = "../imagens/menos.gif";
                var nlinha   = tabela.insertRow(linha.rowIndex+1);
                var ncol     = nlinha.insertCell(0);
                ncol.colSpan = 6;
                ncol.id      = 'dtl2_coluna_'+(nlinha.rowIndex+1);
                jQuery('#'+ncol.id).html('Carregando...');
                jQuery('#'+ncol.id).load('estrategico.php?modulo=principal/cockpit_graduacao_medicina&acao=A&esiid='+esiid+'&atualizarVagasPrivadas2=1');
                divCarregado();
            } else {
                obj.title    = "mais";
                obj.src      = "../imagens/mais.gif";
                tabela.deleteRow(linha.rowIndex+1);
            }
        }

        function detalharVagasPrivadas3(esiid, obj) {
            var tabela = obj.parentNode.parentNode.parentNode;
            var linha = obj.parentNode.parentNode;
            if(obj.title=="mais") {
                obj.title    = "menos";
                obj.src      = "../imagens/menos.gif";
                var nlinha   = tabela.insertRow(linha.rowIndex+1);
                var ncol     = nlinha.insertCell(0);
                ncol.colSpan = 6;
                ncol.id      = 'dtl2_coluna_'+(nlinha.rowIndex+1);
                jQuery('#'+ncol.id).html('Carregando...');
                jQuery('#'+ncol.id).load('estrategico.php?modulo=principal/cockpit_graduacao_medicina&acao=A&esiid='+esiid+'&atualizarVagasPrivadas3=1');
                divCarregado();
            } else {
                obj.title    = "mais";
                obj.src      = "../imagens/mais.gif";
                tabela.deleteRow(linha.rowIndex+1);
            }
        }
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('cockpit/images/banner/bannerMaisMedicos.jpg');}
        #popdiv{display:block;width:100%;height:420px;overflow-y: scroll;}
	</style>
</head>
<body onload="refreshAutomatico();">
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
			<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
				<img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;" />
				<div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
			</div>
			<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_graduacao_medicina&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="2" ><div style="margin:28px" >Graduação em Medicina</div></td>
	</tr>
    <tr>
        <td class="fundo_padrao" width="50%">
            <div>
                <?php exibirTitulo('alvo', 'Vagas'); ?>
            </div>
            <?php
            $sql = "SELECT sh.indid, COALESCE(dsh.tidid1,0) AS cod, sum(dsh.dshqtde) as total
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE sh.indid IN (2823, 2889, 2678)
                    AND sh.sehstatus <> 'I'
                    GROUP BY sh.indid, dsh.tidid1";
            $rsGraduacao = $db->carregar($sql, null, 3200);
            if($rsGraduacao){
                foreach($rsGraduacao as $g){
                    $arInd[$g['indid']][$g['cod']] = $g['total'];
                }
            }

            $totalGraduacao = $arInd['2678']['7263'] + $arInd['2678']['7262'] + $arInd['2823']['7588'] + $arInd['2823']['7589'] + $arInd['2889']['0'];
            $totalGraduacaoPublica = $arInd['2678']['7263'] + $arInd['2678']['7262'];
            $totalGraduacaoPrivada = $arInd['2823']['7588'] + $arInd['2823']['7589'] + $arInd['2889']['0'];
            $valorMeta = 11447;
            $valorMetaPublica = 3615;
            $valorMetaPrivada = 7832;
            $porcentagem = number_format(($totalGraduacao*100)/$valorMeta,0,",",".");
            $porcentagemPublica = number_format(($totalGraduacaoPublica*100)/$valorMetaPublica,0,",",".");
            $porcentagemPrivada = number_format(($totalGraduacaoPrivada*100)/$valorMetaPrivada,0,",",".");

            $titlePublica = '<table><tr><td>Cursos novos</td>';
            $titlePublica .= '<td>'.number_format($arInd['2678']['7262'],0,",",".").'</td>';
            $titlePublica .= '</tr><tr><td>Aumento de vagas</td>';
            $titlePublica .= '<td>'.number_format($arInd['2678']['7263'],0,",",".").'</td>';
            $titlePublica .= '</tr></table>';

            $titlePrivada = '<table><tr><td>Cursos novos</td>';
            $titlePrivada .= '<td>'.number_format($arInd['2823']['7589'],0,",",".").'</td>';
            $titlePrivada .= '</tr><tr><td>Aumento de vagas</td>';
            $titlePrivada .= '<td>'.number_format($arInd['2823']['7588'],0,",",".").'</td>';
            $titlePrivada .= '</tr><tr><td>Reativadas</td>';
            $titlePrivada .= '<td>'.number_format($arInd['2889']['0'],0,",",".").'</td>';
            $titlePrivada .= '</tr></table>';
            ?>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                <tr>
                    <td class="titulo_box bold">
                        <?=number_format($totalGraduacao,0,",",".")?> vagas criadas
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                            <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagem) ?>%;">
                                <?= $porcentagem ?>%
                            </div>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td class="titulo_box bold">
                        <font size=-1><?=number_format($valorMeta,0,",",".")?> vagas (até 2017)</font>
                    </td>
                </tr>
            </table>
            <br>
            <br>

            <link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
            <script type="text/javascript" src="../includes/remedial.js"></script>
            <script type="text/javascript" src="../includes/superTitle.js"></script>
            <table class="tabela_painel" cellpadding="2" cellspacing="1" width="100%" border="1" align="center" >
                <tr>
                    <td class="titulo_box bold center">Federais</td>
                    <td class="titulo_box bold center">Privadas</td>
                </tr>
                <tr>
                    <td>
                        <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                            <tr>
                                <td class="titulo_box bold">
                                    <?=number_format($totalGraduacaoPublica,0,",",".")?> vagas criadas
                                </td>
                            </tr>
                            <tr>
                                <td class="titulo_box bold">
                                    <div onmousemove="SuperTitleOn(this, '<?php echo $titlePublica; ?>' );" onmouseout="SuperTitleOff( this );" style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                                        <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagemPublica) ?>%;">
                                            <?= $porcentagemPublica ?>%
                                        </div>
                                    </div>
                                </td>
                            </tr>
                            <tr>
                                <td class="titulo_box bold">
                                    <font size=-1><?=number_format($valorMetaPublica,0,",",".")?> vagas (até 2017)</font>
                                </td>
                            </tr>
                        </table>
                    </td>
                    <td>
                        <table class="tabela_painel" cellpadding="2" cellspacing="1" width="70%" border="0" align="center" >
                            <tr>
                                <td class="titulo_box bold">
                                    <?=number_format($totalGraduacaoPrivada,0,",",".")?> vagas criadas
                                </td>
                            </tr>
                            <tr>
                                <td class="titulo_box bold">
                                    <div onmousemove="SuperTitleOn(this, '<?php echo $titlePrivada; ?>' );" onmouseout="SuperTitleOff( this );" style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 30px; width: 200px;'>
                                        <div id="div_publica" style="float:left; border: 0px; background-color: #80BC44; text-align: center; color: white; height: 30px; width: <?= str_replace(',','.',$porcentagemPrivada) ?>%;">
                                            <?= $porcentagemPrivada ?>%
                                        </div>
                                    </div>
                                </td>
                            </tr>
                            <tr>
                                <td class="titulo_box bold">
                                    <font size=-1><?=number_format($valorMetaPrivada,0,",",".")?> vagas (até 2017)</font>
                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
        </td>
        <td class="fundo_padrao" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Vagas Criadas - 2013, 2014 e 2015', '<span class="link" onclick="abreIndicadorPopUp(2678);">Federais</span>, <span class="link" onclick="abreIndicadorPopUp(2823);">Vagas criadas - Privadas</span>, <span class="link" onclick="abreIndicadorPopUp(2889);">Vagas reativadas - Privadas</span>'); ?>
            </div>
            <?php
            $sql = "SELECT *
                    FROM (
                        SELECT dpe.dpeanoref AS periodo, '2013' as agrupador, SUM(dsh.dshqtde) AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.indid IN (2823, 2889, 2678)
                        AND sh.sehstatus <> 'I'
                        AND dpe.dpeanoref = '2013'
                        GROUP BY periodo
                    UNION ALL
                        SELECT dpe.dpeanoref AS periodo, '2014' as agrupador, SUM(dsh.dshqtde) AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.indid IN (2823, 2889, 2678)
                        AND sh.sehstatus <> 'I'
                        AND dpe.dpeanoref BETWEEN '2013' AND '2014'
                        GROUP BY periodo
                    UNION ALL
                        SELECT dpe.dpeanoref AS periodo, '2015' as agrupador, SUM(dsh.dshqtde) AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.indid IN (2823, 2889, 2678)
                        AND sh.sehstatus <> 'I'
                        AND dpe.dpeanoref BETWEEN '2013' AND '2015'
                        GROUP BY periodo
                    ) AS FOO
                    ORDER BY agrupador";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){

                $dadosAgrupados = agruparDadosGrafico($arrDados, 'agrupador', 'periodo', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaVagasCriadas', null, null, $formatoValores, null, "", 1000, 600, null, 300);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Metas e resultados - 2013, 2014 e 2015', '<span class="link" onclick="abreIndicadorPopUp(2678);">Federais</span>, <span class="link" onclick="abreIndicadorPopUp(2823);">Vagas criadas - Privadas</span>, <span class="link" onclick="abreIndicadorPopUp(2889);">Vagas reativadas - Privadas</span>'); ?>
            </div>
            <?php
            $sql = "SELECT
                        '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharMetaResultado(\''||cod||'\',this);\">&nbsp;' || tipo AS tipo,
                        periodo,
                        SUM(meta) AS meta,
                        SUM(resultado) AS resultado
                    FROM (
                        SELECT
                            CASE
                                WHEN sh.indid IN (2678) THEN 1
                                WHEN sh.indid IN (2823, 2889) THEN 2
                            END AS cod,
                            CASE
                                WHEN sh.indid IN (2678) THEN 'Vagas Públicas'
                                WHEN sh.indid IN (2823, 2889) THEN 'Vagas Privadas'
                            END AS tipo,
                            dpe.dpeanoref::INTEGER AS periodo,
                            0 AS meta,
                            dsh.dshqtde AS resultado
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        LEFT JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                        WHERE sh.indid IN (2823, 2889, 2678)
                        AND sh.sehstatus <> 'I'
                        AND dpe.dpeanoref BETWEEN '2013' AND '2015'
                    UNION ALL
                        SELECT 1 AS cod, 'Vagas Públicas' AS tipo, '2013' AS periodo, 350 AS meta, 0 AS resultado
                    UNION ALL
                        SELECT 1 AS cod, 'Vagas Públicas' AS tipo, '2014' AS periodo, 1225 AS meta, 0 AS resultado
                    UNION ALL
                        SELECT 1 AS cod, 'Vagas Públicas' AS tipo, '2015' AS periodo, 400 AS meta, 0 AS resultado
                    UNION ALL
                        SELECT 2 AS cod, 'Vagas Privadas' AS tipo, '2013' AS periodo, 1102 AS meta, 0 AS resultado
                    UNION ALL
                        SELECT 2 AS cod, 'Vagas Privadas' AS tipo, '2014' AS periodo, 4210 AS meta, 0 AS resultado
                    UNION ALL
                        SELECT 2 AS cod, 'Vagas Privadas' AS tipo, '2015' AS periodo, 1540 AS meta, 0 AS resultado
                    ) AS FOO
                    GROUP BY periodo, tipo, cod
                    ORDER BY periodo, cod";
            $arrDados = $db->carregar( $sql, null, 3200 );
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th rowspan="2">Meta</th>
                    <th colspan="2">2013</th>
                    <th colspan="2">2014</th>
                    <th colspan="2">2015</th>
                    <th colspan="2">Total</th>
                </tr>
                <tr>
                    <th>Meta</th>
                    <th>Resultado</th>
                    <th>Meta</th>
                    <th>Resultado</th>
                    <th>Meta</th>
                    <th>Resultado</th>
                    <th>Meta</th>
                    <th>Resultado</th>
                </tr>
                <?php
                if($arrDados){
                    $arrSituacao = array();
                    $totalMetas2013 = 0;
                    $totalMetas2014 = 0;
                    $totalMetas2015 = 0;
                    $totalResultados2013 = 0;
                    $totalResultados2014 = 0;
                    $totalResultados2015 = 0;
                    foreach($arrDados as $dado){
                        $arrSituacao[$dado['tipo']][$dado['periodo']]['Meta'] = $dado['meta'];
                        $arrSituacao[$dado['tipo']][$dado['periodo']]['Resultado'] = $dado['resultado'];
                    }
                    foreach($arrSituacao as $tipo => $sit){
                        ?>
                        <tr>
                            <td class="" ><?php echo $tipo ?></td>
                            <td class="numero" ><?php echo number_format($sit['2013']['Meta'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($sit['2013']['Resultado'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($sit['2014']['Meta'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($sit['2014']['Resultado'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($sit['2015']['Meta'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($sit['2015']['Resultado'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($sit['2013']['Meta']+$sit['2014']['Meta']+$sit['2015']['Meta'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($sit['2013']['Resultado']+$sit['2014']['Resultado']+$sit['2015']['Resultado'],0,",",".") ?></td>
                        </tr>
                        <?php
                        $totalMetas2013 +=  $sit['2013']['Meta'];
                        $totalMetas2014 +=  $sit['2014']['Meta'];
                        $totalMetas2015 +=  $sit['2015']['Meta'];
                        $totalResultados2013 +=  $sit['2013']['Resultado'];
                        $totalResultados2014 +=  $sit['2014']['Resultado'];
                        $totalResultados2015 +=  $sit['2015']['Resultado'];
                    }
                }?>
                <tr>
                    <th>Total de vagas ampliadas</th>
                    <th><?php echo number_format($totalMetas2013,0,",",".") ?></th>
                    <th><?php echo number_format($totalResultados2013,0,",",".") ?></th>
                    <th><?php echo number_format($totalMetas2014,0,",",".") ?></th>
                    <th><?php echo number_format($totalResultados2014,0,",",".") ?></th>
                    <th><?php echo number_format($totalMetas2015,0,",",".") ?></th>
                    <th><?php echo number_format($totalResultados2015,0,",",".") ?></th>
                    <th><?php echo number_format($totalMetas2013 + $totalMetas2014 + $totalMetas2015,0,",",".") ?></th>
                    <th><?php echo number_format($totalResultados2013 + $totalResultados2014 + $totalResultados2015,0,",",".") ?></th>
                </tr>
            </table>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_padrao" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Vagas criadas, por Região', '<span class="link" onclick="abreIndicadorPopUp(2678);">Federais</span>, <span class="link" onclick="abreIndicadorPopUp(2823);">Vagas criadas - Privadas</span>, <span class="link" onclick="abreIndicadorPopUp(2889);">Vagas reativadas - Privadas</span>'); ?>
            </div>
            <?php
            $sql = "SELECT
                        regdescricao AS descricao,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio
                    INNER JOIN territorios.estado est ON est.estuf = mun.estuf
                    INNER JOIN territorios.regiao reg ON reg.regcod = est.regcod
                    WHERE sh.indid IN (2823, 2889, 2678)
                    AND sh.sehstatus <> 'I'
                    GROUP BY descricao";
            $dados = $db->carregar( $sql, null, 3200 );
            if($dados){
                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColuna($dados, 'graficoColunaRegiao', null, null, $formatoValores, null, "", 1000, 600, null, 300);
            }
            ?>
        </td>
        <td class="fundo_padrao" width="34%">
            <div>
                <?php exibirTitulo('indicador', 'Vagas criadas, por situação de curso', '<span class="link" onclick="abreIndicadorPopUp(2678);">Federais</span>, <span class="link" onclick="abreIndicadorPopUp(2823);">Vagas criadas - Privadas</span>, <span class="link" onclick="abreIndicadorPopUp(2889);">Vagas reativadas - Privadas</span>'); ?>
            </div>
            <?php
            $sql = "SELECT
						dpe.dpeanoref as periodo,
						case
							when dsh.tidid1 IN (7262, 7589) then 'Novos Cursos'
							when dsh.tidid1 IN (7263, 7588) then 'Cursos Existentes'
							when sh.indid IN (2889) then 'Vagas Reativadas'
						end AS situacao,
						sum(dsh.dshqtde::integer) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
					WHERE sh.indid IN (2823, 2889, 2678)
					AND sh.sehstatus <> 'I'
					GROUP BY periodo, situacao
					ORDER BY periodo, situacao";
            $dados = $db->carregar($sql, null, 3200);
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'periodo', 'situacao', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                );

                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculasSituacao', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
            }
            ?>
        </td>
        <td class="fundo_padrao" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Vagas por tipo de curso', '<span class="link" onclick="abreIndicadorPopUp(2678);">Federais</span>, <span class="link" onclick="abreIndicadorPopUp(2823);">Vagas criadas - Privadas</span>, <span class="link" onclick="abreIndicadorPopUp(2889);">Vagas reativadas - Privadas</span>'); ?>
            </div>
            <br>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_vaga1" name="vaga1" value="1" checked="checked" onclick="" />Todos
                <input type="radio" class="filtrar_vaga1" name="vaga1" value="2" onclick="" />Público
                <input type="radio" class="filtrar_vaga1" name="vaga1" value="3" onclick="" />Privado
            </div>
            <br>
            <div id="div_vaga1">
                <?php gerarGraficoVaga1(1); ?>
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_padrao" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Total de vagas criadas, por UF', '<span class="link" onclick="abreIndicadorPopUp(2678);">Federais</span>, <span class="link" onclick="abreIndicadorPopUp(2823);">Vagas criadas - Privadas</span>, <span class="link" onclick="abreIndicadorPopUp(2889);">Vagas reativadas - Privadas</span>'); ?>
            </div>
            <?php
            $sql = "SELECT
                        mun.estuf AS descricao,
                        sum(dsh.dshqtde::integer) as valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio
                    WHERE sh.indid IN (2823, 2889, 2678)
                    AND sh.sehstatus <> 'I'
                    GROUP BY descricao
                    ORDER BY descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                geraGraficoBarra($arrDados, 'graficoBarraUF', null, null, null, null, "", 1000, 600);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(0);" width="50%">
            <div>
                <?php exibirTitulo('indicador', 'Municípios pré-selecionados'); ?>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Vagas previstas e criadas - Universidades Federais', '<span class="link" onclick="abreIndicadorPopUp(2620);">Vagas previstas</span>, <span class="link" onclick="abreIndicadorPopUp(2678);">Vagas criadas</span>'); ?>
            </div>
            <div id="popdiv">
                <?php
                $sql = "SELECT
                            '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharVagas(\''||cod||'\',this);\">&nbsp;' || universidade  AS universidade,
                            SUM(vagasprevistas) AS vagasprevistas,
                            SUM(vagascriadas) AS vagascriadas,
                            SUM(vagasaumento) AS vagasaumento,
                            SUM(vagasautorizacao) AS vagasautorizacao
                        FROM (
                            SELECT
                                entinst.entid AS cod,
                                entinst.entnome AS universidade,
                                CASE WHEN sh.indid IN (2620) THEN dsh.dshqtde::integer ELSE 0 END AS vagasprevistas,
                                CASE WHEN sh.indid IN (2678) THEN dsh.dshqtde::integer ELSE 0  END AS vagascriadas,
                                CASE WHEN dsh.tidid1 IN (7263, 7259) THEN dsh.dshqtde::integer ELSE 0  END AS vagasaumento,
                                CASE WHEN dsh.tidid1 IN (7262, 7258) THEN dsh.dshqtde::integer ELSE 0  END AS vagasautorizacao
                            FROM painel.seriehistorica sh
                            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                            INNER JOIN entidade.entidade entcampus ON entcampus.entid = dsh.entid
                            inner join entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
                            inner join entidade.funentassoc a on a.fueid = fn.fueid
                            inner join entidade.entidade entinst on entinst.entid = a.entid
                            WHERE sh.indid IN (2620, 2678)
                            AND dpe.dpeanoref <= '2017'
                            AND sh.sehstatus <> 'I'
                        ) AS FOO
                        GROUP BY cod, universidade
                        ORDER BY universidade";
                $arrDados = $db->carregar( $sql, null, 3200 );
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <th rowspan="2">IES / Campus / Ano</th>
                        <th rowspan="2">Vagas previstas</th>
                        <th rowspan="2">Vagas criadas</th>
                        <th colspan="2">Tipo de vaga</th>
                        <th rowspan="2">Total até 2017</th>
                    </tr>
                    <tr>
                        <th>Autorização de vagas (curso novo)</th>
                        <th>Aumento de vagas (cursos existentes)</th>
                    </tr>
                    <?php
                    if($arrDados){
                        $totalVagasPrevistas = 0;
                        $totalVagasCriadas = 0;
                        $totalVagasAumento = 0;
                        $totalVagasAutorizacao = 0;
                        foreach($arrDados as $dado){
                            ?>
                            <tr>
                                <td class="" ><?php echo $dado['universidade'] ?></td>
                                <td class="numero" ><?php echo number_format($dado['vagasprevistas'],0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($dado['vagascriadas'],0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($dado['vagasaumento'],0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($dado['vagasautorizacao'],0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($dado['vagasaumento'] + $dado['vagasautorizacao'],0,",",".") ?></td>
                            </tr>
                            <?php
                            $totalVagasPrevistas += $dado['vagasprevistas'];
                            $totalVagasCriadas += $dado['vagascriadas'];
                            $totalVagasAumento += $dado['vagasaumento'];
                            $totalVagasAutorizacao += $dado['vagasautorizacao'];
                        }
                    }?>
                    <tr>
                        <th>Total</th>
                        <th><?php echo number_format($totalVagasPrevistas,0,",",".") ?></th>
                        <th><?php echo number_format($totalVagasCriadas,0,",",".") ?></th>
                        <th><?php echo number_format($totalVagasAumento,0,",",".") ?></th>
                        <th><?php echo number_format($totalVagasAutorizacao,0,",",".") ?></th>
                        <th><?php echo number_format($totalVagasAumento + $totalVagasAutorizacao,0,",",".") ?></th>
                    </tr>
                </table>
            </div>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Vagas criadas - Privadas', '<span class="link" onclick="abreIndicadorPopUp(2823);">Vagas Criadas</span>, <span class="link" onclick="abreIndicadorPopUp(2889);">Vagas Reativadas</span>'); ?>
            </div>
            <div id="popdiv">
                <?php
                $sql = "SELECT
                            uf,
                            SUM(vagasaumento2823) AS vagasaumento2823,
                            SUM(vagasautorizacao2823) AS vagasautorizacao2823,
                            SUM(vagasaumento2889) AS vagasaumento2889
                        FROM (
                            SELECT
                                'Brasil'::varchar AS uf,
                                CASE WHEN sh.indid IN (2823) AND dsh.tidid1 IN (7588) THEN dsh.dshqtde::integer ELSE 0 END AS vagasaumento2823,
                                CASE WHEN sh.indid IN (2823) AND dsh.tidid1 IN (7589) THEN dsh.dshqtde::integer ELSE 0 END AS vagasautorizacao2823,
                                CASE WHEN sh.indid IN (2889) THEN dsh.dshqtde::integer ELSE 0 END AS vagasaumento2889
                            FROM painel.seriehistorica sh
                            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                            WHERE sh.indid IN (2823, 2889)
                            AND sh.sehstatus <> 'I'
                        ) AS FOO
                        GROUP BY uf
                        ORDER BY uf";
                $arrDadosBrasil = $db->pegaLinha( $sql, null, 3200 );

                $sql = "SELECT
                            '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharVagasPrivadas(\''||uf||'\',this);\">&nbsp;' || uf AS uf,
                            SUM(vagasaumento2823) AS vagasaumento2823,
                            SUM(vagasautorizacao2823) AS vagasautorizacao2823,
                            SUM(vagasaumento2889) AS vagasaumento2889
                        FROM (
                            SELECT
                                mun.estuf AS uf,
                                CASE WHEN sh.indid IN (2823) AND dsh.tidid1 IN (7588) THEN dsh.dshqtde::integer ELSE 0 END AS vagasaumento2823,
                                CASE WHEN sh.indid IN (2823) AND dsh.tidid1 IN (7589) THEN dsh.dshqtde::integer ELSE 0 END AS vagasautorizacao2823,
                                CASE WHEN sh.indid IN (2889) THEN dsh.dshqtde::integer ELSE 0 END AS vagasaumento2889
                            FROM painel.seriehistorica sh
                            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                            INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                            INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::integer
                            INNER JOIN territorios.municipio mun ON mun.muncod = ies.iesmuncod
                            WHERE sh.indid IN (2823, 2889)
                            AND sh.sehstatus <> 'I'
                        ) AS FOO
                        GROUP BY uf
                        ORDER BY uf";
                $arrDados = $db->carregar( $sql, null, 3200 );
                ?>
                <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                    <tr>
                        <th rowspan="2">UF / Município / IES / Ano</th>
                        <th colspan="2">Vagas criadas de graduação de medicina</th>
                        <th>Vagas reativadas de graduação de medicina</th>
                        <th rowspan="2">Total</th>
                    </tr>
                    <tr>
                        <th>Autorização de vagas (curso novo)</th>
                        <th>Aumento de vagas (cursos existentes)</th>
                        <th>Aumento de vagas (cursos existentes)</th>
                    </tr>
                    <?php
                    if($arrDados){
                        ?>
                        <tr>
                            <th><?php echo $arrDadosBrasil['uf'] ?></th>
                            <th><?php echo number_format($arrDadosBrasil['vagasautorizacao2823'],0,",",".") ?></th>
                            <th><?php echo number_format($arrDadosBrasil['vagasaumento2823'],0,",",".") ?></th>
                            <th><?php echo number_format($arrDadosBrasil['vagasaumento2889'],0,",",".") ?></th>
                            <th><?php echo number_format($arrDadosBrasil['vagasautorizacao2823'] + $arrDadosBrasil['vagasaumento2823'] + $arrDadosBrasil['vagasaumento2889'],0,",",".") ?></th>
                        </tr>
                        <?php
                        foreach($arrDados as $dado){
                            ?>
                            <tr>
                                <td class="" ><?php echo $dado['uf'] ?></td>
                                <td class="numero" ><?php echo number_format($dado['vagasautorizacao2823'],0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($dado['vagasaumento2823'],0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($dado['vagasaumento2889'],0,",",".") ?></td>
                                <td class="numero" ><?php echo number_format($dado['vagasautorizacao2823'] + $dado['vagasaumento2823'] + $dado['vagasaumento2889'],0,",",".") ?></td>
                            </tr>
                            <?php
                        }
                    }?>
                </table>
            </div>
        </td>
    </tr>
	<tr>
		<td class="fundo_td_laranja" colspan="2">
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
function gerarGraficoVaga1($detalhe){

    global $db;

    if($detalhe==1){
        $where = "WHERE sh.indid IN (2823, 2889, 2678)";
    }elseif($detalhe==2){
        $where = "WHERE sh.indid IN (2678)";
    }else{
        $where = "WHERE sh.indid IN (2823, 2889)";
    }

    $sql = "SELECT
                case
                    when dsh.tidid1 IN (7262, 7589) then 'Novos Cursos'
                    when dsh.tidid1 IN (7263, 7588) then 'Cursos Existentes'
                    when sh.indid IN (2889) then 'Vagas Reativadas'
                end AS descricao,
                sum(dsh.dshqtde::integer) as valor
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            $where
            AND sh.sehstatus <> 'I'
            GROUP BY descricao
            ORDER BY descricao";
    $arrDados = $db->carregar( $sql, null, 3200 );
    if($arrDados){
        geraGrafico($arrDados, "graficoPizzaSituacao", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, null, null, "#fff", true);
    }
}

function carregarDetalhesMetaResultado($dados) {

    global $db;

    $sql = "SELECT
                situacao,
                periodo,
                SUM(meta) AS meta,
                SUM(resultado) AS resultado
            FROM (
                SELECT
                    CASE
                        WHEN sh.indid IN (2678) THEN 1
                        WHEN sh.indid IN (2823, 2889) THEN 2
                    END AS cod,
                    CASE
                        WHEN sh.indid IN (2678) THEN 'Vagas Públicas'
                        WHEN sh.indid IN (2823, 2889) THEN 'Vagas Privadas'
                    END AS tipo,
                    CASE
                        WHEN tid1.tidid IN (7588,7263) THEN 'Cursos existentes (aumento de vagas)'
                        WHEN tid1.tidid IN (7262,7589) THEN 'Novos cursos (autorizações)'
                        WHEN sh.indid IN (2889) THEN 'Vagas reativadas (supervisão)'
                    END AS situacao,
                    dpe.dpeanoref::INTEGER AS periodo,
                    0 AS meta,
                    dsh.dshqtde AS resultado
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                LEFT JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                WHERE sh.indid IN (2823, 2889, 2678)
                AND sh.sehstatus <> 'I'
                AND dpe.dpeanoref BETWEEN '2013' AND '2015'
            UNION ALL
                SELECT 1 AS cod, 'Vagas Públicas' AS tipo, 'Novos cursos (autorizações)' AS situacao, '2013' AS periodo, 340 AS meta, 0 AS resultado
            UNION ALL
                SELECT 1 AS cod, 'Vagas Públicas' AS tipo, 'Novos cursos (autorizações)' AS situacao, '2014' AS periodo, 880 AS meta, 0 AS resultado
            UNION ALL
                SELECT 1 AS cod, 'Vagas Públicas' AS tipo, 'Novos cursos (autorizações)' AS situacao, '2015' AS periodo, 220 AS meta, 0 AS resultado
            UNION ALL
                SELECT 1 AS cod, 'Vagas Públicas' AS tipo, 'Cursos existentes (aumento de vagas)' AS situacao, '2013' AS periodo, 10 AS meta, 0 AS resultado
            UNION ALL
                SELECT 1 AS cod, 'Vagas Públicas' AS tipo, 'Cursos existentes (aumento de vagas)' AS situacao, '2014' AS periodo, 345 AS meta, 0 AS resultado
            UNION ALL
                SELECT 1 AS cod, 'Vagas Públicas' AS tipo, 'Cursos existentes (aumento de vagas)' AS situacao, '2015' AS periodo, 180 AS meta, 0 AS resultado
            UNION ALL
                SELECT 2 AS cod, 'Vagas Privadas' AS tipo, 'Novos cursos (autorizações)' AS situacao, '2013' AS periodo, 600 AS meta, 0 AS resultado
            UNION ALL
                SELECT 2 AS cod, 'Vagas Privadas' AS tipo, 'Novos cursos (autorizações)' AS situacao, '2014' AS periodo, 4060 AS meta, 0 AS resultado
            UNION ALL
                SELECT 2 AS cod, 'Vagas Privadas' AS tipo, 'Novos cursos (autorizações)' AS situacao, '2015' AS periodo, 1120 AS meta, 0 AS resultado
            UNION ALL
                SELECT 2 AS cod, 'Vagas Privadas' AS tipo, 'Cursos existentes (aumento de vagas)' AS situacao, '2013' AS periodo, 502 AS meta, 0 AS resultado
            UNION ALL
                SELECT 2 AS cod, 'Vagas Privadas' AS tipo, 'Cursos existentes (aumento de vagas)' AS situacao, '2014' AS periodo, 150 AS meta, 0 AS resultado
            UNION ALL
                SELECT 2 AS cod, 'Vagas Privadas' AS tipo, 'Cursos existentes (aumento de vagas)' AS situacao, '2015' AS periodo, 420 AS meta, 0 AS resultado
            UNION ALL
                SELECT 2 AS cod, 'Vagas Privadas' AS tipo, 'Vagas reativadas (supervisão)' AS situacao, '2013' AS periodo, 0 AS meta, 0 AS resultado
            UNION ALL
                SELECT 2 AS cod, 'Vagas Privadas' AS tipo, 'Vagas reativadas (supervisão)' AS situacao, '2014' AS periodo, 0 AS meta, 0 AS resultado
            UNION ALL
                SELECT 2 AS cod, 'Vagas Privadas' AS tipo, 'Vagas reativadas (supervisão)' AS situacao, '2015' AS periodo, 0 AS meta, 0 AS resultado
            ) AS FOO
            WHERE cod = {$dados['esiid']}
            GROUP BY periodo, situacao
            ORDER BY periodo, situacao";
    $arrDados = $db->carregar( $sql, null, 3200 );
    ?>
    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
        <tr>
            <th rowspan="2">Meta</th>
            <th colspan="2">2013</th>
            <th colspan="2">2014</th>
            <th colspan="2">2015</th>
            <th colspan="2">Total</th>
        </tr>
        <tr>
            <th>Meta</th>
            <th>Resultado</th>
            <th>Meta</th>
            <th>Resultado</th>
            <th>Meta</th>
            <th>Resultado</th>
            <th>Meta</th>
            <th>Resultado</th>
        </tr>
        <?php
        if($arrDados){
            $arrSituacao = array();
            $totalMetas2013 = 0;
            $totalMetas2014 = 0;
            $totalMetas2015 = 0;
            $totalResultados2013 = 0;
            $totalResultados2014 = 0;
            $totalResultados2015 = 0;
            foreach($arrDados as $dado){
                $arrSituacao[$dado['situacao']][$dado['periodo']]['Meta'] = $dado['meta'];
                $arrSituacao[$dado['situacao']][$dado['periodo']]['Resultado'] = $dado['resultado'];
            }
            foreach($arrSituacao as $situacao => $sit){
                ?>
                <tr class="zebrado">
                    <td class="" ><?php echo $situacao ?></td>
                    <td class="numero" ><?php echo number_format($sit['2013']['Meta'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($sit['2013']['Resultado'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($sit['2014']['Meta'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($sit['2014']['Resultado'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($sit['2015']['Meta'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($sit['2015']['Resultado'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($sit['2013']['Meta']+$sit['2014']['Meta']+$sit['2015']['Meta'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($sit['2013']['Resultado']+$sit['2014']['Resultado']+$sit['2015']['Resultado'],0,",",".") ?></td>
                </tr>
                <?php
                $totalMetas2013 +=  $sit['2013']['Meta'];
                $totalMetas2014 +=  $sit['2014']['Meta'];
                $totalMetas2015 +=  $sit['2015']['Meta'];
                $totalResultados2013 +=  $sit['2013']['Resultado'];
                $totalResultados2014 +=  $sit['2014']['Resultado'];
                $totalResultados2015 +=  $sit['2015']['Resultado'];
            }
        }?>
        <tr>
            <th>Total de vagas ampliadas</th>
            <th><?php echo number_format($totalMetas2013,0,",",".") ?></th>
            <th><?php echo number_format($totalResultados2013,0,",",".") ?></th>
            <th><?php echo number_format($totalMetas2014,0,",",".") ?></th>
            <th><?php echo number_format($totalResultados2014,0,",",".") ?></th>
            <th><?php echo number_format($totalMetas2015,0,",",".") ?></th>
            <th><?php echo number_format($totalResultados2015,0,",",".") ?></th>
            <th><?php echo number_format($totalMetas2013 + $totalMetas2014 + $totalMetas2015,0,",",".") ?></th>
            <th><?php echo number_format($totalResultados2013 + $totalResultados2014 + $totalResultados2015,0,",",".") ?></th>
        </tr>
    </table>
<?php
}

function carregarDetalhesVagas($dados) {

    global $db;

    $sql = "SELECT
                '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharVagas2(\''||entidcampus||'\',this);\">&nbsp;' || campus  AS campus,
                SUM(vagasprevistas) AS vagasprevistas,
                SUM(vagascriadas) AS vagascriadas,
                SUM(vagasaumento) AS vagasaumento,
                SUM(vagasautorizacao) AS vagasautorizacao
            FROM (
                SELECT
                    entinst.entid AS entiduniversidade,
                    entinst.entnome AS universidade,
                    entcampus.entid AS entidcampus,
                    entcampus.entnome AS campus,
                    CASE WHEN sh.indid IN (2620) THEN dsh.dshqtde::integer ELSE 0 END AS vagasprevistas,
                    CASE WHEN sh.indid IN (2678) THEN dsh.dshqtde::integer ELSE 0  END AS vagascriadas,
                    CASE WHEN dsh.tidid1 IN (7263, 7259) THEN dsh.dshqtde::integer ELSE 0  END AS vagasaumento,
                    CASE WHEN dsh.tidid1 IN (7262, 7258) THEN dsh.dshqtde::integer ELSE 0  END AS vagasautorizacao
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                INNER JOIN entidade.entidade entcampus ON entcampus.entid = dsh.entid
                inner join entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
                inner join entidade.funentassoc a on a.fueid = fn.fueid
                inner join entidade.entidade entinst on entinst.entid = a.entid
                WHERE sh.indid IN (2620, 2678)
                AND dpe.dpeanoref <= '2017'
                AND sh.sehstatus <> 'I'
            ) AS FOO
            WHERE entiduniversidade = {$dados['esiid']}
            GROUP BY entidcampus, campus
            ORDER BY campus";
    $arrDados = $db->carregar( $sql, null, 3200 );
    ?>
    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
        <tr>
            <th rowspan="2">Campus / Ano</th>
            <th rowspan="2">Vagas previstas</th>
            <th rowspan="2">Vagas criadas</th>
            <th colspan="2">Tipo de vaga</th>
            <th rowspan="2">Total até 2017</th>
        </tr>
        <tr>
            <th>Autorização de vagas (curso novo)</th>
            <th>Aumento de vagas (cursos existentes)</th>
        </tr>
        <?php
        if($arrDados){
            $totalVagasPrevistas = 0;
            $totalVagasCriadas = 0;
            $totalVagasAumento = 0;
            $totalVagasAutorizacao = 0;
            foreach($arrDados as $dado){
                ?>
                <tr class="zebrado">
                    <td class="" ><?php echo $dado['campus'] ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasprevistas'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagascriadas'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasaumento'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasautorizacao'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasaumento'] + $dado['vagasautorizacao'],0,",",".") ?></td>
                </tr>
                <?php
                $totalVagasPrevistas += $dado['vagasprevistas'];
                $totalVagasCriadas += $dado['vagascriadas'];
                $totalVagasAumento += $dado['vagasaumento'];
                $totalVagasAutorizacao += $dado['vagasautorizacao'];
            }
        }?>
        <tr>
            <th>Total</th>
            <th><?php echo number_format($totalVagasPrevistas,0,",",".") ?></th>
            <th><?php echo number_format($totalVagasCriadas,0,",",".") ?></th>
            <th><?php echo number_format($totalVagasAumento,0,",",".") ?></th>
            <th><?php echo number_format($totalVagasAutorizacao,0,",",".") ?></th>
            <th><?php echo number_format($totalVagasAumento + $totalVagasAutorizacao,0,",",".") ?></th>
        </tr>
    </table>
<?php
}

function carregarDetalhesVagas2($dados) {

    global $db;

    $sql = "SELECT
                periodo,
                SUM(vagasprevistas) AS vagasprevistas,
                SUM(vagascriadas) AS vagascriadas,
                SUM(vagasaumento) AS vagasaumento,
                SUM(vagasautorizacao) AS vagasautorizacao
            FROM (
                SELECT
                    dpe.dpeanoref AS periodo,
                    entcampus.entid AS entidcampus,
                    CASE WHEN sh.indid IN (2620) THEN dsh.dshqtde::integer ELSE 0 END AS vagasprevistas,
                    CASE WHEN sh.indid IN (2678) THEN dsh.dshqtde::integer ELSE 0  END AS vagascriadas,
                    CASE WHEN dsh.tidid1 IN (7263, 7259) THEN dsh.dshqtde::integer ELSE 0  END AS vagasaumento,
                    CASE WHEN dsh.tidid1 IN (7262, 7258) THEN dsh.dshqtde::integer ELSE 0  END AS vagasautorizacao
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                INNER JOIN entidade.entidade entcampus ON entcampus.entid = dsh.entid
                inner join entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
                inner join entidade.funentassoc a on a.fueid = fn.fueid
                inner join entidade.entidade entinst on entinst.entid = a.entid
                WHERE sh.indid IN (2620, 2678)
                AND dpe.dpeanoref <= '2017'
                AND sh.sehstatus <> 'I'
            ) AS FOO
            WHERE entidcampus = {$dados['esiid']}
            GROUP BY periodo
            ORDER BY periodo";
    $arrDados = $db->carregar( $sql, null, 3200 );
    ?>
    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
        <tr>
            <th rowspan="2">Ano</th>
            <th rowspan="2">Vagas previstas</th>
            <th rowspan="2">Vagas criadas</th>
            <th colspan="2">Tipo de vaga</th>
            <th rowspan="2">Total até 2017</th>
        </tr>
        <tr>
            <th>Autorização de vagas (curso novo)</th>
            <th>Aumento de vagas (cursos existentes)</th>
        </tr>
        <?php
        if($arrDados){
            $totalVagasPrevistas = 0;
            $totalVagasCriadas = 0;
            $totalVagasAumento = 0;
            $totalVagasAutorizacao = 0;
            foreach($arrDados as $dado){
                ?>
                <tr>
                    <td class="" ><?php echo $dado['periodo'] ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasprevistas'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagascriadas'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasaumento'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasautorizacao'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasaumento'] + $dado['vagasautorizacao'],0,",",".") ?></td>
                </tr>
                <?php
                $totalVagasPrevistas += $dado['vagasprevistas'];
                $totalVagasCriadas += $dado['vagascriadas'];
                $totalVagasAumento += $dado['vagasaumento'];
                $totalVagasAutorizacao += $dado['vagasautorizacao'];
            }
        }?>
        <tr>
            <th>Total</th>
            <th><?php echo number_format($totalVagasPrevistas,0,",",".") ?></th>
            <th><?php echo number_format($totalVagasCriadas,0,",",".") ?></th>
            <th><?php echo number_format($totalVagasAumento,0,",",".") ?></th>
            <th><?php echo number_format($totalVagasAutorizacao,0,",",".") ?></th>
            <th><?php echo number_format($totalVagasAumento + $totalVagasAutorizacao,0,",",".") ?></th>
        </tr>
    </table>
<?php
}

function carregarDetalhesVagasPrivadas($dados) {

    global $db;

    $sql = "SELECT
                '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharVagasPrivadas2(\''||cod||'\',this);\">&nbsp;' || municipio AS municipio,
                SUM(vagasaumento2823) AS vagasaumento2823,
                SUM(vagasautorizacao2823) AS vagasautorizacao2823,
                SUM(vagasaumento2889) AS vagasaumento2889
            FROM (
                SELECT
                    mun.estuf AS uf,
                    mun.muncod AS cod,
                    mun.mundescricao AS municipio,
                    CASE WHEN sh.indid IN (2823) AND dsh.tidid1 IN (7588) THEN dsh.dshqtde::integer ELSE 0 END AS vagasaumento2823,
                    CASE WHEN sh.indid IN (2823) AND dsh.tidid1 IN (7589) THEN dsh.dshqtde::integer ELSE 0 END AS vagasautorizacao2823,
                    CASE WHEN sh.indid IN (2889) THEN dsh.dshqtde::integer ELSE 0 END AS vagasaumento2889
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::integer
                INNER JOIN territorios.municipio mun ON mun.muncod = ies.iesmuncod
                WHERE sh.indid IN (2823, 2889)
                AND sh.sehstatus <> 'I'
            ) AS FOO
            WHERE uf = '{$dados['esiid']}'
            GROUP BY cod, municipio
            ORDER BY municipio";
    $arrDados = $db->carregar( $sql, null, 3200 );
    ?>
    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
        <tr>
            <th rowspan="2">Município / IES / Ano</th>
            <th colspan="2">Vagas criadas de graduação de medicina</th>
            <th>Vagas reativadas de graduação de medicina</th>
            <th rowspan="2">Total</th>
        </tr>
        <tr>
            <th>Autorização de vagas (curso novo)</th>
            <th>Aumento de vagas (cursos existentes)</th>
            <th>Aumento de vagas (cursos existentes)</th>
        </tr>
        <?php
        if($arrDados){
            $totalvagasaumento2823 = 0;
            $totalvagasautorizacao2823 = 0;
            $totalvagasaumento2889 = 0;
            ?>
            <?php
            foreach($arrDados as $dado){
                ?>
                <tr class="zebrado">
                    <td class="" ><?php echo $dado['municipio'] ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasautorizacao2823'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasaumento2823'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasaumento2889'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasautorizacao2823'] + $dado['vagasaumento2823'] + $dado['vagasaumento2889'],0,",",".") ?></td>
                </tr>
                <?php
                $totalvagasaumento2823 += $dado['vagasaumento2823'];
                $totalvagasautorizacao2823 += $dado['vagasautorizacao2823'];
                $totalvagasaumento2889 += $dado['vagasaumento2889'];
            }
        }?>
        <tr>
            <th>Total</th>
            <th><?php echo number_format($totalvagasautorizacao2823,0,",",".") ?></th>
            <th><?php echo number_format($totalvagasaumento2823,0,",",".") ?></th>
            <th><?php echo number_format($totalvagasaumento2889,0,",",".") ?></th>
            <th><?php echo number_format($totalvagasautorizacao2823 + $totalvagasaumento2823 + $totalvagasaumento2889,0,",",".") ?></th>
        </tr>
    </table>
<?php
}

function carregarDetalhesVagasPrivadas2($dados) {

    global $db;

    $sql = "SELECT
                '<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharVagasPrivadas3(\''||cod||'\',this);\">&nbsp;' || ies AS ies,
                SUM(vagasaumento2823) AS vagasaumento2823,
                SUM(vagasautorizacao2823) AS vagasautorizacao2823,
                SUM(vagasaumento2889) AS vagasaumento2889
            FROM (
                SELECT
                    mun.muncod AS muncod,
                    ies.iesid AS cod,
                    ies.iesdsc AS ies,
                    CASE WHEN sh.indid IN (2823) AND dsh.tidid1 IN (7588) THEN dsh.dshqtde::integer ELSE 0 END AS vagasaumento2823,
                    CASE WHEN sh.indid IN (2823) AND dsh.tidid1 IN (7589) THEN dsh.dshqtde::integer ELSE 0 END AS vagasautorizacao2823,
                    CASE WHEN sh.indid IN (2889) THEN dsh.dshqtde::integer ELSE 0 END AS vagasaumento2889
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::integer
                INNER JOIN territorios.municipio mun ON mun.muncod = ies.iesmuncod
                WHERE sh.indid IN (2823, 2889)
                AND sh.sehstatus <> 'I'
            ) AS FOO
            WHERE muncod = '{$dados['esiid']}'
            GROUP BY cod, ies
            ORDER BY ies";
    $arrDados = $db->carregar( $sql, null, 3200 );
    ?>
    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
        <tr>
            <th rowspan="2">IES / Ano</th>
            <th colspan="2">Vagas criadas de graduação de medicina</th>
            <th>Vagas reativadas de graduação de medicina</th>
            <th rowspan="2">Total</th>
        </tr>
        <tr>
            <th>Autorização de vagas (curso novo)</th>
            <th>Aumento de vagas (cursos existentes)</th>
            <th>Aumento de vagas (cursos existentes)</th>
        </tr>
        <?php
        if($arrDados){
            $totalvagasaumento2823 = 0;
            $totalvagasautorizacao2823 = 0;
            $totalvagasaumento2889 = 0;
            ?>
            <?php
            foreach($arrDados as $dado){
                ?>
                <tr>
                    <td class="" ><?php echo $dado['ies'] ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasautorizacao2823'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasaumento2823'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasaumento2889'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasautorizacao2823'] + $dado['vagasaumento2823'] + $dado['vagasaumento2889'],0,",",".") ?></td>
                </tr>
                <?php
                $totalvagasaumento2823 += $dado['vagasaumento2823'];
                $totalvagasautorizacao2823 += $dado['vagasautorizacao2823'];
                $totalvagasaumento2889 += $dado['vagasaumento2889'];
            }
        }?>
        <tr>
            <th>Total</th>
            <th><?php echo number_format($totalvagasautorizacao2823,0,",",".") ?></th>
            <th><?php echo number_format($totalvagasaumento2823,0,",",".") ?></th>
            <th><?php echo number_format($totalvagasaumento2889,0,",",".") ?></th>
            <th><?php echo number_format($totalvagasautorizacao2823 + $totalvagasaumento2823 + $totalvagasaumento2889,0,",",".") ?></th>
        </tr>
    </table>
<?php
}

function carregarDetalhesVagasPrivadas3($dados) {

    global $db;

    $sql = "SELECT
                periodo,
                SUM(vagasaumento2823) AS vagasaumento2823,
                SUM(vagasautorizacao2823) AS vagasautorizacao2823,
                SUM(vagasaumento2889) AS vagasaumento2889
            FROM (
                SELECT
                    ies.iesid AS cod,
                    dpe.dpeanoref AS periodo,
                    CASE WHEN sh.indid IN (2823) AND dsh.tidid1 IN (7588) THEN dsh.dshqtde::integer ELSE 0 END AS vagasaumento2823,
                    CASE WHEN sh.indid IN (2823) AND dsh.tidid1 IN (7589) THEN dsh.dshqtde::integer ELSE 0 END AS vagasautorizacao2823,
                    CASE WHEN sh.indid IN (2889) THEN dsh.dshqtde::integer ELSE 0 END AS vagasaumento2889
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                INNER JOIN painel.ies ies ON ies.iesid = dsh.dshcod::integer
                INNER JOIN territorios.municipio mun ON mun.muncod = ies.iesmuncod
                WHERE sh.indid IN (2823, 2889)
                AND sh.sehstatus <> 'I'
            ) AS FOO
            WHERE cod = '{$dados['esiid']}'
            GROUP BY periodo
            ORDER BY periodo";
    $arrDados = $db->carregar( $sql, null, 3200 );
    ?>
    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
        <tr>
            <th rowspan="2">Ano</th>
            <th colspan="2">Vagas criadas de graduação de medicina</th>
            <th>Vagas reativadas de graduação de medicina</th>
            <th rowspan="2">Total</th>
        </tr>
        <tr>
            <th>Autorização de vagas (curso novo)</th>
            <th>Aumento de vagas (cursos existentes)</th>
            <th>Aumento de vagas (cursos existentes)</th>
        </tr>
        <?php
        if($arrDados){
            $totalvagasaumento2823 = 0;
            $totalvagasautorizacao2823 = 0;
            $totalvagasaumento2889 = 0;
            ?>
            <?php
            foreach($arrDados as $dado){
                ?>
                <tr class="zebrado">
                    <td class="" ><?php echo $dado['periodo'] ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasautorizacao2823'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasaumento2823'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasaumento2889'],0,",",".") ?></td>
                    <td class="numero" ><?php echo number_format($dado['vagasautorizacao2823'] + $dado['vagasaumento2823'] + $dado['vagasaumento2889'],0,",",".") ?></td>
                </tr>
                <?php
                $totalvagasaumento2823 += $dado['vagasaumento2823'];
                $totalvagasautorizacao2823 += $dado['vagasautorizacao2823'];
                $totalvagasaumento2889 += $dado['vagasaumento2889'];
            }
        }?>
        <tr>
            <th>Total</th>
            <th><?php echo number_format($totalvagasautorizacao2823,0,",",".") ?></th>
            <th><?php echo number_format($totalvagasaumento2823,0,",",".") ?></th>
            <th><?php echo number_format($totalvagasaumento2889,0,",",".") ?></th>
            <th><?php echo number_format($totalvagasautorizacao2823 + $totalvagasaumento2823 + $totalvagasaumento2889,0,",",".") ?></th>
        </tr>
    </table>
<?php
}
?>