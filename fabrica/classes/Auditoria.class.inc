<?php 
class Auditoria {

	private $id;
	private $analiseSolicitacao;
	private $fiscal;
	private $nomeResponsavelFabrica;
	private $situacaoAuditoria;
	private $idDocumento;
	
	public function setId($id){
		$this->id = $id;
	}
	
	public function getId(){
		return $this->id;
	}
	
	public function setAnaliseSolicitacao(AnaliseSolicitacao $analiseSolicitacao){
		$this->analiseSolicitacao = $analiseSolicitacao;
	}
	
	public function getAnaliseSolicitacao(){
		return $this->analiseSolicitacao;
	}
	
	public function setFiscal(Fiscal $fiscal){
		$this->fiscal = $fiscal;
	}
	
	public function getFiscal(){
		return $this->fiscal;
	}
	
	public function setNomeResponsavelFabrica($nomeResponsavelFabrica){
		$this->nomeResponsavelFabrica = $nomeResponsavelFabrica; 
	}
	
	public function getNomeResponsavelFabrica(){
		return $this->nomeResponsavelFabrica;
	}
	
	public function setSituacaoAuditoria(SituacaoAuditoria $situacaoAuditoria){
		$this->situacaoAuditoria = $situacaoAuditoria;
	}
	
	public function getSituacaoAuditoria(){
		return $this->situacaoAuditoria;
	}
	
	public function setIdDocumento( $idDocumento ){
		$this->idDocumento = $idDocumento;
	}
	
	public function getIdDocumento(){
		return $this->idDocumento;
	}

}