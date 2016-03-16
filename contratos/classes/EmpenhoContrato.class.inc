<?php
	
class EmpenhoContrato extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "contratos.empenhocontrato";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "epcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'epcid' => null, 
									  	'epcnumero' => null, 
									  	'epcdescricao' => null, 
									  	'epcstatus' => null, 
									  );
    
    public function getSqlComboEmpenhos($ctrid)
    {
    	$sql = "SELECT
    				e.epsid as codigo,
    				s.nu_empenho as descricao
    			FROM
    				contratos.empenhovinculocontrato e
    			JOIN contratos.empenho_siafi s ON s.epsid = e.epsid
    			WHERE
    				ctrid = $ctrid
    			ORDER BY
    				s.nu_empenho";
    	return $sql;
    }
    
    public function getSqlComboOrdemPagamento($epcid)
    {
    	$sql = "select
    				orbid as codigo,
    				orbnumero as descricao
    			from
    				contratos.ordembancaria
    			where
    				orbstatus = 'A'
    			and
    				epcid = $epcid
    			order by
    				orbnumero";
    	return $sql;
    }
}