<?php
class ProcessoDAO{
	
	public function __construct(){
		
	}
	
	public function insereProcessoPAR($arrColunas, $arrCampos){
		global $db;
		
		$colunas 	= implode(', ', $arrColunas);
		$campos 	= implode(', ', $arrCampos);
		$sql = "INSERT INTO execucaofinanceira.empenho($colunas) VALUES ($campos)";
		$db->executar( $sql );
	}
	
	public function insereEmpenhoPAR($arrColunas, $arrCampos){
		global $db;
		
		$colunas 	= implode(', ', $arrColunas);
		$campos 	= implode(', ', $arrCampos);
		$sql = "INSERT INTO execucaofinanceira.empenho($colunas) VALUES ($campos)";
		$db->executar( $sql );
	}
	
	public function alteraEmpenhoPAR($arrColunas, $arrCampos){
		global $db;
		
		$colunas 	= implode(', ', $arrColunas);
		$campos 	= implode(', ', $arrCampos);
		$sql = "INSERT INTO execucaofinanceira.empenho($colunas) VALUES ($campos)";
		$db->executar( $sql );
		
		$sql = "UPDATE $this->stNomeTabela SET $campos WHERE {$this->arChavePrimaria[0]} = $valorCampoChave ";		
		return $this->executar( $sql );
	}
}
?>