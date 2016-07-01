<?php
class Model_Vigencia extends Abstract_Model
{
    
    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'pet';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'vigencia';

    /**
     * Entidade
     * @var string / array
     */
    public $entity = array();

    /**
     * Montando a entidade
     * 
     */
    public function __construct($commit = true)
    {
        parent::__construct($commit);
        
        $this->entity['vigid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['disid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['grpid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['datainicioatividade'] = array( 'value' => '' , 'type' => 'date' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['datafimatividade'] = array( 'value' => '' , 'type' => 'date' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['bolsista'] = array( 'value' => '' , 'type' => 'boolean' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
    }
}
