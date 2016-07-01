<?php
	
class SituacaoAtividade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.situacaoatividade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "staid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'staid' => null, 
									  	'stadesc' => null, 
									  	'stadtinclusao' => null, 
									  	'staordem' => null, 
									  	'stastatus' => null, 
									  );

	public function situacaoPossivelSql( $param = array() ){
		$arWhere   = array();
		$arWhere[] = "staid <> 99"; 
		$arWhere[] = "stastatus = 'A'";
		
		if ( !empty($param['not(staid)']) ){
			$param['not(staid)'] = (array) $param['not(staid)'];
			$arWhere[] 			 = "staid NOT IN(" . implode(", ", $param['not(staid)']) . ")";
		}
		
		$sql = "SELECT
        			staid as codigo,
                    stadesc as descricao
				FROM
                	obras2.situacaoatividade
				WHERE
                	" . (implode(" AND ", $arWhere)) . "
				ORDER BY
                	staordem";
	
	    return $sql;
	}
}