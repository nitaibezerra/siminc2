<?php

class Model_Etapa extends Abstract_Model
{

    protected $_schema = 'pto';
    protected $_name = 'etapa';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);
        $this->perfilUsuario = new Model_PerfilUsuario();
        $this->entity['etpid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk', 'label' => 'Código');
        $this->entity['solid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'fk', 'label' => 'Código Projeto');
        $this->entity['acaid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk', 'label' => 'Ação Estratégica');
        $this->entity['etpordem'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Ordem');
        $this->entity['etpdsc'] = array('value' => '', 'type' => 'character varying', 'is_null' => 'NO', 'maximum' => '100', 'contraint' => '', 'label' => 'Etapa');
        $this->entity['etpstatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'NO', 'maximum' => '1', 'contraint' => '', 'label' => 'Status');
        $this->entity['etpobs'] = array('value' => '', 'type' => 'text', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '', 'label' => 'Observação');
    }

    public function getDados($solid = null)
    {
        $solid = ($_SESSION['solid'] ? $_SESSION['solid'] : $solid);
        $this->dadosTabela = $this->getDadosGrid($solid);
        return $this->dadosTabela;
    }

    public function getListing()
    {
        $listing = new Listing(false);
        $listing->setPerPage(999999);
        $listing->setIdTable('table_etapa');
        $listing->setClassTable('table table-striped table-bordered sorted_table');
        $listing->setHead(array(
            $this->getAttributeLabel('etpdsc'),
//            $this->getAttributeLabel('etpordem'),
            $this->getAttributeLabel('acaid'),
            $this->getAttributeLabel('etpobs'),
        ));
        if ($this->perfilUsuario->possuiAcessoConsulta()) {
            $listing->setActions(array('chevron-down' => 'visualizar_atividade','edit' => 'editar_etapa',));
        } else {
            $listing->setActions(array('chevron-down' => 'visualizar_atividade', 'resize-vertical' => 'ordenar', 'edit' => 'editar_etapa', 'delete' => 'excluir_etapa'));
        }
        return $listing;
    }

    public function getDadosGrid($solid_ = null)
    {
        $solid = ($_SESSION['solid'] ? $_SESSION['solid'] : $solid_);
        $sql = "
             SELECT etapa.etpid, etapa.etpdsc, acao.acaid || ' - ' || acao.acadsc, etpobs
                FROM pto.etapa etapa
                LEFT JOIN painel.acao acao ON acao.acaid = etapa.acaid
                WHERE etpstatus = 'A'
                 AND solid = {$solid} ORDER BY etapa.etpordem ASC;
         ";
        return $this->_db->carregar($sql);
    }

    public function possuiEtapa($solid = null)
    {
        if (empty($solid)) {
            $solid = $_SESSION['solid'];
        }
        $sql = " SELECT count(etpid) as total FROM pto.etapa WHERE etpstatus = 'A'  AND solid = {$solid} ; ";
        $result = $this->_db->carregar($sql);
        $total = (int)$result[0]['total'][0];
        return ($total > 0 ? true : false);
    }

    public function salvarEtapa()
    {
        $this->populateEntity($_POST);
        $this->setAttributeValue('solid', $_SESSION['solid']);
        $this->setAttributeValue('etpstatus', 'A');
        $etpordem = $this->getAttributeValue('etpordem');
        if (empty($etpordem)) {
            $dados = $this->getAllByValues(array('etpstatus' => 'A'));
            if ($dados) {
                $count = count($dados);
            }
            $this->setAttributeValue('etpordem', $count + 1);
        }
        $idEtapa = $this->save();
        if ($idEtapa == false) {
            throw new Exception('Erro ao inserir Etapa.');
        } else {
            return $idEtapa;
        }
    }

    public function inativar($id)
    {
        $this->populateEntity(array('etpid' => $id));
        $this->setAttributeValue('etpdsc', ($this->getAttributeValue('etpdsc')));
        $this->setAttributeValue('etpstatus', 'I');
        $id = $this->update();
        if ($id == false) {
            throw new Exception('Erro ao excluir a Etapa.');
        } else {
            return $id;
        }
    }

    public function alterarOrdem($etpid, $ordem, $solid)
    {
        $this->populateEntity(array('etpid' => $etpid));
        $this->setAttributeValue('etpordem', $ordem);
        $this->setAttributeValue('solid', $solid);
        $this->setDecode(false);
        $id = $this->update();
        if ($id == false) {
            throw new Exception('Erro ao ordenar a Etapa.');
        } else {
            return $id;
        }
    }

    public function getTituloEtapa()
    {
        return "<b>Etapa: </b> {$this->getAttributeValue('etpdsc')}";
    }
}
