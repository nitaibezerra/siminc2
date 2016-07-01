<?php 
class Fiscal {

	private $id;//CPF do Fiscal
	private $nome;
	private $email;
	private $funcao;
	private $sexo;
	
	function setId($id){
		$this->id = $id;
	}
	
	function getId(){
		return $this->id;
	}
	
	function setNome($nome){
		$this->nome = $nome;
	}
	
	function getNome(){
		return $this->nome;
	}
	
	function setEmail($email){
		$this->email = $email;
	}
	
	function getEmail(){
		return $this->email;
	}
	
	function setFuncao($funcao){
		$this->funcao = $funcao;
	}
	
	function getFuncao(){
		return $this->funcao;
	} 
	
	function setSexo($sexo){
		$this->sexo = $sexo;
	}
	
	function getSexo(){
		return $this->sexo;
	}
	
}