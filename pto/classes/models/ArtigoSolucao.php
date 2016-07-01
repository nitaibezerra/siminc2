<?php

class Model_ArtigoSolucao extends Abstract_Model {

    protected $_schema = 'pto';
    protected $_name = 'artigosolucao';
    public $entity = array();

    public function __construct($commit = true) {
        parent::__construct($commit);

        $this->entity['arsid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['solid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['artid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
    }

	public function salvarArtigo($arrayArtigo, $idSolucao, $arrayMetas){

        if (count($arrayMetas) == 1 && in_array(Model_Metasolucao::CORPO_LEI_ID, $arrayMetas) && empty($arrayArtigo) ) {
            $this->error[] = array("name" => 'artid', "msg" => ('Não pode estar vazio'));
            throw new Exception('Nenhum Artigo foi selecionado!');
        }


		if (is_array($arrayArtigo) ) {
			$this->deleteAllByValues(array('solid' => $idSolucao));
			foreach( $arrayArtigo as $artigoID){
				$this->setAttributeValue('solid', $idSolucao);
				$this->setAttributeValue('artid', $artigoID);
				$id = $this->save();

				if($id == false){
					throw new Exception('Erro ao inserir Artigo.');
				}
			}
		}

	}
}
