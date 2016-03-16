<?php
/**
 * TipoDocumento: Squema Workflow
 * @author Ráyner rayner.lima@squadra.com.br
 *
 */
class TipoDocumento {
	
	private $id;
	private $descricao;
	private $sistema;
	private $status;
	private $endereco;
	
	const SOLICITACAO_SERVICO = "Solicitação de Serviço";
	
	const ORDEM_SERVICO = "Ordem de Serviço";
	
	const ORDEM_SERVICO_PF = "Ordem de Serviço (PF)";
	
	const ARTEFATOS = "Artefatos";
	
	public function setId( $id ){
		$this->id = $id;
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
	
	public function setSistema( Sistema $sistema ){
		$this->sistema = $sistema;
	}
	
	public function getSistema(){
		return $this->sistema;
	}
	
	public function setStatus( $status ){
		$this->status = $status;
	}
	
	public function getStatus(){
		return $this->status;
	}
	
	public function setEndereco( $endereco ){
		$this->endereco = $endereco;
	}
	
	public function getEndereco(){
		return $this->endereco;
	}
	
}