<?php
class Model_Secretariasolucao extends Abstract_Model
{
    protected $_schema = 'pto';
    protected $_name = 'secretariasolucao';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);
        $this->entity['sesid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'pk');
        $this->entity['solid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
        $this->entity['secid'] = array( 'value' => '' , 'type' => 'integer' ,  'is_null' => 'NO' , 'maximum' => '' , 'contraint' => 'fk');
    }

	public function salvarSecretaria($arraySecretaria, $idSolucao){
		if (is_array($arraySecretaria)) {
			$this->deleteAllByValues(array('solid' => $idSolucao));
			foreach( $arraySecretaria as $secretariaID){
				$this->setAttributeValue('solid', $idSolucao);
				$this->setAttributeValue('secid', $secretariaID);
				$id = $this->save();
				if($id == false){
					throw new Exception('Erro ao inserir Secretaria.');
				}
			}
		}
	}
}
