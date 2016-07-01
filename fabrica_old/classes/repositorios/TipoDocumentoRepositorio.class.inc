<?php

class TipoDocumentoRepositorio extends Modelo {
	
	public function __construct(){
		parent::__construct();
	}
	
	public function recuperePelaDescricao( $descricao ){
		$sql = "SELECT * FROM workflow.tipodocumento where tpddsc = '$descricao'";
		return $this->montaObjeto(parent::pegaLinha($sql));	
	}
	
	private function montaObjeto( $linha ){
		$tipoDocumento = new TipoDocumento();
		$tipoDocumento->setId($linha['tpdid']);
		$tipoDocumento->setDescricao($linha['tpddsc']);
		$sistema = new Sistema();
		$sistema->setId($linha['sisid']);
		$tipoDocumento->setSistema($sistema);
		$tipoDocumento->setStatus($linha['tpdstatus']);
		$tipoDocumento->setEndereco($linha['tpdendereco']);
		return $tipoDocumento;
	}
}