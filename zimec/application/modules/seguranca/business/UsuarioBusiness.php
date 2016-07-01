<?php
class Seguranca_Business_UsuarioBusiness
{
	const LIMITE_DE_TENTATIVAS = 4;
	const STATUS_ATIVO = A;
	const STATUS_PENDENTE = P;
	const STATUS_BLOQUEADO = B;
	
	private $modelUsuario;
	
	public function __construct()
	{
		$this->modelUsuario = new Model_Seguranca_Usuario();
	}
	
	public function alterarStatus($cpf, $status, $justificativa, $sistema = null)
	{
		$usucpfadm = $cpf != $_SESSION['usucpf'] ? "'" . $_SESSION['usucpf'] . "'" : 'null';
		
		if ($sistema) 
		{
			$usuarioSistema = array();
			
			$usuarioSistema['suscod'] = $status;
			$usuarioSistema['sisid'] = $sistema;
			$usuarioSistema['usucpf'] = $cpf;
			
			$this->usuarioSistemaModel->gravar($usuario);
			
			$usuarioHistorico = array();
			
			$usuarioHistorico['htudsc'] = $justificativa;
			$usuarioHistorico['usucpf'] = $cpf;
			$usuarioHistorico['sisid'] = $sistema;
			$usuarioHistorico['suscod'] = $status;
			$usuarioHistorico['usucpfadm'] = $usucpfadm;
			
			$this->historicoUsuario->gravar($usuarioHistorico);
		
			if ($status == 'A')
			{
				$usuario = $this->modelUsuario->getUsuarioByCPF($cpf);
				
				if ( $usuario->suscod != 'A' ) // verifica se status geral não é ativo
				{
					$usuarioSistema = array();
					
					$usuarioSistema['suscod'] = $status;
					$usuarioSistema['usucpf'] = $cpf;
					
					$this->usuarioSistemaModel->gravar($usuario);
					
					$usuarioHistorico = array();
					
					$usuarioHistorico['htudsc'] = $justificativa;
					$usuarioHistorico['usucpf'] = $cpf;
					$usuarioHistorico['suscod'] = $status;
					$usuarioHistorico['usucpfadm'] = $usucpfadm;
					
					$this->historicoUsuario->gravar($usuarioHistorico);
				}
			}
		} 
		else 
		{
			$usuarioSistema = array();
				
			$usuarioSistema['suscod'] = $status;
			$usuarioSistema['usucpf'] = $cpf;
				
			$this->usuarioSistemaModel->gravar($usuario);
				
			$usuarioHistorico = array();
				
			$usuarioHistorico['htudsc'] = $justificativa;
			$usuarioHistorico['usucpf'] = $cpf;
			$usuarioHistorico['suscod'] = $status;
			$usuarioHistorico['usucpfadm'] = $usucpfadm;
				
			$this->historicoUsuario->gravar($usuarioHistorico);
				
			if ($status != 'A')
			{
				$usuarioSistema = array();
				
				$usuarioSistema['suscod'] = $status;
				$usuarioSistema['usucpf'] = $cpf;
				
				$this->usuarioSistemaModel->gravar($usuario);
				
				$sistemas = $this->modelUsuario->getSistemasByCPF($cpf);
				
				foreach ($sistemas as $sistema)
				{
					if ($sistema['suscod'] == $status) continue;
					
					$usuarioHistorico = array();
					
					$usuarioHistorico['htudsc'] = $justificativa;
					$usuarioHistorico['usucpf'] = $cpf;
					$usuarioHistorico['sisid'] = $sistema['sisid'];
					$usuarioHistorico['suscod'] = $status;
					$usuarioHistorico['usucpfadm'] = $usucpfadm;
					
					$this->historicoUsuario->gravar($usuarioHistorico);
				}
			}
		}
	}
}