<?php
	
class ParametroAgenda extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.parametroagenda";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "praid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'praid' => null, 
									  	'agdid' => null, 
									  	'pradsccampo' => null, 
									  	'prastatus' => null 
									  );

	public function carregaDados( $agdid ){
		
		$sql = "SELECT
					pradsccampo
				FROM
					agenda.parametroagenda pa
				WHERE
					agdid = {$agdid}";
		
		$dados = $this->carregar( $sql );
		
		return ($dados ? $dados : array()); 
	}
	
	public function verificaParametroAgenda($pradsccampo, $array ){
		
		if( in_array( $pradsccampo, $array ) ){
			
			return 'checked=checked';
		}else{
			
			return '';
		}
	}
	
	public function isCampoObrigatorioAgenda($pradsccampo, $array ){
		
		if( in_array( $pradsccampo, $array ) ){
			
			return true;
		}else{
			
			return false;
		}
	}
	
	public function salvar( $agdid ){
		
		$arCampos = array_keys( $_POST );
		if( count($_POST) == 0){
			return;
		}
		$sql = "INSERT INTO agenda.parametroagenda
					(agdid, pradsccampo, prastatus)
    			VALUES ";

		$arrInsert = array();
		
		foreach ($arCampos as $ar ){		
						
			 $arrInsert[] = " ({$agdid}, '{$ar}', 't')";
		}
		$strInsert = implode(",", $arrInsert);
		$this->executar( $sql.$strInsert );
	}									  
									  
	public function deletaTodosPorAgenda( $agdid ){
		
		$sql = "DELETE FROM agenda.parametroagenda WHERE agdid = {$agdid}";
		
		$this->executar( $sql );
	}
}