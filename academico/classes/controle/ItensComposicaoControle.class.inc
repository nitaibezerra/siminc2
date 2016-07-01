<?php 
class ItensComposicaoControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	} 
	
	public function carregaItensComposicaoPorSbaid($sbaid, $anoref)
	{
		//$obItensComposicao = new ItensComposicaoSubacao();
		$obItensComposicao = new SubacaoItensComposicao();
		//SubacaoItensComposicao
		return $obItensComposicao->carregaItensComposicaoPorSbaid($sbaid, $anoref);
	}
	
	public function visualizacaoItensComposicao($arParametros, $anoref)
	{
		if($arParametros['visao'] == 'cadItensComposicao'){
			$oDadosUnidade = new UnidadeMedidaControle();
			$this->visao->arUnidadeMedida = $oDadosUnidade->carregaUnidadeMedida();			
			$this->visao->arItensComposicao = $this->carregaItensComposicaoPorSbaid($arParametros['sbaid'], $anoref);
		} else {
			$obItensComposicao = new SubacaoItensComposicao();
			$this->visao->descricaoItem = $obItensComposicao->recuperaDescricaoItem($arParametros['icoid']);
		}

		$this->visao->visualizar($arParametros['visao']);
	}
	
	public function visualizacaoItensComposicaoProInfancia($arParametros, $anoref)
	{		

		$obPreObra = new PreObra();
		if($_SESSION['par']['preid']){
			$stDescricaoObra = $obPreObra->recuperarDescricaoPreObraPorId($_SESSION['par']['preid']);
		}
		
		$this->visao->obraDescricao = $stDescricaoObra;
		$this->visao->preano = $anoref;	
		$this->visao->respSim = $arParametros['respSim'];	
		$this->visao->visualizar($arParametros['visao']);
	}
	
	public function visualizacaoItensComposicaoPronatec($arParametros)
	{
		$this->visao->visualizar($arParametros['visao']);
	}
}