<?php

class Model_AeIniciativa extends Abstract_Model {

    protected $_schema = 'pde';
    protected $_name = 'ae_iniciativa';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['iniid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label'=>'ID Iniciativas');
        $this->entity['ininome'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '500', 'contraint' => '', 'label'=>'Iniciativas');
    }

    public function getOptionsIniciativa( $where = null, $dados = array() ) {
        if( !empty($dados) && is_null($where) ){

            if(!empty($dados['temid'])){
                $strTemid = implode(',', $dados['temid']);
                $where .= " AND ini.temid IN ( {$strTemid} ) ";
            }
            if(!empty($dados['obeid'])){
                $strObeid = implode(',', $dados['obeid']);
                $where .= " AND ob.obeid IN ( {$strObeid} ) ";
            }
        }
        $iniciativa = $this->getIniciativas($where);
        $iniciativa = ($iniciativa ? $iniciativa : array());
        return $this->getOptions($iniciativa, array(), 'iniid');
    }

    public function getIniciativas( $where = null) {
        $sql = "SELECT ini.iniid as codigo, ini.ininome as descricao
                    FROM pde.ae_iniciativa AS ini
                    LEFT JOIN pde.ae_objetivoestrategicoxiniciativa AS obx ON obx.iniid = ini.iniid
                    LEFT JOIN pde.ae_objetivoestrategicoxiniciativa AS ob ON ob.obeid = obx.obeid
                    WHERE 1=1 {$where}
                    ORDER BY ini.ininome ";

        return $this->_db->carregar($sql);
    }

}
