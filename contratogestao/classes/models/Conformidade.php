<?php

class Model_Conformidade extends Abstract_Model {

    protected $_schema = 'contratogestao';
    protected $_name = 'conformidade';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['cofid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label' => 'ID');
        $this->entity['cofdsc'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '100', 'contraint' => '', 'label' => 'Descrição');
        $this->entity['cofpeso'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '', 'label' => 'Peso');
    }

    public function getConformidadeById($cofid) {
        $sql = "SELECT cofdsc || ' ('||cofpeso||')' AS descricao FROM contratogestao.conformidade WHERE cofid = {$cofid}; ";
        $dados = $this->_db->carregar($sql);
        if ($dados) {
            return $dados[0]['descricao'];
        } else {
            return '';
        }
    }

    public function getOptionsConfid() {
        $conformidades = $this->getAll();
        $option = "<option value=''> Selecione </option>";
        foreach ($conformidades as $conformidade) {
            $selected = ( (int)$conformidade['cofid'] === (int)$this->getAttributeValue('cofid') ? 'selected=selected' : '');
            $option .= "<option value='{$conformidade['cofid']}' {$selected} >{$conformidade['cofdsc']}</option>";
        }
        return $option;
    }

}
