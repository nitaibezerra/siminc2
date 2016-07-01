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
class Controller_Instituicao extends Abstract_Controller
{
    public function indexAction()
    {
        $this->formularioAction();
    }

    public function pesquisarAction()
    {
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function formularioAction()
    {
        $model = new Model_Instituicao();
        $model->populateEntity(array('intid' => $_SESSION['instituicao']['intid']));
        $this->view->entity = $model->entity;

        $id = $_SESSION['instituicao']['intid'];
        if($id) {

            if(!is_dir(APPRAIZ . 'arquivos/pdu/')) {
                mkdir(APPRAIZ . 'arquivos/pdu/', 0777);
//                ver('Não existe ' . APPRAIZ . 'arquivos/scrum/');
            } else {
//                ver('Existe ' . APPRAIZ . 'arquivos/scrum/');
            }

            if(!is_dir(APPRAIZ . 'arquivos/pdu/instituicao/')) {
                mkdir(APPRAIZ . 'arquivos/pdu/instituicao/', 0777);
//                ver('Não existe ' . APPRAIZ . 'arquivos/scrum/postit/');
            } else {
//                ver('Existe ' . APPRAIZ . 'arquivos/scrum/postit/');
            }

            if(!is_dir(APPRAIZ . 'arquivos/pdu/instituicao/' . $id)) {
                mkdir(APPRAIZ . 'arquivos/pdu/instituicao/' . $id, 0777);
//                ver('Não existe ' . APPRAIZ . 'arquivos/scrum/' . $this->getPost('entid'));
            } else {
//                ver('Existe ' . APPRAIZ . 'arquivos/scrum/' . $this->getPost('entid'));
            }

            $_SESSION['BOOTSTRAP_FILE_UPLOAD'] = array();
//            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['url'] =  'http://' .$_SERVER['SERVER_NAME'] . '/scrum/files/postit/' . $this->getPost('entid') . '/';
//            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['dir'] =  APPRAIZ . 'www/scrum/files/postit/' . $this->getPost('entid') . '/';
            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['url'] =  'http://' .$_SERVER['SERVER_NAME'] . '/pdu/galeria.php?file=instituicao/' . $id . '/';
            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['dir'] =  APPRAIZ . 'arquivos/pdu/instituicao/' . $id . '/';


        } else {
            if(isset($_SESSION['BOOTSTRAP_FILE_UPLOAD'])){
                unset($_SESSION['BOOTSTRAP_FILE_UPLOAD']);
            }
        }




        $this->render(__CLASS__, __FUNCTION__);
    }

    public function salvarAction()
    {
        $model = new Model_Instituicao();

        $_POST['intdtinclusao'] = 'now()';


        $_POST['intcnpj'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['intcnpj']);
        $_POST['intfonecomercial'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['intfonecomercial']);


        $_POST['intcep'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['endcep1']);
        $_POST['estuflogradouro'] = $_POST['estuf1'];
        $_POST['muncodlogradouro'] = $_POST['muncod1'];
        $_POST['intbairrologradouro'] = $_POST['endbai1'];
        $_POST['intlatitude'] = implode('.' , $_POST['latitude']);
        $_POST['intlongitude'] = implode('.' , $_POST['longitude']);

//ver($_POST['muncodlogradouro']);

        $model->populateEntity($_POST);
        $id = $model->save($_POST);

        if($model->error){
            foreach($model->error as &$error){
                if($error['name'] == 'intcep' ){
                    $error['name'] = 'endcep1' ;
                }
                if($error['name'] == 'intlatitude' ){
                    $error['name'] = 'latitude' ;
                }
                if($error['name'] == 'intlongitude' ){
                    $error['name'] = 'longitude' ;
                }
            }

            $return = array('status' => false , 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $model->error);
        } else {
            $return = array('status' => true , 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $id);
        }

        echo simec_json_encode($return);
    }

    public function listarAction()
    {
        $intid = $this->getPost('intid');
        $estuf = $this->getPost('estuf');

        $where = array();
        if($intid) $where[] = "int.intid = {$intid}";
        if($estuf) $where[] = "int.estuflogradouro = '{$estuf}' ";

        if( $where && count($where) > 0 ) {
            $where = ' WHERE ' . implode(' AND ' , $where );
        } else {
            $where = '';
        }

        $data = "
            SELECT int.intid , '<b>' || intdscsigla || ' - ' || int.intdscrazaosocial || '</b>', mun.mundescricao , int.estuflogradouro
            FROM pdu.instituicao int
            INNER JOIN territorios.municipio mun ON (mun.muncod = int.muncodlogradouro)
            {$where}
            ORDER BY int.estuflogradouro
            ";

        $listing = new Listing();
//    $listing->setTypePage('M');
//    $listing->setPerPage(5);
//    $listing->setPageNumber(7);
//    $listing->setPage(25);
        $listing->setHead(array('Instituição', 'Município', 'UF'));
        $listing->setActions(array('plus' => 'abrir'));
        $listing->listing($data);
//        ver($_POST, d);
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function listarDadosAction()
    {
//        $data = "SELECT
//              int.intid
//              , int.intdscrazaosocial , mun.mundescricao , int.estuflogradouro
//            FROM pdu.instituicao int
//            INNER JOIN territorios.municipio mun ON (mun.muncod = int.muncodlogradouro)";
//
//        $listing = new Listing();
////    $listing->setTypePage('M');
////    $listing->setPerPage(10);
////    $listing->setPageNumber(15);
////    $listing->setPage(25);
//        $listing->setHead(array('Instituição', 'Município', 'UF'));
//        $listing->setActions(array('plus' => 'abrir'));
//        $listing->listing($data);

        $this->view->id = $this->getPost('id');
        $this->render(__CLASS__, __FUNCTION__);
    }
}