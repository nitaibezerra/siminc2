<?php
class Model_Instituicao extends Abstract_Model
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
    protected $_name = 'instituicao';

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

        $this->entity['intid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['muncodlogradouro'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '7' , 'contraint' => 'fk');
        $this->entity['estuflogradouro'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '2' , 'contraint' => 'fk');
        $this->entity['intorgao'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['intdscrazaosocial'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '255' , 'contraint' => '');
        $this->entity['intdscsigla'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '20' , 'contraint' => '');
        $this->entity['intcnpj'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '14' , 'contraint' => '');
        $this->entity['intcodunidade'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '6' , 'contraint' => '');
        $this->entity['intemail'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['intfonecomercial'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '20' , 'contraint' => '');
        $this->entity['intfonefax'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '20' , 'contraint' => '');
        $this->entity['intdsccaracteristica'] = array( 'value' => '' , 'type' => 'text' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['intcep'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '8' , 'contraint' => '');
        $this->entity['intlogradouro'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['intcompllogradouro'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['intbairrologradouro'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['intnumlogradouro'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '10' , 'contraint' => '');
        $this->entity['intsiteinstitucional'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '100' , 'contraint' => '');
        $this->entity['intlatitude'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '12' , 'contraint' => '');
        $this->entity['intlongitude'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '12' , 'contraint' => '');
        $this->entity['inttipo'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['intvincentid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['intstatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['intdtinclusao'] = array( 'value' => '' , 'type' => 'timestamp without time zone' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
    }
}