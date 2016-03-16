<?php
class Model_Procedencia extends Abstract_Model
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
    protected $_name = 'procedencia';

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
        
        $this->entity['prcid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['prcsigla'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '20' , 'contraint' => '');
        $this->entity['prcdsc'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '50' , 'contraint' => '');
        $this->entity['prcresponsavel'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '50' , 'contraint' => '');
        $this->entity['prcremailesponsavel'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '50' , 'contraint' => '');
        $this->entity['prcremailinstitucional'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '50' , 'contraint' => '');
        $this->entity['prcstatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
    }
}
