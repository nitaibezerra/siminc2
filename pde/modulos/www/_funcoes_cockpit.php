<?php

define( 'APPRAIZ_ZEND',  APPRAIZ . 'includes/library/Zend/');
include_once APPRAIZ . 'www/painel/_funcoes.php';

function pegaUsuarioOnline($sisid){
	global $db;
	$sql = "select COALESCE(count(*),0) as usu_online
			from seguranca.usuariosonline
			where sisid in ($sisid)";
	$usu = $db->pegaUm($sql);
	return($usu ? $usu : 0) . ' <span class="subtitulo_box" >On-line<br/>'.date("d/m/Y").'<br>'.date("g:i:s").'</span>';
}

function montaTabelaEstrategico($nomeindicador, $indicador){
	global $db;
	$sql = "SELECT atidescricao, sum(totalitens) AS totalitens, atiordem, sum(aexecutar) as aexecutar, sum(executado) as executado, sum(atrasado) as atrasado, sum(exeatrasado) as exeatrasado
			FROM (
				SELECT a2.atidescricao,
					count(0) as totalitens, a2.atiordem, 0 as aexecutar, 0 as executado, 0 as atrasado, 0 as exeatrasado
				FROM pde.atividade a1
				INNER JOIN pde.atividade a2 on a2.atiidpai = a1.atiid AND a2.atistatus = 'A'
				INNER JOIN pde.atividade a3 on a3.atiidpai = a2.atiid AND a3.atistatus = 'A'
				LEFT JOIN pde.atividade a4 on a4.atiidpai = a3.atiid AND a4.atistatus = 'A'
				LEFT JOIN pde.monitoraitemchecklist mic on (mic.atiid = a3.atiid OR mic.atiid = a4.atiid) AND mic.micstatus = 'A'
				INNER JOIN pde.monitorameta mnm ON mnm.micid = mic.micid AND mnm.mnmstatus = 'A'
				INNER JOIN painel.detalhemetaindicador i on i.metid = mnm.metid AND i.dmistatus = 'A'
				WHERE a1.atiid = ".$indicador."
				AND a1.atistatus = 'A'
				GROUP BY a2.atiordem, a2.atidescricao
			UNION ALL
				SELECT a2.atidescricao, 0 as totalitens, a2.atiordem,
				case
					when doc.esdid = 443 and i.dmidatameta >= CURRENT_DATE and i.dmidatameta <= CURRENT_DATE+5 then 1
					ELSE 0
				end as aexecutar,
				case
					when doc.esdid in (444, 445) then 1
					ELSE 0
				end as executado,
				case
					when doc.esdid = 443 and i.dmidatameta < CURRENT_DATE then 1
					else 0
				end as atrasado,
				case
					when doc.esdid in (444, 445) and i.dmidatameta < i.dmidataexecucao then 1
					else 0
				end as exeatrasado
				FROM pde.atividade a1
				INNER JOIN pde.atividade a2 on a2.atiidpai = a1.atiid AND a2.atistatus = 'A'
				INNER JOIN pde.atividade a3 on a3.atiidpai = a2.atiid AND a3.atistatus = 'A'
				LEFT JOIN pde.atividade a4 on a4.atiidpai = a3.atiid AND a4.atistatus = 'A'
				LEFT JOIN pde.monitoraitemchecklist mic on (mic.atiid = a3.atiid OR mic.atiid = a4.atiid) AND mic.micstatus = 'A'
				INNER JOIN pde.monitorameta mnm ON mnm.micid = mic.micid AND mnm.mnmstatus = 'A'
				INNER JOIN painel.detalhemetaindicador i on i.metid = mnm.metid AND i.dmistatus = 'A'
				INNER JOIN workflow.documento doc ON doc.docid = i.docid
				WHERE a1.atiid = ".$indicador."
				AND a1.atistatus = 'A'
			) as foo
			GROUP BY atiordem, atidescricao
			ORDER BY atiordem, atidescricao";
	$arrDados = $db->carregar( $sql, null, 3200 );
	$contador = 1;
	$totalgeral = 0;
	?>
	<div>
		<img style="float:left" src="../imagens/icones/icons/indicador.png" style="vertical-align:middle;"  />
		<div style="float:left;" class="titulo_box" >
			<a href="#" onclick="abreIndicadores(<?=$indicador?>, '');" ><?=$nomeindicador?></a>
		</div>
	</div>
	<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
		<tr>
			<td class="center bold" rowspan="2">Etapas</td>
			<td class="center bold" rowspan="2">Progresso</td>
			<td class="center bold" colspan="2">Ponto de Atenção</td>
		</tr>
		<tr>
			<td class="center bold">Em<br>Atraso</td>
			<td class="center bold">A Executar<br>(próximos 5 dias)</td>
			<!--<td style="background-color:#3CB371;" class="center bold" >Executado<br>com<br>atraso</td>-->
		</tr>
		<?php
		if($arrDados){
			foreach($arrDados as $dado):
			$porcentagem = number_format(($dado['executado']/$dado['totalitens'])*100,0,",",".");
			?>
			<tr>
				<td class="link" onclick="abreIndicadores(<?=$indicador?>, <?=$dado['atiordem']?>);" ><?php echo $dado['atidescricao'] ?></td>
				<td class="numero" >
				<div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 15px; width: 100px;'>
					<div style='background-color: #80BC44; text-align: right; color: white; height: 15px; width: <?= $porcentagem ?>px;'><?= $porcentagem ?>%&nbsp;</div>
				</div><?php echo $dado['totalitens'] ?>
				</td>
				<td style="<?=$dado['atrasado'] >0?'background-color:#BB0000':''?>;" class="numero" ><?php echo $dado['atrasado'] ?></td>
				<td style="<?=$dado['aexecutar']>0?'background-color:#FFC211':''?>;" class="numero" ><?php echo $dado['aexecutar'] ?></td>
				<!--<td class="fundo_td_verde numero" ><?php echo $dado['exeatrasado'] ?></td>-->
			</tr>
			<?php
			$contador += 1;
			$totalgeral += $dado['totalitens'];
			$totalatrasado += $dado['atrasado'];
			$totalaexecutar += $dado['aexecutar'];
			//$totalexeatrasado += $dado['exeatrasado'];
			endforeach;
		}
		?>
		<tr>
			<td class=" bold" >Total</td>
			<td class=" numero" ><?php echo $totalgeral ?></td>
			<td class=" numero" ><?php echo $totalatrasado ?></td>
			<td class=" numero" ><?php echo $totalaexecutar ?></td>
			<!--<td style="background-color:#3CB371;" class=" numero" ><?php echo $totalexeatrasado ?></td>-->
		</tr>
	</table>
<?php
}

function montaTabelaEstrategicoOculta($nomeindicador, $indicador, $oculta = true){
	global $db;
	$sql = "SELECT atidescricao, sum(totalitens) AS totalitens, atiordem, sum(aexecutar) as aexecutar, sum(executado) as executado, sum(atrasado) as atrasado, sum(exeatrasado) as exeatrasado
			FROM (
				SELECT a2.atidescricao,
					count(0) as totalitens, a2.atiordem, 0 as aexecutar, 0 as executado, 0 as atrasado, 0 as exeatrasado
				FROM pde.atividade a1
				INNER JOIN pde.atividade a2 on a2.atiidpai = a1.atiid AND a2.atistatus = 'A'
				INNER JOIN pde.atividade a3 on a3.atiidpai = a2.atiid AND a3.atistatus = 'A'
				LEFT JOIN pde.atividade a4 on a4.atiidpai = a3.atiid AND a4.atistatus = 'A'
				LEFT JOIN pde.monitoraitemchecklist mic on (mic.atiid = a3.atiid OR mic.atiid = a4.atiid) AND mic.micstatus = 'A'
				INNER JOIN pde.monitorameta mnm ON mnm.micid = mic.micid AND mnm.mnmstatus = 'A'
				INNER JOIN painel.detalhemetaindicador i on i.metid = mnm.metid AND i.dmistatus = 'A'
				WHERE a1.atiid = ".$indicador."
				AND a1.atistatus = 'A'
				GROUP BY a2.atiordem, a2.atidescricao
			UNION ALL
				SELECT a2.atidescricao, 0 as totalitens, a2.atiordem,
				case
					when doc.esdid = 443 and i.dmidatameta >= CURRENT_DATE and i.dmidatameta <= CURRENT_DATE+5 then 1
					ELSE 0
				end as aexecutar,
				case
					when doc.esdid in (444, 445) then 1
					ELSE 0
				end as executado,
				case
					when doc.esdid = 443 and i.dmidatameta < CURRENT_DATE then 1
					else 0
				end as atrasado,
				case
					when doc.esdid in (444, 445) and i.dmidatameta < i.dmidataexecucao then 1
					else 0
				end as exeatrasado
				FROM pde.atividade a1
				INNER JOIN pde.atividade a2 on a2.atiidpai = a1.atiid AND a2.atistatus = 'A'
				INNER JOIN pde.atividade a3 on a3.atiidpai = a2.atiid AND a3.atistatus = 'A'
				LEFT JOIN pde.atividade a4 on a4.atiidpai = a3.atiid AND a4.atistatus = 'A'
				LEFT JOIN pde.monitoraitemchecklist mic on (mic.atiid = a3.atiid OR mic.atiid = a4.atiid) AND mic.micstatus = 'A'
				INNER JOIN pde.monitorameta mnm ON mnm.micid = mic.micid AND mnm.mnmstatus = 'A'
				INNER JOIN painel.detalhemetaindicador i on i.metid = mnm.metid AND i.dmistatus = 'A'
				INNER JOIN workflow.documento doc ON doc.docid = i.docid
				WHERE a1.atiid = ".$indicador."
				AND a1.atistatus = 'A'
			) as foo
			GROUP BY atiordem, atidescricao
			ORDER BY atiordem, atidescricao";
	$arrDados = $db->carregar( $sql, null, 3200 );
	$contador = 1;
	$totalgeral = 0;
	
	if($arrDados){
		$i=0;
		foreach($arrDados as $dado){
			++$i;
			$porcentagem2 += ($dado['executado']/$dado['totalitens'])*100;
		}
	}
	?>
	<table width="100%">
		<tr>
			<td width="60">
				<img id="img_ciclo_<?php echo $indicador; ?>" <?php echo $oculta==true ? 'alt="Mostrar resumo" title="Mostrar resumo"' : ''; ?> style="float:left;vertical-align:middle;<?php echo $oculta==true ? 'cursor:pointer;' : ''; ?>" src="../imagens/icones/icons/indicador.png" class="<?php echo $oculta==true ? 'bt_mostra_clico' : ''?>"/>
			</td>
			<td class="titulo_box" style="padding-top:18px;">
				<span id="spn_ciclo_<?php echo $indicador; ?>" class="<?php echo $oculta==true ? 'bt_mostra_clico' : ''?>" style="cursor:pointer;"><?=$nomeindicador?></span>
			</td>
			<td>
				<?php //if($oculta==true):?>
					<?php $porcentagem2 = number_format($porcentagem2/$i,0,",","."); ?>				
					<div onclick="abreIndicadores(<?=$indicador?>, '');" style='cursor:pointer;margin-top:13px; border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 100px;'>
						<div style='font-size:18px;background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $porcentagem2 ?>px;'><?= $porcentagem2 ?>%&nbsp;</div>
					</div>
				<?php //endif; ?>
			</td>
		</tr>
	</table>
	<table id="tb_ciclo_<?php echo $indicador; ?>" class="tabela_box" cellpadding="2" cellspacing="1" width="100%" <?php echo $oculta==true ? 'style="display:none;"' : ''; ?>>
		<tr>
			<td class="center bold" rowspan="2">Etapas</td>
			<td class="center bold" rowspan="2">Progresso</td>
			<td class="center bold" colspan="2">Ponto de Atenção</td>
		</tr>
		<tr>
			<td class="center bold">Em<br>Atraso</td>
			<td class="center bold">A Executar<br>(próximos 5 dias)</td>
			<!--<td style="background-color:#3CB371;" class="center bold" >Executado<br>com<br>atraso</td>-->
		</tr>
		<?php
		if($arrDados){
			foreach($arrDados as $dado):
			$porcentagem = number_format(($dado['executado']/$dado['totalitens'])*100,0,",",".");
			?>
			<tr>
				<td class="link" onclick="abreIndicadores(<?=$indicador?>, <?=$dado['atiordem']?>);" ><?php echo $dado['atidescricao'] ?></td>
				<td class="numero" >
				<div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 15px; width: 100px;'>
					<div style='background-color: #80BC44; text-align: right; color: white; height: 15px; width: <?= $porcentagem ?>px;'><?= $porcentagem ?>%&nbsp;</div>
				</div><?php echo $dado['totalitens'] ?>
				</td>
				<td style="<?=$dado['atrasado'] >0?'background-color:#BB0000':''?>;" class="numero" ><?php echo $dado['atrasado'] ?></td>
				<td style="<?=$dado['aexecutar']>0?'background-color:#FFC211':''?>;" class="numero" ><?php echo $dado['aexecutar'] ?></td>
				<!--<td class="fundo_td_verde numero" ><?php echo $dado['exeatrasado'] ?></td>-->
			</tr>
			<?php
			$contador += 1;
			$totalgeral += $dado['totalitens'];
			$totalatrasado += $dado['atrasado'];
			$totalaexecutar += $dado['aexecutar'];
			//$totalexeatrasado += $dado['exeatrasado'];
			endforeach;
		}
		?>
		<tr>
			<td class=" bold" >Total</td>
			<td class=" numero" ><?php echo $totalgeral ?></td>
			<td class=" numero" ><?php echo $totalatrasado ?></td>
			<td class=" numero" ><?php echo $totalaexecutar ?></td>
			<!--<td style="background-color:#3CB371;" class=" numero" ><?php echo $totalexeatrasado ?></td>-->
		</tr>
	</table>
<?php
}

function montarIndicadoresProcesso($projeto)
{
	global $db;

	$where = ' AND _atiprojeto in (' . implode(', ', (array)$projeto)  . ')';
	$sql = "
select * from
(
	(select distinct
		micpai.atiid, sehid, mtmid,sehqtde, sehqtde, dmidatameta, ind_filho.indapelido,dmiqtde,
		ati._atiprojeto, ati._atinumero,
		ind.indid as indpai,
		ind.indid || ' - ' ||ind.indnome as indicadorpai,
		to_char(i.dmidatameta, 'DD/MM/YYYY') as datameta,
		to_char(sh.sehdtcoleta, 'DD/MM/YYYY') as datacoleta,
		to_char(sh.sehdtcoleta, 'YYYY-MM-DD') as sehdtcoleta
	from painel.indicador ind
		inner join pde.monitoraitemchecklist micpai ON ind.indid = micpai.indid
		inner join painel.indicadoresvinculados iv ON iv.indid = ind.indid
		inner join painel.indicador ind_filho ON ind_filho.indid = iv.indidvinculo
		inner join pde.monitoraitemchecklist mic ON ind_filho.indid = mic.indid
		inner join pde.atividade ati ON ati.atiid = mic.atiid
		inner join pde.monitorameta mtm ON mtm.micid = mic.micid and mnmstatus = 'A'
		inner join painel.detalhemetaindicador i on i.metid = mtm.metid AND i.dmistatus = 'A'
		left join painel.seriehistorica sh on sh.indid = ind_filho.indid and i.dmiid = sh.dmiid
	where micpai.micestrategico = 't'
	and mtmid = 1
	$where
	)

	union

	(select distinct
		micpai.atiid, sehid, mtmid,sehqtde, sehqtde, dmidatameta, ind.indapelido,dmiqtde,
		ati._atiprojeto, ati._atinumero,
		ind.indid as indpai,
		ind.indid || ' - ' ||ind.indnome as indicadorpai,
		to_char(i.dmidatameta, 'DD/MM/YYYY') as datameta,
		to_char(sh.sehdtcoleta, 'DD/MM/YYYY') as datacoleta,
		to_char(sh.sehdtcoleta, 'YYYY-MM-DD') as sehdtcoleta
	from painel.indicador ind
		inner join pde.monitoraitemchecklist micpai ON ind.indid = micpai.indid
		-- inner join painel.indicadoresvinculados iv ON iv.indid = ind.indid
		-- inner join painel.indicador ind_filho ON ind_filho.indid = iv.indidvinculo
		inner join pde.monitoraitemchecklist mic ON ind.indid = mic.indid
		inner join pde.atividade ati ON ati.atiid = mic.atiid
		inner join pde.monitorameta mtm ON mtm.micid = mic.micid and mnmstatus = 'A'
		inner join painel.detalhemetaindicador i on i.metid = mtm.metid AND i.dmistatus = 'A'
		left join painel.seriehistorica sh on sh.indid = ind.indid and i.dmiid = sh.dmiid
		-- inner join workflow.documento doc on i.docid = doc.docid
		-- left join workflow.historicodocumento hd on hd.docid = doc.docid
	where micpai.micestrategico = 't'
	and mtmid = 2
	$where
	)
) as metas
order by mtmid,	indpai, dmidatameta
					";

			$arrDados = $db->carregar( $sql, null, 3200 );

/****************************************
*      INDICADORES AUTOMÁTICOS          *
****************************************/
$sqlAutomaticos = "    select distinct
                            mic.atiid, 0 as sehid, 2 as mtmid, 0 as sehqtde, 0 as sehqtde, iv.idvdatameta as dmidatameta, ind.indapelido,iv.idvmeta as dmiqtde,
                            ati._atiprojeto, ati._atinumero,
                            ind.indid as indpai,
                            ind.indid || ' - ' ||ind.indnome as indicadorpai,
                            to_char(iv.idvdatameta, 'DD/MM/YYYY') as datameta,
                            '' as datacoleta,
                            iv.indid, iv.idvfiltro
                        from painel.indicadoresvinculados iv
                            inner join pde.monitoraitemchecklist mic ON iv.indidvinculo = mic.indid
                            inner join pde.atividade ati ON ati.atiid = mic.atiid
                            inner join painel.indicador ind ON ind.indid = iv.indidvinculo
                        where mic.micestrategico = 't'
                        $where
                        and idvstatus = 'A'
                        and coalesce(iv.idvmeta, -1) != -1
                        order by iv.indid, iv.idvdatameta";
			$arrDadosAutomaticos = $db->carregar($sqlAutomaticos);

			$aIndicadoresAutomaticos = array();
			if($arrDadosAutomaticos){

//			    ver($arrDadosAutomaticos, d);

				foreach($arrDadosAutomaticos as $dados){
//					$aIndicadoresAutomaticos[$dados['indid']] = $dados;

                    $idvfiltro = str_replace('todos', '', $dados['idvfiltro']);
                    $filtro = transformaFiltroIndicadorParaArray($idvfiltro);

                    // Utilizando mesma função de recuperar parâmetros utilizada no painel para não haver diferença
                    $arrParametros = getParametrosRelatorioIndicador($dados['indid'], $filtro);
                    $totais = $db->carregar( $arrParametros['sql'] );
                    $somaQtd   = 0;
                    $somaValor = 0;
                    if ($totais) {
                        // Recuperando totais dos dados agrupados
                        foreach ($totais as $total) {
                            $somaQtd   += $total['qtde'];
                            $somaValor += $total['valor'];
                        }
                    }

                    // Adicionando array montado dentro do array dos demais indicadores, obedecendo a estrutura do UNION
                    $dados['sehqtde'] = $somaQtd;
                    $dados['sehid']   = $somaQtd ? 1 : null;
                    unset($dados['indid'], $dados['idvfiltro']);
                    $arrDados[] = $dados;
				}
			}

/****************************************
*      FIM INDICADORES AUTOMÁTICOS      *
****************************************/

			$aIndicadores = array();
			if($arrDados){
				foreach($arrDados as $dados){
					$aIndicadores[$dados['indicadorpai']][] = $dados;
				}
			}
		?>

	<div>
		<img style="float:left" src="../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
		<div style="float:left;" class="titulo_box" >Indicadores de Processo</div>
	</div>

	<div style="clear: both"></div>

	<style type="text/css">

		div.div-legenda{
			float: left;
			width: 10%;
			font-size: 9px;
		}

		div.div-dados-indicador{
			float: left;
			width: 89%;
			font-size: 9px;
		}

		div.div-indicador{
			border: 3px solid #D49019;
			background-color: #F4EAD9;
			color: black;
			margin-top: 5px;
		}

		div.div-dados-meta{
			height: 80px;
			float: left;
			margin-top: 5px;
		}

		div.div-meta{
			border: 1px black solid;
			height: 20px;
			right: 0;
			margin: 5px 0px;
		}

		div.div-progresso{
			height: inherit;
			/*
			height: 10px;
			margin-top: 5px;
			*/
		}


	</style>

	<div>
		<?php foreach($aIndicadores as $indicador => $aMetas){
			$meta1 = current($aMetas);
			$widthDiv = (1/count($aMetas)*100);
			$qtd = '';
			?>
			<div class="div-indicador">
				<span class="span-titulo-indicador" style="font-size: 12px; text-align: center; display: block; margin-bottom: 5px;" onclick="abreIndicadores('<?=$meta1['_atiprojeto']; ?>', '<?=substr($meta1['_atinumero'], 0, strpos($meta1['_atinumero'], '.')); ?>');">
					<?php echo substr($indicador, (strpos($indicador, ' - ')+3)); ?>
				</span>
				<div style="clear: both"></div>

				<div class="div-legenda">
					<div style="margin-top: 10px;">Meta</div>
					<?php if (1 == $meta1['mtmid']) {?>
						<div style="margin-top: 45px;">Executado</div>
					<?php } ?>
				</div>
				<div class="div-dados-indicador">
					<?php
					$porcentagem = 0;
					foreach($aMetas as $count => $meta){
								$data = date('Y-m-d');
//								$data = '2013-05-14';
								$diferencaData = strtotime($meta['dmidatameta']) - strtotime($data);

								// Caso o tipo de meta seja de prazo e foi executada
								if (1 == $meta['mtmid'] && ($meta['sehid'] || $diferencaData < 0)) {
									$aQtd[$meta['indpai']] += (int)(!empty($meta['sehid']));
									$porcentagem = $aQtd[$meta['indpai']]*(($count+1)*100)/($count+1);
									$sPorcentagem = $porcentagem / ($count+1);

								// Caso o tipo de meta seja de quantidade e foi executada
								} elseif(2 == $meta['mtmid'] && ($meta['sehid'] || $diferencaData < 0)){
									if ($meta['sehid']) {
										$aQtd[$meta['indpai']] = $meta['sehqtde'];
									} else {
										$aQtd[$meta['indpai']] += $meta['sehqtde'];
									}

									if ($meta['sehqtde']) {
										$qtd = $meta['sehqtde'];
									}
//									ver($meta['dmiqtde'], $aQtd[$meta['indpai']]);
									$porcentagem  = $aQtd[$meta['indpai']]*(($count+1)*100)/$meta['dmiqtde'];
//									ver($aQtd[$meta['indpai']], $meta['dmiqtde'], $porcentagem);
									$sPorcentagem = $porcentagem / ($count+1);
//									$porcentagem = '100%';
								}
							}

							?>
					<?php foreach($aMetas as $count => $meta){ ?>
						<div class="div-dados-meta" style="width: <?php echo $widthDiv; ?>%;">
							<?php
								$diferencaData = strtotime($meta['dmidatameta']) - strtotime($data);
								$color = $diferencaData < 0 ? '#444' : 'black';
							?>
							<div style="text-align: right; font-size: 11px;">
								<span style="color: <?php echo $color; ?>"><?php echo $meta['datameta']; ?></span>
								<br />
								<?php if (1 == $meta['mtmid']) { ?>
									<span><?php echo $meta['indapelido']; ?></span>
								<?php } elseif(2 == $meta['mtmid']) {?>
									<span><?php echo number_format(round($meta['dmiqtde']), 0, ',', '.'); ?>
								<?php } ?>
							</div>

							<?php if (1 == $meta['mtmid']) {
								$color = 'white';
								$boAtrasado = false;

								$dataComparacao = $meta['sehdtcoleta'] ? $meta['sehdtcoleta'] : $data;
                                $diferencaData = strtotime($meta['dmidatameta']) - strtotime($dataComparacao);

								if ($diferencaData < 0) {
									$boAtrasado = true;
									$color = 'red';
								}
								if ($meta['sehid']) {
									$color = $boAtrasado ? '#F6F120' : 'green';
//    								ver($meta, $data, $color, $boAtrasado, d);
								}

							?>
								<div class="div-meta" style="background-color: <?php echo $color;?>;">
								</div>
							<?php } else {
								$color = '';
								$boAtrasado = false;
								if ($sPorcentagem <= 30) {
									$color = 'red';
									$font  = '#fff';
								} elseif ($sPorcentagem <= 70) {
									$color = '#F6F120';
									$font  = '#000';
								} else {
									$color = 'green';
									$font  = '#fff';
								}

								if (!$qtd) {
									$font  = '#000';
								}
								?>
								<div class="div-meta" <?php echo ($meta['sehid'] && '100' == $sPorcentagem) ? 'style="background-color: ' . $color . ';"' : 'style="background-color: white;"' ?>>
									<?php if (!$count) { ?>
										<div class="div-progresso" style="width: <?php echo $sPorcentagem; ?>%; background-color: <?php echo $color; ?>; position: relative;">
											<span style="font-size: 9px; color: <?php echo $font; ?>;"><?php echo $qtd ? number_format(round($qtd), 0, ',', '.') . ' (' . round($sPorcentagem, 2) . ')' : 0; ?>%</span>
										</div>
									<?php } ?>
								</div>
							<?php } ?>
							<div style="text-align: right; font-size: 11px;">
								<?php echo (1 == $meta['mtmid'] && $meta['sehid']) ? $meta['datacoleta'] : ''; ?>
							</div>
						</div>
					<?php } // foreach ($aMetas as $count => $meta) ?>
				</div>
				<div style="clear: both"></div>
			</div>
		<?php } ?>
		<div style="clear: both"></div>

	</div>
	<?php
}


function listaUniversidadesRedeFederal(){
	global $db;

	$sql = "select e.entid, e.entsig, e.entnome, infra.entid AS alerta
			from entidade.entidade e
			inner join entidade.funcaoentidade fe on fe.entid = e.entid and fe.funid = 12
			inner join entidade.endereco ed on ed.entid = e.entid
			LEFT JOIN
				(SELECT entidunidade AS entid FROM academico.demandainfra WHERE dinstatus = 'A' AND (speid <> 1 OR dinsitlicitacao <> 'L' OR dinsitlicitacao IS NULL) GROUP BY entidunidade) AS infra ON infra.entid = e.entid
			where e.entstatus = 'A' and fe.fuestatus = 'A'
			order by e.entsig, e.entnome";
	$dados = $db->carregar( $sql, null, 3200 );

	echo '<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
	<tr>
	<td class="center bold" >Sigla</td>
	<td class="center bold" >Universidade</td>
<!--<td class="center bold" >Infraestrutura</td>-->
	</tr>';
	$count = -1;
	foreach($dados as $dado):
		$count++;
		?>
		<tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
			<td  ><a style="cursor:pointer" onclick="abreUniversidade(<?=$dado['entid'] ?>);"><?php echo $dado['entsig'] ?></a></td>
			<td  ><a style="cursor:pointer" onclick="abreUniversidade(<?=$dado['entid'] ?>);"><?php echo $dado['entnome'] ?></a></td>
        <!--			<td align="center" ><a style="cursor:pointer" onclick="abreUniversidade(<?=$dado['entid'] ?>);"><?php echo ($dado['alerta'] ?  '<img src="/imagens/0_inativo.png" title="Esta universidade possui câmpus com problemas nas demandas de infraestrutura">' : '<img src="/imagens/0_ativo.png" title="Esta universidade não possui problemas nas demandas de infraestrutura">') ?></a></td>-->
		</tr>
	<?php endforeach;
	echo '</table>';

}

function listaUniversidadesEstadoRedeFederal(){
	global $db;

	$sql = "select e.entid, e.entsig, e.entnome, infra.entid AS alerta 
			from entidade.entidade e
			inner join entidade.funcaoentidade fe on fe.entid = e.entid and fe.funid = 12
			inner join entidade.endereco ed on ed.entid = e.entid
			LEFT JOIN
				(SELECT entidunidade AS entid FROM academico.demandainfra WHERE dinstatus = 'A' AND (speid <> 1 OR dinsitlicitacao <> 'L' OR dinsitlicitacao IS NULL) GROUP BY entidunidade) AS infra ON infra.entid = e.entid
			where e.entstatus = 'A'
			and ed.estuf = '{$_REQUEST['estado']}'
			order by e.entsig, e.entnome";
	$dados = $db->carregar( $sql, null, 3200 );

	echo '<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
	<tr>
	<td class="center bold" >Sigla</td>
	<td class="center bold" >Universidade</td>
<!--	<td class="center bold" >Infraestrutura</td>-->
	</tr>';

	foreach($dados as $dado): ?>
		<tr >
			<td  ><a style="cursor:pointer" onclick="abreUniversidade(<?=$dado['entid'] ?>);"><?php echo $dado['entsig'] ?></a></td>
			<td  ><a style="cursor:pointer" onclick="abreUniversidade(<?=$dado['entid'] ?>);"><?php echo $dado['entnome'] ?></a></td>
        <!--			<td align="center" ><a style="cursor:pointer" onclick="abreUniversidade(<?=$dado['entid'] ?>);"><?php echo ($dado['alerta'] ?  '<img src="/imagens/0_inativo.png" title="Esta universidade possui câmpus com problemas nas demandas de infraestrutura">' : '<img src="/imagens/0_ativo.png" title="Esta universidade não possui problemas nas demandas de infraestrutura">') ?></a></td>-->
		</tr>
	<?php endforeach;
	echo '</table>';
}

function listaInstitutosRedeFederal(){
    global $db;

    $sql = "select e.entid, e.entsig, e.entnome, infra.entid AS alerta
			from entidade.entidade e
			inner join entidade.funcaoentidade fe on fe.entid = e.entid and fe.funid = 11
			inner join entidade.endereco ed on ed.entid = e.entid
			LEFT JOIN
				(SELECT entidunidade AS entid FROM academico.demandainfra WHERE dinstatus = 'A' AND (speid <> 1 OR dinsitlicitacao <> 'L' OR dinsitlicitacao IS NULL) GROUP BY entidunidade) AS infra ON infra.entid = e.entid
			where e.entstatus = 'A' and fe.fuestatus = 'A'
			--and e.entid not in (388725, 411791, 411790, 411728,411726,411727,411737,411736,411734,411731,411735,411743,411742,411744,411738,411724,411739,411746,411740,411730,411741,411729,411723,411733,411732,411725,674625)
			order by e.entsig, e.entnome";
    $dados = $db->carregar( $sql, null, 3200 );

    echo '<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
	<tr>
	<td class="center bold" >Sigla</td>
	<td class="center bold" >Instituto</td>
<!--<td class="center bold" >Infraestrutura</td>-->
	</tr>';
    $count = -1;
    foreach($dados as $dado):
        $count++;
        ?>
        <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
            <td  ><a style="cursor:pointer" onclick="abreInstituto(<?=$dado['entid'] ?>);"><?php echo $dado['entsig'] ?></a></td>
            <td  ><a style="cursor:pointer" onclick="abreInstituto(<?=$dado['entid'] ?>);"><?php echo $dado['entnome'] ?></a></td>
            <!--			<td align="center" ><a style="cursor:pointer" onclick="abreUniversidade(<?=$dado['entid'] ?>);"><?php echo ($dado['alerta'] ?  '<img src="/imagens/0_inativo.png" title="Esta universidade possui câmpus com problemas nas demandas de infraestrutura">' : '<img src="/imagens/0_ativo.png" title="Esta universidade não possui problemas nas demandas de infraestrutura">') ?></a></td>-->
        </tr>
    <?php endforeach;
    echo '</table>';

}

function listaInstitutosEstadoRedeFederal(){
    global $db;

    $sql = "select e.entid, e.entsig, e.entnome, infra.entid AS alerta
			from entidade.entidade e
			inner join entidade.funcaoentidade fe on fe.entid = e.entid and fe.funid = 11
			inner join entidade.endereco ed on ed.entid = e.entid
			LEFT JOIN
				(SELECT entidunidade AS entid FROM academico.demandainfra WHERE dinstatus = 'A' AND (speid <> 1 OR dinsitlicitacao <> 'L' OR dinsitlicitacao IS NULL) GROUP BY entidunidade) AS infra ON infra.entid = e.entid
			where e.entstatus = 'A'
			and ed.estuf = '{$_REQUEST['estado']}'
			and e.entid not in (388725, 411791, 411790, 411728,411726,411727,411737,411736,411734,411731,411735,411743,411742,411744,411738,411724,411739,411746,411740,411730,411741,411729,411723,411733,411732,411725,674625)
			order by e.entsig, e.entnome";
    $dados = $db->carregar( $sql, null, 3200 );

    echo '<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
	<tr>
	<td class="center bold" >Sigla</td>
	<td class="center bold" >Instituto</td>
<!--	<td class="center bold" >Infraestrutura</td>-->
	</tr>';

    foreach($dados as $dado): ?>
        <tr >
            <td  ><a style="cursor:pointer" onclick="abreInstituto(<?=$dado['entid'] ?>);"><?php echo $dado['entsig'] ?></a></td>
            <td  ><a style="cursor:pointer" onclick="abreInstituto(<?=$dado['entid'] ?>);"><?php echo $dado['entnome'] ?></a></td>
            <!--			<td align="center" ><a style="cursor:pointer" onclick="abreUniversidade(<?=$dado['entid'] ?>);"><?php echo ($dado['alerta'] ?  '<img src="/imagens/0_inativo.png" title="Esta universidade possui câmpus com problemas nas demandas de infraestrutura">' : '<img src="/imagens/0_ativo.png" title="Esta universidade não possui problemas nas demandas de infraestrutura">') ?></a></td>-->
        </tr>
    <?php endforeach;
    echo '</table>';
}

function geraGrafico(array $dados,$nomeUnico,$titulo,$formatoDica,$formatoValores,$nomeSerie,$mostrapopudetalhes=false,$caminhopopupdetalhes="",$largurapopupdetalhes="",$alturapopupdetalhes="",$mostrarLegenda = false, $aLegendaConfig = array(), $legendaClique = false, $labelcor = '#fff', $exibirTotal="")
{
    if($legendaClique){
        $legendaClique = 'true';
        $cursor = 'pointer';
    } else {
        $legendaClique = 'false';
        $cursor = 'default';
    }

    $array_valores;

    /*  Configuração da exibição das legendas
     *
     *  Alinhamento (align): O alinhamento do box da legenda dentro da área do gráfico.
     *  Valores válidos "left", "center" ou "right".
     *  Valor padrão "center".
     *
     *  Layout (layout): O layout dos itens da legenda.
     *  Valores válidos "horizontal" ou "vertical".
     *  Valor padrão "horizontal".
     */
    if(!is_array($aLegendaConfig)){
        $aLegendaConfig = array();
    }

    $aLegendaConfig['align']  = is_array($aLegendaConfig['align'])  ? $aLegendaConfig['align']  : 'center';
	$aLegendaConfig['layout'] = is_array($aLegendaConfig['layout']) ? $aLegendaConfig['layout'] : 'horizontal';

	$arrCorItem = array(
						"Aguardando solicitação do município"						=> "'#CCCA00'", // Amarelo
						"Análise da solicitação de utilização da ata"				=> "'#EEAAEE'", // Rosa claro
						"Aguardando ciência do fornecedor"							=> "'#00BFFF'", // Azul claro
						"Aguardando autorização do FNDE"							=> "'#848305'", // Amarelo Escuro
						"Aguardando geração do contrato pelo município"				=> "'#FFD700'", // Laranja
						"Contrato em tramitação"									=> "'#AAEEEE'", // Cinza claro
						"Aguardando emissão de OS (com pendência de regularização)"	=> "'#A077BF'", // Roxo
						"Aguardando emissão de OS"									=> "'#7798BF'", // Roxo claro
						"Aguardando aceite da OS pelo fornecedor"					=> "'#A2A9B0'", // Cinza
						"OS Recusada"												=> "'#000000'", // Preto
						"Solicitação cancelada"										=> "'#FF6A6A'", // Vermelho claro
						"Execução até 25%"											=> "'#E5F6DB'", // Verde
						"Execução de 25 a 50%"										=> "'#C5F5AA'", // Verde
						"Execução de 50% a 75%"										=> "'#A4F677'", // Verde
						"Execução acima de 75%"										=> "'#7AC551'", // Verde
						"Concluída"													=> "'#5EFB09'", // Verde
						"Paralisada"												=> "'#FFFF00'", // Amarelo
						"Obra Cancelada"											=> "'#850808'", // Vermelho
						"Execução"													=> "'#E5F6DB'", // Verde
						"Em Reformulação"											=> "'#0000CC'", // Azul Escuro
						"Ensino Médio"												=> "'#FFFF00'", // Amarelo
						"FIC"														=> "'#00BFFF'", // Azul claro
						"Pós-Graduação"												=> "'#FF6A6A'", // Vermelho claro
						"Superior"													=> "'#EEAAEE'", // Rosa claro
						"Técnico"													=> "'#3CA628'", // Verde
						);
        $arrCor = array();
	foreach ($dados as $item)
	{
		$array_valores .= '[\''.$item['descricao'].'\','.$item['valor'].']';
		$strValores .= "'".$item['descricao']."',";
                if(in_array($item['descricao'], array_keys($arrCorItem))){
                    $arrCor[] = $arrCorItem[$item['descricao']];
                }
        $totalGeralPizza+=$item['valor'];
	}

        if(count($arrCor) > 0){
            $cor = implode(', ' , $arrCor);
            $cor .= "
                    , '#7CCD7C' // Amarelo um pouco mais claro
                    , '#DF5353' // Vermelho rosa escuro

                //    , '#0000FF' // Azul
                    , '#008000' // Verde
                //    , '#FFD700' // Gold
                    , '#CD0000' // Vermelho

                    , '#FF4500' // Laranja
                    , '#ff0066' // Rosa choque
                    , '#4B0082' // Roxo
                    , '#808000' // Verde oliva
                    , '#800000' // Marrom
                    , '#2F4F4F' // Cinza escuro
                    , '#006400' // Verde escuro
                    , '#FFA500' // Amarelo quemado
                    ";

        } else {
            $cor = "
                      '#00BFFF' // Azul claro
                    , '#55BF3B' // Verde
                    , '#FFD700' // Amarelo
                    , '#FF6A6A' // Vermelho claro

                    , '#eeaaee' // Rosa claro
                    , '#aaeeee' // Cinza claro

                    , '#7798BF' // Roxo claro
                    , '#DDDF0D' // Verde claro
                    , '#7CCD7C' // Amarelo um pouco mais claro
                    , '#DF5353' // Vermelho rosa escuro

                //    , '#0000FF' // Azul
                    , '#008000' // Verde
                //    , '#FFD700' // Gold
                    , '#CD0000' // Vermelho

                    , '#FF4500' // Laranja
                    , '#ff0066' // Rosa choque
                    , '#4B0082' // Roxo
                    , '#808000' // Verde oliva
                    , '#800000' // Marrom
                    , '#2F4F4F' // Cinza escuro
                    , '#006400' // Verde escuro
                    , '#FFA500' // Amarelo quemado
                ";
        }

	$strValores = trim($strValores,",");
	$array_valores = str_replace('][', '],[', $array_valores);
	?>
		<script>
		jQuery(document).ready(function() {

                // Radialize the colors
                Highcharts.getOptions().colors = Highcharts.map(
                        [
                           <?php echo $cor ?>
                    ]
                        , function(color) {
                    return {
                        radialGradient: { cx: 0.5, cy: 0.3, r: 0.7 },
                        stops: [
                            [0, color],
                            [1, Highcharts.Color(color).brighten(-0.3).get('rgb')] // darken
                        ]
                    };
                });


		jQuery('#<?=$nomeUnico?>').highcharts({
            lang: {
                printChart: 'Imprimir',
                downloadPDF: 'Exportar em PDF',
                downloadJPEG: 'Exportar em JPG',
                downloadPNG: 'Exportar em PNG',
                downloadSVG: 'Exportar em SVG',
                decimalPoint: ',',
                thousandsSep: '.'
            },
            credits: {
                enabled: false
            },
            chart: {
                plotBackgroundColor: null,
                plotBorderWidth: null,
                plotShadow: true,
                backgroundColor:'rgba(255, 255, 255, 0.0)'
            },
            title: {
                text: '<?=$titulo?>',
                style: {
         			color: '<?=$labelcor?>'
      			}
            },
            tooltip: {
                 pointFormat: '<?=$formatoDica?>'
            },

            //habilitar o botão de salvar como imagem, pdf, etc
            exporting: {
        	 enabled: false
			},
			//estilo legenda
			legend: {
			   layout: '<?php echo $aLegendaConfig['layout']; ?>',
			   align:  '<?php echo $aLegendaConfig['align']; ?>',
			   itemStyle: {
				   paddingBottom: '10px',
				   color: '<?=$labelcor?>'
			   }
		   },
            plotOptions: {
                pie: {
                    point: {
                        events: {
                            legendItemClick: function() {
                                    return <?php echo $legendaClique ?>;
                            }
                        }
                    },
                    cursor: '<?php echo $cursor ?>',
                    borderWidth: 0, // Borda dos pedaços da pizza
                    allowPointSelect: true,
                    dataLabels: {
                        enabled: true,
                        color: '<?=$labelcor?>',
//                        connectorColor: 'white',
						//<b>{point.name}</b>: para colocar o nome na legenda
                        <?php if ($formatoValores) { ?>
                            format: '<?=$formatoValores ?>'
                        <?php } else { ?>
                            formatter: function () { return number_format(this.y, 0, ',', '.') + ' (' + number_format(this.percentage, 2, ',') + ') %'; }
                        <?php } ?>
                    },
					showInLegend: '<?=$mostrarLegenda ?>'
                },
                series: {
                    cursor: 'pointer'
                    ,
                    events: {
//                        legendItemClick:  false
                        <?if ($mostrapopudetalhes): ?>
                            click: function(event){
                                var arrValores = new Array (<?=$strValores?>);

                                var x = screen.width/2 - 700/2;
                                var y = screen.height/2 - 450/2;

                                var janela = window.open('<?=$caminhopopupdetalhes?>&parametro='+arrValores[event.point.x],'winpreobra','menubar=0,scrollbars=yes,width=<?=$largurapopupdetalhes?>,height=<?=$alturapopupdetalhes?>,left='+x+',top='+y);
                                janela.focus();
                            }
                        <? endif ?>
                    }
    			}

            },
            series: [{
                type: 'pie',
                name: '<?=$nomeSerie?>',
                data: [ <?PHP echo $array_valores; ?> ]

            }]
        });
    });

    </script>
 	<div id="<?=$nomeUnico?>" ></div>
	<?
    if($exibirTotal){
        echo "<table align='center'>";
        echo "  <tr>";
        echo "      <td class='titulo_box bold'>Total: ".number_format($totalGeralPizza,0,',','.')."</td>";
        echo "  </tr>";
        echo "</table>";
    }
}

function geraGraficoDrilldown($nomeUnico, $dados, $drilldownSeries)
{
    $dados = simec_json_encode($dados);
    $drilldownSeries = simec_json_encode($drilldownSeries);
	?>
    <script>
        jQuery(function () {

            // Create the chart
            jQuery('#<?php echo $nomeUnico; ?>').highcharts({
                chart: {
                    type: 'pie',
                    plotBackgroundColor: null,
                    // margin: [ 50, 50, 100, 80]
                    plotBorderWidth: null,
                    plotShadow: true,
                    backgroundColor:'rgba(255, 255, 255, 0.0)'
                },
                lang: {
                    drillUpText: 'Voltar'
                },
                title: {
                    text: ''
                },
                subtitle: {
                    text: ''
                },
                plotOptions: {
                    series: {
                        dataLabels: {
                            enabled: true,
                            color: '#fff',

                            format: '{point.percentage:.2f} %',
                        }
                    }
                },
                exporting: {
                    enabled: false
                },
                credits: {
                    enabled: false
                },
                tooltip: {
                    headerFormat: '<span style="font-size:11px">{series.name}</span><br>',
                    pointFormat: '<span style="color:{point.color}">{point.name}</span>: <b>{point.y:.2f}%</b>l<br/>'
                },

                series: [{
                    name: 'Raça',
                    colorByPoint: true,
                    data: <?php echo $dados ?>
                }],
                drilldown: {
                    activeAxisLabelStyle: {
                        color: '#F0F0F3',
                        fontWeight: '',
                        textDecoration: ''
                    },
                    activeDataLabelStyle: {
                        color: '#F0F0F3',
                        fontWeight: '',
                        textDecoration: ''
                    },
                    series: <?php echo $drilldownSeries ?>
                }
            });
        });
    </script>
 	<div id="<?=$nomeUnico?>" ></div>
	<?php
}

function geraGraficoPNE($nomeUnico, $dados)
{
    
    $meta = $dados['metaBrasil'];
    $metaTotal = $dados['metaTotal'];

    $casasDecimais = in_array($dados['meta'], array(7)) ? 1 : 0;
    $metaFormatada = number_format($meta, $casasDecimais, ',', '.');

    $complemento = $simbolo = '';

    $casasDecimais = 1;

    switch ($dados['pnetipo']) {
        case ('E'):
            if($dados['descricao'] == 'Brasil'){
            $corMeta = '#006400';
            }else{
                $corMeta = '#0000CD'; 
            }
            break;
        case ('M'):
            $corMeta = '#B8860B';
            break;
        case ('R'):
            $corMeta = '#999999';
            break;
        case ('S'):
            $corMeta = '#999999';
            break;
    }
  //  if($dados['descricao'] != 'Brasil' && $dados['descricao'] != 'Acre'){
        
//die(var_dump($dados));
   // }
    
    switch ($dados['tipo']) {
        case ('P'):
            $complemento = $simbolo = '%';
            $metaFormatada = $meta;
            break;
        case ('A'):
            $complemento = ' anos';
            $metaFormatada = $meta;
            break;
        case ('M'):
            $casasDecimais = 0;
            $complemento = ' matrículas';
            break;
        case ('T'):
            $casasDecimais = 0;
            $complemento = ' títulos';
            break;
    }

    $formatter = "function () { return '<div style=\"text-align:center\"><span style=\"font-size:20px;color:black; font-weight: normal;\">' + number_format(this.y, " . $casasDecimais .", ',', '.') + '" . $simbolo ."</span><br/>' +
                                       '<span style=\"font-size:13px;color:black; font-weight: normal;\">" . $dados['descricao'] . "</span></div>'; }";

	?>

    <script type="text/javascript">
        jQuery(function () {

            var gaugeOptions = {

                chart: {
                    type: 'solidgauge'
                },

                title: null,

                pane: {
                    center: ['50%', '85%'],
                    size: '120%',
                    startAngle: -90,
                    endAngle: 90,
                    background: {
                        backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || '#EEE',
                        innerRadius: '60%',
                        outerRadius: '100%',
                        shape: 'arc'
                    }
                },

                tooltip: {
                    enabled: false
                },

                // the value axis
                yAxis: {
                    stops: [
                        [0, '<?php echo $dados['cor']; ?>']
                    ],
                    lineWidth: 0,
                    minorTickInterval: null,
                    tickPixelInterval: 400,
                    tickWidth: 0,
                    title: {
                        y: -50
                    },
                    labels: {
                        enabled: false
                    },
                    plotBands: [{
                        from: 0,
                        to: parseFloat('<?php echo $meta; ?>'),
                        color: '<?php echo $corMeta; ?>',
                        innerRadius: '100%',
                        outerRadius: '110%'
                    }]
                },

                plotOptions: {
                    solidgauge: {
                        dataLabels: {
                            y: 5,
                            borderWidth: 0,
                            useHTML: true
                        }
                    }
                }
            };

            // The speed gauge
            jQuery('#<?=$nomeUnico?>').highcharts(Highcharts.merge(gaugeOptions, {
                yAxis: {
                    min: 0,
                    max: parseFloat('<?php echo $metaTotal; ?>'),
                    title: {
                        text: '<span style="color:black;"><?php echo $dados['title'] == '' ? 'Meta Brasil:' : $dados['title']; ?>  <?php echo $metaFormatada . $complemento; ?></span>'
                    }
                },

                credits: {
                    enabled: false
                },

                series: [{
                    name: 'Meta',
                    data: [0],
                    dataLabels: {
                        y: 25,
                        formatter: <?php echo $formatter; ?>
                    },
                    tooltip: {
                        valueSuffix: ' '
                    }
                }]

            }));

            // Bring life to the dials
            var chart = $('#<?=$nomeUnico?>').highcharts();
            if (chart) {
                var point = chart.series[0].points[0],
                newVal = parseFloat('<?php echo $dados['valor']; ?>');
                point.update(newVal);
            }
        });
    </script>
    <div id="<?=$nomeUnico?>" style="width: 200px; height: 150px; float: left"></div>
	<?
}

function geraGraficoCallback(array $dados,$nomeUnico,$titulo,$formatoDica,$formatoValores,$nomeSerie,$mostrapopudetalhes=false,$caminhopopupdetalhes="",$largurapopupdetalhes="",$alturapopupdetalhes="",$mostrarLegenda = false, $aLegendaConfig = array(), $legendaClique = false)
{
    if($legendaClique){
        $legendaClique = 'true';
        $cursor = 'pointer';
    } else {
        $legendaClique = 'false';
        $cursor = 'default';
    }

    $array_valores;

    /*  Configuração da exibição das legendas
     *
     *  Alinhamento (align): O alinhamento do box da legenda dentro da área do gráfico.
     *  Valores válidos "left", "center" ou "right".
     *  Valor padrão "center".
     *
     *  Layout (layout): O layout dos itens da legenda.
     *  Valores válidos "horizontal" ou "vertical".
     *  Valor padrão "horizontal".
     */
	$aLegendaConfig['align']  = $aLegendaConfig['align']  ? $aLegendaConfig['align']  : 'center';
	$aLegendaConfig['layout'] = $aLegendaConfig['layout'] ? $aLegendaConfig['layout'] : 'horizontal';

	$arrCorItem = array(
						"Aguardando solicitação do município"						=> "'#CCCA00'", // Amarelo
						"Análise da solicitação de utilização da ata"				=> "'#EEAAEE'", // Rosa claro
						"Aguardando ciência do fornecedor"							=> "'#00BFFF'", // Azul claro
						"Aguardando autorização do FNDE"							=> "'#848305'", // Amarelo Escuro
						"Aguardando geração do contrato pelo município"				=> "'#FFD700'", // Laranja
						"Contrato em tramitação"									=> "'#AAEEEE'", // Cinza claro
						"Aguardando emissão de OS (com pendência de regularização)"	=> "'#A077BF'", // Roxo
						"Aguardando emissão de OS"									=> "'#7798BF'", // Roxo claro
						"Aguardando aceite da OS pelo fornecedor"					=> "'#A2A9B0'", // Cinza
						"OS Recusada"												=> "'#000000'", // Preto
						"Solicitação cancelada"										=> "'#FF6A6A'", // Vermelho claro
						"Execução até 25%"											=> "'#E5F6DB'", // Verde
						"Execução de 25 a 50%"										=> "'#C5F5AA'", // Verde
						"Execução de 50% a 75%"										=> "'#A4F677'", // Verde
						"Execução acima de 75%"										=> "'#7AC551'", // Verde
						"Concluída"													=> "'#5EFB09'", // Verde
						"Paralisada"												=> "'#FFFF00'", // Amarelo
						"Obra Cancelada"											=> "'#850808'", // Vermelho
						"Execução"													=> "'#E5F6DB'", // Verde
						"Em Reformulação"											=> "'#0000CC'", // Azul Escuro
						"Ensino Médio"												=> "'#FFFF00'", // Amarelo
						"FIC"														=> "'#00BFFF'", // Azul claro
						"Pós-Graduação"												=> "'#FF6A6A'", // Vermelho claro
						"Superior"													=> "'#EEAAEE'", // Rosa claro
						"Técnico"													=> "'#3CA628'", // Verde
						);

        $arrCor = array();
	foreach ($dados as $item)
	{
		$array_valores .= '[\''.removeAcentos($item['descricao']).'\','.$item['valor'].']';
		$strValores .= "'".$item['descricao']."',";
                if(in_array($item['descricao'], array_keys($arrCorItem))){
                    $arrCor[] = $arrCorItem[$item['descricao']];
                }
	}

        if(count($arrCor) > 0){
            $cor = implode(', ' , $arrCor);
            $cor .= "
                    , '#7CCD7C' // Amarelo um pouco mais claro
                    , '#DF5353' // Vermelho rosa escuro

                //    , '#0000FF' // Azul
                    , '#008000' // Verde
                //    , '#FFD700' // Gold
                    , '#CD0000' // Vermelho

                    , '#FF4500' // Laranja
                    , '#ff0066' // Rosa choque
                    , '#4B0082' // Roxo
                    , '#808000' // Verde oliva
                    , '#800000' // Marrom
                    , '#2F4F4F' // Cinza escuro
                    , '#006400' // Verde escuro
                    , '#FFA500' // Amarelo quemado
                    ";

        } else {
            $cor = "
                      '#00BFFF' // Azul claro
                    , '#55BF3B' // Verde
                    , '#FFD700' // Amarelo
                    , '#FF6A6A' // Vermelho claro

                    , '#eeaaee' // Rosa claro
                    , '#aaeeee' // Cinza claro

                    , '#7798BF' // Roxo claro
                    , '#DDDF0D' // Verde claro
                    , '#7CCD7C' // Amarelo um pouco mais claro
                    , '#DF5353' // Vermelho rosa escuro

                //    , '#0000FF' // Azul
                    , '#008000' // Verde
                //    , '#FFD700' // Gold
                    , '#CD0000' // Vermelho

                    , '#FF4500' // Laranja
                    , '#ff0066' // Rosa choque
                    , '#4B0082' // Roxo
                    , '#808000' // Verde oliva
                    , '#800000' // Marrom
                    , '#2F4F4F' // Cinza escuro
                    , '#006400' // Verde escuro
                    , '#FFA500' // Amarelo quemado
                ";
        }

	$strValores = trim($strValores,",");
	$array_valores = str_replace('][', '],[', $array_valores);
	?>
		<script>
		jQuery(document).ready(function() {

                // Radialize the colors
                Highcharts.getOptions().colors = Highcharts.map(
                        [
                           <?php echo $cor ?>
                    ]
                        , function(color) {
                    return {
                        radialGradient: { cx: 0.5, cy: 0.3, r: 0.7 },
                        stops: [
                            [0, color],
                            [1, Highcharts.Color(color).brighten(-0.3).get('rgb')] // darken
                        ]
                    };
                });


		jQuery('#<?=$nomeUnico?>').highcharts({
            lang: {
                printChart: 'Imprimir',
                downloadPDF: 'Exportar em PDF',
                downloadJPEG: 'Exportar em JPG',
                downloadPNG: 'Exportar em PNG',
                downloadSVG: 'Exportar em SVG',
                decimalPoint: ',',
                thousandsSep: '.'
            },
            credits: {
                enabled: false
            },
            chart: {
                plotBackgroundColor: null,
                plotBorderWidth: null,
                plotShadow: true,
                backgroundColor:'rgba(255, 255, 255, 0.0)'
            },
            title: {
                text: '<?=$titulo?>',
                style: {
         			color: '#C7C5C5'
      			}
            },
            tooltip: {
                 pointFormat: '<?=$formatoDica?>'
            },

            //habilitar o botão de salvar como imagem, pdf, etc
            exporting: {
        	 enabled: true
			},
			//estilo legenda
			legend: {
			   layout: '<?php echo $aLegendaConfig['layout']; ?>',
			   align:  '<?php echo $aLegendaConfig['align']; ?>',
			   itemStyle: {
				   paddingBottom: '10px',
				   color: '#7F7F7F'
			   }
		   },
            plotOptions: {
                pie: {
                    point: {
                        events: {
                            legendItemClick: function() {
                                    return <?php echo $legendaClique ?>;
                            }
                        }
                    },
                    cursor: '<?php echo $cursor ?>',
                    borderWidth: 0, // Borda dos pedaços da pizza
                    allowPointSelect: true,
                    dataLabels: {
                        enabled: true,
                        color: '#7F7F7F',
//                        connectorColor: 'white',
						//<b>{point.name}</b>: para colocar o nome na legenda
                        <?php if ($formatoValores) { ?>
                            format: '<?=$formatoValores ?>'
                        <?php } else { ?>
                            formatter: function () { return number_format(this.y, 0, ',', '.') + ' (' + number_format(this.percentage, 2, ',') + ') %'; }
                        <?php } ?>
                    },
					showInLegend: '<?=$mostrarLegenda ?>'
                },
                series: {
                    cursor: 'pointer'
                    ,
                    events: {
//                        legendItemClick:  false
                        <?if ($mostrapopudetalhes): ?>
                            click: function(event){
                                var arrValores = new Array (<?=$strValores?>);
                               	<?php echo $caminhopopupdetalhes; ?>, arrValores[event.point.x]);
                            }
                        <? endif ?>
                    }
    			}

            },
            series: [{
                type: 'pie',
                name: '<?=$nomeSerie?>',
                data: [ <?PHP echo $array_valores; ?> ]

            }]
        });
    });

    </script>
 	<div id="<?=$nomeUnico?>" ></div>
	<?
}

function geraGraficoXY(array $eixoX, array $eixoY, array $eixoYMeta = array() , $nomeUnico , $titulo, $altura,$nomeSerie)
{
	foreach ($eixoX as $item)
	{
		$eixox .= "'".$item."',";
	}

	$eixox = trim($eixox,",");
	$eixoy = implode(',', $eixoY);
	$eixoymeta = implode(',', $eixoYMeta);

	?>
		<script>

		 jQuery(document).ready(function() {

		Highcharts.setOptions({
		lang: {
			numericSymbols: null
		}
		});

		jQuery('#<?=$nomeUnico?>').highcharts({
	 		lang:{
	       	 numericSymbols: null
	 		},
            chart: {
                type: 'line'
            },
            title: {
                text: '<?=$titulo?>',
            },
            xAxis: {
                categories: [<?=$eixox?>]
            },
            yAxis: {
                title: {
                    text: ''
                },

                <?php if( strstr($eixox, ',') && strstr($eixoy, ',') ) {  ?>

                tickPositioner: function () {
                var positions = [],
                    tick = Math.floor(this.dataMin),
                    increment = Math.ceil((this.dataMax - this.dataMin) / 6);

                for (; tick - increment <= this.dataMax; tick += increment) {
                    positions.push(tick);
                }
                return positions;
            	}
            	<?php } ?>

            },
            tooltip: {
                valueSuffix: ''
            },
			//habilitar o botão de salvar como imagem, pdf, etc
            exporting: {
				enabled: false
			},
			credits: {
				enabled: false
			},
            legend: {
                layout: 'vertical',
                align: 'right',
                verticalAlign: 'middle',
                borderWidth: 0
            },
            series: [{
            	name: '<?=$nomeSerie?>',
                data: [<?=$eixoy?>],
                color: 'blue'
            }

            <?if ($eixoYMeta){?>

            ,
            {
            	name: 'Meta',
                data: [<?=$eixoymeta?>],
                color: 'red'
            }

            <? } ?>


            ]
        });
    });
    </script>
 	<div style="height:<?=$altura?>px" id="<?=$nomeUnico?>" ></div>
	<?
}

function geraGraficoLinha(array $dados, $eixoX, $nomeUnico , $titulo, $altura, $formatoValores=null)
{
    $eixox = simec_json_encode($eixoX);
    $dadosJson = simec_json_encode($dados);

	?>
		<script>

		 jQuery(document).ready(function() {

		Highcharts.setOptions({
		lang: {
            numericSymbols: null,
            decimalPoint: ',',
            thousandsSep: '.'
		}
		});

		jQuery('#<?=$nomeUnico?>').highcharts({
	 		lang:{
	       	 numericSymbols: null
	 		},
            chart: {
                type: 'line'
            },
            title: {
                text: '<?=$titulo?>',
            },
            xAxis: {
            	categories: <?=$eixox?>,
                labels: {
                     <?php if(isset($formatoValores['rotation'])){ ?>
                        rotation: -45,
                        align: 'right'
                     <?php } ?>
                }
            },
            yAxis: {
                title: {
                    text: ''
                },

                <?php // if( strstr($eixox, ',') && strstr($eixoy, ',') ) {  ?>
                <?php if( true ) {  ?>

                tickPositioner: function () {
                var positions = [],
                    tick = Math.floor(this.dataMin),
                    increment = Math.ceil((this.dataMax - this.dataMin) / 6);

                for (; tick - increment <= this.dataMax; tick += increment) {
                    positions.push(tick);
                }
                return positions;
            	}
            	<?php } ?>

            },
            tooltip: {
                <?php if ($formatoValores['tooltip']) {
                    echo $formatoValores['tooltip'];
                } else {
                    echo "valueSuffix: ''";
                } ?>
            },

			//habilitar o botão de salvar como imagem, pdf, etc
            exporting: {
				enabled: false
			},
			credits: {
				enabled: false
			},
        <?php if(!$formatoValores['alinhamento']) { ?>
        legend: {
                layout: 'vertical',
                align: 'right',
                verticalAlign: 'middle',
                borderWidth: 0
            },
        <?php } ?>
            series: <?php echo $dadosJson; ?>
        });
    });
    </script>
 	<div style="height:<?=$altura?>px" id="<?=$nomeUnico?>" ></div>
	<?
}

function geraGraficoBarra(array $dados,$nomeUnico,$titulo = '',$formatoDica = '',$formatoValores = '',$nomeSerie = '',$caminhopopupdetalhes="",$largurapopupdetalhes="",$alturapopupdetalhes="",$mostrarLegenda = false, $altura = '600')
{
	foreach ($dados as $dado)
	{
		$data      .= $dado['valor'].", ";
		$categoria .= "'".$dado['descricao']."',";
	}
	$strValores = trim($categoria,",");

	?>
<script>

jQuery(function () {

    jQuery('#<?php echo $nomeUnico; ?>').highcharts({
    	chart: {
            type: 'bar',
            plotBackgroundColor: null,
            // margin: [ 50, 50, 100, 80]
            plotBorderWidth: null,
            plotShadow: true,
            backgroundColor:'rgba(255, 255, 255, 0.0)',
        },

//        chart: {
//        },

        title: {
            text: '',
        },
        //habilitar o botão de salvar como imagem, pdf, etc
        exporting: {
            enabled: false
        },
		credits: {
			enabled: false
		},
        xAxis: {
            categories: [<?php echo $categoria; ?>],
            labels: {
                style: {
                    color: '#fff',
                }
            }
        },
        yAxis: {
            title: {
                text: ''
            },
            labels: {
                style: {
                    color: '#fff',
                }
            }
        },
        legend: {
            enabled: false
        },
        tooltip: {
            pointFormat: 'Ocorrências: <b>{point.y:.2f}</b>'
        },
        series: [{

            <?php if($caminhopopupdetalhes){ ?>
            	events: {
                    click: function(event) {
                        var arrValores = new Array (<?=$strValores?>);

                        var x = screen.width/2 - 700/2;
                        var y = screen.height/2 - 450/2;

                        var janela = window.open('<?php echo $caminhopopupdetalhes; ?>&parametro='+arrValores[event.point.x],'winpreobra','menubar=0,scrollbars=yes,width=<?=$largurapopupdetalhes?>,height=<?=$alturapopupdetalhes?>,left='+x+',top='+y);
                        janela.focus();


                    }
                },
            <?php } ?>

        	borderWidth: 0,
            name: 'Population',
            format: '{point.percentage:.2f} %',
            colorByPoint: true,
            colors: [
                     '#00BFFF' // Azul claro
                     , "#55BF3B" // Verde
                     , '#FFD700' // Amarelo
                     , '#FF6A6A' // Vermelho claro
                     , "#eeaaee" // Rosa claro
                     , "#aaeeee" // Cinza claro
                     , "#7798BF" // Roxo claro
                     , "#DDDF0D" // Verde claro
                     , '#7CCD7C' // Amarelo um pouco mais claro
                     , "#DF5353" // Vermelho rosa escuro
                 //    , '#0000FF' // Azul
                     , '#008000' // Verde
                 //    , '#FFD700' // Gold
                     , '#CD0000' // Vermelho
                     , '#FF4500' // Laranja
                     , "#ff0066" // Rosa choque
                     , '#4B0082' // Roxo
                     , '#808000' // Verde oliva
                     , '#800000' // Marrom
                     , '#2F4F4F' // Cinza escuro
                     , '#006400' // Verde escuro
                     , '#FFA500' // Amarelo quemado
              ],
            data: [<?php echo $data; ?>],
            dataLabels: {
                enabled: true,
//                rotation: -90,
                color: '#FFFFFF',
                align: 'right',
                x: 4,
                y: 15,
                style: {
                    fontSize: '9px',
                    fontFamily: 'Verdana, sans-serif',
                    textShadow: '0 0 3px black'
                }
            }
        }]
    });
});

</script>
 	<div style="height: <?php echo $altura; ?>px" id="<?=$nomeUnico?>" ></div>
	<?
}

function geraGraficoBarraAgrupado(array $dados, $categorias, $nomeUnico,$titulo = '',$formatoDica = '',$formatoValores = array(),$nomeSerie = '',$caminhopopupdetalhes="",$largurapopupdetalhes="",$alturapopupdetalhes="",$mostrarLegenda = false, $altura = '600', $cor = '#fff')
{
    $categoria = simec_json_encode($categorias);
    $dadosJson = simec_json_encode($dados);
	?>
<script>

jQuery(function () {

    jQuery('#<?php echo $nomeUnico; ?>').highcharts({
    	chart: {
            type: 'bar',
            plotBackgroundColor: null,
            // margin: [ 50, 50, 100, 80]
            plotBorderWidth: null,
            plotShadow: true,
            backgroundColor:'rgba(255, 255, 255, 0.0)',
        },

//        chart: {
//        },

        title: {
            text: '',
        },
        //habilitar o botão de salvar como imagem, pdf, etc
        exporting: {
            enabled: false
        },
		credits: {
			enabled: false
		},
        xAxis: {
            categories: <?php echo $categoria; ?>,
            labels: {
                style: {
                    color: '<?=$cor?>',
                }
                <?php if ($formatoValores['x']) {
                    echo $formatoValores['x'];
                } ?>
            }
        },
        yAxis: {
            title: {
                text: ''
            },
            labels: {
                style: {
                    color: '<?=$cor?>'
                }
                <?php if ($formatoValores['y']) {
                    echo $formatoValores['y'];
                } ?>
            }
        },
        legend: {
            backgroundColor: (Highcharts.theme && Highcharts.theme.legendBackgroundColorSolid) || 'white',
            borderColor: '#CCC',
            borderWidth: 1,
            shadow: false

        },
        tooltip: {
            <?php if ($formatoValores['tooltip']) {
                echo $formatoValores['tooltip'];
            } else {
                echo "pointFormat: 'Ocorrências: <b>{point.y:.2f}</b>',";
            } ?>
        },
		<?php if ($formatoValores['plotOptions']){ ?>
		plotOptions: {
			series: {
				stacking: 'percent'
			},
			bar: {
				dataLabels: {
					enabled: true,
					format: '{point.percentage:.2f} %',
					color: '#fff'
				}
			}
		},
		<?php } ?>
        series: <?php echo $dadosJson; ?>
    });
});

</script>
 	<div style="height: <?php echo $altura; ?>px" id="<?=$nomeUnico?>" ></div>
	<?
}

function geraGraficoColuna(array $dados,$nomeUnico,$titulo = '',$formatoDica = '',$formatoValores = array(),$nomeSerie = '',$caminhopopupdetalhes="",$largurapopupdetalhes="",$alturapopupdetalhes="",$mostrarLegenda = false, $altura = '600', $config = array(), $cor = '#fff', $exibirTotal=false)

{
	foreach ($dados as $dado)
	{
		$data      .= $dado['valor'].",";
		$categoria .= "'".$dado['descricao']."',";
        $totalGeralColuna += $dado['valor'];
	}
	$strValores = trim($categoria,",");
	$strData = trim($data,",");
	?>
<script>

jQuery(function () {

    jQuery('#<?php echo $nomeUnico; ?>').highcharts({
    	chart: {
            type: 'column',
            plotBackgroundColor: null,
            // margin: [ 50, 50, 100, 80]
            plotBorderWidth: null,
            plotShadow: true,
            backgroundColor:'rgba(255, 255, 255, 0.0)',
        },

//        chart: {
//        },

        title: {
            text: '',
        },
        //habilitar o botão de salvar como imagem, pdf, etc
        exporting: {
            enabled: false
        },
		credits: {
			enabled: false
		},
        xAxis: {
            categories: [<?php echo $categoria; ?>],
            labels: {
                <?php if(isset($config['rotation'])){ ?>
                    rotation: -45,
                    align: 'right',
                <?php } ?>
                style: {
                    color: '<?=$cor?>',
                }
                <?php if ($formatoValores['x']) {
                    echo $formatoValores['x'];
                } ?>
            }
        },
        yAxis: {
            title: {
                text: ''
            },
            labels: {
                style: {
                    color: '<?=$cor?>',
                }
                <?php if ($formatoValores['y']) {
                    echo $formatoValores['y'];
                } ?>
            }
        },
        legend: {
            enabled: false
        },
        tooltip: {
            <?php if ($formatoValores['tooltip']) {
                echo $formatoValores['tooltip'];
            } else {
                echo "pointFormat: 'Ocorrências: <b>{point.y:.2f}</b>',";
            } ?>
        },
        series: [{

            <?php if($caminhopopupdetalhes){ ?>
            	events: {
                    click: function(event) {
                        var arrValores = new Array (<?=$strValores?>);

                        var x = screen.width/2 - 700/2;
                        var y = screen.height/2 - 450/2;

                        var janela = window.open('<?php echo $caminhopopupdetalhes; ?>&parametro='+arrValores[event.point.x],'winpreobra','menubar=0,scrollbars=yes,width=<?=$largurapopupdetalhes?>,height=<?=$alturapopupdetalhes?>,left='+x+',top='+y);
                        janela.focus();


                    }
                },
            <?php } ?>

        	borderWidth: 0,
            name: 'Population',
//            format: '{point.percentage:.2f} %',
            colorByPoint: true,
            data: [<?php echo $strData; ?>],
            dataLabels: {
                //enabled: '<?php echo isset($config['dataLabels']) ? true : false; ?>',
                enabled: true,
                <?php if ($formatoValores['exibirvalor']) {
                    echo $formatoValores['exibirvalor'] . ",";
                } else {
                    echo "formatter: function () { return number_format(this.y, 0, ',', '.'); },";
                } ?>
                color: '<?=$cor?>',
                align: 'right',
//                x: 4,
//                y: 15,
                style: {
                    fontSize: '9px',
                    fontFamily: 'Verdana, sans-serif',
                    textShadow: '0 0 3px black'
                }
            }
        }]
    });
});

</script>
 	<div style="height: <?php echo $altura; ?>px" id="<?=$nomeUnico?>" ></div>
    <?
    if($exibirTotal){
        echo "<table align='center'>";
        echo "  <tr>";
        echo "      <td class='titulo_box bold'>Total: ".number_format($totalGeralColuna,0,',','.')."</td>";
        echo "  </tr>";
        echo "</table>";
    }
}

function geraGraficoColunaAgrupado(array $dados, $categorias, $nomeUnico,$titulo = '',$formatoDica = '',$formatoValores = '',$nomeSerie = '',$caminhopopupdetalhes="",$largurapopupdetalhes="",$alturapopupdetalhes="",$mostrarLegenda = false, $altura = '600', $stacking = true, $cor='#fff')
{
    $categoria = simec_json_encode($categorias);
    $dadosJson = simec_json_encode($dados);
	$strValores = str_replace(array('[', ']'), "", $categoria);
	
	//ver($categoria, $dadosJson);

    if($formatoValores['corX']){
        $corX = $formatoValores['corX'];
    }else{
        $corX = $cor;
    }
    ?>
<script>

jQuery(function () {

    jQuery('#<?php echo $nomeUnico; ?>').highcharts({
        chart: {
            type: 'column',
            plotBackgroundColor: null,
            plotBorderWidth: null,
            plotShadow: true,
            backgroundColor:'rgba(255, 255, 255, 0.0)',
        },
        title: {
            text: '',
        },
        //habilitar o botão de salvar como imagem, pdf, etc
        exporting: {
            enabled: false
        },
		credits: {
			enabled: false
		},
        xAxis: {
            categories: <?php echo $categoria; ?>,
            labels: {
                enable: true,
                style: {
                    color: '<?=$corX?>',
                }
                <?php if ($formatoValores['x']) {
                    echo $formatoValores['x'];
                } ?>
            }
        },
        yAxis: {
        	min: 0,
            title: {
                text: ''
            },
            labels: {
                enable: false,
                style: {
                    color: '<?=$cor?>',
                }
                <?php if ($formatoValores['y']) {
                    echo $formatoValores['y'];
                } ?>
            },
            stackLabels: {
                enabled: <?php if($formatoValores['ocultartotal']){ echo 'false'; }else{ echo 'true';} ?>,
                style: {
            	    fontSize: '9px',
                    color: (Highcharts.theme && Highcharts.theme.textColor) || '<?=$cor?>'
                }
                , formatter: function () { return number_format(this.total, 0, ',', '.'); }
            }
        },
        legend: {
//            align: 'right',
//            x: -70,
//            verticalAlign: 'bottom',
//            y: 50,
//            floating: true,
            backgroundColor: (Highcharts.theme && Highcharts.theme.legendBackgroundColorSolid) || 'white',
            borderColor: '#CCC',
            borderWidth: 1,
            shadow: false
        },
        plotOptions: {
            column: {
            <?php  if($stacking){ echo "stacking: 'normal',"; } ?>
                dataLabels: {
                    enabled: <?php if($formatoValores['exibirvalor']){ echo 'true'; }else{ echo 'false';} ?>,
                    color: (Highcharts.theme && Highcharts.theme.dataLabelsColor) || 'white'
                    <?php if($formatoValores['exibirvalor']){
                        echo $formatoValores['exibirvalor'];
                    }?>
                }
            }
			<?if ($caminhopopupdetalhes): ?>
			, series: {
				cursor: 'pointer',
				events: {
						click: function(event){
							var arrValores = new Array (<?=$strValores?>);

							var x = screen.width/2 - 700/2;
							var y = screen.height/2 - 450/2;

							var janela = window.open('<?=$caminhopopupdetalhes?>&parametro='+arrValores[event.point.x],'winDetalhe','menubar=0,scrollbars=yes,width=<?=$largurapopupdetalhes?>,height=<?=$alturapopupdetalhes?>,left='+x+',top='+y);
							janela.focus();
						}
				}
			}
			<? endif ?>
        },
        tooltip: {
            <?php if ($formatoValores['tooltip']) {
                echo $formatoValores['tooltip'];
            } else {
                echo "pointFormat: 'Ocorrências: <b>{point.y:.2f}</b>',";
            } ?>
        },
        series: <?php echo $dadosJson; ?>
    });
});

</script>
    <div style="height: <?php echo $altura; ?>px" id="<?=$nomeUnico?>" ></div>
    <?
}

function geraGraficoDonut($dados, $categorias, $nomeUnico, $formatoValores, $altura = '300')
{
    $categoria = simec_json_encode($categorias);
    $dadosJson = simec_json_encode($dados);

//    ver($categoria, $dadosJson);
    ?>
<script>

jQuery(function () {

	var colors = Highcharts.getOptions().colors,
    categories = <?php echo $categoria; ?>,
    name = '',
    data = <?php echo $dadosJson; ?>;


// Build the data arrays
var browserData = [];
var versionsData = [];
for (var i = 0; i < data.length; i++) {

    // add browser data
    browserData.push({
        name: categories[i],
        y: data[i].y,
//        color: data[i].color
    });

    // add version data
    for (var j = 0; j < data[i].drilldown.data.length; j++) {
        var brightness = 0.2 - (j / data[i].drilldown.data.length) / 5 ;
        versionsData.push({
            name: data[i].drilldown.categories[j],
            y: data[i].drilldown.data[j],
            color: Highcharts.Color(data[i].color).brighten(-0.4).get('rgb')
        });
    }
}

// Create the chart
jQuery('#<?php echo $nomeUnico; ?>').highcharts({
    chart: {
        type: 'pie',
        backgroundColor:'rgba(255, 255, 255, 0.0)'
    },
    credits: {
        enabled: false
    },
    title: {
        text: ''
    },
    yAxis: {
        title: {
            text: ''
        }
    },
    plotOptions: {
        pie: {
            dataLabels: {
                enabled: true
            },
            showInLegend: true,
            point: {
                events: {
                    legendItemClick: function() {
                            return false;
                    }
                }
            },
        }
    },
    tooltip: {
//        valueSuffix: '%',
        <?php if ($formatoValores['tooltip']) {
            echo $formatoValores['tooltip'];
        } else {
//            echo "pointFormat: 'Ocorrências: <b> {point.y} ({point.percentage:.2f}%)</b>',";
            echo "formatter: function() {
                return '<b>' + this.point.name + '</b><br />Ocorrências: <b>' + number_format(this.y, 0, ',', '.') + '</b><br />' + 'Percentual: <b>'+ number_format(this.point.percentage, 2, ',', '.') + '%</b>';
            },";
        } ?>
    },
    exporting: {
        enabled: false
    },
	credits: {
		enabled: false
	},
    //estilo legenda
    legend: {
        enabled: true,
        itemStyle: {
            paddingBottom: '10px',
            color: 'white'
        }
    },
    series: [{
        name: 'Browsers',
        data: browserData,
        size: '60%',
        dataLabels: {
            formatter: function() {
                //return this.y > 5 ? number_format(this.point.percentage, 2, ',', '.') + '%' : null;
                return this.y > 5 ? number_format(this.y, 0, ',', '.') : null;
            },
            color: 'white',
            distance: -30
        }
    }, {
        name: 'Versions',
        data: versionsData,
        size: '80%',
        innerSize: '60%',
        dataLabels: {
            distance: 15,
            color: 'white',
            formatter: function() {
                return this.y > 1 ? this.point.name +': '+ this.point.y : null;
            }
        }
    }]
});

});

</script>
    <div style="height: <?php echo $altura; ?>px" id="<?=$nomeUnico?>" ></div>
    <?
}

/*
 * Função que agrupa os dados de uma consulta para serem usados em gráficos agrupados.
 * O agrupamento obedece ao formato exigido pelo componente de gráfico.
 *
 * @param array $dados - Dados carregados do banco
 * @param string $campo_divisao - Nome do campo que representará as divisões (eixo x)
 * @param string $campo_grupo - Nome do campo que representará os grupos dentro da divisão
 * @param string $campo_valor - Nome do campo que representará o valor de cada grupo
 *
 * @return array - Retorna um array com duas informações: dados e divisoes, sendo o primeiro com os dados agrupados e o último com todas as divisões únicas
 * @author Orion Teles de Mesquita
 */
function agruparDadosGrafico($dados, $campo_divisao, $campo_grupo, $campo_valor)
{
    $divisoes = array();
    $grupos = array();
    $dadosAgrupados = array();

    foreach ($dados as $dado) {
        $dadosAgrupados[tirar_acentos($dado[$campo_divisao])][tirar_acentos($dado[$campo_grupo])] = (float) $dado[$campo_valor];
        $divisoes[$dado[$campo_divisao]] = tirar_acentos($dado[$campo_divisao]);
        $grupos[$dado[$campo_grupo]] = tirar_acentos($dado[$campo_grupo]);
    }

    $dadosFinais = array();
    foreach ($grupos as $grupo) {
        foreach ($divisoes as $divisao) {
            if(!isset($dadosAgrupados[$divisao][$grupo])){
                $dadosFinais[$grupo][] = 0;
            } else {
                $dadosFinais[$grupo][] = $dadosAgrupados[$divisao][$grupo];
            }
        }
    }
    $aDados = array();
    foreach ($dadosFinais as $divisao => $aDado) {
        $aDados[] = array('name' => $divisao, 'data'=>$aDado);
    }
    return array('dados'=>$aDados, 'divisoes'=>array_values($divisoes));
}

function montaTabelaVistoriaObras($tipofonte, $link, $exibeTitulo='S'){

	global $db;

	$parametroCockpitColTotal="4, '50,55', 1";
	$parametroCockpitCol1="4, '50', 1";
	$parametroCockpitCol2="4, '55', 1";

	$titulo = 'Vistoria';
	$subtitulo = 'Situação quanto ao nível de preenchimento';

	switch($tipofonte){
		case 1: //PROINFÂNCIA
			$titulo = 'Vistoria - Instituição';
			$subtitulo = 'Situação quanto ao nível de preenchimento';
			$coluna = "CASE WHEN o.preid IS NULL AND o.tooid IN (2,4) THEN COUNT(0) END AS coluna1, CASE WHEN o.preid IS NOT NULL THEN COUNT(0) END AS coluna2, ";
			$where = "AND e.prfid IN (41) AND ((o.preid IS NULL AND o.tooid IN (2,4)) OR o.preid IS NOT NULL)";
			$legenda1 = "Pré-PAC";
			$legenda2 = "PAC2";
			$parametroColTotal="3, 1, 41, '1,2,4', ''";
			$parametroCol1="3, 1, 41, '2,4', ''";
			$parametroCol2="3, 1, 41, '1', ''";

            $parametroCockpitColTotal="4, '41', '1,2,4'";
            $parametroCockpitCol1="4, '41', '2,4'";
            $parametroCockpitCol2="4, '41', '1'";
            break;
		case 2: //QUADRAS E COBERTURAS
			$coluna = "CASE WHEN e.prfid in (50) then 1 else 0 end as coluna1, CASE WHEN e.prfid in (55) then 1 else 0 end as coluna2,";
			$where = "AND e.prfid IN (50,55) AND o.tooid IN (1)";
			$legenda1 = "Construção";
			$legenda2 = "Cobertura";
			$parametroColTotal="3, 1, '50,55', 1, ''";
			$parametroCol1="3, 1, '50', 1, ''";
			$parametroCol2="3, 1, '55', 1, ''";
		break;
		case 3: //BRASIL PROFISSIONALIZADO
			$coluna = "0 as coluna1, 0 as coluna2,";
			$where = "AND e.prfid in (40)";
			$legenda1 = "";
			$legenda2 = "";
			$parametroColTotal="3, 1, 40, '', ''";
			$parametroCol1="3, 1, 40, '', ''";
			$parametroCol2="3, 1, 40, '', ''";
		break;
		case 4: //Vistoria
			$titulo = 'Vistoria - Empresa';
			$subtitulo = 'Situação quanto ao nível de preenchimento da empresa contratada pelo FNDE';

			$coluna = "CASE WHEN o.tooid in (2,4) THEN 1 ELSE 0 END AS coluna1, CASE WHEN o.tooid in (1) THEN 1 ELSE 0 END AS coluna2,";
            $where = "AND e.prfid IN (41) AND o.tooid IN (1,2,4) and (d.esdid = 691 and o.obrpercentultvistoria > 80) ";
            $legenda1 = "Pré-PAC";
            $legenda2 = "PAC2";
            $parametroColTotal="3, 1, 41, '1,2,4', ''";
            $parametroCol1="3, 1, 41, '2,4', ''";
            $parametroCol2="3, 1, 41, '1', ''";

            $parametroCockpitColTotal="4, '50,55', 2";
            $parametroCockpitCol1="4, '50', 2";
            $parametroCockpitCol2="4, '55', 2";

            $sql = "SELECT
                        nivelpreenchimento,
                        sum(coluna1) AS coluna1,
                        sum(coluna2) AS coluna2,
                        sum(contador) AS total,
                        corpreenchimento::text
                    FROM (
                        SELECT
                            $coluna
                            -- CASE WHEN o.tooid in (2,4) THEN 1 ELSE 0 END AS coluna1, CASE WHEN o.tooid in (1) THEN 1 ELSE 0 END AS coluna2,
                            CASE
                                WHEN d.esdid IN (691) THEN
                                    (CASE
                                        WHEN COALESCE(o.obrpercentultvistoria,0) > 80 THEN '01 - Paralisada (> 80%)'
                                        WHEN COALESCE(o.obrpercentultvistoria,0) <= 80 THEN '01 - Paralisada (0% a 80%)'
                                    END)
                                WHEN d.esdid = 693 THEN '4 - Obras concluídas'
                                ELSE '5 - Não se aplica'
                            END AS nivelpreenchimento,
                            '#E95646'::text AS corpreenchimento,
                            1 AS CONTADOR
                        FROM obras2.obras o
                        INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
                        INNER JOIN workflow.documento d ON d.docid = o.docid
                        WHERE o.obrstatus = 'A'
                        AND e.orgid=3
                        AND d.esdid <> 770 --Etapa Concluída
                        AND o.obridpai IS NULL
                        $where
                    ) as FOO
                    GROUP BY nivelpreenchimento, corpreenchimento
                    ORDER BY 1";

            $sql = "SELECT
						nivelpreenchimento,
						sum(coluna1) AS coluna1,
						sum(coluna2) AS coluna2,
						sum(contador) AS total,
						corpreenchimento::text
					FROM (
						SELECT
							CASE WHEN o.tooid in (2,4) THEN 1 ELSE 0 END AS coluna1,
							CASE WHEN o.tooid in (1) THEN 1 ELSE 0 END AS coluna2,
							ed.esddsc as nivelpreenchimento,
							'#E95646'::text AS corpreenchimento,
							1 AS CONTADOR
						FROM
							obras2.supervisaoempresa se
							JOIN obras2.obras o ON o.empid = se.empid
							JOIN entidade.endereco en ON en.endid = o.endid
							JOIN territorios.municipio t ON t.muncod = en.muncod
							JOIN workflow.documento		   d ON d.docid = se.docid
							JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
							JOIN obras2.supervisao_os os ON os.sosid = se.sosid AND os.sosstatus = 'A'
							JOIN seguranca.usuario u ON u.usucpf = se.usucpf
							JOIN entidade.entidade e ON e.entid = se.entidvistoriador
						WHERE se.suestatus = 'A'
					) as foo
					GROUP BY nivelpreenchimento, corpreenchimento";
// ver($sql,d);
		break;
	}
	$sql = isset($sql) ? $sql : "SELECT
				nivelpreenchimento,
				sum(coluna1) AS coluna1,
				sum(coluna2) AS coluna2,
				sum(coluna1) + sum(coluna2) AS total,
				corpreenchimento
			FROM (
				SELECT
					$coluna
					CASE
						WHEN d.esdid IN (690, 691) THEN
							(CASE
								WHEN DATE_PART('days', NOW() - o.obrdtultvistoria) < 45 THEN '1 - Obras atualizadas há menos de 45 dias atrás'
								WHEN DATE_PART('days', NOW() - o.obrdtultvistoria) BETWEEN 45  AND 60 THEN '2 - Obras atualizadas entre 45 e 60 dias'
								WHEN DATE_PART('days', NOW() - o.obrdtultvistoria) > 60 THEN '3 - Obras atualizadas há mais de 60 dias'
								ELSE '6 - Obras sem vistoria'
							END)
						WHEN d.esdid = 693 THEN '4 - Obras concluídas'
						ELSE '5 - Não se aplica'
					END AS nivelpreenchimento,
					CASE
						WHEN d.esdid IN (690, 691) THEN
							(CASE
								WHEN DATE_PART('days', NOW() - o.obrdtultvistoria) < 45 THEN '#80BC44'
								WHEN DATE_PART('days', NOW() - o.obrdtultvistoria) BETWEEN 45 AND 60 THEN '#FFC211'
								WHEN DATE_PART('days', NOW() - o.obrdtultvistoria) > 60 THEN '#E95646'
								ELSE '#000'
							END)
						WHEN d.esdid = 693 THEN '#2B86EE'
						ELSE '#888888'
					END AS corpreenchimento
				FROM obras2.obras o
				INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
				INNER JOIN workflow.documento d ON d.docid = o.docid
				WHERE o.obrstatus = 'A'
				AND e.orgid=3
				AND (
				  (DATE_PART('days', NOW() - o.obrdtultvistoria) > 60 AND d.esdid IN (690, 691)) OR
				  (d.esdid <> 770) --Etapa Concluída
				)
				AND d.esdid <> 770 --Etapa Concluída
				AND o.obridpai IS NULL
				$where
				GROUP BY o.preid, o.tooid, d.esdid, o.obrdtultvistoria, e.prfid
			) as FOO
			GROUP BY nivelpreenchimento, corpreenchimento
			ORDER BY 1";
 //ver($sql,d);
	$arrVistoria = $db->carregar($sql);
	$totcoluna1 = 0;
	$totcoluna2 = 0;
	$tottotal = 0;


	if($exibeTitulo=='S'){
	?>
		<div>
			<img style="float:left" src="../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
			<div style="float:left" class="titulo_box" ><?php echo $titulo; ?><br/><span class="subtitulo_box" ><?php echo $subtitulo; ?></span></div>
		</div>
	<?php
	}
	?>
	<div style="clear:both;" >
		<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
			<tr class="fundo_tr_wap">
				<td class="fundo_td_wap center bold" >Preenchimento</td>
				<?if($legenda1):?>
					<td class="fundo_td_wap center bold" ><?=$legenda1?></td>
					<td class="fundo_td_wap center bold" ><?=$legenda2?></td>
				<?endif;?>
				<td class="fundo_td_wap center bold" >Total</td>
			</tr>
			<?php
			if($arrVistoria[0]):
				foreach($arrVistoria as $vis) :
					if($link=='S'){ ?>

						<tr class="fundo_tr_wap">
							<td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap link" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitColTotal?>, '<?=$vis['nivelpreenchimento'] ?>');"><?=$vis['nivelpreenchimento']?></td>
							<?if($legenda1):?>
								<td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap numero link" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitCol1?>, '<?=$vis['nivelpreenchimento'] ?>');" ><?=$vis['coluna1'] ?></td>
								<td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap numero link" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitCol2?>, '<?=$vis['nivelpreenchimento'] ?>');" ><?=$vis['coluna2'] ?></td>
							<?endif;?>
							<td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap numero link" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitColTotal?>, '<?=$vis['nivelpreenchimento'] ?>');" ><?=$vis['total'] ?></td>
						</tr>
					<?}else{?>
						<tr class="fundo_tr_wap">
							<td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap"><?=str_replace(array("1 - ","2 - ","3 - ","4 -", "5 - "),"",$vis['nivelpreenchimento'])?></td>
							<?if($legenda1):?>
								<td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap numero" ><?=$vis['coluna1'] ?></td>
								<td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap numero" ><?=$vis['coluna2'] ?></td>
							<?endif;?>
							<td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="fundo_td_wap numero" ><?=$vis['total'] ?></td>
						</tr>
					<?
					}
					if($legenda1){
						$totcoluna1 += $vis['coluna1'];
						$totcoluna2 += $vis['coluna2'];
					}
					$tottotal += $vis['total'];
					endforeach;
			endif;
			?>
			<tr class="fundo_tr_wap">
				<td class="fundo_td_wap bold">Total</td>
				<?if($legenda1):?>
					<td class="fundo_td_wap numero"><?=$totcoluna1 ?></td>
					<td class="fundo_td_wap numero"><?=$totcoluna2 ?></td>
				<?endif;?>
				<td class="fundo_td_wap numero"><?=$tottotal ?></td>
			</tr>
		</table>
	</div>
<?php
}

function montaTabelaSituacaoObras($tipofonte, $link, $exibeTitulo='S'){
	global $db;

	switch($tipofonte){
		case 1: //PROINFÂNCIA
			$coluna = "CASE WHEN o.preid IS NULL AND o.tooid IN (2,4) THEN COUNT(0) END AS coluna1, CASE WHEN e.prfid IN (41) AND o.tooid IN (1) THEN COUNT(0) END AS coluna2, CASE WHEN e.prfid IN (41) AND o.tooid IN (4) AND o.obranoconvenio IS NULL THEN COUNT(0) END AS coluna3, ";
			$where = "AND e.prfid IN (41) AND ((o.preid IS NULL AND o.tooid IN (2,4)) OR o.preid IS NOT NULL)";
			$legenda1 = "Pré-PAC";
			$legenda2 = "PAC2";
			$parametroColTotal="3, 2, 41, '1,2,4'";
			$parametroCol1="3, 2, 41, '2,4'";
			$parametroCol2="3, 2, 41, '1'";
			$parametroCol3="3, 2, 41, '4'";
			$mapaCol1="prepac";
			$mapaCol2="pac";
			$mapaCol3="emenda";
			$parametroCockpitColTotal="1, 41, '1,2,4'";
			$parametroCockpitCol1="1, 41, '2,4'";
			$parametroCockpitCol2="1, 41, '1'";
			$parametroCockpitCol3="1, 41, '4'";
			$union = "UNION ALL
						SELECT '00-Aguardando empenho' AS situacaoobra, CASE WHEN pto.tooid IN (2,4) THEN COUNT(0) END AS coluna1, CASE WHEN pto.tooid IN (1) THEN COUNT(0) END AS coluna2, 0 AS coluna3, 0 as esdid, 0 as esdordem
						FROM obras.preobra pre
						INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid
						WHERE pre.prestatus = 'A'
						AND pto.ptoclassificacaoobra = 'P'
						AND doc.esdid IN (228)
						AND pre.obrid IS NULL
						GROUP BY situacaoobra, pto.tooid";
		break;
		case 2: //QUADRAS E COBERTURAS
			$coluna = "CASE WHEN e.prfid in (50) THEN COUNT(0) END AS coluna1, CASE WHEN e.prfid in (55) THEN COUNT(0) END AS coluna2, 0 AS coluna3, ";
			//$where = "AND e.prfid IN (50,55) AND o.tooid IN (1)";
			$where = "AND e.prfid IN (50,55) AND o.preid is not null";
			$legenda1 = "Construção";
			$legenda2 = "Cobertura";
			$parametroColTotal="3, 2, '50,55', 1";
			$parametroCol1="3, 2, '50', 1";
			$parametroCol2="3, 2, '55', 1";
			$mapaCol1="pac";
			$mapaCol2="pac";
			$parametroCockpitColTotal="1, '50,55', 1";
			$parametroCockpitCol1="1, '50', 1";
			$parametroCockpitCol2="1, '55', 1";
			$union = "";
		break;
		case 3: //BRASIL PROFISSIONALIZADO
		break;
	}

	$sql = "SELECT
				situacaoobra,
				COALESCE(SUM(coluna1),0) AS coluna1,
				COALESCE(SUM(coluna2),0) AS coluna2,
				COALESCE(SUM(coluna3),0) AS coluna3,
				esdid,
				esdordem
			FROM (
				SELECT	CASE esd.esdid
						WHEN 693 THEN
							CASE
								WHEN DATE_PART('year', o.obrdtultvistoria) <= 2010 THEN '01-'||esd.esddsc||'s até 2010'
								WHEN DATE_PART('year', o.obrdtultvistoria) > 2010 THEN '02-'||esd.esddsc||'s após 2010'
							END
						WHEN 690 THEN --Execução
							CASE
								WHEN COALESCE(o.obrpercentultvistoria,0) > 80 THEN '03-'||esd.esddsc||' (> 80%)'
								WHEN COALESCE(o.obrpercentultvistoria,0) <= 80 THEN '04-'||esd.esddsc||' (0% a 80%)'
							END
						WHEN 768 THEN '05-'||esd.esddsc --Em Reformulação
						WHEN 763 THEN '06-'||esd.esddsc --Licitação
						WHEN 691 THEN '07-'||esd.esddsc --Paralisada
						WHEN 769 THEN '08-'||esd.esddsc --Obra Cancelada
						WHEN 689 THEN '09-'||esd.esddsc --Planejamento pelo proponente
						WHEN 771 THEN '10-'||esd.esddsc --Aguardando registro de preços
						ELSE esd.esddsc
					END AS situacaoobra,
					$coluna
					esd.esdid,
					esd.esdordem
				from obras2.obras o
				inner join obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
				inner join workflow.documento d ON d.docid = o.docid
				inner join workflow.estadodocumento esd ON esd.esdid = d.esdid
				where o.obrstatus = 'A'
				and e.orgid=3
				and d.esdid NOT IN (770) --Etapa Concluída
				and o.obridpai is null
				$where
				group by situacaoobra, esd.esdid, esd.esdordem, e.prfid, o.PREID, o.TOOID, o.obranoconvenio
			$union
			) AS FOO
			group by esdordem, situacaoobra, esdid
			order by situacaoobra, esdordem";
 	//ver($sql,d);
	$arrSituacao = $db->carregar( $sql );
	$totcoluna1 = 0;
	$totcoluna2 = 0;
	$totcoluna3 = 0;
	$tottotal = 0;
	if($exibeTitulo=='S'){
		echo "<div>";
        echo "	<img style='float:left; vertical-align:middle;' src='../imagens/icones/icons/obras.png'/>";
		echo "  <div style='float:left' class='titulo_box' >Situação das Obras<br/></div>";
		echo "</div>";
	}
    if($tipofonte==1){
	?>
        <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
            <tr class="fundo_tr_wap">
                <td class="fundo_td_wap center bold" rowspan="2">Situação</td>
                <td class="fundo_td_wap center bold" colspan="2"><?=$legenda1?></td>
                <td class="fundo_td_wap center bold" rowspan="2"><?=$legenda2?></td>
                <td class="fundo_td_wap center bold" rowspan="2">Emendas</td>
                <td class="fundo_td_wap center bold" rowspan="2">Total</td>
            </tr>
            <tr class="fundo_tr_wap">
                <td class="fundo_td_wap center bold" >Pago Integral<br />2007/2010</td>
                <td class="fundo_td_wap center bold" >Pago Pós<br />2010</td>
            </tr>
            <?php
                if($arrSituacao[0]):
                    foreach($arrSituacao as $sit) :
                        if($link=='S'){
                            ?>
                            <tr class="fundo_tr_wap" style="display: none">
                                <td class="fundo_td_wap"><img border="0" style="cursor:pointer" src="/imagens/icone_br.png" title="Exibir Mapa" onclick="abreMapa('<?=$sit['esdid']?>',1)"> <span style="cursor:pointer;" onclick="abreRelatorioObras(<?=$parametroColTotal?>, <?=$sit['esdid'] ?>);"><?=str_replace(array("01-","02-","03-","04-","05-","06-","07-","08-","09-","10-","11-","12-","00-"),"",$sit['situacaoobra'])?></span></td>
                                <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObras(<?=$parametroCol1?>, <?=$sit['esdid'] ?>);"><?=$sit['coluna1'] ?></span> <img border="0" style="cursor:pointer" src="/imagens/icone_br.png" title="Exibir Mapa" onclick="abreMapa('<?=$sit['esdid'] ?>','<?=$mapaCol1?>')"></td>
                                <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObras(<?=$parametroCol2?>, <?=$sit['esdid'] ?>);"><?=$sit['coluna2'] ?></span> <img border="0" style="cursor:pointer" src="/imagens/icone_br.png" title="Exibir Mapa" onclick="abreMapa('<?=$sit['esdid'] ?>','<?=$mapaCol2?>')"></td>
                                <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObras(<?=$parametroCol3?>, <?=$sit['esdid'] ?>);"><?=$sit['coluna3'] ?></span> <img border="0" style="cursor:pointer" src="/imagens/icone_br.png" title="Exibir Mapa" onclick="abreMapa('<?=$sit['esdid'] ?>','<?=$mapaCol3?>')"></td>
                                <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObras(<?=$parametroColTotal?>, <?=$sit['esdid'] ?>);"><?=( $sit['coluna1'] + $sit['coluna2'] + $sit['coluna3'] ) ?></span></td>
                            </tr>
                            <tr class="fundo_tr_wap">
                                <td class="fundo_td_wap"><span style="cursor:pointer;" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitColTotal?>, <?=$sit['esdid'] ?>);"><?=str_replace(array("01-","02-","03-","04-","05-","06-","07-","08-","09-","10-","11-","12-","00-"),"",$sit['situacaoobra'])?></span></td>
                                <td class="fundo_td_wap numero" >0</td>
                                <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitCol1?>, <?=$sit['esdid'] ?>);"><?=$sit['coluna1'] ?></span></td>
                                <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitCol2?>, <?=$sit['esdid'] ?>);"><?=$sit['coluna2'] ?></span></td>
                                <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitCol3?>, <?=$sit['esdid'] ?>);"><?=$sit['coluna3'] ?></span></td>
                                <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitColTotal?>, <?=$sit['esdid'] ?>);"><?=( $sit['coluna1'] + $sit['coluna2'] + $sit['coluna3'] ) ?></span></td>
                            </tr>
                        <?php
                        }else{
                        ?>
                            <tr class="fundo_tr_wap">
                                <td class="fundo_td_wap"><?=str_replace(array("01-","02-","03-","04-","05-","06-","07-","08-","09-","10-","11-","12-","00-"),"",$sit['situacaoobra'])?></td>
                                <td class="fundo_td_wap numero" >0</td>
                                <td class="fundo_td_wap numero" ><?=$sit['coluna1'] ?></td>
                                <td class="fundo_td_wap numero" ><?=$sit['coluna2'] ?></td>
                                <td class="fundo_td_wap numero" ><?=$sit['coluna3'] ?></td>
                                <td class="fundo_td_wap numero" ><?=( $sit['coluna1'] + $sit['coluna2'] + $sit['coluna3'] ) ?></td>
                            </tr>
                        <?
                        }
                        if($sit['esdordem']==0){
                            echo "<tr><td colspan='6'><hr></td></tr>";
                        }else{
                            $totcoluna1 += $sit['coluna1'];
                            $totcoluna2 += $sit['coluna2'];
                            $totcoluna3 += $sit['coluna3'];
                            $tottotal += $sit['coluna1'] + $sit['coluna2'] + $sit['coluna3'];
                        }
                        endforeach;
                endif;
                if($link=='S'){ ?>
                    <tr class="fundo_tr_wap" style="display: none">
                        <td class="fundo_td_wap bold"><img border="0" style="cursor:pointer" src="/imagens/icone_br.png" title="Exibir Mapa" onclick="abreMapa('99')"> <span style="cursor:pointer;" onclick="abreRelatorioObras(<?=parametroColTotal?>, 'T');">Total</span></td>
                        <td class="fundo_td_wap numero link" onclick="abreRelatorioObras(<?=$parametroCol1?>, 'T');"><?=$totcoluna1 ?></td>
                        <td class="fundo_td_wap numero link" onclick="abreRelatorioObras(<?=$parametroCol2?>, 'T');"><?=$totcoluna2 ?></td>
                        <td class="fundo_td_wap numero link" onclick="abreRelatorioObras(<?=$parametroCol3?>, 'T');"><?=$totcoluna3 ?></td>
                        <td class="fundo_td_wap numero link" onclick="abreRelatorioObras(<?=$parametroColTotal?>, 'T');"><?=$tottotal ?></td>
                    </tr>
                    <tr class="fundo_tr_wap">
                        <td class="fundo_td_wap bold"><span style="cursor:pointer;" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitColTotal?>, 'T');">Total</span></td>
                        <td class="fundo_td_wap numero link">0</td>
                        <td class="fundo_td_wap numero link" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitCol1?>, 'T');"><?=$totcoluna1 ?></td>
                        <td class="fundo_td_wap numero link" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitCol2?>, 'T');"><?=$totcoluna2 ?></td>
                        <td class="fundo_td_wap numero link" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitCol3?>, 'T');"><?=$totcoluna3 ?></td>
                        <td class="fundo_td_wap numero link" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitColTotal?>, 'T');"><?=$tottotal ?></td>
                    </tr>
                <?php
                }else{
                ?>
                    <tr class="fundo_tr">
                        <td class="fundo_td_wap bold">Total</td>
                        <td class="fundo_td_wap numero">0</td>
                        <td class="fundo_td_wap numero"><?=$totcoluna1 ?></td>
                        <td class="fundo_td_wap numero"><?=$totcoluna2 ?></td>
                        <td class="fundo_td_wap numero"><?=$totcoluna3 ?></td>
                        <td class="fundo_td_wap numero"><?=$tottotal ?></td>
                    </tr>
                <?php
                }
                ?>
        </table>
    <?php }else{ ?>
        <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
            <tr class="fundo_tr_wap">
                <td class="fundo_td_wap center bold">Situação</td>
                <td class="fundo_td_wap center bold" ><?=$legenda1?></td>
                <td class="fundo_td_wap center bold" ><?=$legenda2?></td>
                <td class="fundo_td_wap center bold" >Total</td>
            </tr>
            <?php
            if($arrSituacao[0]):
                foreach($arrSituacao as $sit) :
                    if($link=='S'){
                        ?>
                        <tr class="fundo_tr_wap" style="display: none">
                            <td class="fundo_td_wap"><img border="0" style="cursor:pointer" src="/imagens/icone_br.png" title="Exibir Mapa" onclick="abreMapa('<?=$sit['esdid']?>',1)"> <span style="cursor:pointer;" onclick="abreRelatorioObras(<?=$parametroColTotal?>, <?=$sit['esdid'] ?>);"><?=str_replace(array("01-","02-","03-","04-","05-","06-","07-","08-","09-","10-","11-","12-","00-"),"",$sit['situacaoobra'])?></span></td>
                            <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObras(<?=$parametroCol1?>, <?=$sit['esdid'] ?>);"><?=$sit['coluna1'] ?></span> <img border="0" style="cursor:pointer" src="/imagens/icone_br.png" title="Exibir Mapa" onclick="abreMapa('<?=$sit['esdid'] ?>','<?=$mapaCol1?>')"></td>
                            <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObras(<?=$parametroCol2?>, <?=$sit['esdid'] ?>);"><?=$sit['coluna2'] ?></span> <img border="0" style="cursor:pointer" src="/imagens/icone_br.png" title="Exibir Mapa" onclick="abreMapa('<?=$sit['esdid'] ?>','<?=$mapaCol2?>')"></td>
                            <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObras(<?=$parametroColTotal?>, <?=$sit['esdid'] ?>);"><?=( $sit['coluna1'] + $sit['coluna2'] ) ?></span></td>
                        </tr>
                        <tr class="fundo_tr_wap">
                            <td class="fundo_td_wap"><span style="cursor:pointer;" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitColTotal?>, <?=$sit['esdid'] ?>);"><?=str_replace(array("01-","02-","03-","04-","05-","06-","07-","08-","09-","10-","11-","12-","00-"),"",$sit['situacaoobra'])?></span></td>
                            <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitCol1?>, <?=$sit['esdid'] ?>);"><?=$sit['coluna1'] ?></span></td>
                            <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitCol2?>, <?=$sit['esdid'] ?>);"><?=$sit['coluna2'] ?></span></td>
                            <td class="fundo_td_wap numero" ><span style="cursor:pointer;" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitColTotal?>, <?=$sit['esdid'] ?>);"><?=( $sit['coluna1'] + $sit['coluna2'] ) ?></span></td>
                        </tr>
                    <?php
                    }else{
                        ?>
                        <tr class="fundo_tr_wap">
                            <td class="fundo_td_wap"><?=str_replace(array("01-","02-","03-","04-","05-","06-","07-","08-","09-","10-","11-","12-","00-"),"",$sit['situacaoobra'])?></td>
                            <td class="fundo_td_wap numero" ><?=$sit['coluna1'] ?></td>
                            <td class="fundo_td_wap numero" ><?=$sit['coluna2'] ?></td>
                            <td class="fundo_td_wap numero" ><?=( $sit['coluna1'] + $sit['coluna2'] ) ?></td>
                        </tr>
                    <?
                    }
                    if($sit['esdordem']==0){
                        echo "<tr><td colspan='4'><hr></td></tr>";
                    }else{
                        $totcoluna1 += $sit['coluna1'];
                        $totcoluna2 += $sit['coluna2'];
                        $tottotal += $sit['coluna1'] + $sit['coluna2'];
                    }
                endforeach;
            endif;
            if($link=='S'){ ?>
                <tr class="fundo_tr_wap" style="display: none">
                    <td class="fundo_td_wap bold"><img border="0" style="cursor:pointer" src="/imagens/icone_br.png" title="Exibir Mapa" onclick="abreMapa('99')"> <span style="cursor:pointer;" onclick="abreRelatorioObras(<?=parametroColTotal?>, 'T');">Total</span></td>
                    <td class="fundo_td_wap numero link" onclick="abreRelatorioObras(<?=$parametroCol1?>, 'T');"><?=$totcoluna1 ?></td>
                    <td class="fundo_td_wap numero link" onclick="abreRelatorioObras(<?=$parametroCol2?>, 'T');"><?=$totcoluna2 ?></td>
                    <td class="fundo_td_wap numero link" onclick="abreRelatorioObras(<?=$parametroColTotal?>, 'T');"><?=$tottotal ?></td>
                </tr>
                <tr class="fundo_tr_wap">
                    <td class="fundo_td_wap bold"><span style="cursor:pointer;" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitColTotal?>, 'T');">Total</span></td>
                    <td class="fundo_td_wap numero link" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitCol1?>, 'T');"><?=$totcoluna1 ?></td>
                    <td class="fundo_td_wap numero link" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitCol2?>, 'T');"><?=$totcoluna2 ?></td>
                    <td class="fundo_td_wap numero link" onclick="abreRelatorioObrasCockpit(<?=$parametroCockpitColTotal?>, 'T');"><?=$tottotal ?></td>
                </tr>
            <?php
            }else{
                ?>
                <tr class="fundo_tr">
                    <td class="fundo_td_wap bold">Total</td>
                    <td class="fundo_td_wap numero"><?=$totcoluna1 ?></td>
                    <td class="fundo_td_wap numero"><?=$totcoluna2 ?></td>
                    <td class="fundo_td_wap numero"><?=$tottotal ?></td>
                </tr>
            <?php
            }
            ?>
        </table>
    <?php } ?>
<?php
}

function verificarFiltroListaObras($dado, $boAtraso = false)
{
    $preid        = $boAtraso ? 'preid_atraso'        : 'preid';
    $obrid        = $boAtraso ? 'obrid_atraso'        : 'obrid';
    $predescricao = $boAtraso ? 'predescricao_atraso' : 'predescricao';
    $processo     = $boAtraso ? 'processo_atraso'     : 'processo';
    $saldo        = $boAtraso ? 'saldo_atraso'        : 'saldo';
    $estuf        = $boAtraso ? 'estuf_atraso'        : 'estuf';
    $mundescricao = $boAtraso ? 'mundescricao_atraso' : 'mundescricao';
    $mediatempo   = $boAtraso ? 'mediatempo_atraso'   : 'mediatempo';

    if ($_REQUEST['filtro_' . $preid] && false === stripos($dado['preid'], $_REQUEST['filtro_' . $preid])) {
        return false;
    }
    if ($_REQUEST['filtro_' . $obrid] && false === stripos($dado['obrid'], $_REQUEST['filtro_' . $obrid])) {
        return false;
    }
    if ($_REQUEST['filtro_' . $predescricao] && false === stripos($dado['predescricao'], $_REQUEST['filtro_' . $predescricao])) {
        return false;
    }
    if ($_REQUEST['filtro_' . $processo] && false === stripos($dado['processo'], $_REQUEST['filtro_' . $processo])) {
        return false;
    }
    if ($_REQUEST['filtro_' . $saldo] && false === stripos($dado['saldo'], $_REQUEST['filtro_' . $saldo])) {
        return false;
    }
    if ($_REQUEST['filtro_' . $estuf] && false === stripos($dado['estuf'], $_REQUEST['filtro_' . $estuf])) {
        return false;
    }
    if ($_REQUEST['filtro_' . $mundescricao] && false === stripos($dado['mundescricao'], $_REQUEST['filtro_' . $mundescricao])) {
        return false;
    }
    if ($_REQUEST['filtro_' . $mediatempo] && false === stripos($dado['mediatempo'], $_REQUEST['filtro_' . $mediatempo])) {
        return false;
    }

    return true;
}

function verificarFiltroListaObrasSituacao($dado)
{
    $obrid        = $dado['tipo'] == 'Construção' ? 'obrid_construcao'        : 'obrid';
    $obrnome      = $dado['tipo'] == 'Construção' ? 'obrnome_construcao'      : 'obrnome';
    $estuf        = $dado['tipo'] == 'Construção' ? 'estuf_construcao'        : 'estuf';
    $mundescricao = $dado['tipo'] == 'Construção' ? 'mundescricao_construcao' : 'mundescricao';

    if ($_REQUEST['filtro_' . $obrid] && false === stripos($dado['obrid'], $_REQUEST['filtro_' . $obrid])) {
        return false;
    }
    if ($_REQUEST['filtro_' . $obrnome] && false === stripos($dado['obrnome'], $_REQUEST['filtro_' . $obrnome])) {
        return false;
    }
    if ($_REQUEST['filtro_' . $estuf] && false === stripos($dado['estuf'], $_REQUEST['filtro_' . $estuf])) {
        return false;
    }
    if ($_REQUEST['filtro_' . $mundescricao] && false === stripos($dado['mundescricao'], $_REQUEST['filtro_' . $mundescricao])) {
        return false;
    }

    return true;
}

function exibirTitulo($imagem='', $titulo='', $subtitulo='')
{
    /*
     * Imagens pré-definidas (ver ícones em www/imagens/icones;icons):
     * alarm, alvo, busca, busca_preto, call, casas, chat, cone, configs, control, doc, executive, executiverel, financeiro, Refresh
     * Find, ideia, indicador, mais, mapas, mobileinfo, mundo, Next, obras, Presentation, Preview, Previous, Print, recycle, sms, voltar
     */
    if (false !== strpos($imagem, '/')) {
        $caminhoImagem = $imagem;
    } else {
        $caminhoImagem = "../imagens/icones/icons/{$imagem}.png";
    }

    ?>
    <div>
        <img style="float:left" src="<?php echo $caminhoImagem; ?>" style="vertical-align:middle;"  />
        <div style="float:left" class="titulo_box" >
            <?php echo $titulo; ?><br/>
            <?php if ($subtitulo) { ?>
                <span class="subtitulo_box" ><?php echo $subtitulo; ?></span>
            <?php } ?>
        </div>
    </div>
    <div class="clear"></div>
<?php }


function exibeRelatorioMaisMedicos()
{
	global $db;
	
	$arParam['uniid'] = true;
// 	$arParam['muncod'] = true;

	if($_GET['regcod'])
	{
		$arrWhere[] = "e.regcod = '{$_GET['regcod']}' ";
	}
	if($_GET['uniid'])
	{
		$arrWhere[] = "uni.uniid = '{$_GET['uniid']}' ";
	}
	
	$sql = "select
					mun2.estuf,
					count(distinct uni.unisigla) as qtde_instituicoes,
					count(distinct unm.muncod) as qtde_municipios,					
					count(distinct mdc.mdccpf) as qtde_medicos
					,".montaCampoCountSupervisoresTutoresMaisMedicos($arParam)."
				from
					maismedicos.universidademunicipio unm
				inner join
					maismedicos.universidade uni ON uni.uniid = unm.uniid
				inner join
					territorios.municipio mun2 ON mun2.muncod = unm.muncod
				inner join
					territorios.estado e ON e.estuf = mun2.estuf
				inner join
					maismedicos.medico mdc ON mdc.muncod = unm.muncod and mdcstatus = 'A'
				where
					uni.unistatus = 'A'
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
				group by
					mun2.estuf, uni.uniid";

	$sqlDados = "
			select 
					estuf,	
					sum(qtde_instituicoes) as qtde_instituicoes,
					sum(qtde_municipios) as qtde_municipios,
					sum(qtde_tutores) as qtde_tutores,
					sum(qtde_supervisores) as qtde_supervisores,
					sum(qtde_medicos) as qtde_medicos
			from (	
				{$sql}
			) as foo
			group by estuf";

// 	ver($sqlDados, d);
	$arrDados = $db->carregar($sqlDados);

	$sqlTotal = "select
					count(distinct uni.unisigla) as qtde_instituicoes,
					count(distinct unm.muncod) as qtde_municipios,						
					count(distinct mdc.mdccpf) as qtde_medicos
					,".montaCampoCountSupervisoresTutoresMaisMedicos(array($arParam))."
				from
					maismedicos.universidade uni
				left join
					maismedicos.universidademunicipio unm  ON uni.uniid = unm.uniid
				--left join territoriosgeo.municipio mun ON mun.muncod = uni.muncod
				inner join
					territoriosgeo.municipio mun2 ON mun2.muncod = unm.muncod
				inner join
				    territorios.estado e ON e.estuf = mun2.estuf
				inner join
				      maismedicos.medico mdc ON mdc.muncod = unm.muncod and mdcstatus = 'A'
				where
					unistatus = 'A'
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."";
	
// 	ver($sqlTotal, d);
	$arrTotal = $db->pegaLinha($sqlTotal);

	?>
	<script>
		function exibeMunicipiosRelatorioMaisMedicos(estuf)
		{
			var img = jQuery("#img_relatorio_mais_medicos_"+estuf);
			var tr = jQuery("#tr_relatorio_mais_medicos_"+estuf);
			var td = jQuery("#td_relatorio_mais_medicos_"+estuf);
			var html = td.html();

			if(img.attr("src") == "../imagens/mais.gif"){
				img.attr("src","../imagens/menos.gif");
				if(html == "Carregando..."){
					ajaxExibeRelatorioMaisMedicosPorUF(estuf);
				}
				tr.show();
			}else{
				img.attr("src","../imagens/mais.gif");
				tr.hide();
			}
		}

		function ajaxExibeRelatorioMaisMedicosPorUF(estuf)
		{
			var td = jQuery("#td_relatorio_mais_medicos_"+estuf);
			jQuery.ajax({
				type: "POST",
				url: window.location+"&requisicaoAjax=ajaxExibeRelatorioMaisMedicosPorUF&estuf="+estuf,
				async: false,
				success: function(response){
					td.html(response);
				}
			});
		}

		function exibePopUpRelatorioMaisMedicos(parametro,valor,visao,active)
		{
			if(parametro && valor){
				var url = window.location + "&requisicaoAjax=popUpMaisMedicos&" + parametro+"="+valor+"&visao="+visao+"&active="+active;
			}else{
				var url = window.location + "&requisicaoAjax=popUpMaisMedicos&visao="+visao+"&active="+active;
			}
			janela(url,1024,768,"Mais Médicos");
		}


	</script>

		<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
			<tr>
				<th>UF</th>
				<th>Instituições</th>
				<th style="width:100px;">Tutores</th>
				<th>Supervisores</th>
				<th style="width:140px;">Médicos do Programa</th>
			</tr>
			
			<?php ob_start(); ?>
			<?php $num = 0; ?>
			<?php foreach($arrDados as $d): ?>
				<tr <?php echo $num%2==0 ? "class=\"zebrado\"" : "" ?> >
					<td><img src="../imagens/mais.gif" class="link" id="img_relatorio_mais_medicos_<?php echo $d['estuf'] ?>" onclick="exibeMunicipiosRelatorioMaisMedicos('<?php echo $d['estuf'] ?>')"  > <?php echo $d['estuf'] ?> (<?php echo number_format($d['qtde_municipios'],0,",",".") ?>)</td>
					<td onclick="exibePopUpRelatorioMaisMedicos('estuf','<?php echo $d['estuf'] ?>','estado',0)" class="numero link" ><?php echo number_format($d['qtde_instituicoes'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos('estuf','<?php echo $d['estuf'] ?>','estado',1)" class="numero link" ><?php echo number_format($d['qtde_tutores'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos('estuf','<?php echo $d['estuf'] ?>','estado',2)" class="numero link" ><?php echo number_format($d['qtde_supervisores'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos('estuf','<?php echo $d['estuf'] ?>','estado',3)" class="numero link" ><?php echo number_format($d['qtde_medicos'],0,",",".") ?></td>
				</tr>
				<tr style="display:none"  id="tr_relatorio_mais_medicos_<?php echo $d['estuf'] ?>"  <?php echo $num%2==0 ? "class=\"zebrado\"" : "" ?> >
					<td id="td_relatorio_mais_medicos_<?php echo $d['estuf'] ?>" colspan="7<?php //echo count($d) ?>" >Carregando...</td>
				</tr>
				<?php
					foreach($d as $chave => $valor)
					{
						$total[$chave] += $valor;
					}
				?>
				<?php $num++; ?>
			<?php endforeach; ?>
			<?php $tabela = ob_get_contents(); ?>
			<?php ob_clean() ?>

			<?php if(!$_GET['regcod']): ?>
				<tr>
					<td class="bold" >Brasil (<?php echo number_format($total['qtde_municipios'],0,",",".") ?>)</td>
					<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',0)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_instituicoes'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',1)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_tutores'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',2)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_supervisores'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',3)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_medicos'],0,",",".") ?></td>
				</tr>
			<?php else: ?>
				<tr>
					<td class="bold" >Total (<?php echo number_format($arrTotal['qtde_municipios'],0,",",".") ?>)</td>
					<td class="numero bold " ><?php echo number_format($arrTotal['qtde_instituicoes'],0,",",".") ?></td>
					<td class="numero bold " ><?php echo number_format($arrTotal['qtde_tutores'],0,",",".") ?></td>
					<td class="numero bold " ><?php echo number_format($arrTotal['qtde_supervisores'],0,",",".") ?></td>
					<td class="numero bold " ><?php echo number_format($arrTotal['qtde_medicos'],0,",",".") ?></td>
				</tr>
			<?php endif; ?>
			<?php echo $tabela; ?>
		</table>

	<?php
// 	ver($total);

}

function ajaxExibeRelatorioMaisMedicosPorUF(){
	global $db;
	if($_GET['estuf']){
		$estuf = strtoupper($_GET['estuf']);
		$arrWhere[] = "mun2.estuf = '$estuf'";
	}
	if($_GET['uniid']){
		$uniid = trim($_GET['uniid']);
		$arrWhere[] = "uni.uniid = '$uniid'";
	}
	if($_GET['tpmid']){
		$tpmid = strtoupper($_GET['tpmid']);
		$arrWhere[] = "mun2.muncod in (select muncod from territoriosgeo.muntipomunicipio where tpmid = $tpmid)";
	}
	
// 	$params['uniid'] = true;
	$params['muncod'] = true;
	
	$sql = "select 
				muncod,
				mundescricao,	
				qtde_tutores,
				qtde_supervisores,
				sum(qtde_medicos) as qtde_medicos
			from (
				select
					mun2.muncod,
					mun2.mundescricao,
					".montaCampoCountSupervisoresTutoresMaisMedicos($params).",
					count(distinct mdc.mdcid) as qtde_medicos
				from
					maismedicos.universidademunicipio unm
				inner join
					maismedicos.universidade uni ON uni.uniid = unm.uniid				
				inner join
					territoriosgeo.municipio mun2 ON mun2.muncod = unm.muncod
				inner join
				      maismedicos.medico mdc ON mdc.muncod = unm.muncod
				where
					unistatus = 'A'
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
				group by
					mun2.muncod,mun2.mundescricao, uni.uniid
			) as foo
			group by
				muncod,
				mundescricao,
				qtde_tutores,
				qtde_supervisores";
// 	ver($sql, d);
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();

?>
	<?php ob_start(); ?>
	<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
		<tr>
			<th>Município</th>
			<th style="width:100px;">Tutores</th>
			<th>Supervisores</th>
			<th style="width:120px;">Médicos do Programa</th>
		</tr>
		<?php $num = 0; ?>
		<?php foreach($arrDados as $d): ?>
			<?php if($_GET['tpmid']): ?>
				<?php //$d['qtde_medicos'] = recuperaQtdeMedicosPorMunicipio($d['muncod']) ?>
			<?php endif;?>
			<tr onclick="exibePopUpRelatorioMaisMedicos('muncod','<?php echo $d['muncod'] ?>','municipio',0)" <?php echo $num%2==0 ? "class=\"zebrado\"" : "" ?> >
				<td class="link" ><?php echo $d['mundescricao'] ?></td>
				<td class="numero link" ><?php echo number_format($d['qtde_tutores'],0,",",".") ?></td>
				<td class="numero link" ><?php echo number_format($d['qtde_supervisores'],0,",",".") ?></td>
				<td class="numero link" ><?php echo number_format($d['qtde_medicos'],0,",",".") ?></td>
			</tr>
			<?php
				foreach($d as $chave => $valor)
				{
					$total[$chave] += $valor;
				}
			?>
			<?php $num++; ?>
		<?php endforeach; ?>
		<tr>
			<td class="bold" >Total (<?php echo number_format(count($arrDados),0,",",".") ?>)</td>
			<td class="numero bold" ><?php echo number_format($total['qtde_tutores'],0,",",".") ?></td>
			<td class="numero bold" ><?php echo number_format($total['qtde_supervisores'],0,",",".") ?></td>
			<td class="numero bold" ><?php echo number_format($total['qtde_medicos'],0,",",".") ?></td>
		</tr>
	</table>
	<?php $tabela = ob_get_contents(); ?>
	<?php ob_clean(); ?>
	<?php if(count($arrDados) > 5): ?>
		<div style="height:150px;width:100%;overflow:auto" >
			<?php echo $tabela; ?>
		</div>
	<?php else: ?>
		<?php echo $tabela; ?>
	<?php endif;?>
	<?php
}

function ajaxExibeRelatorioMaisMedicosPorUniversidade(){
	global $db;
	
	if($_GET['uniid']){
		$uniid = trim($_GET['uniid']);
		$arrWhere[] = "uni.uniid = '$uniid'";
	}

	$sql = "select
				mun2.muncod,
				mun2.mundescricao,
				count(distinct mdc.mdcid) as qtde_medicos			
				,".montaCampoCountSupervisoresTutoresMaisMedicos(array('uniid'=>true,'muncod'=>true))."			
			from
				maismedicos.universidademunicipio unm
			inner join
				maismedicos.universidade uni ON uni.uniid = unm.uniid
			left join
				territoriosgeo.municipio mun ON mun.muncod = uni.muncod
			inner join
				territoriosgeo.municipio mun2 ON mun2.muncod = unm.muncod
			inner join
			      maismedicos.medico mdc ON mdc.muncod = unm.muncod
			where
				unistatus = 'A'
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			group by
				mun2.muncod,mun2.mundescricao,mun2.estuf,uni.uniid
					";
// 	ver($sql, d);
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	
	$sqlTotal = "select
					sum(qtde_tutores) as qtde_tutores,
					sum(qtde_supervisores) as qtde_supervisores,
					sum(qtde_medicos) as qtde_medicos
				from (".$sql.") as foo";
	
	
	$arrTotal = $db->pegaLinha($sqlTotal);

	?>
	<?php ob_start(); ?>
	<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
		<tr>
			<th width="350">Município</th>				
			<th style="width:100px;">Tutores</th>
			<th>Supervisores</th>
			<th style="width:120px;">Médicos do Programa</th>
		</tr>
		<?php $num = 0; ?>
		<?php foreach($arrDados as $d): ?>
			<?php if($_GET['tpmid']): ?>
				<?php //$d['qtde_medicos'] = recuperaQtdeMedicosPorMunicipio($d['muncod']) ?>
			<?php endif;?>
			<tr onclick="exibePopUpRelatorioMaisMedicos('muncod','<?php echo $d['muncod'] ?>','municipio',0)" <?php echo $num%2==0 ? "class=\"zebrado\"" : "" ?> >
				<td class="link" ><?php echo $d['mundescricao'] ?></td>
				<td class="numero link" ><?php echo number_format($d['qtde_tutores'],0,",",".") ?></td>
				<td class="numero link" ><?php echo number_format($d['qtde_supervisores'],0,",",".") ?></td>
				<td class="numero link" ><?php echo number_format($d['qtde_medicos'],0,",",".") ?></td>
			</tr>			
			<?php $num++; ?>
		<?php endforeach; ?>
		<tr>
			<td class="bold">Total (<?php echo number_format(count($arrDados),0,",",".") ?>)</td>
			<td class="numero bold" ><?php echo number_format($arrTotal['qtde_tutores'],0,",",".") ?></td>
			<td class="numero bold" ><?php echo number_format($arrTotal['qtde_supervisores'],0,",",".") ?></td>
			<td class="numero bold" ><?php echo number_format($arrTotal['qtde_medicos'],0,",",".") ?></td>
		</tr>
	</table>
	<?php $tabela = ob_get_contents(); ?>
	<?php ob_clean(); ?>
	<?php if(count($arrDados) > 5): ?>
		<div style="height:150px;width:100%;overflow:auto" >
			<?php echo $tabela; ?>
		</div>
	<?php else: ?>
		<?php echo $tabela; ?>
	<?php endif;?>
	<?php
}

function popUpMaisMedicos()
{
	global $db;
	extract($_GET);

	if(!strstr($_SERVER['REQUEST_URI'],"maismedicos.php?modulo=relatorio/maismedicos&acao=C"))
	{
		$url_mais_medico = "?modulo=principal/cockpit_mais_medicos&acao=A";
	}else{
		$url_mais_medico = "?modulo=relatorio/maismedicos&acao=C";
	}
	if($_GET['active']!=2 && $_GET['active2'])	
		$_GET['active'] = $_GET['active2'] ? $_GET['active2'] : $_GET['active']; 
	?>
	<?php if(!strstr($_SERVER['REQUEST_URI'],"maismedicos.php?modulo=relatorio/maismedicos&acao=C")): ?>
	<html>
		<head>
			<title>Mais Médicos</title>
			<script language="JavaScript" src="../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<?php endif; ?>
			<link rel="stylesheet" href="http://code.jquery.com/ui/1.10.3/themes/smoothness/jquery-ui.css" />
			<script src="http://code.jquery.com/jquery-1.9.1.js"></script>
			<script src="http://code.jquery.com/ui/1.10.3/jquery-ui.js"></script>
			<script>
			$(function(){
				$( "ul" ).accordion({
				collapsible: true,
				heightStyle: "content",
				active: <?php echo $_GET['active']?>
				});
			});

			function filtrarUnidadeMaisMedicos(uniid,visao)
			{
				var url = "<?php echo $url_mais_medico; ?>&requisicaoAjax=popUpMaisMedicos&visao=instituicao&uniid="+uniid+"&visaoAnterior="+visao+"&regcod=<?php echo $_GET['regcod']?>&estuf=<?php echo $_GET['estuf']?>&tpmid=<?php echo $_GET['tpmid']?>&muncod=<?php echo $_GET['muncod']?>&active=<?php echo $_GET['active'] == "0" ? "0" : (int)$_GET['active'] - 1 ?>";
				window.location = url;
			}
			</script>
		<?php if(!strstr($_SERVER['REQUEST_URI'],"maismedicos.php?modulo=relatorio/maismedicos&acao=C")): ?>
		</head>
		<body>
		<?php endif; ?>
			<?php
				switch($_GET['visao'])
				{
					case "brasil":
						$descricao = "Brasil";
					break;
					case "estado":
						$descricao = "Estado - ".$_GET['estuf'];
					break;
					case "instituicao":
						$sql = "select unisigla || ' - ' || uninome as nome from maismedicos.universidade where uniid = ".$_GET['uniid'];
						$nomeInstituicao = $db->pegaUm($sql);
						$descricao = $nomeInstituicao;
					break;
					case "regiao":
						$sql = "select regdescricao from territorios.regiao where regcod = '".$_GET['regcod']."'";
						$nomeRegiao = $db->pegaUm($sql);
						$descricao = "Região ".$nomeRegiao;
					break;
					case "regiaosaude":
						$sql = "select tpmdsc from territoriosgeo.tipomunicipio where tpmid = '".$_GET['tpmid']."'";
						$nomeRegiao = $db->pegaUm($sql);
						$descricao = "Região da Saúde - ".$nomeRegiao;
					break;
					case "municipio":
						$sql = "select mundescricao || '/' || estuf as nome from territorios.municipio where muncod = '".$_GET['muncod']."'";
						$nomeMunicipio = $db->pegaUm($sql);
						$descricao = $nomeMunicipio;
					break;
				}

				if($_GET['visaoAnterior'] == "estado"){
					$url = "$url_mais_medico&requisicaoAjax=popUpMaisMedicos&estuf={$_GET['estuf']}&visao=estado&active={$_GET['active']}";
					$rastro = "<a href=\"$url\" >Estado - {$_GET['estuf']}</a> >> $descricao ";
				}elseif($_GET['visaoAnterior'] == "municipio"){
					if($_GET['muncod']){
						$sql = "select mundescricao || '/' || estuf as nome from territorios.municipio where muncod = '".$_GET['muncod']."'";
						$nomeMun = $db->pegaUm($sql);
					}
					$url = "$url_mais_medico&requisicaoAjax=popUpMaisMedicos&muncod={$_GET['muncod']}&visao=municipio&active={$_GET['active']}";
					$rastro = "<a href=\"$url\" >$nomeMun</a> >> $descricao ";
				}elseif($_GET['visaoAnterior'] == "brasil"){
					$url = "$url_mais_medico&requisicaoAjax=popUpMaisMedicos&visao=brasil&active={$_GET['active']}";
					$rastro = "<a href=\"$url\" >Brasil</a> >> $descricao ";
				}elseif($_GET['visaoAnterior'] == "regiao"){
					if($_GET['regcod']){
						$sql = "select regdescricao from territorios.regiao where regcod = '".$_GET['regcod']."'";
						$nomeRegiao = $db->pegaUm($sql);
					}
					$url = "$url_mais_medico&requisicaoAjax=popUpMaisMedicos&regcod={$_GET['regcod']}&visao=regiao&active={$_GET['active']}";
					$rastro = "<a href=\"$url\" >Região $nomeRegiao</a> >> $descricao ";
				}elseif($_GET['visaoAnterior'] == "regiaosaude"){
					if($_GET['tpmid']){
						$sql = "select tpmdsc from territoriosgeo.tipomunicipio where tpmid = '".$_GET['tpmid']."'";
						$nomeRegiao = $db->pegaUm($sql);
					}
					$url = "$url_mais_medico&requisicaoAjax=popUpMaisMedicos&tpmid={$_GET['tpmid']}&visao=regiaosaude&active={$_GET['active']}";
					$rastro = "<a href=\"$url\" >Região da Saúde - $nomeRegiao</a> >> $descricao ";
				}else{
					$rastro = "&nbsp;";
				}
				
				
				$arWhereMedicos = array();
				if($ativo)
					$arWhereMedicos[] = "mdcstatus = '$ativo'";
			?>
			<?php monta_titulo("Relatório - $descricao",$rastro) ?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td>
						<ul id="accordion">
							<?php if($visao == "estado"): ?>
								<li>
									<?php
									$arrWhere[] = "mun2.estuf = '$estuf'";
									$arrUniid = listaInstituicoesPopUpMaisMedicos($arrWhere);
									?>
								</li>
								<li>
									<?php listaTutoresPopUpMaisMedicos($arrUniid); ?>
								</li>
								<li>
									<?php listaSupervisoresPopUpMaisMedicos($arrUniid); ?>
								</li>
								<li>
									<?php
									$arWhereMedicos[] = "mun.estuf = '$estuf'";
									listaMedicosPopUpMaisMedicos($arWhereMedicos);
									?>
								</li>
							<?php endif; ?>
							<?php if($visao == "municipio"): ?>
								<li>
									<?php
									$arrWhere[] = "mun2.muncod = '$muncod'";
									$arrUniid = listaInstituicoesPopUpMaisMedicos($arrWhere);
									?>
								</li>
								<li>
									<?php listaTutoresPopUpMaisMedicos($arrUniid); ?>
								</li>
								<li>
									<?php listaSupervisoresPopUpMaisMedicos($arrUniid); ?>
								</li>
								<li>
									<?php 
									$arWhereMedicos[] = "mun.muncod = '$muncod'";
									listaMedicosPopUpMaisMedicos($arWhereMedicos);
									?>
								</li>
							<?php endif; ?>
							<?php if($visao == "regiao"): ?>
								<li>
									<?php
									$arrWhere[] = "mun2.estuf in (select estuf from territorios.estado where regcod = '{$_GET['regcod']}')";
									$arrUniid = listaInstituicoesPopUpMaisMedicos($arrWhere);
									?>
								</li>
								<li>
									<?php listaTutoresPopUpMaisMedicos($arrUniid); ?>
								</li>
								<li>
									<?php listaSupervisoresPopUpMaisMedicos($arrUniid); ?>
								</li>
								<li>
									<?php 
									$arWhereMedicos[] = "mun.estuf in (select estuf from territorios.estado where regcod = '{$_GET['regcod']}')";
									listaMedicosPopUpMaisMedicos($arWhereMedicos);
									?>
								</li>
							<?php endif; ?>
							<?php if($visao == "regiaosaude"): ?>
								<li>
									<?php
									$arrWhere[] = "mun.muncod in (select muncod from territoriosgeo.muntipomunicipio where tpmid = '{$_GET['tpmid']}')";
									$arrUniid = listaInstituicoesPopUpMaisMedicos($arrWhere);
									?>
								</li>
								<li>
									<?php listaTutoresPopUpMaisMedicos($arrUniid); ?>
								</li>
								<li>
									<?php listaSupervisoresPopUpMaisMedicos($arrUniid); ?>
								</li>
								<li>
									<?php 
									$arWhereMedicos[] = "mun.muncod in (select muncod from territoriosgeo.muntipomunicipio where tpmid = '{$_GET['tpmid']}')";
									listaMedicosPopUpMaisMedicos($arWhereMedicos);
									?>
								</li>
							<?php endif; ?>
							<?php if($visao == "instituicao"): ?>
								<li>
									<?php listaTutoresPopUpMaisMedicos(array($_GET['uniid'])); ?>
								</li>
								<li>
									<?php listaSupervisoresPopUpMaisMedicos(array($_GET['uniid'])); ?>
								</li>
								<li>
									<?php 
									$arrMun = retornaMunicipiosInstituicao($_GET['uniid']);
									$arWhereMedicos[] = "mun.muncod in ('".implode("','",$arrMun)."')";
									listaMedicosPopUpMaisMedicos($arWhereMedicos);
									?>
								</li>
							<?php endif; ?>
							<?php if($visao == "brasil"): ?>
								<li>
									<?php $arrUniid = listaInstituicoesPopUpMaisMedicos(); ?>
								</li>
								<li>
									<?php listaTutoresPopUpMaisMedicos(); ?>
								</li>
								<li>
									<?php listaSupervisoresPopUpMaisMedicos(); ?>
								</li>
								<li>
									<?php listaMedicosPopUpMaisMedicos($arWhereMedicos); ?>
								</li>
							<?php endif; ?>
						</ul>
					</td>
				</tr>
				<tr>
					<td align="center" >
						<?php if(!strstr($_SERVER['REQUEST_URI'],"maismedicos.php?modulo=relatorio/maismedicos&acao=C")): ?>
							<input type="button" name="btn_fechar" value="Fechar" onclick="window.close();" />
						<?php else: ?>
							<input type="button" name="btn_fechar" value="Sair" onclick="window.location='http://simec.mec.gov.br'" />
						<?php endif; ?>
					</td>
				</tr>
			</table>
			<?php if(!strstr($_SERVER['REQUEST_URI'],"maismedicos.php?modulo=relatorio/maismedicos&acao=C")): ?>
				</body>
			</html>
		<?php else: ?>
			<script type="text/javascript" language="javascript">
			document.getElementById( 'aguarde' ).style.visibility = 'hidden';
			document.getElementById('aguarde').style.display = 'none';
			</script>
		</body>
		</html>
		<?php endif; ?>
	<?php


}

function listaInstituicoesPopUpMaisMedicos($arrWhere = array())
{
	global $db;
	$sql = "select
				uni.uniid,
				'<a href=\"javascript:filtrarUnidadeMaisMedicos(' || uni.uniid || ',\'{$_GET['visao']}\')\" >' || unisigla || ' - ' || uninome || '</a>'  as nome,
				mun.mundescricao || '/' || mun.estuf as localizacao
			from
				maismedicos.universidade uni
			inner join
				maismedicos.universidademunicipio unm ON uni.uniid = unm.uniid
			inner join
				territoriosgeo.municipio mun ON mun.muncod = uni.muncod
			inner join
				territoriosgeo.municipio mun2 ON mun2.muncod = unm.muncod
			where
				unistatus = 'A'
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			group by
				uni.uniid, unisigla, uninome, mun.mundescricao, mun.estuf
			order by
				nome";
	//dbg($sql);
	$arrCab = array("Instituição","Município/UF");
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	foreach($arrDados as $chave => $d)
	{
		$arrUniid[] = $d['uniid'];
		unset($arrDados[$chave]['uniid']);
	}
	$total = number_format(count($arrDados),0,",",".");
	echo "<h3>Instituições ($total)</h3>";
	$db->monta_lista_simples($arrDados, $arrCab, 1000,1000,"N");
	return $arrUniid;
}

function listaTutoresPopUpMaisMedicos($arrUniid = array())
{
	global $db;
	$sql = "select
				tutnome,
				tuttelefone,
				tutemail,
				unisigla || ' - ' || uninome as nome
			from
				maismedicos.tutor tut
			inner join
				maismedicos.universidade uni ON tut.uniid = uni.uniid and tut.tuttipo = 'T' and tut.tutstatus = 'A' and tut.tutvalidade is true
			where
				uni.unistatus = 'A'
			".($arrUniid ? " and uni.uniid in (".implode(",",$arrUniid).")" : "")."
			order by
				tutnome";
	$arrCab = array("Tutor","Telefone","E-mail","Instituição");
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	$total = number_format(count($arrDados),0,",",".");
	echo "<h3>Tutores ($total)</h3>";
	$db->monta_lista_simples($arrDados, $arrCab, 1000,1000,"N");
}

function listaSupervisoresPopUpMaisMedicos($arrUniid = array())
{
	global $db;
	$sql = "select
				tutnome,
				tuttelefone,
				tutemail,
				unisigla || ' - ' || uninome as nome
			from
				maismedicos.tutor sup
			inner join
				maismedicos.universidade uni ON sup.uniid = uni.uniid and sup.tuttipo = 'S' and sup.tutstatus = 'A' and sup.tutvalidade is true
			where
				uni.unistatus = 'A'
			".($arrUniid ? " and uni.uniid in (".implode(",",$arrUniid).")" : "")."
			order by
				tutnome";
	$arrCab = array("Supervisor","Telefone","E-mail","Instituição");
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	$total = number_format(count($arrDados),0,",",".");
	echo "<h3>Supervisores ($total)</h3>";
	$db->monta_lista_simples($arrDados, $arrCab, 1000,1000,"N");
}

function listaMedicosPopUpMaisMedicos($arrWhere = array())
{
	global $db;
	$sql = "select distinct
				mdcnome,
				mdcperfil,
				mdcperfilmuncod,
				mdcfasealocacao,
				mdcpaisatuacao,
				mdcnacionalidade,
				mundescricao || '/' || mun.estuf as localizacao,
				case when mdcstatus = 'A' then '<center><font color=\"green\">Ativo</font></center>' else '<center><font color=\"red\">Inativo</font></center>' end as status
			from
				maismedicos.medico mdc
			inner join
				maismedicos.universidademunicipio unm ON mdc.muncod = unm.muncod
			inner join
				territoriosgeo.municipio mun ON mun.muncod = unm.muncod			
			".($arrWhere ? " where ".implode(" and ",$arrWhere) : "")."
			order by
				mdcnome,localizacao";
	
	$arrCab = array("Médico","Perfil","Perfil Município","Fase Locação","País de Atuação","Nacionalidade","Município/UF", 'Situação');
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	$total = number_format(count($arrDados),0,",",".");

	$countAtivos=0;
	$countInativos=0;
	if($arrDados){
		foreach($arrDados as $dados){
			if($dados['status'] == "<center><font color=\"green\">Ativo</font></center>"){
				$countAtivos++;
			}else{
				$countInativos++;
			}
		}
	}
	
	$countAtivos = number_format($countAtivos,0,",",".");
	$countInativos = number_format($countInativos,0,",",".");
	
	$filtro = '<table class="tabela" cellpadding="3" cellspacing="1" align="center">
					<tr>
						<td>
							Situação:&nbsp;
							<select name="filtro_ativo" onchange="filtroMedicos(this.value)" >
								<option value="">Todos</option>
								<option value="A" '.($_GET['ativo'] == 'A' ? 'selected' : '').'>Ativo</option>
								<option value="I" '.($_GET['ativo'] == 'I' ? 'selected' : '').'>Inativo</option>
							</select>
						</td>
					</tr>
				</table>';
	
	if($_GET){
		foreach($_GET as $k => $v){
			if(!in_array($k, array('modulo', 'acao', 'ativo')))
				$arParams[] = $k."=".$v;
		}
	}
	$stParams = implode('&', $arParams);
	unset($_GET['active']);
		
	echo "<h3>Médicos do Programa (Ativos: {$countAtivos}, Inativos: {$countInativos}, Total: {$total})</h3>";
	echo "<div style=\"height:600px;overflow:auto;\">";
	echo "<script>
				function filtroMedicos(valor)
				{
					document.location.href = 'maismedicos.php?modulo=relatorio/maismedicos&acao=C&active2=3&ativo='+valor+'&{$stParams}'
				}
		  </script>";
	echo $filtro;
	$db->monta_lista_simples($arrDados, $arrCab, 1000000,1000000,"N");
	echo "</div>";		
}

function retornaMunicipiosInstituicao($uniid)
{
	global $db;
	$sql = "select distinct muncod from maismedicos.universidademunicipio where uniid = $uniid";
	return $db->carregarColuna($sql);

}

function listaRemessaCreditoMaisMedicos()
{
	global $db; ?>
	<html>
		<head>
			<title>Remessa de Crédito - Mais Médicos</title>
			<script language="JavaScript" src="../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
			<script>
			/**
			 * Alterar visibilidade de um bloco.
			 * 
			 * @param string indica o bloco a ser mostrado/escondido
			 * @return void
			 */
			function onOffBloco( bloco )
			{
				var div_on = document.getElementById( bloco + '_div_filtros_on' );
				var div_off = document.getElementById( bloco + '_div_filtros_off' );
				var img = document.getElementById( bloco + '_img' );
				var input = document.getElementById( bloco + '_flag' );
				if ( div_on.style.display == 'none' )
				{
					div_on.style.display = 'block';
					div_off.style.display = 'none';
					input.value = '0';
					img.src = '/imagens/menos.gif';
				}
				else
				{
					div_on.style.display = 'none';
					div_off.style.display = 'block';
					input.value = '1';
					img.src = '/imagens/mais.gif';
				}
			}
			
			/**
			 * Alterar visibilidade de um campo.
			 * 
			 * @param string indica o campo a ser mostrado/escondido
			 * @return void
			 */
			function onOffCampo( campo )
			{
				var div_on = document.getElementById( campo + '_campo_on' );
				var div_off = document.getElementById( campo + '_campo_off' );
				var input = document.getElementById( campo + '_campo_flag' );
				if ( div_on.style.display == 'none' )
				{
					div_on.style.display = 'block';
					div_off.style.display = 'none';
					input.value = '1';
				}
				else
				{
					div_on.style.display = 'none';
					div_off.style.display = 'block';
					input.value = '0';
				}
			}

			function enviarFormulario()
			{
				selectAllOptions(document.getElementById('uniid'));
				selectAllOptions(document.getElementById('strid'));
				
				document.formulario.submit();
			}

			function limpaFormulario(rmcid, periodo)
			{
				document.location.href = '/pde/estrategico.php?modulo=principal/cockpit_mais_medicos&acao=A&requisicaoAjax=listaRemessaCreditoMaisMedicos&rmcid='+rmcid+'&periodo='+periodo;
			}
			</script>
		</head>
		<body>
			<?php extract($_REQUEST);  ?>
			<form id="formulario" name="formulario" method="post">
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td class="subtitulodireita">CPF</td>
						<td><?=campo_texto('tutcpf','','','',16,14,'###.###.###-##','', '', '', '', '', '', $tutcpf, "this.value=mascaraglobal('###.###.###-##',this.value);");?></td>
					</tr>
					<tr>
						<td class="subtitulodireita">Nome</td>
						<td><?=campo_texto('tutnome','','','',50,50,'','', '', '', '', '', '', $tutnome);?></td>
					</tr>
					<tr>
						<td class="subtitulodireita">Função</td>
						<td>
							<input type="radio" name="tuttipo" value="T" <?php echo $_REQUEST['tuttipo'] == 'T' ? 'checked' : ''; ?> />&nbsp;Tutor&nbsp;
							<input type="radio" name="tuttipo" value="S" <?php echo $_REQUEST['tuttipo'] == 'S' ? 'checked' : ''; ?>/>&nbsp;Supervisor&nbsp;
							<input type="radio" name="tuttipo" value="A" <?php echo (empty($_REQUEST['tuttipo']) || $_REQUEST['tuttipo'] == 'A') ? 'checked' : ''; ?>/>&nbsp;Ambos
						</td>
					</tr>
					<?php 
					// -- Universidades
// 					$uniid = array();
					$sql_carregados = '';
					if ($_REQUEST['uniid'] && $_REQUEST['uniid'][0] != '') {
						$sql_carregados = "SELECT 
											uniid as codigo, 
											uninome as descricao 
										FROM maismedicos.universidade where unistatus = 'A'
	                                   AND uniid IN(" . implode(',', $_REQUEST['uniid']) . ")
	                                 ORDER BY uninome asc";
						
// 						$uniid = $db->carregar($sql_carregados);
					}
					$stSql = "select uniid as codigo, uninome as descricao from maismedicos.universidade where unistatus = 'A' order by uninome asc";
					mostrarComboPopup( 'Instituição', 'uniid', $stSql, $sql_carregados, 'Selecione a(s) Instituição(ões)');
					
					// -- Coordenação
// 					$strid = array();
					$sql_carregados = '';
					if ($_REQUEST['strid'] && $_REQUEST['strid'][0] != '') {
						$sql_carregados = "select 
											strid as codigo, 
											strdsc as descricao 
										from maismedicos.situacaoregistro
	                                   where strid IN(" . implode(',', $_REQUEST['strid']) . ")
	                                 ORDER BY strcod asc";
						
// 						$strid = $db->carregar($sql_carregados);
					}
					
					$stSql = "select strid as codigo, strcod || ' - ' || strdsc as descricao from maismedicos.situacaoregistro where strstatus = 'A' order by strcod asc";
					mostrarComboPopup('Situação', 'strid', $stSql, $sql_carregados, 'Selecione a(s) Situação(ões)');
					?>				
					<tr>
						<td colspan="2" class="subtituloesquerda">
							<center>
							<input type="button" value="Pesquisar" onclick="enviarFormulario()" />
							<input type="button" value="Limpar" onclick="limpaFormulario('<?php echo $_REQUEST['rmcid']; ?>', '<?php echo $_REQUEST['periodo']?>')" />
							</center>							
						</td>					
					</tr>
				</table>
			</form>
			<?php
			
			$arPeriodo = explode('/', $_GET['periodo']);
			
			if(!empty($_POST['tutcpf'])){
				$_POST['tutcpf'] = str_replace(array('.','-'),'', $_POST['tutcpf']);
				$arWhere[] = "tut.tutcpf = '{$_POST['tutcpf']}'";
			}
			if(!empty($_POST['tutnome'])){
				$arWhere[] = "tut.tutnome ilike '%{$_POST['tutnome']}%'";
			}
			if( $strid[0] && $strid_campo_flag ){
				$arWhere[] = " sit.strid ". (!$strid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( ".implode( ",", $strid )." ) ";
			}
			if( $uniid[0] && $uniid_campo_flag ){
				$arWhere[] = " uni.uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( ".implode( ",", $uniid )." ) ";
			}			
			if(!empty($_POST['tuttipo']) && $_POST['tuttipo'] != 'A'){	
				$operador = $_POST['tuttipo'] == 'T' ? '=' : '<>';			
				$arWhere[] = "tut.tuttipo {$operador} 'T'";
			}
			
			if(!empty($arPeriodo[1]) && !empty($arPeriodo[0])){
				$arPeriodo[0] = str_pad($arPeriodo[0], 2, "0", STR_PAD_LEFT);
				$mesAno = $arPeriodo[1].$arPeriodo[0];
				$arWhere[] = "dt_ini_periodo ilike '{$mesAno}%' AND det.dt_fim_periodo ilike '{$mesAno}%'";
			}
			
			monta_titulo("Informativo de pagamento de bolsas","Referência: ".$_GET['periodo']);
			$rmcid = $_GET['rmcid'];
			$sql = "select
	   				tut.tutnome,
	   				case when tut.tuttipo = 'T'
	   					then 'Tutor'
	   					else 'Supervisor'
	   				end as funcao,
	   				case when tut.tuttipo = 'T'
	   					then '5000'
	   					else '4000'
	   				end as valor,
	   				--fpgdsc,
					to_char(to_date(dt_ini_periodo,'YYYYMMDD'), 'MM/YYYY') as periodo,
	   				uni.uninome,
	   				coalesce(sit.strdsc,'N/A') as tpodsc
	   			from
	   				maismedicos.tutor tut
	   			inner join
	   				maismedicos.universidade uni ON uni.uniid = tut.uniid
	   			inner join
	   				maismedicos.remessadetalhe det ON det.tutid = tut.tutid and det.cs_ocorrencia = '0000'
	   			inner join
	   				maismedicos.remessacabecalho cab ON cab.rmcid = det.rmcid
	   			inner join
	   				maismedicos.folhapagamento fpg ON fpg.fpgid = cab.fpgid
	   			inner join 
					maismedicos.autorizacaopagamento apg on apg.rmdid = det.rmdid and apg.apgstatus = 'A'	   				 
	   			left join
	   				maismedicos.situacaoregistro sit ON sit.strcod = det.cs_ocorrencia	   			
	   			".($arWhere ? ' where '.implode(' and ', $arWhere) : '')."
	   			order by
	   				tut.tutnome, to_char(to_date(dt_ini_periodo,'YYYYMMDD'), 'YYYYMM')";
// 			ver($sql);
		   	$arrCab = array("Nome","Função","Valor (R$)","Mês","Instituição","Situação");
		   	$db->monta_lista($sql, $arrCab, 10000000, 10, "S", "center", "");
			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td align="center" >
						<input type="button" name="btn_fechar" value="Fechar" onclick="window.close();" />
					</td>
				</tr>
			</table>
		</body>
	</html>
	<?php

}

function exibeRelatorioMaisMedicoRegiao()
{
	global $db;

	$sql = "select
				r.regdescricao,
				r.regcod,
		 		count(distinct e.estuf) as qtde_estados,
				count(distinct uni.unisigla) as qtde_instituicoes,
				count(distinct unm.muncod) as qtde_municipios,				
				".montaCampoCountSupervisoresTutoresMaisMedicos(array('uniid'=>true, 'muncod'=>true)).",
				count(distinct mdc.mdccpf) as qtde_medicos
			from
				maismedicos.universidademunicipio unm
			inner join
				maismedicos.universidade uni ON uni.uniid = unm.uniid
			inner join
				territoriosgeo.municipio mun2 ON mun2.muncod = unm.muncod
			inner join
			       territorios.estado e ON e.estuf = mun2.estuf
			inner join
			       territorios.regiao r ON e.regcod = r.regcod
			inner join
			      maismedicos.medico mdc ON mdc.muncod = unm.muncod and mdcstatus = 'A'
			where
				unistatus = 'A'
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			group by
				r.regdescricao,r.regcod,uni.uniid, mun2.muncod
			order by
				r.regdescricao";
	
	$sql = "select
				sum(qtde_estados) as qtde_estados,
				sum(qtde_instituicoes) as qtde_instituicoes,
				sum(qtde_municipios) as qtde_municipios,
				sum(qtde_tutores) as qtde_tutores,
				sum(qtde_supervisores) as qtde_supervisores,
				regdescricao,
				regcod,
				sum(qtde_medicos) as qtde_medicos
			from ({$sql}) as foo
			group by 
				regdescricao,regcod";
	
// 	ver($sql,d);
	$sqlTotal = "select
		 		count(distinct e.estuf) as qtde_estados,
				count(distinct uni.unisigla) as qtde_instituicoes,
				count(distinct unm.muncod) as qtde_municipios,
				(select count(distinct tut.tutcpf) from maismedicos.universidade uni_tut inner join maismedicos.tutor tut ON tut.uniid = uni_tut.uniid and tut.tuttipo = 'T' and tut.tutstatus = 'A' and tut.tutvalidade is true where uni_tut.unistatus = 'A') as qtde_tutores,
				(select count(distinct sup.tutcpf) from maismedicos.universidade uni_sup inner join maismedicos.tutor sup ON sup.uniid = uni_sup.uniid and sup.tuttipo = 'S' and sup.tutstatus = 'A' and sup.tutvalidade is true where uni_sup.unistatus = 'A') as qtde_supervisores,
				count(distinct mdc.mdccpf) as qtde_medicos
			from
				maismedicos.universidademunicipio unm
			inner join
				maismedicos.universidade uni ON uni.uniid = unm.uniid
			left join
				territoriosgeo.municipio mun ON mun.muncod = uni.muncod
			inner join
			       territorios.estado e ON e.estuf = mun.estuf
			inner join
			       territorios.regiao r ON e.regcod = r.regcod
			inner join
				territoriosgeo.municipio mun2 ON mun2.muncod = unm.muncod
			inner join
			      maismedicos.medico mdc ON mdc.muncod = unm.muncod and mdcstatus = 'A'
			where
				unistatus = 'A'
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."";
	
	$arrDados = $db->carregar($sql);

	?>
	<script>
		function exibeEstadoRelatorioMaisMedicos(regcod)
		{
			var img = jQuery("#img_relatorio_mais_medicos_"+regcod);
			var tr = jQuery("#tr_relatorio_mais_medicos_"+regcod);
			var td = jQuery("#td_relatorio_mais_medicos_"+regcod);
			var html = td.html();

			if(img.attr("src") == "../imagens/mais.gif"){
				img.attr("src","../imagens/menos.gif");
				if(html == "Carregando..."){
					ajaxExibeRelatorioMaisMedicosPorRegiao(regcod);
				}
				tr.show();
			}else{
				img.attr("src","../imagens/mais.gif");
				tr.hide();
			}
		}

		function ajaxExibeRelatorioMaisMedicosPorRegiao(regcod)
		{
			var td = jQuery("#td_relatorio_mais_medicos_"+regcod);
			jQuery.ajax({
				type: "POST",
				url: window.location+"&requisicaoAjax=exibeRelatorioMaisMedicos&regcod="+regcod,
				async: false,
				success: function(response){
					td.html(response);
				}
			});
		}

		function exibePopUpRelatorioMaisMedicos(parametro,valor,visao,active)
		{
			if(parametro && valor){
				var url = window.location + "&requisicaoAjax=popUpMaisMedicos&" + parametro+"="+valor+"&visao="+visao+"&active="+active;
			}else{
				var url = window.location + "&requisicaoAjax=popUpMaisMedicos&visao="+visao+"&active="+active;
			}
			janela(url,1024,768,"Mais Médicos");
		}


	</script>

		<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
			<tr>
				<th>Região</th>
				<th>Municípios</th>
				<th>Instituições</th>
				<th style="width:100px;">Tutores</th>
				<th>Supervisores</th>
				<th style="width:140px;">Médicos do Programa</th>
			</tr>
			<?php ob_start(); ?>
			<?php $num = 0; ?>
			<?php foreach($arrDados as $d): ?>
				<?php $d['regcod'] = trim($d['regcod'])  ?>
				<tr <?php echo $num%2==0 ? "class=\"zebrado\"" : "" ?> >
					<td><img src="../imagens/mais.gif" class="link" id="img_relatorio_mais_medicos_<?php echo $d['regcod'] ?>" onclick="exibeEstadoRelatorioMaisMedicos('<?php echo $d['regcod'] ?>')"  > <?php echo $d['regdescricao'] ?> (<?php echo number_format($d['qtde_estados'],0,",",".") ?>)</td>
					<td onclick="exibePopUpRelatorioMaisMedicos('regcod','<?php echo $d['regcod'] ?>','regiao',0)" class="numero link" ><?php echo number_format($d['qtde_municipios'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos('regcod','<?php echo $d['regcod'] ?>','regiao',0)" class="numero link" ><?php echo number_format($d['qtde_instituicoes'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos('regcod','<?php echo $d['regcod'] ?>','regiao',1)" class="numero link" ><?php echo number_format($d['qtde_tutores'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos('regcod','<?php echo $d['regcod'] ?>','regiao',2)" class="numero link" ><?php echo number_format($d['qtde_supervisores'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos('regcod','<?php echo $d['regcod'] ?>','regiao',3)" class="numero link" ><?php echo number_format($d['qtde_medicos'],0,",",".") ?></td>
				</tr>
				
				<tr style="display:none"  id="tr_relatorio_mais_medicos_<?php echo $d['regcod'] ?>"  <?php echo $num%2==0 ? "class=\"zebrado\"" : "" ?> >
					<td id="td_relatorio_mais_medicos_<?php echo $d['regcod'] ?>" colspan="<?php echo count($d) ?>" >Carregando...</td>
				</tr>
				<?php
					foreach($d as $chave => $valor)
					{
						$total[$chave] += $valor;
					}
				?>
				<?php $num++; ?>
			<?php endforeach; ?>
			<?php $tabela = ob_get_contents(); ?>
			<?php
			$arrTotal = $db->pegaLinha($sqlTotal);
			?>
			<?php ob_clean() ?>
			<tr>
				<td class="bold" >Brasil (<?php echo number_format(count($arrDados),0,",",".") ?>)</td>
				<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',0)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_municipios'],0,",",".") ?></td>
				<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',0)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_instituicoes'],0,",",".") ?></td>
				<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',1)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_tutores'],0,",",".") ?></td>
				<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',2)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_supervisores'],0,",",".") ?></td>
				<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',3)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_medicos'],0,",",".") ?></td>
			</tr>
			<?php echo $tabela; ?>
		</table>

	<?php

}

function exibeRelatorioInstituicaoSupervisora()
{
	global $db;

	$params['uniid'] = true;
// 	$params['muncod'] = true;
	
	$sql = "select
				uni.uninome,uni.uniid	
				,count(distinct mun2.muncod) as qtde_municipios				
				,count(distinct mdc.mdccpf) as qtde_medicos
				,".montaCampoCountSupervisoresTutoresMaisMedicos($params)."				
			from
				maismedicos.universidademunicipio mun2 
			inner join
				maismedicos.universidade uni ON uni.uniid = mun2.uniid				
			inner join 
				maismedicos.medico mdc ON mdc.muncod = mun2.muncod and mdcstatus = 'A'
			where
				unistatus = 'A'			
			group by 
				uni.uniid ,uni.uninome
			order by 
				uni.uninome";
	//ver($sql, d);
	
	$sqlDados = "select 
					uninome,uniid,
					sum(qtde_municipios) as qtde_municipios,
					sum(qtde_medicos) as qtde_medicos, 
					sum(qtde_tutores) as qtde_tutores, 
					sum(qtde_supervisores) as qtde_supervisores 
				from ({$sql}) as foo
				group by 
					uninome,uniid";
	$arrDados = $db->carregar($sqlDados);

	$sqlTotal = "select
		 		count(distinct e.estuf) as qtde_estados,
				count(distinct uni.unisigla) as qtde_instituicoes,
				count(distinct unm.muncod) as qtde_municipios,
				(select count(distinct tut.tutcpf) from maismedicos.universidade uni_tut inner join maismedicos.tutor tut ON tut.uniid = uni_tut.uniid and tut.tuttipo = 'T' and tut.tutstatus = 'A' and tut.tutvalidade is true where uni_tut.unistatus = 'A') as qtde_tutores,
				(select count(distinct sup.tutcpf) from maismedicos.universidade uni_sup inner join maismedicos.tutor sup ON sup.uniid = uni_sup.uniid and sup.tuttipo = 'S' and sup.tutstatus = 'A' and sup.tutvalidade is true where uni_sup.unistatus = 'A') as qtde_supervisores,
				count(distinct mdc.mdccpf) as qtde_medicos
			from
				maismedicos.universidademunicipio unm
			inner join
				maismedicos.universidade uni ON uni.uniid = unm.uniid
			left join
				territoriosgeo.municipio mun ON mun.muncod = uni.muncod
			inner join
			       territorios.estado e ON e.estuf = mun.estuf
			inner join
			       territorios.regiao r ON e.regcod = r.regcod
			inner join
				territoriosgeo.municipio mun2 ON mun2.muncod = unm.muncod
			inner join
			      maismedicos.medico mdc ON mdc.muncod = unm.muncod and mdcstatus = 'A'
			where
				unistatus = 'A'";
// 	ver($sqlTotal, d);
	$arrTotal = $db->pegaLinha($sqlTotal);
	
	?>
	<script>
	
		function exibeEstadoRelatorioMaisMedicos(uniid)
		{
			var img = jQuery("#img_relatorio_mais_medicos_"+uniid);
			var tr = jQuery("#tr_relatorio_mais_medicos_"+uniid);
			var td = jQuery("#td_relatorio_universidades_mm_"+uniid);
			var html = td.html();

			if(img.attr("src") == "../imagens/mais.gif"){
				img.attr("src","../imagens/menos.gif");
				tr.show();
				if(html == "Carregando..."){
					ajaxExibeRelatorioMaisMedicosPorUF(uniid);
				}
			}else{
				img.attr("src","../imagens/mais.gif");
				tr.hide();
			}
		}

		function ajaxExibeRelatorioMaisMedicosPorUF(uniid)
		{
			var td = jQuery("#td_relatorio_universidades_mm_"+uniid);
			td.html('Carregando...');			
			jQuery.ajax({
				type: "POST",
				url: window.location+"&requisicaoAjax=ajaxExibeRelatorioMaisMedicosPorUniversidade&uniid="+uniid,
				async: false,
				success: function(response){
					td.html(response);
				}
			});
		}
		
		function exibePopUpRelatorioMaisMedicos(parametro,valor,visao,active)
		{
			if(parametro && valor){
				var url = window.location + "&requisicaoAjax=popUpMaisMedicos&" + parametro+"="+valor+"&visao="+visao+"&active="+active;
			}else{
				var url = window.location + "&requisicaoAjax=popUpMaisMedicos&visao="+visao+"&active="+active;
			}
			janela(url,1024,768,"Mais Médicos");
		}

	</script>

		<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
			<tr>
				<th>Instituição Supervisora</th>				
				<th style="width:100px;">Tutores</th>
				<th style="width:160px;">Supervisores</th>
				<th style="width:140px;">Médicos do Programa</th>
			</tr>
			<tr>
				<td class="bold" >Brasil (<?php echo number_format(count($arrDados),0,",",".") ?>)</td>
				<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',1)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_tutores'],0,",",".") ?></td>
				<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',2)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_supervisores'],0,",",".") ?></td>
				<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',3)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_medicos'],0,",",".") ?></td>
			</tr>
			<?php $num = 0; ?>
			<?php foreach($arrDados as $d): ?>
				<?php $d['uniid'] = trim($d['uniid'])  ?>
				<tr <?php echo $num%2==0 ? "class=\"zebrado\"" : "" ?> >
					<td><img src="../imagens/mais.gif" class="link" id="img_relatorio_mais_medicos_<?php echo $d['uniid'] ?>" onclick="exibeEstadoRelatorioMaisMedicos('<?php echo $d['uniid'] ?>')"  >&nbsp;<?php echo $d['uninome'] ?> (<?php echo number_format($d['qtde_municipios'],0,",",".") ?>)</td>					
					<td onclick="exibePopUpRelatorioMaisMedicos('uniid','<?php echo $d['uniid'] ?>','instituicao',1)" class="numero link" ><?php echo number_format($d['qtde_tutores'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos('uniid','<?php echo $d['uniid'] ?>','instituicao',2)" class="numero link"><?php echo number_format($d['qtde_supervisores'],0,",",".") ?></td>							
					<td onclick="exibePopUpRelatorioMaisMedicos('uniid','<?php echo $d['uniid'] ?>','instituicao',3)" class="numero link" ><?php echo number_format($d['qtde_medicos'],0,",",".") ?></td>
				</tr>
				
				<tr style="display:none"  id="tr_relatorio_mais_medicos_<?php echo $d['uniid'] ?>"  <?php echo $num%2==0 ? "class=\"zebrado\"" : "" ?> >
					<td id="td_relatorio_universidades_mm_<?php echo $d['uniid'] ?>" colspan="<?php echo count($d) ?>" >Carregando...</td>
				</tr>
				<?php
					foreach($d as $chave => $valor)
					{
						$total[$chave] += $valor;
					}
				?>
				<?php $num++; ?>
			<?php endforeach; ?>

		</table>

	<?php

}

function exibeRelatorioMaisMedicosRegioesSaude()
{
	global $db;

	$sql = "select
				tpm.tpmid,
				tpm.tpmdsc,
				count(distinct e.estuf) as qtde_estados,
				count(distinct uni.unisigla) as qtde_instituicoes,
				count(distinct mun2.muncod) as qtde_municipios,
				".montaCampoCountSupervisoresTutoresMaisMedicos(array('uniid'=>true, 'muncod'=>true)).",
				count(distinct mdc.mdccpf) as qtde_medicos
			from
				maismedicos.universidademunicipio unm
			inner join
				maismedicos.universidade uni ON uni.uniid = unm.uniid
			left join territoriosgeo.municipio mun ON mun.muncod = uni.muncod
			inner join
				territoriosgeo.municipio mun2 ON mun2.muncod = unm.muncod
			inner join
			       territorios.estado e ON e.estuf = mun.estuf
			inner join
				territoriosgeo.muntipomunicipio tgm ON tgm.muncod = mun.muncod
			inner join
			       territoriosgeo.tipomunicipio tpm ON tgm.tpmid = tpm.tpmid
			inner join
			      maismedicos.medico mdc ON mdc.muncod = unm.muncod and mdcstatus = 'A'
			where
				unistatus = 'A'
			and
				tpm.tpmstatus = 'A'
			and
				gtmid = 1
			group by
				tpm.tpmid, tpm.tpmdsc, uni.uniid, mun2.muncod
			order by
				tpm.tpmdsc";
	
	$sql = "select 
				sum(qtde_estados) as qtde_estados,
				sum(qtde_instituicoes) as qtde_instituicoes,
				sum(qtde_municipios) as qtde_municipios,
				sum(qtde_tutores) as qtde_tutores,
				sum(qtde_supervisores) as qtde_supervisores,
				sum(qtde_medicos) as qtde_medicos,
				tpmid,
				tpmdsc
			from ({$sql}) as foo
			group by
				tpmid,
				tpmdsc";

	$sqlTotal = "select
				count(distinct e.estuf) as qtde_estados,
				count(distinct uni.unisigla) as qtde_instituicoes,
				count(distinct unm.muncod) as qtde_municipios,
				(select count(distinct tut.tutcpf) from maismedicos.universidade uni_tut inner join maismedicos.tutor tut ON tut.uniid = uni_tut.uniid and tut.tuttipo = 'T' and tut.tutstatus = 'A' and tut.tutvalidade is true where uni_tut.unistatus = 'A') as qtde_tutores,
				(select count(distinct sup.tutcpf) from maismedicos.universidade uni_sup inner join maismedicos.tutor sup ON sup.uniid = uni_sup.uniid and sup.tuttipo = 'S' and sup.tutstatus = 'A' and sup.tutvalidade is true where uni_sup.unistatus = 'A') as qtde_supervisores,
				count(distinct mdc.mdccpf) as qtde_medicos
			from
				maismedicos.universidademunicipio unm
			inner join
				maismedicos.universidade uni ON uni.uniid = unm.uniid
			left join
				territoriosgeo.municipio mun ON mun.muncod = uni.muncod
			inner join
				territoriosgeo.municipio mun2 ON mun2.muncod = unm.muncod
			inner join
			       territorios.estado e ON e.estuf = mun.estuf
			inner join
				territoriosgeo.muntipomunicipio tgm ON tgm.muncod = mun.muncod
			inner join
			       territoriosgeo.tipomunicipio tpm ON tgm.tpmid = tpm.tpmid
			inner join
			      maismedicos.medico mdc ON mdc.muncod = unm.muncod and mdcstatus = 'A'
			where
				unistatus = 'A'
			and
				tpm.tpmstatus = 'A'
			and
				gtmid = 1
			";
	$arrDados = $db->carregar($sql);

	?>
	<script>
		function exibeMunicipiosRelatorioMaisMedicosPorRegiaoSaude(tpmid)
		{
			var img = jQuery("#img_relatorio_mais_medicos_"+tpmid);
			var tr = jQuery("#tr_relatorio_mais_medicos_"+tpmid);
			var td = jQuery("#td_relatorio_mais_medicos_"+tpmid);
			var html = td.html();

			if(img.attr("src") == "../imagens/mais.gif"){
				img.attr("src","../imagens/menos.gif");
				if(html == "Carregando..."){
					ajaxExibeRelatorioMaisMedicosPorRegiaoSaude(tpmid);
				}
				tr.show();
			}else{
				img.attr("src","../imagens/mais.gif");
				tr.hide();
			}
		}

		function ajaxExibeRelatorioMaisMedicosPorRegiaoSaude(tpmid)
		{
			var td = jQuery("#td_relatorio_mais_medicos_"+tpmid);
			jQuery.ajax({
				type: "POST",
				url: window.location+"&requisicaoAjax=ajaxExibeRelatorioMaisMedicosPorUF&tpmid="+tpmid,
				async: false,
				success: function(response){
					td.html(response);
				}
			});
		}

		function exibePopUpRelatorioMaisMedicos(parametro,valor,visao,active)
		{
			if(parametro && valor){
				var url = window.location + "&requisicaoAjax=popUpMaisMedicos&" + parametro+"="+valor+"&visao="+visao+"&active="+active;
			}else{
				var url = window.location + "&requisicaoAjax=popUpMaisMedicos&visao="+visao+"&active="+active;
			}
			janela(url,1024,768,"Mais Médicos");
		}


	</script>

		<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
			<tr>
				<th>Região</th>
				<th>Municípios</th>
				<th>Instituições</th>
				<th style="width:100px;">Tutores</th>
				<th>Supervisores</th>
				<th>Médicos do Programa</th>
			</tr>
			<?php ob_start(); ?>
			<?php $num = 0; ?>
			<?php foreach($arrDados as $d): ?>
				<?php $d['tpmid'] = trim($d['tpmid'])  ?>
				<tr <?php echo $num%2==0 ? "class=\"zebrado\"" : "" ?> >
					<td><img src="../imagens/mais.gif" class="link" id="img_relatorio_mais_medicos_<?php echo $d['tpmid'] ?>" onclick="exibeMunicipiosRelatorioMaisMedicosPorRegiaoSaude('<?php echo $d['tpmid'] ?>')"  > <?php echo $d['tpmdsc'] ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos('tpmid','<?php echo $d['tpmid'] ?>','regiaosaude',0)" class="numero link" ><?php echo number_format($d['qtde_municipios'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos('tpmid','<?php echo $d['tpmid'] ?>','regiaosaude',0)" class="numero link" ><?php echo number_format($d['qtde_instituicoes'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos('tpmid','<?php echo $d['tpmid'] ?>','regiaosaude',1)" class="numero link" ><?php echo number_format($d['qtde_tutores'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos('tpmid','<?php echo $d['tpmid'] ?>','regiaosaude',2)" class="numero link" ><?php echo number_format($d['qtde_supervisores'],0,",",".") ?></td>
					<td onclick="exibePopUpRelatorioMaisMedicos('tpmid','<?php echo $d['tpmid'] ?>','regiaosaude',3)" class="numero link" ><?php echo number_format($d['qtde_medicos'],0,",",".") ?></td>
				</tr>
				<tr style="display:none"  id="tr_relatorio_mais_medicos_<?php echo $d['tpmid'] ?>"  <?php echo $num%2==0 ? "class=\"zebrado\"" : "" ?> >
					<td id="td_relatorio_mais_medicos_<?php echo $d['tpmid'] ?>" colspan="<?php echo count($d) ?>" >Carregando...</td>
				</tr>
				<?php
					foreach($d as $chave => $valor)
					{
						$total[$chave] += $valor;
					}
				?>
				<?php $num++; ?>
			<?php endforeach; ?>
			<?php $tabela = ob_get_contents(); ?>
			<?php
			$arrTotal = $db->pegaLinha($sqlTotal);
			?>
			<?php ob_clean() ?>
			<tr>
				<td class="bold" >Brasil (<?php echo number_format(count($arrDados),0,",",".") ?>)</td>
				<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',0)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_municipios'],0,",",".") ?></td>
				<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',0)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_instituicoes'],0,",",".") ?></td>
				<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',1)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_tutores'],0,",",".") ?></td>
				<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',2)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_supervisores'],0,",",".") ?></td>
				<td onclick="exibePopUpRelatorioMaisMedicos(null,null,'brasil',3)" class="numero bold link" ><?php echo number_format($arrTotal['qtde_medicos'],0,",",".") ?></td>
			</tr>
			<?php echo $tabela; ?>
		</table>

	<?php

}

function recuperaMedicosPorRegiaoSaude($tpmid)
{
	global $db;
	$sql = "select count(mdcid) from  maismedicos.medico where muncod in (select muncod from territoriosgeo.muntipomunicipio where tpmid = '$tpmid')";
	return $db->pegaUm($sql);
}

function recuperaQtdeMedicosPorMunicipio($muncod)
{
	global $db;
	$sql = "select count(mdcid) from  maismedicos.medico where muncod = '$muncod'";
	return $db->pegaUm($sql);
}

function recuperaMunicipiosPorRegiaoSaude($tpmid)
{
	global $db;
	$sql = "select count(distinct muncod) from maismedicos.medico where muncod in (select muncod from territoriosgeo.muntipomunicipio where tpmid = '$tpmid')";
	return $db->pegaUm($sql);
}

function recuperaTotalInstituicoesPorUf($estuf)
{
	global $db;
	$sql = "select count(distinct uniid) from maismedicos.universidademunicipio where muncod in (select muncod from territoriosgeo.municipio where estuf = '$estuf')";
	return $db->pegaUm($sql);
}

function recuperaTotalTutoresPorUf($estuf)
{
	global $db;
	$sql = "select count(distinct tut.tutid) as qtde_supervisores from maismedicos.universidademunicipio uni
	left join
			      maismedicos.tutor tut ON tut.uniid = uni.uniid and tut.tuttipo = 'T' and tut.tutstatus = 'A' and tut.tutvalidade is true
	where uni.muncod in (select muncod from territoriosgeo.municipio where estuf = '$estuf')";
	return $db->pegaUm($sql);
}

function recuperaTotalSupervisoresPorUf($estuf)
{
	global $db;
	$sql = "select count(distinct tut.tutid) as qtde_supervisores from maismedicos.universidademunicipio uni
	left join
			      maismedicos.tutor tut ON tut.uniid = uni.uniid and tut.tuttipo = 'S' and tut.tutstatus = 'A' and tut.tutvalidade is true
	where uni.muncod in (select muncod from territoriosgeo.municipio where estuf = '$estuf')";
	return $db->pegaUm($sql);
}

function recuperaTotalInstituicoesPorRegiao($regcod)
{
	global $db;
	$sql = "select count(distinct uniid) from maismedicos.universidademunicipio
	where muncod in (select muncod from territoriosgeo.municipio where estuf in (select estuf from territoriosgeo.estado where regcod = '$regcod' ) )";
	return $db->pegaUm($sql);
}

function recuperaTotalTutoresPorRegiao($regcod)
{
	global $db;
	$sql = "select count(distinct tut.tutid) as qtde_supervisores from maismedicos.universidademunicipio uni
	left join
			      maismedicos.tutor tut ON tut.uniid = uni.uniid and tut.tuttipo = 'T' and tut.tutstatus = 'A' and tut.tutvalidade is true
	where uni.muncod in (select muncod from territoriosgeo.municipio where estuf in (select estuf from territoriosgeo.estado where regcod = '$regcod'))";
	return $db->pegaUm($sql);
}

function recuperaTotalSupervisoresPorRegiao($regcod)
{
	global $db;
	$sql = "select count(distinct tut.tutid) as qtde_supervisores from maismedicos.universidademunicipio uni
	left join
			      maismedicos.tutor tut ON tut.uniid = uni.uniid and tut.tuttipo = 'S' and tut.tutstatus = 'A' and tut.tutvalidade is true
	where uni.muncod in (select muncod from territoriosgeo.municipio where estuf in (select estuf from territoriosgeo.estado where regcod = '$regcod'))";
	return $db->pegaUm($sql);
}

function montaTabelaExamesMec($nomeindicador, $arrNumeroProjeto, $subtitulo='')
{
	global $db;
	$arrSituacao=array();
	$count=0;
	
	if(!is_array($arrNumeroProjeto)){
		$arrNumeroProjeto = array($arrNumeroProjeto);
	}
	foreach($arrNumeroProjeto as $numeroProjeto){
		if($count==0){
			$where = "_atinumero LIKE '".$numeroProjeto."%' ";
		}else{
			$where = $where . "OR _atinumero LIKE '".$numeroProjeto."%' ";
		}
		$count++;
	}

    if(in_array(26, $arrNumeroProjeto) || in_array(18, $arrNumeroProjeto) || in_array(19, $arrNumeroProjeto) || in_array(37, $arrNumeroProjeto) || in_array(38, $arrNumeroProjeto) || in_array(47, $arrNumeroProjeto) || in_array(48, $arrNumeroProjeto) || in_array(58, $arrNumeroProjeto) || in_array(59, $arrNumeroProjeto) || in_array(60, $arrNumeroProjeto)){
        $condicao = "icl.iclprazo IS NOT NULL AND ";
    }else{
        $condicao = "";
    }

	$sql = "SELECT numero || '-' || atidescricao as atidescricao, situacao, count(0) as total
			FROM (
				SELECT DISTINCT
					icl.iclid,
					SUBSTR(_atinumero,0,5) AS numero,
					CASE
						WHEN ".$condicao." ch1.entid IS NOT NULL AND ch2.entid IS NOT NULL AND ch3.entid IS NOT NULL THEN
							CASE
								WHEN val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL AND val3.vldid IS NOT NULL THEN 'Executados'
								WHEN icl.iclprazo < date(now()) THEN
									CASE
										WHEN ch1.entid IS NOT NULL AND val1.vldid IS NULL THEN 'Em Atraso'
										WHEN ch2.entid IS NOT NULL AND val2.vldid IS NULL THEN 'Em Atraso'
										WHEN ch3.entid IS NOT NULL AND val3.vldid IS NULL THEN 'Em Atraso'
									END
							ELSE 'A Executar'
							END
						WHEN ".$condicao." ch1.entid IS NOT NULL AND ch2.entid IS NOT NULL AND ch3.entid IS NULL THEN
							CASE
								WHEN val1.vldid IS NOT NULL AND val2.vldid IS NOT NULL THEN 'Executados'
								WHEN icl.iclprazo < date(now()) THEN
									CASE
										WHEN ch1.entid IS NOT NULL AND val1.vldid IS NULL THEN 'Em Atraso'
										WHEN ch2.entid IS NOT NULL AND val2.vldid IS NULL THEN 'Em Atraso'
										WHEN ch3.entid IS NOT NULL AND val3.vldid IS NULL THEN 'Em Atraso'
									END
							ELSE 'A Executar'
							END
						WHEN ".$condicao." ch1.entid IS NOT NULL AND ch2.entid IS NULL AND ch3.entid IS NULL THEN
							CASE
								WHEN val1.vldid IS NOT NULL THEN 'Executados'
								WHEN icl.iclprazo < date(now()) THEN
									CASE
										WHEN ch1.entid IS NOT NULL AND val1.vldid IS NULL THEN 'Em Atraso'
										WHEN ch2.entid IS NOT NULL AND val2.vldid IS NULL THEN 'Em Atraso'
										WHEN ch3.entid IS NOT NULL AND val3.vldid IS NULL THEN 'Em Atraso'
									END
							ELSE 'A Executar'
							END
					END AS situacao
				FROM pde.itemchecklist icl
				INNER JOIN workflow.documento doc ON doc.docid = icl.docid
				INNER JOIN pde.atividade ati ON ati.atiid = icl.atiid
				LEFT JOIN pde.validacao val1 ON val1.iclid = icl.iclid AND val1.tpvid = 1
				LEFT JOIN pde.validacao val2 ON val2.iclid = icl.iclid AND val2.tpvid = 2
				LEFT JOIN pde.validacao val3 ON val3.iclid = icl.iclid AND val3.tpvid = 3
				LEFT JOIN pde.checklistentidade ch1 ON ch1.iclid = icl.iclid AND ch1.tpvid = 1
				LEFT JOIN pde.checklistentidade ch2 ON ch2.iclid = icl.iclid AND ch2.tpvid = 2
				LEFT JOIN pde.checklistentidade ch3 ON ch3.iclid = icl.iclid AND ch3.tpvid = 3
				WHERE icl.atiid in (select atiid from pde.atividade where $where and atistatus = 'A')
			) AS FOO
			LEFT JOIN (
				SELECT atiid, _atinumero, atidescricao
				FROM pde.atividade
				WHERE _atiprojeto = 114098
				AND _atiprofundidade = 2
				AND atistatus = 'A'
				AND ( $where )
				) n ON n._atinumero = FOO.numero
			GROUP BY numero, atidescricao, situacao
			ORDER BY numero, situacao DESC";
	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $dado){
			$arrSituacao[$dado['atidescricao']][$dado['situacao']]['total'][] = $dado['total'];
		}
	}
	?>
	<div>
		<img style="float:left" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
		<div style="float:left;" class="titulo_box" >
			<?=$nomeindicador?>
			<?php if ($subtitulo) { ?>
                <br><span class="subtitulo_box" ><?php echo $subtitulo; ?></span>
            <?php } ?>
		</div>
	</div>
	<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
		<?php if(in_array(26, $arrNumeroProjeto) || in_array(18, $arrNumeroProjeto) || in_array(19, $arrNumeroProjeto) || in_array(37, $arrNumeroProjeto) || in_array(38, $arrNumeroProjeto) || in_array(47, $arrNumeroProjeto) || in_array(48, $arrNumeroProjeto) || in_array(58, $arrNumeroProjeto) || in_array(59, $arrNumeroProjeto) || in_array(60, $arrNumeroProjeto)){ ?>
			<tr>
				<td class="center bold" colspan="5">Itens de Checklist</td>
			</tr>
			<tr>
				<td class="center bold">Descrição</td>
				<td class="center bold">Executados</td>
				<td class="center bold">Em Atraso</td>
				<td class="center bold">A Executar</td>
				<td class="center bold">Total</td>
			</tr>
		<?php }else{ ?>
			<tr>
				<th colspan="5">Itens de Checklist</th>
			</tr>
			<tr>
				<th>Descrição</th>
				<th>Executados</th>
				<th>Em Atraso</th>
				<th>A Executar</th>
				<th>Total</th>
			</tr>
		<?php
		}
		if($arrDados){
			$count = -1;
			foreach($arrSituacao as $chave => $sit):
				$somaExecutados = (is_array($sit['Executados']['total'])?array_sum($sit['Executados']['total']):0);
				$somaEmAtraso = (is_array($sit['Em Atraso']['total'])?array_sum($sit['Em Atraso']['total']):0);
				$somaAExecutar = (is_array($sit['A Executar']['total'])?array_sum($sit['A Executar']['total']):0);
				$count++;
				?>
				<tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
					<td class="" ><?php echo substr($chave,5) ?></td>
					<td class="link numero" onclick="abreRelatorioEnem('form=1&agrupadores[0]=atividades&cockpit=1&tipo=1&numero=<?php echo substr($chave,0,4) ?>&arvore_=true')"><?php $total_executado+=$somaExecutados; echo number_format($somaExecutados,0,",",".") ?></td>
					<td class="link numero" onclick="abreRelatorioEnem('form=1&agrupadores[0]=atividades&cockpit=1&tipo=2&numero=<?php echo substr($chave,0,4) ?>&arvore_=true')" ><?php $total_ematraso+=$somaEmAtraso; echo number_format($somaEmAtraso,0,",",".") ?></td>
					<td class="link numero" onclick="abreRelatorioEnem('form=1&agrupadores[0]=atividades&cockpit=1&tipo=3&numero=<?php echo substr($chave,0,4) ?>&arvore_=true')" ><?php $total_pendentes+=$somaAExecutar; echo number_format($somaAExecutar,0,",",".") ?></td>
					<td class="link numero" onclick="abreRelatorioEnem('form=1&agrupadores[0]=atividades&cockpit=1&numero=<?php echo substr($chave,0,4) ?>&prazovencido=nao&arvore_=true')" ><?php $total_geral+=($somaExecutados+$somaEmAtraso+$somaAExecutar); echo number_format($somaExecutados+$somaEmAtraso+$somaAExecutar,0,",",".") ?></td>
				</tr>
			<?php
			endforeach;
		} ?>
		<?php if(in_array(26, $arrNumeroProjeto) || in_array(18, $arrNumeroProjeto) || in_array(19, $arrNumeroProjeto) || in_array(37, $arrNumeroProjeto) || in_array(38, $arrNumeroProjeto) || in_array(47, $arrNumeroProjeto) || in_array(48, $arrNumeroProjeto) || in_array(58, $arrNumeroProjeto) || in_array(59, $arrNumeroProjeto) || in_array(60, $arrNumeroProjeto)){ ?>
			<tr>
				<td class="bold" >Total</th>
				<td class="bold numero" ><?php echo number_format($total_executado,0,",",".") ?></td>
				<td class="bold numero" ><?php echo number_format($total_ematraso,0,",",".") ?></td>
				<td class="bold numero" ><?php echo number_format($total_pendentes,0,",",".") ?></td>
				<td class="bold numero" ><?php echo number_format($total_geral,0,",",".") ?></td>
			</tr>
		<?php }else{ ?>
			<tr>
				<th class="bold" >Total</th>
				<th class="bold numero" ><?php echo number_format($total_executado,0,",",".") ?></th>
				<th class="bold numero" ><?php echo number_format($total_ematraso,0,",",".") ?></th>
				<th class="bold numero" ><?php echo number_format($total_pendentes,0,",",".") ?></th>
				<th class="bold numero" ><?php echo number_format($total_geral,0,",",".") ?></th>
			</tr>
		<?php } ?>
	</table>
<?php
}

# - permissaoPerfilConsultaCockpit: VERIFICA SE O USUÁRIO TEM PERMISSÃO DE ACESSO AO COCKPIT, CASO NÃO TENHNA, RETORNA A PAGINA DE MONITORAMENTO. AVERIFICAÇÃO É FEITA JUNTO AO USUÁRIO REPONSABILIDADE.
function permissaoPerfilConsultaCockpit($url_atual){
    global $db;

    $aryWhere[] = "rpustatus = 'A'";
    
    if($_SESSION['usucpf']){
    	$aryWhere[] = "ur.usucpf = '{$_SESSION['usucpf']}'";
    }
    
    if(PERFIL_CONSULTA_COCKPIT){
    	$aryWhere[] = "pflcod = ".PERFIL_CONSULTA_COCKPIT;
    }
    
    $sql = "SELECT  	c.cocid,
                		TRIM(REPLACE(c.coclink, '/pde/', '')) as coclink
        	FROM 		pde.usuarioresponsabilidade ur
        	INNER JOIN 	pde.cockpit c on c.cocid = ur.cocid
        				".(is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '')."";
    
    $projetos = $db->carregar($sql);

    if($projetos != ''){
        foreach ($projetos as $dados){
            $links[] = $dados['coclink'];
        }
        if( !in_array($url_atual, $links) ){
            $db->sucesso( 'principal/alinhamento_estrategico&acao=A', '', 'O seu perfil não dá permissão para acessar esse cockpit!', 'N', 'N' );
        }
    }
}

function montaCampoCountSupervisoresTutoresMaisMedicos($params = array())
{
	
	$stCamposContaSupTut = "
					--Subselect para calcular a quantidade de tutores cadastrados
					(select
						count(distinct tut.tutcpf)
					from maismedicos.tutor tut
					where tut.tutstatus = 'A'
					".($params['uniid']==true ? ' and tut.uniid = uni.uniid ' : '')."
					".($params['muncod']==true ? ' and tut.muncod = mun2.muncod ' : '')."
					and tut.tuttipo = 'T'
					and tut.tutvalidade is true) as qtde_tutores
	
					--Subselect para calcular a quantidade de supervisores cadastrados
					,(select
						count(distinct sup.tutcpf)
					from maismedicos.tutor sup
					where sup.tutstatus = 'A' 
					".($params['uniid']==true ? ' and sup.uniid = uni.uniid ' : '')."
					".($params['muncod']==true ? ' and sup.muncod = mun2.muncod ' : '')."
					and sup.tuttipo = 'S'
					and sup.tutvalidade is true) as qtde_supervisores
					
					";
	
	return $stCamposContaSupTut;
}

/* Pesquisar chave no Array */
function searcharray($value, $key, $array) {
    foreach ($array as $k => $val) {
        if ($val[$key] == $value) {
            return $k;
        }
    }
    return null;
}

function exibirTabelaFinanceiro($acaid){

    global $db;

    $sql = "SELECT vacid FROM planacomorc.vinculacaoacaoestrategica WHERE acaid = {$acaid} LIMIT 1";
    $vacid = $db->pegaUm($sql);

    if (isset($vacid)) {
        $sql = "SELECT
                    aet.ano,
                    aet.comentario,
                    aet.empenhado,
                    aet.pago,
                    aet.rap_npp,
                    aet.rap_pp
                FROM planacomorc.vwacaoestrategicatotal aet
                WHERE vacid = {$vacid}
                AND (empenhado > 0 OR pago > 0 OR rap_npp > 0 OR aet.rap_pp > 0)
                ORDER BY 1";
        $execucaoTotal = $db->carregar( $sql, null, 3200 );
        $arrValores = $execucaoTotal;
        foreach ($arrValores as $key => $value) {
            $saidaTabela[0]['tipo'] = 'Despesas Empenhadas (R$)';
            $saidaTabela[0][$value['ano']] = $execucaoTotal[searcharray($value['ano'], 'ano', $execucaoTotal)]['empenhado'];
        }
        foreach ($arrValores as $key => $value) {
            $saidaTabela[1]['tipo'] = 'Valores Pagos (R$)';
            $saidaTabela[1][$value['ano']] = $execucaoTotal[searcharray($value['ano'], 'ano', $execucaoTotal)]['pago'];
        }
        foreach ($arrValores as $key => $value) {
            $saidaTabela[2]['tipo'] = 'RAP não Processado Pago (R$)';
            $saidaTabela[2][$value['ano']] = $execucaoTotal[searcharray($value['ano'], 'ano', $execucaoTotal)]['rap_npp'];
        }
        foreach ($arrValores as $key => $value) {
            $saidaTabela[3]['tipo'] = 'RAP Processado Pago (R$)';
            $saidaTabela[3][$value['ano']] = $execucaoTotal[searcharray($value['ano'], 'ano', $execucaoTotal)]['rap_pp'];
        }
        echo "<table class='tabela_box link' cellpadding='2' cellspacing='1' width='100%' onclick=window.open('/planacomorc/planacomorc.php?modulo=sistema/tabelasapoio/popupImpressaoVinculacaoOrcamentaria&acao=A&vacid=".$vacid."','blank','height=500,width=1000,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');>";
        echo "  <tr>";
        echo "        <th>&nbsp;</th>";
        if($saidaTabela[0]['2012'] || $saidaTabela[1]['2012'] || $saidaTabela[2]['2012'] || $saidaTabela[3]['2012']){
            echo "      <th>2012</th>";
        }
        if($saidaTabela[0]['2013'] || $saidaTabela[1]['2013'] || $saidaTabela[2]['2013'] || $saidaTabela[3]['2013']){
            echo "      <th>2013</th>";
        }
        if($saidaTabela[0]['2014'] || $saidaTabela[1]['2014'] || $saidaTabela[2]['2014'] || $saidaTabela[3]['2014']){
            echo "      <th>2014</th>";
        }
        if($saidaTabela[0]['2015'] || $saidaTabela[1]['2015'] || $saidaTabela[2]['2015'] || $saidaTabela[3]['2015']){
            echo "      <th>2015</th>";
        }
        if($saidaTabela[0]['2016'] || $saidaTabela[1]['2016'] || $saidaTabela[2]['2016'] || $saidaTabela[3]['2016']){
            echo "      <th>2016</th>";
        }
        echo "  </tr>";
        for ($i = 0; $i <= 3; $i++) {
            echo "  <tr class=".(($i%2) ? 'zebrado' : '').">";
            echo "      <td class='' >".$saidaTabela[$i]['tipo']."</td>";
            if($saidaTabela[0]['2012'] || $saidaTabela[1]['2012'] || $saidaTabela[2]['2012'] || $saidaTabela[3]['2012']){
             echo "      <td class='numero'>".number_format($saidaTabela[$i]['2012'],2,',','.')."</td>";
            }
            if($saidaTabela[0]['2013'] || $saidaTabela[1]['2013'] || $saidaTabela[2]['2013'] || $saidaTabela[3]['2013']){
                echo "      <td class='numero'>".number_format($saidaTabela[$i]['2013'],2,',','.')."</td>";
            }
            if($saidaTabela[0]['2014'] || $saidaTabela[1]['2014'] || $saidaTabela[2]['2014'] || $saidaTabela[3]['2014']){
                echo "      <td class='numero'>".number_format($saidaTabela[$i]['2014'],2,',','.')."</td>";
            }
            if($saidaTabela[0]['2015'] || $saidaTabela[1]['2015'] || $saidaTabela[2]['2015'] || $saidaTabela[3]['2015']){
                echo "      <td class='numero'>".number_format($saidaTabela[$i]['2015'],2,',','.')."</td>";
            }
            if($saidaTabela[0]['2016'] || $saidaTabela[1]['2016'] || $saidaTabela[2]['2016'] || $saidaTabela[3]['2016']){
                echo "      <td class='numero'>".number_format($saidaTabela[$i]['2016'],2,',','.')."</td>";
            }
            echo "  </tr>";
        }
        echo "</table>";
    }
}

function montaTabelaCiclo($indicador){
	
	global $db;
	
	$sql = "SELECT atidescricao, sum(totalitens) AS totalitens, atiordem, sum(aexecutar) as aexecutar, sum(executado) as executado, sum(atrasado) as atrasado, sum(exeatrasado) as exeatrasado
			FROM (
				SELECT a2.atidescricao,
					count(0) as totalitens, a2.atiordem, 0 as aexecutar, 0 as executado, 0 as atrasado, 0 as exeatrasado
				FROM pde.atividade a1
				INNER JOIN pde.atividade a2 on a2.atiidpai = a1.atiid AND a2.atistatus = 'A'
				INNER JOIN pde.atividade a3 on a3.atiidpai = a2.atiid AND a3.atistatus = 'A'
				LEFT JOIN pde.atividade a4 on a4.atiidpai = a3.atiid AND a4.atistatus = 'A'
				LEFT JOIN pde.monitoraitemchecklist mic on (mic.atiid = a3.atiid OR mic.atiid = a4.atiid) AND mic.micstatus = 'A'
				INNER JOIN pde.monitorameta mnm ON mnm.micid = mic.micid AND mnm.mnmstatus = 'A'
				INNER JOIN painel.detalhemetaindicador i on i.metid = mnm.metid AND i.dmistatus = 'A'
				WHERE a1.atiid = ".$indicador."
				AND a1.atistatus = 'A'
				GROUP BY a2.atiordem, a2.atidescricao
			UNION ALL
				SELECT a2.atidescricao, 0 as totalitens, a2.atiordem,
				case
					when doc.esdid = 443 and i.dmidatameta >= CURRENT_DATE and i.dmidatameta <= CURRENT_DATE+5 then 1
					ELSE 0
				end as aexecutar,
				case
					when doc.esdid in (444, 445) then 1
					ELSE 0
				end as executado,
				case
					when doc.esdid = 443 and i.dmidatameta < CURRENT_DATE then 1
					else 0
				end as atrasado,
				case
					when doc.esdid in (444, 445) and i.dmidatameta < i.dmidataexecucao then 1
					else 0
				end as exeatrasado
				FROM pde.atividade a1
				INNER JOIN pde.atividade a2 on a2.atiidpai = a1.atiid AND a2.atistatus = 'A'
				INNER JOIN pde.atividade a3 on a3.atiidpai = a2.atiid AND a3.atistatus = 'A'
				LEFT JOIN pde.atividade a4 on a4.atiidpai = a3.atiid AND a4.atistatus = 'A'
				LEFT JOIN pde.monitoraitemchecklist mic on (mic.atiid = a3.atiid OR mic.atiid = a4.atiid) AND mic.micstatus = 'A'
				INNER JOIN pde.monitorameta mnm ON mnm.micid = mic.micid AND mnm.mnmstatus = 'A'
				INNER JOIN painel.detalhemetaindicador i on i.metid = mnm.metid AND i.dmistatus = 'A'
				INNER JOIN workflow.documento doc ON doc.docid = i.docid
				WHERE a1.atiid = ".$indicador."
				AND a1.atistatus = 'A'
			) as foo
			GROUP BY atiordem, atidescricao
			ORDER BY atiordem, atidescricao";
	
	$arrDados = $db->carregar( $sql, null, 3200 );
	$contador = 1;
	$totalgeral = 0;
	
	if($arrDados){
		$i=0;
		foreach($arrDados as $dado){
			++$i;
			$porcentagem2 += ($dado['executado']/$dado['totalitens'])*100;
		}
	}
	
	?>
	<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" <?php echo $oculta==true ? 'style="display:none;"' : ''; ?>>
		<tr>
			<td class="center bold" rowspan="2">Etapas</td>
			<td class="center bold" rowspan="2" colspan="2">Progresso</td>
			<td class="center bold" colspan="2">Ponto de Atenção</td>
		</tr>
		<tr>
			<td class="center bold">Em<br>Atraso</td>
			<td class="center bold">A Executar<br>(próximos 5 dias)</td>
		</tr>
		<?php
		if($arrDados){
			foreach($arrDados as $dado):
			$porcentagem = number_format(($dado['executado']/$dado['totalitens'])*100,0,",",".");
			?>
			<tr>
				<td class="link" onclick="abreIndicadores(<?=$indicador?>, <?=$dado['atiordem']?>);" ><?php echo $dado['atidescricao'] ?></td>
				<td class="numero" >
					<div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 15px; width: 100px;'>
						<div style='background-color: #80BC44; text-align: right; color: white; height: 15px; width: <?= $porcentagem ?>px;'><?= $porcentagem ?>%&nbsp;</div>
					</div>
				</td>
				<td class=" numero"><?php echo $dado['totalitens'] ?></td>
				<td style="<?=$dado['atrasado'] >0?'background-color:#BB0000':''?>;" class="numero" ><?php echo $dado['atrasado'] ?></td>
				<td style="<?=$dado['aexecutar']>0?'background-color:#FFC211':''?>;" class="numero" ><?php echo $dado['aexecutar'] ?></td>
			</tr>
			<?php
			$contador += 1;
			$totalgeral += $dado['totalitens'];
			$totalatrasado += $dado['atrasado'];
			$totalaexecutar += $dado['aexecutar'];
			endforeach;
		}
		?>
		<tr>
			<td class=" bold" >Total</td>
			<td class=" numero" width="100">
				<?php $porcentagem2 = number_format($porcentagem2/$i,0,",","."); ?>				
				<div title="Abrir indicador" alt="Abrir indicador" onclick="abreIndicadores(<?=$indicador?>, '');" style='cursor:pointer;border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 100px;'>
					<div style='font-size:18px;background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $porcentagem2 ?>px;'><?= $porcentagem2 ?>%&nbsp;</div>
				</div>
			</td>
			<td class=" numero" width="100"><?php echo $totalgeral ?></td>
			<td class=" numero" width="100"><?php echo $totalatrasado ?></td>
			<td class=" numero" width="100"><?php echo $totalaexecutar ?></td>
		</tr>
	</table>
<?php
}

function montaColunaCategoriaPronatec(){
    $colunaSituacao = "CASE
                            WHEN dsh.tidid1 IN (5214,5219,5218,8110,8114,8112) OR dsh.tidid2 IN (6032,6041,6046,6055,6060,6069,6074,6083,6088,6097,6102,6111,6116,6125,6916,6921,6926,6931,6936,6941,6946,6953,6960,6971,6978,7449,7458,7465,8048,8057,8064,7877,7881,7879,9271,9275,9273,9291,9292,9294,9311,9303,9307,9330,9322,9326,10221,10223,10225,10234,10236,10238) THEN 'Conclusao'
                            WHEN dsh.tidid1 IN (5216,8111) OR dsh.tidid2 IN (6033,6035,6039,6047,6049,6053,6061,6063,6067,6075,6077,6081,6089,6091,6095,6103,6105,6109,6117,6120,6123,6955,6962,6973,6980,7450,7452,7456,8049,8051,8055,7878,9272,9287,9288,9297,9313,9304,9306,9323,9332,9325,10222,10235) THEN 'Em curso'
                            WHEN dsh.tidid1 IN (5217,5215,8109,8108) OR dsh.tidid2 IN (6037,6051,6065,6079,6093,6107,6121,6915,6920,6925,6930,6935,6940,6945,6956,6963,6974,6981,7454,7464,8053,8063,7876,7875,9270,9269,9293,9296,6979,6972,6961,6954,9310,9314,9305,9329,9324,9333,10219,10220,10232,10233) THEN 'Abandono'
                            WHEN dsh.tidid1 IN (5220,9285,8113,10212) OR dsh.tidid2 IN (6034,6043,6044,6048,6057,6058,6062,6071,6072,6076,6085,6086,6090,6099,6100,6104,6113,6114,6119,6127,6128,6957,6964,6975,6982,7451,7460,7461,8050,8059,8060,7880,9274,9289,9290,9295,9312,9309,9308,9331,9327,9328,10224,10237) THEN 'Interrupcao'
                        ELSE 'Outras'
                        END ";
    return $colunaSituacao;
}