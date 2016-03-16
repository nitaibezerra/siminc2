<?php
	
class NaturezaDespesa extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "public.naturezadespesa";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ndpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'gndcod' => null, 
									  	'ndpano' => null, 
									  	'ndpcod' => null, 
									  	'ndpdsc' => null, 
									  	'edpcod' => null, 
									  	'mapcod' => null, 
									  	'ctecod' => null, 
									  	'sbecod' => null, 
									  	'cagcod' => null, 
									  	'ndpid' => null, 
									  	'ndpstatus' => null, 
									  );
	public function existeCodNatureza($ndpcod)
	{
		return $this->pegaUm("select ndpid from public.naturezadespesa where ndpstatus = 'A' and ndpcod = '$ndpcod'");	
	}
	
	public function pegaCodNaturezaPorId($ndpid)
	{
		return $this->pegaUm("select ndpcod from public.naturezadespesa where ndpstatus = 'A' and ndpid = '$ndpid'");	
	}
}