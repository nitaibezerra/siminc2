<?php
class Model_Curso extends Abstract_Model
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
    protected $_name = 'curso';

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
        
        $this->entity['curid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['nome'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '500' , 'contraint' => '');
    }
}
