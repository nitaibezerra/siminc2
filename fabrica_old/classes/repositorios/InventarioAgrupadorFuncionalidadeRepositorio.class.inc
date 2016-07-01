<?php
class InventarioAgrupadorFuncionalidadeRepositorio extends Modelo {
	
	public function __contruct(){
		parent::__contruct();		
	}
	
	public function salvar( $nomeInventarioAgrupadorFuncionalidade ){
		$sql = "INSERT INTO inventario.tb_simec_agrupador_funcionalidade (no_agrupador, dt_cadastro)
				VALUE ( $nomeInventarioAgrupadorFuncionalidade , now()) returning co_agrupador";
		return $this->pegaUm($sql);
	}
	
	public function recupereTodos(){
		$sql = "SELECT * FROM inventario.tb_simec_agrupador_funcionalidade";
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	private function montaObjeto( $linha ){
		$iaf = new InventarioAgrupadorFuncionalidade();
		$iaf->setId($id);
		$iaf->setNome($nome);
		$iaf->setDataCadastro(new DateTime($dataCadastro));
		return $iaf;
	}
	
}