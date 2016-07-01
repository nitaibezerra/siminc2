<?php

/**
 * Classe Modelo Geral CPROC
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto de Modelo Geral CPROC
 */
class ModeloCproc extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela;

	/**
	 * Chave primaria - a ser determinado na classe filha
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria;

	/**
	 * Status - a ser determinado na classe filha
	 * @name $status
	 * @var array
	 * @access protected
	 */
	protected $status;
	
	/**
	 * Atributos da Tabela - a ser determinado na classe filha
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos;

	/**
	 * Campos da Tabela - a ser determinado na classe filha
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos;

	/**
	 * Atributos da Tabela obrigatórios - a ser determinado na classe filha
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios;

	/**
	 * Atributos Integer da Tabela - a ser determinado na classe filha
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosInt;

	/**
	 * Array de lista de assessoramentos
	 * 
	 * @var array
	 */
	public $lista;

	/**
	 * Id do último assessorado inserido
	 * 
	 * @var integer
	 */
	public $inserido;

	/**
	 * Busca lista de dados baseado em filtro ou não
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 */
	public function busca( $filtros = false ){
		$sql = $this->listaQuery( $filtros );
		
		$this->lista = $this->carregar( $sql );

		return $this;
	}

	/**
	 * Monta query para uma lista com todos os dados
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 */
	public function listaQuery( $filtros = false ){
		$sql = "
			SELECT * FROM " . $this->stNomeTabela;
		if( $filtros != false ){
			$sql .= " WHERE ";
			foreach ($filtros as $chave => $valor)
				$sql .= " " . $chave . " = " . ((!in_array($chave, $this->arAtributosInt))?"'".$valor."'":$valor) . " ";
		}

		return $sql;
	}

	/**
	 * Insert
	 * 
	 * @param array $dados
	 * @author Sávio Resende
	 * @return $this
	 */
	public function cadastrar( Array $dados ){

		// if( !$this->validaCamposObrigatorios() ) return false;
		
		$campos = $this->arCampos;
		$this->popularObjeto( $campos, $dados );
		
		$this->arAtributos[ $this->arChavePrimaria[0] ] = $this->inserir();
		$this->inserido = $this->arAtributos[ $this->arChavePrimaria[0] ];
		return $this->commit();

	}

	/**
	 * Exclui Logicamente
	 *
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirLogicamente(){
		$this->carregarPorId( $this->arAtributos[ "{$this->arChavePrimaria[0]}" ] );
		$this->arAtributos[ $this->status ] = 'I';
		$this->alterar();
		return $this->commit();
	}

	/**
	 * Carrega Objeto por ID
	 *
	 * @param Integer $assid
	 * @author Sávio Resende
	 * @return str|bool retorna string '1' para wf já existente ou bool para resultado de atualziacao para o wf recém criado.
	 */
	public function carregar( $chave ){
		$this->carregarPorId( $chave );

		return $this;
	}

	/**
	 * Valida campos obrigatorios no objeto populado
	 *
	 * @author Sávio Resende
	 * @return bool
	 */
	public function validaCamposObrigatorios(){

		foreach ($this->arAtributosObrigatorios as $chave => $valor){
			if( !isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]) )
				return false;
		}

		return true;
	}

	/**
	 * Atualiza
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizar(){
		
		if( $this->validaCamposObrigatorios() ){
			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
	}

	/**
	 * Popula Objeto com Array
	 *
	 * @param array $arDados
	 * @return $this
	 * @author Sávio Resende
	 */
	public function popula( Array $arDados ){
		$this->popularObjeto( $this->arCampos, $arDados );
		return $this;
	}
}