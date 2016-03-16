<?php 
class UnidadeMedidaControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	} 
	
	public function carregaUnidadeMedida()
	{
		$undtipo = ($_SESSION['itrid'] == 1) ? 'E' : 'M' ;
		$obUnidadeMedida = new UnidadeMedida();
		$arUnidadeMedida = $obUnidadeMedida->carregaUnidadeMedidaPorUndtipo();
		return $arUnidadeMedida;
	}
	
}