<?php
class Model_Area extends Abstract_Model
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
    protected $_name = 'area';

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
        
        $this->entity['areid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['dimid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['aredsc'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '500' , 'contraint' => '');
        $this->entity['arestatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['arecod'] = array( 'value' => '' , 'type' => 'smallint' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
    }
    
    public function getOrdemMaxima( $dimid ){
        global $db;
        
        $sql = "
            SELECT MAX(arecod) AS arecod FROM pdu.area WHERE arestatus = 'A' AND dimid = {$dimid};
        ";
        return $arecod = ( $db->pegaUm($sql) + 1 );
    }
}
