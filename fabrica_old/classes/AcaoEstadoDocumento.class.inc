<?php

class AcaoEstadoDocumento {
	
	private $id;
	private $estadoDocumentoOrigem;
	private $estadoDocumentoDestino;
	private $descricaoARealizar;
	private $status;
	private $descricaoRealizado;
//	private $comentario;
//	private $funcaoPreCondicao;
//	private $observacao;
//	private $funcaoPosCondicao;
//	private $visivel;
//	private $icone;
//	private $visivelQuandoAcaoNegativa;
	
	public function setId( $id ){
		$this->id = $id;
	}
	
	public function getId(){
		return $this->id;
	}
	
	public function setEstadoDocumentoOrigem( EstadoDocumento $estadoDocumentoOrigem ) {
		$this->estadoDocumentoOrigem = $estadoDocumentoOrigem;
	}
	
	public function getEstadoDocumentoOrigem(){
		return $this->estadoDocumentoOrigem;
	}
	
	public function setEstadoDocumentoDestino( EstadoDocumento $estadoDocumentoDestino ){
		$this->estadoDocumentoDestino = $estadoDocumentoDestino;
	}
	
	public function getEstadoDocumentoDestino(){
		return $this->estadoDocumentoDestino;
	}
	
	public function setDescricaoARealizar( $descricaoARealizar ){
		$this->descricaoARealizar = $descricaoARealizar;
	}
	
	public function getDescricaoARealizar(){
		return $this->descricaoARealizar;
	} 
	
	public function setStatus( $status ){
		$this->status = $status;
	}
	
	public function getStatus(){
		return $this->status;
	}
	
	public function setDescricaoRealizado( $descricaoRealizado ){
		$this->descricaoRealizado = $descricaoRealizado;
	}
	
	public function getDescricaoRealizado(){
		return $this->descricaoRealizado;
	}
	
}