<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
permissaoPerfilConsultaCockpit('estrategico.php?modulo=principal/cockpit_pronatec&acao=A');

$codper = $_REQUEST['codper'] ? $_REQUEST['codper'] : 2;
if($codper){
    if($codper==1){
        $periodoPronatec = "";
        $tituloperiodo = "TOTAL";
    }elseif($codper==2){
        $periodoPronatec = "AND dpe.dpeanoref BETWEEN '2011' AND '2014'";
        $tituloperiodo = " (2011 - 2014)";
    }elseif($codper==3){
        $periodoPronatec = "AND dpe.dpeanoref BETWEEN '2015' AND '2018'";
        $tituloperiodo = " (2015 - 2018)";
    }
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
		jQuery.noConflict();

		jQuery(function(){

			jQuery(function(){
				jQuery('.toggle_consolidado1').click(function(){
					jQuery('#graficos_consolidados1').toggle('fade');
				});

				setTimeout(function() {jQuery('#graficos_consolidados1').hide()} , 500);

			});

			jQuery(function(){
				jQuery('.toggle_consolidado2').click(function(){
					jQuery('#graficos_consolidados2').toggle('fade');
				});

				setTimeout(function() {jQuery('#graficos_consolidados2').hide()} , 500);

			});
			
			jQuery(function(){
				jQuery('.toggle_consolidado3').click(function(){
					jQuery('#graficos_consolidados3').toggle('fade');
				});

				setTimeout(function() {jQuery('#graficos_consolidados3').hide()} , 500);

			});
			
			jQuery(function(){
				jQuery('.toggle_consolidado4').click(function(){
					jQuery('#graficos_consolidados4').toggle('fade');
				});

				setTimeout(function() {jQuery('#graficos_consolidados4').hide()} , 500);

			});
			
			jQuery(function(){
				jQuery('.toggle_consolidado5').click(function(){
					jQuery('#graficos_consolidados5').toggle('fade');
				});

				setTimeout(function() {jQuery('#graficos_consolidados5').hide()} , 500);

			});
		});

		atualizaUsuario();

	    function abrePopUpObrasSituacao(tipo, situacao){
	        window.open('/pde/estrategico.php?modulo=principal/popuplistaobrassituacao&acao=A&tipo=' + tipo + '&situacao='+situacao);
	    }

	    function abreDetalhePronatec(codper){
	        window.open('/pde/estrategico.php?modulo=principal/popupDetalhesPronatec&acao=A&geral=1&codper='+codper,'winDetalhe');
	    }

	    function abreDetalheCursoPronatec(tipo){
	        window.open('/pde/estrategico.php?modulo=principal/popupDetalhesCursoPronatec&acao=A&tipo='+tipo,'winDetalheCurso');
	    }

		function abreMetaPronatec(){
	        window.open('/pde/estrategico.php?modulo=principal/popupMetasPronatec&acao=A','winDetalheMeta');
	    }

        function getAnoPronatec(codper){
            window.location='estrategico.php?modulo=principal/cockpit_pronatec&acao=A&codper='+codper;
        }
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
		.fundo_titulo{background-image:url('../imagens/fundobrasilpro.jpg')};
		
		p.toggle_consolidado1{
			text-align: center;
		}
		
		p.toggle_consolidado2{
			text-align: center;
		}
		
		p.toggle_consolidado3{
			text-align: center;
		}
		
		p.toggle_consolidado4{
			text-align: center;
		}
		
		p.toggle_consolidado5{
			text-align: center;
		}
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_pronatec&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
        <?php
        $sql= "SELECT 1 AS codigo, 'Total' AS descricao
                UNION ALL
                SELECT 2 AS codigo, '2011 - 2014' AS descricao
                UNION ALL
                SELECT 3 AS codigo, '2015 - 2018' AS descricao
		";
        ?>
		<td class="fundo_titulo" style="text-align:center" colspan="8" >
            <div style="margin:28px" >Pronatec<br>
                (<?=$db->monta_combo('codper',$sql,'S','','getAnoPronatec(this.value);','','','','N',"codper","",$codper);?>)
            </div>
        </td>
	</tr>
	<tr>
		<td class="fundo_padrao link"  align="middle" onclick="window.location='estrategico.php?modulo=principal/cockpit_acordo&acao=A';" rowspan="2">
			<div>
				<?php exibirTitulo('Presentation', 'Acordo de Gratuidade'); ?>
			</div>
		</td>
		<td class="fundo_padrao link"  align="middle" onclick="window.location='estrategico.php?modulo=principal/cockpit_bolsa_formacao&acao=A';" rowspan="2">
			<div>
				<?php exibirTitulo('Presentation', 'Bolsa-Formação'); ?>
			</div>
		</td>
		<td class="fundo_padrao link"  align="middle" onclick="window.location='estrategico.php?modulo=principal/cockpit_brasil_pro&acao=A';" rowspan="2">
			<div>
				<?php exibirTitulo('Presentation', 'Brasil Profissio<br>nalizado'); ?>
			</div>
		</td>
		<td class="fundo_padrao link"  align="middle" onclick="window.location='estrategico.php?modulo=principal/cockpit_etec&acao=A';" rowspan="2">
			<div>
				<?php exibirTitulo('Presentation', 'E-TEC'); ?>
			</div>
		</td>
		<td class="fundo_padrao link"  align="middle" onclick="window.location='estrategico.php?modulo=principal/cockpit_rede_federal2&acao=A';" rowspan="2">
			<div>
				<?php exibirTitulo('Presentation', 'Rede Federal EPT'); ?>
			</div>
		</td>
		<td class="fundo_padrao" colspan="2" width="34%">
			<?php
			$sql = "SELECT tid2.tiddsc AS tipo, SUM(dsh.dshqtde::integer) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2577)
                    AND sh.sehstatus <> 'I'
                    $periodoPronatec
                    GROUP BY tipo
                    ORDER BY tipo";
			$arrDados = $db->carregar( $sql );
			foreach($arrDados as $dado){
				if($dado['tipo']=='FIC'){
					$totalFIC = $dado['valor'];
				}else{
					$totalTecnico = $dado['valor'];
				}
				$totalExecucao += $dado['valor'];
			}
			$percentualFIC = ($totalFIC / $totalExecucao) * 100;
			$percentualTecnico = ($totalTecnico / $totalExecucao) * 100;
			?>
			<div>
				<div style="float: left; width: 50%;"> <?php exibirTitulo('executive', '<span class="link" onclick="abreDetalhePronatec('.$codper.');">Execução<br />'.number_format($totalExecucao,0,",",".") . '<br>matrículas</span>'); ?></div>
				<div style="float: left; width: 50%;"> <?php exibirTitulo('alvo', '<span class="link" onclick="abreMetaPronatec();">Meta 2011-2014<br />8.000.000<br>vagas</span>'); ?></div>
				<div style="float: left; width: 50%;"> <?php exibirTitulo('configs', '<span class="link" onclick="abreDetalheCursoPronatec(1);">FIC<br />'.number_format($percentualFIC,0,",",".").'%' . '<br />' . number_format($totalFIC,0,",","."). '</span>'); ?></div>
				<div style="float: left; width: 50%;"> <?php exibirTitulo('configs', '<span class="link" onclick="abreDetalheCursoPronatec(2);">TEC<br />'.number_format($percentualTecnico,0,",",".").'%' . '<br />' . number_format($totalTecnico,0,",","."). '</span>'); ?></div>
			</div>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2577);" width="33%" rowspan="2">
			<div>
				<?php exibirTitulo('indicador', 'Matrículas'); ?>
			</div>
			<?php
			// Matrículas (Bolsa-Formação)
			$sql = "SELECT tid1.tiddsc AS acao, tid2.tiddsc AS tipo, SUM(dsh.dshqtde::integer) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2577)
                    AND sh.sehstatus <> 'I'
                    $periodoPronatec
                    GROUP BY acao, tipo
                    ORDER BY acao, tipo";
			$dados = $db->carregar( $sql );

            $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );
			geraGraficoBarraAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoBarraMatriculas', null, null, $formatoValores, null, "", 1000, 600, null, 300);
			?>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao" align="center">
			<p class="toggle_consolidado3 titulo_box link">Evolução de Matrículas por Iniciativa</p>
		</td>
		<td class="fundo_padrao" align="center">
			<p class="toggle_consolidado4 titulo_box link">Evolução de Matrículas por Categoria de Situação</p>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr id="graficos_consolidados3">
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2577);">
			<div>
				<?php exibirTitulo('executive', 'Evolução de Matrículas por Iniciativa'); ?>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
                	<td>
						<?
						$arrSituacao = array();
						$sql = "SELECT tid1.tiddsc AS acao, tid1.tidid, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde::integer) AS valor
                                FROM painel.seriehistorica sh
                                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                                INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                                WHERE sh.indid IN (2577)
                                AND sh.sehstatus <> 'I'
                                $periodoPronatec
                                GROUP BY periodo, acao, tid1.tidid
                                ORDER BY periodo, acao";
						$arrDados = $db->carregar( $sql, null, 3200 );
						if($arrDados){
							foreach($arrDados as $dado){
								$arrSituacao[$dado['periodo']][$dado['tidid']]['total'][] = $dado['valor'];
							}
						}
						$arrX = array();
						$arrA = array();
						$arrB = array();
						$arrC = array();
						$arrD = array();
						$arrE = array();
						$arrTotal = array();
						
						foreach($arrSituacao as $periodo => $sit)
						{
							$arrX[] = $periodo;
							$vA = Round((is_array($sit['7529']['total'])?array_sum($sit['7529']['total']):0));
							$vB = Round((is_array($sit['7528']['total'])?array_sum($sit['7528']['total']):0));
							$vC = Round((is_array($sit['8118']['total'])?array_sum($sit['8118']['total']):0));
							$vD = Round((is_array($sit['7530']['total'])?array_sum($sit['7530']['total']):0));
							$vE = Round((is_array($sit['7870']['total'])?array_sum($sit['7870']['total']):0));
							
							//if($vA > 0){
								$arrA[] = $vA;
								$vTotal += $vA;
							//}
							//if($vB > 0){
								$arrB[] = $vB;
								$vTotal += $vB;
							//}
							//if($vC > 0){
								$arrC[] = $vC;
								$vTotal += $vC;
							//}
							//if($vD > 0){
								$arrD[] = $vD;
								$vTotal += $vD;
							//}
							//if($vE > 0){
								$arrE[] = $vE;
								$vTotal += $vE;
							//}
							$arrTotal[] = $vTotal;
						}
						
						$aDados = array(
							array('name' => 'Acordo de Gratuidade', 'data'=>$arrA),
                            array('name' => 'Bolsa-Formacao', 'data'=>$arrB),
                            array('name' => 'Brasil Profissionalizado', 'data'=>$arrC),
                            array('name' => 'Rede Federal EPT', 'data'=>$arrD),
                            array('name' => 'E-TEC', 'data'=>$arrE),
                            array('name' => 'Total de Matriculas', 'data'=>$arrTotal),
                        );

                        $formatoValores = array(
                            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                        );
						
                        echo geraGraficoLinha($aDados, $arrX, "graficoLinhadoTempoMatriculasAcoes", "Iniciativas",300, $formatoValores);
						?>
                	</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr id="graficos_consolidados4">
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(3201);">
			<div>
				<?php exibirTitulo('executive', 'Evolução de Matrículas por Categoria de Situação'); ?>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
                	<td>
						<?
						$arrSituacao = array();
						$sql = "SELECT ".montaColunaCategoriaPronatec()." AS situacao, dpe.dpeanoref AS periodo, SUM(dsh.dshqtde::integer) AS valor
                                FROM painel.seriehistorica sh
                                INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                                INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                                WHERE sh.indid IN (3201)
                                AND sh.sehstatus <> 'I'
                                $periodoPronatec
                                GROUP BY situacao, periodo
                                ORDER BY periodo, situacao";
						$arrDados = $db->carregar( $sql );
						if($arrDados){
							foreach($arrDados as $dado){
								$arrSituacao[$dado['periodo']][$dado['situacao']]['total'][] = $dado['valor'];
							}

                            $arrX = array();
                            $arrA = array();
                            $arrB = array();
                            $arrC = array();
                            $arrD = array();
                            $arrE = array();
                            $arrTotal = array();
                            $vTotal = 0;
                            foreach($arrSituacao as $periodo => $sit)
                            {
                                $arrX[] = $periodo;
                                $vA = Round((is_array($sit['Abandono']['total'])?array_sum($sit['Abandono']['total']):0));
                                $vB = Round((is_array($sit['Conclusao']['total'])?array_sum($sit['Conclusao']['total']):0));
                                $vC = Round((is_array($sit['Em curso']['total'])?array_sum($sit['Em curso']['total']):0));
                                $vD = Round((is_array($sit['Interrupcao']['total'])?array_sum($sit['Interrupcao']['total']):0));
                                $vE = Round((is_array($sit['Outras']['total'])?array_sum($sit['Outras']['total']):0));

                                //if($vA > 0){
                                $arrA[] = $vA;
                                $vTotal += $vA;
                                //}
                                //if($vB > 0){
                                $arrB[] = $vB;
                                $vTotal += $vB;
                                //}
                                //if($vC > 0){
                                $arrC[] = $vC;
                                $vTotal += $vC;
                                //}
                                //if($vD > 0){
                                $arrD[] = $vD;
                                $vTotal += $vD;
                                //}
                                //if($vE > 0){
                                $arrE[] = $vE;
                                $vTotal += $vE;
                                //}
                                $arrTotal[] = $vTotal;
                            }

                            $aDados = array(
                                array('name' => 'Abandono', 'data'=>$arrA),
                                array('name' => 'Conclusao', 'data'=>$arrB),
                                array('name' => 'Em curso', 'data'=>$arrC),
                                array('name' => 'Interrupcao', 'data'=>$arrD),
                                //array('name' => 'Outras', 'data'=>$arrE),
                                array('name' => 'Total de Matriculas', 'data'=>$arrTotal),
                            );

                            $formatoValores = array(
                                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
                            );

                            echo geraGraficoLinha($aDados, $arrX, "graficoLinhadoTempoMatriculasSituacoes", "Categoria de Situações",300,$formatoValores);
						}
						?>
                	</td>
				</tr>
			</table>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(3201);" width="33%">
			<div>
				<?php exibirTitulo('indicador', 'Total'); ?>
			</div>
			<?php
			$sql = "SELECT ".montaColunaCategoriaPronatec()." AS descricao, SUM(dsh.dshqtde::integer) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (3201)
                    AND sh.sehstatus <> 'I'
                    $periodoPronatec
                    GROUP BY descricao
                    ORDER BY descricao";
			$arrDados = $db->carregar( $sql, null, 3200 );
			if($arrDados){
			    geraGrafico($arrDados, "graficoPizzaTotal", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, null, null, "#fff", false);
            }
			?>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(3201);" width="34%">
			<div>
				<?php exibirTitulo('indicador', 'FIC'); ?>
			</div>
			<?php
            $sql = "SELECT ".montaColunaCategoriaPronatec()." AS descricao, SUM(dsh.dshqtde::integer) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (3201)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid1 IN (9315) --FIC
                    $periodoPronatec
                    GROUP BY descricao
                    ORDER BY descricao";
			$arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
			    geraGrafico($arrDados, "graficoPizzaFic", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, null, null, "#fff", false);
            }
			?>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(3201);" width="33%" colspan="2">
			<div>
				<?php exibirTitulo('indicador', 'Técnico'); ?>
			</div>
			<?php
            $sql = "SELECT ".montaColunaCategoriaPronatec()." AS descricao, SUM(dsh.dshqtde::integer) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (3201)
                    AND sh.sehstatus <> 'I'
                    AND dsh.tidid1 IN (9316) --Técnico
                    $periodoPronatec
                    GROUP BY descricao
                    ORDER BY descricao";
			$arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
			    geraGrafico($arrDados, "graficoPizzaTecnico", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true, null, null, "#fff", false);
            }
			?>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao" colspan="2" rowspan="2">
			<div>
				<?php exibirTitulo('obras', 'Rede Federal de EPT'); ?>
			</div>
			<?php
			$sql = "select obras.*, coalesce(dias.dias_sem_atualizacao, 0) as dias_sem_atualizacao
                    from  (
                        SELECT
                            s.stoid, s.stodesc as situacao,
                            CASE
                                WHEN o.tobraid IN (3,4,5) THEN 'Ampliação / Reforma'
                            ELSE 'Construção'
                            END AS tipo,
                            CASE
								WHEN s.stodesc = 'Concluída'  THEN 1
								WHEN s.stodesc = 'Em Execução'  THEN 2
								WHEN s.stodesc = 'Em Licitação'  THEN 3
                                WHEN s.stodesc = 'Em Elaboração de Projetos'  THEN 4
                                WHEN s.stodesc = 'Em Planejamento pelo Proponente'  THEN 5
                                WHEN s.stodesc = 'Paralisada'  THEN 6
                                WHEN s.stodesc = 'Contrato Cancelado'  THEN 7
                                WHEN s.stodesc = 'Obra Cancelada'  THEN 8
                                ELSE 99
                            END AS ordem --, obrdtvistoria, obrpercexec, *
                            , count(0) as total
                        FROM obras.obrainfraestrutura o
                            INNER JOIN obras.situacaoobra s ON s.stoid = o.stoid
                        WHERE o.obsstatus = 'A'
                        AND o.orgid = 2
                        GROUP BY s.stoid, situacao, tipo, ordem
                    ) as obras
                        left join (
                            SELECT
                                s.stoid, s.stodesc as situacao,
                                CASE
                                    WHEN o.tobraid IN (3,4,5) THEN 'Ampliação / Reforma'
                                ELSE 'Construção'
                                END AS tipo,
                                max(DATE_PART('days', NOW() - obrdtvistoria)) as dias_sem_atualizacao
                                -- ,obrdtvistoria, obrpercexec, *
                                -- , count(0) as total
                            FROM obras.obrainfraestrutura o
                                INNER JOIN obras.situacaoobra s ON s.stoid = o.stoid
                            WHERE o.obsstatus = 'A'
                            AND o.orgid = 2
                            and obrpercexec < 100.00
                            GROUP BY s.stoid, situacao, tipo
                        ) as dias on dias.stoid = obras.stoid and dias.tipo = obras.tipo
                    ORDER BY ordem, situacao";

			$arrDados = $db->carregar( $sql, null, 3200 );
			$somaApliacao = 0;
			$diasApliacao = 0;
			$somaConstrucao = 0;
			$diasConstrucao = 0;
			$somaTotal = 0;
			$totalApliacao = 0;
			$totalConstrucao = 0;
			$totalGeral = 0;
			$arrSituacao = array();
			if($arrDados){
				foreach($arrDados as $dado){
					$arrSituacao[$dado['situacao']][$dado['tipo']]['total'][] = $dado['total'];
					$arrSituacao[$dado['situacao']][$dado['tipo']]['dias'][] = $dado['dias_sem_atualizacao'];
					$arrSituacao[$dado['situacao']]['stoid'] = $dado['stoid'];
				}
			}
			?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<th class="center bold" >Situação</th>
					<th class="center bold" >Reforma / Ampliações</th>
					<th class="center bold" >Construções</th>
					<th class="center bold" >Total</th>
				</tr>
				<?php
				$count = -1;
				foreach($arrSituacao as $chave => $sit):
					$somaApliacao = (is_array($sit['Ampliação / Reforma']['total'])?array_sum($sit['Ampliação / Reforma']['total']):0);
					$diasApliacao = (is_array($sit['Ampliação / Reforma']['total'])?array_sum($sit['Ampliação / Reforma']['dias']):0);

					$somaConstrucao = (is_array($sit['Construção']['total'])?array_sum($sit['Construção']['total']):0);
					$diasConstrucao = (is_array($sit['Construção']['total'])?array_sum($sit['Construção']['dias']):0);

					$somaTotal = $somaApliacao + $somaConstrucao;
					$count++;
					
					if($sit['stoid']==3){
						$concluida=true;
					}else{
						$concluida=false;
					}
					?>
					<tr class="<?php echo ($count%2) ? '"link zebrado"' : '"link"'; ?>" onclick="abrePopUpObrasSituacao(2, '<?php echo $sit['stoid']; ?>');">
						<td class="" ><?=$chave ?></td>
						<td class="numero" ><?= geraLinha($somaApliacao, $diasApliacao, $concluida); ?></td>
						<td class="numero" ><?= geraLinha($somaConstrucao, $diasConstrucao, $concluida); ?></td>
						<td class="numero" ><?=number_format($somaTotal,0,",",".")?></td>
					</tr>
					<?php
					$totalApliacao += $somaApliacao;
					$totalConstrucao += $somaConstrucao;
					$totalGeral = $totalApliacao + $totalConstrucao;
				endforeach; ?>
				<tr class="">
					<th class="bold" >Total</th>
					<th class="" ><?=number_format($totalApliacao,0,",",".")?></th>
					<th class="" ><?=number_format($totalConstrucao,0,",",".")?></th>
					<th class="numero" ><?=number_format($totalGeral,0,",",".")?></th>
				</tr>
			</table>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(771);">
			<div>
				<?php exibirTitulo('obras', 'Obras Brasil<br>Profissionalizado'); ?>
			</div>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(525);">
			<div>
				<?php exibirTitulo('casas', 'Expansão<br>Fase III'); ?>
			</div>
			<?php
			$sql = "SELECT count(distinct entid) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (525)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND dsh.tidid1 IN (2596, 2597)
					AND dsh.tidid2 IN (1738)
					AND sh.sehstatus <> 'I'";
			$totalCampusFuncionando = $db->pegaUm( $sql, null, 3200 );
			$valorMeta = 208;
			$porcentagem = number_format(($totalCampusFuncionando/$valorMeta)*100,0,",",".");
			?>
			<table class="tabela_painel" cellpadding="2" cellspacing="1" width="100%" border="0" >
				<tr>
					<td class="titulo_box bold" align="center">
						<?php exibirTitulo('alvo', '208 câmpus<br>2011-14'); ?>
						<div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 100px;'>
							<div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $porcentagem ?>px;'><?= $porcentagem ?>%&nbsp;</div>
						</div><?php echo number_format($totalCampusFuncionando,0,",",".") ?> funcionando
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao" align="center">
			<p class="toggle_consolidado2 titulo_box link">Evolução das Obras</p>
		</td>
		<td class="fundo_padrao" align="center">
			<p class="toggle_consolidado1 titulo_box link">Evolução das Obras</p>
		</td>
	</tr>
</table>

<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr id="graficos_consolidados2">
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(771);">
			<div>
				<?php exibirTitulo('obras', 'Evolução das Obras - Brasil Profissionalizado'); ?>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
                	<td>
						<?
						$arrSituacao = array();
						$sql = "select dpe.dpedsc AS periodo, dpe.dpeanoref, dpe.dpeordem, sum(dsh.dshqtde) as total, tid.tiddsc as situacao
								from painel.seriehistorica sh
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
								INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid2
								where sh.indid in (771)
								and sh.sehstatus <> 'I'
								AND dpe.dpedatainicio >= '2013-01-01'
								AND dsh.tidid2 NOT IN (2968, 4779, 4783, 4782, 2966, 2967)
								group by dpe.dpedsc, dpe.dpeanoref, dpe.dpeordem, situacao
								order by dpe.dpeanoref, dpe.dpeordem, situacao";
						$arrDados = $db->carregar( $sql, null, 3200 );
						if($arrDados){
							foreach($arrDados as $dado){
								$arrSituacao[$dado['periodo']][$dado['situacao']]['total'][] = $dado['total'];
							}
						}
						$arrX = array();
						$arrA = array();
						$arrB = array();
						$arrC = array();
						$arrD = array();
						$arrE = array();
						$arrF = array();
						$arrG = array();
						$arrH = array();
						$arrI = array();
						$arrJ = array();
						$arrK = array();
						$arrL = array();
						$arrM = array();
						
						foreach($arrSituacao as $periodo => $sit)
						{
							$arrX[] = $periodo;
							$arrA[] = Round((is_array($sit['Em Execução']['total'])?array_sum($sit['Em Execução']['total']):0));
							$arrB[] = Round((is_array($sit['Paralisada']['total'])?array_sum($sit['Paralisada']['total']):0));
							$arrC[] = Round((is_array($sit['Concluída']['total'])?array_sum($sit['Concluída']['total']):0));
							//$arrD[] = Round((is_array($sit['Em Elaboração de Projetos']['total'])?array_sum($sit['Em Elaboração de Projetos']['total']):0));
							$arrE[] = Round((is_array($sit['Em Licitação']['total'])?array_sum($sit['Em Licitação']['total']):0));
							//$arrF[] = Round((is_array($sit['Em Reformulação']['total'])?array_sum($sit['Em Reformulação']['total']):0));
							$arrG[] = Round((is_array($sit['Obra Cancelada']['total'])?array_sum($sit['Obra Cancelada']['total']):0));
							//$arrH[] = Round((is_array($sit['Aguardando 1º repasse']['total'])?array_sum($sit['Aguardando 1º repasse']['total']):0));
							//$arrI[] = Round((is_array($sit['Celebração de aditivo']['total'])?array_sum($sit['Celebração de aditivo']['total']):0));
							$arrJ[] = Round((is_array($sit['Em Planejamento pelo Proponente']['total'])?array_sum($sit['Em Planejamento pelo Proponente']['total']):0));
							//$arrK[] = Round((is_array($sit['Contrato Cancelado']['total'])?array_sum($sit['Contrato Cancelado']['total']):0));
							//$arrL[] = Round((is_array($sit['Convênio Cancelado']['total'])?array_sum($sit['Convênio Cancelado']['total']):0));
							$arrM[] = Round((is_array($sit['Contratação']['total'])?array_sum($sit['Contratação']['total']):0));
						}
						
						$aDados = array(
							array('name' => 'Em Execucao', 'data'=>$arrA),
                            array('name' => 'Paralisada', 'data'=>$arrB),
                            array('name' => 'Concluida', 'data'=>$arrC),
                            //array('name' => 'Em Elaboracao de Projetos', 'data'=>$arrD),
                            array('name' => 'Em Licitacao', 'data'=>$arrE),
                            //array('name' => 'Em Reformulacao', 'data'=>$arrF),
                            array('name' => 'Obra Cancelada', 'data'=>$arrG),
                            //array('name' => 'Aguardando 1o repasse', 'data'=>$arrH),
                            //array('name' => 'Celebracao de aditivo', 'data'=>$arrI),
                            array('name' => 'Em Planejamento pelo Proponente', 'data'=>$arrJ),
                            //array('name' => 'Contrato Cancelado', 'data'=>$arrK),
                            //array('name' => 'Convenio Cancelado', 'data'=>$arrL),
                            array('name' => 'Contratacao', 'data'=>$arrM),
                        );

                        $formatoValores = array(
                            'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                            'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                            'alinhamento' => "formatter: function() { return '1'; }"
                        );
						
                        echo geraGraficoLinha($aDados, $arrX, "graficoLinhadoTempoBrasilPro", "Etapas",300, $formatoValores);
						?>
                	</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr id="graficos_consolidados1">
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2294);">
			<div>
				<?php exibirTitulo('obras', 'Evolução das Obras - Expansão Fase III'); ?>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
                	<td>
						<?
						$arrSituacao = array();
						$sql = "select dpe.dpedsc AS periodo, dpe.dpeanoref, dpe.dpeordem, sum(dsh.dshqtde) as total, tid.tiddsc as situacao
								from painel.seriehistorica sh
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
								INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid1
								where sh.indid in (2294)
								and sh.sehstatus <> 'I'
								group by dpe.dpedsc, dpe.dpeanoref, dpe.dpeordem, situacao
								order by dpe.dpeanoref, dpe.dpeordem, situacao";
						$arrDados = $db->carregar( $sql, null, 3200 );
						if($arrDados){
							foreach($arrDados as $dado){
								$arrSituacao[$dado['periodo']][$dado['situacao']]['total'][] = $dado['total'];
							}

                            $arrX = array();
                            $arrA = array();
                            $arrB = array();
                            $arrC = array();
                            $arrD = array();
                            $arrE = array();
                            $arrF = array();
                            $arrG = array();
                            $arrH = array();
                            $arrI = array();
                            $arrJ = array();

                            foreach($arrSituacao as $periodo => $sit)
                            {
                                $arrX[] = $periodo;
                                $arrA[] = Round((is_array($sit['Em Execução']['total'])?array_sum($sit['Em Execução']['total']):0));
                                $arrB[] = Round((is_array($sit['Paralisada']['total'])?array_sum($sit['Paralisada']['total']):0));
                                $arrC[] = Round((is_array($sit['Concluída']['total'])?array_sum($sit['Concluída']['total']):0));
                                $arrD[] = Round((is_array($sit['Em Elaboração de Projetos']['total'])?array_sum($sit['Em Elaboração de Projetos']['total']):0));
                                $arrE[] = Round((is_array($sit['Em Licitação']['total'])?array_sum($sit['Em Licitação']['total']):0));
                                $arrF[] = Round((is_array($sit['Em Reformulação']['total'])?array_sum($sit['Em Reformulação']['total']):0));
                                $arrG[] = Round((is_array($sit['Obra Cancelada']['total'])?array_sum($sit['Obra Cancelada']['total']):0));
                                $arrH[] = Round((is_array($sit['Etapa Concluída']['total'])?array_sum($sit['Etapa Concluída']['total']):0));
                                $arrI[] = Round((is_array($sit['Aguardando registro de preços']['total'])?array_sum($sit['Aguardando registro de preços']['total']):0));
                                $arrJ[] = Round((is_array($sit['Em Planejamento pelo Proponente']['total'])?array_sum($sit['Em Planejamento pelo Proponente']['total']):0));
                            }

                            $aDados = array(
                                array('name' => 'Em Execucao', 'data'=>$arrA),
                                array('name' => 'Paralisada', 'data'=>$arrB),
                                array('name' => 'Concluida', 'data'=>$arrC),
                                //array('name' => 'Em Elaboracao de Projetos', 'data'=>$arrD),
                                array('name' => 'Em Licitacao', 'data'=>$arrE),
                                //array('name' => 'Em Reformulacao', 'data'=>$arrF),
                                //array('name' => 'Obra Cancelada', 'data'=>$arrG),
                                //array('name' => 'Etapa Concluida', 'data'=>$arrH),
                                //array('name' => 'Aguardando registro de precos', 'data'=>$arrI),
                                array('name' => 'Em Planejamento pelo Proponente', 'data'=>$arrJ),
                            );

                            $formatoValores = array(
                                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                                'tooltip' => "formatter: function() { return '<b>' + this.x + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }",
                                'alinhamento' => "formatter: function() { return '1'; }"
                            );

                            echo geraGraficoLinha($aDados, $arrX, "graficoLinhadoTempoExpansao", "Etapas",300, $formatoValores);
						}
						?>
                	</td>
				</tr>
			</table>
		</td>
	</tr>
</table>

<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(3202);" width="60%" rowspan="3">
            <div>
                <?php exibirTitulo('executive', 'Matrículas por Categoria de Situação'); ?>
            </div>
            <?php
            $sql = "SELECT tid1.tiddsc AS acao, ".montaColunaCategoriaPronatec()." AS situacao, SUM(dsh.dshqtde::integer) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (3202)
                    AND sh.sehstatus <> 'I'
                    $periodoPronatec
                    GROUP BY acao, situacao
                    ORDER BY acao, situacao";
            $dados = $db->carregar( $sql, null, 3200 );
            if($dados){
                $dadosAgrupados = agruparDadosGrafico($dados, 'acao', 'situacao', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />Porcentagem: <b>' + number_format(this.percentage, 2, ',', '.') + '%</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaSituacao', null, null, $formatoValores, null, null, 1000, 600, null, 300);
            }
            ?>
		</td>
		<!-- INÍCIO DO FINANCEIRO -->
		<?php
		// Parâmetros para a nova conexão com o banco do SIAFI
		$servidor_bd = $servidor_bd_siafi;
		$porta_bd    = $porta_bd_siafi;
		$nome_bd     = $nome_bd_siafi;
		$usuario_db  = $usuario_db_siafi;
		$senha_bd    = $senha_bd_siafi;
		// Cria o novo objeto de conexão
		$db2 = new cls_banco();
		// Parâmetros da nova conexão com o banco do SIAFI para o componente 'combo_popup'.
		$dados_conexao = array(
			'servidor_bd' => $servidor_bd_siafi,
			'porta_bd' => $porta_bd_siafi,
			'nome_bd' => $nome_bd_siafi,
			'usuario_db' => $usuario_db_siafi,
			'senha_bd' => $senha_bd_siafi
		);
		?>
		<td class="fundo_padrao" colspan="2">
			<?php
			$sql = "SELECT
						ano,
						sum(valor1) AS empenhado,
						sum(valor2) AS dotacao,
						sum(valor3) AS pago
					FROM (
						SELECT
							2011 as ano,
							CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor1,
							CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor2,
							CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor3
						FROM
							dw.saldo2011 sld
						WHERE (substr(sld.plicod, 2, 4) in ('FP05','FD01','FP01') OR sld.acacod in ('10FU', '8650', '1H10'))
						AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
					UNION ALL
						SELECT
							2012 as ano,
							CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor1,
							CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor2,
							CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor3
						FROM
							dw.saldo2012 sld
						WHERE (substr(sld.plicod, 2, 4) in ('FP05','FD01','FP01') OR sld.acacod in ('20RG'))
						AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
					UNION ALL
						SELECT
							2013 as ano,
							CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor1,
							CASE WHEN sld.sldcontacontabil in ('192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor2,
							CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
								CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.1821 ELSE (sld.sldvalor) END
							ELSE 0
							END AS valor3
						FROM
							dw.saldo2013 sld
						WHERE (substr(sld.plicod, 2, 4) in ('FP05','FD01','FP01') OR sld.acacod in ('20RG'))
						AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','192110101','192110201','192110209','192110301','192110303','192130101','192130102','192130103','192130201','192140100','192140200','192190201','192190209','192190301','192190302','292130301','292410403')
					) as foo
					GROUP BY ano
					ORDER BY ano";
			$arrDados = $db2->carregar($sql,null,3200);
			$total_dotacao = 0;
			$total_empenhado = 0;
			$total_pago = 0;
			?>
			<div>
				<?php exibirTitulo('financeiro', 'Orçamentário/Financeiro'); ?>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<th class="center bold" >Ano</th>
					<th class="center bold" >Empenhado</th>
					<!--<th class="center bold" >Pago</th>-->
				</tr>
				<?php
				$count = -1;
				foreach($arrDados as $chave => $dado):
				    $count++;
					?>
					<tr class="link <?php echo ($count%2) ? 'zebrado' : ''; ?>" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['ano'] ?>&escala=1&agrupador[0]=subacao&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&subacao[0]=FP05&subacao[1]=FD01&subacao[2]=FP01&alterar_ano=0');" >
						<td><?php echo $dado['ano'] ?></td>
						<td class="numero" ><?php $total_empenhado+=$dado['empenhado']; echo number_format($dado['empenhado'],2,",",".") ?></td>
						<!--<td class="numero" ><?php $total_pago+=$dado['pago'];  echo number_format($dado['pago'],2,",",".") ?></td>-->
					</tr>
				<?php endforeach; ?>
				<tr>
					<th class="bold" >Total</th>
					<th class="numero bold" ><?php echo number_format($total_empenhado,2,",",".") ?></th>
					<!--<th class="numero bold" ><?php echo number_format($total_pago,2,",",".") ?></th>-->
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<!-- Tabela Processos -->
		<td class="fundo_padrao link" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/portfolio-de-processos/')";>
			<div>
				<?php exibirTitulo('recycle', 'Mapa do Processo'); ?>
			</div>
		</td>
		<!-- Tabela Processos -->
		<td class="fundo_padrao link" onclick="abreAcaoPainel('146,145,16,33,85');">
			<div>
				<?php exibirTitulo('indicador', 'Indicadores'); ?>
			</div>
		</td>
	</tr>
	<tr>
		<!-- Tabela Alinhamento Estratégico -->
		<td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('pe',2,5,11,'');" align="center">
			<?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
			<img src="cockpit/images/alinhamentoEstrategico.png" />
		</td>
		<!-- FIM Tabela Alinhamento Estratégico -->
		<td class="fundo_padrao" align="middle">
			<br><br><br><p class="toggle_consolidado5 titulo_box link">Perfil dos beneficiários</p>
		</td>
	</tr>
	<tr id="graficos_consolidados5">
		<td width="100%" colspan="4">
			<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" class="tabela_painel">
				<tr>
					<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2761);" width="50%">
						<div>
							<?php exibirTitulo('indicador', 'Matrículas por gênero'); ?>
						</div>
						<?
						$sql = "SELECT
								tid1.tiddsc AS descricao,
								sum(dsh.dshqtde) as valor
							FROM painel.seriehistorica sh
							INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
							INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
							INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
							WHERE sh.indid in (2761)
							AND sh.sehstatus <> 'I'
							AND dsh.tidid1 NOT IN (7448)
							$periodoPronatec
							GROUP BY descricao
							ORDER BY descricao";
						$arrDados = $db->carregar( $sql, null, 3200 );
                        if($arrDados){
						    geraGrafico($arrDados, "graficoPizzaGenero", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
                        }
						?>
					</td>
					<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2784);">
						<div>
							<?php exibirTitulo('indicador', 'Matrículas por raça/cor '); ?>
						</div>
						<?
						$sql = "SELECT
								tid1.tiddsc AS descricao,
								sum(dsh.dshqtde) as valor
							FROM painel.seriehistorica sh
							INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
							INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid2
							INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
							WHERE sh.indid in (2784)
							AND sh.sehstatus <> 'I'
							AND dsh.tidid2 NOT IN (7501)
							$periodoPronatec
							GROUP BY descricao
							ORDER BY descricao";
						$arrDados = $db->carregar( $sql, null, 3200 );
                        if($arrDados){
						    geraGrafico($arrDados, "graficoPizzaRaca", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
                        }
						?>
					</td>
				</tr>
				<tr>
					<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2576);">
						<div>
							<?php exibirTitulo('indicador', 'Matrículas por faixa etária'); ?>
						</div>
						<?
						$sql = "SELECT
								tid1.tiddsc AS descricao,
								sum(dsh.dshqtde) as valor
							FROM painel.seriehistorica sh
							INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
							INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid2
							INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
							WHERE sh.indid in (2576)
							AND sh.sehstatus <> 'I'
							$periodoPronatec
							GROUP BY descricao
							ORDER BY descricao";
						$arrDados = $db->carregar( $sql, null, 3200 );
                        if($arrDados){
						    geraGrafico($arrDados, "graficoPizzaIdade", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
                        }
						?>
					</td>
					<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2783);">
						<div>
							<?php exibirTitulo('indicador', 'Matrículas por nível escolaridade'); ?>
						</div>
						<?
						$sql = "SELECT
								tid1.tiddsc AS descricao,
								sum(dsh.dshqtde) as valor
							FROM painel.seriehistorica sh
							INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
							INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
							INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
							WHERE sh.indid in (2783)
							AND sh.sehstatus <> 'I'
							AND dsh.tidid1 NOT IN (7485)
							$periodoPronatec
							GROUP BY descricao
							ORDER BY descricao";
						$arrDados = $db->carregar( $sql, null, 3200 );
                        if($arrDados){
						    geraGrafico($arrDados, "graficoPizzaEscolaridade", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
                        }
						?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2577);" colspan="3">
			<div>
				<?php exibirTitulo('mapas', 'Número de Matrículas por Estado'); ?>
            </div>
            <?php
            $sql = "SELECT dsh.dshuf as descricao, tid1.tiddsc AS rede, SUM(dsh.dshqtde::integer) AS valor
                    FROM painel.seriehistorica sh
                    INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
                    INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
                    INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
                    WHERE sh.indid IN (2577)
                    AND sh.sehstatus <> 'I'
                    $periodoPronatec
                    GROUP BY descricao, rede
                    ORDER BY descricao, rede";
            $arrDados = $db->carregar( $sql );
            if($arrDados){
                $dadosAgrupados = agruparDadosGrafico($arrDados, 'descricao', 'rede', 'valor');

                $formatoValores = array(
                    'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                    'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Total: <b>'+ number_format(this.point.stackTotal, 0, ',', '.') + '</b>'; }"
                );
                geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaMatriculaEstado', null, null, $formatoValores, null, "", 1000, 600, null, 300);
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
    function geraLinha($qtd, $dias, $concluida=false){

        $font = '#fff';
        if ($qtd > 60) {
            $cor = '#f00';
        } elseif ($qtd > 45) {
            $cor = '#FFD700';
            $font = '#000';
        } else {
            $cor = 'green';
        }
		
		if($concluida){
			$cor = 'green';
		}

        ?>
        <center>
            <span style='padding: 0 5px; border-width:1px; border-style:solid; border-color:rgb(0, 0, 0); background-color:<?=$cor?>; text-align:center; color:<?php echo $font; ?>; height:15px;'>
                <?php echo number_format($qtd,0,",","."); ?>
            </span>
        </center>
        <?
    }
?>