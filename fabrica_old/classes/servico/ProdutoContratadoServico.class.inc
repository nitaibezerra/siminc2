<?php
class ProdutoContratadoServico {
	
	private $colecaoProdutosContratados = array();
	
	private $servicoFaseProdutoRepositorio;
	
	public function __construct(){
		$this->servicoFaseProdutoRepositorio = new ServicoFaseProdutoRepositorio();
	}
	
	public function criaColecaoProdutosContratados ( array $listaServicoFaseProduto ){
		$this->criaColecaoProdutosContratado($listaServicoFaseProduto);
		return $this->recuperaColecaoProdutosContratados();
	}
	
	public function recupereProdutoContratadoPorId( $idProdutoContratado ){
		
		$idAnaliseSolicitacao = substr($idProdutoContratado, 0, strpos($idProdutoContratado, '-'));
		$idProdutoContratado = substr($idProdutoContratado, (strpos($idProdutoContratado, '-')+1));
		$idProduto = substr($idProdutoContratado, 0, strpos($idProdutoContratado, '-'));
		$idDisciplina = substr($idProdutoContratado, (strpos($idProdutoContratado, '-')+1));
		
		$sql = "select sfp.sfpid from fabrica.servicofaseproduto sfp
				join fabrica.fasedisciplinaproduto fdp on sfp.fdpid = fdp.fdpid
				join fabrica.fasedisciplina fd on fd.fsdid = fdp.fsdid
				join fabrica.produto p on p.prdid = fdp.prdid
				where sfp.ansid = $idAnaliseSolicitacao AND p.prdid = $idProduto AND fd.dspid = $idDisciplina";
		
		$resultSet = $this->servicoFaseProdutoRepositorio->carregar($sql);
		$listaServicoFaseProduto = array();
		foreach ($resultSet as $linha){
			$listaServicoFaseProduto[] = $this->servicoFaseProdutoRepositorio->recuperePorId($linha['sfpid']);
		}
		return $this->criaColecaoProdutosContratados($listaServicoFaseProduto);
		
	}
	
	private function recuperaColecaoProdutosContratados(){
		return $this->colecaoProdutosContratados;
	}

	private function criaColecaoProdutosContratado( array $listaServicoFaseProduto ){
		foreach ($listaServicoFaseProduto as $servicoFaseProduto) {
			$analiseSolicitacao = $servicoFaseProduto->getAnaliseSolicitacao();
			$produto = $servicoFaseProduto->getFaseDisciplinaProduto()->getProduto();
			$disciplina = $servicoFaseProduto->getFaseDisciplinaProduto()->getFaseDisciplina()->getDisciplina();
			$produtoContratado = new ProdutoContratado($analiseSolicitacao, $produto, $disciplina);
			$this->addProdutoContratado($produtoContratado, $servicoFaseProduto);
		}
	}
	
	private function addProdutoContratado ( ProdutoContratado $produtoContratado, ServicoFaseProduto $servicoFaseProduto ){
		if (empty($this->colecaoProdutosContratados)){
			$this->associaServicoFaseProdutoAoProdutoContratado($produtoContratado, $servicoFaseProduto);
		} else {
			foreach ( $this->colecaoProdutosContratados as $chave => $pContratado ){
				if ( $produtoContratado->equals($pContratado) ){
					$pContratado->appendServicoFaseProduto($servicoFaseProduto);
				} else {
					$this->associaServicoFaseProdutoAoProdutoContratado($produtoContratado, $servicoFaseProduto);
				}
			}
		}
	}
	
	private function associaServicoFaseProdutoAoProdutoContratado( ProdutoContratado $produtoContratado, ServicoFaseProduto $servicoFaseProduto ){
		$produtoContratado->appendServicoFaseProduto($servicoFaseProduto);
		$this->colecaoProdutosContratados[$produtoContratado->getId()] = $produtoContratado;
	}
	
}