<?php 
class Sistema {
	
	const FABRICA = 87;
	
	private $id;
	private $sigla;
	private $descricao;
	
	function setId($id){
		$this->id = $id;
	}
	
	function getId(){
		return $this->id;
	}
	
	function setSigla($sigla){
		$this->sigla = $sigla;
	}
	
	function getSigla(){
		return $this->sigla;
	}
	
	function setDescricao($descricao){
		$this->descricao = $descricao;
	}
	
	function getDescricao(){
		return $this->descricao;
	}
	
}



