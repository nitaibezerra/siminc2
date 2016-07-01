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
class Controller_Campus extends Abstract_Controller
{

    public function indexAction()
    {
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function formularioAction()
    {
        $model = new Model_Instituicaocampus();
        $modelTipoExistencia = new Model_Tipoexistencia();
        $tiposExistencia = $modelTipoExistencia->getAll(null , array('tecdsc'));

        $this->view->tiposExistencia = $tiposExistencia;


        $id = $this->getPost('id');
        $model->populateEntity(array( 'cmpid' => $id));
        $this->dateTimeConvert($model->entity['cmpdtinauguracao']['value']);
        $this->view->entity = $model->entity;
        if($id) {
            if(!is_dir(APPRAIZ . 'arquivos/pdu/')) {
                mkdir(APPRAIZ . 'arquivos/pdu/', 0777);
//                ver('Não existe ' . APPRAIZ . 'arquivos/scrum/');
            } else {
//                ver('Existe ' . APPRAIZ . 'arquivos/scrum/');
            }

            if(!is_dir(APPRAIZ . 'arquivos/pdu/campus/')) {
                mkdir(APPRAIZ . 'arquivos/pdu/campus/', 0777);
//                ver('Não existe ' . APPRAIZ . 'arquivos/scrum/postit/');
            } else {
//                ver('Existe ' . APPRAIZ . 'arquivos/scrum/postit/');
            }

            if(!is_dir(APPRAIZ . 'arquivos/pdu/campus/' . $id)) {
                mkdir(APPRAIZ . 'arquivos/pdu/campus/' . $id, 0777);
//                ver('Não existe ' . APPRAIZ . 'arquivos/scrum/' . $this->getPost('entid'));
            } else {
//                ver('Existe ' . APPRAIZ . 'arquivos/scrum/' . $this->getPost('entid'));
            }

            $_SESSION['BOOTSTRAP_FILE_UPLOAD'] = array();
//            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['url'] =  'http://' .$_SERVER['SERVER_NAME'] . '/scrum/files/postit/' . $this->getPost('entid') . '/';
//            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['dir'] =  APPRAIZ . 'www/scrum/files/postit/' . $this->getPost('entid') . '/';
            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['url'] =  'http://' .$_SERVER['SERVER_NAME'] . '/pdu/galeria.php?file=campus/' . $id . '/';
            $_SESSION['BOOTSTRAP_FILE_UPLOAD']['dir'] =  APPRAIZ . 'arquivos/pdu/campus/' . $id . '/';
        } else {
            if(isset($_SESSION['BOOTSTRAP_FILE_UPLOAD'])){
                unset($_SESSION['BOOTSTRAP_FILE_UPLOAD']);
            }
        }


//ver($model->entity , $this->getPost('id'), d);
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function listarAction()
    {
        $listing = new Listing();
//    $listing->setTypePage('M');

//    $listing->setPageNumber(5);
//    $listing->setPage(10);

        $listing->setHead(array('Campus', 'Municipio', 'UF'));
//$listing->enableCount(true);

        $id = $this->getPost('id');
        $estuf = $this->getPost('estuf');

        $where = array();
//        if($id) $where[] = "cmp.cmpid = {$id}";
        if($estuf) $where[] = "cmp.estuflogradouro = '{$estuf}' ";

        if( $where && count($where) > 0 ) {
            $where = implode(' AND ' , $where );
        } else {
            $where = '';
        }

        if(!$id){

            $id = $_SESSION['instituicao']['intid'];
            $this->view->exibirTitulo = true;


        } else {
            $this->view->exibirTitulo = true;
//            $data = "SELECT
//                   --cmp.cmpid,
//                   cmp.cmpdscrazaosocial , mun.mundescricao , cmp.estuflogradouro
//                FROM pdu.instituicaocampus cmp
//                LEFT JOIN territorios.municipio mun ON (mun.muncod = cmp.muncodlogradouro)
//                WHERE cmp.intid = {$id}
//                AND cmptipo = 'C'
//                {$where}";
        }

        if($id){
            $listing->setActions(array('edit' => 'editar' , 'delete' => 'excluir'));
            $data = "SELECT
                               cmp.cmpid,
                               cmp.cmpdscrazaosocial , mun.mundescricao , cmp.estuflogradouro
                            FROM pdu.instituicaocampus cmp
                            LEFT JOIN territorios.municipio mun ON (mun.muncod = cmp.muncodlogradouro)
                            WHERE cmp.intid = {$id}
                            AND cmptipo = 'C'
                            {$where}";
        } else {
            $data = array();
        }

        $this->view->data = $data;
        $this->view->listing = $listing;

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function listarCampiReitoriaAction()
    {
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function listarreitoriaAction()
    {
        $listing = new Listing();
//    $listing->setTypePage('M');
//    $listing->setPageNumber(5);
//    $listing->setPage(10);
        $listing->setHead(array('Campus', 'Municipio', 'UF'));
//$listing->enableCount(true);

        $id = $this->getPost('id');

        if($id){
            $estuf = $this->getPost('estuf');
        } else {
            $id = $_SESSION['instituicao']['intid'];
        }

        $listing->setActions(array('edit' => 'editar' , 'delete' => 'excluir'));

        if($id){
            $data = "SELECT
                       cmp.cmpid,
                       cmp.cmpdscrazaosocial , mun.mundescricao , cmp.estuflogradouro
                    FROM pdu.instituicaocampus cmp
                    LEFT JOIN territorios.municipio mun ON (mun.muncod = cmp.muncodlogradouro)
                    WHERE cmp.intid = {$id}
                    AND cmptipo = 'R'";
        } else {
            $data = false;
        }

        $this->view->data = $data;
        $this->view->listing = $listing;

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function salvarAction()
    {
        $model = new Model_Instituicaocampus();

        $_POST['cmpdtinclusao'] = 'now()';
        $_POST['cmpcnpj'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['cmpcnpj']);
        $_POST['cmpfonecomercial'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['cmpfonecomercial']);
        $_POST['cmpstatus'] = 'A';
        $_POST['intid'] = $_SESSION['instituicao']['intid'];
//        $_POST['cmpareatotal'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['cmpareatotal']);
//        $_POST['cmpareaconstgeral'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['cmpareaconstgeral']);
//        $_POST['cmpareaconstlab'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['cmpareaconstlab']);
//        $_POST['cmpareaconstsala'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['cmpareaconstsala']);


        $_POST['cmpcep'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['endcep1']);
        $_POST['estuflogradouro'] = $_POST['estuf1'];
        $_POST['muncodlogradouro'] = $_POST['muncod1'];
        $_POST['cmpbairrologradouro'] = $_POST['endbai1'];
        $_POST['cmplatitude'] = implode('.' , $_POST['latitude']);
        $_POST['cmplongitude'] = implode('.' , $_POST['longitude']);

//        ver($this->getPost('muncodlogradouro'), d);
        $model->populateEntity($_POST);
//        ver($model->entity, d);

        $id = $model->save($_POST);

        if($model->error){

            foreach($model->error as &$error){
                if($error['name'] == 'cmpcep' ){
                    $error['name'] = 'endcep1' ;
                }
                if($error['name'] == 'cmplatitude' ){
                    $error['name'] = 'latitude' ;
                }
                if($error['name'] == 'cmplongitude' ){
                    $error['name'] = 'longitude' ;
                }
            }

            $return = array('status' => false , 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $model->error);
        } else {
            $return = array('status' => true , 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $id);
        }

        echo simec_json_encode($return);
    }

    public function deletarAction()
    {
        $id = $this->getPost('id');

        $model = new Model_Instituicaocampus(false);
        $model->populateEntity(array( 'cmpid' => $id));
        $result = $model->delete();

        if($result){
            $return = array('status' => true , 'msg' => utf8_encode('Deletado com sucesso!'), 'result' => '');
        } else {
            $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar!'), 'result' => '');
        }

        echo simec_json_encode($return);
    }
}