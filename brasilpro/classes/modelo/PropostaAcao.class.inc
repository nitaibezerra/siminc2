<?php
	
class PropostaAcao extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.propostaacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ppaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ppaid' => null, 
									  	'crtid' => null, 
									  	'ppadsc' => null,
									  );
									  
	public function recuperarPropostaAcoesCriterioGuia($crtid)
	{
		$sql = "SELECT DISTINCT 
					ppa.ppaid, 
					ppa.crtid, 
					ppa.ppadsc 
			    FROM {$this->stNomeTabela} ppa			    
			    WHERE ppa.crtid = {$crtid}
			    ORDER BY ppa.ppadsc";

		return $this->carregar($sql);
	}
	
	public function recuperarPropostaAcoesGuia($indid)
	{
		$sql = "SELECT DISTINCT 
					ppa.ppaid, 
					ppa.ppadsc
			    FROM {$this->stNomeTabela} ppa
			    INNER JOIN par.criterio cr ON ppa.crtid = cr.crtid			    
			    WHERE cr.indid = {$indid}
			    ORDER BY ppa.ppadsc";

		return $this->carregar($sql);
	}
	
	public function recuperarPropostaAcoesPorCriterio($crtid)
	{
		if($crtid){
			$sql = "SELECT  ppa.ppaid,
							ppa.ppadsc
					FROM {$this->stNomeTabela} ppa
						INNER JOIN par.criterio crt ON crt.crtid = ppa.crtid
					WHERE crt.crtid = $crtid";
			
//			ver($sql, d);
		}

		return $this->pegaLinha($sql);
	}
	
}									  