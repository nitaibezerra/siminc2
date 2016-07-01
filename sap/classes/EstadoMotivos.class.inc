<?php

/**
 * Modelo de Relacionamento de Estados e Motivos
 * @author Silas Matheus
 */
class EstadoMotivos extends Modelo{
	
	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sap.estadomotivos";

	/**
	 * Chave primária
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('esmid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'esmid' => null,
		'ecoid' => null,
		'mecid' => null
	);	
	
	/**
	 * Retorna o id do estado motivo de acordo com o estado e motivo selecionados
	 * @name pegarId
	 * @author Silas Matheus
	 * @access public
	 * @param int $ecoid - Id do estado de conservacao
	 * @param int $mecid - Id do motivo de estado de conservacao  
	 * @return int
	 */
	public function pegarId($ecoid, $mecid){
	
		$ecoid = addslashes($ecoid);
		$mecid = addslashes($mecid);
		
		$sql = "SELECT 
					esmid 
				FROM 
					$this->stNomeTabela
				WHERE
					ecoid=$ecoid
				AND
					mecid=$mecid";
					
		return $this->pegaUm($sql);
	
	}
	
	/**
	 * Retorna os ids dos motivos de acordo com o estado de conservacao
	 * @name pegarMotivosPorEstadoConservacao
	 * @author Silas Matheus
	 * @access public
	 * @param int $ecoid - Id do estado de conservacao
	 * @return int
	 */
	public function pegarMotivosPorEstadoConservacao($ecoid = ''){
	
		$sql = "SELECT
					mecid 
				FROM 
					$this->stNomeTabela";				
					
		if(!empty($ecoid)){
			$ecoid = addslashes($ecoid);
			$sql .= " WHERE ecoid=$ecoid";
		}
					
		return $this->carregarColuna($sql);
		
	}
	
	/**
	 * Atualiza as associacoes os motivos para os estados de conservacoes 
	 * @name relacionarMotivosEstados
	 * @author Silas Matheus
	 * @access public
	 * @param int $ecoid - Id do estado de conservacao
	 * @param int $motivos - Array com os ids dos motivos
	 * @return bool
	 */
	public function relacionarMotivosEstados($ecoid, $motivos){

		$atualizou = true;
		$motivosAntigos = $this->pegarMotivosPorEstadoConservacao($ecoid);
		
		$ecoid = addslashes($ecoid);

		// verificando se existe registro associado ao estado e motivo (para não deixar deletar)
		$deletados = array_diff($motivosAntigos, $motivos);
		
		if(count($deletados) > 0){
			
			$deletados = join($deletados, ',');
			$sql = "SELECT
						count(*) 
					FROM 
						$this->stNomeTabela esm
					INNER JOIN
						sap.rgp rgp 
						ON rgp.esmid=esm.esmid
					WHERE
						mecid in ($deletados)
					AND
						ecoid=$ecoid";			
						
			if($this->pegaUm($sql) == 0){
			
				// deletando as relacoes 
				$sql = "DELETE 
						FROM 
							$this->stNomeTabela esm
						WHERE
							mecid in ($deletados)
						AND
							ecoid=$ecoid";
							
				$this->executar($sql);
				$this->commit();
			
			}else{
			
				// existem rgps relacionados com esse estado e motivo
				$atualizou = false;
			
			}
			
		}
					
		// adicionando as novas relacoes entre estado e motivo
		$novos = array_diff($motivos, $motivosAntigos);
		foreach($motivos as $mecid){

			$sql = "SELECT
						count(*) 
					FROM 
						$this->stNomeTabela
					WHERE
						mecid=$mecid
					AND
						ecoid=$ecoid";
						
			if($this->pegaUm($sql) == 0){
				
				$sql = "INSERT INTO
							$this->stNomeTabela
							(mecid,ecoid)
						VALUES
							($mecid, $ecoid)";
				$this->executar($sql);
				$this->commit();
				
			} 
		
		}
		
		return $atualizou;
	
	}
	
	
}