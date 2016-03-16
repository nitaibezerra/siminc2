<?php 
class FaseDisciplinaProduto {

	private $id;
	private $faseDisciplina;
	private $produto;
	private $status;
	private $contratada;
	private $misto;
	
	function setId($id){
		$this->id = $id;
	}
	
	function getId(){
		return $this->id;
	}
	
	function setFaseDisciplina(FaseDisciplina $faseDisciplina){
		$this->faseDisciplina = $faseDisciplina;
	}
	
	function getFaseDisciplina(){
		return $this->faseDisciplina;
	}
	
	function setProduto(Produto $produto){
		$this->produto = $produto;
	}
	
	function getProduto(){
		return $this->produto;
	}
	
	function setStatus($status){
		$this->status = $status;
	}
	
	function getStatus(){
		return $this->status;
	}
	
	function setContratada($contratada){
		$this->contratada = $contratada;
	}
	
	function getContratada(){
		return $this->contratada;
	}
	
	function setMisto($misto){
		$this->misto = $misto;
	}
	
	function getMisto(){
		return $this->misto;
	}
	
}