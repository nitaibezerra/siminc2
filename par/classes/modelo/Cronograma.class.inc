<?php
	
class Cronograma extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.cronograma";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ppsid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ppsid' => null, 
									  	'ppsdsc' => null, 
									  	'ppsordem' => null,
    									'ppaid' => null,
    									'frmid' => null,
									    'prgid' => null,
									    'indid' => null,
									    'ppsestrategiaimplementacao' => null,
									    'undid' => null,
									    'ppspeso' => null,
    									'foaid' => null,
    									'ppsobra' => null,
    									'ppsptres' => null,
    									'ppsnaturezadespesa' => null,
    									'ppscronograma' => null,
    									'ppsmonitora' => null,
									  );
									  
	public function carregaCronogramaPorSbaid($sbaid, $anoref)
	{
		$sql = "SELECT 
			cro.inicio,
			cro.fim,
			cro.anotermino,
			cro.quantidade
		FROM par.subacaotemporaria sba
		LEFT JOIN par.subacaotemporariacronograma cro ON cro.subid = sba.subid
		WHERE sba.subid = {$sbaid}
		AND cro.anoreferencia = '{$anoref}'";

		return $this->pegaLinha($sql);
	}
									  
}									  