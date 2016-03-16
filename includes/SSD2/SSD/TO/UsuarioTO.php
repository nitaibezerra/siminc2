<?php
class UsuarioTO{
	
	private $idUsuarioSsd;
	private $dados;
	
	public static function loadUserInfo(
						$endereco, 
						$emailAlternativo, 
						$teste = null ,
						$cellPhoneNumber ,
						$cityAddress , 
						$cnpj ,
						$cpf ,
						$dispatcherAgency ,
						$email , 
						$login ,
						$lotacao , 
						$name , 
						$nationality , 
						$cityNaturalidade ,
						$nis, 
						$postalCode ,
						$responsibleCpf , 
						$responsibleName ,
						$rg ,
						$socialReason ,
						$ufUser ,
						$telephoneNumber ,
						$workInstitution ,
						$userId
	
	){
		$this->dados = array(
				'endereco' 			=> $endereco, 
				'email_alternativo'	=> $emailAlternativo, 
				'campoNull'			=> $teste = null ,
				'celular'			=> $cellPhoneNumber ,
				'cidade'			=> $cityAddress, 
				'cnpj'				=> $cnpj,
				'cpf'				=> $cpf,
				'orgao_expedidor'	=> $dispatcherAgency,
				'email'				=> $email, 
				'login'				=> $login,
				'lotacao'			=> $lotacao, 
				'nome'				=> $name, 
				'nacionalidade'		=> $nationality, 
				'cidadeOrigem'		=> $cityNaturalidade,
				'nis'				=> $nis, 
				'cep'				=> $postalCode,
				'cpfResponsavel'	=> $responsibleCpf, 
				'nomeResponsavel'	=> $responsibleName,
				'rg'				=> $rg,
				'razaoSocial'		=> $socialReason,
				'ufUsuario'			=> $ufUser,
				'telefone'			=> $telephoneNumber,
				'instituicao'		=> $workInstitution,
				'idUsuario'			=> $userId
		);
		
		$usuario = new UsuarioTO();
		$usuario->setCoUsuarioSSD($userId);
		return $usuario;
	}
	
	private function setCoUsuarioSSD($idUsuario){
		
		$this->idUsuarioSsd = $idUsuario;
	}
	
	public function getCoUsuarioSSD(){
		
		return $this->dados;
	}
}
?>