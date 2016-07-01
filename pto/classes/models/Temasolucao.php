<?php

class Model_Temasolucao extends Abstract_Model
{
    protected $_schema = 'pto';
    protected $_name = 'temasolucao';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['tesid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['solid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['temid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
    }

    public function salvarTema($arrayTemas, $idSolucao)
    {
        if (is_array($arrayTemas)) {
            $this->deleteAllByValues(array('solid' => $idSolucao));
            foreach ($arrayTemas as $temaID) {
                $this->setAttributeValue('solid', $idSolucao);
                $this->setAttributeValue('temid', $temaID);
                $id = $this->save();
                if ($id == false) {
                    throw new Exception('Erro ao inserir o Tema.');
                }
            }
        } else {
            $this->error[] = array("name" => 'temid', "msg" => ('Não pode estar vazio'));
            throw new Exception('Nenhum Tema Selecionado!');
        }
    }
}
