<?php
//Cabeçalho do sistema
include  APPRAIZ."includes/cabecalho.inc";

//Verifica se existe tcpid setado, senão, efetua redirecionamento
Ted_Utils_Model::capturaTcpid(true);

//Declaração de Objetos
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$previsao = new Ted_Form_Previsao();
$cumprimento = new Ted_Form_Cumprimento();

//Setando o tcpid da sessão no formulário
$previsao->getElement('tcpid')->setValue(Ted_Utils_Model::capturaTcpid());

?>
<section class="col-md-12">
	<ol class="breadcrumb">
		<li><a href="ted.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
		<li class="">Cadastro de Termo de Cooperação</li>
		<li class="active">Cumprimento</li>
	</ol>
	<?php $gerenciadorAba->capturaAbaAtiva();?>
	<nav class="col-md-2">
		<?php echo $gerenciadorAba->apresentaAbas(); ?>
		<?php echo Ted_Utils_Model::montaInformacaoTermo();?>		
	</nav>	
	<section class="col-md-10">
		<section class="well">
			<h4 class="text-center">Relatório de Cumprimento</h4>
		</section>	
		<br>			
		<section>
			<?php echo $cumprimento->showForm(); ?>
		</section>
	</section>			
</section>