<?php
//123
class ContaCorrente extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "emenda.contacorrente";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "cocid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'cocid' => null, 
									  	'ptrid' => null, 
									  	'cocnu_seq_mov_conta' => null, 
									  	'cocseq_conta' => null, 
									  	'cocds_razao_social' => null, 
									  	'cocandamento' => null, 
									  	'cocnu_banco' => null, 
									  	'cocnu_agencia' => null, 
									  	'cocnu_conta_corrente' => null, 
									  	'coccnpj' => null, 
									  	'cocsituacao_conta' => null,
									  	'cocdatainclusao' => null,
									  	'cocdsproblema' => null,
									  );
									  
	public function carregaContaCorrentePorPtrid($ptrid){
		$sql = "SELECT 
				    cc.cocid,
				    cc.ptrid,     
				    cc.cocnu_seq_mov_conta,
				    cc.cocseq_conta,
				    cc.cocds_razao_social,
				    cc.cocandamento,
				    cc.cocnu_banco,
				    cc.cocnu_agencia,
				    cc.cocnu_conta_corrente,
				    cc.coccnpj,
				    cc.cocsituacao_conta,
				    cc.cocdatainclusao,
				    pt.ptragenciabancaria
				FROM emenda.contacorrente cc
					inner join emenda.planotrabalho pt on cc.ptrid = pt.ptrid
				where cc.ptrid = $ptrid";
		$arDados = $this->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();
		return $arDados;
	}
}