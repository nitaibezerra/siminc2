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
include_once APPRAIZ . 'includes/workflow.php';

class Controller_ExecucaoFatorAvaliado extends Abstract_Controller {

    public function __construct() {
        parent::__construct();

        $this->view->fatorAvaliado = new Model_FatoravaliadoExecucao();
        $this->view->documento = new Model_Documento();
        $this->view->comentarioDocumento = new Model_Comentariodocumento();
        $this->view->hierarquiaContrato = new Model_Hierarquiacontrato();
		$this->view->satisfacao = new Model_Satisfacao();

        $this->view->perfilUsuario = new Model_PerfilUsuario();
        $this->view->perfilUsuario->validaAcessoTelaContrato();
    }

    public function indexAction() {

        if ($_POST['fatid']) {
            $this->view->possuiErro = false;
            $this->view->validadorObrigatorio = false;

            $this->view->fatorAvaliado->setAttributeValue('fatid', (int) $this->getPost('fatid'));
            $this->view->fatorAvaliado->populateEntity();
            $docId = (int) $this->view->fatorAvaliado->getAttributeValue('docid');
            $this->getComentario();
            $this->setTitulos();
            $this->gravaArquivoNaExecucao();
            if (!$this->view->possuiErro) {
                try {
                    $acao = $this->view->fatorAvaliado->getAcao($this->getPost('acao'));
                    $acaoRecusado = $this->view->fatorAvaliado->getAcaoRecusado($this->getPost('acao'), $this->getPost('recusado_retorno'));

                    if ($acao) {

                        if ($this->getPost('acao') === 'validacao' OR $this->getPost('acao') === 'certificacao') {
                            $this->view->fatorAvaliado->setAttributeValue('cofid', (int) $this->getPost('cofid'));
                            $this->view->fatorAvaliado->setAttributeValue('temid', (int) $this->getPost('temid'));
                            $this->view->fatorAvaliado->setAttributeValue('satid', (int) $this->getPost('satid'));
                        }

                        $this->view->fatorAvaliado->setAttributeValue('fatprazo', date('d/m/Y', strtotime($this->view->fatorAvaliado->getAttributeValue('fatprazo'))));
                        $this->view->fatorAvaliado->setAttributeValue('fatdsc', $this->view->fatorAvaliado->getAttributeValue('fatdsc'));
                        $fatvalordesembolso = $this->view->fatorAvaliado->getAttributeValue('fatvalordesembolso');
                        $fatvalordesembolso = number_format($fatvalordesembolso, 2, ',', '.');
                        $this->view->fatorAvaliado->setAttributeValue('fatvalordesembolso', $fatvalordesembolso);


                        if ($this->view->fatorAvaliado->save()) {
                            $cmddsc = ( $this->getPost('cmddsc') ? $this->getPost('cmddsc') : 'enviado' );

                            if ($this->getPost('recusado') == 1 && $this->getPost('recusado_retorno')) {
                                $validaR = wf_alterarEstado($docId, $acaoRecusado, $cmddsc, array());
                            } else {
                                $validaA = wf_alterarEstado($docId, $acao, $cmddsc, array());
                            }

                            echo '<script>alert("Dados enviados com sucesso!");</script>';
                            $this->view->fatorAvaliado = new Model_FatoravaliadoExecucao();
                        } else {
                            var_dump($this->view->fatorAvaliado);
                            exit;
                        }
                    } else {
                        $this->view->validadorObrigatorio = 'Ainda nao e possivel executar este item pq nao existe um validador atribuido.';
                    }
                } catch (Exception $exc) {
                    if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
                        echo $exc->getTraceAsString();
                    }
                }
            }
        }

        $this->setDadosDaTabela();
        $this->view->listing = $this->view->fatorAvaliado->getListingExecucao();
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function formularioAction() {
        $this->view->fatorAvaliado->setAttributeValue('fatid', (int) $this->getPost('idFatorAvaliado'));
        $this->view->fatorAvaliado->populateEntity();
        $this->view->conformidade = new Model_Conformidade();
        $this->view->tempestividade = new Model_Tempestividade();

        $this->view->conformidade->setAttributeValue('cofid', $this->view->fatorAvaliado->getAttributeValue('cofid'));
        $this->view->tempestividade->setAttributeValue('temid', $this->view->fatorAvaliado->getAttributeValue('temid'));
        $this->view->satisfacao->setAttributeValue('satid', $this->view->fatorAvaliado->getAttributeValue('satid'));

        $this->setTitulos();
        $this->getComentario();
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function pesquisarAction() {
        $this->setDadosDaTabela( $this->getPost('conid') );
        $this->view->listing = $this->view->fatorAvaliado->getListingExecucao();
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function downloadAction() {
        $this->view->fatorAvaliado = new Model_FatoravaliadoExecucao();
        $this->view->fatorAvaliado->getArquivo((int) $_GET['arqid']);
    }

    public function getComentario() {
        $this->view->documento->setAttributeValue('docid', (int) $this->view->fatorAvaliado->getAttributeValue('docid'));
        $this->view->documento->populateEntity();
        $this->view->documento->treatEntityToUser();

        if ($this->view->documento->getAttributeValue('hstid')) {
            $comentarioDocumento = $this->view->comentarioDocumento->getByValues(array('docid' => (int) $this->view->documento->getAttributeValue('docid'), 'hstid' => (int) $this->view->documento->getAttributeValue('hstid')));
            $this->view->comentarioDocumento->setAttributeValue('cmddsc', $comentarioDocumento['cmddsc']);
        }
    }

    public function setDadosDaTabela($conid = false) {
        $this->view->fatorAvaliado = new Model_FatoravaliadoExecucao();
        if ($conid) {
            $this->view->fatorAvaliado->setAttributeValue('conid', (int)$conid );
            $this->view->fatorAvaliado->populateEntity();
        }
        $this->view->dataExecutor = $this->view->fatorAvaliado->getConsultaExecucao(Model_Fatoravaliado::ESTADO_EXECUTOR);
        $this->view->dataValidador = $this->view->fatorAvaliado->getConsultaExecucao(Model_Fatoravaliado::ESTADO_VALIDADOR);
        $this->view->dataCertificador = $this->view->fatorAvaliado->getConsultaExecucao(Model_Fatoravaliado::ESTADO_CERTIFICADOR);
    }

    public function setTitulos() {
        $contrato = new Model_Contrato();
        $contrato->setAttributeValue('conid', (int) $this->view->fatorAvaliado->getAttributeValue('conid'));
        $contrato->populateEntity();
        $this->view->titulo = ("{$contrato->getAttributeValue('consigla')} - {$contrato->getAttributeValue('condescricao')}");
        $this->view->fator = ($this->view->fatorAvaliado->getAttributeValue('fatdsc'));
    }

    public function gravaArquivoNaExecucao() {
        if ($this->getPost('acao') === 'execucao') {
            $arqid = $this->view->fatorAvaliado->getAttributeValue('arqid');
            if (is_uploaded_file($_FILES['arqid']['tmp_name'])) {
                $this->view->fatorAvaliado->setArqId();
            } else if (empty($arqid)) {
                echo '<script>alert("É necessário inserir um arquivo!");</script>';
                $this->view->possuiErro = true;
            }
        }
    }

}
