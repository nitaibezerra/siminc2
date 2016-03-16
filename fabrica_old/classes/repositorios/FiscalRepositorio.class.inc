<?php 
class FiscalRepositorio extends ModeloFabrica {
	
	function recupereTodos(){
		$sql = "select distinct
					u.usucpf,
					u.usunome,
					u.usuemail,
					u.usufuncao,
					u.ususexo
				from
					fabrica.fiscalcontrato f
				inner join
					seguranca.usuario u ON u.usucpf = f.usucpf
				order by 2";
		
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	function recuperePorId($idFiscal){
		$sql = "select distinct
					u.usucpf,
					u.usunome,
					u.usuemail,
					u.usufuncao,
					u.ususexo
				from
					fabrica.fiscalcontrato f
				inner join
					seguranca.usuario u ON u.usucpf = f.usucpf
				where u.usucpf = '$idFiscal'";
		
		try {
			$linha = parent::pegaLinha($sql);
			$fiscal= $this->montaObjeto($linha);
			return $fiscal;
		} catch (Exception $e){
			return null;
		}
	}
	
	private function montaObjeto($linha){
		$fiscal = new Fiscal();
		$fiscal->setId($linha['usucpf']);
		$fiscal->setNome($linha['usunome']);
		$fiscal->setEmail($linha['usuemail']);
		$fiscal->setFuncao($linha['usufuncao']);
		$fiscal->setSexo($linha['ususexo']);
		return $fiscal;
	}

}

