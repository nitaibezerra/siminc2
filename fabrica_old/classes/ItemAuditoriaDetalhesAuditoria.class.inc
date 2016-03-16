<?php
class ItemAuditoriaDetalhesAuditoria {
	
	private $id;
	private $itemAuditoria;
	private $detalhesAuditoria;
	
	public function setId( $id ){
		$this->id = $id;
	}
	
	public function getId(){
		return $this->id;
	}
	
	public function setItemAuditoria( ItemAuditoria $itemAuditoria ){
		$this->itemAuditoria = $itemAuditoria;
	}
	
	public function getItemAuditoria(){
		return $this->itemAuditoria;
	}
	
	public function setDetalhesAuditoria( DetalhesAuditoria $detalhesAuditoria ){
		$this->detalhesAuditoria = $detalhesAuditoria;
	}
	
}