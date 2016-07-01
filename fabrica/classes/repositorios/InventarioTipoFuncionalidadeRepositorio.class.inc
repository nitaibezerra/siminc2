<?php
class InventarioTipoFuncionalidadeRepositorio extends Modelo{
	
	public function __construct(){
		parent::__construct();
	}
	
	public function recupereTodos(){
		$sql = "SELECT * FROM inventario.tb_simec_tipo_funcionalidade";
		$resultSet = $this->carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	private function montaObjeto( $linha ){
		$itf = new InventarioTipoFuncionalidade();
		$itf->setId($id);
		$itf->setDescricao($descricao);
		$itf->setDataCadastro(new DateTime($dataCadastro));
		return $itf;
	}
}