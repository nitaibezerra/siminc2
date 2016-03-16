<?php
	
class SubacaoBeneficiario extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.subacaobeneficiario";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sabid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sabid' => null, 
									  	'sbaid' => null, 
									  	'benid' => null,
    									'sabqtdurbano' => null,
    									'sabqtdrural' => null,
									    'sabano' => null
									  );

	public function deletaBeneficiarioPorSubacao($sbaid,$sabano)
	{
		$sql = "delete from $this->stNomeTabela where sbaid = $sbaid and sabano = $sabano";
		$this->executar($sql);	
		return $this->commit($sql);
	}

	public function montaLista($sbaid,$sabano = null)
	{
		if( $sabano ){
			$where = "and sabano = ".$sabano;
		}
		$sql = "select
					'<center><img class=\"middle link\" title=\"Excluir Beneficiário\" onclick=\"deletarBeneficiario(this, '|| sab.sabid ||', '|| sab.sabano ||' )\"  src=\"../imagens/excluir.gif\" /></center>' as acao,
					bendsc,
					sum(sabqtdrural) as sabqtdrural,
					sum(sabqtdurbano) as sabqtdurbano
				from
					par.beneficiario ben
				inner join
					par.subacaobeneficiario sab ON sab.benid = ben.benid
				where
					sbaid = $sbaid
				{$where}
				GROUP BY
					bendsc, sab.sabid, sab.sabano
				ORDER BY
					bendsc";

		$cabecalho = array("Ação","Beneficiário","Qtde. Zona Rural","Qtde. Zona Urbana");
		
		return $this->monta_lista_simples($sql,$cabecalho,100,5,"S","100%","N");
	}
	
	public function conta($sbaid,$sabano = null)
	{
		if( $sabano ){
			$where = "and sabano = ".$sabano;
		}
		$sql = "select
					count(bendsc)
				from
					par.beneficiario ben
				inner join
					par.subacaobeneficiario sab ON sab.benid = ben.benid
				where
					sbaid = $sbaid
				{$where}";

		
		return $this->pegaUm($sql);
	}
	
	public function deletaPorPtoid($ptoid)
	{
		$this->executar("delete FROM par.subacaobeneficiario WHERE sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND aciid in(select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid}))");
	}
	
	public function deletaPorSbaid($sbaid, $ano = array())
	{
		$anos = implode( $ano, "," );
		$this->executar("delete FROM par.subacaobeneficiario WHERE sbaid = {$sbaid} AND sabano IN ({$anos})");
	}
									  
}									  