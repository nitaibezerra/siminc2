<?php 
class AnaliseSolicitacaoRepositorio extends Modelo {
	
	function recuperePorId($idAnaliseSolicitacao){
		$sql = "select ansid from fabrica.analisesolicitacao where ansid = $idAnaliseSolicitacao";
		try {
			$linha = parent::pegaLinha($sql);
			$analiseSolicitacao= $this->montaObjeto($linha);
			return $analiseSolicitacao;
		} catch (Exception $e){
			return null;
		}
	}
	
	function recuperePorIdSolicitacao( $idSolicitacao ){
	$sql = "select ansid from fabrica.analisesolicitacao where scsid = $idSolicitacao";
		$linha = parent::pegaLinha($sql);
		$analiseSolicitacao= $this->montaObjeto($linha);
		return $analiseSolicitacao;
	}

	private function montaObjeto($linha){
		$analiseSolicitacao = new AnaliseSolicitacao();
		$analiseSolicitacao->setId($linha['ansid']);
		return $analiseSolicitacao;
	}
}