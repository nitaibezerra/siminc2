<?php
	
class ArquivoSupervisaoFormularioTecnologiaMi extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.arquivosupervisaoformulariotecnologiami";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "asfid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'asfid' => null, 
									  	'arqid' => null, 
									  	'ftiid' => null, 
									  	'supid' => null, 
									  	'asfstatus' => null, 
									  );
									  
	public function carregaPorArqid( $arqid ){
		$sql = "SELECT 
					*
				FROM
					obras2.arquivosupervisaoformulariotecnologiami
				WHERE
					arqid = {$arqid}";
		$dados = $this->pegaLinha( $sql );
		
		$this->popularDadosObjeto( $dados );
		
		return ($dados ? $dados : array());
	}									  
}