<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of Configuracao
 *
 * @author LucianoRibeiro
 */
class Controller_Guia extends Abstract_Controller{

    #EXCLUIR
    public function excluirDimensaoAction(){
        $id = $this->getPost('id');

        $model = new Model_Dimensao(false);
        $model->populateEntity( array( 'dimid' => $id) );
        $result = $model->delete();

        if($result){
            $return = array('status' => true , 'msg' => utf8_encode('Deletado com sucesso!'), 'result' => '');
        } else {
            $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar!'), 'result' => '');
        }

        echo simec_json_encode($return);
    }

    public function excluirAreaAction(){
        $id = $this->getPost('id');

        $model = new Model_Area(false);
        $model->populateEntity( array( 'areid' => $id) );
        $result = $model->delete();

        if($result){
            $return = array('status' => true , 'msg' => utf8_encode('Deletado com sucesso!'), 'result' => '');
        } else {
            $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar!'), 'result' => '');
        }

        echo simec_json_encode($return);
    }

    public function excluirIndicadorAction(){
        $id = $this->getPost('id');

        $model = new Model_Indicador(false);
        $model->populateEntity( array( 'indid' => $id) );
        $result = $model->delete();

        if($result){
            $return = array('status' => true , 'msg' => utf8_encode('Deletado com sucesso!'), 'result' => '');
        } else {
            $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar!'), 'result' => '');
        }

        echo simec_json_encode($return);
    }

    public function excluirCriterioAction(){
        $id = $this->getPost('id');

        $model = new Model_Criterio(false);
        $model->populateEntity( array( 'crtid' => $id) );
        $result = $model->delete();

        if($result){
            $return = array('status' => true , 'msg' => utf8_encode('Deletado com sucesso!'), 'result' => '');
        } else {
            $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar!'), 'result' => '');
        }

        echo simec_json_encode($return);
    }

    #SALVAR
    public function salvarDimensaoAction(){
        $model = new Model_Dimensao();

        $_POST['dimstatus'] = 'A';

        $model->populateEntity($_POST);

        $id = $model->save($_POST);

        if($model->error){
            $return = array('status' => false , 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $model->error);
        } else {
            $return = array('status' => true , 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $id);
        }

        echo simec_json_encode($return);
    }

    public function salvarAreaAction(){
        $model = new Model_Area();

        $_POST['arestatus'] = 'A';

        $model->populateEntity($_POST);

        $id = $model->save($_POST);

        if($model->error){
            $return = array('status' => false , 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $model->error);
        } else {
            $return = array('status' => true , 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $id);
        }

        echo simec_json_encode($return);
    }

    public function salvarIndicadorAction(){
        $model = new Model_Indicador();

        $_POST['indstatus'] = 'A';

        $model->populateEntity($_POST);

        $id = $model->save($_POST);

        if($model->error){
            $return = array('status' => false , 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $model->error);
        } else {
            $return = array('status' => true , 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $id);
        }

        echo simec_json_encode($return);
    }

    public function salvarCriterioAction(){
        $model = new Model_Criterio();

        $_POST['crtstatus'] = 'A';

        $model->populateEntity($_POST);

        $id = $model->save($_POST);

        if($model->error){
            $return = array('status' => false , 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $model->error);
        } else {
            $return = array('status' => true , 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $id);
        }

        echo simec_json_encode($return);
    }

    #RENDERISA OS FORMULARIOS
    public function formularioDimensaoAction(){
        #FORMULARIO "PAI" - CAMPOS INSTRUMENTO.
        $modelInstrumento = new Model_Instrumento();
        $modelInstrumento->populateEntity(array('itrid' => $this->getPost('itrid')));
        $this->view->entityInstrumento = $modelInstrumento->entity;

        #FORMULARIO "FILHO" - CAMPOS DIMENSÃO.
        $model = new Model_Dimensao();
        $model->populateEntity($_POST);
        $this->view->entity = $model->entity;

        #ORDEM DA DIMENSÃO
        $ordem = $model->entity['dimcod']['value'];
        if( $ordem == '' ){
            $itrid = $modelInstrumento->entity['itrid']['value'];
            $ordem = $model->getOrdemMaxima( $itrid );
        }
        $this->view->ordem = $ordem;

        #TIPO DE AÇÃO A SER TOMADA PELO FORMULÁRIO
        $this->view->tipoAcao = $_POST['tipo_acao'];

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function formularioAreaAction(){
        #FORMULARIO "PAI" - CAMPOS DIMENSÃO.
        $modelDimensao = new Model_Dimensao();
        $modelDimensao->populateEntity(array('dimid' => $this->getPost('dimid')));
        $this->view->entityDimensao = $modelDimensao->entity;

        #FORMULARIO "FILHO" - CAMPOS ÁREA.
        $model = new Model_Area();
        $model->populateEntity($_POST);
        $this->view->entity = $model->entity;

        #ORDEM DA ÁREA
        $ordem = $model->entity['arecod']['value'];
        if( $ordem == '' ){
            $dimid = $modelDimensao->entity['dimid']['value'];
            $ordem = $model->getOrdemMaxima( $dimid );
        }
        $this->view->ordem = $ordem;

        #TIPO DE AÇÃO A SER TOMADA PELO FORMULÁRIO
        $this->view->tipoAcao = $_POST['tipo_acao'];

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function formularioIndicadorAction(){
        #FORMULARIO "PAI" - CAMPOS ÁREA.
        $modelArea = new Model_Area();
        $modelArea->populateEntity(array('areid' => $this->getPost('areid')));
        $this->view->entityArea = $modelArea->entity;
        
        #FORMULARIO "FILHO" - CAMPOS INDICADOR.
        $model = new Model_Indicador();
        $model->populateEntity($_POST);
        $this->view->entity = $model->entity;
        
        #ORDEM DO INDICADOR
        $ordem = $model->entity['indcod']['value'];
        if( $ordem == '' ){
            $areid = $modelArea->entity['areid']['value'];
            $ordem = $model->getOrdemMaxima( $areid );
        }
        $this->view->ordem = $ordem;

        #TIPO DE AÇÃO A SER TOMADA PELO FORMULÁRIO
        $this->view->tipoAcao = $_POST['tipo_acao'];

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function formularioCriterioAction(){
        #FORMULARIO "PAI" - CAMPOS INDICADOR.
        $modelIndicador = new Model_Indicador();
        $modelIndicador->populateEntity(array('indid' => $this->getPost('indid')));
        $this->view->entityIndicador = $modelIndicador->entity;

        #FORMULARIO "FILHO" - CAMPOS CRITERIO.
        $model = new Model_Criterio();
        $model->populateEntity($_POST);
        $this->view->entity = $model->entity;

        #TIPO DE AÇÃO A SER TOMADA PELO FORMULÁRIO
        $this->view->tipoAcao = $_POST['tipo_acao'];

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function arvoreAction(){
        $this->render(__CLASS__, __FUNCTION__);
    }
}
