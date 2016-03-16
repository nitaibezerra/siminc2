<?php 
class FaseDisciplinaProdutoRepositorio extends Modelo {
	
	private $faseDisciplinaRepositorio;
	private $produtoRepositorio;
	
	public function __construct(){
		parent::__construct();
		$this->faseDisciplinaRepositorio = new FaseDisciplinaRepositorio();
		$this->produtoRepositorio = new ProdutoRepositorio();
	}
	
	public function recuperePorId($idFaseDisciplinaProduto){
		$sql = "select fdpid, fsdid, prdid, fdpstatus, contratada, misto from fabrica.fasedisciplinaproduto where fdpid = $idFaseDisciplinaProduto";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	public function recuperePorFaseDisciplinaProduto( $idFaseDisciplina, $idProduto ){
		$sql = "select fdpid, fsdid, prdid, fdpstatus, contratada, misto 
				from fabrica.fasedisciplinaproduto
				where fdpstatus = 'A' AND fsdid = $idFaseDisciplina AND prdid = $idProduto ";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	public function recuperePorFaseDisciplina( $idFaseDisciplina ){
		$sql = "select fdpid, fsdid, prdid, fdpstatus, contratada, misto from fabrica.fasedisciplinaproduto
				where fdpstatus = 'A' AND fsdid = $idFaseDisciplinaProduto";
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	public function recuperePorProduto ( $idProduto ){
		$sql = "select fdpid, fsdid, prdid, fdpstatus, contratada, misto from fabrica.fasedisciplinaproduto
		where fdpstatus = 'A' AND prdid = $idProduto";
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	private function montaObjeto($linha){
		$faseDisciplinaProduto = new FaseDisciplinaProduto();
		
		$faseDisciplinaProduto->setId($linha['fdpid']);
		$faseDisciplinaProduto->setStatus($linha['fdpstatus']);
		$faseDisciplinaProduto->setContratada($linha['contratada']);
		$faseDisciplinaProduto->setMisto($linha['misto']);		
		
		$faseDisciplina = $this->faseDisciplinaRepositorio->recuperePorId($linha['fsdid']);
		$faseDisciplinaProduto->setFaseDisciplina($faseDisciplina);
		
		$produto = $this->produtoRepositorio->recuperePorId($linha['prdid']);
		$faseDisciplinaProduto->setProduto($produto);
		
		return $faseDisciplinaProduto;
	}
	
}