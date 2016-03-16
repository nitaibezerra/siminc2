<?php 
if($_REQUEST['requisicao'] == 'carregaRoll'){
	switch ($_REQUEST['area']){
		case 'pe':
			$sql = "select obeid as codigo, obedsc as descricao from monitora.jit_pe_objetivoestrategico where exoid = {$_REQUEST['exoid']} order by obedsc";
			$subTitulo = "Objetivos Estratégicos";			
			break;
		case 'ppa':
			$sql = "select objid as codigo, objdsc as descricao from monitora.jit_ppa_programa p
					inner join monitora.jit_ppa_objetivo o on o.prgid = p.prgid
					where p.exoid = {$_REQUEST['exoid']} order by objdsc";
			$subTitulo = "Objetivos";
			break;
		case 'pne':
			$sql = "select metid || '_' || coalesce( dseid::varchar, '') as codigo, metdsc as descricao from monitora.jit_pne_meta
					where exoid = {$_REQUEST['exoid']}
					order by metid";
			$subTitulo = "Metas";
			break;
		case 'ae':
			$sql = "select acaoid as codigo, acadsc as descricao from monitora.jit_acoes
				where exoid = {$_REQUEST['exoid']}";
				$subTitulo = "Ações";
			break;
	}
	if($sql){
		$rs = $db->carregar($sql);
		echo '<center><h2>'.strtoupper($_REQUEST['area']).'</h2>';
		echo '<p><b>'.$subTitulo.'</b></p></center>';			
		if($rs){
			echo '<div id="'.($_REQUEST['area'] != 'pne' ? 'div_lnk_objetivos_metas2' : 'div_lnk_objetivos_metas').'">';
			foreach($rs as $dados){
				//echo '<button class="objetivos_metas" id="'.$_REQUEST['area'].'_'.$dados['codigo'].'">'.$dados['descricao'].'</button>';
				echo '<div id="'.$_REQUEST['area'].'_'.$dados['codigo'].'"  class="objetivos_metas '.($_REQUEST['classe'] ? $_REQUEST['classe'] : 'verde').'" style="margin-bottom: 5px; cursor: pointer;"> '.$dados['descricao'].'</div>';
			}
			echo '</div>';			
		}
	}else{
		echo "Sem registros.";	
	}
	die;
}

if($_REQUEST['requisicao'] == 'carregaTable'){
	
	$arId = explode('_', $_REQUEST['id']);
	if(is_array($arId)){
		switch ($arId[0]){
			case 'pe':
				montaGridPE();
				break;
			case 'ppa':
				montaGridPPA();
				break;
			case 'pne':
				montaGridPNE();
				break;
			case 'ae':
				montaGridAE();
				break;
			case 'orc':
				//montaGridPNE();
				break;
		}
	}
	die;
}

if($_REQUEST['requisicao'] == 'carregaMetasPNE'){
	if($_REQUEST['id']>0){
		$sql = "select * from monitora.jit_pne_meta where dseid = {$_REQUEST['id']}";
		$rs = $db->carregar($sql);
		if($rs){
			foreach($rs as $data){			
				echo '
					<div id="'.$data['exoid'].'_'.$data['metid'].'_'.$data['dseid'].'" class="tile1 tile4 verde clickMetasPNE" style="margin:0 0 0 0;">
					    <span class="textoDescricao">'.$data['metdsc'].'</span><br/>
					</div>
				';
			}
		}
	}
	die;
}

if($_REQUEST['requisicao'] == 'carregaObjetivosPPA'){
	if($_REQUEST['id']>0){
		$sql = "select objid, objdsc, exoid 
				from monitora.jit_ppa_objetivo o
				left join monitora.jit_ppa_programa p on p.prgid = o.prgid 
				where obeid = {$_REQUEST['id']}";
		$rs = $db->carregar($sql);
		if($rs){
			foreach($rs as $data){
				echo '
				<div id="'.$data['exoid'].'_'.$data['objid'].'" class="tile1 tile4 azul clickObjetivosPPA">
					<span class="textoDescricao">'.$data['objdsc'].'</span><br/>
				</div>
				';
			}
		}
	}
	die;
}

if($_REQUEST['requisicao'] == 'carregaDesafiosPE'){
	if($_REQUEST['id']>0){
		$sql = "select obeid, dsedsc, metid, m.dseid, m.exoid from monitora.jit_pne_meta m
				inner join monitora.jit_pe_desafioprioritario d on m.dseid = d.dseid
				where m.metid = {$_REQUEST['id']}";	
		//dbg($sql);
		$rs = $db->carregar($sql);
		if($rs){
			foreach($rs as $data){
				echo '
				<div id="'.$data['exoid'].'_'.$data['obeid'].'_'.$data['dseid'].'" class="tile1 tile4 amarelo clickDesafiosPE">
					<span class="textoDescricao">'.$data['dsedsc'].'</span><br/>
				</div>
				';
			}
		}
	}
	die;
}

if($_REQUEST['requisicao'] == 'carregaMetasPNEPPA'){
	if($_REQUEST['id']>0){
		$sql = "select * from monitora.jit_pne_meta where metid = {$_REQUEST['id']}";
		$rs = $db->carregar($sql);
		if($rs){
			foreach($rs as $data){
				echo '
				<div id="'.$data['exoid'].'_'.$data['metid'].'_'.$data['dseid'].'" class="tile1 tile4 verde clickMetasPNE">
					<span class="textoDescricao">'.$data['metdsc'].'</span><br/>
				</div>
				';
			}
		}
	}
	die;
}

if($_REQUEST['requisicao'] == 'carregaMetasAcoesPE'){
	if($_REQUEST['id']>0){
		$sql = "select * from monitora.jit_pe_acao where dseid = {$_REQUEST['id']}";
		$rs = $db->carregar($sql);
		if($rs){
			foreach($rs as $data){
				echo '
				<div class="tile1 tile4 azulClaro" style="cursor: default">
					'.($data['acalinkpainelestrategico'] ? '<a href="'.$data['acalinkpainelestrategico'].'" class="linkPopupPainel" target="_blank" alt="Painel Estratégico" title="Painel Estratégico"><img src="../imagens/odometro.png" height="15" border="0" /></a>&nbsp;' : '').'
					'.($data['acalinkindicador'] ? '<a href="'.$data['acalinkindicador'].'" target="_blank" class="linkPopupIndicador" alt="Indicador" title="Indicador"><img src="../imagens/seriehistorica_ativa.gif" height="15" border="0" /></a>&nbsp;' : '').'
					'.($data['acalinkdetalheindicador'] ? '<a href="'.$data['acalinkdetalheindicador'].'" target="_blank" class="linkPopupIndicador" alt="Detalhe do Indicador" title="Detalhe do Indicador"><img src="../imagens/eixos-mini2.png" height="15" border="0" /></a>&nbsp;' : '').'
					<span class="textoDescricao" title="'.$data['acadetalhe'].'" alt="'.$data['acadetalhe'].'">'.$data['acadsc'].'</span><br/>
				</div>
				';
			}
		}
	}
	die;
}

if($_REQUEST['requisicao'] == 'carregaAcoesPEPPA'){
	if($_REQUEST['id']>0){
		$sql = "select * from monitora.jit_pe_acao where metidpne = {$_REQUEST['id']}";
		$rs = $db->carregar($sql);
		if($rs){
			foreach($rs as $data){
				echo '
				<div class="tile1 tile4 azulClaro" style="cursor: default">
					'.($data['acalinkpainelestrategico'] ? '<a href="'.$data['acalinkpainelestrategico'].'" target="_blank" class="linkPopupPainel" alt="Painel Estratégico" title="Painel Estratégico"><img src="../imagens/odometro.png" height="15" border="0" /></a>&nbsp;' : '').'
					'.($data['acalinkindicador'] ? '<a href="'.$data['acalinkindicador'].'" target="_blank" class="linkPopupIndicador" alt="Indicador" title="Indicador"><img src="../imagens/seriehistorica_ativa.gif" height="15" border="0" /></a>&nbsp;' : '').'
					'.($data['acalinkdetalheindicador'] ? '<a href="'.$data['acalinkdetalheindicador'].'" target="_blank" class="linkPopupIndicador" alt="Detalhe do Indicador" title="Detalhe do Indicador"><img src="../imagens/eixos-mini2.png" height="15" border="0" /></a>&nbsp;' : '').'
					<span class="textoDescricao" title="'.$data['acadetalhe'].'" alt="'.$data['acadetalhe'].'">'.$data['acadsc'].'</span><br/>
				</div>
				';
			}
		}
	}
	die;
}

if($_REQUEST['requisicao'] == 'carregaAcoesPDE'){
	if($_REQUEST['id']>0){
		$sql = "select * from monitora.jit_pne_acoes_pde where metid = {$_REQUEST['id']}";
		$rs = $db->carregar($sql);
		if($rs){
			foreach($rs as $data){
				echo '
				<div class="tile1 tile4 azulClaro" style="cursor: default">
				'.($data['acalinkpainelestrategico'] ? '<a href="'.$data['acalinkpainelestrategico'].'" target="_blank" class="linkPopupPainel" alt="Painel Estratégico" title="Painel Estratégico"><img src="../imagens/odometro.png" height="15" border="0" /></a>&nbsp;' : '').'
				'.($data['acalinkindicador'] ? '<a href="'.$data['acalinkindicador'].'" target="_blank" class="linkPopupIndicador" alt="Indicador" title="Indicador"><img src="../imagens/seriehistorica_ativa.gif" height="15" border="0" /></a>&nbsp;' : '').'
				'.($data['acalinkdetalheindicador'] ? '<a href="'.$data['acalinkdetalheindicador'].'" target="_blank" class="linkPopupIndicador" alt="Detalhe do Indicador" title="Detalhe do Indicador"><img src="../imagens/eixos-mini2.png" height="15" border="0" /></a>&nbsp;' : '').'
				<span class="textoDescricao" title="'.$data['acadetalhe'].'" alt="'.$data['acadetalhe'].'">'.$data['acadsc'].'</span><br/>
				</div>
				';
			}
		}
	}
	die;
}

if($_REQUEST['requisicao'] == 'carregarDesafiosPNE'){
	if($_REQUEST['id']>0){
		$sql = "select d.*, exoid from monitora.jit_pne_meta m
				inner join monitora.jit_pe_desafioprioritario d on d.dseid = m.dseid 
				where m.metid = {$_REQUEST['id']}";
		//dbg($sql);
		$rs = $db->carregar($sql);
		if($rs){
			foreach($rs as $data){
				echo '
				<div id="'.$data['exoid'].'_'.$data['obeid'].'_'.$data['dseid'].'" class="tile1 tile4 amarelo clickDesafiosPE">				
					<span class="textoDescricao" >'.$data['dsedsc'].'</span><br/>
				</div>
				';
			}
		}
	}
	die;
}

if($_REQUEST['requisicao'] == 'carregaObjetivoPPA'){
	if($_REQUEST['id']>0){
		$sql = "select distinct exoid, obj.* from monitora.jit_pe_objetivoestrategico obe
				inner join monitora.jit_pe_desafioprioritario dse on dse.obeid = obe.obeid
				inner join monitora.jit_ppa_objetivo obj on obj.obeid = dse.obeid 
				where dse.dseid = {$_REQUEST['id']}";
		$rs = $db->carregar($sql);
		if($rs){
			foreach($rs as $data){
			echo '
				<div id="'.$data['exoid'].'_'.$data['objid'].'" class="tile1 tile4 azul clickObjetivosPPA">
					<span class="textoDescricao" >'.$data['objdsc'].'</span><br/>
				</div>
			';
			}
		}
	}
	die;
}

if($_REQUEST['requisicao'] == 'carregaEstrategiasPNE'){
	if($_REQUEST['id']>0){
		$sql = "select * from monitora.jit_pne_estrategias where metid = {$_REQUEST['id']}";
		$rs = $db->carregar($sql);
		if($rs){
			foreach($rs as $data){
				echo '
					<div class="tile1 tile4 azulClaro" style="cursor: default">
						<span class="textoDescricao">'.$data['estdsc'].'</span><br/>
					</div>
				';
			}
		}
	}	
	die;
}




?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		
		<meta http-equiv="X-UA-Compatible" content="IE=7" />
		<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
		<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>		
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="css/jit_win8_desafios.css"/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		<script type="text/javascript" src="js/jit_win8_desafios.js"></script>
		<script language="javascript" type="text/javascript" src="../includes/Jit/jit.js"></script>
		<style type="text/css">
			html {
			   background: url("cockpit/images/bg_mon.jpg") repeat-x scroll 0 28px #010206;
			   color: #333333;
			   padding: 0;
			}
			
			#busca{background: none repeat scroll 0% 0% rgb(255, 255, 255); width:400px;border-width: 1px; border-style: solid; border-color: rgb(204, 204, 204) rgb(153, 153, 153) rgb(153, 153, 153) rgb(204, 204, 204); color: rgb(0, 0, 0); font: 18px arial,sans-serif bold; height: 35px;}
			.fundo_td_laranja{background-color:#EE9200}
  			.fundo_td_laranja:hover{background-color:#EBB513}
		</style>
		 
		<script>
		$(function(){
			
		    $(".tile").mousedown(function(){       
		        $(this).addClass("selecionado");
		    });
		 
		    $(".tile").mouseup(function(){     
		        $(this).removeClass("selecionado");
		    });
		    
		    $('.area').click(function(){
		    	$('#td_content_table').html('');
		    	
				id = this.id.split('_')[1];
				switch(id){
					case 'pe':
						$('.eixo').show();
						$('#caminho_nivel1').html('Planejamento Estratégico');
						$('#caminho_nivel2').html('');
						$('#caminho_nivel3').html('');
						break;
					case 'ppa':
						//$('.eixo').hide();
						//$('#div_educacao_basica, #div_educacao_profissional, #div_educacao_superior').show();
						$('.eixo').show();
						$('#caminho_nivel1').html('Plano Plurianual');
						$('#caminho_nivel2').html('');
						$('#caminho_nivel3').html('');
						break;
					case 'pne':
						//$('.eixo').hide();
						//carregarMetasPne();
						$('.eixo').show();
						$('#caminho_nivel1').html('Plano Nacional de Educação');
						$('#caminho_nivel2').html('');
						$('#caminho_nivel3').html('');
						break;
					case 'ae':
						$('.eixo').show();
						$('#caminho_nivel1').html('Ações Estratégicas');
						$('#caminho_nivel2').html('');
						$('#caminho_nivel3').html('');
						break;
					case 'orc':
						$('.eixo').show();
						$('#caminho_nivel1').html('Orçamento');
						$('#caminho_nivel2').html('');
						$('#caminho_nivel3').html('');
						break;
				}
				$('#div_roll').html('');
				
				$('.area').addClass('nao_selecionado').removeClass('marcado');				
				$(this).removeClass('nao_selecionado').addClass('marcado');
				$('.eixo').removeClass('nao_selecionado').removeClass('marcado');
			});
			
		    
		    $('.eixo').click(function(){
				$('#td_content_table').html('');
				
				idEixo = $(this).children().attr('id');
				idArea = $('.marcado').attr('id').split('_')[1];	

				var classeCss; 
				switch(idEixo){
					case '1':
						classeCss = 'azulClaro';
						$('#caminho_nivel2').html(' > Educação Básica');
						$('#caminho_nivel3').html('');
						break;
					case '2':
						classeCss = 'azulEscuro';
						$('#caminho_nivel2').html(' > Educação Profissional e Tecnológica');
						$('#caminho_nivel3').html('');						
						break;
					case '3':
						classeCss = 'marromClaro';						
						$('#caminho_nivel2').html(' > Educação Superior - Graduação, Pós-Graduação, Ensino, Pesquisa e Extensão');
						$('#caminho_nivel3').html('');
						break;
					case '4':
						classeCss = 'verdeEscuro';						
						$('#caminho_nivel2').html(' > Planejamento, Gestão e Transparência');
						$('#caminho_nivel3').html('');
						break;
				}			
								
				$('.eixo').addClass('nao_selecionado').removeClass('marcado');				
				$(this).removeClass('nao_selecionado').addClass('marcado');
				
				if(idEixo>0){
					$.ajax({
						url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
						type	: 'post',
						data	: 'requisicao=carregaRoll&exoid='+idEixo+'&area='+idArea+'&classe='+classeCss,
						success	: function(e){
							$('#div_roll').html(e);
						}
					});
				}
			});

			$('.objetivos_metas').live('click', function(){
				$('#td_content_table').html('');

				arData = this.id.split('_');
				
				$('#objetivos_metas').val(arData[1]);

				$('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');				
				$(this).removeClass('nao_selecionado').addClass('marcado');

				$('#caminho_nivel3').html(' > '+delimitador($(this).html(), 25));
				
				$.ajax({
					url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
					type	: 'post',
					data	: 'requisicao=carregaTable&id='+this.id,
					success	: function(e){
						$('#td_content_table').css('background', 'url("../imagens/fundo_opaco50.png") repeat');
						$('#td_content_table').html(e);
						if(arData[0] == 'pne') carregaTablePNE(arData[1], arData[2]);
					}
				});				
			});

			$('.desafios').live('click',function(){
				id = this.id;
	        	// Carrega Metas PNE
				$.ajax({
					url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
					type	: 'post',
					data	: 'requisicao=carregaMetasPNE&id='+id,
					success	: function(e){
						$('#div_metas_pne').html(e);
					}
				});
				
				// Carrega metas e ações
				$.ajax({
					url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
					type	: 'post',
					data	: 'requisicao=carregaMetasAcoesPE&id='+id,
					success	: function(e){
						$('#div_acoes_metas').html(e);
					}
				});
				
				// Carrega objetivos PPA
				$.ajax({
					url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
					type	: 'post',
					data	: 'requisicao=carregaObjetivosPPA&id='+$('#objetivos_metas').val(),
					success	: function(e){
						$('#div_objetivo_ppa').html(e);
					}
				});
				
				$('.desafios').addClass('nao_selecionado').removeClass('marcado');				
				$(this).removeClass('nao_selecionado').addClass('marcado');
		    });

		    $('.metas, .iniciativas').live('click', function(){
				arIds = this.id.split('_');
		    	
		    	// Carrega Desafios PE
				$.ajax({
					url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
					type	: 'post',
					data	: 'requisicao=carregaDesafiosPE&id='+arIds[1],
					success	: function(e){
						$('#div_desafios_pe').html(e);
					}
				});

				// Carrega Metas PNE e PPA
				$.ajax({
					url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
					type	: 'post',
					data	: 'requisicao=carregaMetasPNEPPA&id='+arIds[1],
					success	: function(e){
						$('#div_metas_pne').html(e);
					}
				});

				// Carrega Ações e Metas
				$.ajax({
					url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
					type	: 'post',
					data	: 'requisicao=carregaAcoesPEPPA&id='+arIds[1],
					success	: function(e){
						$('#div_acoes_metas').html(e);
					}
				});

				$('.iniciativas, .metas').removeClass('nao_selecionado').removeClass('marcado');;				
				$('.'+arIds[0]).addClass('nao_selecionado').removeClass('marcado');;				
				$(this).removeClass('nao_selecionado').addClass('marcado');
			});


			$('.clickObjetivosPPA').live('click',function(){
				//pega id - linha nº 92
				param = this.id;
				
				idEixo = param.split('_')[0];
				idObjetivosMetas = param.split('_')[1];
				
				//limpa tela
				$('#td_content_table').html('');
				$('#div_roll').html('');
				$('#caminho_nivel1').html('');
				$('#caminho_nivel2').html('');
				$('#caminho_nivel3').html('');
				
				//area
				$('.area').removeClass('nao_selecionado').removeClass('marcado');
				$('.area').addClass('nao_selecionado').removeClass('marcado');
				$('#div_ppa').removeClass('nao_selecionado').addClass('marcado');
				$('#caminho_nivel1').html('Plano Plurianual');
				
				//eixo
				$('.eixo').removeClass('nao_selecionado').removeClass('marcado');				
				$('.eixo').addClass('nao_selecionado').removeClass('marcado');
				
				idArea = $('.marcado').attr('id').split('_')[1];	
				var classeCss; 
				
				switch(idEixo){
					case '1':
						classeCss = 'azulClaro';
						$('#div_educacao_basica').removeClass('nao_selecionado').addClass('marcado');
						$('#caminho_nivel2').html(' > Educação Básica');
						$('#caminho_nivel3').html('');
						break;
					case '2':
						classeCss = 'azulEscuro';
						$('#div_educacao_profissional').removeClass('nao_selecionado').addClass('marcado');
						$('#caminho_nivel2').html(' > Educação Profissional e Tecnológica');
						$('#caminho_nivel3').html('');						
						break;
					case '3':
						classeCss = 'marromClaro';						
						$('#div_educacao_superior').removeClass('nao_selecionado').addClass('marcado');
						$('#caminho_nivel2').html(' > Educação Superior - Graduação, Pós-Graduação, Ensino, Pesquisa e Extensão');
						$('#caminho_nivel3').html('');
						break;
					case '4':
						classeCss = 'verdeEscuro';						
						$('#div_planejamento').removeClass('nao_selecionado').addClass('marcado');
						$('#caminho_nivel2').html(' > Planejamento, Gestão e Transparência');
						$('#caminho_nivel3').html('');
						break;
				}			
								
				if(idEixo>0){
					$.ajax({
						url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
						type	: 'post',
						data	: 'requisicao=carregaRoll&exoid='+idEixo+'&area='+idArea+'&classe='+classeCss,
						success	: function(e){
							$('#div_roll').html(e);
							
							$('#objetivos_metas').val(idObjetivosMetas);

							$('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');				
							$('#ppa_'+idObjetivosMetas).removeClass('nao_selecionado').addClass('marcado');
							$('#caminho_nivel3').html(' > '+delimitador($('#ppa_'+idObjetivosMetas).html(), 25));
							
							
							$.ajax({
								url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
								type	: 'post',
								data	: 'requisicao=carregaTable&id=ppa_'+idObjetivosMetas,
								success	: function(e){
									$('#td_content_table').css('background', 'url("../imagens/fundo_opaco50.png") repeat');
									$('#td_content_table').html(e);
								}
							});
							
									
						}
					});
				}
				
		    });
		    
		    
		    $('.clickMetasPNE').live('click',function(){
				//pega id - linha nº 73
				param = this.id;
				
				idEixo = param.split('_')[0];
				idMetasPNE = param.split('_')[1];
				idDseid = param.split('_')[2];
				
				//limpa tela
				$('#td_content_table').html('');
				$('#div_roll').html('');
				$('#caminho_nivel1').html('');
				$('#caminho_nivel2').html('');
				$('#caminho_nivel3').html('');
				
				//area
				$('.area').removeClass('nao_selecionado').removeClass('marcado');
				$('.area').addClass('nao_selecionado').removeClass('marcado');
				$('#div_pne').removeClass('nao_selecionado').addClass('marcado');
				$('#caminho_nivel1').html('Plano Nacional de Educação');
				
				//eixo
				$('.eixo').removeClass('nao_selecionado').removeClass('marcado');				
				$('.eixo').addClass('nao_selecionado').removeClass('marcado');
				
				idArea = $('.marcado').attr('id').split('_')[1];	
				var classeCss; 
				
				switch(idEixo){
					case '1':
						classeCss = 'azulClaro';
						$('#div_educacao_basica').removeClass('nao_selecionado').addClass('marcado');
						$('#caminho_nivel2').html(' > Educação Básica');
						$('#caminho_nivel3').html('');
						break;
					case '2':
						classeCss = 'azulEscuro';
						$('#div_educacao_profissional').removeClass('nao_selecionado').addClass('marcado');
						$('#caminho_nivel2').html(' > Educação Profissional e Tecnológica');
						$('#caminho_nivel3').html('');						
						break;
					case '3':
						classeCss = 'marromClaro';						
						$('#div_educacao_superior').removeClass('nao_selecionado').addClass('marcado');
						$('#caminho_nivel2').html(' > Educação Superior - Graduação, Pós-Graduação, Ensino, Pesquisa e Extensão');
						$('#caminho_nivel3').html('');
						break;
					case '4':
						classeCss = 'verdeEscuro';						
						$('#div_planejamento').removeClass('nao_selecionado').addClass('marcado');
						$('#caminho_nivel2').html(' > Planejamento, Gestão e Transparência');
						$('#caminho_nivel3').html('');
						break;
				}			
						
				if(idEixo>0){
					$.ajax({
						url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
						type	: 'post',
						data	: 'requisicao=carregaRoll&exoid='+idEixo+'&area='+idArea+'&classe='+classeCss,
						success	: function(e){
							$('#div_roll').html(e);
							
							$('#objetivos_metas').val(idMetasPNE);

							$('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');				
							$('#pne_'+idMetasPNE+'_'+idDseid).removeClass('nao_selecionado').addClass('marcado');
							$('#caminho_nivel3').html(' > '+delimitador($('#pne_'+idMetasPNE+'_'+idDseid).html(), 25));
							
							
							$.ajax({
								url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
								type	: 'post',
								data	: 'requisicao=carregaTable&id=pne_'+idMetasPNE,
								success	: function(e){
									$('#td_content_table').css('background', 'url("../imagens/fundo_opaco50.png") repeat');
									$('#td_content_table').html(e);
									carregaTablePNE(idMetasPNE, idDseid);
								}
							});
							
									
						}
					});
				}
				
		    });
		    
		    
		    $('.clickDesafiosPE').live('click',function(){
				//pega id - linha nº 92
				param = this.id;
				
				idEixo = param.split('_')[0];
				idObeid = param.split('_')[1];
				idDseid = param.split('_')[2];
				
				//limpa tela
				$('#td_content_table').html('');
				$('#div_roll').html('');
				$('#caminho_nivel1').html('');
				$('#caminho_nivel2').html('');
				$('#caminho_nivel3').html('');
				
				//area
				$('.area').removeClass('nao_selecionado').removeClass('marcado');
				$('.area').addClass('nao_selecionado').removeClass('marcado');
				$('#div_pe').removeClass('nao_selecionado').addClass('marcado');
				$('#caminho_nivel1').html('Planejamento Estratégico');
				
				//eixo
				$('.eixo').removeClass('nao_selecionado').removeClass('marcado');				
				$('.eixo').addClass('nao_selecionado').removeClass('marcado');
				
				idArea = $('.marcado').attr('id').split('_')[1];	
				var classeCss; 
				
				switch(idEixo){
					case '1':
						classeCss = 'azulClaro';
						$('#div_educacao_basica').removeClass('nao_selecionado').addClass('marcado');
						$('#caminho_nivel2').html(' > Educação Básica');
						$('#caminho_nivel3').html('');
						break;
					case '2':
						classeCss = 'azulEscuro';
						$('#div_educacao_profissional').removeClass('nao_selecionado').addClass('marcado');
						$('#caminho_nivel2').html(' > Educação Profissional e Tecnológica');
						$('#caminho_nivel3').html('');						
						break;
					case '3':
						classeCss = 'marromClaro';						
						$('#div_educacao_superior').removeClass('nao_selecionado').addClass('marcado');
						$('#caminho_nivel2').html(' > Educação Superior - Graduação, Pós-Graduação, Ensino, Pesquisa e Extensão');
						$('#caminho_nivel3').html('');
						break;
					case '4':
						classeCss = 'verdeEscuro';						
						$('#div_planejamento').removeClass('nao_selecionado').addClass('marcado');
						$('#caminho_nivel2').html(' > Planejamento, Gestão e Transparência');
						$('#caminho_nivel3').html('');
						break;
				}			
								
				if(idEixo>0){
					$.ajax({
						url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
						type	: 'post',
						data	: 'requisicao=carregaRoll&exoid='+idEixo+'&area='+idArea+'&classe='+classeCss,
						success	: function(e){
							$('#div_roll').html(e);
							
							$('#objetivos_metas').val(idObeid);

							$('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');				
							$('#pe_'+idObeid).removeClass('nao_selecionado').addClass('marcado');
							$('#caminho_nivel3').html(' > '+delimitador($('#pe_'+idObeid).html(), 25));
							
							
							$.ajax({
								url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
								type	: 'post',
								data	: 'requisicao=carregaTable&id=pe_'+idObeid,
								success	: function(e){
									$('#td_content_table').css('background', 'url("../imagens/fundo_opaco50.png") repeat');
									$('#td_content_table').html(e);
									
									$('.desafios').addClass('nao_selecionado').removeClass('marcado');				
									$('#des_'+idDseid).removeClass('nao_selecionado').addClass('marcado');
									$('#caminho_nivel3').html(' > '+delimitador($('#des_'+idDseid).html(), 25));
									
									// Carrega Metas PNE
									$.ajax({
										url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
										type	: 'post',
										data	: 'requisicao=carregaMetasPNE&id='+idDseid,
										success	: function(e){
											$('#div_metas_pne').html(e);
										}
									});
									
									// Carrega metas e ações
									$.ajax({
										url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
										type	: 'post',
										data	: 'requisicao=carregaMetasAcoesPE&id='+idDseid,
										success	: function(e){
											$('#div_acoes_metas').html(e);
										}
									});
									
									// Carrega objetivos PPA
									$.ajax({
										url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
										type	: 'post',
										data	: 'requisicao=carregaObjetivosPPA&id='+idObeid,
										success	: function(e){
											$('#div_objetivo_ppa').html(e);
										}
									});
									
								}
							});
							
							
									
						}
					});
				}
				
		    });
		    
		    
			$('.linkPopupIndicador').live('click',function(){				
				var popUp = window.open(this.href, 'popIndicador', 'height=600,width=800,scrollbars=yes,top=50,left=200');
				popUp.focus();
				return false;
			});
			$('.linkPopupPainel').live('click',function(){				
				var popUp = window.open(this.href, 'popPainel', 'height=600,width=800,scrollbars=yes,top=50,left=200');
				popUp.focus();
				return false;
			});
			
		});

		function carregarMetasPne()
		{
			$.ajax({
				url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
				type	: 'post',
				data	: 'requisicao=carregaRoll&area=pne',
				success	: function(e){
					$('#div_roll').html(e);
				}
			});
		}

		function carregaTablePNE(id, dseid)
		{
			// Carrega ações e metas PNE
			$.ajax({
				url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
				type	: 'post',
				data	: 'requisicao=carregaAcoesPDE&id='+id,
				success	: function(e){
					$('#td_acoes_metas').html(e);
				}
			});

			// Carrega Desafios
			$.ajax({
				url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
				type	: 'post',
				data	: 'requisicao=carregarDesafiosPNE&id='+id,
				success	: function(e){
					$('#td_planejamento_estrategico').html(e);
				}
			});

			// Carrega Objetivos PPA
			$.ajax({
				url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
				type	: 'post',
				data	: 'requisicao=carregaObjetivoPPA&id='+dseid,
				success	: function(e){
					$('#td_objetivos_ppa').html(e);
				}
			});

			// Carrega estrategias 
			$.ajax({
				url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
				type	: 'post',
				data	: 'requisicao=carregaEstrategiasPNE&id='+id,
				success	: function(e){
					$('#td_estrategias_pne').html(e);
				}
			});

		}

		function delimitador(texto, qtd)
		{
			qtd = qtd ? qtd : 150;
			total = texto.lenght;
			if(total == qtd){
				return texto;
			}else{
				return texto.substr(0, qtd)+'...';
			}
		}
		
		function buscar(busca) {
			window.open('/painel/painel.php?modulo=principal/painel_controle&acao=A&buscacockpit='+busca,'Observações','scrollbars=yes,height=800,width=1500,status=no,toolbar=no,menubar=no,location=no');
		}
		
		function exibeBuscaEnter(event){
			if(event.keyCode == 13){
				buscar(document.getElementById('busca').value);
			}
		}
	
		</script>
		
	</head>
	<body style="background: none;">
		<input type="hidden" name="objetivos_metas" id="objetivos_metas" value="" />
		
		<table width="100%" border=0>
			<tr>
				<td align="left" width="5%" style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
					<img src="../imagens/icones/icons/control.png" style="vertical-align:middle;" alt="" title="" />
				</td>
				<td  align="left" style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">					
					<font size="+2"><b>SIMEC</b></font><br/>
					Monitoramento Estratégico
				</td>
			</tr>
		</table>
		
		<br/>
		
		<table width="100%">
			<tr>
				<td class="fundo_td_laranja" colspan="4">
				    <div style="text-align:center;"  >
				    <img src="../imagens/icones/icons/executiverel.png"  style="vertical-align:middle;"  />
				    <input type="text" onclick="this.style.color='#000000';this.value='';"  name="busca" size="61" maxlength="60" value="Digite aqui o que você procura"  onfocus="this.select();" onblur="if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaEnter(event)' style='color:#D3D3D3;'    title='' class=' normal' />
				    <img src="../imagens/icones/icons/Find.png"  style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);"  />
				    </div>
			    </td>
			</tr>
		</table>
		
		<br/>
		
		<table  width="100%" cellpadding="0" cellspacing="0" border="0">
			<tr>
				<td align="right" valign="bottom" colspan="2">
					<!--  
					<a href="estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A">
						<img src="../imagens/icones/icons/mais.png" style="vertical-align:middle;" alt="Mostrar Árvore Completa" title="Mostrar Árvore Completa" border="0" /></a>
					-->
					<span id="caminho_nivel1" style="font-size:14px;"></span>
					<span id="caminho_nivel2" style="font-size:14px;"></span>
					<span id="caminho_nivel3" style="font-size:14px;"></span>
				</td>
			</tr>
			<tr>
				<td width="700" align="center" valign="top">
					<div class="pagina">	
						<div class="linha">
							<div class="tile0 tile2 tile3 verde area" id="div_pne">
							   <span class="titulo">
							    <img src="../imagens/icones/icons/alvo.png" align="top" style="float:left; 	width: 40px;height: 40px">
							     Plano Nacional de Educação</span>
							</div>
							<div class="tile0 tile2 tile3 azul area" id="div_ppa">
							    <span class="titulo">
							    <img src="../imagens/icones/icons/configs.png" align="top" style="float:left; width: 40px;height: 40px;">
							    Plano Plurianual</span>
							</div>
							<div class="tile0 tile2 tile3 amarelo area" id="div_pe">
							    <span class="titulo">
							    <img src="../imagens/icones/icons/doc.png" align="top" style="float:left; width: 40px;height: 40px;">
							    Planejamento Estratégico</span>
							</div>
						</div>
						<div class="linha">
							<div class="tile0 tile2 tile3 laranja area" style="width:320px" id="div_ae">
							   <span class="titulo">
							    <img src="../imagens/icones/icons/indicador.png" align="top" style="float:left; 	width: 40px;height: 40px">
							     Ações Estratégicas</span>
							</div>
							<div class="tile0 tile2 tile3 roxo area" style="width:320px" id="div_orc">
							   <span class="titulo">
							    <img src="../imagens/icones/icons/financeiro.png" align="top" style="float:left; 	width: 40px;height: 40px">
							     Orçamento</span>
							</div>
						</div>
						<div class="linha">
							<div class="tile1 azulClaro eixo" id="div_educacao_basica" style="display:none;">
							    <span class="SubTitulo" id="1">Educação Básica</span><br/>
							</div>
							<div class="tile1 azulEscuro eixo" id="div_educacao_profissional" style="display:none;">
							    <span class="SubTitulo" id="2">Educação Profissional e Tecnológica</span><br/>
							</div>
							<div class="tile1 marromClaro eixo" id="div_educacao_superior" style="display:none;">
							    <span class="SubTitulo" id="3">Educação Superior - Graduação, Pós-Graduação, Ensino, Pesquisa e Extensão</span><br/>
							</div>
							<div class="tile1 verdeEscuro eixo" id="div_planejamento" style="display:none;">
							    <span class="SubTitulo" id="4">Planejamento, Gestão e Transparência</span><br/>
							</div>	
						</div>
					</div>
					
					<div id="div_roll"></div>
				</td>
				<td align="center" valign="top" id="td_content_table" style="background: none;">
					&nbsp;
				</td>
			</tr>
		</table>
	</body>
</html>

<?php 

function montaGridPE()
{
	global $db;
	$arIds = explode('_', $_REQUEST['id']);
	$sql = "select dseid, obeid, dsedsc from monitora.jit_pe_desafioprioritario where obeid = {$arIds[1]}";
	$rs = $db->carregar($sql);
	?>
	<?php if($rs): ?>
		<div class="pagina" style="width:100%">			
			<table width="100%">
				<tr>
					<td bgcolor="#4682B4" width="33%"><span class="tituloInterno">Desafios</span></td>
				</tr>
				<tr>
					<td align="left" valign="top" style="padding-right:4px;">
						<div id="div_roll_pagina">
						<?php foreach($rs as $data): ?>
							<div class="tile1  tile4 azulClaro desafios" id="des_<?php echo $data['dseid']?>">
							    <span class="textoDescricao"><?php echo $data['dsedsc']; ?></span><br/>
							</div>
						<?php endforeach; ?>
						</div>
					</td>
				</tr>
			</table>
			<table width="100%">
				<tr>
					<td bgcolor="#4682B4" width="33%"><span class="tituloInterno">Objetivo PPA</span></td>
					<td bgcolor="#4682B4" width="33%"><span class="tituloInterno">Metas PNE</span></td>
					<td bgcolor="#4682B4" width="33%"><span class="tituloInterno">Ações/Metas</span></td>
				</tr>
				<tr>
					<td id="div_objetivo_ppa" align="left" valign="top" style="padding-right:4px;"></td>
					<td id="div_metas_pne" align="left" valign="top" style="padding-right:4px;"></td>
					<td id="div_acoes_metas" align="left" valign="top" style="padding-right:4px;"></td>
				</tr>
			</table>
		</div>
	<?php endif; ?>
	<?php 
}

function montaGridPPA()
{
	global $db;
	$arIds = explode('_', $_REQUEST['id']);
	$sql = "select metdsc, metid, metmeta, metiniciativa, metidpne from monitora.jit_ppa_meta where objid = {$arIds[1]}";
	$rs = $db->carregar($sql);
	?>
	<?php if($rs): ?>
		<div class="pagina" style="width:100%">	
			
			<table width="100%" cellpadding="3">
				<tr>
					<td bgcolor="#4682B4" width="50%"><span class="titulo">Metas</span></td>
					<td bgcolor="#4682B4" width="50%"><span class="titulo">Iniciativas</span></td>
				</tr>
				<tr>
					<td align="left" valign="top">
						<div id="div_roll_pagina">
						<?php foreach($rs as $data): ?>
							<?php if($data['metmeta'] == 't'): ?>
								<div class="tile1 tile4 azulClaro metas" id="metas_<?php echo $data['metidpne'].'_*'.$data['metid']?>">
								    <span class="textoDescricao"><?php echo $data['metdsc']; ?></span><br/>
								</div>
							<?php endif; ?>
						<?php endforeach; ?>
						</div>
					</td>
					<td align="left" valign="top">
						<div id="div_roll_pagina">
						<?php foreach($rs as $data): ?>
							<?php if($data['metiniciativa'] == 't'): ?>
								<div class="tile1 tile4 azulClaro iniciativas" id="iniciativas_<?php echo $data['metidpne']?>">
								    <span class="textoDescricao"><?php echo $data['metdsc']; ?></span><br/>
								</div>
							<?php endif; ?>
						<?php endforeach; ?>
						</div>
					</td>
				</tr>
			</table>
			
			<table width="100%" cellpadding="3" style="margin-left: 5px;">
				<tr>
					<td bgcolor="#4682B4" width="33%"><span class="titulo">Desafios PE</span></td>
					<td bgcolor="#4682B4" width="33%"><span class="titulo">Metas PNE</span></td>
					<td bgcolor="#4682B4" width="33%"><span class="titulo">Ações/Metas</span></td>
				</tr>
				<tr>
					<td id="div_desafios_pe" align="left" valign="top"></td>
					<td id="div_metas_pne" align="left" valign="top"></td>
					<td id="div_acoes_metas" align="left" valign="top"></td>
				</tr>
			</table>
		</div>
	<?php endif; ?>
	<?php 
}


function montaGridAE()
{
	global $db;
	$arIds = explode('_', $_REQUEST['id']);
	$sql = "select acadetalhe from monitora.jit_acoes where acaoid = {$arIds[1]}";
	$rs = $db->carregar($sql);
	?>
	<div class="pagina" style="width:100%">
		<table width="100%" cellpadding="3">
			<tr>			
				<td  bgcolor="#4682B4"><span class="titulo">Detalhe da Ação</span></td>			
			</tr>	
			<tr>
				<td colspan="3">
					<div id="div_roll_pagina">
						<div id="td_acoes_ae"  class="tile1  tile4 azulClaro acoes" >
						<? 
						if ($rs)	
						{
							echo($rs[0]['acadetalhe']);
						}				
						?>
						</div>
					</div>
				</td>
			</tr>
		</table>
	</div>
	<?php 
}

function montaGridPNE()
{	
	?>	
	<div class="pagina" style="width:100%">
		<table width="100%" cellpadding="3">
			<tr>			
				<td colspan="3" bgcolor="#4682B4"><span class="titulo">Estratégias</span></td>			
			</tr>	
			<tr>
				<td colspan="3">
					<div id="div_roll_pagina">
						<div id="td_estrategias_pne">
						</div>
					</div>
				</td>
			</tr>
			<tr>
				<td align="left" valign="top" width="33%" bgcolor="#4682B4"><span class="titulo">Objetivos PPA</span></td>
				<td align="left" valign="top" width="33%" bgcolor="#4682B4"><span class="titulo">Planejamento Estratégico</span></td>
				<td align="left" valign="top" width="33%" bgcolor="#4682B4"><span class="titulo">Ações/Metas</span></td>
			</tr>
			<tr>
				<td id="td_objetivos_ppa" align="left" valign="top"></td>
				<td id="td_planejamento_estrategico" align="left" valign="top"></td>
				<td id="td_acoes_metas" align="left" valign="top" ></td>
			</tr>
		</table>
	</div>
	<?php 
}

function delimitador($texto){
	if(strlen($texto) > 120){
		$texto = substr($texto,0,120).'...';
	}
	return $texto;
}

if($_REQUEST['chamadaExterna']){
	// exexmplo para chamar a partir de outra pagina 
	//chamadaExterna=1&area=pe&ideixo=1&idobjetivo=3&iddesafio=1
	//idobjetivo2 = quando tiver dois codigos no objetivo (casos do pne)
	//areas possívei pe,ppa,ae,orc,pne
	//codelmeta, codeliniciativa = numero que vem apos o caracter * (ver propriedades do div na tabela de metas/iniciativas)
echo "<script>		
	$(document).ready(function() {
	var area            = '{$_REQUEST['area']}';
	var ideixo 	        = '{$_REQUEST['ideixo']}';
	var idobjetivo      = '{$_REQUEST['idobjetivo']}';
	var idobjetivo2     = '{$_REQUEST['idobjetivo2']}';
	var iddesafio       = '{$_REQUEST['iddesafio']}';
	var idmeta          = '{$_REQUEST['idmeta']}';
	var idiniciativa    = '{$_REQUEST['idiniciativa']}';
	var codelmeta       = '{$_REQUEST['codelmeta']}';
	var codeliniciativa = '{$_REQUEST['codeliniciativa']}';
	
	//variaveis locais
	var classe = '';
	
	
	$('#div_'+area).trigger('click');
	
	if (ideixo != '')
	{
		$('#td_content_table').html('');
		
		if (ideixo == 1)
		{
			classe = 'azulClaro';
			$('#caminho_nivel2').html(' > Educação Básica');
			$('#caminho_nivel3').html('');
		}
		if (ideixo == 2)
		{
			classe = 'azulEscuro';
			$('#caminho_nivel2').html(' > Educação Profissional e Tecnológica');
			$('#caminho_nivel3').html('');
		}
		
		if (ideixo == 3)
		{
			classe = 'marromClaro';
			$('#caminho_nivel2').html(' > Educação Superior - Graduação, Pós-Graduação, Ensino, Pesquisa e Extensão');
			$('#caminho_nivel3').html('');
		}
		
		if (ideixo == 4)
		{
			classe = 'verdeEscuro';
			$('#caminho_nivel2').html(' > Planejamento, Gestão e Transparência');
			$('#caminho_nivel3').html('');
		}
												
		$.ajax({
			url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
			type	: 'post',
			data	: 'requisicao=carregaRoll&exoid='+ideixo+'&area='+area+'&classe='+classe,
			success	: function(e){
				$('#div_roll').html(e);
			
				$('div[id=div_educacao_basica]').addClass('nao_selecionado').removeClass('marcado');
				$('div[id=div_educacao_profissional]').addClass('nao_selecionado').removeClass('marcado');
				$('div[id=div_educacao_superior]').addClass('nao_selecionado').removeClass('marcado');
				$('div[id=div_planejamento]').addClass('nao_selecionado').removeClass('marcado');
		
				if (ideixo == 1)
				{
					$('div[id=div_educacao_basica]').removeClass('nao_selecionado').addClass('marcado');				
				}
				if (ideixo == 2)
				{
					$('div[id=div_educacao_profissional]').removeClass('nao_selecionado').addClass('marcado');				
				}
				if (ideixo == 3)
				{
					$('div[id=div_educacao_superior]').removeClass('nao_selecionado').addClass('marcado');				
				}
				if (ideixo == 4)
				{
					$('div[id=div_planejamento]').removeClass('nao_selecionado').addClass('marcado');				
				}
				
			}
		});
	}
	
	if (idobjetivo != '')
	{
		$('#td_content_table').html('');	
	
		$.ajax({
			url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
			type	: 'post',
			data	: 'requisicao=carregaTable&id='+area+'_'+idobjetivo,
			success	: function(e){
				$('#td_content_table').css('background', 'url(\'../imagens/fundo_opaco50.png\') repeat');
				$('#td_content_table').html(e);
				
				if (area != 'pne')
				{
					$('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');
					$('#'+area+'_'+idobjetivo).removeClass('nao_selecionado').addClass('marcado');
					
					$('#div_lnk_objetivos_metas2').scrollTop($('#div_lnk_objetivos_metas2').scrollTop() + ($('#'+area+'_'+idobjetivo).position().top - $('#div_lnk_objetivos_metas2').position().top) - ($('#div_lnk_objetivos_metas2').height()/2) + ($('#'+area+'_'+idobjetivo).height()/2) );
					
				}
				else
				{
					carregaTablePNE(idobjetivo, idobjetivo2);
					$('.objetivos_metas').addClass('nao_selecionado').removeClass('marcado');
					$('div[id^='+area+'_'+idobjetivo+'_'+idobjetivo2+']').removeClass('nao_selecionado').addClass('marcado');
					
					//para fazer scroll ate o objetivo selecionado
					$('#div_lnk_objetivos_metas').scrollTop($('#div_lnk_objetivos_metas').scrollTop() + ($('div[id^='+area+'_'+idobjetivo+'_'+idobjetivo2+']').position().top - $('#div_lnk_objetivos_metas').position().top) - ($('#div_lnk_objetivos_metas').height()/2) + ($('div[id^='+area+'_'+idobjetivo+'_'+idobjetivo2+']').height()/2) );
				}
					
			}
		});
	}
	
	if (iddesafio !='')
	{
		$.ajax({
			url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
			type	: 'post',
			data	: 'requisicao=carregaMetasPNE&id=des_'+iddesafio,
			success	: function(e){
				$('#div_metas_pne').html(e);
			}
		});
				
		// Carrega metas e ações
		$.ajax({
			url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
			type	: 'post',
			data	: 'requisicao=carregaMetasAcoesPE&id=des_'+iddesafio,
			success	: function(e){
				$('#div_acoes_metas').html(e);
			}
		});
		
		
		$.ajax({
			url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
			type	: 'post',
			data	: 'requisicao=carregaObjetivosPPA&id=des_'+iddesafio,
			success	: function(e){
				$('#div_objetivo_ppa').html(e);
				$('.desafios').addClass('nao_selecionado').removeClass('marcado');				
				$('div[id=des_'+iddesafio+']' ).removeClass('nao_selecionado').addClass('marcado');
			}
		});
	}
	
	if (codelmeta != '' || codeliniciativa != '')
	{
	
		var descricao = '';
		
		if (codelmeta != '')
		{
			id = idmeta;
			descricao = 'metas';
			codel = codelmeta;
		}
		else
		{
			id = idiniciativa;
			descricao = 'iniciativas';
			codel = codeliniciativa;
			
		}
		
		$.ajax({
			url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
			type	: 'post',
			data	: 'requisicao=carregaDesafiosPE&id='+id,
			success	: function(e){
				$('#div_desafios_pe').html(e);
			}
		});
	
		$.ajax({
			url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
			type	: 'post',
			data	: 'requisicao=carregaMetasPNEPPA&id='+id,
			success	: function(e){
				$('#div_metas_pne').html(e);
			}
		});
	
		$.ajax({
			url		: 'estrategico.php?modulo=relatorio/estrategico/relatorio_jit_pe&acao=A',
			type	: 'post',
			data	: 'requisicao=carregaAcoesPEPPA&id='+id,
			success	: function(e){
				$('#div_acoes_metas').html(e);
				$('.iniciativas, .metas').removeClass('nao_selecionado').removeClass('marcado');			
				$('.'+descricao).addClass('nao_selecionado').removeClass('marcado');		
				$('div[id$=*'+codel+']').removeClass('nao_selecionado').addClass('marcado');

				//codigo para fazer um scroll ate a meta/iniciativa selecionada
				$('#div_roll_pagina').scrollTop($('#div_roll_pagina').scrollTop() + ($('div[id$=*'+codel+']').position().top - $('#div_roll_pagina').position().top) - ($('#div_roll_pagina').height()/2) + ($('div[id$=*'+codel+']').height()/2) );
			}
		});
	}
});
					
</script>";

}


?>