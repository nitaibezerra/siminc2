<?php
class Model_Indicador extends Abstract_Model
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
    protected $_name = 'indicador';

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
        
        $this->entity['indid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['areid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['inddsc'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '500' , 'contraint' => '');
        $this->entity['indstatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['indcod'] = array( 'value' => '' , 'type' => 'smallint' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
    }
    
    public function getOrdemMaxima( $indid ){
        global $db;
        
        $sql = "
            SELECT MAX(indcod) AS indcod FROM pdu.indicador WHERE indstatus = 'A' AND indid = {$indid};
        ";
        return $indcod = ( $db->pegaUm($sql) + 1 );
    }
    
}
