<?php

$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$pdf = new Ted_Form_Pdf();

//Verifica se existe tcpid setado, senão, efetua redirecionamento
Ted_Utils_Model::capturaTcpid(true);

Ted_Utils_Model::userIsAllowed();

//Setando o TCPID da sessão no campo do formulário
$pdf->getElement('tcpid')->setValue(Ted_Utils_Model::capturaTcpid());

if (isset($_POST['requisicao']) && ($_POST['requisicao'] == 'gerarPDF')) {

    include_once APPRAIZ . 'elabrev/classes/modelo/HtmlToPdf.class.inc';

    $html = $pdf->showForm();
    $pdfObj = new HtmlToPdf($html);
    $pdfObj->setTitle("Termo_De_Execucao_Descentralizada_n_{$pdf->getElement('tcpid')->getValue()}.pdf");
    $pdfObj->getPDF();
}

//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
?>
<section class="col-md-12">
	<ol class="breadcrumb">
		<li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
		<li class="active">Concedente</li>
	</ol>

    <nav class="col-md-2">
        <?php echo $gerenciadorAba->apresentaAbas(); ?>
    </nav>

    <div class="col-md-10">
        <div class="col-md-2"></div>
        <div class="col-md-8">
            <?php echo Ted_Utils_Model::montaInformacaoTermo();?>
        </div>
        <div class="col-md-2"></div>
    </div>

	<section class="col-md-10">
		<section class="well">
			<h4 class="text-center">Gerar PDF</h4>
		</section>
		<?php echo $pdf->showForm(); ?>
	</section>
</section>