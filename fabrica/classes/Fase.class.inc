<?php 
class Fase {

	private $id;
	private $descricao;
	private $status;
	
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
	
	function setStatus($status){
		$this->status = $status;
	}
	
	function getStatus(){
		return $this->status;
	}
	
}
