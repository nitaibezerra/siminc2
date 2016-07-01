<?php
/**
 * Controle responsavel pelas procedencias.
 *
 * @author Ruy Junior Ferreira Silva <ruyjfs@gmail.com>
 * @since  13/10/2014
 *
 * @name       Procedencia
 * @package    classes
 * @subpackage controllers
 * @version    $Id
 */
class Controller_Procedencia extends Abstract_Controller
{
    protected $_model;

    public function __construct()
    {
        parent::__construct();
        $this->_model = new Model_Procedencia();
    }

    public function indexAction()
    {
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function formularioAction()
    {
        $id = $this->getPost('id');
        $this->_model->populateEntity(array( 'prcid' => $id));
        $this->view->entity = $this->_model->entity;

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function listarAction()
    {
        $listing = new Listing();

        $listing->setHead(array('Sigla', 'Descrição' , 'Responsável', 'E-mail do responsável' , 'E-mail institucional'));
        $listing->enableCount(true);

        $prcsigla = $this->getPost('prcsigla');
        $prcdsc = $this->getPost('prcdsc');
        $prcresponsavel = $this->getPost('prcresponsavel');

        $where = array();
        if($prcsigla) $where[] = "prcsigla ILIKE  '%{$prcsigla}%'";
        if($prcdsc) $where[] = "prcdsc ILIKE '%{$prcdsc}%' ";
        if($prcresponsavel) $where[] = "prcresponsavel ILIKE '%{$prcresponsavel}%' ";

        if( $where && count($where) > 0 ) {
            $where = ' WHERE ' . implode(' AND ' , $where );
        } else {
            $where = '';
        }

        $listing->setActions(array('edit' => 'editar' , 'delete' => 'excluir'));
        $data = "SELECT prcid, prcsigla, prcdsc, prcresponsavel, prcremailesponsavel, prcremailinstitucional--, prcstatus
                 FROM demandasse.procedencia
                 {$where} ";

        $this->view->exibirTitulo = true;
        $this->view->data = $data;
        $this->view->listing = $listing;

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function salvarAction()
    {
        $this->_model->populateEntity($_POST);

        $id = $this->_model->save($_POST);

        if($this->_model->error){
            $return = array('status' => false , 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $this->_model->error);
        } else {
            $return = array('status' => true , 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $id);
        }

        echo simec_json_encode($return);
    }

    public function deletarAction()
    {
        $id = $this->getPost('id');

        $dataForm = array();
        $dataForm['prcid'] = $id;
        $dataForm['prcstatus'] = 'I';

        $this->_model->populateEntity($dataForm);

        $this->_model->setDecode(false);
        $result = $this->_model->save($_POST);

//        $result = $this->_model->delete();

        if($result){
            $return = array('status' => true , 'msg' => utf8_encode('Deletado com sucesso!'), 'result' => '');
        } else {
            $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar!'), 'result' => '');
        }

        echo simec_json_encode($return);
    }
}