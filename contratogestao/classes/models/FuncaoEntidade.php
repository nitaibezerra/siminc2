<?php

class Model_FuncaoEntidade extends Abstract_Model {

    protected $_schema = 'entidade';
    protected $_name = 'funcaoentidade';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['fueid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['funid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'funcaoentidade_funid_key');
        $this->entity['funid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['entid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'funcaoentidade_funid_key');
        $this->entity['entid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['fuedata'] = array('value' => '', 'type' => 'timestamp without time zone', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['fuestatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
    }
   
    public function getEntidadeFuncaoContratoGestao($etapa) {
        $funcao = '';
        switch ($etapa) {
            case Model_PerfilUsuario::EXECUTOR :
                $funcao = FUNCAO_EXECUTOR;
                break;
            case Model_PerfilUsuario::VALIDADOR:
                $funcao = FUNCAO_VALIDADOR;
                break;
            case Model_PerfilUsuario::CERTIFICADOR:
                $funcao = FUNCAO_CERTIFICADOR;
                break;
        }
        return $funcao;
    }
}
