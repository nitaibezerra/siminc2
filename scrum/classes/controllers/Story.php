<?php

/**
 * Controle responsavel pela estoria.
 * 
 * @author ruy.silva@mec.gov.br
 * @since  08/11/2013
 * 
 * @name       Board
 * @package    classes
 * @subpackage controllers
 * @version    $Id
 */
class Controller_Story extends Abstract_Controller
{
    
    public function defaultAction()
    {
        $this->formAction();
    }
    
    public function formAction()
    {
        $modelPrograma = new Model_Programa();
        
        $this->view->programs = $modelPrograma->getAll();
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    public function listAction()
    {
        $this->view->dataForm = $this->getPost('dataForm');
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    public function saveAction()
    {
        $model = new Model_Estoria();
        $model->populateEntity($_POST);
        $id = $model->save($_POST);
        
        if($model->error){
            $return = array('status' => false , 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $model->error);
        } else {
            $return = array('status' => true , 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $id);
        }
        
        echo simec_json_encode($return);
    }
    
    public function entityJsonAction()
    {
        $id = $this->getPost('id');
        
        $model = new Model_Estoria();
        $model->populateEntity(array( 'estid' => $id));
        
        $modelSubPrograma = new Model_Subprg();
        $modelSubPrograma->populateEntity(array('subprgid' => $model->entity['subprgid']['value']));
            
        $entitys = $model->entity + $modelSubPrograma->entity;
        
        foreach($entitys as &$entity){
            if(!empty($entity['value'])){
                $entity['value'] = utf8_encode($entity['value']);
            }
        }
        
        
        echo simec_json_encode($entitys);
    }
    
    public function deleteAction()
    {
        $id = $this->getPost('id');
        
        $modelEntregavel = new Model_Entregavel(false);
        $entregaveis = $modelEntregavel->getAllByValues(array( 'estid' => $id));
        
        if($entregaveis){
            $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar esta estória pois ela possui entregável!'), 'result' => '');
        } else {
            $model = new Model_Estoria(false);
            $model->populateEntity(array( 'estid' => $id));
            $result = $model->delete();
            
            if($result){
                $return = array('status' => true , 'msg' => utf8_encode('Estória deletada com sucesso!'), 'result' => '');
            } else {
                $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar esta estória!'), 'result' => '');
            }
        }
        
        echo simec_json_encode($return);
    }
    
    public function selectSubProgramAction()
    {
        $prgid = $this->getPost('prgid');
        
        if($prgid){
            $model = new Model_Subprg();
            $subprograms = $model->getAllByValues(array('prgid' => $prgid));
        } else {
            $subprograms = array();
        }
        
        $this->view->subprograms = $subprograms;
        $this->render(__CLASS__, __FUNCTION__);
    }
}