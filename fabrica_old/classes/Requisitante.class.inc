<?php
class Requisitante {

	private $cpf;
	
	private $nome;
	
	function setCpf($cpf){
		$this->cpf = $cpf;
	}
	
	function getCpf(){
		return $this->cpf;
	}
	
	function setNome($nome){
		$this->nome = $nome;
	}
	
	function getNome(){
		return $this->nome;
	}
	
}

