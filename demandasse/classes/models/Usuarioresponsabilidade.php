<?php
class Model_Usuarioresponsabilidade extends Abstract_Model
{
    
    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'demandasse';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'usuarioresponsabilidade';

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
        
        $this->entity['rpuid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['pflcod'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['usucpf'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['rpustatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['rpudata_inc'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
    }
}
