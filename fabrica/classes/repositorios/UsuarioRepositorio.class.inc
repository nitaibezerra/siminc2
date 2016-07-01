<?php
class UsuarioRepositorio extends Modelo {
	
	private $perfilRepositorio;
	 
	public function __construct(){
		parent::__construct();
		$this->perfilRepositorio = new PerfilRepositorio();
	}
	
	public function recuperePorId( $idUsuario ){
		$sql = "SELECT u.usucpf, u.usunome FROM seguranca.usuario u WHERE usucpf = '$idUsuario'";
		$linha = parent::pegaLinha($sql);
		return $this->montaObjeto($linha);
	}
	
	private function montaObjeto( $linha ){
		$usuario = new Usuario();
		$usuario->setId($linha['usucpf']);
		$usuario->setNome($linha['usunome']);
		
		$perfis = $this->perfilRepositorio->recuperePorIdUsuarioIdSistema($usuario->getId(), Sistema::FABRICA);
		
		$usuario->setPerfis($perfis);
		return $usuario;
	}
}