<?php
class Model_Registroshorasdeatendimento extends Abstract_Model
{
    
    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'demandas';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'registroshorasdeatendimento';

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
        parent::__construct($commit = true);
        
        $this->entity['idregistrohoras'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['cpftecnicoresponsavel'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => '');
        $this->entity['data'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['horainicio'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['horafim'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['qtdhoras'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
    }
}
