<?php
class ItemAuditoriaDetalhesAuditoriaRepositorio extends Modelo {
	
	public function __construct(){
		parent::__construct();
	}
	
	public function recupereItensAuditoria( $idDetalhesAuditoria ){
		$sql = "SELECT iadaid, dtaid, itemid FROM fabrica.itemauditoriadetalhesauditoria WHERE dtaid = $idDetalhesAuditoria";
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	public function salvarItensAuditoria( $idDetalhesAuditoria, array $idItensAuditoria ){
		$retorno = $this->removerItensAuditoria($idDetalhesAuditoria);
		if (count($idItensAuditoria)>0){
			$sql = "INSERT INTO fabrica.itemauditoriadetalhesauditoria (dtaid, itemid) VALUES ";
			for( $x=1; $x <= count($idItensAuditoria); $x++){
				$id = $idItensAuditoria[$x - 1];
				$sql .= "($idDetalhesAuditoria, $id)";
				if ($x < count($idItensAuditoria)) {
					$sql .= ", ";
				}
			}
			return (bool) parent::executar($sql) && parent::commit();
		}
		return $retorno;
	}
	
	public function removerItensAuditoria( $idDetalhesAuditoria ){
		$sql = "DELETE FROM fabrica.itemauditoriadetalhesauditoria WHERE dtaid = $idDetalhesAuditoria";
		return (bool) parent::executar($sql) && parent::commit();
	}
	
	private function montaObjeto( $linha ){
		$itemAuditoriaDetalhesAuditoria = new ItemAuditoriaDetalhesAuditoria();
		
		$detalhesAuditoria = new DetalhesAuditoria();
		$detalhesAuditoria->setId($linha['dtaid']);
		
		$itemAuditoria = new ItemAuditoria();
		$itemAuditoria->setId($linha['itemid']);
		
		$itemAuditoriaDetalhesAuditoria->setId($linha['iadaid']);
		$itemAuditoriaDetalhesAuditoria->setDetalhesAuditoria($detalhesAuditoria);
		$itemAuditoriaDetalhesAuditoria->setItemAuditoria($itemAuditoria);
		
		return $itemAuditoriaDetalhesAuditoria;
	} 
	
	
}