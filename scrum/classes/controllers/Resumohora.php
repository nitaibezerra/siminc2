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
class Controller_Resumohora extends Abstract_Controller
{

    public function defaultAction()
    {
        
        // Pegando o programa pelo cockie
        if (isset($_COOKIE["prgid"])) {
            $prgid = $_COOKIE["prgid"];
        } else {
            $prgid = '';
        }
        
        $this->view->prgid = $prgid;
        $this->view->sptid = $sptid;
        $this->render(__CLASS__, __FUNCTION__);
        
    }
    
    public function listAction()
    {
        
        // Pegando o programa pelo cockie
        $prgid = $this->getPost('prgid');
        if($prgid){
            setcookie("prgid", $prgid, time() + 60 * 60 * 24 * 30, "/");
        }else if(!$prgid && isset($_COOKIE["prgid"])){
            $prgid = $_COOKIE["prgid"];
        }
        
        // Pegando a sprint atual de acordo com a data
        $sptid = $this->getPost('sptid');
        if(!$sptid) {
            $modelSprint = new Model_Sprint();
            $sprint = $modelSprint->getSprint();
            if($sprint) $sptid = $sprint['sptid'];
        }
        
        // Se tiver programa e sprint, carrega todos os entregaveis por eles.
        if($prgid && $sptid){
            $modelEntregavel = new Model_Entregavel();
            $list = $modelEntregavel->retornaDadosParaContagemDeHoras($prgid , $sptid);
        } else {
            $list = '';
        }
        
        $modelEntStatus = new Model_Entstatus();
        $this->view->status = $modelEntStatus->getAllStatus();
        $this->view->list = $list;
        $this->render(__CLASS__, __FUNCTION__);
    }

}
