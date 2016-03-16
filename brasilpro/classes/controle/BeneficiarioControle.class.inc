<?php 
class BeneficiarioControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	} 
	
	public function carregaBeneficiarioPorSbaid($sbaid, $anoref)
	{
		$obBeneficiario = new Beneficiario();
		return $obBeneficiario->carregaBeneficiarioPorSbaid($sbaid, $anoref);
	}
	
}