<?php
class Model_Demanda extends Abstract_Model
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
    protected $_name = 'demanda';

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
        
        $this->entity['dmdid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['tpdid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['prcid_orig'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['prcid_dest'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['docid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['dmdassunto'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '500' , 'contraint' => '');
        $this->entity['dmddtentdocumento'] = array( 'value' => '' , 'type' => 'timestamp with time zone' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmddtemidocumento'] = array( 'value' => '' , 'type' => 'timestamp with time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdprazoemdias'] = array( 'value' => '' , 'type' => 'numeric' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdprazoemdata'] = array( 'value' => '' , 'type' => 'timestamp with time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdreferencia'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '50' , 'contraint' => '');
        $this->entity['dmdnumsidoc'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '20' , 'contraint' => '');
        $this->entity['dmddb'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['usucpfinclusao'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['dmddtinclusao'] = array( 'value' => '' , 'type' => 'timestamp with time zone' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['usucpfalteracao'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['dmddtalteracao'] = array( 'value' => '' , 'type' => 'timestamp with time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['dmdstatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['usucpfinativacao'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'YES' , 'maximum' => '11' , 'contraint' => 'fk');
        $this->entity['dmddtinativacao'] = array( 'value' => '' , 'type' => 'timestamp with time zone' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['docid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['dmdnumdocumento'] = array('value' => '', 'type' => 'numeric', 'is_null' => 'NO', 'maximum' => '', 'constraint' => '');
        $this->entity['dmdreiteracao'] = array('value' => '', 'type' => 'boolean', 'is_null' => 'NO', 'maximum' => '', 'constraint' => '');
    }
}
