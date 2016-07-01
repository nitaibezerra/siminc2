<?php
class HistoricoItemAuditoria {
	
	private $id;
	private $historicoAuditoria;
	private $itemAuditoria;
	
	public function setId( $id ){
		$this->id = $id;
	}
	
	public function getId(){
		return $this->id;
	}
	
	public function setHistoricoAuditoria( HistoricoAuditoria $historicoAuditoria ){
		$this->historicoAuditoria = $auditoria;
	}
	
	public function getHistoricoAuditoria(){
		return $this->historicoAuditoria;
	}
	
	public function setItemAuditoria( ItemAuditoria $itemAuditoria ){
		$this->itemAuditoria = $itemAuditoria;
	}
	
	public function getItemAuditoria(){
		return $this->itemAuditoria;
	}
	
}