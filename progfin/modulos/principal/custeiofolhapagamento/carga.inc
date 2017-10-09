<?php
/**
 * Arquivo de carga de dados da programação financeira.
 * $Id: carga.inc 102309 2015-09-10 14:29:34Z maykelbraz $
 */
/**
 * Funções de apoio para a liberação financeira.
 * @see _funcoescusteiofolhapagamento.php
 */
require(APPRAIZ . "www/progfin/_funcoescusteiofolhapagamento.php");


$fm = new Simec_Helper_FlashMessage('progfin/custeiofolhapagamento');
//Gerar excel
if($_POST['reqexcel'] === 'excel' && (!empty($_POST['mes']) ) ){

	if(!empty($_POST['mes'])){
		$mes       = "AND cfp.cfpmes::integer = ".$_POST['mes'];
	}
		$exercicio = $_SESSION['exercicio'];
		global $db;
		$sql = "SELECT
                            cfp.cfpano,
                            CASE cfp.cfpmes
                                WHEN '01' THEN 'janeiro'
                                WHEN '02' THEN 'fevereiro'
                                WHEN '03' THEN 'março'
                                WHEN '04' THEN 'abril'
                                WHEN '05' THEN 'maio'
                                WHEN '06' THEN 'junho'
                                WHEN '07' THEN 'julho'
                                WHEN '08' THEN 'agosto'
                                WHEN '09' THEN 'setembro'
                                WHEN '10' THEN 'outubro'
                                WHEN '11' THEN 'novembro'
                                WHEN '12' THEN 'dezembro'
                            END,
                            cfp.cfptipobeneficio,
                            cfp.cfptipofolha,
                            cfp.cfporgao,
                            cfp.cfpupag,
		                    cfp.cfpsgregime,
                            cfp.cfpsituacao,
                            cfp.cfpctrl,
                            cfp.cfpcodigodespesa,
                            cfp.cfprubrica,
                            cfp.cfpvalor
		        FROM
		            progfin.cargacusteiofolhapagamento cfp
		        WHERE
                            cfpano::integer = ".$exercicio.$mes."
		        ORDER BY
		            cfp.cfpano, cfp.cfpmes, cfp.cfptipobeneficio, cfp.cfptipofolha, cfp.cfporgao, cfp.cfpupag,
		            cfp.cfpsgregime, cfp.cfpsituacao, cfp.cfpctrl, cfp.cfpcodigodespesa, cfp.cfprubrica, cfp.cfpvalor";

		$cabecalho = array('Ano','Mês', 'Tipo Beneficio', 'Tipo Folha', 'Orgão', 'Unidade Pagadora', 'Regime', 'Situação', 'Controle', 'Elemento Despesa', 'Rubrica', 'Valor');
		ini_set("memory_limit", "1024M");
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header("Content-type: application/vnd.ms-excel");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatExcel".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,1000000000,5,'N','100%', 'S');
		exit;
		die();
}

if (isset($_POST['requisicao']) || !empty($_POST['requisicao'])) {



	$_POST['reqexcel'] = '';
		$requisicao = $_POST['requisicao'];
		    switch ($requisicao) {
		        case 'processarArquivo':
		            $resultado = processarArquivoCusteioFolha($_FILES['custeiofolhapagamento'], $_SESSION['usucpf'], $_SESSION['exercicio']);
		            break;
		        case 'processarDados':
		            $resultado = processarDadosCusteioFolha($_SESSION['usucpf'], $_SESSION['exercicio']);
		            break;
		    }
    $fm->addMensagem(
            $resultado['msg'], $resultado['sucesso'] ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO
    );
}
/**
 * Cabecalho padrão do Sistema.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="progfin.php?modulo=inicio&acao=C">Programação Financeira</a></li>
        <li>Extração de Custeio da Folha de Pagamento</li>
        <li class="active">Upload do Arquivo de Folha de Pagamento</li>
    </ol>
    <div class="row col-md-12">
        <?php
        $arquivo = '';
// -- Exibindo mensagens do sistema
        echo $fm->getMensagens();
        require(dirname(__FILE__) . "/carga/formCarga.inc");
        ?>
    </div>
</div>