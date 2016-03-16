<?php
	
class Orgao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.orgao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "orgid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'orgid' => null, 
									  	'orgdesc' => null, 
									  	'orgstatus' => null, 
									  	'orgdtinclusao' => null, 
									  	'orgtipo' => null, 
									  	'orgcodigo' => null, 
									  );

	public function listaCombo(){
		$sql = "SELECT
					orgid AS codigo,
					orgdesc AS descricao
				FROM
					obras2.orgao
				WHERE
					orgstatus = 'A'
				ORDER BY
					orgdesc;";

		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}
									  
	public function pegaDescricao( $orgid ){
		$sql = "SELECT
					orgdesc
				FROM
					obras2.orgao
				WHERE
					orgid = {$orgid}";
		
		return $this->pegaUm( $sql );
	}
}