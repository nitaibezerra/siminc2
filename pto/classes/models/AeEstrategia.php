<?php

class Model_AeEstrategia extends Abstract_Model {

    protected $_schema = 'pde';
    protected $_name = 'ae_estrategia';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['estid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label'=>'ID Estratégia');
        $this->entity['estnome'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '500', 'contraint' => '', 'label'=>'Estratégia');
        $this->entity['metid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk', 'label'=>'Dispositivos PNE');
    }
    
    public function getOptionsEstrategia($where = null, $dados = array() ) {
        if( !empty($dados) && is_null($where) && !empty($dados['mpneid'])){
            $strTemid = implode(',', $dados['mpneid']);
            $where = " AND metid IN ( {$strTemid} ) ";
        }
        $estrategia = $this->getObjetivosEstrategicos($where);
        $estrategia = ($estrategia ? $estrategia : array());
        return $this->getOptions($estrategia, array(), 'estid');
    }

    public function getObjetivosEstrategicos($where = null) {
        $sql = "SELECT estid as codigo, estnome as descricao FROM pde.ae_estrategia  WHERE 1=1 {$where} ORDER BY estordem";
        return $this->_db->carregar($sql);
    }

}
