<?php 

class ProdutoRepositorio extends Modelo {
	
	private $disciplinaRepositorio;
	
	public function __construct(){
		parent::__construct();
		$this->disciplinaRepositorio = new DisciplinaRepositorio();
	} 

	public function recupereTodosPorDisciplina($idDisciplina){
		$semNecessidade = Produto::SERVICO_SEM_NECESSIDADE_DE_ARTEFATO;
		$sql = "select * from fabrica.produto where prdstatus = 'A' and prdid <> $semNecessidade AND dspid = $idDisciplina";

		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	public function recuperePorId($idProduto){
		$sql = "select * from fabrica.produto where prdstatus = 'A' and prdid = $idProduto";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	private function montaObjeto($linha){
		$produto = new Produto();
		$produto->setId($linha['prdid']);
		$disciplina = $this->disciplinaRepositorio->recuperePorId($linha['dspid']);
		$produto->setDisciplina($disciplina);
		$produto->setDescricao($linha['prddsc']);
		$produto->setStatus($linha['prdstatus']);
		return $produto;
	}
}