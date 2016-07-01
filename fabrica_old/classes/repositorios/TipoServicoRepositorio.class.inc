<?php
class TipoServicoRepositorio extends Modelo {
	
	public function __construct() {
		parent::__construct();
	}
	
	public function recuperePorId( $idTipoServico ){
		$sql = "SELECT tpsid, tpsdsc, tpsstatus FROM fabrica.tiposervico WHERE tpsstatus = 'A' AND tpsid = $idTipoServico";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	private function montaObjeto( $linha ){
		$tipoServico = new TipoServico();
		$tipoServico->setId( $linha['tpsid'] );
		$tipoServico->setDescricao( $linha['tpsdsc'] );
		$tipoServico->setStatus( $linha['tpsstatus'] );
		return $tipoServico;
	}
}