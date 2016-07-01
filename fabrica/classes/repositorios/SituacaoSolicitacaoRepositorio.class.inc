<?php 
class SituacaoSolicitacaoRepositorio extends Modelo {


	function recupereTodos(){
		$sql = "select distinct ed.esdid, ed.esddsc
				from workflow.estadodocumento ed 
				join workflow.tipodocumento td 
					on ed.tpdid = ed.tpdid
				where ed.tpdid = 26 
                ORDER BY ed.esddsc";
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	function recuperePorId($idSituacaoSolicitacaoServico){
		$sql = "select ed.esdid, ed.esddsc
				from workflow.estadodocumento ed 
				join workflow.tipodocumento td 
					on ed.tpdid = ed.tpdid
				join workflow.documento doc
					on ed.tpdid = doc.tpdid
				where ed.tpdid = 26 and td.tpdid = 26 and ed.esdid = $idSituacaoSolicitacaoServico";
		try {
			$linha = parent::pegaLinha($sql);
			$situacao= $this->montaObjeto($linha);
			return $situacao;
		} catch (Exception $e){
			return null;
		}
	}
	
	private function montaObjeto($linha){
		$situacao = new SituacaoSolicitacao();
		$situacao->setId($linha['esdid']);
		$situacao->setDescricao($linha['esddsc']);
		return $situacao;
	}
	
	
}
