<?php
class TipoExecucaoRepositorio extends Modelo {
	
	
	public function __construct(){
		parent::__construct();
	}
	
	public function recuperePorId( $idTipoExecucao ){
		$sql = "SELECT tpeid, tpedsc, tpestatus FROM fabrica.tipoexecucao where tpeid = $idTipoExecucao";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);	
	}
	
	private function montaObjeto( $linha ){
		$tipoExecucao = new TipoExecucao();
		$tipoExecucao->setId($linha['tpeid']);
		$tipoExecucao->setDescricao($linha['tpedsc']);
		$tipoExecucao->setStatus($linha['tpestatus']);
		return $tipoExecucao;
	}
}