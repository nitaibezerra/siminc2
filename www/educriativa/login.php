<?php 
include "config.inc";
include_once APPRAIZ . "educriativa/autoload.php";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

$configuracao = new Educriativa_Model_Configuracao();

$sql = "select to_char(dataexpiracao, 'YYYY-MM-DD') data_termino,
               to_char(dataexpiracao, 'YYYYMMDDHH24MISS') data_expiracao,
               to_char(dataexpiracao, 'DD/MM/YYYY \à\s HH24:MI:SS') as data_formatada
          from criatividadeeducacao.configuracao";

$dados = $configuracao->pegaLinha($sql);

$dataTermino = $dados['data_termino'];
$dataExpiracao = $dados['data_expiracao'];
$dataFormatada = $dados['data_formatada'];
$dias = floor((strtotime($dataTermino) - strtotime(date('Y-m-d'))) / ((60*60*24)+1));
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
				<!-- BEGIN VALIDATION FORM WIZARD -->
				<div class="row">
					<?php if ($_GET['sucesso']): ?>
		                <div class="col-lg-12 col-sm-12 col-xs-12">
		                    <div class="alert alert-success" role="alert">
		                        <p>Sua inscrição na Chamada Pública Inovação e Criatividade na Educação Básica foi finalizada corretamente.</p>
		                        <p><strong>Obrigado por sua participação.</strong></p>
		                    </div>
		                </div>
		            <?php endif; ?>
		
		            <?php if ($_GET['finalizado']): ?>
		                <div class="col-lg-12 col-sm-12 col-xs-12">
		                    <div class="alert alert-warning" role="alert">
		                        <p>Este formulário já foi finalizado!</p>
		                        <p>Agradeçemos sua participação na pesquisa.</p>
		                    </div>
		                </div>
		            <?php endif; ?>
					
					<?php if(date('YmdHis') >= $dataExpiracao) : ?>
		                <div class="col-lg-12 col-sm-12 col-xs-12">
		                    <div class="alert alert-success" role="alert">
		                        <p>Envio do questionario encerrado em <?php echo $dataFormatada; ?>.</p>
		                        <p>Obrigado pela colaboração!</p>
		                    </div>
		                </div>
		            <?php else : ?>
					<div class="col-sm-6" style="text-align: justify;">
						<div class="panel-group" id="accordion3">
							<div class="card panel expanded">
								<div class="card-head card-head-sm collapsed link" data-toggle="collapse" data-parent="#accordion3" data-target="#accordion3-1" aria-expanded="true">
									<header>O que é o projeto Inovação e Criatividade na Educação Básica?</header>
									<div class="tools">
										<a class="btn btn-icon-toggle"><i class="fa fa-angle-down"></i></a>
									</div>
								</div>
								<div id="accordion3-1" class="collapse in">
									<div class="card-body">						
										<p>O século 21 já inicia com uma revolução na comunicação, que traz mudanças no mundo da educação e do trabalho. As novas tecnologias facilitam o autoaprendizado, a formação de comunidades de aprendizagem e de redes e a produção de conhecimento em diversos suportes.  Também no mundo do trabalho as relações são mais fluidas, menos regulamentadas, carreiras e caminhos profissionais surgem de um dia para o outro e a duração da vida ativa se prolonga para além dos convencionais 60, 65 anos. À medida que ocorrem as transformações, aumenta o compromisso ético com as gerações do futuro, exigindo prudência e criatividade para encontrar novas formas sustentáveis de lidar com os recursos ambientais. </p>
										<p>Diante deste quadro, o projeto visa criar as bases para uma política pública de fomento à inovação e à criatividade na educação básica, estimulando as escolas, instituições e organizações que ousaram romper com os padrões educacionais tradicionais para criar uma nova escola que forme cidadãos integrais, felizes, produtores de conhecimento e cultura e que se relacionem com o planeta de modo responsável, sustentável e respeitoso. Pretende fortalecer as experiências inovadoras para que elas superem o isolamento, a fragmentação, a descontinuidade no tempo e a dependência de voluntarismo; levantar referências de inovação para uma efetiva mudança da educação básica e apoiar os processos que ampliam o impacto das experiências inovadoras relevantes para além de seu polo inicial.</p>
									</div>
								</div>
							</div>
							<div class="card panel">
								<div class="card-head card-head-sm collapsed link" data-toggle="collapse" data-parent="#accordion3" data-target="#accordion3-2" aria-expanded="false">
									<header>Quais as características de uma organização inovadora e criativa?</header>
									<div class="tools">
										<a class="btn btn-icon-toggle"><i class="fa fa-angle-down"></i></a>
									</div>
								</div>
								<div id="accordion3-2" class="collapse">
									<div class="card-body">						
				                        <p>1.	GESTÃO: Corresponsabilização na construção e gestão do projeto político pedagógico: Estruturação do trabalho da equipe, da organização do espaço, do tempo e do percurso do estudante com base em um sentido compartilhado de educação, que orienta a cultura institucional e os processos de aprendizagem e de tomada de decisão, garantindo-se que os critérios de natureza pedagógica sejam sempre preponderantes.</p>
				                        <p>2.	CURRÍCULO: Desenvolvimento integral: Estruturação de um currículo voltado para a formação integral, que reconhece a multidimensionalidade da experiência humana - afetiva, ética, social, cultural e intelectual.</p>
				                        <p>Produção de conhecimento e cultura: Estratégias voltadas para tornar a instituição educativa espaço de produção de conhecimento e cultura, que conecta os interesses dos estudantes, os saberes comunitários e os conhecimentos acadêmicos para transformar o contexto socioambiental.</p>
				                        <p>Sustentabilidade (social, econômica, ecológica e cultural): Estratégias pedagógicas que levem a uma nova forma de relação do ser humano com o contexto planetário.</p>
				                        <p>3.	AMBIENTE: Ambiente físico que manifeste a intenção de educação humanizada, potencializadora da criatividade, com os recursos disponíveis para exploração e a convivência enriquecedora nas diferença. Estratégias que estimulam o diálogo entre os diversos segmentos da comunidade, a mediação de conflitos por pares, o bem-estar de todos, a valorização da diversidade e das diferenças e a promoção da equidade.</p>
				                        <p>4.	MÉTODOS: Protagonismo: Estratégias pedagógicas que reconhecem o estudante como protagonista de sua própria aprendizagem; que reconhecem e permitem ao estudante expressar sua singularidade e desenvolver projetos de seu interesse que impactem a comunidade e que contribuam para a sua futura formação profissional.</p>
				                        <p>5.	ARTICULAÇÃO COM OUTROS AGENTES: Rede de direitos: Estratégias intersetoriais e em rede, envolvendo a comunidade, para a garantia dos direitos fundamentais dos estudantes, reconhecendo-se que o direito à educação é indissociável dos demais.</p>
									</div>
								</div>
							</div>
							<div class="card panel">
								<div class="card-head card-head-sm collapsed link" data-toggle="collapse" data-parent="#accordion3" data-target="#accordion3-3" aria-expanded="false">
									<header>Qual o objetivo da chamada pública?</header>
									<div class="tools">
										<a class="btn btn-icon-toggle"><i class="fa fa-angle-down"></i></a>
									</div>
								</div>
								<div id="accordion3-3" class="collapse">
									<div class="card-body">						
			                        	<p>O objetivo da chamada pública é mapear e caracterizar as intervenções inovadoras que ocorrem em nível local, por iniciativa de escolas, comunidades ou outras organizações educativas. As organizações que apresentarem as características de inovação e criatividade e aquelas que demonstrarem estratégias claras para desenvolver estas características serão reconhecidas e divulgadas pelo MEC e, em fase posterior, poderão se inscrever em programas voltados para o seu fortalecimento.</p>
									</div>
								</div>
							</div>
							<div class="card panel">
								<div class="card-head card-head-sm collapsed link" data-toggle="collapse" data-parent="#accordion3" data-target="#accordion3-4" aria-expanded="false">
									<header>Quem pode participar?</header>
									<div class="tools">
										<a class="btn btn-icon-toggle"><i class="fa fa-angle-down"></i></a>
									</div>
								</div>
								<div id="accordion3-4" class="collapse">
									<div class="card-body">
				                        <ol type="I">
                                            <li>Escolas públicas de educação básica (educação infantil, ensino fundamental, ensino médio, ensino técnico e EJA) das redes públicas federal, estaduais/distrital e municipais.</li>
											<li>Escolas privadas de educação básica (educação infantil, ensino fundamental, ensino médio e/ou ensino médio integrado e EJA).</li>
											<li>Associações, organizações sociais (OS) e organizações da sociedade civil que atuam no campo da educação com crianças, adolescentes e/ou jovens.</li>
											<li>Instituições educacionais comunitárias, filantrópicas e confessionais que atuam com crianças, adolescentes e/ou jovens.</li>
				                        </ol>
									</div>
								</div>
							</div>
						</div>
                    </div>
					<div class="col-sm-6">
						<br>
						<div class="card">
							<div class="card-head card-head-sm style-primary">
								<header><i class="fa fa-user"></i> LOGIN</header>
							</div>
							<div class="card-body">
								<form action="controlador.php" class="form floating-label form-validation" role="form" novalidate="novalidate" accept-charset="utf-8" method="post">
									<div class="col-sm-12">
										<div class="form-group">
											<input type="text" required class="form-control" data-inputmask="'mask': '999.999.999-99', 'showMaskOnHover': false" id="usucpf" name="usucpf">
											<label for="usucpf">CPF do participante</label>
											<?php echo ( (isset($_GET['cpf']) and $_GET['cpf'] == 1) ? ' <p class="help-block has-error login-error">Campo Obrigatório</p>' : ''); ?>
		                            		<?php echo ( (isset($_GET['cpf']) and $_GET['cpf'] == 2) ? ' <p class="help-block has-error login-error">CPF Incorreto!</p>' : ''); ?>
										</div>
									</div>
									<div class="col-sm-9">
										<div class="form-group <?php echo ( isset($_GET['captcha']) ? ' has-error' : ''); ?>">
			                            	<input type="text" required class="form-control" name="captcha" id="captcha" maxlength="4">
			                            	<label for="captcha">Digite os caracteres da imagem ao lado</label>
			                            	<?php echo ( (isset($_GET['captcha']) and $_GET['captcha'] == 1) ? ' <p class="help-block has-error login-error">Campo Obrigatório</p>' : ''); ?>
				                            <?php echo ( (isset($_GET['captcha']) and $_GET['captcha'] == 2) ? ' <p class="help-block has-error login-error">Caracteres Incorretos - digite novamente!</p>' : ''); ?>
										</div>
									</div>
									<div class="col-sm-3" style="padding: 0px">
										<img src="captcha.php" width="113" height="49">
									</div>
									<br>
									<div class="row">
										<div class="col-xs-12 text-center">
											<button class="btn btn-primary btn-raised" type="submit">Entrar</button>
										</div><!--end .col -->
									</div><!--end .row -->
								</form>
							</div>
							<div class="card-footer text-center">
								<h4 class="text-default-light">Navegadores compativeis</h4>
								<img src="img/navegadores.png" />
							</div>
						</div>
					</div>
					<?php endif; ?>
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

                    <?php if ($_GET['sucesso']){ ?>
                        <p>Sua inscrição na Chamada Pública Inovação e Criatividade na Educação Básica foi finalizada corretamente.</p>
                        <p><strong>Obrigado por sua participação.</strong></p>
                    <?php } else {
                        echo 'Leia as informações ao lado para saber se você pode se inscrever.';
                    }?>

                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">OK</button>
                </div>
            </div>
        </div>
    </div>

	<?php require_once "footer.php"; ?>

</body>
