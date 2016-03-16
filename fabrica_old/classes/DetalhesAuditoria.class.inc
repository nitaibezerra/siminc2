<?php 
class DetalhesAuditoria {
	
	private $id;
	private $dataAuditoria;
	private $resultado;
	private $motivo;
	private $observacao;
	private $servicoFaseProduto;
	private $auditoria;
	
	public function setId($id){
		$this->id = $id;
	}
	
	public function getId(){
		return $this->id;
	}
	
	public function setDataAuditoria($dataAuditoria){
		$this->dataAuditoria = $dataAuditoria;
	}
	
	public function getDataAuditoria(){
		return $this->dataAuditoria;
	}
	
	public function setResultado($resultado){
		$this->resultado = $resultado;
	}
	
	public function getResultado(){
		return $this->resultado;
	}
	
	public function setMotivo($motivo){
		$this->motivo = $motivo;
	}
	
	public function getMotivo(){
		return $this->motivo;
	}
	
	public function setObservacao($observacao){
		$this->observacao = $observacao;
	}
	
	public function getObservacao(){
		return $this->observacao;
	}
	
	public function setServicoFaseProduto(ServicoFaseProduto $servicoFaseProduto){
		$this->servicoFaseProduto = $servicoFaseProduto;
	}
	
	public function getServicoFaseProduto(){
		return $this->servicoFaseProduto;
	}
	
	public function setAuditoria(Auditoria $auditoria){
		$this->auditoria = $auditoria;
	}
	
	public function getAuditoria(){
		return $this->auditoria;
	}
	
	public function possuiAuditoria(){
		if ($this->getResultado() != null || $this->getResultado() != '') {
			return true;
		} else {
			return false;
		}
	}
	
	public function auditoriaAceitavel(){
			if ($this->getResultado() == 1 || $this->getResultado() == 2) {
			return true;
		} else {
			return false;
		}
	}
	
	public function auditoriaInaceitavel(){
			if ($this->getResultado() == 3) {
			return true;
		} else {
			return false;
		}
	}

}