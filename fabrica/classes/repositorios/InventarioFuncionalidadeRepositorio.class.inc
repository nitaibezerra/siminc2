<?php
class InventarioFuncionalidadeRepositorio extends Modelo{

	public function __construct(){
		parent::__construct();
	}
	
	public function recupereTodosPeloIdSistema( $idSistema ){
		$sql = "SELECT * FROM inventario.tb_simec_funcionalidade WHERE sisid = $idSistema";
		$resultSet = $this->carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	private function montaObjeto( $linha ){
		$inventarioFuncionalidade = new InventarioFuncionalidade();
		$inventarioFuncionalidade->setComplexidade($complexidade);
		$inventarioFuncionalidade->setDescricao($descricao);
		$inventarioFuncionalidade->setDescricaoArlRlr($descricaoArlRlr);
		$inventarioFuncionalidade->setDescricaoTd($descricaoTd);
		$inventarioFuncionalidade->setId($id);
		$inventarioFuncionalidade->setInventarioAgrupadorFuncionalidade(new InventarioAgrupadorFuncionalidade($inventarioAgrupadorFuncionalidade));
		$inventarioFuncionalidade->setInventarioTipoFuncionalidade(new InventarioTipoFuncionalidade($inventarioTipoFuncionalidade));
		$inventarioFuncionalidade->setQtdeArlRlr($qtdeArlRlr);
		$inventarioFuncionalidade->setQtdePF($qtdePF);
		$inventarioFuncionalidade->setQtdeTd($qtdeTd);
		return $inventarioFuncionalidade;
	}
	
}