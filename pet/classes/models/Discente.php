<?php

class Model_Discente extends Abstract_Model
{

    protected $_schema = 'pet';
    protected $_name = 'discente';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['disid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['nome'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '150', 'contraint' => '');
        $this->entity['cpf'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '11', 'contraint' => '');
    }

    public function getDiscentesPorGrupo($idGrupo)
    {
        $sql = "SELECT
                  DISTINCT vig.datainicioatividade as dt,
                  dis.cpf, dis.nome, vig.datainicioatividade, vig.datafimatividade, vig.bolsista
                FROM pet.discente AS dis
                INNER JOIN pet.vigencia AS vig ON vig.disid = dis.disid
                WHERE vig.grpid = {$idGrupo}
                ORDER BY dis.nome
                ";

        $dados = $this->_db->carregar($sql);
        $dados = $this->tratarDados($dados);
        $dados = $dados ? $dados : array();
        return $dados;
    }

    public function getListaDiscentes($idGrupo)
    {
        $dados = $this->getDiscentesPorGrupo($idGrupo);

        $listagem = new Listing(false);
        $listagem->setPerPage(30);
        $listagem->setHead(array('CPF', 'Nome', 'Data Inicio', 'Data Fim', 'Bolsista'));
        $listagem->setEnablePagination(false);
        $listagem->listing($dados);
    }

    public function tratarDados($dados)
    {
        if ($dados) {
            $newData = array();
            foreach ($dados as $key=>$valor) {
                $newData[$key]['cpf'] = formatar_cpf($valor['cpf']);
                $newData[$key]['nome'] = $valor['nome'];
                $newData[$key]['datainicioatividade'] = formata_data($valor['datainicioatividade']);
                $newData[$key]['datafimatividade'] = formata_data($valor['datafimatividade']);
                $newData[$key]['bolsista'] = ( $valor['bolsista'] == 't' ? 'SIM' : 'NÃO');
            }
            return $newData;
        }
    }

}
