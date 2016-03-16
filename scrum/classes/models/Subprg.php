<?php
class Model_Subprg extends Abstract_Model
{
    
    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'scrum';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'subprg';

    /**
     * Entidade
     * @var string / array
     */
    public $entity = array();

    /**
     * Montando a entidade
     * 
     */
    public function __construct()
    {
        parent::__construct();
        
        $this->entity['subprgid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['subprgdsc'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['sisid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['prgid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['subprgcolor'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '7' , 'contraint' => '');
        $this->entity['sidid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
    }
}
