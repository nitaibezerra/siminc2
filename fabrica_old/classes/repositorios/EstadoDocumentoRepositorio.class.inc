<?php
class EstadoDocumentoRepositorio extends Modelo {
	
	public function __construct(){
		parent::__construct();
	}
	
	public function recuperePorTipoDocumentoEDescricao( $idTipoDocumento, $descricaoEstadoDocumento ){
		$sql = "SELECT esdid, tpdid, esdstatus, esdordem, esddsc 
				FROM workflow.estadodocumento WHERE tpdid = $idTipoDocumento and esddsc = '$descricaoEstadoDocumento'";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	public function recuperePorIdDocumento( $idDocumento ){
		$sql = "SELECT ed.esdid, ed.tpdid, ed.esdstatus, ed.esdordem, ed.esddsc
				FROM workflow.estadodocumento ed
				JOIN workflow.documento d
					ON d.esdid = ed.esdid
				WHERE d.docid = $idDocumento";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	private function montaObjeto( $linha ){
		$tipoDocumento = new TipoDocumento();
		$tipoDocumento->setId($linha['tpdid']);
		
		$estadoDocumento = new EstadoDocumento();
		$estadoDocumento->setId($linha['esdid']);
		$estadoDocumento->setTipoDocumento($tipoDocumento);
		$estadoDocumento->setStatus($linha['esdstatus']);
		$estadoDocumento->setOrdem($linha['esdordem']);
		$estadoDocumento->setDescricao($linha['esddsc']);
		return $estadoDocumento;
	}
	
}