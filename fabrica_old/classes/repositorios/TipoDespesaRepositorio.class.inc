<?php

class TipoDespesaRepositorio extends Modelo {
	
	private $modelTipoDespesa;
	
	public function __construct()
	{
		parent::__construct();
		$this->modelTipoDespesa = new TipoDespesa();
	}

	/**
	 * Métodos de Negócio
	 */
	public function listarTodos()
	{
		$sql 	   = "SELECT ". $this->getCampos() ." FROM ". $this->modelTipoDespesa->getNomeTabela();
		$registros = $this->iteracaoRegistro( $sql );
		
		return $registros;
	}

	public function listarAtivos()
	{
		$sql 	   = "SELECT ". $this->getCampos() ." FROM ". $this->modelTipoDespesa->getNomeTabela() . " WHERE tpdpsstatus = 'A' ";
		$registros = $this->iteracaoRegistro( $sql );
	
		return $registros;
	}

	/**
	 * Métodos Auxiliares
	 */	
	private function montaObjeto( $linha )
	{
		$retorno = new TipoDespesa();
		$retorno->setId( $linha['tpdpsid'] ) ;
		$retorno->setDescricao( $linha['tpdpsdsc']);
		
		return $retorno;
	}
	
	private function iteracaoRegistro( $sql )
	{
		$lista 		= array();
		$resultSet  = parent::carregar( $sql );
	
		if ( $resultSet != null )
		{
			foreach ( $resultSet as $linha )
			{
				$lista[] = $this->montaObjeto( $linha );
			}
		}
		return $lista;
	}
	
	private function getCampos()
	{
		return implode( ',', array_keys( $this->modelTipoDespesa->getAtributos() ) );
	}	
	/*
	public function listarTodos()
	{
		$sql = "SELECT ". $this->getCampos() ." FROM ". $this->stNomeTabela;
		return parent::executar( $sql );
		
        $resultSet = parent::carregar( $sql );
        if ( $resultSet != null )
        {
            foreach ( $resultSet as $linha )
            {
                $lista[] = $this->montaObjeto( $linha );
            }
        }
        return $lista;
	}
	

	
	public function listarPorId( $id )
	{
		$sql = "SELECT ". $this->getCampos() ." FROM ". $this->stNomeTabela . " WHERE " . $this->arChavePrimaria = $id;
		return parent::carregar( $sql );
	}
	
	public function listarComFiltro( $where = null )
	{
		$sql       = "SELECT ". $this->getCampos() ." FROM ". $this->stNomeTabela . " WHERE " . $where;
		return parent::executar( $sql );
	}*/

}