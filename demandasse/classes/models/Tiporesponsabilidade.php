<?php
class Model_Tiporesponsabilidade extends Abstract_Model
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
    protected $_name = 'tiporesponsabilidade';

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
        
        $this->entity['tprcod'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['tprdsc'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['tprsnvisivelperfil'] = array( 'value' => '' , 'type' => 'boolean' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['tprsigla'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['tprurl'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '255' , 'contraint' => '');
    }
}
