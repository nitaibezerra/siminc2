<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
if(isset($_REQUEST['detalhe_hospital1'])){
    gerarGraficoHospital1($_REQUEST['detalhe_hospital1']);
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
            jQuery('.filtrar_hospital1').click(function(){
                jQuery('#div_hospital1').load('estrategico.php?modulo=principal/cockpit_ebserh&acao=A&detalhe_hospital1='+jQuery(this).val());
            });
        });
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
        .fundo_titulo{background-image:url('cockpit/images/banner/bannerMaisMedicos.jpg')}
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_ebserh&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >EBSERH</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(3150);" width="33%">
			<div>
				<?php exibirTitulo('indicador', 'Percentual de hospitais conforme<br>situação de adesão à EBSERH'); ?>
			</div>
            <?php
            $sql = "select
                        CASE
                            WHEN tid1.tidid IN (8671) THEN 'Em processo de adesão'
                            WHEN tid1.tidid IN (8670) THEN 'Contrato assinado'
                        ELSE tid1.tiddsc
                        END AS descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (3150)
					and sh.sehstatus <> 'I'
					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid and sh.sehstatus <> 'I')
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaPercentualHospitais", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
		</td>
        <td class="fundo_padrao" width="67%" colspan="2" rowspan="3">
            <div class="link" onclick="abreIndicadorPopUp(3150);">
                <?php exibirTitulo('indicador', 'Universidades e hospitais, por situação e fase de adesão à EBSERH'); ?>
            </div>
            <br>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_hospital1" name="hospital1" value="1" checked="checked" onclick="" />Total
                <input type="radio" class="filtrar_hospital1" name="hospital1" value="2" onclick="" />Sem Adesão
                <input type="radio" class="filtrar_hospital1" name="hospital1" value="3" onclick="" />Em processo
                <input type="radio" class="filtrar_hospital1" name="hospital1" value="4" onclick="" />Contrato assinado
            </div>
            <br>
            <div id="div_hospital1">
                <?php gerarGraficoHospital1(1); ?>
            </div>
        </td>
	</tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3150);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Número de hospitais<br>por situação de adesão'); ?>
            </div>
            <?php
            $sql = "SELECT
                    SUBSTRING(dpe.dpedsc,1,3) ||'<br>'||dpe.dpeanoref AS periodo,
                    CASE
                        WHEN tid1.tidid IN (8671) THEN 'Em processo de adesão'
                        WHEN tid1.tidid IN (8670) THEN 'Contrato assinado'
                    ELSE tid1.tiddsc
                    END AS situacao,
                    SUM(dsh.dshqtde::integer) as valor, dpe.dpeanoref, dpe.dpeordem
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                WHERE sh.indid IN (3150)
                AND sh.sehstatus <> 'I'
                GROUP BY periodo, situacao, dpe.dpeanoref, dpe.dpeordem
                ORDER BY dpe.dpeanoref, dpe.dpeordem, situacao";
            $arrDados = $db->carregar( $sql );

            $dadosAgrupados = agruparDadosGrafico($arrDados, 'periodo', 'situacao', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaHospitaisSituacao', null, null, $formatoValores, null, "estrategico.php?modulo=principal/popupDetalhesPronatec&acao=A", 1000, 600, null, 300);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3150);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Número de hospitais em processo<br>de adesão, por fase'); ?>
            </div>
            <?php
            $sql = "SELECT
                    SUBSTRING(dpe.dpedsc,1,3) ||'<br>'||dpe.dpeanoref AS periodo,
                    CASE
                            WHEN tid2.tidid IN (8674) THEN 'Fase I (Dimensionamento)'
                            WHEN tid2.tidid IN (8675) THEN 'Fase II (DEST/MP)'
                            WHEN tid2.tidid IN (8676) THEN 'Fase III (Trâmites finais)'
                    ELSE tid2.tiddsc
                    END AS fase,
                    SUM(dsh.dshqtde::integer) as valor, dpe.dpeanoref, dpe.dpeordem
                FROM painel.seriehistorica sh
                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                WHERE sh.indid IN (3150)
                AND sh.sehstatus <> 'I'
                AND dsh.tidid2 NOT IN (8673) --Fase 0
                GROUP BY periodo, fase, dpe.dpeanoref, dpe.dpeordem
                ORDER BY dpe.dpeanoref, dpe.dpeordem, fase";
            $arrDados = $db->carregar( $sql );

            $dadosAgrupados = agruparDadosGrafico($arrDados, 'periodo', 'fase', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
            );
            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaHospitaisFase', null, null, $formatoValores, null, "estrategico.php?modulo=principal/popupDetalhesPronatec&acao=A", 1000, 600, null, 300);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Hospitais Universitários com o AGHU<br>implantado e ampliado', '<span class="link" onclick="abreIndicadorPopUp(3164);">Implantado</span>, <span class="link" onclick="abreIndicadorPopUp(3166);">Ampliado</span>'); ?>
            </div>
            <?php
            $sql = "SELECT
                        Replace(dpe.dpedsc,'º Semestre','o Sem.') AS periodo,
						CASE
						    WHEN sh.indid IN (3164) THEN 'Implantado'
						    WHEN sh.indid IN (3166) THEN 'Ampliado'
						END AS grupo,
						sum(dsh.dshqtde) as valor, dpe.dpeanoref, dpe.dpeordem
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					WHERE sh.indid IN (3164, 3166)
					AND sh.sehstatus <> 'I'
					GROUP BY periodo, grupo, dpe.dpeanoref, dpe.dpeordem
					ORDER BY dpe.dpeanoref, dpe.dpeordem, grupo";
            $dados = $db->carregar($sql, null, 3200);

            $dadosAgrupados = agruparDadosGrafico($dados, 'periodo', 'grupo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

            geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaHospitaisImplantadoAmpliado', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3165);" width="34%">
            <div>
                <?php exibirTitulo('indicador', 'Processos mapeados e desenhados<br>nos hospitais'); ?>
            </div>
            <?php
            $sql = "SELECT SUBSTRING(dpe.dpedsc,1,3) ||'<br>'||dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor, dpe.dpeanoref, dpe.dpeordem
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (3165)
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
                    $vA = Round($dado['valor']);
                    $vTotal += $vA;
                    $arrA[] = $vA;
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Processos', 'data'=>$arrA),
                    array('name' => 'Total de Processos', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaProcessosHospitais", "Processos mapeados e desenhados nos hospitais",300, $formatoValores);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3168);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Equipamentos de TIC nos hospitais<br>por tipo e valor'); ?>
            </div>
            <?php
            $sql = "SELECT
                        Replace(dpe.dpedsc,'º Semestre','o Sem.') AS periodo,
                        dsh.tidid1 AS etapa,
                        sum(dsh.dshqtde) as total, dpe.dpeanoref, dpe.dpeordem
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid in (3168)
                    AND sh.sehstatus <> 'I'
                    GROUP BY periodo, etapa, dpe.dpeanoref, dpe.dpeordem
                    ORDER BY dpe.dpeanoref, dpe.dpeordem, etapa";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                foreach($arrDados as $dado){
                    $arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
                }

                $arrX = array();
                $arrA = array();
                $arrB = array();
                $arrC = array();
                $arrD = array();
                $arrE = array();
                $arrTotal = array();
                $vTotal = 0;

                foreach($arrEtapa as $periodo => $sit)
                {
                    $arrX[] = $periodo;
                    $vA = Round((is_array($sit['8854']['total'])?array_sum($sit['8854']['total']):0));
                    $vB = Round((is_array($sit['8855']['total'])?array_sum($sit['8855']['total']):0));
                    $vC = Round((is_array($sit['8856']['total'])?array_sum($sit['8856']['total']):0));
                    $vD = Round((is_array($sit['8857']['total'])?array_sum($sit['8857']['total']):0));
                    $vE = Round((is_array($sit['8858']['total'])?array_sum($sit['8858']['total']):0));

                    $arrA[] = $vA;
                    $vTotal += $vA;
                    $arrB[] = $vB;
                    $vTotal += $vB;
                    $arrC[] = $vC;
                    $vTotal += $vC;
                    $arrD[] = $vD;
                    $vTotal += $vD;
                    $arrE[] = $vE;
                    $vTotal += $vE;
                    $arrTotal[] = $vTotal;
                }

                $aDados = array(
                    array('name' => 'Ativos de rede', 'data'=>$arrA),
                    array('name' => 'Microcomputadores', 'data'=>$arrB),
                    array('name' => 'Servidores', 'data'=>$arrC),
                    array('name' => 'Storage', 'data'=>$arrD),
                    array('name' => 'Telefonia', 'data'=>$arrE),
                    array('name' => 'Total', 'data'=>$arrTotal),
                );

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                    'alinhamento' => "formatter: function() { return '1'; }"
                );

                echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoEquipamentosTIC", "Equipamentos de TIC nos hospitais",300, $formatoValores);
            }
            ?>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1848);" width="25%">
            <div>
                <?php exibirTitulo('indicador', 'Quadro de pessoal no<br>momento do dimensionamento'); ?>
            </div>
            <?php
            $sql = "select
                        dpe.dpedsc as descricao,
                        sum(dsh.dshqtde) as valor
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    where sh.indid in (1848)
                    and sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
                    group by descricao
                    order by descricao";
            $arrInscricoes = $db->pegaLinha( $sql, null, 3200 );
            ?>
            <div>
                <div style="float: left; width: 100%;"> <?php exibirTitulo('executiverel', 'Funcionários<br />'.number_format($arrInscricoes['valor'],0,",","."), 'Não cumulativo<br>('.$arrInscricoes['descricao'].')'); ?></div>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3169);" width="25%">
            <div>
                <?php exibirTitulo('indicador', 'Quadro Previsto de<br>Funcionários nos hospitais'); ?>
            </div>
            <?php
            $sql = "select
                        dpe.dpedsc as descricao,
                        sum(dsh.dshqtde) as valor
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    where sh.indid in (3169)
                    and sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
                    group by descricao
                    order by descricao";
            $arrInscricoes = $db->pegaLinha( $sql, null, 3200 );
            ?>
            <div>
                <div style="float: left; width: 100%;"> <?php exibirTitulo('executiverel', 'Funcionários<br />'.number_format($arrInscricoes['valor'],0,",","."), 'Não cumulativo<br>('.$arrInscricoes['descricao'].')'); ?></div>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3171);" width="25%">
            <div>
                <?php exibirTitulo('indicador', 'Acréscimo de força de<br>trabalho nos hospitais'); ?>
            </div>
            <?php
            $sql = "select
                        dpe.dpedsc as descricao,
                        sum(dsh.dshqtde) as valor
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    where sh.indid in (3171)
                    and sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
                    group by descricao
                    order by descricao";
            $arrInscricoes = $db->pegaLinha( $sql, null, 3200 );
            ?>
            <div>
                <div style="float: left; width: 100%;"> <?php exibirTitulo('executiverel', 'Profissionais<br />'.number_format($arrInscricoes['valor'],0,",","."), 'Não cumulativo<br>('.$arrInscricoes['descricao'].')'); ?></div>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3174);" width="25%">
            <div>
                <?php exibirTitulo('indicador', 'Percentual de acréscimo de<br>força de trabalho nos hospitais'); ?>
            </div>
            <?php
            $sql = "select
                        dpe.dpedsc as descricao,
                        sum(dsh.dshqtde)/count(0) as valor
                    from painel.seriehistorica sh
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    where sh.indid in (3174)
                    and sh.sehstatus <> 'I'
                    AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
                    group by descricao
                    order by descricao";
            $arrInscricoes = $db->pegaLinha( $sql, null, 3200 );
            ?>
            <div>
                <div style="float: left; width: 100%;"> <?php exibirTitulo('executiverel', 'Funcionários<br />'.number_format($arrInscricoes['valor'],2,",",".").'%', 'Não cumulativo<br>('.$arrInscricoes['descricao'].')'); ?></div>
            </div>
        </td>
    </tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1848);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Quadro de pessoal no momento<br>do dimensionamento por vínculo'); ?>
            </div>
            <?php
            $sql = "select
                        tid1.tiddsc AS descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (1848)
					and sh.sehstatus <> 'I'
					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid and sh.sehstatus <> 'I')
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaQuadroPessoal", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3172);" width="34%">
            <div>
                <?php exibirTitulo('indicador', 'Concursos nos hospitais<br>por situação'); ?>
            </div>
            <?php
            $sql = "select
                        tid1.tiddsc AS descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (3172)
					and sh.sehstatus <> 'I'
					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid and sh.sehstatus <> 'I')
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaConcursoHospitais", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3170);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Vagas em concursos em hospitais<br>por área'); ?>
            </div>
            <?php
            $sql = "select
                        tid1.tiddsc AS descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (3170)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaVagasHospitais", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(3113);" colspan="3">
            <div>
                <?php exibirTitulo('indicador', 'Evolução da execução orçamentária da EBSERH'); ?>
            </div>
            <?php
            $sql = "SELECT acao, periodo, valor
                    FROM (
                        SELECT 1 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        WHERE sh.indid in (3113)
                        AND sh.sehstatus <> 'I'
                        GROUP BY periodo
                    UNION ALL
                        SELECT 2 AS acao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde) * ipc.ipcindice AS valor
                        FROM painel.seriehistorica sh
                        INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                        INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                        INNER JOIN painel.ipca ipc ON ipc.ipcano = dpe.dpeanoref
                        WHERE sh.indid in (3113)
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

                foreach($arrSituacao as $periodo => $sit)
                {
                    $arrX[] = ($periodo=='2014' ? '*2014' : $periodo);
                    $vA = ((is_array($sit['1']['total'])?array_sum($sit['1']['total']):0));
                    $vB = ((is_array($sit['2']['total'])?array_sum($sit['2']['total']):0));
                    $arrA[] = $vA;
                    $arrB[] = $vB;
                    $arrTotal[] = $vA + $vB;
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

                echo geraGraficoLinha($aDados, $arrX, "graficoLinhaEvolucaoEBSERH", "Evolução da execução orçamentária da EBSERH",300, $formatoValores);
                echo "<br><br>";
                echo"<table><tr><td class='bold'>*2014: Previsão</td></tr></table>";
            }
            ?>
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
function gerarGraficoHospital1($detalhe){

    global $db;

    if($detalhe==2){
        $where = "AND dsh.tidid1 IN (8668)";
    }elseif($detalhe==3){
        $where = "AND dsh.tidid1 IN (8671)";
    }elseif($detalhe==4){
        $where = "AND dsh.tidid1 IN (8670)";
    }

    $sql = "SELECT DISTINCT
                entinst.entsig AS universidade,
                enthospital.entnome AS hospital,
                CASE
                    WHEN tid1.tidid IN (8671) THEN 'Em processo'
                    WHEN tid1.tidid IN (8670) THEN 'Contrato assinado'
                ELSE tid1.tiddsc
                END AS situacao,
                tid1.tidid AS idsituacao,
                CASE
                    WHEN tid2.tidid IN (8673) THEN ''
                    WHEN tid2.tidid IN (8674) THEN 'Fase I (Dimensionamento)'
                    WHEN tid2.tidid IN (8675) THEN 'Fase II (DEST/MP)'
                    WHEN tid2.tidid IN (8676) THEN 'Fase III (Trâmites finais)'
                ELSE tid2.tiddsc
                END AS fase
            FROM painel.seriehistorica sh
            INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
            INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
            INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
            INNER JOIN entidade.entidade enthospital on enthospital.entid = dsh.entid
            INNER JOIN entidade.funcaoentidade fn on enthospital.entid = fn.entid and fn.funid = 16 and fn.fuestatus = 'A'
            INNER JOIN entidade.funentassoc a on a.fueid = fn.fueid
            INNER JOIN entidade.entidade entinst on entinst.entid = a.entid
            WHERE sh.indid in (3150)
            AND sh.sehstatus <> 'I'
            and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid and sh.sehstatus <> 'I')
            $where
            ORDER BY universidade, hospital, situacao, fase";
    $arrDados = $db->carregar( $sql, null, 3200 );
    ?>
    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
        <tr>
            <th width="10%">Universidade</th>
            <th width="50%">Hospital</th>
            <th width="20%">Situação</th>
            <th width="20%">Fase</th>
        </tr>
        <?php
        if($arrDados){
            $count = -1;
            foreach($arrDados as $dado):
                $count++;
                ?>
                <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                    <td><?php echo $dado['universidade'] ?></td>
                    <td><?php echo $dado['hospital'] ?></td>
                    <td class="bold <?php echo ($dado['idsituacao']==8668) ? 'fundo_vermelho' : ''; ?><?php echo ($dado['idsituacao']==8671) ? 'fundo_amarelo' : ''; ?><?php echo ($dado['idsituacao']==8670) ? 'fundo_verde' : ''; ?>"><?php echo $dado['situacao'] ?></td>
                    <td><?php echo $dado['fase'] ?></td>
                </tr>
                <?php
            endforeach;
        }
    echo "</table>";
}