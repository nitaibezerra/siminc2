<?PHP

    set_time_limit(0);

    include_once APPRAIZ . 'pde/www/_funcoes_cockpit.php';
    include_once APPRAIZ . 'includes/library/simec/funcoes.inc';
    include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
    include_once APPRAIZ . 'maismedicos/classes/Cockpit_Mais_Medicos.class.inc';
    
    $obCockpit = new Cockpit_Mais_Medicos();
    
    if($_REQUEST['requisicao']=='mudaTipoGegiao'){
    	$obCockpit->montaGraficoBarrasPorRegiao($_REQUEST['metodo']);
    	die;
    }
    
    if($_REQUEST['requisicao']=='mostraDetalheCiclo'){
    	montaTabelaCiclo($_REQUEST['indid']);
    	die;
    }
    
    if(!$nao_valida_acesso){
    	permissaoPerfilConsultaCockpit( substr($_SESSION['favurl'],0,60) );
    }
    
    if($_REQUEST['requisicaoAjax']){
    	$_REQUEST['requisicaoAjax']();
    	die;
    }
    
    if( $_REQUEST['useronline'] ){
        echo pegaUsuarioOnline('99,15,23');
        exit;
    }
    
    if(strstr($_SERVER['REQUEST_URI'],"maismedicos.php?modulo=relatorio/maismedicos&acao=C")){
    	if($_REQUEST['requisicaoAjax'] != "popUpMaisMedicos"){
    		echo "<script>alert('Acesso negado.');history.back(-1);</script>";
    		die;
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
        <script language="javascript" type="text/javascript" src="/includes/jquery-cycle/jquery.cycle.all.js"></script>
        <script language="javascript" type="text/javascript" src="/js/estrategico.js"></script>

        <link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
        <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/jquery.mousewheel.js"></script>
        <script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/perfect-scrollbar.js"></script>

        <script language="javascript" src="/includes/Highcharts-3.0.0/js/highcharts.js"></script>
        <script language="javascript" src="/includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

        <link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
        <script language="javascript" type="text/javascript" src="/library/jquery_totem/jquery.totemticker.min.js"></script>

        <link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='/includes/listagem.css'/>
        <link rel='stylesheet' type='text/css' href='css/cockpit.css'/>

        <script language="JavaScript" src="../includes/funcoes.js"></script>
        <script>
//
            $(function(){
                
//                 $('.div_fotos_interno').cycle({
//                     fx: 'scrollDown'
//                 });

                $('.filtro_qtds').click(function(){
                    window.location.href = '/pde/estrategico.php?modulo=principal/cockpit_mais_medicos&acao=A&filtro_qtd=' + $(this).attr('id');
                });

//                 $('.scrollbar').perfectScrollbar({wheelPropagation:true});

                $('.lista-noticias').totemticker({
                    row_height  :   '100px',
                    next        :   '#ticker-next',
                    previous    :   '#ticker-previous',
                    stop        :   '#stop',
                    start       :   '#start',
                    mousestop   :   true,
                });

                $('.btnDetalharPagamento').click(function(){
                	arParams = this.id.split('_');
                	if($(this).attr('src')=='../imagens/mais.gif'){
                		$('.pagamentos_'+arParams[1]).show();
                		$(this).attr('src', '../imagens/menos.gif')
                	}else{
                		$('.pagamentos_'+arParams[1]).hide();
                		$(this).attr('src', '../imagens/mais.gif')
                	}
                });	

                $('.btnMostraDetalheCiclo').click(function(){
                    
					indid = this.id;

					if($(this).attr('src')=='../imagens/mais.gif'){

// 						if($('#detalhe_'+indid).html()==''){
							
							$.ajax({
								url		: '/pde/estrategico.php?modulo=principal/cockpit_mais_medicos&acao=A',
								type	: 'post',
								data	: 'requisicao=mostraDetalheCiclo&indid='+indid,
								success	: function(e){
									$('#detalhe_'+indid).html(e).show();
								}
							});
// 						}

						$(this).attr('src', '../imagens/menos.gif');
						
					}else{
						
						$('#detalhe_'+indid).hide();
						$(this).attr('src', '../imagens/mais.gif');
					}
				});
            });
            
//             atualizaUsuario();
            
            function abreMapaPainel()
            {
                window.open('/painel/painel.php?modulo=principal/mapas/mapaPadrao&acao=A&mapid=20&cockpit=1&id_104=checked','Indicadores','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
            }

            function exibeInformacaoPagamento(periodo)
            {
            	var url = window.location + "&requisicaoAjax=listaRemessaCreditoMaisMedicos&periodo="+periodo;
            	janela(url,800,600,"Pagamento");
            }

            function exibeRelatorioMaisMedico(tipo)
            {
            	var url = window.location + "&requisicaoAjax="+tipo;
            	var div = $("#div_tabela_relatorio_mais_medico");
            	div.html('<center><p>&nbsp;</p><p><img src="../imagens/carregando.gif" align="absmiddle" />&nbsp;Carregando...</p></center>')
    			$.ajax({
    				type: "POST",
    				url: url,
    				async: false,
    				success: function(response){
    					div.html(response);
    				}
    			});
            }
            
        </script>

        <style type="text/css">
            #div-ciclos{height: 900px;}
            #div-qtd{height: 550px;}
            .fundo_titulo{background-image:url('cockpit/images/banner/bannerMaisMedicos.jpg')};
        </style>
    </head>
    <body onload="refreshAutomatico();">

        <table border="0" align="center" width="98%" cellspacing="0" cellpadding="5" class="tabela_painel">
            <tr>
                <td class="titulo_pagina" >
                    <div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
                        <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
                        <div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
                    </div>
                    <img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
                    <div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_mais_medicos&acao=A';">
                        <img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
                    </div>
                </td>
            </tr>
            <!-- Título-->
            <tr>
            	<td class="fundo_titulo" style="text-align:center" colspan="6" ><br>Programa Mais Médicos<br><br></td>
            </tr>
        </table>
        
        <table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
			<tr class="fundo_padrao">
				<td align="center">
					<?php
					$totalMeta = $db->pegaUm("select count(distinct mdccpf) from maismedicos.medico where mdcstatus = 'A'");
					$valorMeta = 14000;
					$porcentagem = number_format(($totalMeta/$valorMeta)*100,0,",",".");
					$horizontal = $porcentagem * 2;
					exibirTitulo('alvo', number_format($totalMeta,0,",",".").' médicos ativos', substr(mes_extenso(date('m')),0,3).'/'.date('Y'));
					?>
					<div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
						<div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontal ?>px;'>
						<?= $porcentagem ?>%&nbsp;</div>
					</div>
					<font color="white">14.000 médicos previstos (até 2016)</font>
				</td>
				<td align="center">
					<?php
					$totalMeta = $db->pegaUm("select count(distinct tutcpf) from maismedicos.tutor where tutstatus = 'A' and tuttipo = 'S' and tutvalidade = true");
					$valorMeta = 1750;
					$porcentagem = number_format(($totalMeta/$valorMeta)*100,0,",",".");
					$horizontal = $porcentagem * 2;
					exibirTitulo('alvo', number_format($totalMeta,0,",",".").' supervisores incorporados ', substr(mes_extenso(date('m')),0,3).'/'.date('Y'));
					?>
					<div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
						<div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontal ?>px;'>
						<?= $porcentagem ?>%&nbsp;</div>
					</div>
					<font color="white">1.750 supervisores previstos (até 2016)</font>
				</td>
				<td align="center">
					<?php
					$totalMeta = $db->pegaUm("select count(distinct tutcpf) from maismedicos.tutor where tutstatus = 'A' and tuttipo = 'T' and tutvalidade = true");
					$valorMeta = 175;
					$porcentagem = number_format(($totalMeta/$valorMeta)*100,0,",",".");
					$horizontal = $porcentagem * 2;
					exibirTitulo('alvo', number_format($totalMeta,0,",",".").' tutores incorporados ', substr(mes_extenso(date('m')),0,3).'/'.date('Y')); ?>
					<div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
						<div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontal ?>px;'><?= $porcentagem ?>%&nbsp;</div>
					</div>
					<font color="white">175 tutores previstos (até 2016)</font>
				</td>
				<td align="center">
					<?php
					$totalInst = $db->pegaUm("select count(distinct idunasus) from maismedicos.universidade where unistatus = 'A'");
					$totalInstMunicipio = $db->pegaUm("select count(distinct muncod) from maismedicos.universidademunicipio");
					$totalMunicipio = $db->pegaUm("select count(distinct muncod) from territorios.municipio");
					$porcentagem = number_format(($totalInstMunicipio*100)/$totalMunicipio,2,",",".");
					$horizontal = $porcentagem * 2;
					exibirTitulo('alvo', $totalInst.' Instituições Supervisoras atuando em '.$totalInstMunicipio.' ', substr(mes_extenso(date('m')),0,3).'/'.date('Y')); ?>
					<div style='border-width: 1px; border-style: solid; border-color: rgb(0, 0, 0); background-color: #FFFFFF; text-align: right; color: white; height: 25px; width: 200px;'>
						<div style='background-color: #80BC44; text-align: right; color: white; height: 25px; width: <?= $horizontal ?>px;'>
						<?= $porcentagem ?>%&nbsp;</div>
					</div>
					<font color="white"><?php echo $totalMunicipio; ?> municípios</font>
				</td>
			</tr>
		</table>
					
        <table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
        	<tr class="fundo_padrao">
        		<td>
        		<!-- Tabela Adesão - Instituições Participantes -->
                    <?php exibirTitulo('mapas', 'Adesão - Instituições Participantes'); ?>
                    <?php
                        // POR UF
                        $sql = "select count(distinct ies) as ies, count(distinct muncod) as muncod
                                from (
                                    select  uni.uniid as ies,
                                        mun.muncod,
                                        count(distinct (tut.tutid)) + count(distinct (sup.tutid)) as total
                                    from maismedicos.universidade uni
                                        inner join territorios.municipio mun ON mun.muncod = uni.muncod
                                        left join maismedicos.tutor tut ON uni.uniid = tut.uniid and tut.tuttipo = 'T' and tut.tutstatus = 'A' and tut.tutvalidade is true
                                        left join maismedicos.tutor sup ON uni.uniid = sup.uniid and sup.tuttipo = 'S' and sup.tutstatus = 'A' and sup.tutvalidade is true
                                    group by mun.muncod, ies
                                    order by mun.muncod, ies
                                ) as tbl
                                where total > 0";

                        $dados = $db->pegaLinha($sql);
                        ?>
                    <table style="width:100%;">
                    	<tr>
                    		<td style="width:65%;">
	                    		<a target="_blank" href="/painel/painel.php?modulo=principal/mapas/mapaPadrao&acao=A&mapid=32&cockpit=1&id_132=checked">
	                            	<img class="link" src="../imagens/cockpit/mapa_cockpit.png" border="0"/></a>
                    		</td>
                    		<!-- 
                    		<td>
                    			<span class="subtitulo_box" ><b><font size="3"><?php echo $dados['ies']; ?></font></b>&nbsp;IES</span><br />
                            	<span class="subtitulo_box" ><b><font size="3"><?php echo $dados['muncod']; ?></font></b>&nbsp;MUNICÍPIOS</span>
                    		</td>
                    		 -->
                    	</tr>
                    </table>                   
                <!-- FIM Tabela Adesão - Instituições Participantes -->
        		</td>
        		<td>
        			<div style="overflow:auto;width:100%;height:280px;">
        			<table width="100%">
                		<tr>
                			<td width="60"><img aligin="absmiddle" src="../imagens/icones/icons/alvo.png" />&nbsp;</td>
                			<td class="titulo_box" style="padding-top:18px;">Relatório</td>
                		</tr>
                	</table>
                    <div style="width:100%;text-align:center" >
						<input type="radio" name="tipo_exibicao_mais_medicos" value="regiao" onclick="exibeRelatorioMaisMedico('exibeRelatorioMaisMedicoRegiao')" />Região 	
						<input type="radio" name="tipo_exibicao_mais_medicos" value="uf" checked="checked" onclick="exibeRelatorioMaisMedico('exibeRelatorioMaisMedicos')" />UF 
						<input type="radio" name="tipo_exibicao_mais_medicos" value="saude" onclick="exibeRelatorioMaisMedico('exibeRelatorioMaisMedicosRegioesSaude')" />Regiões da Saúde
						<input type="radio" name="tipo_exibicao_mais_medicos" value="saude" onclick="exibeRelatorioMaisMedico('exibeRelatorioInstituicaoSupervisora')" />Instituição Supervisora 
					</div>
					<div id="div_tabela_relatorio_mais_medico" >
                    	<?php exibeRelatorioMaisMedicos() ?>
                    </div>
                    Obs.: Algumas instituições atuam em mais de uma UF, tendo seus dados referentes à contagem de instituições, tutores e supervisores computados em todas as UFs em que atuam, no modo detalhado.
                    </div>
        		</td>
        	</tr>
        	<tr class="fundo_padrao" style="width:50%">
        		<td>
        			<?php exibirTitulo('indicador', 'Relatório do Projeto, por Regiões'); ?>
        			
        			<p><center>
					<input type="radio" name="relatorioRegiao" value="contaMunicipiosPorRegiao" /> Municípios  
					<input type="radio" name="relatorioRegiao" value="contaTutoresPorRegiao" /> Tutores   
					<input type="radio" name="relatorioRegiao" value="contaSupervisoresPorRegiao" /> Supervisores   
					<input type="radio" name="relatorioRegiao" value="contaMedicosPorRegiao" checked/> Médicos do Programa
					</center></p><br/>
					
					<script>
						$(function(){
							$('[name=relatorioRegiao]').click(function(){
								metodo = this.value;
								$('#montaGraficoRegiao').html('<center><p><img src="../imagens/carregando.gif" border="0" />&nbsp;Carregando...<p></center>');
								$.ajax({
									url		: document.location,
									type	: 'post',
									data	: 'requisicao=mudaTipoGegiao&metodo='+metodo,
									success	: function(e){
										$('#montaGraficoRegiao').html(e);
// 										divCarregado('montaGraficoRegiao');
									}
								});
							});
						});
					</script>
					
					<div id="montaGraficoRegiao" style="">
						<?php $obCockpit->montaGraficoBarrasPorRegiao(); ?>
					</div>
					
	    		</td>
        		<td style=wwidth: 50%;">
        			<table width="100%">
                		<tr>
                			<td width="60"><img aligin="absmiddle" src="../imagens/icones/icons/casas.png" />&nbsp;</td>
                			<td class="titulo_box" style="padding-top:18px;">Visitas realizadas</td>
                		</tr>
                	</table>
                	<div style="width:100%;max-height:380px;overflow:auto;">
                		<?php $obCockpit->montaGraficoPeriodoSemVisitaMedico(); ?>
                	</div>
        		</td>
        	</tr>
        </table>
        
        <table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">        
        	<tr class="fundo_padrao">
        		<td>
        			<table width="100%">
                		<tr>
                			<td width="60"><img aligin="absmiddle" src="../imagens/icones/icons/indicador.png" />&nbsp;</td>
                			<td class="titulo_box" style="padding-top:18px;">Quantitativo de médicos (Indicador 2792 P/ trimestre)</td>
                		</tr>
                	</table>
        			<center><p>Gráfico barras</p></center>
        		</td>
        		<td>
        			<table width="100%">
                		<tr>
                			<td width="60"><img aligin="absmiddle" src="../imagens/icones/icons/indicador.png" />&nbsp;</td>
                			<td class="titulo_box" style="padding-top:18px;">Quantitativo de Supervisores (Indicador 2293 P/ trimestre</td>
                		</tr>
                	</table>
        			<center><p>Gráfico barras</p></center>
        		</td>
        		<td>
        			<table width="100%">
                		<tr>
                			<td width="60"><img aligin="absmiddle" src="../imagens/icones/icons/indicador.png" />&nbsp;</td>
                			<td class="titulo_box" style="padding-top:18px;">Quantitativo de Tutores (Indicador 2292 P/ trimestre)</td>
                		</tr>
                	</table>
        			<center><p>Gráfico barras</p></center>
        		</td>
        	</tr>
        </table>
        		
        <table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
        	<tr class="fundo_padrao">
        		<td>
        			<!-- Informativo de pagamento-->
                    <table width="100%">
                		<tr>
                			<td width="60"><img aligin="absmiddle" src="../imagens/icones/icons/financeiro.png" />&nbsp;</td>
                			<td class="titulo_box" style="padding-top:18px;">Informativo de pagamento de bolsas Tutores/Supervisores</td>
                		</tr>
                	</table>
                	
                    <div>
                        <?php
                            // Informativo de Pagamento de bolsas de Tutores / Supervisores
							$sql = "select * from (
											select
												substr(dt_ini_periodo, 5, 2) || '/' || substr(dt_ini_periodo, 1, 4) as periodo,
												substr(dt_ini_periodo, 5, 2) as mes,
												substr(dt_ini_periodo, 1, 4) as ano,
												count(tut.tutid) as tutor,
												count(sup.tutid) as supervisor,
												sum(vl_credito) as total,
												1 as ordem
											from 
												maismedicos.autorizacaopagamento aut
											inner join 
												maismedicos.remessadetalhe det ON det.rmdid = aut.rmdid
											left join
												maismedicos.tutor tut ON tut.tutid = det.tutid and tut.tuttipo = 'T'
											left join
												maismedicos.tutor sup ON sup .tutid = det.tutid and sup .tuttipo = 'S'
											where 
												aut.apgstatus = 'A'
											and
												det.cs_ocorrencia = '0000'
											group by 
												substr(dt_ini_periodo, 1, 4), substr(dt_ini_periodo, 5, 2)
											order by 
												substr(dt_ini_periodo, 1, 4), substr(dt_ini_periodo, 5, 2)
										) as tbls order by ano desc, mes desc";
							
                            $arrDados = $db->carregar($sql);
                            
                            if($arrDados){
								foreach($arrDados as $dados){
									$arPagamentos[$dados['ano']][$dados['mes']]['tutores'] = $dados['tutor'];
									$arPagamentos[$dados['ano']][$dados['mes']]['supervisores'] = $dados['supervisor'];
									$arPagamentos[$dados['ano']][$dados['mes']]['valor'] = $dados['total'];
									$arPagamentosTotais[$dados['ano']]['tutores'] += $dados['tutor'] ? $dados['tutor'] : 0;
									$arPagamentosTotais[$dados['ano']]['supervisores'] += $dados['supervisor'] ? $dados['supervisor'] : 0;
									$arPagamentosTotais[$dados['ano']]['valor'] += $dados['total'] ? $dados['total'] : 0;
								}
							}
                        ?>
                        <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                        	<thead>
                        		<tr>
                        			<th>Período de Referência</th>
	                                <th>Tutores</th>
	                                <th>Supervisores</th>
	                                <th>Total</th>
                        		</tr>
                        	</thead>                        		
                        	<tbody>
                        		<?php if($arPagamentos): ?>
                        			<?php foreach($arPagamentos as $k => $v): ?>
                        				<tr>
                        					<td>
                        						<img src="../imagens/mais.gif" id="pagamentos_<?php echo $k; ?>" class="btnDetalharPagamento" style="cursor:pointer;" />
                        						<?php echo $k; ?>
                        					</td>
                        					<td><?php echo $arPagamentosTotais[$k]['tutores'] ?></td>
                        					<td><?php echo $arPagamentosTotais[$k]['supervisores'] ?></td>
                        					<td><?php echo number_format($arPagamentosTotais[$k]['valor'],2,",","."); ?></td>
                        				</tr>
                        				<?php foreach($arPagamentos[$k] as $i => $d): ?>
                        					<tr title="Abrir informativo de pagamento de bolsas" alt="Abrir informativo de pagamento de bolsas" onclick="exibeInformacaoPagamento('<?php echo $i.'/'.$k; ?>')" class="link pagamentos_<?php echo $k; ?>" style="display:none;cursor:pointer;">
                        						<td>&nbsp;&nbsp;<img src="../imagens/seta_filho.gif" /><?php echo mes_extenso($i); ?></td>
                        						<td><?php echo $d['tutores'] ?></td>
                        						<td><?php echo $d['supervisores'] ?></td>
                        						<td><?php echo number_format($d['valor'],2,",","."); ?></td>
                        					</tr>
                        				<?php endforeach; ?>
                        			<?php endforeach; ?>
                        		<?php else: ?>
									<tr>
										<td colspan="2" algin="center"><b>Sem registros.</b></td>
									</tr>                        			
                        		<?php endif; ?>
                        	</tbody>
                        </table>
                        <?php unset($arPagamentos); ?>
                    </div>
                <!-- FIM Informativo de pagamento-->
        		</td>
        		<td>
        			<table width="100%">
                		<tr>
                			<td width="60"><img aligin="absmiddle" src="../imagens/icones/icons/mais.png" />&nbsp;</td>
                			<td class="titulo_box" style="padding-top:18px;">Implantação do Programa Mais Médico</td>
                		</tr>
                	</table>
                	
                	<?php 
                	$arCiclos = array(
                		array("ciclo"=>"4º Ciclo","indid"=>"150573" ),
                		array("ciclo"=>"3º Ciclo","indid"=>"148382" ),
                		array("ciclo"=>"2º Ciclo - Segunda etapa","indid"=>"148074" ),
                		array("ciclo"=>"2º Ciclo - Primeira etapa","indid"=>"147355" ),
                		array("ciclo"=>"1º Ciclo","indid"=>"146835" )
                	);					
                	?>
                	
                	<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%">
                		<tbody>
	                	<?php foreach($arCiclos as $ciclo): ?>
	                		<tr>
	                			<td>
	                				<img id="<?php echo $ciclo['indid']; ?>" src="../imagens/mais.gif" class="btnMostraDetalheCiclo" style="cursor:pointer;" />
	                					<?php echo $ciclo['ciclo']; ?>
	                			</td>
	                		</tr>
	                		<tr>
	                			<td id="detalhe_<?php echo $ciclo['indid']; ?>" style="background:#152D56;display:none;"></td>
	                		</tr>
	                	<?php endforeach; ?>
	                	</tbody>
                	</table>
					<p>&nbsp;</p>
					<p>&nbsp;</p>
        		
        		</td>
        		<td>
        			<table width="100%">
                		<tr>
                			<td width="60"><img aligin="absmiddle" src="../imagens/icones/icons/alarm.png" />&nbsp;</td>
                			<td class="titulo_box" style="padding-top:18px;">Situação dos Médicos Intercambistas nos Módulos de Acolhimento e Avaliação (Indicador 2885)</td>
                		</tr>
                	</table>
        			<center><p>Gráfico pizza</p></center>
        		</td>
        	</tr>
        </table>
        
        <table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	        <tr class="fundo_padrao">
	        	<td>
	        	 <!-- Notícias - Mais Médicos -->
                    <?php exibirTitulo('chat', 'Notícias - Mais Médicos'); ?>
                    <div>
                        <ul class="lista-noticias">
                            <?php
                            $xml = utf8_encode(file_get_contents('http://www.google.com/alerts/feeds/04180951689783258528/13040220840775365612'));
                            $rss = new SimpleXMLElement($xml);

                            foreach ($rss as $item) {
                                if(isset($item->title)) { ?>
                                    <li>
                                        <a target="_blank" href="<?php echo (string)$item->link['href']; ?>">
                                            <p><?php echo utf8_decode($item->title); ?></p><br>
                                            <p><?php echo utf8_decode($item->content); ?></p>
                                        </a>
                                    </li>
                               <?php }
                            } ?>
                        </ul>
                        <div class="clear"></div>
                    </div>
                <!-- FIM Notícias - Mais Médicos -->
	        	</td>
	        </tr>
        </table>
        
        
        <p>&nbsp;</p>
    </body>
</html>