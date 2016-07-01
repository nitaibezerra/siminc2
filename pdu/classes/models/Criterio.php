<?php
class Model_Criterio extends Abstract_Model
{
    
    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'pdu';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'criterio';

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
        
        $this->entity['crtid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['indid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['crtdsc'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '2000' , 'contraint' => '');
        $this->entity['crtstatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['crtpontuacao'] = array( 'value' => '' , 'type' => 'smallint' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['crtpeso'] = array( 'value' => '' , 'type' => 'numeric' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
    }
}
