<?php

class Model_Identificacaogrupo extends Abstract_Model
{

    protected $_schema = 'pet';
    protected $_name = 'identificacaogrupo';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['idgid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['grpid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['descricaoprojeto'] = array('value' => '', 'type' => 'text', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['descricaotrajetoria'] = array('value' => '', 'type' => 'text', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
        $this->entity['descricaointeracaocolegiado'] = array('value' => '', 'type' => 'text', 'is_null' => 'NO', 'maximum' => '', 'contraint' => '');
    }

    public function getIdentificacaoGrupoPorIdGrupo($grpid)
    {
        $dados = $this->getAllByValues(array('grpid' => $grpid));
        if ($dados)
            $this->populateEntity($dados[0]);
    }
}
