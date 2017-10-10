<?PHP
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );

// Educação Básica Obras 1
$_SESSION['obras']['orgid'] = 2;
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
		
		atualizaUsuario();
		
		function abrePopUpObrasSituacao(tipo, situacao){
	        window.open('/pde/estrategico.php?modulo=principal/popuplistaobrassituacao&acao=A&tipo=' + tipo + '&situacao='+situacao);
	    }
		
		function abreObras1(obrid){
    		window.open('/obras/obras.php?modulo=principal/cadastro&acao=A&obrid='+obrid);
    	}
		
		function abreMetaPronatec(){
			window.open('/pde/estrategico.php?modulo=principal/popupMetasPronatec&acao=A');
		}
		
		function abreDetalheRedeFederal(){
			window.open('/pde/estrategico.php?modulo=principal/popupDetalhesRedeFederal&acao=A');
		}
		
		function visualizarXLS(tipo){
			window.location='estrategico.php?modulo=principal/cockpit_rede_federal&acao=A&tipo='+tipo;
		}
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
		.fundo_titulo{background-image:url('../imagens/fundobrasilpro.jpg')};
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
			<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_rede_federal&acao=A';">
				<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="4" ><div style="margin:28px" >Expansão da Rede Federal de EPT</div></td>
	</tr>
	<tr>
		<td class="fundo_padrao" width="30%" >
			<div>
				<?php exibirTitulo('alvo', '<span class="link" onclick="abreMetaPronatec();">Meta 2014 - PRONATEC<br />343.080 matrículas em cursos<br />técnicos regulares</span>'); ?></div>
			</div>
		</td>
		<td class="fundo_padrao" colspan="2" rowspan="2" width="55%">
			<div>
				<?php exibirTitulo('indicador', 'Obras por situação'); ?>
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
		<td class="fundo_azul_padrao" rowspan="3" width="15%">
			<div>
				<?php exibirTitulo('configs', 'Obras da Expansão da <br/>Rede Federal de <br/>Educação Profissional, <br/>Científica e Tecnológica<br/>'); ?>
			</div>
			<?php
			$sql = "SELECT
						obr.obrid, ent.entnome,
						mun.mundescricao AS municipio,
						mun.estuf AS uf,
						sto.stodesc,
						obr.obrpercexec,
						arqid
					FROM obras.obrainfraestrutura obr
					INNER JOIN entidade.entidade ent ON ent.entid = obr.entidunidade
					INNER JOIN obras.tipoobra tob ON tob.tobaid = obr.tobraid
					INNER JOIN obras.situacaoobra sto ON sto.stoid = obr.stoid
					INNER JOIN entidade.endereco edc ON edc.endid = obr.endid
					INNER JOIN territorios.municipio mun ON mun.muncod = edc.muncod
					INNER JOIN
						(select MAX(arqid) AS arqid, obrid FROM obras.fotos GROUP BY obrid) AS fot ON fot.obrid = obr.obrid
					WHERE obr.obsstatus = 'A'
					AND prfid = 34 --EXPANSÃO RFEPT
					AND obr.obrpercexec >= 50
					ORDER BY random()
					LIMIT 50";
			$arrFotos = $db->carregar($sql);
			?>
			<div style="clear:both" id="div_fotos_padrao"  >
				<?php for($x=5;$x>=1;$x--): ?>
					<div style="clear:both" class="div_fotos_interno"  >
					<?php for($i=$x;$i<=(5+$x);$i++): ?>
						<div class="div_fotos_padrao" >
							<table border="0" >
								<tr onclick="abreObras1(<?=$arrFotos[$i]['obrid']?>);">
									<td>
										<img width="80" height="60" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $arrFotos[$i]['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=obras" />
									</td>
									<td style="color:#FFFFFF" >
										<?php echo substr($arrFotos[$i]['entnome'],0,60) ?>...<br/>
										<?php echo $arrFotos[$i]['municipio'] ?> / <?php echo $arrFotos[$i]['uf'] ?><br/>
										<?php echo $arrFotos[$i]['stodesc'] ?> <br/>
										<?php echo str_replace(array(".00","."),array("",","),$arrFotos[$i]['obrpercexec']) ?>% de execução
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
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(525);">
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
					<td class="titulo_box bold" align="center">Expansão Fase III</td>
				</tr>
				<tr>
					<td class="titulo_box bold" align="center">
						<?php exibirTitulo('alvo', '208 câmpus 2011-14'); ?>
						<div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 100px;'>
							<div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $porcentagem ?>px;'><?= $porcentagem ?>%&nbsp;</div>
						</div><?php echo number_format($totalCampusFuncionando,0,",",".") ?> funcionando
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao" >
			<div class="link" onclick="abreIndicadorPopUp(2347);">
				<?php exibirTitulo('indicador', 'Ingressantes em cursos<br>técnicos regulares - Meta Pronatec'); ?>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
                	<td>
						<?
						$sql = "SELECT dpeanoref AS periodo, SUM(qtdereal) AS qtdereal, SUM(qtdemeta) AS qtdemeta
								FROM (
									SELECT dpe.dpeanoref, sum(dsh.dshqtde) AS qtdereal, 0 AS qtdemeta
									FROM painel.seriehistorica sh
									INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
									INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
									WHERE sh.indid IN (2347)
									AND sh.sehstatus <> 'I'
									AND dsh.tidid2 IN (5130,5131,5188,5190,5189,5129,5132)
									AND dpe.dpeanoref >= '2011'
									GROUP BY dpe.dpeanoref
								UNION ALL
									SELECT '2011' AS dpeanoref, 0 AS qtdereal, 72000 AS qtdemeta
								UNION ALL
									SELECT '2012' AS dpeanoref, 0 AS qtdereal, 79560 AS qtdemeta
								UNION ALL
									SELECT '2013' AS dpeanoref, 0 AS qtdereal, 90360 AS qtdemeta
								UNION ALL
									SELECT '2014' AS dpeanoref, 0 AS qtdereal, 101160 AS qtdemeta
								) AS foo
								GROUP BY dpeanoref
								ORDER BY dpeanoref";
						$dados = $db->carregar( $sql, null, 3200 );
						$arrayx = array();
						$arrayy = array();
						$arrXY = array();
						
						if($dados){
						foreach ($dados as $dado)
							{
								$arrayx[] = $dado['periodo'];
								if($dado['qtdereal']>0){
									$arrayy[] = $dado['qtdereal'];
								}
								$arrXY[] = $dado['qtdemeta'];
							}
						}

						echo geraGraficoXY($arrayx, $arrayy, $arrXY, "graficoMatriculasPronatec", "Matriculas Rede Federal Pronatec",200, "Matriculas");

						?>
					</td>
				</tr>
				<tr>
					<td class="fundo_padrao" align="left">
						<input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(1);" style="cursor: pointer;"/>
					</td>
				</tr>
				<tr id="graficos_consolidados2">
					<td>
						<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
							<tr>
								<th class="center bold" >Ano</th>
								<th class="center bold" >Empenhado</th>
							</tr>
							<?php
							if($arrDados){
								$count = -1;
								foreach($arrDados as $chave => $dado):
									$count++;
									?>
									<tr class="link <?php echo ($count%2) ? 'zebrado' : ''; ?>" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['ano'] ?>&escala=1&agrupador[0]=subacao&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&subacao[0]=FP05&subacao[1]=FD01&subacao[2]=FP01&alterar_ano=0');" >
										<td><?php echo $dado['ano'] ?></td>
										<td class="numero" ><?php $total_empenhado+=$dado['empenhado']; echo number_format($dado['empenhado'],2,",",".") ?></td>
										<!--<td class="numero" ><?php $total_pago+=$dado['pago'];  echo number_format($dado['pago'],2,",",".") ?></td>-->
									</tr>
								<?php endforeach;
							}?>
							<tr>
								<th class="bold" >Total</th>
								<th class="numero bold" ><?php echo number_format($total_empenhado,2,",",".") ?></th>
							</tr>
						</table>
					</td>
				</tr>
				<script>		
					$(function(){
						$('.tabelaPne').hide();
					});
				</script>
			</table>
		</td>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2294);" colspan="2">
			<div>
				<?php exibirTitulo('indicador', 'Evolução das obras da Expansão Fase III (2011-2014)', 'Câmpus por situação'); ?>
			</div>
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

			echo geraGraficoLinha($aDados, $arrX, "graficoLinhadoTempo", "Etapas",200);
			?>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao link" colspan="4">

            <div>
                <?php exibirTitulo('indicador', 'Linha do tempo da criação/federalização das unidades'); ?>
            </div>
            <?php
            $sql = "SELECT count(distinct entid) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					WHERE sh.indid IN (525)
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					AND dsh.tidid1 IN (2596, 2597, 7107)
					AND dsh.tidid2 IN (1738)
					AND sh.sehstatus <> 'I'";
            $qtd = $db->pegaUm($sql);

            $dados = array(
                array('descricao'=>"Nilo Peçanha<br />1909-1910", 'valor'=>'20'),
                array('descricao'=>"Hermes da Fonseca<br />1911-1914", 'valor'=>'1'),
                array('descricao'=>"Venceslau Brás<br />1914-1918", 'valor'=>'1'),
                array('descricao'=>"Delfim Moreira<br />1918-1919", 'valor'=>'1'),
                array('descricao'=>"Epitácio Pessoa<br />1919-1922", 'valor'=>'3'),
                array('descricao'=>"Artur Bernardes<br />1922-1926", 'valor'=>'1'),
                array('descricao'=>"Getúlio Vargas<br />1930-1945 e 1951-1954 ", 'valor'=>'15'),
                array('descricao'=>"Gaspar Dutra<br />1946-1951", 'valor'=>'11'),
                array('descricao'=>"Café Filho<br />1954-1955", 'valor'=>'1'),
                array('descricao'=>"Juscelino Kubitschek<br />1956-1961", 'valor'=>'7'),
                array('descricao'=>"João Goulart<br />1961-1964", 'valor'=>'8'),
                array('descricao'=>"Ranieri Mazzilli<br />1964-1964", 'valor'=>'1'),
                array('descricao'=>"Castelo Branco<br />1964-1967", 'valor'=>'2'),
                array('descricao'=>"Costa e Silva<br />1967-1969", 'valor'=>'9'),
                array('descricao'=>"Emílio G. Médici<br />1969-1974", 'valor'=>'2'),
                array('descricao'=>"Ernesto Geisel<br />1974-1979", 'valor'=>'1'),
                array('descricao'=>"João B. Figueiredo<br />1979-1985", 'valor'=>'4'),
                array('descricao'=>"José Sarney<br />1985-1990", 'valor'=>'11'),
                array('descricao'=>"Fernando Collor de Mello<br />1990-1992", 'valor'=>'4'),
                array('descricao'=>"Itamar Franco<br />1992-1995", 'valor'=>'26'),
                array('descricao'=>"Fernando Henrique Cardoso<br />1995-2003", 'valor'=>'11'),
                array('descricao'=>"Luiz Inácio Lula da Silva<br />2003-2010", 'valor'=>'214'),
                array('descricao'=>"Dilma Rousseff<br />2011-2014", 'valor'=>$qtd),
            );

            $config = array('rotation'=>'-45', 'dataLabels'=>'true');
            geraGraficoColuna($dados, 'graficoColunaExpansao', null, null, $formatoValores, null, 'estrategico.php?modulo=principal/popuplistaunidades&acao=A', 1000, 600, null, 300, $config);
            ?>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2294);" rowspan="3">
			<div>
				<?php exibirTitulo('indicador', 'Unidades<br>em funcionamento'); ?>
			</div>
			<?
			$sql = "SELECT
						tid.tiddsc AS descricao,
						sum(dsh.dshqtde) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid2
					WHERE sh.indid in (2294)
					AND sh.sehstatus <> 'I'
					AND sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid AND s.sehstatus <> 'I')
					GROUP BY descricao";
			$arrDados = $db->carregar( $sql, null, 3200 );
            if($arrDados){
                geraGrafico($arrDados, "graficoPizzaUnidadesFuncionamento", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
            }
			?>
		</td>
		<td class="fundo_padrao" rowspan="3" colspan="2">
			<div>
				<?php exibirTitulo('indicador', '<span class="link" onclick="abreDetalheRedeFederal();">Ingressantes Etapa / Modalidade</span>', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal EPT</span>, <span class="link" onclick="abreIndicadorPopUp(2401);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2393);">E-TEC</span>'); ?>
			</div>
			<?
			$sql = "SELECT
						CASE
							WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) OR dsh.tidid1 IN (5303, 5304) OR sh.indid in (2393) THEN 'Técnico'
							WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
							WHEN dsh.tidid2 IN (5129, 9263) OR dsh.tidid1 IN (5302) THEN 'FIC'
							WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
							WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
						END AS descricao,
						sum(dsh.dshqtde) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
					WHERE
						(
							sh.indid in (2347)
							OR
							(sh.indid in (2401) AND dsh.tidid2 IN (5323))
							OR
							(sh.indid in (2393) AND dsh.tidid1 IN (5209) AND dpe.dpeanoref >= '2011')
						)
					AND sh.sehstatus <> 'I'
					GROUP BY descricao
					ORDER BY descricao";
			$arrDados = $db->carregar( $sql, null, 3200 );
			geraGrafico($arrDados, "graficoPizzaIngressantes", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
			?>
			<table width="100%">
				<tr>
					<td class="fundo_padrao" align="left">
						<input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(2);" style="cursor: pointer;"/>
					</td>
				</tr>
			</table>
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
		<td class="fundo_padrao">
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
						WHERE sld.acacod in ('10FU', '8650', '1H10')
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
						WHERE sld.acacod in ('20RG')
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
						WHERE sld.acacod in ('20RG')
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
				foreach($arrDados as $dado):
				    $count++;
					if($dado['ano']==2011)
					{
					?>
						<tr class="link <?php echo ($count%2) ? 'zebrado' : ''; ?>" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['ano'] ?>&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&acacod[0]=10FU&acacod[1]=8650&acacod[2]=1H10&alterar_ano=0')" >
					<?php
					}else{
					?>
						<tr class="link <?php echo ($count%2) ? 'zebrado' : ''; ?>" onclick="abreRelatorio('painel=1&submetido=1&ano=<?php echo $dado['ano'] ?>&escala=1&agrupador[0]=acacod&agrupadorColunas[0]=6&agrupadorColunas[1]=7&agrupadorColunas[2]=129&agrupadorColunas[3]=97&acacod[0]=20RG&alterar_ano=0')" >
					<?php
					}
					?>
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
		<td class="fundo_padrao link" onclick="window.open('http://escritoriodeprocessos.mec.gov.br/portfolio-de-processos/');">
			<div>
				<?php exibirTitulo('recycle', 'Mapa do Processo'); ?>
			</div>
		</td>
		<!-- Tabela Processos -->
	</tr>
	<tr>
		<td class="fundo_padrao link" onclick="abreAcaoPainel(85);">
			<div>
				<?php exibirTitulo('indicador', 'Indicadores'); ?>
			</div>
			<?php //montarIndicadoresProcesso(array(133995, 142017)); ?>
		</td>
	</tr>
	<tr>
		<td class="fundo_padrao" colspan="4">
			<div>
				<?php exibirTitulo('indicador', 'Comparação do Esforço Escolar (em horas-aula)', '<span class="link" onclick="abreIndicadorPopUp(2347);">Rede Federal EPT</span>, <span class="link" onclick="abreIndicadorPopUp(2401);">Bolsa-Formação</span>, <span class="link" onclick="abreIndicadorPopUp(2399);">Acordo de Gratuidade</span>, <span class="link" onclick="abreIndicadorPopUp(2393);">E-TEC</span>'); ?>
			</div>
			<?php
			$sql = "select rede, tipo, sum(valor) as valor
					from(
						select
							'Rede Federal' as rede,
							CASE WHEN dsh.tidid2 IN (5129) THEN 'FIC' ELSE 'Técnico' END AS tipo,
							case
								when dsh.tidid2 IN (5130) then sum(dsh.dshqtde) * 3000
								when dsh.tidid2 IN (5188) then sum(dsh.dshqtde) * 2400
							else sum(dsh.dshqtde) * 1000
							end as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						WHERE sh.indid IN (2347)
						AND sh.sehstatus <> 'I'
						AND dsh.tidid2 IN (5130,5131,5188,5190,5189,5129,5132)
						AND dpe.dpeanoref >= '2011'
						GROUP BY dsh.tidid2
					UNION ALL
						select
							'Rede Federal' as rede,
							'Total' as tipo,
							case
								when dsh.tidid2 IN (5130) then sum(dsh.dshqtde) * 3000
								when dsh.tidid2 IN (5188) then sum(dsh.dshqtde) * 2400
							else sum(dsh.dshqtde) * 1000
							end as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
						WHERE sh.indid IN (2347)
						AND sh.sehstatus <> 'I'
						AND dsh.tidid2 IN (5130,5131,5188,5190,5189,5129,5132)
						AND dpe.dpeanoref >= '2011'
						GROUP BY dsh.tidid2
					union all
						select
							tid2.tiddsc as rede,
							case when dsh.tidid1 = 5302 then 'FIC' else 'Técnico' end as tipo,
							case when dsh.tidid1 = 5302 then sum(dsh.dshqtde) * 200 else sum(dsh.dshqtde) * 1000 end as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
						WHERE sh.indid IN (2401)
						AND sh.sehstatus <> 'I'
						and dsh.tidid2 IN (5323, 5324, 5325)
						group by rede, tipo, dsh.tidid1
					union all
						select
							tid2.tiddsc as rede,
							'Total' as tipo,
							case when dsh.tidid1 = 5302 then sum(dsh.dshqtde) * 200 else sum(dsh.dshqtde) * 1000 end as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
						WHERE sh.indid IN (2401)
						AND sh.sehstatus <> 'I'
						and dsh.tidid2 IN (5323, 5324, 5325)
						group by rede, tipo, dsh.tidid1
					union all
						select
							tid2.tiddsc as rede,
							tid1.tiddsc as tipo, 
							case when dsh.tidid1 = 5298 then sum(dsh.dshqtde) * 200 else sum(dsh.dshqtde) * 1000 end as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
						WHERE sh.indid IN (2399)
						AND sh.sehstatus <> 'I'
						group by rede, tipo, dsh.tidid1
					union all
						select
							tid2.tiddsc as rede,
							'Total' as tipo,
							case when dsh.tidid1 = 5298 then sum(dsh.dshqtde) * 200 else sum(dsh.dshqtde) * 1000 end as valor
						from painel.seriehistorica sh
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
						INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
						WHERE sh.indid IN (2399)
						AND sh.sehstatus <> 'I'
						group by rede, tipo, dsh.tidid1
					union all
						SELECT
							'Rede Federal' as rede,
							'Técnico' as tipo,
							sum(dsh.dshqtde) * 1000 as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						WHERE sh.indid IN (2393)
						AND sh.sehstatus <> 'I'
						AND dpe.dpeanoref >= '2011'
						AND dsh.tidid1 IN (5209)
					UNION ALL
						SELECT
							'Rede Federal' as rede,
							'Total' as tipo,
							sum(dsh.dshqtde) * 1000 as valor
						FROM painel.seriehistorica sh
						INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
						INNER JOIN painel.detalhetipodadosindicador tid ON tid.tidid = tidid1
						INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
						WHERE sh.indid IN (2393)
						AND sh.sehstatus <> 'I'
						AND dpe.dpeanoref >= '2011'
						AND dsh.tidid1 IN (5209)
					) as foo
					group by rede, tipo
					order by rede, tipo";
			$dados = $db->carregar( $sql, null, 3200 );

            $dadosAgrupados = agruparDadosGrafico($dados, 'rede', 'tipo', 'valor');

            $formatoValores = array(
                'y' => ",formatter: function () { return number_format(this.value, 0, ',', '.'); }",
                'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }"
            );

			geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaEsforcoEscolar', null, null, $formatoValores, null, "", 1000, 600, null, 300, false);
			?>
			<table width="50%">
				<tr>
					<td width="50%">
						Cursos técnicos conc. e subseq. = 1000h<br>
						Cursos técnicos integrados = 3000h<br>
						Cursos PROEJA integrados = 2400h<br>
						Cursos FIC = 200h
					</td>
					<td width="50%" align="right">
						Esforço Escolar = Carga Horária X Nº de Matrículas ingressantes
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
<?php /*
		<td class="fundo_padrao link" colspan="3" onclick="abreIndicadorPopUp(538);">
			<div>
				<?php exibirTitulo('indicador', 'Investimento em novos câmpus'); ?>
			</div>
			<?php
			$sql = "SELECT dsh.dshuf as descricao, dpe.dpeanoref as ano, sum(dsh.dshqtde::integer) as valor
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on dpe.dpeid = sh.dpeid
					WHERE sh.indid IN (538)
					AND sh.sehstatus <> 'I'
					AND dpe.dpeanoref BETWEEN '2011' AND '2013'
					GROUP BY descricao, ano
					ORDER BY descricao, ano";
			$dados = $db->carregar( $sql, null, 3200 );

			$dadosAgrupados = agruparDadosGrafico($dados, 'descricao', 'ano', 'valor');

			$formatoValores = array(
				'y' => ",formatter: function () { return number_format(this.value, 2, ',', '.'); }",
				'tooltip' => "formatter: function() { return '<b>' + this.x + ' (' + this.series.name + ') </b><br />Ocorrências: <b>R$ ' + number_format(this.y, 2, ',', '.') + '</b><br />' + 'Total: <b> R$ '+ number_format(this.point.stackTotal, 2, ',', '.') + '</b>'; }"
			);
			geraGraficoColunaAgrupado($dadosAgrupados['dados'], $dadosAgrupados['divisoes'], 'graficoColunaInvestimentoEstado', null, null, $formatoValores, null, null, 1000, 600, null, 300);
			?>
		</td>
*/ ?>
		<td class="fundo_padrao" colspan="3" rowspan="2">
			<div>
				<?php exibirTitulo('indicador', 'Evolução de matrículas atendidas', '<span class="link" onclick="abreIndicadorPopUp(2575);">Rede Federal</span>, <span class="link" onclick="abreIndicadorPopUp(2401);">Bolsa-Formação</span>'); ?>
			</div>
			<?
			$arrEtapa = array();
			$sql = "SELECT
						--dpe.dpeanoref AS periodo, 
						REPLACE(dpe.dpeanoref, '2014', '2013') as periodo,
						CASE
							WHEN dsh.tidid1 IN (6994,6995,6996,6997,6998,6999, 5303, 5304) THEN 'Técnico'
							WHEN dsh.tidid1 IN (6987,6986,6985,6984) THEN 'Pós-Graduação'
							WHEN dsh.tidid1 IN (7000, 9264, 5302) THEN 'FIC'
							WHEN dsh.tidid1 IN (7001) THEN 'Ensino Médio'
							WHEN dsh.tidid1 IN (6988,6989,6990,6991,6992,6993) THEN 'Superior'
						END AS etapa,
						sum(dsh.dshqtde) as total
					FROM painel.seriehistorica sh
					INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
					INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
					WHERE (sh.indid in (2575) OR (sh.indid in (2401) AND dsh.tidid2 IN (5323)))
					AND sh.sehstatus <> 'I'
					GROUP BY periodo, etapa
					ORDER BY periodo, etapa";
			$arrDados = $db->carregar( $sql, null, 3200 );
			if($arrDados){
				foreach($arrDados as $dado){
					$arrEtapa[$dado['periodo']][$dado['etapa']]['total'][] = $dado['total'];
				}
			}
			$arrX = array();
			$arrA = array();
			$arrB = array();
			$arrC = array();
			$arrD = array();
			$arrE = array();
			$arrTotal = array();
			
			foreach($arrEtapa as $periodo => $sit)
			{
				$arrX[] = $periodo;
				$vA = Round((is_array($sit['Técnico']['total'])?array_sum($sit['Técnico']['total']):0));
				$vB = Round((is_array($sit['Pós-Graduação']['total'])?array_sum($sit['Pós-Graduação']['total']):0));
				$vC = Round((is_array($sit['FIC']['total'])?array_sum($sit['FIC']['total']):0));
				$vD = Round((is_array($sit['Ensino Médio']['total'])?array_sum($sit['Ensino Médio']['total']):0));
				$vE = Round((is_array($sit['Superior']['total'])?array_sum($sit['Superior']['total']):0));
				$vF = Round((is_array($sit['Total']['total'])?array_sum($sit['Total']['total']):0));
				$vTotal = 0;
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
				array('name' => 'Tecnico', 'data'=>$arrA),
				array('name' => 'Pos-Graduacao', 'data'=>$arrB),
				array('name' => 'FIC', 'data'=>$arrC),
				array('name' => 'Ensino Medio', 'data'=>$arrD),
				array('name' => 'Superior', 'data'=>$arrE),
				array('name' => 'Total', 'data'=>$arrTotal),
			);

			echo geraGraficoLinha($aDados, $arrX, "graficoEvolucaoMatriculas", "Etapas/Modalidades",300);
			?>
			<table>
				<tr>
					<td class="fundo_padrao" align="left">
						<input type="button" name="btnXls" value="Visualizar XLS" onclick="visualizarXLS(3);" style="cursor: pointer;"/>
					</td>
				</tr>
			</table>
		</td>
		<!-- Tabela Alinhamento Estratégico -->
		<td class="fundo_padrao link" onclick="abreAlinhamentoEstrategico('ae',2,85,'','');" align="center" >
			<?php exibirTitulo('configs', 'Alinhamento Estratégico'); ?>
			<img src="cockpit/images/alinhamentoEstrategico.png" />
		</td>
		<!-- FIM Tabela Alinhamento Estratégico -->
	</tr>
	<tr>
		<td class="fundo_padrao toggle_consolidado1 link">
			<div>
				<?php exibirTitulo('indicador', 'Matrículas ingressantes<br>em regiões específicas<br>2011 a 2013'); ?>
			</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="0" cellpadding="0" class="tabela_painel">
	<tr id="graficos_consolidados1">
		<td width="100%">
			<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5" class="tabela_painel">
				<tr>
					<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2347);" width="50%">
						<div>
							<?php exibirTitulo('indicador', 'Áreas Indígenas'); ?>
						</div>
						<?
						$sql = "SELECT
								CASE
									WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) THEN 'Técnico'
									WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
									WHEN dsh.tidid2 IN (5129, 9263) THEN 'FIC'
									WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
									WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
								END AS descricao,
								sum(dsh.dshqtde) as valor
							FROM painel.seriehistorica sh
							INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
							INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
							WHERE sh.indid in (2347)
							AND dpe.dpeanoref BETWEEN '2011' AND '2013'
							AND sh.sehstatus <> 'I'
							AND dsh.dshcodmunicipio IN (SELECT muncod
														FROM territorios.muntipomunicipio mt
														INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mt.tpmid
														WHERE tpm.gtmid = 3)
							GROUP BY descricao
							ORDER BY descricao";
						$arrDados = $db->carregar( $sql, null, 3200 );

						geraGrafico($arrDados, "graficoPizzaIndio", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
						?>
					</td>
					<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2347);">
						<div>
							<?php exibirTitulo('indicador', 'Semiárido'); ?>
						</div>
						<?
						$sql = "SELECT
								CASE
									WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) THEN 'Técnico'
									WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
									WHEN dsh.tidid2 IN (5129, 9263) THEN 'FIC'
									WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
									WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
								END AS descricao,
								sum(dsh.dshqtde) as valor
							FROM painel.seriehistorica sh
							INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
							INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
							WHERE sh.indid in (2347)
							AND dpe.dpeanoref BETWEEN '2011' AND '2013'
							AND sh.sehstatus <> 'I'
							AND dsh.dshcodmunicipio IN (SELECT muncod
														FROM territorios.muntipomunicipio mt
														INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mt.tpmid
														WHERE tpm.gtmid = 12)
							GROUP BY descricao
							ORDER BY descricao";
						$arrDados = $db->carregar( $sql, null, 3200 );

						geraGrafico($arrDados, "graficoPizzaSemiarido", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
						?>
					</td>
				</tr>
				<tr>
					<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2347);">
						<div>
							<?php exibirTitulo('indicador', 'Territórios da Cidadania'); ?>
						</div>
						<?
						$sql = "SELECT
								CASE
									WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) THEN 'Técnico'
									WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
									WHEN dsh.tidid2 IN (5129, 9263) THEN 'FIC'
									WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
									WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
								END AS descricao,
								sum(dsh.dshqtde) as valor
							FROM painel.seriehistorica sh
							INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
							INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
							WHERE sh.indid in (2347)
							AND dpe.dpeanoref BETWEEN '2011' AND '2013'
							AND sh.sehstatus <> 'I'
							AND dsh.dshcodmunicipio IN (SELECT muncod
														FROM territorios.muntipomunicipio mt
														INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mt.tpmid
														WHERE tpm.gtmid = 5)
							GROUP BY descricao
							ORDER BY descricao";
						$arrDados = $db->carregar( $sql, null, 3200 );

						geraGrafico($arrDados, "graficoPizzaTerritorioCidadania", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
						?>
					</td>
					<td class="fundo_padrao link" onclick="abreIndicadorPopUp(2347);">
						<div>
							<?php exibirTitulo('indicador', 'Grandes Cidades e Capitais'); ?>
						</div>
						<?
						$sql = "SELECT
								CASE
									WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) THEN 'Técnico'
									WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
									WHEN dsh.tidid2 IN (5129, 9263) THEN 'FIC'
									WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
									WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
								END AS descricao,
								sum(dsh.dshqtde) as valor
							FROM painel.seriehistorica sh
							INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
							INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
							WHERE sh.indid in (2347)
							AND dpe.dpeanoref BETWEEN '2011' AND '2013'
							AND sh.sehstatus <> 'I'
							AND dsh.dshcodmunicipio IN (SELECT muncod
														FROM territorios.muntipomunicipio mt
														INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mt.tpmid
														WHERE tpm.gtmid = 1)
							GROUP BY descricao
							ORDER BY descricao";
						$arrDados = $db->carregar( $sql, null, 3200 );

						geraGrafico($arrDados, "graficoPizzaGrandesCidades", "","<b>{series.name}: {point.percentage:.2f}%</b>","","Ocorrências", null, null, null, null, true);
						?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td class="fundo_td_laranja">
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
//GERAR DADOS EM EXCEL
$gerarExcel = $_REQUEST['tipo'];
if($gerarExcel==1){
	//Ingressantes em cursos técnicos regulares – Meta Pronatec
	$sql = "SELECT dpe.dpeanoref AS Ano, tid1.tiddsc AS Situacao, tid2.tiddsc AS Modalidade, sum(dsh.dshqtde)::integer AS Matriculas
			FROM painel.seriehistorica sh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
			INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
			INNER JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
			WHERE sh.indid IN (2347)
			AND sh.sehstatus <> 'I'
			AND dsh.tidid2 IN (5130,5131,5188,5190,5189,5129,5132)
			AND dpe.dpeanoref >= '2011'
			GROUP BY dpe.dpeanoref, tid1.tiddsc, tid2.tiddsc
			ORDER BY dpe.dpeanoref, tid1.tiddsc, tid2.tiddsc";
	$dadosExcel = $db->carregar( $sql );
	ob_clean();
	$db->sql_to_excel( $dadosExcel, "Ingressantes_Cursos_Tecnicos_Regulares" );
}elseif($gerarExcel==2){
	//Ingressantes etapa/modalidade
	$sql = "SELECT
				dpe.dpeanoref AS ano, 
				CASE
					WHEN dsh.tidid2 IN (5132,5189,5190,5188,5131,5130) OR dsh.tidid1 IN (5303, 5304) OR sh.indid in (2393) THEN 'Técnico'
					WHEN dsh.tidid2 IN (5191,5138,5137,5136) THEN 'Pós-Graduação'
					WHEN dsh.tidid2 IN (5129, 9263) OR dsh.tidid1 IN (5302) THEN 'FIC'
					WHEN dsh.tidid2 IN (5234) THEN 'Ensino Médio'
					WHEN dsh.tidid2 IN (5135,5140,5141,5133,5134,5139) THEN 'Superior'
				END AS tipo,
				tid1.tiddsc AS situacao,
				sum(dsh.dshqtde) as matricula
			FROM painel.seriehistorica sh
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
			INNER JOIN painel.detalhetipodadosindicador tid1 ON tid1.tidid = dsh.tidid1
			INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
			WHERE
				(
					sh.indid in (2347)
					OR
					(sh.indid in (2401) AND dsh.tidid2 IN (5323))
					OR
					(sh.indid in (2393) AND dsh.tidid1 IN (5209) AND dpe.dpeanoref >= '2011')
				)
			AND sh.sehstatus <> 'I'
			GROUP BY ano, tipo, situacao
			ORDER BY ano, tipo, situacao";
	$dadosExcel = $db->carregar( $sql );
	ob_clean();
	$db->sql_to_excel( $dadosExcel, "Ingressantes_Etapa_Modalidade" );
}elseif($gerarExcel==3){
	//Evolução de matrículas atendidas
	$sql = "SELECT
			REPLACE(dpe.dpeanoref, '2014', '2013') as ano,
			CASE
				WHEN dsh.tidid1 IN (6994,6995,6996,6997,6998,6999, 5303, 5304) THEN 'Técnico'
				WHEN dsh.tidid1 IN (6987,6986,6985,6984) THEN 'Pós-Graduação'
				WHEN dsh.tidid1 IN (7000, 9264, 5302) THEN 'FIC'
				WHEN dsh.tidid1 IN (7001) THEN 'Ensino Médio'
				WHEN dsh.tidid1 IN (6988,6989,6990,6991,6992,6993) THEN 'Superior'
			END AS etapa,
			sum(dsh.dshqtde)::integer as total
		FROM painel.seriehistorica sh
		INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = sh.sehid
		LEFT JOIN painel.detalhetipodadosindicador tid2 ON tid2.tidid = dsh.tidid2
		INNER JOIN painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
		WHERE (sh.indid in (2575) OR (sh.indid in (2401) AND dsh.tidid2 IN (5323)))
		AND sh.sehstatus <> 'I'
		GROUP BY ano, etapa
		ORDER BY ano, etapa";
	$dadosExcel = $db->carregar( $sql );
	ob_clean();
	$db->sql_to_excel( $dadosExcel, "Evolucao_Matriculas_Atendidas" );
}


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
	
	function montaValores($valorInicial, $valorFinal, $contaMes){
		global $arrXY;

		if($contaMes > 1){
			//====Monta linha meta=====
			$step = ($valorFinal - $valorInicial)/$contaMes;
			$x = $valorInicial;
			//==========================
			while($x<=$valorFinal){
				$arrXY[] = round($x);
				$x += $step;
			}
		}else{
			$arrXY[] = $valorFinal;
		}
	}
?>