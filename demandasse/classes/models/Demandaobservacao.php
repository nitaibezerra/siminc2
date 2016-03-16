<?php
class Model_Demandaobservacao extends Abstract_Model
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
    protected $_name = 'demandaobservacao';

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
        
        $this->entity['dmoid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['dmdid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmotexto'] = array( 'value' => '' , 'type' => 'text' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['usucpfinclusao'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['dmodtinclusao'] = array( 'value' => '' , 'type' => 'timestamp with time zone' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['usucpfalteracao'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['dmodtalteracao'] = array( 'value' => '' , 'type' => 'timestamp with time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmostatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['usucpfinativacao'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['dmodtinativacao'] = array( 'value' => '' , 'type' => 'timestamp with time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
    }
}
