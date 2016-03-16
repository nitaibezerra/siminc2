<?php
class Usuario {

	private $id;
	private $nome;
	private $perfis;
	
	public function setId( $id ){
		$this->id = $id;
	}
	
	public function getId(){
		return $this->id;
	}
	
	public function setNome( $nome ){
		$this->nome;
	}
	
	public function getNome(){
		return $this->nome;
	}
	
	public function setPerfis( array $perfis ){
		$this->perfis = $perfis;
	}
	
	public function getPerfis(){
		return $this->perfis;
	}
	
	public function isSuperUsuario(){
		$superUsuario = false;
		
		foreach ($this->perfis as $perfil) {
			$superUsuario = $perfil->getDescricao() == "Super Usuário" ? $superUsuario || true : $superUsuario;
		}
		
		return $superUsuario;
	}
}