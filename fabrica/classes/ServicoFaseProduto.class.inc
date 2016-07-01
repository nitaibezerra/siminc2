<?php 
class ServicoFaseProduto {

	private $id;
	private $faseDisciplinaProduto;
	private $analiseSolicitacao;
	private $tipoExecucao;
	private $repositorio;
	private $padraoNome;
	private $padraoDiretorio;
	private $encontrado;
	private $atualizado;
	private $necessario;
	
	public function setId($id){
		$this->id = $id;
	}
	
	public function getId(){
		return $this->id;	
	}
	
	public function setFaseDisciplinaProduto(FaseDisciplinaProduto $faseDisciplinaProduto){
		$this->faseDisciplinaProduto = $faseDisciplinaProduto;
	}
	
	public function getFaseDisciplinaProduto(){
		return $this->faseDisciplinaProduto;
	}
	
	public function setAnaliseSolicitacao(AnaliseSolicitacao $analiseSolicitacao){
		$this->analiseSolicitacao = $analiseSolicitacao;
	}
	
	public function getAnaliseSolicitacao(){
		return $this->analiseSolicitacao;
	}
	
	public function setTipoExecucao(TipoExecucao $tipoExecucao){
		$this->tipoExecucao = $tipoExecucao;
	}
	
	public function getTipoExecucao(){
		return $this->tipoExecucao;
	}
	
	public function setRepositorio($repositorio){
		$this->repositorio = $repositorio;
	}
	
	public function getRepositorio(){
		return $this->repositorio;
	}
	
	public function getRepositorioParaDownload(){
		$enderecoSVN = $this->getRepositorio();
		$enderecoSVN = str_replace("svn+ssh://", "", $enderecoSVN);
		$enderecoSVN = preg_replace("/(\w+)?@?subversion.mec.gov.br/", "http://svnconsulta.mec.gov.br", $enderecoSVN);
		return $enderecoSVN;
	}
	
	public function setPadraoNome($padraoNome){
		$this->padraoNome = $padraoNome;
	}
	
	public function getPadraoNome(){
		return $this->padraoNome;
	}
	
	public function isPadraoNome(){
		return $this->padraoNome == "t" ? true : false;
	}
	
	public function setPadraoDiretorio($padraoDiretorio){
		$this->padraoDiretorio = $padraoDiretorio;
	}
	
	public function getPadraoDiretorio(){
		return $this->padraoDiretorio;
	}
	
	public function isPadraoDiretorio(){
		return $this->padraoDiretorio == "t" ? true : false;
	}
	
	public function setEncontrado($encontrado){
		$this->encontrado = $encontrado;
	}
	
	public function getEncontrado(){
		return $this->encontrado;
	}
	
	public function isEncontrado(){
		return $this->encontrado == "t" ? true : false;
	}
	
	public function setAtualizado($atualizado){
		$this->atualizado = $atualizado;
	}
	
	public function getAtualizado(){
		return $this->atualizado;
	}
	
	public function isAtualizado(){
		return $this->atualizado == "t" ? true : false;
	}
	
	public function setNecessario($necessario){
		$this->necessario = $necessario;
	}
	
	public function getNecessario(){
		return $this->necessario;
	}
	
	public function isNecessario(){
		return $this->necessario == "t" ? true : false;
	}
	
	public function isDetalhadoEmAuditoria(){
		return $this->isAtualizado() OR $this->isEncontrado() OR 
				$this->isNecessario() OR $this->isPadraoDiretorio() OR $this->isPadraoNome();
	}
	
	public function possuiRepositorio(){
		if ($this->getRepositorio()==""){
			return false;
		} else {
			return true;
		}
	}
}