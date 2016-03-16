<?php 
class PropostaAcaoControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	} 
	
	public function carregaPontuacao($crtid)
	{
		$obPropostaAcao = new PropostaAcao();
		$arPropostaAcao = $obPropostaAcao->recuperarPropostaAcoesPorCriterio($crtid);
		return $arPontuacao;		
	}
	
}