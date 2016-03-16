<?php

class Model_Contrato extends Abstract_Model
{

    protected $_schema = 'contratogestao';
    protected $_name = 'contrato';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);

        $this->entity['conid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label' => 'ID');
        $this->entity['hqcid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label' => 'ID Hieraquia');
        $this->entity['consigla'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '10', 'contraint' => '', 'label' => 'Sigla');
        $this->entity['condescricao'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '500', 'contraint' => '', 'label' => 'Descrição');
        $this->entity['datainicial'] = array('value' => '', 'type' => 'date', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Data Inicial');
        $this->entity['datafinal'] = array('value' => '', 'type' => 'date', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Data Final');
        $this->entity['conmeta'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Meta');
        $this->entity['conpeso'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Peso');
        $this->entity['concontratada'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '100', 'contraint' => '', 'label' => 'Contratada');
        $this->entity['conprocesso'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '50', 'contraint' => '', 'label' => 'Processo');
        $this->entity['conobjetivo'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '500', 'contraint' => '', 'label' => 'Objetivo');
        $this->entity['connumerocontrato'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '50', 'contraint' => '', 'label' => 'Número Contrato');
        $this->entity['connumeroaditivo'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '50', 'contraint' => '', 'label' => 'Número Aditivo');
        $this->entity['conarearesponsavel'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'YES', 'maximum' => '50', 'contraint' => '', 'label' => 'Área do Responsável');
        $this->entity['conaditivo'] = array('value' => '', 'type' => 'boolean', 'is_null' => 'NO', 'contraint' => '', 'label' => 'Aditivo');
    }

    public function getLabelAtribute($atributo)
    {
        return $this->entity[$atributo]['label'];
    }

    public function getContratoById($id)
    {
        $sql = "SELECT *
                    FROM contratogestao.hierarquiacontrato as hc
                        INNER JOIN contratogestao.contrato as c ON hc.hqcid = c.hqcid 
                    WHERE c.conid = {$id} AND constatus = 'A'
                    ORDER BY hc.hqcid, hc.hqcordem
                    ";
        $dados = $this->_db->carregar($sql);
        return $dados[0];
    }

    public function salvar($idHierarquia)
    {
        $this->setAttributeValue('consigla', strtoupper($this->getAttributeValue('consigla')));
        $this->setAttributeValue('hqcid', (int)$idHierarquia);
        $this->setAttributeValue('conmeta', 3);
        $this->setAttributeValue('conpeso', 1);
        if ($this->getAttributeValue('conaditivo') === 'on') {
            $this->setAttributeValue('conaditivo', 't');
        } else {
            $this->setAttributeValue('conaditivo', 'f');
        }
        return $this->save();
    }

    public function getContratoByIdHierarquia($id)
    {
        $sql = "SELECT *
                    FROM contratogestao.hierarquiacontrato as hc
                        INNER JOIN contratogestao.contrato as c ON hc.hqcid = c.hqcid 
                    WHERE c.hqcid = {$id} AND hqcstatus = 'A'
                    ORDER BY hc.hqcid, hc.hqcordem
                    ";
        $dados = $this->_db->carregar($sql);
        return $dados[0];
    }


    public function possuiFatorAvaliado($hqcid)
    {
        $hqcid = (int)$hqcid;
        $hierarquiacontrato = new Model_Hierarquiacontrato();
        $dados = $hierarquiacontrato->getNosComPai($hqcid, " AND (q.h).hqcnivel = 7 ");

        if ($dados) {
            foreach($dados as $atividade){
                $fatorAvaliado = new Model_Fatoravaliado();
                $dadosF = $fatorAvaliado->getAllByValues(array('conid' => (int) $atividade['conid'], 'fatstatus' => 'A' ));
                if ($dadosF) {
                    return true;
                }
            }
        }
        return false;
    }

}
