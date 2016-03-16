<?php
class ItemAuditoria {

	private $id;
	private $nome;
	private $descricao;
	private $situacao;
	
	public function setId( $id ){
		$this->id = $id;
	}
	
	public function getId(){
		return $this->id;
	}
	
	public function setNome( $nome ){
		$this->nome = $nome;
	}
	
	public function getNome(){
		return $this->nome;
	}
	
	public function setDescricao( $descricao ){
		$this->descricao = $descricao;
	}
	
	public function getDescricao(){
		return $this->descricao;
	}
	
	public function setSituacao( $situacao ) {
		$this->situacao = $situacao;
	}
	
	public function getSituacao(){
		return $this->situacao;
	}
}