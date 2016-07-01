<?php
	
class ObraRecusaOS extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.obrarecusaos";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "oroid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'oroid' => null, 
									  	'osmid' => null, 
									  	'troid' => null, 
									  	'usucpf' => null, 
									  	'orodtcadastro' => null, 
									  );

    public function pegaMotivo($osmid){
        $sql = "SELECT * FROM obras2.obrarecusaos ros
                JOIN obras2.tiporecusaos tos ON tos.troid = ros.troid
                WHERE ros.osmid = $osmid";

        $result = $this->pegaLinha($sql);

        return ($result) ? $result : array();
    }
}