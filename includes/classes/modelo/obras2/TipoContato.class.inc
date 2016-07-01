<?php
	
class TipoContato extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipocontato";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tpcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tpcid' => null, 
									  	'tpcdesc' => null, 
									  	'tpcstatus' => null, 
									  	'tpcdtinclusao' => null, 
									  );
    
	public function listaCombo(){
		$sql = "SELECT 
					tpcid as codigo, 
					tpcdesc as descricao 
				FROM 
					obras2.tipocontato
				WHERE
					tpcstatus = 'A' ";

		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}
}