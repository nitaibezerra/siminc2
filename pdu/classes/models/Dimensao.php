<?php
class Model_Dimensao extends Abstract_Model
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
    protected $_name = 'dimensao';

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
        
        $this->entity['dimid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['itrid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['dimdsc'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '500' , 'contraint' => '');
        $this->entity['dimstatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['dimcod'] = array( 'value' => '' , 'type' => 'smallint' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
    }
    
    public function isValid()
    {
        $isValid = parent::isValid();

        if($isValid){
            if($this->entity['dimcod']['value']){
                $dimcod = $this->entity['dimcod']['value'];
                if($dimcod > 32767){
                    $this->error[] = array("name" => 'dimcod', "msg" => utf8_encode($this->entity['dimcod']['value'] . self::MSG_INTEIRO_INVALIDO));
                    $isValid = false;
                }
            } 
        }
        
        return $isValid;
    }
    
    public function getOrdemMaxima( $itrid ){
        global $db;
        
        $sql = "
            SELECT MAX(dimcod) AS dimcod FROM pdu.dimensao WHERE dimstatus = 'A' AND itrid = {$itrid};
        ";
        return $dimcod = ( $db->pegaUm($sql) + 1 );
    }
}
