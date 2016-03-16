<?php

class Model_Colegiado extends Abstract_Model
{

    protected $_schema = 'pet';
    protected $_name = 'colegiado';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['colid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['idgid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['colstatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
        $this->entity['nome'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '500', 'contraint' => '');
    }
	
    public function getListaColegiadoPorGrupo($grpid, $somenteLeitura = false)
    {
        $sql = "SELECT
                    col.colid, col.nome
                FROM pet.colegiado AS col
                INNER JOIN pet.identificacaogrupo AS idg ON idg.idgid = col.idgid
                WHERE col.colstatus = 'A' AND idg.grpid = {$grpid}
                ORDER BY col.nome ";

        $dados = $this->_db->carregar($sql);

        $dados = $dados ? $dados : array();
        if (count($dados) > 0) {
            $listagem = new Listing(false);
            $listagem->setPerPage(30);
            $listagem->setEnablePagination(false);
			if(!$somenteLeitura){
				$listagem->setHead(array('Nome'));
				$listagem->setActions(array('edit' => 'Editar', 'delete' => 'Apagar'));
			}else{
				$listagem->setHead(array('Código','Nome'));
			}
            $listagem->listing($dados);
        }
    }

    public function excluir($id){
        $this->populateEntity(array('colid' => $id));
        $this->setAttributeValue('colstatus', 'I');
		$this->setDecode(false);
        $this->treatEntityToUser();
        $this->save();
    }
    
}
