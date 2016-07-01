<?php
	
class TecnologiaMi extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tecnologiami";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tmiid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tmiid' => null, 
									  	'tminome' => null, 
									  	'tmistatus' => null, 
									  );

	public function carregaPorObrid( $obrid ){
		$sql = "SELECT 
					tm.*
				FROM
					obras2.obras o
				JOIN entidade.endereco e ON e.endid = o.endid
				JOIN obras2.empresami_uf eu ON eu.estuf = e.estuf AND
											   eu.eufstatus = 'A'
				JOIN obras2.tecnologiami tm ON tm.tmiid = eu.tmiid AND
											   tm.tmistatus = 'A'
				WHERE
					o.obrid = {$obrid}";
		$dados = $this->pegaLinha( $sql );
		
		$this->popularDadosObjeto( $dados );
		
		return ($dados ? $dados : array());
	}
    
    public function listaCombo() {

        $sql = "SELECT 
            tmi.tmiid AS codigo, 
            tmi.tminome AS descricao 
            FROM obras2.tecnologiami tmi 
            WHERE tmi.tmistatus = 'A'";
        $dados = $this->carregar($sql);
        return ($dados ? $dados : array());
    }
}