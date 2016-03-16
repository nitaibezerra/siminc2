<?php
include "config.inc";
include_once APPRAIZ . "educriativa/autoload.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$organizacaoAtuacao = new Educriativa_Model_OrganizacaoAreaAtuacao();
$organizacaoNivel	= new Educriativa_Model_OrganizacaoNivelEnsino();
$organizacaoRede	= new Educriativa_Model_OrganizacaoRede();
$organizacaoTipo	= new Educriativa_Model_OrganizacaoTipo();
$organizacaoSite	= new Educriativa_Model_OrganizacaoSiteWeb();
$organizacaoFaixa	= new Educriativa_Model_OrganizacaoFaixaEtaria();
$questionario 		= new Educriativa_Model_Questionario($_SESSION['queid']);
$particiante 		= new Educriativa_Model_Participante($_SESSION['parid']);
$organizacao		= new Educriativa_Model_Organizacao($_SESSION['orgid']);
$pergunta 			= new Educriativa_Model_Pergunta();
$resposta 			= new Educriativa_Model_Resposta();
$atuacao 			= new Educriativa_Model_AreaAtuacao();
$grupo 				= new Educriativa_Model_Grupo();
$nivel 				= new Educriativa_Model_NivelEnsino();
$rede 				= new Educriativa_Model_Rede();
$site 				= new Educriativa_Model_SitesWeb();
$faixa 				= new Educriativa_Model_FaixaEtaria();

$estados = $organizacao->listarEstados();
$municipios = $organizacao->estuf ? $organizacao->listarMunicipios($organizacao->estuf) : array();

if ($questionario->quesituacao == 'F') {
	ob_clean();
	header("Location: login.php?finalizado=1");
	die;
}

switch ($_REQUEST['action']) 
{
	case 'salvarResponsavel':
		$particiante->atualizar();
		die;
	break;
	case 'salvarOrganizacao':
		$organizacao->atualizar();
		die;
	break;
	case 'buscarOrganizacao':
		include_once APPRAIZ . "www/includes/webservice/pj.php";
		$organizacao->carregarPessoaJuridica($_REQUEST['orgcnpj']);
		die;
	break;
	case 'salvarQuestionario':
		$questionario->finalizar();
		die;
		break;
	case 'salvarResposta':
		$resposta->salvarResposta();
		die;
		break;
	case 'buscarEndereco':
		$particiante->carregarEndereco($_REQUEST['orgcep']);
		die;
	break;
	case 'detalharVideo':
		$organizacao->detalharVideo($_REQUEST['orglinkvideo']);
		die;
	case 'carregarMunicipio':
		$organizacao->carregarMunicipios($_REQUEST['estuf']);
		die;
	break;
	case 'carregarTipo':
        $organizacaoTipo->montarOptions($_REQUEST['gruid']);
		die;
	break;
}
?>

<!DOCTYPE html>
<html lang="pt-BR">
<?php require "header.php"; ?>
<body class="menubar-hoverable header-fixed ">

	<!-- BEGIN HEADER-->
	<!-- barra do governo -->
	<div id="barra-brasil">
	  <a href="http://brasil.gov.br" class="barraGoverno">Portal do Governo Brasileiro</a>
	</div>
	<!-- fim barra do governo -->

	<div class="container">
	  <div id="topo">
	    <div class="row">
	      <div class="col-md-3 col-sm-6 col-xs-6 logo">
	      	<img src="img/logo.png" alt="">
	       </div>
	    </div> <!--  row -->
	  </div> <!--  topo -->
	</div> <!--  container -->
	<!-- END HEADER-->

	<!-- BEGIN BASE-->
	<div id="base">
	
		<!-- BEGIN CONTENT-->
		<div id="content" class="section-body contain-lg shadow">
			<section>
				<!-- BEGIN INTRO -->
				<div class="row">
					<div class="col-lg-12 col-sm-12 col-xs-12 text-left">
						<h2>Olá, <small><?php echo $particiante->parnome; ?></small>
							<a href="login.php" id="btn-sair" class="btn btn-danger pull-right btn-sair sair">
								<span class="fa fa-power-off"></span> Sair
							</a>
						</h2>
						<hr/>
					</div>
				</div><!--end .row -->
				<!-- END INTRO -->

				<!-- BEGIN VALIDATION FORM WIZARD -->
				<div class="row">
					<div class="col-lg-12">
						<div class="card">
							<div class="card-head style-primary">
								<header>INOVAÇÃO E CRIATIVIDADE NA EDUCAÇÃO BÁSICA</header>
							</div>
							<div class="card-body">
								<div id="rootwizard2" class="form-wizard form-wizard-horizontal">
									<form id="form" class="form floating-label form-validation" role="form" novalidate="novalidate" method="post">
										<div class="form-wizard-nav">
											<div class="progress"><div class="progress-bar progress-bar-primary"></div></div>
											<ul class="nav nav-justified">
												<li class="active">
													<a href="#step1" class="save_step" data-toggle="tab" data-action="salvarResponsavel">
														<span class="step " data-action="salvarResponsavel">1</span>
														<span class="title"><i class="fa fa-user"></i> RESPONSÁVEL INSCRIÇÃO</span>
													</a>
												</li>
												<li>
													<a href="#step2" class="save_step" data-toggle="tab" data-action="salvarOrganizacao">
														<span class="step" data-action="salvarOrganizacao">2</span>
														<span class="title"><i class="fa fa-building-o"></i> DADOS DA ORGANIZAÇÃO</span>
													</a>
												</li>
												<li>
													<a href="#step3" class="save_step" data-toggle="tab" data-action="salvarOrganizacao">
														<span class="step" data-action="salvarQuestionario">3</span>
														<span class="title"><i class="fa fa-check-square-o"></i> QUESTIONÁRIO ESTRATÉGICO</span>
													</a>
												</li>
											</ul>
										</div><!--end .form-wizard-nav -->

										<div class="tab-content clearfix">
											<div class="tab-pane active" id="step1">
												<?php require_once 'form/responsavel.php'; ?>
											</div><!--end #step1 -->
											<div class="tab-pane" id="step2">
												<?php require_once 'form/organizacao.php'; ?>
											</div><!--end #step2 -->
											<div class="tab-pane" id="step3">
												<?php require_once 'form/questionario.php'; ?>
											</div><!--end #step3 -->
										</div><!--end .tab-content -->
										<ul class="pager wizard">
											<li class="previous pull-left"><button type="button" class="btn-raised btn ink-reaction btn-primary prev">Anterior</button></li>
											<li class="next pull-right" id="btn-proximo"><button class="btn-raised btn ink-reaction btn-primary save">Próximo</button></li>
											<li class="pull-right" id="btn-finalizar" style="display: none;" ><button type="button" class="btn btn-danger ink-reaction finalizar">Finalizar</button></li>
										</ul>
									</form>
								</div><!--end #rootwizard -->
							</div><!--end .card-body -->
						</div><!--end .card -->
					</div><!--end .col -->
				</div><!--end .row -->
				<!-- END VALIDATION FORM WIZARD -->
			</section>
		</div><!--end #content-->
		<!-- END CONTENT -->

	</div><!--end #base-->
	<!-- END BASE -->

    <!-- Modal -->
    <div class="modal fade" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
        <div class="modal-dialog" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h4 class="modal-title" id="myModalLabel">Atenção!</h4>
                </div>
                <div class="modal-body">
                    As três páginas do formulário devem ser preenchidas. <br />
                    Ao final, clique em <span style="color: red;">FINALIZAR</span> para sua inscrição ser analisada.
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">Entendi</button>
                </div>
            </div>
        </div>
    </div>

	<?php require_once "footer.php"; ?>

</body>
