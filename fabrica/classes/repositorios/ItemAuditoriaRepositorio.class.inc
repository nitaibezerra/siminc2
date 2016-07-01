<?php
class ItemAuditoriaRepositorio extends Modelo {
	
	public function recupereTodos( $orderBy = 'itemnome', $ordem = Ordenador::ORDEM_PADRAO, 
		$limit = Paginador::LIMIT_PADRAO, $offset = Paginador::OFFSET_PADRAO, $where = ''){
			
		$sql = "SELECT * FROM ";
		$sql .= "(SELECT ia.itemid, ia.itemnome, ia.itemdsc, ia.itemsituacao 
				FROM fabrica.itemauditoria ia ORDER BY $orderBy LIMIT $limit OFFSET $offset) itemsituacao ";
		if ($where != '') {
			$sql .= $where;		
		}
		
		$sql .= " ORDER BY $orderBy $ordem";
		
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	public function recupereTodosSemPaginacao(){
			
		$sql = "SELECT ia.itemid, ia.itemnome, ia.itemdsc, ia.itemsituacao 
				FROM fabrica.itemauditoria ia WHERE ia.itemsituacao = true ORDER BY itemnome ";
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	public function recuperePorId( $idItemAuditoria ){
		$sql = "SELECT ia.itemid, ia.itemnome, ia.itemdsc, ia.itemsituacao 
				FROM fabrica.itemauditoria ia
				WHERE ia.itemid = $idItemAuditoria";
		
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	public function armazene( ItemAuditoria $itemAuditoria ){
		$itemnome = $itemAuditoria->getNome();
		$itemdsc = $itemAuditoria->getDescricao();
		$itemsituacao = $itemAuditoria->getSituacao();
		$itemid = $itemAuditoria->getId();
		
		if ($itemid == '') {
			$sql = "INSERT INTO fabrica.itemauditoria (itemnome, itemdsc, itemsituacao)
					VALUES('$itemnome', '$itemdsc', '$itemsituacao')";
		} else {
			$sql = "UPDATE fabrica.itemauditoria 
					SET itemnome = '$itemnome', itemdsc = '$itemdsc', itemsituacao = '$itemsituacao' 
					WHERE itemid = $itemid";
		}
		return (bool) parent::executar($sql) && parent::commit();
	}
	
	public function recupereQtdeRegistros(){
		$sql = "SELECT count(*) as qtde FROM fabrica.itemauditoria ia";
		return parent::pegaUm($sql);
	}	
	
	private function montaObjeto($linha){
		$itemAuditoria = new ItemAuditoria();
		$itemAuditoria->setId($linha['itemid']);
		$itemAuditoria->setNome($linha['itemnome']);
		$itemAuditoria->setDescricao($linha['itemdsc']);
		$itemAuditoria->setSituacao($linha['itemsituacao']=="t" ? "Ativo" : "Desativado");
		return $itemAuditoria;
	}
}