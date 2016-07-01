<?php
class Model_Pesacompconsolidado extends Abstract_Model
{
    
    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'pes';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'pesacompconsolidado';

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
        
        $this->entity['acccodigo'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['uorcodigo'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '20' , 'contraint' => '');
        $this->entity['accmes'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['tidcodigo'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['accvalor'] = array( 'value' => '' , 'type' => 'numeric' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['accano'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
    }
}
