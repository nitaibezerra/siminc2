<?php
class Model_Entstatus extends Abstract_Model
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
    protected $_name = 'entstatus';

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
        
        $this->entity['entstid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['entstdsc'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '50' , 'contraint' => '');
    }
    
    public function getAllStatus()
    {
        $arrStatus = $this->_db->carregar('SELECT * FROM scrum.entstatus where entstid != 6 ORDER BY entstid');
        return $arrStatus;
    }
}
