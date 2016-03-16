<?php 
class FiscalServico {
	
	private $fiscalRepositorio;
	
	function __construct(){
		$this->fiscalRepositorio = new FiscalRepositorio();
	}
	
	function isFiscal($idFiscal){
		$fiscal = $this->fiscalRepositorio->recuperePorId($idFiscal);
		return $fiscal->getId() == null ? false : true;
	}
}

