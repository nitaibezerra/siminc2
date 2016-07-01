<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */

//Declaração de Objetos
$modelo = new Spo_Service_Periodo();

/**
 * Cabecalho padrão do SIMEC.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
$tipo = $_GET['tipo'];
switch($tipo){
    case 'acao':
        $arquivo = 'periodoAcao.inc';
        $pagina = 'Período de Acompanhamento de Ações';
        break;
    case 'subacao':
        $arquivo = 'periodoSubacao.inc';
        $pagina = 'Período de Acompanhamento de Subações';
        break;
    case 'relatorio':
        $arquivo = 'periodoRelatorioTCU.inc';
        $pagina = 'Período de Acompanhamento de Relatório do TCU';
        break;
    case 'receita':
        $arquivo = 'periodoReceitaOrc.inc';
        $pagina = 'Período de Alteração de Receita Orçamentária';
        break;
    case 'alteracao':
        $arquivo = 'periodoAlteracaoCred.inc';
        $pagina = 'Momento de Alterações de Crédito';
        break;
    case 'ploa':
        $arquivo = 'periodoProposta.inc';
        $pagina = 'Período de Preechimento da Proposta (PLOA '.(((int)$_SESSION['exercicio'])+1).')';
        break;

}
?>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="">Período</li>
        <li class=""><?=$pagina?></li>
    </ol>
<?php
require(dirname(__FILE__) . "/views/". $arquivo);
?>
</section>