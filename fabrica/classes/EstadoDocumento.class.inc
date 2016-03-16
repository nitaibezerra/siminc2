<?php
class EstadoDocumento {

	
	const ARTEFATO_PENDENTE = "Pendente";
	const ARTEFATO_EM_AUDITORIA = "Em Auditoria";
	const ARTEFATO_FINALIZADA = "Finalizada";
	 
	private $id;
	private $tipoDocumento;
	private $status;
	private $ordem;
	private $descricao;
	 
	
	public function setId( $id ){
		$this->id = $id;
	}
	
	public function getId(){
		return $this->id;
	}
	
	public function setTipoDocumento( TipoDocumento $tipoDocumento ){
		$this->numTipoDocumento = $tipoDocumento;
	}
	
	public function getTipoDocumento(){
		return $this->numTipoDocumento;
	}
	
	public function setStatus( $status ){
		$this->status = $status;
	}
	
	public function getStatus(){
		return $this->status;
	}
	
	public function setOrdem( $ordem ){
		$this->ordem = $ordem;
	}
	
	public function getOrdem(){
		return $this->ordem;
	}
	
	public function setDescricao( $descricao ){
		$this->descricao = $descricao;
	}
	
	public function getDescricao(){
		return $this->descricao;
	}
	
}