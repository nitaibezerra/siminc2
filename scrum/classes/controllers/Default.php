<?php

/**
 * Controle responsavel pelas entidades.
 * 
 * @author Equipe simec - Consultores OEI
 * @since  17/10/2013
 * 
 * @name       Board
 * @package    classes
 * @subpackage controllers
 * @version    $Id
 */
class Controller_Default extends Abstract_Controller
{
    public function defaultAction()
    {
//        $this->formAction();
//        $this->render(__CLASS__, __FUNCTION__);
    }
    
    
    public function selectSubProgramAction()
    {
        $prgid = $this->getPost('prgid');
        $subprgid = $this->getPost('subprgid');
        
        if($prgid){
            $model = new Model_Subprg();
            $subprograms = $model->getAllByValues(array('prgid' => $prgid));
        } else {
            $subprograms = array();
        }
        
        $this->view->subprgid = $subprgid;
        $this->view->subprograms = $subprograms;
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    public function selectStoryAction()
    {
        $subprgid = $this->getPost('subprgid');
        $estid = $this->getPost('estid');
        
        if($subprgid){
            $model = new Model_Estoria();
            $story = $model->getAllByValues(array('subprgid' => $subprgid));
        } else {
            $story = array();
        }
        
        $this->view->estid = $estid;
        $this->view->storys = $story;
        $this->render(__CLASS__, __FUNCTION__);
    }
    
}