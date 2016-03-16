<?php
	
class Indicador extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.indicador";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "indid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'indid' => null, 
									  	'areid' => null, 
									  	'inddsc' => null, 
									  	'indstatus' => null,
    									'indcod' => null, 
									  );
									  
	protected $stOrdem = null;

	public function recuperarIndicadoresGuia($areid)
	{
		$sql = "SELECT distinct
				    ind.indid,
				    ind.indcod,
				    ind.inddsc,
				    ind.areid				    
				FROM {$this->stNomeTabela} ind
				INNER JOIN par.area ard on ind.areid = ard.areid
				INNER JOIN par.dimensao dim on dim.dimid = ard.dimid 
				    	AND dim.dimstatus = 'A'
				WHERE ind.indstatus = 'A'
				AND ind.areid = {$areid}
				ORDER BY ind.indcod
				--limit 1";
		
		return $this->carregar($sql);
	}
	
	public function recuperarOrdemIndicadorPorAreid($areid)
	{
		$sql = "SELECT 
					max(indcod) as ordem 
				FROM {$this->stNomeTabela}
				WHERE areid = {$areid}";
				
		$this->stOrdem = $this->pegaUm($sql)+1;
	}
	
	public function verificaSubitensGuia($indid)
	{
		
		$sql = "SELECT 
					count(crtid) 
				FROM par.criterio 
				WHERE indid = {$indid} AND c.crtstatus = 'A'";
				
		$nrCriterio = $this->pegaUm($sql);
		
		if($nrCriterio > 0){
			return false;
		} else {
			return true;
		}
		
	}
	
	public function deletarIndicadorGuia($indid)
	{
		
		if(!$this->verificaSubitensGuia($indid)){
			
			alert("Não foi possível excluir o indicador, existem subitens cadastrados.");
			echo "<script>window.location.href = 'par.php?modulo=principal/configuracao/guia&acao=A';</script>";		
			return false;
		}		
		
		$this->indid = $indid;
		$this->excluir();
		$this->commit();			
	}
}									  