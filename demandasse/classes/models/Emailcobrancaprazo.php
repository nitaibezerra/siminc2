<?php
class Model_Emailcobrancaprazo extends Abstract_Model
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
    protected $_name = 'emailcobrancaprazo';

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
        
        $this->entity['ecpid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['dmdid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['usucpf'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['ecpemailde'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '50' , 'contraint' => '');
        $this->entity['ecpemailpara'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '50' , 'contraint' => '');
        $this->entity['ecpemailcc'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '50' , 'contraint' => '');
        $this->entity['ecpassunto'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['ecpcorpoemail'] = array( 'value' => '' , 'type' => 'text' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['ecpdtenvio'] = array( 'value' => '' , 'type' => 'timestamp with time zone' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
    }
}
