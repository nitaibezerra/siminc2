<?php

/**
 * Controle responsavel pelas entidades.
 * 
 * @author Ruy Ferreira <ruy.silva@mec.gov.br>
 * @since  21/11/2013
 * 
 * @name       Program
 * @package    classes
 * @subpackage controllers
 * @version    $Id
 */
class Controller_Subprogram extends Abstract_Controller
{
    public function defaultAction()
    {
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    public function formAction()
    {
        $dataForm = $this->getPost('dataForm');
        
        $model = new Model_Subprg();
        
        if($dataForm['id']){
            $model->populateEntity(array('subprgid' => $dataForm['id']));
        }
        
        $modelPrograma = new Model_Programa();
        
        $this->view->entity = $model->entity;
        $this->view->program = $modelPrograma->getAll();
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    public function listAction()
    {
        $this->view->dataForm = $this->getPost('dataForm');
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    public function saveAction()
    {
        $model = new Model_Subprg();
        $model->populateEntity($_POST);
        $id = $model->save($_POST);
        
        if($model->error){
            $return = array('status' => false , 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $model->error);
        } else {
            $return = array('status' => true , 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $id);
        }
        
        echo simec_json_encode($return);
    }
    
    public function deleteAction()
    {
        $id = $this->getPost('id');
        
        $modelEstoria = new Model_Estoria();
        $estorias = $modelEstoria->getAllByValues(array( 'subprgid' => $id));
        
        if($estorias){
            $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar este sub-projeto pois ele possui entregáveis!'), 'result' => '');
        } else {
            $model = new Model_Subprg(false);
            $model->populateEntity(array( 'subprgid' => $id));
            $result = $model->delete();
            
            if($result){
                $return = array('status' => true , 'msg' => utf8_encode('Sub-projeto deletada com sucesso!'), 'result' => '');
            } else {
                $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar este sub-projeto!'), 'result' => '');
            }
        }
        
        echo simec_json_encode($return);
    }
}