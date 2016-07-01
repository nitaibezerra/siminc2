<?php
class AcaoEstadoDocumentoRepositorio extends Modelo {
	
	public function __construct(){
		parent::__construct();
	}
	
	public function recuperePorEstadoDocumentoOrigemEstadoDocumentoDestino( $idEstadoDocumentoOrigem, $idEstadoDocumentoDestino ){
		$sql = "SELECT * FROM workflow.acaoestadodoc WHERE esdidorigem = $idEstadoDocumentoOrigem AND esdiddestino = $idEstadoDocumentoDestino";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	public function montaObjeto( $linha ){
		$estadoDocumentoOrigem = new EstadoDocumento();
		$estadoDocumentoOrigem->setId($linha['esdidorigem']);
		
		$estadoDocumentoDestino = new EstadoDocumento();
		$estadoDocumentoDestino->setId($linha['esdiddestino']);
		
		$acaoEstadoDocumento = new AcaoEstadoDocumento();
		$acaoEstadoDocumento->setId($linha['aedid']);
		$acaoEstadoDocumento->setEstadoDocumentoOrigem($estadoDocumentoOrigem);
		$acaoEstadoDocumento->setEstadoDocumentoDestino($estadoDocumentoDestino);
		$acaoEstadoDocumento->setStatus($linha['aedstatus']);
		$acaoEstadoDocumento->setDescricaoARealizar($linha['aeddscrealizar']);
		$acaoEstadoDocumento->setDescricaoRealizado($linha['aeddscrealizada']);
		return $acaoEstadoDocumento;
	}
}