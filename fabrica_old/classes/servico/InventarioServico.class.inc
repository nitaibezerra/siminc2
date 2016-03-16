<?php
class InventarioServico {
	
	private $inventarioRepositorio;
	
	
	public function __contruct(){
		$this->inventarioRepositorio = new InventarioRepositorio();
	}
	
	
}