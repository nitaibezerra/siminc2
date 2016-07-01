<?php
class Model_Tutor extends Abstract_Model
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
    protected $_name = 'tutor';

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
        
        $this->entity['tutid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['grpid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['nome'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '500' , 'contraint' => '');
        $this->entity['cpf'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => '');
        $this->entity['datainiciotutoria'] = array( 'value' => '' , 'type' => 'date' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
    }
}
