<?php
class Model_Estoria extends Abstract_Model
{
    
    /**
     * Nome do schema
     * @var string
     */
    protected $_schema = 'scrum';

    /**
     * Nome da tabela
     * @var string
     */
    protected $_name = 'estoria';

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
        
        $this->entity['estid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['estdsc'] = array( 'value' => '' , 'type' => 'text' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => '');
        $this->entity['subprgid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['esttitulo'] = array( 'value' => '' , 'type' => 'character varying' ,  'is_null' => 'NO' , 'maximum' => '50' , 'contraint' => '');
    }
    
    public function getAllStoryProgramByStory($entid)
    {
        $sql = "SELECT * FROM scrum.estoria est
                LEFT JOIN scrum.subprg subprg on (subprg.subprgid = est.subprgid)
                WHERE subprg.subprgid = (SELECT subprgid FROM scrum.entregavel ent LEFT JOIN scrum.estoria est ON (est.estid = ent.estid) WHERE entid = {$entid})";
                
        $result = $this->_db->carregar($sql);
                
        return $result;
    }
}
