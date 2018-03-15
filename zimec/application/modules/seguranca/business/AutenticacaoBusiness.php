<?php
import('seguranca.business.UsuarioBusiness');

class Seguranca_Business_AutenticacaoBusiness
{
	const LIMITE_DE_TENTATIVAS = 4;
	const STATUS_ATIVO = A;
	const STATUS_PENDENTE = P;
	const STATUS_BLOQUEADO = B;
	
	public static $erros = array
	(
		'cpf' => "O cpf informado não está cadastrado.",
		'status_pendente' => "Sua conta está pendente para aprovação, aguarde a avaliação dos administradores.",
		'status_bloqueado' => "Sua conta está bloqueada, clique <a href='solicitar_ativacao_de_conta.php' title='solicitar ativação de conta'>aqui</a> para solicitar a ativação.",
		'senha' => "A senha informada não é válida. Tentativas restantes: %s",
		'tentativas' => "Você excedeu a quantidade de tentativas.",
		'permissao' => "Você não possui permissão de acesso em nenhum dos módulos.",
	);
	
	private $businessUsuario;
	
	private $modelUsuario;
	
	private $modelMenu;
	
	public function __construct()
	{
		$this->modelMenu = new Model_Seguranca_Menu();
		$this->modelUsuario = new Model_Seguranca_Usuario();
		$this->businessUsuario = new Seguranca_Business_UsuarioBusiness();
		$this->businessUsuario = new Seguranca_Business_UsuarioBusiness();
	}
	
	public function bloquearUsuario($cpf) 
	{
		$usuario = array();
		$usuario['usucpf'] = $cpf;
		$usuario['usutentativas'] = '0';
		
		$this->modelUsuario->gravar($usuario);
		
		$this->businessUsuario->alterarStatus($cpf, STATUS_BLOQUEADO, "Usuário bloqueado por exceder a quantidade de tentativas de login com senha inválida.");

		throw new Exception(self::$erros['status_bloqueado']);
	}
	
	public function recuperarPermissoes($cpf) 
	{
		try {
			return $this->modelUsuario->getPermissoes($cpf);
		} catch (Exception $e) {
			throw new Exception($e->getMessage(), $e->getCode());
		}
	}
	
	public function recuperarUltimoAcesso($cpf)
	{
		try {
			return $this->modelUsuario->getUltimoAcesso($cpf);
		} catch (Exception $e) {
			throw new Exception($e->getMessage(), $e->getCode());
		}
	}
	
	public function recuperarMenu($cpf, $sisid = null)
	{
		try {
			$result = array();
			$submenus1 = array();
			$submenus2 = array();
			$menus = $this->modelMenu->getMenu($cpf, $sisid);
			
			foreach ($menus as $menu) {
				$sisid = $menu['sisid'];
				$mnuid = $menu['mnuid'];
				$mnuidpai = $menu['mnuidpai'];
					
				if ($mnuid && $sisid) {
					if ($menu['mnutipo'] == '1') {
						$result[$sisid]['menus'][$mnuid] = $menu->toArray();
					}
			
					if ($menu['mnutipo'] == 2 && !empty($mnuidpai)) {
						if (!isset($result[$sisid]['menus'][$mnuidpai]['submenus'])) {
							$result[$sisid]['menus'][$mnuidpai]['submenus'] = array();
						}
						$submenus1[$sisid][$mnuid] = array('mnuidpai' => $mnuidpai);
						$result[$sisid]['menus'][$mnuidpai]['submenus'][$mnuid] = $menu->toArray();
					}
			
					if ($menu['mnutipo'] == 3 && !empty($mnuidpai)) {
						$submenupai = $submenus1[$sisid][$mnuidpai]['mnuidpai'];
						if (!isset($result[$sisid]['menus'][$submenupai]['submenus'][$mnuidpai]['submenus'])) {
							$result[$sisid]['menus'][$submenupai]['submenus'][$mnuidpai]['submenus'] = array();
						}
						$submenus2[$sisid][$mnuid] = array('mnuidpai' => $mnuidpai);
						$result[$sisid]['menus'][$submenupai]['submenus'][$mnuidpai]['submenus'][$mnuid] = $menu->toArray();
					}
					if ($menu['mnutipo'] == 4 && !empty($mnuidpai)) {
						$submenupai = $submenus2[$sisid][$mnuidpai]['mnuidpai'];
						$submenuavo = $submenus1[$sisid][$submenupai]['mnuidpai'];
						if (!isset($result[$sisid]['menus'][$submenuavo]['submenus'][$submenupai]['submenus'][$mnuidpai]['submenus'])) {
							$result[$sisid]['menus'][$submenuavo]['submenus'][$submenupai]['submenus'][$mnuidpai]['submenus'] = array();
						}
						$result[$sisid]['menus'][$submenuavo]['submenus'][$submenupai]['submenus'][$mnuidpai]['submenus'][$mnuid] = $menu->toArray();
					}
				}
			}
			
			return $result;
		} catch (Exception $e) {
			throw new Exception($e->getMessage(), $e->getCode());
		}
	}
	
	public function atualizarTentativas($cpf, $tentativas) 
	{
		die('atualizarTentativas');
		
		$usuario = array();
		$usuario['usucpf'] = $cpf;
		$usuario['usutentativas'] = $tentativas;
		
		$this->modelUsuario->gravar($usuario);
		
		throw new Exception(sprintf(self::$erros['senha'], $alerta['tentativas']));
	}
	
	public function authenticate($cpf, $senha)
	{
		try {
			$cpf = preg_replace("/[^0-9]/", "", trim($cpf));
			$senha = trim($senha);
			$usuario = $this->modelUsuario->getUsuarioByCPF($cpf);
			
			if (!$usuario->usucpf) {
				throw new Exception(self::$erros[$usuario->usucpf]);
			}
			
			switch ($usuario->suscod) {
				case self::STATUS_ATIVO:
					break;
				case self::STATUS_PENDENTE:
					throw new Exception(self::$erros['status_pendente']);
				case self::STATUS_BLOQUEADO:
					throw new Exception(self::$erros['status_bloqueado']);
				default:
					throw new Exception(self::$erros['status_bloqueado']);
			}
			
			if ($this->decrypt($usuario->ususenha, '') != $senha)
			{
				$usuario->usutentativas += 1;
					
				$alerta['senha'] = sprintf( $alerta['senha'], LIMITE_DE_TENTATIVAS - $usuario->usutentativas + 1);
			
				if ($usuario->usutentativas > LIMITE_DE_TENTATIVAS)
				{
					$this->bloquearUsuario($usuario->usucpf);
				}
				else
				{
					$this->atualizarTentativas($usuario->usucpf, $usuario->usutentativas);
				}
			}
			
			if ($usuario->usutentativas > 0)
			{
				$usuario = array();
				$usuario['usucpf'] = $cpf;
				$usuario['usutentativas'] = '0';
			
				$this->modelUsuario->gravar($usuario);
			}
			
			unset( $usuario->ususenha );
			
			$auth = array();
			
			$sistema = $this->recuperarUltimoAcesso($usuario->usucpf);

			if (!$sistema->sisid) {
				throw new Exception(self::$erros['permissao']);
			}
			
			$_SESSION['sisid'] = $sistema->sisid;
			
			$auth['usucpf'] = $cpf;
			$auth['usunome'] = $usuario->usunome;
			$auth['usufuncao'] = $usuario->usufuncao;
			$auth['usudataultacesso'] = date('Y/m/d H:i:s');
			$auth['sisid'] = $sistema->sisid;
			$auth['sisdiretorio'] = $sistema->sisdiretorio;
			$auth['sisarquivo'] = $sistema->sisarquivo;
			$auth['sisantigo'] = FALSE; //!$sistema->siszend;
			$auth['paginainicial'] = $sistema->paginainicial;

			return $auth;
		} catch (Exception $e) {
			throw new Exception($e->getMessage(), $e->getCode());
		}

	}
	
	public function decrypt($enc_text, $password, $iv_len = 16)
	{
		$enc_text = base64_decode($enc_text);
		$n = strlen($enc_text);
		$i = $iv_len;
		$plain_text = '';
		$iv = substr($password ^ substr($enc_text, 0, $iv_len), 0, 512);
		
		while ($i < $n) {
			$block = substr($enc_text, $i, 16);
			$plain_text .= $block ^ pack('H*', md5($iv));
			$iv = substr($block . $iv, 0, 512) ^ $password;
			$i += 16;
		}
		
		return preg_replace('/\\x13\\x00*$/', '', $plain_text);
	}

}