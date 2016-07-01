<?php 
class PropostaSubacaoControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	} 
	
	public function carregaPropostaSubacaoPorSbaid($sbaid)
	{
		$obPropostaSubacao = new PropostaSubacao();
		return $obPropostaSubacao->carregaPropostaSubacaoPorSbaid($sbaid);
	}
	
}