<?php
class InventarioFuncionalidade {
	
	private $id;
	private $descricao;
	private $inventarioTipoFuncionalidade;
	private $inventarioAgrupadorFuncionalidade;
	private $qtdeArlRlr;
	private $qtdeTd;
	private $descricaoArlRlr;
	private $descricaoTd;
	private $complexidade;
	private $qtdePF;

	public function setId( $id ){
		$this->id = $id;
	}
	
	public function getId(){
		return $this->id;
	}
	
	public function setDescricao( $descricao ){
		$this->descricao = $descricao;
	}
	
	public function getDescricao(){
		return $this->descricao;
	}
	
	public function setInventarioTipoFuncionalidade( InventarioTipoFuncionalidade $inventarioTipoFuncionalidade ){
		$this->inventarioTipoFuncionalidade = $inventarioTipoFuncionalidade;
	}
	
	public function getInventarioTipoFuncionalidade(){
		return $this->inventarioTipoFuncionalidade;
	}
	
	public function setInventarioAgrupadorFuncionalidade( InventarioAgrupadorFuncionalidade $inventarioAgrupadorFuncionalidade ){
		$this->inventarioAgrupadorFuncionalidade = $inventarioAgrupadorFuncionalidade;
	}
	
	public function getInventarioAgrupadorFuncionalidade(){
		return $this->inventarioAgrupadorFuncionalidade;
	}
	
	public function setQtdeArlRlr( $qtdeArlRlr ){
		$this->qtdeArlRlr = $qtdeArlRlr;
	}
	
	public function getQtdeArlRlr(){
		return $this->qtdeArlRlr;
	}
	
	public function setQtdeTd( $qtdeTd ){
		$this->qtdeTd = $qtdeTd;
	}
	
	public function getQtdeTd(){
		return $this->qtdeTd;
	}
	
	public function setDescricaoArlRlr( $descricaoArlRlr ){
		$this->descricaoArlRlr = $descricaoArlRlr;
	}
	
	public function getDescricaoArlRlr(){
		return $this->descricaoArlRlr;
	}
	
	public function setDescricaoTd( $descricaoTd ){
		$this->descricaoTd = $descricaoTd;
	}
	
	public function getDescricaoTd(){
		return $this->descricaoTd;
	}
	
	public function setComplexidade( $complexidade ){
		$this->complexidade = $complexidade;
	}
	
	public function getComplexidade(){
		return $this->complexidade;
	}
	
	public function setQtdePF( $qtdePF ){
		$this->qtdePF = $qtdePF;
	}
	
	public function getQtdePF(){
		return $this->qtdePF;
	}
	
}