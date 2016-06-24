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
class Controller_Postit extends Abstract_Controller
{
    public function defaultAction()
    {
//        $this->formAction();
        $this->render(__CLASS__, __FUNCTION__);
    }
    
    public function formAction()
    {
        $model = new Model_Entregavel();
        $modelPrograma = new Model_Programa();
        $modelStatus = new Model_Entstatus();
        
        $dataForm = $this->getPost('dataForm');
        $model->entity['subprgid']['value'] = '';
        $model->entity['prgid']['value'] = '';
		$id = false;
		if(isset($dataForm['id'])){
			$id = (int)$dataForm['id'];
		}
        if( !empty($id) ){
            $model->populateEntity(array('entid' => $dataForm['id']));
            if($model->entity['estid']['value']){
                
                $dataStory = $model->getProgramSubProgramByStory($model->entity['estid']['value']);
                
                $model->entity['prgid']['value'] = $dataStory['prgid'];
                $model->entity['subprgid']['value'] = $dataStory['subprgid'];
                $model->entity['prgid']['value'] = $dataStory['prgid'];
            }
        } else {
            // Pegando o programa pelo cockie
            if (isset($_COOKIE["prgid"])) {
                $model->entity['prgid']['value'] = $_COOKIE["prgid"];
            } else {
                $prgid = '';
            }
        }
        
        
        $this->view->status = $modelStatus->getAll();
        $this->view->entity = $model->entity;
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
        $model = new Model_Entregavel();
        $model->populateEntity($_POST);
        
        if(!$model->entity['entid']['value']){
            $model->entity['entdtcad']['value'] = 'now()';
            $model->entity['entordsprint']['value'] = '1';
        } else {
            $model->entity['usucpfsol']['value'] = $_SESSION['usucpf'];
        }
        
//        ver( $model->getEntityValues(), d);
        
        $id = $model->save();
        
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
        
        $model = new Model_Entregavel(false);
        $model->populateEntity(array( 'entid' => $id));
        $result = $model->delete();
            
        if($result){
            $return = array('status' => true , 'msg' => utf8_encode('Entregável deletada com sucesso!'), 'result' => '');
        } else {
            $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar este entregável!'), 'result' => '');
        }
        
        echo simec_json_encode($return);
    }
    
    
    
    public function salvarFormularioModalAction()
    {
        $modelEntregavel = new Model_Entregavel(false);
        $modelDemanda = new Model_Demanda(false);
        $modelRegistroHorasDeAtendimento = new Model_Registroshorasdeatendimento(false);
        $modelDocumento = new Model_Documento(false);

        // POPULANDO Entregavel
        $modelEntregavel->populateEntity($_POST);
        
//        subprgdsc esttitulo
        
        if($modelEntregavel->entity['enthrsexec']['value'] > $this->getPost('qtdMaxHoraSprint'))
        
        // POPULANDO Demanda
        $demanda = array();
        $demanda['dmdid'] = $modelEntregavel->entity['dmdid']['value'];
        $demanda['usucpfdemandante'] = $modelEntregavel->entity['usucpfsol']['value'];
        $demanda['usucpfanalise'] = $modelEntregavel->entity['usucpfsol']['value'];
        $demanda['usucpfexecutor'] = $modelEntregavel->entity['usucpfresp']['value'];
        $demanda['docid'] = '';
        $demanda['usucpfinclusao'] = $this->user()->cpf;
//        $demanda['dmdidorigem'] = '1'; // Origem Sistemas de informações - ordid
        $demanda['tipid'] = '905'; // Tipo da demanda - Evolutiva - tipodemanda
        $demanda['celid'] = '2'; // Celula B - Daniel Brito - Gerente DANIEL AREAS BRITO
        $demanda['necessidade'] = $modelEntregavel->entity['entdsc'];
        $demanda['atendimentoRemoto'] = 'f';
        $demanda['dmdatendurgente'] = 'f';
        $demanda['dmdjudicial'] = 'f';
        $demanda['unaid'] = '15'; // DTI - Diretoria de Tecnologia da Informação
        $demanda['lcaid'] = '3'; // Edifício Anexo II
        $demanda['laaid'] = '15'; // 3º andar
        $demanda['dmdsalaatendimento'] = '315'; // Sala 315 - Equipe SiMEC
        $demanda['usdgabinete'] = 'f';
        $demanda['usdramal'] = '9902'; // Ramal do Raj Kkkk...
        $demanda['usdramal'] = '9902'; // 
        $demanda['dmddsc'] = $this->getPost('entdsc'); //  
        $demanda['dmddatainclusao'] = date('d/m/Y H:i:s');
        $demanda['dmdstatus'] = 'A';
        $demanda['dmdhorarioatendimento'] = 'C'; // Padrao do DEMANDAS - Sem comentario no codigo
        $demanda['dmddatainiprevatendimento'] =  trim($this->getPost('dmddatainiprevatendimento') . ' ' . $this->getPost('hiniatendimento'));
        $demanda['dmddatafimprevatendimento'] = trim($this->getPost('dmddatafimprevatendimento') . ' ' . $this->getPost('hfimatendimento'));
        $demanda['priid'] = $this->getPost('priid');
        $demanda['dmdclassificacao'] = $this->getPost('dmdclassificacao');
        $demanda['dmdclassificacaosistema'] = $this->getPost('dmdclassificacaosistema');
        
        // Recuperando o sidId
        if($modelEntregavel->entity['estid']['value']){
            
            $entregavel = $modelEntregavel->pegarTudoPorEstoria($modelEntregavel->entity['estid']['value']);
            
            if($entregavel){
                $demanda['sidid'] = $entregavel['sidid'];
                $demanda['dmdtitulo'] = utf8_encode($entregavel['subprgdsc'] . ' - ' . $entregavel['esttitulo']); //"Subprograma - Titulo estoria"
            }
        } 
        
        
        $modelDemanda->populateEntity($demanda);
        
//        ver( $modelDemanda->getEntityValues(), d);
        // Se nao tiver documento cadastra um novo para esta demanda
        if(empty($modelDemanda->entity['docid']['value'])){
            
            // POPULANDO docid
            $tpdid = 35; // DEMANDA_WORKFLOW_GENERICO
            $esdid = 108; // Em atendimento
            $docdsc = "SCRUM";
            $modelDocumento->populateEntity(array('tpdid' => $tpdid,'docdsc' => $docdsc, 'esdid' => $esdid));
            $docId = $modelDocumento->save();
            $modelDemanda->entity['docid']['value'] = $docId;
        }

        //verifica alteração na data fim
        if($_POST['dmddatafimprevatendimento_old'])
        {
            $datafim_old = $_POST['dmddatafimprevatendimento_old'] ." ". substr($_POST['hfimatendimento_old'], 0, 5);

            $dia = substr($_POST['dmddatafimprevatendimento_old'], 0, 2);
            $mes = substr($_POST['dmddatafimprevatendimento_old'], 3, 2);
            $ano = substr($_POST['dmddatafimprevatendimento_old'], 6, 4);

            if( ($datafim_old) &&
                ( $datafim_old != $_POST['dmddatafimprevatendimento']." ".substr($_POST['hfimatendimento'], 0, 5) ) &&
                (int)date("Ymd") >= (int)($ano.$mes.$dia) ){

                $remetente = array("nome"=>"Módulo Demandas", "email"=>$_SESSION['email_sistema']);
                $emailTec = $_SESSION['email_sistema'];
//                $emailCopia = $_SESSION['email_sistema'];

                // Seta assunto
                $assunto  = "SCRUM - Demanda [{$demanda['dmdid']}] – Alteração de demanda atrasada pelo Analista: ".utf8_decode($_POST['analista']);

                // Seta Conteúdo
                $conteudo = "SCRUM - Demanda [{$demanda['dmdid']}] foi alterada a data de previsão de término pelo Analista: ".utf8_decode($_POST['analista']);
                $conteudo .= "<br><br><b>Assunto:</b> ". $entregavel['subprgdsc'] . " - " . $entregavel['esttitulo'];
                $conteudo .= "<br><b>Descrição:</b> ". utf8_decode($this->getPost('entdsc'));
                $conteudo .= "<br><b>Programador Responsável:</b> ". utf8_decode($_POST['programador']);
                $conteudo .= "<br><br><b>Previsão de início do atendimento:</b> ". $this->getPost('dmddatainiprevatendimento_old') . " " . substr($this->getPost('hiniatendimento_old'), 0, 5) . " ---> " . $this->getPost('dmddatainiprevatendimento') . " " . $this->getPost('hiniatendimento');
                $conteudo .= "<br><b>Previsão de término do atendimento:</b> ". $this->getPost('dmddatafimprevatendimento_old') . " " . substr($this->getPost('hfimatendimento_old'), 0, 5) . " ---> " . $this->getPost('dmddatafimprevatendimento') . " " . $this->getPost('hfimatendimento');

                enviar_email( $remetente, $emailTec, $assunto, $conteudo, $emailCopia );

            }

        }

        // POPULANDO RegistroHorasDeAtendimento
//        $modelRegistroHorasDeAtendimento->populateEntity($_POST);
        
        // Voltando transacao com o banco e todas as tabelas
        if(empty($modelDemanda->entity['docid']['value'])){
            $modelDocumento->rollback();
            
            $return = array('status' => false , 'msg' => utf8_encode('Não pode salvar um documento para esta demanda!'), 'result' => '');
        } else {
            if(empty($modelEntregavel->entity['entdtcad']['value'])){
                $modelEntregavel->entity['entdtcad']['value'] = $demanda['dmddatainclusao'] = date('Y-m-d H:i:s');
            }
            
            $idDemanda = $modelDemanda->save();
            
            if($idDemanda){
                $modelEntregavel->entity['dmdid']['value'] = $idDemanda;
            }
            
            $modelEntregavel->entity['entdsc']['value'] = trim($modelEntregavel->entity['entdsc']['value']);
            $idEntregavel = $modelEntregavel->save();
            
            
            if($idEntregavel && $idDemanda){
                $modelDocumento->commit();
                $modelEntregavel->commit();
                $modelDemanda->commit();
                $return = array('status' => true , 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'idEntregavel = ' . $idEntregavel . ' - idDemanda = ' . $idDemanda);
            } else {
                $modelDocumento->rollback();
                $modelEntregavel->rollback();
                $modelDemanda->rollback();

//                $arrError = $modelEntregavel->error + $modelDemanda->error + $modelDocumento->error;
                $arrError = array();
                if($modelEntregavel->error){
//                    foreach($modelEntregavel->error as &$erro){
//                        $erro['msg'] = utf8_encode($erro['msg']);
//                    }
                    $arrError += $modelEntregavel->error;
                }
                if($modelDemanda->error){
//                    foreach($modelDemanda->error as &$erro){
//                        $erro['msg'] = utf8_encode($erro['msg']);
//                    }
                    $arrError += $modelDemanda->error;
                }
                if($modelDocumento->error) {
//                    foreach($modelDocumento->error as &$erro){
//                        $erro['msg'] = utf8_encode($erro['msg']);
//                    }
                    $arrError += $modelDocumento->error;
                }
                $return = array('status' => false , 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $arrError);
            }
            
        }

        echo simec_json_encode($return);
    }
    
    
    public function formularioModalAction()
    {
        //pflcod = 1042
        // 4

        $modelSprint = new Model_Sprint();
        $modelEntregavel = new Model_Entregavel();
        $modelEstoria = new Model_Estoria();
        $modelUsuarioResponsabilidade = new Model_Usuarioresponsabilidade();
        $modelStatus = new Model_Entstatus();
        $modelDemanda = new Model_Demanda();
        $modelRegistroHorasDeAtendimento = new Model_Registroshorasdeatendimento();

        if($this->getPost('entid')) {
            $modelEntregavel->populateEntity(array('entid' => $this->getPost('entid')));
            if($modelEntregavel->entity['dmdid']['value']) {
                $modelDemanda->populateEntity(array('dmdid' => $modelEntregavel->entity['dmdid']['value']));
            }

            if(!is_dir(APPRAIZ . 'arquivos/scrum/')) {
                mkdir(APPRAIZ . 'arquivos/scrum/', 0777);
//                ver('Não existe ' . APPRAIZ . 'arquivos/scrum/');
            } else {
//                ver('Existe ' . APPRAIZ . 'arquivos/scrum/');
            }

            if(!is_dir(APPRAIZ . 'arquivos/scrum/postit/')) {
                mkdir(APPRAIZ . 'arquivos/scrum/postit/', 0777);
//                ver('Não existe ' . APPRAIZ . 'arquivos/scrum/postit/');
            } else {
//                ver('Existe ' . APPRAIZ . 'arquivos/scrum/postit/');
            }

            if(!is_dir(APPRAIZ . 'arquivos/scrum/postit/' . $this->getPost('entid'))) {
                mkdir(APPRAIZ . 'arquivos/scrum/postit/' . $this->getPost('entid'), 0777);
//                ver('Não existe ' . APPRAIZ . 'arquivos/scrum/' . $this->getPost('entid'));
            } else {
//                ver('Existe ' . APPRAIZ . 'arquivos/scrum/' . $this->getPost('entid'));
            }

            $_SESSION['BOOTSTRAP_FILE_UPLOAD'] = array();
//            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['url'] =  'http://' .$_SERVER['SERVER_NAME'] . '/scrum/files/postit/' . $this->getPost('entid') . '/';
//            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['dir'] =  APPRAIZ . 'www/scrum/files/postit/' . $this->getPost('entid') . '/';
            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['url'] =  'http://' .$_SERVER['SERVER_NAME'] . '/scrum/galeria.php?file=postit/' . $this->getPost('entid') . '/';
            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['dir'] =  APPRAIZ . 'arquivos/scrum/postit/' . $this->getPost('entid') . '/';


        } else {
            if(isset($_SESSION['BOOTSTRAP_FILE_UPLOAD'])){
                unset($_SESSION['BOOTSTRAP_FILE_UPLOAD']);
            }
        }

//        ver(is_dir(APPRAIZ . 'www/scrum/files/') , $_SESSION['BOOTSTRAP_FILE_UPLOAD']['dir'], 'teste', d);
//        exit;

        if(!empty($modelEntregavel->entity['sptid']['value'])){
            $sprint = $modelSprint->getByValues(array('sptid' => $modelEntregavel->entity['sptid']['value']));
            $this->dateConvert($sprint['sptiniequipecio']);
            $this->dateConvert($sprint['sptfim']);
        }

        $this->dateTimeConvert($modelDemanda->entity['dmddatainiprevatendimento']['value']);
        $this->dateTimeConvert($modelDemanda->entity['dmddatafimprevatendimento']['value']);

        $dataInicio = explode(' ', $modelDemanda->entity['dmddatainiprevatendimento']['value']);
        $dataFim = explode(' ', $modelDemanda->entity['dmddatafimprevatendimento']['value']);
//            $this->dateConvert($dataFim[0]);

        $modelDemanda->entity['dmddatainiprevatendimento']['value'] = $dataInicio[0];
        $modelDemanda->entity['dmddatafimprevatendimento']['value'] = $dataFim[0];
        $modelDemanda->entity['hiniatendimento']['value'] = $dataInicio[1];
        $modelDemanda->entity['hfimatendimento']['value'] = $dataFim[1];
        
        if($modelEntregavel->entity['dmdid']['value']){
            if(!$modelDemanda->entity['priid']['value']) $modelDemanda->entity['priid']['value'] = 1;
            if(!$modelDemanda->entity['dmdclassificacao']['value']) $modelDemanda->entity['dmdclassificacao']['value'] = 'P';
        } else {
            $modelDemanda->entity['priid']['value'] = 1;
            $modelDemanda->entity['dmdclassificacao']['value'] = 'P';
        }
        
        $solicitante = $modelEntregavel->carregarUsuarioPorCPF($modelEntregavel->entity['usucpfsol']['value']);
        $equipe = $modelUsuarioResponsabilidade->carregarEquipe($_COOKIE["prgid"]);
        if(!$equipe) $equipe = array();
        
        $status = $modelStatus->getAll();
        $prioridades = $modelDemanda->carregarPrioridades();
        $classificacao = $modelDemanda->carregarClassificacao();
        $tipoDemanda = $modelDemanda->carregarTipoDemanda();
        
        $this->dateTimeConvert($modelEntregavel->entity['entdtcad']['value']);
        
        $estorias = $modelEstoria->getAllStoryProgramByStory($modelEntregavel->entity['entid']['value']);
        
        $this->view->entity = $modelEntregavel->entity;
        $this->view->entityRegistroHorasDeAtendimento = $modelRegistroHorasDeAtendimento->entity;
        $this->view->entityDemanda = $modelDemanda->entity;
        $this->view->solicitante = $solicitante;
        $this->view->sprint = $sprint;
        $this->view->estoria = $estorias;
        $this->view->equipe = $equipe;
        $this->view->status = $status;
        $this->view->prioridades = $prioridades;
        $this->view->classificacao = $classificacao;
        $this->view->tipoDemanda = $tipoDemanda;
        
        if($modelEntregavel->entity['enthrsexec']['value'] && $this->getPost('qtdMaxHoraSprint')){
            $qtdMaxHoraSprint = $this->getPost('qtdMaxHoraSprint') - $modelEntregavel->entity['enthrsexec']['value'];
        } else {
            $qtdMaxHoraSprint = '';
        }
        
        $this->view->qtdMaxHoraSprint = $qtdMaxHoraSprint;
        $this->render( __CLASS__, __FUNCTION__ );
    }

    public function detailModalAction()
    {
        //pflcod = 1042
        // 4

        $modelSprint = new Model_Sprint();
        $modelEntregavel = new Model_Entregavel();
        $modelEstoria = new Model_Estoria();
        $modelUsuarioResponsabilidade = new Model_Usuarioresponsabilidade();
        $modelStatus = new Model_Entstatus();
        $modelDemanda = new Model_Demanda();
        $modelRegistroHorasDeAtendimento = new Model_Registroshorasdeatendimento();

        if($this->getPost('entid')) {
            $modelEntregavel->populateEntity(array('entid' => $this->getPost('entid')));
            if($modelEntregavel->entity['dmdid']['value']) {
                $modelDemanda->populateEntity(array('dmdid' => $modelEntregavel->entity['dmdid']['value']));
            }

            if(!is_dir(APPRAIZ . 'arquivos/scrum/')) {
                mkdir(APPRAIZ . 'arquivos/scrum/', 0777);
//                ver('Não existe ' . APPRAIZ . 'arquivos/scrum/');
            } else {
//                ver('Existe ' . APPRAIZ . 'arquivos/scrum/');
            }

            if(!is_dir(APPRAIZ . 'arquivos/scrum/postit/')) {
                mkdir(APPRAIZ . 'arquivos/scrum/postit/', 0777);
//                ver('Não existe ' . APPRAIZ . 'arquivos/scrum/postit/');
            } else {
//                ver('Existe ' . APPRAIZ . 'arquivos/scrum/postit/');
            }

            if(!is_dir(APPRAIZ . 'arquivos/scrum/postit/' . $this->getPost('entid'))) {
                mkdir(APPRAIZ . 'arquivos/scrum/postit/' . $this->getPost('entid'), 0777);
//                ver('Não existe ' . APPRAIZ . 'arquivos/scrum/' . $this->getPost('entid'));
            } else {
//                ver('Existe ' . APPRAIZ . 'arquivos/scrum/' . $this->getPost('entid'));
            }

            $_SESSION['BOOTSTRAP_FILE_UPLOAD'] = array();
//            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['url'] =  'http://' .$_SERVER['SERVER_NAME'] . '/scrum/files/postit/' . $this->getPost('entid') . '/';
//            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['dir'] =  APPRAIZ . 'www/scrum/files/postit/' . $this->getPost('entid') . '/';
            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['url'] =  'http://' .$_SERVER['SERVER_NAME'] . '/scrum/galeria.php?file=postit/' . $this->getPost('entid') . '/';
            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['dir'] =  APPRAIZ . 'arquivos/scrum/postit/' . $this->getPost('entid') . '/';


        } else {
            if(isset($_SESSION['BOOTSTRAP_FILE_UPLOAD'])){
                unset($_SESSION['BOOTSTRAP_FILE_UPLOAD']);
            }
        }

//        ver(is_dir(APPRAIZ . 'www/scrum/files/') , $_SESSION['BOOTSTRAP_FILE_UPLOAD']['dir'], 'teste', d);
//        exit;

        if(!empty($modelEntregavel->entity['sptid']['value'])){
            $sprint = $modelSprint->getByValues(array('sptid' => $modelEntregavel->entity['sptid']['value']));
            $this->dateConvert($sprint['sptiniequipecio']);
            $this->dateConvert($sprint['sptfim']);
        }

        $this->dateTimeConvert($modelDemanda->entity['dmddatainiprevatendimento']['value']);
        $this->dateTimeConvert($modelDemanda->entity['dmddatafimprevatendimento']['value']);

        $dataInicio = explode(' ', $modelDemanda->entity['dmddatainiprevatendimento']['value']);
        $dataFim = explode(' ', $modelDemanda->entity['dmddatafimprevatendimento']['value']);
//            $this->dateConvert($dataFim[0]);

        $modelDemanda->entity['dmddatainiprevatendimento']['value'] = $dataInicio[0];
        $modelDemanda->entity['dmddatafimprevatendimento']['value'] = $dataFim[0];
        $modelDemanda->entity['hiniatendimento']['value'] = $dataInicio[1];
        $modelDemanda->entity['hfimatendimento']['value'] = $dataFim[1];

        if($modelEntregavel->entity['dmdid']['value']){
            if(!$modelDemanda->entity['priid']['value']) $modelDemanda->entity['priid']['value'] = 1;
            if(!$modelDemanda->entity['dmdclassificacao']['value']) $modelDemanda->entity['dmdclassificacao']['value'] = 'P';
        } else {
            $modelDemanda->entity['priid']['value'] = 1;
            $modelDemanda->entity['dmdclassificacao']['value'] = 'P';
        }

        $solicitante = $modelEntregavel->carregarUsuarioPorCPF($modelEntregavel->entity['usucpfsol']['value']);
        $equipe = $modelUsuarioResponsabilidade->carregarEquipe($_COOKIE["prgid"]);
        if(!$equipe) $equipe = array();

        $status = $modelStatus->getAll();
        $prioridades = $modelDemanda->carregarPrioridades();
        $classificacao = $modelDemanda->carregarClassificacao();
        $tipoDemanda = $modelDemanda->carregarTipoDemanda();

        $this->dateTimeConvert($modelEntregavel->entity['entdtcad']['value']);

        $estorias = $modelEstoria->getAllStoryProgramByStory($modelEntregavel->entity['entid']['value']);

        $this->view->entity = $modelEntregavel->entity;
        $this->view->entityRegistroHorasDeAtendimento = $modelRegistroHorasDeAtendimento->entity;
        $this->view->entityDemanda = $modelDemanda->entity;
        $this->view->solicitante = $solicitante;
        $this->view->sprint = $sprint;
        $this->view->estoria = $estorias;
        $this->view->equipe = $equipe;
        $this->view->status = $status;
        $this->view->prioridades = $prioridades;
        $this->view->classificacao = $classificacao;
        $this->view->tipoDemanda = $tipoDemanda;

        if($modelEntregavel->entity['enthrsexec']['value'] && $this->getPost('qtdMaxHoraSprint')){
            $qtdMaxHoraSprint = $this->getPost('qtdMaxHoraSprint') - $modelEntregavel->entity['enthrsexec']['value'];
        } else {
            $qtdMaxHoraSprint = '';
        }

        $this->view->qtdMaxHoraSprint = $qtdMaxHoraSprint;
        $this->render( __CLASS__, __FUNCTION__ );
    }

    public function ultimaDemandaAction()
    {
        $modelUsuarioResponsabilidade = new Model_Usuarioresponsabilidade();
        $ultimaDemanda = $modelUsuarioResponsabilidade->carregarUltimaDemanda($this->getPost('cpf'));
        
        $this->dateTimeConvert($ultimaDemanda['dmddatainiprevatendimento']);
        $this->dateTimeConvert($ultimaDemanda['dmddatafimprevatendimento']);
        if($ultimaDemanda){
            foreach($ultimaDemanda as &$value) $value = utf8_encode($value);
        }
        
        echo simec_json_encode($ultimaDemanda);
    }
    
    public function changeSprintAction()
    {
        global $db;
                
        $sql = '';
        if($_POST['arrPostitId']){

            if(!$_POST['idsprint']) $_POST['idsprint'] = 'null';

            $editar = false;
            foreach($_POST['arrPostitId'] as $key => $idPostit){

                if($idPostit == $_POST['idpostit']) $editar = true;

                $ordem = $key + 1;

                if($editar){
                    $sql .= "UPDATE scrum.entregavel SET sptid = {$_POST['idsprint']} , entordsprint  = {$ordem} WHERE entid = {$idPostit} RETURNING entid; ";
                }
            }
        }

        $idEntregavel = $db->pegaUm($sql);
        $db->commit();

        echo $idEntregavel;
    }

}