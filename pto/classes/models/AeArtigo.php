<?php

class Model_AeArtigo extends Abstract_Model {

    protected $_schema = 'pde';
    protected $_name = 'ae_artigo';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['artid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['artordem'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
        $this->entity['artnome'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '500', 'contraint' => '');
        $this->entity['temid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['sitid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['artprazo'] = array('value' => '', 'type' => 'date', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
    }
    
    public function getOptionsArtigo($where = null, $dados = array()) {
        if( !empty($dados) && is_null($where) && !empty($dados['temid'])){
            $strTemid = implode(',', $dados['temid']);
            $where = " AND temid IN ( {$strTemid} ) ";
        }
        $artigo = $this->getArtigos($where);
        $artigo = ($artigo ? $artigo : array());
        return $this->getOptions($artigo, array(), 'artid');
    }

    public function getArtigos($where = null) {
        $sql = "SELECT artid as codigo, artnome as descricao FROM pde.ae_artigo  WHERE 1=1 {$where} ORDER BY artordem";
        return $this->_db->carregar($sql);
    }

}
