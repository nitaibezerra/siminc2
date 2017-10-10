<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
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
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('../imagens/cockpit/fundo_ed_superior.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_bolsa_permanencia&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Bolsa Permanência</div></td>
	</tr>
	<tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2010);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Instituições homologadas'); ?>
            </div>
            <?php
            $sql = "select
						tid1.tiddsc as descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (2010)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaInscricoesHomologadas", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, "", false, '#fff', true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3126);" width="34%">
            <div>
                <?php exibirTitulo('indicador', 'Bolsistas homologados'); ?>
            </div>
            <br>
            <?php
            $sql = "SELECT
                        periodo,
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    INNER JOIN
                        (SELECT Replace(Replace(d.dpedsc,'º ','o<br>'),'Semestre','Sem.') AS periodo, d.dpedatainicio, d.dpedatafim, d.dpeanoref||d.dpeordem AS ordem FROM painel.detalheperiodicidade d WHERE d.perid = 2) as foo ON dpe.dpedatainicio BETWEEN foo.dpedatainicio AND foo.dpedatafim
                    WHERE sh.indid in (3126)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo, ordem
                    ORDER BY ordem";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrX = array();
                $arrA = array();
                $arrTotal = array();
                $vTotal = 0;

                foreach($arrDados as $dado)
                {
                    $arrX[] = $dado['periodo'];
                    $vA = Round($dado['valor']);
                    $vTotal += $vA;
                    $arrA[] = $vA;
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    //array('name' => 'Bolsistas', 'data'=>$arrA),
                    array('name' => 'Bolsistas', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 2, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaBolsistasHomologados", "Bolsistas homologados",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3123);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Bolsistas homologados<br>por grupo de bolsistas'); ?>
            </div>
            <?php
            $sql = "select
						tid1.tiddsc as descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (3123)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaBolsistasHomologadosGrupo", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, "", false, '#fff', true);
            }
            ?>
        </td>
	</tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3123);">
            <div>
                <?php exibirTitulo('indicador', 'Bolsistas homologados por gênero'); ?>
            </div>
            <?php
            $sql = "select
						tid2.tiddsc as descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
					where sh.indid in (3123)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaBolsistasHomologadosGenero", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, "", false, '#fff', true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3125);">
            <div>
                <?php exibirTitulo('indicador', 'Renda per capita média dos bolsistas'); ?>
            </div>
            <br>
            <?php
            $sql = "SELECT acao, periodo, valor
            FROM (
                SELECT 1 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshvalor)/SUM(dsh.dshqtde) AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                WHERE sh.indid in (3125)
                AND sh.sehstatus <> 'I'
                GROUP BY periodo
            UNION ALL
                SELECT 2 AS acao, dpe.dpeanoref AS periodo, (SUM(dsh.dshvalor)/SUM(dsh.dshqtde)) * ipc.ipcindice AS valor
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                INNER JOIN painel.ipca ipc ON ipc.ipcano = dpe.dpeanoref
                WHERE sh.indid in (3125)
                AND sh.sehstatus <> 'I'
                GROUP BY periodo, ipc.ipcindice
            ) AS FOO
            ORDER BY periodo";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrSituacao = array();
                foreach($arrDados as $dado){
                    $arrSituacao[$dado['periodo']][$dado['acao']]['total'][] = $dado['valor'];
                }

                $arrX = array();
                $arrA = array();
                $arrB = array();
                $vA = 0;
                $vB = 0;

                foreach($arrSituacao as $periodo => $sit)
                {
                    $arrX[] = $periodo;
                    $vA += ((is_array($sit['1']['total'])?array_sum($sit['1']['total']):0));
                    $vB += ((is_array($sit['2']['total'])?array_sum($sit['2']['total']):0));
                    $arrA[] = $vA;
                    $arrB[] = $vB;
                }

                $aDados = array(
                    array('name' => 'Reais', 'data'=>$arrA),
                    array('name' => 'Reais (Indice IPCA)', 'data'=>$arrB),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return 'R$' + number_format(this.value, 2, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );



/*
            $sql = "SELECT Replace(dpe.dpedsc,'º','o') AS periodo, SUM(dsh.dshvalor)/SUM(dsh.dshqtde) AS valor, dpe.dpeanoref, dpe.dpeordem
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (3125)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo, dpe.dpeanoref, dpe.dpeordem
                    ORDER BY dpe.dpeanoref, dpe.dpeordem";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                $arrX = array();
                $arrA = array();
                $arrTotal = array();
                $vTotal = 0;

                foreach($arrDados as $dado)
                {
                    $arrX[] = $dado['periodo'];
                    $vA = Round($dado['valor'],2);
                    $vTotal += $vA;
                    $arrA[] = $vA;
                    //$arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Reais', 'data'=>$arrA),
                    //array('name' => 'Total Reais', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return 'R$ ' + number_format(this.value, 2, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: R$ <b>' + number_format(this.y, 2, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );
*/

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaRendaBolsistas", "Renda per capita média dos bolsistas",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3125);">
            <div>
                <?php exibirTitulo('indicador', 'Renda per capita média<br>por grupos de bolsistas*'); ?>
            </div>
            <?php
            $sql = "SELECT
                        Replace(dpe.dpedsc,'º Semestre','º') AS periodo,
						tid1.tiddsc as grupo,
						SUM(dsh.dshvalor)/SUM(dsh.dshqtde) as valor, dpe.dpeanoref, dpe.dpeordem
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					WHERE sh.indid IN (3125)
					AND sh.sehstatus <> 'I'
					GROUP BY periodo, grupo, dpe.dpeanoref, dpe.dpeordem
					ORDER BY dpe.dpeanoref, dpe.dpeordem, grupo";
            $dados = $db->carregar($sql, null, 3200);

            $dadosAgrupados = agruparDadosGrafico($dados, 'periodo', 'grupo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return 'R$ ' + number_format(this.value, 2, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: R$ <b>' + number_format(this.y, 2, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaRendaBolsistasGrupos', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
            echo "<br><br>";
            echo"<table><tr><td class='bold'>*Valores correntes</td></tr></table>";
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3126);">
            <div>
                <?php exibirTitulo('indicador', 'Distribuição de bolsas<br>por tipo de instituição'); ?>
            </div>
            <?php
            $sql = "select
						tid2.tiddsc as descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
					where sh.indid in (3126)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaDistribuicaoBolsasTipo", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, "", false, '#fff', true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3126);" rowspan="2" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Distribuição de bolsas por áreas'); ?>
            </div>
            <?php
            $sql = "SELECT SUM(dsh.dshqtde)
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE sh.indid in (3126)
                    AND sh.sehstatus <> 'I'";
            $totalBolsistas = $db->pegaUm( $sql );

            $sql = "SELECT tid1.tiddsc as descricao, SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    WHERE sh.indid in (3126)
                    AND sh.sehstatus <> 'I'
                    GROUP BY descricao
                    ORDER BY valor DESC";
            $arrDados = $db->carregar( $sql, null, 3200 );
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>Área</th>
                    <th>Quantidade</th>
                    <th>Percentual</th>
                </tr>
                <?php
                if($arrDados){
                    $count = -1;
                    $valorTotal = 0;
                    $percentualTotal = 0;
                    foreach($arrDados as $dado):
                        $count++;
                        $percentualBolsistas = ($dado['valor'] / $totalBolsistas) * 100;
                        ?>
                        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                            <td><?php echo $dado['descricao'] ?></td>
                            <td class="numero" ><?php echo number_format($dado['valor'],0,",",".") ?></td>
                            <td class="numero" ><?php echo number_format($percentualBolsistas,2,",",".") ?>%</td>
                        </tr>
                    <?php
                    $valorTotal += $dado['valor'];
                    $percentualTotal += $percentualBolsistas;
                    endforeach;
                }
                ?>
                <tr>
                    <th>Total</th>
                    <th><?php echo number_format($valorTotal,0,",",".") ?></th>
                    <th><?php echo number_format($percentualTotal,2,",",".") ?>%</th>
                </tr>
            </table>
        </td>

    </tr>
	<tr>
        <!--
        <td class="fundo_padrao" colspan="2">
            <div>
                <?php exibirTitulo('financeiro', 'Orçamentário / Financeiro'); ?>
            </div>
            <?=exibirTabelaFinanceiro(164);?>
        </td>
        -->
		<!-- Tabela Alinhamento Estratégico -->
		<td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',3,164,'','');" align="center" >
			<?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
			<img src="cockpit/images/alinhamentoEstrategico.png" />
		</td>
		<!-- FIM Tabela Alinhamento Estratégico -->
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