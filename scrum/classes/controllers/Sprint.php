<?php

/**
 * Controle responsavel pelo kanban.
 * 
 * @author ruy.silva@mec.gov.br
 * @since  08/11/2013
 * 
 * @name       Board
 * @package    classes
 * @subpackage controllers
 * @version    $Id
 */
class Controller_Sprint extends Abstract_Controller
{

    public function defaultAction()
    {
        
        // Pegando o programa pelo cockie
        if (isset($_COOKIE["prgid"])) {
            $prgid = $_COOKIE["prgid"];
        } else {
            $prgid = '';
        }
        
        // Pegando a sprint atual de acordo com a data
        $modelSprint = new Model_Sprint();
        $sprint = $modelSprint->getSprint();
        if($sprint) {
            $sptid = $sprint['sptid'];
        } else {
            $sptid = '';
        }
        
        $this->view->prgid = $prgid;
        $this->view->sptid = $sptid;
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    public function listAction()
    {
        $model = new Model_Sprint();
        $modelPrograma = new Model_Programa();
        $modelEntregavel = new Model_Entregavel();
        
        // Pegando o id do programa nos dados em post, se não tiver, pega no cookie.
        $prgid = $this->getPost('prgid');
        if ($prgid) {
            setcookie("prgid", $prgid, time() + 60 * 60 * 24 * 30, "/");
        } else if (isset($_COOKIE["prgid"])) {
            $prgid = $_COOKIE["prgid"];
        } 

        // Pegando sprint atual.
        $sprintCurrent = $model->getSprint($this->getPost('sptid'));
        
        // Se tiver sprint atual busca a sprint anterior e a proxima sprint.
        $sprintPrevious = false;
        $sprintNext = false;
        if($sprintCurrent){
            $sprintPrevious = $model->getSprint($sprintCurrent['sptid'] - 1);
            $sprintNext = $model->getSprint($sprintCurrent['sptid'] + 1);
        }
        
        $program = array();
        $postitSprintPrevious = array();
        $postitSprintCurrent = array();
        $postitSprintNext = array();
        $postitBackLog = array();
        
        // Se tiver o id do programa busca o programa.
        // Se tiver a sprint atual e o programa.
        if($prgid){
            $program = $modelPrograma->getByValues(array( 'prgid' => $prgid));
            if($sprintCurrent && $program){

                // Carrega sprint anterior
                $postitSprintPrevious = $modelEntregavel->getAllPostit($prgid, $sprintCurrent['sptid'] - 1);

                // Carrega sprint atual
                $postitSprintCurrent = $modelEntregavel->getAllPostit($prgid, $sprintCurrent['sptid']);

                // Carrega proxima sprint
                $postitSprintNext = $modelEntregavel->getAllPostit($prgid, $sprintCurrent['sptid'] + 1);
            }
            
            // Backlog
            $postitBackLog = $modelEntregavel->getAllPostitBackLog($prgid);
        }
        
        // Programa
        $this->view->program = $program;
        
        // Todos os ciclos (sprint)
        $this->view->sprintPrevious = $sprintPrevious;
        $this->view->sprintCurrent = $sprintCurrent;
        $this->view->sprintNext = $sprintNext;
        
        // Todos os postits
        $this->view->postitSprintCurrent = $postitSprintCurrent;
        $this->view->postitSprintNext = $postitSprintNext;
        $this->view->postitSprintPrevious = $postitSprintPrevious;
        $this->view->postitBackLog = $postitBackLog;
        
        $this->render(__CLASS__, __FUNCTION__);
    }
}