<?php 
class RequisitanteRepositorio extends Modelo{
	
	function recuperaTodos(){
		$sql = "SELECT distinct u.usucpf as cpf, u.usunome as nome
			FROM seguranca.usuario u
			INNER JOIN seguranca.perfilusuario o ON u.usucpf=o.usucpf
			INNER JOIN seguranca.perfil p ON p.pflcod=o.pflcod
			WHERE sisid=87 order by 2";
		
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	function recuperePorId(){
		$sql = "SELECT distinct u.usucpf as cpf, u.usunome as nome
			FROM seguranca.usuario u
			INNER JOIN seguranca.perfilusuario o ON u.usucpf=o.usucpf
			INNER JOIN seguranca.perfil p ON p.pflcod=o.pflcod
			WHERE sisid=87 order by 2";
		try {
			$linha = parent::pegaLinha($sql);
			$requisitante = $this->montaObjeto($linha);
			return $requisitante;
		} catch (Exception $e){
			return null;
		}
	
	}
	
	function recuperePeloCpf($cpfRequisitante){
		$sql = "SELECT distinct u.usucpf as cpf, u.usunome as nome
			FROM seguranca.usuario u
			INNER JOIN seguranca.perfilusuario o ON u.usucpf=o.usucpf
			INNER JOIN seguranca.perfil p ON p.pflcod=o.pflcod
			WHERE sisid=87 AND u.usucpf = '$cpfRequisitante' order by 2";
		try {
			$linha = parent::pegaLinha($sql);
			$requisitante = $this->montaObjeto($linha);
			return $requisitante;
		} catch (Exception $e){
			return null;
		}
	
	}

	private function montaObjeto($linha){
		$requisitante = new Requisitante();
		$requisitante->setCpf($linha['cpf']);
		$requisitante->setNome($linha['nome']);
		return $requisitante;
	}


}
