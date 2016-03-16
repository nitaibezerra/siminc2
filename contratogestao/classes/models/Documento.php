<?php

class Model_Documento extends Abstract_Model {

    protected $_schema = 'workflow';
    protected $_name = 'documento';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['docid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['tpdid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['esdid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['docdsc'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '500', 'contraint' => '');
        $this->entity['unicod'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '5', 'contraint' => '');
        $this->entity['docdatainclusao'] = array('value' => '', 'type' => 'timestamp without time zone', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['hstid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
    }

    public function getEstadoDocumentoById($docid) {
        $sql = "SELECT esd.esddsc
                FROM workflow.documento doc
                JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE doc.docid = {$docid}; ";

        $dados = $this->_db->carregar($sql);
        if ($dados) {
            return $dados[0]['esddsc'];
        } else {
            return '';
        }
    }
}
?>