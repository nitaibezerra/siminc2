<?php
class Inventario {
	
	private $id;
	private $sistema;
	private $dataCadastro;
	private $dataEncerramento;
	private $cpfCadastro;
	private $cpfEncerramento;
	private $situacao;
	private $qtdePF;

	public function setId( $id ){
		$this->id = $id;
	}
	
	public function getId(){
		return $id;
	}
	
	public function setSistema( Sistema $sistema ){
		$this->sistema = $sistema;
	}
	
	public function getSistema(){
		return $this->sistema;
	}
	
	public function setDataCadastro( DateTime $dataCadastro ){
		$this->dataCadastro = $dataCadastro;
	}
	
	public function getDataCadastro(){
		return $this->dataCadastro;
	}
	
	public function setDataEncerramento( DateTime $dataEncerramento ){
		$this->dataEncerramento = $dataEncerramento;
	}
	
	public function getDataEncerramento(){
		return $this->dataEncerramento;
	}
	
	public function setCpfCadastro( $cpfCadastro ){
		$this->cpfCadastro = $cpfCadastro;
	}
	
	public function getCpfCadastro(){
		return $this->cpfCadastro;
	}
	
	public function setCpfEncerramento( $cpfEncerramento ){
		$this->cpfEncerramento = $cpfEncerramento;
	}
	
	public function getCpfEncerramento(){
		return $this->cpfEncerramento;
	}
	
	public function setSituacao( $situacao ){
		$this->situacao = $situacao;	
	}
	
	public function getSituacao(){
		return $this->situacao;
	}
	
	public function setQtdePF( $qtdePF ){
		$this->qtdePF = $qtdePF; 
	}
	
}