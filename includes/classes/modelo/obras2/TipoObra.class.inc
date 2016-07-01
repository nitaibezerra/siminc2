<?php
	
class TipoObra extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tipoobra";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tobid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tobid' => null, 
									  	'tobdesc' => null, 
									  	'tobstatus' => null, 
									  	'tobdtinclusao' => null, 
									  	'cpiid' => null, 
									  );

	public function listaCombo(){
		$sql = "SELECT
					tobid AS codigo,
					tobdesc AS descricao
				FROM
					obras2.tipoobra
				WHERE
					tobstatus = 'A'";

		$dados = $this->carregar($sql);
		
		return (is_array($dados) ? $dados : array());
	}
    public function listaComboSql(){
        $sql = "SELECT
					tobid AS codigo,
					tobdesc AS descricao
				FROM
					obras2.tipoobra
				WHERE
					tobstatus = 'A'";

        return $sql;
    }

									  
}