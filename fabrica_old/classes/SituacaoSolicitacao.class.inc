<?php 
class SituacaoSolicitacao {

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
	
}
