<?php
function listaProjetosMobile(){
	global $mobile;
	$sql = "
			select
				'estrategico.php?modulo=principal/mobile_estrategico&acao=A&titulo_pagina=Metas&requisicao=listaMetas&nomeprojeto=' || a.atidescricao || '&projeto=' || a.atiid as link,
				a.atidescricao as descricao,
				(
					select 
						distinct count(met.metid)
					from
						pde.atividade ati
					inner join
						pde.monitoraitemchecklist mic ON mic.atiid = ati.atiid
					inner join
						painel.indicador ind ON ind.indid = mic.indid
					inner join
						painel.metaindicador met ON met.indid = ind.indid
					inner join
						painel.detalhemetaindicador dmi ON met.metid = dmi.metid
					where
						ati.atistatus = 'A'
					and
						mic.micstatus = 'A'
					and
						ind.indstatus = 'I'
					and
						met.metstatus = 'A'
					and
						dmi.dmistatus = 'A'
					and
						dmi.docid is not null
					and
						dmi.dmidatameta is not null
					and
						a.atiid = ati._atiprojeto
				) || ' meta(s)' as total
			from pde.atividade a
			where
				 a.atiestrategico = true and
				 a.atiidpai is null and
				a.atistatus = 'A' and
				(a.atiid != " . PROJETO_PDE . " AND a.atiid != " . PROJETOENEM . " AND a.atiid != " . PROJETOSEB . " )
			order by
				a.atidescricao
			";
		$mobile->monta_lista($sql);
}

function listaMetas(){
	global $mobile;
	$projeto = $_GET['projeto'];
	$data = $_POST['data'] ? $_POST['data'] : date("d/m/Y", mktime(0,0,0,date("m")+1,date("d"),date("Y")));
	if($data){
		$arrWhere[] = "dmi.dmidatameta::date <= '".formata_data_sql($data)."'";
	}
	
	$sql = "select
			ati.atiid,
			ati.atidescricao,
			ati._atinumero,
			ati.atidatainicio,
			ati.atidatafim,
			count( distinct met.metid) as total_metas
		from
			pde.atividade ati
		inner join
			pde.monitoraitemchecklist mic ON mic.atiid = ati.atiid
		inner join
			painel.indicador ind ON ind.indid = mic.indid
		inner join
			painel.metaindicador met ON met.indid = ind.indid
		inner join
			painel.detalhemetaindicador dmi ON met.metid = dmi.metid
		--inner join
			--painel.seriehistorica seh ON seh.dmiid = dmi.dmiid
		inner join
			pde.atividade proj ON proj.atiid = ati._atiprojeto
		where
			ati.atistatus = 'A'
		and
			mic.micstatus = 'A'
		and
			ind.indstatus = 'I'
		--and
			--ind.unmid != ".UNIDADEMEDICAO_BOLEANA."
		and
			met.metstatus = 'A'
		and
			dmi.dmistatus = 'A'
		and
			dmi.docid is not null
		and
			dmi.dmidatameta is not null
		and
			ati._atiprojeto = $projeto
		".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
		group by
			ati.atiid,
			ati.atidescricao,
			ati._atinumero,
			ati.atidatainicio,
			ati.atidatafim
		order by
			ati._atinumero";
	
	$arrAtividades = $mobile->carregar($sql);
	
	if($arrAtividades){
		?> 
		<ul data-role="listview" data-theme="d" data-divider-theme="d"> <?php
		foreach($arrAtividades as $ati){
			?> <li data-role="list-divider" ><?php echo $ati['_atinumero']." ".$ati['atidescricao']; ?> <br /><span style="color:#008000;font-weight:normal;font-size:11px;" >(<?php echo formata_data($ati['atidatainicio']) ?> - <?php echo formata_data($ati['atidatafim']) ?>)</span><span class="ui-li-count"><?php echo $ati['total_metas'] ?></span></li> <?php
			?>
			<?php $arrMetas  = recuperaMetasPorAtividade($ati['atiid'])?>
			<?php foreach($arrMetas as $met): ?>
				<?php
				$sql = "select 
							dmi.dmiid,
							dmi.dmiqtde as meta,
							dmi.dmivalor,
							dmi.docid,
							dpe.dpedsc as referencia,
							dmi.dmidataexecucao,
							dmi.dmidatavalidacao::date as dmidatavalidacao,
							dmi.dmdestavel as mnmqtdestavel,
							dmi.dmdcritico as mnmqtdcritico
						from 
							painel.detalhemetaindicador dmi
						inner join
							pde.monitorameta mnm ON mnm.metid = dmi.metid
						inner join
							painel.detalheperiodicidade dpe ON dpe.dpeid = dmi.dpeid
						inner join
							workflow.documento doc ON doc.docid = dmi.docid --and doc.esdid = ".WK_ESTADO_DOC_FINALIZADO."
						where 
							dmi.dmistatus = 'A'
						and
							dmi.dmidatameta is not null 
						and
							mnm.metid = {$met['metid']}
						".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
						order by
							--dpedatafim desc
							dmi.dmidatameta 
						limit 1";
				$arrDados = $mobile->pegaLinha($sql);
				if($arrDados['dmidataexecucao'] && $met['indid'] && $arrDados['dmiid']){
					$sql = "select 
								sehvalor as mnmvalor,
								sehqtde as mnmqtd,
								sehdtcoleta::date as mvddata 
							from
								painel.seriehistorica
							where
								indid = {$met['indid']}
							and
								dmiid = {$arrDados['dmiid']}
							--and
								--sehdtcoleta::date between '{$arrDados['dmddatainiexecucao']}' and '{$arrDados['dmddatafimexecucao']}'
							and
								sehstatus = 'A'
							order by
								sehid desc
							limit
								1";
					$arrExecucao = $mobile->pegaLinha($sql);
				}else{
					$arrExecucao = false;
				}
				//Verifica se a execução foi realizada em atrazo
				if($arrExecucao){
					if((int)str_replace("-","",$arrExecucao['mvddata']) > (int)str_replace("-","",$arrDados['dmddatafimexecucao'])){
						$atraso = " <span style=\"cursor:pointer\" title='Execução realizada com atraso' >(A)</span>";
					}else{
						$atraso = "";	
					}
				}else{
					$atraso = "";
				}
				if($arrExecucao['mnmqtd'] && $arrDados['meta']){
					
					$arrExecucao['mnmqtd'] = (float)$arrExecucao['mnmqtd'];
					$arrDados['meta'] = (float)$arrDados['meta'];
					$porcentagem = round((($arrExecucao['mnmqtd'] ? $arrExecucao['mnmqtd'] : 1)/($arrDados['meta'] ? $arrDados['meta'] : 1))*100,2);
				}else{
					$porcentagem = 0;
				}
				
				//Verifica se o executado é maior que a meta
				if($arrExecucao['mnmqtd'] > $arrDados['meta']){
					$porcentagem = 100;
				}
				
				if($met['estid'] == 2){ //Menor melhor
					$img_indicador = "indicador-vermelha.png";
				}else{
					$img_indicador = "indicador-verde.png";
				}

				if($met['unmid'] == UNIDADEMEDICAO_BOLEANA || $met['mtmid'] == 1){
					if($arrDados['referencia'] && $arrExecucao['mvddata']){
							if(strlen($atraso) > 5){
								$cor_td = "#E95646";
							}else{
								$cor_td = "#80BC44";
							}
						}else{
							$cor_td = "";
						}	
				}
				
				if($met['estid'] == 2 && $arrDados['mnmqtdestavel'] && $arrDados['mnmqtdcritico']){ //Menor melhor
					$arrMedidor[0] = array("inicio" => 0, "fim" => $arrDados['mnmqtdestavel'], "cor" => "#80BC44", "bgcolor" => "#80BC44");
					$arrMedidor[1] = array("inicio" => $arrDados['mnmqtdestavel'], "fim" => $arrDados['mnmqtdcritico'], "cor" => "#FFFF00", "bgcolor" => "#FFC211");
					$arrMedidor[2] = array("inicio" => $arrDados['mnmqtdcritico'], "fim" => 100, "cor" => "#E95646", "bgcolor" => "#E95646");
				}elseif($met['estid'] != 2 && $arrDados['mnmqtdestavel'] && $arrDados['mnmqtdcritico']){ //Maior Melhor
					$arrMedidor[0] = array("inicio" => 0, "fim" => $arrDados['mnmqtdcritico'], "cor" => "#E95646", "bgcolor" => "#E95646");
					$arrMedidor[1] = array("inicio" => $arrDados['mnmqtdcritico'], "fim" => $arrDados['mnmqtdestavel'], "cor" => "#FFC211", "bgcolor" => "#FFC211");
					$arrMedidor[2] = array("inicio" => $arrDados['mnmqtdestavel'], "fim" => 100, "cor" => "#80BC44", "bgcolor" => "#80BC44");
				}
				$valor = $porcentagem;
				
				$arrExecutor = false;
				$arrValidador = false;
				
				if($met['mtmid']){
					$arrExecutor = recuparMonitoraMetaExecutor($met['mtmid']);
					$arrValidador = recuparMonitoraMetaValidador($met['mtmid']);
				}
				
				?>
				<li>
					<h3 style="color:#4169E1;cursor:pointer" onclick="javascript:exibeMetas('<?php echo $met['micid'] ?>','<?php echo $_GET['nomeprojeto'] ?>','<?php echo $_GET['projeto'] ?>','<?php echo $met['indnome'] ?>')" ><?php echo $met['indnome'] ?></h3>
					<?php if($met['mtidsc']): ?>
						<p><strong>Tipo: </strong><?php echo $met['mtidsc'] ? $met['mtidsc'] : "N/A" ?></p>
					<?php endif; ?>
					<p><strong>Executor: </strong><?php echo $arrExecutor['nome_executor'] ? $arrExecutor['nome_executor'] : "N/A" ?></p>
					<p><strong>Validador: </strong><?php echo $arrValidador['nome_validador'] ? $arrValidador['nome_validador'] : "N/A" ?></p>
					<div class="ui-li-aside">
						<?php if($met['unmid'] == UNIDADEMEDICAO_BOLEANA || $met['mtmid'] == 1): ?>
							<?php if($arrExecucao['mvddata']): ?>
								<div onclick="javascript:exibeMetas('<?php echo $met['micid'] ?>','<?php echo $_GET['nomeprojeto'] ?>','<?php echo $_GET['projeto'] ?>','<?php echo $met['indnome'] ?>')" style="cursor:pointer;text-align:right;width:100px;height:30px;color:#FFFFFF;text-shadow:none;float:right;padding:8px;background-color:<?php echo $cor_td ?>" >
								<span style="font-size:12px;" >Executado<br />em <?php echo formata_data($arrExecucao['mvddata']) ?></span>
								</div>
								<div style="margin-right:10px;text-align:right;width:100px;height:50px;color:#333;text-shadow:none;float:right;padding:8px;background-color:#ffffff" >
								<span style="font-size:12px" >Meta:</span><br />
								<span style="font-size:10px;font-weight:normal;" ><?php echo $arrDados['referencia'] ?></span>
								<input type="hidden" name="hdn_cor_<?php echo str_replace("#","",$cor_td) ?>" value="1" />
								</div>
							<?php else: ?>
								<div style="text-align:right;width:100px;height:30px;color:#FFFFFF;text-shadow:none;float:right;padding:8px;background-color:#ffffff" >
								<span style="font-size:12px;color:#000000" >Não executado</span>
								<input type="hidden" name="hdn_nao_executado" value="1" />
								</div>
								<div style="margin-right:10px;text-align:right;width:100px;height:50px;color:#333;text-shadow:none;float:right;padding:8px;background-color:#ffffff" >
								<span style="font-size:12px" >Meta:</span><br />
								<span style="font-size:10px;font-weight:normal;" ><?php echo $arrDados['referencia'] ?></span>
								</div>
							<?php endif; ?>
						<?php else: ?>
							<?php if($arrExecucao['mvddata']): ?>
								<div onclick="javascript:exibeMetas('<?php echo $met['micid'] ?>','<?php echo $_GET['nomeprojeto'] ?>','<?php echo $_GET['projeto'] ?>','<?php echo $met['indnome'] ?>')" style="cursor:pointer;text-align:right;width:100px;height:50px;color:#FFFFFF;text-shadow:none;float:right;padding:8px;background-color:<?php echo retornaCorMeta($arrMedidor,$valor) ?>" >
								<span style="font-size:12px" >Executado<br/></span>
								<span style="font-size:18px;font-weight:bold" ><?php echo number_format($arrExecucao['mnmqtd'],0,'','.') ?></span>
								<span style="font-size:12px;" >(<?php echo str_replace(".",",",$porcentagem) ?>%)</span><br />
								<span style="font-size:12px;" >em <?php echo $arrExecucao['mvddata'] ? formata_data($arrExecucao['mvddata']) : "N/A" ?></span>
								<input type="hidden" name="hdn_cor_<?php echo str_replace("#","",retornaCorMeta($arrMedidor,$valor)) ?>" value="1" />
								</div>
								<div style="margin-right:10px;text-align:right;width:100px;height:50px;color:#333;text-shadow:none;float:right;padding:8px;background-color:#ffffff" >
								<span style="font-size:12px" >Meta: <?php echo number_format($arrDados['meta'],0,'','.') ?></span><br />
								<span style="font-size:10px;font-weight:normal;" ><?php echo $arrDados['referencia'] ?></span>
								</div>
							<?php else: ?>
								<div style="text-align:right;width:100px;height:30px;color:#FFFFFF;text-shadow:none;float:right;padding:8px;background-color:#ffffff" >
								<span style="font-size:12px;color:#000000" >Não executado</span>
								<input type="hidden" name="hdn_nao_executado" value="1" />
								</div>
								<div style="margin-right:10px;text-align:right;width:100px;height:50px;color:#333;text-shadow:none;float:right;padding:8px;background-color:#ffffff" >
								<span style="font-size:12px" >Meta: <?php echo number_format($arrDados['meta'],0,'','.') ?></span><br />
								<span style="font-size:10px;font-weight:normal;" ><?php echo $arrDados['referencia'] ?></span>
								</div>
							<?php endif; ?>
						<?php endif; ?>
					</div>
				</li>
			<?php endforeach; ?>
			<?php
		}
		?> </ul> <?php
	}
	
}

function recuperaMetasPorAtividade($atiid)
{
	global $mobile;
	$sql = "select
				*,
				to_char((select max(dmi.dmidatameta) from painel.detalhemetaindicador dmi where dmi.metid = mmi.metid limit 1),'DD/MM/YYYY') as data_meta
			from
				 pde.monitoraitemchecklist mic
			inner join
				painel.indicador ind ON ind.indid = mic.indid
			left join
				painel.eixo exo ON exo.exoid = ind.exoid
			left join
				painel.acao aca ON aca.acaid = ind.acaid 
			left join
				pde.monitorameta mmi ON mic.micid = mmi.micid
			left join
				pde.monitoratipoindicador mti ON mti.mtiid = mic.mtiid
			where
				atiid = $atiid
			and
				micstatus = 'A'
			and
				mnmstatus = 'A'
			and
				ind.indstatus = 'I'
			order by
				mic.micordem";
	$arrDados = $mobile->carregar($sql);
	return $arrDados ? $arrDados : array();
}

function exibeMetas($micid = null)
{
	global $mobile;
	$micid = $_GET['micid'];
	if(!$micid){
		return false;
	}
	$sql = "select 
			*
		from 
			painel.metaindicador met
		inner join
			painel.indicador ind ON ind.indid = met.indid
		inner join
			pde.monitoraitemchecklist mic ON mic.indid = ind.indid
		inner join
			painel.unidademeta ume ON ind.umeid = ume.umeid
		inner join
			painel.detalhemetaindicador dmi ON dmi.metid = met.metid  
		where 
			mic.micid = $micid
		and
			ind.indstatus = 'I'
		and
			met.metstatus = 'A'
		order by
			met.metid";
	
	$arrDados = $mobile->pegaLinha($sql);
	$metid = $arrDados['metid'];
	$umedesc = $arrDados['umedesc'];
	$_SESSION['indid'] = $arrDados['indid'];
	$formatoinput = pegarFormatoInput($arrDados['indid']);
	$formatoinput['campovalor']['mascara'] = !$formatoinput['campovalor']['mascara'] ? "###.###.###.###.###,##" : $formatoinput['campovalor']['mascara'];
	$unmid = $arrDados['unmid'];
	if($arrDados['unmid'] != UNIDADEMEDICAO_BOLEANA){
		$sql = "select
				dpe.dpedsc,
				CASE WHEN dmi.dmidatameta IS NOT NULL
					THEN to_char(dmi.dmidatameta,'DD/MM/YYYY')
					ELSE 'N/A' 
				END as dmidatameta,
				CASE WHEN dmi.dmidataexecucao IS NOT NULL
					THEN to_char(dmi.dmidataexecucao,'DD/MM/YYYY')
					ELSE 'N/A' 
				END as dtexecucao,
				CASE WHEN dmi.dmidatavalidacao IS NOT NULL
					THEN to_char(dmi.dmidatavalidacao,'DD/MM/YYYY')
					ELSE 'N/A' 
				END as dmidatavalidacao,
				seh.sehqtde,
				to_char(seh.sehdtcoleta,'dd/mm/yyyy') as sehdtcoleta,
				dmi.dmiqtde
			from
				painel.detalhemetaindicador dmi
			inner join
				painel.detalheperiodicidade dpe ON dmi.dpeid = dpe.dpeid
			inner join
				painel.seriehistorica seh ON seh.dmiid = dmi.dmiid
			where
				dmi.metid = $metid
			and
				dmi.dmistatus = 'A'
			order by
				dmi.dmidatameta";
		$arrDados = $mobile->carregar($sql);
		$cabecalho = array("Data da Meta","Data de Execução","Valor da Meta","Valor Executado","Porcentagem Executada");
	}else{
		$sql = "select
				dpe.dpedsc,
				CASE WHEN dmi.dmidatameta IS NOT NULL
					THEN to_char(dmi.dmidatameta,'DD/MM/YYYY')
					ELSE 'N/A' 
				END as dmidatameta,
				CASE WHEN dmi.dmidataexecucao IS NOT NULL
					THEN to_char(dmi.dmidataexecucao,'DD/MM/YYYY')
					ELSE 'N/A' 
				END as dtexecucao,
				CASE WHEN dmi.dmidatavalidacao IS NOT NULL
					THEN to_char(dmi.dmidatavalidacao,'DD/MM/YYYY')
					ELSE 'N/A' 
				END as dmidatavalidacao,
				to_char(seh.sehdtcoleta,'dd/mm/yyyy') as sehdtcoleta
			from
				painel.detalhemetaindicador dmi
			inner join
				painel.detalheperiodicidade dpe ON dmi.dpeid = dpe.dpeid
			inner join
				painel.seriehistorica seh ON seh.dmiid = dmi.dmiid
			where
				dmi.metid = $metid
			and
				dmi.dmistatus = 'A'
			order by
				dmi.dmidatameta";
		$arrDados = $mobile->carregar($sql);
		$cabecalho = array("Data da Meta","Data de Execução");
	}
	
	$arrAlfabeto = array("a","b","c","d","e","f","g");
		
	if($arrDados && $unmid != UNIDADEMEDICAO_BOLEANA): ?>
		<div class="ui-grid-d">
			<div class="ui-bar ui-bar-d"></div>
		<?php foreach($cabecalho as $k => $cab): ?>
			<div style="text-align:center;height:30px" class="ui-block-<?php echo $arrAlfabeto[$k] ?>"><div class="ui-bar ui-bar-b"><?php echo $cab ?></div></div>
		<?php endforeach;?>
		<?php $n=0;foreach($arrDados as $dado): ?>
			<?php 
				if($dado['sehqtde'] && $dado['dmiqtde']){
					$dado['sehqtde'] = (float)$dado['sehqtde'];
					$dado['dmiqtde'] = (float)$dado['dmiqtde'];
					$porcentagem = round((($dado['sehqtde'] ? $dado['sehqtde'] : 1)/($dado['dmiqtde'] ? $dado['dmiqtde'] : 1))*100,2);
				}else{
					$porcentagem = 0;
				}
				//Verifica se o executado é maior que a meta
				if($dado['sehqtde'] > $dado['dmiqtde']){
					$porcentagem = 100;
				}
				$porcentagem = str_replace(".",",",$porcentagem);
			 ?>
			<div style="height:30px" class="ui-block-<?php echo $arrAlfabeto[$n+0] ?>"><div style="text-align:right" class="ui-bar ui-bar-c"><?php $arrData[] = $dado['dmidatameta']; echo  $dado['dmidatameta'] ?></div></div>
			<div style="height:30px" class="ui-block-<?php echo $arrAlfabeto[$n+1] ?>"><div style="text-align:right" class="ui-bar ui-bar-c"><?php echo  $dado['sehdtcoleta'] ?></div></div>
			<div style="height:30px" class="ui-block-<?php echo $arrAlfabeto[$n+2] ?>"><div style="text-align:right" class="ui-bar ui-bar-c"><?php $arrMeta[] = $dado['dmiqtde']; echo  number_format($dado['dmiqtde'],0,'','.') ?></div></div>
			<div style="height:30px" class="ui-block-<?php echo $arrAlfabeto[$n+3] ?>"><div style="text-align:right" class="ui-bar ui-bar-c"><?php $arrExecutada[] = $dado['sehqtde']; echo  number_format($dado['sehqtde'],0,'','.') ?></div></div>
			<div style="height:30px" class="ui-block-<?php echo $arrAlfabeto[$n+3] ?>"><div style="text-align:right" class="ui-bar ui-bar-c"><?php echo  $porcentagem ?>%</div></div>
		<?php ;endforeach; ?>
		<div class="ui-bar ui-bar-d"></div>
		</div>
		<link type="text/css" href="/includes/jquery-jqplot-1.0.0/jquery.jqplot.min.css" rel="stylesheet" />
		<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/jquery.jqplot.min.js"></script>
		<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.logAxisRenderer.js"></script>
    	<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.categoryAxisRenderer.min.js"></script>
    	<script type="text/javascript" src="/includes/jquery-jqplot-1.0.0/plugins/jqplot.pointLabels.min.js"></script>
		
		<script>
			jQuery(document).ready(function(){
				var line1 = [<?php echo implode(",",$arrMeta) ?>];
				var line2 = [<?php echo implode(",",$arrExecutada) ?>];
				var ticks = ['<?php echo implode("','",$arrData) ?>'];
			 	jQuery.jqplot('chartdiv',[line1,line2],{
				 	seriesDefaults:{
			            renderer:jQuery.jqplot.BarRenderer,
			            rendererOptions: {fillToZero: true}
			        },
				 	series:[
				            {label:'Valores das Metas'},
				            {label:'Valores Executados'},
				        ],
				    legend: {
			            show: true,
			            placement: 'outsideGrid'
			        },
			         axes: {
			            // Use a category axis on the x axis and use our custom ticks.
			            xaxis: {
			                renderer: jQuery.jqplot.CategoryAxisRenderer,
			                ticks: ticks
			            },
			            // Pad the y axis just a little so bars can get close to, but
			            // not touch, the grid boundaries.  1.2 is the default padding.
			            yaxis: {
			                pad: 1.05,
			                tickOptions: {formatString: '%d'}
			            }
			        }
			 	});
			});
		</script>
		<center>
			<div id="chartdiv" style="margin-top:10px;height:70%;width:60%; "></div>
		</center>
	<?php else: ?>
		<div class="ui-grid-a">
			<div class="ui-bar ui-bar-d"></div>
		<?php foreach($cabecalho as $k => $cab): ?>
			<div style="text-align:center;height:30px" class="ui-block-<?php echo $arrAlfabeto[$k] ?>"><div class="ui-bar ui-bar-b"><?php echo $cab ?></div></div>
		<?php endforeach;?>
		<?php $arrDados = !$arrDados ? array() : $arrDados ?>
		<?php $n=0;foreach($arrDados as $dado):?>
			<div style="height:30px" class="ui-block-<?php echo $arrAlfabeto[$n+0] ?>"><div  style="text-align:right" class="ui-bar ui-bar-c"><?php echo  $dado['dmidatameta'] ?></div></div>
			<div style="height:30px" class="ui-block-<?php echo $arrAlfabeto[$n+1] ?>"><div  style="text-align:right" class="ui-bar ui-bar-c"><?php echo  $dado['sehdtcoleta'] ?></div></div>
		<?php ;endforeach; ?>
		<div class="ui-bar ui-bar-d"></div>
		</div>
	<?php endif;
}