<?php 
class CronogramaControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	} 
	
	public function carregaCronogramaPorSbaid($sbaid, $anoref)
	{
		$obCronograma = new Cronograma();
		return $obCronograma->carregaCronogramaPorSbaid($sbaid, $anoref);
	}
	
}