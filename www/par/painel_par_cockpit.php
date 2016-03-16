<?php
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html lang="pt-BR" xml:lang="pt-BR" xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>SIMEC - Painel Estratégico</title>
	<link href="./cockpit/barra_do_governo/css/barra_do_governo.css" rel="stylesheet" media="screen" type="text/css" />
	<link rel="stylesheet" type="text/css" media="all" href="./cockpit/css/principal.css">
	<link href="./cockpit/images/favicon.ico" rel="shortcut icon" type="image/x-icon"/>
	<style media="all">#barra-brasil-v3-marca { width:1000px }</style>
	<script src="./cockpit/js/cufon-yui.js" type="text/javascript"></script>
	<script src="./cockpit/js/Segoe_WP_N_Light_300.font.js" type="text/javascript"></script>
	<script type="text/javascript">
		//Cufon.replace('h1'); // Works without a selector engine
		Cufon.replace('.menu_obras'); 
		Cufon.replace('.menu_pac_programa'); 
		Cufon.replace('.menu_transporte_escolar');
		Cufon.replace('.menu_tecnologia_escola');
		Cufon.replace('.menu_mobiliario_equipamento'); 
		Cufon.replace('.menu_dinheiro_escola');
		Cufon.replace('.menu_fundeb');
		Cufon.replace('.txt_programa');
	</script>
</head>
<body>
	<div id="barra-brasil-v3" class="barraGoverno">		
		<div id="barra-brasil-v3-marca">Brasil &ndash; Governo Federal &ndash; Minist&eacute;rio da Educa&ccedil;&atilde;o</div>
	</div>
	<div class="wrapper">
		<div class="borda">
			<div class="container">
				<div class="page">
					<div class="section">
						<div class="row">
							<div class="column col4of5">
								<div class="content">
									<div class="marca">
										<h1 class="simec">
											<a tabindex="1" title="Voltar à página inicial" class="txtIndent" href="estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A">Simec</a>
										</h1>
									</div>
								</div>
							</div>
							<div class="column col1of5 lastColumn">
								<div class="content">
									<div class="atualizar">
									<a tabindex="2" title="Atualizar" class="txtIndent" href="estrategico.php?modulo=principal/painel_cockpit&acao=A">Atualizar</a>
									</div>
								</div>
							</div>
						</div>
						<div class="row">
							<div class="column col1of1">
								<div class="content">
									<div class="img_monitoramento">
									<a tabindex="1" title="Voltar à página Painel Estratégico" class="txtIndent" href="estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A">Painel Estratégico</a>
									</div>
								</div>
							</div>
						</div>
						<br>
						
						<div class="row">
							<div class="column col1of3">
								<div class="content">
									<div class="menu_obras">Obras - PAR</div>
									<div class="obra_par_programa" onclick="window.location.href='painel_estrategico_detalhe.php'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_mais_educacao.jpg" alt="Mais Educação" width="316" height="67"></div>
										<div class="txt_programa">Saiba mais...</div>
									</div>
								</div>
							</div>
							<div class="column col1of3">
								<div class="content">
									<div class="menu_pac_programa">Obras - PAC</div>
									<div class="obra_pac_programa" onclick="window.location.href='painel_estrategico_detalhe.php'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_mais_educacao.jpg" alt="Mais Educação" width="316" height="67"></div>
										<div class="txt_programa">Saiba mais...</div>
									</div>
								</div>
							</div>
							<div class="column col1of3">
								<div class="content">
									<div class="menu_transporte_escolar">Transporte Escolar</div>
									<div class="transporte_escolar_programa" onclick="window.location.href='painel_estrategico_detalhe.php'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_brasil_profissionalizado.jpg" alt="Brasil Profissionalizado" width="316" height="67"></div>
										<div class="txt_programa">Saiba mais...</div>
									</div>
								</div>
							</div>
							<div class="column col1of3 lastColumn">
								<div class="content">
									<div class="menu_tecnologia_escola">Tecnologia na Escola</div>
									<div class="tecnologia_escola_programa" onclick="window.location.href='painel_estrategico_detalhe.php'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_ed_superior.jpg" alt="Expansão da Educação Superior" width="316" height="67"></div>
										<div class="txt_programa">Saiba mais...</div>
									</div>
								</div>
							</div>
							<div class="column col1of3">
								<div class="content">
									<div class="menu_mobiliario_equipamento">Mobiliário Equipamento</div>
									<div class="mobiliario_equipamento_programa" onclick="window.location.href='painel_estrategico_detalhe.php'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_mais_educacao.jpg" alt="Mais Educação" width="316" height="67"></div>
										<div class="txt_programa">Saiba mais...</div>
									</div>
								</div>
							</div>
							<div class="column col1of3">
								<div class="content">
									<div class="menu_dinheiro_escola">Dinheiro Direto na Escola</div>
									<div class="dinheiro_direto_escola_programa" onclick="window.location.href='painel_estrategico_detalhe.php'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_brasil_profissionalizado.jpg" alt="Brasil Profissionalizado" width="316" height="67"></div>
										<div class="txt_programa">Saiba mais...</div>
									</div>
								</div>
							</div>
							<div class="column col1of3 lastColumn">
								<div class="content">
									<div class="menu_fundeb">FUNDEB</div>
									<div class="fundeb_programa" onclick="window.location.href='painel_estrategico_detalhe.php'" style="cursor:pointer;">
										<div class="img_programa"><img src="./cockpit/images/img_ed_superior.jpg" alt="Expansão da Educação Superior" width="316" height="67"></div>
										<div class="txt_programa">Saiba mais...</div>
									</div>
								</div>
							</div>
						</div>
						<br>
						<div class="content">
							<div class="relatorio">Relatório Consolidado</div>
						</div>
						
					</div>
				</div>
			</div>
		</div>
	</div>
</body>
</html>