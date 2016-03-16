<?php
	
class Acao extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.acao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "aciid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'aciid' => null, 
									  	'ppaid' => null, 
									  	'ptoid' => null, 
									  	'acidsc' => null,
    									'acinomeresponsavel' => null,
    									'acicargoresponsavel' => null,
    									'acidemandapotencial' => null,
    									'aciresultadoesperado' => null,
    									'acidata' => null,
    									'acistatus' => null,
    									'usucpf' => null, 
									  );
									  
	public function deletaPorPtoid($ptoid)
	{
		if( $_SESSION['par']['boExcluiSubacao'] == 'S' ){
			$this->executar("DELETE FROM par.acao WHERE ptoid = {$ptoid}");
		} else {
			$this->executar("UPDATE par.acao SET acistatus = 'I' WHERE aciid in(select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid})");
		}	
	}
	
	public function recuperaAcaoPorIndicador($indid,$inuid = null )
	{
		$inuid = !$inuid ? $_SESSION['par']['inuid'] : $inuid;
		
		$sql = "select
					aca.aciid,
					aca.acidsc
				from 
					par.acao aca
				inner join
					par.pontuacao pto ON pto.ptoid = aca.ptoid and ptostatus = 'A'
				inner join
					par.criterio crt ON crt.crtid = pto.crtid AND c.crtstatus = 'A'
				where
					aca.acistatus = 'A' AND
					pto.inuid = $inuid
				and
					crt.indid = $indid";
		
		return $this->carregar($sql);		
	}
									  
}									  