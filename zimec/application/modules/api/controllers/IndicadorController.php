<?php
import('api.business.IndicadorBusiness');

class Api_IndicadorController extends Simec_Controller_Rest
{
	private $businessIndicador;
	
	public function init() 
	{
//		$this->businessIndicador = new Api_Business_IndicadorBusiness();
	}
	
	/**
	 * @Rest(uri="/api/indicador/demandas", method="GET")
	 */
	public function demandasAction()
	{
		$demandas = $this->businessIndicador->listaDemandas();
	
		$this->_encode($demandas);
	}
	
	/**
	 * @Rest(uri="/api/indicador/tempo", method="GET")
	 */
	public function tempoAction() 
	{
//		$tempo = $this->businessIndicador->totalTempoExcucao();
		
		$this->_encode(array('callback' => 'ok'));
	}
	
	/**
	 * @Rest(uri="/api/indicador/quantidade", method="GET")
	 */
	public function quantidadeAction() 
	{
		$querys = $this->businessIndicador->totalQueryExcucao();
		
		$this->_encode($querys);
	}
	
	/**
	 * @Rest(uri="/api/indicador/usuarios", method="GET")
	 */
	public function usuariosAction() 
	{
		$online = $this->businessIndicador->totalUsuariosOnline();
		
		$this->_encode($online);
	}
}