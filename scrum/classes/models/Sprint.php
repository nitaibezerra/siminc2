<?php
class Model_Sprint extends Abstract_Model
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
    protected $_name = 'sprint';

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
        
        $this->entity['sptid'] = array( 'value' => '' , 'type' => 'bigint' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['sptinicio'] = array( 'value' => '' , 'type' => 'date' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
        $this->entity['sptfim'] = array( 'value' => '' , 'type' => 'date' ,  'is_null' => 'YES' , 'maximum' => '' , 'contraint' => '');
    }
    
    public function getSprint($sptid = null)
    {
        if($sptid){
            $sqlSprintAtual = "SELECT sptid ,
                               TO_CHAR( sptinicio , 'dd/mm/yyyy' ) as sptinicio, 
                               TO_CHAR( sptfim , 'dd/mm/yyyy' ) as sptfim FROM scrum.sprint 
                               WHERE sptid = {$sptid}";
        } else {
            $sqlSprintAtual = 'SELECT sptid , 
                               TO_CHAR( sptinicio , \'dd/mm/yyyy\' ) as sptinicio, 
                               TO_CHAR( sptfim , \'dd/mm/yyyy\' ) as sptfim FROM scrum.sprint 
                               WHERE now() 
                               between sptinicio and sptfim';
        }
    
        $printAtual = $this->_db->pegaLinha($sqlSprintAtual);
        return $printAtual;
    }
}
