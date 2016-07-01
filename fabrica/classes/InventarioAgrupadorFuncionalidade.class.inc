<?php
class InventarioAgrupadorFuncionalidade {
	
	private $id;
	private $nome;
	private $dataCadastro;
	
	public function setId( $id ){
		$this->id = $id;
	}
	
	public function getId(){
		return $id;
	}
	
	public function setNome( $nome ){
		$this->nome = $nome;
	}
	
	public function getNome(){
		return $this->nome;
	}
	
	public function setDataCadastro( DateTime $dataCadastro ){
		$this->dataCadastro = $dataCadastro;
	}
	
	public function getDataCadastro(){
		return $this->dataCadastro;
	}
	
}