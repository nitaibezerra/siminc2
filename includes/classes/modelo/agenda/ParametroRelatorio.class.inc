<?php
	
class ParametroRelatorio extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.parametrorelatorio";	

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
    protected $arAtributos     = array(
									  	'parid' => null, 
									  	'agdid' => null, 
									  	'pardsccampo' => null, 
									  	'parstatus' => null 
									  );

	public function carregaDados( $agdid ){
		
		$sql = "SELECT
					pardsccampo
				FROM
					agenda.parametrorelatorio pa
				WHERE
					agdid = {$agdid}";
		
		$dados = $this->carregar( $sql );
		
		return ($dados ? $dados : array()); 
	}
	
	public function verificaParametroAgenda($pardsccampo, $array ){
		
		if( in_array( $pardsccampo, $array ) ){
			
			return 'checked=checked';
		}else{
			
			return '';
		}
	}
	
	public function isCampoObrigatorioAgenda($pardsccampo, $array ){
		
		if( in_array( $pardsccampo, $array ) ){
			
			return true;
		}else{
			
			return false;
		}
	}
	
	public function controleCamposRelatorio( $agdid, $campo ){
		
		$dadosconsulta = $this->carregaDados($agdid);
		
		$dadosparametroagenda = array();
		
		foreach( $dadosconsulta as $arr ){
		
			array_push($dadosparametroagenda, $arr['pardsccampo'] );
		}		

		if( !in_array($campo, $dadosparametroagenda ) ){
			
			$save = $this->salvar($_SESSION['agenda']['agdid'], $campo);
		}else{
			$del  = $this->deletaCampoPorAgenda($_SESSION['agenda']['agdid'], $campo);
		}
		$this->commit();
	}
	
	public function salvar( $agdid, $campo ){
		
		$sql = "INSERT INTO agenda.parametrorelatorio
					(agdid, pardsccampo, parstatus)
    			VALUES 
    			({$agdid}, '{$campo}', 't')";

		$this->executar( $sql );
	}									  
									  
	public function deletaCampoPorAgenda( $agdid, $campo){
		
		$sql = "DELETE FROM agenda.parametrorelatorio WHERE agdid = {$agdid} AND pardsccampo = '$campo'";
		
		$this->executar( $sql );
	}
}