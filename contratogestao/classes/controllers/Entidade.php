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
class Controller_Entidade extends Abstract_Controller {

    public function __construct() {
        parent::__construct();
    }

    public function salvarEntidadeFatorAvaliadoAction() {
        $entidade = new Model_Entidade(false);
        $entidadeFuncao = new Model_FuncaoEntidade(false);
        $etapa = $this->getPost('etapa');
        $cnpj = $entidade->removeMaskCnpj($this->getPost('entnumcpfcnpj'));
        $_POST['entnumcpfcnpj'] = $cnpj;


        $dados = $entidade->getByValues(array('entnumcpfcnpj' => $cnpj));
        if ($dados) {
            $entidade->populateEntity(array('entid' => $dados['entid']));
            $entidade->populateEntity($_POST);
        } else {
            $entidade->populateEntity($_POST);
        }
        $entidade->treatEntityToUser();
        
        try {
            /** SALVAR ENTIDADE */
            $idEntidade = $entidade->save();
            if ($idEntidade) {

                $funcao = $entidadeFuncao->getEntidadeFuncaoContratoGestao($etapa);
                $dadosFuncao = $entidadeFuncao->getByValues(array('funid' => $funcao, 'entid' => $idEntidade));

                if ($dadosFuncao) {
                    $entidadeFuncao->populateEntity(array('fueid' => $dadosFuncao['fueid']));
                } else {
                    $entidadeFuncao->setAttributeValue('fuedata', date('d/m/Y H:i:s'));
                    $entidadeFuncao->setAttributeValue('funid', $funcao);
                    $entidadeFuncao->setAttributeValue('entid', $idEntidade);
                    $entidadeFuncao->setAttributeValue('fuestatus', 'A');
                }

                /** SALVAR ENTIDADE FUNCAO */
                if ($entidadeFuncao->save()) {

                    $entidade->commit();
                    $entidadeFuncao->commit();

                    $return = array(
                        'etapa' => $etapa,
                        'status' => true,
                        'msg' => self::DADOS_SALVO_COM_SUCESSO,
                        'novoEntidade' => ($entidade->getAttributeValue('entid') ? 0 : 1),
                        'entid' => str_replace("'", "", $entidade->getAttributeValue('entid')),
                        'nome' => str_replace("'", "", $entidade->getAttributeValue('entnome')),
                    );
                } else {
                    $return = array('status' => false, 'msg' => self::ERRO_AO_SALVAR, 'result' => $entidadeFuncao->error);
                }
            } else {
                $return = array('status' => false, 'msg' => self::ERRO_AO_SALVAR, 'result' => $entidade->error);
            }
        } catch (Exception $exc) {
            if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
                echo $exc->getTraceAsString();
            }
            $entidade->rollback();
            $entidadeFuncao->rollback();
            echo $exc->getTraceAsString();
        }

        echo simec_json_encode($return);
    }

    public function getEntidadeCnpjAction() {
        $entidade = new Model_Entidade();
        $cnpj = $entidade->removeMaskCnpj($this->getPost('entnumcpfcnpj'));
        $dados = $entidade->getByValues(array('entnumcpfcnpj' => $cnpj));
        $retorno = array();

        if (!empty($cnpj)) {
            if ($dados) {
                $entidade->populateEntity($dados);
                $entidade->treatEntityToUser();
                $retorno = $entidade->getDadosEntidadeFatorAvaliado();
            } else {
                $entidadeRetorno = $entidade->getDadosEntidadeFatorAvaliadoReceitaFederal($cnpj);

                if ($entidadeRetorno) {
                    $retorno = $entidadeRetorno->getDadosEntidadeFatorAvaliado();
                    $retorno ['novoEntidade'] = 1;
                }
            }
        }
        echo simec_json_encode($retorno);
    }

}
