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
class Controller_Program extends Abstract_Controller
{
    public function defaultAction()
    {
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    public function formAction()
    {
        $dataForm = $this->getPost('dataForm');
        
        $model = new Model_Programa();
        
        if( isset($dataForm['id']) && $dataForm['id']){
            $model->populateEntity(array('prgid' => $dataForm['id']));
            $modelUsuarioresponsabilidades = new Model_Usuarioresponsabilidade();
            $responsibles = $modelUsuarioresponsabilidades->getAllByValues(array('prgid' => $dataForm['id'], 'rpustatus' => 'A'));
            
            foreach($responsibles as &$responsible){
                $responsible = $responsible['usucpf'];
            }
            
        } else {
            $responsibles = array();
        }
        
        $this->view->responsibles = $responsibles;
        $this->view->entity = $model->entity;
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    public function listAction()
    {
        $this->view->dataForm = $this->getPost('dataForm');
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    public function saveAction()
    {
        $model = new Model_Programa();
        $model->populateEntity($_POST);
        $id = $model->save($_POST);
        
        $usucpfs = $this->getPost('usucpf');
        
        if($usucpfs && is_array($usucpfs) && count($usucpfs) > 0){
            $modelUsuarioresponsabilidade = new Model_Usuarioresponsabilidade();
            
            if($id){
                $responsibles = $modelUsuarioresponsabilidade->getAllByValues(array('prgid' => $id, 'rpustatus' => 'A'));
                
                if($responsibles){
                    $modelUsuarioresponsabilidade->desactiveAllByProgram($id);
                }
                
                foreach($usucpfs as $usucpf){
                    $modelUsuarioresponsabilidade->clearEntity();
                    
                    $userData = $modelUsuarioresponsabilidade->getByValues(array('prgid' => $id, 'usucpf' => $usucpf));
                    
                    if($userData){
                        $userData['rpustatus'] = 'A';
                    } else {
                        $userData = array();
                        $userData['pflcod'] = '1040';
                        $userData['usucpf'] = $usucpf;
                        $userData['rpustatus'] = 'A';
                        $userData['rpudata_inc'] = 'now()';
                        $userData['prgid'] = $id;
                    }
                    
                    $modelUsuarioresponsabilidade->populateEntity($userData);
                    $modelUsuarioresponsabilidade->save();
                }
                
            }
        }
        
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
        
        $modelSubprograma = new Model_Subprg(false);
        $subprogramas = $modelSubprograma->getAllByValues(array( 'prgid' => $id));
        
        if($subprogramas){
            $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar este projeto pois ele possui Sub-projetos!'), 'result' => '');
        } else {

            $modelUsuarioresponsabilidade = new Model_Usuarioresponsabilidade();
            $result = $modelUsuarioresponsabilidade->deleteAllByValues(array( 'prgid' => $id));

            $model = new Model_Programa(false);
            $model->populateEntity(array( 'prgid' => $id));
            $result = $model->delete();

            if($result){
                $return = array('status' => true , 'msg' => utf8_encode('Projeto deletada com sucesso!'), 'result' => '');
            } else {
                $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar este projeto!'), 'result' => '');
            }
        }
        
        echo simec_json_encode($return);
    }
}