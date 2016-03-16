<?php

class Model_Satisfacao extends Abstract_Model {

    protected $_schema = 'contratogestao';
    protected $_name = 'satisfacao';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['satid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label' => 'ID');
        $this->entity['satdsc'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '100', 'contraint' => '', 'label' => 'Descrição');
        $this->entity['satobs'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '100', 'contraint' => '', 'label' => 'Observação');
    }

    public function getSatisfacaoById($satid) {
        $sql = "SELECT satdsc AS descricao FROM contratogestao.satisfacao WHERE satid = {$satid}; ";
        $dados = $this->_db->carregar($sql);
        if ($dados) {
            return $dados[0]['descricao'];
        } else {
            return '';
        }
    }

    public function getOptionsSatid() {
        $satisfacaos = $this->getAll();
        $option = "<option value=''> Selecione </option>";
        foreach ($satisfacaos as $satisfacao) {
            $selected = ( (int)$satisfacao['satid'] === (int)$this->getAttributeValue('satid') ? 'selected=selected' : '');
            $option .= "<option value='{$satisfacao['satid']}' {$selected} >{$satisfacao['satdsc']}</option>";
        }
        return $option;
    }

}
