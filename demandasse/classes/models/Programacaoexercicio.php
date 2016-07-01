<?php
class Model_Programacaoexercicio extends Abstract_Model
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
    protected $_name = 'programacaoexercicio';

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
        
        $this->entity['prsano'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'NO' , 'maximum' => '4' , 'contraint' => 'pk');
        $this->entity['prsdata_inicial'] = array( 'value' => '' , 'type' => 'date' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['prsdata_termino'] = array( 'value' => '' , 'type' => 'date' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['prsexerccorrente'] = array( 'value' => '' , 'type' => 'boolean' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['prsstatus'] = array( 'value' => '' , 'type' => 'character' ,  'is_null' => 'YES' , 'maximum' => '1' , 'contraint' => '');
        $this->entity['prsativo'] = array( 'value' => '' , 'type' => 'smallint' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['prsexercicioaberto'] = array( 'value' => '' , 'type' => 'boolean' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
    }
}
