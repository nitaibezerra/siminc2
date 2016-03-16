<?php
/**
 * Produto
 * @author rayner
 *
 */
class Produto {
	
	const SERVICO_SEM_NECESSIDADE_DE_ARTEFATO = 44;
	
	private $id;
	private $disciplina;
	private $descricao;
	private $status;
	
	function setId($id){
		$this->id = $id;
	}
	
	function getId(){
		return $this->id;
	}
	
	function setDisciplina(Disciplina $disciplina){
		$this->disciplina = $disciplina;
	}
	 
	function getDisciplina(){
		return $this->disciplina;
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