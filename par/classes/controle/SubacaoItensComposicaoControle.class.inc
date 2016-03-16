<?php 
class SubacaoitensComposicaoControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	} 
        
        public function listarSubacaoItensComposicaoPorSbdid($arrParam = array(), $booSQL = FALSE) {
            try {
                if (is_array($arrParam)) {
                    $obSubacaoItensComposicao = new SubacaoItensComposicao();
                    $mixSubacaoItensComposicao = $obSubacaoItensComposicao->listarSubacaoItensComposicaoPorSbdid($arrParam, $booSQL);
                    return $mixSubacaoItensComposicao;
                } else {
                    return false;
                }
            } catch (Exception $exception) {
                echo $exception->getTraceAsString();
            }
        }
	
}