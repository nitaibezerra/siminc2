<?php 
class FaseDisciplina {

	private $id;
	private $fase;
	private $disciplina;
	private $porcentagemPontoFuncao;
	private $descricao;
	
	function setId($id){
		$this->id = $id;
	}
	
	function getId(){
		return $this->id;
	}
	
	function setFase(Fase $fase){
		$this->fase = $fase;
	}
	
	function getFase(){
		return $this->fase;
	}
	
	function setDisciplina(Disciplina $disciplina){
		$this->disciplina = $disciplina;
	}
	
	function getDisciplina(){
		return $this->disciplina;
	}
	
	function setPorcentagemPontoFuncao($porcentagemPontoFuncao){
		$this->porcentagemPontoFuncao = $porcentagemPontoFuncao;
	}
	
	function getPorcentagemPontoFuncao(){
		return $this->porcentagemPontoFuncao;
	}
	
	function setDescricao($descricao){
		$this->descricao = $descricao;
	}
	
	function getDescricao(){
		return $this->descricao;
	}
	
}