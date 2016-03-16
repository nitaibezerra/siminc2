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
class Controller_Dirigente extends Abstract_Controller
{
    public function carregarDadosDirigentePorCpfAction()
    {
        $cpf = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['cpf']);

        $model = new Model_Dirigente();
        $dirigente = $model->getByValues(array( 'drgcpf' => $cpf));
        if($dirigente){
            echo simec_json_encode($dirigente);
        }else {
            echo 'false';
        }
    }

    public function indexAction()
    {
        global $db;

        $intid = $_SESSION['instituicao']['intid'];

        $sql = "SELECT tpd.tpdid , tpd.tpddsc , drg.drgnome, drg.drgid FROM pdu.tipodirigente tpd
                  LEFT JOIN pdu.unidadedirigente udd ON (udd.tpdid = tpd.tpdid AND udd.intid = {$intid} )
                  LEFT JOIN pdu.dirigente drg ON (udd.drgid = drg.drgid)
                ORDER BY tpd.tpdid;";
        $tiposDirigente = $db->carregar($sql);
//        ver($sql , $tiposDirigente[0]['tpdid'], d);
        $this->view->tiposDirigente = $tiposDirigente;
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function formularioAction()
    {
        global $db;

        $tpdid = $this->getPost('tpdid');
        $sql = "SELECT tpddsc FROM pdu.tipodirigente WHERE tpdid = {$tpdid};";
        $tpddsc = $db->pegaUm($sql);

        $model = new Model_Dirigente();
        $drgid = $this->getPost('drgid');
        if($drgid){
            $model->populateEntity(array('drgid' => $drgid));
        }

        $this->view->entity = $model->entity;
        $this->view->tpdid = $tpdid;
        $this->view->tpddsc = $tpddsc;
        $this->render(__CLASS__, __FUNCTION__);
    }

    /**
     *
     */
    public function salvarAction()
    {
        $modelDirigente = new Model_Dirigente();
        $modelUnidadeDirigente = new Model_Unidadedirigente();

        $_POST['drgcpf'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['drgcpf']);

        $dirigenteExistente = $modelDirigente->getByValues(array('drgcpf' => $this->getPost('drgcpf')));
        if($dirigenteExistente){
            $_POST['drgid'] = $dirigenteExistente['drgid'];
        }


        $_POST['drgfonecomercial'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['drgfonecomercial']);
        $_POST['drgfonefax'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['drgfonefax']);
        $_POST['drgfonecelular'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['drgfonecelular']);
        $_POST['drgstatus'] = 'A';
        $_POST['drgdtinclusao'] = 'now()';

        $_POST['drgcep'] = str_replace(array('/' , '.' , '-', "\\"), '', $_POST['endcep1']);
        $_POST['estuflogradouro'] = $_POST['estuf1'];
        $_POST['muncodlogradouro'] = $_POST['muncod1'];
        $_POST['drgbairrologradouro'] = $_POST['endbai1'];
        $_POST['drglatitude'] = implode('.' , $_POST['latitude']);
        $_POST['drglongitude'] = implode('.' , $_POST['longitude']);


        $intid = $_SESSION['instituicao']['intid'];
        $tpdid = $this->getPost('tpdid');

        //        ver($this->getPost('muncodlogradouro'), d);
        $modelDirigente->populateEntity($_POST);
        $idDirigente = $modelDirigente->save($_POST);

        if($idDirigente){

            // Editando a tabela associativa de dirigente com a instituicao caso ja exista algum dirigente do mesmo tipo para esta entidade.
            $unidadeDirigente = $modelUnidadeDirigente->getByValues( array( 'tpdid' => $tpdid, 'intid' => $intid ) );
            if($unidadeDirigente){
                $modelUnidadeDirigente->populateEntity(array('uddid' => $unidadeDirigente['uddid'] , 'drgid'=> $idDirigente , 'tpdid' => $tpdid , 'intid' => $intid ));
            } else {
                $modelUnidadeDirigente->populateEntity(array('drgid'=> $idDirigente , 'tpdid' => $tpdid ,  'intid' => $intid ));
            }

            $idUnidadeDirigente = $modelUnidadeDirigente->save();
        }

        if($modelDirigente->error){
            $return = array('status' => false , 'msg' => utf8_encode('Os dados não foram salvos!'), 'result' => $modelDirigente->error);
        } else {
            $return = array('status' => true , 'msg' => utf8_encode('Os dados foram salvos!'), 'result' => 'id = ' . $idDirigente);
        }

        echo simec_json_encode($return);
    }

    public function deletarAction()
    {
        $tpdid = $this->getPost('id');

        $model = new Model_Unidadedirigente( false );
        $unidadeDirigente = $model->getByValues( array( 'tpdid'=> $tpdid ) );
        $model->populateEntity( array( 'uddid' => $unidadeDirigente['uddid'] ) );
        $result = $model->delete();

        if($result){
            $return = array('status' => true , 'msg' => utf8_encode('Deletado com sucesso!'), 'result' => '');
        } else {
            $return = array('status' => false , 'msg' => utf8_encode('Não pode deletar!'), 'result' => '');
        }

        echo simec_json_encode($return);
    }
}