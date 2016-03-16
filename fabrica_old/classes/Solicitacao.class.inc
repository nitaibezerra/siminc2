<?php 
class Solicitacao {

	private $id;
	private $sistema;
	private $analiseSolicitacao;
	private $requisitante;

	/**
	 * 
	 * Deprecated
	 * Ao invés do uso desta propriedade utilise EstadoDocumento
	 * @var SituacaoSolicitacao
	 */
	private $situacao;
	private $estadoDocumento;
	
	private $auditoria;
	private $produtosContratados;
	private $produtosAuditados;
	private $dataAbertura;
	private $dataFinalizacao;
	private $quantidadeProdutosContratados;
	private $quantidadeProdutosAuditados;
	private $ordemServicoItemUm;
	private $listaOrdemServicoItemUm;
	private $fiscal;

	public function  setId($id){
		$this->id = $id;
	}
	
	public function getId(){
		return $this->id;
	}
	
	public function setSistema(Sistema $sistema){
		$this->sistema = $sistema;
	}
	
	public function getSistema(){
		return $this->sistema;
	}
	
	public function setAnaliseSolicitacao($analiseSolicitacao){
		$this->analiseSolicitacao = $analiseSolicitacao;
	}
	
	public function getAnaliseSolicitacao(){
		return $this->analiseSolicitacao;
	}
	
	public function setRequisitante(Requisitante $requisitante){
		$this->requisitante = $requisitante;
	}
	
	public function getRequisitante(){
		return $this->requisitante;
	}
	
	/**
	 * 
	 * Deprecated
	 * Ao invés do uso desta propriedade utilise setEstadoDocumento
	 * @var SituacaoSolicitacao situacao
	 */
	public function setSituacao($situacao){	
		$this->situacao = $situacao;
	}
	
	/**
	 * 
	 * Deprecated
	 * Ao invés do uso desta propriedade utilise setEstadoDocumento
	 */
	public function getSituacao(){
		return $this->situacao;
	}
	
	public function setEstadoDocumento( EstadoDocumento $estadoDocumento ){
		$this->estadoDocumento = $estadoDocumento;
	}
	
	public function getEstadoDocumento(){
		return $this->estadoDocumento;
	}
	
	public function setAuditoria(Auditoria $auditoria){
		$this->auditoria = $auditoria;
	}
	
	public function getAuditoria(){
		return $this->auditoria;
	}
	
	public function possuiAuditoria(){
		if ($this->getAuditoria() == null) {
			return false;
		} else {
			return true;
		}
	}
	
	public function setProdutosContratados($produtosContratados){
		$this->produtosContratados = $produtosContratados;
	}
	
	public function getProdutosContratados(){
		return $this->produtosContratados;
	}
	
	public function setProdutosAuditados($produtosAuditados){
		$this->produtosAuditados = $produtosAuditados;
	}
	
	public function getProdutosAuditados(){
		return $this->produtosAuditados;
	}
	
	public function setQuantidadeProdutosContratados( $quantidadeProdutosContratados ){
		$this->quantidadeProdutosContratados = $quantidadeProdutosContratados;
	} 
	
	public function getQuantidadeProdutosContratados(){
		return $this->quantidadeProdutosContratados == null ? count($this->getProdutosContratados()) : $this->quantidadeProdutosContratados;
	}
	
	public function setQuantidadeProdutosAuditados( $quantidadeProdutosAuditados ){
		$this->quantidadeProdutosAuditados = $quantidadeProdutosAuditados;
	}
	
	public function getQuantidadeProdutosAuditados(){
		return $this->quantidadeProdutosAuditados == null ? count($this->getProdutosAuditados()) : $this->quantidadeProdutosAuditados;
	}
	
	public function getPorcentagemAuditada(){
		if ($this->getQuantidadeProdutosContratados() == 0 ){
			return number_format(0, 2, ',', '') . "%";
		}
		$porcentagem = $this->getQuantidadeProdutosAuditados() * 100 / $this->getQuantidadeProdutosContratados();
		return number_format($porcentagem, 2, ',', '') . "%";
	}
	
	public function setDataAbertura($dataAbertura){
		$this->dataAbertura = date_create($dataAbertura);
	}
	
	public function getDataAbertura(){
		return $this->dataAbertura;
	}
	
	public function getDataAberturaFormatada($formato){
		return $this->getDataAbertura()->format($formato);
	}
	
	public function setDataFinalizacao($dataFinalizacao){
		$this->dataFinalizacao = date_create($dataFinalizacao);
	}
	
	public function getDataFinalizacao(){
		return $this->dataFinalizacao;
	}
	
	public function getDataFinalizacaoFormatada($formato){
		return $this->getDataFinalizacao()->format($formato);
	}
	
	public function setOrdemServicoItemUm( OrdemServico $ordemServicoItemUm ){
		$this->ordemServicoItemUm = $ordemServicoItemUm;
	}
	
	public function getOrdemServicoItemUm(){
		return $this->ordemServicoItemUm;
	}
	
	public function setListaOrdensServicoItemUm( array $listaOrdensServicoItemUm ){
		$this->listaOrdemServicoItemUm = $listaOrdensServicoItemUm;
	}
	
	public function getListaOrdensServicoItemUm(){
		return $this->listaOrdemServicoItemUm;
	}
	
	public function isPassivelAuditoria(){
		return $this->getEstadoDocumento()->getId() == StatusSolicitacaoServico::FINALIZADA ||
			$this->getEstadoDocumento()->getId() == StatusSolicitacaoServico::AGUARDANDO_PAGAMENTO || 
			$this->getEstadoDocumento()->getId() == StatusSolicitacaoServico::EM_EXECUCAO &&
			$this->getOrdemServicoItemUm()->getStatusOrdemServico() == StatusOrdemServico::AGUARDANDO_HOMOLOGACAO_GESTOR ||
			$this->getOrdemServicoItemUm()->getStatusOrdemServico() == StatusOrdemServico::EM_AVALIACAO ||
			$this->getOrdemServicoItemUm()->getStatusOrdemServico() == StatusOrdemServico::EM_APROVACAO ||
			$this->getOrdemServicoItemUm()->getStatusOrdemServico() == StatusOrdemServico::FINALIZADA ? true : false;
	}
	
	public function setFiscal( Fiscal $fiscal ){
		$this->fiscal = $fiscal;
	}
	
	public function getFiscal(){
		return $this->fiscal;
	}
	
}



