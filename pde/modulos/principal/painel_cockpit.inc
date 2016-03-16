<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<?php 
$perfis = pegaPerfilGeral($_SESSION['usucpf']);

$acessoViverSemLimites = false;
$acessoPronatec = false;
$acessoPronacampo = false;
$acessoBrasilSemMiseria = false;
$acessoPlanoNacionalEducacao = false;
$acessoMaisEducacao = false;
$acessoBrasilProfissionalizado = false;
$acessoExpansaoEducacaoSuperior = false;
$acessoConstrucaoCoberturaQuadras = false;
$acessoBolsaFormacao = false;
$acessoCienciaSemFronteiras = false;
$acessoCrechePreEscola = false;
$acessoExpansaoRedeFederalEpt = false;
$acessoCaminhoEscola = false;
$acessoMulheresMil = false;
$acessoMonitoramentoEnem = false;
$acessoUniversidadeAberta = false;
$acessoEducacaoDigital = false;
$acessoFormacaoProfessores = false;
$acessoEnsinoMedioInovador = false;
$acessoPactoAlfabetizacaoIdadeCerta = false;
$acessoProinfanciaExecucao = false;

//verifica se é adm , su ou consulta e libera acesso pra todos 
if (in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_ADMINISTRADOR, $perfis) || in_array(PERFIL_CONSULTA, $perfis)  )
{
	$acessoViverSemLimites = true;
	$acessoPronatec = true;
	$acessoPronacampo = true;
	$acessoBrasilSemMiseria = true;
	$acessoPlanoNacionalEducacao = true;
	$acessoMaisEducacao = true;
	$acessoBrasilProfissionalizado = true;
	$acessoExpansaoEducacaoSuperior = true;
	$acessoConstrucaoCoberturaQuadras = true;
	$acessoBolsaFormacao = true;
	$acessoCienciaSemFronteiras = true;
	$acessoCrechePreEscola = true;
	$acessoExpansaoRedeFederalEpt = true;
	$acessoCaminhoEscola = true;
	$acessoMulheresMil = true;
	$acessoMonitoramentoEnem = true;
	$acessoUniversidadeAberta = true;
	$acessoEducacaoDigital = true;
	$acessoFormacaoProfessores = true;
	$acessoEnsinoMedioInovador = true;
	$acessoPactoAlfabetizacaoIdadeCerta = true;
	$acessoProinfanciaExecucao = true;
}
else
{
	$sql = "select _atiprojeto::integer from pde.responsavelatividade ra
			inner join pde.atividade a on ra.atiid = a.atiid
			inner join entidade.entidade ent on ent.entid = ra.entid
			where ent.entnumcpfcnpj = '{$_SESSION['usucpf']}'
			group by _atiprojeto";
	$projetos = $db->carregar($sql);
	
	if ($projetos)
	{
		foreach ($projetos as $projeto)
		{
			switch ($projeto['_atiprojeto']){
				case 131178:
					$acessoBrasilSemMiseria = true;
					$acessoViverSemLimites = true;
					$acessoPronatec = true;
					break;
				case 129808:
					$acessoViverSemLimites = true;
					break;
				case 129708:
					$acessoViverSemLimites = true;
					break;
				case 129798:
					$acessoViverSemLimites = true;
					break;
				case 129759:
					$acessoViverSemLimites = true;
					break;
				case 129716:
					$acessoViverSemLimites = true;
					break;
				case 129326:
					$acessoPronatec = true;
					break;
				case 129397:
					$acessoPronatec = true;
					$acessoBolsaFormacao = true;
					break;
				case 129399:
					$acessoPronatec = true;
					break;
				case 129347:
					$acessoPronatec = true;
					break;
				case 129373:
					$acessoPronatec = true;
					$acessoExpansaoRedeFederalEpt = true;
					break;
				case 130765:
					$acessoPronacampo = true;
					break;
				case 130761:
					$acessoPronacampo = true;
					break;
				case 130758:
					$acessoPronacampo = true;
					$acessoFormacaoProfessores = true;
					break;
				case 132193:
					$acessoPronacampo = true;
					$acessoFormacaoProfessores = true;
					break;
				case 133270:
					$acessoPronacampo = true;
					$acessoEducacaoDigital = true;
					break;
				case 130762:
					$acessoPronacampo = true;
					$acessoFormacaoProfessores = true;
					break;
				case 132022:
					$acessoPronacampo = true;
					break;
				case 132034:
					$acessoPronacampo = true;
					break;
				case 130760:
					$acessoPronacampo = true;
					break;
				case 130759:
					$acessoPronacampo = true;
					break;
				case 130766:
					$acessoPronacampo = true;
					$acessoCaminhoEscola = true;
					break;
				case 63631:
					$acessoBrasilSemMiseria = true;
					break;
				case 129250:
					$acessoBrasilSemMiseria = true;
					$acessoMulheresMil = true;
					break;
				case 129399:
					$acessoBrasilProfissionalizado = true;
					break;
				case 129543:
					$acessoExpansaoEducacaoSuperior = true;
					break;
				case 55654:
					$acessoExpansaoEducacaoSuperior = true;
					break;
				case 129209:
					$acessoExpansaoEducacaoSuperior = true;
					break;
				case 129596:
					$acessoConstrucaoCoberturaQuadras= true;
					break;
				case 130531:
					$acessoCienciaSemFronteiras = true;
					break;
				case 129185:
					$acessoCrechePreEscola = true;
					break;
				case 129391:
					$acessoUniversidadeAberta = true;
					$acessoFormacaoProfessores = true;
					break;
				case 133490:
					$acessoEducacaoDigital = true;
					break;
				case 133996:
					$acessoFormacaoProfessores = true;
					break;
				case 129267:
					$acessoFormacaoProfessores = true;
					break;
				case 129779:
					$acessoEnsinoMedioInovador = true;
					break;
				case 129348:
					$acessoMaisEducacao = true;
					break;
				case 140635:
					$acessoMaisEducacao = true;
					break;
				case 140859:
					$acessoMaisEducacao = true;
					break;
				case 133995:
					$acessoPactoAlfabetizacaoIdadeCerta = true;
					break;
				case 142017:
					$acessoPactoAlfabetizacaoIdadeCerta = true;
					break;
			}
		}
	}
}

//cockpits que nao foram especificados quem pode acessar
$acessoPlanoNacionalEducacao = true;
$acessoMonitoramentoEnem = true;
$acessoProinfanciaExecucao = true;
?>


<html lang="pt-BR" xml:lang="pt-BR" xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title>SIMEC - Monitoramento Estratégico</title>
	<link href="./cockpit/barra_do_governo/css/barra_do_governo.css" rel="stylesheet" media="screen" type="text/css" />
	<link rel="stylesheet" type="text/css" media="all" href="./cockpit/css/principal.css">
	<link href="./cockpit/images/favicon.ico" rel="shortcut icon" type="image/x-icon"/>
	<style media="all">#barra-brasil-v3-marca { width:1000px }</style>
	<script src="./cockpit/js/cufon-yui.js" type="text/javascript"></script>
	<script src="./cockpit/js/Segoe_WP_N_Light_300.font.js" type="text/javascript"></script>
	<script type="text/javascript">
		//Cufon.replace('h1'); // Works without a selector engine
		Cufon.replace('.ed_basica'); 
		Cufon.replace('.ed_profissional');
		Cufon.replace('.ed_superior');
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
							<div class="column col3of10">
								<div class="content">
									<div class="marca">
										<h1 class="simec">
										<a tabindex="1" title="Voltar à página inicial" class="txtIndent" href="estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A">Simec</a>
										</h1>
									</div>
								</div>
							</div>
							<div class="column col3of10">
								<div class="content">
									<div align="center" style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/alinhamento_estrategico&acao=A';">
									<img src="./cockpit/images/img_alinhamento.png" style="vertical-align:middle;" alt="" title="" width="316" height="67" />
									<p><font size="+2" color="#FFFFFF" ><b>Alinhamento Estratégico</b></font></p>
									</div>
								</div>
							</div>
							<div class="column col1of5">
								<div class="content">
									<div align="right">	
										<a href="wap/"><img src="cockpit/images/mobileicon2.png" /></a>
										<p ><font color=#FFFFFF>Mobile</p>
									</div>
								</div>
							</div>
							<div class="column col1of5">
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
									<a tabindex="1" title="Voltar à página Monitoramento Estratégico" class="txtIndent" href="estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A">Monitoramento Estratégico</a>
									</div>
								</div>
							</div>
						</div>
						<br>
						<div class="content">
							<div class="agenda">Agendas</div>
						</div>
						<div class="row">
							<div class="column col1of3">
								<div class ="content2">
									<div class="ag_programa" onclick=<?=$acessoViverSemLimites?"window.location.href='estrategico.php?modulo=principal/cockpit_viver_sem_limites&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoViverSemLimites?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_viver_sem_limite.jpg" alt="Viver Sem Limite" width="316" height="67"></div>
										<div class="txt_programa">Viver Sem Limite</div>
									</div>
								</div>
							</div>
							<div class="column col1of3">
								<div class ="content2">
									<div class="ag_programa" onclick=<?=$acessoPronatec?"window.location.href='estrategico.php?modulo=principal/cockpit_pronatec&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoPronatec?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_pronatec.jpg" alt="Pronatec" width="316" height="67"></div>
										<div class="txt_programa">Pronatec</div>
									</div>
								</div>
							</div>
							<div class="column col1of3">
								<div class ="content2">
									<div class="ag_programa" onclick=<?=$acessoPronacampo?"window.location.href='estrategico.php?modulo=principal/cockpit_pronacampo&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoPronacampo?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_pronacampo.jpg" alt="Pronacampo" width="316" height="67"></div>
										<div class="txt_programa">Pronacampo</div>
									</div>
								</div>
							</div>
							<div class="column col1of3">
								<div class ="content2">
									<div class="ag_programa" onclick=<?=$acessoBrasilSemMiseria?"window.location.href='estrategico.php?modulo=principal/cockpit_brasil_sem_miseria&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoBrasilSemMiseria?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_brasil_sem_miseria.jpg" alt="Brasil sem Miséria" width="316" height="67"></div>
										<div class="txt_programa">Brasil sem miséria</div>
									</div>
								</div>
							</div>
							<div class="column col1of3">
								<div class ="content2">
									<div class="ag_programa" onclick=<?=$acessoPlanoNacionalEducacao?"window.location.href='estrategico.php?modulo=principal/cockpit_pne&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoPlanoNacionalEducacao?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_pne.jpg" alt="Plano Nacional de Educação" width="316" height="67"></div>
										<div class="txt_programa">Plano Nacional de Educação</div>
									</div>
								</div>
							</div>
						</div>
						<div class="row">
							<div class="column col1of3">
								<div class="content">
									<div class="ed_basica">Educação Básica</div>
									<div class="eb_programa" onclick=<?=$acessoMaisEducacao?"window.location.href='estrategico.php?modulo=principal/cockpit_mais_educacao&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoMaisEducacao?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_mais_educacao.jpg" alt="Mais Educação" width="316" height="67"></div>
										<div class="txt_programa">Mais Educação</div>
									</div>
									<div class="eb_programa" onclick=<?=$acessoConstrucaoCoberturaQuadras?"window.location.href='estrategico.php?modulo=principal/quadras_estrategico&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoConstrucaoCoberturaQuadras?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_quadras.jpg" alt="Construção e cobertura de quadras esportivas escolares" width="316" height="67"></div>
										<div class="txt_programa">Construção e cobertura de quadras esportivas</div>
									</div>
									<div class="eb_programa" onclick=<?=$acessoCrechePreEscola?"window.location.href='estrategico.php?modulo=principal/cockpit_estrategico&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoCrechePreEscola?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_pro_infancia.jpg" alt="Creches e Pré-escolas" width="316" height="67"></div>
										<div class="txt_programa">Creches e Pré-escolas</div>
									</div>
									<div class="eb_programa" onclick=<?=$acessoProinfanciaExecucao?"window.location.href='estrategico.php?modulo=principal/cockpit_proinfancia_execucao&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoProinfanciaExecucao?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_obra.jpg" alt="Proinfância Execução" width="316" height="67"></div>
										<div class="txt_programa">Proinfância Execução</div>
									</div>
									<div class="eb_programa" onclick=<?=$acessoCaminhoEscola?"window.location.href='estrategico.php?modulo=principal/cockpit_caminhos_escola&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoCaminhoEscola?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_caminhos.jpg" alt="Caminho da Escola" width="316" height="67"></div>
										<div class="txt_programa">Caminho da Escola</div>
									</div>
									<div class="eb_programa" onclick=<?=$acessoMonitoramentoEnem?"window.location.href='estrategico.php?modulo=principal/cockpit_enem2013&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoMonitoramentoEnem?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_enem.jpg" alt="Monitoramento ENEM 2013" width="316" height="67"></div>
										<div class="txt_programa">Monitoramento ENEM 2013</div>
									</div>
									<div class="eb_programa" onclick=<?=$acessoUniversidadeAberta?"window.location.href='estrategico.php?modulo=principal/cockpit_uab&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoUniversidadeAberta?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_polos.jpg" alt="Universidade Aberta do Brasil - UAB" width="316" height="67"></div>
										<div class="txt_programa">Universidade Aberta do Brasil - UAB</div>
									</div>
									<div class="eb_programa" onclick=<?=$acessoEducacaoDigital?"window.location.href='estrategico.php?modulo=principal/cockpit_educacao_digital&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoEducacaoDigital?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_educacao_digital.jpg" alt="Educação Digital" width="316" height="67"></div>
										<div class="txt_programa">Educação Digital</div>
									</div>
									<div class="eb_programa" onclick=<?=$acessoFormacaoProfessores?"window.location.href='estrategico.php?modulo=principal/cockpit_formacao_professores&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoFormacaoProfessores?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_formacao_professores.jpg" alt="Formação de Professores" width="316" height="67"></div>
										<div class="txt_programa">Formação de Professores</div>
									</div>
									<div class="eb_programa" onclick=<?=$acessoEnsinoMedioInovador?"window.location.href='estrategico.php?modulo=principal/cockpit_ensino_medio_inovador&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoEnsinoMedioInovador?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_ensino_medio_inovador.jpg" alt="Ensino Médio Inovador" width="316" height="67" /></div>
										<div class="txt_programa">Ensino Médio Inovador</div>
									</div>
									<div class="eb_programa" onclick=<?=$acessoPactoAlfabetizacaoIdadeCerta?"window.location.href='estrategico.php?modulo=principal/cockpit_pacto&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoPactoAlfabetizacaoIdadeCerta?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_pacto.jpg" alt="Pacto pela Alfabetização na Idade Certa" width="316" height="67" /></div>
										<div class="txt_programa">Pacto pela Alfabetização na Idade Certa</div>
									</div>
								</div>
							</div>
							<div class="column col1of3">
								<div class="content">
									<div class="ed_profissional">Educação Profissional</div>
									<div class="ep_programa" onclick=<?=$acessoBrasilProfissionalizado?"window.location.href='estrategico.php?modulo=principal/cockpit_brasil_pro&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoBrasilProfissionalizado?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_brasil_profissionalizado.jpg" alt="Brasil Profissionalizado" width="316" height="67"></div>
										<div class="txt_programa">Brasil Profissionalizado</div>
									</div>
									<div class="ep_programa" onclick=<?=$acessoBolsaFormacao?"window.location.href='estrategico.php?modulo=principal/cockpit_bolsa_formacao&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoBolsaFormacao?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_bolsa_formacao.jpg" alt="Bolsa-Formação" width="316" height="67"></div>
										<div class="txt_programa">Bolsa-Formação</div>
									</div>
									<div class="ep_programa" onclick=<?=$acessoExpansaoRedeFederalEpt?"window.location.href='estrategico.php?modulo=principal/cockpit_rede_federal&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoExpansaoRedeFederalEpt?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_expansaorfept.jpg" alt="Expansão da Rede Federal de EPT" width="316" height="67"></div>
										<div class="txt_programa">Expansão da Rede Federal de EPT</div>
									</div>
									<div class="ep_programa" onclick=<?=$acessoMulheresMil?"window.location.href='estrategico.php?modulo=principal/cockpit_mulheres_mil&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoMulheresMil?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_mulheres_mil.jpg" alt="Mulheres Mil" width="316" height="67" /></div>
										<div class="txt_programa">Mulheres Mil</div>
									</div>
								</div>
							</div>
							<div class="column col1of3 lastColumn">
								<div class="content">
									<div class="ed_superior">Educação Superior</div>
									<div class="es_programa" onclick=<?=$acessoExpansaoEducacaoSuperior?"window.location.href='estrategico.php?modulo=principal/cockpit_educacao_superior&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoExpansaoEducacaoSuperior?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_ed_superior.jpg" alt="Expansão da Educação Superior" width="316" height="67"></div>
										<div class="txt_programa">Expansão da Educação Superior</div>
									</div>
									<div class="es_programa" onclick=<?=$acessoCienciaSemFronteiras?"window.location.href='estrategico.php?modulo=principal/cockpit_ciencia_sem_fronteiras&acao=A'":""?> style="cursor:pointer;">
										<div class="<?=$acessoCienciaSemFronteiras?'img_programa':'sem_acesso' ?>"><img src="./cockpit/images/img_ciencia_sem_fronteiras.jpg" alt="Ciência sem Fronteiras" width="316" height="67"></div>
										<div class="txt_programa">Ciência sem Fronteiras</div>
									</div>
								</div>
							</div>
						</div>
						
					</div>
				</div>
			</div>
		</div>
	</div>
</body>
</html>