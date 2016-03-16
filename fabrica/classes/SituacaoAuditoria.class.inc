<?php 
class SituacaoAuditoria {
	
	const PENDENTE = 1;
	const CONCLUIDA = 2;
	
	private $id;
	private $descricao;
	
	function setId($id){
		$this->id = $id;
	}
	
	function getId(){
		return $this->id;
	}

	function setDescricao($descricao){
		$this->descricao = $descricao; 
	}
	
	function getDescricao(){
		return $this->descricao; 
	}
	
	function getValues(){
		return $this->values;
	}
	
}

