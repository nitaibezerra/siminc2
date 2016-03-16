<?php 
class EstadoControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	} 
	
	public function carregarUf($booSQL = FALSE)
	{
		$obEstado = new Estado();
		$arrEstado = $obEstado->carregarUf($booSQL);
		return $arrEstado;
	}
}