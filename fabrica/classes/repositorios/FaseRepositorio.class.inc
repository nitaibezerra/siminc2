<?php 
class FaseRepositorio extends Modelo {

	public function recuperePorId($idFase){
		$sql = "select fasid, fasdsc, fasstatus from fabrica.fase where fasid = $idFase";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	private function montaObjeto($linha){
		$fase = new Fase();
		$fase->setId($linha['fasid']);
		$fase->setDescricao($linha['fasdsc']);
		$fase->setStatus($linha['fasstatus']);
		return $fase;
	}
}
