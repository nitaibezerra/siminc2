<?php
class ProdutoContratado {
	
	private $id;
	private $analiseSolicitacao;
	private $produto;
	private $disciplina;
	private $listaServicoFaseProduto;
	
	public function __construct(AnaliseSolicitacao $analiseSolicitacao, Produto $produto, Disciplina $disciplina){
		$this->setAnaliseSolicitacao($analiseSolicitacao);
		$this->setProduto($produto);
		$this->setDisciplina($disciplina);
		$this->setId($analiseSolicitacao->getId() ."-". $produto->getId() ."-". $disciplina->getId());
	}
	
	public function setId( $id ){
		$this->id = $id;
	}
	
	public function getId(){
		return $this->id;
	}
	
	public function setAnaliseSolicitacao( AnaliseSolicitacao $analiseSolicitacao ){
		$this->analiseSolicitacao = $analiseSolicitacao;
	}
	
	public function getAnaliseSolicitacao(){
		return $this->analiseSolicitacao;
	}
	
	public function setProduto( Produto $produto ){
		$this->produto = $produto;
	}
	
	public function getProduto(){
		return $this->produto;
	}
	
	public function setDisciplina( Disciplina $disciplina ){
		$this->disciplina = $disciplina;
	}
	
	public function getDisciplina(){
		return $this->disciplina;
	}
	
	public function equals( ProdutoContratado $produtoContratado ){
		return $this->getId() == $produtoContratado->getId();
	}
	
	public function appendServicoFaseProduto( ServicoFaseProduto $servicoFaseProduto ){
		$this->listaServicoFaseProduto[] = $servicoFaseProduto;
	}
	
	public function getListaServicoFaseProduto(){
		return 	$this->listaServicoFaseProduto;
	}
	
}