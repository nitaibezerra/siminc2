<?php

class Model_Tempestividade extends Abstract_Model {

    protected $_schema = 'contratogestao';
    protected $_name = 'tempestividade';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['temid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label' => 'ID');
        $this->entity['temdsc'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '100', 'contraint' => '', 'label' => 'Descrição');
        $this->entity['tempeso'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '', 'label' => 'Peso');
    }

    public function getTempestividadeById($cofid){
        $sql = "SELECT temdsc || ' ('||tempeso||')' AS descricao
                FROM contratogestao.tempestividade
                WHERE temid = {$cofid}; ";

        $dados = $this->_db->carregar($sql);
        if($dados){
            return $dados[0]['descricao'];
        }else{
            return '';
        }
    }
    
    public function getOptionsTempestividade() {
        $tempestividades = $this->getAll();
        $option = "<option value=''> Selecione </option>";
        foreach ($tempestividades as $tempestividade) {
            $selected = ( (int)$tempestividade['temid'] === (int)$this->getAttributeValue('temid') ? 'selected=selected' : '');
            $option .= "<option value='{$tempestividade['temid']}' {$selected} >{$tempestividade['temdsc']}</option>";
        }
        return $option;
    }
}