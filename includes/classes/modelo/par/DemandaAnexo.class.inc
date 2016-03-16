<?php
	
class DemandaAnexo extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.demandaanexo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dmdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                            'dmdid' => null, 
                                            'arqid' => null, 
                                            'aqsstatus' => null, 
                                      );
    
    public function carregaPorArqidDmdid($arqid,$dmdid) {
            $sql = "SELECT * FROM {$this->stNomeTabela} where arqid = " . $arqid . " and dmdid = " . $dmdid;
            $dados = $this->pegaLinha($sql);
            $this->popularDadosObjeto($dados);
    }
}