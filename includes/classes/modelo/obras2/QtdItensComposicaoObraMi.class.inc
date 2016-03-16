<?php
    
class QtdItensComposicaoObraMi extends Modelo{
    
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.qtditenscomposicaoobrami";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "qioid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'qioid' => null,
                                        'qiostatus' => null,
                                        'qioquantidade' => null,
                                        'obrid' => null,
                                        'icmid' => null,
                                      );

    public function pegaPorObridEIcmid($obrid, $icmid){
        $sql = "select * from obras2.qtditenscomposicaoobrami where obrid = $obrid AND icmid = $icmid";

        return $this->pegaLinha($sql);
    }


    public function clearAll($obrid){
        $sql = "delete from obras2.qtditenscomposicaoobrami where obrid = {$obrid}";
        $this->executar($sql);
        $this->commit();
    }

}