<?php

//require(APPRAIZ . 'includes/library/simec/xls.class.php');

/**
 * Controle inicial
 *
 * @author Equipe simec - Consultores OEI - Junio Santos
 * @since  07/06/2014
 *
 * @name       Controller_Default
 * @package    classes
 * @subpackage controllers
 */
class Controller_Default extends Abstract_Controller
{

    const MSG_NIVEL_MAIOR_7 = 'Não é possivel inserir um item com nível maior que 7!';
    const DADOS_ALTERADOS_COM_SUCESSO = 'Dados alterados com sucesso!';
    const DADOS_ORDENADOS_COM_SUCESSO = 'Dados ordenados com sucesso!';
    const ERRO_AO_ALTERAR = 'Erro ao alterar dado. <br> consulte o administrador do sistema!';
    const ERRO_AO_ORDENAR = 'Erro ao ordenar os dados. <br> consulte o administrador do sistema!';
    const ERRO_AO_EXCLUIR_FILHOS = 'Não é possível excluir. <br> Este registro possui vinculos!';
    const ERRO_AO_EXCLUIR = 'Erro ao excluir dados. <br> consulte o administrador do sistema!';
    const DADOS_EXCLUIDOS_COM_SUCESSO = 'Dados excluídos com sucesso!';
    const DADOS_SALVO_COM_SUCESSO = 'Dados salvo com sucesso!';
    const ERRO_AO_SALVAR = 'Erro ao salvar os dados. <br> consulte o administrador do sistema!';
    const ERRO_AO_MOVER = 'Erro ao mover os dados. <br> Este registro possui vinculos!';

    public function __construct()
    {
		parent::__construct();
        $this->view->perfilUsuario = new Model_PerfilUsuario();
        $this->view->perfilUsuario->validaAcessoTelaContrato();

        $this->view->titulo = 'Cadastro de Contrato';
    }

    public function indexAction()
    {
        $this->view->modelHierarquiacontrato = new Model_Hierarquiacontrato(false);
        $this->render(__CLASS__, __FUNCTION__);
    }

    public function listarAction()
    {
		$hqcidpai_interagido = $_POST['hqcidpai_interagido'];
        $this->view->modelHierarquiacontrato = new Model_Hierarquiacontrato();
		if(!empty($hqcidpai_interagido)){
			$dados = $this->view->modelHierarquiacontrato->getNos(false, " AND (q.h).hqcid = $hqcidpai_interagido ");
			$this->view->hqcidpai_interagido = $hqcidpai_interagido;
			$path = '';
			if($dados){
				$path = str_replace('{', '', str_replace('}', '', $dados[0]['breadcrumb_hqcid']));
			}
			$this->view->path_hqcidpai = $path;
		}

        $this->render(__CLASS__, __FUNCTION__);
    }

    public function adicionarAction()
    {
        $this->view->contrato = new Model_Contrato();
        $this->view->hierarquiaContrato = new Model_Hierarquiacontrato();

        $id = (int)$_POST['id'];
        $contratos = $this->view->contrato->getContratoById($id);
        $this->view->hierarquiaContrato->setDataInsert($contratos);
        $this->view->noContrato = empty($id);

        if (!empty($id)) {
            $this->view->titulo = 'Cadastro de ' . $this->view->hierarquiaContrato->getNivelDescricao($contratos['hqcnivel'] + 1);
        }

        $this->render(__CLASS__, __FUNCTION__);
        exit;
    }

    public function editarAction()
    {
        $this->view->contrato = new Model_Contrato();
        $this->view->hierarquiaContrato = new Model_Hierarquiacontrato();
        $contrato = $this->view->contrato->getContratoById((int)$_POST['id']);

        $this->view->titulo = "Editar de Contrato / {$contrato['consigla']} - {$contrato['condescricao']}";

        $this->view->contrato->populateEntity($contrato);
        $this->view->contrato->treatEntityToUser();
        $this->view->hierarquiaContrato->populateEntity($contrato);
        $this->view->noContrato = empty($contrato['hqcidpai']);
        $this->render(__CLASS__, __FUNCTION__);
        exit;
    }

    public function excluirAction()
    {
        $this->view->contrato = new Model_Contrato(false);
        $this->view->hierarquiaContrato = new Model_Hierarquiacontrato(false);
        $conid = (int)$this->getPost('id');
        $contrato = $this->view->contrato->getContratoById($conid);


        if ($this->view->perfilUsuario->validarAcessoModificacao($conid) === false) {
            $return = array('status' => true, 'msg' => self::ERRO_SEM_PERMISAO, 'result' => '', 'type' => 'warning');
        } elseif ($this->view->hierarquiaContrato->verificaVinculos($contrato['hqcid'])) {
            $return = array('status' => true, 'msg' => self::ERRO_AO_EXCLUIR_FILHOS, 'result' => '', 'type' => 'warning');
        } else {
            try {
                $hqcidpai = $contrato['hqcidpai'];
                if (empty($hqcidpai) OR strtolower($hqcidpai) == 'null') {
                    $usuarioResponsabilidade = new Model_UsuarioResponsabilidade();
                    $usuarioResponsabilidade->deleteAllByValues(array('conid' => $conid));
                }

                $this->view->contrato->populateEntity(array('conid' => (int) $conid ));
                $this->view->contrato->setAttributeValue('condescricao', $this->view->contrato->getAttributeValue('condescricao'));
                $this->view->contrato->setAttributeValue('concontratada', $this->view->contrato->getAttributeValue('concontratada'));
                $this->view->contrato->setAttributeValue('conprocesso', $this->view->contrato->getAttributeValue('conprocesso'));
                $this->view->contrato->setAttributeValue('conobjetivo', $this->view->contrato->getAttributeValue('conobjetivo'));
                $this->view->contrato->setAttributeValue('constatus', 'I');
                $this->view->contrato->update();

                $hqcid =   (int)str_replace('\'', '', $this->view->contrato->getAttributeValue('hqcid') );
                $this->view->hierarquiaContrato->populateEntity(array('hqcid' => $hqcid));
                $this->view->hierarquiaContrato->setAttributeValue('hqcstatus', 'I');
                $this->view->hierarquiaContrato->update();

                $this->view->contrato->commit();
                $this->view->hierarquiaContrato->commit();
                $return = array('status' => false, 'msg' => self::DADOS_EXCLUIDOS_COM_SUCESSO, 'result' => '', 'type' => 'success');
            } catch (Exception $exc) {
                if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
                    echo $exc->getTraceAsString();
                }
                $return = array('status' => true, 'msg' => self::ERRO_AO_EXCLUIR, 'result' => '', 'type' => 'danger');
                $this->view->contrato->rollback();
                $this->view->hierarquiaContrato->rollback();
            }
        }
        $return['msg'] = '<div class="alert alert-' . $return['type'] . '" role="alert">' . $return['msg'] . '</div>';
        echo simec_json_encode($return);
    }

    public function ordenarAction()
    {
        $this->view->hierarquiaContrato = new Model_Hierarquiacontrato();
        $contrato = new Model_Contrato();
        $novaOrdem = $_POST['novaOrdem'];
        if (is_array($novaOrdem) && !empty($novaOrdem)) {
            try {
                foreach ($novaOrdem as $ordem => $id) {
                    $hierarquiaContrato = new Model_Hierarquiacontrato();
                    $dado = $hierarquiaContrato->getHierarquiaContratoById((int)$id);
                    $hierarquiaContrato->populateEntity($dado);
                    $hierarquiaContrato->entity['hqcordem']['value'] = $ordem + 1;
                    $hierarquiaContrato->save();
                }
                $return = array('status' => 1, 'msg' => self::DADOS_ORDENADOS_COM_SUCESSO);
            } catch (Exception $exc) {
                $return = array('status' => 0, 'msg' => self::ERRO_AO_ORDENAR);
            }
        } elseif ($contrato->possuiFatorAvaliado( (int)$this->getPost('hqcid') )) {
			$return = array('status' => 0, 'msg' => self::ERRO_AO_MOVER);
        }else {
            try {

                if ($this->view->hierarquiaContrato->alterarNiveisNosFilhos($_POST['hqcid'], $_POST['hqcnivel'])) {
                    $this->view->hierarquiaContrato->populateEntity($_POST);
                    $this->view->hqcidpai_interagido = $_POST['hqcidpai'];
                    $this->view->hierarquiaContrato->save();
					$return = array('status' => 1, 'msg' => self::DADOS_ORDENADOS_COM_SUCESSO);
                } else {
					$return = array('status' => 0, 'msg' => self::MSG_NIVEL_MAIOR_7);
                }
            } catch (Exception $exc) {
				$return = array('status' => 0, 'msg' => self::ERRO_AO_ALTERAR);
            }
        }
		echo simec_json_encode($return);
		exit;
    }

    public function salvarAction()
    {
        $modelContrato = new Model_Contrato(false);
        $modelHierarquiacontrato = new Model_Hierarquiacontrato(false);
        $conid = $this->getPost('conid');
        $modelContrato->populateEntity($_POST);
        $modelHierarquiacontrato->populateEntity($_POST);

        if ($this->view->perfilUsuario->validarAcessoModificacao($conid) === false and !empty($conid)) {
            $return = array('status' => false, 'msg' => self::ERRO_SEM_PERMISAO);
        } else {
            if ($modelHierarquiacontrato->getAttributeValue('hqcidpai') && empty($modelHierarquiacontrato->entity['hqcordem']['value'])) {
                $modelHierarquiacontrato->entity['hqcordem']['value'] = $modelHierarquiacontrato->countItemFilhos($modelHierarquiacontrato->getAttributeValue('hqcidpai'));
            }

            if ((int)$modelHierarquiacontrato->getAttributeValue('hqcnivel') > 7) {
                $return = array('status' => false, 'msg' => self::MSG_NIVEL_MAIOR_7, 'result' => array());
            } else {

                $idHierarquia = $modelHierarquiacontrato->save();
                $idContrato = $modelContrato->salvar($idHierarquia);

                $hqcidpai = $modelHierarquiacontrato->getAttributeValue('hqcidpai');
                if (empty($hqcidpai) OR strtolower($hqcidpai) == 'null') {
                    $usuarioResponsabilidade = new Model_UsuarioResponsabilidade();
                    $usuarioResponsabilidade->salvarUsuarioRespContrato($idContrato);
                }

                if ($idContrato === false OR $idHierarquia === false) {
                    $erros = array();
                    if ($idHierarquia === false) {
                        $erros = $modelHierarquiacontrato->error;
                    }
                    if ($idContrato === false) {
                        $erros = array_merge($modelContrato->error, $erros);
                    }
                    $modelHierarquiacontrato->rollback();
                    $modelContrato->rollback();
                    $return = array('status' => false, 'msg' => self::ERRO_AO_SALVAR, 'result' => $erros);
                } else {
                    $modelHierarquiacontrato->commit();
                    $modelContrato->commit();
                    $return = array('status' => true, 'hqcidpai_interagido' => $_POST['hqcidpai'], 'msg' => self::DADOS_SALVO_COM_SUCESSO);
                }
            }
        }
        echo simec_json_encode($return);
    }

	public function visalizarItemAction()
	{
		$hqcid = (int)$_POST['hqcid'];
		$level = (int)$_POST['level'];

		if(!empty($hqcid) && !empty($level) ){
			$modelHierarquiacontrato = new Model_Hierarquiacontrato(false);
			$script = ' <script language="javascript" src="/contratogestao/js/contrato_gestao.js"></script> ';
			$htmlArvore =  $modelHierarquiacontrato->getArvore(" AND (q.h).hqcidpai = {$hqcid} AND level = {$level} " );
			if(!empty($htmlArvore)){
				echo $script.$htmlArvore;
			}
		}
		exit;


	}
}
