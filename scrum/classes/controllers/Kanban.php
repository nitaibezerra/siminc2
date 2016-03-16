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
class Controller_Kanban extends Abstract_Controller
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
    
    public function tramitSprintAction()
    {    
        global $db;
        if ($_POST['idpostit'] && $_POST['idstatus'])
        {
            $sql = "UPDATE scrum.entregavel SET entstid= {$_POST['idstatus']} WHERE entid = {$_POST['idpostit']} RETURNING entid;";
            $idEntregavel = $db->pegaUm($sql);

            if($idEntregavel){

                $modelEntregavel = new Model_Entregavel();
                $entregavel = $modelEntregavel->getByValues(array('entid' => $idEntregavel));

                if($entregavel['dmdid']){
                    $modelDemanda = new Model_Demanda();
                    $demanda = $modelDemanda->getByValues(array('dmdid' => $entregavel['dmdid']));
                    
                    if($demanda['docid']){

                        $modelDocumento = new Model_Documento();

                        switch($_POST['idstatus']){
                            // Em analise - 107
                            case 1:
                                $esdid = 107;
                                $dmddataconclusao = '';
                                break;
                            // Em atendimento - 108
                            case 2:
                                $esdid = 108;
                                $dmddataconclusao = '';
                                break;
                            // Aguardando Validacao - 111
                            case 3:
                                $esdid = 111;
                                $dmddataconclusao = '';
                                break;
                            // Finalizada - 109
                            case 5:
                                $esdid = 109;
                                $dmddataconclusao = date('Y-m-d');
                                break;
                        }

                        $modelDemanda->populateEntity(array('dmdid' => $entregavel['dmdid'],  'dmddataconclusao' => $dmddataconclusao));
                        $modelDemanda->save();
//ver($dmddataconclusao, d);
                        $modelDocumento->populateEntity(array('docid' => $demanda['docid'] , 'esdid' => $esdid));
                        $result = $modelDocumento->save();

//                        ver($result , $esdid, d);
                    }
                }
            }

            $db->commit();

            echo $idEntregavel;
        }
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
            $list = $modelEntregavel->getAllByProgramAndSprint($prgid , $sptid);
        } else {
            $list = '';
        }
        
        $modelEntStatus = new Model_Entstatus();
        $this->view->status = $modelEntStatus->getAllStatus();
        $this->view->list = $list;
        $this->render(__CLASS__, __FUNCTION__);
    }

}
