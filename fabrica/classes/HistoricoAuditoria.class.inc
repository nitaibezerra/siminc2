<?php
class HistoricoAuditoria {
	
	private $id;
	private $auditoria;
	private $fiscal;
	private $dataAuditoria;
	private $motivo;
	private $observacao;
	private $situacaoAuditoria;
	
	public function setId( $id ){
		$this->id = $id;
	} 
	
	public function getId(){
		return $this->id;
	}
	
	public function setAuditoria( Auditoria $auditoria ){
		$this->auditoria = $auditoria;
	}
	
	public function getAuditoria(){
		return $this->auditoria;
	}
	
	public function setFiscal( Fiscal $fiscal ){
		$this->fiscal = $fiscal;
	}
	
	public function getFiscal(){
		return $this->fiscal;
	}
	
	public function setDataAuditoria( DateTime $dataAuditoria ){
		$this->dataAuditoria = $dataAuditoria;
	}
	
	public function getDataAuditoria(){
		return $this->dataAuditoria;
	}
	
	public function getDataAuditoriaFormatada($formato){
		return $this->dataAuditoria->format($formato);
	}
	
	public function setMotivo( $motivo ){
		$this->motivo = $motivo;
	}
	
	public function getMotivo(){
		return $this->motivo;
	}
	
	public function setObservacao( $observacao ){
		$this->observacao = $observacao;
	}
	
	public function getObservacao(){
		return $this->observacao;
	}
	
	public function setSituacaoAuditoria(SituacaoAuditoria $situacaoAuditoria){
		$this->situacaoAuditoria = $situacaoAuditoria;
	}
	
	public function getSituacaoAuditoria(){
		return $this->situacaoAuditoria;
	}
}