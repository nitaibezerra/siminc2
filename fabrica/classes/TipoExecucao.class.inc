<?php
class TipoExecucao {
	
	const CONTRATADA = 1;
	const CONTRATANTE = 2;
	
	private $id;
	private $descricao;
	private $status;
	
	public function setId( $id ){
		$this->id = $id;
	}
	
	public function getId(){
		return $this->id;
	}
	
	public function setDescricao( $descricao ){
		$this->descricao = $descricao;
	}
	
	public function getDescriaco(){
		return $this->descricao;
	}
	
	public function setStatus( $status ){
		$this->status = $status;
	}
	
	public function getStatus(){
		return $this->status;
	}
}