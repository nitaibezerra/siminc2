<?php
set_time_limit(0);

include_once APPRAIZ . 'pde/www/_funcoes_cockpit.php';

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
		jQuery.noConflict();

		jQuery(function(){
			jQuery('.div_fotos_interno').cycle({
				fx: 'scrollDown'
			});

			jQuery('.scrollbar').perfectScrollbar({wheelPropagation:true});

			jQuery('.lista-noticias').totemticker({
				row_height  :   '100px',
				next        :   '#ticker-next',
				previous    :   '#ticker-previous',
				stop        :   '#stop',
				start       :   '#start',
				mousestop   :   true,
			});
		});


		atualizaUsuario();
		
	function abreLinkExterno()
	{
		var url = "http://www.cienciasemfronteiras.gov.br/web/csf/bolsistas-pelo-mundo/";
		window.open(url,'Bolsistas pelo Mundo','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
	}
	
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
	  	.fundo_titulo{background-image:url('cockpit/images/bannerCienciaSemFronteira.png');background-repeat:no-repeat;background-color:#FFFFFF;background-position:center;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
	</style>
</head>
<body onload="refreshAutomatico();">

<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
		<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
            <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
        	<div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
		</div>
		<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_ciencia_sem_fronteiras&acao=A';">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<!-- Título-->
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="4" ><div style="margin:50px" >&nbsp;</div></td>
	</tr>
	<!-- Fim Título-->
	<tr>
		<td class="fundo_padrao" width="40%" rowspan="3" >
			<?php
			$sql = "select to_char(max(sehdtcoleta),'DD/MM/YYYY') as dataatualizacao from painel.seriehistorica where indid = 1559";
			$dataatualizacao = $db->pegaUm($sql);
            ?>
            <div>
                <?php exibirTitulo('alvo', 'Metas e bolsas concedidas<br>por fonte de financiamento', '<span class="link" onclick="abreIndicadorPopUp(1559);">Modalidades de bolsa no exterior</span><br><span class="link" onclick="abreIndicadorPopUp(2591);">Modalidades de bolsa no país</span><br>Atualizado em ' . $dataatualizacao); ?>
            </div>
            <?php
            $sql = "SELECT dpeanoref, SUM(qtde) AS qtde, SUM(qtdemeta) AS qtdemeta
                    FROM(
                        select dpe.dpeanoref, sum(dsh.dshqtde) as qtde, 0 as qtdemeta
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        where
                            (
                                (sh.indid in (1559) AND dsh.tidid1 IN (4098)) --CAPES
                                OR
                                (sh.indid in (2591) AND dsh.tidid2 IN (7126)) --CAPES
                            )
                        and sh.sehstatus <> 'I'
                        group by dpe.dpeanoref
                    UNION ALL
                        select '2011' as dpeanoref, 0 as qtde, 3400 as qtdemeta
                    UNION ALL
                        select '2012' as dpeanoref, 0 as qtde, 13600 as qtdemeta
                    UNION ALL
                        select '2013' as dpeanoref, 0 as qtde, 25800 as qtdemeta
                    UNION ALL
                        select '2014' as dpeanoref, 0 as qtde, 40000 as qtdemeta
                    ) AS FOO
                    GROUP BY dpeanoref
                    ORDER BY dpeanoref";
            $dados = $db->carregar( $sql, null, 3200 );

            $arrayx = array();
            $arrayy = array();
            $acumulay = 0;
            $arrXY = array();

            if($dados){
            foreach ($dados as $dado)
                {
                    $arrayx[] = $dado['dpeanoref'];
                    $acumulay += $dado['qtde'];
                    if($dado['qtde']>0){
                        $arrayy[] = $acumulay;
                    }
                    $arrXY[] = $dado['qtdemeta'];
                }
            }
            echo "<table width='95%'><tr><td>";
            echo geraGraficoXY($arrayx, $arrayy, $arrXY, "grafico1", "Bolsas Concedidas CAPES",200, "Bolsas");
            echo "</td></tr></table>";

            $sql = "SELECT dpeanoref, SUM(qtde) AS qtde, SUM(qtdemeta) AS qtdemeta
                    FROM(
                        select dpe.dpeanoref, sum(dsh.dshqtde) as qtde, 0 as qtdemeta
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        where
                            (
                                (sh.indid in (1559) AND dsh.tidid1 IN (4099)) --CNPQ
                                OR
                                (sh.indid in (2591) AND dsh.tidid2 IN (7127)) --CNPQ
                            )
                        and sh.sehstatus <> 'I'
                        group by dpe.dpeanoref
                    UNION ALL
                        select '2011' as dpeanoref, 0 as qtde, 3890 as qtdemeta
                    UNION ALL
                        select '2012' as dpeanoref, 0 as qtde, 10030 as qtdemeta
                    UNION ALL
                        select '2013' as dpeanoref, 0 as qtde, 20260 as qtdemeta
                    UNION ALL
                        select '2014' as dpeanoref, 0 as qtde, 35000 as qtdemeta
                    ) AS FOO
                    GROUP BY dpeanoref
                    ORDER BY dpeanoref";
            $dados = $db->carregar( $sql, null, 3200 );

            $arrayx = array();
            $arrayy = array();
            $acumulay = 0;
            $arrXY = array();

            if($dados){
            foreach ($dados as $dado)
                {
                    $arrayx[] = $dado['dpeanoref'];
                    $acumulay += $dado['qtde'];
                    if($dado['qtde']>0){
                        $arrayy[] = $acumulay;
                    }
                    $arrXY[] = $dado['qtdemeta'];
                }
            }
            echo "<table width='90%'><tr><td>";
            echo geraGraficoXY($arrayx, $arrayy, $arrXY, "grafico2", "Bolsas Concedidas CNPQ",200, "Bolsas");
            echo "</td></tr></table>";

            $sql = "SELECT dpeanoref, SUM(qtde) AS qtde, SUM(qtdemeta) AS qtdemeta
                    FROM(
                        select dpe.dpeanoref, sum(dsh.dshqtde) as qtde, 0 as qtdemeta
                        from painel.seriehistorica sh
                        inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                        inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                        where sh.indid in (1559)
                        and sh.sehstatus <> 'I'
                        group by dpe.dpeanoref
                    UNION ALL
                        select '2011' as dpeanoref, 0 as qtde, 7290 as qtdemeta
                    UNION ALL
                        select '2012' as dpeanoref, 0 as qtde, 23630 as qtdemeta
                    UNION ALL
                        select '2013' as dpeanoref, 0 as qtde, 46060 as qtdemeta
                    UNION ALL
                        select '2014' as dpeanoref, 0 as qtde, 101000 as qtdemeta
                    ) AS FOO
                    GROUP BY dpeanoref
                    ORDER BY dpeanoref";
            $dados = $db->carregar( $sql, null, 3200 );

            $arrayx = array();
            $arrayy = array();
            $acumulay = 0;
            $arrXY = array();

            if($dados){
            foreach ($dados as $dado)
                {
                    $arrayx[] = $dado['dpeanoref'];
                    $acumulay += $dado['qtde'];
                    if($dado['qtde']>0){
                        $arrayy[] = $acumulay;
                    }
                    $arrXY[] = $dado['qtdemeta'];
                }
            }
            echo "<table width='90%'><tr><td>";
            echo geraGraficoXY($arrayx, $arrayy, $arrXY, "grafico3", "Bolsas Concedidas Total",200, "Bolsas");
            echo "</td></tr></table>";
            ?>
		</td>
		<td class="fundo_padrao link" width="40%" onclick="abreIndicadorPopUp(1561);" rowspan="2">
			<?php
			$sql = "select to_char(max(sehdtcoleta),'DD/MM/YYYY') as dataatualizacao from painel.seriehistorica where indid = 1561";
			$dataatualizacao = $db->pegaUm($sql);
			?>
			<div>
				<?php exibirTitulo('alvo', 'Bolsas concedidas<br>por modalidade e ano', 'Atualizado em ' . $dataatualizacao); ?>
			</div>
			<?php
			$soma2011 = 0;
			$soma2012 = 0;
			$soma2013 = 0;
			$soma2014 = 0;
			$sql = "select
                        tid.tiddsc as tipo,
                        dpe.dpeanoref as ano,
                        sum(dsh.dshqtde) as total
                    from painel.indicador i
                    inner join painel.seriehistorica sh on sh.indid=i.indid
                    inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
                    inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
                    inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
                    where i.indid in (1561,2591)
                    and sh.sehstatus <> 'I'
                    and dpe.dpeanoref between '2011' and '2014'
                    group by tipo, ano
                    order by tipo, ano";
			$arrDados = $db->carregar( $sql, null, 3200 );
			if($arrDados){
				foreach($arrDados as $dado){
					$arrTipo[$dado['tipo']][$dado['ano']]['total'][] = $dado['total'];
				}
			}
			?>
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<th class="center bold" >Modalidade de Bolsa</td>
					<th class="center bold" >2011</th>
					<th class="center bold" >2012</th>
					<th class="center bold" >2013</th>
                    <th class="center bold" >2014</th>
					<th class="center bold" >Total</th>
				</tr>
				<?php
				$count = -1;
				if($arrDados){
					foreach($arrTipo as $chave => $onb):
					$total2011 = is_array($onb['2011']['total'])?array_sum($onb['2011']['total']):0;
					$total2012 = is_array($onb['2012']['total'])?array_sum($onb['2012']['total']):0;
					$total2013 = is_array($onb['2013']['total'])?array_sum($onb['2013']['total']):0;
                    $total2014 = is_array($onb['2014']['total'])?array_sum($onb['2014']['total']):0;
					$count++;
					?>
					<tr <?php echo ($count%2) ? 'class="zebrado"' : ''; ?>>
						<td class="" ><?=$chave ?></td>
						<td class="numero" ><?=number_format($total2011,0,",",".") ?></td>
						<td class="numero" ><?=number_format($total2012,0,",",".") ?></td>
						<td class="numero" ><?=number_format($total2013,0,",",".") ?></td>
                        <td class="numero" ><?=number_format($total2014,0,",",".") ?></td>
						<td class="numero" ><?=number_format($total2011+$total2012+$total2013+$total2014,0,",",".") ?></td>
					</tr>
					<?php
					$soma2011 += is_array($onb['2011']['total'])?array_sum($onb['2011']['total']):0;
					$soma2012 += is_array($onb['2012']['total'])?array_sum($onb['2012']['total']):0;
					$soma2013 += is_array($onb['2013']['total'])?array_sum($onb['2013']['total']):0;
                    $soma2014 += is_array($onb['2014']['total'])?array_sum($onb['2014']['total']):0;
					endforeach;
					$somaTotal = $soma2011 + $soma2012 + $soma2013 + $soma2014;
				}
				?>
				<tr>
					<th class="bold"  >Total</th>
					<th class="numero bold"  ><?php echo number_format($soma2011,0,",",".")?></th>
					<th class="numero bold"  ><?php echo number_format($soma2012,0,",",".")?></th>
					<th class="numero bold"  ><?php echo number_format($soma2013,0,",",".")?></th>
                    <th class="numero bold"  ><?php echo number_format($soma2014,0,",",".")?></th>
					<th class="numero bold"  ><?php echo number_format($somaTotal,0,",",".")?></th>
				</tr>
			</table>
		</td>
		<td class="fundo_padrao link" width="20%" align="middle" onclick="window.location='estrategico.php?modulo=principal/cockpit_ingles_sem_fronteiras&acao=A';">
			<div>
				<?php exibirTitulo('Presentation', 'Inglês sem Fronteiras'); ?>
			</div>
		</td>
	</tr>
<!--
    <tr>
        <td class="fundo_padrao link" width="20%" onclick="window.location='estrategico.php?modulo=principal/cockpit_ciencia_sem_fronteiras_socioeconomico&acao=A';">
            <div>
                <?php exibirTitulo('Presentation', 'Ciência sem Fronteiras<br>Socioeconômico'); ?>
            </div>
        </td>
    </tr>
-->
	<tr>
		<td class="fundo_azul_padrao link" onclick="abreIndicadorPopUp(1563);" valign="top" rowspan="3">
			<div>
				<?php exibirTitulo('configs', 'Bolsas concedidas<br>por país de destino'); ?>
			</div>
			<br><br>
			<?php
			$sql = "select tid.tidid, tid.tiddsc, SUM(dsh.dshqtde) AS total
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
					inner join painel.detalhetipoindicador tdi on tid.tdiid = tdi.tdiid
					where i.indid in (1563)
					and sehstatus <> 'I'
					group by tid.tidid, tid.tiddsc
					order by tid.tiddsc";
	        $arrFotos = $db->carregar( $sql );
			?>
			<div style="clear:both" id="div_fotos_padrao" >
				<?php for($x=17;$x>=1;$x--): ?>
					<div style="clear:both" class="div_fotos_interno" >
						<?php for($i=$x;$i<=(17+$x);$i++): ?>
							<div class="div_fotos_padrao" >
								<table border="0"  >
									<tr>
										<td style="color:#FFFFFF" >
											<img src="./cockpit/images/paises/<?php echo $arrFotos[$i]['tidid'] ?>.gif" width="90px" height="50px"  />
										</td>
										<td style="color:#FFFFFF" >
											<b><?php echo $arrFotos[$i]['tiddsc'] ?></b><br />
											<b>Nº de bolsistas:</b> <?php echo number_format($arrFotos[$i]['total'],0,",",".") ?><br/>
										</td>
									</tr>
								</table>
							</div>
						<?php endfor; ?>
					</div>
				<?php endfor; ?>
			</div>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao link" valign="top" onclick="abreIndicadorPopUp(1560);">
			<div>
				<?php exibirTitulo('indicador', 'Bolsas concedidas<br>por área prioritária'); ?>
			</div>
			<?
			$sql = "select descricao, valor
					from(
						select 'Outros' as descricao, sum(valor) as valor
						from (
							select tid.tiddsc as descricao, sum(dsh.dshqtde) as valor
							from painel.seriehistorica sh
							inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
							inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
							where sh.indid in (1560)
							and sh.sehstatus <> 'I'
							group by dsh.tidid1, descricao
							order by valor DESC, descricao
							OFFSET 9
						) as foo
					union all
						select tid.tiddsc as descricao, sum(dsh.dshqtde) as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
						where sh.indid in (1560)
						and sh.sehstatus <> 'I'
						group by dsh.tidid1, descricao
						order by valor DESC, descricao
						limit 9
					) as foo2
					order by descricao";
			$arrDados = $db->carregar( $sql, null, 3200 );

			geraGrafico($arrDados, "graficoPizzaArea", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
			?>
		</td>
	</tr>
	<tr>
        <td class="fundo_padrao" valign="top">
            <div>
                <?php exibirTitulo('indicador', 'Bolsas concedidas<br>por fonte de financiamento', '<span class="link" onclick="abreIndicadorPopUp(1559);">Modalidades de bolsa no exterior</span><BR><span class="link" onclick="abreIndicadorPopUp(2591);">Modalidades de bolsa no país</span>'); ?>
            </div>
            <?
            $sql = "select
			            CASE sh.indid
			                WHEN 1559 THEN tid1.tiddsc
			                WHEN 2591 THEN tid2.tiddsc
			            END AS descricao, sum(dsh.dshqtde) as valor
					from painel.seriehistorica sh
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					LEFT JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
					LEFT JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					where sh.indid in (1559, 2591)
					and sh.sehstatus <> 'I'
					and dpe.dpeanoref between '2011' and '2014'
					GROUP BY descricao
					ORDER BY descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaFonte", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" valign="top" onclick="abreIndicadorPopUp(1562);">
            <div>
                <?php exibirTitulo('configs', 'Bolsas concedidas<br>por gênero'); ?>
            </div>
            <?
            $sql = "select tid.tiddsc as descricao, sum(dsh.dshqtde) as valor
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
					where i.indid in (1562)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaGenero", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
    </tr>
    <tr>
        <td class="fundo_padrao link" valign="top" onclick="abreIndicadorPopUp(2886);" rowspan="3">
            <div>
                <?php exibirTitulo('configs', 'Bolsas concedidas<br>por raça/cor'); ?>
            </div>
            <?
            $sql = "select tid.tiddsc as descricao, sum(dsh.dshqtde) as valor
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid2
					where i.indid in (2886)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaRacaCor", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <td class="fundo_padrao link" valign="top" onclick="abreIndicadorPopUp(2887);" rowspan="3">
            <div>
                <?php exibirTitulo('configs', 'Bolsas concedidas<br>por faixa etária'); ?>
            </div>
            <?
            $sql = "select tid.tiddsc as descricao, sum(dsh.dshqtde) as valor
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid2
					where i.indid in (2887)
					and sh.sehstatus <> 'I'
					group by descricao
					order by descricao";
            $arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaFaixaEtaria", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
            ?>
        </td>
        <td class="fundo_padrao">
            <div>
                <?php exibirTitulo('financeiro', 'Orçamentário / Financeiro'); ?>
            </div>
            <?=exibirTabelaFinanceiro(168);?>
<?php /* ?>
            <?php
            // Parâmetros para a nova conexão com o banco do SIAFI
            $servidor_bd = $servidor_bd_siafi;
            $porta_bd = $porta_bd_siafi;
            $nome_bd = $nome_bd_siafi;
            $usuario_db = $usuario_db_siafi;
            $senha_bd = $senha_bd_siafi;

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
            $sql = "SELECT ano, sum(empenhado) as empenhado, sum(pago) as pago, filtro
					FROM (
						SELECT
							2011 AS ano,
							sum(valor1) AS empenhado,
							sum(valor2) AS pago,
							'painel=1&submetido=1&ano=2011&escala=1&agrupador[0]=uo&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&orgao[0]=26000&uo[]=26291&ptres[0]=003130&alterar_ano=0' AS filtro
						FROM
							(SELECT
								CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
									CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
								ELSE 0
								END AS valor1,
								CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
									CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
								ELSE 0
								END AS valor2
							FROM dw.saldo2011 sld
							WHERE substr(sld.orgcod, 1, 2) in (substr('26000', 1, 2)) AND sld.unicod in ('26291') AND sld.ptres in ('003130') AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','292130301','292410403')
							) AS foo
						WHERE
							valor1 <> 0 OR valor2 <> 0
					UNION ALL
						SELECT
							2012 AS ano,
							sum(valor1) AS empenhado,
							sum(valor2) AS pago,
							'painel=1&submetido=1&ano=2012&escala=1&agrupador[0]=uo&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&orgao[0]=26000&uo[]=26291&enquadramento[0]=T&alterar_ano=0' AS filtro
						FROM
							(SELECT
								CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
									CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
								ELSE 0
								END AS valor1,
								CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
									CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
								ELSE 0
								END AS valor2
							FROM dw.saldo2012 sld
							WHERE substr(sld.orgcod, 1, 2) in (substr('26000', 1, 2)) AND sld.unicod in ('26291') AND substr(sld.plicod, 1, 1) in ('T') AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','292130301','292410403')
							) AS foo
						WHERE
							valor1 <> 0 OR valor2 <> 0
					UNION ALL
						SELECT
							2013 AS ano,
							sum(valor1) AS empenhado,
							sum(valor2) AS pago,
							'painel=1&submetido=1&ano=2013&escala=1&agrupador[0]=uo&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&orgao[0]=26000&uo[]=26291&ptres[0]=061719&alterar_ano=0' AS filtro
						FROM
							(SELECT
								CASE
									WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
										CASE WHEN sld.ungcod='154004' THEN (sld.sldvalor)*2.398 ELSE (sld.sldvalor) END
									ELSE 0 END
								AS valor1,
								CASE
									WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
									CASE WHEN sld.ungcod='154004' then (sld.sldvalor)*2.398 ELSE (sld.sldvalor) END
									ELSE 0
								END AS valor2
							FROM
								dw.saldo2013 sld
							WHERE substr(sld.orgcod, 1, 2) in (substr('26000', 1, 2)) AND sld.unicod in ('26291') AND sld.ptres in ('061719') AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','292130301','292410403')
							) AS foo
						WHERE
							valor1 <> 0 OR valor2 <> 0
					) AS FOO
					GROUP BY
						ano, filtro
					ORDER BY
						ano";

            //temporário
            $sql = "select 2011 as ano, 64600865.91 as empenhado
						union all
						select 2012 as ano, 474874960.16 as empenhado
						union all
						select 2013 as ano, 1063969546.34 as empenhado";
            $arrDados = $db2->carregar($sql,null,3200);
            $count = -1;
            $totalEmpenhado = 0;
            ?>
            <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                <tr>
                    <th class="center bold" >Ano</th>
                    <th class="center bold" >Valor Empenhado (R$)</th>
                    <!--<th class="center bold" >Valor Pago (R$)</th>-->
                </tr>
                <?php foreach($arrDados as $dado):
                    $count++;
                    ?>
                    <tr <?php echo ($count%2) ? 'class="zebrado "' : 'class=""'; ?> ">
					<td class="" ><?php echo $dado['ano'] ?></td>
					<td class="numero" ><?php echo number_format($dado['empenhado'],2,",",".") ?></td>
					<!--<td class="numero" ><?php echo number_format($dado['pago'],2,",",".") ?></td>-->
				</tr>
				<?php
				$totalEmpenhado += $dado['empenhado'];
                endforeach; ?>
                <tr>
                    <th>Total</th>
                    <th class="numero" ><?php echo number_format($totalEmpenhado,2,",",".") ?></th>
                </tr>
            </table>
 <?php */ ?>
        </td>
    </tr>
    <tr>
        <!-- Tabela Alinhamento Estratégico -->
        <td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae', '3', '168', '', '');" align="center" >
            <?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
            <img src="cockpit/images/alinhamentoEstrategico.png" />
        </td>
        <!-- FIM Tabela Alinhamento Estratégico -->
    </tr>
    <tr>
        <td class="fundo_padrao link" onclick="abreLinkExterno();">
            <div>
                <?php exibirTitulo('mapas', 'Mapa de bolsistas<br>pelo mundo', 'Confira aqui o<br>quantitativo de<br>bolsas implementadas e<br>bolsas vigentes'); ?>
            </div>
            <br><br>
            <center><img src="cockpit/images/mundo.jpg" width="184px" height="104px"  /></center>
        </td>
    </tr>
	<tr>
		<td class="fundo_padrao_laranja" colspan="4">
		    <div style="text-align:center;"  >
		    <img src="../imagens/icones/icons/executiverel.png"  style="vertical-align:middle;"  />
		    <input type="text" onclick="this.style.color='#000000';this.value='';"  name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;'    title='' class=' normal' />
		    <img src="../imagens/icones/icons/Find.png"  style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);"  />
		    </div>
	    </td>
	</tr>
</table>
</body>
</html>