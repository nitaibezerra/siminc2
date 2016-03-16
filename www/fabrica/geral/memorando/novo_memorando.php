<?php 
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';

$memorando = new Memorando();
$memorando->setDataMemorando(DateTimeUtil::retiraMascaraRetornandoObjetoDateTime($_POST['dataMemorando']));
$memorando->setNumeroMemorando($_POST['numeroMemorando']);

$memovlrajuste = ( $_POST['memovlrajuste'] ? str_replace('.' , '', $_POST['memovlrajuste']) : null );
$memovlrajuste = ( $memovlrajuste ? str_replace(',' , '.', $memovlrajuste) : null );

$fiscalRepositorio 	= new FiscalRepositorio();
$fiscal 			= $fiscalRepositorio->recuperePorId($_POST['cpfServidorPublico']);

$memorando->setFiscal($fiscal);
$memorando->setPrestadorServico($_POST['empresaContratada']);
$memorando->setStatusMemorando(StatusMemorando::MEMORANDO_NAO_IMPRESSO);
$memorando->setTextoMemorando(utf8_decode($_POST['textoMemorando']));
$memorando->setGlosaMemorando($_POST['array']);
$memorando->setJustificativaGlosaMemorando(utf8_decode($_POST['justificativaGlosaMemorando']));
$memorando->setDescricaoAjuste( utf8_decode($_POST['memodscajuste']) );
$memorando->setValorAjuste( $memovlrajuste );

//$memorando->setTipoDespesaId( $_POST['formmemotpdpsid'] );


if( $_POST['empresaContratada'] == PrestadorServico::PRESTADORA_SERVICO_FABRICA ||
	$_POST['empresaContratada'] == PrestadorServico::PRESTADORA_SERVICO_POLITEC ||
	$_POST['empresaContratada'] == PrestadorServico::PRESTADORA_SERVICO_MBA )
{
    $memorando->setTipoDespesaId( $_POST['formmemotpdpsid'] );
}else {
    // seto o tipo como 'Capital' para emissão de memorando urgente
    //corrigir para receber do formulário
    $memorando->setTipoDespesaId( 2 );
}



$osSelecionadas = $_POST['osSelecionadas'];
$ordemServicoRepositorio = new OrdemServico();

foreach ($osSelecionadas as $osSelecionada){
	$ordemServico = new OrdemServico();
	$ordemServico = $ordemServicoRepositorio->recuperePorId($osSelecionada);
	$ordensServico[] = $ordemServico;
}
$memorando->setListaDeOrdensDeServico( $ordensServico );

$memorandoRepositorio = new MemorandoRepositorio();
$memoid = $memorandoRepositorio->salvar($memorando);
$retorno = simec_json_encode( array('memoid'=>$memoid, 'formmemotpdpsid'=>$_POST['formmemotpdpsid']) );
echo $retorno;