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
class Controller_Usuario extends Abstract_Controller
{

	public function __construct()
	{
		parent::__construct();
	}

	public function usuarioSistemaAction()
	{
		$this->view->usuario = new Model_Usuario();
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function salvarUsuarioAction()
	{
		$mudouSenha = 0;
		$usuario = new Model_Usuario(false);
		$usuarioSistema = new Model_UsuarioSistema(false);
		$perfilUsuario = new Model_PerfilUsuario(false);

		$etapa = $this->getPost('etapa');

		$cpf = $usuario->removeMaskCpf($this->getPost('usucpf'));
		$_POST['usucpf'] = $cpf;

		$usuario->populateEntity($_POST);

		if (empty($usuario->entity['ususenha']['value'])) {
			$mudouSenha = 1;
		}

		try {
			/** SALVAR USUARIO */
			if ($usuario->salvar($cpf, $etapa)) {

				$usuarioSistema->setUsuarioSistema($cpf, ID_SISTEMA);
				/** SALVAR USUARIO SISTEMA */
				if ($usuarioSistema->salvar($cpf)) {

					$perfilUsuario->setPerfilUsuario($cpf);
					/** SALVAR PEFIL USUARIO */
					if ($perfilUsuario->salvar($cpf)) {

						$usuario->commit();
						$usuarioSistema->commit();
						$perfilUsuario->commit();
						$return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO), 'novoUsuario' => $mudouSenha, 'cpf' => str_replace("'", "", $usuario->getAttributeValue('usucpf')), 'nome' => str_replace("'", "", $usuario->getAttributeValue('usunome')),);
					} else {
						$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $perfilUsuario->error);
					}
				} else {
					$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $usuarioSistema->error);
				}
			} else {
				$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $usuario->error);
			}
		} catch (Exception $exc) {
			if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
				echo $exc->getTraceAsString();
			}
			$usuario->rollback();
			$usuarioSistema->rollback();
			$perfilUsuario->rollback();
			$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR));
		}
		echo simec_json_encode($return);
	}

	public function getUsuarioCpfAction()
	{
		$usuario = new Model_Usuario();
		$cpf = $usuario->removeMaskCpf($this->getPost('cpf'));
		$dados = $usuario->getByValues(array('usucpf' => $cpf));

		$usuario->populateEntity($dados);
		$usuario->treatEntityToUser();
		$retorno = $usuario->getDadosUsuarioFatorAvaliado($dados);

		if ($retorno) {
			echo simec_json_encode($retorno);
		} else {
			$retorno = $usuario->getDadosUsuarioFatorAvaliadoReceitaFederal($cpf);
			$retorno ['novoUsuario'] = 1;
			echo simec_json_encode($retorno);
		}
	}

	public function getMunicipiosAction()
	{
		$usuario = new Model_Usuario();
		$usuario->populateEntity($_POST);
		echo $usuario->getComboMunicipios($this->getPost('regcod'));
	}

	public function listarAction()
	{
		$params = array();
		parse_str($_POST['parans'], $params);
		$this->view->parans = $params;
		$this->view->usuario = new Model_Usuario();
		$this->render(__CLASS__, __FUNCTION__);
	}

	public function vincularUsuarioAction()
	{

		$usuarioSistema = new Model_UsuarioSistema(false);
		$perfilUsuario = new Model_PerfilUsuario(false);
		$cpf = $this->getPost('usuid');

		$dadosUsuario = $usuarioSistema->getUsuarioSistema($cpf, ID_SISTEMA);
		if (is_array($dadosUsuario) and count($dadosUsuario) > 0) {
			$return = array('status' => false, 'msg' => ('Usuário já vinculado ao sistema!'), 'result' => $perfilUsuario->error);
		} else {

			try {

				$usuarioSistema->setUsuarioSistema($cpf, ID_SISTEMA);
				/** SALVAR USUARIO SISTEMA */
				if ($usuarioSistema->salvar($cpf)) {

					$perfilUsuario->setPerfilUsuario($cpf, SOLUCAO_PERFIL_CONSULTA);
					/** SALVAR PEFIL USUARIO */
					if ($perfilUsuario->salvar($cpf)) {

						$usuarioSistema->commit();
						$perfilUsuario->commit();

						$return = array('status' => true, 'msg' => (self::DADOS_SALVO_COM_SUCESSO));
					} else {
						$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $perfilUsuario->error);
					}
				} else {
					$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR), 'result' => $usuarioSistema->error);
				}
			} catch (Exception $exc) {
				if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
					echo $exc->getTraceAsString();
				}
				$usuarioSistema->rollback();
				$perfilUsuario->rollback();
				$return = array('status' => false, 'msg' => (self::ERRO_AO_SALVAR));
			}
		}
		echo simec_json_encode($return);
	}

	public function getOrgaosAction()
	{
		$usuario = new Model_Usuario();
		$usuario->populateEntity($_POST);
		echo $usuario->getComboOrgaos($this->getPost('tpocod'), $this->getPost('regcod'), $this->getPost('muncod'));
	}

}
