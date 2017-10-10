<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

if(isset($_REQUEST['semestre_bolsas'])){
	gerarGraficoBolsasOfertadas($_REQUEST['semestre_bolsas']);
	die;
}
if(isset($_REQUEST['semestre_bolsas2'])){
    gerarGraficoBolsasOcupadas($_REQUEST['semestre_bolsas2']);
    die;
}
?>
<!DOCTYPE HTML>
<html>
<head>
	<meta http-equiv="X-UA-Compatible" content="IE=7" />
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
	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
		.fundo_titulo{background-image:url('../imagens/cockpit/fundo_ed_superior.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
	</style>
	
	<script type="text/javascript">
		jQuery(function(){
			jQuery('.filtrar_semestre').click(function(){
				jQuery('#div_bolsas_ofertadas').load('estrategico.php?modulo=principal/cockpit_prouni&acao=A&semestre_bolsas='+jQuery(this).val());
			});
            jQuery('.filtrar_semestre2').click(function(){
                jQuery('#div_bolsas_ocupadas').load('estrategico.php?modulo=principal/cockpit_prouni&acao=A&semestre_bolsas2='+jQuery(this).val());
            });
		});
	</script>
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_prouni&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="4" ><div style="margin:28px" >PROUNI</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(1493);" width="25%">
            <?php
            $sql = "SELECT
                        SUM(dsh.dshqtde) AS valor,
                        (SELECT dpe1.dpedsc FROM painel.detalheperiodicidade dpe1 WHERE dpe1.perid = 2 AND dpe1.dpedatainicio = min(dpe.dpedatainicio)) AS inicio,
                        (SELECT dpe1.dpedsc FROM painel.detalheperiodicidade dpe1 WHERE dpe1.perid = 2 AND dpe1.dpedatainicio = max(dpe.dpedatainicio)) AS fim
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (1493)
                    AND sh.sehstatus <> 'I'";
            $dados = $db->pegaLinha($sql, null, 3200);

            $sql = "SELECT
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE sh.indid IN (1493)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 IN (3499, 3500)";
            $totalProuni = $db->pegaUm($sql, null, 3200);

            $sql = "SELECT
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE sh.indid IN (1493)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 IN (4797)";
            $totalProies = $db->pegaUm($sql, null, 3200);
            ?>
            <?php exibirTitulo('doc', number_format($dados['valor'],0,",",".").' bolsas ofertadas<br><font size=-1>('.number_format($totalProuni,0,",",".").' bolsas PROUNI e<br>'.number_format($totalProies,0,",",".").' bolsas PROIES)</font>', 'Cumulativo<br>'.$dados['inicio'].' - '.$dados['fim']); ?>
		</td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2894);" width="25%">
            <?php
            $sql = "SELECT
                        SUM(dsh.dshqtde) AS valor,
                        (SELECT dpe1.dpedsc FROM painel.detalheperiodicidade dpe1 WHERE dpe1.perid = 2 AND dpe1.dpedatainicio = min(dpe.dpedatainicio)) AS inicio,
                        (SELECT dpe1.dpedsc FROM painel.detalheperiodicidade dpe1 WHERE dpe1.perid = 2 AND dpe1.dpedatainicio = max(dpe.dpedatainicio)) AS fim
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2894)
                    AND sh.sehstatus <> 'I'";
            $dados = $db->pegaLinha($sql, null, 3200);

            $sql = "SELECT
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE sh.indid IN (2894)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 IN (7823)";
            $totalProuni = $db->pegaUm($sql, null, 3200);

            $sql = "SELECT
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE sh.indid IN (2894)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 IN (7824)";
            $totalProies = $db->pegaUm($sql, null, 3200);
            ?>
            <?php exibirTitulo('doc', number_format($dados['valor'],0,",",".").' bolsas ocupadas<br><font size=-1>('.number_format($totalProuni,0,",",".").' bolsas PROUNI e<br>'.number_format($totalProies,0,",",".").' bolsas PROIES)</font>', 'Cumulativo<br>'.$dados['inicio'].' - '.$dados['fim']); ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2892);" width="25%">
            <?php
            $sql = "SELECT
                        SUM(dsh.dshqtde) AS valor,
                        (SELECT dpe1.dpedsc FROM painel.detalheperiodicidade dpe1 WHERE dpe1.perid = 2 AND dpe1.dpedatainicio = min(dpe.dpedatainicio)) AS inicio,
                        (SELECT dpe1.dpedsc FROM painel.detalheperiodicidade dpe1 WHERE dpe1.perid = 2 AND dpe1.dpedatainicio = max(dpe.dpedatainicio)) AS fim
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2892)
                    AND sh.sehstatus <> 'I'";
            $dados = $db->pegaLinha($sql, null, 3200);

            $sql = "SELECT
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE sh.indid IN (2892)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 IN (7819)";
            $totalProuni = $db->pegaUm($sql, null, 3200);

            $sql = "SELECT
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE sh.indid IN (2892)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid2 IN (7820)";
            $totalProies = $db->pegaUm($sql, null, 3200);
            ?>
            <?php exibirTitulo('doc', number_format($dados['valor'],0,",",".").' bolsas ativas<br><font size=-1>('.number_format($totalProuni,0,",",".").' bolsas PROUNI e<br>'.number_format($totalProies,0,",",".").' bolsas PROIES)</font>', 'Cumulativo<br>'.$dados['inicio'].' - '.$dados['fim']); ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2891);" width="25%">
            <?php
            $sql = "SELECT
                        SUM(dsh.dshqtde) AS valor,
                        (SELECT dpe1.dpedsc FROM painel.detalheperiodicidade dpe1 WHERE dpe1.perid = 2 AND dpe1.dpedatainicio = min(dpe.dpedatainicio)) AS inicio,
                        (SELECT dpe1.dpedsc FROM painel.detalheperiodicidade dpe1 WHERE dpe1.perid = 2 AND dpe1.dpedatainicio = max(dpe.dpedatainicio)) AS fim
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2891)
                    AND sh.sehstatus <> 'I'";
            $dados = $db->pegaLinha($sql, null, 3200);

            $sql = "SELECT
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE sh.indid IN (2891)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid1 IN (7817)";
            $totalProuni = $db->pegaUm($sql, null, 3200);

            $sql = "SELECT
                        SUM(dsh.dshqtde) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    WHERE sh.indid IN (2891)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid1 IN (7818)";
            $totalProies = $db->pegaUm($sql, null, 3200);
            ?>
            <?php exibirTitulo('doc', number_format($dados['valor'],0,",",".").' bolsas encerradas<br>por conclusão de curso<br><font size=-1>('.number_format($totalProuni,0,",",".").' bolsas PROUNI e<br>'.number_format($totalProies,0,",",".").' bolsas PROIES)</font>', 'Cumulativo<br>'.$dados['inicio'].' - '.$dados['fim']); ?>
        </td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
    <tr>
        <td class="fundo_padrao">
            <div class="link" onclick="abreIndicadorPopUp(1493);" width="33%">
                <?php exibirTitulo('indicador', 'Bolsas ofertadas'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_semestre" name="semestre" value="1" checked="checked" onclick="" />1º Semestre
                <input type="radio" class="filtrar_semestre" name="semestre" value="2" onclick="" />2º Semestre
                <input type="radio" class="filtrar_semestre" name="semestre" value="" onclick="" />Total
            </div>

            <div id="div_bolsas_ofertadas">
                <?php gerarGraficoBolsasOfertadas(1); ?>
            </div>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1493);" width="34%">
            <div>
                <?php exibirTitulo('indicador', 'Bolsas ofertadas<br>por modalidade de concorrência'); ?>
            </div>
            <?php
            $sql = "select
						tid1.tiddsc as descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (1493)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            geraGrafico($arrDados, "graficoPizzaConcorrencia", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(1493);" width="33%">
            <div>
                <?php exibirTitulo('indicador', 'Bolsas ofertadas<br>por modalidade de bolsa'); ?>
            </div>
            <?php
            $sql = "select
						tid2.tiddsc as descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
					where sh.indid in (1493)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            geraGrafico($arrDados, "graficoPizzaBolsa", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2894);">
            <div>
                <?php exibirTitulo('indicador', 'Bolsas ocupadas<br>por gênero'); ?>
            </div>
            <?php
            $sql = "select
						tid1.tiddsc as descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (2894)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaBolsaGenero", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2893);">
            <div>
                <?php exibirTitulo('indicador', 'Bolsas ocupadas<br>por cor/raça'); ?>
            </div>
            <?php
            $sql = "select
						tid1.tiddsc as descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (2893)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaBolsaCor", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2787);">
            <div>
                <?php exibirTitulo('indicador', 'Bolsas ocupadas<br>por faixa etária'); ?>
            </div>
            <?php
            $sql = "select
						tid2.tiddsc as descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
					where sh.indid in (2787)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaBolsaFaixa", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
    </tr>
	<tr>
        <td class="fundo_padrao" rowspan="2">
            <div class="link" onclick="abreIndicadorPopUp(2894);">
                <?php exibirTitulo('indicador', 'Bolsas ocupadas'); ?>
            </div>
            <div style="width:100%;text-align:center" >
                <input type="radio" class="filtrar_semestre2" name="semestre2" value="1" checked="checked" onclick="" />1º Semestre
                <input type="radio" class="filtrar_semestre2" name="semestre2" value="2" onclick="" />2º Semestre
                <input type="radio" class="filtrar_semestre2" name="semestre2" value="" onclick="" />Total
            </div>

            <div id="div_bolsas_ocupadas">
                <?php gerarGraficoBolsasOcupadas(1); ?>
            </div>
        </td>


        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2892);" rowspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Bolsas ativas por situação'); ?>
            </div>
            <?php
            $sql = "select
						tid1.tiddsc as descricao,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					where sh.indid in (2892)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar($sql, null, 3200);
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaBolsaSituacao", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',3,13,'','');" align="center" >
            <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <!-- FIM Tabela Alinhamento Estratégico -->
	</tr>
    <tr>
        <!-- Tabela Processos -->
        <td class="fundo_padrao link" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/processos/prouni/default.htm')";>
            <div>
                <?php exibirTitulo('recycle', 'Mapa do Processo'); ?>
            </div>
        </td>
        <!-- Tabela Processos -->
    </tr>
	<tr>
		<td class="fundo_padrao link" colspan="3" onclick="abreIndicadorPopUp(2290)";>
			<div>
				<?php exibirTitulo('financeiro', 'Orçamentário/Financeiro - Renúncia Fiscal'); ?>
			</div>
			<?php
			$sql = "select
						dpedsc as periodo,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					where sh.indid in (2290)
					and sh.sehstatus <> 'I'
					group by periodo
					order by periodo";
			$arrDados = $db->carregar( $sql, null, 3200 );
			if($arrDados){
				foreach($arrDados as $dado){
					$arrTipo[$dado['periodo']] = $dado['valor'];
				}
			}
			?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<th class="center bold" >Ano</th>
					<th class="center bold" >2005</th>
					<th class="center bold" >2006</th>
					<th class="center bold" >2007</th>
					<th class="center bold" >2008</th>
					<th class="center bold" >2009</th>
					<th class="center bold" >2010</th>
					<th class="center bold" >2011</th>
					<th class="center bold" >2012</th>
					<th class="center bold" >2013*</th>
					<th class="center bold" >2014*</th>
					<th class="center bold" >2015*</th>
				</tr>
				<tr>
					<td class="bold">R$</td>
					<td class="numero" ><?php echo number_format($arrTipo['2005'],2,",",".") ?></td>
					<td class="numero" ><?php echo number_format($arrTipo['2006'],2,",",".") ?></td>
					<td class="numero" ><?php echo number_format($arrTipo['2007'],2,",",".") ?></td>
					<td class="numero" ><?php echo number_format($arrTipo['2008'],2,",",".") ?></td>
					<td class="numero" ><?php echo number_format($arrTipo['2009'],2,",",".") ?></td>
					<td class="numero" ><?php echo number_format($arrTipo['2010'],2,",",".") ?></td>
					<td class="numero" ><?php echo number_format($arrTipo['2011'],2,",",".") ?></td>
					<td class="numero" ><?php echo number_format($arrTipo['2012'],2,",",".") ?></td>
					<td class="numero" ><?php echo number_format($arrTipo['2013'],2,",",".") ?></td>
					<td class="numero" ><?php echo number_format($arrTipo['2014'],2,",",".") ?></td>
					<td class="numero" ><?php echo number_format($arrTipo['2015'],2,",",".") ?></td>
				</tr>
			</table>
			<table align="right">
			<tr>
			<td><b>*</b>Valores Estimados</td>
			</tr>
			</table>
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
function gerarGraficoBolsasOfertadas($semestre)
{
	global $db;
	
	if($semestre){
		$where = "AND dpe.dpeordem = {$semestre}";
	}
	$sql = "select
				dpeanoref as descricao,
				sum(dsh.dshqtde) as valor
			from painel.seriehistorica sh
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			where sh.indid in (1493)
			and sh.sehstatus <> 'I'
			$where
			group by descricao
			order by descricao";
	$dados = $db->carregar( $sql, null, 3200 );

	$formatoValores = array(
		'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
		'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
	);
	geraGraficoColuna($dados, 'graficoColunaProuni', null, null, $formatoValores, null, "", 1000, 600, null, 300);
}

function gerarGraficoBolsasOcupadas($semestre)
{
    global $db;

    if($semestre){
        $where = "AND dpe.dpeordem = {$semestre}";
    }
    $sql = "select
				dpe.dpeanoref as ano,
				tid2.tiddsc AS tipo,
				sum(dsh.dshqtde) as valor
			from painel.seriehistorica sh
			inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
			inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
			INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
			where sh.indid in (2894)
			and sh.sehstatus <> 'I'
			$where
			group by ano, tipo
			order by ano, tipo";
    $dados = $db->carregar( $sql, null, 3200 );

    $dadosAgrupados = agruparDadosGrafico($dados, 'ano', 'tipo', 'valor');

    $formatoValores = array(
        'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
        'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />Porcentagem: <b>' + number_format(this.percentage, 2, ',', '.') + '%</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
    );

    geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaProuniOcupadas', null, null, $formatoValores, null, null, 1000, 600, null, 300);

}
?>