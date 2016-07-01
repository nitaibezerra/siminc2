<?php
	
class ObraAditivoExclusao extends Modelo
{	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'obras2.obraaditivoexclusao';	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('oaeid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'oaeid' => null, 
        'obrid' => null, 
        'docid' => null, 
        'oaejustificativa' => null, 
        'oaestatus' => null, 
    );
    
    /**
     * 
     * @param type $obrid
     * @return array
     */
    public function pegaLinhaPorObrid($obrid)
    {
        $strSQL = "SELECT * FROM {$this->stNomeTabela} WHERE obrid = {$obrid} ORDER BY oaeid DESC ";
        $dados = $this->pegaLinha($strSQL);
        return ($dados) ? $dados : array();
    }
    
    /**
     * 
     * @param type $obrid
     * @return type
     */
    public function possuiDocid($obrid)
    {
        $strSQL = "SELECT * FROM {$this->stNomeTabela} WHERE obrid = {$obrid}";
        $result = $this->pegaLinha($strSQL);
        return (!$result) ? false : $result['docid'];
    }
}