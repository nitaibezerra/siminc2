<?php
	
class SubacaoDetalhe extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.subacaodetalhe";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sbdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
										'sbdid' => null,								  	
									    'sbaid' => null,
									  	'sbdparecer' => null,
									  	'sbdparecerdemerito' => null,
									  	'sbdquantidade' => null,
									  	'sbdano' => null,
									  	'sbdinicio' => null,
									  	'sbdfim' => null,
									  	'ssuid' => null, 
									  	'sbdanotermino' => null, 
									  	'sbddetalhamento' => null, 
									  	'sbdnaturezadespesa' => null,
    									'sbdplanointerno' => null,
    									'sbdptres' => null,
    									'sbdrepitemvencido' => null,
    									'sbdreformulacaoempenho' => null
									  );

	public function retornaDetalheSubacao($ano,$sbaid)
	{
		$sql = "select sbdid from par.subacaodetalhe where sbdano = '$ano' and sbaid = $sbaid";
		return $this->pegaUm($sql);
	}

	public function retornaDetalheSubacaoTotal($sbaid)
	{
		$sql = "select sbdid from par.subacaodetalhe where sbaid = $sbaid";
		return $this->carregarColuna( $sql );
	}
	
	public function deletaPorPtoid($ptoid)
	{
		$this->executar("delete FROM par.subacaodetalhe WHERE sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND aciid in(select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid}))");
	}
									  
	public function deletaPorSbaid($sbaid, $ano = array())
	{
		$anos = implode( $ano, "," );
		$this->executar("delete FROM par.subacaodetalhe WHERE sbaid = {$sbaid} AND sbdano IN ({$anos})");
	}
}									  