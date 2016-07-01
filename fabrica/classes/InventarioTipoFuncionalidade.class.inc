<?php
class InventarioTipoFuncionalidade{
	
	private $id;
	private $descricao;
	private $dataCadastro;

	public function setId( $id ){
		$this->id= $id;
	}
	
	public function getId(){
		return $this->id;
	}
	
	public function setDescricao( $descricao ){
		$this->descricao = $descricao;
	}
	
	public function getDescricao(){
		return $this->descricao;
	}
	
	public function setDataCadastro( DateTime $dataCadastro ){
		$this->dataCadastro = $dataCadastro;
	}
	
	public function getDataCadastro(){
		return $this->dataCadastro;
	}
	
}