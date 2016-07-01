<?php
class Model_IniciativaSolucao extends Abstract_Model
{
    protected $_schema = 'pto';
    protected $_name = 'iniciativasolucao';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);
        
        $this->entity['insid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['solid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['iniid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
    }
    
    public function salvarIniciativaSolucao($arrayIniciativas, $idSolucao)
    {
        if (is_array($arrayIniciativas)) {
            $this->deleteAllByValues(array('solid' => $idSolucao));
            foreach ($arrayIniciativas as $iniciativasID) {
                $this->setAttributeValue('solid', $idSolucao);
                $this->setAttributeValue('iniid', $iniciativasID);
                $id = $this->save();
                if ($id == false) {
                    throw new Exception('Erro ao inserir a Iniciativa.');
                }
            }
        //} else {
        //    $this->error[] = array("name" => 'iniid', "msg" => ('Não pode estar vazio'));
        //    throw new Exception('Nenhuma Iniciativa Selecionada!');
        }
    }
}
