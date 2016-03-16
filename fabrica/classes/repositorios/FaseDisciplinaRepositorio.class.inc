<?php 
class FaseDisciplinaRepositorio extends Modelo {
	
	private $disciplinaRepositorio;
	private $faseRepositorio;
	
	public function __construct(){
		parent::__construct();
		$this->disciplinaRepositorio = new DisciplinaRepositorio();
		$this->faseRepositorio = new FaseRepositorio();
	}
	
	public function recuperePorId($idFaseDisciplina){
		$sql = "select fsdid, fasid, dspid, fadvalor, fsddsc from fabrica.fasedisciplina where fsdid = $idFaseDisciplina";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	public function recuperePorFaseDisciplina( $idFase, $idDisciplina ){
		$sql = "select fsdid, fasid, dspid, fadvalor, fsddsc from fabrica.fasedisciplina where fasid = $idFase AND dspid = $idDisciplina";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	function recuperePorFase( $idFase ){
		$sql = "select fsdid, fasid, dspid, fadvalor, fsddsc from fabrica.fasedisciplina where fasid = $idFase";
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	function recuperePorDisciplina( $idDisciplina ){
		$sql = "select fsdid, fasid, dspid, fadvalor, fsddsc from fabrica.fasedisciplina where dspid = $idDisciplina";
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	private function montaObjeto($linha){
		$faseDisciplina = new FaseDisciplina();
		$faseDisciplina->setId($linha['fsdid']);
		$fase = $this->faseRepositorio->recuperePorId($linha['fasid']);
		$faseDisciplina->setFase($fase);
		$disciplina = $this->disciplinaRepositorio->recuperePorId($linha['dspid']);
		$faseDisciplina->setDisciplina($disciplina);
		$faseDisciplina->setPorcentagemPontoFuncao($linha['fadvalor']);
		$faseDisciplina->setDescricao($linha['fsddsc']);
		return $faseDisciplina;
	}
}
