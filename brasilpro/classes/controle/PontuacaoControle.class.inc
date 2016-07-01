<?php 
class PontuacaoControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	} 
	
	public function carregaPontuacao($indid)
	{
		$obPontuacao = new Pontuacao();
		$arPontuacao = $obPontuacao->recuperaCriteriosPotuacao($indid);
		return $arPontuacao;		
	}
	
	public function recuperaPontuacaoPorIndid($indid)
	{
		$oPontuacao = new Pontuacao();
		return $oPontuacao->recuperaPontuacaoPorIndid($indid);
	}
	
	public function recuperarPropostaAcoesPorCriterio($crtid)
	{
		$oPropostaAcao = new PropostaAcao();
		return $oPropostaAcao->recuperarPropostaAcoesPorCriterio($crtid);
	}
	
	public function recuperarSubacoesPorCriterio($crtid)
	{
		$oPropostaSubacao = new PropostaSubacao();
		return $oPropostaSubacao->recuperarSubacoesPorCriterio($crtid);
	}
	
	public function recuperaProximoAnteriorPorIndid($indid)
	{
		$oPontuacao = new Pontuacao();
		return $oPontuacao->recuperaProximoAnteriorPorIndid($indid);
	}
	
}