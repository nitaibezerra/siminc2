<?php
class Model_Dirigente extends Abstract_Model
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
    protected $_name = 'dirigente';

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
        
        $this->entity['drgid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['estuflogradouro'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '2' , 'contraint' => 'fk');
        $this->entity['muncodlogradouro'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '7' , 'contraint' => 'fk');
        $this->entity['drgnome'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '255' , 'contraint' => '');
        $this->entity['drgcpf'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '14' , 'contraint' => '');
        $this->entity['drgfuncao'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '255' , 'contraint' => '');
        $this->entity['drgemail'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['drgfonecomercial'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '20' , 'contraint' => '');
        $this->entity['drgfonefax'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '20' , 'contraint' => '');
        $this->entity['drgfonecelular'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '20' , 'contraint' => '');
        $this->entity['drgcep'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '8' , 'contraint' => '');
        $this->entity['drglogradouro'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['drgcompllogradouro'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['drgbairrologradouro'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['drgnumlogradouro'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '10' , 'contraint' => '');
        $this->entity['drglatitude'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '12' , 'contraint' => '');
        $this->entity['drglongitude'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '12' , 'contraint' => '');
        $this->entity['drgvincentid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['drgstatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['drgdtinclusao'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
    }
}
