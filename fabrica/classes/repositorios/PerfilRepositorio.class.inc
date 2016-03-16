<?php
class PerfilRepositorio extends Modelo {
	
	public function __construct(){
		parent::__construct();
	}
	
	public function recuperePorIdUsuarioIdSistema( $idUsuario, $idSistema ){
		$sql = "SELECT p.pflcod, p.pfldsc
				FROM seguranca.usuario u
				JOIN seguranca.perfilusuario pu
					on u.usucpf = pu.usucpf
				JOIN seguranca.perfil p
					on p.pflcod = pu.pflcod
				WHERE u.usucpf = '$idUsuario' AND p.sisid = $idSistema";
		$resultSet = parent::carregar($sql);
		if ($resultSet != null){
			foreach ($resultSet as $linha){
				$lista[] = $this->montaObjeto($linha);
			}
		}
		return $lista;
	}
	
	private function montaObjeto( $linha ){
		$perfil = new Perfil();
		$perfil->setId($linha['pflcod']);
		$perfil->setDescricao($linha['pfldsc']);
		return $perfil;
	}
}