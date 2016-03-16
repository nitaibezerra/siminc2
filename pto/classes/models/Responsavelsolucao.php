<?php

class Model_Responsavelsolucao extends Abstract_Model
{
    protected $_schema = 'pto';
    protected $_name = 'responsavelsolucao';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['resid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['solid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['usucpf'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '11', 'contraint' => 'fk');
        $this->entity['restipo'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '1', 'contraint' => '');
    }

    public function getOptionsResponsavelSe()
    {
        $sql = $this->getSqlUsuarioSistema();
        $dados = $this->_db->carregar($sql);
        return $this->getOptions($dados, array(), 'usucpf', 'usunome');
    }

    private function getSqlUsuarioSistema()
    {
        $id = ID_SISTEMA;
        return "SELECT usu_sis.sisid, usu_sis.usucpf,
                      usu.usunome, usu.usuemail
                FROM seguranca.usuario_sistema usu_sis
                INNER JOIN seguranca.usuario usu ON usu.usucpf = usu_sis.usucpf
                WHERE usu_sis.susstatus = 'A'
                    AND usu_sis.sisid =  {$id}
                    ORDER BY usu.usunome";

    }

    public function salvarResponsavelSe($arrayResponsavelSe, $idSolucao)
    {

        if (is_array($arrayResponsavelSe)) {
            $this->deleteAllByValues(array('solid' => $idSolucao, 'restipo' => 'S'));

            foreach ($arrayResponsavelSe as $responsavelSeID) {

                $this->setAttributeValue('solid', $idSolucao);
                $this->setAttributeValue('usucpf', $responsavelSeID);
                $this->setAttributeValue('restipo', 'S');
                $id = $this->save();

                if ($id == false) {
                    throw new Exception('Erro ao inserir Responsável SE.');
                }
            }
        }
    }

    public function salvarResponsavelSecretariaAutarquia($arrayResponsavelSecretariaAutarquia, $idSolucao)
    {
        if (is_array($arrayResponsavelSecretariaAutarquia)) {
            $this->deleteAllByValues(array('solid' => $idSolucao, 'restipo' => 'A'));
            foreach ($arrayResponsavelSecretariaAutarquia as $responsavelSecretariaAutarquiaID) {
                $this->setAttributeValue('solid', $idSolucao);
                $this->setAttributeValue('usucpf', $responsavelSecretariaAutarquiaID);
                $this->setAttributeValue('restipo', 'A');
                $id = $this->save();
                if ($id == false) {
                    throw new Exception('Erro ao inserir Responsável Secretaria/Autarquia.');
                }
            }
        }
    }

    public function getResponsavelPainel($arrayIds, $tipo)
    {
        if (is_array($arrayIds) and count($arrayIds) > 0) {
            $arrayIds = array_map("trim", $arrayIds);
            $ids = implode('\',\'', $arrayIds);

            $sql = "SELECT DISTINCT usu.usucpf, usu.usunome, usu.usuemail
                FROM seguranca.usuario usu
                INNER JOIN pto.responsavelsolucao respusu ON usu.usucpf = respusu.usucpf
                            WHERE usu.usucpf in ('{$ids}' )
                            AND respusu.restipo = '{$tipo}'
                            ORDER BY usu.usunome
                   ";
            $dados = $this->_db->carregar($sql);

//            ver($dados, d);
            $arrayDescricao = array();
            if ($dados) {
                foreach ($dados as $valor) {
                    $arrayDescricao[] = $valor['usunome'];
                }
            }
            return implode('<br> ', $arrayDescricao);
        } else {
            return false;
        }
    }
}
