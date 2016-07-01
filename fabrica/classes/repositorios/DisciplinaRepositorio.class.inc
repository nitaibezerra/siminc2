<?php 

class DisciplinaRepositorio extends Modelo {
	
	public function recuperarTodos(){
		$sql = "select * from fabrica.disciplina where dspstatus = 'A'";
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	public function recuperePorId($idDisciplina){
		$sql = "select dspid, dspdsc, dspstatus from fabrica.disciplina where dspstatus = 'A' and dspid = $idDisciplina";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	private function montaObjeto($linha){
		$disciplina = new Disciplina();
		$disciplina->setId($linha['dspid']);
		$disciplina->setDescricao($linha['dspdsc']);
		$disciplina->setStatus($linha['dspstatus']);
		return $disciplina;
	}
}
