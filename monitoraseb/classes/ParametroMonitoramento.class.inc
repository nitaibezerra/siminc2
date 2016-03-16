<?php

class ParametroMonitoramento extends Modelo{
	
	/**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "monitoraseb.parametro";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "parid" );
     
    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos = array('pardscmonitoramento' => null);
	
	protected $arrPesquisa = null;
	
	function recuperaIdRegistro(){ 
		$sql = "SELECT parid FROM monitoraseb.parametro";
		return $this->pegaUm($sql);
	}
	
	public function salvarMonitoramento($parid,$pardscmonitoramento){
		if($this->validaTextoMaximo($pardscmonitoramento)){
			$sql = "";
			if($parid!=null && $parid!=""){
				$sql.= "update monitoraseb.parametro set pardscmonitoramento='$pardscmonitoramento' where parid=$parid";
			}else{
				$sql.= "insert into monitoraseb.parametro (parid, pardscmonitoramento) values (1,'$pardscmonitoramento') ";
			}
			
			$this->executar($sql);
			
			return "Operação Realizada com Sucesso.";
		}else{
			return "O campo tamanho máximo do campo e de 3000 caracteres.";
		}	
			
	}
	
	private function validaTextoMaximo($pardscmonitoramento){
		return (strlen($pardscmonitoramento)<=3000)? true : false;
	}
}
