<?php
class HistoricoItemAuditoriaRepositorio extends Modelo {
	
	private $itemAuditoriaRepositorio;
	
	public function __construct(){
		parent::__construct();
		$this->itemAuditoriaRepositorio = new ItemAuditoriaRepositorio();
	}
	
	public function recuperePorIdHistoricoAuditoria( $idHistoricoAuditoria ){
		$sql = "SELECT * FROM fabrica.historicoitemauditoria where haid = $idHistoricoAuditoria";
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	private function montaObjeto( $linha ){
		$historioAuditoria = new HistoricoAuditoria();
		$historioAuditoria->setId($linha['haid']);
		
		$itemAuditoria = $this->itemAuditoriaRepositorio->recuperePorId($linha['itemid']);
		
		$historicoItemAuditoria = new HistoricoItemAuditoria();
		$historicoItemAuditoria->setId($linha['hiaid']);
		$historicoItemAuditoria->setHistoricoAuditoria($historicoAuditoria);
		$historicoItemAuditoria->setItemAuditoria($itemAuditoria);
		return $historicoItemAuditoria;
	}
}