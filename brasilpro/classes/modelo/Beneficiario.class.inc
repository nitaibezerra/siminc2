<?php
	
class Beneficiario extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.beneficiario";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "benid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'benid' => null,
									    'bendsc' => null,
									    'benidfnde' => null
									  );
									  
	public function carregaBeneficiarioPorSbaid($sbaid, $anoref)
	{
		$sql = "SELECT 
					ben.benid,
					ben.bendsc,
					ben.benidfnde,
					vlrurbano,
					vlrrural,
					(vlrurbano+vlrrural) as total
				FROM {$this->stNomeTabela} ben
				INNER JOIN par.subacaobeneficiario sbe ON sbe.benid = ben.benid 
				WHERE sbe.sbaid = {$sbaid}
				AND sbe.sabano = '{$anoref}'";
		
		return $this->carregar($sql);
	}
									  
}									  