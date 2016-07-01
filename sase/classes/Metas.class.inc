<?php

/**
 * Classe Metas
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 *
 * Objeto de Modelo de Metas
 * TODO classe não testada
 */
class Metas extends Modelo
{

	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "sase.meta";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('metid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array(
		'metid' => null,
		'mettitulo' => null,
		'metfontemunicipio' => null,
		'metfonteestado' => null,
		'metstatus' => null,
		'metchamada' => null
	);

	/**
	 * Atributos String da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosStr = array(
		'mettitulo'
	);

	/**
	 * Campos da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arCampos = array(
		'metid',
		'mettitulo',
		'metfontemunicipio',
		'metfonteestado',
		'metstatus'
	);

	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'mettitulo',
		'metfontemunicipio',
		'metfonteestado',
		'metstatus'
	);

	/**
	 * RetornaSQLListaGeralMetas
	 *
	 * @param array $filtros
	 * @author Eduardo Dunice
	 * @return $sql
	 */
	public function retornaSQLListaGeralMetas()
	{
		$sql = "SELECT * FROM sase.meta WHERE metstatus = 'A' ORDER BY metid";

		return $sql;
	}

	/**
	 * RetornaArrayListaGeral
	 *
	 * @param array $filtros
	 * @author Eduardo Dunice
	 * @return array $arrMetas
	 */
	public function retornaArrayListaGeral()
	{
		$sql = self::retornaSQLListaGeralMetas();

		return self::carregar($sql);
	}

	/**
	 * Monta query para a função monta_lista da classe_simec
	 *
	 * @param array $filtros
	 * @author Sávio Resende
	 * @return $sql
	 */
	public function montaListaQuery( $filtros = false ){
		$sql = "
			SELECT
				'<a style=\"cursor:pointer\" onclick=\"editarMetas( ' || metid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>
				<a style=\"cursor:pointer\" onclick=\"excluirMetas( ' || metid || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
				mettitulo,
				CASE WHEN metstatus = 'I' THEN 'Inativo' WHEN metstatus = 'A' THEN 'Ativo' END AS metstatus
			FROM " . $this->stNomeTabela . " ";
		if( $filtros != false ){
			$sql .= " WHERE 1=1 ";
			foreach ($filtros as $chave => $valor)
				$sql .= " AND " . $chave . ((in_array($chave, $this->arAtributosStr))?" LIKE '%".$valor."%'":" = '".$valor."'") . " ";
		}

		return $sql;
	}

	/**
	 * Atualiza Metas
	 *
	 * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
	 * @author Sávio Resende
	 */
	public function atualizarMetas(){
		if( $this->validaCamposObrigatorios() ){
			$this->alterar();
			return $this->commit();
		}

		return 'invalido';
	}

	/**
	 * Exclui logicamente o Metas
 	 *
	 * @param integer $stacod
	 * @return bool
	 * @author Sávio Resende
	 */
	public function excluirMetas( $metid ){
		$this->carregarPorId( $metid );
		$this->arAtributos['metstatus'] = 'I';
		$this->alterar();
		return $this->commit();
	}

	/**
	 * Popula Objeto com Array
	 *
	 * @param array $arDados
	 * @return $this
	 * @author Sávio Resende
	 */
	public function populaMetas( Array $arDados ){
		$this->popularObjeto( $this->arCampos, $arDados );
		return $this;
	}

	/**
	 * Valida campos obrigatorios no objeto populado
	 *
	 * @author Sávio Resende
	 * @return bool
	 */
	public function validaCamposObrigatorios(){
		foreach ($this->arAtributosObrigatorios as $chave => $valor)
			if( !isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]) )
				return false;

		return true;
	}

	/**
	 * Cadastra Metas Montado no Objeto
	 *
	 * @author Sávio Resende
	 * @return bool|string - retorna string 'invalido' para quando tiver campos obrigatorios vazios
	 */
	public function cadastrarMetas(){
		if( $this->validaCamposObrigatorios() ){
			$this->metid = $this->inserir();
			return $this->commit();
		}

		return 'invalido';
	}


	/**
	 * Retornar Array de Submetas dessa meta
	 *
	 * @author Eduardo Dunice
	 * @return bool|array
	 */
	public function retornarArraySubmetas($metid = null)
	{
	    $metid = $metid ? $metid : $this->metid;

		$sql = self::retornarSQLSubmetas($metid);

		$arrSubmetas = self::carregar($sql);

		if ($arrSubmetas[0]['subid'] != '') return $arrSubmetas;

		return false;
	}//end retornarArraySubmetas();


	/**
	 * Retornar SQL de Submetas dessa meta
	 *
	 * @author Eduardo Dunice
	 * @return string
	 */
	public function retornarSQLSubmetas($metid)
	{
		$sql = "SELECT *
                FROM sase.submeta
		        WHERE metid = $metid";

		return $sql;
	}//end retornarArraySubmetas();


}//end Class
?>