<!DOCTYPE HTML>

<html>

    <head>
    
        <meta http-equiv="X-UA-Compatible" content="IE=9" />
        <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
        <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

        <script language="javascript" type="text/javascript" src="/library/jquery/jquery-1.10.2.js"></script>
        
        <!--  
        <script language="javascript" type="text/javascript" src="/includes/jquery-cycle/jquery.cycle.all.js"></script>

        <link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
        <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/jquery.mousewheel.js"></script>
        <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/perfect-scrollbar.js"></script>

        <script language="javascript" src="/includes/Highcharts-3.0.0/js/highcharts.js"></script>
        <script language="javascript" src="/includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

        <link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
        <script language="javascript" type="text/javascript" src="/library/jquery_totem/jquery.totemticker.min.js"></script>
		-->
		
        <link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='/includes/listagem.css'/>
        <link rel='stylesheet' type='text/css' href='css/painel.css'/>

        <script language="JavaScript" src="/includes/funcoes.js"></script>
        
        <script>
	        function refreshAutomatico() {
	        	setTimeout("location.reload(true);",3600000);
	        }                
        </script>

        <style type="text/css">
        	body{
				background-image:url('/imagens/degrade-fundo-preto.png');
				background-repeat:repeat-x;
				background-color:#004500;
				margin:0px;
				padding-top:0px;
			}
			.div_topo{
				padding: 15px;
				color:#FFFFFF;
				width:48%;
				font-weight:bold;
			}
            .cabecalho_tb{
            	clear: both;
            	background-image:url('images/bannerMaisMedicos.jpg');
            	background-repeat:repeat-xt;
    			background-position:2px -50px;
            	color:#FFFFFF;
            	height: 172px;
            	/* margin: 15px; 
            	max-width: 1431; 
            	width: 95%;
            	align: center; */
    			
            }
            
            .cabecalho_td{
            	text-shadow:#000000 0px 4px 2px;    
			    font-weight:bold;
			    font-size:30px; 
            }
            
            .cabecalho_tb_new{
            	clear: both;
            	background-image:url('images/bannerPainel_p.jpg');
            	/*background-repeat:repeat-xt;
    			background-position:2px -50px; */
            	color:#FFFFFF;
            	            	
            }
            .cabecalho_td_new{
            	text-shadow:#000000 0px 4px 2px;    
			    font-weight:bold;
			    height: 172px;
			    width: 886px; 
			    font-size:30px; 
            }
                          
            .box{
            	clear: both;
            	background-color:#152D56;
            	 /* max-width: 100%;
            	margin: 15px; */
            	width: 95%;
            	align: center;
            }    
            
            .table_box{
            	background-color:#152D56;
            	/* margin: 15px; */
            	width: 95%;
            }   
            
             .table_box td {
             	border: 2px solid green;
             }
             
             .calculado{
				background: #CEFFCE;
			}
                
        </style>
        
    </head>
    
	<body onload="refreshAutomatico();">
	
		<div class="div_topo" style="float:left;text-align:left;">
	        <img style="float:left" src="/imagens/icones/icons/control.png" style="vertical-align:middle;"  />
		    <br/><?php echo SIGLA_SISTEMA; ?><br/>
		    Monitoramento Estratégico
		</div>
		<div class="div_topo" style="float:right;text-align:right;">
	        <img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="images/voltar.png" style="vertical-align:middle;" />
		    <img src="/imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		<div style="clear:both;">&nbsp;</div>
	
    <!--  
		<table class="cabecalho_tb" width="95%" align="center" cellpadding="0" cellspacing="0">
			<tr>
				<td class="cabecalho_td"><br/><center>Programa Mais Médicos</center><br/><br/></td>
			</tr>
		</table>
		
		
		<table style="border 0px;" width="95%" align="center" cellpadding="0" cellspacing="0" border="0">
			<tr>
				<td style="padding-bottom:2px;" bgcolor="#014401" width="30%">&nbsp;</td>
				<td width="886" height="172px"><img src="images/bannerPainel_p.jpg" height="172" border="0" /></td>
				<td bgcolor="white" width="70%">&nbsp;</td>
			</tr>
		</table>
	-->
		
		<table width="95%" align="center" border="0" cellpadding="0" cellspacing="0">
			<tr>
				<td bgcolor="#014401" width="60px">&nbsp;</td>
				<td background="images/bannerPainel_p.jpg" height="172px" width="886">&nbsp;</td>
				<td bgcolor="white">&nbsp;</td>
			</tr>
		</table>
		
		<!--  
		<table width="95%" align="center" cellpadding="0" cellspacing="0">
			<tr>
				<td class="div_topo">
					<img style="float:left" src="/imagens/icones/icons/control.png" style="vertical-align:middle;"  />
			        <br/><?php echo SIGLA_SISTEMA; ?><br/>
			        Monitoramento Estratégico
				</td>
				<td></td>				
				<td class="div_topo">
					<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="images/voltar.png" style="vertical-align:middle;" />
			        <img src="/imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
				</td>
			</tr>
			<tr>
				<td bgcolor="#014401">&nbsp;</td>
				<td ><img src="images/bannerPainel_p.jpg" width="854" /></td>
				<td bgcolor="white">&nbsp;</td>
			</tr>
		</table>
		-->
		
		<?php 
		
		for($x=2013;$x<=date('Y');$x++){
			$arAnos[$x] = $x;
		}
		
		$sql = "select 	
							to_char(to_date(dt_ini_periodo, 'YYYYMMDD'), 'YYYY') as ano,
							tut.tuttipo as tipo, 	
							count(*) as qtde,
							sum(vl_credito) as valor
						from maismedicos.autorizacaopagamento aut
        				join maismedicos.remessadetalhe rmd on rmd.rmdid = aut.rmdid
						join maismedicos.tutor tut on tut.tutid = rmd.tutid
						where aut.apgstatus = 'A'
        				and rmd.cs_ocorrencia = '0000'
						group by tut.tuttipo, to_char(to_date(dt_ini_periodo, 'YYYYMMDD'), 'YYYY')
						order by to_char(to_date(dt_ini_periodo, 'YYYYMMDD'), 'YYYY'), tut.tuttipo";
		
		$rsValoresQtdeBolsasPagas = $db->carregar($sql);
		
		if($rsValoresQtdeBolsasPagas){
					foreach($rsValoresQtdeBolsasPagas as $pagamento){
		
						$tipo = $pagamento['tipo']=='T' ? 'Tutor' : 'Supervisor';
						
						$var1 = 'qtde'.$pagamento['ano'].$tipo;
						$var2 = 'vlr'.$pagamento['ano'].$tipo;
						$var3 = 'vlr'.$tipo;
						
						${$var1} = $pagamento['qtde'];
						${$var2} = $pagamento['valor'];
						${$var3} += $pagamento['valor'];
						
						
					}
				}
				
				$sql = "select 
							count(distinct tut.tutcpf) as total, 
							tut.tuttipo 
						from maismedicos.tutor tut
						join maismedicos.universidade uni on tut.uniid = uni.uniid
						where tut.tutstatus = 'A' 
						group by tut.tuttipo";
				
				$rsBolsistas = $db->carregar($sql);
				if($rsBolsistas){
					foreach($rsBolsistas as $bolsista){
						$arBolsista[$bolsista['tuttipo']] = $bolsista['total'];
					}
				}
				
				$sql = "select to_char(wssdata, 'YYYY') as ano, count(distinct nucpf) as total from maismedicos.ws_profissionais 
						where wssstatus = 'A' 
						and situacao = 1
						and wssdata in ( (select max(wssdata) from maismedicos.ws_profissionais  where wssstatus = 'A' group by to_char(wssdata, 'YYYY')) )
						group by to_char(wssdata, 'YYYY')";
				
				$rsMedicosAno = $db->carregar($sql);
				if($rsMedicosAno){
					foreach($rsMedicosAno as $medicos){
						$arMedicos[$medicos['ano']] = $medicos['total'];
					}
				}
				
				$sql = "select to_char(wssdata, 'YYYY') as ano, count(distinct nucpf) as total from maismedicos.ws_supervisores 
            			where wssstatus = 'A'
            			and situacao = 1
            			and validado = true
						and to_char(wssdata, 'YYYYMMDD') in ( (select max(to_char(wssdata,'YYYYMMDD')) from maismedicos.ws_supervisores  where wssstatus = 'A' group by to_char(wssdata, 'YYYY')) )
						group by to_char(wssdata, 'YYYY')";
				
				$rsSupervisoresAno = $db->carregar($sql);
				if($rsSupervisoresAno){
					foreach($rsSupervisoresAno as $supervisores){
						$arSupervisores[$supervisores['ano']] = $supervisores['total'];
					}
				}
				
				$sql = "select to_char(wstdata, 'YYYY') as ano, count(distinct nucpf) as total from maismedicos.ws_tutores 
            			where wststatus = 'A'
            			and situacao = 1
            			and validado = true
						and to_char(wstdata, 'YYYYMMDD') in ( (select max(to_char(wstdata,'YYYYMMDD')) from maismedicos.ws_tutores  where wststatus = 'A' group by to_char(wstdata, 'YYYY')) )
						group by to_char(wstdata, 'YYYY')";
				
				$rsTutoresAno = $db->carregar($sql);
				if($rsTutoresAno){
					foreach($rsTutoresAno as $tutores){
						$arTutores[$tutores['ano']] = $tutores['total'];
					}
				}
				
				$sql = "select to_char(wssdata, 'YYYY') as ano, count(distinct cpfprofissional) as total from maismedicos.ws_planotrabalho_itens
        				where wssstatus = 'A'
						and to_char(wssdata, 'YYYYMMDD') in ( (select max(to_char(wssdata,'YYYYMMDD')) from maismedicos.ws_planotrabalho_itens  where wssstatus = 'A' group by to_char(wssdata, 'YYYY')) )
						group by to_char(wssdata, 'YYYY')";
				
				$rsPlanoTrabAno = $db->carregar($sql);
				if($rsPlanoTrabAno){
					foreach($rsPlanoTrabAno as $plano){
						$arPlanoTrabAno[$plano['ano']] = $plano['total'];
					}
				}
				
				$sql = "select to_char(dtvisita, 'YYYY') as ano, count(distinct cpfprofissional) as total from maismedicos.ws_respostas_formulario
						where wssstatus = 'A'
						and status = 'Finalizado'
						and to_char(dtvisita, 'YYYYMM') >= '201310' and to_char(dtvisita, 'YYYYMM') <= to_char(now(), 'YYYYMM')
						and to_char(dtvisita, 'YYYYMM') in ( (select max(to_char(dtvisita,'YYYYMM')) from maismedicos.ws_respostas_formulario  where wssstatus = 'A' group by to_char(dtvisita, 'YYYY')) )
						group by to_char(dtvisita, 'YYYY')";
				
				$rsFormularios = $db->carregar($sql);
				if($rsFormularios){
					foreach($rsFormularios as $form){
						$arFormularios[$form['ano']] = $form['total'];
					}
				}
				
				$sql = "select 
							count(distinct cpfprofissional) as visitas,
							(select count(distinct mdccpf) from maismedicos.medico where mdcstatus = 'A') as medicos
						from maismedicos.ws_respostas_formulario  
						where wssstatus = 'A' 
						and status = 'Finalizado' 
						and to_char(dtvisita, 'YYYYMM') = to_char(now(), 'YYYYMM')";
				
				$rsMedicosVisitas = $db->pegaLinha($sql);
				
				
				
				
				/*
				 * $sql = "select 		
							count(distinct mdc.mdccpf) as medicos,
							count(distinct tut.tutcpf) as supervisores
						from maismedicos.universidade uni
						join maismedicos.medico mdc on uni.idunasus = mdc.coinstituicaosupervisora
							and mdc.mdcstatus = 'A'
						join maismedicos.tutor tut on tut.uniid = uni.uniid
							and tut.tutstatus = 'A' 
							and tut.tuttipo = 'S'
						where uni.unistatus = 'A'";
						
				$rsSupMedicos = $db->pegaLinha($sql);
					
				//Medicos com supervisores
				$sql = "select 
							count(distinct mdccpf) as sem_supervisor,
							(select count(distinct mdccpf) from maismedicos.medico where mdcstatus = 'A') as total_medicos
						from maismedicos.medico where mdccpf not in (
							select cpfprofissional from maismedicos.ws_planotrabalho p
							join maismedicos.ws_planotrabalho_itens i on p.ptrid = i.ptrid
								and i.wssstatus = 'A' 
								and i.situacao = 1
							where p.wssstatus = 'A' 
						)";
				
				$rsMedicosSupervisor = $db->pegaLinha($sql);
				$calculoMedicosSemSupervisor = (100*$rsMedicosSupervisor['sem_supervisor'])/$rsMedicosSupervisor['total_medicos'];
				$calculoMedicosComSupervisor = 100-$calculoMedicosSemSupervisor;
				*/
				
		?>
		<table style="text-color:white;" bgcolor="#FFFFFF" width="95%" align="center" cellpadding="3" cellspacing="1">
			<thead>
				<tr>
					<th rowspan="2">&nbsp;</th>
					<th rowspan="2">Indicadores de Implantação<br/>Integração ensino-serviço<br/>supervisão</th>
					<th colspan="<?php echo count($arAnos)+1; ?>">Resultados</th>
				</tr>
				<tr>
					<?php if(is_array($arAnos)): ?>
						<?php foreach($arAnos as $ano): ?>
							<th width="120"><?php echo $ano; ?></th>
						<?php endforeach; ?>
					<?php endif; ?>
					<th width="120">Total</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td valign="middle">PMMB</td>
					<td valign="middle">Nº de Bolsas supervisão pagas</td>
					<?php if(is_array($arAnos)): ?>
						<?php foreach($arAnos as $ano): ?>
							<?php
							$var = 'qtde'.$ano.'Supervisor';
							$totalQtdeSupervisor += ${$var}; 
							?>
							<td valign="middle" class="calculado"><?php echo number_format(${$var}, 0, ',', '.'); ?></td>
						<?php endforeach; ?>
					<?php endif; ?>
					<td valign="middle" class="calculado"><?php echo number_format($totalQtdeSupervisor, 0, ',', '.'); ?> <br/> (R$ <?php echo number_format($vlrSupervisor,2,',','.'); ?>)</td>
				</tr>
				<tr>
					<td valign="middle">PMMB</td>
					<td valign="middle">Nº de Bolsas  tutoria  pagas</td>
					<?php if(is_array($arAnos)): ?>
						<?php foreach($arAnos as $ano): ?>
							<?php
							$var = 'qtde'.$ano.'Tutor';
							$totalQtdeTutor += ${$var}; 
							?>
							<td valign="middle" class="calculado"><?php echo number_format(${$var}, 0, ',', '.'); ?></td>
						<?php endforeach; ?>
					<?php endif; ?>
					<td valign="middle" class="calculado"><?php echo number_format($totalQtdeTutor, 0, ',', '.'); ?> <br/> (R$ <?php echo number_format($vlrTutor,2,',','.'); ?>)</td>
				</tr>
				<tr>
					<td valign="middle">PMMB</td>
					<td valign="middle">%º de médicos com supervisor</td>
					<?php if(is_array($arAnos)): ?>
						<?php foreach($arAnos as $ano): ?>
								<?php 
								$valorComPorcentagem = 100-((100*($arMedicos[$ano]-$arPlanoTrabAno[$ano]))/$arMedicos[$ano]);
								$valorComPorcentagem = $valorComPorcentagem>100 ? 100 : $valorComPorcentagem;
								$valorComPorcentagem = $valorComPorcentagem<0 ? 0 : $valorComPorcentagem;
								//X = 100*477 / 14098
								
								$valorSem = $arMedicos[$ano]-$arPlanoTrabAno[$ano];
								$valorSem = $valorSem<0 ? 0 : $valorSem;
								$valorCom = $arPlanoTrabAno[$ano]>$arMedicos[$ano] ? $arMedicos[$ano] : $arPlanoTrabAno[$ano];
								?>
								<td valign="middle" class="calculado">
									<center><?php echo number_format($valorComPorcentagem, 0, ',', '.'); ?>% (<?php echo $valorCom.' de '.$arMedicos[$ano]; ?>)
									<br/>(<?php echo $ano==date('Y') ? mes_extenso(date('m')) : 'DEZEMBRO'; ?>)</center>
								</td>
						<?php endforeach; ?>
					<?php endif; ?>
					<td valign="middle" class="calculado">-</td>
				</tr>
				<tr>
					<td valign="middle">PMMB</td>
					<td valign="middle">Relação entre nº de supervisores  e médicos</td>
					<?php if(is_array($arAnos)): ?>
						<?php foreach($arAnos as $ano): ?>
								<?php 
								$valor = $arSupervisores[$ano]>0 ? $arMedicos[$ano]/$arSupervisores[$ano] : 0;
								?>
								<td valign="middle" class="calculado">
									<center>1:<?php echo number_format($valor, 2, ',', '.'); ?> (<?php echo $arMedicos[$ano].'/'.$arSupervisores[$ano]; ?>)
									<br/>(<?php echo $ano==date('Y') ? mes_extenso(date('m')) : 'DEZEMBRO'; ?>)</center>
								</td>
						<?php endforeach; ?>
					<?php endif; ?>
					<td valign="middle" class="calculado">-</td>
				</tr>
				<tr>
					<td valign="middle">PMMB</td>
					<td valign="middle">Relação entre nº de tutores e supervisores</td>
					<?php if(is_array($arAnos)): ?>
						<?php foreach($arAnos as $ano): ?>
								<?php 
								$valor = $arTutores[$ano]>0 ? $arSupervisores[$ano]/$arTutores[$ano] : 0;
								?>
								<td valign="middle" class="calculado">
									<center>1:<?php echo number_format($valor, 2, ',', '.'); ?> (<?php echo $arSupervisores[$ano].'/'.$arTutores[$ano]; ?>)
									<br/>(<?php echo $ano==date('Y') ? mes_extenso(date('m')) : 'DEZEMBRO'; ?>)</center>
								</td>
						<?php endforeach; ?>
					<?php endif; ?>
					<td valign="middle" class="calculado">-</td>
				</tr>
				<tr>
					<td valign="middle">PMMB</td>
					<td valign="middle">% de médicos visitados no mês</td>
					<?php if(is_array($arAnos)): ?>
						<?php foreach($arAnos as $ano): ?>
								<?php 
								$valor = 100*($arFormularios[$ano]/$arMedicos[$ano]);
								?>
								<td valign="middle" class="calculado">
									<center><?php echo number_format($valor, 2, ',', '.'); ?>% (<?php echo $arFormularios[$ano].' de '.$arMedicos[$ano]; ?>)
									<br/>(<?php echo $ano==date('Y') ? mes_extenso(date('m')) : 'DEZEMBRO'; ?>)</center>
								</td>
						<?php endforeach; ?>
					<?php endif; ?>
					<td valign="middle" class="calculado">-</td>
					<!--  
					<td valign="middle">N/D</td>
					<td valign="middle" class="calculado" title="Formula: 100*total_visitas/total_medicos"><?php echo number_format((100*$rsMedicosVisitas['visitas'])/$rsMedicosVisitas['medicos'], 2, ',', '.').'% ('.mes_extenso(date('m')).')' ?></td>
					<td valign="middle" class="calculado"><?php echo $rsMedicosVisitas['visitas'].' de '.$rsMedicosVisitas['medicos'].' (ativos)'; ?></td>
					-->
				</tr>
				<tr>
					<td valign="middle">PMMB</td>
					<td valign="middle">Nº de visitas de supervisores por município no mês e por instituição</td>
					<td colspan="<?php echo count($arAnos)+1; ?>" valign="middle" align="center">Ver tabela detalhada</td>
				</tr>
				<tr>
					<td valign="middle">PMMB</td>
					<td valign="middle">% de planos de trabalho atualizados no mês</td>
					<?php if(is_array($arAnos)): ?>
						<?php foreach($arAnos as $ano): ?>
								<?php 
								$valor = $arPlanoTrabAno[$ano]>0 ? 100*($arMedicos[$ano]/$arPlanoTrabAno[$ano]) : 0;
								?>
								<td valign="middle" class="calculado">
									<center><?php echo number_format($valor, 2, ',', '.'); ?>%
									<br/>(<?php echo $ano==date('Y') ? mes_extenso(date('m')) : 'DEZEMBRO'; ?>)</center>
								</td>
						<?php endforeach; ?>
					<?php endif; ?>
					<td valign="middle" class="calculado">-</td>
				</tr>
				<tr>
					<td valign="middle">RESIDÊNCIA</td>
					<td valign="middle">Ampliação de vagas de preceptoria</td>
					<td colspan="<?php echo count($arAnos)+1; ?>" valign="middle" align="center">Somente 2015</td>
				</tr>
				<tr>
					<td valign="middle">RESIDÊNCIA</td>
					<td valign="middle">Nº de bolsas  preceptoria - graduação  pagas</td>
					<td colspan="<?php echo count($arAnos)+1; ?>" valign="middle" align="center">Somente 2015</td>
				</tr>
				<tr>
					<td valign="middle">RESIDÊNCIA</td>
					<td valign="middle">Nº de bolsa preceptoria - pós-graduação  pagas</td>
					<td colspan="<?php echo count($arAnos)+1; ?>" valign="middle" align="center">Somente 2015</td>
				</tr>
			</tbody>
		</table>
		
        <!--  
		<table bgcolor="#152D56" width="95%" align="center" cellpadding="0" cellspacing="0" border="1" style="border: 2px solid #EE9200;color:#ffffff">
	        <tr>
				<td><br/>Teste<br/></td>
				<td><br/>Teste<br/><br/>Teste<br/></td>
	        </tr>
		</table>
    	-->
    	
    </body>
    
</html>