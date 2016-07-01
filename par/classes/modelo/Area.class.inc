<?php
	
class Area extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.area";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "areid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'areid' => null, 
									  	'dimid' => null, 
									  	'aredsc' => null, 
									  	'arecod' => null,
    									'arestatus' => null, 
									  );
									  
	protected $stOrdem = null;
									  
	public function recuperarAreasGuia($dimid)
	{
		$sql = "SELECT DISTINCT 
					ad.areid, 
					ad.arecod, 
					ad.aredsc, 
					ad.dimid
			    FROM {$this->stNomeTabela} ad
			    INNER JOIN par.dimensao d ON d.dimid = ad.dimid 
			    	AND d.dimstatus = 'A'
			    WHERE ad.arestatus = 'A'
			    AND ad.dimid = {$dimid}
			    ORDER BY ad.arecod
			    --limit 1";

		return $this->carregar($sql);
	}
	
	public function recuperarOrdemAreaPorDimid($dimid)
	{
		$sql = "SELECT 
					max(arecod) as ordem 
				FROM {$this->stNomeTabela}
				WHERE dimid = {$dimid}";
				
		$this->stOrdem =  $this->pegaUm($sql)+1;
	}
	
	public function verificaSubitensGuia($areid)
	{
		
		$sql = "SELECT 
					count(areid) 
				FROM par.indicador 
				WHERE areid = {$areid}";
		
		if($this->pegaUm($sql) > 0){
			return false;
		} else {
			return true;
		}
		
	}
	
	public function deletarAreaGuia($areid)
	{
		
		if(!$this->verificaSubitensGuia($areid)){
			
			alert("Não foi possível excluir a área, existem subitens cadastrados.");
			echo "<script>window.location.href = 'par.php?modulo=principal/configuracao/guia&acao=A';</script>";		
			return false;
		}		
		
		$this->areid = $areid;
		$this->excluir();
		$this->commit();			
	}
									  
}									  