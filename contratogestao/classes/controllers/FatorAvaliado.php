<?php

/**
 * Controle inicial
 * 
 * @author Equipe simec - Consultores OEI - Junio Santos
 * @since  18/07/2014
 * 
 * @name       Controller_Default
 * @package    classes
 * @subpackage controllers
 */
require_once APPRAIZ . 'includes/workflow.php';

class Controller_FatorAvaliado extends Abstract_Controller {

    public $conid;

    public function __construct() {
        parent::__construct();
        $this->view->perfilUsuario = new Model_PerfilUsuario();
        $this->view->perfilUsuario->validaAcessoTelaContrato();
        $this->view->satisfacao = new Model_Satisfacao();

        $this->view->titulo = 'Gerenciar Fator Avaliado';
        $this->conid = $_SESSION['conid'];
    }

    public function fatorAvaliadoAction() {
        $this->view->fatorAvaliado = new Model_Fatoravaliado();
        $this->view->conformidade = new Model_Conformidade();
        $this->view->contrato = new Model_Contrato();
        $_SESSION['conid'] = (int) $this->getPost('id');

        $atividade = $this->view->contrato->getContratoById((int) $this->getPost('id'));
        $this->view->fatorAvaliado->populateEntity($atividade);
        $this->view->titulo = "Gerenciar Fator Avaliado / {$atividade['consigla']} - {$atividade['condescricao']}";
        $this->view->acao = " :: Adicionar Fator Avaliado";
        $this->view->contrato->treatEntityToUser();

        $this->view->data = $this->view->fatorAvaliado->getDados($_SESSION['conid']);
        $this->view->listing = $this->view->fatorAvaliado->getListing();

        $this->render(__CLASS__, __FUNCTION__);
        exit;
    }

    public function formEtapaControleAction() {
        $this->view->titulo = 'Cadastrar ';
        $this->view->fatorAvaliado = new Model_Fatoravaliado();
        $this->view->etapa = $this->getPost('etapa');
        $this->view->titulo = 'Cadastrar ' . ucfirst($this->getPost('etapa'));
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function getPessoasAction() {
        $this->view->fatorAvaliado = new Model_Fatoravaliado();
        $this->view->entidade = new Model_Entidade();
        $this->view->usuario = new Model_Usuario();
        $this->view->etapa = $this->getPost('etapa');
        $this->view->tipoPessoa = $this->getPost('tipoPessoa');

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function listarAction() {
        $this->view->fatorAvaliado = new Model_Fatoravaliado();
        $this->view->data = $this->view->fatorAvaliado->getDados($this->conid);
        $this->view->listing = $this->view->fatorAvaliado->getListing();
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function editarAction() {
        $this->view->fatorAvaliado = new Model_Fatoravaliado();
        $this->view->conformidade = new Model_Conformidade();
        $this->view->contrato = new Model_Contrato();

        $fatorAvaliado = $this->view->fatorAvaliado->getByValues(array('fatid' => (int) $this->getPost('id')));
        $atividade = $this->view->contrato->getContratoById((int) $fatorAvaliado['conid']);
        $this->view->titulo = "Gerenciar Fator Avaliado / {$atividade['consigla']} - {$atividade['condescricao']}";
        $this->view->acao = " :: Editar Fator Avaliado";

        $this->view->fatorAvaliado->populateEntity($fatorAvaliado);
        $this->view->fatorAvaliado->treatEntityToUser();

        $this->setPessoa();

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function excluirAction() {
        $fatorAvaliado = new Model_Fatoravaliado();
        if ($this->view->perfilUsuario->validarAcessoModificacao($_SESSION['conid']) === false) {
            $return = array('status' => false, 'msg' => self::ERRO_SEM_PERMISAO, 'type' => 'danger');
        } else {
            try {
                $fatorAvaliado->populateEntity(array('fatid' => (int) $this->getPost('id')));
                $fatorAvaliado->setAttributeValue('fatstatus', 'I');
                $fatorAvaliado->setAttributeValue('fatdsc', $fatorAvaliado->getAttributeValue('fatdsc'));
                $fatorAvaliado->treatEntityToUser();

                $fatorAvaliado->save();
                $return = array('status' => true, 'msg' => self::DADOS_EXCLUIDOS_COM_SUCESSO, 'type' => 'success');
            } catch (Exception $exc) {
                if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
                    echo $exc->getTraceAsString();
                }
                $return = array('status' => false, 'msg' => self::ERRO_AO_EXCLUIR, 'type' => 'danger');
            }
        }
        $return['msg'] = '<div class="alert alert-' . $return['type'] . '" role="alert">' . $return['msg'] . '</div>';
        echo simec_json_encode($return);
    }

    public function salvarAction() {
        $fatorAvaliado = new Model_Fatoravaliado();
        $fatorAvaliado->populateEntity($_POST);
        $fatorAvaliado->setAttributeValue('conid', $this->conid);
        //$fatorAvaliado->treatEntityToDataBase();

        if ($this->view->perfilUsuario->validarAcessoModificacao($_SESSION['conid']) === false) {
            $return = array('status' => false, 'msg' => self::ERRO_SEM_PERMISAO, 'type' => 'success');
        } else {
            if ($fatorAvaliado->salvar($this->conid)) {
                $return = array('status' => true, 'msg' => self::DADOS_SALVO_COM_SUCESSO);
            } else {
                $return = array('status' => false, 'msg' => self::ERRO_AO_SALVAR, 'result' => $fatorAvaliado->error);
            }
        }

        echo simec_json_encode($return);
    }

    public function setPessoa() {
        if ($this->view->fatorAvaliado->getNomesEtapaControle(Model_PerfilUsuario::EXECUTOR)) {
            $this->view->executor = ' -- <b>Selecionado:</b> ' . $this->view->fatorAvaliado->getNomesEtapaControle(Model_PerfilUsuario::EXECUTOR);
        } else {
            $this->view->executor = false;
        }

        if ($this->view->fatorAvaliado->getNomesEtapaControle(Model_PerfilUsuario::VALIDADOR)) {
            $this->view->validador = ' -- <b>Selecionado:</b> ' . $this->view->fatorAvaliado->getNomesEtapaControle(Model_PerfilUsuario::VALIDADOR);
        } else {
            $this->view->validador = false;
        }

        if ($this->view->fatorAvaliado->getNomesEtapaControle(Model_PerfilUsuario::CERTIFICADOR)) {
            $this->view->certificador = ' -- <b>Selecionado:</b> ' . $this->view->fatorAvaliado->getNomesEtapaControle(Model_PerfilUsuario::CERTIFICADOR);
        } else {
            $this->view->certificador = false;
        }
    }

}
