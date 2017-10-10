<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
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


    <!-- Add mousewheel plugin -->
    <script type="text/javascript" src="/library/fancybox/helpers/jquery.mousewheel-3.0.6.pack.js"></script>

    <!-- Add fancyBox main JS and CSS files -->
    <script type="text/javascript" src="/library/fancybox/jquery.fancybox.js"></script>
    <link rel="stylesheet" type="text/css" href="/library/fancybox/jquery.fancybox.css" media="screen" />

    <!-- Add Button helper (this is optional) -->
    <link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-buttons.css" />
    <script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-buttons.js"></script>

    <!-- Add Thumbnail helper (this is optional) -->
    <link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-thumbs.css" />
    <script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-thumbs.js"></script>

    <!-- Add Media helper (this is optional) -->
    <script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-media.js"></script>

	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
		.fundo_titulo{background-image:url('../imagens/fundocreche.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
	</style>

    <script>
        function abreDetalheProinfancia(){
            window.open('/pde/estrategico.php?modulo=principal/popupDetalhesProinfancia&acao=A');
        }

        function abreRelatorioVistoria(quadro, prfid, tooid, stoid){
            window.open('/pde/estrategico.php?modulo=principal/popuplistaobras&acao=A&quadro='+quadro+'&prfid='+prfid+'&tooid='+tooid+'&tipo='+stoid);
        }

        $(document).ready(function() {
            $('.fancybox').fancybox();
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_proinfancia_execucao_resumo&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" >Acompanhamento de Obras</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao"width="40%">
			<div>
                <?php exibirTitulo('obras', 'Acompanhamento de Obras', '<span class="link" onclick="abreDetalheProinfancia();">Detalhar Metodologia Inovadora</span>'); ?>
            </div>
			<?php
			$sql = "SELECT
						situacaoobra,
						COALESCE(SUM(coluna1),0) AS coluna1,
						COALESCE(SUM(coluna2),0) AS coluna2,
						COALESCE(SUM(coluna3),0) AS coluna3
					FROM (
						SELECT	CASE 
								WHEN esd.esdid IN (693) THEN '01-Concluída'
								WHEN esd.esdid IN (690) THEN
									CASE
										WHEN COALESCE(o.obrpercentultvistoria,0) > 50 THEN '02-Em obras com mais de 50% de construção'
										WHEN COALESCE(o.obrpercentultvistoria,0) <= 50 THEN '03-Em obras com menos de 50% de construção'
									END
								WHEN esd.esdid IN (763) THEN '04-Licitação'
								WHEN esd.esdid IN (762,689,771,764) THEN '05-Ação Preparatória'
								WHEN esd.esdid IN (861,862,863,870,871,872,864,873,874,875) THEN '06-Em contratação'
								WHEN esd.esdid IN (691) THEN '07-Paralisada'
								WHEN esd.esdid IN (769) THEN '08-Obra Cancelada'
								else esddsc
							END AS situacaoobra,
							CASE WHEN o.preid IS NULL AND o.tooid IN (2,4) THEN COUNT(0) END AS coluna1, 
							CASE WHEN o.preid IS NOT NULL AND o.tpoid NOT IN (104,105) THEN COUNT(0) END AS coluna2,
							CASE WHEN o.preid IS NOT NULL AND o.tpoid IN (104,105) THEN COUNT(0) END AS coluna3,
							esd.esdid,
							esd.esdordem
						from obras2.obras o
						inner join obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
						inner join workflow.documento d ON d.docid = o.docid
						inner join workflow.estadodocumento esd ON esd.esdid = d.esdid
						where o.obrstatus = 'A'
						and e.orgid=3
						and d.esdid NOT IN (770, 884, 768) --Etapa Concluída e Aguardando solicitação do município e Em reformulação
						and o.obridpai is null
						AND e.prfid IN (41) AND ((o.preid IS NULL AND o.tooid IN (2,4)) OR o.preid IS NOT NULL)
						group by situacaoobra, esd.esdid, esd.esdordem, e.prfid, o.preid, o.tooid, o.tpoid
					) AS FOO
					group by situacaoobra
					order by situacaoobra";
			$arrDados = $db->carregar( $sql );
			
//			$arrDados = array(
//                array(
//                    'situacaoobra'=>'01-Concluída',
//                    'coluna1'=>'1140',
//                    'coluna2'=>'221',
//                    'coluna3'=>'1',
//                ),
//                array(
//                    'situacaoobra'=>'02-Em obras com mais de 50% de construção',
//                    'coluna1'=>'654',
//                    'coluna2'=>'943',
//                    'coluna3'=>'0',
//                ),
//                array(
//                    'situacaoobra'=>'03-Em obras com menos de 50% de construção',
//                    'coluna1'=>'132',
//                    'coluna2'=>'581',
//                    'coluna3'=>'27',
//                ),
//                array(
//                    'situacaoobra'=>'04-Licitação',
//                    'coluna1'=>'12',
//                    'coluna2'=>'168',
//                    'coluna3'=>'0',
//                ),
//                array(
//                    'situacaoobra'=>'05-Ação Preparatória',
//                    'coluna1'=>'100',
//                    'coluna2'=>'441',
//                    'coluna3'=>'360',
//                ),
//                array(
//                    'situacaoobra'=>'06-Contrato em Tramitação (empresa/município)',
//                    'coluna1'=>'0',
//                    'coluna2'=>'0',
//                    'coluna3'=>'1971',
//                ),
//                array(
//                    'situacaoobra'=>'07-Paralisada',
//                    'coluna1'=>'411',
//                    'coluna2'=>'308',
//                    'coluna3'=>'0',
//                ),
//                array(
//                    'situacaoobra'=>'08-Obra Cancelada',
//                    'coluna1'=>'94',
//                    'coluna2'=>'9',
//                    'coluna3'=>'0',
//                ),
//            );

			?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<th rowspan="3">&nbsp;</th>
					<th colspan="4">Situação das Creches</th>
				</tr>
				<tr>
					<th rowspan="2">Pré-PAC (Convênios)</th>
					<th colspan="2">PAC 2</th>
					<th rowspan="2">Total</th>
				</tr>
				<tr>
					<th>Método Convencional</th>
					<th>Metodologia Inovadora</th>
				</tr>
				<?php
				$count = -1;
				foreach($arrDados as $dado):
					$count++;
					?>
					<tr class="<?php echo ($count%2) ? 'link zebrado' : ''; ?>">
						<td><?php echo str_replace(array("01-","02-","03-","04-","05-","06-","07-","08-"),"",$dado['situacaoobra']) ?></td>
						<td class="numero" ><?php echo number_format($dado['coluna1'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($dado['coluna2'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($dado['coluna3'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($dado['coluna1'] + $dado['coluna2'] + $dado['coluna3'],0,",",".") ?></td>
					</tr>
				<?php
				$somaColuna1 += $dado['coluna1'];
				$somaColuna2 += $dado['coluna2'];
				$somaColuna3 += $dado['coluna3'];
				$somaColunas = $somaColuna1 + $somaColuna2 + $somaColuna3;
				endforeach ?>
				<tr>
					<th>Total</th>
					<th class="numero" ><?php echo number_format($somaColuna1,0,",",".") ?></th>
					<th class="numero" ><?php echo number_format($somaColuna2,0,",",".") ?></th>
					<th class="numero" ><?php echo number_format($somaColuna3,0,",",".") ?></th>
					<th class="numero" ><?php echo number_format($somaColunas,0,",",".") ?></th>
				</tr>
			</table>

			<div style="margin-top: 10px;">*Ação preparatória em MI abrange as situações: aguardando empenho, em reformulação, aguardando geração de termo e Aguardando solicitação do município</div>

        </td>
        <td class="fundo_padrao link" onclick="abreIndicadorPopUp(2660);" width="60%" rowspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Cronograma de Previsão de Início das Obras'); ?>
            </div>
            <?php
            $sql = "select
						dpe.dpedsc AS periodo,
						tid.tiddsc AS ano,
						sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
					where sh.indid in (2660)
					and sh.sehstatus <> 'I'
					group by periodo, ano, dpeordem,dpeanoref
					order by dpeanoref, dpeordem";
            $dados = $db->carregar($sql);
            $dadosAgrupados = array();
            foreach($dados as $dado){
                $dadosAgrupados[$dado['ano']][$dado['periodo']] = $dado['valor'];
            }
//            ver($dadosAgrupados);

            $aPeriodo = array(
                'Dezembro/2013',
                'Janeiro/2014',
                'Fevereiro/2014',
                'Março/2014',
                'Abril/2014',
                'Maio/2014',
                'Junho/2014',
                'Julho/2014',
                'Agosto/2014',
                'Setembro/2014',
                'Outubro/2014',
                'Novembro/2014',
                'Dezembro/2014',
                'Janeiro/2015',
            );
            ?>

            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>Empresa</th>
                    <th>Dez/2013</th>
                    <th>Jan/2014</th>
                    <th>Fev/2014</th>
                    <th>Mar/2014</th>
                    <th>Abr/2014</th>
                    <th>Mai/2014</th>
                    <th>Jun/2014</th>
                    <th>Jul/2014</th>
                    <th>Ago/2014</th>
                    <th>Set/2014</th>
                    <th>Out/2014</th>
                    <th>Nov/2014</th>
                    <th>Dez/2014</th>
                    <th>2015</th>
                    <th>TOTAL</th>
                </tr>
                <?php
                $count = -1;
                $aTotal = array();
                $totalGeral = 0;
                foreach($dadosAgrupados as $empresa => $dados):
                    $count++;
                    ?>
                    <tr class="<?php echo ($count%2) ? 'link zebrado' : ''; ?>">
                        <td><?php echo $empresa; ?></td>
                        <?php
                        $total = 0;
                        foreach($aPeriodo as $periodo){
                            $valor = isset($dados[$periodo]) ? $dados[$periodo] : 0;
                            $aTotal[$periodo] += $valor;
                            $total += $valor;
                            ?>
                            <td class="numero" ><?php echo $valor ? number_format($valor,0,",",".") : ' - '?></td>
                        <?php } ?>
                        <td class="numero" ><?php echo $total ? number_format($total,0,",",".") : ' - '?></td>
                    </tr>
                <?php
                endforeach ?>
                <tr class="<?php echo ($count%2) ? 'link zebrado' : ''; ?>">
                    <th>Total início próximos meses</th>
                    <?php foreach($aPeriodo as $periodo){
                        $valor = isset($aTotal[$periodo]) ? $aTotal[$periodo] : 0;
                        ?>
                        <th class="numero" ><?php echo $valor ? number_format($valor,0,",",".") : ' - '?></th>
                    <?php } ?>
                    <th class="numero" ><?php echo number_format(array_sum($aTotal),0,",","."); ?></th>
                </tr>
            </table>
        </td>
	</tr>
	<tr>
		<!-- Tabela Processos -->
		<td class="fundo_padrao link" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/processos/proinfancia/obraim/Pr%C3%A9-Obra%20MI.htm')";>
			<div>
				<?php exibirTitulo('recycle', 'Fluxo pré-obra MI'); ?>
			</div>
		</td>
		<!-- Tabela Processos -->
    </tr>
	<tr>
		<!-- Tabela Processos -->
		<td class="fundo_padrao">
			<div>
				<?php exibirTitulo('recycle', 'Situação Obra MI'); ?>
			</div>
			<?php
			$sql = "SELECT
							CASE
								WHEN d.esdid IN (870,861,862,863,871,875) THEN 'Em Contratação'
								WHEN d.esdid IN (872,864,873,874) THEN 'Contratadas'
							ELSE 'Obras Iniciadas'
							END AS situacao,
							count(DISTINCT o.preid) AS total,
							pre.estuf
						FROM obras2.obras o
						INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
						INNER JOIN workflow.documento d ON d.docid = o.docid
						INNER JOIN
							(SELECT MAX(aosdtalteracao), aoscnpjfornecedor AS cnpj, preid FROM par.adesaoobraspacsituacao GROUP BY cnpj, preid) AS f ON f.preid = o.preid
						INNER JOIN obras.preobra pre ON pre.preid = f.preid
						WHERE o.obrstatus = 'A'
						AND e.orgid=3
						AND e.prfid=41
						AND o.obridpai is null
						AND d.esdid NOT IN (884, 768)
						AND o.tooid in (1,2,4)
						AND o.tpoid IN (104,105)
						AND f.cnpj IN ('', '', '', '')
						GROUP BY situacao, pre.estuf
						ORDER BY situacao, pre.estuf";
			$arrDados = $db->carregar( $sql, null, 3200 );
			if($arrDados){
				foreach($arrDados as $dado){
					$arrSituacaoMI[$dado['situacao']][$dado['estuf']]['total'][] = $dado['total'];
				}
			}
			?>

			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" >Fase / UF</td>
					<td class="center bold" >Total</td>
				</tr>
				<?php foreach($arrSituacaoMI as $chave => $sit):
					$somaAC = (is_array($sit['AC']['total'])?array_sum($sit['AC']['total']):0);
					$somaAL = (is_array($sit['AL']['total'])?array_sum($sit['AL']['total']):0);
					$somaAM = (is_array($sit['AM']['total'])?array_sum($sit['AM']['total']):0);
					$somaAP = (is_array($sit['AP']['total'])?array_sum($sit['AP']['total']):0);
					$somaBA = (is_array($sit['BA']['total'])?array_sum($sit['BA']['total']):0);
					$somaCE = (is_array($sit['CE']['total'])?array_sum($sit['CE']['total']):0);
					$somaDF = (is_array($sit['DF']['total'])?array_sum($sit['DF']['total']):0);
					$somaES = (is_array($sit['ES']['total'])?array_sum($sit['ES']['total']):0);
					$somaGO = (is_array($sit['GO']['total'])?array_sum($sit['GO']['total']):0);
					$somaMA = (is_array($sit['MA']['total'])?array_sum($sit['MA']['total']):0);
					$somaMG = (is_array($sit['MG']['total'])?array_sum($sit['MG']['total']):0);
					$somaMS = (is_array($sit['MS']['total'])?array_sum($sit['MS']['total']):0);
					$somaMT = (is_array($sit['MT']['total'])?array_sum($sit['MT']['total']):0);
					$somaPA = (is_array($sit['PA']['total'])?array_sum($sit['PA']['total']):0);
					$somaPB = (is_array($sit['PB']['total'])?array_sum($sit['PB']['total']):0);
					$somaPE = (is_array($sit['PE']['total'])?array_sum($sit['PE']['total']):0);
					$somaPI = (is_array($sit['PI']['total'])?array_sum($sit['PI']['total']):0);
					$somaPR = (is_array($sit['PR']['total'])?array_sum($sit['PR']['total']):0);
					$somaRJ = (is_array($sit['RJ']['total'])?array_sum($sit['RJ']['total']):0);
					$somaRN = (is_array($sit['RN']['total'])?array_sum($sit['RN']['total']):0);
					$somaRO = (is_array($sit['RO']['total'])?array_sum($sit['RO']['total']):0);
					$somaRR = (is_array($sit['RR']['total'])?array_sum($sit['RR']['total']):0);
					$somaRS = (is_array($sit['RS']['total'])?array_sum($sit['RS']['total']):0);
					$somaSC = (is_array($sit['SC']['total'])?array_sum($sit['SC']['total']):0);
					$somaSE = (is_array($sit['SE']['total'])?array_sum($sit['SE']['total']):0);
					$somaSP = (is_array($sit['SP']['total'])?array_sum($sit['SP']['total']):0);
					$somaTO = (is_array($sit['TO']['total'])?array_sum($sit['TO']['total']):0);
					$somaUF = $somaAC + $somaAL + $somaAM + $somaAP + $somaBA + $somaCE + $somaDF + $somaES + $somaGO + $somaMA + $somaMG + $somaMS + $somaMT + $somaPA + $somaPB + $somaPE + $somaPI + $somaPR + $somaRJ + $somaRN + $somaRO + $somaRR + $somaRS + $somaSC + $somaSE + $somaSP + $somaTO;
					?>
					<tr class="">
						<td class="" ><?=$chave ?></td>
						<td class="numero"><?=number_format($somaUF,0,",",".")?></td>
					</tr>
					<?php
					$totalAC += $somaAC;
					$totalAL += $somaAL;
					$totalAM += $somaAM;
					$totalAP += $somaAP;
					$totalBA += $somaBA;
					$totalCE += $somaCE;
					$totalDF += $somaDF;
					$totalES += $somaES;
					$totalGO += $somaGO;
					$totalMA += $somaMA;
					$totalMG += $somaMG;
					$totalMS += $somaMS;
					$totalMT += $somaMT;
					$totalPA += $somaPA;
					$totalPB += $somaPB;
					$totalPE += $somaPE;
					$totalPI += $somaPI;
					$totalPR += $somaPR;
					$totalRJ += $somaRJ;
					$totalRN += $somaRN;
					$totalRO += $somaRO;
					$totalRR += $somaRR;
					$totalRS += $somaRS;
					$totalSC += $somaSC;
					$totalSE += $somaSE;
					$totalSP += $somaSP;
					$totalTO += $somaTO;
					$totalUF = $totalAC + $totalAL + $totalAM + $totalAP + $totalBA + $totalCE + $totalDF + $totalES + $totalGO + $totalMA + $totalMG + $totalMS + $totalMT + $totalPA + $totalPB + $totalPE + $totalPI + $totalPR + $totalRJ + $totalRN + $totalRO + $totalRR + $totalRS + $totalSC + $totalSE + $totalSP + $totalTO;
				endforeach; ?>
				<tr class="">
					<td class="bold" >Total</td>
					<td class="numero" ><?=number_format($totalUF,0,",",".")?></td>
				</tr>
			</table>
		</td>
		<!-- Tabela Processos -->
		<td class="fundo_padrao">
			<div style="margin-top: 10px;">
                <?php exibirTitulo('indicador', 'Situação Real'); ?>
            </div>
			<?php
			$sql = "SELECT sigla, descricao, SUM(execucao) AS execucao, SUM(previsto) AS previsto
					FROM (
						SELECT
							CASE f.cnpj
								WHEN '' THEN 'CASAALTA CONSTRUCOES LTDA'
								WHEN '' THEN 'CONSÓRCIO JGA / VENÂNCIO'
								WHEN '' THEN 'MVC COMPONENTES PLASTICOS LTDA'
								WHEN '' THEN 'CONSÓRCIO PIB'
							END AS descricao,
							CASE f.cnpj
								WHEN '' THEN 'Casa Alta'
								WHEN '' THEN 'JGA'
								WHEN '' THEN 'MVC'
								WHEN '' THEN 'PIB'
							END AS sigla,
							CASE
								WHEN d.esdid NOT IN (870,861,862,863,871,875, 872,864,873,874) THEN 1
							ELSE 0
							END AS execucao,
							0 AS previsto
						FROM obras2.obras o
						INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
						INNER JOIN workflow.documento d ON d.docid = o.docid
						INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
						INNER JOIN
							(SELECT MAX(aosdtalteracao), aoscnpjfornecedor AS cnpj, preid FROM par.adesaoobraspacsituacao GROUP BY cnpj, preid) AS f ON f.preid = o.preid
						WHERE o.obrstatus = 'A'
						AND e.orgid=3
						AND e.prfid=41
						AND o.obridpai is null
						AND o.tooid in (1,2,4)
						AND o.tpoid IN (104,105)
						AND d.esdid NOT IN (884, 768)
						AND f.cnpj IN ('', '', '', '')
					UNION ALL
						SELECT
							CASE dsh.tidid1
								WHEN 7138 THEN 'CASAALTA CONSTRUCOES LTDA'
								WHEN 7139 THEN 'CONSÓRCIO JGA / VENÂNCIO'
								WHEN 7141 THEN 'MVC COMPONENTES PLASTICOS LTDA'
								WHEN 7140 THEN 'CONSÓRCIO PIB'
							END AS descricao,
							CASE dsh.tidid1
								WHEN 7138 THEN 'Casa Alta'
								WHEN 7139 THEN 'JGA'
								WHEN 7141 THEN 'MVC'
								WHEN 7140 THEN 'PIB'
							END AS sigla,
							0 AS execucao,
							sum(dsh.dshqtde)::INTEGER AS previsto
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						WHERE sh.indid in (2660)
						AND sh.sehstatus <> 'I'
						AND CURRENT_DATE >= dpe.dpedatainicio
						GROUP BY descricao, sigla
					) AS FOO
					GROUP BY sigla, descricao
					ORDER BY descricao";
			$arrDados = $db->carregar( $sql );
			?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>Empresa</th>
                    <th>Iniciadas</th>
                    <th>Previsto (até <?php echo date('m'). '/'.date('Y') ?>)</th>
                </tr>
				<?php
				$count = -1;
				foreach($arrDados as $dado):
					$count++;
					?>
					<tr class="<?php echo ($count%2) ? 'link zebrado' : ''; ?>">
						<td><?php echo $dado['descricao'] ?></td>
						<td class="numero" ><?php echo number_format($dado['execucao'],0,",",".") ?></td>
						<td class="numero" ><?php echo number_format($dado['previsto'],0,",",".") ?></td>
					</tr>
				<?php
				$somaExecucao += $dado['execucao'];
				$somaPrevisto += $dado['previsto'];
				endforeach ?>
				<tr>
					<th>Total</th>
					<th class="numero" ><?php echo number_format($somaExecucao,0,",",".") ?></th>
					<th class="numero" ><?php echo number_format($somaPrevisto,0,",",".") ?></th>
				</tr>
			</table>
            <div style="text-align: center; margin-top: 10px;">
                <!--<img src="../imagens/icones/icons/img/situacaoMI.png" width="700px"/>-->
			<?php
			$sql = "SELECT tipo, sigla, SUM(total) AS total
					FROM (
						SELECT
							CASE f.cnpj
								WHEN '' THEN 'Casa Alta'
								WHEN '' THEN 'JGA'
								WHEN '' THEN 'MVC'
								WHEN '' THEN 'PIB'
							END AS sigla,
							CASE
								WHEN d.esdid NOT IN (870,861,862,863,871,875, 872,864,873,874) THEN 1
							ELSE 0
							END AS total,
							'obras iniciadas' AS tipo
						FROM obras2.obras o
						INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
						INNER JOIN workflow.documento d ON d.docid = o.docid
						INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
						INNER JOIN
							(SELECT MAX(aosdtalteracao), aoscnpjfornecedor AS cnpj, preid FROM par.adesaoobraspacsituacao GROUP BY cnpj, preid) AS f ON f.preid = o.preid
						WHERE o.obrstatus = 'A'
						AND e.orgid=3
						AND e.prfid=41
						AND o.obridpai is null
						AND o.tooid in (1,2,4)
						AND o.tpoid IN (104,105)
						AND d.esdid NOT IN (884, 768)
						AND f.cnpj IN ('', '', '', '')
					UNION ALL
						SELECT
							CASE dsh.tidid1
								WHEN 7138 THEN 'Casa Alta'
								WHEN 7139 THEN 'JGA'
								WHEN 7141 THEN 'MVC'
								WHEN 7140 THEN 'PIB'
							END AS sigla,
							sum(dsh.dshqtde)::INTEGER AS total,
							'cronograma previsto (até '||TO_CHAR(CURRENT_DATE, 'MM/YYYY')||')' AS tipo
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						INNER JOIN painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						WHERE sh.indid in (2660)
						AND sh.sehstatus <> 'I'
						AND CURRENT_DATE >= dpe.dpedatainicio
						GROUP BY sigla
					) AS FOO
					GROUP BY tipo, sigla
					ORDER BY sigla";
			$dados = $db->carregar( $sql );

            $dadosAgrupados = agruparDadosGrafico($dados, 'sigla', 'tipo', 'total');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
			geraGraficoBarraAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoBarraMatriculas', null, null, $formatoValores, null, "", 1000, 600, null, 300);
			?>
            </div>
		</td>
    </tr>
	<tr>
		<!-- Tabela Processos -->
		<td class="fundo_padrao link" colspan="2">

            <?php
            $sql = "SELECT DISTINCT
                         o.obrid,
                         tos.tomdsc AS tomdsc,
                         tos.tomid,
                         ed.esdid,
                         ed.esddsc
                 FROM obras2.ordemservicomi os
                         JOIN obras2.obras o ON (os.obrid = o.obrid)
                         JOIN entidade.entidade e ON (e.entid = o.entid)
                         JOIN obras2.tipoosmi tos ON (os.tomid = tos.tomid)
                         JOIN seguranca.usuario u ON (os.usucpf = u.usucpf)
                         JOIN workflow.documento d ON (os.docid = d.docid)
                         JOIN workflow.estadodocumento ed ON (ed.esdid = d.esdid)
                         LEFT JOIN entidade.endereco edo ON (edo.endid = o.endid)
                         -- LEFT JOIN public.uf uf ON (uf.regcod = edo.estuf)
                         LEFT JOIN territorios.municipio m ON (m.muncod = edo.muncod)
                         LEFT JOIN obras2.supervisao_grupo_empresa sge ON (e.entid = sge.entid)
                         LEFT JOIN obras2.supervisaomi smi ON (smi.entidvistoriador = e.entid)
                         LEFT JOIN obras2.obrascontrato oc ON (oc.obrid = o.obrid)
                         LEFT JOIN(SELECT
                                 euf.emiid,
                                 euf.estuf,
                                 em.emidscresumo AS descricao
                                 --em.emidsc AS descricao
                             FROM obras2.empresami em
                             JOIN obras2.empresami_uf euf ON euf.emiid = em.emiid AND euf.eufstatus = 'A'
                             WHERE emistatus = 'A') foo ON (edo.estuf = foo.estuf)
                 WHERE ed.esdstatus = 'A' AND os.osmstatus = 'A'
                 ORDER BY esddsc
";
            $arrDados = $db->carregar( $sql, null, 3200 );

            $dados = array();
            foreach ($arrDados as $dado) {
                $dados[$dado['tomdsc']][$dado['esddsc']]++;
            }
            ?>

            <table width="100%">
                <tr>
                    <td width="33%">
                        <div>
                            <?php exibirTitulo('recycle', 'OS - Projeto de Implantação'); ?>
                        </div>

                        <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                            <tr>
                                <th>Situação</th>
                                <th>Total</th>
                            </tr>
                            <?php
                            $count = -1;
                            foreach($dados['Projeto de Implantação'] as $situacao => $quantidade):
                                $count++;
                                ?>
                                <tr onclick="abreRelatorioVistoria('6', '3', '0', '<?php echo $situacao; ?>');" class="<?php echo ($count%2) ? 'link zebrado' : ''; ?>">
                                    <td ><?php echo $situacao; ?></td>
                                    <td class="numero" ><?php echo $quantidade; ?></td>
                                </tr>
                            <?php
                            endforeach ?>
                            <tr>
                                <th>Total</th>
                                <th class="numero" ><?php echo number_format(array_sum($dados['Projeto de Implantação']),0,",",".") ?></th>
                            </tr>
                        </table>

                    </td>
                    <td>
                        <div>
                            <?php exibirTitulo('recycle', 'OS - Sondagem'); ?>
                        </div>

                        <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                            <tr>
                                <th>Situação</th>
                                <th>Total</th>
                            </tr>
                            <?php
                            $count = -1;
                            foreach($dados['Sondagem'] as $situacao => $quantidade):
                                $count++;
                                ?>
                                <tr onclick="abreRelatorioVistoria('6', '2', '0', '<?php echo $situacao; ?>');" class="<?php echo ($count%2) ? 'link zebrado' : ''; ?>">
                                    <td ><?php echo $situacao; ?></td>
                                    <td class="numero" ><?php echo $quantidade; ?></td>
                                </tr>
                            <?php
                            endforeach ?>
                            <tr>
                                <th>Total</th>
                                <th class="numero" ><?php echo number_format(array_sum($dados['Sondagem']),0,",",".") ?></th>
                            </tr>
                        </table>
                    </td>
                    <td>
                        <div>
                            <?php exibirTitulo('recycle', 'OS - Execução'); ?>
                        </div>

                        <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                            <tr>
                                <th>Situação</th>
                                <th>Total</th>
                            </tr>
                            <?php
                            $count = -1;
                            foreach($dados['Execução'] as $situacao => $quantidade):
                                $count++;
                                ?>
                                <tr onclick="abreRelatorioVistoria('6', '1', '0', '<?php echo $situacao; ?>');" class="<?php echo ($count%2) ? 'link zebrado' : ''; ?>">
                                    <td ><?php echo $situacao; ?></td>
                                    <td class="numero" ><?php echo $quantidade; ?></td>
                                </tr>
                            <?php
                            endforeach ?>
                            <tr>
                                <th>Total</th>
                                <th class="numero" ><?php echo number_format(array_sum($dados['Execução']),0,",",".") ?></th>
                            </tr>
                        </table>

                    </td>
                </tr>
            </table>
		</td>
		<!-- Tabela Processos -->
    </tr>
    <tr>
        <td class="fundo_padrao link" width="60%" colspan="2">
            <div>
                <?php exibirTitulo('indicador', 'Amostragem de Terrenos'); ?>
            </div>
            <?php
            $sql = "select a.*, m.mundescricao
                        from pde.cockpit_amostragem_terrenos a
                            inner join territorios.municipio m on m.muncod = a.muncod
                        ";
            $dados = $db->carregar($sql);
            ?>

            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>UF</th>
                    <th>Município</th>
                    <th>Classificação</th>
                </tr>
                <?php
                $count = -1;
                foreach($dados as $dado):
                    $aClassificacao = array('F'=>'Fácil', 'M'=>'Médio', 'D'=>'Difícil');
                    $count++;

                    $caminhoFoto = '/pde/img/cockpit/' . $dado['catcaminhofoto'];
                    $href = file_exists($caminhoFoto) ? $caminhoFoto : '';
                    $href = '/pde/img/cockpit/proinfancia/' . str_pad($dado['catcaminhofoto'], 2, 0, STR_PAD_RIGHT);
                    ?>
                    <tr class="<?php echo ($count%2) ? 'link zebrado' : ''; ?> <?php echo $href ? 'fancybox' : '' ?>" <?php echo $href ? 'href="' . $href . '"' : '' ?> data-fancybox-group="gallery" title="<?php echo $dado['estuf'] . ' - ' . $dado['mundescricao'] ?>">
                        <td><?php echo $dado['estuf']; ?></td>
                        <td><?php echo $dado['mundescricao']; ?></td>
                        <td><?php echo $aClassificacao[$dado['catclassificacao']]; ?></td>
                    </tr>
                <?php endforeach; ?>
            </table>
        </td>
		<?php
		//OCULTADO A PEDIDO DA SALA DE SITUAÇÃO DIA 17/02/2014
		?>
        <td class="fundo_padrao" style="display: none;">
            <div>
                <?php exibirTitulo('alarm', 'Agenda de Reuniões'); ?>
            </div>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th>Dia da Semana</th>
                    <th>Data</th>
                    <th>Empresa</th>
                    <th>Horário</th>
                    <th>Uf - Município</th>
                    <th>Obras MI</th>
                </tr>

                <tr>
                    <td style="vertical-align: middle" align="center" rowspan="5">Terça</td>
                    <td style="vertical-align: middle"  align="center" rowspan="5">21/01/2014</td>
                    <td style="vertical-align: middle"  rowspan="5">CASA ALTA</td>

                    <td align="center">09:30 - 10:30</td>
                    <td>GO - Nova Gama</td>
                    <td align="right">8</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">11:00 - 12:00</td>
                    <td>GO - Águas Lindas</td>
                    <td align="right">9</td>
                </tr>
                <tr>
                    <td align="center">14:00 - 15:00</td>
                    <td>GO - Aparecida de Goiânia</td>
                    <td align="right">17</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">15:30 - 16:00</td>
                    <td>GO - Luziânia</td>
                    <td align="right">12</td>
                </tr>
                <tr>
                    <td align="center">17:00 - 18:00</td>
                    <td>DF</td>
                    <td align="right">39</td>
                </tr>

                <tr class="zebrado">
                    <td style="vertical-align: middle" align="center" rowspan="5">Quarta</td>
                    <td style="vertical-align: middle"  align="center" rowspan="5">22/01/2014</td>
                    <td style="vertical-align: middle"  rowspan="5">CASA ALTA</td>

                    <td align="center">09:30 - 10:30</td>
                    <td>GO - Goiânia</td>
                    <td align="right">37</td>
                </tr>
                <tr>
                    <td align="center">11:00 - 12:00</td>
                    <td>MG - Betim</td>
                    <td align="right">13</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">14:00 - 15:00</td>
                    <td>MG - Montes Claros</td>
                    <td align="right">17</td>
                </tr>
                <tr>
                    <td align="center">15:30 - 16:00</td>
                    <td></td>
                    <td align="right"></td>
                </tr>
                <tr class="zebrado">
                    <td align="center">17:00 - 18:00</td>
                    <td></td>
                    <td align="right"></td>
                </tr>

                <tr>
                    <td style="vertical-align: middle" align="center" rowspan="5">Quinta</td>
                    <td style="vertical-align: middle"  align="center" rowspan="5">23/01/2014</td>
                    <td style="vertical-align: middle"  rowspan="5">CASA ALTA</td>

                    <td align="center">09:30 - 10:30</td>
                    <td>PR - Londrina</td>
                    <td align="right">15</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">11:00 - 12:00</td>
                    <td>PR - Ponta Grossa</td>
                    <td align="right">8</td>
                </tr>
                <tr>
                    <td align="center">14:00 - 15:00</td>
                    <td>RO - Porto Velho</td>
                    <td align="right">16</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">15:30 - 16:00</td>
                    <td>MS - Campo Grande</td>
                    <td align="right">12</td>
                </tr>
                <tr>
                    <td align="center">17:00 - 18:00</td>
                    <td>MT - Várzea Grande</td>
                    <td align="right">11</td>
                </tr>

                <tr class="zebrado">
                    <td style="vertical-align: middle" align="center" rowspan="5">Sexta</td>
                    <td style="vertical-align: middle"  align="center" rowspan="5">24/01/2014</td>
                    <td style="vertical-align: middle"  rowspan="5">CASA ALTA</td>

                    <td align="center">09:30 - 10:30</td>
                    <td>SP - Carapicuíba</td>
                    <td align="right">12</td>
                </tr>
                <tr>
                    <td align="center">11:00 - 12:00</td>
                    <td>SP - Limeira</td>
                    <td align="right">8</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">14:00 - 15:00</td>
                    <td>SP - Osasco</td>
                    <td align="right">9</td>
                </tr>
                <tr>
                    <td align="center">15:30 - 16:00</td>
                    <td>SP - Ribeirão das Neves</td>
                    <td align="right">11</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">17:00 - 18:00</td>
                    <td>MG - Uberlândia</td>
                    <td align="right">12</td>
                </tr>

                <tr>
                    <td style="vertical-align: middle" align="center" rowspan="5">Segunda</td>
                    <td style="vertical-align: middle"  align="center" rowspan="5">27/01/2014</td>
                    <td style="vertical-align: middle"  rowspan="5">CONSÓRCIO VENÂNIO - JGA</td>

                    <td align="center">09:30 - 10:30</td>
                    <td>CE - Fortaleza</td>
                    <td align="right">93</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">11:00 - 12:00</td>
                    <td>PB - Campina Grande</td>
                    <td align="right">8</td>
                </tr>
                <tr>
                    <td align="center">14:00 - 15:00</td>
                    <td>PB - João Pessoa</td>
                    <td align="right">32</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">15:30 - 16:00</td>
                    <td>RN - Natal</td>
                    <td align="right">16</td>
                </tr>
                <tr>
                    <td align="center">17:00 - 18:00</td>
                    <td>PI - Teresina-PI</td>
                    <td align="right">18</td>
                </tr>

                <tr class="zebrado">
                    <td style="vertical-align: middle" align="center" rowspan="5">Terça</td>
                    <td style="vertical-align: middle"  align="center" rowspan="5">28/01/2014</td>
                    <td style="vertical-align: middle"  rowspan="5">MVC</td>

                    <td align="center">09:30 - 10:30</td>
                    <td>PE - Caruaru</td>
                    <td align="right">13</td>
                </tr>
                <tr>
                    <td align="center">11:00 - 12:00</td>
                    <td>PE - Jaboatão dos Guararapes</td>
                    <td align="right">14</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">14:00 - 15:00</td>
                    <td>PE - Olinda</td>
                    <td align="right">12</td>
                </tr>
                <tr>
                    <td align="center">15:30 - 16:00</td>
                    <td>PE - Petrolina</td>
                    <td align="right">11</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">17:00 - 18:00</td>
                    <td>RJ - Belford Roxo</td>
                    <td align="right">10</td>
                </tr>

                <tr>
                    <td style="vertical-align: middle" align="center" rowspan="5">Quarta</td>
                    <td style="vertical-align: middle"  align="center" rowspan="5">29/01/2014</td>
                    <td style="vertical-align: middle"  rowspan="5">MVC</td>

                    <td align="center">09:30 - 10:30</td>
                    <td>RJ - Nova Iguaçu</td>
                    <td align="right">11</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">11:00 - 12:00</td>
                    <td>RJ - São Gonçalo</td>
                    <td align="right">12</td>
                </tr>
                <tr>
                    <td align="center">14:00 - 15:00</td>
                    <td>SE - Nossa Senhora do Socorro</td>
                    <td align="right">9</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">15:30 - 16:00</td>
                    <td>RS - Alvorada</td>
                    <td align="right">13</td>
                </tr>
                <tr>
                    <td align="center">17:00 - 18:00</td>
                    <td>RS - Gravataí</td>
                    <td align="right">12</td>
                </tr>

                <tr class="zebrado">
                    <td style="vertical-align: middle" align="center" rowspan="5">Quinta</td>
                    <td style="vertical-align: middle"  align="center" rowspan="5">30/01/2014</td>
                    <td style="vertical-align: middle"  rowspan="5">MVC</td>

                    <td align="center">09:30 - 10:30</td>
                    <td>RS - Pelotas</td>
                    <td align="right">14</td>
                </tr>
                <tr>
                    <td align="center">11:00 - 12:00</td>
                    <td>RS - Porto Alegre</td>
                    <td align="right">18</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">14:00 - 15:00</td>
                    <td>RS - Santa Maria</td>
                    <td align="right">10</td>
                </tr>
                <tr>
                    <td align="center">15:30 - 16:00</td>
                    <td>BA - Itabuna</td>
                    <td align="right">8</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">17:00 - 18:00</td>
                    <td>BA - Lauro de Freitas</td>
                    <td align="right">7</td>
                </tr>

                <tr>
                    <td style="vertical-align: middle" align="center" rowspan="5">Sexta</td>
                    <td style="vertical-align: middle"  align="center" rowspan="5">31/01/2014</td>
                    <td style="vertical-align: middle"  rowspan="5">MVC</td>

                    <td align="center">09:30 - 10:30</td>
                    <td>BA - Salvador</td>
                    <td align="right">23</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">11:00 - 12:00</td>
                    <td>BA - Teixeira de Freitas</td>
                    <td align="right">8</td>
                </tr>
                <tr>
                    <td align="center">14:00 - 15:00</td>
                    <td>MA - São Luis</td>
                    <td align="right">14</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">15:30 - 16:00</td>
                    <td></td>
                    <td align="right"></td>
                </tr>
                <tr>
                    <td align="center">17:00 - 18:00</td>
                    <td></td>
                    <td align="right"></td>
                </tr>

                <tr class="zebrado">
                    <td style="vertical-align: middle" align="center" rowspan="5">Segunda</td>
                    <td style="vertical-align: middle"  align="center" rowspan="5">03/02/2014</td>
                    <td style="vertical-align: middle"  rowspan="5">CONSÓRCIO PIB</td>

                    <td align="center">09:30 - 10:30</td>
                    <td>AM - Manaus</td>
                    <td align="right">26</td>
                </tr>
                <tr>
                    <td align="center">11:00 - 12:00</td>
                    <td>PA - Castanhal</td>
                    <td align="right">9</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">14:00 - 15:00</td>
                    <td>PA - Marabá</td>
                    <td align="right">16</td>
                </tr>
                <tr>
                    <td align="center">15:30 - 16:00</td>
                    <td>PA - Santarém</td>
                    <td align="right">12</td>
                </tr>
                <tr class="zebrado">
                    <td align="center">17:00 - 18:00</td>
                    <td>RR - Boa Vista</td>
                    <td align="right">16</td>
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